#include "IsData.h"

#include "TSelector.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"
#include "TCanvas.h"
#include "TParameter.h"
#include "TH1I.h"
#include "TH2I.h"
#include "TStyle.h"
#include "THStack.h"
#include "Electrons.h"
#include "Muons.h"
#include "Leptons.h"
#include <memory>
#include "Math/Vector4D.h"
#include "Math/Vector4Dfwd.h"
#include "TEntryList.h"

using PtEtaPhiMVector = ROOT::Math::PtEtaPhiMVector;

class PreSelector : public TSelector {

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
  TTreeReaderArray<Int_t> Muon_pdgId = {fReader, "Muon_pdgId"};
  TTreeReaderArray<Bool_t> Muon_looseId = {fReader, "Muon_looseId"};
  TTreeReaderArray<Bool_t> Muon_mediumId = {fReader, "Muon_mediumId"};
  TTreeReaderArray<Bool_t> Muon_tightId = {fReader, "Muon_tightId"};
  TTreeReaderArray<Float_t> Muon_pfRelIso03_all = {fReader, "Muon_pfRelIso03_all"};
  TTreeReaderArray<Float_t> Muon_pfRelIso03_chg = {fReader, "Muon_pfRelIso03_chg"};
  TTreeReaderArray<Float_t> Muon_pfRelIso04_all = {fReader, "Muon_pfRelIso04_all"};
  TTreeReaderArray<Bool_t> Muon_isGlobal = {fReader, "Muon_isGlobal"};
  TTreeReaderArray<Float_t> Muon_dxy = {fReader, "Muon_dxy"};
  TTreeReaderArray<Float_t> Muon_dz = {fReader, "Muon_dz"};
  TTreeReaderArray<UChar_t> Muon_highPtId = {fReader, "Muon_highPtId"};


  // Electrons
  // https://github.com/cms-sw/cmssw/blob/master/PhysicsTools/NanoAOD/python/electrons_cff.py
  TTreeReaderValue<UInt_t> nElectron = {fReader, "nElectron"};
  TTreeReaderArray<Float_t> Electron_pt = {fReader, "Electron_pt"};
  TTreeReaderArray<Float_t> Electron_eta = {fReader, "Electron_eta"};
  TTreeReaderArray<Float_t> Electron_mass = {fReader, "Electron_mass"};
  TTreeReaderArray<Float_t> Electron_phi = {fReader, "Electron_phi"};
  TTreeReaderArray<Float_t> Electron_dxy = {fReader, "Electron_dxy"};
  TTreeReaderArray<Float_t> Electron_dz = {fReader, "Electron_dz"};
  TTreeReaderArray<Int_t> Electron_charge = {fReader, "Electron_charge"};
  TTreeReaderArray<Int_t> Electron_pdgId = {fReader, "Electron_pdgId"};
  TTreeReaderArray<Int_t> Electron_cutBased = {fReader, "Electron_cutBased"}; // cut-based ID Fall17 V2 (0:fail, 1:veto, 2:loose, 3:medium, 4:tight)
  TTreeReaderArray<Float_t> Electron_pfRelIso03_all = {fReader, "Electron_pfRelIso03_all"};
  TTreeReaderArray<Float_t> Electron_pfRelIso03_chg = {fReader, "Electron_pfRelIso03_chg"};
  TTreeReaderArray<Float_t> Electron_miniPFRelIso_all = {fReader, "Electron_miniPFRelIso_all"};
  TTreeReaderArray<Float_t> Electron_miniPFRelIso_chg = {fReader, "Electron_miniPFRelIso_chg"};

  // GenPart
#ifndef CMSDATA
  TTreeReaderArray<Int_t> Electron_genPartIdx = {fReader, "Electron_genPartIdx"};
  TTreeReaderArray<Int_t> Muon_genPartIdx = {fReader, "Muon_genPartIdx"};
  TTreeReaderArray<Float_t> GenPart_eta = {fReader, "GenPart_eta"};
  TTreeReaderArray<Float_t> GenPart_mass = {fReader, "GenPart_mass"};
  TTreeReaderArray<Float_t> GenPart_phi = {fReader, "GenPart_phi"};
  TTreeReaderArray<Float_t> GenPart_pt = {fReader, "GenPart_pt"};
  TTreeReaderArray<Int_t> GenPart_genPartIdxMother = {fReader, "GenPart_genPartIdxMother"};
  TTreeReaderArray<Int_t> GenPart_pdgId = {fReader, "GenPart_pdgId"};
  TTreeReaderArray<Int_t> GenPart_status = {fReader, "GenPart_status"};
  TTreeReaderArray<Int_t> GenPart_statusFlags = {fReader, "GenPart_statusFlags"};
#endif
  // Neutrinos
  TTreeReaderValue<Float_t> MET_phi = {fReader, "MET_phi"};
  TTreeReaderValue<Float_t> MET_pt = {fReader, "MET_pt"};
  TTreeReaderValue<Float_t> MET_significance = {fReader, "MET_significance"};

