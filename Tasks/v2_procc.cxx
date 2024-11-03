
#include "../Core/JEUtility.h"

#include <memory>


using std::unique_ptr;

void v2_procc(){

  TFile *f = new TFile("root_files/SeparatePtPlane_q2_1_100.root","READ");

  JEUtility *jet = new JEUtility(f, 1.6); /* R */

  TH1* h = jet->calculateV2Jet();
  h->SaveAs("root_files/v2Jet.root");

}
