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

using std::unique_ptr;



class JEFW {
  public:
    enum class DataType {
      DATA,
      MC,
      BACKGROUND
    };
    JEFW() = default;
    explicit JEFW(const char* name, DataType type, std::string subtask);
    explicit JEFW(TDirectory* dir, DataType type);
    ~JEFW();
    
    void help() {
      dir->ls();
    }

    template <bool rhophi=false>
    void setCentrality(const std::vector<int> vec_centlimits) {
      if constexpr (rhophi) {
        auto xbin = hS_histRhoPhi->GetAxis(0)->FindBin(vec_centlimits.at(0));
        auto ybin = hS_histRhoPhi->GetAxis(0)->FindBin(vec_centlimits.at(1));
        hS_histRhoPhi->GetAxis(0)->SetRange(xbin,ybin);
      } else {
        auto xbin = hS_hist->GetAxis(0)->FindBin(vec_centlimits.at(0));
        auto ybin = hS_hist->GetAxis(0)->FindBin(vec_centlimits.at(1));
        hS_hist->GetAxis(0)->SetRange(xbin,ybin);
      }

      // auto xbin = hS_hist->GetAxis(0)->FindBin(vec_centlimits.at(0));
      // auto ybin = hS_hist->GetAxis(0)->FindBin(vec_centlimits.at(1));
      // hS_hist->GetAxis(0)->SetRange(xbin,ybin);
    }
    template <bool rhophi=false>
    void setq2Range(const std::vector<int> vec_q2limits) {
      if constexpr (rhophi) {
        auto xbin = hS_histRhoPhi->GetAxis(3)->FindBin(vec_q2limits.at(0));
        auto ybin = hS_histRhoPhi->GetAxis(3)->FindBin(vec_q2limits.at(1));
        hS_histRhoPhi->GetAxis(3)->SetRange(xbin,ybin);
      } else {
        auto xbin = hS_hist->GetAxis(3)->FindBin(vec_q2limits.at(0));
        auto ybin = hS_hist->GetAxis(3)->FindBin(vec_q2limits.at(1));
        hS_hist->GetAxis(3)->SetRange(xbin,ybin);
      }
    }

    template <bool rhoPhi = true>
    TObjArray* separatePlanes(const char* id="") {
      TH2* h{nullptr};
      if constexpr (!rhoPhi)
        h = dynamic_cast<TH2*>(hS_hist->Projection(2,1));
      if constexpr (rhoPhi){
        h = dynamic_cast<TH2*>(hS_histRhoPhi->Projection(2,1));
      }
      h->SetName(Form("h2%s",id));

      if (!h) std::cerr << "could not project the TH2" << std::endl;
      
      TObjArray* hv_pt{new TObjArray()};
      hv_pt->SetName(Form("planes%s",id));
      hv_pt->SetOwner(kTRUE);

      auto ax1{h->GetXaxis()}; //pt
      auto ax2{h->GetYaxis()}; //dphi

      for (int i{0}; i<3; i++) {
        hv_pt->Add(new TH1F(Form("hv_%i%s",i,id),";#it{p}_{T,jet};entries",ax1->GetNbins(), ax1->GetXmin(), ax1->GetXmax()));
      }
      hv_pt->Add(new TH1F(Form("hv_planestatus%s",id),";plane status;entries",ax2->GetNbins(), ax2->GetXmin(), ax2->GetXmax()));

      TH1D* projPhi{dynamic_cast<TH1D*>(h->ProjectionY(Form("_dphi_hist%s",id)))};
      
      for (int i{1}; i<ax2->GetNbins()+1; i++) {
        double dphi{projPhi->GetBinCenter(i)};
        int vPlane{this->planeState(dphi)};
        // std::cout << "bin center i: " << i << " dphi: " << dphi << " plane: " << vPlane << std::endl;
        auto hplane{dynamic_cast<TH1F*>(hv_pt->FindObject(Form("hv_planestatus%s",id)))};
        int binIndex = hplane->FindBin(dphi);
        hplane->SetBinContent(binIndex, vPlane);

        TH1F* hL{dynamic_cast<TH1F*>(hv_pt->FindObject(Form("hv_%i%s",vPlane,id)))};
        if (!hL) {
          std::cerr << "could not find histogram" << std::endl;
          continue;
        }

        auto tmpHist{dynamic_cast<TH1D*>(h->ProjectionX(Form("_proj_pTjet%i%s",i,id),i,i))};
        if (!tmpHist) std::cerr << "could not project the TH2 for add" << std::endl;
        hL->Add(tmpHist);
      }
      return hv_pt;
    }

