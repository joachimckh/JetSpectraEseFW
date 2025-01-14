
#include "JEFW.hpp"

#include <memory>
#include <vector>
#include <iostream>

#include <TH1.h>

using std::unique_ptr;

int main(int argc, char *argv[]){

  if (argc < 4) {
    std::cerr << "Usage: " << argv[1] << " <run number>" << std::endl;
    std::cerr << "Usage: " << argv[2] << " <q2 min <1,100>>" << std::endl;
    std::cerr << "Usage: " << argv[3] << " <q2 max <1,100>>" << std::endl;
    std::cerr << "Usage: " << argv[4] << " <Centrality min <1,100>>" << std::endl;
    std::cerr << "Usage: " << argv[5] << " <Centrality max <1,100>>" << std::endl;
    std::cerr << "Usage: " << argv[6] << " jet Radius values" << std::endl;
  }
  const char* HLrun = argv[1];
  unique_ptr<JEFW> jet{new JEFW(Form("/Users/joachimcarlokristianhansen/jet_analysis/hyperloop_data/LHC23zzh_pass4_small/jet/%s/AnalysisResults.root",HLrun))};
  jet->Init("data");

  int q2Min = std::atoi(argv[2]);
  int q2Max = std::atoi(argv[3]);
  std::vector<int> q2{q2Min,q2Max};
  // jet->DrawRaw();
  // jet->DrawXYZ(0);
  int centMin = std::atoi(argv[4]);
  int centMax = std::atoi(argv[5]);
  std::vector<int> cent{centMin,centMax};
  jet->setCentrality(cent);

  
  TObjArray* h = jet->separatePlanes(q2);

  float jetRadius = std::atof(argv[6]);
  h->SaveAs(Form("processed_data/separatePtPlane_q2_%i_%i-centrality-%i-%i-R%.1f.root",q2.at(0),q2.at(1),cent.at(0),cent.at(1),jetRadius));

  // TH1* h2 = jet->aziIntEse(q2); /* Denne her skal lige fixes hvis den skal bruges */
  // h2->SaveAs(Form("root_files/pt_dphiInt_q2_%i_%i-Centrality-%i-%i.root",q2.at(0),q2.at(1),cent.at(0),cent.at(1)));

  return 0;
}
