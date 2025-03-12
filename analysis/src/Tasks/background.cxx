#include <TFile.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TLatex.h>
#include <TF1.h>
#include <THnSparse.h>

#include "JEFW.hpp"

#include "../JAnalysis/rootStyle.hpp"

TH2D* createFluctMatr(TH1* hFluctuations);
void drawBkgfluctuations(TH1* pr, std::vector<int> const& cent, std::vector<int> const& q2Range,const char* indentifier, const char* saveID);
void drawMat(TH2D* hMat, std::vector<int> const& cent, std::vector<int> const& q2Range, const char* id, const char* saveID);

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <bkg run> <bkg size>, <q2 min, q2 max>" << std::endl;
  }
  rootStyle();
  const char* bkgRun{argv[1]};
  const char* bkgSize{argv[2]}; //
  int q2Min = std::atoi(argv[3]);
  int q2Max = std::atoi(argv[4]);
  
  std::vector<int> q2Range = {q2Min, q2Max};
  std::vector<int> cent{30,50};

  unique_ptr<JEFW> jet{new JEFW(Form("/Users/joachimcarlokristianhansen/jet_analysis/hyperloop_data/%s/jet/%s/AnalysisResults.root",bkgSize, bkgRun),JEFW::DataType::BACKGROUND,"")};
  jet->setCentrality<false>(cent);
  jet->setq2Range<false>(q2Range);
  auto objarr = jet->separatePlanes<false>();
  auto in = dynamic_cast<TH1F*>(objarr->FindObject("hv_0"));
  in->Scale(1. / in->Integral("width"),"");
  auto out = dynamic_cast<TH1F*>(objarr->At(1));
  out->Scale(1. / out->Integral("width"),"");

  auto fBackground = new TFile(Form("/Users/joachimcarlokristianhansen/jet_analysis/hyperloop_data/%s/jet/%s/AnalysisResults.root",bkgSize,bkgRun),"READ");
  auto tmpDir = dynamic_cast<TDirectory*>(fBackground->Get("jet-spectra-ese-task")) ;

  // loop over the different random cones hCentRhoRandomConewoLeadingJet, hCentRhoRandomConeRndTrackDirwoOneLeadingJet, hCentRhoRandomConeRndTrackDirwoTwoLeadingJet
  std::array<THnSparse*, 1> histograms = {
    dynamic_cast<THnSparse*>(tmpDir->Get("hCentRhoRandomConewoLeadingJet")),
    // dynamic_cast<THnSparse*>(tmpDir->Get("hCentRhoRandomConeRndTrackDirwoOneLeadingJet")),
    // dynamic_cast<THnSparse*>(tmpDir->Get("hCentRhoRandomConeRndTrackDirwoTwoLeadingJet"))
  };

  for (auto *hRandomCone : histograms) {
    // auto hRandomCone = dynamic_cast<THnSparse*>(tmpDir->Get("hCentRhoRandomConewoLeadingJet")); /*hCentRhoRandomConewoLeadingJet, hCentRhoRandomConeRndTrackDirwoOneLeadingJet, hCentRhoRandomConeRndTrackDirwoTwoLeadingJet*/
    hRandomCone->GetAxis(3)->SetRange(q2Min, q2Max);
    TH2* h2 = hRandomCone->Projection(1,0);
    h2->SetName(Form("abawlda_%s",hRandomCone->GetName()));
    auto xbin = h2->GetXaxis()->FindBin(cent.at(0));
    auto ybin = h2->GetXaxis()->FindBin(cent.at(1));
    h2->GetXaxis()->SetRange(xbin,ybin);
    auto hFluctuations = dynamic_cast<TH1D*>(h2->ProjectionY(Form("_tmpfluctuations_%s",hRandomCone->GetName())));
    hFluctuations->Scale(1. / hFluctuations->Integral("width"),"");
    drawBkgfluctuations(hFluctuations, cent, q2Range, "", hRandomCone->GetName());
    drawBkgfluctuations(in, cent, q2Range, "in-plane", hRandomCone->GetName());
    drawBkgfluctuations(out, cent, q2Range, "out-of-plane", hRandomCone->GetName());
    

    auto hMat = createFluctMatr(hFluctuations);
    auto hMatIn = createFluctMatr(in);
    auto hMatOut = createFluctMatr(out);
    
    drawMat(hMat, cent, q2Range, "", hRandomCone->GetName());
    drawMat(hMatIn, cent, q2Range, "in-plane", hRandomCone->GetName());
    drawMat(hMatOut, cent, q2Range, "out-of-plane", hRandomCone->GetName());
  }
}

