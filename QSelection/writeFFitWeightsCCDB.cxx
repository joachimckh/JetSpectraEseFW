#include "Common/Core/FFitWeights.h"

#include <vector>

void writeFFitWeightsCCDB(){


  TFile* f = new TFile("/Users/joachimcarlokristianhansen/jet_analysis/hyperloop_data/LHC23zzh_pass4_small/ese/285940/AnalysisResults.root", "READ");

  TDirectory *dir = reinterpret_cast<TDirectory*>(f->Get("ese-table-producer"));

  FFitWeights* w = static_cast<FFitWeights*>(dir->Get("weights"));


  std::vector<int> nh{2,3};
  std::vector<std::string> stv{"FT0C"};
  w->qSelectionSpline(nh, stv);


  TFile *fout = new TFile("Weights_LHC23zzh_pass4_small.root","RECREATE");

  w->Write("ccdb_object");
  fout->Close();
}
