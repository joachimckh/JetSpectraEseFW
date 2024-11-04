

void qa()
{

  const char* DATArun = "286287"; /* R=0.4 */
  
  auto f = TFile::Open(Form("/Users/joachimcarlokristianhansen/jet_analysis/hyperloop_data/LHC23zzh_pass4_small/jet/%s/AnalysisResults.root",DATArun));
  auto dir = reinterpret_cast<TDirectory*>(f->Get("jet-spectra-ese-task"));

  auto h_collisions = reinterpret_cast<TH1F*>(dir->Get("h_collisions"));

  auto c = new TCanvas("c","",800,600);
  c->cd();

  h_collisions->SetLineColor(kBlack);
  h_collisions->Draw();

  TLatex latex2;
  latex2.SetTextSize(0.04);  
  latex2.DrawLatex(4, 80e6, "ALICE Pb--Pb #sqrt{s_{NN}} = 5.36 TeV");

  c->SaveAs("figures/qa.pdf");
}