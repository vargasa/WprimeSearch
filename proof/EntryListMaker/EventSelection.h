 #ifndef EVENTSELECTION
#define EVENTSELECTION
#include "TTreeReaderValue.h"
#include "TTreeReader.h"
#include "TSelector.h"


class EventSelection {

 protected:
  TTreeReader fReader;

  TTreeReaderValue<Bool_t> HLT_Ele115_CaloIdVT_GsfTrkIdT = {fReader, "HLT_Ele115_CaloIdVT_GsfTrkIdT"};
  TTreeReaderValue<Bool_t> HLT_Ele27_WPLoose_Gsf = {fReader, "HLT_Ele27_WPLoose_Gsf"};
  TTreeReaderValue<Bool_t> HLT_Photon175 = {fReader, "HLT_Photon175"};
  TTreeReaderValue<Bool_t> HLT_Mu50 = {fReader, "HLT_Mu50"};
  TTreeReaderValue<Bool_t> HLT_TkMu50 = {fReader, "HLT_TkMu50"};
  TTreeReaderValue<Bool_t> Flag_globalTightHalo2016Filter = {fReader, "Flag_globalTightHalo2016Filter"};
  TTreeReaderValue<Bool_t> Flag_hcalLaserEventFilter = {fReader, "Flag_hcalLaserEventFilter"};
  TTreeReaderValue<Bool_t> Flag_EcalDeadCellTriggerPrimitiveFilter = {fReader, "Flag_EcalDeadCellTriggerPrimitiveFilter"};
  TTreeReaderValue<Bool_t> Flag_eeBadScFilter = {fReader, "Flag_eeBadScFilter"};
  TTreeReaderValue<Bool_t> Flag_BadPFMuonFilter = {fReader, "Flag_BadPFMuonFilter"};
  TTreeReaderValue<Bool_t> Flag_BadPFMuonSummer16Filter = {fReader, "Flag_BadPFMuonSummer16Filter"};
  TTreeReaderValue<Bool_t> Flag_HBHENoiseFilter = {fReader, "Flag_HBHENoiseFilter"};
  TTreeReaderValue<Bool_t> Flag_HBHENoiseIsoFilter = {fReader, "Flag_HBHENoiseIsoFilter"};
  TTreeReaderValue<Int_t> PV_npvsGood = {fReader, "PV_npvsGood"}; // total number of reconstructed primary vertices

  UInt_t Year;
  Bool_t ElectronHLTs;
  Bool_t MuonHLTs;
  Bool_t Flags;

 public:

  EventSelection() {};
  ~EventSelection() {};
  Bool_t ElectronTest() { return ElectronHLTs; };
  Bool_t MuonTest() { return MuonHLTs; };
  Bool_t FlagsTest() { return Flags; };
  void ReadEntry(Long64_t entry, UInt_t year);

};

void EventSelection::ReadEntry(Long64_t entry, UInt_t year){

  fReader.SetEntry(entry);

  if(year == 2016){
    ElectronHLTs = (*HLT_Ele27_WPLoose_Gsf||*HLT_Ele115_CaloIdVT_GsfTrkIdT||*HLT_Photon175);
    MuonHLTs = (*HLT_Mu50||*HLT_TkMu50);
    Flags = *Flag_HBHENoiseIsoFilter && *Flag_EcalDeadCellTriggerPrimitiveFilter &&
      *Flag_globalTightHalo2016Filter && *Flag_BadPFMuonSummer16Filter
      && *PV_npvsGood > 0;
  }

}
#endif
