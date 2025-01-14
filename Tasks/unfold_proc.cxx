#include "JEFW.hpp"
#include "JEfficiency.hpp"


#include <memory>
#include <iostream>

#include <TCanvas.h>
#include <TLegend.h>
#include <TH1.h>

using std::unique_ptr;

int main(int argc, char *argv[]){

  if (argc < 3) {
    std::cerr << "Usage: " << argv[0] << " <run number>" << std::endl;
    std::cerr << "Usage: " << argv[1] << " <R>" << std::endl;
    std::cerr << "Usage: " << argv[2] << " <mc run number>" << std::endl;
  }
  const char* DATArun = argv[1]; /* 286287, R=0.4 | 286533, R=0.2*/
  const char* Rval = argv[2];
  unique_ptr<JEFW> jet{new JEFW(Form("/Users/joachimcarlokristianhansen/jet_analysis/hyperloop_data/LHC23zzh_pass4_small/jet/%s/AnalysisResults.root",DATArun))};
  jet->Init("data");
  int q2Min = 70;
  int q2Max = 100;

  std::vector<int> q2{q2Min,q2Max};
  auto objarr = jet->separatePlanes(q2);
  // auto hist = reinterpret_cast<TH1F*>(objarr->FindObject("hv_1"));
  // auto hist = jet->GetHistPt(0);

  // mc
  const char* MCrun = argv[2];// "284907";
  unique_ptr<JEFW> mcjet{new JEFW(Form("/Users/joachimcarlokristianhansen/jet_analysis/hyperloop_data/LHC24g3/jet/%s/AnalysisResults.root",MCrun))};
  mcjet->Init("mc");
  auto resp = mcjet->getHistMC();
  // auto htrue = dynamic_cast<TH1D*>(resp->ProjectionY("htrue1")); /* true from response */
  auto htrue = mcjet->getHistPt(1); /* true from TRUE process function */
  auto hmatched = mcjet->getHistPt(2);


  // auto eff = jeff->Efficiency();
  // hReco->Sumw2(false);
  // eff->Sumw2(false);
  // hReco->Divide(eff);
  TObjArray* SepPlanes = new TObjArray();
  SepPlanes->SetName("planes");
  SepPlanes->SetOwner(kTRUE);
  for (int i{0}; i<2; i++){
    auto hist = reinterpret_cast<TH1F*>(objarr->FindObject(Form("hv_%i",i)));

    unique_ptr<JEfficiency> jeff{new JEfficiency(hist, resp, htrue, hmatched)}; 
    auto hReco = jeff->Unfold(5);

    TCanvas c2("c2","",800,600);
    c2.cd();
    gPad->SetLogy();
    hist->SetStats(0);
    hist->SetLineColor(kBlack);

    hist->GetXaxis()->SetRangeUser(10,120);
    Double_t scale1 = 1.0/(hist->Integral());
    hist->Scale(scale1);
    hist->Draw();

    hist->SetTitle("");
    hist->GetYaxis()->SetTitle("#frac{1}{#it{N}_{events}} #frac{d^{2}N}{d#it{p}_{T}d#it{#eta}}");


    hReco->SetLineColor(kGreen);
    Double_t scale2 = 1.0/(hReco->Integral());
    hReco->Scale(scale2);
    hReco->Draw("same");

    htrue->SetLineColor(kRed);
    Double_t scale3 = 1.0/(htrue->Integral());
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

    c2.SaveAs(Form("figures/unfold_response_%i_%s.pdf",i,Rval));

    hReco->SetName(Form("hv_%i",i));
    SepPlanes->Add(hReco);

  }

  

  SepPlanes->SaveAs(Form("root_files/unfolded_q2_%i_%i_R%s.root",q2.at(0),q2.at(1),Rval));

}
