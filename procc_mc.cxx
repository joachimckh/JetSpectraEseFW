
#include "Core/JEFW.h"
#include "Core/JEfficiency.h"

#include <memory>


using std::unique_ptr;

void procc_mc(){

  unique_ptr<JEFW> jet{new JEFW("../../jetSpectraESE/AnalysisResults.root")};
  jet->Init("data");
  auto hist = jet->GetHist(0);

  unique_ptr<JEFW> mcjet{new JEFW("../../mcjetSpectraESE/AnalysisResults.root")};
  mcjet->Init("mc");
  auto resp = mcjet->GetHistMC();
  auto htrue = dynamic_cast<TH1D*>(resp->ProjectionY("htrue1"));


  unique_ptr<JEfficiency> JEfficiency{new JEfficiency(hist, resp)}; 
  auto hReco = JEfficiency->Unfold(4);


  TCanvas c2("c2","",800,600);
  c2.cd();
  gPad->SetLogy();
  hist->SetStats(0);
  hist->SetLineColor(kBlack);
  hist->Draw();

  hist->SetTitle("");
  hist->GetYaxis()->SetTitle("d^{2}N/d#it{p}_{T}d#it{#eta}");


  hReco->SetLineColor(kGreen);
  hReco->Draw("same");

  htrue->SetLineColor(kRed);
  htrue->Draw("same");


  TLegend * leg = new TLegend(  0.55,  0.65,  0.86, 0.85);
  leg->AddEntry(hist, "Measured", "l");
  leg->AddEntry(hReco, "Unfolded", "l");
  leg->AddEntry(htrue, "True", "l");
  //draw legend without frame
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->Draw();

  c2.SaveAs("figures/unfold_response.pdf");

}
