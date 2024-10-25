
#include "Core/JEFW.h"
#include "Core/JEfficiency.h"

#include <memory>


using std::unique_ptr;

void procc_mc(){

  // unique_ptr<JEFW> jet{new JEFW("../../jetSpectraESE/AnalysisResults.root")};
  unique_ptr<JEFW> jet{new JEFW("/Users/joachimcarlokristianhansen/jet_analysis/localr/loca_merge/train_278523_sub_jet/AnalysisResults.root")};
  jet->Init("data");
  auto hist = jet->GetHistPt(0);

  unique_ptr<JEFW> mcjet{new JEFW("../../mcjetSpectraESE/AnalysisResults.root")};
  mcjet->Init("mc");
  auto resp = mcjet->GetHistMC();
  // auto htrue = dynamic_cast<TH1D*>(resp->ProjectionY("htrue1"));
  auto htrue = mcjet->GetHistPt(1);
  auto hmatched = mcjet->GetHistPt(2);

  unique_ptr<JEfficiency> jeff{new JEfficiency(hist, resp, htrue, hmatched)}; 
  auto hReco = jeff->Unfold(5);

  // auto eff = jeff->Efficiency();
  // hReco->Sumw2(false);
  // eff->Sumw2(false);
  // hReco->Divide(eff);

  TCanvas c2("c2","",800,600);
  c2.cd();
  gPad->SetLogy();
  hist->SetStats(0);
  hist->SetLineColor(kBlack);

  hist->GetXaxis()->SetRangeUser(10,50);
  Double_t scale1 = hist->GetXaxis()->GetBinWidth(1)/(hist->Integral());
  hist->Scale(scale1);
  hist->Draw();

  hist->SetTitle("");
  hist->GetYaxis()->SetTitle("#frac{1}{#it{N}_{events}} #frac{d^{2}N}{d#it{p}_{T}d#it{#eta}}");


  hReco->SetLineColor(kGreen);
  Double_t scale2 = hReco->GetXaxis()->GetBinWidth(1)/(hReco->Integral());
  hReco->Scale(scale2);
  hReco->Draw("same");

  htrue->SetLineColor(kRed);
  Double_t scale3 = htrue->GetXaxis()->GetBinWidth(1)/(htrue->Integral());
  htrue->Scale(scale3);
  htrue->Draw("same");


  TLegend * leg = new TLegend(  0.55,  0.65,  0.86, 0.85);
  leg->AddEntry(hist, "Measured", "l");
  leg->AddEntry(hReco, "Unfolded", "l");
  leg->AddEntry(htrue, "True", "l");
  //draw legend without frame
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->Draw();

  c2.SaveAs("figures/unfold_response_woEff.pdf");

}
