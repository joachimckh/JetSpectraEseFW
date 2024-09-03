// Copyright 2019-2020 CERN and copyright holders of ALICE O2.
// See https://alice-o2.web.cern.ch/copyright for details of the copyright holders.
// All rights not expressly granted are reserved.
//
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

#include "PWGJE/Core/JetEseContainer.h"

using std::shared_ptr;
using std::unique_ptr;

class JEFW {
  public:
    JEFW() = default;
    explicit JEFW(const char* name);
    ~JEFW();

    void Init();
    void DrawRaw() const;
    void DrawXYZ(const int lvl);

    int PlaneState(const float &dPhi);
    void SeparatePlanes(std::vector<int> vec_q2limits);

  private:
  unique_ptr<TFile> inFile;
  const char* path;
  shared_ptr<TH3F> hist;

  const char* GetPtName(const char* pf = "_pt") {
    return Form("jet%s", pf);
  };
   
  ClassDef(JEFW, 1); // calibration class

};

#endif // CORE_JEFW_H_
