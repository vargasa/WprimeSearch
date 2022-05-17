#include "TFile.h"
#include "PreSelector.h"
#include "TMath.h"
#include "TLegend.h"
#include "TError.h"
#include "TVector2.h"
#include <functional>
#include <bitset>

#define FillS(xx) Fill(xx,1.)

PreSelector::PreSelector(TTree *)
{

  HOverlap=0;

#ifndef CMSDATA
  SFPileup = 0;
#endif
  HCutFlow = 0;
  HNEl = 0;
  HNMu = 0;
  HTruePileup = 0;
  HPileup = 0;

}

Int_t PreSelector::nbTag(){

  float medium;
  if (Year == 2016) {
    medium = 0.3093;
  } else if (Year == 2017) {
    medium = 0.3033;
  } else if (Year == 2018) {
    medium = 0.2770;
  }

  Int_t nbtag = 0;
  for(uint i = 0; i < *nJet; ++i){
    if( Jet_btagDeepFlavB[i] > medium){
      ++nbtag;
    }
  }
  return nbtag;
}

Float_t PreSelector::GetMuonPtDiff(const int& ln){
  return Muon_tunepRelPt[ln] - 1.;
}

std::string PreSelector::GetMuonTypeString(const int& ln){

  std::string type;

  if (Muon_isPFcand[ln]){
    type = "Muon.PF";
  } else {
    type = "Muon.NonPF";
  }

  return type;
}

void PreSelector::Begin(TTree *tree) {

  if (fInput->FindObject("SampleName")) {
    // Lesson: TString can't be in TCollection
    TNamed *p = dynamic_cast<TNamed *>(fInput->FindObject("SampleName"));
    SampleName = p->GetTitle();
  }

  if (fInput->FindObject("FileNameOut")) {
    TNamed *p = dynamic_cast<TNamed *>(fInput->FindObject("FileNameOut"));
    FileNameOut = p->GetTitle();
  }

    if (fInput->FindObject("Year")) {
    TNamed *p = dynamic_cast<TNamed *>(fInput->FindObject("Year"));
    Year = atoi(p->GetTitle());
  }

  if (fInput->FindObject("IsData")){
    TNamed *p = dynamic_cast<TNamed *>(fInput->FindObject("IsData"));
    if(std::string(p->GetTitle()).find("true") != std::string::npos){
      IsData = true;
    } else {
      IsData = false;
    }
  }

  if (fInput->FindObject("IsUL")){
    TNamed *p = dynamic_cast<TNamed *>(fInput->FindObject("IsUL"));
    if(std::string(p->GetTitle()).find("true") != std::string::npos){
      IsUL = true;
    } else {
      IsUL = false;
    }
  }

  std::clog << "Processing Year: [" << Year << "]\tIsData? " << IsData << "\tIsUL? " << IsUL << "\n";
}

template<typename T, typename... Args>
void PreSelector::InitHVec(std::vector<T*>& vec,
                           std::string_view name,
                           Args... args){

  std::vector<std::string> idst = {};
  std::vector<std::string> crs_ = { "SR1","CR1","CR2","CR3" };
  std::vector<std::string> weights_ = { "NoSF", "WCentral", "WAllUp", "WAllDown"};
  std::vector<std::string> chs_ = { "A", "B", "C", "D" };

  for(auto cr: crs_){
    for(auto w: weights_){
      for(auto ch: chs_){
        idst.emplace_back(cr+"_"+ch+"_"+w);
      }
    }
  }

  for(const auto id: idst){
    vec.emplace_back(new T(Form("%s_%s",name.data(),id.c_str()),
                           name.data(),
                           args...));
  }

#ifndef CMSDATA

  if(name == "HMassWZ"){

    std::vector<std::string> rgs = { "SR1" };
    std::vector<std::string> chs = { "A","B","C","D" };
    std::vector<std::string> sys = {
      "ElReco","ElTrigger","ElID",
      "MuTrigger","MuID",
      "MetUncl",
      "Pileup",
      "AllUp","AllDown"
    };
#if defined(Y2016) || defined (Y2017)
    sys.push_back("L1Pref");
#endif
    std::vector<std::string> limit = { "Up","Down" };
    std::vector<std::string> syst;

    for(auto r: rgs) {
      for(auto c: chs) {
        for(auto s: sys) {
          for(auto l: limit){
            syst.push_back(Form("%s_%s_%s_%s",r.c_str(),c.c_str(),s.c_str(),l.c_str()));
          }
        }
      }
    }

    if(ApplyKFactors){
      std::vector<std::string> type = { "EWK","QCD" };
      for(auto r:rgs){
        for(auto ch: chs){
          for(auto t: type){
            for(auto l: limit){
              syst.push_back(Form("%s_%s_KFactor_%s_%s",r.c_str(),ch.c_str(),t.c_str(),l.c_str()));
            }
          }
        }
      }
    }

    for(const auto id: syst){
      vec.emplace_back(new T(Form("%s_%s",name.data(),id.c_str()),
                             name.data(),
                             args...));
    }

    int ci = idst.size();
    for(const auto s: syst){
      HIdx.insert({s,ci++});
    }
  }

#endif

  for(auto h: vec){
    fOutput->Add(h);
  }

}

