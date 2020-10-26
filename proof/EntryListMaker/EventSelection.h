#ifndef EVENTSELECTION
#define EVENTSELECTION
#include "TTreeReaderValue.h"
#include "TTreeReader.h"
#include "TSelector.h"
#include "TH2F.h"
#include "TH2D.h"
#include "../IsData.h"

class EventSelection : public TSelector{

 protected:
  TTreeReader fReader;

  std::vector<const char*> BranchNamesList;
  const char *MakeBranchList(const char *bname);

#ifdef Y2016
  TTreeReaderValue<Bool_t> HLT_Ele27_WPTight_Gsf = {fReader, MakeBranchList("HLT_Ele27_WPTight_Gsf")};
  TTreeReaderValue<Bool_t> HLT_Photon175 = {fReader, MakeBranchList("HLT_Photon175")};
  TTreeReaderValue<Bool_t> HLT_Mu50 = {fReader, MakeBranchList("HLT_Mu50")};
  TTreeReaderValue<Bool_t> HLT_TkMu50 = {fReader, MakeBranchList("HLT_TkMu50")};
  TTreeReaderValue<Bool_t> Dummy_TkMu50 = {fReader, "HLT_TkMu50"}; /*Do not dereference*/
  TTreeReaderValue<Bool_t> Flag_globalTightHalo2016Filter = {fReader, MakeBranchList("Flag_globalTightHalo2016Filter")};
  TTreeReaderValue<Bool_t> Flag_hcalLaserEventFilter = {fReader, MakeBranchList("Flag_hcalLaserEventFilter")};
  TTreeReaderValue<Bool_t> Flag_EcalDeadCellTriggerPrimitiveFilter = {fReader, MakeBranchList("Flag_EcalDeadCellTriggerPrimitiveFilter")};
  TTreeReaderValue<Bool_t> Flag_eeBadScFilter = {fReader, MakeBranchList("Flag_eeBadScFilter")};
  TTreeReaderValue<Bool_t> Flag_BadPFMuonFilter = {fReader, MakeBranchList("Flag_BadPFMuonFilter")};
  TTreeReaderValue<Bool_t> Flag_BadPFMuonSummer16Filter = {fReader, MakeBranchList("Flag_BadPFMuonSummer16Filter")};
  TTreeReaderValue<Bool_t> Flag_HBHENoiseFilter = {fReader, MakeBranchList("Flag_HBHENoiseFilter")};
  TTreeReaderValue<Bool_t> Flag_HBHENoiseIsoFilter = {fReader, MakeBranchList("Flag_HBHENoiseIsoFilter")};
#elif defined(Y2017)
  // 2017 HLTs && Flags
  TTreeReaderValue<Bool_t> HLT_Mu50 = {fReader, MakeBranchList("HLT_Mu50")};
  TTreeReaderValue<Bool_t> HLT_OldMu100 = {fReader, MakeBranchList("HLT_OldMu100")};
  TTreeReaderValue<Bool_t> Dummy_OldMu100 = {fReader, "HLT_OldMu100"}; /*Do not dereference*/
  TTreeReaderValue<Bool_t> HLT_TkMu100 = {fReader, MakeBranchList("HLT_TkMu100")};
  TTreeReaderValue<Bool_t> Dummy_TkMu100 = {fReader, "HLT_TkMu100"}; /*Do not dereference*/
  TTreeReaderValue<Bool_t> HLT_Ele35_WPTight_Gsf = {fReader, MakeBranchList("HLT_Ele35_WPTight_Gsf")};
  TTreeReaderValue<Bool_t> HLT_Photon200 = {fReader, MakeBranchList("HLT_Photon200")};
  TTreeReaderValue<Bool_t> Flag_goodVertices = {fReader, MakeBranchList("Flag_goodVertices")};
  TTreeReaderValue<Bool_t> Flag_globalSuperTightHalo2016Filter = {fReader, MakeBranchList("Flag_globalSuperTightHalo2016Filter")};
  TTreeReaderValue<Bool_t> Flag_HBHENoiseFilter = {fReader, MakeBranchList("Flag_HBHENoiseFilter")};
  TTreeReaderValue<Bool_t> Flag_HBHENoiseIsoFilter = {fReader, MakeBranchList("Flag_HBHENoiseIsoFilter")};
  TTreeReaderValue<Bool_t> Flag_EcalDeadCellTriggerPrimitiveFilter = {fReader, MakeBranchList("Flag_EcalDeadCellTriggerPrimitiveFilter")};
  TTreeReaderValue<Bool_t> Flag_BadPFMuonFilter = {fReader, MakeBranchList("Flag_BadPFMuonFilter")};
  TTreeReaderValue<Bool_t> Flag_BadChargedCandidateFilter = {fReader, MakeBranchList("Flag_BadChargedCandidateFilter")};
  TTreeReaderValue<Bool_t> Flag_eeBadScFilter = {fReader, MakeBranchList("Flag_eeBadScFilter")};
#elif defined(Y2018)
  // 2018 HLTs && Flags
  TTreeReaderValue<Bool_t> HLT_Ele32_WPTight_Gsf = {fReader, MakeBranchList("HLT_Ele32_WPTight_Gsf")};
  TTreeReaderValue<Bool_t> HLT_Photon200 = {fReader, MakeBranchList("HLT_Photon200")};
  TTreeReaderValue<Bool_t> HLT_Mu50 = {fReader, MakeBranchList("HLT_Mu50")};
  TTreeReaderValue<Bool_t> HLT_OldMu100 = {fReader, MakeBranchList( "HLT_OldMu100")};
  TTreeReaderValue<Bool_t> HLT_TkMu100 = {fReader, MakeBranchList("HLT_TkMu100")};
  TTreeReaderValue<Bool_t> Flag_goodVertices = {fReader, MakeBranchList("Flag_goodVertices")};
  TTreeReaderValue<Bool_t> Flag_globalSuperTightHalo2016Filter = {fReader, MakeBranchList("Flag_globalSuperTightHalo2016Filter")};
  TTreeReaderValue<Bool_t> Flag_HBHENoiseFilter = {fReader, MakeBranchList("Flag_HBHENoiseFilter")};
  TTreeReaderValue<Bool_t> Flag_HBHENoiseIsoFilter = {fReader, MakeBranchList("Flag_HBHENoiseIsoFilter")};
  TTreeReaderValue<Bool_t> Flag_EcalDeadCellTriggerPrimitiveFilter = {fReader, MakeBranchList("Flag_EcalDeadCellTriggerPrimitiveFilter")};
  TTreeReaderValue<Bool_t> Flag_BadPFMuonFilter = {fReader, MakeBranchList("Flag_BadPFMuonFilter")};
  TTreeReaderValue<Bool_t> Flag_BadChargedCandidateFilter = {fReader, MakeBranchList("Flag_BadChargedCandidateFilter")};
  TTreeReaderValue<Bool_t> Flag_eeBadScFilter = {fReader, MakeBranchList("Flag_eeBadScFilter")};
#endif

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

#ifdef Y2016
  Year = 2016;
#elif defined(Y2017)
  Year = 2017;
#elif defined(Y2018)
  Year = 2018;
#endif

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

#ifdef Y2016
  if (IsMissingBranch){
    std::clog << Form("Superseeding branch content: %s <- %s\n", HLT_TkMu50.GetBranchName(),HLT_Mu50.GetBranchName());
    HLT_TkMu50 = HLT_Mu50;
  } else {
    HLT_TkMu50 = Dummy_TkMu50;
  }
#elif defined(Y2017)
  if (IsMissingBranch) {
    std::clog <<  Form("Superseeding branch content: %s <- %s\n", HLT_TkMu100.GetBranchName(),HLT_Mu50.GetBranchName());
    HLT_TkMu100 = HLT_Mu50;
    std::clog <<  Form("Superseeding branch content: %s <- %s\n", HLT_OldMu100.GetBranchName(),HLT_Mu50.GetBranchName());
    HLT_OldMu100 = HLT_Mu50;
  } else {
    HLT_TkMu100 = Dummy_TkMu100;
    HLT_OldMu100 = Dummy_OldMu100;
  }
#elif defined(Y2018)
  //
#endif

