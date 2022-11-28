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
#include "Electrons.h"
#include "Muons.h"
#include "Leptons.h"
#include <memory>
#include "Math/Vector4D.h"
#include "Math/Vector4Dfwd.h"
#include "TEntryList.h"
#include "EntryListMaker/EventSelection.h"
#include "TGraphAsymmErrors.h"
#include <unordered_map>
#include "XYMETCorrection_withUL17andUL18andUL16.h"

using PtEtaPhiMVector = ROOT::Math::PtEtaPhiMVector;

struct ZPairInfo{

  Float_t Delta = -1.f;
  Float_t Mass = -1.f;
  std::pair<UInt_t,UInt_t> Pair;

  Bool_t empty(){
    return Mass < 0.f;
  }

};

struct MetUnclObj {
  Double_t PtUp;
  Double_t PtDown;
  Double_t PhiUp;
  Double_t PhiDown;
};

struct Nu4VObj {
  ROOT::Math::PxPyPzMVector Met;
  ROOT::Math::PxPyPzMVector MetUnclUp;
  ROOT::Math::PxPyPzMVector MetUnclDown;
};

struct WmtObj {
  Float_t Met; // No Corrections Applied
  Float_t MetUnclUp; //Unclustered Met Up
  Float_t MetUnclDown; //Unclustered Met Down
};

class PreSelector : public EventSelection {

#if !defined(CMSDATA)
  enum {
    kSR1 = 0,
    kCR1 = 16, // 4 Channels * 4 Weights
    kCR2 = 32,
  };
#else
  enum {
    kSR1 = 0,
    kCR1 = 4, // 4 Channels
    kCR2 = 8
  };
#endif

 private :

  const std::pair<float,float> phiHEM = {-1.57,-0.87};
  const std::pair<float,float> etaHEM = {-2.5,-1.3};

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
  TTreeReaderArray<Float_t> Muon_tunepRelPt = {fReader, "Muon_tunepRelPt"};
  TTreeReaderArray<Float_t> Muon_pt = {fReader, "Muon_pt"};
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
  TTreeReaderArray<Float_t> Jet_phi = {fReader, "Jet_phi"};
  TTreeReaderArray<Float_t> Jet_eta = {fReader, "Jet_eta"};
  TTreeReaderArray<Float_t> Jet_pt = {fReader, "Jet_pt"};

  // GenPart
#ifndef CMSDATA
  TTreeReaderValue<UInt_t> nLHEPdfWeight = {fReader, "nLHEPdfWeight"};
  TTreeReaderArray<Float_t> LHEPdfWeight = {fReader, "LHEPdfWeight"};
  TTreeReaderValue<UInt_t> nLHEReweightingWeight = {fReader, "nLHEReweightingWeight"};
  TTreeReaderArray<Float_t> LHEReweightingWeight = {fReader, "LHEReweightingWeight"};
  TTreeReaderValue<UInt_t> nLHEScaleWeight = {fReader, "nLHEScaleWeight"};
  TTreeReaderArray<Float_t> LHEScaleWeight = {fReader, "LHEScaleWeight"};
  TTreeReaderValue<UInt_t> nPSWeight = {fReader, "nPSWeight"};
  TTreeReaderArray<Float_t> PSWeight = {fReader, "PSWeight"};

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
  TTreeReaderValue<Float_t> MET_MetUnclustEnUpDeltaX = {fReader,"MET_MetUnclustEnUpDeltaX"};
  TTreeReaderValue<Float_t> MET_MetUnclustEnUpDeltaY = {fReader,"MET_MetUnclustEnUpDeltaY"};

#if defined(Y2016) || defined (Y2017)
  TTreeReaderValue<Float_t> L1PreFiringWeight_Nom = {fReader, "L1PreFiringWeight_Nom"};
  TTreeReaderValue<Float_t> L1PreFiringWeight_Up = {fReader, "L1PreFiringWeight_Up"};
  TTreeReaderValue<Float_t> L1PreFiringWeight_Dn = {fReader, "L1PreFiringWeight_Dn"};
  TTreeReaderValue<Float_t> L1PreFiringWeight_ECAL_Nom = {fReader, "L1PreFiringWeight_ECAL_Nom"};
  TTreeReaderValue<Float_t> L1PreFiringWeight_ECAL_Up = {fReader, "L1PreFiringWeight_ECAL_Up"};
  TTreeReaderValue<Float_t> L1PreFiringWeight_ECAL_Dn = {fReader, "L1PreFiringWeight_ECAL_Dn"};
  TTreeReaderValue<Float_t> L1PreFiringWeight_Muon_Nom = {fReader, "L1PreFiringWeight_Muon_Nom"};
  TTreeReaderValue<Float_t> L1PreFiringWeight_Muon_Up = {fReader, "L1PreFiringWeight_Muon_SystUp"};
  TTreeReaderValue<Float_t> L1PreFiringWeight_Muon_Dn = {fReader, "L1PreFiringWeight_Muon_SystDn"};
#endif
#endif
  // Neutrinos
  TTreeReaderValue<Float_t> MET_phi = {fReader, "MET_phi"};
  TTreeReaderValue<Float_t> MET_pt = {fReader, "MET_pt"};
  TTreeReaderValue<Float_t> MET_significance = {fReader, "MET_significance"};

