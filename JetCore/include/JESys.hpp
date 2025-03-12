/// \file JESys.h
/// \brief Header file for JESys class
///
/// \author Joachim C. K. B. Hansen
#pragma once


#include <vector>
#include <iostream>

#include <TH1.h>
#include <TF1.h>

class JESys {

public:

  JESys(TH1* numerator, TH1* denominator) : numerator(numerator), denominator(denominator) {}



  TH1 *getRatio() {
    ratio = dynamic_cast<TH1*>(numerator->Clone(Form("hRatio%i", nameCounter1++)));
    ratio->Divide(denominator);
    setDrawSettings();
    return ratio;
  }


  TH1 *getBarlow() {
    if (!barlow) barlowTest();
    setDrawSettings();
    return barlow;
  }

  void fitRatio() {
    TF1* fitFunc = new TF1("fitFunc", "pol0", 30, 120);
    fitFunc->SetParameter(0, 1);
    ratio->Fit(fitFunc, "R");
    // ratio->Fit("pol0");
  }


  template <int shiftMarker = 0>
  void setDrawSettings() {
    auto applyStyle = [](TH1* hist, size_t colorIndex, size_t markerIndex) {
      if (!hist) return;
      hist->SetMarkerStyle(markers[(markerIndex + (shiftMarker)) % markers.size()]);
      hist->SetMarkerColor(colors[(colorIndex + (shiftMarker+2)) % colors.size()]);
      hist->SetLineColor(colors[(colorIndex+ (shiftMarker+2)) % colors.size()]);
      hist->SetStats(0);
    };

    applyStyle(ratio, 0, 0);  // kBlack, kFullCircle
    applyStyle(barlow, 1, 0);  // kRed+1, kFullCircle

  }

private:
  TH1* numerator{nullptr};
  TH1* denominator{nullptr};
  TH1* ratio{nullptr};

  TH1* barlow{nullptr};

  int nameCounter1 = 0;
  int nameCounter2 = 0;



  void barlowTest() {
    // |def - variation| / sqrt(|sigma_def|^2 - |sigma_variation|^2)
    if (!barlow) {
      barlow = dynamic_cast<TH1*>(numerator->Clone(Form("hBarlow%i", nameCounter2++)));
      for (int i{1}; i < numerator->GetNbinsX()+1; i++)
      {
        double def{numerator->GetBinContent(i)};
        double var{denominator->GetBinContent(i)};
        double sigma_def{numerator->GetBinError(i)};
        double sigma_var{denominator->GetBinError(i)};

        double val{std::abs(def - var) / std::sqrt(std::abs(sigma_def * sigma_def - sigma_var * sigma_var))};
        barlow->SetBinContent(i, val);
      }
    }
  }


  static constexpr std::array<Int_t,8> colors = {kBlack, kRed+1 , kBlue+1, kGreen+3, kMagenta+1, kOrange-1,kCyan+2,kYellow+2};
  static constexpr std::array<Int_t, 10> markers = {kFullCircle, kFullSquare,kOpenCircle,kOpenSquare,kOpenDiamond,kOpenCross,kFullCross,kFullDiamond,kFullStar,kOpenStar};
};
