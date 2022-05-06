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

#if defined(Y2016)
  TTreeReaderValue<Bool_t> HLT_Ele27_WPTight_Gsf = {fReader, MakeBranchList("HLT_Ele27_WPTight_Gsf")};
  TTreeReaderValue<Bool_t> HLT_Photon175 = {fReader, MakeBranchList("HLT_Photon175")};
  TTreeReaderValue<Bool_t> HLT_Mu50 = {fReader, MakeBranchList("HLT_Mu50")};
  TTreeReaderValue<Bool_t> HLT_TkMu50 = {fReader, MakeBranchList("HLT_TkMu50")};
  TTreeReaderValue<Bool_t> Dummy_TkMu50 = {fReader, "HLT_TkMu50"}; /*Do not dereference*/
  TTreeReaderValue<Bool_t> Flag_globalSuperTightHalo2016Filter = {fReader, MakeBranchList("Flag_globalSuperTightHalo2016Filter")};
  TTreeReaderValue<Bool_t> Flag_hcalLaserEventFilter = {fReader, MakeBranchList("Flag_hcalLaserEventFilter")};
  TTreeReaderValue<Bool_t> Flag_EcalDeadCellTriggerPrimitiveFilter = {fReader, MakeBranchList("Flag_EcalDeadCellTriggerPrimitiveFilter")};
  TTreeReaderValue<Bool_t> Flag_BadPFMuonFilter = {fReader, MakeBranchList("Flag_BadPFMuonFilter")};
  TTreeReaderValue<Bool_t> Flag_BadPFMuonSummer16Filter = {fReader, MakeBranchList("Flag_BadPFMuonSummer16Filter")};
  TTreeReaderValue<Bool_t> Flag_HBHENoiseFilter = {fReader, MakeBranchList("Flag_HBHENoiseFilter")};
  TTreeReaderValue<Bool_t> Flag_HBHENoiseIsoFilter = {fReader, MakeBranchList("Flag_HBHENoiseIsoFilter")};
  TTreeReaderValue<Bool_t> Flag_goodVertices = {fReader, MakeBranchList("Flag_goodVertices")};
  TTreeReaderValue<Bool_t> Flag_eeBadScFilter = {fReader, MakeBranchList("Flag_eeBadScFilter")};
  TTreeReaderValue<Bool_t> Flag_hfNoisyHitsFilter = {fReader, MakeBranchList("Flag_hfNoisyHitsFilter")};
  TTreeReaderValue<Bool_t> Flag_BadPFMuonDzFilter = {fReader, MakeBranchList("Flag_BadPFMuonDzFilter")};
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
  TTreeReaderValue<Bool_t> Flag_hfNoisyHitsFilter = {fReader, MakeBranchList("Flag_hfNoisyHitsFilter")};
  TTreeReaderValue<Bool_t> Flag_BadPFMuonDzFilter = {fReader, MakeBranchList("Flag_BadPFMuonDzFilter")};
#if defined(ULSAMPLE)
  TTreeReaderValue<Bool_t> Flag_ecalBadCalibFilter = {fReader, MakeBranchList("Flag_ecalBadCalibFilter")};
#else
  TTreeReaderValue<Bool_t> Flag_ecalBadCalibFilter = {fReader, MakeBranchList("Flag_ecalBadCalibFilterV2")};
#endif
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
  TTreeReaderValue<Bool_t> Flag_hfNoisyHitsFilter = {fReader, MakeBranchList("Flag_hfNoisyHitsFilter")};
  TTreeReaderValue<Bool_t> Flag_BadPFMuonDzFilter = {fReader, MakeBranchList("Flag_BadPFMuonDzFilter")};
#if defined(ULSAMPLE)
  TTreeReaderValue<Bool_t> Flag_ecalBadCalibFilter = {fReader, MakeBranchList("Flag_ecalBadCalibFilter")};
#else
  TTreeReaderValue<Bool_t> Flag_ecalBadCalibFilter = {fReader, MakeBranchList("Flag_ecalBadCalibFilterV2")};
