#include <iostream>
#include <vector>
#include <string>
#include <memory>

#include <TFile.h>
#include <THnSparse.h>
#include <TH1.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TLine.h>

#include "../JAnalysis/rootStyle.hpp"

#include "JEFW.hpp"

void analyzeRhoPhi(std::vector<const char*> const& paths, std::vector<int> const& colors, std::vector<std::string> const& labels);

int main(int argc, char* argv[]) {
  if (argc < 3) {
    std::cerr << "Usage: " << argv[0] << " <path1> <label1> <path2> <label2> ... " << std::endl;
    return 1;
  }
  
  rootStyle();
  
  std::vector<const char*> paths;
  std::vector<TFile*> files;
  std::vector<std::string> labels;
  std::vector<TH1D*> histograms;
  std::vector<TH1D*> histogramsRhoPhi;
  
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
    paths.push_back(argv[i]);
  }
  
  
  std::vector<int> colors = {kOrange - 1, kCyan + 2, kRed, kGreen + 2, kBlue, kMagenta};

  analyzeRhoPhi(paths, colors, labels);
  
  for (size_t i = 0; i < files.size(); ++i) {
    auto dir = dynamic_cast<TDirectory*>(files[i]->Get("jet-spectra-ese-task"));
    if (!dir) {
      std::cerr << "Directory 'jet-spectra-ese-task' not found in file: " << files[i]->GetName() << std::endl;
      continue;
    }
    
    auto hSparse = dynamic_cast<THnSparse*>(dir->Get("hCentJetPtdPhiq2"));
    auto hSparseRhoPhi = dynamic_cast<THnSparse*>(dir->Get("hCentJetPtdPhiq2RhoPhi"));
    auto hEvents = dynamic_cast<TH1F*>(dir->Get("hEventCounter"));
    if (!hSparse || !hEvents) {
      std::cerr << "Histograms not found in file: " << files[i]->GetName() << std::endl;
      continue;
    }
    
    double scale = hEvents->GetBinContent(7);
    hSparse->GetAxis(0)->SetRange(hSparse->GetAxis(0)->FindBin(30), hSparse->GetAxis(0)->FindBin(50));
    hSparseRhoPhi->GetAxis(0)->SetRange(hSparseRhoPhi->GetAxis(0)->FindBin(30), hSparseRhoPhi->GetAxis(0)->FindBin(50));
    auto hPt = hSparse->Projection(1);
    auto hPtRhoPhi = hSparseRhoPhi->Projection(1);
    hPt->SetName(Form("#it{p}_{T} #rho %zu", i));
    hPtRhoPhi->SetName(Form("#it{p}_{T} #rho(#phi) %zu", i));
    hPt->Scale(1. / scale, "width");
    hPtRhoPhi->Scale(1. / scale, "width");
    
    hPt->SetLineColor(colors[i % colors.size()]);
    hPt->SetMarkerStyle(20 + i);
    hPt->SetMarkerColor(colors[i % colors.size()]);
    hPt->SetStats(0);

    hPtRhoPhi->SetLineColor(colors[i % colors.size()]);
    hPtRhoPhi->SetMarkerStyle(24 + i);
    hPtRhoPhi->SetMarkerColor(colors[i % colors.size()]);
    hPtRhoPhi->SetStats(0);
    
    histograms.push_back(hPt);
    histogramsRhoPhi.push_back(hPtRhoPhi);
  }
  
  if (histograms.empty()) {
    std::cerr << "No valid histograms extracted." << std::endl;
    return 1;
  }
  
  auto c = new TCanvas("c", "", 800, 600);
  c->cd();
  gPad->SetLogy();
  
  histograms[0]->GetYaxis()->SetTitle("#frac{1}{#it{N}_{ev}} #frac{d^{2}N}{d#it{p}_{T}d#it{#eta}}");
  histograms[0]->SetMinimum(1e-8);
  histograms[0]->SetMaximum(10);
  histograms[0]->Draw("EP");
  
  if (histograms.size() > 1) {
    for (size_t i = 1; i < histograms.size(); ++i) {
      histograms[i]->Draw("EPsame");
    }
  }

  for (const auto& hist : histogramsRhoPhi ) {
    hist->Draw("EPsame");
  }
  
  auto leg = new TLegend(0.45, 0.55, 0.85, 0.85);
  for (size_t i = 0; i < histograms.size(); ++i) {
    leg->AddEntry(histograms[i], histograms[i]->GetName(), "lep");
  }
  for (size_t i = 0; i < histogramsRhoPhi.size(); ++i) {
    leg->AddEntry(histogramsRhoPhi[i], histogramsRhoPhi[i]->GetName(), "lep");
  }

  leg->Draw();

  
  c->SaveAs("JFigures/comparisons/compare_spectra_rhophi.pdf");



    
}


