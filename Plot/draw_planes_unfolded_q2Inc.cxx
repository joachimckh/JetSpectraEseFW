


void draw_planes_unfolded_q2Inc() {
  

  std::unique_ptr<TFile> f = std::unique_ptr<TFile>(new TFile("root_files/unfolded_q2_1_100.root","READ"));
  TH1D* h_in_inclusive_raw = dynamic_cast<TH1D*>(f->Get("hv_0"));
  TH1D* h_out_inclusive_raw = dynamic_cast<TH1D*>(f->Get("hv_1"));
  JEUtility *jet = new JEUtility(f.get(), 1.0); /* R in ideal case */
  auto [h_in_inclusive_Corr, h_out_inclusive_Corr] = jet->YieldCorrectedJet();


  int n = 16;
  Double_t bin_edges[17]={0,5,10,15,20,25,30,40,50,60,70,80,90,100,120,160,200};
  TH1* h_in_inclusive = h_in_inclusive_raw->Rebin(n, "hInInclusiveReb", bin_edges);
  TH1* h_out_inclusive = h_out_inclusive_raw->Rebin(n, "hOutInclusiveReb", bin_edges);

  TH1* h_in_inclusive_corr_Rebin = h_in_inclusive_Corr->Rebin(n, "hInCorr_re", bin_edges);
  TH1* h_out_inclusive_corr_Rebin = h_out_inclusive_Corr->Rebin(n, "hOutCorr_re", bin_edges);

  TCanvas c1("c1","",800,600);
  c1.cd();
  
  h_in_inclusive->GetYaxis()->SetTitle("#frac{1}{#it{N}_{ev}} #frac{d^{2}N}{d#it{p}_{T}d#it{#eta}}");
  h_in_inclusive->SetLineColor(kBlack);
  h_in_inclusive->GetXaxis()->SetRangeUser(10,150);
  h_in_inclusive->Draw("EP");

  // h_out_inclusive->SetLineColor(kRed);
  // h_out_inclusive->GetXaxis()->SetRangeUser(10,150);
  // h_out_inclusive->Draw("EPsame");

  h_in_inclusive_corr_Rebin->SetLineColor(kBlue);
  h_in_inclusive_corr_Rebin->GetXaxis()->SetRangeUser(10,150);
  h_in_inclusive_corr_Rebin->Draw("EPsame");

  // h_out_inclusive_corr_Rebin->SetLineColor(kGreen);
  // h_out_inclusive_corr_Rebin->GetXaxis()->SetRangeUser(10,150);
  // h_out_inclusive_corr_Rebin->Draw("EPsame");

  gPad->SetLogy();

  TLegend * leg = new TLegend(  0.65,  0.65,  0.86, 0.75);
  leg->AddEntry(h_in_inclusive,"in-plane","l");
  // leg->AddEntry(h_out_inclusive,"out-of-plane","l");
  leg->AddEntry(h_in_inclusive_corr_Rebin,"in-plane corr","l");
  // leg->AddEntry(h_out_inclusive_corr_Rebin,"out-of-plane corr","l");
  leg->Draw();

  c1.SaveAs("figures/pt_separatePlanes_inclusive_UNFOLDED.pdf");

  TCanvas c1rat("c1rat","",400,200);
  TH1F *hRatio = (TH1F*)h_out_inclusive->Clone("hRatio");
  TH1F *hRatioCorr = (TH1F*)h_out_inclusive_corr_Rebin->Clone("hRatioCorr");
  hRatio->Divide(h_in_inclusive);  
  hRatio->SetLineColor(kBlack);
  hRatio->GetYaxis()->SetTitle("out-of-plane/in-plane");
  hRatio->GetXaxis()->SetRangeUser(10,150);
  hRatio->GetYaxis()->SetRangeUser(0.5,1.5);
  hRatio->Draw();

  hRatioCorr->Divide(h_in_inclusive_corr_Rebin);
  hRatioCorr->SetLineColor(kRed);
  hRatioCorr->Draw("same");

  TLine *line = new TLine(10, 1, 150, 1);
  line->SetLineStyle(2);
  line->Draw();
  c1rat.SaveAs("figures/pt_separatePlanes_inclusive_ratio_UNFOLDED.pdf");

}
