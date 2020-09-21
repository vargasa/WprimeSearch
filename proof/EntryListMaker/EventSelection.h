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
  Bool_t IsMissingBranch{};
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
  Bool_t Notify();
};

void EventSelection::Init(TTree *tree)
{

  IsMissingBranch = false;

  for(auto brn: BranchNamesList){
    const TBranch *b = tree->FindBranch(brn);
    if(b == nullptr){
      std::cerr << "EventSelection::Init Error: Tree " << tree->GetName()
                << " Branch: " << brn << " not found " << Year << std::endl;
      std::cerr << "URL: " << tree->GetCurrentFile()->GetEndpointUrl()->GetUrl() <<std::endl;
      IsMissingBranch = true;
    }
  }

  if (Year == 2016) {
    if (IsMissingBranch){
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

void EventSelection::ReadEntry(const Long64_t& entry){

  fReader.SetEntry(entry);

  MinLeptons = (*nElectron + *nMuon) > 2;

  if(Year == 2016){
    ElectronHLTs = (*HLT_Ele27_WPTight_Gsf||*HLT_Photon175);
    MuonHLTs = (*HLT_Mu50||*HLT_TkMu50);
    Flags = *Flag_HBHENoiseIsoFilter && *Flag_EcalDeadCellTriggerPrimitiveFilter &&
      *Flag_globalTightHalo2016Filter && *Flag_BadPFMuonSummer16Filter
      && *PV_npvsGood > 0;
  } else if (Year == 2017) {
    // https://twiki.cern.ch/twiki/bin/view/CMS/MuonHLT2017
    // https://twiki.cern.ch/twiki/bin/view/CMS/MuonReferenceEffs2017
    // https://twiki.cern.ch/twiki/bin/view/CMS/EgHLTScaleFactorMeasurements
    // ElectronHLTs = (*HLT_Ele35_WPTight_Gsf || *HLT_Photon200);
    // MuonHLTs = *HLT_Mu50;
    // Flags = true;
  } else if (Year == 2018) {
    // https://twiki.cern.ch/twiki/bin/view/CMS/MuonReferenceEffs2018
    // ElectronHLTs = (*HLT_Ele32_WPTight_Gsf || *HLT_Photon200);
    // MuonHLTs = (*HLT_Mu50 || *HLT_OldMu100 || *HLT_TkMu1100);
    // Flags = true;
  }

}
#endif