void PreSelector::SlaveBegin(TTree *tree) {

  if (fInput->FindObject("FileNameOut")) {
    TNamed *p = dynamic_cast<TNamed *>(fInput->FindObject("FileNameOut"));
    FileNameOut = p->GetTitle();
  }

  if (fInput->FindObject("Year")) {
    TNamed *p = dynamic_cast<TNamed *>(fInput->FindObject("Year"));
    Year = atoi(p->GetTitle());
  }

  if (fInput->FindObject("IsData")){
    TNamed *p = dynamic_cast<TNamed *>(fInput->FindObject("IsData"));
    if(std::string(p->GetTitle()).find("true") != std::string::npos){
      IsData = true;
    } else {
      IsData = false;
    }
  }

  if (fInput->FindObject("IsUL")){
    TNamed *p = dynamic_cast<TNamed *>(fInput->FindObject("IsUL"));
    if(std::string(p->GetTitle()).find("true") != std::string::npos){
      IsUL = true;
    } else {
      IsUL = false;
    }
  }

  std::clog << "Processing Year: [" << Year << "]\tIsData? " << IsData << "\tIsUL? " << IsUL << "\n";

  TH1::SetDefaultSumw2();

  const Int_t DistBins = 32;    // 0.2 Bin size
  const Float_t MaxDist = 6.4;  // >2*Pi

  InitHVec<TH1F>(HDistl1l2,"HDistl1l2",DistBins,0.,MaxDist);
  InitHVec<TH1F>(HDistl1l3,"HDistl1l3",DistBins,0.,MaxDist);
  InitHVec<TH1F>(HDistl2l3,"HDistl2l3",DistBins,0.,MaxDist);
  InitHVec<TH1F>(HDistZl3,"HDistZl3",DistBins,0.,MaxDist);
  InitHVec<TH1F>(HDistZW,"HDistZW",DistBins,0.,MaxDist);
  InitHVec<TH2F>(HIDl1l2,"HIDl1l2",2,0.,2.,3,0.,3.);

  InitHVec<TH1F>(HWZDist,"HWZDist",DistBins,0.,MaxDist);
  InitHVec<TH2F>(HWZPtDist,"HWZPtDist",100,0.,1400.,DistBins,0.,MaxDist);
  InitHVec<TH1F>(HZPt,"HZPt",60,0.,1e3);
  InitHVec<TH1F>(HWPt,"HWPt",60,0.,1e3);
  InitHVec<TH1F>(HWZPt,"HWZPt",60,0.,1e3);


  InitHVec<TH1F>(HDxyl1,"HDxyl1",1500,-0.25,0.25);
  InitHVec<TH1F>(HDxyl2,"HDxyl2",1500,-0.25,0.25);
  InitHVec<TH1F>(HDxyl3,"HDxyl3",1500,-0.25,0.25);

  InitHVec<TH2F>(HDxyl1l2,"HDxyl1l2",
                 1500,-0.25,0.25,
                 1500,-0.25,0.25);

  InitHVec<TH1F>(HDzl1,"HDzl1",400,-0.5,0.5);
  InitHVec<TH1F>(HDzl2,"HDzl2",400,-0.5,0.5);
  InitHVec<TH1F>(HDzl3,"HDzl3",400,-0.5,0.5);

  InitHVec<TH1F>(HIP3Dl1,"HIP3Dl1",400,-0.5,0.5);
  InitHVec<TH1F>(HIP3Dl2,"HIP3Dl2",400,-0.5,0.5);
  InitHVec<TH1F>(HIP3Dl3,"HIP3Dl3",400,-0.5,0.5);

  InitHVec<TH1F>(HSIP3Dl1,"HSIP3Dl1",400,-50.,50.);
  InitHVec<TH1F>(HSIP3Dl2,"HSIP3Dl2",400,-50.,50.);
  InitHVec<TH1F>(HSIP3Dl3,"HSIP3Dl3",400,-50.,50.);

  InitHVec<TH1F>(HRelIsol1,"HRelIsol1",100,0.,1.0);
  InitHVec<TH1F>(HRelIsol2,"HRelIsol2",100,0.,1.0);
  InitHVec<TH1F>(HRelIsol3,"HRelIsol3",100,0.,1.0);

  const Double_t MaxnLep = 7;
  const Double_t MinnLep = 0;
  const Int_t nLepBins = 7;

  InitHVec<TH1F>(HnEl,"HnEl",nLepBins,MinnLep,MaxnLep);
  InitHVec<TH1F>(HnMu,"HnMu",nLepBins,MinnLep,MaxnLep);

  const Int_t nJetBins = 15;
  InitHVec<TH1F>(HnJet,"HnJet",nJetBins,0.,(float)nJetBins);

  InitHVec<TH1F>(HMuonPtDiff,"HMuonPtDiff",201,-0.4,0.4);
  InitHVec<TH1F>(HMuonPF,"HMuonPF",2,0.,2.);

  const Float_t MinMass = 0.;
  const Float_t MaxMass = 2200.;
  const Int_t MassBins = 22;

  const Float_t MinWMass = 0.;
  const Float_t MaxWMass = 500;

  InitHVec<TH1F>(HMassW,"HMassW",MassBins,MinWMass,MaxWMass);

  const Float_t HMaxZMass = 200.;
  const Float_t HMinZMass = 0.;
  const Int_t ZMassBins = 100;
  InitHVec<TH1F>(HMassZ,"HMassZ",ZMassBins,HMinZMass,HMaxZMass);

  const Float_t MaxTWMass = 250.;
  const Int_t TWMassBins = 15;
  InitHVec<TH1F>(HMassTW,"HMassTW",TWMassBins,0.,MaxTWMass);

  InitHVec<TH1F>(HPtZMWZ,"HPtZMWZ",60,0.,2);
  InitHVec<TH1F>(HPtWMWZ,"HPtWMWZ",60,0.,2);

  const Float_t MaxLt = 1000.;
  const Int_t NLtBins = 50;
  InitHVec<TH1F>(HLt,"HLt",NLtBins,0.,MaxLt);

  InitHVec<TH1F>(HPtl1Lt,"HPtl1Lt",50 , 0., 1.);
  InitHVec<TH1F>(HPtl2Lt,"HPtl2Lt",50 , 0., 1.);
  InitHVec<TH1F>(HPtl3Lt,"HPtl3Lt",50 , 0., 1.);
  InitHVec<TH1F>(HMetPtLt,"HMetPtLt",50 , 0., 1.);
  InitHVec<TH1F>(HPtl3Met,"HPtl3Met",150 , 0., 3.);

  InitHVec<TH1F>(HCosl3Met,"HCosl3Met",100 , -1., 1.);

  InitHVec<TH2F>(HMassZWZ,"HMassZWZ",MassBins,0.,1.5*HMaxZMass,MassBins,0.,MaxMass);
  InitHVec<TH2F>(HMuonPtType,"HMuonPtType",2,0.,2.,201,-0.4,0.4);

  InitHVec<TH2F>(HZElId,"HZElId",6,-0.5,5.5,6,-0.5,5.5);

  const Float_t MaxDeltaR = 6.f;
  const Int_t NBinsDeltaR = 10;
  const Float_t MaxWZM = 5000.f;
  const Float_t MinWZM = 500.f;
  const Int_t NBinsWZM = 45;
  const Int_t ZPtBins = 20;
  const Float_t ZPtMax = 2000;
  InitHVec<TH2F>(HMassZTW,"HMassZTW",
                 ZMassBins, HMinZMass, HMaxZMass,
                 TWMassBins,0.,MaxTWMass);

  InitHVec<TH2F>(HDeltaRPtZ,"HDeltaRPtZ",
                 NBinsDeltaR,0.,MaxDeltaR,
                 ZPtBins,0.,ZPtMax);

  InitHVec<TH2F>(HPtWPtZ,"HPtWPtZ",
                 110,0.,1100.,
                 ZPtBins,0.,ZPtMax);

  InitHVec<TH2F>(HDeltaRMWZ,"HDeltaRMWZ",
                 NBinsDeltaR,0.,MaxDeltaR,
                 NBinsWZM,MinWZM,MaxWZM);

  InitHVec<TH2F>(HLtMWZ,"HLtMWZ",
                 20,0.,2000.,
                 NBinsWZM,MinWZM,MaxWZM);


  HOverlap = new TH1F("HOverlap","Overlapping events."
                      " -1: l<3 0:None 1: NoOverlap",6,-1,5);
  fOutput->Add(HOverlap);

  const Int_t nPvsBins = 99;
  const Float_t minPvs = 0.;
  const Float_t maxPvs = 99.;

  HPileup = new TH1D("HPileup","PV_npvs",
                     nPvsBins,minPvs,maxPvs);
  fOutput->Add(HPileup);

  HTruePileup = new TH1D("HTruePileup","PV_npvs",
                         nPvsBins,minPvs,maxPvs);

  fOutput->Add(HTruePileup);

  InitHVec<TH1F>(HPileup_,"HPileup",nPvsBins,minPvs,maxPvs);


  HCutFlow = new TH1D("HCutFlow","",50,0.,50.);  /* Limits are meaningless here */
  fOutput->Add(HCutFlow);

  const Int_t BinsLep = 10;
  const Float_t MinLep = 0.;
  const Float_t MaxLep = 10.;

  InitHVec<TH2I>(HNLep,"HNLep",BinsLep,MinLep,MaxLep,BinsLep,MinLep,MaxLep);

  HNEl = new TH2I("HNEl","",BinsLep,MinLep,MaxLep,BinsLep,MinLep,MaxLep);
  fOutput->Add(HNEl);
  HNMu = new TH2I("HNMu","",BinsLep,MinLep,MaxLep,BinsLep,MinLep,MaxLep);
  fOutput->Add(HNMu);

  const Float_t MaxPt = 2000.f;
  const Double_t MaxMet = 600.;
  const Double_t MinMet = 0.;
  const Float_t MaxPtMet = 1000.f;
  const Int_t MetBins = 60;

  InitHVec<TH1F>(HPtLeading,"HPtLeading",200,0.,MaxPt);
  InitHVec<TH1F>(HPtl1,"HPtl1",200,0.,MaxPt);
  InitHVec<TH1F>(HPtl2,"HPtl2",400,0.,MaxPt);
  InitHVec<TH1F>(HPtl3,"HPtl3",200,0.,MaxPt);
  InitHVec<TH1F>(HElPt,"HElPt",200,0.,MaxPt);
  InitHVec<TH1F>(HMuPt,"HMuPt",200,0.,MaxPt);
  InitHVec<TH1F>(HMetPt,"HMetPt",200,0.,MaxPt);

  const Float_t MaxEta = 3.;
  const Int_t EtaBins = 25;

  InitHVec<TH1F>(HEtal1,"HEtal1",EtaBins,-1*MaxEta,MaxEta);
  InitHVec<TH1F>(HEtal2,"HEtal2",EtaBins,-1*MaxEta,MaxEta);
  InitHVec<TH1F>(HEtal3,"HEtal3",EtaBins,-1*MaxEta,MaxEta);
  InitHVec<TH1F>(HElEta,"HElEta",EtaBins,-1*MaxEta,MaxEta);
  InitHVec<TH1F>(HMuEta,"HMuEta",EtaBins,-1*MaxEta,MaxEta);

  InitHVec<TH2F>(HPtEtal1,"HPtEtal1",
                 MetBins,0, MaxPtMet,
                 EtaBins,-1*MaxEta,MaxEta);

  InitHVec<TH2F>(HPtEtal2,"HPtEtal2",
                 MetBins,0, MaxPtMet,
                 EtaBins,-1*MaxEta,MaxEta);


  InitHVec<TH2F>(HPtEtal3,"HPtEtal3",
                 MetBins,0, MaxPtMet,
                 EtaBins,-1*MaxEta,MaxEta);

  const Float_t MaxPhi = TMath::Pi();
  const Int_t PhiBins = 20;

  InitHVec<TH2F>(HEtaPhil1,"HEtaPhil1",EtaBins,-1.*MaxEta,MaxEta,PhiBins,-1*MaxPhi,MaxPhi);
  InitHVec<TH2F>(HEtaPhil2,"HEtaPhil2",EtaBins,-1.*MaxEta,MaxEta,PhiBins,-1*MaxPhi,MaxPhi);
  InitHVec<TH2F>(HEtaPhil3,"HEtaPhil3",EtaBins,-1.*MaxEta,MaxEta,PhiBins,-1*MaxPhi,MaxPhi);
  InitHVec<TH2F>(HPtl1l2,"HPtl1l2",50,0,250,50,0,250);
  InitHVec<TH2F>(HPtl1l3,"HPtl1l3",50,0,250,50,0,250);
  InitHVec<TH1F>(HPhil1,"HPhil1",PhiBins,-1*MaxPhi,MaxPhi);
  InitHVec<TH1F>(HPhil2,"HPhil2",PhiBins,-1*MaxPhi,MaxPhi);
  InitHVec<TH1F>(HPhil3,"HPhil3",PhiBins,-1*MaxPhi,MaxPhi);
  InitHVec<TH1F>(HElPhi,"HElPhi",PhiBins,-1*MaxPhi,MaxPhi);
  InitHVec<TH1F>(HMuPhi,"HMuPhi",PhiBins,-1*MaxPhi,MaxPhi);
  InitHVec<TH1F>(HMetPhi,"HMetPhi",PhiBins,-1*MaxPhi,MaxPhi);

#if !defined(CMSDATA)

  InitHVec<TH1F>(HMetUnclUpPt,"HMetUnclUpPt",100,0.,MaxPt);
  InitHVec<TH1F>(HMetUnclDownPt,"HMetUnclDownPt",100,0.,MaxPt);

  InitHVec<TH1F>(HMetUnclUpPhi,"HMetUnclUpPhi",PhiBins,-1*MaxPhi,MaxPhi);
  InitHVec<TH1F>(HMetUnclDownPhi,"HMetUnclDownPhi",PhiBins,-1*MaxPhi,MaxPhi);

  InitHVec<TH1F>(HFakeString,"HFakeString",15,0,15);

  std::vector<std::string> prefill = {
    "El.Prompt.Loose","El.Prompt.Medium","El.Prompt.Tight",
    "El.NonPrompt.Loose","El.NonPrompt.Medium","El.NonPrompt.Tight",
    "El.HFD.Loose","El.HFD.Medium","El.HFD.Tight",
    "Mu.Prompt.Tracker","Mu.Prompt.Global",
    "Mu.NonPrompt.Tracker","Mu.NonPrompt.Global",
    "Mu.HFD.Tracker","Mu.HFD.Global"
  };

  for(long unsigned int n = 0; n < HFakeString.size(); ++n){
    for(auto& ss: prefill){
      HFakeString[n]->Fill(ss.c_str(),1e-6);
    }
  }

  const UInt_t BinsPdgId = 101;
  const Float_t PdgIdMin = -50.5;
  const Float_t PdgIdMax = 50.5;

  InitHVec<TH1F>(HGenPartPdgIdl1,"HGenPartPdgIdl1",
                 BinsPdgId,PdgIdMin,PdgIdMax);
  InitHVec<TH1F>(HGenPartPdgIdl2,"HGenPartPdgIdl2",
                 BinsPdgId,PdgIdMin,PdgIdMax);
  InitHVec<TH1F>(HGenPartPdgIdl3,"HGenPartPdgIdl3",
                 BinsPdgId,PdgIdMin,PdgIdMax);
  InitHVec<TH2F>(HGenPartZ,"HGenPartZ",
                 BinsPdgId,PdgIdMin,PdgIdMax,
                 BinsPdgId,PdgIdMin,PdgIdMax);
  InitHVec<TH2F>(HGenPartW,"HGenPartW",
                 BinsPdgId,PdgIdMin,PdgIdMax,
                 BinsPdgId,PdgIdMin,PdgIdMax);
  InitHVec<TH2F>(HGenPartZWp,"HGenPartZWp",
                 BinsPdgId,PdgIdMin,PdgIdMax,
                 BinsPdgId,PdgIdMin,PdgIdMax);
  InitHVec<TH2F>(HGenPartWWp,"HGenPartWWp",
                 BinsPdgId,PdgIdMin,PdgIdMax,
                 BinsPdgId,PdgIdMin,PdgIdMax);
  InitHVec<TH2F>(HGenPartChgF,"HGenPartChgF",6,-2.,2.,BinsPdgId,PdgIdMin,PdgIdMax);


  if (fInput->FindObject("SampleName")) {
    // Lesson: TString can't be in TCollection
    TNamed *p = dynamic_cast<TNamed *>(fInput->FindObject("SampleName"));
    SampleName = p->GetTitle();
  }

  if(fInput->FindObject("SFDb")){
    SFDb = dynamic_cast<TList*>(fInput->FindObject("SFDb"));
  }

  assert(SFDb);

  if(SampleName.Contains("DYJetsToLL")){
    ApplyKFactors = true;
    SFDYKFactorQCD = static_cast<TH1F*>(SFDb->FindObject("SFDYKFactorQCD"));
    SFDYKFactorEWK = static_cast<TH1F*>(SFDb->FindObject("SFDYKFactorEWK"));
    assert(SFDYKFactorEWK and SFDYKFactorQCD);
    KSFMinPt = SFDYKFactorQCD->GetBinLowEdge(1);
    KSFMaxPt = SFDYKFactorQCD->GetBinLowEdge(SFDYKFactorQCD->GetNbinsX())
      + SFDYKFactorQCD->GetBinWidth(SFDYKFactorQCD->GetNbinsX());
    std::clog << Form("Apply kFactors to sample: %s\n",SampleName.Data());

  }

#if defined(Y2016) && !defined(ULSAMPLE)
  SFElectronTrigger1 = dynamic_cast<TGraphAsymmErrors*>(SFDb->FindObject("SFElectronTrigger1"));
  SFElectronTrigger2 = dynamic_cast<TGraphAsymmErrors*>(SFDb->FindObject("SFElectronTrigger2"));
  SFMuonTriggerBF = dynamic_cast<TH2F*>(SFDb->FindObject("SFMuonTriggerBF"));
  SFMuonTriggerGH = dynamic_cast<TH2F*>(SFDb->FindObject("SFMuonTriggerGH"));
  SFMuonHighPtIDBF = dynamic_cast<TH2D*>(SFDb->FindObject("SFMuonHighPtIDBF"));
  SFMuonHighPtIDGH = dynamic_cast<TH2D*>(SFDb->FindObject("SFMuonHighPtIDGH"));
  SFElectronLooseID = dynamic_cast<TH2F*>(SFDb->FindObject("SFElectronLooseID"));
  SFElectronMediumID = dynamic_cast<TH2F*>(SFDb->FindObject("SFElectronLooseID")); // Find Legacy Medium
  SFElectronTightID = dynamic_cast<TH2F*>(SFDb->FindObject("SFElectronTightID"));
#endif

#if defined(Y2016) and defined(ULSAMPLE)
  SFPileuppreVFP = dynamic_cast<TH1F*>(SFDb->FindObject("PileupNominalpreVFP"));
  SFPileuppostVFP = dynamic_cast<TH1F*>(SFDb->FindObject("PileupNominalpostVFP"));
  SFPileupUppreVFP = dynamic_cast<TH1F*>(SFDb->FindObject("PileupUppreVFP"));
  SFPileupDownpreVFP = dynamic_cast<TH1F*>(SFDb->FindObject("PileupDownpreVFP"));
  SFPileupUppostVFP = dynamic_cast<TH1F*>(SFDb->FindObject("PileupUppostVFP"));
  SFPileupDownpostVFP = dynamic_cast<TH1F*>(SFDb->FindObject("PileupDownpostVFP"));
  SFMuonTrigger =  dynamic_cast<TH2F*>(SFDb->FindObject("SFMuonTrigger"));
  SFMuonHighPtIDpreVFP = dynamic_cast<TH2F*>(SFDb->FindObject("SFMuonHighPtIDpreVFP"));
  SFMuonHighPtIDpostVFP = dynamic_cast<TH2F*>(SFDb->FindObject("SFMuonHighPtIDpostVFP"));
  SFMuonTrkHighPtIDpreVFP =  dynamic_cast<TH2F*>(SFDb->FindObject("SFMuonTrkHighPtIDpreVFP"));
  SFMuonTrkHighPtIDpostVFP = dynamic_cast<TH2F*>(SFDb->FindObject("SFMuonTrkHighPtIDpostVFP"));
  SFElectronRecopreVFP =  dynamic_cast<TH2F*>(SFDb->FindObject("SFElectronRecopreVFP"));
  SFElectronRecopostVFP =  dynamic_cast<TH2F*>(SFDb->FindObject("SFElectronRecopostVFP"));
  SFElectronRecopreVFPB20 =  dynamic_cast<TH2F*>(SFDb->FindObject("SFElectronRecopreVFPB20"));
  SFElectronRecopostVFPB20 =  dynamic_cast<TH2F*>(SFDb->FindObject("SFElectronRecopostVFPB20"));
  SFElectronHLTLoosepreVFP =  dynamic_cast<TH2F*>(SFDb->FindObject("SFElectronHLTLoosepreVFP"));
  SFElectronHLTMediumpreVFP =  dynamic_cast<TH2F*>(SFDb->FindObject("SFElectronHLTMediumpreVFP"));
  SFElectronHLTTightpreVFP =  dynamic_cast<TH2F*>(SFDb->FindObject("SFElectronHLTTightpreVFP"));
  SFElectronHLTLoosepostVFP =  dynamic_cast<TH2F*>(SFDb->FindObject("SFElectronHLTLoosepostVFP"));
  SFElectronHLTMediumpostVFP =  dynamic_cast<TH2F*>(SFDb->FindObject("SFElectronHLTMediumpostVFP"));
  SFElectronHLTTightpostVFP =  dynamic_cast<TH2F*>(SFDb->FindObject("SFElectronHLTTightpostVFP"));
  SFElectronLooseIDpreVFP = dynamic_cast<TH2F*>(SFDb->FindObject("SFElectronLooseIDpreVFP"));
  SFElectronLooseIDpostVFP = dynamic_cast<TH2F*>(SFDb->FindObject("SFElectronLooseIDpostVFP"));
  SFElectronMediumIDpreVFP = dynamic_cast<TH2F*>(SFDb->FindObject("SFElectronMediumIDpreVFP"));
  SFElectronMediumIDpostVFP = dynamic_cast<TH2F*>(SFDb->FindObject("SFElectronMediumIDpostVFP"));
  SFElectronTightIDpreVFP = dynamic_cast<TH2F*>(SFDb->FindObject("SFElectronTightIDpreVFP"));
  SFElectronTightIDpostVFP = dynamic_cast<TH2F*>(SFDb->FindObject("SFElectronTightIDpostVFP"));
#endif

#if !defined(ULSAMPLE) and (defined(Y2017) || defined(Y2018))
  SFMuonTrigger = dynamic_cast<TH2F*>(SFDb->FindObject("SFMuonTrigger"));
  SFMuonHighPtID = dynamic_cast<TH2F*>(SFDb->FindObject("SFMuonHighPt"));
  SFMuonTrkHighPtID = dynamic_cast<TH2F*>(SFDb->FindObject("SFMuonTrkHighPtID"));
  SFElectronTrigger1 = dynamic_cast<TGraphAsymmErrors*>(SFDb->FindObject("SFElectronTrigger1"));
  SFElectronTrigger2 = dynamic_cast<TGraphAsymmErrors*>(SFDb->FindObject("SFElectronTrigger2"));
  SFElectronLooseID = dynamic_cast<TH2F*>(SFDb->FindObject("SFElectronLooseID"));
  SFElectronMediumID = dynamic_cast<TH2F*>(SFDb->FindObject("SFElectronLooseID")); // Find Legacy Medium
  SFElectronTightID = dynamic_cast<TH2F*>(SFDb->FindObject("SFElectronTightID"));
#endif

#if defined(ULSAMPLE) and (defined(Y2017) || defined(Y2018))
  SFPileup = dynamic_cast<TH1F*>(SFDb->FindObject("PileupNominal"));
  SFPileupUp = dynamic_cast<TH1F*>(SFDb->FindObject("PileupUp"));
  SFPileupDown = dynamic_cast<TH1F*>(SFDb->FindObject("PileupDown"));
  SFMuonTrigger = dynamic_cast<TH2F*>(SFDb->FindObject("SFMuonTrigger"));
  SFMuonHighPtID = dynamic_cast<TH2F*>(SFDb->FindObject("SFMuonHighPt"));
  SFMuonTrkHighPtID = dynamic_cast<TH2F*>(SFDb->FindObject("SFMuonTrkHighPtID"));
  SFElectronReco =  dynamic_cast<TH2F*>(SFDb->FindObject("SFElectronReco"));
  SFElectronRecoB20 =  dynamic_cast<TH2F*>(SFDb->FindObject("SFElectronRecoB20"));
  SFElectronHLTLoose =  dynamic_cast<TH2F*>(SFDb->FindObject("SFElectronHLTLoose"));
  SFElectronHLTMedium =  dynamic_cast<TH2F*>(SFDb->FindObject("SFElectronHLTMedium"));
  SFElectronHLTTight =  dynamic_cast<TH2F*>(SFDb->FindObject("SFElectronHLTTight"));
  SFElectronLooseID = dynamic_cast<TH2F*>(SFDb->FindObject("SFElectronLooseID"));
  SFElectronMediumID = dynamic_cast<TH2F*>(SFDb->FindObject("SFElectronMediumID"));
  SFElectronTightID = dynamic_cast<TH2F*>(SFDb->FindObject("SFElectronTightID"));
#endif

  InitHVec<TH1F>(HScaleFactors,"HScaleFactors",70,-1.,6.);
  InitHVec<TH2F>(HSFs,"HSFs",15,0.,15.,150,0.,3.);
#endif

  const Double_t wzbins[31] = {
    60,80,100,125,150,175,205,235,265,300,335,370,410,450,490,540,590,650,720,790,890,1000,
    1190,1340,1500,2000,2500,3000,4000,5500,7500
  };

  InitHVec<TH1F>(HMassWZ,"HMassWZ",30,wzbins);
}

