/// \file JEFW.cxx
/// \brief Class for handling jet spectra ESE framework
///
/// \author Joachim C. K. B. Hansen

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

JEFW::DataType JEFW::getDataType(const std::string& type) const 
{
  if (type == "data") {
    return DATA;
  } else if (type == "mc") {
    return MC;
  } else {
    throw std::invalid_argument("Invalid data type");
  }
}

void JEFW::Init(std::string type)
{
  inFile = unique_ptr<TFile>(new TFile(path,"READ"));
  dir = reinterpret_cast<TDirectoryFile*>(inFile->Get("jet-spectra-ese-task"));
  
  switch (getDataType(type)) {
    case DATA:
      hist = reinterpret_cast<TH3F*>(dir->Get("jet_pt_dPhi_q2;1")); /* hJetPtdPhiq2 */
      h_pt_bkg = reinterpret_cast<TH1F*>(dir->Get("h_jet_pt_bkgsub")); /* hJetPt_bkgsub */

      break;
    case MC:
      hist_mc = reinterpret_cast<TH2F*>(dir->Get("h_response_mat_match"));
      hmatched = reinterpret_cast<TH1F*>(dir->Get("h_part_jet_pt_match"));
      htruth = reinterpret_cast<TH1F*>(dir->Get("h_part_jet_pt"));
      break;
  }
  
};

void JEFW::DrawRaw() const
{
  TCanvas c("tmp","",800,600);
  c.cd();
  hist->Draw();
  c.SaveAs("figures/raw_pt_dphi_q2.pdf");
};

void JEFW::DrawXYZ(const int lvl)
{
  TCanvas c("tmp","",800,600);
  
  c.cd();
  const char* name;
  TH1D* htmp;
  switch(lvl) { 
    case 0:
      htmp = hist->ProjectionX("_tmpX");
      name = "pt";
      break;
    case 1:
      htmp = hist->ProjectionY("_tmpY");
      name = "dPhi";
      break;
    case 2:
      htmp = hist->ProjectionZ("_tmpZ");
      name = "q2";
      break;
  }
  htmp->SetLineColor(kBlack);
  htmp->Draw();
  
  c.Draw();
  c.SaveAs(Form("figures/%s.pdf",name));
};

int JEFW::PlaneState(const float &dPhi) 
{
  if ( TMath::Abs(TMath::Cos(dPhi)) <= TMath::Sqrt(2)/2.0 ) return 0; // in plane 30 * TMath::Pi()/180
  if ( TMath::Abs(TMath::Cos(dPhi)) > TMath::Sqrt(2)/2.0 ) return 1; // out of plane 60 * TMath::Pi()/180
  return -1;
};

auto JEFW::SeparatePlanes(std::vector<int> vec_q2limits) 
{
  TH3F* hTMP = reinterpret_cast<TH3F*>(hist->Clone(Form("_pt_dPhi_q2_%i_%i", vec_q2limits.at(0), vec_q2limits.at(1))));
  hTMP->GetZaxis()->SetRange(vec_q2limits.at(0),vec_q2limits.at(1));
  TH1* h = reinterpret_cast<TH1*>(hTMP->Project3D("yx"));

  
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
  
  return hv_pt;
  // hv_pt->SaveAs(Form("root_files/SeparatePtPlane_q2_%i_%i.root",vec_q2limits.at(0),vec_q2limits.at(1)));
  // delete hv_pt;
};

TH1* JEFW::AziIntEse(std::vector<int> vec_q2limits)
{
  TH3F* hTMP = reinterpret_cast<TH3F*>(hist->Clone(Form("_pt_dPhi_q2_%i_%i", vec_q2limits.at(0), vec_q2limits.at(1))));
  TH2F* h_ptq2 = reinterpret_cast<TH2F*>(hTMP->Project3D("zx"));

  TH1* h_out = h_ptq2->ProjectionX(Form("pt_q2_%i_%i",vec_q2limits.at(0),vec_q2limits.at(1)),vec_q2limits.at(0),vec_q2limits.at(1));

  return h_out;
};

TH1F* JEFW::eventPlaneResolution(std::string A, std::string B, std::string C)
{

  const char* nameAC = Form("hCosPsi2%sm%s", A.c_str(), C.c_str());
  const char* nameAB = Form("hCosPsi2%sm%s", A.c_str(), B.c_str());
  const char* nameBC = Form("hCosPsi2%sm%s", B.c_str(), C.c_str());


  TH3F* hAC = reinterpret_cast<TH3F*>(dir->Get(nameAC));
  TH3F* hAB = reinterpret_cast<TH3F*>(dir->Get(nameAB));
  TH3F* hBC = reinterpret_cast<TH3F*>(dir->Get(nameBC));

  /* centrality, cos(2 (psiN - psiM)), q2PERC */
  TH2D* hAC_centInt = dynamic_cast<TH2D*>(hAC->Project3D("yz"));
  TH2D* hAB_centInt = dynamic_cast<TH2D*>(hAB->Project3D("yz"));
  TH2D* hBC_centInt = dynamic_cast<TH2D*>(hBC->Project3D("yz"));

  TProfile *pAC = hAC_centInt->ProfileX();
  TProfile *pAB = hAB_centInt->ProfileX();
  TProfile *pBC = hBC_centInt->ProfileX();

  TH1F* hOut = new TH1F(Form("h%s%s%s",A.c_str(),B.c_str(),C.c_str()),";#it{q}_{2}; R_{2}",100,0,100);

  for (int i{0}; i< pAC->GetNbinsX(); i++)
  {
    double A = pAC->GetBinContent(i);
    double B = pAB->GetBinContent(i);
    double C = pBC->GetBinContent(i);
    double sigmaA = pAC->GetBinError(i);
    double sigmaB = pAB->GetBinError(i);
    double sigmaC = pBC->GetBinError(i);

    double val = std::sqrt(A * B / C);
    double termA = (B / (A * C)) * sigmaA;
    double termB = (A / (B * C)) * sigmaB;
    double termC = (A * B / (C * C * C)) * sigmaC;
    double error = 0.5 * std::sqrt(termA * termA + termB * termB + termC * termC);

    hOut->SetBinContent(i,val);
    hOut->SetBinError(i,error);
  }
  
  return hOut;
};

TH1* JEFW::getEventPlane(const char* name)
{
  TH2F* h = reinterpret_cast<TH2F*>(dir->Get(Form("hPsi2%s",name)));
  TH1* hOut = h->ProjectionY(Form("psi2_%s",name));
  return hOut;
};

void JEFW::JERebin(int n, Double_t* bin_edges)
{
  /* rebin */
  hmatched = reinterpret_cast<TH1F*>(hmatched->Rebin(n, "hMatchedReb", bin_edges));
  htruth = reinterpret_cast<TH1F*>(htruth->Rebin(n, "hTruthReb", bin_edges));
};
