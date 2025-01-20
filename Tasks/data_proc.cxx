
#include "JEFW.hpp"

#include <memory>
#include <vector>
#include <iostream>

#include <TH1.h>
#include <TFile.h>
#include <TVectorD.h>

using std::unique_ptr;

int main(int argc, char *argv[]){

  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <run number> <q2 min> <q2 max> <cent min> <cent max> <jet radius> <small,full>" << std::endl;
  }
  const char* HLrun = argv[1];
  unique_ptr<JEFW> jet{new JEFW(Form("/Users/joachimcarlokristianhansen/jet_analysis/hyperloop_data/LHC23zzh_pass4_%s/jet/%s/AnalysisResults.root",argv[7],HLrun))};
  jet->Init("data");

  auto hist = jet->getHist()->Clone("hCentJetPtdPhiq2Clone");

  int q2Min = std::atoi(argv[2]);
  int q2Max = std::atoi(argv[3]);
  std::vector<int> q2{q2Min,q2Max};
  // jet->DrawRaw();
  // jet->DrawXYZ(0);
  int centMin = std::atoi(argv[4]);
  int centMax = std::atoi(argv[5]);
  std::vector<int> cent{centMin,centMax};
  
  
  jet->setCentrality(cent);
  jet->setq2Range(q2);
  
  TObjArray* h = jet->separatePlanes();

  double nEvents{jet->eventCounter(7)}; /* 30-50 centr */
  TVectorD* nEventsVec = new TVectorD(1);
  (*nEventsVec)[0] = nEvents;

  float jetRadius = std::atof(argv[6]);
  TFile *f = new TFile(Form("processed_data/raw/separatePtPlane_q2_%i_%i-centrality-%i-%i-R%.1f.root",q2.at(0),q2.at(1),cent.at(0),cent.at(1),jetRadius),"RECREATE");
  h->Write();
  hist->Write();
  nEventsVec->Write("nEvents");
  f->Close();
  // h->SaveAs(Form("processed_data/raw/separatePtPlane_q2_%i_%i-centrality-%i-%i-R%.1f.root",q2.at(0),q2.at(1),cent.at(0),cent.at(1),jetRadius));

  // TH1* h2 = jet->aziIntEse(q2); /* Denne her skal lige fixes hvis den skal bruges */
  // h2->SaveAs(Form("root_files/pt_dphiInt_q2_%i_%i-Centrality-%i-%i.root",q2.at(0),q2.at(1),cent.at(0),cent.at(1)));

  return 0;
}
