#include <iostream>

#include <TFile.h>
#include <TH1.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TLatex.h>
#include <TLine.h>
#include <TVectorD.h>

#include "JEUtility.hpp"
#include "JEFW.hpp"
#include "rootStyle.hpp"


int main(int argc, char *argv[]) {

  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << "<jet radius>" << std::endl;
  }

  const char* Rval = argv[1];
  float fRval = std::atof(Rval);
  rootStyle();


  // const char* HLrun = argv[1];
  // unique_ptr<JEFW> jet{new JEFW(Form("/Users/joachimcarlokristianhansen/jet_analysis/hyperloop_data/LHC23zzh_pass4_small/jet/%s/AnalysisResults.root",HLrun))};
  // jet->Init("data");
  // auto h{jet->getHist()};

  TFile* f = new TFile(Form("processed_data/raw/separatePtPlane_q2_%i_%i-centrality-%i-%i-R%s.root",0,100,30,50,Rval),"READ");
  TH1F* h_in_inclusive_raw = dynamic_cast<TH1F*>(f->Get("hv_0"));
  TH1F* h_out_inclusive_raw = dynamic_cast<TH1F*>(f->Get("hv_1"));
  TVectorD* tmp = dynamic_cast<TVectorD*>(f->Get("nEvents"));
  double nEvents = static_cast<double>((*tmp)[0]);

  auto h = dynamic_cast<THnSparse*>(f->Get("hCentJetPtdPhiq2Clone"));

  

  h->GetAxis(0)->SetRange(30,50); /* centr: 30-50%      */
  h->GetAxis(3)->SetRange(0,100); /* q2:    full range  */
  auto hPtInclusive = h->Projection(1);

  h_in_inclusive_raw->Scale(1 / nEvents, "width");
  h_out_inclusive_raw->Scale(1 / nEvents, "width");
  hPtInclusive->Scale(1 / nEvents, "width");


  std::vector<int> xAxis = {0,200};

  TCanvas* c = new TCanvas("c", "Canvas with ratio", 800, 800);

  float upperPadHeight = 0.7;
  float lowerPadHeight = 0.3;

  TPad* upperPad = new TPad("upperPad", "Upper Pad", 0.0, lowerPadHeight, 1.0, 1.0);
  upperPad->SetBottomMargin(0.05);
  upperPad->Draw();

  TPad* lowerPad = new TPad("lowerPad", "Lower Pad", 0.0, 0.0, 1.0, lowerPadHeight);
  lowerPad->SetTopMargin(0.1); //0.02
  lowerPad->SetBottomMargin(0.3);
  lowerPad->Draw();


  upperPad->cd();

  gPad->SetLogy();

  hPtInclusive->SetLineColor(kBlack);
  hPtInclusive->GetXaxis()->SetRangeUser(xAxis.at(0),xAxis.at(1));
  hPtInclusive->GetXaxis()->SetTitleSize(0.05);
  hPtInclusive->GetYaxis()->SetTitleSize(0.05);
  hPtInclusive->GetYaxis()->SetTitle("#frac{1}{#it{N}_{ev}} #frac{d^{2}N}{d#it{p}_{T}d#it{#eta}}");
  hPtInclusive->Draw();

  h_in_inclusive_raw->SetLineColor(kRed);
  h_in_inclusive_raw->Draw("same");

  h_out_inclusive_raw->SetLineColor(kBlue);
  h_out_inclusive_raw->Draw("same");

  TLatex latex;
  latex.SetTextSize(0.04);  
  latex.DrawLatexNDC(.5, 0.8, "Pb--Pb #sqrt{s_{NN}} = 5.36 TeV");
  latex.DrawLatexNDC(.5,.75,Form("Charged jets, anti-#it{k}_{T}, R=%s",Rval));
  latex.DrawLatexNDC(.5,.7,Form("#it{p}_{T}^{lead track}>5 GeV/#it{c}, |#eta_{jet}|<%.1f",0.9-fRval));

  auto leg = new TLegend(0.45, 0.3, 0.65, 0.5);
  leg->AddEntry(hPtInclusive,"inclusive","l");
  leg->AddEntry(h_in_inclusive_raw,"in-plane","l");
  leg->AddEntry(h_out_inclusive_raw,"out-of-plane","l");
  leg->Draw();

  lowerPad->cd();

  TH1F* hRatioOut = (TH1F*)h_out_inclusive_raw->Clone("hRatioOut");
  hRatioOut->Divide(hPtInclusive);

  TH1F* hRatioIn = (TH1F*)h_in_inclusive_raw->Clone("hRatioIn");
  hRatioIn->Divide(hPtInclusive);

  hRatioOut->SetLineColor(kBlue);
  hRatioOut->GetXaxis()->SetRangeUser(xAxis.at(0),xAxis.at(1));
  hRatioOut->GetYaxis()->SetRangeUser(0,1.2);
  hRatioOut->GetXaxis()->SetTitleSize(0.05);
  hRatioOut->GetYaxis()->SetTitleSize(0.05);
  hRatioOut->GetYaxis()->SetTitle("in or out/inclusive");
  hRatioOut->GetXaxis()->SetTitle("#it{p}_{T}^{jet} (GeV/#it{c})");
  hRatioOut->Draw();

  hRatioIn->SetLineColor(kRed);
  hRatioIn->Draw("same");

  TLine *line = new TLine(xAxis.at(0), 1, xAxis.at(1), 1);
  line->SetLineStyle(2);
  line->Draw();


  c->SaveAs(Form("JFigures/rawyields/raw_pt_inout-plane-R%s.pdf",Rval));

  return 0;
}