#endif
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
  Bool_t FailMuonHLTs{};
  Bool_t FailElectronHLTs{};

#if !defined(CMSDATA)
  // Just because we won't override Notify()
  TList *SFDb;
  TH2F* SFElectronLooseID;
  TH2F* SFElectronMediumID;
  TH2F* SFElectronTightID;
  TH2F* SFMuonHighPtID;
  TH2F* SFMuonTrkHighPtID;
  TH2F* SFMuonTrigger;
  TH1F* SFPileup;
  TH1F* SFPileupUp;
  TH1F* SFPileupDown;
#if defined(Y2016) && !defined(ULSAMPLE)
  TH2D* SFMuonHighPtIDBF;
  TH2D* SFMuonHighPtIDGH;
  TH2F* SFMuonTriggerBF;
  TH2F* SFMuonTriggerGH;
#endif
#if defined(Y2016) && defined(ULSAMPLE)
  Bool_t IsPreVFP{};
  TH1F* SFPileuppreVFP;
  TH1F* SFPileuppostVFP;
  TH1F* SFPileupUppreVFP;
  TH1F* SFPileupUppostVFP;
  TH1F* SFPileupDownpreVFP;
  TH1F* SFPileupDownpostVFP;
  TH2F* SFMuonHighPtIDpreVFP;
  TH2F* SFMuonHighPtIDpostVFP;
  TH2F* SFMuonTrkHighPtIDpreVFP;
  TH2F* SFMuonTrkHighPtIDpostVFP;
  TH2F* SFElectronLooseIDpreVFP;
  TH2F* SFElectronLooseIDpostVFP;
  TH2F* SFElectronMediumIDpreVFP;
  TH2F* SFElectronMediumIDpostVFP;
  TH2F* SFElectronTightIDpreVFP;
  TH2F* SFElectronTightIDpostVFP;
  TH2F* SFElectronHLTLoosepreVFP;
  TH2F* SFElectronHLTLoosepostVFP;
  TH2F* SFElectronHLTMediumpreVFP;
  TH2F* SFElectronHLTMediumpostVFP;
  TH2F* SFElectronHLTTightpreVFP;
  TH2F* SFElectronHLTTightpostVFP;
#endif


  
#endif

 public:

  EventSelection() {};
  ~EventSelection() {};
  Bool_t ElectronTest() const { return ElectronHLTs; };
  void Init(TTree *tree);
  Bool_t MuonTest() const { return MuonHLTs; };
  Bool_t FlagsTest() const { return Flags; };
  Bool_t MinLeptonsTest() const { return MinLeptons; };
  Bool_t ReadEntry(const Long64_t& entry);
  Bool_t Notify();
};

void EventSelection::Init(TTree *tree)
{

#if defined(Y2016)
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

#if defined(Y2016)
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

  if(IsMissingBranch){

    /* This is the last resort HLT: HLT_Mu50 */
    TBranch *b = tree->FindBranch(HLT_Mu50.GetBranchName());

    if (b == nullptr) {
      FailMuonHLTs = true;
    }

    std::vector<std::string> ElectronHLTs = {
#if defined(Y2016)
      "HLT_Ele27_WPTight_Gsf","HLT_Photon175"
#elif defined(Y2017)
      "HLT_Ele32_WPTight_Gsf","HLT_Photon200"
#elif defined(Y2018)
      "HLT_Ele32_WPTight_Gsf","HLT_Photon200"
#endif
    };

    for (auto path: ElectronHLTs) {
      b = tree->FindBranch(path.c_str());
      if (b == nullptr) {
        FailElectronHLTs = true;
      }
    }

  }

  fReader.SetTree(tree);

}

