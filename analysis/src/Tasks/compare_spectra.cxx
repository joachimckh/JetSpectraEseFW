#include <iostream>
#include <vector>
#include <string>
#include <TFile.h>
#include <THnSparse.h>
#include <TH1.h>
#include <TCanvas.h>
#include <TLegend.h>
#include "../JAnalysis/rootStyle.hpp"

int main(int argc, char* argv[]) {
  if (argc < 5 || (argc % 2) == 0) {
    std::cerr << "Usage: " << argv[0] << " <path1> <label1> <path2> <label2> ... " << std::endl;
    return 1;
  }
  
  rootStyle();
  
  std::vector<TFile*> files;
  std::vector<std::string> labels;
  std::vector<TH1D*> histograms;
  
  for (int i = 1; i < argc; i += 2) {
    std::string filePath = argv[i];
    std::string label = argv[i + 1];
    TFile* file = TFile::Open(filePath.c_str(), "READ");
    if (!file || file->IsZombie()) {
        std::cerr << "Error opening file: " << filePath << std::endl;
        continue;
    }
    files.push_back(file);
    labels.push_back(label);
  }
  
  if (files.size() < 2) {
    std::cerr << "At least two valid ROOT files are required." << std::endl;
    return 1;
  }
  
  std::vector<int> colors = {kOrange - 1, kCyan + 2, kRed, kGreen + 2, kBlue, kMagenta};
  
  for (size_t i = 0; i < files.size(); ++i) {
    auto dir = dynamic_cast<TDirectory*>(files[i]->Get("jet-spectra-ese-task"));
    if (!dir) {
      std::cerr << "Directory 'jet-spectra-ese-task' not found in file: " << files[i]->GetName() << std::endl;
      continue;
    }
    
    auto hSparse = dynamic_cast<THnSparse*>(dir->Get("hCentJetPtdPhiq2"));
    auto hEvents = dynamic_cast<TH1F*>(dir->Get("hEventCounter"));
    if (!hSparse || !hEvents) {
      std::cerr << "Histograms not found in file: " << files[i]->GetName() << std::endl;
      continue;
    }
    
    double scale = hEvents->GetBinContent(7);
    hSparse->GetAxis(0)->SetRange(hSparse->GetAxis(0)->FindBin(30), hSparse->GetAxis(0)->FindBin(50));
    auto hPt = hSparse->Projection(1);
    hPt->SetName(Form("h_pt_%zu", i));
    hPt->Scale(1. / scale, "width");
    
    hPt->SetLineColor(colors[i % colors.size()]);
    hPt->SetMarkerStyle(20 + i);
    hPt->SetMarkerColor(colors[i % colors.size()]);
    hPt->SetStats(0);
    
    histograms.push_back(hPt);
  }
  
  if (histograms.empty()) {
    std::cerr << "No valid histograms extracted." << std::endl;
    return 1;
  }
  
  auto c = new TCanvas("c", "", 800, 600);
  c->cd();
  gPad->SetLogy();
  
  histograms[0]->GetYaxis()->SetTitle("#frac{1}{#it{N}_{ev}} #frac{d^{2}N}{d#it{p}_{T}d#it{#eta}}");
  histograms[0]->Draw("EP");
  
  for (size_t i = 1; i < histograms.size(); ++i) {
    histograms[i]->Draw("EPsame");
  }
  
  auto leg = new TLegend(0.45, 0.55, 0.85, 0.85);
  for (size_t i = 0; i < histograms.size(); ++i) {
    leg->AddEntry(histograms[i], labels[i].c_str(), "lep");
  }

  // load yubiao's here:
  // auto f = TFile::Open("/Users/joachimcarlokristianhansen/jet_analysis/localr/v2_yubiao/jet_v2_in-out-of-plane_comp_normalized_to_event.root", "READ");
  // auto hYOut = dynamic_cast<TH1D*>(f->Get("jet_pt_out-of-plane_30-50cent_r(0.2)"));
  // hYOut->GetXaxis()->SetRange(hYOut->GetXaxis()->FindBin(0.001), hYOut->GetXaxis()->FindBin(200));
  // auto hYin = dynamic_cast<TH1D*>(f->Get("jet_pt_in-plane_30-50cent_r(0.2)"));
  // hYin->GetXaxis()->SetRange(hYin->GetXaxis()->FindBin(0.001), hYin->GetXaxis()->FindBin(200));
  // hYin->Add(hYOut);
  // hYin->SetLineColor(kBlack);
  // hYin->SetMarkerStyle(20);
  // hYin->SetMarkerColor(kBlack);
  // hYin->Scale(3);
  // hYin->Draw("same");
  // leg->AddEntry(hYin, "Yubiao in+out", "lep");

  leg->Draw();

  
  c->SaveAs("JFigures/comparisons/compare_spectra.pdf");
    
}