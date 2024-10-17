// This software is distributed under the terms of the GNU General Public
// License v3 (GPL Version 3), copied verbatim in the file "COPYING".
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

/*
Author: Joachim Hansen
*/

#include "JEfficiency.h"

#include "RooUnfoldResponse.h"
#include "RooUnfoldBayes.h"

ClassImp(JEfficiency);

JEfficiency::JEfficiency(TH1F* hist, TH2F* RESPONSE) : 
hist{hist},
RESPONSE{RESPONSE}
{
}

JEfficiency::~JEfficiency() 
{
}

TH1D* JEfficiency::Unfold(int iterations) {
  TH1F* hMeasured = (TH1F*) RESPONSE->ProjectionX("hMeasured");
  TH1F* hTrue = (TH1F*) RESPONSE->ProjectionY("hTrue");

  
  RooUnfoldResponse response(hMeasured, hTrue, RESPONSE);
  RooUnfoldBayes unfold(&response, hist, iterations);
  TH1D* hUnfolded = reinterpret_cast<TH1D*>(unfold.Hreco());
  return hUnfolded;
};
