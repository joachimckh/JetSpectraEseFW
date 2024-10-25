
#include "Core/JEFW.h"

#include <memory>


using std::unique_ptr;

void procc_data(){

  unique_ptr<JEFW> jet{new JEFW("/Users/joachimcarlokristianhansen/jet_analysis/localr/loca_merge/train_278523_sub_jet/AnalysisResults.root")};
  jet->Init("data");

  int q2Min = 1;
  int q2Max = 100;
  // jet->DrawRaw();
  // jet->DrawXYZ(0);

  std::vector<int> q2{q2Min,q2Max};
  jet->SeparatePlanes(q2);

  jet->AziIntEse(q2);
}
