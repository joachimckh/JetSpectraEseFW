#include <memory>
#include <iostream>

#include <TFile.h>
#include <TH1.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TLatex.h>
#include <TLine.h>
#include <TVectorD.h>

#include "JEUtility.hpp"
#include "rootStyle.hpp"


int main(int argc, char *argv[]) {

  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <run number> <q2 min> <q2 max> <cent min> <cent max> <jet radius>" << std::endl;
  }

  int q2min = std::atoi(argv[1]);
  int q2max = std::atoi(argv[2]);
  int centmin = std::atoi(argv[3]);
  int centmax = std::atoi(argv[4]);
  const char* Rval = argv[5];
  float fRval = std::atof(Rval);
  float reso = std::atof(argv[6]);
  
  
  std::unique_ptr<TFile> f = std::unique_ptr<TFile>(new TFile(Form("processed_data/raw/separatePtPlane_q2_%i_%i-centrality-%i-%i-R%s.root",q2min,q2max,centmin,centmax,Rval),"READ"));
  TH1F* h_in_inclusive_raw = dynamic_cast<TH1F*>(f->Get("hv_0"));
  TH1F* h_out_inclusive_raw = dynamic_cast<TH1F*>(f->Get("hv_1"));
  TVectorD* tmp = dynamic_cast<TVectorD*>(f->Get("nEvents"));
  double nEvents = static_cast<double>((*tmp)[0]);
  std::cout << "nEvents: " << nEvents << std::endl;
  
  
  JEUtility *jet = new JEUtility(f.get(), reso); /* R in ideal case 0.71 (run2) */
  auto hs = jet->YieldCorrectedJet();
  TH1* h_in_inclusive_corr = hs.first;
  TH1* h_out_inclusive_corr = hs.second;


  const int n = 19;
  Double_t bin_edges[n+1]={0,5,10,15,20,25,30,35,40,45,50,55,60,70,80,90,100,120,160,200};
  TH1* h_in_inclusive = h_in_inclusive_raw->Rebin(n, "hInInclusiveReb", bin_edges);
  TH1* h_out_inclusive = h_out_inclusive_raw->Rebin(n, "hOutInclusiveReb", bin_edges);

  TH1* h_in_inclusive_corr_Rebin = h_in_inclusive_corr->Rebin(n, "hInCorr_re", bin_edges);
  TH1* h_out_inclusive_corr_Rebin = h_out_inclusive_corr->Rebin(n, "hOutCorr_re", bin_edges);

  h_in_inclusive->Scale(1 / nEvents, "width");
  h_out_inclusive->Scale(1 / nEvents, "width");
  h_in_inclusive_raw->Scale(1 / nEvents, "width");
  h_out_inclusive_raw->Scale(1 / nEvents, "width");
  h_in_inclusive_corr_Rebin->Scale(1 / nEvents, "width");
  h_out_inclusive_corr_Rebin->Scale(1 / nEvents, "width");
  // h_in_inclusive->Scale(1 / h_in_inclusive->Integral());
  // h_out_inclusive->Scale(1 / h_out_inclusive->Integral());
  // h_in_inclusive_raw->Scale(1 / h_in_inclusive_raw->Integral());
  // h_out_inclusive_raw->Scale(1 / h_out_inclusive_raw->Integral());
  // h_in_inclusive_corr_Rebin->Scale(1 / h_in_inclusive_corr_Rebin->Integral());
  // h_out_inclusive_corr_Rebin->Scale(1 / h_out_inclusive_corr_Rebin->Integral());
  

  rootStyle();

  std::vector<int> xAxis = {0,200};

  TCanvas c1_raw("c1_raw","",800,600);
  c1_raw.cd();

  h_in_inclusive->GetYaxis()->SetTitle("#frac{1}{#it{N}_{ev}} #frac{d^{2}N}{d#it{p}_{T}d#it{#eta}}");
  h_in_inclusive->SetLineColor(kRed);
  h_in_inclusive->GetXaxis()->SetRangeUser(xAxis.at(0),xAxis.at(1));
  h_in_inclusive->GetYaxis()->SetRangeUser(1e-8,1);
  h_in_inclusive->Draw("EP");

  h_out_inclusive->SetLineColor(kBlack);
  h_out_inclusive->Draw("EPsame");

  gPad->SetLogy();

  TLegend * leg_raw = new TLegend(  0.65,  0.65,  0.86, 0.75);
  leg_raw->AddEntry(h_in_inclusive,"in-plane (Not corrected)","l");
  leg_raw->AddEntry(h_out_inclusive,"out-of-plane (Not corrected)","l");
  leg_raw->Draw();

  TLatex latex_raw;
  latex_raw.SetTextSize(0.04);  
  latex_raw.DrawLatexNDC(.3, 0.8, "Pb--Pb #sqrt{s_{NN}} = 5.36 TeV");
  latex_raw.DrawLatexNDC(.3,.75,Form("Charged jets, anti-#it{k}_{T}, R=%s",Rval));
  latex_raw.DrawLatexNDC(.3,.7,Form("#it{p}_{T}^{lead track}>5 GeV/#it{c}, |#eta_{jet}|<%.1f",0.9-fRval));
  latex_raw.DrawLatexNDC(.3,.65,Form("%i-%i%%-#it{q}_{2}^{FT0C}",q2min,q2max));

  c1_raw.SaveAs(Form("JFigures/yield/pt-in-and-outofplane-raw-cent-%i-%i-q2-%i-%i-R%s.pdf",centmin,centmax,q2min,q2max,Rval));

  TCanvas c1("c1","",800,600);
  c1.cd();

  h_in_inclusive_corr_Rebin->GetYaxis()->SetTitle("#frac{1}{#it{N}_{ev}} #frac{d^{2}N}{d#it{p}_{T}d#it{#eta}}");
  h_in_inclusive_corr_Rebin->SetLineColor(kRed);
  h_in_inclusive_corr_Rebin->GetXaxis()->SetRangeUser(xAxis.at(0),xAxis.at(1));
  h_in_inclusive_corr_Rebin->GetYaxis()->SetRangeUser(1e-8,1);
  h_in_inclusive_corr_Rebin->Draw("EP");

  h_out_inclusive_corr_Rebin->SetLineColor(kBlack);
  // h_out_inclusive_corr_Rebin->GetXaxis()->SetRangeUser(xAxis.at(0),xAxis.at(1));
  h_out_inclusive_corr_Rebin->Draw("EPsame");

  gPad->SetLogy();

  TLegend * leg = new TLegend(  0.65,  0.65,  0.86, 0.75);
  // leg->AddEntry(h_in_inclusive,"in-plane","l");
  // leg->AddEntry(h_out_inclusive,"out-of-plane","l");
  leg->AddEntry(h_in_inclusive_corr_Rebin,"in-plane (Corrected)","l");
  leg->AddEntry(h_out_inclusive_corr_Rebin,"out-of-plane (Corrected)","l");
  leg->Draw();

  TLatex latex;
  latex.SetTextSize(0.04);  
  latex.DrawLatexNDC(.3, 0.8, "Pb--Pb #sqrt{s_{NN}} = 5.36 TeV");
  latex.DrawLatexNDC(.3,.75,Form("Charged jets, anti-#it{k}_{T}, R=%s",Rval));
  latex.DrawLatexNDC(.3,.7,Form("#it{p}_{T}^{lead track}>5 GeV/#it{c}, |#eta_{jet}|<%.1f",0.9-fRval));
  latex.DrawLatexNDC(.3,.65,Form("%i-%i%%-#it{q}_{2}^{FT0C}",q2min,q2max));


  c1.SaveAs(Form("JFigures/yield/pt-in-and-outofplane-yieldcorrected-cent-%i-%i-q2-%i-%i-R%s.pdf",centmin,centmax,q2min,q2max,Rval));

  TCanvas c1rat("c1rat","",400,200);
  TH1F *hRatio = (TH1F*)h_out_inclusive->Clone("hRatio");
  TH1F *hRatioCorr = (TH1F*)h_out_inclusive_corr_Rebin->Clone("hRatioCorr");
  hRatio->Divide(h_in_inclusive);  
  hRatio->SetLineColor(kBlack);
  hRatio->GetYaxis()->SetTitle("out-of-plane/in-plane");
  hRatio->GetXaxis()->SetRangeUser(xAxis.at(0),xAxis.at(1));
  hRatio->GetYaxis()->SetRangeUser(0,1.4);
  hRatio->Draw();

  hRatioCorr->Divide(h_in_inclusive_corr_Rebin);
  hRatioCorr->SetLineColor(kRed);
  hRatioCorr->Draw("same");

  TLine *line = new TLine(xAxis.at(0), 1, xAxis.at(1), 1);
  line->SetLineStyle(2);
  line->Draw();

  auto leg2 = new TLegend(0.65, 0.2, 0.86, 0.45);
  leg2->AddEntry(hRatio,"Before #it{R}_{2} correction","l");
  leg2->AddEntry(hRatioCorr,"After #it{R}_{2} correction","l");
  leg2->Draw();

  TLatex latex2;
  latex2.SetTextSize(0.04);  
  latex2.DrawLatexNDC(.3, 0.8, "Pb--Pb #sqrt{s_{NN}} = 5.36 TeV");
  latex2.DrawLatexNDC(.3,.75,Form("Charged jets, anti-#it{k}_{T}, R=%s",Rval));
  latex2.DrawLatexNDC(.3,.7,Form("#it{p}_{T}^{lead track}>5 GeV/#it{c}, |#eta_{jet}|<%.1f",0.9-fRval));
  latex2.DrawLatexNDC(.3,.65,Form("%i-%i%%-#it{q}_{2}^{FT0C}",q2min,q2max));

  c1rat.SaveAs(Form("JFigures/yield/pt-out-divided-in-cent-%i-%i-q2-%i-%i-R%s.pdf",centmin,centmax,q2min,q2max,Rval));

  TCanvas c2("c2","",800,600);
  c2.cd();

  TH1F* hRatCorr = (TH1F*)h_in_inclusive_corr_Rebin->Clone("hRatCorr");
  hRatCorr->Divide(h_in_inclusive);

  TH1F* hRatCorr2 = (TH1F*)h_out_inclusive_corr_Rebin->Clone("hRatCorrOut");
  hRatCorr2->Divide(h_out_inclusive);

  hRatCorr->GetYaxis()->SetTitle("corrected (after/before)");
  hRatCorr->GetXaxis()->SetRangeUser(xAxis.at(0),xAxis.at(1));
  hRatCorr->GetYaxis()->SetRangeUser(0.3,1.8);
  hRatCorr->SetLineColor(kRed);
  hRatCorr->Draw();

  hRatCorr2->SetLineColor(kBlue);
  hRatCorr2->Draw("same");

  TLatex latex3;
  latex3.SetTextSize(0.04);  
  latex3.DrawLatexNDC(.3, 0.8, "Pb--Pb #sqrt{s_{NN}} = 5.36 TeV");
  latex3.DrawLatexNDC(.3,.75,Form("Charged jets, anti-#it{k}_{T}, R=%s",Rval));
  latex3.DrawLatexNDC(.3,.7,Form("#it{p}_{T}^{lead track}>5 GeV/#it{c}, |#eta_{jet}|<%.1f",0.9-fRval));
  latex3.DrawLatexNDC(.3,.65,Form("%i-%i%%-#it{q}_{2}^{FT0C}",q2min,q2max));

  c2.SaveAs(Form("JFigures/yield/yieldRatio-cent-%i-%i-q2-%i-%i-R%s.pdf",centmin,centmax,q2min,q2max,Rval));


  return 0;
}