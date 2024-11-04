
#include "../Core/JEUtility.h"

#include <memory>


using std::unique_ptr;

void v2_procc(){

  const char* Rval = "0.4";
  // raw
  // TFile *f = new TFile("root_files/SeparatePtPlane_q2_1_100.root","READ");

  // unfolded
  TFile *f = new TFile(Form("root_files/unfolded_q2_1_100_R%s.root",Rval),"READ");
  JEUtility *jet = new JEUtility(f, 0.71); /* R taken from DPG for now */
  jet->JERebin(16, new Double_t[17]{0,5,10,15,20,25,30,40,50,60,70,80,90,100,120,160,200});

  TH1* h = jet->calculateV2Jet();
  h->SaveAs(Form("root_files/v2Jet_R%s.root",Rval));

}
