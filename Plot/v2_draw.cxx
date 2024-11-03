


void v2_draw()
{
    TFile *f = new TFile("/Users/joachimcarlokristianhansen/jet_analysis/localr/HEPData/HEPData-ins1394678-v1-root.root","READ");
    TH1 *h = (TH1*)f->Get("Table 2/Hist1D_y1");
    h->Draw();

    TFile *f2 = new TFile("root_files/v2Jet.root","READ");
    TH1 *h2 = (TH1*)f2->Get("hV2Jet");


    TCanvas c1("c1","",800,600);
    c1.cd();

    h2->SetLineColor(kBlack);
    h2->GetXaxis()->SetRangeUser(20,90);
    // int n = 5;
    // Double_t bin_edges[6]={20,30,40,50,60,80};
    // TH1* h2Reb = h2->Rebin(n, "h2Reb", bin_edges);
    h2->Draw();

    h->SetLineColor(kRed);
    h->Draw("same");

    

    TLegend leg(0.7,0.7,0.9,0.9);
    leg.AddEntry(h,"HEPData","l");
    leg.AddEntry(h2,"v2Jet","l");
    leg.Draw();

    c1.SaveAs("figures/v2Jet.pdf");
}

