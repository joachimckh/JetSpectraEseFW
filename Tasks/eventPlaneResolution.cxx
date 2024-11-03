#include "../Core/JEFW.h"


void eventPlaneResolution()
{
  unique_ptr<JEFW> jet{new JEFW("/Users/joachimcarlokristianhansen/jet_analysis/localr/jetSpectraESE/AnalysisResults.root")};
  jet->Init("data");



  TH1F* hFT0A = jet->eventPlaneResolution("FT0A","FV0A","FT0C");
  TH1F* hFT0C = jet->eventPlaneResolution("FT0C","FV0A","FT0A");
  TH1F* hFV0A = jet->eventPlaneResolution("FV0A","FT0A","FT0C");
  TH1F* hTPCPos = jet->eventPlaneResolution("TPCpos","TPCneg","FV0A");

  TH1* psiFT0A = jet->getEventPlane("FT0A");
  TH1* psiFT0C = jet->getEventPlane("FT0C");
  TH1* psiFV0A = jet->getEventPlane("FV0A");


  TCanvas *c1 = new TCanvas("c1","c1",800,600);

  hFT0A->SetLineColor(kBlack);
  hFT0A->Draw("hist");

  hFT0C->SetLineColor(kRed);
  hFT0C->Draw("hist same");

  hFV0A->SetLineColor(kBlue);
  hFV0A->Draw("hist same");

  hTPCPos->SetLineColor(kGreen);
  hTPCPos->Draw("hist same");

  TLegend *leg = new TLegend(0.7,0.2,0.9,0.4);
  leg->AddEntry(hFT0A,"FT0A","l");
  leg->AddEntry(hFT0C,"FT0C","l");
  leg->AddEntry(hFV0A,"FV0A","l");
  leg->AddEntry(hTPCPos,"TPCPos","l");

  leg->Draw();

  c1->SaveAs("figures/eventPlaneResolution.pdf");

  TCanvas *c2 = new TCanvas("c2","c2",800,600);

  psiFT0A->SetLineColor(kBlack);
  psiFT0A->Draw("hist");

  psiFT0C->SetLineColor(kRed);
  psiFT0C->Draw("hist same");

  psiFV0A->SetLineColor(kBlue);
  psiFV0A->Draw("hist same");

  TLegend *leg2 = new TLegend(0.7,0.2,0.9,0.4);
  leg2->AddEntry(psiFT0A,"FT0A","l");
  leg2->AddEntry(psiFT0C,"FT0C","l");
  leg2->AddEntry(psiFV0A,"FV0A","l");

  leg2->Draw();

  c2->SaveAs("figures/eventPlane.pdf");
}
