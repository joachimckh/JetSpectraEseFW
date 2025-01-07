#include "Common/Core/FFitWeights.h"

#include <vector>
#include <iostream>

void writeFFitWeightsCCDB(){

  const char *path = "/Users/joachimcarlokristianhansen/jet_analysis/hyperloop_data/LHC23zzh_pass4_small/ese/297376/AnalysisResults.root";
  TFile* f = new TFile(path, "READ");

  TDirectory *dir = reinterpret_cast<TDirectory*>(f->Get("ese-table-producer"));

  FFitWeights* w = static_cast<FFitWeights*>(dir->Get("weights"));

  w->setResolution(4000);
  auto res{w->getResolution()};
  std::cout << "Creating splines with resolution of: " << res << std::endl;

  std::vector<int> nh{2};
  // std::vector<std::string> stv{"FT0C","FT0A","FV0A"};
  std::vector<std::string> stv{"FT0C"};
  w->qSelection(nh, stv);


  TFile *fout = new TFile("Weights_LHC23zzh_pass4_small_Calib4_FT0C.root","RECREATE");

  w->Write("ccdb_object");
  fout->Close();
}
