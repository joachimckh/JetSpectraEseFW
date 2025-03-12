/// \file JEWrapper.h
/// \brief Header file for JEWrapper class
///
/// \author Joachim C. K. B. Hansen
#pragma once

#include <vector>
#include <array>
#include <memory>
#include <iostream>

#include "JEFW.hpp"
#include "JEUtility.hpp"
#include "JEfficiency.hpp"

enum class Plane { IN, OUT };
enum class q2RangeType { INCLUSIVE, LOW, HIGH };
enum class Rebin { YES, NO };

class JEWrapper {
  
public:

  JEWrapper(const char* path, const char* subTask = "") {
    file = TFile::Open(path, "READ");
    dir = dynamic_cast<TDirectory*>(file->Get(Form("jet-spectra-ese-task%s",subTask)));
    jet = std::unique_ptr<JEFW>(new JEFW(dir, JEFW::DataType::DATA));
    auto centHist = dynamic_cast<TH1F*>(dir->Get("hCentralitySel"));
    setEventScale(centHist);
    setBins();
  }

  void printObservables() const {
    std::cout << "Modified Observables" << "\n";
    std::cout << "jet pT in/out with rho(phi) + low and high q2" << "\n";
  }

  template <Plane planeType, q2RangeType q2, Rebin rebin = Rebin::NO, bool EPR2=true> 
  TH1* getPt() {
    return mightSetPt<planeType, q2, rebin, EPR2>();
  }

  template <q2RangeType rangeType, Rebin rebin, bool EPR2=true>
  TH1* getPtRatio() {
    return mightSetRatioPt<rangeType, rebin, EPR2>();
  }

  template <Rebin rebin = Rebin::NO>
  TH1* getPt() {
    if (!jetPt) {
      if (!hSparse) {
        hSparse = dynamic_cast<THnSparse*>(dir->Get("hCentJetPtdPhiq2"));
      }
      hSparse->GetAxis(0)->SetRange(cent_30_50.at(0), cent_30_50.at(1));
      jetPt = dynamic_cast<TH1*>(hSparse->Projection(1));
      jetPt->SetName("jetPt");
      if constexpr (rebin == Rebin::YES)
        jetPt = jetPt->Rebin(nBins, "jetPt_reb", binEdges.data());

      jetPt->Scale(1.0 / eventScale, "width");
      hSparse->GetAxis(0)->SetRange(0, 0); // reset range
    }
    return jetPt;
  }

  void setBins(int n = 19, std::vector<double> edges = {0,5,10,15,20,25,30,35,40,45,50,55,60,70,80,90,100,120,160,200}) {
    nBins = n;
    binEdges = edges;
  }

  template <int shiftMarker = 0>
  void setPtDrawSettings() {
    auto applyStyle = [](TH1* hist, size_t colorIndex, size_t markerIndex) {
      if (!hist) return;
      hist->SetMarkerStyle(markers[(markerIndex + shiftMarker) % markers.size()]);
      hist->SetMarkerColor(colors[(colorIndex + shiftMarker) % colors.size()]);
      hist->SetLineColor(colors[(colorIndex + shiftMarker) % colors.size()]);
      hist->SetStats(0);
    };

    // {kBlack, kRed+1 , kBlue+1, kGreen+3, kMagenta+1, kOrange-1,kCyan+2,kYellow+2};
    // {kFullCircle, kFullSquare,kOpenCircle,kOpenSquare,kOpenDiamond,kOpenCross,kFullCross,kFullDiamond,kFullStar,kOpenStar};
    applyStyle(jetPt, 0, 0);  // kBlack, kFullCircle
    applyStyle(jetPtIn_rhoPhi, 3, 0);  // kGreen+3, kFullCircle
    applyStyle(jetPtOut_rhoPhi, 1, 0);  // kRed+1, kFullCircle
    applyStyle(jetPtIn_q2High_rhoPhi, 3, 0);  // kGreen+3, kFullCircle
    applyStyle(jetPtOut_q2High_rhoPhi, 1, 0);  // kRed+1, kFullCircle
    applyStyle(jetPtIn_q2Low_rhoPhi, 4, 1);  // kMagenta+1, kFullSquare
    applyStyle(jetPtOut_q2Low_rhoPhi, 2, 1);  // kBlue+1, kFullSquare
    applyStyle(jetPt_OutDividedIn_q2High, 6, 0);  // kCyan+2, shiftable marker
    applyStyle(jetPt_OutDividedIn, 0, 2);  // kBlack, shiftable marker
    applyStyle(jetPt_OutDividedIn_q2Low, 5, 0);  // kOrange-1, shiftable marker

    if (jetPt_OutDividedIn_q2High) jetPt_OutDividedIn_q2High->SetMarkerSize(0.8);
    if (jetPt_OutDividedIn) jetPt_OutDividedIn->SetMarkerSize(0.8);
    if (jetPt_OutDividedIn_q2Low) jetPt_OutDividedIn_q2Low->SetMarkerSize(0.8);
  }

