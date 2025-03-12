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

TH2* getCorrHistOI(TDirectory *d, const char* name) {
  TH3F* hTMP = dynamic_cast<TH3F*>(d->Get(name));
  // hTMP->GetXaxis()->SetRange(30,50);
  TH2D* hTMP2 = dynamic_cast<TH2D*>(hTMP->Project3D("yz"));
  hTMP2->SetStats(0);
  return hTMP2;
}

TH1* getEP(TDirectory *dir, const char* name) {
  auto hblau = dynamic_cast<TH2F*>(dir->Get(name));
  auto hblau2 = hblau->ProjectionY(Form("hblau2_%s",name),30,50);
  hblau2->Scale(1.0/hblau2->Integral());
  return hblau2;
}

int main(int argc, char *argv[]){

  if (argc < 2) {
    std::cerr << "Usage: " << argv[1] << " <run number> <A> <B> <C> <size>" << std::endl;
  }

  auto fin = Form("/Users/joachimcarlokristianhansen/jet_analysis/hyperloop_data/LHC23zzh_pass4_%s/jet/%s/AnalysisResults.root",argv[5], argv[1]);

  TFile *f = new TFile(fin,"READ");
  TDirectory *dir = dynamic_cast<TDirectory*>(f->Get("jet-spectra-ese-task"));

  // KEY: TH3F	hQvecUncorV2;1	
  // KEY: TH3F	hQvecRectrV2;1	
  // KEY: TH3F	hQvecTwistV2;1	
  // KEY: TH3F	hQvecFinalV2;1

  // KEY: TH2F	hEPUncorV2;1	//final is the EP A
  // KEY: TH2F	hEPRectrV2;1	
  // KEY: TH2F	hEPTwistV2;1

  rootStyle();

  TCanvas *c1 = new TCanvas("c1","c1",1400,1400);
  c1->Divide(2,2);

  c1->cd(1);
  auto h1{getCorrHistOI(dir,"hQvecUncorV2")};
  h1->GetXaxis()->SetRangeUser(-1.5,1.5);
  h1->GetYaxis()->SetRangeUser(-1.5,1.5);
  h1->Draw();

  c1->cd(2);
  auto h2{getCorrHistOI(dir,"hQvecRectrV2")};
  h2->GetXaxis()->SetRangeUser(-1.5,1.5);
  h2->GetYaxis()->SetRangeUser(-1.5,1.5);
  h2->Draw();

  c1->cd(3);
  auto h3{getCorrHistOI(dir,"hQvecTwistV2")};
  h3->GetXaxis()->SetRangeUser(-1.5,1.5);
  h3->GetYaxis()->SetRangeUser(-1.5,1.5);
  h3->Draw();

  c1->cd(4);
  auto h4{getCorrHistOI(dir,"hQvecFinalV2")};
  h4->Draw();
  // hReso->SetLineColor(kBlack);
  // hReso->SetMarkerStyle(20);
  // hReso->SetMarkerSize(.5);
  // hReso->SetMarkerColor(kBlack);
  // hReso->GetXaxis()->SetRangeUser(0,100);
  // hReso->GetYaxis()->SetRangeUser(0, 1);
  // hReso->Draw("EPhist");
  c1->SaveAs(Form("JFigures/eventplane/QVecResoAll.pdf"));

  TCanvas *c2 = new TCanvas("c2","c2",800,600);
  c2->cd();

  auto h01{getEP(dir,"hEPUncorV2")};
  auto h02{getEP(dir,"hEPRectrV2")};
  auto h03{getEP(dir,"hEPTwistV2")};
  auto h04{getEP(dir,"hPsi2FT0A")};

  h04->SetLineColor(kBlack);
  h04->SetMarkerStyle(20);
  h04->SetMarkerColor(kBlack);
  h04->SetMarkerSize(.5);
  h04->SetStats(0);
  h04->GetYaxis()->SetRangeUser(0, 0.015);
  h04->GetXaxis()->SetTitle("#Psi_{2}^{FT0A}");
  h04->GetYaxis()->SetTitle("Entries");
  h04->Draw("EP");
  // psiFT0A->GetYaxis()->SetRangeUser(0, 6*10e4);

  // h03->SetLineColor(kRed);
  // h03->SetMarkerStyle(27);
  // h03->SetMarkerColor(kRed);
  // h03->SetMarkerSize(.5);
  // h03->Draw("EPsame");

  // h02->SetLineColor(kBlue);
  // h02->SetMarkerStyle(24);
  // h02->SetMarkerColor(kBlue);
  // h02->SetMarkerSize(.5);
  // h02->Draw("EPsame");

  h01->SetLineColor(kRed);
  h01->SetMarkerStyle(25);
  h01->SetMarkerColor(kRed);
  h01->SetMarkerSize(.5);
  h01->Draw("EPsame");

  auto leg = new TLegend(0.2, 0.2, 0.4, 0.4);
  leg->AddEntry(h01, "Uncorrected", "lep");
  // leg->AddEntry(h02, "Recentered", "lep");
  // leg->AddEntry(h03, "Twisted", "lep");
  leg->AddEntry(h04, "Full", "lep");
  leg->Draw();

  TLatex latex2;
  latex2.SetTextSize(0.04);  
  latex2.DrawLatexNDC(.4, 0.3, "Pb--Pb #sqrt{s_{NN}} = 5.36 TeV");
  latex2.DrawLatexNDC(.4, 0.25, "30-50% centrality");

  c2->SaveAs("JFigures/eventplane/EPCorrSteps.pdf");

}
