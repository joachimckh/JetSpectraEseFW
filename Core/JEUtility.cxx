/// \file JEUtility.cxx
/// \brief Utility Class for handling jet spectra ESE framework
///
/// \author Joachim C. K. B. Hansen

#include "JEUtility.h"

ClassImp(JEUtility);

JEUtility::JEUtility(TFile* planes, double R) : R(R)
{
  h_in = reinterpret_cast<TH1*>(planes->Get("hv_0"));
  h_out = reinterpret_cast<TH1*>(planes->Get("hv_1"));
}

JEUtility::~JEUtility() 
{
}

TH1* JEUtility::calculateV2Jet()
{
  const double cst = std::numbers::pi/( 3.0 * std::sqrt(3.0) );

  TH1* hReturn = new TH1D("hV2Jet",";#it{p}_{T,jet}; #it{v}_{2}",210,0,210);

  for (int i{1}; i < h_in->GetNbinsX()+1; i++)
  {
    double A = h_in->GetBinContent(i);
    double B = h_out->GetBinContent(i);
    double sigmaA = h_in->GetBinError(i);
    double sigmaB = h_out->GetBinError(i);


    double R2inv = 1 / R;
    double num = A - B;
    double denom = A + B;
    double val = (denom!=0) ? num/denom : 0;
    
    double v2 = cst * R2inv * val;
    double error = (denom!=0) ? cst * R2inv * 2 / (denom * denom) * TMath::Sqrt(B * B * sigmaA * sigmaA + A * A * sigmaB * sigmaB) : 0;

    hReturn->SetBinContent(i, v2);
    hReturn->SetBinError(i, error);


  }
  return hReturn;
};

