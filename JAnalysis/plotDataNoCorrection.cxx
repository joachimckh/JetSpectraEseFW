#include <memory>
#include <iostream>

#include <TFile.h>
#include <TH1.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TLatex.h>
#include <TLine.h>

#include "JEUtility.hpp"
#include "rootStyle.hpp"


int main(int argc, char *argv[]) {

  if (argc < 5) {
    std::cerr << "Usage: " << argv[1] << " <q2 min <1,100>>" << std::endl;
    std::cerr << "Usage: " << argv[2] << " <q2 max <1,100>>" << std::endl;
    std::cerr << "Usage: " << argv[3] << " <Centrality min <1,100>>" << std::endl;
    std::cerr << "Usage: " << argv[4] << " <Centrality max <1,100>>" << std::endl;
    std::cerr << "Usage: " << argv[5] << " jet Radius values" << std::endl;
  }

  int q2min = std::atoi(argv[1]);
  int q2max = std::atoi(argv[2]);
  int centmin = std::atoi(argv[3]);
  int centmax = std::atoi(argv[4]);
  const char* Rval = argv[5];

  
  
  
  std::unique_ptr<TFile> f = std::unique_ptr<TFile>(new TFile(Form("processed_data/separatePtPlane_q2_%i_%i-centrality-%i-%i-R%s.root",q2min,q2max,centmin,centmax,Rval),"READ"));
  TH1F* h_in_inclusive_raw = dynamic_cast<TH1F*>(f->Get("hv_0"));
  TH1F* h_out_inclusive_raw = dynamic_cast<TH1F*>(f->Get("hv_1"));
  JEUtility *jet = new JEUtility(f.get(), 0.606452); /* R in ideal case 0.71 (run2) */
  auto hs = jet->YieldCorrectedJet();
  TH1* h_in_inclusive_Corr = hs.first;
  TH1* h_out_inclusive_Corr = hs.second;


  int n = 16;
  Double_t bin_edges[17]={0,5,10,15,20,25,30,40,50,60,70,80,90,100,120,160,200};
  TH1* h_in_inclusive = h_in_inclusive_raw->Rebin(n, "hInInclusiveReb", bin_edges);
  TH1* h_out_inclusive = h_out_inclusive_raw->Rebin(n, "hOutInclusiveReb", bin_edges);

  TH1* h_in_inclusive_corr_Rebin = h_in_inclusive_Corr->Rebin(n, "hInCorr_re", bin_edges);
  TH1* h_out_inclusive_corr_Rebin = h_out_inclusive_Corr->Rebin(n, "hOutCorr_re", bin_edges);


  rootStyle();

  std::vector<int> xAxis = {30,200};

  TCanvas c1("c1","",800,600);
  c1.cd();

  h_in_inclusive_corr_Rebin->GetYaxis()->SetTitle("#frac{1}{#it{N}_{ev}} #frac{d^{2}N}{d#it{p}_{T}d#it{#eta}}");
  h_in_inclusive_corr_Rebin->SetLineColor(kBlack);
  h_in_inclusive_corr_Rebin->GetXaxis()->SetRangeUser(xAxis.at(0),xAxis.at(1));
  h_in_inclusive_corr_Rebin->Draw("EPsame");

  h_out_inclusive_corr_Rebin->SetLineColor(kRed);
  h_out_inclusive_corr_Rebin->GetXaxis()->SetRangeUser(xAxis.at(0),xAxis.at(1));
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
  latex.DrawLatexNDC(.3, 0.8, "ALICE Pb--Pb #sqrt{s_{NN}} = 5.36 TeV");
  latex.DrawLatexNDC(.3,.75,Form("R=%s",Rval));


  c1.SaveAs(Form("JFigures/yield/pt-in-and-outofplane-yieldcorrected-cent-%i-%i-q2-%i-%i-R%s.pdf",centmin,centmax,q2min,q2max,Rval));

  TCanvas c1rat("c1rat","",400,200);
  TH1F *hRatio = (TH1F*)h_out_inclusive->Clone("hRatio");
  TH1F *hRatioCorr = (TH1F*)h_out_inclusive_corr_Rebin->Clone("hRatioCorr");
  hRatio->Divide(h_in_inclusive);  
  hRatio->SetLineColor(kBlack);
  hRatio->GetYaxis()->SetTitle("out-of-plane/in-plane");
  hRatio->GetXaxis()->SetRangeUser(xAxis.at(0),xAxis.at(1));
  hRatio->GetYaxis()->SetRangeUser(0.5,1.5);
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
  latex2.DrawLatexNDC(.3, 0.8, "ALICE Pb--Pb #sqrt{s_{NN}} = 5.36 TeV");
  latex2.DrawLatexNDC(.3,.75,Form("R=%s",Rval));

  c1rat.SaveAs(Form("JFigures/yield/pt-out-divided-in-cent-%i-%i-q2-%i-%i-R%s.pdf",centmin,centmax,q2min,q2max,Rval));

  TCanvas c2("c2","",800,600);
  c2.cd();

  TH1F* hRatCorr = (TH1F*)h_in_inclusive_corr_Rebin->Clone("hRatCorr");
  hRatCorr->Divide(h_in_inclusive);

  hRatCorr->GetYaxis()->SetTitle("corr/inclusive");
  hRatCorr->GetXaxis()->SetRangeUser(xAxis.at(0),xAxis.at(1));
  hRatCorr->GetYaxis()->SetRangeUser(0.5,1.5);
  hRatCorr->Draw();

  c2.SaveAs(Form("JFigures/yield/yieldRatio-cent-%i-%i-q2-%i-%i-R%s.pdf",centmin,centmax,q2min,q2max,Rval));


  return 0;
}