  void setInclusiveR2(const double &r2) {
    incR2 = r2;
  }

private:
  TFile *file{nullptr};
  TDirectory *dir{nullptr};

  THnSparse* hSparse{nullptr};

  TH1* jetPt{nullptr}; // fully inclusive

  TH1* jetPtIn_rhoPhi{nullptr}; // Inclusive
  TH1* jetPtOut_rhoPhi{nullptr};

  TH1* jetPtIn_q2Low_rhoPhi{nullptr}; // low q2
  TH1* jetPtOut_q2Low_rhoPhi{nullptr};

  TH1* jetPtIn_q2High_rhoPhi{nullptr}; // high q2
  TH1* jetPtOut_q2High_rhoPhi{nullptr};

  TH1* jetPt_OutDividedIn{nullptr};
  TH1* jetPt_OutDividedIn_q2Low{nullptr};
  TH1* jetPt_OutDividedIn_q2High{nullptr};


  std::vector<int> cent_30_50 = {30,50};

  TObjArray* h{nullptr};
  TObjArray* hLow{nullptr};
  TObjArray* hHigh{nullptr};

  unique_ptr<JEFW> jet;
  
  double eventScale;

  int nBins;
  std::vector<double> binEdges;

  double lowR2 = 0.546;
  double incR2 = 0.671;
  double highR2 = 0.608;
  

  template <bool rhoPhi = true, q2RangeType rangeType>
  void separate() {
    std::vector<int> q2{};
    switch (rangeType) {
      case q2RangeType::INCLUSIVE: q2 = {0, 100}; break;
      case q2RangeType::LOW:       q2 = {0, 30};  break;
      case q2RangeType::HIGH:      q2 = {70, 100}; break;
    }
    jet->setCentrality<rhoPhi>(cent_30_50);
    jet->setq2Range<rhoPhi>(q2);
    switch (rangeType) {
      case q2RangeType::INCLUSIVE: h = jet->separatePlanes<true>("inc"); break;
      case q2RangeType::LOW:       hLow = jet->separatePlanes<true>("low"); break;
      case q2RangeType::HIGH:      hHigh = jet->separatePlanes<true>("high"); break;
    }
  }

