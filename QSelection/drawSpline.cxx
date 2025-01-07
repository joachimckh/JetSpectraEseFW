#include "Common/Core/FFitWeights.h"

#include <vector>
#include "TSpline.h"

void drawSpline(){


  TFile* f = new TFile("Weights_LHC23zzh_pass4_small_Calib4_FT0C.root", "READ");

  FFitWeights *w = reinterpret_cast<FFitWeights*>(f->Get("ccdb_object"));
  w->ls();


  auto sp = reinterpret_cast<TGraph*>(w->getDataArray()->At(9));

  float x{700.2};
  auto y{sp->Eval(x)};
  TGraph *gr = new TGraph();
  gr->SetPoint(0, x, y);


  TCanvas *c = new TCanvas("c", "c", 800, 800);
  c->cd();
  sp->SetLineColor(kBlack);
  sp->Draw();

  gr->SetMarkerStyle(20);
  gr->SetMarkerSize(1.5);
  gr->SetMarkerColor(kRed);
  gr->Draw("Psame");
  

  c->SaveAs("spline.png");
}
