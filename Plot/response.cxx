

#include <TH2F.h>
#include <iostream>
#include <cmath>

// Assuming your original histogram is already created and filled, and is named h_original

TH2F* rebin2DHistogram(TH2F* h_original) {
    // Custom bin edges for X-axis
    int n = 16;
    Double_t bin_edges[17] = {0, 5, 10, 15, 20, 25, 30, 40, 50, 60, 70, 80, 90, 100, 120, 160, 200};


    // Create a new histogram with custom X-axis bins and original Y-axis bins
    TH2F* h_rebinned = new TH2F("h_rebinned", "Rebinned Histogram", n, bin_edges, n, bin_edges);

    // Loop over each bin in the original histogram and transfer content
    for (int xBin = 1; xBin <= h_original->GetNbinsX(); ++xBin) {
        for (int yBin = 1; yBin <= h_original->GetNbinsY(); ++yBin) {
            double content = h_original->GetBinContent(xBin, yBin);
            double error = h_original->GetBinError(xBin, yBin);

            // Skip empty bins to save processing time
            if (content == 0 && error == 0) continue;

            // Get the center of the current bin in the original histogram
            double xCenter = h_original->GetXaxis()->GetBinCenter(xBin);
            double yCenter = h_original->GetYaxis()->GetBinCenter(yBin);

            // Find the corresponding bin in the rebinned histogram
            int newXBin = h_rebinned->GetXaxis()->FindBin(xCenter);
            int newYBin = h_rebinned->GetYaxis()->FindBin(yCenter);

            // Check if the new bin index is valid
            if (newXBin > 0 && newXBin <= h_rebinned->GetNbinsX() && newYBin > 0 && newYBin <= h_rebinned->GetNbinsY()) {
                // Add content to the corresponding bin in the new histogram
                h_rebinned->SetBinContent(h_rebinned->GetBin(newXBin, newYBin), content);
                h_rebinned->SetBinError(h_rebinned->GetBin(newXBin, newYBin),
                                        sqrt(pow(h_rebinned->GetBinError(h_rebinned->GetBin(newXBin, newYBin)), 2) + pow(error, 2)));
            } else {
                std::cerr << "Warning: New bin index out of range for (xCenter: " << xCenter 
                          << ", yCenter: " << yCenter << ")" << std::endl;
            }
        }
    }

    // Now, h_rebinned should contain the rebinned data
    return h_rebinned;
}

void response(){


  const char* MCrun = "284907";
  std::unique_ptr<JEFW> mcjet{new JEFW(Form("/Users/joachimcarlokristianhansen/jet_analysis/hyperloop_data/LHC24g3/jet/%s/AnalysisResults.root",MCrun))};
  mcjet->Init("mc");
  auto resp_raw = mcjet->GetHistMC();
  // auto htrue = dynamic_cast<TH1D*>(resp->ProjectionY("htrue1"));
  // auto htrue = mcjet->GetHistPt(1);
  // auto hmatched = mcjet->GetHistPt(2);

  // unique_ptr<JEfficiency> jeff{new JEfficiency(hist, resp, htrue, hmatched)}; 
  //auto hReco = jeff->Unfold(5);
  
  auto resp = rebin2DHistogram(resp_raw);

  TCanvas c2("c2","",800,600);
  c2.cd();
  gPad->SetLogz();
  resp->SetStats(0);
  resp->GetXaxis()->SetRangeUser(10,120);
  resp->GetYaxis()->SetRangeUser(10,120);
  
  resp->GetXaxis()->SetTitle("p_{T,jet}^{truth} (GeV/c)");
  resp->GetYaxis()->SetTitle("p_{T,jet}^{measured} (GeV/c)");
  
  resp->Draw("colz");

  resp->SetTitle("");
  resp->SaveAs("tmp.root");
  //hist->GetYaxis()->SetTitle("d^{2}N/d#it{p}_{T}d#it{#eta}");



  // TLegend * leg = new TLegend(  0.55,  0.65,  0.86, 0.85);
  // leg->AddEntry(hist, "Measured", "l");
  // //draw legend without frame
  // leg->SetFillStyle(0);
  // leg->SetBorderSize(0);
  // leg->Draw();

  c2.SaveAs("figures/response.pdf");

}
