/// \file JEfficiency.cxx
/// \brief Class for unfolding and calculating efficiency
///
/// \author Joachim C. K. B. Hansen


#include "JEfficiency.hpp"
#include "JEUtility.hpp"

#include <RooUnfoldBayes.h>
#include <RooUnfoldSvd.h>
#include <TSVDUnfold.h>



ClassImp(JEfficiency);

JEfficiency::JEfficiency(TH1D* hData, THnSparse* hMCRespSparse, TH1D* hRecMatched, TH2F* hCentRhoRandomConewoLeadingJet, TH1* hGenUnmatched, TH1* hRecUnmatched) : 
hData{hData},
hMCRespSparse{hMCRespSparse},
hRecMatched{hRecMatched},
hCentRhoRandomConewoLeadingJet{hCentRhoRandomConewoLeadingJet},
hGenUnmatched{hGenUnmatched},
hRecUnmatched{hRecUnmatched},
hResponseFluctuations{nullptr},
hResponseDetector{nullptr},
hResponseTotal{nullptr},
hFluctuations{nullptr},
fPbPb{false}
{
}

JEfficiency::~JEfficiency() 
{
}

RooUnfoldResponse* JEfficiency::createResponse(bool fRebin)
{ 
  std::vector<double> binEdges = {0,1,2,3,4, 5,6,7,8,9, 10,12,14,16,18, 20, 25, 30, 40, 50};//, 55, 60, 65, 70, 75, 80, 90, 100, 120};//, 140, 160, 200};
  const int nBins = binEdges.size() - 1;

  if (!hResponseTotal) {
    std::cerr << "Response matrix not set" << std::endl;
    std::cout << "Set Response using .setRespSettings() method" << std::endl;
  }
  TH1D* mcp{dynamic_cast<TH1D*>(hResponseTotal->ProjectionY("hGen,MCP"))};
  TH1D* mcd{dynamic_cast<TH1D*>(hResponseTotal->ProjectionX("hReco,MCD"))};

  if (fRebin) {
    mcp = dynamic_cast<TH1D*>(mcp->Rebin(nBins, mcp->GetName(), binEdges.data()));
    mcd = dynamic_cast<TH1D*>(mcd->Rebin(nBins, mcd->GetName(), binEdges.data()));
  }

  if (!mcp || !mcd) {
    std::cerr << "Could not project response matrix" << std::endl;
  }
  RooUnfoldResponse* response = new RooUnfoldResponse(mcd, mcp);

  TH1D *fake = dynamic_cast<TH1D*>(hRecUnmatched->Clone("_Fake"));
  TH1D *miss = dynamic_cast<TH1D*>(hGenUnmatched->Clone("_Miss"));

  if (fRebin) {
    fake = dynamic_cast<TH1D*>(fake->Rebin(nBins, fake->GetName(), binEdges.data()));
    miss = dynamic_cast<TH1D*>(miss->Rebin(nBins, miss->GetName(), binEdges.data()));
  }

  fake->Add(mcd, -1);
  miss->Add(mcp, -1);

  for (auto i{1}; i <= hResponseTotal->GetNbinsX(); i++) {
    for (auto j{1}; j <= hResponseTotal->GetNbinsY(); j++) { 
      Double_t bincenx = hResponseTotal->GetXaxis()->GetBinCenter(i);
      Double_t binceny = hResponseTotal->GetYaxis()->GetBinCenter(j);
      Double_t bincont = hResponseTotal->GetBinContent(i, j);
      response->Fill(bincenx, binceny, bincont);
    }
  }
  for (auto i{1}; i <= miss->GetNbinsX(); i++) { 
    Double_t bincenx = miss->GetXaxis()->GetBinCenter(i);
    Double_t bincont = miss->GetBinContent(i);
    response->Miss(bincenx, bincont);
  }
  for (auto i{1}; i <= fake->GetNbinsX(); i++) {
    Double_t bincenx = fake->GetXaxis()->GetBinCenter(i);
    Double_t bincont = fake->GetBinContent(i);
    response->Fake(bincenx, bincont);
  }
  return response;
}


TH1D* JEfficiency::bayesUnfold(int iterations, bool fRebin)
{
  std::vector<double> binEdges = {0,1,2,3,4, 5,6,7,8,9, 10,12,14,16,18, 20, 25, 30, 40, 50};//, 55, 60, 65, 70, 75, 80, 90, 100, 120};//, 140, 160, 200};
  const int nBins = binEdges.size() - 1;
  auto clone = dynamic_cast<TH1D*>(hData->Clone("hData_cloneBayes"));
  if (fRebin)
    clone = dynamic_cast<TH1D*>(clone->Rebin(nBins, clone->GetName(), binEdges.data()));
  RooUnfoldResponse *response{this->createResponse(fRebin)};
  RooUnfoldBayes unfold(response, clone, iterations);
  TH1D* hUnfolded = dynamic_cast<TH1D*>(unfold.Hreco());
  if (!hUnfolded) {
    std::cerr << "Could not unfold" << std::endl;
  }
  return hUnfolded;
};


