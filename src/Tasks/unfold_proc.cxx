#include "JEFW.hpp"
#include "JEfficiency.hpp"


#include <memory>
#include <iostream>

#include <TCanvas.h>
#include <TLegend.h>
#include <TLatex.h>
#include <TH1.h>
#include <TF1.h>

#include "../JAnalysis/rootStyle.hpp"

using std::unique_ptr;

void drawEfficiency(TH1* eff, TH1* pur, const char* identifier);
void drawResponses(TH2D* hRespTotal, TH2D* hRespFlucts, TH2D* hRespDetector, const char* identifier,
                    const char* periodFlucts, const char* periodDetector);
void plotSvdBayes(TH1* hSvd, TH1* hBayes2, float nNorm, const char* identifier);
void plotUnfolded(TH1* hGen, TH1* hReco, TH1* hUnfolded, float nNormGen, float nNormRec, const char* identifier);
void drawBkgfluctuations(TH2* hist, std::vector<int> const& cent);

int main(int argc, char *argv[]){
  rootStyle();
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << "<run number> <data size> <mc run number> <mc size> <bkg run> <bkg size> <mc pp run> <mc pp dataset> <R> <iter Bayes> <iter Svd>" << std::endl;
  }
  const char* dataRun{argv[1]};
  const char* dataSize{argv[2]};
  const char* mcRun{argv[3]}; 
  const char* mcSize{argv[4]};
  const char* bkgRun{argv[5]}; 
  const char* bkgSize{argv[6]};
  const char* mcPPRun{argv[7]};
  const char* mcPPDataset{argv[8]};
  const char* Rval{argv[9]};
  const int iterBayes{std::stoi(argv[10])};
  const int iterSvd{std::stoi(argv[11])};

  unique_ptr<JEFW> jet{new JEFW(Form("/Users/joachimcarlokristianhansen/jet_analysis/hyperloop_data/%s/jet/%s/AnalysisResults.root",dataSize, dataRun),JEFW::DataType::DATA,"")};
  std::vector<int> q2{0,100};
  std::vector<int> cent{30,50};
  jet->setCentrality(cent);
  jet->setq2Range(q2);
  TObjArray* objarr = jet->separatePlanes();
  // doing inclusive initially
  auto dataHistSparse{jet->getSparse()};
  auto xbin = dataHistSparse->GetAxis(0)->FindBin(cent.at(0));
  auto ybin = dataHistSparse->GetAxis(0)->FindBin(cent.at(1));
  dataHistSparse->GetAxis(0)->SetRange(xbin,ybin);
  auto dataHist{dataHistSparse->Projection(1)};

  // mc PbPb
  unique_ptr<JEFW> mcjet{new JEFW(Form("/Users/joachimcarlokristianhansen/jet_analysis/hyperloop_data/%s/jet/%s/AnalysisResults.root",mcSize,mcRun),JEFW::DataType::MC,"")};
  auto resp = mcjet->getSparse();
  auto hGeneratedPbPb{mcjet->getUnmatched(0,&cent)}; // hGen,MCP unmatched
  auto detUnmatchPb{mcjet->getDetReco()};
  auto hDetUnmatchedPb = detUnmatchPb->ProjectionY("hDetectorRecoUnmatched",detUnmatchPb->GetXaxis()->FindBin(cent.at(0)),detUnmatchPb->GetXaxis()->FindBin(cent.at(1)));
  auto respClone = dynamic_cast<THnSparse*>(resp->Clone("hRespMcDMcPMatchSubBkgClone"));
  respClone->GetAxis(0)->SetRange(respClone->GetAxis(0)->FindBin(cent.at(0)), respClone->GetAxis(0)->FindBin(cent.at(1)));
  auto detMatch = respClone->Projection(1);

  auto PbDir = mcjet->getDir();
  auto PbGenCount = dynamic_cast<TH1F*>(PbDir->Get("hMCPartEventCounter"));
  auto PbRecCount = dynamic_cast<TH1F*>(PbDir->Get("hMCDMatchedEventCounter"));
  int lastBin_PbGen = PbGenCount->FindLastBinAbove(0);
  int lastBin_PbRec = PbRecCount->FindLastBinAbove(0);
  auto nNormGenPb = PbGenCount->GetBinContent(lastBin_PbGen);
  auto nNormRecPb = PbRecCount->GetBinContent(lastBin_PbRec);

  // mc pp
  unique_ptr<JEFW> mcppJet{new JEFW(Form("/Users/joachimcarlokristianhansen/jet_analysis/hyperloop_data/%s/jet/%s/AnalysisResults.root",mcPPDataset,mcPPRun),JEFW::DataType::MC,"")};
  auto dresp_pp = mcppJet->getSparse();
  auto htrue_pp = mcppJet->getPt(); 

  auto ppDir = mcppJet->getDir();
  auto ppGenCount = dynamic_cast<TH1F*>(ppDir->Get("hMCPartEventCounter"));
  auto ppRecCount = dynamic_cast<TH1F*>(ppDir->Get("hMCDMatchedEventCounter"));
  int lastBin_ppGen = ppGenCount->FindLastBinAbove(0);
  int lastBin_ppRec = ppRecCount->FindLastBinAbove(0);
  auto nNormGen = ppGenCount->GetBinContent(lastBin_ppGen);
  auto nNormRec = ppRecCount->GetBinContent(lastBin_ppRec);
  auto hResp_pp = dynamic_cast<THnSparse*>(ppDir->Get("hRespMcDMcPMatchSubBkg"));
  // hResp_pp->GetAxis(0)->SetRange(hResp_pp->GetAxis(0)->FindBin(cent.at(0)), hResp_pp->GetAxis(0)->FindBin(cent.at(1)));
  auto hDetector_pp = hResp_pp->Projection(1);

  auto hPartUnMatched{mcppJet->getUnmatched(0)}; // hGen,MCP unmatched
  auto detUnmatch{mcppJet->getDetReco()};
  auto hDetUnmatched = detUnmatch->ProjectionY("hDetectorRecoUnmatched"/*,detUnmatch->GetXaxis()->FindBin(cent.at(0)),detUnmatch->GetXaxis()->FindBin(cent.at(1))*/);


  // background-task
  TFile *fBackground = new TFile(Form("/Users/joachimcarlokristianhansen/jet_analysis/hyperloop_data/%s/jet/%s/AnalysisResults.root",bkgSize,bkgRun),"READ");
  TDirectory* tmpDir{dynamic_cast<TDirectory*>(fBackground->Get("jet-background-analysis"))};
  TH2F* hCentRhoRandomConewoLeadingJet = dynamic_cast<TH2F*>(tmpDir->Get("h2_centrality_rhorandomconewithoutleadingjet"));
  drawBkgfluctuations(hCentRhoRandomConewoLeadingJet, cent);
  
  
  
  unique_ptr<JEfficiency> jeff_pp{new JEfficiency(hDetector_pp, dresp_pp /* detector response */, hDetector_pp /* rec matched*/, hCentRhoRandomConewoLeadingJet /* bkg */, hPartUnMatched /* gen unmatched */, hDetUnmatched /* rec unmatched*/)};  // for pp closure
  jeff_pp->setRespSettings();
  jeff_pp->createResponseMatrix();


  unique_ptr<JEfficiency> jeff{new JEfficiency(detMatch, resp /* detector response */, detMatch /* rec matched*/, hCentRhoRandomConewoLeadingJet /* bkg */, hPartUnMatched /*hGeneratedPbPb gen unmatched */, hDetUnmatched /* hDetUnmatchedPb: rec unmatched*/)};  // for PbPb closure
  jeff->setPbPb(true,std::make_pair(30,50));
  jeff->setRespSettings();
  jeff->createResponseMatrix();
  


  auto respI = jeff_pp->createResponse(true);
  TCanvas cRooResp("cRooResp","",800,600);
  cRooResp.cd();

  respI->Hresponse()->Draw("colz");
  cRooResp.SaveAs("JFigures/unfold/responseRooUnf_pp.pdf");


  auto hBayes{jeff_pp->bayesUnfold(iterBayes,false)};
  auto hSvd{jeff_pp->svdUnfold(iterSvd)}; 
  auto hBayes2{jeff_pp->bayesUnfold(iterBayes,true)};

  auto hRespDetector_pp{jeff_pp->getResponseDetector()};
  auto hRespFlucts_pp{jeff_pp->getFluctuations()};
  auto hRespTotal_pp{jeff_pp->getResponseTotal()};
  drawResponses(hRespTotal_pp, hRespFlucts_pp, hRespDetector_pp, "pp", mcPPDataset, mcPPDataset);
  auto efficiency{jeff_pp->Efficiency()};
  auto purity{jeff_pp->Purity()};
  drawEfficiency(efficiency, purity, "pp");
  plotUnfolded(hPartUnMatched, hDetUnmatched, hBayes, nNormGen, nNormRec, "pp");
  plotSvdBayes(hSvd, hBayes2, nNormGen, "pp");
  

  auto hBayesLead{jeff->bayesUnfold(iterBayes,true)};
  plotUnfolded(hGeneratedPbPb, detMatch, hBayesLead, nNormGenPb, nNormRecPb, "PbPb");

  /* for PbPb */
  auto hRespDetector{jeff->getResponseDetector()};
  auto hRespFlucts{jeff->getFluctuations()};
  auto hRespTotal{jeff->getResponseTotal()};
  drawResponses(hRespTotal, hRespFlucts, hRespDetector, "PbPb", bkgSize, mcSize);

  auto hSvdPb{jeff->svdUnfold(iterSvd)}; 
  plotSvdBayes(hSvdPb, hBayesLead, nNormGenPb, "PbPb");
  TCanvas fuckyi("hest","",1400,600);
  fuckyi.Divide(2,1);
  fuckyi.cd(1);
  hSvdPb->Draw("EP");
  fuckyi.cd(2);
  hBayesLead->Draw("EP");
  fuckyi.SaveAs("JFigures/unfold/svd_hest.pdf");

  auto respI2 = jeff->createResponse(true);
  TCanvas cRooResp2("cRooResp2","",800,600);
  cRooResp2.cd();

  respI2->Hresponse()->Draw("colz");
  cRooResp2.SaveAs("JFigures/unfold/responseRooUnf_PbPb.pdf");

  return 0;
}

