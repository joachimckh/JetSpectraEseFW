#include <iostream>

#include <TFile.h>
#include <TH1.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TLatex.h>
#include <TLine.h>
#include <TVectorD.h>

#include "JEUtility.hpp"
#include "JEFW.hpp"
#include "rootStyle.hpp"


int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <run number> <q2 min> <q2 max> <cent min> <cent max> <jet radius>" << std::endl;
  }
  const char* hlRun{argv[1]};
  const char* rSize{argv[2]};
  const char* rVal{argv[3]};
  double fRval = std::atof(rVal);


  rootStyle();

  
  TFile* f = new TFile(Form("/Users/joachimcarlokristianhansen/jet_analysis/hyperloop_data/LHC23zzh_pass4_%s/jet/%s/AnalysisResults.root",rSize,hlRun),"READ");
  TDirectory *dir = dynamic_cast<TDirectory*>(f->Get("jet-spectra-ese-task"));

  auto h = dynamic_cast<THnSparse*>(dir->Get("hCentJetPtdPhiq2")); /* Centrality, pT, dPhi, q2 */
  h->GetAxis(0)->SetRange(30,50); /* centr: 30-50% */
  
  auto h_phi_q2 = h->Projection(3,2); /* dPhi, q2 */
  auto h_pt_q2 = h->Projection(3,1); /* pT, q2 */


  TH1F* hEvs = dynamic_cast<TH1F*>(dir->Get("hEventCounter"));
  double nEvents{hEvs->GetBinContent(7)}; /* 30-50 centr */

  TCanvas c1("c1","",800,600);
  c1.cd();

  

  h_phi_q2->GetYaxis()->SetRange(0,10);
  auto h_phi_q2_proj1 = h_phi_q2->ProjectionX("hPhiQ2Proj_low");
  h_phi_q2->GetYaxis()->SetRange(90,100);
  auto h_phi_q2_proj2 = h_phi_q2->ProjectionX("hPhiQ2Proj_high");
  

  h_phi_q2_proj1->SetLineColor(kRed+2);
  h_phi_q2_proj2->SetLineColor(kOrange+1);

  h_phi_q2_proj2->GetYaxis()->SetRangeUser(0,350*10e3);
  h_phi_q2_proj2->GetXaxis()->SetTitleSize(0.05);
  h_phi_q2_proj2->GetYaxis()->SetTitleSize(0.05);
  h_phi_q2_proj2->GetYaxis()->SetTitle("entries");;
  h_phi_q2_proj2->Draw("hist");
  h_phi_q2_proj1->Draw("hist same");
  

  auto leg = new TLegend(0.35,0.18,0.55,0.38);
  leg->AddEntry(h_phi_q2_proj2,"10% highest-#it{q}_{2}","l");
  leg->AddEntry(h_phi_q2_proj1,"10% lowest-#it{q}_{2}","l");
  leg->Draw();

  TLatex latex;
  latex.SetTextSize(0.04);  
  latex.DrawLatexNDC(.5, 0.85, "Pb--Pb #sqrt{s_{NN}} = 5.36 TeV");
  latex.DrawLatexNDC(.5,.8,Form("Charged jets, anti-#it{k}_{T}, R=%s",rVal));
  latex.DrawLatexNDC(.5,.75,Form("#it{p}_{T}^{lead track}>5 GeV/#it{c}, |#eta_{jet}|<%.1f",0.9-fRval));

  c1.SaveAs(Form("JFigures/various/phi_q2_%s.pdf",rVal));

  TCanvas c2("c2","",800,600);
  c2.cd();
  gPad->SetLogy();
  
  const int n = 19;
  Double_t bin_edges[n+1]={0,5,10,15,20,25,30,35,40,45,50,55,60,70,80,90,100,120,160,200};

  auto h_pt_inclusive = h_pt_q2->ProjectionX("hPtInclusive");
  h_pt_q2->GetYaxis()->SetRange(0,10);
  auto h_pt_q2_proj1 = h_pt_q2->ProjectionX("hPtQ2Proj_low");
  auto h_pt_q2_proj1_r = h_pt_q2_proj1->Rebin(n, "h_pt_q2_proj1_r", bin_edges);
  h_pt_q2->GetYaxis()->SetRange(90,100);
  auto h_pt_q2_proj2 = h_pt_q2->ProjectionX("hPtQ2Proj_high");
  auto h_pt_q2_proj2_r = h_pt_q2_proj2->Rebin(n, "h_pt_q2_proj2_r", bin_edges);

  h_pt_q2_proj1_r->Scale(1 / nEvents, "width");
  h_pt_q2_proj2_r->Scale(1 / nEvents, "width");

  h_pt_q2_proj1_r->SetLineColor(kRed+2);
  h_pt_q2_proj2_r->SetLineColor(kOrange+1);

  h_pt_q2_proj1_r->GetXaxis()->SetTitleSize(0.05);
  h_pt_q2_proj1_r->GetYaxis()->SetTitleSize(0.05);
  h_pt_q2_proj1_r->GetYaxis()->SetTitle("#frac{1}{#it{N}_{ev}} #frac{d^{2}N}{d#it{p}_{T}d#it{#eta}}");
  h_pt_q2_proj1_r->Draw("");
  h_pt_q2_proj2_r->Draw("same");
  
  

  auto leg2 = new TLegend(0.65,0.35,0.85,0.55);
  leg2->AddEntry(h_pt_q2_proj2_r,"10% highest-#it{q}_{2}","l");
  leg2->AddEntry(h_pt_q2_proj1_r,"10% lowest-#it{q}_{2}","l");
  leg2->Draw();

  TLatex latex2;
  latex2.SetTextSize(0.04);  
  latex2.DrawLatexNDC(.5, 0.85, "Pb--Pb #sqrt{s_{NN}} = 5.36 TeV");
  latex2.DrawLatexNDC(.5,.8,Form("Charged jets, anti-#it{k}_{T}, R=%s",rVal));
  latex2.DrawLatexNDC(.5,.75,Form("#it{p}_{T}^{lead track}>5 GeV/#it{c}, |#eta_{jet}|<%.1f",0.9-fRval));

  c2.SaveAs(Form("JFigures/various/pt_q2_%s.pdf",rVal));
  


  return 0;
}