void PreSelector::SortByDescPt(std::vector<UInt_t>& GoodIdx, const Leptons& l){
   std::function<bool(const int&, const int&)> sortDescPt =
    [&](const Int_t& i, const Int_t& j){
      return !(l.pt[i] < l.pt[j]);
    };

   std::sort(GoodIdx.begin(),GoodIdx.end(),
            sortDescPt);
}

std::vector<UInt_t> PreSelector::GetGoodMuon(const Muons& Mu){
  std::vector<UInt_t> GoodIndex = {};
  if(!MuonTest()) return GoodIndex;
  if(*Mu.n == 0) return GoodIndex;

  leadMuIdx = LeadingIdx(Mu);

  if(Mu.pt[leadMuIdx] < 52.)  /* HLT_Mu50_OR_HLT_TkMu50 lower pt limit from SFDB*/
    return GoodIndex;
  const Float_t MaxEta = 2.4;
  const Float_t MinPt = 20.;
  if(abs(Mu.eta[leadMuIdx]) >= MaxEta){
    HCutFlow->FillS("LeadingMuOut");
    return GoodIndex;
  }
  GoodIndex.reserve(10);
  for (UInt_t i=0; i<*Mu.n;i++){
    if( Muon_highPtId[i] >=1 && Mu.pt[i]>MinPt && abs(Mu.eta[i])<MaxEta)
      GoodIndex.emplace_back(i);
  }

  SortByDescPt(GoodIndex,Mu);

  return GoodIndex;
}

Int_t PreSelector::LeadingIdx(const Leptons& l) {

  Int_t idx = -1;
  Double_t pt = -1.;

  for(uint i = 0; i < *l.n; ++i){
    if( l.pt[i] > pt ){
      idx = i;
      pt = l.pt[i];
    }
  }

  return idx;

}

std::vector<UInt_t> PreSelector::GetGoodElectron(const Electrons& El){
  const Float_t MaxEta = 2.5;

  Float_t MinPt;
  Float_t Delta = 5.f;

#if defined(Y2016)
  MinPt = 27. + Delta;
#elif defined(Y2017)
  MinPt = 35. + Delta;
#elif defined(Y2018)
  MinPt = 32. + Delta;
#endif

  Float_t AbsMinPt = 10.; // No SF available under threshold

  std::pair<double,double> etaGap = std::make_pair(1.4442,1.5660);

  std::vector<UInt_t> GoodIndex = {};
  if(!ElectronTest()) return GoodIndex;
  if(*El.n == 0) return GoodIndex; /*Photon Trigger*/

  leadElIdx = LeadingIdx(El);

  if( El.pt[leadElIdx] < MinPt ) return GoodIndex;

  if(abs(El.eta[leadElIdx]) >= MaxEta){
    HCutFlow->FillS("LeadingElOut");
    return GoodIndex;
  }
  GoodIndex.reserve(10);

  std::pair<float,float> phiHEM = {-1.57,-0.87};
  std::pair<float,float> etaHEM = {-2.5,-1.3};

  UInt_t index = 0;
  for (UInt_t i = 0; i< *El.n; ++i){
    double abseta =  abs(El.eta[i]);
    if(El.pt[i] > AbsMinPt and
       El.cutBased[i]>=2 and
       abseta < MaxEta and
       ( abseta < etaGap.first or abseta > etaGap.second)
#if defined(Y2018)
       and !( El.eta[i] > etaHEM.first and El.eta[i] < etaHEM.second )
       and !( El.phi[i] > phiHEM.first and El.phi[i] < phiHEM.second )
#endif
       )
      GoodIndex.emplace_back(i);
  }

  if(GoodIndex.size() == 0) return GoodIndex;
  if(GoodIndex.size() > 1) SortByDescPt(GoodIndex,El);

#if !defined(CMSDATA) && defined(ULSAMPLE)
  const int id = El.cutBased[GoodIndex[0]];

#if defined(Y2017) || defined(Y2018)
  if (id == 2) {
    SFElectronHLT= SFElectronHLTLoose;
  } else if (id == 3) {
    SFElectronHLT= SFElectronHLTMedium;
  } else if (id == 4) {
    SFElectronHLT= SFElectronHLTTight;
  } else {
    assert(true);
  }
#elif defined(Y2016)
  if (IsPreVFP){
    SFElectronReco = SFElectronRecopreVFP;
    SFElectronRecoB20 = SFElectronRecopreVFPB20;
    SFElectronLooseID = SFElectronLooseIDpreVFP;
    SFElectronMediumID = SFElectronMediumIDpreVFP;
    SFElectronTightID = SFElectronTightIDpreVFP;
    if ( id == 2 ) {
      SFElectronHLT = SFElectronHLTLoosepreVFP;
    } else if (id == 3){
      SFElectronHLT = SFElectronHLTMediumpreVFP;
    } else if (id == 4){
      SFElectronHLT = SFElectronHLTTightpreVFP;
    } else {
      assert(false);
    }
  } else {
    SFElectronReco = SFElectronRecopostVFP;
    SFElectronRecoB20 = SFElectronRecopostVFPB20;
    SFElectronLooseID = SFElectronLooseIDpostVFP;
    SFElectronMediumID = SFElectronMediumIDpostVFP;
    SFElectronTightID = SFElectronTightIDpostVFP;
    if ( id == 2 ) {
      SFElectronHLT = SFElectronHLTLoosepostVFP;
    } else if (id == 3){
      SFElectronHLT = SFElectronHLTMediumpostVFP;
    } else if (id == 4){
      SFElectronHLT = SFElectronHLTTightpostVFP;
    } else {
      assert(false);
    }
  }
#endif
#endif //ULSAMPLE

  return GoodIndex;
};

float PreSelector::MassRecoZ(const float& pt1, const float& eta1, const float& phi1, const float& m1,
                             const float& pt2, const float& eta2, const float& phi2, const float& m2) const{

  ROOT::Math::PtEtaPhiMVector l1(pt1, eta1, phi1, m1);
  ROOT::Math::PtEtaPhiMVector l2(pt2, eta2, phi2, m2);

  return (l1+l2).M();
};

Float_t PreSelector::MassRecoW(const ROOT::Math::PtEtaPhiMVector& lep){

  return PreSelector::MassRecoW(lep.Pt(),lep.Phi(),*MET_pt,*MET_phi);

};


Float_t PreSelector::MassRecoW(const float& ptl, const float& phil,
                             const float& ptmet, const float& phimet) const{
  return TMath::Sqrt(2.*ptl*ptmet*(1-TMath::Cos(phil-phimet)));
};

std::vector<std::pair<UInt_t,UInt_t>> PreSelector::GetElectronPermutations(const Electrons& el) const{

  // Ignore opposite charge requirement for electrons

  std::vector<std::pair<UInt_t,UInt_t>> perms;

  for(uint i = 0; i < GoodElectron.size(); ++i){
    for(uint j = i+1; j < GoodElectron.size(); ++j){
      perms.push_back(std::make_pair(GoodElectron[i],GoodElectron[j]));
    }
  }

  return perms;

};


std::vector<std::pair<UInt_t,UInt_t>> PreSelector::GetMuonPairs(const Muons& m) const{

  // return leading pair sorted by Pt

  std::vector<std::pair<UInt_t,UInt_t>> pairs;

  for(uint i = 0; i < GoodMuon.size(); ++i){
    for(uint j = i+1; j < GoodMuon.size(); ++j){
      if (m.charge[i] != m.charge[j])
        pairs.push_back(std::make_pair(GoodMuon[i],GoodMuon[j]));
    }
  }

  return pairs;

};


ZPairInfo PreSelector::FindZ(const Electrons& el) const{
  return FindZ(PreSelector::GetElectronPermutations(el), el, GoodElectron);
}

ZPairInfo PreSelector::FindZ(const Muons& mu) const{
  return FindZ(PreSelector::GetMuonPairs(mu), mu, GoodMuon);
}

ZPairInfo PreSelector::FindZ(const std::vector<std::pair<UInt_t,UInt_t>>& Pairs,
                             const Leptons& l,
                             const std::vector<UInt_t>& GoodLepton) const{

  ZPairInfo z1;

  if(Pairs.size() == 0)
    return z1;

  std::vector<ZPairInfo> PairsWithMass;

  const Double_t ZNominalMass = 91.1876;

  for(const auto& pair: Pairs){
    UInt_t i = pair.first;
    UInt_t j = pair.second;
    Float_t m = PreSelector::MassRecoZ(l.pt[i],l.eta[i],l.phi[i],l.mass,
                                       l.pt[j],l.eta[j],l.phi[j],l.mass);
    z1.Delta = abs(ZNominalMass-m);
    z1.Mass = m;
    z1.Pair = pair;
    PairsWithMass.emplace_back(z1);
  }

  if(PairsWithMass.size() > 1) {
    auto sortAscByDelta = [&] (const ZPairInfo& p1, const ZPairInfo& p2) {
      return p1.Delta < p2.Delta;
    };
    std::sort(PairsWithMass.begin(),PairsWithMass.end(),
              sortAscByDelta);
  }

  return PairsWithMass[0]; /*Pair with smallest delta*/

}


Nu4VObj PreSelector::GetNu4VFix(const ROOT::Math::PtEtaPhiMVector& lep){

  Nu4VObj s;
  Float_t pz = (lep.Pz()/lep.Pt())*(*MET_pt);
  s.Met = Get4V(pz,*MET_pt,*MET_phi);

#if !defined(CMSDATA)
  Float_t pz_up = (lep.Pz()/lep.Pt())*(MetUncl.PtUp);
  Float_t pz_down = (lep.Pz()/lep.Pt())*(MetUncl.PtDown);

  s.MetUnclUp = Get4V(pz_up,MetUncl.PtUp,MetUncl.PhiUp);
  s.MetUnclDown = Get4V(pz_down,MetUncl.PtDown,MetUncl.PhiDown);
#endif

  return s;
}

Nu4VObj PreSelector::GetNu4V(const ROOT::Math::PtEtaPhiMVector& lep){

  Nu4VObj s;
  Float_t NuPz, NuPz_up, NuPz_down = 0.;
  const Float_t Mw = 80.379;
  Float_t a = 2.*lep.Pt()*(*MET_pt);
  Float_t k = Mw*Mw - wmt.Met*wmt.Met;
  Float_t b = (k + a);
  Float_t c = k*(k + 4.*lep.Pt()*(*MET_pt));

  if ( c < 0 ) return GetNu4VFix(lep);

  Float_t d = lep.P()*TMath::Sqrt(c);

  NuPz = (lep.Pz()*b - d)/(2.*lep.Pt()*lep.Pt()); // What About the Positive Solution?

  s.Met = Get4V(NuPz,*MET_pt,*MET_phi);


#if !defined(CMSDATA)
  Float_t a_up = 2.*lep.Pt()*(MetUncl.PtUp);
  Float_t k_up = Mw*Mw - wmt.MetUnclUp*wmt.MetUnclUp;
  Float_t b_up = (k_up + a_up);
  Float_t c_up = k_up*(k_up + 4.*lep.Pt()*(MetUncl.PtUp));

  Float_t a_down = 2.*lep.Pt()*(MetUncl.PtDown);
  Float_t k_down = Mw*Mw - wmt.MetUnclDown*wmt.MetUnclDown;
  Float_t b_down = (k_down + a_down);
  Float_t c_down = k_down*(k_down + 4.*lep.Pt()*(MetUncl.PtDown));

  if ( c < 0 or c_down < 0 or c_up < 0 ) return GetNu4VFix(lep);

  Float_t d_up = lep.P()*TMath::Sqrt(c_up);
  Float_t d_down = lep.P()*TMath::Sqrt(c_down);

  NuPz_up = (lep.Pz()*b_up - d_up)/(2.*lep.Pt()*lep.Pt());
  NuPz_down = (lep.Pz()*b_down - d_down)/(2.*lep.Pt()*lep.Pt());

  s.MetUnclUp = Get4V(NuPz_up,MetUncl.PtUp,MetUncl.PhiUp);
  s.MetUnclDown = Get4V(NuPz_down,MetUncl.PtDown,MetUncl.PhiDown);
#endif

  return s;

}

ROOT::Math::PxPyPzMVector PreSelector::Get4V(const Float_t& pz, const Float_t& pt, const Float_t phi){

  const Float_t MNu = 0.;

  return ROOT::Math::PxPyPzMVector((pt)*TMath::Cos(phi),
                                   (pt)*TMath::Sin(phi),
                                   pz,MNu);

}



Float_t PreSelector::GetEtaPhiDistance(const float& eta1, const float& phi1,
                                        const float& eta2, const float& phi2) const{
  Double_t dphi = TVector2::Phi_mpi_pi(phi1-phi2);
  return sqrt(pow(eta2-eta1,2.)+pow(dphi,2.));
}