void analyzeRhoPhi(std::vector<const char*> const& paths, std::vector<int> const& colors, std::vector<std::string> const& labels) {

  std::vector<int> q2{0,100};
  std::vector<int> cent{30,50};
  std::vector<TH1F*> histograms;
  std::vector<TH1F*> histogramsRhoPhi;
  std::vector<TH1F*> histogramsOut;
  std::vector<TH1F*> histogramsRhoPhiOut;

  for (size_t i = 0; i < paths.size(); ++i) {
    auto jet = new JEFW(paths[i], JEFW::DataType::DATA, "");
    jet->setCentrality(cent);
    jet->setCentrality<true>(cent);
    // jet->setq2Range(q2);
    // auto p = jet->getSparse2();
    // auto xbin = p->GetAxis(0)->FindBin(30);
    // auto ybin = p->GetAxis(0)->FindBin(50);
    // p->GetAxis(0)->SetRange(xbin,ybin);
    TObjArray* h = jet->separatePlanes<false>();
    TObjArray* hRhoPhi = jet->separatePlanes<true>("rhoPhi");


    auto hPt = dynamic_cast<TH1F*>(h->FindObject("hv_0"));
    auto hPtRhoPhi = dynamic_cast<TH1F*>(hRhoPhi->FindObject("hv_0rhoPhi"));
    auto hPtOut = dynamic_cast<TH1F*>(h->FindObject("hv_1"));
    auto hPtRhoPhiOut = dynamic_cast<TH1F*>(hRhoPhi->FindObject("hv_1rhoPhi"));
    
    double scale = jet->eventCounter(7);
    hPt->SetName(Form("#it{p}_{T}^{in} #rho %zu", i));
    hPtOut->SetName(Form("#it{p}_{T}^{out} #rho %zu", i));
    hPtRhoPhi->SetName(Form("#it{p}_{T}^{in} #rho(#phi) %zu", i));
    hPtRhoPhiOut->SetName(Form("#it{p}_{T}^{out} #rho(#phi) %zu", i));
    hPt->Scale(1. / scale );
    hPtOut->Scale(1. / scale );
    hPtRhoPhi->Scale(1. / scale );
    hPtRhoPhiOut->Scale(1. / scale );
    hPt->SetLineColor(colors[i % colors.size()]);
    hPt->SetMarkerStyle(20 + i);
    hPt->SetMarkerColor(colors[i % colors.size()]);
    hPt->SetStats(0);
    hPtRhoPhi->SetLineColor(colors[i % colors.size()+1]);
    hPtRhoPhi->SetMarkerStyle(24 + i);
    hPtRhoPhi->SetMarkerColor(colors[i % colors.size()+1]);
    hPtRhoPhi->SetStats(0);
    hPtOut->SetLineColor(colors[i % colors.size()]);
    hPtOut->SetMarkerStyle(20 + i);
    hPtOut->SetMarkerColor(colors[i % colors.size()]);
    hPtOut->SetStats(0);
    hPtRhoPhiOut->SetLineColor(colors[i % colors.size() + 1]);
    hPtRhoPhiOut->SetMarkerStyle(24 + i);
    hPtRhoPhiOut->SetMarkerColor(colors[i % colors.size() + 1]);
    hPtRhoPhiOut->SetStats(0);
    
    histograms.push_back(hPt);
    histogramsOut.push_back(hPtOut);
    histogramsRhoPhi.push_back(hPtRhoPhi);
    histogramsRhoPhiOut.push_back(hPtRhoPhiOut);
  }
  auto c1 = new TCanvas("c1", "", 1400, 600);
  c1->Divide(2,1);
  c1->cd(1);
  gPad->SetLogy();
  histograms.at(0)->GetYaxis()->SetTitle("#frac{1}{#it{N}_{ev}} #frac{d^{2}N}{d#it{p}_{T}d#it{#eta}}");
  histograms.at(0)->SetMinimum(1e-8);
  histograms.at(0)->SetMaximum(10);
  histograms.at(0)->GetXaxis()->SetRangeUser(-50, 200);
  histograms.at(0)->Draw("EP");
  if (histograms.size() > 1) {
    for (size_t i = 1; i < histograms.size(); ++i) {
      histograms[i]->Draw("EPsame");
    }
  }

  for (const auto& hist : histogramsRhoPhi ) {
    hist->Draw("EPsame");
  }
  
  auto leg1 = new TLegend(0.45, 0.55, 0.85, 0.85);
  for (size_t i = 0; i < histograms.size(); ++i) {
    leg1->AddEntry(histograms[i], histograms[i]->GetName(), "lep");
  }
  for (size_t i = 0; i < histogramsRhoPhi.size(); ++i) {
    leg1->AddEntry(histogramsRhoPhi[i], histogramsRhoPhi[i]->GetName(), "lep");
  }
  leg1->Draw();

  c1->cd(2);
  gPad->SetLogy();
  histogramsOut.at(0)->GetYaxis()->SetTitle("#frac{1}{#it{N}_{ev}} #frac{d^{2}N}{d#it{p}_{T}d#it{#eta}}");
  histogramsOut.at(0)->SetMinimum(1e-8);
  histogramsOut.at(0)->SetMaximum(10);
  histogramsOut.at(0)->GetXaxis()->SetRangeUser(-50, 200);
  histogramsOut.at(0)->Draw("EP");
  if (histogramsOut.size() > 1) {
    for (size_t i = 1; i < histograms.size(); ++i) {
      histogramsOut[i]->Draw("EPsame");
    }
  }

  for (const auto& hist : histogramsRhoPhiOut ) {
    hist->Draw("EPsame");
  }
  
  auto leg2 = new TLegend(0.45, 0.55, 0.85, 0.85);
  for (size_t i = 0; i < histogramsOut.size(); ++i) {
    leg2->AddEntry(histogramsOut[i], histogramsOut[i]->GetName(), "lep");
  }
  for (size_t i = 0; i < histogramsRhoPhi.size(); ++i) {
    leg2->AddEntry(histogramsRhoPhiOut[i], histogramsRhoPhiOut[i]->GetName(), "lep");
  }
  leg2->Draw();

    
  c1->SaveAs("JFigures/comparisons/compare_spectra_rhophi_in.pdf");

  auto c2 = new TCanvas("c2", "", 1400, 600);
  c2->Divide(2,1);
  c2->cd(1);

  auto hRat = dynamic_cast<TH1F*>(histograms.at(0)->Clone("hRat"));
  hRat->Divide(histogramsRhoPhi.at(0));

  hRat->SetMarkerStyle(20);
  hRat->SetMarkerColor(kBlack);
  hRat->SetLineColor(kBlack);
  hRat->GetYaxis()->SetTitle("Ratio");
  hRat->GetYaxis()->SetRangeUser(0, 2);
  hRat->GetXaxis()->SetRangeUser(-50, 200);
  hRat->Draw("EP");

  auto line = new TLine(-50, 1, 200, 1);
  line->SetLineStyle(2);
  line->SetLineColor(kRed);
  line->Draw();

  c2->cd(2);

  auto hRatOut = dynamic_cast<TH1F*>(histogramsOut.at(0)->Clone("hRatOut"));
  hRatOut->Divide(histogramsRhoPhiOut.at(0));

    
  hRatOut->SetMarkerStyle(20);
  hRatOut->SetMarkerColor(kBlack);
  hRatOut->SetLineColor(kBlack);
  hRatOut->GetYaxis()->SetTitle("Ratio");
  hRatOut->GetYaxis()->SetRangeUser(0., 2);
  hRatOut->GetXaxis()->SetRangeUser(-50, 200);
  hRatOut->Draw("EP");

  line->Draw();

  c2->SaveAs("JFigures/comparisons/compare_spectra_rhophi_ratio.pdf");

}