  fReader.SetTree(tree);

}

Bool_t EventSelection::Notify() {
  std::clog << Form("Processing: %s\n",(fReader.GetTree())->GetCurrentFile()->GetEndpointUrl()->GetUrl());

#ifdef Y2016
  std::clog << Form("Branch being processed (HLT_TkMu50): %s\n", HLT_TkMu50.GetBranchName());
#elif defined(Y2017)
  std::clog << Form("Branch being processed (HLT_OldMu100): %s\n", HLT_OldMu100.GetBranchName());
  std::clog << Form("Branch being processed (HLT_TkMu100): %s\n", HLT_TkMu100.GetBranchName());
#endif
  return kTRUE;
}

const char* EventSelection::MakeBranchList(const char *bname){
  BranchNamesList.emplace_back(bname);
  return bname;
};

void EventSelection::ReadEntry(const Long64_t& entry){

  fReader.SetEntry(entry);

  MinLeptons = (*nElectron + *nMuon) > 2;

#ifdef Y2016
  ElectronHLTs = (*HLT_Ele27_WPTight_Gsf||*HLT_Photon175);
  MuonHLTs = (*HLT_Mu50||*HLT_TkMu50);
  Flags = *Flag_HBHENoiseIsoFilter && *Flag_EcalDeadCellTriggerPrimitiveFilter &&
    *Flag_globalTightHalo2016Filter && *Flag_BadPFMuonSummer16Filter
    && *PV_npvsGood > 0;
#elif defined(Y2017)
  ElectronHLTs = *HLT_Ele35_WPTight_Gsf or *HLT_Photon200;
  MuonHLTs = *HLT_Mu50 or *HLT_OldMu100 or *HLT_TkMu100;
  Flags = *Flag_goodVertices and *Flag_globalSuperTightHalo2016Filter
    and *Flag_HBHENoiseFilter and *Flag_HBHENoiseIsoFilter
    and *Flag_EcalDeadCellTriggerPrimitiveFilter
    and *Flag_BadPFMuonFilter and *Flag_BadChargedCandidateFilter
    and *Flag_eeBadScFilter and *PV_npvsGood > 0;
#elif defined(Y2018)
  ElectronHLTs = *HLT_Ele32_WPTight_Gsf or *HLT_Photon200;
  MuonHLTs = *HLT_Mu50 or *HLT_OldMu100 or *HLT_TkMu100;
  Flags = *Flag_goodVertices and *Flag_globalSuperTightHalo2016Filter
    and *Flag_HBHENoiseFilter and *Flag_HBHENoiseIsoFilter
    and *Flag_EcalDeadCellTriggerPrimitiveFilter and *Flag_BadPFMuonFilter
    and *Flag_BadChargedCandidateFilter and *Flag_eeBadScFilter
    and *PV_npvsGood > 0;
#endif

}
#endif
