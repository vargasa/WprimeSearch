#include "TSelector.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TParameter.h"
#include <memory>
#include "TEntryList.h"

class EventIDMaker : public TSelector {

 private :

  TTreeReader fReader;

  TTreeReaderValue<UInt_t> run = {fReader, "run"};
  TTreeReaderValue<UInt_t> luminosityBlock = {fReader, "luminosityBlock"};
  TTreeReaderValue<ULong64_t> event = {fReader, "event"};

  TTreeReaderValue<Bool_t> HLT_DoubleEle33_CaloIdL_MW = {fReader, "HLT_DoubleEle33_CaloIdL_MW"};
  TTreeReaderValue<Bool_t> HLT_Ele115_CaloIdVT_GsfTrkIdT = {fReader, "HLT_Ele115_CaloIdVT_GsfTrkIdT"};
  TTreeReaderValue<Bool_t> HLT_IsoMu20 = {fReader, "HLT_IsoMu20"};
  TTreeReaderValue<Bool_t> HLT_Mu55 = {fReader, "HLT_Mu55"};

  // Beam Halo Filter
  TTreeReaderValue<Bool_t> Flag_globalTightHalo2016Filter = {fReader, "Flag_globalTightHalo2016Filter"};

  // HCAL laser filter
  TTreeReaderValue<Bool_t> Flag_hcalLaserEventFilter = {fReader, "Flag_hcalLaserEventFilter"};

  // ECAL dead cell trigger primitive filter
  TTreeReaderValue<Bool_t> Flag_EcalDeadCellTriggerPrimitiveFilter = {fReader, "Flag_EcalDeadCellTriggerPrimitiveFilter"};

  TTreeReaderValue<Bool_t> Flag_eeBadScFilter = {fReader, "Flag_eeBadScFilter"};
  TTreeReaderValue<Bool_t> Flag_BadPFMuonFilter = {fReader, "Flag_BadPFMuonFilter"};
  TTreeReaderValue<Bool_t> Flag_BadPFMuonSummer16Filter = {fReader, "Flag_BadPFMuonSummer16Filter"};

  // HBHE event-level noise filtering
  TTreeReaderValue<Bool_t> Flag_HBHENoiseFilter = {fReader, "Flag_HBHENoiseFilter"};
  TTreeReaderValue<Bool_t> Flag_HBHENoiseIsoFilter = {fReader, "Flag_HBHENoiseIsoFilter"};

  TTreeReaderValue<Float_t> MET_pt = {fReader, "MET_pt"};
  TTreeReaderValue<Int_t> PV_npvsGood = {fReader, "PV_npvsGood"}; // total number of reconstructed primary vertices

  std::unordered_map<Int_t, std::vector<std::pair<UInt_t,UInt_t>>> GoldenJson;
  Long64_t GetEventIndex(UInt_t run,ULong64_t event);
  TEntryList *EntryList;
  TTree *eTree;
  Long64_t EventID;
  TTree *EventIndexTree;
  std::unordered_set<Long64_t> EventIndex;
  void AddTreeToEventIndex(std::string treeName);

 public :

  TString SampleName;

  EventIDMaker(TTree * = 0);
  virtual ~EventIDMaker() { }
  virtual void    Begin(TTree *tree);
  virtual void    Init(TTree *tree);
  virtual void    SlaveBegin(TTree *tree);
  virtual Bool_t  Process(Long64_t entry);
  virtual void    Terminate();
  virtual Int_t   Version() const { return 2; }
  void BuildGoldenJson();

  Bool_t IsGold(UInt_t Run, UInt_t LuminosityBlock);

  ClassDef(EventIDMaker,0);
};