  template<typename T, typename... Args>
  void InitHVec(std::vector<T*>& vec, std::string_view name, Args... args);

  // THs
  std::vector<TH1F*> HnEl;
  std::vector<TH1F*> HnMu;
  std::vector<TH1F*> HMassTW;
  std::vector<TH1F*> HMassZ;
  std::vector<TH1F*> HMassW;
  std::vector<TH1F*> HMassWZ;
  std::vector<TH1F*> HPtZMWZ;
  std::vector<TH1F*> HPtWMWZ;
  std::vector<TH1F*> HLt;
  std::vector<TH1F*> HPtl1Lt;
  std::vector<TH1F*> HPtl2Lt;
  std::vector<TH1F*> HPtl3Lt;
  std::vector<TH1F*> HMetPtLt;
  std::vector<TH1F*> HPtl3Met;
  std::vector<TH1F*> HCosl3Met;
  std::vector<TH1F*> HDistl1l2;
  std::vector<TH1F*> HDistl1l3;
  std::vector<TH1F*> HDistl2l3;
  std::vector<TH1F*> HDistZl3;
  std::vector<TH1F*> HDistZW;
  std::vector<TH1F*> HDxyl1;
  std::vector<TH1F*> HDxyl2;
  std::vector<TH1F*> HDxyl3;
  std::vector<TH1F*> HDzl1;
  std::vector<TH1F*> HDzl2;
  std::vector<TH1F*> HDzl3;
  std::vector<TH1F*> HIP3Dl1;
  std::vector<TH1F*> HIP3Dl2;
  std::vector<TH1F*> HIP3Dl3;
  std::vector<TH1F*> HSIP3Dl1;
  std::vector<TH1F*> HSIP3Dl2;
  std::vector<TH1F*> HSIP3Dl3;
  std::vector<TH1F*> HnJet;
  std::vector<TH1F*> HMuonPtDiff;
  std::vector<TH1F*> HMuonPF;

  std::vector<TH2F*> HMassZWZ;
  std::vector<TH2F*> HPtl1l2;
  std::vector<TH2F*> HPtl1l3;
  std::vector<TH2F*> HZElId;
  std::vector<TH2F*> HIDl1l2;
  std::vector<TH2F*> HMuonPtType;

  //Angular


  TH1F *HOverlap;

#ifndef CMSDATA

  const Int_t ElPdgId = 11;
  const Int_t MuPdgId = 13;

  std::vector<TH1F*> HFakeString;
  std::vector<TH1F*> HGenPartPdgIdl1;
  std::vector<TH1F*> HGenPartPdgIdl2;
  std::vector<TH1F*> HGenPartPdgIdl3;
  std::vector<TH2F*> HGenPartZ;
  std::vector<TH2F*> HGenPartW;
  std::vector<TH2F*> HGenPartChgF;
  std::vector<TH2F*> HSFs;
  std::vector<TH2F*> HGenPartZWp;
  std::vector<TH2F*> HGenPartWWp;

  std::vector<TH1F*> HMetUnclUpPtRatio;
  std::vector<TH1F*> HMetUnclUpPhiRatio;
  std::vector<TH1F*> HMetUnclDownPtRatio;
  std::vector<TH1F*> HMetUnclDownPhiRatio;
  std::vector<TH1F*> HNuMetUnclUpPtRatio;
  std::vector<TH1F*> HNuMetUnclUpPhiRatio;
  std::vector<TH1F*> HNuMetUnclDownPtRatio;
  std::vector<TH1F*> HNuMetUnclDownPhiRatio;

#endif

  TH1D *HCutFlow;

  std::vector<TH2I*> HNLep;