void PreSelector::FillH1(std::vector<TH1F*>& h1, const Int_t& nh, const Double_t& binContent){

  enum {
    kNoSf = 0,
    kWCentral = 4,
    kWAllUp = 8,
    kWAllDown = 12,
  };

#if defined(CMSDATA)
  assert( WCentral == 1.);
  h1[nh+kNoSf]->Fill(binContent);
#else
  h1[nh+kNoSf]->Fill(binContent,*genWeight);
  h1[nh+kWCentral]->Fill(binContent,WCentral);
  h1[nh+kWAllUp]->Fill(binContent,WAllUp);
  h1[nh+kWAllDown]->Fill(binContent,WAllDown);
#endif

}

void PreSelector::FillCategory(const Int_t& crOffset, const Leptons& lz,const Leptons& lw){

  assert(IsA_ xor IsB xor IsC xor IsD);

#if !defined(CMSDATA)
  Float_t genW = *genWeight;
#else
  Float_t genW = 1.;
#endif

  WCentral  = 1.;
  Int_t nch = -1;

  if(IsD){
    nch=3;
  } else if (IsC) {
    nch=2;
  } else if (IsB) {
    nch=1;
  } else if (IsA_) {
    nch=0;
  }

  Int_t nh = nch + crOffset;

  // dR Histos
  Double_t wzdist = GetEtaPhiDistance(wb.Met.Eta(),wb.Met.Phi(),zb.Eta(),zb.Phi());
  Double_t l1l2dist = GetEtaPhiDistance(lep1.Eta(),lep1.Phi(),lep2.Eta(),lep2.Phi());
  Double_t l1l3dist = GetEtaPhiDistance(lep1.Eta(),lep1.Phi(),lep3.Eta(),lep3.Phi());
  Double_t l2l3dist = GetEtaPhiDistance(lep2.Eta(),lep2.Phi(),lep3.Eta(),lep3.Phi());

  // 2DHistos
  HWZPtDist[nh]->Fill((wb.Met+zb).Pt(),wzdist);
  HPtEtal1[nh]->Fill(lep1.Pt(),lep1.Eta());
  HPtEtal2[nh]->Fill(lep2.Pt(),lep2.Eta());
  HPtEtal3[nh]->Fill(lep3.Pt(),lep3.Eta());
  HMassZTW[nh]->Fill(zb.M(),wmt.Met);
  HDeltaRPtZ[nh]->Fill(l1l2dist,zb.Pt());
  HPtWPtZ[nh]->Fill(wb.Met.Pt(),zb.Pt());
  HDeltaRMWZ[nh]->Fill(l1l2dist,(wb.Met+zb).M());
  HLtMWZ[nh]->Fill(lt,(wb.Met+zb).M());
  HNLep[nh]->Fill(GoodMuon.size(),GoodElectron.size());
  HMassZWZ[nh]->Fill(PairZMass,(wb.Met+zb).M());
  HEtaPhil1[nh]->Fill(lep1.Eta(),lep1.Phi());
  HEtaPhil2[nh]->Fill(lep2.Eta(),lep2.Phi());
  HEtaPhil3[nh]->Fill(lep3.Eta(),lep3.Phi());
  HPtl1l2[nh]->Fill(lep1.Pt(),lep2.Pt());
  HPtl1l3[nh]->Fill(lep1.Pt(),lep3.Pt());
  HDxyl1l2[nh]->Fill(lz.dxy[l1],lz.dxy[l2]);

  if(IsA_ or IsB) { // PairEl
    HZElId[nh]->Fill(Electron_cutBased[l1],Electron_cutBased[l2]);
  }

  HCutFlow->FillS(Form("%d",nh));

#ifndef CMSDATA

  DefineSFs(nh);

  HGenPartChgF[nh]->Fill(lz.charge[l1],GenPart_pdgId[lz.genPartIdx[l1]]);
  HGenPartChgF[nh]->Fill(lz.charge[l2],GenPart_pdgId[lz.genPartIdx[l2]]);

  std::pair<Int_t,Int_t> motherl1 = GetMother(lz.genPartIdx[l1],lz.pdgId[l1]);
  std::pair<Int_t,Int_t> motherl2 = GetMother(lz.genPartIdx[l2],lz.pdgId[l2]);
  std::pair<Int_t,Int_t> motherl3 = GetMother(lw.genPartIdx[l3],lw.pdgId[l3]);

  HGenPartPdgIdl1[nh]->Fill(lz.pdgId[l1]);
  HGenPartPdgIdl2[nh]->Fill(lz.pdgId[l2]);
  HGenPartPdgIdl3[nh]->Fill(lw.pdgId[l3]);
  HGenPartZ[nh]->Fill( motherl1.second, lz.pdgId[l1] );
  HGenPartZ[nh]->Fill( motherl2.second, lz.pdgId[l2] );
  HGenPartW[nh]->Fill( motherl3.second, lw.pdgId[l3] );
  HGenPartZWp[nh]->Fill( motherl1.second, GetMother(motherl1).second );
  HGenPartZWp[nh]->Fill( motherl2.second, GetMother(motherl2).second );
  HGenPartWWp[nh]->Fill( motherl3.second, GetMother(motherl3).second );
#endif

  // Eta histos
  FillH1(HPileup_,nh,(double)*PV_npvs);
  FillH1(HEtal1,nh,lep1.Eta());
  FillH1(HEtal2,nh,lep2.Eta());
  FillH1(HEtal3,nh,lep3.Eta());

  // Pt histos
  FillH1(HPtLeading,nh, leadingLepton != nullptr? leadingLepton->Pt(): 999.);
  FillH1(HPtl1,nh,lep1.Pt());
  FillH1(HPtl2,nh,lep2.Pt());
  FillH1(HPtl3,nh,lep3.Pt());
  FillH1(HMetPt,nh,*MET_pt);
#ifndef CMSDATA
  FillH1(HMetUnclUpPt,nh,MetUncl.PtUp);
  FillH1(HMetUnclDownPt,nh,MetUncl.PtDown);
#endif
  FillH1(HZPt,nh,zb.Pt());
  FillH1(HWPt,nh,wb.Met.Pt());
  FillH1(HWZPt,nh,(wb.Met+zb).Pt());
  FillH1(HLt,nh,lt);

  FillH1(HPtl1Lt,nh,lep1.Pt()/lt);
  FillH1(HPtl2Lt,nh,lep2.Pt()/lt);
  FillH1(HPtl3Lt,nh,lep3.Pt()/lt);
  FillH1(HMetPtLt,nh,*MET_pt/lt);
  FillH1(HPtl3Met,nh,*MET_pt/lep3.Pt());

  FillH1(HCosl3Met,nh,TMath::Cos(lep3.Phi()-*MET_phi));

  // n Histos
  FillH1(HnEl,nh,(double)GoodElectron.size());
  FillH1(HnMu,nh,(double)GoodMuon.size());
  FillH1(HnJet,nh,(double)*nJet);

  // Mass Histos
  double wzm_Met = (wb.Met+zb).M();
#if !defined(CMSDATA)
  double wzm_MetUnclUp = (wb.MetUnclUp+zb).M();
  double wzm_MetUnclDown = (wb.MetUnclDown+zb).M();
#endif
  FillH1(HMassW,nh,wb.Met.M());
  FillH1(HMassZ,nh,PairZMass);
  FillH1(HMassTW,nh,wmt.Met);
  FillH1(HMassWZ,nh,wzm_Met);
  FillH1(HPtZMWZ,nh,zb.Pt()/wzm_Met);
  FillH1(HPtWMWZ,nh,wb.Met.Pt()/wzm_Met);

  if(IsA_){
    HIDl1l2[nh]->Fill("El1",GetElIDString(Electron_cutBased[l1]).c_str(),1.);
    HIDl1l2[nh]->Fill("El2",GetElIDString(Electron_cutBased[l2]).c_str(),1.);
    HIDl1l2[nh]->Fill("El3",GetElIDString(Electron_cutBased[l3]).c_str(),1.);
    FillH1(HElPt,nh,lep1.Pt());
    FillH1(HElPt,nh,lep2.Pt());
    FillH1(HElPt,nh,lep3.Pt());
    FillH1(HElEta,nh,lep1.Eta());
    FillH1(HElEta,nh,lep2.Eta());
    FillH1(HElEta,nh,lep3.Eta());
    FillH1(HElPhi,nh,lep1.Phi());
    FillH1(HElPhi,nh,lep2.Phi());
    FillH1(HElPhi,nh,lep3.Phi());
#ifndef CMSDATA
    if(crOffset==0){ // SR1
      HMassWZ[HIdx["SR1_A_ElReco_Up"]]->Fill(wzm_Met,WElRecoUp);
      HMassWZ[HIdx["SR1_A_ElReco_Down"]]->Fill(wzm_Met,WElRecoDown);
      HMassWZ[HIdx["SR1_A_ElTrigger_Up"]]->Fill(wzm_Met,WElTrigUp);
      HMassWZ[HIdx["SR1_A_ElTrigger_Down"]]->Fill(wzm_Met,WElTrigDown);
      HMassWZ[HIdx["SR1_A_ElID_Up"]]->Fill(wzm_Met,WElIDUp);
      HMassWZ[HIdx["SR1_A_ElID_Down"]]->Fill(wzm_Met,WElIDDown);
      HMassWZ[HIdx["SR1_A_MetUncl_Up"]]->Fill(wzm_MetUnclUp);
      HMassWZ[HIdx["SR1_A_MetUncl_Down"]]->Fill(wzm_MetUnclDown);
      HMassWZ[HIdx["SR1_A_Pileup_Up"]]->Fill(wzm_Met,WPileupUp);
      HMassWZ[HIdx["SR1_A_Pileup_Down"]]->Fill(wzm_Met,WPileupDown);
#if defined(Y2016) || defined(Y2017)
      HMassWZ[HIdx["SR1_A_L1Pref_Up"]]->Fill(wzm_Met,(*L1PreFiringWeight_ECAL_Up)*(*genWeight));
      HMassWZ[HIdx["SR1_A_L1Pref_Down"]]->Fill(wzm_Met,(*L1PreFiringWeight_ECAL_Dn)*(*genWeight));
#endif
      if(ApplyKFactors){
        HMassWZ[HIdx["SR1_A_KFactor_EWK_Up"]]->Fill(wzm_Met,WKEWKUp);
        HMassWZ[HIdx["SR1_A_KFactor_EWK_Down"]]->Fill(wzm_Met,WKEWKDown);
        HMassWZ[HIdx["SR1_A_KFactor_QCD_Up"]]->Fill(wzm_Met,WKQCDUp);
        HMassWZ[HIdx["SR1_A_KFactor_QCD_Down"]]->Fill(wzm_Met,WKQCDDown);
      }
    }
    HFakeString[nh]->FillS((GetFakeString(Electron_genPartIdx[l1],ElPdgId,Electron_cutBased[l1])).c_str());
    HFakeString[nh]->FillS((GetFakeString(Electron_genPartIdx[l2],ElPdgId,Electron_cutBased[l2])).c_str());
    HFakeString[nh]->FillS((GetFakeString(Electron_genPartIdx[l3],ElPdgId,Electron_cutBased[l3])).c_str());
#endif
  } else if (IsB) {
    HIDl1l2[nh]->Fill("El1",GetElIDString(Electron_cutBased[l1]).c_str(),1.);
    HIDl1l2[nh]->Fill("El2",GetElIDString(Electron_cutBased[l2]).c_str(),1.);
    HIDl1l2[nh]->Fill("Mu1",GetMuIDString(Muon_highPtId[l3]).c_str(),1.);
    HMuonPF[nh]->FillS(GetMuonTypeString(l3).c_str());
    HMuonPtDiff[nh]->Fill(GetMuonPtDiff(l3),genW);
    HMuonPtType[nh]->Fill(GetMuonTypeString(l3).c_str(),GetMuonPtDiff(l3),1.);
    FillH1(HElPt,nh,lep1.Pt());
    FillH1(HElPt,nh,lep2.Pt());
    FillH1(HMuPt,nh,lep3.Pt());
    FillH1(HElEta,nh,lep1.Eta());
    FillH1(HElEta,nh,lep2.Eta());
    FillH1(HMuEta,nh,lep3.Eta());
    FillH1(HElPhi,nh,lep1.Phi());
    FillH1(HElPhi,nh,lep2.Phi());
    FillH1(HMuPhi,nh,lep3.Phi());
#ifndef CMSDATA
    if(crOffset==0){ // SR1
      HMassWZ[HIdx["SR1_B_ElReco_Up"]]->Fill(wzm_Met,WElRecoUp);
      HMassWZ[HIdx["SR1_B_ElReco_Down"]]->Fill(wzm_Met,WElRecoDown);
      HMassWZ[HIdx["SR1_B_ElTrigger_Up"]]->Fill(wzm_Met,WElTrigUp);
      HMassWZ[HIdx["SR1_B_ElTrigger_Down"]]->Fill(wzm_Met,WElTrigDown);
      HMassWZ[HIdx["SR1_B_MuTrigger_Up"]]->Fill(wzm_Met,WMuTrigUp);
      HMassWZ[HIdx["SR1_B_MuTrigger_Down"]]->Fill(wzm_Met,WMuTrigDown);
      HMassWZ[HIdx["SR1_B_ElID_Up"]]->Fill(wzm_Met,WElIDUp);
      HMassWZ[HIdx["SR1_B_ElID_Down"]]->Fill(wzm_Met,WElIDDown);
      HMassWZ[HIdx["SR1_B_MuID_Up"]]->Fill(wzm_Met,WMuIDUp);
      HMassWZ[HIdx["SR1_B_MuID_Down"]]->Fill(wzm_Met,WMuIDDown);
      HMassWZ[HIdx["SR1_B_MetUncl_Up"]]->Fill(wzm_MetUnclUp);
      HMassWZ[HIdx["SR1_B_MetUncl_Down"]]->Fill(wzm_MetUnclDown);
      HMassWZ[HIdx["SR1_B_Pileup_Up"]]->Fill(wzm_Met,WPileupUp);
      HMassWZ[HIdx["SR1_B_Pileup_Down"]]->Fill(wzm_Met,WPileupDown);
#if defined(Y2016) || defined(Y2017)
      HMassWZ[HIdx["SR1_B_L1Pref_Up"]]->Fill(wzm_Met,(*L1PreFiringWeight_Up)*(*genWeight));
      HMassWZ[HIdx["SR1_B_L1Pref_Down"]]->Fill(wzm_Met,(*L1PreFiringWeight_Dn)*(*genWeight));
#endif
      if(ApplyKFactors){
        HMassWZ[HIdx["SR1_B_KFactor_EWK_Up"]]->Fill(wzm_Met,WKEWKUp);
        HMassWZ[HIdx["SR1_B_KFactor_EWK_Down"]]->Fill(wzm_Met,WKEWKDown);
        HMassWZ[HIdx["SR1_B_KFactor_QCD_Up"]]->Fill(wzm_Met,WKQCDUp);
        HMassWZ[HIdx["SR1_B_KFactor_QCD_Down"]]->Fill(wzm_Met,WKQCDDown);
      }
    }
    HFakeString[nh]->FillS((GetFakeString(Electron_genPartIdx[l1],ElPdgId,Electron_cutBased[l1])).c_str());
    HFakeString[nh]->FillS((GetFakeString(Electron_genPartIdx[l2],ElPdgId,Electron_cutBased[l2])).c_str());
    HFakeString[nh]->FillS((GetFakeString(Muon_genPartIdx[l3],MuPdgId,Muon_highPtId[l3])).c_str());
#endif

  } else if (IsC) {
    HIDl1l2[nh]->Fill("Mu1",GetMuIDString(Muon_highPtId[l1]).c_str(),1.);
    HIDl1l2[nh]->Fill("Mu2",GetMuIDString(Muon_highPtId[l2]).c_str(),1.);
    HIDl1l2[nh]->Fill("El1",GetElIDString(Electron_cutBased[l3]).c_str(),1.);
    HMuonPF[nh]->FillS(GetMuonTypeString(l1).c_str());
    HMuonPF[nh]->FillS(GetMuonTypeString(l2).c_str());
    HMuonPtDiff[nh]->Fill(GetMuonPtDiff(l1),genW);
    HMuonPtDiff[nh]->Fill(GetMuonPtDiff(l2),genW);
    HMuonPtType[nh]->Fill(GetMuonTypeString(l1).c_str(),GetMuonPtDiff(l1),1.);
    HMuonPtType[nh]->Fill(GetMuonTypeString(l2).c_str(),GetMuonPtDiff(l2),1.);
    FillH1(HMuPt,nh,lep1.Pt());
    FillH1(HMuPt,nh,lep2.Pt());
    FillH1(HElPt,nh,lep3.Pt());
    FillH1(HMuEta,nh,lep1.Eta());
    FillH1(HMuEta,nh,lep2.Eta());
    FillH1(HElEta,nh,lep3.Eta());
    FillH1(HMuPhi,nh,lep1.Phi());
    FillH1(HMuPhi,nh,lep2.Phi());
    FillH1(HElPhi,nh,lep3.Phi());
#ifndef CMSDATA
    if(crOffset==0){ // SR1
      HMassWZ[HIdx["SR1_C_ElReco_Up"]]->Fill(wzm_Met,WElRecoUp);
      HMassWZ[HIdx["SR1_C_ElReco_Down"]]->Fill(wzm_Met,WElRecoDown);
      HMassWZ[HIdx["SR1_C_ElTrigger_Up"]]->Fill(wzm_Met,WElTrigUp);
      HMassWZ[HIdx["SR1_C_ElTrigger_Down"]]->Fill(wzm_Met,WElTrigDown);
      HMassWZ[HIdx["SR1_C_MuTrigger_Up"]]->Fill(wzm_Met,WMuTrigUp);
      HMassWZ[HIdx["SR1_C_MuTrigger_Down"]]->Fill(wzm_Met,WMuTrigDown);
      HMassWZ[HIdx["SR1_C_ElID_Up"]]->Fill(wzm_Met,WElIDUp);
      HMassWZ[HIdx["SR1_C_ElID_Down"]]->Fill(wzm_Met,WElIDDown);
      HMassWZ[HIdx["SR1_C_MuID_Up"]]->Fill(wzm_Met,WMuIDUp);
      HMassWZ[HIdx["SR1_C_MuID_Down"]]->Fill(wzm_Met,WMuIDDown);
      HMassWZ[HIdx["SR1_C_MetUncl_Up"]]->Fill(wzm_MetUnclUp);
      HMassWZ[HIdx["SR1_C_MetUncl_Down"]]->Fill(wzm_MetUnclDown);
      HMassWZ[HIdx["SR1_C_Pileup_Up"]]->Fill(wzm_Met,WPileupUp);
      HMassWZ[HIdx["SR1_C_Pileup_Down"]]->Fill(wzm_Met,WPileupDown);
#if defined(Y2016) || defined(Y2017)
      HMassWZ[HIdx["SR1_C_L1Pref_Up"]]->Fill(wzm_Met,(*L1PreFiringWeight_Up)*(*genWeight));
      HMassWZ[HIdx["SR1_C_L1Pref_Down"]]->Fill(wzm_Met,(*L1PreFiringWeight_Dn)*(*genWeight));
#endif
      if(ApplyKFactors){
        HMassWZ[HIdx["SR1_C_KFactor_EWK_Up"]]->Fill(wzm_Met,WKEWKUp);
        HMassWZ[HIdx["SR1_C_KFactor_EWK_Down"]]->Fill(wzm_Met,WKEWKDown);
        HMassWZ[HIdx["SR1_C_KFactor_QCD_Up"]]->Fill(wzm_Met,WKQCDUp);
        HMassWZ[HIdx["SR1_C_KFactor_QCD_Down"]]->Fill(wzm_Met,WKQCDDown);
      }
    }
    HFakeString[nh]->FillS((GetFakeString(Muon_genPartIdx[l1],MuPdgId,Muon_highPtId[l1])).c_str());
    HFakeString[nh]->FillS((GetFakeString(Muon_genPartIdx[l2],MuPdgId,Muon_highPtId[l2])).c_str());
    HFakeString[nh]->FillS((GetFakeString(Electron_genPartIdx[l3],ElPdgId,Electron_cutBased[l3])).c_str());
#endif
  } else if (IsD) {
    HIDl1l2[nh]->Fill("Mu1",GetMuIDString(Muon_highPtId[l1]).c_str(),1.);
    HIDl1l2[nh]->Fill("Mu2",GetMuIDString(Muon_highPtId[l2]).c_str(),1.);
    HIDl1l2[nh]->Fill("Mu3",GetMuIDString(Muon_highPtId[l3]).c_str(),1.);
    HMuonPF[nh]->FillS(GetMuonTypeString(l1).c_str());
    HMuonPF[nh]->FillS(GetMuonTypeString(l2).c_str());
    HMuonPF[nh]->FillS(GetMuonTypeString(l3).c_str());
    HMuonPtDiff[nh]->Fill(GetMuonPtDiff(l1),genW);
    HMuonPtDiff[nh]->Fill(GetMuonPtDiff(l2),genW);
    HMuonPtDiff[nh]->Fill(GetMuonPtDiff(l3),genW);
    HMuonPtType[nh]->Fill(GetMuonTypeString(l1).c_str(),GetMuonPtDiff(l1),1.);
    HMuonPtType[nh]->Fill(GetMuonTypeString(l2).c_str(),GetMuonPtDiff(l2),1.);
    HMuonPtType[nh]->Fill(GetMuonTypeString(l3).c_str(),GetMuonPtDiff(l3),1.);
    FillH1(HMuPt,nh,lep1.Pt());
    FillH1(HMuPt,nh,lep2.Pt());
    FillH1(HMuPt,nh,lep3.Pt());
    FillH1(HMuEta,nh,lep1.Eta());
    FillH1(HMuEta,nh,lep2.Eta());
    FillH1(HMuEta,nh,lep3.Eta());
    FillH1(HMuPhi,nh,lep1.Phi());
    FillH1(HMuPhi,nh,lep2.Phi());
    FillH1(HMuPhi,nh,lep3.Phi());
#ifndef CMSDATA
    if(crOffset==0){ // SR1
      HMassWZ[HIdx["SR1_D_MuTrigger_Up"]]->Fill(wzm_Met,WMuTrigUp);
      HMassWZ[HIdx["SR1_D_MuTrigger_Down"]]->Fill(wzm_Met,WMuTrigDown);
      HMassWZ[HIdx["SR1_D_MuID_Up"]]->Fill(wzm_Met,WMuIDUp);
      HMassWZ[HIdx["SR1_D_MuID_Down"]]->Fill(wzm_Met,WMuIDDown);
      HMassWZ[HIdx["SR1_D_MetUncl_Up"]]->Fill(wzm_MetUnclUp);
      HMassWZ[HIdx["SR1_D_MetUncl_Down"]]->Fill(wzm_MetUnclDown);
      HMassWZ[HIdx["SR1_D_Pileup_Up"]]->Fill(wzm_Met,WPileupUp);
      HMassWZ[HIdx["SR1_D_Pileup_Down"]]->Fill(wzm_Met,WPileupDown);
#if defined(Y2016) || defined(Y2017)
      HMassWZ[HIdx["SR1_D_L1Pref_Up"]]->Fill(wzm_Met,(*L1PreFiringWeight_Muon_Up)*(*genWeight));
      HMassWZ[HIdx["SR1_D_L1Pref_Down"]]->Fill(wzm_Met,(*L1PreFiringWeight_Muon_Dn)*(*genWeight));
#endif
      if(ApplyKFactors){
        HMassWZ[HIdx["SR1_D_KFactor_EWK_Up"]]->Fill(wzm_Met,WKEWKUp);
        HMassWZ[HIdx["SR1_D_KFactor_EWK_Down"]]->Fill(wzm_Met,WKEWKDown);
        HMassWZ[HIdx["SR1_D_KFactor_QCD_Up"]]->Fill(wzm_Met,WKQCDUp);
        HMassWZ[HIdx["SR1_D_KFactor_QCD_Down"]]->Fill(wzm_Met,WKQCDDown);
      }
    }
    HFakeString[nh]->FillS((GetFakeString(Muon_genPartIdx[l1],MuPdgId,Muon_highPtId[l1])).c_str());
    HFakeString[nh]->FillS((GetFakeString(Muon_genPartIdx[l2],MuPdgId,Muon_highPtId[l2])).c_str());
    HFakeString[nh]->FillS((GetFakeString(Muon_genPartIdx[l3],MuPdgId,Muon_highPtId[l3])).c_str());
#endif
  }

  // dR Histos
  FillH1(HWZDist,nh,wzdist);
  FillH1(HDistl1l2,nh,l1l2dist);
  FillH1(HDistl1l3,nh,l1l3dist);
  FillH1(HDistl2l3,nh,l2l3dist);
  FillH1(HDistZl3,nh,GetEtaPhiDistance(zb.Eta(),zb.Phi(),lep3.Eta(),lep3.Phi()));
  FillH1(HDistZW,nh,GetEtaPhiDistance(zb.Eta(),zb.Phi(),wb.Met.Eta(),wb.Met.Phi()));

  // Phi Histos
  FillH1(HPhil1,nh,lep1.Phi());
  FillH1(HPhil2,nh,lep2.Phi());
  FillH1(HPhil3,nh,lep3.Phi());
  FillH1(HMetPhi,nh,*MET_phi);
#if !defined (CMSDATA)
  FillH1(HMetUnclUpPhi,nh,MetUncl.PhiUp);
  FillH1(HMetUnclDownPhi,nh,MetUncl.PhiDown);
#endif

  // Dxyz RelIso
  FillH1(HDxyl1,nh,lz.dxy[l1]);
  FillH1(HDxyl2,nh,lz.dxy[l2]);
  FillH1(HDxyl3,nh,lw.dxy[l3]);
  FillH1(HDzl1,nh,lz.dz[l1]);
  FillH1(HDzl2,nh,lz.dz[l2]);
  FillH1(HDzl3,nh,lw.dz[l3]);
  FillH1(HIP3Dl1,nh,lz.ip3d[l1]);
  FillH1(HIP3Dl2,nh,lz.ip3d[l2]);
  FillH1(HIP3Dl3,nh,lw.ip3d[l3]);
  FillH1(HSIP3Dl1,nh,lz.sip3d[l1]);
  FillH1(HSIP3Dl2,nh,lz.sip3d[l2]);
  FillH1(HSIP3Dl3,nh,lw.sip3d[l3]);
  FillH1(HRelIsol1,nh,lz.relIso[l1]);
  FillH1(HRelIsol2,nh,lz.relIso[l2]);
  FillH1(HRelIsol3,nh,lz.relIso[l3]);
}


