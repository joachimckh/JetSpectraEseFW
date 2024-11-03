
void draw_planes_unfolded_q2() {

  std::unique_ptr<TFile> f = std::unique_ptr<TFile>(new TFile("root_files/unfolded_q2_1_30.root","READ"));
  TH1D* h_in_inclusive_q2Small_raw = dynamic_cast<TH1D*>(f->Get("hv_0"));
  TH1D* h_out_inclusive_q2Small_raw = dynamic_cast<TH1D*>(f->Get("hv_1"));

  std::unique_ptr<TFile> fl = std::unique_ptr<TFile>(new TFile("root_files/unfolded_q2_70_100.root","READ"));
  TH1D* h_in_inclusive_q2Large_raw = dynamic_cast<TH1D*>(fl->Get("hv_0"));
  TH1D* h_out_inclusive_q2Large_raw = dynamic_cast<TH1D*>(fl->Get("hv_1"));

  int n = 16;
  Double_t bin_edges[17]={0,5,10,15,20,25,30,40,50,60,70,80,90,100,120,160,200};
  TH1* h_in_inclusive_small = h_in_inclusive_q2Small_raw->Rebin(n, "h_in_inclusive_q2Small_raw", bin_edges);
  TH1* h_out_inclusive_small = h_out_inclusive_q2Small_raw->Rebin(n, "h_out_inclusive_q2Small_raw", bin_edges);
  TH1* h_in_inclusive_large = h_in_inclusive_q2Large_raw->Rebin(n, "h_in_inclusive_q2Large_raw", bin_edges);
  TH1* h_out_inclusive_large = h_out_inclusive_q2Large_raw->Rebin(n, "h_out_inclusive_q2Large_raw", bin_edges);

  TCanvas c1("c1","",800,600);
  c1.cd();

  h_in_inclusive_small->GetYaxis()->SetTitle("#frac{1}{#it{N}_{ev}} #frac{d^{2}N}{d#it{p}_{T}d#it{#eta}}");
  h_in_inclusive_small->SetLineColor(kViolet);
  h_in_inclusive_small->GetXaxis()->SetRangeUser(10,150);
  h_in_inclusive_small->Draw("EP");

  h_out_inclusive_small->SetLineColor(kBlue);
  h_out_inclusive_small->GetXaxis()->SetRangeUser(10,150);
  h_out_inclusive_small->Draw("EPsame");

  h_in_inclusive_large->SetLineColor(kCyan);
  h_in_inclusive_large->GetXaxis()->SetRangeUser(10,150);
  h_in_inclusive_large->Draw("EPsame");

  h_out_inclusive_large->SetLineColor(kOrange+1);
  h_out_inclusive_large->GetXaxis()->SetRangeUser(10,150);
  h_out_inclusive_large->Draw("EPsame");


  gPad->SetLogy();

  TLegend * leg = new TLegend(  0.65,  0.65,  0.86, 0.75);
  leg->AddEntry(h_in_inclusive_large,"in-plane (large-#it{q}_{2})","l");
  leg->AddEntry(h_in_inclusive_small,"in-plane (small-#it{q}_{2})","l");
  leg->AddEntry(h_out_inclusive_small,"out-of-plane (small-#it{q}_{2})","l");
  leg->AddEntry(h_out_inclusive_large,"out-of-plane (large-#it{q}_{2})","l");
  

  leg->Draw();

  c1.SaveAs("figures/pt_separatePlanes_q2_UNFOLDED.pdf");


}