  std::vector<TH1F*> HPtLeading;
  std::vector<TH1F*> HPtl1;
  std::vector<TH1F*> HPtl2;
  std::vector<TH1F*> HPtl3;
  std::vector<TH1F*> HElPt;
  std::vector<TH1F*> HMuPt;
  std::vector<TH1F*> HMetPt;
  std::vector<TH1F*> HMetUnclUpPt;
  std::vector<TH1F*> HMetUnclDownPt;

  std::vector<TH1F*> HEtal1;
  std::vector<TH1F*> HEtal2;
  std::vector<TH1F*> HEtal3;
  std::vector<TH1F*> HElEta;
  std::vector<TH1F*> HMuEta;

  std::vector<TH1F*> HPhil1;
  std::vector<TH1F*> HPhil2;
  std::vector<TH1F*> HPhil3;
  std::vector<TH1F*> HElPhi;
  std::vector<TH1F*> HMuPhi;
  std::vector<TH1F*> HMetPhi;
  std::vector<TH1F*> HMetUnclUpPhi;
  std::vector<TH1F*> HMetUnclDownPhi;

  std::vector<TH1F*> HZPt;
  std::vector<TH1F*> HWPt;
  std::vector<TH1F*> HWZPt;


  TH1D *HPileup;
  TH1D *HTruePileup;
  std::vector<TH1F*> HPileup_;

  TH2I* HNEl;
  TH2I* HNMu;

  Int_t l1, l2, l3; // Lepton pair index and lead remaining
  Int_t leadMuIdx; // Leading is not 0th index
  Int_t leadElIdx;
  WmtObj wmt;// W Transverse mass;
  Float_t lt; // Sum of leptons Pt

  Bool_t IsA_{},IsB{},IsC{},IsD{};
  std::pair<double,double> CorrMetPtPhi;

  std::vector<UInt_t> GoodElectron;
  std::vector<UInt_t> GoodMuon;
  std::vector<UInt_t> SameFlvWCand;
  Bool_t PairEl{}, PairMu{};
  PtEtaPhiMVector lep1, lep2, zb, lep3;
  PtEtaPhiMVector* leadingLepton;
  Nu4VObj wb; // Three Values for Met, MetUnclUp and MetUnclDown
  Nu4VObj nu;

  Float_t PairZMass;

  Double_t WCentral, WAllUp, WAllDown;
  Double_t WElTrigUp, WElTrigDown, WElTrigNom,
    WMuTrigUp, WMuTrigDown, WMuTrigNom,
    WLHEPdfUp, WLHEPdfDown,
    WLHEScaleUp, WLHEScaleDown,
    WElRecoUp, WElRecoDown, WElRecoNom,
    WPileupUp, WPileupDown, WPileupNom,
    WElIDUp, WElIDDown, WElIDNom,
    WMuIDUp, WMuIDDown, WMuIDNom,
    WKEWKUp,WKEWKDown, WKEWKNom,
    WKQCDUp,WKQCDDown, WKQCDNom;

  std::unordered_map<std::string,int> HIdx;

 public :

  TString SampleName;
  TString FileNameOut;

  PreSelector(TTree * = 0);
  ~PreSelector() { }
  void    Begin(TTree *tree);
  void    SlaveBegin(TTree *tree);
  Bool_t  Process(Long64_t entry);
  void    Terminate();
  Int_t   Version() const { return 2; }

  std::vector<UInt_t> GetGoodMuon(const Muons&);
  std::vector<UInt_t> GetGoodElectron(const Electrons&);
  Int_t LeadingIdx(const Leptons& l);
  void SortByDescPt(std::vector<UInt_t>& GoodIdx, const Leptons& l);

  bool DefineW(const Leptons& l);
  Bool_t CheckMuonPair(const std::pair<UInt_t,UInt_t>&) const;
  Float_t GetEtaPhiDistance(const float&,const float&,const float&,const float&) const;
  Float_t MassRecoZ(const float&,const float&,const float&,const float&,
                     const float&,const float&,const float&,const float&) const;
  Float_t MassRecoW(const float&,const float&,const float&,const float&) const;
  Float_t MassRecoW(const ROOT::Math::PtEtaPhiMVector&);
  std::pair<int,std::unique_ptr<double[]>> generateBins(const float spacingFactor, const double firstBin, const double secondBin, double limitEdge);

  void FindLeadingLepton();
  void FillCategory(const Int_t& crOffset,
                    const Leptons& lz,const Leptons& lw);
  void FillH1(std::vector<TH1F*>& h1, const Int_t& nh, const Double_t& binContent);

