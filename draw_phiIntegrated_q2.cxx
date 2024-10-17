void SetStyle(Bool_t graypalette=kFALSE);

void draw_phiIntegrated_q2() {
  SetStyle();

  std::unique_ptr<TFile> f = std::unique_ptr<TFile>(new TFile("root_files/pt_dphiInt_q2_0_30.root","READ"));
  auto h_small = std::unique_ptr<TH1D>(dynamic_cast<TH1D*>(f->Get("pt_q2_0_30;1")));

  std::unique_ptr<TFile> f2 = std::unique_ptr<TFile>(new TFile("root_files/pt_dphiInt_q2_70_100.root","READ"));
  auto h_large = std::unique_ptr<TH1D>(dynamic_cast<TH1D*>(f2->Get("pt_q2_70_100;1")));

  TCanvas c1("c1","",800,600);
  c1.cd();

  h_large->SetLineColor(kBlack);
  h_large->GetXaxis()->SetRangeUser(0,50);
  h_large->Draw("EP");

  h_small->SetLineColor(kRed);
  h_small->Draw("EPsame");

  gPad->SetLogy();
  

  // // TLatex * text = new TLatex (0.6,87,"Pb-Pb #sqrt{#it{s}_{NN}} = 5.02 TeV");
  // // text->Draw();
  
  // TLatex * text = new TLatex (0.55,5,"#it{p}_{T}^{lead track} > 5 GeV/#it{c}");
  // text->Draw();
  TLatex * text2 = new TLatex (0.55,15,"Charged-particle jets, anti-#it{k}_{T}, R=0.4");
  text2->Draw();
  TLatex * text11 = new TLatex (0.55,35,"30-50\% FT0-C");
  text11->Draw();

  TLegend * leg = new TLegend(  0.65,  0.65,  0.86, 0.75);
  leg->AddEntry(h_large.get(),"large-#it{q}_{2}","l");
  leg->AddEntry(h_small.get(),"small-#it{q}_{2}","l");
  leg->Draw();

  c1.SaveAs("figures/pt_azimuthal_integrated_q2.pdf");

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