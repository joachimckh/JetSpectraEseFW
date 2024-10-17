// This software is distributed under the terms of the GNU General Public
// License v3 (GPL Version 3), copied verbatim in the file "COPYING".
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

/*
Author: Joachim Hansen

*/
#ifndef CORE_JEFW_H_
#define CORE_JEFW_H_

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
#include "TMath.h"


using std::shared_ptr;
using std::unique_ptr;

class JEFW {
  public:
    enum DataType {
        DATA = 0,
        MC = 1
      };
    JEFW() = default;
    explicit JEFW(const char* name);
    ~JEFW();

    void Init(std::string type);
    void DrawRaw() const;
    void DrawXYZ(const int lvl);

    int PlaneState(const float &dPhi);
    void SeparatePlanes(std::vector<int> vec_q2limits);
    void AziIntEse(std::vector<int> vec_q2limits);

    TH3F* GetHist() const {
      return hist.get();
    };
    TH1F* GetHistPt(const int i) const {
      switch (i) {
        case 0:
          return h_pt_bkg;
        case 1:
          return htruth;
        case 2:
          return hmatched;
      }
      return nullptr;
    };
    TH2F* GetHistMC() const {
      return hist_mc.get();
    };

  private:
    unique_ptr<TFile> inFile;
    const char* path;
    shared_ptr<TH3F> hist;
    shared_ptr<TH2F> hist_mc;
    TH1F* h_pt_bkg;
    TH1F* htruth;
    TH1F* hmatched;

    const char* GetPtName(const char* pf = "_pt") {
      return Form("jet%s", pf);
    };
    const char* GetMCName() {
      return "h_response_mat_match";
    };

    DataType getDataType(const std::string& type) const;
   
  ClassDef(JEFW, 1); // jet fw class

};

#endif // CORE_JEFW_H_
