/// \file JEUtility.cxx
/// \brief Utility Class for handling jet spectra ESE framework
///
/// \author Joachim C. K. B. Hansen

#include "JEUtility.hpp"

#include <utility>

ClassImp(JEUtility);

JEUtility::JEUtility(TFile* planes, double R) : R(R)
{
  h_in = dynamic_cast<TH1*>(planes->Get("hv_0"));
  h_out = dynamic_cast<TH1*>(planes->Get("hv_1"));
}

JEUtility::JEUtility(TH1* h_in, TH1* h_out, double R) : h_in(h_in), h_out(h_out), R(R)
{
}

JEUtility::~JEUtility() 
{
}

TH1* JEUtility::calculateV2Jet()
{
  const double cst{std::numbers::pi/( 3.0 * std::sqrt(3.0) )};

  // TH1* hReturn = new TH1D("hV2Jet",";#it{p}_{T,jet}; #it{v}_{2}",210,0,210);
  TH1* hReturn = dynamic_cast<TH1*>(h_in->Clone("hV2Jet"));

  for (int i{1}; i < h_in->GetNbinsX()+1; i++)
  {
    double A{h_in->GetBinContent(i)};
    double B{h_out->GetBinContent(i)};
    double sigmaA{h_in->GetBinError(i)};
    double sigmaB{h_out->GetBinError(i)};


    double R2inv{1 / R};
    double num{A - B};
    double denom{A + B};
    double val{(denom!=0) ? num/denom : 0};
    
    double v2{cst * R2inv * val};
    double error{(denom!=0) ? cst * R2inv * 2 / (denom * denom) * std::sqrt(B * B * sigmaA * sigmaA + A * A * sigmaB * sigmaB) : 0};

    hReturn->SetBinContent(i, v2);
    hReturn->SetBinError(i, error);


  }
  return hReturn;
};

void JEUtility::JERebin(int n, Double_t* bin_edges)
{
  /* rebin */
  h_in = dynamic_cast<TH1F*>(h_in->Rebin(n, "hInReb", bin_edges));
  h_out = dynamic_cast<TH1F*>(h_out->Rebin(n, "hOutReb", bin_edges));
};

std::pair<TH1*, TH1*> JEUtility::YieldCorrectedJet()
{
  /* 
    N_in^Corr = N_in + N_out / (1 + N_out^Corr/N_in^Corr) 
    N_out^Corr= N_in + N_out / (1 + N_in^Corr/N_out^Corr)
  */
  TH1* h_rat{this->YieldRatio()};
  TH1* h_in_Corr{dynamic_cast<TH1*>(h_in->Clone("hYieldCorrectedJet_in"))};
  TH1* h_out_Corr{dynamic_cast<TH1*>(h_out->Clone("hYieldCorrectedJet_out"))};
  for (int i{1}; i < h_in->GetNbinsX()+1; i++)
  {
    double num{h_in->GetBinContent(i) + h_out->GetBinContent(i)};


    double denum_in{1.0 + h_rat->GetBinContent(i)};
    double denum_out{1.0 + 1.0/h_rat->GetBinContent(i)};
    double val_in{(denum_in!=0) ? num/denum_in : 0};
    double val_out{(denum_out!=0) ? num/denum_out : 0};

    h_in_Corr->SetBinContent(i, val_in);
    h_in_Corr->SetBinError(i, h_in->GetBinError(i));
    h_out_Corr->SetBinContent(i, val_out);
    h_out_Corr->SetBinError(i, h_out->GetBinError(i));
  }


  return std::make_pair(h_in_Corr, h_out_Corr);
}
TH1* JEUtility::YieldRatio()
{ 
  /* N_out/N_in */
  TH1* hV2{this->calculateV2Jet()};
  TH1 *hRatio{dynamic_cast<TH1*>(hV2->Clone("hRatio"))};
  for (int i{1}; i < hV2->GetNbinsX()+1; i++)
  {
    double num{2 * std::numbers::pi - 1.0 * hV2->GetBinContent(i) * 6*std::sqrt(3)};
    double denum{2 * std::numbers::pi + 1.0 * hV2->GetBinContent(i) * 6*std::sqrt(3)};
    double val{(denum!=0) ? num/denum : 0};
    hRatio->SetBinContent(i, val);
  }
  return hRatio;
}

namespace jetutility 
{
    
