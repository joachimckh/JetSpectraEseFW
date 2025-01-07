/// \file JEfficiency.cxx
/// \brief Class for unfolding and calculating efficiency
///
/// \author Joachim C. K. B. Hansen


#include "JEfficiency.hpp"

#include <RooUnfoldResponse.h>
#include <RooUnfoldBayes.h>

ClassImp(JEfficiency);

JEfficiency::JEfficiency(TH1F* hist, TH2F* RESPONSE, TH1F* htruth, TH1F* hmatched) : 
hist{hist},
RESPONSE{RESPONSE},
htruth{htruth},
hmatched{hmatched}
{
}

JEfficiency::~JEfficiency() 
{
}

TH1D* JEfficiency::Unfold(int iterations)
{
  TH1F* hMeasured = (TH1F*) RESPONSE->ProjectionX("hMeasured");
  TH1F* hTrue = (TH1F*) RESPONSE->ProjectionY("hTrue");

  
  RooUnfoldResponse response(hMeasured, hTrue, RESPONSE);
  RooUnfoldBayes unfold(&response, hist, iterations);
  TH1D* hUnfolded = reinterpret_cast<TH1D*>(unfold.Hreco());
  return hUnfolded;
};

TH1D* JEfficiency::Efficiency()
{
  TH1D* out = reinterpret_cast<TH1D*>(hmatched->Clone());
  // divide mathced by truth to get efficiency
  out->Divide(htruth);
  return out;
};

