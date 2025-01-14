/// \file JEUtility.cxx
/// \brief Utility Class for handling jet spectra ESE framework
///
/// \author Joachim C. K. B. Hansen

#include "JEUtility.hpp"

#include <utility>

ClassImp(JEUtility);

JEUtility::JEUtility(TFile* planes, double R) : R(R)
{
  h_in = dynamic_cast<TH1*>(planes->Get("hv_0"));
  h_out = dynamic_cast<TH1*>(planes->Get("hv_1"));
}

JEUtility::~JEUtility() 
{
}

TH1* JEUtility::calculateV2Jet()
{
  const double cst = std::numbers::pi/( 3.0 * std::sqrt(3.0) );

  // TH1* hReturn = new TH1D("hV2Jet",";#it{p}_{T,jet}; #it{v}_{2}",210,0,210);
  TH1* hReturn = dynamic_cast<TH1*>(h_in->Clone("hV2Jet"));

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
    double error = (denom!=0) ? cst * R2inv * 2 / (denom * denom) * std::sqrt(B * B * sigmaA * sigmaA + A * A * sigmaB * sigmaB) : 0;

    hReturn->SetBinContent(i, v2);
    hReturn->SetBinError(i, error);


  }
  return hReturn;
};

void JEUtility::JERebin(int n, Double_t* bin_edges)
{
  /* rebin */
  h_in = dynamic_cast<TH1F*>(h_in->Rebin(n, "hInReb", bin_edges));
  h_out = dynamic_cast<TH1F*>(h_out->Rebin(n, "hOutReb", bin_edges));
};

std::pair<TH1*, TH1*> JEUtility::YieldCorrectedJet()
{
  /* 
    N_in^Corr = N_in + N_out / (1 + N_out^Corr/N_in^Corr) 
    N_out^Corr= N_in + N_out / (1 + N_in^Corr/N_out^Corr)
  */
  TH1* h_rat = this->YieldRatio();
  TH1* h_in_Corr = dynamic_cast<TH1*>(h_in->Clone("hYieldCorrectedJet_in"));
  TH1* h_out_Corr = dynamic_cast<TH1*>(h_out->Clone("hYieldCorrectedJet_out"));
  for (int i{1}; i < h_in->GetNbinsX()+1; i++)
  {
    double num = h_in->GetBinContent(i) + h_out->GetBinContent(i);


    double denum_in = 1.0 + h_rat->GetBinContent(i);
    double denum_out = 1.0 + 1.0/h_rat->GetBinContent(i);
    double val_in = (denum_in!=0) ? num/denum_in : 0;
    double val_out = (denum_out!=0) ? num/denum_out : 0;

    h_in_Corr->SetBinContent(i, val_in);
    h_in_Corr->SetBinError(i, h_in->GetBinError(i));
    h_out_Corr->SetBinContent(i, val_out);
    h_out_Corr->SetBinError(i, h_out->GetBinError(i));
  }


  return std::make_pair(h_in_Corr, h_out_Corr);
}
TH1* JEUtility::YieldRatio()
{ 
  /* N_out/N_in */
  TH1* hV2 = this->calculateV2Jet();
  TH1 *hRatio = dynamic_cast<TH1*>(hV2->Clone("hRatio"));
  for (int i{1}; i < hV2->GetNbinsX()+1; i++)
  {
    double num = 2 * std::numbers::pi - 1.0 * hV2->GetBinContent(i) * 6*std::sqrt(3);
    double denum = 2 * std::numbers::pi + 1.0 * hV2->GetBinContent(i) * 6*std::sqrt(3);
    double val = (denum!=0) ? num/denum : 0;
    hRatio->SetBinContent(i, val);
  }
  return hRatio;
}
