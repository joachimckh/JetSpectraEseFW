
#include "JEFW.hpp"
#include "JEfficiency.hpp"

#include <memory>
#include <iostream>

#include <TFile.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TLine.h>
#include <TLegend.h>

using std::unique_ptr;



int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <run number> <data set>" << std::endl;
  }
  const char* run = argv[1];
  const char* dataset = argv[2];
  unique_ptr<JEFW> mcjet{new JEFW(Form("/Users/joachimcarlokristianhansen/jet_analysis/hyperloop_data/%s/jet/%s/AnalysisResults.root",dataset,run),JEFW::DataType::MC,"")};
  std::vector<double> binEdges = {0,1,2,3,4,5,6,7,8,10,12,14,16,18,20,22,24,26,28,30,35,40,45,50,55,60,65,70,75,80,90,100,120,160,200};
  const int nBins = binEdges.size() - 1;
  // mcjet->JERebin(n, bin_edges);
  // auto hmatched = mcjet->getHistPt(2);
  // auto htruth = mcjet->getHistPt(1);

  auto centMCDMCP = mcjet->getSparse();
  auto htrue_pp = mcjet->getPt();  //hPartJetPtSubBkg generated

  auto jdir = mcjet->getDir();
  // auto ppGenCount = dynamic_cast<TH1F*>(jdir->Get("hMCPartEventCounter"));
  // auto ppRecCount = dynamic_cast<TH1F*>(jdir->Get("hMCDMatchedEventCounter"));
  // int lastBin_ppGen = ppGenCount->FindLastBinAbove(0);
  // int lastBin_ppRec = ppRecCount->FindLastBinAbove(0);
  // auto nNormGen = ppGenCount->GetBinContent(lastBin_ppGen);
  // auto nNormRec = ppRecCount->GetBinContent(lastBin_ppRec);
  // hResp_pp->GetAxis(0)->SetRange(hResp_pp->GetAxis(0)->FindBin(cent.at(0)), hResp_pp->GetAxis(0)->FindBin(cent.at(1)));
  auto hRecoMatch = centMCDMCP->Projection(1);

  auto hPartUnMatched{mcjet->getUnmatched(0)}; // hGen,MCP unmatched

  auto detUnmatch{mcjet->getDetReco()};
  auto hDetUnmatched = detUnmatch->ProjectionY("hDetectorRecoUnmatched"/*,detUnmatch->GetXaxis()->FindBin(cent.at(0)),detUnmatch->GetXaxis()->FindBin(cent.at(1))*/);

  // unique_ptr<JEfficiency> jeff{new JEfficiency(hDetector_pp, dresp_pp /* detector response */, hDetector_pp /* rec matched*/,  /* bkg */, hPartUnMatched /* gen unmatched */, hDetUnmatched /* rec unmatched*/)};  // for pp closure
//   TH1D* JEfficiency::Efficiency()
// {
//   /* kinematic efficiency */
//   /* N_reco-matched / N_generated */
//   TH1D* out{dynamic_cast<TH1D*>(hRecMatched->Clone("_kinematic_efficiency"))};
//   out->Divide(hGenUnmatched);
//   return out;
// };

// TH1D* JEfficiency::Purity() {
//   /* N_reco-match / N_reco-total */
//   TH1D* out{dynamic_cast<TH1D*>(hRecMatched->Clone("_purity"))};
//   out->Divide(hRecUnmatched);
//   return out;
// };

  // rebinning
  hRecoMatch = dynamic_cast<TH1D*>(hRecoMatch->Rebin(nBins, "hRecoMatchReb", binEdges.data()));
  hPartUnMatched = dynamic_cast<TH1D*>(hPartUnMatched->Rebin(nBins, "hPartUnMatchedReb", binEdges.data()));
  hDetUnmatched = dynamic_cast<TH1D*>(hDetUnmatched->Rebin(nBins, "hDetUnmatchedReb", binEdges.data()));

  auto hEfficiency = dynamic_cast<TH1*>(hRecoMatch->Clone("efficiency_clone"));
  hEfficiency->Divide(hPartUnMatched); // "Binomial errors for divide"

  auto hPurity = dynamic_cast<TH1*>(hRecoMatch->Clone("purity_clone"));
  hPurity->Divide(hDetUnmatched);

  TCanvas c1("c1","",800,600);
  c1.cd();

  hEfficiency->SetTitle("");
  hEfficiency->GetYaxis()->SetTitle("Efficiency");
  hEfficiency->GetXaxis()->SetTitle("p_{T,jet} (GeV/c)");

  hEfficiency->SetStats(0);
  hEfficiency->SetLineColor(kBlack);
  hEfficiency->Draw("hist");

  hEfficiency->GetXaxis()->SetRangeUser(0, 100);
  // hEfficiency->GetYaxis()->SetRangeUser(0, 1.1);


  TLine *line = new TLine(0, 1, 100, 1);
  line->SetLineStyle(2);
  line->Draw();

  auto leg = new TLegend(0.6,0.2,0.8,0.4);
  leg->AddEntry(hEfficiency,Form("%s",dataset),"l");
  leg->Draw();

  c1.SaveAs(Form("JFigures/unfold/efficiency_%s.pdf",dataset));


  TCanvas c2("c2","",800,600);
  c2.cd();

  hPurity->SetTitle("");
  hPurity->GetYaxis()->SetTitle("Purity");
  hPurity->GetXaxis()->SetTitle("p_{T,jet} (GeV/c)");

  hPurity->SetStats(0);
  hPurity->SetLineColor(kBlack);
  hPurity->Draw("hist");

  hPurity->GetXaxis()->SetRangeUser(0, 100);
  hPurity->GetYaxis()->SetRangeUser(0, 1.1);

  TLine *line2 = new TLine(0, 1, 100, 1);
  line2->SetLineStyle(2);
  line2->Draw();


  auto leg2 = new TLegend(0.6,0.2,0.8,0.4);
  leg2->AddEntry(hPurity,Form("%s",dataset),"l");
  leg2->Draw();

  c2.SaveAs(Form("JFigures/unfold/purity_%s.pdf",dataset));

}
