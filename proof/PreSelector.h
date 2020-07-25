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
#include "EntryListMaker/EventSelection.h"
#include "TGraphAsymmErrors.h"

using PtEtaPhiMVector = ROOT::Math::PtEtaPhiMVector;

struct ZPairInfo{

  Float_t Delta = -1;
  Float_t Mass = -1;
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

  TH2F *HMassZWZA;
  TH2F *HMassZWZB;
  TH2F *HMassZWZC;
  TH2F *HMassZWZD;

  TH1F *HMassWZA;
  TH1F *HMassWZB;
  TH1F *HMassWZC;
  TH1F *HMassWZD;

  //Angular
  TH1F *HDistl1l2;
  TH1F *HDistl1l3;
  TH1F *HDistl2l3;

  TH1I *HOverlap;

#ifndef CMSDATA
  TEntryList *ELPass;
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
  TH1F *HScaleFactors;
  TH1D *SFPileup;


  TH1F *HPileupA_SFUp;
  TH1F *HPileupB_SFUp;
  TH1F *HPileupC_SFUp;
  TH1F *HPileupD_SFUp;

  TH1F *HMetA_SFUp;
  TH1F *HMetB_SFUp;
  TH1F *HMetC_SFUp;
  TH1F *HMetD_SFUp;

  TH1F *HMassWA_SFUp;
  TH1F *HMassWB_SFUp;
  TH1F *HMassWC_SFUp;
  TH1F *HMassWD_SFUp;

  TH1F *HMassWZA_SFUp;
  TH1F *HMassWZB_SFUp;
  TH1F *HMassWZC_SFUp;
  TH1F *HMassWZD_SFUp;

  TH1F *HMassZA_SFUp;
  TH1F *HMassZB_SFUp;
  TH1F *HMassZC_SFUp;
  TH1F *HMassZD_SFUp;

  TH1F *HMassTWA_SFUp;
  TH1F *HMassTWB_SFUp;
  TH1F *HMassTWC_SFUp;
  TH1F *HMassTWD_SFUp;

  TH1F *HPileupA_SFDown;
  TH1F *HPileupB_SFDown;
  TH1F *HPileupC_SFDown;
  TH1F *HPileupD_SFDown;

  TH1F *HMetA_SFDown;
  TH1F *HMetB_SFDown;
  TH1F *HMetC_SFDown;
  TH1F *HMetD_SFDown;

  TH1F *HMassWA_SFDown;
  TH1F *HMassWB_SFDown;
  TH1F *HMassWC_SFDown;
  TH1F *HMassWD_SFDown;

  TH1F *HMassWZA_SFDown;
  TH1F *HMassWZB_SFDown;
  TH1F *HMassWZC_SFDown;
  TH1F *HMassWZD_SFDown;

  TH1F *HMassZA_SFDown;
  TH1F *HMassZB_SFDown;
  TH1F *HMassZC_SFDown;
  TH1F *HMassZD_SFDown;

  TH1F *HMassTWA_SFDown;
  TH1F *HMassTWB_SFDown;
  TH1F *HMassTWC_SFDown;
  TH1F *HMassTWD_SFDown;

#endif
  TH1F *HCutFlow;

  TH2I *HNLepA;
  TH2I *HNLepB;
  TH2I *HNLepC;
  TH2I *HNLepD;

  TH1F *HPtl1;
  TH1F *HPtl2;
  TH1F *HPtl3;
  TH1F *HMetPt;

  TH1F *HWZDist;
  TH1F *HWZPt;
  TH2F *HWZPtDist;

  TH1D *HPileup;

  TH1F *HPileupA;
  TH1F *HPileupB;
  TH1F *HPileupC;
  TH1F *HPileupD;

  TH2I *HNEl;
  TH2I *HNMu;

  UInt_t l1, l2, l3; // Lepton pair index and lead remaining
  Float_t wmt; // W Transverse mass;

  std::vector<UInt_t> GoodElectron;
  std::vector<UInt_t> GoodMuon;
  PtEtaPhiMVector lep1, lep2, zb, wb, lep3;
  std::vector<ROOT::Math::PxPyPzMVector> nu;

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
  std::vector<UInt_t> GetGoodElectron(const Electrons&);

  void DefineW(Leptons l);
  Bool_t CheckElectronPair(const std::pair<UInt_t,UInt_t>&) const;
  Bool_t CheckMuonPair(const std::pair<UInt_t,UInt_t>&) const;
  Float_t GetEtaPhiDistance(const float&,const float&,const float&,const float&) const;
  Float_t MassRecoZ(const float&,const float&,const float&,const float&,
                     const float&,const float&,const float&,const float&) const;
  Float_t MassRecoW(const float&,const float&,const float&,const float&) const;
  Float_t MassRecoW(const ROOT::Math::PtEtaPhiMVector&);

  void FillCommon(const Leptons& lz,const Leptons& lw);
  void FillA();
  void FillB();
  void FillC();
  void FillD();

#ifndef CMSDATA
  std::pair<Int_t,Int_t> GetMother(Int_t,Int_t) const;
  std::pair<Int_t,Int_t> GetMother(std::pair<Int_t,Int_t> Daughter) const;

  Double_t GetElectronSF(const Float_t& eta, const Float_t& pt, const Int_t& option) const;
  Double_t GetMuonSF(const Float_t& eta,const Float_t& pt, const Int_t& option) const;
  Double_t GetSFFromHisto(TH1* h,const Float_t& eta,const Float_t& pt,const Int_t& option) const;
  Double_t GetSFFromGraph(TGraphAsymmErrors* g,const Float_t& eta, const Int_t& option) const;
  Float_t GetSFFromHisto(TH1* h, const Int_t& npv);

  TList *SFDb;
  TH2F* SFMuonTriggerBF;
  TH2F* SFMuonTriggerGH;
  TH2D* SFMuonIDBF;
  TH2D* SFMuonIDGH;
  TGraphAsymmErrors* SFElectronTrigger1;
  TGraphAsymmErrors* SFElectronTrigger2;
#endif

  std::pair<UInt_t,UInt_t> GetLeptonPair(const Leptons&, const std::vector<UInt_t>&) const;
  ZPairInfo FindZ(const Leptons&,const std::vector<UInt_t>&) const;
  std::vector<ROOT::Math::PxPyPzMVector> GetNu4V(const ROOT::Math::PtEtaPhiMVector&,const Float_t&);
  std::vector<ROOT::Math::PxPyPzMVector> GetNu4VAlt(ROOT::Math::PtEtaPhiMVector,Float_t); 
  std::vector<ROOT::Math::PxPyPzMVector> GetNu4VFix(const ROOT::Math::PtEtaPhiMVector& lep,
                                                    const Float_t& Wmt);
  ROOT::Math::PxPyPzMVector Get4V(const Float_t& Pz);

  ClassDef(PreSelector,0);
};