bool PreSelector::DefineW(const Leptons& l){

  lep3 = PtEtaPhiMVector(l.pt[l3], l.eta[l3], l.phi[l3], l.mass);
  if(lep3.Pt() < 30) {
    l3 = -1; /* safeguard */
    HCutFlow->FillS("Wlep<30");
    return false;
  }
  if(GetEtaPhiDistance(lep2.Eta(),lep2.Phi(),lep3.Eta(),lep3.Phi()) < 1.0 ){
    l3 = -1;
    HCutFlow->FillS("Distl2l3<1.");
    return false;
  }
  if(GetEtaPhiDistance(lep1.Eta(),lep1.Phi(),lep3.Eta(),lep3.Phi()) < 1.0 ){
    l3 = -1;
    HCutFlow->FillS("Distl1l2<1.");
    return false;
  }

  wmt.Met = PreSelector::MassRecoW(lep3.Pt(), lep3.Phi(), *MET_pt, *MET_phi);
  nu = PreSelector::GetNu4V(lep3);
  wb.Met = (lep3 + nu.Met);

#if !defined(CMSDATA)
  wmt.MetUnclUp = PreSelector::MassRecoW(lep3.Pt(), lep3.Phi(), MetUncl.PtUp, MetUncl.PhiUp);
  wmt.MetUnclDown = PreSelector::MassRecoW(lep3.Pt(), lep3.Phi(), MetUncl.PtDown, MetUncl.PtDown);
  wb.MetUnclUp = (lep3 + nu.MetUnclUp);
  wb.MetUnclDown = (lep3 + nu.MetUnclDown);
#endif

  return true;

}

Bool_t PreSelector::CheckElectronPair(const std::pair<UInt_t,UInt_t>& p) const{

  Float_t LeadingMinPt = 50.;

  if (Electron_pt[p.first] < LeadingMinPt)
    return kFALSE;
  return kTRUE;
}

Bool_t PreSelector::CheckMuonPair(const std::pair<UInt_t,UInt_t>& p) const{

  if (Muon_tunepRelPt[p.first]*Muon_pt[p.first] < 70.) return kFALSE;
  const Float_t farFromPV = 1e-2;
  if (Muon_ip3d[p.first] > farFromPV or Muon_ip3d[p.second] > farFromPV) return kFALSE;

  Bool_t GlobalHighPtl1 = (Muon_highPtId[p.first] == 2);
  Bool_t GlobalHighPtl2 = (Muon_highPtId[p.second] == 2);

  if (!(GlobalHighPtl1 or GlobalHighPtl2)) {
    HCutFlow->FillS("FailZGlbHighPt");
    return kFALSE;
  } else {
    if (GlobalHighPtl1 and !Muon_isPFcand[p.first]) {
      HCutFlow->FillS("FailZGlbHighPt&PFCand");
      return kFALSE;
    } else if (GlobalHighPtl2 and !Muon_isPFcand[p.second]) {
      HCutFlow->FillS("FailZGlbHighPt&PFCand");
      return kFALSE;
    }
  }
  return kTRUE;
}

