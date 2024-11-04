
void draw_planes_unfolded_q2() {
  const char* Rval = "0.4";

  std::unique_ptr<TFile> f = std::unique_ptr<TFile>(new TFile(Form("root_files/unfolded_q2_1_30_R%s.root",Rval),"READ"));
  // TH1D* h_in_inclusive_q2Small_raw = dynamic_cast<TH1D*>(f->Get("hv_0"));
  // TH1D* h_out_inclusive_q2Small_raw = dynamic_cast<TH1D*>(f->Get("hv_1"));
  JEUtility *jet_small = new JEUtility(f.get(), 0.4); /* R local */
  auto hs_small = jet_small->YieldCorrectedJet();
  TH1* h_in_inclusive_q2Small_raw = hs_small.first;
  TH1* h_out_inclusive_q2Small_raw = hs_small.second;

  std::unique_ptr<TFile> fl = std::unique_ptr<TFile>(new TFile(Form("root_files/unfolded_q2_70_100_R%s.root",Rval),"READ"));
  // TH1D* h_in_inclusive_q2Large_raw = dynamic_cast<TH1D*>(fl->Get("hv_0"));
  // TH1D* h_out_inclusive_q2Large_raw = dynamic_cast<TH1D*>(fl->Get("hv_1"));
  JEUtility *jet_large = new JEUtility(fl.get(), 0.65); /* R local */
  auto hs_large = jet_large->YieldCorrectedJet();
  TH1* h_in_inclusive_q2Large_raw = hs_large.first;
  TH1* h_out_inclusive_q2Large_raw = hs_large.second;

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

  TLegend * leg = new TLegend(  0.2,  0.2,  0.4, 0.4);
  leg->AddEntry(h_in_inclusive_large,"in-plane (large-#it{q}_{2})","l");
  leg->AddEntry(h_in_inclusive_small,"in-plane (small-#it{q}_{2})","l");
  leg->AddEntry(h_out_inclusive_small,"out-of-plane (small-#it{q}_{2})","l");
  leg->AddEntry(h_out_inclusive_large,"out-of-plane (large-#it{q}_{2})","l");
  leg->Draw();

  TLatex latex2;
  latex2.SetTextSize(0.04);  
  latex2.DrawLatex(60, 1e-1, "ALICE Pb--Pb #sqrt{s_{NN}} = 5.36 TeV");

  c1.SaveAs(Form("figures/pt_separatePlanes_q2_UNFOLDED_R%s.pdf",Rval));


  // TCanvas c1rat("c1rat","",400,200);
  // TH1F *hRatio = (TH1F*)h_in_inclusive_large->Clone("hRatio");
  // hRatio->Divide(h_in_inclusive_small);  
  // hRatio->SetLineColor(kBlack);
  // hRatio->GetYaxis()->SetTitle("out-of-plane/in-plane");
  // hRatio->GetXaxis()->SetRangeUser(10,150);
  // hRatio->GetYaxis()->SetRangeUser(0.5,1.5);
  // hRatio->Draw();

  // TLine *line = new TLine(10, 1, 150, 1);
  // line->SetLineStyle(2);
  // line->Draw();

  // c1rat.SaveAs("figures/pt_separatePlanes_q2_ratio_UNFOLDED.pdf");

}
