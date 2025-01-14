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

#include <TObjArray.h>
#include <TH3D.h>
#include <TH2D.h>
#include <TH1D.h>
#include <TFile.h>


class JEUtility {
  public:
    JEUtility() = default;
    explicit JEUtility(TFile* planes, double R);
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
