
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
  
  auto jetDefault = std::unique_ptr<JEWrapper>(new JEWrapper(argv[1], argv[2])); // default FT0A
  auto jetFT0C = std::unique_ptr<JEWrapper>(new JEWrapper(argv[3], argv[4])); // subtask FT0C EP resolution systematic
  jetFT0C->setInclusiveR2(0.686);


  // skal gøres for in/out men ikke q2
  // ->getPt<Plane::OUT, q2RangeType::INCLUSIVE, Rebin::YES>();

  auto histDefault = jetDefault->getPt<Plane::OUT, q2RangeType::INCLUSIVE, Rebin::YES>();
  auto histFT0C = jetFT0C->getPt<Plane::OUT, q2RangeType::INCLUSIVE, Rebin::YES>();

  

  jetDefault->setPtDrawSettings<>();
  jetFT0C->setPtDrawSettings<1>();


  auto jesy = std::unique_ptr<JESys>(new JESys(histDefault, histFT0C));
  auto ratio = jesy->getRatio();
  jesy->fitRatio();
  auto barlow = jesy->getBarlow();

  auto canvFT0C = new TCanvas("canvFT0C","canvFT0C",800,600);
  // gPad->SetLogy();

  histFT0C->Draw("EP");
  // histDefault->Draw("EPsame");
  
  // auto leg = new TLegend(0.6, 0.5, 0.8, .7);
  // leg->AddEntry(histDefault, "Default", "lep");
  // leg->AddEntry(histFT0C, "FT0C", "lep");
  // leg->Draw();

  canvFT0C->SaveAs("JFigures/systematics/FT0C.pdf");

  auto canvFT0C_ratio = new TCanvas("canvFT0C_ratio","canvFT0C_ratio",800,600);
  
  ratio->GetYaxis()->SetTitle("Ratio");
  ratio->Draw("EP");

  canvFT0C_ratio->SaveAs("JFigures/systematics/FT0C_ratio.pdf");


  auto canvFT0C_barlow = new TCanvas("canvFT0C_barlow","canvFT0C_barlow",800,600);
  barlow->GetXaxis()->SetRangeUser(30,120);
  barlow->GetYaxis()->SetTitle("Barlow");
  barlow->Draw("EP");

  canvFT0C_barlow->SaveAs("JFigures/systematics/FT0C_barlow.pdf");



}

