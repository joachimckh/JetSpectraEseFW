#include <TFile.h>
#include <TDirectory.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TCanvas.h>



int main(int argc, char* argv[]) {

  //input An.root...

  auto f = TFile::Open(argv[1],"READ");
  auto dir = dynamic_cast<TDirectory*>(f->Get("jet-spectra-ese-task"));

  auto cent = dynamic_cast<TH1F*>(dir->Get("hCentralitySel"));
  auto hrho = dynamic_cast<TH2F*>(dir->Get("hRho"));
  auto hrhophi = dynamic_cast<TH2F*>(dir->Get("hRhoPhi"));
  auto fit0 = dynamic_cast<TH2F*>(dir->Get("hfitPar0"));

  auto chi2 = dynamic_cast<TH2F*>(dir->Get("hCentChi2Ndf"));

  auto eventcount = dynamic_cast<TH1F*>(dir->Get("hEventCounter"));

  const char* savePath = "JFigures/QA";

  auto c0 = new TCanvas("c0","c0",800,600);
  c0->cd();
  cent->SetStats(0);
  cent->Draw();
  c0->SaveAs(Form("%s/hCentralitySel.pdf",savePath));

  auto c1 = new TCanvas("c1","c1",800,600);
  c1->cd();
  gPad->SetLogz();
  hrho->SetStats(0);
  hrho->GetYaxis()->SetTitle("#rho^{event}");
  hrho->Draw("colz");
  c1->SaveAs(Form("%s/hRho.pdf",savePath));

  auto c2 = new TCanvas("c2","c2",800,600);
  c2->cd();
  gPad->SetLogz();
  hrhophi->SetStats(0);
  hrhophi->GetYaxis()->SetTitle("#rho(#phi)");
  hrhophi->Draw("colz");
  c2->SaveAs(Form("%s/hRhoPhi.pdf",savePath));

  auto c3 = new TCanvas("c3","c3",800,600);
  c3->cd();
  gPad->SetLogz();
  fit0->SetStats(0);
  fit0->GetYaxis()->SetTitle("#rho_{0}");
  fit0->Draw("colz");
  c3->SaveAs(Form("%s/hfitPar0.pdf",savePath));

  auto c4 = new TCanvas("c4","c4",800,600);
  c4->cd();
  eventcount->SetStats(0);
  eventcount->Draw();
  c4->SaveAs(Form("%s/hEventCounter.pdf",savePath));

  auto c5 = new TCanvas("c5","c5",800,600);
  c5->cd();
  gPad->SetLogz();
  chi2->SetStats(0);
  // chi2->GetYaxis()->SetTitle("#chi^{2}/NDF");
  chi2->SetTitle("");
  chi2->Draw("colz");
  c5->SaveAs(Form("%s/hCentChi2Ndf.pdf",savePath));



}
