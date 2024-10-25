#include "Common/Core/FFitWeights.h"

#include <vector>
#include "TSpline.h"

void drawSpline(){


  TFile* f = new TFile("TestWeights.root", "READ");

  FFitWeights *w = reinterpret_cast<FFitWeights*>(f->Get("ccdb_object"));
  w->ls();


  TSpline* sp = reinterpret_cast<TSpline3*>(w->GetDataArray()->At(10));
  TSpline* sp2 = reinterpret_cast<TSpline3*>(w->GetDataArray()->At(30));

  TCanvas *c = new TCanvas("c", "c", 800, 800);
  c->cd();

  sp->Draw();

  sp2->SetLineColor(kRed);
  sp2->Draw("same");

  c->SaveAs("spline.png");
}