  // THs
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

  TH1F *HMassTWA;
  TH1F *HMassTWB;
  TH1F *HMassTWC;
  TH1F *HMassTWD;

  TH1F *HMassZA;
  TH1F *HMassZB;
  TH1F *HMassZC;
  TH1F *HMassZD;

  TH1F *HMassWA;
  TH1F *HMassWB;
  TH1F *HMassWC;
  TH1F *HMassWD;

  TH1F *HMassWZA;
  TH1F *HMassWZB;
  TH1F *HMassWZC;
  TH1F *HMassWZD;

  //Angular
  TH1F *HPairEtaPhi;

  TH1I *HOverlap;

#ifndef CMSDATA
  TH1F *HGenPartZA;
  TH1F *HGenPartZB;
  TH1F *HGenPartZC;
  TH1F *HGenPartZD;

  TH1F *HGenPartWA;
  TH1F *HGenPartWB;
  TH1F *HGenPartWC;
  TH1F *HGenPartWD;

  TH1F *HGenPartFA;
  TH1F *HGenPartFB;
  TH1F *HGenPartFC;
  TH1F *HGenPartFD;
#endif
  TH1F *HCutFlow;
  TH2I *HRunLumi;
  TH1I *HRun;
  TH1I *HLumi;

  TH2I *HNLepA;
  TH2I *HNLepB;
  TH2I *HNLepC;
  TH2I *HNLepD;

  TH1F *HPtl1;
  TH1F *HPtl2;
  TH1F *HPtlead;
  TH1F *HMetPt;

  TH2I *HNEl;
  TH2I *HNMu;

  UInt_t l1, l2, lead; // Lepton pair index and lead remaining
  std::vector<UInt_t> GoodElectron;
  std::vector<UInt_t> GoodMuon;
  PtEtaPhiMVector lep1, lep2, zb, wb, lep3;
  std::vector<ROOT::Math::PxPyPzMVector> nu;

  Float_t BestZMass;
  const Float_t w = 1.;

#ifdef CMSDATA
  TEntryList *EntryList;
#endif

 public :

  TString SampleName;

  PreSelector(TTree * = 0);
  virtual ~PreSelector() { }
  virtual void    Begin(TTree *tree);
  virtual void    Init(TTree *tree);
  virtual void    SlaveBegin(TTree *tree);
  virtual Bool_t  Process(Long64_t entry);
  virtual void    Terminate();
  virtual Int_t   Version() const { return 2; }

  std::vector<UInt_t> GetGoodMuon(Muons);
  std::vector<UInt_t> GetGoodElectron(Electrons);

  Bool_t CheckElectronPair(std::pair<UInt_t,UInt_t>);
  Bool_t CheckMuonPair(std::pair<UInt_t,UInt_t>);
  Float_t GetEtaPhiDistance(Float_t,Float_t,Float_t,Float_t);
  Double_t GetMassFromPair(std::pair<Int_t,Int_t>);
  Double_t MassRecoZ(Double_t,Double_t,Double_t,Double_t,
                     Double_t,Double_t,Double_t,Double_t);
  Double_t MassRecoW(Double_t,Double_t,Double_t,Double_t);
  Double_t MassRecoW(ROOT::Math::PtEtaPhiMVector,Float_t,Float_t);
  void FillCommon(Leptons lz, Leptons lw);
  void FillA();
  void FillB();
  void FillC();
  void FillD();

#ifndef CMSDATA
  std::pair<Int_t,Int_t> GetMother(Int_t,Int_t);
  std::pair<Int_t,Int_t> GetMother(std::pair<Int_t,Int_t> Daughter);
#endif

#ifdef CMSDATA
  Long64_t GetEventIndex(UInt_t run,ULong64_t event);
#endif
  std::vector<Float_t> GetWWZWTMass(Float_t lPt, Float_t lEta, Float_t lPhi, Float_t lMass);
  std::vector<std::pair<UInt_t,UInt_t>> GetLeptonPairs(Leptons, std::vector<UInt_t>);
  std::vector<std::tuple<Double_t,Double_t,std::pair<UInt_t,UInt_t>>> FindZ(Leptons,std::vector<UInt_t>);
  std::vector<ROOT::Math::PxPyPzMVector> GetNu4V(ROOT::Math::PtEtaPhiMVector,Float_t,Float_t,Float_t);
  std::vector<ROOT::Math::PxPyPzMVector> GetNu4VAlt(ROOT::Math::PtEtaPhiMVector,Float_t,Float_t,Float_t); 
  std::vector<ROOT::Math::PxPyPzMVector> GetNu4VFix(ROOT::Math::PtEtaPhiMVector lep,
                                                                 Float_t MetPt, Float_t MetPhi, Float_t Wmt);
  ROOT::Math::PxPyPzMVector Get4V(Float_t MetPt, Float_t MetPhi, Float_t Pz);

  ClassDef(PreSelector,0);
};