Bool_t EventSelection::Notify() {

  std::string fullPath((fReader.GetTree())->GetCurrentFile()->GetEndpointUrl()->GetUrl());

  std::clog << Form("Processing: %s\n",fullPath.c_str());

#if defined(Y2016)
  std::clog << Form("Branch being processed (HLT_TkMu50): %s\n", HLT_TkMu50.GetBranchName());
#if !defined(CMSDATA) && defined(ULSAMPLE)
  if (fullPath.find("preVFP") != std::string::npos) {
    SFPileup = SFPileuppreVFP;
    SFPileupUp = SFPileupUppreVFP;
    SFPileupDown = SFPileupDownpreVFP;
    SFElectronLooseID = SFElectronLooseIDpreVFP;
    SFElectronTightID = SFElectronTightIDpreVFP;
    SFMuonHighPtID = SFMuonHighPtIDpreVFP;
    SFMuonTrkHighPtID = SFMuonTrkHighPtIDpreVFP;
    IsPreVFP = true;
    std::clog << "Notify: Setting preVFP Muon and Electron SFs\t";
  } else {
    SFPileup = SFPileuppostVFP;
    SFPileupUp = SFPileupUppostVFP;
    SFPileupDown = SFPileupDownpostVFP;
    SFElectronLooseID = SFElectronLooseIDpostVFP;
    SFElectronTightID = SFElectronTightIDpostVFP;
    SFMuonHighPtID = SFMuonHighPtIDpostVFP;
    SFMuonTrkHighPtID = SFMuonTrkHighPtIDpostVFP;
    IsPreVFP = false;
    std::clog << "Notify: Setting postVFP Muon and Electron SFs\t";
  }
  std::clog << "[Ok]\n";
#endif
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

Bool_t EventSelection::ReadEntry(const Long64_t& entry){

  fReader.SetEntry(entry);

  if (FailMuonHLTs and FailElectronHLTs) return kFALSE;
  if (FailMuonHLTs and (*nElectron < 3)) return kFALSE;
  if (FailElectronHLTs and (*nMuon < 3)) return kFALSE;

  MinLeptons = (*nElectron + *nMuon) > 2;

#if defined(Y2016)
  ElectronHLTs = (*HLT_Ele27_WPTight_Gsf||*HLT_Photon175);
  MuonHLTs = (*HLT_Mu50||*HLT_TkMu50);
  Flags = *Flag_goodVertices and *Flag_globalSuperTightHalo2016Filter
    and *Flag_HBHENoiseFilter and *Flag_HBHENoiseIsoFilter
    and *Flag_EcalDeadCellTriggerPrimitiveFilter
    and *Flag_BadPFMuonFilter and *Flag_eeBadScFilter
    and *Flag_hfNoisyHitsFilter and *Flag_BadPFMuonDzFilter
    and *PV_npvsGood > 0;
#elif defined(Y2017)
  ElectronHLTs = *HLT_Ele35_WPTight_Gsf or *HLT_Photon200;
  MuonHLTs = *HLT_Mu50 or *HLT_OldMu100 or *HLT_TkMu100;
  Flags = *Flag_goodVertices and *Flag_globalSuperTightHalo2016Filter
    and *Flag_HBHENoiseFilter and *Flag_HBHENoiseIsoFilter
    and *Flag_EcalDeadCellTriggerPrimitiveFilter
    and *Flag_BadPFMuonFilter and *Flag_ecalBadCalibFilter
    and *Flag_eeBadScFilter and *Flag_hfNoisyHitsFilter
    and *Flag_BadPFMuonDzFilter
    and *PV_npvsGood > 0;
#elif defined(Y2018)
  ElectronHLTs = *HLT_Ele32_WPTight_Gsf or *HLT_Photon200;
  MuonHLTs = *HLT_Mu50 or *HLT_OldMu100 or *HLT_TkMu100;
  Flags = *Flag_goodVertices and *Flag_globalSuperTightHalo2016Filter
    and *Flag_HBHENoiseFilter and *Flag_HBHENoiseIsoFilter
    and *Flag_EcalDeadCellTriggerPrimitiveFilter and *Flag_BadPFMuonFilter
    and *Flag_ecalBadCalibFilter and *Flag_BadPFMuonDzFilter
    and *Flag_eeBadScFilter and *Flag_hfNoisyHitsFilter
    and *PV_npvsGood > 0;
#endif

  return kTRUE;

}
#endif
