/// \file JEFW.h
/// \brief Header file for JEFW class
///
/// \author Joachim C. K. B. Hansen

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
    auto SeparatePlanes(std::vector<int> vec_q2limits);
    TH1* AziIntEse(std::vector<int> vec_q2limits);

    TH3F* GetHist() const {
      return hist;
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
      return hist_mc;
    };

    TH1F* eventPlaneResolution(std::string A, std::string B, std::string C);
    TH1* getEventPlane(const char* name);

    void JERebin(int n, Double_t* bin_edges);

  private:
    unique_ptr<TFile> inFile;
    const char* path;
    TH3F* hist;
    TH2F* hist_mc;
    TH1F* h_pt_bkg;
    TH1F* htruth;
    TH1F* hmatched;

    TDirectory* dir;


    DataType getDataType(const std::string& type) const;
   
  ClassDef(JEFW, 1); // jet fw class

};

#endif // CORE_JEFW_H_
