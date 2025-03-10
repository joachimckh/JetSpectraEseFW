
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
  auto jetVZ9 = std::unique_ptr<JEWrapper>(new JEWrapper(argv[5], argv[6])); // sub VZ9



  auto hPtDefault = jetDefault->getPt<Rebin::YES>();
  auto hPtLead = jetLead->getPt<Rebin::YES>();
  auto hPtVZ9 = jetVZ9->getPt<Rebin::YES>();

  jetDefault->setPtDrawSettings<>();
  jetLead->setPtDrawSettings<1>();
  jetVZ9->setPtDrawSettings<2>();


  auto jesyLead = std::unique_ptr<JESys>(new JESys(hPtDefault, hPtLead));
  auto ratioLead = jesyLead->getRatio();
  jesyLead->fitRatio();
  auto barlowLead = jesyLead->getBarlow();

  auto jesyVZ9 = std::unique_ptr<JESys>(new JESys(hPtDefault, hPtVZ9));
  auto ratioVZ9 = jesyVZ9->getRatio();
  jesyVZ9->fitRatio();
  auto barlowVZ9 = jesyVZ9->getBarlow();

  auto canv = new TCanvas("can","can",800,600);
  gPad->SetLogy();

  hPtDefault->Draw("EP");
  hPtLead->Draw("EPsame");
  hPtVZ9->Draw("EPsame");
  auto leg = new TLegend(0.6, 0.5, 0.8, .7);
  leg->AddEntry(hPtDefault, "Default", "lep");
  leg->AddEntry(hPtLead, "#it{p}_{T}^{lead}>6 GeV", "lep");
  leg->AddEntry(hPtVZ9, "|V_{Z}|<9 cm", "lep");
  leg->Draw();

  canv->SaveAs("JFigures/systematics/eventProp.pdf");

  auto canv_ratio = new TCanvas("canvEvent_ratio","canvEvent_ratio",800,600);
  
  ratioLead->GetYaxis()->SetTitle("Ratio");
  ratioLead->Draw("EP");
  ratioVZ9->Draw("EPsame");

  canv_ratio->SaveAs("JFigures/systematics/eventProp_ratio.pdf");


  auto canv_barlow = new TCanvas("canv_barlow","canv_barlow",800,600);
  barlowLead->GetYaxis()->SetTitle("Barlow");
  barlowLead->Draw("hist");
  barlowLead->GetYaxis()->SetRangeUser(0,10);
  barlowVZ9->Draw("histsame");

  canv_barlow->SaveAs("JFigures/systematics/eventProp_barlow.pdf");



}

