/// \file JEUtility.h
/// \brief Header file for JEUtility class
///
/// \author Joachim C. K. B. Hansen

#ifndef CORE_JEUtility_H_
#define CORE_JEUtility_H_

#include <vector>
#include <string>
#include <utility>
#include <algorithm>
#include <memory>

#include "TObjArray.h"
#include "TH3D.h"
#include "TH2D.h"
#include "TH1D.h"
#include "TFile.h"


class JEUtility {
  public:
    JEUtility() = default;
    explicit JEUtility(TFile* planes, double R);
    ~JEUtility();

    TH1* calculateV2Jet();

  private:
    TH1* h_in;
    TH1* h_out;
    double R;

   
   
  ClassDef(JEUtility, 1); // jet utility fw class

};

#endif // CORE_JEUtility_H_
