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
#include "JEWrapper.hpp"

#include "rootStyle.hpp"


int main(int argc, char *argv[]) {


  const char* pathFull = argv[1];
  auto jet = std::unique_ptr<JEWrapper>(new JEWrapper(pathFull));

  auto jet2 = std::unique_ptr<JEWrapper>(new JEWrapper(pathFull));

  // auto h_in_inclusive_low = jet->getPt<Plane::IN, q2RangeType::LOW, Rebin::YES>();
  // auto h_out_inclusive_low = jet->getPt<Plane::OUT, q2RangeType::LOW, Rebin::YES>();
  // auto h_in_inclusive_high = jet->getPt<Plane::IN, q2RangeType::HIGH, Rebin::YES>();
  // auto h_out_inclusive_high = jet->getPt<Plane::OUT, q2RangeType::HIGH, Rebin::YES>();
  // auto h_in_inclusive = jet->getPt<Plane::IN, q2RangeType::INCLUSIVE, Rebin::YES>();
  // auto h_out_inclusive = jet->getPt<Plane::OUT, q2RangeType::INCLUSIVE, Rebin::YES>();
  
  
  // std::unique_ptr<TFile> f = std::unique_ptr<TFile>(new TFile(Form("processed_data/raw/separatePtPlane_q2_%i_%i-centrality-%i-%i-R%s.root",q2min,q2max,centmin,centmax,Rval),"READ"));
  // TH1F* h_in_inclusive_raw = dynamic_cast<TH1F*>(f->Get("hv_0"));
  // TH1F* h_out_inclusive_raw = dynamic_cast<TH1F*>(f->Get("hv_1"));
  // TVectorD* tmp = dynamic_cast<TVectorD*>(f->Get("nEvents"));
  // double nEvents = static_cast<double>((*tmp)[0]);
  // std::cout << "nEvents: " << nEvents << std::endl;
  
  
  // JEUtility *jet = new JEUtility(f.get(), reso); /* R in ideal case 0.71 (run2) */
  // auto hs = jet->YieldCorrectedJet();
  // TH1* h_in_inclusive_corr = hs.first;
  // TH1* h_out_inclusive_corr = hs.second;


  // const int n = 19;
  // Double_t bin_edges[n+1]={0,5,10,15,20,25,30,35,40,45,50,55,60,70,80,90,100,120,160,200};
  // TH1* h_in_inclusive = h_in_inclusive_raw->Rebin(n, "hInInclusiveReb", bin_edges);
  // TH1* h_out_inclusive = h_out_inclusive_raw->Rebin(n, "hOutInclusiveReb", bin_edges);

  // TH1* h_in_inclusive_corr_Rebin = h_in_inclusive_corr->Rebin(n, "hInCorr_re", bin_edges);
  // TH1* h_out_inclusive_corr_Rebin = h_out_inclusive_corr->Rebin(n, "hOutCorr_re", bin_edges);

  // h_in_inclusive->Scale(1 / nEvents, "width");
  // h_out_inclusive->Scale(1 / nEvents, "width");
  // h_in_inclusive_raw->Scale(1 / nEvents, "width");
  // h_out_inclusive_raw->Scale(1 / nEvents, "width");
  // h_in_inclusive_corr_Rebin->Scale(1 / nEvents, "width");
  // h_out_inclusive_corr_Rebin->Scale(1 / nEvents, "width");
  // h_in_inclusive->Scale(1 / h_in_inclusive->Integral());
  // h_out_inclusive->Scale(1 / h_out_inclusive->Integral());
  // h_in_inclusive_raw->Scale(1 / h_in_inclusive_raw->Integral());
  // h_out_inclusive_raw->Scale(1 / h_out_inclusive_raw->Integral());
  // h_in_inclusive_corr_Rebin->Scale(1 / h_in_inclusive_corr_Rebin->Integral());
  // h_out_inclusive_corr_Rebin->Scale(1 / h_out_inclusive_corr_Rebin->Integral());
  

  rootStyle();

  std::vector<int> xAxis = {30,160};


  // TCanvas c1rat("c1rat","",400,200);
  // TH1F *hRatio = (TH1F*)h_out_inclusive->Clone("hRatio");
  // TH1F *hRatioCorr = (TH1F*)h_out_inclusive_corr_Rebin->Clone("hRatioCorr");
  // hRatio->Divide(h_in_inclusive);  
  // hRatio->SetLineColor(kBlack);
  // hRatio->GetYaxis()->SetTitle("out-of-plane/in-plane");
  // hRatio->GetXaxis()->SetRangeUser(xAxis.at(0),xAxis.at(1));
  // hRatio->GetYaxis()->SetRangeUser(0,1.4);

  // hRatio->SetMarkerStyle(20);
  // hRatio->SetMarkerColor(kBlack);
  // hRatio->Draw();

  // hRatioCorr->Divide(h_in_inclusive_corr_Rebin);
  // hRatioCorr->SetLineColor(kRed+1);
  // hRatioCorr->SetMarkerStyle(21);
  // hRatioCorr->SetMarkerColor(kRed+1);
  // hRatioCorr->Draw("same");

  // TLine *line = new TLine(xAxis.at(0), 1, xAxis.at(1), 1);
  // line->SetLineStyle(2);
  // line->Draw();

  // auto leg2 = new TLegend(0.65, 0.2, 0.86, 0.45);
  // leg2->AddEntry(hRatio,"Before #it{R}_{2} correction","lep");
  // leg2->AddEntry(hRatioCorr,"After #it{R}_{2} correction","lep");
  // leg2->Draw();

  // TLatex latex2;
  // latex2.SetTextSize(0.04);  
  // latex2.DrawLatexNDC(.2,.35, "Pb--Pb #sqrt{s_{NN}} = 5.36 TeV");
  // latex2.DrawLatexNDC(.2,.3,Form("Charged jets, anti-#it{k}_{T}, R=%s",Rval));
  // latex2.DrawLatexNDC(.2,.25,Form("#it{p}_{T}^{lead track}>5 GeV/#it{c}, |#eta_{jet}|<%.1f",0.9-fRval));
  // latex2.DrawLatexNDC(.2,.2,Form("%i-%i%%-#it{q}_{2}^{FT0C}",q2min,q2max));

  // c1rat.SaveAs(Form("JFigures/yield/pt-out-divided-in-cent-%i-%i-q2-%i-%i-R%s.pdf",centmin,centmax,q2min,q2max,Rval));

  TCanvas c2("c2","",400,200);
  c2.cd();

  // auto hRatio_low = jet->getPtRatio<q2RangeType::LOW, Rebin::YES>();
  // auto hRatio_high = jet->getPtRatio<q2RangeType::HIGH, Rebin::YES>();
  auto hRatCorr = jet->getPtRatio<q2RangeType::INCLUSIVE, Rebin::YES>();
  jet->setPtDrawSettings<>();
  auto hRatUncorr = jet2->getPtRatio<q2RangeType::INCLUSIVE, Rebin::YES, false>();
  jet->setPtDrawSettings<1>();

  // auto hRatio_low_unCorr = jet->getPtRatio<q2RangeType::LOW, Rebin::YES, false>();
  // auto hRatio_high_unCorr = jet->getPtRatio<q2RangeType::HIGH, Rebin::YES, false>();
  

  
  
  hRatCorr->GetYaxis()->SetTitle("corrected (after/before)");
  hRatCorr->GetXaxis()->SetRangeUser(xAxis.at(0),xAxis.at(1));
  hRatCorr->GetYaxis()->SetRangeUser(0.3,1.2);
  hRatCorr->SetMarkerStyle(20);
  hRatCorr->SetMarkerColor(kRed+2);
  hRatCorr->SetLineColor(kRed+2);
  hRatCorr->Draw("EP");

  hRatUncorr->SetLineColor(kBlack);
  hRatUncorr->SetMarkerStyle(20);
  hRatUncorr->SetMarkerColor(kBlack);
  hRatUncorr->Draw("EPsame");

  TLine *line2 = new TLine(xAxis.at(0), 1, xAxis.at(1), 1);
  line2->SetLineStyle(2);
  line2->Draw();

  auto leg2 = new TLegend(0.65, 0.2, 0.86, 0.45);
  leg2->AddEntry(hRatUncorr,"Before #it{R}_{2} correction","lep");
  leg2->AddEntry(hRatCorr,"After #it{R}_{2} correction","lep");
  
  leg2->Draw();

  TLatex latex3;
  latex3.SetTextSize(0.04);  
  latex3.DrawLatexNDC(.2, .3, "Pb--Pb #sqrt{s_{NN}} = 5.36 TeV");
  latex3.DrawLatexNDC(.2,.25,Form("Charged jets, anti-#it{k}_{T}, R=%s","0.2"));
  latex3.DrawLatexNDC(.2,.2,Form("#it{p}_{T}^{lead track}>5 GeV/#it{c}, |#eta_{jet}|<%.1f",0.9-0.2));

  c2.SaveAs(Form("JFigures/yield/yieldRatio-cent-%i-%i-q2-%i-%i-R%s.pdf",30,50,0,100,"0.2"));


  return 0;
}