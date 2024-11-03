



void response(){


  const char* MCrun = "284907";
  std::unique_ptr<JEFW> mcjet{new JEFW(Form("/Users/joachimcarlokristianhansen/jet_analysis/hyperloop_data/LHC24g3/jet/%s/AnalysisResults.root",MCrun))};
  mcjet->Init("mc");
  auto resp = mcjet->GetHistMC();
  // auto htrue = dynamic_cast<TH1D*>(resp->ProjectionY("htrue1"));
  // auto htrue = mcjet->GetHistPt(1);
  // auto hmatched = mcjet->GetHistPt(2);

  // unique_ptr<JEfficiency> jeff{new JEfficiency(hist, resp, htrue, hmatched)}; 
  //auto hReco = jeff->Unfold(5);

  TCanvas c2("c2","",800,600);
  c2.cd();
  gPad->SetLogz();
  resp->SetStats(0);
  resp->GetXaxis()->SetRangeUser(10,120);
  resp->GetYaxis()->SetRangeUser(10,120);
  
  resp->GetXaxis()->SetTitle("p_{T,jet}^{truth} (GeV/c)");
  resp->GetYaxis()->SetTitle("p_{T,jet}^{measured} (GeV/c)");
  
  resp->Draw();

  resp->SetTitle("");
  //hist->GetYaxis()->SetTitle("d^{2}N/d#it{p}_{T}d#it{#eta}");



  // TLegend * leg = new TLegend(  0.55,  0.65,  0.86, 0.85);
  // leg->AddEntry(hist, "Measured", "l");
  // //draw legend without frame
  // leg->SetFillStyle(0);
  // leg->SetBorderSize(0);
  // leg->Draw();

  c2.SaveAs("figures/response.pdf");

}