void plotUnfolded(TH1* hGen, TH1* hReco, TH1* hUnfolded, float nNormGen, float nNormRec, const char* identifier) {
  TCanvas c1("c1","c1",800,600);
  c1.cd();

  gPad->SetLogy();

  hReco->Scale(1 / nNormRec,"width");
  hReco->SetTitle("");
  hReco->SetStats(0);
  hReco->SetLineColor(kBlack);
  hReco->SetMarkerStyle(20);
  hReco->SetMarkerColor(kBlack);
  
  hUnfolded->Scale(1 / nNormGen,"width");
  hUnfolded->Scale(1,"width");
  hUnfolded->SetTitle("");
  hUnfolded->SetStats(0);
  hUnfolded->SetLineColor(kRed+1);
  hUnfolded->SetMarkerStyle(20);
  hUnfolded->SetMarkerColor(kRed+1);

  hGen->Scale(1 / nNormGen,"width");
  // std::vector<double> binEdges = {0,1,2,3,4, 5,6,7,8,9, 10,12,14,16,18, 20, 25, 30, 40, 50};//, 55, 60, 65, 70, 75, 80, 90, 100, 120};//, 140, 160, 200};
  // const int nBins = binEdges.size() - 1;
  // hPartUnMatched = dynamic_cast<TH1D*>(hPartUnMatched->Rebin(nBins, "hGenRebinned", binEdges.data()));
  hGen->Scale(1,"width");
  hGen->SetLineColor(kGreen);
  hGen->SetMarkerStyle(25);
  hGen->SetMarkerColor(kGreen);

  hUnfolded->GetXaxis()->SetRangeUser(0,120);
  hUnfolded->GetXaxis()->SetTitle("p_{T,jet}");
  hUnfolded->Draw("EP");


  hGen->Draw("EPsame");
  hReco->Draw("EPsame");

  auto leg = new TLegend(0.55, 0.65, 0.86, 0.85);
  leg->AddEntry(hReco, "Reconstructed", "lep");
  leg->AddEntry(hUnfolded, "Reconstructed (Bayes)", "lep");
  leg->AddEntry(hGen, "Generated", "lep");
  leg->Draw();

  c1.SaveAs(Form("JFigures/unfold/RecoGenUnfolded_%s.pdf",identifier));

  TCanvas c1ratio("c1ratio","c1ratio",800,200);
  c1ratio.cd();

  TH1D* hRatio = dynamic_cast<TH1D*>(hUnfolded->Clone(Form("hRatio_%s",identifier)));
  hRatio->Divide(hGen);
  hRatio->SetTitle("");
  hRatio->SetStats(0);
  hRatio->SetLineColor(kBlack);
  hRatio->SetMarkerStyle(20);
  hRatio->SetMarkerColor(kBlack);
  hRatio->GetYaxis()->SetTitle("Unfolded/Generated");
  hRatio->GetYaxis()->SetRangeUser(0.7,1.3);
  hRatio->GetXaxis()->SetRangeUser(0,50);
  hRatio->Draw("EP");

  c1ratio.SaveAs(Form("JFigures/unfold/ratio_unfolded_with_generated_%s.pdf",identifier));
}