  TH2D* getMatrixProductTH2xTH2(TH2D* histA, TH2D* histB) { /* taken from Aimeric */
    int nBinsX_A{histA->GetNbinsX()};
    int nBinsY_A{histA->GetNbinsY()};
    int nBinsX_B{histB->GetNbinsX()};
    int nBinsY_B{histB->GetNbinsY()};

    if (nBinsY_A != nBinsX_B) {
      std::cout << "#########################################################################################" << std::endl;
      std::cout << "###################### MATRIX PRODUCT IMPOSSIBLE DUE TO DIMENSIONS ######################" << std::endl;
      std::cout << "#########################################################################################" << std::endl;
      std::cout << "matrix A times B, with A(" << nBinsY_A << " x " << nBinsX_A << ") and B(" << nBinsY_B << " x " << nBinsX_B << ")" << std::endl;
    }

    int nBinsK{nBinsY_A};
    
    std::vector<double> vectorBinsX_AB{GetTH1Bins(histB->ProjectionX(histB->GetName()+(TString)"projX", 1, nBinsX_B))};
    std::vector<double> vectorBinsY_AB{GetTH1Bins(histA->ProjectionY(histA->GetName()+(TString)"projY", 1, nBinsY_A))};
    double* binsX_AB = &vectorBinsX_AB[0];
    double* binsY_AB = &vectorBinsY_AB[0];
    int nBinsX_AB{nBinsX_B};
    int nBinsY_AB{nBinsY_A};

    TH2D* histAB = new TH2D("histAB", "histAB", nBinsX_B, binsX_AB, nBinsY_A, binsY_AB);
    histAB->Sumw2();


    double productContent_iBinX_iBinY;
    double productError2_iBinX_iBinY;
    for(int iBinX = 0; iBinX <= nBinsX_AB+1; iBinX++){ // 0 and n+1 take underflow and overflow into account
      for(int iBinY = 0; iBinY <= nBinsY_AB+1; iBinY++){ // 0 and n+1 take underflow and overflow into account
        productContent_iBinX_iBinY = 0;
        productError2_iBinX_iBinY = 0;
        if (0 < iBinX && iBinX < nBinsX_AB+1 && 0 < iBinY && iBinY < nBinsY_AB+1) { // reason we do this, is because we don't want the under/ofverflows spread inside the matrix when we multiply, which happens when we do ; by separating it, we can still have the under/overflows calculated correctly in the else case, so that we can use them for the kinematic efficiency
          for(int iBinK{1}; iBinK <= nBinsK; iBinK++){ // 0 and n+1 take underflow and overflow into account
            productContent_iBinX_iBinY += histA->GetBinContent(iBinX, iBinK) * histB->GetBinContent(iBinK, iBinY); 
            productError2_iBinX_iBinY += std::pow(histB->GetBinContent(iBinK, iBinY), 2)*std::pow(histA->GetBinError(iBinX, iBinK), 2) + std::pow(histA->GetBinContent(iBinX, iBinK), 2)*std::pow(histB->GetBinError(iBinK, iBinY), 2); // simple sigma_ab = a2sigma_a2 + b2sigma_b2 ; that assumes there are no correlations; here it s background fluct from PbPB sim, and detector effects from a pp sim, so we can maybe say theyre not correlated    
          }
        } else {
          for(int iBinK{0}; iBinK <= nBinsK+1; iBinK++){ // 0 and n+1 take underflow and overflow into account
            productContent_iBinX_iBinY += histA->GetBinContent(iBinX, iBinK) * histB->GetBinContent(iBinK, iBinY); 
            productError2_iBinX_iBinY += std::pow(histB->GetBinContent(iBinK, iBinY), 2)*std::pow(histA->GetBinError(iBinX, iBinK), 2) + std::pow(histA->GetBinContent(iBinX, iBinK), 2)*std::pow(histB->GetBinError(iBinK, iBinY), 2); // simple sigma_ab = a2sigma_a2 + b2sigma_b2 ; that assumes there are no correlations; here it s background fluct from PbPB sim, and detector effects from a pp sim, so we can maybe say theyre not correlated    
          }
        }
        histAB->SetBinContent(iBinX, iBinY, productContent_iBinX_iBinY);
        histAB->SetBinError(iBinX, iBinY, std::sqrt(productError2_iBinX_iBinY));  
      }
    }
    return histAB;
  };

  std::vector<double> GetTH1Bins(TH1* H1_histo) {
    std::vector<double> bins;
    for(int iBin{1}; iBin <= H1_histo->GetNbinsX(); iBin++){
      bins.push_back(H1_histo->GetBinLowEdge(iBin));
    }
    bins.push_back(H1_histo->GetXaxis()->GetBinUpEdge(H1_histo->GetNbinsX()));
    return bins;
  }

  void WeightMatrixWithPrior(TH2D* H2D_hist, TH1D* priorSpectrum){
  
    double binContent{-999.};
    double binError{-999.}; 
    double binErrorA{-999.};
    double binErrorB{-999.};
    double errorA{-999.};
    double errorB{-999.};
    for(int iBinY{1}; iBinY <= H2D_hist->GetNbinsY(); iBinY++){ // 0 and n+1 take underflow and overflow into account
      for(int iBinX{1}; iBinX <= H2D_hist->GetNbinsX(); iBinX++){ // 0 and n+1 take underflow and overflow into account
        binContent = H2D_hist->GetBinContent(iBinX, iBinY) * priorSpectrum->GetBinContent(iBinY);
        errorA = priorSpectrum->GetBinContent(iBinY)*priorSpectrum->GetBinContent(iBinY) * H2D_hist->GetBinError(iBinX, iBinY)*H2D_hist->GetBinError(iBinX, iBinY);
        errorB = H2D_hist->GetBinContent(iBinX, iBinY)*H2D_hist->GetBinContent(iBinX, iBinY) * priorSpectrum->GetBinError(iBinY)*priorSpectrum->GetBinError(iBinY);
        H2D_hist->SetBinContent(iBinX, iBinY, binContent);
        H2D_hist->SetBinError(iBinX, iBinY, sqrt(errorA + errorB)); // sigma(AB)2 = sigma(A)2 B2 + sigma(B)2 A2
      }
    }
  }

}