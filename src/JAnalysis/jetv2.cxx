
#include <memory>
#include <iostream>
#include <utility>

#include <TCanvas.h>
#include <TH1.h>
#include <TLegend.h>
#include <TFile.h>
#include <TLatex.h>
#include <TH3.h>
#include <TH2.h>

#include "rootStyle.hpp"


int main(int argc, char *argv[])
{   

  const char* Rval = argv[1];
  TFile *f = new TFile("/Users/joachimcarlokristianhansen/jet_analysis/localr/HEPData/HEPData-ins1394678-v1-root.root","READ");
  TH1 *h = (TH1*)f->Get("Table 2/Hist1D_y1");
  

  TFile *f2 = new TFile(Form("processed_data/v2Jet_R%s.root",Rval),"READ");
  TH1 *h2 = (TH1*)f2->Get("hV2Jet");


  rootStyle();
  TCanvas c1("c1","",800,600);
  c1.cd();

  h2->SetLineColor(kBlack);
  h2->GetXaxis()->SetRangeUser(20,90);
  h2->GetYaxis()->SetRangeUser(-0.1,0.24);
  // int n = 5;
  // Double_t bin_edges[6]={20,30,40,50,60,80};
  // TH1* h2Reb = h2->Rebin(n, "h2Reb", bin_edges);
  h2->GetYaxis()->SetTitle("#it{v}_{2,jet}");
  h2->Draw();

  h->SetLineColor(kRed);
  h->Draw("same");

  

  TLegend leg(0.2,0.2,0.4,0.4);
  leg.AddEntry(h,"HEPData","l");
  leg.AddEntry(h2,"This Analysis","l");
  leg.Draw();

  c1.SaveAs(Form("JFigures/comparisons/v2Jet_R%s.pdf",Rval));
}