void plotSvdBayes(TH1* hSvd, TH1* hBayes2, float nNorm, const char* identifier) {
  TCanvas c2("c2","c2",800,600);
  c2.cd();

  gPad->SetLogy();

  hSvd->GetXaxis()->SetTitle("p_{T,jet}");
  hSvd->Scale(1 / nNorm,"width");
  hSvd->SetStats(0);
  hSvd->SetLineColor(kBlack);
  hSvd->SetMarkerStyle(20);
  hSvd->SetMarkerColor(kBlack);

  hBayes2->Scale(1 / nNorm,"width");
  hBayes2->SetStats(0);
  hBayes2->SetLineColor(kRed+1);
  hBayes2->SetMarkerStyle(24);
  hBayes2->SetMarkerColor(kRed+1);

  hSvd->Draw("EP");
  hBayes2->Draw("EPsame");

  auto leg2 = new TLegend(0.55, 0.65, 0.86, 0.85);
  leg2->AddEntry(hSvd, "Reconstructed (SVD)", "lep");
  leg2->AddEntry(hBayes2, "Reconstructed (Bayes)", "lep");
  leg2->Draw();

  c2.SaveAs(Form("JFigures/unfold/both_unfolded_%s.pdf",identifier));

  TCanvas c2ratio("c2ratio","c2ratio",800,200);
  c2ratio.cd();

  TH1D* hRatio2 = dynamic_cast<TH1D*>(hBayes2->Clone("hRatio2"));
  hRatio2->Divide(hSvd);

  hRatio2->SetStats(0);
  hRatio2->GetXaxis()->SetTitle("p_{T,jet}");
  hRatio2->GetYaxis()->SetTitle("Bayes/SVD");
  hRatio2->SetLineColor(kBlack);
  hRatio2->SetMarkerStyle(20);
  hRatio2->SetMarkerColor(kBlack);
  hRatio2->GetYaxis()->SetRangeUser(0.6,1.4);
  hRatio2->Draw("EP");

  c2ratio.SaveAs(Form("JFigures/unfold/bayes_divided_svd_%s.pdf",identifier));
}

