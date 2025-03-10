/// \file JEFW.cxx
/// \brief Class for handling jet spectra ESE framework
///
/// \author Joachim C. K. B. Hansen

#include "JEFW.hpp"

ClassImp(JEFW);

JEFW::JEFW(const char* name, JEFW::DataType type, std::string subtask) : 
inFile{nullptr},
dir{nullptr},
hS_hist{nullptr}
{
  inFile = unique_ptr<TFile>(new TFile(name,"READ"));
  dir = dynamic_cast<TDirectoryFile*>(inFile->Get(Form("jet-spectra-ese-task%s",subtask.c_str())));
  
  if (type == JEFW::DataType::DATA) {
    hS_hist = dynamic_cast<THnSparse*>(dir->Get("hCentJetPtdPhiq2"));
    hS_histRhoPhi = dynamic_cast<THnSparse*>(dir->Get("hCentJetPtdPhiq2RhoPhi"));
    h_ptBkg = dynamic_cast<TH1F*>(dir->Get("hJetPt_bkgsub"));
  } else if (type == JEFW::DataType::MC) {
    hS_hist = dynamic_cast<THnSparse*>(dir->Get("hRespMcDMcPMatchSubBkg"));
    h_ptBkg = dynamic_cast<TH1F*>(dir->Get("hPartJetPtSubBkg"));

    h2_genMatched = dynamic_cast<TH2F*>(dir->Get("hPartJetPtMatchSubBkg"));
    h2_detUnmatched = dynamic_cast<TH2F*>(dir->Get("hDetectorJetPtSubBkg"));
    hS_PartPt = dynamic_cast<THnSparse*>(dir->Get("hPartJetSparse"));
    hS_DetPt = dynamic_cast<THnSparse*>(dir->Get("hDetJetSparse"));
  } else if ( type == JEFW::DataType::BACKGROUND ) {
    hS_hist = dynamic_cast<THnSparse*>(dir->Get("hCentRhoRandomConewoLeadingJet"));
  }

  if (!hS_hist) {
    std::cerr << "could not find the thnsparse" << std::endl;
    return;
  }
}

JEFW::JEFW(TDirectory* dir, DataType type) :
inFile{nullptr},
dir{dir},
hS_hist{nullptr} 
{ 
  if (type == JEFW::DataType::DATA) {
    hS_hist = dynamic_cast<THnSparse*>(dir->Get("hCentJetPtdPhiq2"));
    hS_histRhoPhi = dynamic_cast<THnSparse*>(dir->Get("hCentJetPtdPhiq2RhoPhi"));
    h_ptBkg = dynamic_cast<TH1F*>(dir->Get("hJetPt_bkgsub"));
  } else if (type == JEFW::DataType::MC) {
    hS_hist = dynamic_cast<THnSparse*>(dir->Get("hRespMcDMcPMatchSubBkg"));
    h_ptBkg = dynamic_cast<TH1F*>(dir->Get("hPartJetPtSubBkg"));

    h2_genMatched = dynamic_cast<TH2F*>(dir->Get("hPartJetPtMatchSubBkg"));
    h2_detUnmatched = dynamic_cast<TH2F*>(dir->Get("hDetectorJetPtSubBkg"));
    hS_PartPt = dynamic_cast<THnSparse*>(dir->Get("hPartJetSparse"));
    hS_DetPt = dynamic_cast<THnSparse*>(dir->Get("hDetJetSparse"));
  } else if ( type == JEFW::DataType::BACKGROUND ) {
    hS_hist = dynamic_cast<THnSparse*>(dir->Get("hCentRhoRandomConewoLeadingJet"));
  }

  if (!hS_hist) {
    std::cerr << "could not find the thnsparse" << std::endl;
    return;
  }
}

JEFW::~JEFW() 
{
}