void PreSelector::FillRegion(const int regOffset,
                             const Electrons& Els, const Muons& Mus){

  //3e0mu
  if(IsA_){
    FillCategory(regOffset,Els,Els);
  }
  // 2e1mu
  if(IsB){
    FillCategory(regOffset,Els,Mus);
  }
  // 1e2Mu
  if(IsC){
    FillCategory(regOffset,Mus,Els);
  }
  // 0e3mu
  if(IsD){
    FillCategory(regOffset,Mus,Mus);
  }
  // 3leptons
  HOverlap->Fill(IsA_+IsB+IsC+IsD);
}

Bool_t PreSelector::PairMuDefineW(const Electrons& Els, const Muons& Mus){

  auto WElectronOk = [&](){
    Bool_t ok{};
    for(const int& n: GoodElectron){
      if( Electron_cutBased[n] == 4 ) {
        l3 = n;
        ok = true;
        break;
      }
    }
    if (!ok) {
      l3 = -1;
      HCutFlow->FillS("WElNotTight");
    }
    return ok;
  };

  if ( SameFlvWCand.size()>0 and GoodElectron.size() == 0 ){
    //assert(SameFlvWCand.size()==1);
    l3 = SameFlvWCand[0];
    IsD = true;
  } else if( SameFlvWCand.size() > 0 and GoodElectron.size() > 0 ) {
    if( Muon_tunepRelPt[SameFlvWCand[0]]*Muon_pt[SameFlvWCand[0]] > Electron_pt[GoodElectron[0]] ){
      l3 = SameFlvWCand[0];
      IsD = true;
    } else if (WElectronOk()) {
      IsC = true;
    } else {
      return kFALSE;
    }
  } else if (SameFlvWCand.size() == 0 and GoodElectron.size() > 0) {
    if (WElectronOk()){
      IsC = true;
    } else {
      return kFALSE;
    }
  } else {
    assert(false);
  }

  assert (IsC xor IsD);

  if(IsC)
    return DefineW(Els);

  if(IsD)
    return DefineW(Mus);

  return kFALSE;

}

Bool_t PreSelector::PairElDefineW(const Electrons& Els, const Muons& Mus){

  auto WMuonOk = [&](){
    Bool_t ok{};
    for (const int& i: GoodMuon) {
      const Float_t minPt = 50.;
      const Float_t farFromPV = 1e-2;
      if(Muon_highPtId[i] == 2 and Muon_tunepRelPt[i]*Muon_pt[i] > minPt and Muon_ip3d[i] < farFromPV){
        l3 = i;
        ok = true;
        break;
      }
    }
    if (!ok) {
      l3 = -1;
      HCutFlow->FillS("FailWMuonGlbHighPtId");
    }
    return ok;
  };

  auto WElectronOk = [&](){
    Bool_t ok{};
    for(const int& n: GoodElectron){
      if( Electron_cutBased[n] == 4 ) {
        l3 = n;
        ok = true;
        break;
      }
    }
    if (!ok) {
      l3 = -1;
      HCutFlow->FillS("WElNotTight");
    }
    return ok;
  };

  if (SameFlvWCand.size() > 0 and GoodMuon.size() == 0) {
    if(WElectronOk()){
      IsA_ = true;
    } else {
      return kFALSE;
    }
  } else if (SameFlvWCand.size() > 0 and GoodMuon.size() > 0) {
    if( WMuonOk() and (Muon_tunepRelPt[l3]*Muon_pt[l3] > Electron_pt[SameFlvWCand[0]]) ){
      // l3 defined through WMuonOk
      IsB = true;
    } else if (WElectronOk()) {
      // l3 defined through WElectronOk
      IsA_ = true;
    } else {
      return kFALSE;
    }
  } else if( SameFlvWCand.size() == 0 and (GoodMuon.size()>0 and WMuonOk()) ) {
    // l3 defined through WMuonOk
    IsB = true;
  } else {
    assert(!IsA_);
    assert(!IsB);
    assert(l3 == -1);
    return kFALSE;
  }

  assert (IsA_ xor IsB);

  if (IsA_)
    return DefineW(Els);
  if (IsB)
    return DefineW(Mus);

  return kFALSE;

}

std::string PreSelector::GetMuIDString(UChar_t& id){

  std::string ids = "";

  if (id == 1) {
    ids = "trkHighPt";
  } else if(id == 2) {
    ids = "HighPt";
  } else {
    ids = "Mu_Error";
  }
  return ids.c_str();

}

std::string PreSelector::GetElIDString(Int_t& id){

  std::string ids = "";

  if (id == 2) {
    ids = "Loose";
  } else if (id == 3) {
    ids = "Medium";
  } else if (id == 4) {
    ids = "Tight";
  } else if (id == 0) {
    ids = "Fail";
  } else if (id == 1) {
    ids = "Veto";
  } else {
    ids = "El_Error";
  }
  return ids.c_str();
}

void PreSelector::FindLeadingLepton(){
  if((lep1.Pt() > lep2.Pt()) and (lep1.Pt() > lep3.Pt())){
    leadingLepton = &lep1;
  } else if ((lep2.Pt() > lep1.Pt()) and (lep2.Pt() > lep3.Pt())) {
    leadingLepton = &lep2;
  } else if ((lep3.Pt() > lep1.Pt()) and (lep3.Pt() > lep2.Pt())) {
    leadingLepton = &lep3;
  } else {
    leadingLepton = nullptr;
  }
}

Bool_t PreSelector::Process(Long64_t entry) {


  IsA_ = IsB = IsC = IsD = false;
  PairMu = PairEl = false;
  l1 = l2 = l3 = -1;
  PairZMass = -1.;
  SameFlvWCand.clear();

  ReadEntry(entry);

  HCutFlow->FillS("NoCuts");

#ifndef CMSDATA
  HCutFlow->Fill("genWeight",*genWeight);
  HTruePileup->Fill(static_cast<Double_t>(*Pileup_nTrueInt),*genWeight);
#endif

  HPileup->Fill(static_cast<Double_t>(*PV_npvs));

  if (!ElectronTest()) HCutFlow->FillS("FailElectronHLTs");
  if (!MuonTest()) HCutFlow->FillS("FailMuonHLTs");

  if (!ElectronTest() && !MuonTest() ){
    HCutFlow->FillS("FailHLT");
    return kFALSE;
  }
  if (!FlagsTest()){
    HCutFlow->FillS("FailFlags");
    return kFALSE;
  }

  if(!MinLeptonsTest()) {
    HCutFlow->FillS("lep<3");
    return kFALSE;
  }

  if (*MET_pt < 40.){
    HCutFlow->FillS("MET<40");
    return kFALSE;
  }

#ifndef CMSDATA
  Muons Mus(nMuon,Muon_tunepRelPt,Muon_pt,Muon_eta,Muon_phi,
            Muon_charge,Muon_dxy,Muon_dz,Muon_pfRelIso03_all,
            Muon_ip3d,Muon_sip3d,
            Muon_tightId, Muon_genPartIdx, Muon_pdgId );

  Electrons Els(nElectron,Electron_pt,Electron_eta,Electron_phi,
                Electron_charge,Electron_dxy,Electron_dz,Electron_pfRelIso03_all,
                Electron_ip3d,Electron_sip3d,
                Electron_cutBased, Electron_genPartIdx, Electron_pdgId);
#else
  Muons Mus(nMuon,Muon_tunepRelPt,Muon_pt,Muon_eta,Muon_phi,
            Muon_charge,Muon_dxy,Muon_dz,Muon_pfRelIso03_all,
            Muon_ip3d,Muon_sip3d,
            Muon_tightId);

  Electrons Els(nElectron,Electron_pt,Electron_eta,Electron_phi,
                Electron_charge,Electron_dxy,Electron_dz,Electron_pfRelIso03_all,
                Electron_ip3d,Electron_sip3d,
                Electron_cutBased);
#endif

  GoodElectron = PreSelector::GetGoodElectron(Els);
  GoodMuon = PreSelector::GetGoodMuon(Mus);

  HNEl->Fill(*nElectron,GoodElectron.size());
  HNMu->Fill(*nMuon,GoodMuon.size());

  if( (GoodElectron.size() + GoodMuon.size()) < 3 ){
    HCutFlow->FillS("goodLep<3");
    return kFALSE;
  }

  ////////////// Define Z //////////////

  const Float_t MinZMass = 70.;
  const Float_t MaxZMass = 111.;

  ZPairInfo *zt = nullptr;
  ZPairInfo ztel;
  ZPairInfo ztmu;

  ztmu = FindZ(Mus);
  if(!ztmu.empty() && CheckMuonPair(ztmu.Pair)) PairMu = true;

  ztel = FindZ(Els);
  if(!ztel.empty() && CheckElectronPair(ztel.Pair)) PairEl = true;

  if (!PairEl && !PairMu) {
    HCutFlow->FillS("NoPairs");
    return kFALSE;
  }

  if (PairEl && PairMu){
    HCutFlow->FillS("BothFlvPairs");
  }

  if (PairEl && PairMu) {
    if (ztel.Delta > ztmu.Delta) {
      PairEl = false;
    } else {
      PairMu = false;
    }
  }

  if(PairEl){
    if (*MET_pt < 40.) {
      return kFALSE;
    }
    zt = &ztel;
  } else { //PairMu
    if (*MET_pt < 60.) {
      return kFALSE;
    }
    zt = &ztmu;
  }

  if(PairMu) HCutFlow->FillS("PairMu");
  if(PairEl) HCutFlow->FillS("PairEl");

  PairZMass = (*zt).Mass;
  Bool_t IsZMassOk = (PairZMass > MinZMass) && (PairZMass < MaxZMass);
  if(!IsZMassOk){
    HCutFlow->FillS("FailZMassWindow");
  }

  l1 = (*zt).Pair.first;
  l2 = (*zt).Pair.second;

  if(PairEl){
    lep1 = PtEtaPhiMVector(Electron_pt[l1],Electron_eta[l1],
                           Electron_phi[l1],Electrons::mass);
    lep2 = PtEtaPhiMVector(Electron_pt[l2],Electron_eta[l2],
                           Electron_phi[l2],Electrons::mass);
  } else { //PairMu
    lep1 = PtEtaPhiMVector(Muon_tunepRelPt[l1]*Muon_pt[l1],Muon_eta[l1],
                           Muon_phi[l1],Muons::mass);
    lep2 = PtEtaPhiMVector(Muon_tunepRelPt[l2]*Muon_pt[l2],Muon_eta[l2],
                           Muon_phi[l2],Muons::mass);
  }

  Bool_t Zl1PtCut = lep1.Pt() < 50.;
  if(Zl1PtCut){
    HCutFlow->FillS("FailZl1PtCut");
    return kFALSE;
  }

  zb   = lep1 + lep2;

  ////////////// Define W //////////////

  assert(PairEl xor PairMu);

  if(PairMu){
    for(const int& i: GoodMuon){
      if(i!=l1 && i!=l2){
        const Float_t minPt = 50.;
        const Float_t farFromPV = 1e-2;
        if(Muon_highPtId[i] == 2 and Muon_tunepRelPt[i]*Muon_pt[i] > minPt and Muon_ip3d[i] < farFromPV)
          SameFlvWCand.emplace_back(i);
      }
    }
    if(SameFlvWCand.size() == 0 and GoodElectron.size() == 0){
      HCutFlow->FillS("PairMu_NoWlepCand");
      return kFALSE;
    }
    if(!PairMuDefineW(Els,Mus))
      return kFALSE;
  } else { // PairEl
    for(const int& i: GoodElectron){
      if(i!=l1 && i!=l2)
        SameFlvWCand.emplace_back(i);
    }
    if(SameFlvWCand.size() == 0 and GoodMuon.size() == 0){
      HCutFlow->FillS("PairEl_NoWlepCand");
      return kFALSE;
    }
    if(!PairElDefineW(Els,Mus))
      return kFALSE;
  }

  assert(l3 != -1);
  assert(IsA_ xor IsB xor IsC xor IsD);

  //////////////////////////////////////

  if( (lep1 + lep2 + lep3).M() < 120. ){
    HCutFlow->FillS("MLeps<120");
    return kFALSE;
  }

  lt = lep1.Pt()+lep2.Pt()+lep3.Pt();

  if( lt < 110.){
    HCutFlow->FillS("FailsLTCut");
    return kFALSE;
  }

  FindLeadingLepton();

#ifndef CMSDATA
  MetUncl = GetMetUncl();
#endif

  auto printEventInfo = [&](){
    std::cout <<
      Form("%d%d%d%d\tRun: %u\tEvent: %llu\tLumiblock: %u\tmll: %.4f\n",
           IsA_,IsB,IsC,IsD,
           *run, *event, *luminosityBlock, zb.M());
  };


  const float_t l1l2Dist = GetEtaPhiDistance(lep1.Eta(),lep1.Phi(),lep2.Eta(),lep2.Phi());
  Bool_t ZDistCut = l1l2Dist < 1.5;
  if(IsZMassOk){
    if(ZDistCut){
      HCutFlow->FillS("SR1");
      FillRegion(0,Els,Mus); // 1st SR
    } else {
      HCutFlow->FillS("CR1");
      FillRegion(8,Els,Mus); // 8 -> CR1 Slot
    }
  } else {
    HCutFlow->FillS("CR2");
    FillRegion(16,Els,Mus);
    if(nbTag()>=2){
      HCutFlow->FillS("CR3");
      FillRegion(24,Els,Mus);
    }
  }

  return kTRUE;
}

void PreSelector::Terminate() {

  gStyle->SetOptStat(1111111);

  std::unique_ptr<TFile> fOut(TFile::Open(FileNameOut.Data(),"UPDATE"));
  fOut->mkdir(Form("%d",Year));
  fOut->mkdir(Form("%d/%s",Year,SampleName.Data()));
  fOut->cd(Form("%d/%s",Year,SampleName.Data()));

  TObjLink *lnk = fOutput->FirstLink();
  while (lnk) {
    lnk->GetObject()->Write(lnk->GetObject()->GetName(),TObject::kOverwrite);
    lnk = lnk->Next();
  }

  fOut->Write();
  fOut->Close();

}

