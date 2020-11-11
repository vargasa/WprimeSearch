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
#include "IsData.h"

using PtEtaPhiMVector = ROOT::Math::PtEtaPhiMVector;

struct ZPairInfo{

  Float_t Delta = -1.f;
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

  // nJets
  TTreeReaderValue<UInt_t> nJet = {fReader, "nJet"};

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
#endif
  // Neutrinos
  TTreeReaderValue<Float_t> MET_phi = {fReader, "MET_phi"};
  TTreeReaderValue<Float_t> MET_pt = {fReader, "MET_pt"};
  TTreeReaderValue<Float_t> MET_significance = {fReader, "MET_significance"};

  template<typename T, typename... Args>
  void InitHVec(std::vector<T*>& vec, std::string_view name, Args... args);


  // THs
  std::vector<TH1F*> HMet_;

  std::vector<TH1F*> HnEl;
  std::vector<TH1F*> HnMu;

  TH1F *HnJet;

  std::vector<TH1F*> HMassTW_;
  std::vector<TH1F*> HMassZ_;
  std::vector<TH1F*> HMassW;

  std::vector<TH2F*> HMassZWZ;

  TH2F* HMassZTW;
  TH2F* HDeltaRPtZ;
  TH2F* HPtWPtZ;
  TH2F* HDeltaRMWZ;
  TH2F* HLtMWZ;

  std::vector<TH1F*> HMassWZ;
  std::vector<TH1F*> HLt;

  //Angular
  TH1F *HDistl1l3;
  TH1F *HDistl2l3;

  std::vector<TH1F*> HDistl1l2;

  TH1F *HOverlap;

#ifndef CMSDATA
  TEntryList *ELPass;
  std::vector<TH1F*> HGenPartZ;
  std::vector<TH1F*> HGenPartW;
  std::vector<TH1F*> HGenPartF;

  TH1F *HScaleFactors;
  TH1D *SFPileup;

  TH1F* HLog;

#endif
  TH1D *HCutFlow;

  std::vector<TH2I*> HNLep;

  TH1F *HPtl1;
  TH1F *HPtl2;
  TH1F *HPtl3;
  TH1F *HMetPt;

  TH1F *HEtal1;
  TH1F *HEtal2;
  TH1F *HEtal3;

  TH2F *HPtEtal1;
  TH2F *HPtEtal2;
  TH2F *HPtEtal3;

  TH1F *HPhil1;
  TH1F *HPhil2;
  TH1F *HPhil3;
  TH1F *HMetPhi;

  TH1F *HWZDist;
  TH1F *HWZPt;
  TH2F *HWZPtDist;

  TH1D *HPileup;
  std::vector<TH1F*> HPileup_;

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

  bool DefineW(Leptons l);
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
#ifdef Y2016
  TH2F* SFMuonTriggerBF;
  TH2F* SFMuonTriggerGH;
  TH2D* SFMuonIDBF;
  TH2D* SFMuonIDGH;
  TGraphAsymmErrors* SFElectronTrigger1;
  TGraphAsymmErrors* SFElectronTrigger2;
#elif defined(Y2017) || defined(Y2018)
  TH2F* SFMuonTrigger;
  TH2D* SFMuonID;
  TGraphAsymmErrors* SFElectronTrigger1;
  TGraphAsymmErrors* SFElectronTrigger2;
#endif
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

