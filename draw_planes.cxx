void SetStyle(Bool_t graypalette=kFALSE);


void draw_planes() {
  SetStyle();
  

  std::unique_ptr<TFile> f = std::unique_ptr<TFile>(new TFile("root_files/SeparatePtPlane_q2_1_100.root","READ"));
  std::unique_ptr<TH1F> h_in_inclusive = std::unique_ptr<TH1F>(dynamic_cast<TH1F*>(f->Get("hv_0")));
  std::unique_ptr<TH1F> h_out_inclusive = std::unique_ptr<TH1F>(dynamic_cast<TH1F*>(f->Get("hv_1")));

  std::unique_ptr<TFile> fs = std::unique_ptr<TFile>(new TFile("root_files/SeparatePtPlane_q2_1_30.root","READ"));
  std::unique_ptr<TH1F> h_in_inclusive_small = std::unique_ptr<TH1F>(dynamic_cast<TH1F*>(fs->Get("hv_0")));
  std::unique_ptr<TH1F> h_out_inclusive_small = std::unique_ptr<TH1F>(dynamic_cast<TH1F*>(fs->Get("hv_1")));

  std::unique_ptr<TFile> fl = std::unique_ptr<TFile>(new TFile("root_files/SeparatePtPlane_q2_70_100.root","READ"));
  std::unique_ptr<TH1F> h_in_inclusive_large = std::unique_ptr<TH1F>(dynamic_cast<TH1F*>(fl->Get("hv_0")));
  std::unique_ptr<TH1F> h_out_inclusive_large = std::unique_ptr<TH1F>(dynamic_cast<TH1F*>(fl->Get("hv_1")));

  TCanvas c1("c1","",800,600);
  c1.cd();

  h_in_inclusive->SetLineColor(kBlack);
  h_in_inclusive->GetXaxis()->SetRangeUser(0,50);
  h_in_inclusive->Draw("EP");

  h_out_inclusive->SetLineColor(kRed);
  h_out_inclusive->Draw("EPsame");

  gPad->SetLogy();
  

  // TLatex * text = new TLatex (0.6,87,"Pb-Pb #sqrt{#it{s}_{NN}} = 5.02 TeV");
  // text->Draw();
  
  TLatex * text = new TLatex (0.55,5,"#it{p}_{T}^{lead track} > 5 GeV/#it{c}");
  text->Draw();
  TLatex * text2 = new TLatex (0.55,15,"Charged-particle jets, anti-#it{k}_{T}, R=0.4");
  text2->Draw();
  TLatex * text11 = new TLatex (0.55,35,"30-50\% FT0-C");
  text11->Draw();

  TLegend * leg = new TLegend(  0.65,  0.65,  0.86, 0.75);
  leg->AddEntry(h_in_inclusive.get(),"in-plane","l");
  leg->AddEntry(h_out_inclusive.get(),"out-of-plane","l");
  leg->Draw();

  c1.SaveAs("figures/pt_separatePlanes_inclusive.pdf");



  TCanvas c2("c2","",800,600);
  c2.cd();

  h_in_inclusive_large->SetLineColor(kBlack);
  h_in_inclusive_large->GetXaxis()->SetRangeUser(0,50);
  h_in_inclusive_large->Draw("EP");
  h_in_inclusive_small->SetLineColor(kRed+1);
  h_in_inclusive_small->Draw("EPsame");

  h_out_inclusive_small->SetLineColor(kBlue+1);
  h_out_inclusive_large->SetLineColor(kMagenta+1);
  h_out_inclusive_small->Draw("EPsame");
  h_out_inclusive_large->Draw("EPsame");

  gPad->SetLogy();

  TLegend * leg2 = new TLegend(  0.55,  0.65,  0.86, 0.85);
  leg2->AddEntry(h_in_inclusive_large.get(),"in-plane, large-#it{q}_{2}","l");
  leg2->AddEntry(h_in_inclusive_small.get(),"in-plane, small-#it{q}_{2}","l");
  leg2->AddEntry(h_out_inclusive_large.get(),"out-of-plane, large-#it{q}_{2}","l");
  leg2->AddEntry(h_out_inclusive_small.get(),"out-of-plane, small-#it{q}_{2}","l");
  leg2->Draw();

  TLatex * text321 = new TLatex (0.55,5,"#it{p}_{T}^{lead track} > 5 GeV/#it{c}");
  text321->Draw();
  TLatex * text2112 = new TLatex (0.55,15,"Charged-particle jets, anti-#it{k}_{T}, R=0.4");
  text2112->Draw();
  TLatex * text1141 = new TLatex (0.55,35,"30-50\% FT0-C");
  text1141->Draw();

  c2.SaveAs("figures/pt_separatePlanes_q2.pdf");

}


void SetStyle(Bool_t graypalette) {
  cout << "Setting style!" << endl;
  
  gStyle->Reset("Plain");
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  if(graypalette) gStyle->SetPalette(8,0);
  else gStyle->SetPalette(1);
  gStyle->SetCanvasColor(10);
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetFrameLineWidth(1);
  gStyle->SetFrameFillColor(kWhite);
  gStyle->SetPadColor(10);
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  gStyle->SetPadBottomMargin(0.15);
  gStyle->SetPadLeftMargin(0.15);
  gStyle->SetHistLineWidth(1);
  gStyle->SetHistLineColor(kRed);
  gStyle->SetFuncWidth(2);
  gStyle->SetFuncColor(kGreen);
  gStyle->SetLineWidth(2);
  gStyle->SetLabelSize(0.045,"xyz");
  gStyle->SetLabelOffset(0.01,"y");
  gStyle->SetLabelOffset(0.01,"x");
  gStyle->SetLabelColor(kBlack,"xyz");
  gStyle->SetTitleSize(0.05,"xyz");
  gStyle->SetTitleOffset(1.25,"y");
  gStyle->SetTitleOffset(1.2,"x");
  gStyle->SetTitleFillColor(kWhite);
  gStyle->SetTextSizePixels(26); 
  gStyle->SetTextFont(42);
  //  gStyle->SetTickLength(0.04,"X");  gStyle->SetTickLength(0.04,"Y"); 

  gStyle->SetLegendBorderSize(0);
  gStyle->SetLegendFillColor(kWhite);
  //  gStyle->SetFillColor(kWhite);
  gStyle->SetLegendFont(42);


}