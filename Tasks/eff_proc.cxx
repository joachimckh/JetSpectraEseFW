
#include "JEFW.hpp"
#include "JEfficiency.hpp"

#include <memory>
#include <iostream>

#include <TFile.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TLine.h>
#include <TLegend.h>

using std::unique_ptr;



int main(int argc, char *argv[]) {

  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <run number>" << std::endl;
  }
  const char* run = argv[1];
  unique_ptr<JEFW> mcjet{new JEFW(Form("/Users/joachimcarlokristianhansen/jet_analysis/hyperloop_data/LHC24g3/jet/%s/AnalysisResults.root",run))};
  mcjet->Init("mc");
  int n = 16;
  Double_t bin_edges[17]={0,5,10,15,20,25,30,40,50,60,70,80,90,100,120,160,200};
  mcjet->JERebin(n, bin_edges);
  auto hmatched = mcjet->GetHistPt(2);
  auto htruth = mcjet->GetHistPt(1);


  // divide mathced by truth to get efficiency
  hmatched->Divide(htruth);

  

  TCanvas c2("c2","",800,600);
  c2.cd();

  hmatched->SetTitle("");
  hmatched->GetYaxis()->SetTitle("Efficiency");
  hmatched->GetXaxis()->SetTitle("p_{T,jet}^{truth} (GeV/c)");

  hmatched->SetStats(0);
  hmatched->SetLineColor(kBlack);
  hmatched->Draw();

  hmatched->GetXaxis()->SetRangeUser(0, 200);
  hmatched->GetYaxis()->SetRangeUser(0, 1.1);

  // draw dashed line at 1
  TLine *line = new TLine(0, 1, 200, 1);
  line->SetLineStyle(2);
  line->Draw();

  // TLatex latex;
  // latex.SetTextSize(0.04);  
  // latex.DrawLatex(60, 0.3, "LHC24g3");
  auto leg = new TLegend(0.6,0.2,0.8,0.4);
  leg->AddEntry(hmatched,"LHC24g3","l");
  leg->Draw();

  c2.SaveAs("figures/efficiency.pdf");


  return 0;
}
