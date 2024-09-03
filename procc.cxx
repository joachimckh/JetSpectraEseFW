
#include "Core/JEFW.h"

#include <memory>


using std::unique_ptr;

void procc(){

  unique_ptr<JEFW> jet{new JEFW("../jetSpectraESE/AnalysisResults.root")};
  jet->Init();


  // jet->DrawRaw();
  // jet->DrawXYZ(0);

  std::vector<int> q2{1,100};
  jet->SeparatePlanes(q2);

}
