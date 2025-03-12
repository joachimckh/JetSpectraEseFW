
#include <memory>
#include <vector>
#include <iostream>

#include <TH1.h>
#include <TFile.h>
#include <TVectorD.h>
#include <TLegend.h>
#include <TCanvas.h>

#include "JEWrapper.hpp"
#include "JESys.hpp"


#include "../JAnalysis/rootStyle.hpp"

int main(int argc, char* argv[]) {
  rootStyle();
  
  auto jetDefault = std::unique_ptr<JEWrapper>(new JEWrapper(argv[1], argv[2])); // default  
  auto jetLead = std::unique_ptr<JEWrapper>(new JEWrapper(argv[3], argv[4])); // sub Lead track 6 GeV
  auto jetVZ = std::unique_ptr<JEWrapper>(new JEWrapper(argv[5], argv[6])); // sub VZ6

  // ->getPt<Plane::IN, q2RangeType::INCLUSIVE, Rebin::YES>();
  auto histDefault = jetDefault->getPt<Plane::OUT, q2RangeType::INCLUSIVE, Rebin::YES>();
  auto hLeadTrack = jetLead->getPt<Plane::OUT, q2RangeType::INCLUSIVE, Rebin::YES>();
  auto hVtxZ = jetVZ->getPt<Plane::OUT, q2RangeType::INCLUSIVE, Rebin::YES>();

  jetDefault->setPtDrawSettings<>();
  jetLead->setPtDrawSettings<1>();
  jetVZ->setPtDrawSettings<2>();


  auto jesyLead = std::unique_ptr<JESys>(new JESys(histDefault, hLeadTrack));
  auto ratioLead = jesyLead->getRatio();
  jesyLead->fitRatio();
  auto barlowLead = jesyLead->getBarlow();
  jesyLead->setDrawSettings<>();

  auto jesyVZ = std::unique_ptr<JESys>(new JESys(histDefault, hVtxZ));
  auto ratioVZ = jesyVZ->getRatio();
  jesyVZ->fitRatio();
  auto barlowVZ = jesyVZ->getBarlow();
  jesyVZ->setDrawSettings<1>();

  auto canv = new TCanvas("can","can",800,600);
  gPad->SetLogy();

  histDefault->GetXaxis()->SetRangeUser(30,120);
  histDefault->Draw("EP");
  hLeadTrack->Draw("EPsame");
  hVtxZ->Draw("EPsame");
  auto leg = new TLegend(0.6, 0.5, 0.8, .7);
  leg->AddEntry(histDefault, "Default", "lep");
  leg->AddEntry(hLeadTrack, "#it{p}_{T}^{lead}>6 GeV", "lep");
  leg->AddEntry(hVtxZ, "|V_{Z}|<7 cm", "lep");
  leg->Draw();

  canv->SaveAs("JFigures/systematics/eventProp.pdf");

  auto canv_ratio = new TCanvas("canvEvent_ratio","canvEvent_ratio",800,600);
  
  ratioLead->GetYaxis()->SetTitle("Ratio");
  ratioLead->GetXaxis()->SetRangeUser(30,120);
  ratioLead->Draw("EP");
  ratioVZ->Draw("EPsame");

  canv_ratio->SaveAs("JFigures/systematics/eventProp_ratio.pdf");


  auto canv_barlow = new TCanvas("canv_barlow","canv_barlow",800,600);
  barlowLead->GetYaxis()->SetTitle("Barlow Test");
  barlowLead->GetXaxis()->SetRangeUser(30,120);
  barlowLead->Draw("hist");
  barlowLead->GetYaxis()->SetRangeUser(0,10);
  barlowVZ->Draw("histsame");

  auto leg2 = new TLegend(0.6, 0.5, 0.8, .7);
  leg2->AddEntry(barlowLead, "#it{p}_{T}^{lead track} > 6 GeV/#it{c}", "l");
  leg2->AddEntry(barlowVZ, "|V_{Z}|<7 cm", "l");
  leg2->Draw();

  canv_barlow->SaveAs("JFigures/systematics/eventProp_barlow.pdf");



}

