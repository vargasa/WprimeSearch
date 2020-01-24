#include "TSelector.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"
#include "TCanvas.h"
#include "TH1I.h"
#include "TStyle.h"
#include "THStack.h"

class PreSelector : public TSelector {

 private :

  TTreeReader fReader;

  TTreeReaderValue<Bool_t> HLT_DoubleEle33_CaloIdL_MW = {fReader, "HLT_DoubleEle33_CaloIdL_MW"};
  TTreeReaderValue<Bool_t> HLT_IsoMu20 = {fReader, "HLT_IsoMu20"};

  // Beam Halo Filter
  TTreeReaderValue<Bool_t> Flag_globalTightHalo2016Filter = {fReader, "Flag_globalTightHalo2016Filter"};

  // HCAL laser filter
  TTreeReaderValue<Bool_t> Flag_hcalLaserEventFilter = {fReader, "Flag_hcalLaserEventFilter"};

  // ECAL dead cell trigger primitive filter
  TTreeReaderValue<Bool_t> Flag_EcalDeadCellTriggerPrimitiveFilter = {fReader, "Flag_EcalDeadCellTriggerPrimitiveFilter"};

  TTreeReaderValue<Bool_t> Flag_eeBadScFilter = {fReader, "Flag_eeBadScFilter"};
  TTreeReaderValue<Bool_t> Flag_BadPFMuonFilter = {fReader, "Flag_BadPFMuonFilter"};
  TTreeReaderValue<Bool_t> Flag_BadPFMuonSummer16Filter = {fReader, "Flag_BadPFMuonSummer16Filter"};

  // Tracking Failure system ??

  // Bad EE Supercrystal filter ??

  // No scraping
  TTreeReaderArray<Bool_t> IsoTrack_isHighPurityTrack = {fReader, "IsoTrack_isHighPurityTrack"};


  // At least one good primary vertex
  TTreeReaderValue<Int_t> PV_npvsGood = {fReader, "PV_npvsGood"}; // total number of reconstructed primary vertices
  TTreeReaderValue<Float_t> PV_ndof = {fReader, "PV_ndof"};
  TTreeReaderValue<Float_t> PV_x = {fReader, "PV_x"};
  TTreeReaderValue<Float_t> PV_y = {fReader, "PV_y"};
  TTreeReaderValue<Float_t> PV_z = {fReader, "PV_z"};
  TTreeReaderValue<Float_t> PV_chi2 = {fReader, "PV_chi2"};
  TTreeReaderValue<Float_t> PV_score = {fReader, "PV_score"};
  TTreeReaderValue<Int_t> PV_npvs = {fReader, "PV_npvs"};

  // HBHE event-level noise filtering
  TTreeReaderValue<Bool_t> Flag_HBHENoiseFilter = {fReader, "Flag_HBHENoiseFilter"};
  TTreeReaderValue<Bool_t> Flag_HBHENoiseIsoFilter = {fReader, "Flag_HBHENoiseIsoFilter"};


  // Muons
  // https://github.com/cms-sw/cmssw/blob/master/PhysicsTools/NanoAOD/python/muons_cff.py
  TTreeReaderValue<UInt_t> nMuon = {fReader, "nMuon"};
  TTreeReaderArray<Float_t> Muon_pt = {fReader, "Muon_pt"};
  TTreeReaderArray<Float_t> Muon_eta = {fReader, "Muon_eta"};
  TTreeReaderArray<Float_t> Muon_mass = {fReader, "Muon_mass"};
  TTreeReaderArray<Float_t> Muon_phi = {fReader, "Muon_phi"};
  TTreeReaderArray<Int_t> Muon_charge = {fReader, "Muon_charge"};
  TTreeReaderArray<Bool_t> Muon_looseId = {fReader, "Muon_looseId"};
  TTreeReaderArray<Bool_t> Muon_mediumId = {fReader, "Muon_mediumId"};
  TTreeReaderArray<Bool_t> Muon_tightId = {fReader, "Muon_tightId"};
  TTreeReaderArray<Float_t> Muon_pfRelIso03_all = {fReader, "Muon_pfRelIso03_all"};
  TTreeReaderArray<Float_t> Muon_pfRelIso03_chg = {fReader, "Muon_pfRelIso03_chg"};
  TTreeReaderArray<Float_t> Muon_pfRelIso04_all = {fReader, "Muon_pfRelIso04_all"};
  TTreeReaderArray<Bool_t> Muon_isGlobal = {fReader, "Muon_isGlobal"};
  TTreeReaderArray<Float_t> Muon_dxy = {fReader, "Muon_dxy"};
  TTreeReaderArray<Float_t> Muon_dz = {fReader, "Muon_dz"};

