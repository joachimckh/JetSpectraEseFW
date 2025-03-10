/// \file JEUtility.h
/// \brief Header file for JEUtility class
///
/// \author Joachim C. K. B. Hansen

#pragma once

#include <vector>
#include <string>
#include <utility>
#include <algorithm>
#include <memory>
#include <numbers>
#include <iostream>

#include <TObjArray.h>
#include <TH3D.h>
#include <TH2D.h>
#include <TH1D.h>
#include <TFile.h>

#include <TH1.h>


class JEUtility {
  public:
    JEUtility() = default;
    explicit JEUtility(TFile* planes, double R);
    explicit JEUtility(TH1* h_in, TH1* h_out, double R);
    ~JEUtility();

    TH1* calculateV2Jet();

    void JERebin(int n, Double_t* bin_edges);

    
    std::pair<TH1*, TH1*> YieldCorrectedJet();
    TH1* YieldRatio();

  private:
    TH1* h_in;
    TH1* h_out;
    double R;

   
   
  ClassDef(JEUtility, 1); // jet utility fw class

};

namespace jetutility 
{

  inline void scale(TH1* h)
  {
    double cst{1.0/(h->Integral())};
    h->Scale(cst);
  }

  TH2D* getMatrixProductTH2xTH2(TH2D* histA, TH2D* histB);
  std::vector<double> GetTH1Bins(TH1* H1_histo);
  void WeightMatrixWithPrior(TH2D* H2D_hist, TH1D* priorSpectrum);

} // namespace jetutility
