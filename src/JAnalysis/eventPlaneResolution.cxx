#include "JEFW.hpp"

#include <memory>
#include <iostream>
#include <utility>

#include <TCanvas.h>
#include <TH1.h>
#include <TLegend.h>
#include <TFile.h>
#include <TLatex.h>

#include "rootStyle.hpp"

int main(int argc, char *argv[]){

  if (argc < 2) {
    std::cerr << "Usage: " << argv[1] << " <run number> <A> <B> <C> <size>" << std::endl;
  }

  auto fin = Form("/Users/joachimcarlokristianhansen/jet_analysis/hyperloop_data/LHC23zzh_pass4_%s/jet/%s/AnalysisResults.root",argv[5], argv[1]);

  unique_ptr<JEFW> jet{new JEFW(fin,JEFW::DataType::DATA,"")};
  std::pair<int,int> cent{30,50};
  TH1F* hReso = jet->eventPlaneResolution("A","B","C",cent);

  /*    Default case is (jetSpectraEseTask):
          "cfgEPRefA": "FT0A",
          "cfgEPRefB": "TPCpos",
          "cfgEPRefC": "TPCneg"
  */ 

  
  TH1* psiFT0A = jet->getEventPlane("FT0A",cent, 1);
  TH1* psiFT0C = jet->getEventPlane("FT0C",cent, 1);
  TH1* psiFV0A = jet->getEventPlane("FV0A",cent, 1);
  TH1* psiTPCpos = jet->getEventPlane("TPCpos",cent, 1);
  TH1* psiTPCneg = jet->getEventPlane("TPCneg",cent, 1);

  // const int nReb = 5;
  // psiFT0A = psiFT0A->Rebin(nReb);
  // psiFT0C = psiFT0C->Rebin(nReb);
  // psiFV0A = psiFV0A->Rebin(nReb);
  // psiTPCpos = psiTPCpos->Rebin(nReb);
  // psiTPCneg = psiTPCneg->Rebin(nReb);

  rootStyle();

  TCanvas *c1 = new TCanvas("c1","c1",800,600);

  hReso->SetLineColor(kBlack);
  hReso->SetMarkerStyle(20);
  hReso->SetMarkerSize(.5);
  hReso->SetMarkerColor(kBlack);
  hReso->GetXaxis()->SetRangeUser(0,100);
  hReso->GetYaxis()->SetRangeUser(0, 1);
  hReso->Draw("EPhist");


  TLegend *leg = new TLegend(0.3,0.18,0.8,0.56);
  leg->AddEntry(hReso,Form("A: %s, B: %s, C: %s",argv[2],argv[3],argv[4]),"l");
  leg->Draw();

  TLatex latex;
  latex.SetTextSize(0.04);  
  latex.DrawLatexNDC(.18, 0.85, "Pb--Pb #sqrt{s_{NN}} = 5.36 TeV");
  latex.DrawLatexNDC(.18, 0.8, "30-50% centrality");

  TFile *res_out = new TFile(Form("processed_data/ep/eventPlaneResolution-A-%s-B-%s-C-%s.root",argv[2],argv[3],argv[4]),"RECREATE"); 
  hReso->Write();
  res_out->Close();
  double sum{0};
  double weights{0};

  double sum_low{0};
  double w_low{0};
  double sum_high{0};
  double w_high{0};
  for (int i{1}; i<hReso->GetNbinsX(); i++) {
    auto val{hReso->GetBinContent(i)};
    sum += val;
    weights += 1;
    if (i <= 30) {
      sum_low += val;
      w_low += 1;
    }
    if (i >= 70) {
      sum_high += val;
      w_high += 1;
    }
  }
  std::cout << "Mean full: " << sum/weights << std::endl;
  std::cout << "Mean low: " << sum_low/w_low << std::endl;
  std::cout << "Mean high: " << sum_high/w_high << std::endl;
  c1->SaveAs(Form("JFigures/eventplane/eventPlaneResolution-A-%s-B-%s-C-%s.pdf",argv[2],argv[3],argv[4]));
  c1->Close();

  TCanvas *c2 = new TCanvas("c2","c2",800,600);

  psiFT0A->SetLineColor(kBlack);
  psiFT0A->SetMarkerStyle(20);
  psiFT0A->SetMarkerColor(kBlack);
  psiFT0A->SetMarkerSize(.5);
  psiFT0A->SetStats(0);
  psiFT0A->GetYaxis()->SetRangeUser(0, 0.015);
  psiFT0A->GetYaxis()->SetTitle("Entries");
  psiFT0A->Draw("EP");
  // psiFT0A->GetYaxis()->SetRangeUser(0, 6*10e4);

  psiFT0C->SetLineColor(kRed);
  psiFT0C->SetMarkerStyle(20);
  psiFT0C->SetMarkerColor(kRed);
  psiFT0C->SetMarkerSize(.5);
  psiFT0C->Draw("EPsame");

  // psiFV0A->SetLineColor(kBlue);
  // psiFV0A->SetMarkerStyle(20);
  // psiFV0A->SetMarkerColor(kBlue);
  // psiFV0A->SetMarkerSize(.5);
  // psiFV0A->Draw("EPsame");

  psiTPCpos->SetLineColor(kGreen);
  psiTPCpos->SetMarkerStyle(20);
  psiTPCpos->SetMarkerColor(kGreen);
  psiTPCpos->SetMarkerSize(.5);
  psiTPCpos->Draw("EPsame");

  psiTPCneg->SetLineColor(kOrange+1);
  psiTPCneg->SetMarkerStyle(20);
  psiTPCneg->SetMarkerColor(kOrange+1);
  psiTPCneg->SetMarkerSize(.5);
  psiTPCneg->Draw("EPsame");


  TLegend *leg2 = new TLegend(0.4,0.45,0.6,0.65);
  leg2->AddEntry(psiFT0A,"FT0A","lep");
  leg2->AddEntry(psiFT0C,"FT0C","lep");
  // leg2->AddEntry(psiFV0A,"FV0A","lep");
  leg2->AddEntry(psiTPCpos,"TPCPos","lep");
  leg2->AddEntry(psiTPCneg,"TPCNeg","lep");
  leg2->Draw();

  TLatex latex2;
  latex2.SetTextSize(0.04);  
  latex2.DrawLatexNDC(.3, 0.25, "Pb--Pb #sqrt{s_{NN}} = 5.36 TeV");
  latex2.DrawLatexNDC(.3, 0.2, "30-50% centrality");

  c2->SaveAs("JFigures/eventplane/eventPlane.pdf");
  c2->Close();

  TCanvas *c4 = new TCanvas("c4","c4",800,600);

  auto hFullEPR{jet->eventPlaneResolutionInclusive()};

  hFullEPR->SetLineColor(kBlack);
  hFullEPR->SetMarkerStyle(20);
  hFullEPR->SetMarkerSize(.5);
  hFullEPR->SetMarkerColor(kBlack);
  hFullEPR->GetXaxis()->SetRangeUser(0,80);
  hFullEPR->GetYaxis()->SetRangeUser(0, 1);
  hFullEPR->Draw("EPhist");

  TLatex latexF;
  latexF.SetTextSize(0.04);  
  latexF.DrawLatexNDC(.48, 0.85, "Pb--Pb #sqrt{s_{NN}} = 5.36 TeV");
  latexF.DrawLatexNDC(.48, 0.8, "A: FT0A");
  latexF.DrawLatexNDC(.48, 0.75, "B: TPCpos");
  latexF.DrawLatexNDC(.48, 0.7, "C: TPCneg");

  c4->SaveAs("JFigures/eventplane/InclusiveEPR.pdf");

  return 0;
}