TH1D* JEfficiency::svdUnfold(int iterations)
{  
  std::vector<double> binEdges = {0,1,2,3,4, 5,6,7,8,9, 10,12,14,16,18, 20, 25, 30, 40, 50};//, 55, 60, 65, 70, 75, 80, 90, 100, 120};//, 140, 160, 200};
  const int nBins = binEdges.size() - 1;
  auto clone_hData = dynamic_cast<TH1D*>(hData->Rebin(nBins, "hData_cloneSvd", binEdges.data()));

  RooUnfoldResponse *response{this->createResponse(true)};
  // RooUnfoldSvd unfoldTMP(response, hData, 1);
  // unfoldTMP.Hreco();
  // TSVDUnfold *tsvdUnfold = (TSVDUnfold*)unfoldTMP.Impl();
  // int unfoldParam = GetSvdBestRegularisationParameter(tsvdUnfold);

  RooUnfoldSvd unfold(response, clone_hData, iterations); 

  TH1D* hUnfolded = dynamic_cast<TH1D*>(unfold.Hreco());
  return hUnfolded;
};

TH1D* JEfficiency::Efficiency()
{
  /* kinematic efficiency */
  /* N_reco-matched / N_generated */
  TH1D* out{dynamic_cast<TH1D*>(hRecMatched->Clone("_kinematic_efficiency"))};
  out->Divide(hGenUnmatched);
  return out;
};

TH1D* JEfficiency::Purity() {
  /* N_reco-match / N_reco-total */
  TH1D* out{dynamic_cast<TH1D*>(hRecMatched->Clone("_purity"))};
  out->Divide(hRecUnmatched);
  return out;
};

void JEfficiency::ptResponseFluctuations() { /* id: Get_PtResponseMatrix_Fluctuations */

  hResponseFluctuations = new TH2D("hResponseFluctuations_Fluct","",200,0,200,200,0,200);

  constexpr double GLOBAL_epsilon{0.0000000000000000001};

  if (fPbPb) {
    int ibinZeroFluct{hFluctuations->FindBin(0+GLOBAL_epsilon)};
    double integralError;
    for(int iBinRec{0}; iBinRec <= hResponseFluctuations->GetNbinsX()+1; iBinRec++){
      for(int iBinGen{0}; iBinGen <= hResponseFluctuations->GetNbinsY()+1; iBinGen++){
        double ptGen{hResponseFluctuations->GetYaxis()->GetBinLowEdge(iBinGen)}; 
        double ptRec_low{hResponseFluctuations->GetXaxis()->GetBinLowEdge(iBinRec)};
        double ptRec_up{hResponseFluctuations->GetXaxis()->GetBinLowEdge(iBinRec+1)};
        int iBin_fluct_low{hFluctuations->GetXaxis()->FindBin(ptRec_low - ptGen + GLOBAL_epsilon)};
        int iBin_fluct_high{hFluctuations->GetXaxis()->FindBin(ptRec_up - ptGen - GLOBAL_epsilon)};
        hResponseFluctuations->SetBinContent(iBinRec, iBinGen, hFluctuations->IntegralAndError(iBin_fluct_low, iBin_fluct_high, integralError)); 
        hResponseFluctuations->SetBinError(iBinRec, iBinGen, integralError); 
      }
    }
  }
  else {
    for(int iBinRec{0}; iBinRec <= hResponseFluctuations->GetNbinsX()+1; iBinRec++){
      hResponseFluctuations->SetBinContent(iBinRec, iBinRec, 1);
      hResponseFluctuations->SetBinError(iBinRec, iBinRec, 0);
    }
  }

};

void JEfficiency::setRespSettings()
{
  if (fPbPb) {
    auto xxbin = hMCRespSparse->GetAxis(0)->FindBin(centRange.first);
    auto yybin = hMCRespSparse->GetAxis(0)->FindBin(centRange.second);
    hMCRespSparse->GetAxis(0)->SetRange(xxbin, yybin);
  }
  hResponseDetector = hMCRespSparse->Projection(2,1);

  hCentRhoRandomConewoLeadingJet->Sumw2();
  auto xbin = hCentRhoRandomConewoLeadingJet->GetXaxis()->FindBin(centRange.first);
  auto ybin = hCentRhoRandomConewoLeadingJet->GetXaxis()->FindBin(centRange.second);
  hCentRhoRandomConewoLeadingJet->GetXaxis()->SetRange(xbin,ybin);
  hFluctuations = dynamic_cast<TH1D*>(hCentRhoRandomConewoLeadingJet->ProjectionY("_tmpfluctuations"));
  hFluctuations->Scale(1. / hFluctuations->Integral("width"),"");

};

void JEfficiency::createResponseMatrix()
{
  this->ptResponseFluctuations();

  hResponseTotal = jetutility::getMatrixProductTH2xTH2(hResponseFluctuations, hResponseDetector);

};
