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

  auto h_in_inclusive_low = jet->getPt<Plane::IN, q2RangeType::LOW, Rebin::YES>();
  auto h_out_inclusive_low = jet->getPt<Plane::OUT, q2RangeType::LOW, Rebin::YES>();
  auto h_in_inclusive_high = jet->getPt<Plane::IN, q2RangeType::HIGH, Rebin::YES>();
  auto h_out_inclusive_high = jet->getPt<Plane::OUT, q2RangeType::HIGH, Rebin::YES>();
  auto h_in_inclusive = jet->getPt<Plane::IN, q2RangeType::INCLUSIVE, Rebin::YES>();
  auto h_out_inclusive = jet->getPt<Plane::OUT, q2RangeType::INCLUSIVE, Rebin::YES>();

  auto hRatio_low = jet->getPtRatio<q2RangeType::LOW, Rebin::YES>();
  auto hRatio_high = jet->getPtRatio<q2RangeType::HIGH, Rebin::YES>();
  auto hRatio_inclusive = jet->getPtRatio<q2RangeType::INCLUSIVE, Rebin::YES>();

  jet->setPtDrawSettings<>();

  rootStyle();
  std::vector<int> xAxis = {30,120};

  TCanvas c("c","",800,600);
  c.cd();

  h_in_inclusive_high->GetXaxis()->SetRangeUser(xAxis.at(0),xAxis.at(1));
  h_in_inclusive_high->GetYaxis()->SetRangeUser(1e-8,1e-3);
  h_in_inclusive_high->GetYaxis()->SetTitle("#frac{1}{#it{N}_{ev}} #frac{d^{2}N}{d#it{p}_{T}d#it{#eta}}");

  h_in_inclusive_high->Draw("EP");
  h_out_inclusive_high->Draw("EP same");
  h_in_inclusive_low->Draw("EP same");
  h_out_inclusive_low->Draw("EP same");


  TLegend* leg = new TLegend(  0.2,  0.2,  0.4, 0.4);

  leg->AddEntry(h_in_inclusive_high,"in-plane (high-#it{q}_{2})","lep");
  leg->AddEntry(h_in_inclusive_low,"in-plane (low-#it{q}_{2})","lep");
  leg->AddEntry(h_out_inclusive_low,"out-of-plane (low-#it{q}_{2})","lep");
  leg->AddEntry(h_out_inclusive_high,"out-of-plane (high-#it{q}_{2})","lep");
  leg->Draw();


  gPad->SetLogy();

  TLatex latex;
  latex.SetTextSize(0.04);  
  latex.DrawLatexNDC(.4, 0.8, "Pb--Pb #sqrt{s_{NN}} = 5.36 TeV");
  latex.DrawLatexNDC(.4,.75,Form("Charged jets, anti-#it{k}_{T}, R=%s","0.2"));
  latex.DrawLatexNDC(.4,.7,Form("#it{p}_{T}^{lead track}>5 GeV/#it{c}, |#eta_{jet}|<%.1f",0.9-0.2));
  // latex.DrawLatexNDC(.4,.65,"#it{q}_{2}^{FT0C}");
    
  c.SaveAs(Form("JFigures/yield/pt-in-and-outofplane-combined-yieldcorrected-cent-%i-%i-q2-R%s.pdf",30,50,"0.2"));


  TCanvas c2("c2","",400,200);
  c2.cd();

  hRatio_high->GetYaxis()->SetTitle("out-of-plane/in-plane");
  hRatio_high->GetXaxis()->SetRangeUser(xAxis.at(0),xAxis.at(1));
  hRatio_high->GetYaxis()->SetRangeUser(0,1.4);
  
  hRatio_high->Draw("EP");
  hRatio_inclusive->Draw("EPsame");
  hRatio_low->Draw("EPsame");

  TLine *line2 = new TLine(xAxis.at(0), 1, xAxis.at(1), 1);
  line2->SetLineStyle(2);
  line2->Draw();

  auto leg2 = new TLegend(0.65, 0.2, 0.85, 0.4);
  leg2->AddEntry(hRatio_high,"high-#it{q}_{2}","lep");
  leg2->AddEntry(hRatio_inclusive,"inclusive","lep");
  leg2->AddEntry(hRatio_low,"low-#it{q}_{2}","lep");
  leg2->Draw();

  TLatex latex2;
  latex2.SetTextSize(0.04);  
  latex2.DrawLatexNDC(.2, 0.85, "Pb--Pb #sqrt{s_{NN}} = 5.36 TeV");
  latex2.DrawLatexNDC(.2,.8,Form("Charged jets, anti-#it{k}_{T}, R=%s","0.2"));
  latex2.DrawLatexNDC(.2,.75,Form("#it{p}_{T}^{lead track}>5 GeV/#it{c}, |#eta_{jet}|<%.1f",0.9-0.2));

  c2.SaveAs(Form("JFigures/yield/pt-in-and-outofplane-combined-ratio-cent-%i-%i-q2-R%s.pdf",30,50,"0.2"));


}
