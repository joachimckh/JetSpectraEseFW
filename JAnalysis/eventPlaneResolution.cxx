#include "JEFW.hpp"

#include <memory>
#include <iostream>
#include <utility>

#include <TCanvas.h>
#include <TH1.h>
#include <TLegend.h>
#include <TFile.h>

#include "rootStyle.hpp"

int main(int argc, char *argv[]){

  if (argc < 2) {
    std::cerr << "Usage: " << argv[1] << " <run number> <A> <B> <C>" << std::endl;
  }

  auto fin = Form("/Users/joachimcarlokristianhansen/jet_analysis/hyperloop_data/LHC23zzh_pass4_small/jet/%s/AnalysisResults.root",argv[1]);

  unique_ptr<JEFW> jet{new JEFW(fin)};
  jet->Init("data");
  std::pair<int,int> cent{30,50};
  TH1F* hReso = jet->eventPlaneResolution("A","B","C",cent);

  /*    Default case is (jetSpectraEseTask):
          "cfgEPRefA": "FT0A",
          "cfgEPRefB": "TPCpos",
          "cfgEPRefC": "TPCneg"
  */ 

  
  TH1* psiFT0A = jet->getEventPlane("FT0A",cent);
  TH1* psiFT0C = jet->getEventPlane("FT0C",cent);
  TH1* psiFV0A = jet->getEventPlane("FV0A",cent);
  TH1* psiTPCpos = jet->getEventPlane("TPCpos",cent);
  TH1* psiTPCneg = jet->getEventPlane("TPCneg",cent);

  const int nReb = 5;
  psiFT0A = psiFT0A->Rebin(nReb);
  psiFT0C = psiFT0C->Rebin(nReb);
  psiFV0A = psiFV0A->Rebin(nReb);
  psiTPCpos = psiTPCpos->Rebin(nReb);
  psiTPCneg = psiTPCneg->Rebin(nReb);

  rootStyle();

  TCanvas *c1 = new TCanvas("c1","c1",800,600);

  hReso->SetLineColor(kBlack);
  hReso->GetXaxis()->SetRangeUser(0,100);
  hReso->GetYaxis()->SetRangeUser(0, 1);
  hReso->Draw("hist");


  TLegend *leg = new TLegend(0.5,0.2,0.8,0.5);
  leg->AddEntry(hReso,Form("A: %s, B: %s, C: %s",argv[2],argv[3],argv[4]),"l");

  leg->Draw();

  TFile *res_out = new TFile(Form("processed_data/ep/eventPlaneResolution-A-%s-B-%s-C-%s.root",argv[2],argv[3],argv[4]),"RECREATE"); 
  hReso->Write();
  res_out->Close();
  double sum{0};
  double weights{0};
  for (int i{1}; i<hReso->GetNbinsX(); i++) {
    sum += hReso->GetBinContent(i);
    weights += 1;
  }
  std::cout << "Mean: " << sum/weights << std::endl;
  c1->SaveAs(Form("JFigures/eventplane/eventPlaneResolution-A-%s-B-%s-C-%s.pdf",argv[2],argv[3],argv[4]));
  c1->Close();

  TCanvas *c2 = new TCanvas("c2","c2",800,600);

  psiFT0A->SetLineColor(kBlack);
  psiFT0A->SetStats(0);
  psiFT0A->Draw("EP");
  // psiFT0A->GetYaxis()->SetRangeUser(0, 6*10e4);

  psiFT0C->SetLineColor(kRed);
  psiFT0C->Draw("EPsame");

  psiFV0A->SetLineColor(kBlue);
  psiFV0A->Draw("EPsame");

  psiTPCpos->SetLineColor(kGreen);
  psiTPCpos->Draw("EPsame");

  psiTPCneg->SetLineColor(kOrange);
  psiTPCneg->Draw("EPsame");


  TLegend *leg2 = new TLegend(0.4,0.45,0.6,0.65);
  leg2->AddEntry(psiFT0A,"FT0A","l");
  leg2->AddEntry(psiFT0C,"FT0C","l");
  leg2->AddEntry(psiFV0A,"FV0A","l");
  leg2->AddEntry(psiTPCpos,"TPCPos","l");
  leg2->AddEntry(psiTPCneg,"TPCNeg","l");

  leg2->Draw();

  c2->SaveAs("JFigures/eventplane/eventPlane.pdf");
  c2->Close();


  // TCanvas *c3 = new TCanvas("c3","c3",800,600);

  
  // TH1* FT0CmFT0A = jet->getR2S("FT0CmFT0A",cent);
  // TH1* FT0CmFV0A = jet->getR2S("FT0CmFV0A",cent);
  // TH1* FV0AmFT0A = jet->getR2S("FV0AmFT0A",cent);
  // TH1* FT0AmFT0C = jet->getR2S("FT0AmFT0C",cent);
  // TH1* FT0AmFV0A = jet->getR2S("FT0AmFV0A",cent);
  // TH1* FV0AmFT0C = jet->getR2S("FV0AmFT0C",cent);
  // TH1* TPCposmTPCneg = jet->getR2S("TPCposmTPCneg",cent);
  // TH1* TPCposmFV0A = jet->getR2S("TPCposmFV0A",cent);
  // TH1* TPCnegmFV0A = jet->getR2S("TPCnegmFV0A",cent);

  // FT0CmFT0A->SetLineColor(kBlack);
  // FT0CmFT0A->GetYaxis()->SetRangeUser(0, 3000*1e3);
  // FT0CmFT0A->Draw("hist");

  // FT0CmFV0A->SetLineColor(kRed);
  // FT0CmFV0A->Draw("hist same");

  // FV0AmFT0A->SetLineColor(kBlue);
  // FV0AmFT0A->Draw("hist same");

  // FT0AmFT0C->SetLineColor(kGreen);
  // FT0AmFT0C->Draw("hist same");

  // FT0AmFV0A->SetLineColor(kOrange);
  // FT0AmFV0A->Draw("hist same");

  // FV0AmFT0C->SetLineColor(kPink);
  // FV0AmFT0C->Draw("hist same");

  // TPCposmTPCneg->SetLineColor(kViolet);
  // TPCposmTPCneg->Draw("hist same");

  // TPCposmFV0A->SetLineColor(kAzure);
  // TPCposmFV0A->Draw("hist same");

  // TPCnegmFV0A->SetLineColor(kTeal);
  // TPCnegmFV0A->Draw("hist same");
  
  // TLegend *leg3 = new TLegend(0.6,0.5,0.8,0.7);
  // leg3->AddEntry(FT0CmFT0A,"FT0CmFT0A","l");
  // leg3->AddEntry(FT0CmFV0A,"FT0CmFV0A","l");
  // leg3->AddEntry(FV0AmFT0A,"FV0AmFT0A","l");
  // leg3->AddEntry(FT0AmFT0C,"FT0AmFT0C","l");
  // leg3->AddEntry(FT0AmFV0A,"FT0AmFV0A","l");
  // leg3->AddEntry(FV0AmFT0C,"FV0AmFT0C","l");
  // leg3->AddEntry(TPCposmTPCneg,"TPCposmTPCneg","l");
  // leg3->AddEntry(TPCposmFV0A,"TPCposmFV0A","l");
  // leg3->AddEntry(TPCnegmFV0A,"TPCnegmFV0A","l");

  // leg3->Draw();
  
  // c3->SaveAs("figures/R2Single.pdf");


  // TCanvas *c4 = new TCanvas("c4","c4",800,600);

  // /* R2 FT0C */
  // // const char* A = "FT0C";
  // // const char* B = "FV0A";
  // // const char* C = "FT0A";
  // /* R2 FT0A */
  // const char* A = "FT0A";
  // const char* B = "FV0A";
  // const char* C = "FT0C";
  // auto hINCs = jet->InclusiveEPR(Form("%s",A),Form("%s",B),Form("%s",C));

  // hINCs.at(0)->SetLineColor(kBlack);
  // hINCs.at(0)->GetXaxis()->SetRangeUser(cent.first,cent.second);
  // hINCs.at(0)->GetYaxis()->SetRangeUser(0.2, 0.9);
  // hINCs.at(0)->Draw("hist");

  // hINCs.at(1)->SetLineColor(kRed);
  // hINCs.at(1)->Draw("hist same");

  // hINCs.at(2)->SetLineColor(kBlue);
  // hINCs.at(2)->Draw("hist same");

  // hINCs.at(3)->SetLineColor(kGreen);
  // hINCs.at(3)->Draw("hist same");

  // auto leg4 = new TLegend(0.25,0.2,0.45,0.4);
  // leg4->AddEntry(hINCs.at(0),"3-sub","l");
  // leg4->AddEntry(hINCs.at(1),Form("%s-%s",A,C),"l");
  // leg4->AddEntry(hINCs.at(2),Form("%s-%s",A,B),"l");
  // leg4->AddEntry(hINCs.at(3),Form("%s-%s",B,C),"l");
  // leg4->Draw();

  // c4->SaveAs("figures/InclusiveEPR.pdf");

  return 0;
}
