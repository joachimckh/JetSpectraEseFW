#include "Common/Core/FFitWeights.h"

#include <vector>

void writeFFitWeightsCCDB(){


  TFile* f = new TFile("/Users/joachimcarlokristianhansen/jet_analysis/localr/qVectors/AnalysisResults1.root", "READ");

  TDirectory *dir = reinterpret_cast<TDirectory*>(f->Get("ese-table-producer"));

  FFitWeights* w = static_cast<FFitWeights*>(dir->Get("weights"));


  std::vector<int> nh{2,3};
  std::vector<std::string> stv{"FT0C"};
  w->qSelectionSpline(nh, stv);


  TFile *fout = new TFile("TestWeights.root","RECREATE");

  w->Write("ccdb_object");
  fout->Close();
}
