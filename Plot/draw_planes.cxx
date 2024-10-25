void hScale(TH1* h)
{
  Double_t scale = h->GetXaxis()->GetBinWidth(1)/(h->Integral());
  h->Scale(scale);
}

void draw_planes() {
  
  

  std::unique_ptr<TFile> f = std::unique_ptr<TFile>(new TFile("root_files/SeparatePtPlane_q2_1_100.root","READ"));
  TH1F* h_in_inclusive = dynamic_cast<TH1F*>(f->Get("hv_0"));
  TH1F* h_out_inclusive = dynamic_cast<TH1F*>(f->Get("hv_1"));

  std::unique_ptr<TFile> fs = std::unique_ptr<TFile>(new TFile("root_files/SeparatePtPlane_q2_1_30.root","READ"));
  TH1F* h_in_inclusive_small = dynamic_cast<TH1F*>(fs->Get("hv_0"));
  TH1F* h_out_inclusive_small = dynamic_cast<TH1F*>(fs->Get("hv_1"));

  std::unique_ptr<TFile> fl = std::unique_ptr<TFile>(new TFile("root_files/SeparatePtPlane_q2_70_100.root","READ"));
  TH1F* h_in_inclusive_large = dynamic_cast<TH1F*>(fl->Get("hv_0"));
  TH1F* h_out_inclusive_large = dynamic_cast<TH1F*>(fl->Get("hv_1"));

  TCanvas c1("c1","",800,600);
  c1.cd();
  
  h_in_inclusive->GetYaxis()->SetTitle("#frac{1}{#it{N}_{events}} #frac{d^{2}N}{d#it{p}_{T}d#it{#eta}}");
  h_in_inclusive->SetLineColor(kBlack);
  h_in_inclusive->GetXaxis()->SetRangeUser(10,150);
  hScale(h_in_inclusive);
  h_in_inclusive->Draw("EP");

  h_out_inclusive->SetLineColor(kRed);
  h_out_inclusive->GetXaxis()->SetRangeUser(10,150);
  hScale(h_out_inclusive);
  h_out_inclusive->Draw("EPsame");

  gPad->SetLogy();

  // TLatex latex;
  // latex.SetTextSize(0.05);
  // latex.SetTextAlign(13);  //align at top
  // latex.DrawLatex(.5,5e-4,"Charged-particle jets, anti-#it{k}_{T}, R=0.4");
  // latex.DrawLatex(.5,2.5e-4,"30-50\% FT0-C");
  // latex.DrawLatex(.5,1e-4,"#it{p}_{T}^{lead track} > 5 GeV/#it{c}");
  

  TLegend * leg = new TLegend(  0.65,  0.65,  0.86, 0.75);
  leg->AddEntry(h_in_inclusive,"in-plane","l");
  leg->AddEntry(h_out_inclusive,"out-of-plane","l");
  leg->Draw();

  c1.SaveAs("figures/pt_separatePlanes_inclusive.pdf");

  TCanvas c1rat("c1rat","",400,200);
  TH1F *hRatio = (TH1F*)h_in_inclusive->Clone("hRatio");
  hRatio->Divide(h_out_inclusive);  
  hRatio->SetLineColor(kBlack);
  hRatio->GetYaxis()->SetTitle("in-plane/out-of-plane");
  hRatio->GetXaxis()->SetRangeUser(10,150);
  hRatio->GetYaxis()->SetRangeUser(0.5,1.5);
  hRatio->Draw();
  c1rat.SaveAs("figures/pt_separatePlanes_inclusive_ratio.pdf");



  TCanvas c2("c2","",800,600);
  c2.cd();

  h_in_inclusive_large->SetLineColor(kBlack);
  h_in_inclusive_large->GetXaxis()->SetRangeUser(10,60);
  hScale(h_in_inclusive_large);
  h_in_inclusive_large->GetYaxis()->SetTitle("#frac{1}{#it{N}_{events}} #frac{d^{2}N}{d#it{p}_{T}d#it{#eta}}");
  h_in_inclusive_large->Draw("EP");

  h_in_inclusive_small->SetLineColor(kRed+1);
  h_in_inclusive_small->GetXaxis()->SetRangeUser(10,60);
  hScale(h_in_inclusive_small);
  // h_in_inclusive_small->Draw("EPsame");

  
  h_out_inclusive_large->SetLineColor(kGreen+1);
  h_out_inclusive_large->GetXaxis()->SetRangeUser(10,60);
  hScale(h_out_inclusive_large);
  h_out_inclusive_large->Draw("EPsame");

  h_out_inclusive_small->SetLineColor(kBlue+1);
  h_out_inclusive_small->GetXaxis()->SetRangeUser(10,60);
  hScale(h_out_inclusive_small);
  // h_out_inclusive_small->Draw("EPsame");

  gPad->SetLogy();

  TLegend * leg2 = new TLegend(  0.55,  0.65,  0.86, 0.85);
  leg2->AddEntry(h_in_inclusive_large,"in-plane, large-#it{q}_{2}","l");
  // leg2->AddEntry(h_in_inclusive_small,"in-plane, small-#it{q}_{2}","l");
  leg2->AddEntry(h_out_inclusive_large,"out-of-plane, large-#it{q}_{2}","l");
  // leg2->AddEntry(h_out_inclusive_small,"out-of-plane, small-#it{q}_{2}","l");
  leg2->Draw();

  c2.SaveAs("figures/pt_separatePlanes_q2.pdf");

}
