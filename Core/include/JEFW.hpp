/// \file JEFW.h
/// \brief Header file for JEFW class
///
/// \author Joachim C. K. B. Hansen

#pragma once

#include <vector>
#include <string>
#include <utility>
#include <algorithm>
#include <memory>
#include <iostream>
#include <numbers>

#include <TObjArray.h>
#include <TH3D.h>
#include <TH2D.h>
#include <TH1D.h>
#include <TFile.h>
#include <TMath.h>
#include <TProfile.h>
#include <TCanvas.h>
#include <THnSparse.h>

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
    void drawRaw() const;
    void drawXYZ(const int lvl);

    int planeState(const float &dPhi);
    void setCentrality(const std::vector<int> vec_centlimits);
    void setq2Range(const std::vector<int> vec_q2limits);
    TObjArray* separatePlanes();
    TH1* aziIntEse(std::vector<int> vec_q2limits);

    THnSparse* getHist() const {
      return hist;
    };
    TH1F* getHistPt(const int i) const {
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
    TH2F* getHistMC() const {
      return hist_mc;
    };

    TH1F* eventPlaneResolution(std::string A, std::string B, std::string C, std::pair<int,int> cent);
    TH1* getEventPlane(const char* name, std::pair<int,int> cent);
    TH1* getR2S(const char* name, std::pair<int,int> cent);
    std::vector<TH1*> inclusiveEPR(std::string A, std::string B, std::string C);

    void JERebin(int n, Double_t* bin_edges);

    double eventCounter(const int iBin);

  private:
    unique_ptr<TFile> inFile;
    const char* path;
    THnSparse* hist;
    TH2F* hist_mc;
    TH1F* h_pt_bkg;
    TH1F* htruth;
    TH1F* hmatched;

    TDirectory* dir;


    DataType getDataType(const std::string& type) const;
    std::pair<double, double> getEPR3Val(TProfile* pAC, TProfile* pAB, TProfile* pBC, int i);
   
  ClassDef(JEFW, 1); // jet fw class

};
