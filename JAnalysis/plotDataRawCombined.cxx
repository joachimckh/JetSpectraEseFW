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
#include "rootStyle.hpp"

std::tuple<TH1*, TH1*> getHists(TFile* f, bool fRebin, float resolution) {

  TH1F* h_in_inclusive_raw = dynamic_cast<TH1F*>(f->Get("hv_0"));
  TH1F* h_out_inclusive_raw = dynamic_cast<TH1F*>(f->Get("hv_1"));
  TVectorD* tmp = dynamic_cast<TVectorD*>(f->Get("nEvents"));
  double nEvents = static_cast<double>((*tmp)[0]);
  std::cout << "nEvents: " << nEvents << std::endl;
  JEUtility *jet = new JEUtility(f, 0.606452); /* R in ideal case 0.71 (run2) */
  auto hs = jet->YieldCorrectedJet();
  TH1* h_in_inclusive_corr = hs.first;
  TH1* h_out_inclusive_corr = hs.second;


  const int n = 19;
  Double_t bin_edges[n+1]={0,5,10,15,20,25,30,35,40,45,50,55,60,70,80,90,100,120,160,200};
  
  auto h_in_inclusive_raw_reb = h_in_inclusive_raw->Rebin(n, "hInInclusiveReb", bin_edges);
  auto h_out_inclusive_raw_reb = h_out_inclusive_raw->Rebin(n, "hOutInclusiveReb", bin_edges);
  auto h_in_inclusive_corr_reb = h_in_inclusive_corr->Rebin(n, "hInCorr_re", bin_edges);
  auto h_out_inclusive_corr_reb = h_out_inclusive_corr->Rebin(n, "hOutCorr_re", bin_edges);

  h_in_inclusive_corr->Scale(1 / nEvents, "width");
  h_out_inclusive_corr->Scale(1 / nEvents, "width");
  h_in_inclusive_corr_reb->Scale(1 / nEvents, "width");
  h_out_inclusive_corr_reb->Scale(1 / nEvents, "width");
  
  if (fRebin) {
    return std::make_tuple(h_in_inclusive_corr_reb, h_out_inclusive_corr_reb);
  } else {
    return std::make_tuple(h_in_inclusive_corr, h_out_inclusive_corr);
  } 
}


int main(int argc, char *argv[]) {

  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << "<jet radius>" << std::endl;
  }

  const char* Rval = argv[1];
  float fRval = std::atof(Rval);
  constexpr bool fRebin = true;
  
  
  TFile* flow = new TFile(Form("processed_data/raw/separatePtPlane_q2_%i_%i-centrality-%i-%i-R%s.root",0,30,30,50,Rval),"READ");
  TFile* fhigh = new TFile(Form("processed_data/raw/separatePtPlane_q2_%i_%i-centrality-%i-%i-R%s.root",70,100,30,50,Rval),"READ");

  auto [h_in_inclusive_low, h_out_inclusive_low] = getHists(flow, fRebin, 0.54);
  auto [h_in_inclusive_high, h_out_inclusive_high] = getHists(fhigh, fRebin, 0.68);

  if (!h_in_inclusive_low || !h_out_inclusive_low || !h_in_inclusive_high || !h_out_inclusive_high) {
    std::cerr << "Something went wrong getting histograms" << std::endl;
    return 1;
  }

  h_out_inclusive_high->SetLineColor(kOrange+1);
  h_in_inclusive_low->SetLineColor(kViolet);
  h_out_inclusive_low->SetLineColor(kBlue);
  h_in_inclusive_high->SetLineColor(kCyan);

  rootStyle();
  std::vector<int> xAxis = {30,160};

  TCanvas c("c","",800,600);
  c.cd();

  h_in_inclusive_high->GetXaxis()->SetRangeUser(xAxis.at(0),xAxis.at(1));
  h_in_inclusive_high->GetYaxis()->SetRangeUser(1e-8,1);
  h_in_inclusive_high->GetYaxis()->SetTitle("#frac{1}{#it{N}_{ev}} #frac{d^{2}N}{d#it{p}_{T}d#it{#eta}}");
  h_in_inclusive_high->Draw("EP");

  h_in_inclusive_low->Draw("EP same");
  h_out_inclusive_low->Draw("EP same");
  h_out_inclusive_high->Draw("EP same");
  


  

  gPad->SetLogy();

  TLegend* leg = new TLegend(  0.6,  0.4,  0.8, 0.6);
  leg->AddEntry(h_in_inclusive_high,"in-plane (high-#it{q}_{2})","l");
  leg->AddEntry(h_in_inclusive_low,"in-plane (low-#it{q}_{2})","l");
  leg->AddEntry(h_out_inclusive_low,"out-of-plane (low-#it{q}_{2})","l");
  leg->AddEntry(h_out_inclusive_high,"out-of-plane (high-#it{q}_{2})","l");
  leg->Draw();

  TLatex latex;
  latex.SetTextSize(0.04);  
  latex.DrawLatexNDC(.4, 0.8, "Pb--Pb #sqrt{s_{NN}} = 5.36 TeV");
  latex.DrawLatexNDC(.4,.75,Form("Charged jets, anti-#it{k}_{T}, R=%s",Rval));
  latex.DrawLatexNDC(.4,.7,Form("#it{p}_{T}^{lead track}>5 GeV/#it{c}, |#eta_{jet}|<%.1f",0.9-fRval));
  // latex.DrawLatexNDC(.4,.65,"#it{q}_{2}^{FT0C}");
    
  c.SaveAs(Form("JFigures/yield/pt-in-and-outofplane-combined-yieldcorrected-cent-%i-%i-q2-R%s.pdf",30,50,Rval));


  return 0;
}