void drawMat(TH2D* hMat, std::vector<int> const& cent, std::vector<int> const& q2Range, const char* id, const char* saveID) {
  auto c1 = new TCanvas("c1","c1",800,600);
  c1->cd();
  gPad->SetLogz();
  hMat->SetMinimum(1e-10);
  hMat->SetMaximum(1e0);
  hMat->GetXaxis()->SetTitle("#it{p}_{T,jet}^{rec}");
  hMat->GetYaxis()->SetTitle("#it{p}_{T,jet}^{gen}");
  hMat->SetStats(0);

  hMat->Draw("colz");

  TLatex latex;
  latex.SetTextSize(0.04);
  latex.DrawLatexNDC(0.2, 0.8, "Pb--Pb #sqrt{s_{NN}} = 5.36 TeV");
  latex.DrawLatexNDC(0.2, 0.75, Form("Centrality: %i-%i%%", cent.at(0), cent.at(1)));
  latex.DrawLatexNDC(0.2, 0.7, Form("#it{q}_{2}: %i-%i", q2Range.at(0), q2Range.at(1)));
  latex.DrawLatexNDC(0.2, 0.65, Form("%s", id));

  c1->SaveAs(Form("JFigures/background/fluctMatr_%i_%i_%s_%s.pdf",q2Range.at(0),q2Range.at(1),id, saveID));
}

void drawBkgfluctuations(TH1* pr, std::vector<int> const& cent, std::vector<int> const& q2Range,const char* indentifier, const char* saveID) {

  // auto pr = hist->ProjectionY("bkgProj__", hist->GetXaxis()->FindBin(30), hist->GetXaxis()->FindBin(50));

  auto cBkg = new TCanvas("cBkg","cBkg",800,600);
  cBkg->cd();
  gPad->SetLogy();

  pr->SetLineColor(kBlack);
  pr->SetMarkerStyle(20);
  pr->SetMarkerColor(kBlack);
  pr->SetStats(0);

  pr->GetXaxis()->SetRangeUser(-40,80);
  pr->Scale(1./pr->Integral("width"));
  pr->GetYaxis()->SetTitle("Probability density");
  pr->GetXaxis()->SetTitle("#it{p}_{T, Random cone} - #it{A}_{Random cone} * #rho");
  pr->Draw("EP");

  TF1* fitGauss = new TF1("fitGauss", "gaus", -20, 60);
  TFitResultPtr fitResult = pr->Fit(fitGauss, "SN");

  fitGauss->SetLineColor(kRed);
  fitGauss->SetLineWidth(2);
  fitGauss->SetLineStyle(2);

  fitGauss->Draw("same");
  double mean = fitGauss->GetParameter(1);
  double sigma = fitGauss->GetParameter(2);

  TLatex latex;
  latex.SetTextSize(0.04);
  latex.DrawLatexNDC(0.5, 0.8, "Pb--Pb #sqrt{s_{NN}} = 5.36 TeV");
  latex.DrawLatexNDC(0.5, 0.75, Form("Centrality: %i-%i%%", cent.at(0), cent.at(1)));
  latex.DrawLatexNDC(0.5, 0.7, Form("#it{q}_{2}: %i-%i", q2Range.at(0), q2Range.at(1)));
  latex.DrawLatexNDC(0.5, 0.65, Form("%s", indentifier));

  auto leg = new TLegend(0.6, 0.3, 0.8, 0.5);
  leg->AddEntry(pr, "Random Cones", "lep");
  leg->AddEntry(fitGauss, Form("fit #mu=%.2f #sigma=%.2f",mean,sigma), "l");
  leg->Draw();


  cBkg->SaveAs(Form("JFigures/background/bkgfluctuations_%i_%i_%s_%s.pdf",q2Range.at(0),q2Range.at(1),indentifier,saveID));
}


TH2D* createFluctMatr(TH1* hFluctuations){

  auto hResponseFluctuations = new TH2D("hResponseFluctuations_Fluct","",200,0,200,200,0,200);

  constexpr double GLOBAL_epsilon{0.0000000000000000001};

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

  return hResponseFluctuations;
}