void drawSingularResponse(TH2* hResp, const char* period, const char* save_name, std::pair<float,float> range, const char* id) {
  
  TCanvas c(Form("c%s",save_name),"",800,600);
  c.cd();
  gPad->SetLogz();

  hResp->SetMinimum(range.first);
  hResp->SetMaximum(range.second);
  hResp->SetContour(100);

  hResp->SetTitle(Form("%s",save_name));
  hResp->GetXaxis()->SetTitle("#it{p}_{T,jet}^{rec}");
  hResp->GetYaxis()->SetTitle("#it{p}_{T,jet}^{gen}");
  hResp->SetStats(0);
  hResp->Draw("colz");

  TLatex lat;
  lat.SetTextSize(0.04);  
  // lat.DrawLatexNDC(.2, 0.8, "Pb--Pb #sqrt{s_{NN}} = 5.36 TeV");
  lat.DrawLatexNDC(.2, 0.75, Form("%s",period));

  c.Update();
  c.SaveAs(Form("JFigures/unfold/response_%s_%s.pdf",save_name, id));
}

void drawResponses(TH2D* hRespTotal, TH2D* hRespFlucts, TH2D* hRespDetector, const char* identifier,
                    const char* periodFlucts, const char* periodDetector) {
  drawSingularResponse(hRespTotal, Form("%s",periodDetector), "Total", std::make_pair(1e-17,1e5), identifier);
  drawSingularResponse(hRespFlucts, Form("%s",periodFlucts), "Fluctuations", std::make_pair(1e-10,1e0), identifier);
  drawSingularResponse(hRespDetector, Form("%s",periodDetector), "Detector", std::make_pair(1e-8,1e5), identifier);
}

