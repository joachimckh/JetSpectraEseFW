/// \file JEfficiency.h
/// \brief Header file for JEfficiency class
///
/// \author Joachim C. K. B. Hansen

#pragma once

#include <vector>
#include <string>
#include <utility>
#include <algorithm>
#include <memory>

#include <TObjArray.h>
#include <TH3D.h>
#include <TH2D.h>
#include <TH1D.h>
#include <TFile.h>
#include <TMath.h>
#include <THnSparse.h>

#include <RooUnfoldResponse.h>

#include <iostream>

using std::shared_ptr;
using std::unique_ptr;


class JEfficiency {
  public:
    JEfficiency() = default;
    explicit JEfficiency(TH1D* hData /* measured data */, 
                         THnSparse* hMCRespSparse /* MCD, MCP*/, 
                         TH1D* hRecMatched /* geo match Particle: Generated*/, 
                         TH2F* hCentRhoRandomConewoLeadingJet,
                         TH1* hGenUnmatched,
                         TH1* hRecUnmatched);
    // explicit JEfficiency(THnSparse* hMCRespSparse /* MCD, MCP*/, 
    //                      TH1D* hRecMatched /* geo match Particle: Generated*/, 
    //                      TH1* hGenUnmatched,
    //                      TH1* hRecUnmatched);
    ~JEfficiency();


    TH1D* bayesUnfold(int iterations, bool fRebin);
    TH1D* svdUnfold(int iterations);
    TH1D* Efficiency();
    TH1D* Purity();
    
    void ptResponseFluctuations();
    void setRespSettings();

    TH2D* getFluctuations() const { return hResponseFluctuations; };
    void createResponseMatrix();

    RooUnfoldResponse *createResponse(bool fRebin);

    TH2D* getResponseTotal() const { return hResponseTotal; };
    TH2D* getResponseDetector() const { return hResponseDetector; };

    void setPbPb( bool setPbPb, std::pair<int,int> cent ) { fPbPb = setPbPb; centRange = cent; };


  private:
    TH1D* hData;
    THnSparse* hMCRespSparse; /* from JS: MCD, MCP */
    TH1D* hRecMatched;
    TH2F* hCentRhoRandomConewoLeadingJet;
  
    TH2D* hResponseFluctuations;
    TH2D* hResponseDetector;
    TH2D* hResponseTotal;
    TH1D* hFluctuations;

    TH1* hGenUnmatched;
    TH1* hRecUnmatched;

    bool fPbPb;
    std::pair<int,int> centRange;



  ClassDef(JEfficiency, 1); // jet unfolding and efficiency class

};

