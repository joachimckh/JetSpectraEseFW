
#include "JEFW.hpp"

#include <memory>
#include <vector>

#include <TH1.h>

using std::unique_ptr;

int main(int argc, char *argv[]){

  const char* HLrun = "286287";
  unique_ptr<JEFW> jet{new JEFW(Form("/Users/joachimcarlokristianhansen/jet_analysis/hyperloop_data/LHC23zzh_pass4_small/jet/%s/AnalysisResults.root",HLrun))};
  jet->Init("data");

  int q2Min = 1;
  int q2Max = 100;
  // jet->DrawRaw();
  // jet->DrawXYZ(0);

  std::vector<int> q2{q2Min,q2Max};
  TObjArray* h = jet->SeparatePlanes(q2);

  h->SaveAs(Form("root_files/SeparatePtPlane_q2_%i_%i.root",q2.at(0),q2.at(1)));

  TH1* h2 = jet->AziIntEse(q2);
  h2->SaveAs(Form("root_files/pt_dphiInt_q2_%i_%i.root",q2.at(0),q2.at(1)));

  return 0;
}
