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
    std::cerr << "Usage: " << argv[0] << " <run number> <jet radius>, <ptLow>, <ptHigh>" << std::endl;
  }
  const char* hlRun{argv[1]};
  const char* rSize{argv[2]};
  const char* rVal{argv[3]};
  double fRval = std::atof(rVal);
  const char* dataset{argv[4]};


  rootStyle();

  
  TFile* f = new TFile(Form("/Users/joachimcarlokristianhansen/jet_analysis/hyperloop_data/%s/jet/%s/AnalysisResults.root",rSize,hlRun),"READ");
  TDirectory *dir = dynamic_cast<TDirectory*>(f->Get("jet-spectra-ese-task"));

  auto h = dynamic_cast<THnSparse*>(dir->Get("hCentJetPtdPhiq2")); /* Centrality, pT, dPhi, q2 */
  h->GetAxis(0)->SetRange(h->GetAxis(0)->FindBin(30),h->GetAxis(0)->FindBin(50)); /* centr: 30-50% */

  // int ptLow{std::atoi(argv[4])};
  // int ptHigh{std::atoi(argv[5])};
  // h->GetAxis(1)->SetRange(ptLow,ptHigh); /* centr: 30-50% */
  auto h_phi_q2 = h->Projection(3,2); /* dPhi, q2, pt */
  h->GetAxis(1)->SetRange(0,-1);
  auto h_pt_q2 = h->Projection(3,1); /* pT, q2 */


  TH1F* hEvs = dynamic_cast<TH1F*>(dir->Get("hEventCounter"));
  double nEvents{hEvs->GetBinContent(7)}; /* 30-50 centr */

  TCanvas c1("c1","",800,600);
  c1.cd();

  // h_phi_q2->GetZaxis()->SetRange(ptLow,ptHigh);

  h_phi_q2->GetYaxis()->SetRange(0,10);
  auto h_phi_q2_proj1 = h_phi_q2->ProjectionX("hPhiQ2Proj_low");
  h_phi_q2->GetYaxis()->SetRange(90,100);
  auto h_phi_q2_proj2 = h_phi_q2->ProjectionX("hPhiQ2Proj_high");
  
  // low kOrange-1
  // high kCyan+2

  h_phi_q2_proj1->SetLineColor(kOrange-1);
  h_phi_q2_proj1->SetMarkerStyle(20);
  h_phi_q2_proj1->SetMarkerColor(kOrange-1);

  h_phi_q2_proj2->SetLineColor(kCyan+2);
  h_phi_q2_proj2->SetMarkerStyle(20);
  h_phi_q2_proj2->SetMarkerColor(kCyan+2);

  h_phi_q2_proj2->GetYaxis()->SetRangeUser(0,250*10e3);
  h_phi_q2_proj2->GetXaxis()->SetTitleSize(0.05);
  h_phi_q2_proj2->GetYaxis()->SetTitleSize(0.05);
  h_phi_q2_proj2->GetYaxis()->SetTitle("entries");;
  h_phi_q2_proj2->Draw("EP");
  h_phi_q2_proj1->Draw("EP same");
  

  auto leg = new TLegend(0.35,0.18,0.55,0.38);
  leg->AddEntry(h_phi_q2_proj2,"30% highest-#it{q}_{2}","lep");
  leg->AddEntry(h_phi_q2_proj1,"30% lowest-#it{q}_{2}","lep");
  leg->Draw();

  TLatex latex;
  latex.SetTextSize(0.04);  
  latex.DrawLatexNDC(.5, 0.85, "Pb--Pb #sqrt{s_{NN}} = 5.36 TeV");
  latex.DrawLatexNDC(.5,.8,Form("Charged jets, anti-#it{k}_{T}, R=%s",rVal));
  latex.DrawLatexNDC(.5,.75,Form("#it{p}_{T}^{lead track}>5 GeV/#it{c}, |#eta_{jet}|<%.1f",0.9-fRval));
  // latex.DrawLatexNDC(.5,.65,Form("#it{p}_{T}^{jet} = %i-%i GeV/#it{c}",ptLow,ptHigh));

  c1.SaveAs(Form("JFigures/various/phi_q2_%s_jet_pt_full.pdf",rVal));//,ptLow,ptHigh));



  TCanvas c2("c2","",800,600);
  c2.cd();
  gPad->SetLogy();
  
  const int n = 19;
  Double_t bin_edges[n+1]={0,5,10,15,20,25,30,35,40,45,50,55,60,70,80,90,100,120,160,200};

  auto h_pt_inclusive = h_pt_q2->ProjectionX("hPtInclusive");
  h_pt_q2->GetYaxis()->SetRange(0,30);
  auto h_pt_q2_proj1 = h_pt_q2->ProjectionX("hPtQ2Proj_low");
  auto h_pt_q2_proj1_r = h_pt_q2_proj1->Rebin(n, "h_pt_q2_proj1_r", bin_edges);
  h_pt_q2->GetYaxis()->SetRange(70,100);
  auto h_pt_q2_proj2 = h_pt_q2->ProjectionX("hPtQ2Proj_high");
  auto h_pt_q2_proj2_r = h_pt_q2_proj2->Rebin(n, "h_pt_q2_proj2_r", bin_edges);

  h_pt_q2_proj1_r->Scale(1 / nEvents, "width");
  h_pt_q2_proj2_r->Scale(1 / nEvents, "width");

  h_pt_q2_proj1_r->SetLineColor(kOrange-1);
  h_pt_q2_proj1_r->SetMarkerStyle(20);
  h_pt_q2_proj1_r->SetMarkerColor(kOrange-1);
  h_pt_q2_proj2_r->SetLineColor(kCyan+2);
  h_pt_q2_proj2_r->SetMarkerStyle(24);
  h_pt_q2_proj2_r->SetMarkerColor(kCyan+2);

  h_pt_q2_proj1_r->GetXaxis()->SetTitleSize(0.05);
  h_pt_q2_proj1_r->GetYaxis()->SetTitleSize(0.05);
  h_pt_q2_proj1_r->GetYaxis()->SetTitle("#frac{1}{#it{N}_{ev}} #frac{d^{2}N}{d#it{p}_{T}d#it{#eta}}");
  h_pt_q2_proj1_r->Draw("EP");
  h_pt_q2_proj2_r->Draw("EPsame");
  
  

  auto leg2 = new TLegend(0.65,0.35,0.85,0.55);
  leg2->AddEntry(h_pt_q2_proj2_r,"30% highest-#it{q}_{2}","lep");
  leg2->AddEntry(h_pt_q2_proj1_r,"30% lowest-#it{q}_{2}","lep");
  leg2->Draw();

  TLatex latex2;
  latex2.SetTextSize(0.04);  
  latex2.DrawLatexNDC(.5, 0.85, "Pb--Pb #sqrt{s_{NN}} = 5.36 TeV");
  latex2.DrawLatexNDC(.5,.8,Form("Charged jets, anti-#it{k}_{T}, R=%s",rVal));
  latex2.DrawLatexNDC(.5,.75,Form("#it{p}_{T}^{lead track}>5 GeV/#it{c}, |#eta_{jet}|<%.1f",0.9-fRval));

  c2.SaveAs(Form("JFigures/various/pt_q2_%s.pdf",rVal));
  

  TCanvas* c123 = new TCanvas("c123", "Canvas with ratio", 800, 800);

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

  h_pt_q2_proj1_r->GetXaxis()->SetTitleSize(0.05);
  h_pt_q2_proj1_r->GetYaxis()->SetTitleSize(0.05);
  h_pt_q2_proj1_r->GetYaxis()->SetTitle("#frac{1}{#it{N}_{ev}} #frac{d^{2}N}{d#it{p}_{T}d#it{#eta}}");
  h_pt_q2_proj1_r->GetXaxis()->SetRangeUser(30,120);
  h_pt_q2_proj1_r->Draw("EP");
  h_pt_q2_proj2_r->Draw("EPsame");


  auto leg3 = new TLegend(0.65,0.35,0.85,0.55);
  leg3->AddEntry(h_pt_q2_proj2_r,"30% highest-#it{q}_{2}","lep");
  leg3->AddEntry(h_pt_q2_proj1_r,"30% lowest-#it{q}_{2}","lep");
  leg3->Draw();

  TLatex latex3;
  latex3.SetTextSize(0.04);
  latex3.DrawLatexNDC(.5, 0.85, "Pb--Pb #sqrt{s_{NN}} = 5.36 TeV");
  latex3.DrawLatexNDC(.5,.8,Form("Charged jets, anti-#it{k}_{T}, R=%s",rVal));
  latex3.DrawLatexNDC(.5,.75,Form("#it{p}_{T}^{lead track}>5 GeV/#it{c}, |#eta_{jet}|<%.1f",0.9-fRval));

  lowerPad->cd();

  TH1F* hRatioOut = (TH1F*)h_pt_q2_proj2_r->Clone("hRatioOut");
  hRatioOut->Divide(h_pt_q2_proj1_r);

  hRatioOut->SetLineColor(kBlack);
  hRatioOut->SetMarkerStyle(20);
  hRatioOut->SetMarkerColor(kBlack);

  hRatioOut->GetYaxis()->SetTitle("highest/lowest");
  hRatioOut->GetXaxis()->SetTitle("#it{p}_{T} (GeV/#it{c})");

  hRatioOut->GetXaxis()->SetTitleSize(0.1);
  hRatioOut->GetYaxis()->SetTitleSize(0.05);

  hRatioOut->GetXaxis()->SetRangeUser(30,120);
  hRatioOut->GetYaxis()->SetRangeUser(0.5,1.5);

  hRatioOut->Draw();

  TLine *line = new TLine(30, 1, 120, 1);
  line->SetLineColor(kBlack);
  line->Draw();


  c123->SaveAs(Form("JFigures/various/pt_q2_ratio_%s.pdf",rVal));

  return 0;
}
