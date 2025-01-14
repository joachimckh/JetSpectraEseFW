/// \file JEFW.cxx
/// \brief Class for handling jet spectra ESE framework
///
/// \author Joachim C. K. B. Hansen

#include "JEFW.hpp"

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

std::pair<double, double> JEFW::getEPR3Val(TProfile* pAC, TProfile* pAB, TProfile* pBC, int i)
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

  double insqrt = termA * termA + termB * termB + termC * termC;
  double error = 0.5 * std::sqrt(insqrt);

  return std::make_pair(val,error);
}

void JEFW::Init(std::string type)
{
  inFile = unique_ptr<TFile>(new TFile(path,"READ"));
  dir = dynamic_cast<TDirectoryFile*>(inFile->Get("jet-spectra-ese-task"));
  
  switch (getDataType(type)) {
    case DATA:
      hist = dynamic_cast<THnSparse*>(dir->Get("hCentJetPtdPhiq2;1;1")); /* hJetPtdPhiq2 old */
      if (!hist) {
        std::cerr << "could not find the thnsparse" << std::endl;
        return;
      }
      h_pt_bkg = dynamic_cast<TH1F*>(dir->Get("hJetPt_bkgsub")); /* hJetPt_bkgsub old */

      break;
    case MC:
      hist_mc = dynamic_cast<TH2F*>(dir->Get("h_response_mat_match"));
      hmatched = dynamic_cast<TH1F*>(dir->Get("h_part_jet_pt_match"));
      htruth = dynamic_cast<TH1F*>(dir->Get("h_part_jet_pt"));
      break;
  }
  
};

void JEFW::drawRaw() const
{
  TCanvas c("tmp","",800,600);
  c.cd();
  hist->Draw();
  c.SaveAs("figures/raw_pt_dphi_q2.pdf");
};

void JEFW::drawXYZ(const int lvl)
{
  TCanvas c("tmp","",800,600);
  
  c.cd();
  const char* name;
  TH1D* htmp;
  switch(lvl) { 
    case 0:
      htmp = hist->Projection(1);
      name = "pt";
      break;
    case 1:
      htmp = hist->Projection(2);
      name = "dPhi";
      break;
    case 2:
      htmp = hist->Projection(3);
      name = "q2";
      break;
  }
  htmp->SetLineColor(kBlack);
  htmp->Draw();
  
  c.Draw();
  c.SaveAs(Form("figures/%s.pdf",name));
};

int JEFW::planeState(const float &dPhi) 
{
  if ( TMath::Abs(TMath::Cos(dPhi)) <= TMath::Sqrt(2)/2.0 ) return 0; // in plane 30 * TMath::Pi()/180
  if ( TMath::Abs(TMath::Cos(dPhi)) > TMath::Sqrt(2)/2.0 ) return 1; // out of plane 60 * TMath::Pi()/180
  return -1;
};

void JEFW::setCentrality(const std::vector<int> vec_centlimits)
{
  hist->GetAxis(0)->SetRange(vec_centlimits.at(0),vec_centlimits.at(1));
};

TObjArray* JEFW::separatePlanes(std::vector<int> vec_q2limits) 
{
  // TH3F* hTMP = dynamic_cast<TH3F*>(hist->Clone(Form("_pt_dPhi_q2_%i_%i", vec_q2limits.at(0), vec_q2limits.at(1))));
  TH3F* hTMP = dynamic_cast<TH3F*>(hist->Projection(1 /* jet pT */,2 /* Delta Phi */,3 /* q2 */)->Clone(Form("_pt_dPhi_q2_%i_%i", vec_q2limits.at(0), vec_q2limits.at(1))));
  hTMP->GetZaxis()->SetRange(vec_q2limits.at(0),vec_q2limits.at(1));
  TH1* h = dynamic_cast<TH1*>(hTMP->Project3D("yx"));

  
  TObjArray* hv_pt = new TObjArray();
  hv_pt->SetName("planes");
  hv_pt->SetOwner(kTRUE);

  auto ax1 = hTMP->GetXaxis(); //pt
  for (int i{0}; i<2; i++) {
    hv_pt->Add(new TH1F(Form("hv_%i",i),";#it{p}_{T,jet};entries",ax1->GetNbins(), ax1->GetXmin(), ax1->GetXmax()));
  }
  
  auto ax2 = hTMP->GetYaxis(); //dphi
  

  TH1D* projPhi = dynamic_cast<TH1D*>(hTMP->ProjectionY("_dPHIY"));
  std::vector<int> fKeep;
  for (int i{1}; i<ax2->GetNbins()+1; i++) {
    float dphi = projPhi->GetBinContent(i);
    int vPlane = this->planeState(dphi);
    fKeep.push_back(vPlane);

    if (vPlane<0) continue;
    TH1D* hL = dynamic_cast<TH1D*>(hv_pt->FindObject(Form("hv_%i",vPlane)));
    if (!hL) {
      printf("could not find histogram");
      continue;
    }
    auto tmpH = dynamic_cast<TH1D*>(hTMP->ProjectionX(Form("_tmpPTXjet%i",i),i,i,vec_q2limits.at(0),vec_q2limits.at(1)));
    hL->Add(tmpH);
  }
  
  return hv_pt;
  // hv_pt->SaveAs(Form("root_files/SeparatePtPlane_q2_%i_%i.root",vec_q2limits.at(0),vec_q2limits.at(1)));
  // delete hv_pt;
};