void drawEfficiency(TH1* eff, TH1* pur, const char* identifier) {
  TCanvas cEff("cEff","cEff",1400,600);
  cEff.Divide(2,1);
  cEff.cd(1);
  eff->SetStats(0);
  eff->GetYaxis()->SetTitle("Efficiency");
  eff->SetLineColor(kBlack);
  eff->SetMarkerStyle(20);
  eff->SetMarkerColor(kBlack);
  // eff->GetYaxis()->SetRangeUser(0,1.2);
  eff->Draw("hist");
  cEff.cd(2);
  pur->SetStats(0);
  pur->GetYaxis()->SetTitle("Purity");
  pur->SetLineColor(kRed);
  pur->SetMarkerStyle(20);
  pur->SetMarkerColor(kRed);
  pur->GetYaxis()->SetRangeUser(0,1.2);
  pur->Draw("hist");
  cEff.SaveAs(Form("JFigures/unfold/efficiency_%s.pdf",identifier));
}


void drawBkgfluctuations(TH2* hist, std::vector<int> const& cent) {

  auto pr = hist->ProjectionY("bkgProj__", hist->GetXaxis()->FindBin(30), hist->GetXaxis()->FindBin(50));

  TCanvas cBkg("cBkg","cBkg",800,600);
  cBkg.cd();
  gPad->SetLogy();

  pr->SetLineColor(kBlack);
  pr->SetMarkerStyle(20);
  pr->SetMarkerColor(kBlack);
  pr->SetStats(0);

  pr->GetXaxis()->SetRangeUser(-40,80);
  pr->Scale(1./pr->Integral("width"));
  pr->GetYaxis()->SetTitle("Probability density");
  pr->GetXaxis()->SetTitle("#it{p}_{T, Random cone} - #it{A}_{Random cone} * #rho");
  pr->Draw("EP");

  TF1* fitGauss = new TF1("fitGauss", "gaus", -20, 60);
  TFitResultPtr fitResult = pr->Fit(fitGauss, "SN");

  fitGauss->SetLineColor(kRed);
  fitGauss->SetLineWidth(2);
  fitGauss->SetLineStyle(2);

  fitGauss->Draw("same");
  double mean = fitGauss->GetParameter(1);
  double sigma = fitGauss->GetParameter(2);

  TLatex latex;
  latex.SetTextSize(0.04);
  latex.DrawLatexNDC(0.5, 0.8, "Pb--Pb #sqrt{s_{NN}} = 5.36 TeV");
  latex.DrawLatexNDC(0.5, 0.75, Form("Centrality: %i-%i%%", cent.at(0), cent.at(1)));

  auto leg = new TLegend(0.6, 0.5, 0.8, 0.7);
  leg->AddEntry(pr, "Random Cones", "lep");
  leg->AddEntry(fitGauss, Form("fit #mu=%.2f #sigma=%.2f",mean,sigma), "l");
  leg->Draw();


  cBkg.SaveAs("JFigures/unfold/bkgfluctuations.pdf");

}
