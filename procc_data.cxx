
#include "Core/JEFW.h"

#include <memory>


using std::unique_ptr;

void procc_data(){

  unique_ptr<JEFW> jet{new JEFW("../../jetSpectraESE/AnalysisResults.root")};
  jet->Init();

  int q2Min = 70;
  int q2Max = 100;
  // jet->DrawRaw();
  // jet->DrawXYZ(0);

  std::vector<int> q2{q2Min,q2Max};
  jet->SeparatePlanes(q2);

  jet->AziIntEse(q2);
}