///////////////////////////////////////////////////////////
//////////////////// ONLY MC Methods //////////////////////
///////////////////////////////////////////////////////////

#ifndef CMSDATA
///////////////////////////////////////////////////////////
MetUnclObj PreSelector::GetMetUncl(){

  Float_t metPx = (*MET_pt)*(cos(*MET_phi));
  Float_t metPy = (*MET_pt)*(sin(*MET_phi));

  Float_t metPxUp = metPx + *MET_MetUnclustEnUpDeltaX;
  Float_t metPxDown = metPx - *MET_MetUnclustEnUpDeltaX;

  Float_t metPyUp = metPy + *MET_MetUnclustEnUpDeltaY;
  Float_t metPyDown = metPy - *MET_MetUnclustEnUpDeltaY;

  struct MetUnclObj r = {
    sqrt(pow(metPxUp,2.)+pow(metPyUp,2.)),
    sqrt(pow(metPxDown,2.)+pow(metPyDown,2.)),
    atan2(metPyUp,metPxUp),
    atan2(metPyDown,metPxDown)
  };

  return r;

}

///////////////////////////////////////////////////////////
std::string PreSelector::GetFakeString(const int& genPartIdx,
                                       const int& pdgId, const int& idn) const{

  std::function<Bool_t(const int&)> isPrompt = [&] (const int& gpidx) {
    const int maxNFlags = 14;
    const int isPromptFlag = 0;
    std::bitset<maxNFlags> flags(GenPart_statusFlags[gpidx]);
    return flags[isPromptFlag];
  };

  std::function<std::string(void)> idnToString = [&] () {
    if(pdgId == ElPdgId){
      if ( idn == 2) return "Loose";
      if ( idn == 3) return "Medium";
      if ( idn == 4) return "Tight";
    } else if (pdgId == MuPdgId) {
      if ( idn == 1) return "Tracker";
      if ( idn == 2) return "Global";
    }
    return "ERROR";
  };

  Int_t pdgIdMother = GetMother(genPartIdx,pdgId).second;
  std::string fakeContent = "";

  if(abs(pdgId) == ElPdgId){
    fakeContent += std::string("El.");
  } else {
    fakeContent += std::string("Mu.");
  }

  std::vector accept = { pdgId /*e or Mu*/, 15 /*tau*/, 4 /*c*/, 5 /*b*/ };

  if (isPrompt(genPartIdx)) {
    fakeContent += std::string("Prompt.") + idnToString();
  } else {
    if (std::find(accept.begin(),accept.end(),abs(pdgIdMother)) != accept.end()) {
      fakeContent += std::string("HFD.") + idnToString(); // Heavy Flavor Decay
      return fakeContent;
    }
    fakeContent += std::string("NonPrompt.") + idnToString();
  }
  return fakeContent;
}
///////////////////////////////////////////////////////////
Double_t PreSelector::GetSFFromGraph(TGraphAsymmErrors* g,const Float_t& eta,
                                     const Int_t& option) const {

  assert(g!=NULL);

  Double_t sf = -1.;
  Double_t* sfp;

  Int_t i = 0;
  while(i < g->GetN()){
    sfp = (g->GetY()+i);
    /* Test right bin limit*/
    if( eta < (*(g->GetX()+i) + g->GetErrorX(i))) break;
    i++;
  }

  if( i == g->GetN() ) i = (g->GetN()-1);

  sf = sfp? *sfp: -1.;

  switch(option){
  case -1:
    sf -= g->GetErrorYlow(i);
    break;
  case 1:
    sf += g->GetErrorYhigh(i);
    break;
  case 0:
    break;
  }

  assert(sf>0.);

  return sf;
}
///////////////////////////////////////////////////////////
Double_t PreSelector::GetZPtFromGen() const{

  const int zid = 23;
  double zptMax = 0.;
  const int maxNFlags = 14;
  const int fromHardProcess = 8;
  const int isFirstCopy = 12;

  auto checkFlags = [&](const int& idx) {
    std::bitset<maxNFlags> flags(GenPart_statusFlags[idx]);
    return ( flags[fromHardProcess] and flags[isFirstCopy] );
  };

  for(uint i = 0; i < GenPart_pdgId.GetSize(); ++i){
    if( abs(GenPart_pdgId[i]) == zid and GenPart_pt[i] > zptMax
        and checkFlags(i) ){
      zptMax = GenPart_pt[i];
    }
  }

  return zptMax;
}
///////////////////////////////////////////////////////////
Double_t PreSelector::GetSFFromHisto(TH1* h,const Float_t& x, const Float_t& y,
                                    const Int_t& option) const {
  assert(h!= NULL);
  assert(x < h->GetXaxis()->GetXmax() && x > h->GetXaxis()->GetXmin());
  if(y > h->GetYaxis()->GetXmax())
    return GetSFFromHisto(h,x,h->GetYaxis()->GetXmax() - 1e-3,option);
  Int_t nbin = h->FindBin(x,y);
  Double_t sf = h->GetBinContent(nbin);
  switch(option){
  case -1:
    sf -= h->GetBinErrorLow(nbin);
    if(sf<0.) sf = 1e-6;
    break;
  case 1:
    sf += h->GetBinErrorUp(nbin);
    break;
  case 0:
    break;
  }

  assert(sf>=0.);

  return sf;
}
///////////////////////////////////////////////////////////
Float_t PreSelector::GetSFFromHisto(TH1* h, const Int_t& npv, const int option = 0){

  if(!h) return 1.;
  Int_t nbin = h->FindBin(npv);
  assert(copysign(1.,h->GetBinContent(nbin)) > 0);

  Float_t sf = h->GetBinContent(nbin);
  switch(option){
  case -1:
    sf -= h->GetBinErrorLow(nbin);
    if(sf<0.) sf = 1e-6;
    break;
  case 1:
    sf += h->GetBinErrorUp(nbin);
    break;
  case 0:
    break;
  }

  return sf;
}
///////////////////////////////////////////////////////////
#if !defined(ULSAMPLE)
Double_t PreSelector::GetElTriggerSF(const Float_t& eta, const Float_t& pt,
                                   const Int_t& option) const{

  assert(abs(eta)<2.5);

  Double_t sf = -1;

#if defined(Y2016)
  /* These are the default for Legacy */
  /* UL SFs haven't been computed     */
  /* 2 bins in pt */
  if( pt < 175.){
    sf = GetSFFromGraph(SFElectronTrigger1,eta,option);
  } else {
    sf = GetSFFromGraph(SFElectronTrigger2,eta,option);
  }
#endif

#if (defined(Y2017) || defined(Y2018))
  /* These are the default for Legacy */
  /* UL SFs haven't been computed     */
  if (pt < 200.){
    sf = GetSFFromGraph(SFElectronTrigger1,eta,option);
  } else {
    sf = GetSFFromGraph(SFElectronTrigger2,eta,option);
  }
#endif

  assert(sf>0);

  return sf;

}
#endif
#if defined(ULSAMPLE)
Double_t PreSelector::GetElTriggerSF(const Float_t& eta, const Float_t& pt,
                                     const Int_t& option) const{
  return GetSFFromHisto(SFElectronHLT,eta,pt,option);
}
#endif
///////////////////////////////////////////////////////////
Double_t PreSelector::GetKFactor(TH1* h /*EWK or QCD*/, const Double_t& ZGenPt, const int& option) const{
  assert(ApplyKFactors and h);
  double sf = 1.;

  if( ZGenPt < KSFMinPt or ZGenPt > KSFMaxPt)
    return sf;

  const int nbin = h->FindBin(ZGenPt);

  sf *= h->GetBinContent(nbin);

  switch(option){
  case -1:
    sf -= h->GetBinErrorLow(nbin);
    break;
  case 1:
    sf += h->GetBinErrorUp(nbin);
    break;
  case 0:
    break;
  }

  return sf;
}
///////////////////////////////////////////////////////////
Double_t PreSelector::GetElIDSF(Int_t id /* 2: loose. 3: medium. 4: tight*/,
                                const Float_t& eta, const Float_t& pt,
                                const Int_t& option) const{
  /* Option 0: Central Value, -1: Low, +1: up */
  Double_t sf;

  TH2F* h2source;

  if(id == 2){
    h2source = SFElectronLooseID;
  } else if(id == 3){
    h2source = SFElectronMediumID;
  } else if(id == 4){
    h2source = SFElectronTightID;
  }

  sf = GetSFFromHisto(h2source, eta,pt,option);
  return sf;
}

///////////////////////////////////////////////////////////
Double_t PreSelector::GetMuIDSF(UChar_t MuonID /* 2: highPt. 1: TrkHighPt*/,
                                const Float_t& eta, const Float_t& pt,
                                const Int_t& option) const{
  /* Option 0: Central Value, -1: Low, +1: up */

  assert(abs(eta)<2.4);

  Double_t sf = -1;

#if defined(Y2016) && !defined(ULSAMPLE)
  // https://twiki.cern.ch/twiki/bin/view/CMS/MuonLegacy2016
  const Double_t LumiBF = 20.;
  const Double_t LumiGH = 16.;
  // TrkHighPt SF Not available for Legacy. Using 1.0 instead
  Double_t SFBF = MuonID == 2? GetSFFromHisto(SFMuonHighPtIDBF,eta,pt,option) : 1.0;
  Double_t SFGH = MuonID == 2? GetSFFromHisto(SFMuonHighPtIDGH,eta,pt,option) : 1.0;
  sf = (LumiBF*SFBF+LumiGH*SFGH)/(LumiBF+LumiGH);
#endif

#if (defined(Y2016) && defined(ULSAMPLE)) || (defined(Y2017) || defined (Y2018))
  sf = GetSFFromHisto(MuonID == 2? SFMuonHighPtID:SFMuonTrkHighPtID,
                      abs(eta),pt,option);
#endif
  assert(sf>0);

  return sf;

}

