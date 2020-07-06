 #ifndef EVENTSELECTION
#define EVENTSELECTION
#include "TTreeReaderValue.h"
#include "TTreeReader.h"
#include "TSelector.h"
#include "TH2F.h"
#include "TH2D.h"

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
  Bool_t ElectronTest() { return ElectronHLTs; };
  void Init(TTree *tree);
  Bool_t MuonTest() { return MuonHLTs; };
  Bool_t FlagsTest() { return Flags; };
  Bool_t MinLeptonsTest() { return MinLeptons; };
  void ReadEntry(Long64_t entry);
  Float_t GetMuonSF(TList *SFDB, Float_t eta, Float_t pt);
  Float_t GetSF(TH1* h,Float_t eta, Float_t pt);
  Bool_t Notify();
};

void EventSelection::Init(TTree *tree)
{

  for(auto brn: BranchNamesList){
    const TBranch *b = tree->FindBranch(brn);
    if(b == nullptr){
      std::cerr << "EventIDSelection::Init Error: Tree " << tree->GetName()
		<< " Branch: " << brn << " not found " << Year << std::endl;
      std::cerr << "URL: " << tree->GetCurrentFile()->GetEndpointUrl()->GetUrl() <<std::endl;
      BrokenTree = true;
    }
  }

  if (Year == 2016) {
    if (BrokenTree){
      HLT_TkMu50 = HLT_Mu50;
      std::clog << Form("Superseeding branch content: %s <- %s\n", HLT_TkMu50.GetBranchName(),HLT_Mu50.GetBranchName());
    } else {
      TTreeReaderValue<Bool_t> tmp{fReader,"HLT_TkMu50"};
      HLT_TkMu50 = tmp;
      std::clog << Form("Restoring %s Branch",HLT_TkMu50.GetBranchName());
    }
  }

  fReader.SetTree(tree);

}


Bool_t EventSelection::Notify(){
  std::clog << Form("Processing: %s\n",(fReader.GetTree())->GetCurrentFile()->GetEndpointUrl()->GetUrl());
  return kTRUE;
}

const char* EventSelection::MakeBranchList(const char *bname){
  BranchNamesList.emplace_back(bname);
  return bname;
};

Float_t EventSelection::GetSF(TH1* h,Float_t x, Float_t y){
  return h->GetBinContent(h->FindBin(x,y));
}


Float_t EventSelection::GetMuonSF(TList *SFDb, Float_t eta, Float_t pt){

  Float_t sf = -1;

  TH2F *SFMuonTriggerBF = dynamic_cast<TH2F*>(SFDb->FindObject("SFMuonTriggerBF"));
  TH2F *SFMuonTriggerGH = dynamic_cast<TH2F*>(SFDb->FindObject("SFMuonTriggerGH"));

  TH2D *SFMuonIDBF = dynamic_cast<TH2D*>(SFDb->FindObject("SFMuonIDBF"));
  TH2D *SFMuonIDGH = dynamic_cast<TH2D*>(SFDb->FindObject("SFMuonIDGH"));

  if (Year == 2016) {
    const Float_t LumiBF = 3.11; //fb-1
    const Float_t LumiGH = 5.54;
    const Float_t SFTriggerBF = GetSF(SFMuonTriggerBF,abs(eta),pt);
    const Float_t SFTriggerGH = GetSF(SFMuonTriggerGH,abs(eta),pt);
    const Float_t SFIDBF = GetSF(SFMuonIDBF,eta,pt);
    const Float_t SFIDGH = GetSF(SFMuonIDGH,eta,pt);
    sf = (LumiBF*SFTriggerBF+LumiGH*SFTriggerGH)/(LumiBF+LumiGH);
    sf *=(LumiBF*SFIDBF+LumiGH*SFIDGH)/(LumiBF+LumiGH);
  }

  return sf;

}

void EventSelection::ReadEntry(Long64_t entry){

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