TH1F* JEFW::eventPlaneResolution(std::string A, std::string B, std::string C, std::pair<int,int> cent)
{

  const char* nameAC{Form("hCosPsi2%sm%s", A.c_str(), C.c_str())};
  const char* nameAB{Form("hCosPsi2%sm%s", A.c_str(), B.c_str())};
  const char* nameBC{Form("hCosPsi2%sm%s", B.c_str(), C.c_str())};


  TH3F* hAC{dynamic_cast<TH3F*>(dir->Get(nameAC))};
  TH3F* hAB{dynamic_cast<TH3F*>(dir->Get(nameAB))};
  TH3F* hBC{dynamic_cast<TH3F*>(dir->Get(nameBC))};

  auto xbin = hAC->GetXaxis()->FindBin(cent.first);
  auto ybin = hAC->GetXaxis()->FindBin(cent.second);
  hAC->GetXaxis()->SetRange(xbin,ybin);
  hAB->GetXaxis()->SetRange(xbin,ybin);
  hBC->GetXaxis()->SetRange(xbin,ybin);

  /* centrality, cos(2 (psiN - psiM)), q2PERC */
  TH2D* hAC_centInt{dynamic_cast<TH2D*>(hAC->Project3D("yz"))};
  TH2D* hAB_centInt{dynamic_cast<TH2D*>(hAB->Project3D("yz"))};
  TH2D* hBC_centInt{dynamic_cast<TH2D*>(hBC->Project3D("yz"))};

  hAC->GetXaxis()->SetRange(0, 0);
  hAB->GetXaxis()->SetRange(0, 0);
  hBC->GetXaxis()->SetRange(0, 0);

  TProfile *pAC{dynamic_cast<TProfile*>(hAC_centInt->ProfileX())};
  TProfile *pAB{dynamic_cast<TProfile*>(hAB_centInt->ProfileX())};
  TProfile *pBC{dynamic_cast<TProfile*>(hBC_centInt->ProfileX())};

  TH1F* hOut{new TH1F(Form("h%s%s%s",A.c_str(),B.c_str(),C.c_str()),";#it{q}_{2}; R_{2}",100,0,100)};

  for (int i{1}; i < pAC->GetNbinsX()+1; i++)
  {
    auto [val, error] = getEPR3Val(pAC,pAB,pBC,i);

    if (val > 3 || val < 0) continue;
    
    hOut->SetBinContent(i,val);
    hOut->SetBinError(i,error);
  }
  return hOut;
}

TH1F* JEFW::eventPlaneResolutionInclusive() {
  const char* nameAC{Form("hCosPsi2%sm%s", "A", "C")};
  const char* nameAB{Form("hCosPsi2%sm%s", "A", "B")};
  const char* nameBC{Form("hCosPsi2%sm%s", "B", "C")};

  TH3F* hAC{dynamic_cast<TH3F*>(dir->Get(nameAC))};
  TH3F* hAB{dynamic_cast<TH3F*>(dir->Get(nameAB))};
  TH3F* hBC{dynamic_cast<TH3F*>(dir->Get(nameBC))};

  /* centrality, cos(2 (psiN - psiM)), q2PERC */
  TH2D* hAC_centInt{dynamic_cast<TH2D*>(hAC->Project3D("yx"))};
  TH2D* hAB_centInt{dynamic_cast<TH2D*>(hAB->Project3D("yx"))};
  TH2D* hBC_centInt{dynamic_cast<TH2D*>(hBC->Project3D("yx"))};


  TProfile *pAC{dynamic_cast<TProfile*>(hAC_centInt->ProfileX())};
  TProfile *pAB{dynamic_cast<TProfile*>(hAB_centInt->ProfileX())};
  TProfile *pBC{dynamic_cast<TProfile*>(hBC_centInt->ProfileX())};

  TH1F* hOut{new TH1F(Form("h%s%s%s","AF","BF","CF"),";Centrality; R_{2}",100,0,100)};

  for (int i{1}; i < pAC->GetNbinsX()+1; i++)
  {
    auto [val, error] = getEPR3Val(pAC,pAB,pBC,i);

    if (val > 3 || val < 0) continue;
    
    hOut->SetBinContent(i,val);
    hOut->SetBinError(i,error);
  }
  return hOut;
}

TH1* JEFW::getEventPlane(const char* name, std::pair<int,int> cent, bool fScale)
{
  TH2F* h{dynamic_cast<TH2F*>(dir->Get(Form("hPsi2%s",name)))};
  TH1* hOut{h->ProjectionY(Form("psi2_%s",name),cent.first,cent.second)};
  if (fScale) hOut->Scale(1.0/hOut->Integral());
  return hOut;
}

TH1* JEFW::getR2S(const char* name, std::pair<int,int> cent)
{
  TH3F* h{dynamic_cast<TH3F*>(dir->Get(Form("hCosPsi2%s",name)))};
  // h->GetXaxis()->SetRange(cent.first,cent.second);
  TH1* hOut{h->ProjectionY(Form("PROJhCosPsi2%s",name),cent.first,cent.second)};
  return hOut;
}

double JEFW::eventCounter(const int iBin)
{
  TH1F* h{dynamic_cast<TH1F*>(dir->Get("hEventCounter"))};
  return h->GetBinContent(iBin);
}

