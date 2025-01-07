/// \file JEfficiency.h
/// \brief Header file for JEfficiency class
///
/// \author Joachim C. K. B. Hansen

#ifndef CORE_JEfficiency_H_
#define CORE_JEfficiency_H_

#include <vector>
#include <string>
#include <utility>
#include <algorithm>
#include <memory>

#include <TObjArray.h>
#include <TH3D.h>
#include <TH2D.h>
#include <TH1D.h>
#include <TFile.h>
#include <TMath.h>

using std::shared_ptr;
using std::unique_ptr;


class JEfficiency {
  public:
    JEfficiency() = default;
    explicit JEfficiency(TH1F* hist /* measured data */, TH2F* RESPONSE, TH1F* htruth, TH1F* hmatched);
    ~JEfficiency();


    TH1D* Unfold(int iterations);
    TH1D* Efficiency();
    

  private:
    // Placeholder for future members and methods
    TH1F* hist;
    TH2F* RESPONSE;
    TH1F* htruth;
    TH1F* hmatched;


  ClassDef(JEfficiency, 1); // jet unfolding class

};


#endif // CORE_JEfficiency_H_