///////////////////////////////////////////////////////////
Double_t PreSelector::GetMuTriggerSF(const Float_t& eta, Float_t pt,
                               const Int_t& option) const{
  /* Option 0: Central Value, -1: Low, +1: up */

  assert(abs(eta)<2.4);

  Double_t sf = -1;

#if defined(Y2016) && !defined(ULSAMPLE)
  // https://twiki.cern.ch/twiki/bin/view/CMS/MuonLegacy2016
  const Double_t LumiBF = 20.;
  const Double_t LumiGH = 16.;
  Double_t SFTriggerBF = GetSFFromHisto(SFMuonTriggerBF,abs(eta),pt,option);
  Double_t SFTriggerGH = GetSFFromHisto(SFMuonTriggerGH,abs(eta),pt,option);
  sf = (LumiBF*SFTriggerBF+LumiGH*SFTriggerGH)/(LumiBF+LumiGH);
#endif

#if (defined(Y2016) && defined(ULSAMPLE)) || ((defined(Y2017) || defined(Y2018)) && defined(ULSAMPLE))
  if (pt > 1e3) pt = 0.999e3;
  sf = GetSFFromHisto(SFMuonTrigger,pt,abs(eta),option);
#endif

#if (defined(Y2017) || defined (Y2018)) && !defined(ULSAMPLE)
  sf = GetSFFromHisto(SFMuonTrigger,abs(eta),pt,option);
#endif

  assert(sf>0);

  return sf;

}
///////////////////////////////////////////////////////////
std::pair<Int_t,Int_t> PreSelector::GetMother(std::pair<Int_t,Int_t> Daughter) const{
  return PreSelector::GetMother(Daughter.first,Daughter.second);
}
///////////////////////////////////////////////////////////
std::pair<Int_t,Int_t> PreSelector::GetMother(Int_t GenPartIdx, Int_t PdgId /*\Mu or e*/) const{

  if(GenPartIdx == -1 ) return std::make_pair(GenPartIdx,9999);

  std::pair<Int_t,Int_t> Mother;
  Mother.first = GenPart_genPartIdxMother[GenPartIdx];
  Mother.second = GenPart_pdgId[Mother.first];

  if(abs(Mother.second) == abs(PdgId)){
    Mother = PreSelector::GetMother(Mother);
  }

  return Mother;
}
///////////////////////////////////////////////////////////
void PreSelector::DefineSFs(const int& nh){

  WCentral = -1.;

  WPileupUp = GetSFFromHisto(SFPileupUp,*PV_npvs) ;
  WPileupDown = GetSFFromHisto(SFPileupDown,*PV_npvs);

  if(IsA_){

    float Pileup_ = GetSFFromHisto(SFPileup,*PV_npvs);
    float ElTrigger = GetElTriggerSF(Electron_eta[leadElIdx], Electron_pt[leadElIdx],0);
    float ElRecol1 =
      GetSFFromHisto(lep1.Pt() > 20.? SFElectronReco:SFElectronRecoB20,lep1.Eta(),lep1.Pt(),0);
    float ElRecol2 =
      GetSFFromHisto(lep2.Pt() > 20.? SFElectronReco:SFElectronRecoB20,lep2.Eta(),lep2.Pt(),0);
    float ElRecol3 =
      GetSFFromHisto(lep3.Pt() > 20.? SFElectronReco:SFElectronRecoB20,lep3.Eta(),lep3.Pt(),0);
    float ElIDl1 = GetElIDSF(Electron_cutBased[l1],lep1.Eta(),lep1.Pt(),0);
    float ElIDl2 = GetElIDSF(Electron_cutBased[l2],lep2.Eta(),lep2.Pt(),0);
    float ElIDl3 = GetElIDSF(Electron_cutBased[l3],lep3.Eta(),lep3.Pt(),0);

    WCentral = Pileup_*ElTrigger*ElRecol1*ElRecol2*ElRecol3*ElIDl1*ElIDl2*ElIDl3;

    WElTrigUp = GetElTriggerSF(Electron_eta[leadElIdx], Electron_pt[leadElIdx],1);
    WElTrigDown = GetElTriggerSF(Electron_eta[leadElIdx], Electron_pt[leadElIdx],-1);

    WElRecoUp = GetSFFromHisto(SFElectronReco,lep1.Eta(),lep1.Pt(),1);
    WElRecoUp *= GetSFFromHisto(SFElectronReco,lep2.Eta(),lep2.Pt(),1);
    WElRecoUp *= GetSFFromHisto(SFElectronReco,lep3.Eta(),lep3.Pt(),1);

    WElRecoDown = GetSFFromHisto(SFElectronReco,lep1.Eta(),lep1.Pt(),-1);
    WElRecoDown *= GetSFFromHisto(SFElectronReco,lep2.Eta(),lep2.Pt(),-1);
    WElRecoDown *= GetSFFromHisto(SFElectronReco,lep3.Eta(),lep3.Pt(),-1);

    WElIDUp =  GetElIDSF(Electron_cutBased[l1],lep1.Eta(),lep1.Pt(),1);
    WElIDUp *= GetElIDSF(Electron_cutBased[l2],lep2.Eta(),lep2.Pt(),1);
    WElIDUp *= GetElIDSF(Electron_cutBased[l3],lep3.Eta(),lep3.Pt(),1);

    WElIDDown = GetElIDSF(Electron_cutBased[l1],lep1.Eta(),lep1.Pt(),-1);
    WElIDDown *= GetElIDSF(Electron_cutBased[l2],lep2.Eta(),lep2.Pt(),-1);
    WElIDDown *= GetElIDSF(Electron_cutBased[l3],lep3.Eta(),lep3.Pt(),-1);

    WAllUp = WPileupUp*WElTrigUp*WElRecoUp*WElIDUp;
    WAllDown = WPileupDown*WElTrigDown*WElRecoDown*WElIDDown;

#if defined(Y2016) || defined(Y2017)
    WCentral *= *L1PreFiringWeight_ECAL_Nom;
    WAllUp *= *L1PreFiringWeight_ECAL_Up;
    WAllDown *= *L1PreFiringWeight_ECAL_Dn;
    HSFs[nh]->Fill("Prefiring",*L1PreFiringWeight_ECAL_Nom,1.);
#endif

    HSFs[nh]->Fill("Pileup",Pileup_,1.);
    HSFs[nh]->Fill("ElTriggerSF",ElTrigger,1.);
    HSFs[nh]->Fill("ELRecol1",ElRecol1,1.);
    HSFs[nh]->Fill("ElRecol2",ElRecol2,1.);
    HSFs[nh]->Fill("ElRecol3",ElRecol3,1.);
    HSFs[nh]->Fill("ElIDl1",ElIDl1,1.);
    HSFs[nh]->Fill("ElIDl2",ElIDl2,1.);
    HSFs[nh]->Fill("ElIDl3",ElIDl3,1.);


  } else if (IsB) {

    float Pileup_ = GetSFFromHisto(SFPileup,*PV_npvs);
    float ElTrigger = GetElTriggerSF(Electron_eta[leadElIdx],Electron_pt[leadElIdx],0);
    float MuTrigger = GetMuTriggerSF(Muon_eta[leadMuIdx],Muon_tunepRelPt[leadMuIdx]*Muon_pt[leadMuIdx],0);
    float ElIDl1 = GetElIDSF(Electron_cutBased[l1],lep1.Eta(),lep1.Pt(),0);
    float ElIDl2 = GetElIDSF(Electron_cutBased[l2],lep2.Eta(),lep2.Pt(),0);
    float ElRecol1 = GetSFFromHisto(lep1.Pt()>20.?SFElectronReco:SFElectronRecoB20,lep1.Eta(),lep1.Pt(),0);
    float ElRecol2 = GetSFFromHisto(lep2.Pt()>20.?SFElectronReco:SFElectronRecoB20,lep2.Eta(),lep2.Pt(),0);
    float MuIDl3 = GetMuIDSF(Muon_highPtId[l3],lep3.Eta(),lep3.Pt(),0);

    WCentral = Pileup_*ElTrigger*MuTrigger*ElIDl1*ElIDl2*ElRecol1*ElRecol2*MuIDl3;

    WElTrigUp = GetElTriggerSF(Electron_eta[leadElIdx],Electron_pt[leadElIdx],1);
    WElTrigDown = GetElTriggerSF(Electron_eta[leadElIdx],Electron_pt[leadElIdx],-1);

    WMuTrigUp = GetMuTriggerSF(Muon_eta[leadMuIdx],Muon_tunepRelPt[leadMuIdx]*Muon_pt[leadMuIdx],1);
    WMuTrigDown = GetMuTriggerSF(Muon_eta[leadMuIdx],Muon_tunepRelPt[leadMuIdx]*Muon_pt[leadMuIdx],-1);

    WElRecoUp = GetSFFromHisto(SFElectronReco,lep1.Eta(),lep1.Pt(),1);
    WElRecoUp *= GetSFFromHisto(SFElectronReco,lep2.Eta(),lep2.Pt(),1);

    WElRecoDown = GetSFFromHisto(SFElectronReco,lep1.Eta(),lep1.Pt(),-1);
    WElRecoDown *= GetSFFromHisto(SFElectronReco,lep2.Eta(),lep2.Pt(),-1);

    WElIDUp = GetElIDSF(Electron_cutBased[l1],lep1.Eta(),lep1.Pt(),1);
    WElIDUp *= GetElIDSF(Electron_cutBased[l2],lep2.Eta(),lep2.Pt(),1);

    WElIDDown = GetElIDSF(Electron_cutBased[l1],lep1.Eta(),lep1.Pt(),-1);
    WElIDDown *= GetElIDSF(Electron_cutBased[l2],lep2.Eta(),lep2.Pt(),-1);

    WMuIDUp = GetMuIDSF(Muon_highPtId[l3],lep3.Eta(),lep3.Pt(),1);
    WMuIDDown = GetMuIDSF(Muon_highPtId[l3],lep3.Eta(),lep3.Pt(),-1);

    WAllUp = WPileupUp*WElTrigUp*WMuTrigUp*WElIDUp*WElRecoUp*WMuIDUp;
    WAllDown = WPileupDown*WElTrigDown*WMuTrigDown*WElIDDown*WElRecoDown*WMuIDDown;

#if defined(Y2016) || defined(Y2017)
    WCentral *= *L1PreFiringWeight_Nom;
    WAllUp *= *L1PreFiringWeight_Up;
    WAllDown *= *L1PreFiringWeight_Dn;
    HSFs[nh]->Fill("Prefiring",*L1PreFiringWeight_Nom,1.);
#endif

    HSFs[nh]->Fill("Pileup",Pileup_,1.);
    HSFs[nh]->Fill("ElTrigger",ElTrigger,1.);
    HSFs[nh]->Fill("MuTrigger",MuTrigger,1.);
    HSFs[nh]->Fill("ELRecol1",ElRecol1,1.);
    HSFs[nh]->Fill("ElRecol2",ElRecol2,1.);
    HSFs[nh]->Fill("ElIDl1",ElIDl1,1.);
    HSFs[nh]->Fill("ElIDl2",ElIDl2,1.);
    HSFs[nh]->Fill("MuIDl3",MuIDl3,1.);

  } else if (IsC) {

    float Pileup_ = GetSFFromHisto(SFPileup,*PV_npvs);
    float ElTrigger = GetElTriggerSF(Electron_eta[leadElIdx],Electron_pt[leadElIdx],0);
    float MuTrigger = GetMuTriggerSF(Muon_eta[leadMuIdx],Muon_tunepRelPt[leadMuIdx]*Muon_pt[leadMuIdx],0);
    float MuIDl1 = GetMuIDSF(Muon_highPtId[l1],lep1.Eta(),lep1.Pt(),0);;
    float MuIDl2 = GetMuIDSF(Muon_highPtId[l2],lep2.Eta(),lep2.Pt(),0);;
    float ElRecol3 = GetSFFromHisto(lep3.Pt()>20.?SFElectronReco:SFElectronRecoB20,lep3.Eta(),lep3.Pt(),0);
    float ElIDl3 = GetElIDSF(Electron_cutBased[l3],lep3.Eta(),lep3.Pt(),0);

    WCentral = Pileup_*ElTrigger*MuTrigger*MuIDl1*MuIDl2*ElRecol3*ElIDl3;

    WElRecoUp = GetSFFromHisto(SFElectronReco,lep3.Eta(),lep3.Pt(),1);
    WElRecoDown = GetSFFromHisto(SFElectronReco,lep3.Eta(),lep3.Pt(),-1);

    WElTrigUp = GetElTriggerSF(Electron_eta[leadElIdx],Electron_pt[leadElIdx],1);
    WElTrigDown = GetElTriggerSF(Electron_eta[leadElIdx],Electron_pt[leadElIdx],-1);
    WMuTrigUp = GetMuTriggerSF(Muon_eta[leadMuIdx],Muon_tunepRelPt[leadMuIdx]*Muon_pt[leadMuIdx],1);
    WMuTrigDown = GetMuTriggerSF(Muon_eta[leadMuIdx],Muon_tunepRelPt[leadMuIdx]*Muon_pt[leadMuIdx],-1);

    WMuIDUp   = GetMuIDSF(Muon_highPtId[l1],lep1.Eta(),lep1.Pt(),1);
    WMuIDDown = GetMuIDSF(Muon_highPtId[l1],lep1.Eta(),lep1.Pt(),-1);

    WMuIDUp  *= GetMuIDSF(Muon_highPtId[l2],lep2.Eta(),lep2.Pt(),1);
    WMuIDDown *= GetMuIDSF(Muon_highPtId[l2],lep2.Eta(),lep2.Pt(),-1);

    WElIDUp   = GetElIDSF(Electron_cutBased[l3],lep3.Eta(),lep3.Pt(),1);
    WElIDDown = GetElIDSF(Electron_cutBased[l3],lep3.Eta(),lep3.Pt(),-1);

    WAllUp = WPileupUp*WElTrigUp*WMuTrigUp*WMuIDUp*WElRecoUp*WElIDUp;
    WAllDown = WPileupDown*WElTrigDown*WMuTrigDown*WMuIDDown*WElRecoDown*WElIDDown;

#if defined(Y2016) || defined(Y2017)
    WCentral *= *L1PreFiringWeight_Nom;
    WAllUp *= *L1PreFiringWeight_Up;
    WAllDown *= *L1PreFiringWeight_Dn;
    HSFs[nh]->Fill("Prefiring",*L1PreFiringWeight_Nom,1.);
#endif

    HSFs[nh]->Fill("Pileup",Pileup_,1.);
    HSFs[nh]->Fill("ElTrigger",ElTrigger,1.);
    HSFs[nh]->Fill("MuTrigger",MuTrigger,1.);
    HSFs[nh]->Fill("MuIDl1",MuIDl1,1.);
    HSFs[nh]->Fill("MuIDl2",MuIDl2,1.);
    HSFs[nh]->Fill("ElRecol3",ElRecol3,1.);
    HSFs[nh]->Fill("ElIDl3",ElIDl3,1.);

  } else if (IsD) {

    float Pileup_ = GetSFFromHisto(SFPileup,*PV_npvs);
    float MuTrigger = GetMuTriggerSF(Muon_eta[leadMuIdx],Muon_tunepRelPt[leadMuIdx]*Muon_pt[leadMuIdx],0);
    float MuIDl1 = GetMuIDSF(Muon_highPtId[l1],lep1.Eta(),lep1.Pt(),0);
    float MuIDl2 = GetMuIDSF(Muon_highPtId[l2],lep2.Eta(),lep2.Pt(),0);
    float MuIDl3 = GetMuIDSF(Muon_highPtId[l3],lep3.Eta(),lep3.Pt(),0);

    WCentral = Pileup_*MuTrigger*MuIDl1*MuIDl2*MuIDl3;

    WMuTrigUp = GetMuTriggerSF(Muon_eta[leadMuIdx],Muon_tunepRelPt[leadMuIdx]*Muon_pt[leadMuIdx],1);
    WMuTrigDown = GetMuTriggerSF(Muon_eta[leadMuIdx],Muon_tunepRelPt[leadMuIdx]*Muon_pt[leadMuIdx],-1);

    WMuIDUp   = GetMuIDSF(Muon_highPtId[l1],lep1.Eta(),lep1.Pt(),1);
    WMuIDDown = GetMuIDSF(Muon_highPtId[l1],lep1.Eta(),lep1.Pt(),-1);

    WMuIDUp   *= GetMuIDSF(Muon_highPtId[l2],lep2.Eta(),lep2.Pt(),1);
    WMuIDDown *= GetMuIDSF(Muon_highPtId[l2],lep2.Eta(),lep2.Pt(),-1);

    WMuIDUp   *= GetMuIDSF(Muon_highPtId[l3],lep3.Eta(),lep3.Pt(),1);
    WMuIDDown *= GetMuIDSF(Muon_highPtId[l3],lep3.Eta(),lep3.Pt(),-1);

    WAllUp = WPileupUp*WMuTrigUp*WMuIDUp;
    WAllDown = WPileupUp*WMuTrigDown*WMuIDDown;

#if defined(Y2016) || defined(Y2017)
    WCentral *= *L1PreFiringWeight_Muon_Nom;
    WAllUp *= *L1PreFiringWeight_Muon_Up;
    WAllDown *= *L1PreFiringWeight_Muon_Dn;
    HSFs[nh]->Fill("Prefiring",*L1PreFiringWeight_Muon_Nom,1.);
#endif
    HSFs[nh]->Fill("MuTrigger",MuTrigger,1.);
    HSFs[nh]->Fill("MuIDl1",MuIDl1,1.);
    HSFs[nh]->Fill("MuIDl2",MuIDl2,1.);
    HSFs[nh]->Fill("MuIDl3",MuIDl3,1.);

  } else {
    assert (false);
  }

  Double_t ksf = 1.;

  if (ApplyKFactors) {
    Double_t zpt = GetZPtFromGen();
    if(zpt > 0.){
      ksf  = GetKFactor(SFDYKFactorEWK,zpt,0);
      ksf *= GetKFactor(SFDYKFactorQCD,zpt,0);
      WKEWKUp = GetKFactor(SFDYKFactorEWK,zpt,1);
      WKEWKDown = GetKFactor(SFDYKFactorEWK,zpt,-1);
      WKQCDUp = GetKFactor(SFDYKFactorQCD,zpt,1);
      WKQCDDown = GetKFactor(SFDYKFactorQCD,zpt,-1);
    }
    WCentral *= ksf;
    WAllUp *= WKEWKUp*WKQCDUp;
    WAllDown *= WKEWKDown*WKQCDDown;
    HSFs[nh]->Fill("KFactor",ksf,1.);
  }

  WCentral *= *genWeight;
  WAllUp *= *genWeight;
  WAllDown *= *genWeight;
  WElRecoUp *= *genWeight;
  WElRecoDown *= *genWeight;
  WElTrigUp *= *genWeight;
  WElTrigDown *= *genWeight;
  WMuTrigUp *= *genWeight;
  WMuTrigDown *= *genWeight;
  WElIDUp *= *genWeight;
  WElIDDown *= *genWeight;
  WMuIDUp *= *genWeight;
  WMuIDDown *= *genWeight;

  HSFs[nh]->Fill("Central",WCentral,1.);
  HSFs[nh]->Fill("WAllDown",WAllDown,1.);
  HSFs[nh]->Fill("WAllUp",WAllUp,1.);
}
///////////////////////////////////////////////////////////
#endif /////////////// Only MC Methods ////////////////////
///////////////////////////////////////////////////////////
