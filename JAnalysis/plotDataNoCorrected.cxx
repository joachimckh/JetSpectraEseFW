#include <memory>
#include <iostream>


#include <TFile.h>
#include <TH1.h>

#include "JEUtility.hpp"

int main(int argc, char *argv[]) {

  if (argc < 2) {
    std::cerr << "Usage: " << argv[1] << " jet Radius values" << std::endl;
  }

  const char* Rval = "0.4";

  std::unique_ptr<TFile> f = std::unique_ptr<TFile>(new TFile(Form("processed_data/unfolded_q2_1_100_R%s.root",Rval),"READ"));
  TH1D* h_in_inclusive_raw = dynamic_cast<TH1D*>(f->Get("hv_0"));
  TH1D* h_out_inclusive_raw = dynamic_cast<TH1D*>(f->Get("hv_1"));
  JEUtility *jet = new JEUtility(f.get(), 0.71); /* R in ideal case */
  auto hs = jet->YieldCorrectedJet();
  TH1* h_in_inclusive_Corr = hs.first;
  TH1* h_out_inclusive_Corr = hs.second;


  int n = 16;
  Double_t bin_edges[17]={0,5,10,15,20,25,30,40,50,60,70,80,90,100,120,160,200};
  TH1* h_in_inclusive = h_in_inclusive_raw->Rebin(n, "hInInclusiveReb", bin_edges);
  TH1* h_out_inclusive = h_out_inclusive_raw->Rebin(n, "hOutInclusiveReb", bin_edges);

  TH1* h_in_inclusive_corr_Rebin = h_in_inclusive_Corr->Rebin(n, "hInCorr_re", bin_edges);
  TH1* h_out_inclusive_corr_Rebin = h_out_inclusive_Corr->Rebin(n, "hOutCorr_re", bin_edges);


  return 0;
}