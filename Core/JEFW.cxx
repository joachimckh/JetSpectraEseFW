// This software is distributed under the terms of the GNU General Public
// License v3 (GPL Version 3), copied verbatim in the file "COPYING".
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

/*
Author: Joachim Hansen
*/

#include "JEFW.h"

ClassImp(JEFW);

JEFW::JEFW(const char* name) : 
inFile{nullptr},
path{name}
{
}

JEFW::~JEFW() 
{
}

JEFW::DataType JEFW::getDataType(const std::string& type) const {
  if (type == "data") {
    return DATA;
  } else if (type == "mc") {
    return MC;
  } else {
    throw std::invalid_argument("Invalid data type");
  }
}

void JEFW::Init(std::string type) {
  inFile = unique_ptr<TFile>(new TFile(path,"READ"));
  auto dir = reinterpret_cast<TDirectoryFile*>(inFile->Get("jet-spectra-ese-task"));
  // auto container = reinterpret_cast<JetEseContainer*>(dir->Get("jetContainer"));
  // auto jet_data = container->GetDataArray();
  
  switch (getDataType(type)) {
    case DATA:
      hist = shared_ptr<TH3F>(reinterpret_cast<TH3F*>(dir->Get(this->GetPtName("_pt_dPhi_q2"))));
      h_pt_bkg = reinterpret_cast<TH1F*>(dir->Get("h_jet_pt_bkgsub"));
      break;
    case MC:
      hist_mc = shared_ptr<TH2F>(reinterpret_cast<TH2F*>(dir->Get(this->GetMCName())));
      hmatched = reinterpret_cast<TH1F*>(dir->Get("h_part_jet_pt_match"));
      htruth = reinterpret_cast<TH1F*>(dir->Get("h_part_jet_pt"));
      break;
  }
  
};

void JEFW::DrawRaw() const {
  TCanvas c("tmp","",800,600);
  c.cd();
  hist->Draw();
  c.SaveAs("figures/raw_pt_dphi_q2.pdf");
};

void JEFW::DrawXYZ(const int lvl) {
  TCanvas c("tmp","",800,600);
  
  c.cd();
  const char* name;
  shared_ptr<TH1D> htmp;
  switch(lvl) { 
    case 0:
      htmp = shared_ptr<TH1D>(hist->ProjectionX("_tmpX"));
      name = "pt";
      break;
    case 1:
      htmp = shared_ptr<TH1D>(hist->ProjectionY("_tmpY"));
      name = "dPhi";
      break;
    case 2:
      htmp = shared_ptr<TH1D>(hist->ProjectionZ("_tmpZ"));
      name = "q2";
      break;
  }
  htmp->SetLineColor(kBlack);
  htmp->Draw();
  
  c.Draw();
  c.SaveAs(Form("figures/%s.pdf",name));
};

int JEFW::PlaneState(const float &dPhi) {

  if ( TMath::Abs(TMath::Cos(dPhi)) <= TMath::Sqrt(2)/2.0 ) return 0; // in plane
  if ( TMath::Abs(TMath::Cos(dPhi)) > TMath::Sqrt(2)/2.0 ) return 1; // out of plane
  return -1;
};

void JEFW::SeparatePlanes(std::vector<int> vec_q2limits) {
  unique_ptr<TH3F> hTMP = unique_ptr<TH3F>(reinterpret_cast<TH3F*>(hist->Clone(Form("_pt_dPhi_q2_%i_%i", vec_q2limits.at(0), vec_q2limits.at(1)))));
  hTMP->GetZaxis()->SetRange(vec_q2limits.at(0),vec_q2limits.at(1));
  unique_ptr<TH1> h = unique_ptr<TH1>(reinterpret_cast<TH1*>(hTMP->Project3D("yx")));

  
  TObjArray* hv_pt = new TObjArray();
  hv_pt->SetName("planes");
  hv_pt->SetOwner(kTRUE);

  auto ax1 = hTMP->GetXaxis(); //pt
  for (int i{0}; i<2; i++) {
    hv_pt->Add(new TH1F(Form("hv_%i",i),";#it{p}_{T,jet};entries",ax1->GetNbins(), ax1->GetXmin(), ax1->GetXmax()));
  }
  
  auto ax2 = hTMP->GetYaxis(); //dphi
  

  TH1D* projPhi = reinterpret_cast<TH1D*>(hTMP->ProjectionY("_dPHIY"));
  std::vector<int> fKeep;
  for (int i{1}; i<ax2->GetNbins()+1; i++) {
    float dphi = projPhi->GetBinContent(i);
    int vPlane = this->PlaneState(dphi);
    fKeep.push_back(vPlane);

    if (vPlane<0) continue;
    TH1D* hL = reinterpret_cast<TH1D*>(hv_pt->FindObject(Form("hv_%i",vPlane)));
    if (!hL) {
      printf("could not find histogram");
      continue;
    }
    auto tmpH = reinterpret_cast<TH1D*>(hist->ProjectionX(Form("_tmpPTXjet%i",i),i,i,vec_q2limits.at(0),vec_q2limits.at(1)));
    hL->Add(tmpH);
  }
  
  hv_pt->SaveAs(Form("root_files/SeparatePtPlane_q2_%i_%i.root",vec_q2limits.at(0),vec_q2limits.at(1)));
  delete hv_pt;
};

void JEFW::AziIntEse(std::vector<int> vec_q2limits) {
  unique_ptr<TH3F> hTMP = unique_ptr<TH3F>(reinterpret_cast<TH3F*>(hist->Clone(Form("_pt_dPhi_q2_%i_%i", vec_q2limits.at(0), vec_q2limits.at(1)))));
  unique_ptr<TH2F> h_ptq2 = unique_ptr<TH2F>(reinterpret_cast<TH2F*>(hTMP->Project3D("zx")));

  auto h_out = h_ptq2->ProjectionX(Form("pt_q2_%i_%i",vec_q2limits.at(0),vec_q2limits.at(1)),vec_q2limits.at(0),vec_q2limits.at(1));

  h_out->SaveAs(Form("root_files/pt_dphiInt_q2_%i_%i.root",vec_q2limits.at(0),vec_q2limits.at(1)));
};
