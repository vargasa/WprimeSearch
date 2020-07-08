#ifndef EVENTSELECTION
#define EVENTSELECTION
#include "TTreeReaderValue.h"
#include "TTreeReader.h"
#include "TSelector.h"
#include "TH2F.h"
#include "TH2D.h"
#include "TGraphAsymmErrors.h"

class EventSelection : public TSelector{

 protected:
  TTreeReader fReader;

  std::vector<const char*> BranchNamesList;
  const char *MakeBranchList(const char *bname);

  TTreeReaderValue<Bool_t> HLT_Ele115_CaloIdVT_GsfTrkIdT = {fReader, MakeBranchList("HLT_Ele115_CaloIdVT_GsfTrkIdT")};
  TTreeReaderValue<Bool_t> HLT_Ele27_WPTight_Gsf = {fReader, MakeBranchList("HLT_Ele27_WPTight_Gsf")};
  TTreeReaderValue<Bool_t> HLT_Photon175 = {fReader, MakeBranchList("HLT_Photon175")};
  TTreeReaderValue<Bool_t> HLT_Mu50 = {fReader, MakeBranchList("HLT_Mu50")};
  TTreeReaderValue<Bool_t> HLT_TkMu50 = {fReader, MakeBranchList("HLT_TkMu50")};
  TTreeReaderValue<Bool_t> Dummy_TkMu50 = {fReader, "HLT_TkMu50"}; /*Do not dereference*/
  TTreeReaderValue<Bool_t> HLT_IsoMu24 = {fReader, MakeBranchList("HLT_IsoMu24")};
  TTreeReaderValue<Bool_t> HLT_IsoTkMu24 = {fReader, MakeBranchList("HLT_IsoTkMu24")};
  TTreeReaderValue<Bool_t> Flag_globalTightHalo2016Filter = {fReader, MakeBranchList("Flag_globalTightHalo2016Filter")};
  TTreeReaderValue<Bool_t> Flag_hcalLaserEventFilter = {fReader, MakeBranchList("Flag_hcalLaserEventFilter")};
  TTreeReaderValue<Bool_t> Flag_EcalDeadCellTriggerPrimitiveFilter = {fReader, MakeBranchList("Flag_EcalDeadCellTriggerPrimitiveFilter")};
  TTreeReaderValue<Bool_t> Flag_eeBadScFilter = {fReader, MakeBranchList("Flag_eeBadScFilter")};
  TTreeReaderValue<Bool_t> Flag_BadPFMuonFilter = {fReader, MakeBranchList("Flag_BadPFMuonFilter")};
  TTreeReaderValue<Bool_t> Flag_BadPFMuonSummer16Filter = {fReader, MakeBranchList("Flag_BadPFMuonSummer16Filter")};
  TTreeReaderValue<Bool_t> Flag_HBHENoiseFilter = {fReader, MakeBranchList("Flag_HBHENoiseFilter")};
  TTreeReaderValue<Bool_t> Flag_HBHENoiseIsoFilter = {fReader, MakeBranchList("Flag_HBHENoiseIsoFilter")};
  TTreeReaderValue<Int_t> PV_npvsGood = {fReader, MakeBranchList("PV_npvsGood")}; // total number of reconstructed primary vertices

  TTreeReaderValue<UInt_t> nMuon = {fReader, MakeBranchList("nMuon")};
  TTreeReaderValue<UInt_t> nElectron = {fReader, MakeBranchList("nElectron")};
  Int_t Year;
  Bool_t BrokenTree{};
  Bool_t MinLeptons{};
  Bool_t ElectronHLTs{};
  Bool_t MuonHLTs{};
  Bool_t Flags{};

 public:

  EventSelection() {};
  ~EventSelection() {};
  Bool_t ElectronTest() const { return ElectronHLTs; };
  void Init(TTree *tree);
  Bool_t MuonTest() const { return MuonHLTs; };
  Bool_t FlagsTest() const { return Flags; };
  Bool_t MinLeptonsTest() const { return MinLeptons; };
  void ReadEntry(const Long64_t& entry);
  Float_t GetElectronSF(TList *SFDb, const Float_t& eta, const Float_t& pt) const;
  Float_t GetMuonSF(TList *SFDB,const Float_t& eta,const Float_t& pt) const;
  Float_t GetSFFromHisto(TH1* h,const Float_t& eta,const Float_t& pt) const;
  Float_t GetSFFromGraph(TGraphAsymmErrors* g,const Float_t& eta) const;
  Bool_t Notify();
};

