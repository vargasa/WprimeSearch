#include "IsData.h"

#include "TSelector.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"
#include "TCanvas.h"
#include "TParameter.h"
#include "TH1I.h"
#include "TH2I.h"
#include "TH3F.h"
#include "TStyle.h"
#include "THStack.h"
#include "../Muons.h"
#include "../Leptons.h"
#include <memory>
#include "Math/Vector4D.h"
#include "Math/Vector4Dfwd.h"
#include "TEntryList.h"
#include "EventSelection.h"
#include "TGraphAsymmErrors.h"
#include "IsData.h"
#include <unordered_map>

using PtEtaPhiMVector = ROOT::Math::PtEtaPhiMVector;

struct ZPairInfo{

  Float_t Mass = -1.f;
  std::pair<UInt_t,UInt_t> Pair;

  Bool_t empty(){
    return Mass < 0.f;
  }

};

class PreSelector : public EventSelection {

 private :

  TTreeReaderValue<UInt_t> run = {fReader, "run"};
  TTreeReaderValue<UInt_t> luminosityBlock = {fReader, "luminosityBlock"};
  TTreeReaderValue<ULong64_t> event = {fReader, "event"};

  // No scraping
  TTreeReaderArray<Bool_t> IsoTrack_isHighPurityTrack = {fReader, "IsoTrack_isHighPurityTrack"};

  TTreeReaderValue<Float_t> PV_ndof = {fReader, "PV_ndof"};
  TTreeReaderValue<Float_t> PV_x = {fReader, "PV_x"};
  TTreeReaderValue<Float_t> PV_y = {fReader, "PV_y"};
  TTreeReaderValue<Float_t> PV_z = {fReader, "PV_z"};
  TTreeReaderValue<Float_t> PV_chi2 = {fReader, "PV_chi2"};
  TTreeReaderValue<Float_t> PV_score = {fReader, "PV_score"};
  TTreeReaderValue<Int_t> PV_npvs = {fReader, "PV_npvs"};

  // Muons
  // https://github.com/cms-sw/cmssw/blob/master/PhysicsTools/NanoAOD/python/muons_cff.py
  TTreeReaderArray<Float_t> Muon_pt = {fReader, "Muon_pt"};
  TTreeReaderArray<Float_t> Muon_tunepRelPt = {fReader, "Muon_tunepRelPt"};
  TTreeReaderArray<Float_t> Muon_eta = {fReader, "Muon_eta"};
  TTreeReaderArray<Float_t> Muon_mass = {fReader, "Muon_mass"};
  TTreeReaderArray<Float_t> Muon_phi = {fReader, "Muon_phi"};
  TTreeReaderArray<Int_t> Muon_charge = {fReader, "Muon_charge"};
  TTreeReaderArray<Int_t> Muon_pdgId = {fReader, "Muon_pdgId"};
  TTreeReaderArray<Bool_t> Muon_looseId = {fReader, "Muon_looseId"};
  TTreeReaderArray<Bool_t> Muon_mediumId = {fReader, "Muon_mediumId"};
  TTreeReaderArray<Bool_t> Muon_tightId = {fReader, "Muon_tightId"};
  TTreeReaderArray<Bool_t> Muon_isPFcand = {fReader, "Muon_isPFcand"};
  TTreeReaderArray<Float_t> Muon_pfRelIso03_all = {fReader, "Muon_pfRelIso03_all"};
  TTreeReaderArray<Float_t> Muon_pfRelIso03_chg = {fReader, "Muon_pfRelIso03_chg"};
  TTreeReaderArray<Float_t> Muon_pfRelIso04_all = {fReader, "Muon_pfRelIso04_all"};
  TTreeReaderArray<Bool_t> Muon_isGlobal = {fReader, "Muon_isGlobal"};
  TTreeReaderArray<Float_t> Muon_dxy = {fReader, "Muon_dxy"};
  TTreeReaderArray<Float_t> Muon_dz = {fReader, "Muon_dz"};
  TTreeReaderArray<UChar_t> Muon_highPtId = {fReader, "Muon_highPtId"};
  TTreeReaderArray<Float_t> Muon_tkRelIso = {fReader, "Muon_tkRelIso"};
  TTreeReaderArray<Float_t> Muon_ip3d = {fReader, "Muon_ip3d"};
  TTreeReaderArray<Float_t> Muon_sip3d = {fReader, "Muon_sip3d"};

  // Electrons
  // https://github.com/cms-sw/cmssw/blob/master/PhysicsTools/NanoAOD/python/electrons_cff.py
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
  TTreeReaderArray<Float_t> Electron_ip3d = {fReader, "Electron_ip3d"};
  TTreeReaderArray<Float_t> Electron_sip3d = {fReader, "Muon_sip3d"};

  // nJets
  TTreeReaderValue<UInt_t> nJet = {fReader, "nJet"};
  TTreeReaderArray<Float_t> Jet_btagDeepFlavB = {fReader, "Jet_btagDeepFlavB"};