TH1* JEFW::aziIntEse(std::vector<int> vec_q2limits)
{
  TH3F* hTMP = dynamic_cast<TH3F*>(hist->Clone(Form("_pt_dPhi_q2_%i_%i", vec_q2limits.at(0), vec_q2limits.at(1))));
  TH2F* h_ptq2 = dynamic_cast<TH2F*>(hTMP->Project3D("zx"));

  TH1* h_out = h_ptq2->ProjectionX(Form("pt_q2_%i_%i",vec_q2limits.at(0),vec_q2limits.at(1)),vec_q2limits.at(0),vec_q2limits.at(1));

  return h_out;
};

TH1F* JEFW::eventPlaneResolution(std::string A, std::string B, std::string C, std::pair<int,int> cent)
{

  const char* nameAC = Form("hCosPsi2%sm%s", A.c_str(), C.c_str());
  const char* nameAB = Form("hCosPsi2%sm%s", A.c_str(), B.c_str());
  const char* nameBC = Form("hCosPsi2%sm%s", B.c_str(), C.c_str());


  TH3F* hAC = dynamic_cast<TH3F*>(dir->Get(nameAC));
  TH3F* hAB = dynamic_cast<TH3F*>(dir->Get(nameAB));
  TH3F* hBC = dynamic_cast<TH3F*>(dir->Get(nameBC));

  hAC->GetXaxis()->SetRange(cent.first,cent.second);
  hAB->GetXaxis()->SetRange(cent.first,cent.second);
  hBC->GetXaxis()->SetRange(cent.first,cent.second);

  /* centrality, cos(2 (psiN - psiM)), q2PERC */
  TH2D* hAC_centInt = dynamic_cast<TH2D*>(hAC->Project3D("yz"));
  TH2D* hAB_centInt = dynamic_cast<TH2D*>(hAB->Project3D("yz"));
  TH2D* hBC_centInt = dynamic_cast<TH2D*>(hBC->Project3D("yz"));

  hAC->GetXaxis()->SetRange(0, 0);
  hAB->GetXaxis()->SetRange(0, 0);
  hBC->GetXaxis()->SetRange(0, 0);

  TProfile *pAC = dynamic_cast<TProfile*>(hAC_centInt->ProfileX());
  TProfile *pAB = dynamic_cast<TProfile*>(hAB_centInt->ProfileX());
  TProfile *pBC = dynamic_cast<TProfile*>(hBC_centInt->ProfileX());

  TH1F* hOut = new TH1F(Form("h%s%s%s",A.c_str(),B.c_str(),C.c_str()),";#it{q}_{2}; R_{2}",100,0,100);

  for (int i{1}; i< pAC->GetNbinsX()+1; i++)
  {
    auto [val, error] = getEPR3Val(pAC,pAB,pBC,i);

    if (val > 3 || val < 0) continue;
    
    hOut->SetBinContent(i,val);
    hOut->SetBinError(i,error);
  }
  return hOut;
};

TH1* JEFW::getEventPlane(const char* name, std::pair<int,int> cent)
{
  TH2F* h = dynamic_cast<TH2F*>(dir->Get(Form("hPsi2%s",name)));
  TH1* hOut = h->ProjectionY(Form("psi2_%s",name),cent.first,cent.second);
  return hOut;
};

TH1* JEFW::getR2S(const char* name, std::pair<int,int> cent)
{
  TH3F* h = dynamic_cast<TH3F*>(dir->Get(Form("hCosPsi2%s",name)));
  // h->GetXaxis()->SetRange(cent.first,cent.second);
  TH1* hOut = h->ProjectionY(Form("PROJhCosPsi2%s",name),cent.first,cent.second);
  return hOut;
}

void JEFW::JERebin(int n, Double_t* bin_edges)
{
  /* rebin */
  hmatched = dynamic_cast<TH1F*>(hmatched->Rebin(n, "hMatchedReb", bin_edges));
  htruth = dynamic_cast<TH1F*>(htruth->Rebin(n, "hTruthReb", bin_edges));
};

std::vector<TH1*> JEFW::inclusiveEPR(std::string A, std::string B, std::string C)
{
  const char* nameAC = Form("hCosPsi2%sm%s", A.c_str(), C.c_str());
  const char* nameAB = Form("hCosPsi2%sm%s", A.c_str(), B.c_str());
  const char* nameBC = Form("hCosPsi2%sm%s", B.c_str(), C.c_str());


  TH3F* hAC = dynamic_cast<TH3F*>(dir->Get(nameAC));
  TH3F* hAB = dynamic_cast<TH3F*>(dir->Get(nameAB));
  TH3F* hBC = dynamic_cast<TH3F*>(dir->Get(nameBC));

  auto hAC2 = dynamic_cast<TH2*>(hAC->Project3D("yx"));
  auto hAB2 = dynamic_cast<TH2*>(hAB->Project3D("yx"));
  auto hBC2 = dynamic_cast<TH2*>(hBC->Project3D("yx"));

  TProfile *pAC = dynamic_cast<TProfile*>(hAC2->ProfileX());
  TProfile *pAB = dynamic_cast<TProfile*>(hAB2->ProfileX());
  TProfile *pBC = dynamic_cast<TProfile*>(hBC2->ProfileX());

  TH1F* hCombined = new TH1F(Form("INCh%s%s%s",A.c_str(),B.c_str(),C.c_str()),";Centrality; R_{2}",100,0,100);
  for (int i{1}; i< pAC->GetNbinsX()+1; i++)
  {
    auto [val, error] = getEPR3Val(pAC,pAB,pBC,i);

    if (val > 3 || val < 0) continue;
    
    hCombined->SetBinContent(i,val);
    hCombined->SetBinError(i,error);
  }

  std::vector<TH1*> vec;
  vec.push_back(hCombined);
  vec.push_back(pAC);
  vec.push_back(pAB);
  vec.push_back(pBC);
  return vec;
};