Float_t EventSelection::GetSFFromGraph(TGraphAsymmErrors* g,const Float_t& eta) const {

  Double_t* a;

  for(Int_t i = 0; i < g->GetN(); i++){
    a = (g->GetY()+i);
    /* Test right bin limit*/
    if( eta < (*(g->GetX()+i) + g->GetErrorX(i))) break;
  }

  return static_cast<Float_t>(*a);

}

void EventSelection::Init(TTree *tree)
{

  BrokenTree = false;

  for(auto brn: BranchNamesList){
    const TBranch *b = tree->FindBranch(brn);
    if(b == nullptr){
      std::cerr << "EventSelection::Init Error: Tree " << tree->GetName()
		<< " Branch: " << brn << " not found " << Year << std::endl;
      std::cerr << "URL: " << tree->GetCurrentFile()->GetEndpointUrl()->GetUrl() <<std::endl;
      BrokenTree = true;
    }
  }

  if (Year == 2016) {
    if (BrokenTree){
      std::clog << Form("Superseeding branch content: %s <- %s\n", HLT_TkMu50.GetBranchName(),HLT_Mu50.GetBranchName());
      HLT_TkMu50 = HLT_Mu50;
    } else {
      HLT_TkMu50 = Dummy_TkMu50;
    }
  }

  fReader.SetTree(tree);

}


Bool_t EventSelection::Notify() {
  std::clog << Form("Processing: %s\n",(fReader.GetTree())->GetCurrentFile()->GetEndpointUrl()->GetUrl());
  if (Year == 2016) std::clog << Form("Branch being processed: %s\n", HLT_TkMu50.GetBranchName());
  return kTRUE;
}

const char* EventSelection::MakeBranchList(const char *bname){
  BranchNamesList.emplace_back(bname);
  return bname;
};

Float_t EventSelection::GetSFFromHisto(TH1* h,const Float_t& x, const Float_t& y) const {
  return h->GetBinContent(h->FindBin(x,y));
}


Float_t EventSelection::GetElectronSF(TList *SFDb, const Float_t& eta, const Float_t& pt) const{

  Float_t sf = -1;

  if(Year == 2016){
    auto SFElectronTrigger1 = static_cast<TGraphAsymmErrors*>(SFDb->FindObject("SFElectronTrigger1"));
    auto SFElectronTrigger2 = static_cast<TGraphAsymmErrors*>(SFDb->FindObject("SFElectronTrigger2"));

    /* 2 bins in pt */
    if( pt < 175.){
      sf = GetSFFromGraph(SFElectronTrigger1,eta); 
    } else {
      sf = GetSFFromGraph(SFElectronTrigger2,eta);
    }
  }

  return sf;

}


Float_t EventSelection::GetMuonSF(TList *SFDb, const Float_t& eta, const Float_t& pt) const{

  Float_t sf = -1;

  auto SFMuonTriggerBF = static_cast<TH2F*>(SFDb->FindObject("SFMuonTriggerBF"));
  auto SFMuonTriggerGH = static_cast<TH2F*>(SFDb->FindObject("SFMuonTriggerGH"));

  auto SFMuonIDBF = static_cast<TH2D*>(SFDb->FindObject("SFMuonIDBF"));
  auto SFMuonIDGH = static_cast<TH2D*>(SFDb->FindObject("SFMuonIDGH"));

  if (Year == 2016) {
    const Float_t LumiBF = 3.11; //fb-1
    const Float_t LumiGH = 5.54;
    const Float_t SFTriggerBF = GetSFFromHisto(SFMuonTriggerBF,abs(eta),pt);
    const Float_t SFTriggerGH = GetSFFromHisto(SFMuonTriggerGH,abs(eta),pt);
    const Float_t SFIDBF = GetSFFromHisto(SFMuonIDBF,eta,pt);
    const Float_t SFIDGH = GetSFFromHisto(SFMuonIDGH,eta,pt);
    sf = (LumiBF*SFTriggerBF+LumiGH*SFTriggerGH)/(LumiBF+LumiGH);
    sf *=(LumiBF*SFIDBF+LumiGH*SFIDGH)/(LumiBF+LumiGH);
  }

  return sf;

}

void EventSelection::ReadEntry(const Long64_t& entry){

  fReader.SetEntry(entry);

  MinLeptons = (*nElectron + *nMuon) > 2;

  if(Year == 2016){
    ElectronHLTs = (*HLT_Ele27_WPTight_Gsf||*HLT_Photon175);
    MuonHLTs = (*HLT_Mu50||*HLT_TkMu50);
    Flags = *Flag_HBHENoiseIsoFilter && *Flag_EcalDeadCellTriggerPrimitiveFilter &&
      *Flag_globalTightHalo2016Filter && *Flag_BadPFMuonSummer16Filter
      && *PV_npvsGood > 0;
  }

}
#endif