  // Electrons
  // https://github.com/cms-sw/cmssw/blob/master/PhysicsTools/NanoAOD/python/electrons_cff.py
  TTreeReaderValue<UInt_t> nElectron = {fReader, "nElectron"};
  TTreeReaderArray<Float_t> Electron_pt = {fReader, "Electron_pt"};
  TTreeReaderArray<Float_t> Electron_eta = {fReader, "Electron_eta"};
  TTreeReaderArray<Float_t> Electron_mass = {fReader, "Electron_mass"};
  TTreeReaderArray<Float_t> Electron_phi = {fReader, "Electron_phi"};
  TTreeReaderArray<Int_t> Electron_charge = {fReader, "Electron_charge"};
  TTreeReaderArray<Int_t> Electron_cutBased = {fReader, "Electron_cutBased"}; // cut-based ID Fall17 V2 (0:fail, 1:veto, 2:loose, 3:medium, 4:tight)
  TTreeReaderArray<Float_t> Electron_pfRelIso03_all = {fReader, "Electron_pfRelIso03_all"};
  TTreeReaderArray<Float_t> Electron_pfRelIso03_chg = {fReader, "Electron_pfRelIso03_chg"};
  TTreeReaderArray<Float_t> Electron_miniPFRelIso_all = {fReader, "Electron_miniPFRelIso_all"};
  TTreeReaderArray<Float_t> Electron_miniPFRelIso_chg = {fReader, "Electron_miniPFRelIso_chg"};

  // Neutrinos
  TTreeReaderValue<Float_t> MET_phi = {fReader, "MET_phi"};
  TTreeReaderValue<Float_t> MET_pt = {fReader, "MET_pt"};
  TTreeReaderValue<Float_t> MET_significance = {fReader, "MET_significance"};

  TCanvas *ch;

  // THs
  TH1F *HElectron_ptA;
  TH1F *HMuon_ptA;

  TH1F *HElectron_ptB;
  TH1F *HMuon_ptB;

  TH1F *HElectron_ptC;
  TH1F *HMuon_ptC;

  TH1F *HElectron_ptD;
  TH1F *HMuon_ptD;

  TH1F *HMetA;
  TH1F *HMetB;
  TH1F *HMetC;
  TH1F *HMetD;

  TH1I *HnElA;
  TH1I *HnElB;
  TH1I *HnElC;
  TH1I *HnElD;

  TH1I *HnMuA;
  TH1I *HnMuB;
  TH1I *HnMuC;
  TH1I *HnMuD;

  TH1F *HMassA;
  TH1F *HMassB;
  TH1F *HMassC;
  TH1F *HMassD;

 public :

  PreSelector(TTree * = 0);
  virtual ~PreSelector() { }
  virtual void    Begin(TTree *tree);
  virtual void    Init(TTree *tree);
  virtual void    SlaveBegin(TTree *tree);
  virtual Bool_t  Process(Long64_t entry);
  virtual void    Terminate();
  virtual Int_t   Version() const { return 2; }

  ClassDef(PreSelector,0);
};