  template <Plane planeType, q2RangeType rangeType, Rebin rebin, bool EPR2>
  TH1* mightSetPt() {
    auto& hRef = (rangeType == q2RangeType::INCLUSIVE) ? h : 
                (rangeType == q2RangeType::LOW) ? hLow : hHigh;

    auto& jetPtIn = (rangeType == q2RangeType::INCLUSIVE) ? jetPtIn_rhoPhi : 
                    (rangeType == q2RangeType::LOW) ? jetPtIn_q2Low_rhoPhi : 
                    jetPtIn_q2High_rhoPhi;

    auto& jetPtOut = (rangeType == q2RangeType::INCLUSIVE) ? jetPtOut_rhoPhi : 
                    (rangeType == q2RangeType::LOW) ? jetPtOut_q2Low_rhoPhi : 
                    jetPtOut_q2High_rhoPhi;

    auto resR2 = (rangeType == q2RangeType::INCLUSIVE) ? incR2 : 
                (rangeType == q2RangeType::LOW) ? lowR2 : 
                highR2;

    const char* inName = (rangeType == q2RangeType::INCLUSIVE) ? "hv_0inc" : 
                        (rangeType == q2RangeType::LOW) ? "hv_0low" : 
                        "hv_0high";

    const char* outName = (rangeType == q2RangeType::INCLUSIVE) ? "hv_1inc" : 
                          (rangeType == q2RangeType::LOW) ? "hv_1low" : 
                          "hv_1high";

    if (!hRef) {
      this->separate<true, rangeType>();
      jetPtIn = dynamic_cast<TH1F*>(hRef->FindObject(inName));
      jetPtOut = dynamic_cast<TH1F*>(hRef->FindObject(outName));
      if constexpr (rebin == Rebin::YES) {
        jetPtIn = jetPtIn->Rebin(nBins, Form("%s_reb", inName), binEdges.data());
        jetPtOut = jetPtOut->Rebin(nBins, Form("%s_reb", outName), binEdges.data());
      }
      jetPtIn->Scale(1.0 / eventScale, "width");
      jetPtOut->Scale(1.0 / eventScale, "width");
      if constexpr (EPR2) {
        auto jetScaler = JEUtility(jetPtIn, jetPtOut, resR2);
        auto hs = jetScaler.YieldCorrectedJet();
        jetPtIn = hs.first;
        jetPtOut = hs.second;
      }
    }

    if constexpr (planeType == Plane::IN)
      return jetPtIn;
    else if constexpr (planeType == Plane::OUT)
      return jetPtOut;
    else
      return nullptr;
  }

  template <q2RangeType rangeType, Rebin rebin, bool EPR2=true>
  TH1* mightSetRatioPt() {
    auto &jetPtIn = (rangeType == q2RangeType::INCLUSIVE) ? jetPtIn_rhoPhi : 
                    (rangeType == q2RangeType::LOW) ? jetPtIn_q2Low_rhoPhi : 
                    jetPtIn_q2High_rhoPhi;

    auto &jetPtOut = (rangeType == q2RangeType::INCLUSIVE) ? jetPtOut_rhoPhi :
                     (rangeType == q2RangeType::LOW) ? jetPtOut_q2Low_rhoPhi :
                     jetPtOut_q2High_rhoPhi;

    auto &jetPtOutDivided = (rangeType == q2RangeType::INCLUSIVE) ? jetPt_OutDividedIn :
                            (rangeType == q2RangeType::LOW) ? jetPt_OutDividedIn_q2Low :
                            jetPt_OutDividedIn_q2High;

    if (!jetPtOutDivided) {
      if (!jetPtIn || !jetPtOut) {
        jetPtIn = mightSetPt<Plane::IN, rangeType, rebin, EPR2>();
        jetPtOut = mightSetPt<Plane::OUT, rangeType, rebin, EPR2>();
      }
      jetPtOutDivided = dynamic_cast<TH1F*>(jetPtOut->Clone(Form("%s_divided_%s", jetPtOut->GetName(), jetPtIn->GetName())));
      jetPtOutDivided->Divide(jetPtOut, jetPtIn);
    }

    return jetPtOutDivided;
  }

  void setEventScale(TH1* centHist) {
    eventScale = centHist->Integral(centHist->GetXaxis()->FindBin(cent_30_50.at(0)),centHist->GetXaxis()->FindBin(cent_30_50.at(1)));
  }



  static constexpr std::array<Int_t,8> colors = {kBlack, kRed+1 , kBlue+1, kGreen+3, kMagenta+1, kOrange-1,kCyan+2,kYellow+2};
  static constexpr std::array<Int_t, 10> markers = {kFullCircle, kFullSquare,kOpenCircle,kOpenSquare,kOpenDiamond,kOpenCross,kFullCross,kFullDiamond,kFullStar,kOpenStar};
};