    TObjArray* process() {
      this->setCentrality<false>(std::vector<int>{30,50});
      // setq2Range<true>(0,100);
      return separatePlanes();
    }

    THnSparse* getSparse() const {
      return hS_hist;
    };
    THnSparse* getSparse2() const {
      return hS_histRhoPhi;
    };

    template <typename T>
    T* getObj(const char* name) {
      return dynamic_cast<T*>(dir->Get(name));
    }

    TH1* getUnmatched(const int &iW, const std::vector<int> *cent = nullptr) {
      TH2* tmp{nullptr};
      switch(iW) {
        case 0:
          tmp = hS_PartPt->Projection(1,0);
          tmp->SetName(Form("tmpHest%i",iW));
          break;
        case 1:
          tmp = hS_DetPt->Projection(1,0);
          tmp->SetName(Form("tmpHest%i",iW));
          break;
        default:
          break;
      }
      TH1* tmp2{nullptr};
      if (cent){
        auto xbin = tmp->GetXaxis()->FindBin(cent->at(0));
        auto ybin = tmp->GetXaxis()->FindBin(cent->at(1));
        tmp->GetXaxis()->SetRange(xbin,ybin);
      }
      tmp2 = tmp->ProjectionY(Form("tmp_%i",iW));

      return tmp2;
    }

    TH1F* getPt() const {
      return h_ptBkg;  
    };
    TH2F* getDetReco() const { return h2_detUnmatched; }
    TH2F* getParMatch() const { return h2_genMatched; }



    TH1F* eventPlaneResolutionInclusive();
    TH1F* eventPlaneResolution(std::string A, std::string B, std::string C, std::pair<int,int> cent);
    TH1* getEventPlane(const char* name, std::pair<int,int> cent, bool fScale);
    TH1* getR2S(const char* name, std::pair<int,int> cent);

    TDirectory* getDir() const { return dir; }
    double eventCounter(const int iBin);

  private:
    unique_ptr<TFile> inFile;
    TDirectory* dir;
    THnSparse* hS_hist;
    THnSparse* hS_histRhoPhi;
    TH1F* h_ptBkg; // data or gen

    TH2F* h2_detUnmatched;
    TH2F* h2_genMatched;
    THnSparse* hS_PartPt; // pure gen
    THnSparse* hS_DetPt; // pure reco

    

    std::pair<double, double> getEPR3Val(TProfile* pAC, TProfile* pAB, TProfile* pBC, int i) {
      double A{pAC->GetBinContent(i)};
      double B{pAB->GetBinContent(i)};
      double C{pBC->GetBinContent(i)};
      double sigmaA{pAC->GetBinError(i)};
      double sigmaB{pAB->GetBinError(i)};
      double sigmaC{pBC->GetBinError(i)};

      double val{std::sqrt(A * B / C)};
      double termA{(B / (A * C)) * sigmaA};
      double termB{(A / (B * C)) * sigmaB};
      double termC{(A * B / (C * C * C)) * sigmaC};

      double insqrt{termA * termA + termB * termB + termC * termC};
      double error{0.5 * std::sqrt(insqrt)};

      return std::make_pair(val,error);
    }

    int planeState(const float &dPhi) {
      if ( std::abs(dPhi) < std::numbers::pi/6 || std::abs(dPhi) > std::numbers::pi - std::numbers::pi/6) return 0; // pi/6 = 30 degrees in-plane
      if ( std::abs(dPhi) > std::numbers::pi/3 && std::abs(dPhi) < std::numbers::pi - std::numbers::pi/3) return 1; // pi/3 = 60 degrees out-of-plane
      // if ( std::abs(dPhi) < std::numbers::pi/4 || std::abs(dPhi) > std::numbers::pi - std::numbers::pi/4) return 0; // yubiao
      // if ( std::abs(dPhi) > std::numbers::pi/4 && std::abs(dPhi) < std::numbers::pi - std::numbers::pi/4) return 1;
      else return 2;
    }
   
  ClassDef(JEFW, 1); // jet fw class

};
