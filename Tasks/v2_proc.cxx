
#include "JEUtility.hpp"

#include <memory>
#include <iostream>

#include <TFile.h>
#include <TH1.h>

using std::unique_ptr;

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <R value, 0.2,0.4>" << std::endl;
  }

  const char* Rval = argv[1];// "0.4";
  // raw
  // TFile *f = new TFile("root_files/SeparatePtPlane_q2_1_100.root","READ");

  TFile *f = new TFile(Form("processed_data/raw/separatePtPlane_q2_0_100-centrality-30-50-R%s.root",Rval),"READ");
  JEUtility *jet = new JEUtility(f, 0.6);
  jet->JERebin(16, new Double_t[17]{0,5,10,15,20,25,30,40,50,60,70,80,90,100,120,160,200});

  TH1* h = jet->calculateV2Jet();
  h->SaveAs(Form("processed_data/vn/v2Jet_R%s.root",Rval));

}