  // GenPart
#ifndef CMSDATA
  TTreeReaderValue<Float_t> genWeight = {fReader, "genWeight"};
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
  TTreeReaderValue<Float_t> Pileup_nTrueInt = {fReader, "Pileup_nTrueInt"};
#endif
  // Neutrinos
  TTreeReaderValue<Float_t> MET_phi = {fReader, "MET_phi"};
  TTreeReaderValue<Float_t> MET_pt = {fReader, "MET_pt"};
  TTreeReaderValue<Float_t> MET_significance = {fReader, "MET_significance"};

  Bool_t PairMu;

  TH2F* HPResidualB_T;
  TH2F* HPResidualO_T; // O+E bins Merged

  TH2F* HPResidualB_G;
  TH2F* HPResidualO_G;
  TH2F* HPResidualE_G;

  TH2F* HMassZPt_A_G;
  TH2F* HMassZPt_B_G;
  TH2F* HMassZPt_C_G;
  TH2F* HMassZPt_A_T;
  TH2F* HMassZPt_B_T;
  TH2F* HMassZPt_C_T;

  // THs
  std::vector<TH1F*> HnMu;
  TH1F* HMassZ;
  std::vector<TH1F*> HLt;
  std::vector<TH1F*> HDistl1l2;
  std::vector<TH1F*> HDxyl1;
  std::vector<TH1F*> HDxyl2;
  std::vector<TH2F*> HDxyl1l2;
  std::vector<TH1F*> HDzl1;
  std::vector<TH1F*> HDzl2;
  std::vector<TH1F*> HIP3Dl1;
  std::vector<TH1F*> HIP3Dl2;
  std::vector<TH1F*> HSIP3Dl1;
  std::vector<TH1F*> HSIP3Dl2;
  std::vector<TH1F*> HRelIsol1;
  std::vector<TH1F*> HRelIsol2;

#ifndef CMSDATA

  const Int_t ElPdgId = 11;
  const Int_t MuPdgId = 13;

  std::vector<TH1F*> HGenPartPdgIdl1;
  std::vector<TH1F*> HGenPartPdgIdl2;
  std::vector<TH2F*> HGenPartZ;
  std::vector<TH2F*> HGenPartChgF;
  TH1D *SFPileup;

#endif

  TH1D *HCutFlow;

  std::vector<TH2I*> HNLep;

  std::vector<TH1F*> HPtl1;
  std::vector<TH1F*> HPtl2;
  TH1F* HMuonPt;

  std::vector<TH1F*> HEtal1;
  std::vector<TH1F*> HEtal2;
  std::vector<TH1F*> HMuEta;

  std::vector<TH2F*> HPtEtal1;
  std::vector<TH2F*> HPtEtal2;

  std::vector<TH1F*> HPhil1;
  std::vector<TH1F*> HPhil2;
  std::vector<TH1F*> HMuPhi;

  TH1D *HPileup;
  TH1D *HTruePileup;
  std::vector<TH1F*> HPileup_;

  TH1I* HNMu;

  Int_t l1, l2; // Lepton pair index 
  Int_t leadMuIdx; // Leading is not 0th index
  Int_t leadElIdx;
  Float_t lt; // Sum of leptons Pt


  std::vector<UInt_t> GoodElectron;
  std::vector<UInt_t> GoodMuon;
  PtEtaPhiMVector lep1, lep2, zb;

  Float_t PairZMass;

 public :

  TString SampleName;

  PreSelector(TTree * = 0);
  ~PreSelector() { }
  void    Begin(TTree *tree);
  void    SlaveBegin(TTree *tree);
  Bool_t  Process(Long64_t entry);
  void    Terminate();
  Int_t   Version() const { return 2; }

  std::vector<UInt_t> GetGoodMuon(const Muons&);
  Int_t LeadingIdx(const Leptons& l);
  void SortByDescPt(std::vector<UInt_t>& GoodIdx, const Leptons& l);

  Bool_t CheckMuonPair(const std::pair<UInt_t,UInt_t>&) const;
  Float_t GetEtaPhiDistance(const float&,const float&,const float&,const float&) const;
  Float_t MassRecoZ(const float&,const float&,const float&,const float&,
                     const float&,const float&,const float&,const float&) const;

  std::pair<Int_t,Int_t> GetMother(Int_t,Int_t) const;
  std::pair<Int_t,Int_t> GetMother(std::pair<Int_t,Int_t> Daughter) const;
  Double_t GetPResidual(const Leptons& l, const int& idx) const;


  std::vector<std::pair<UInt_t,UInt_t>> GetMuonPairs(const Muons& m) const;
  ZPairInfo FindZ(const Muons& mu) const;
  ZPairInfo FindZ(const std::vector<std::pair<UInt_t,UInt_t>>& Pairs,
                  const Leptons& l,
                  const std::vector<UInt_t>& GoodLepton) const;

  ClassDef(PreSelector,0);
};

