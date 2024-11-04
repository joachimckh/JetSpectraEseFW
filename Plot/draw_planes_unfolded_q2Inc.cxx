


void draw_planes_unfolded_q2Inc() {
  
  const char* Rval = "0.4";

  std::unique_ptr<TFile> f = std::unique_ptr<TFile>(new TFile(Form("root_files/unfolded_q2_1_100_R%s.root",Rval),"READ"));
  TH1D* h_in_inclusive_raw = dynamic_cast<TH1D*>(f->Get("hv_0"));
  TH1D* h_out_inclusive_raw = dynamic_cast<TH1D*>(f->Get("hv_1"));
  JEUtility *jet = new JEUtility(f.get(), 0.71); /* R in ideal case */
  auto hs = jet->YieldCorrectedJet();
  TH1* h_in_inclusive_Corr = hs.first;
  TH1* h_out_inclusive_Corr = hs.second;


  int n = 16;
  Double_t bin_edges[17]={0,5,10,15,20,25,30,40,50,60,70,80,90,100,120,160,200};
  TH1* h_in_inclusive = h_in_inclusive_raw->Rebin(n, "hInInclusiveReb", bin_edges);
  TH1* h_out_inclusive = h_out_inclusive_raw->Rebin(n, "hOutInclusiveReb", bin_edges);

  TH1* h_in_inclusive_corr_Rebin = h_in_inclusive_Corr->Rebin(n, "hInCorr_re", bin_edges);
  TH1* h_out_inclusive_corr_Rebin = h_out_inclusive_Corr->Rebin(n, "hOutCorr_re", bin_edges);

  TCanvas c1("c1","",800,600);
  c1.cd();
  
  // h_in_inclusive->GetYaxis()->SetTitle("#frac{1}{#it{N}_{ev}} #frac{d^{2}N}{d#it{p}_{T}d#it{#eta}}");
  // h_in_inclusive->SetLineColor(kBlack);
  // h_in_inclusive->GetXaxis()->SetRangeUser(10,150);
  // h_in_inclusive->Draw("EP");

  // h_out_inclusive->SetLineColor(kRed);
  // h_out_inclusive->GetXaxis()->SetRangeUser(10,150);
  // h_out_inclusive->Draw("EPsame");
  h_in_inclusive_corr_Rebin->GetYaxis()->SetTitle("#frac{1}{#it{N}_{ev}} #frac{d^{2}N}{d#it{p}_{T}d#it{#eta}}");
  h_in_inclusive_corr_Rebin->SetLineColor(kBlack);
  h_in_inclusive_corr_Rebin->GetXaxis()->SetRangeUser(10,150);
  h_in_inclusive_corr_Rebin->Draw("EPsame");

  h_out_inclusive_corr_Rebin->SetLineColor(kRed);
  h_out_inclusive_corr_Rebin->GetXaxis()->SetRangeUser(10,150);
  h_out_inclusive_corr_Rebin->Draw("EPsame");

  gPad->SetLogy();

  TLegend * leg = new TLegend(  0.65,  0.65,  0.86, 0.75);
  // leg->AddEntry(h_in_inclusive,"in-plane","l");
  // leg->AddEntry(h_out_inclusive,"out-of-plane","l");
  leg->AddEntry(h_in_inclusive_corr_Rebin,"in-plane (Corrected)","l");
  leg->AddEntry(h_out_inclusive_corr_Rebin,"out-of-plane (Corrected)","l");
  leg->Draw();

  TLatex latex;
  latex.SetTextSize(0.04);  
  latex.DrawLatex(30, 0.1, "ALICE Pb--Pb #sqrt{s_{NN}} = 5.36 TeV");

  c1.SaveAs(Form("figures/pt_separatePlanes_inclusive_UNFOLDED_R%s.pdf",Rval));

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

  auto leg2 = new TLegend(0.65, 0.2, 0.86, 0.45);
  leg2->AddEntry(hRatio,"Before #it{R}_{2} correction","l");
  leg2->AddEntry(hRatioCorr,"After #it{R}_{2} correction","l");
  leg2->Draw();

  TLatex latex2;
  latex2.SetTextSize(0.04);  
  latex2.DrawLatex(20, 1.4, "ALICE Pb--Pb #sqrt{s_{NN}} = 5.36 TeV");

  c1rat.SaveAs(Form("figures/pt_separatePlanes_inclusive_ratio_UNFOLDED_R%s.pdf",Rval));

  TCanvas c2("c2","",800,600);
  c2.cd();

  TH1F* hRatCorr = (TH1F*)h_in_inclusive_corr_Rebin->Clone("hRatCorr");
  hRatCorr->Divide(h_in_inclusive);

  hRatCorr->GetYaxis()->SetTitle("corr/inclusive");
  hRatCorr->GetXaxis()->SetRangeUser(10,150);
  hRatCorr->GetYaxis()->SetRangeUser(0.5,1.5);
  hRatCorr->Draw();

  c2.SaveAs(Form("figures/yieldRatio_in_q2Inc_R%s.pdf",Rval));


}
