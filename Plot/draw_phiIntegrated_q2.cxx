void hScale(TH1D* h)
{
  Double_t scale = h->GetXaxis()->GetBinWidth(1)/(h->Integral());
  h->Scale(scale);
}

void draw_phiIntegrated_q2() {

  std::unique_ptr<TFile> f = std::unique_ptr<TFile>(new TFile("root_files/pt_dphiInt_q2_0_30.root","READ"));
  TH1D* h_small = dynamic_cast<TH1D*>(f->Get("pt_q2_0_30;1"));

  std::unique_ptr<TFile> f2 = std::unique_ptr<TFile>(new TFile("root_files/pt_dphiInt_q2_70_100.root","READ"));
  TH1D* h_large = dynamic_cast<TH1D*>(f2->Get("pt_q2_70_100;1"));

  TCanvas c1("c1","",800,600);
  c1.DrawFrame(0,0,1,1);
  c1.cd();

  h_large->SetLineColor(kBlack);
  h_large->GetXaxis()->SetRangeUser(0,100);

  h_large->GetYaxis()->SetTitle("#frac{1}{#it{N}_{events}}#frac{d^{2}N}{d#it{p}_{T}d#it{#eta}}");

  hScale(h_large);
  h_large->Draw("EP");

  h_small->SetLineColor(kRed);
  hScale(h_small);
  h_small->Draw("EPsame");

  gPad->SetLogy();
  
  TLatex latex;
  latex.SetTextSize(0.05);
  latex.SetTextAlign(13);  //align at top
  latex.DrawLatex(.5,5e-4,"Charged-particle jets, anti-#it{k}_{T}, R=0.4");
  latex.DrawLatex(.5,2.5e-4,"30-50\% FT0-C");


  TLegend * leg = new TLegend(  0.65,  0.65,  0.86, 0.75);
  leg->AddEntry(h_large,"large-#it{q}_{2}","l");
  leg->AddEntry(h_small,"small-#it{q}_{2}","l");
  leg->Draw();

  c1.SaveAs("figures/pt_azimuthal_integrated_q2.pdf");

}