  std::string GetElIDString(Int_t& id);
  std::string GetMuIDString(UChar_t& id);
  std::string GetMuonTypeString(const int& ln);
  Float_t GetMuonPtDiff(const int& ln);
  std::string GetRegionNameFromOffset(const int& offset) const;

#if defined(Y2018)
  Bool_t CheckHEMElectron() ;
  Bool_t CheckHEMJet() ;
#endif

#ifndef CMSDATA
  std::pair<Int_t,Int_t> GetMother(Int_t,Int_t) const;
  std::pair<Int_t,Int_t> GetMother(std::pair<Int_t,Int_t> Daughter) const;
  Double_t GetZPtFromGen() const;
  std::string GetFakeString(const int& genPartIdx, const int& pdgId, const int& idn) const;
  Double_t GetKFactor(TH1* h /*EWK or QCD*/, const Double_t& ZGenPt, const int& option) const;
  Double_t GetElTriggerSF(const Float_t& eta, const Float_t& pt, const Int_t& option) const;
  Double_t GetMuTriggerSF(const Float_t& eta, Float_t pt, const Int_t& option) const;
  Double_t GetMuIDSF(UChar_t MuonID /* 2: highPt. 1: TrkHighPt*/,const Float_t& eta, const Float_t& pt, const Int_t& option) const;
  Double_t GetElIDSF(const Int_t& nl, const Int_t& option) const;
  Double_t GetSFFromHisto(TH1* h,const Float_t& eta,const Float_t& pt,const Int_t& option) const;
  Double_t GetSFFromGraph(TGraphAsymmErrors* g,const Float_t& eta, const Int_t& option) const;
  Float_t GetSFFromHisto(TH1* h, const Int_t& npv, const Int_t option);
  void DefineSFs(const int& nh);
  std::pair<Float_t,Float_t> GetMinMaxFromArray(TTreeReaderValue<UInt_t>* size,
                                                TTreeReaderArray<Float_t>* content);
  MetUnclObj GetMetUncl();

  MetUnclObj MetUncl;

  Bool_t ApplyKFactors{};
  Double_t KSFMinPt;
  Double_t KSFMaxPt;
  TH1F* SFDYKFactorQCD;
  TH1F* SFDYKFactorEWK;

  TGraphAsymmErrors* SFElectronTrigger1;
  TGraphAsymmErrors* SFElectronTrigger2;

#if defined(ULSAMPLE)
  TH2F* SFElectronHLT;
  TH2F* SFElectronHLTLoose;
  TH2F* SFElectronHLTMedium;
  TH2F* SFElectronHLTTight;
  TH2F* SFElectronReco;
  TH2F* SFElectronRecoB20;
#if defined(Y2016)
  TH2F* SFElectronRecopreVFP;
  TH2F* SFElectronRecopostVFP;
  TH2F* SFElectronRecopreVFPB20;
  TH2F* SFElectronRecopostVFPB20;
  TH2F* SFElectronHLTpreVFPLoose;
  TH2F* SFElectronHLTpreVFPMedium;
  TH2F* SFElectronHLTpreVFPTight;
  TH2F* SFElectronHLTpostVFPLoose;
  TH2F* SFElectronHLTpostVFPMedium;
  TH2F* SFElectronHLTpostVFPTight;
#endif
#endif

#endif

  Bool_t PairElDefineW(const Electrons& Els, const Muons& Mus);
  Bool_t PairMuDefineW(const Electrons& Els, const Muons& Mus);
  void FillRegion(const int regOffset, const Electrons& Els, const Muons& Mus);
  std::vector<std::pair<UInt_t,UInt_t>> GetMuonPairs(const Muons& m) const;
  std::vector<std::pair<UInt_t,UInt_t>> GetElectronPermutations(const Electrons& el) const;
  ZPairInfo FindZ(const Electrons& el) const;
  ZPairInfo FindZ(const Muons& mu) const;
  ZPairInfo FindZ(const std::vector<std::pair<UInt_t,UInt_t>>& Pairs,
                  const Leptons& l,
                  const std::vector<UInt_t>& GoodLepton) const;
  Nu4VObj GetNu4V(const ROOT::Math::PtEtaPhiMVector&);
  Nu4VObj GetNu4VFix(const ROOT::Math::PtEtaPhiMVector& lep);
  ROOT::Math::PxPyPzMVector Get4V(const Float_t& pz, const Float_t& pt, const Float_t phi);

  Int_t nbTag();

  Int_t Year;
  Bool_t IsUL;
  Bool_t IsData;

  ClassDef(PreSelector,0);
};

