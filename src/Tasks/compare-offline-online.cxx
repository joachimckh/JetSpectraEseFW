#include <iostream>
#include <vector>
#include <string>
#include <memory>

#include <TFile.h>
#include <THnSparse.h>
#include <TH1.h>
#include <TCanvas.h>
#include <TLegend.h>

#include "../JAnalysis/rootStyle.hpp"

#include "JEFW.hpp"

void analyzeRhoPhi(std::vector<const char*> const& paths, std::vector<int> const& colors, std::vector<std::string> const& labels);

int main(int argc, char* argv[]) {  
  rootStyle();

  std::vector<int> cent = {30,50};
  auto jet = new JEFW("/Users/joachimcarlokristianhansen/jet_analysis/localr/jetSpectraESE/OFFLINE_IN_OUT_vs_ONLINE_IN_OUT/AnalysisResults.root", JEFW::DataType::DATA, "");
  jet->setCentrality(cent);
  // jet->setq2Range(q2);
  TObjArray* h = jet->separatePlanes(0);
  double scale = jet->eventCounter(7);

  auto hPtIn = dynamic_cast<TH1F*>(h->FindObject("hv_0"));
  auto hPtOut = dynamic_cast<TH1F*>(h->FindObject("hv_1"));
  auto dir = jet->getDir();
  auto hTrueIn = dynamic_cast<THnSparse*>(dir->Get("hCentJetPtq2In"));
  auto hTrueOut = dynamic_cast<THnSparse*>(dir->Get("hCentJetPtq2Out"));
  hTrueIn->GetAxis(0)->SetRange(hTrueIn->GetAxis(0)->FindBin(30), hTrueIn->GetAxis(0)->FindBin(50));
  hTrueOut->GetAxis(0)->SetRange(hTrueOut->GetAxis(0)->FindBin(30), hTrueOut->GetAxis(0)->FindBin(50));
  auto hPtTrueIn = dynamic_cast<TH1D*>(hTrueIn->Projection(1));
  auto hPtTrueOut = dynamic_cast<TH1D*>(hTrueOut->Projection(1));



  auto canvas = new TCanvas("canvas", "", 1400, 600);
  canvas->Divide(2,1);
  canvas->cd(1);

  hPtIn->SetMarkerStyle(20);
  hPtIn->SetMarkerColor(kOrange - 1);
  hPtIn->SetLineColor(kOrange - 1);

  hPtOut->SetMarkerStyle(20);
  hPtOut->SetMarkerColor(kCyan + 2);
  hPtOut->SetLineColor(kCyan + 2);

  hPtTrueIn->SetMarkerStyle(24);
  hPtTrueIn->SetMarkerColor(kBlack);
  hPtTrueIn->SetLineColor(kBlack);

  hPtTrueOut->SetMarkerStyle(24);
  hPtTrueOut->SetMarkerColor(kBlack);
  hPtTrueOut->SetLineColor(kBlack);

  hPtIn->Draw("EP");
  hPtTrueIn->Draw("EPsame");

  canvas->cd(2);
  hPtOut->Draw("EP");
  hPtTrueOut->Draw("EPsame");


  canvas->SaveAs("JFigures/comparisons/compare_spectra_in_outOFFLINEONLINE.pdf");

  auto canvas2 = new TCanvas("canvas2", "", 1400, 600);
  canvas2->Divide(2,1);
  canvas2->cd(1);

  auto hRatIn = dynamic_cast<TH1F*>(hPtIn->Clone("hRatIn"));
  hRatIn->Divide(hPtIn, hPtTrueIn, 1, 1,"B");

  hRatIn->SetMarkerStyle(20);
  hRatIn->SetMarkerColor(kBlack);
  hRatIn->SetLineColor(kBlack);

  hRatIn->GetYaxis()->SetRangeUser(0.5, 1.5);

  hRatIn->Draw("EP");

  canvas2->cd(2);

  auto hRatOut = dynamic_cast<TH1F*>(hPtOut->Clone("hRatOut"));
  hRatOut->Divide(hPtOut, hPtTrueOut, 1, 1, "B");

  hRatOut->SetMarkerStyle(20);
  hRatOut->SetMarkerColor(kBlack);
  hRatOut->SetLineColor(kBlack);

  hRatOut->GetYaxis()->SetRangeUser(0.5, 1.5);

  hRatOut->Draw("EP");

  canvas2->SaveAs("JFigures/comparisons/compare_spectra_in_out_ratioOFFLINEONLINE.pdf");
}