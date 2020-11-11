#include "TFile.h"
#include "PreSelector.h"
#include "TMath.h"
#include "TLegend.h"
#include "TError.h"
#include "TVector2.h"

#define FillS(xx) Fill(xx,1.)

PreSelector::PreSelector(TTree *)
{

  HMetPt=0;

  HnJet=0;


  HMassZTW=0;
  HDeltaRPtZ=0;
  HPtWPtZ=0;
  HDeltaRMWZ=0;
  HLtMWZ=0;

  HOverlap=0;

  HDistl1l3=0;
  HDistl2l3=0;


#ifndef CMSDATA
  ELPass=0;

  HScaleFactors = 0;
  SFPileup = 0;

  HLog = 0;
#endif
  HCutFlow = 0;

  HPtl1 = 0;
  HPtl2 = 0;
  HPtl3 = 0;

  HEtal1 = 0;
  HEtal2 = 0;
  HEtal3 = 0;

  HPtEtal1 = 0;
  HPtEtal2 = 0;
  HPtEtal3 = 0;

  HPhil1 = 0;
  HPhil2 = 0;
  HPhil3 = 0;
  HMetPhi = 0;

  HWZDist = 0;
  HWZPtDist = 0;

  HPileup = 0;

  HWZPt = 0;
}

#ifndef CMSDATA
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
#endif
#ifndef CMSDATA
Double_t PreSelector::GetSFFromHisto(TH1* h,const Float_t& x, const Float_t& y,
                                    const Int_t& option) const {
  assert(h!= NULL);
#if defined(Y2016)
  /* Check for eta */
  /* 2016 provides x(eta) and y(pt)  */
  assert(x < h->GetXaxis()->GetXmax() && x > h->GetXaxis()->GetXmin());
  if(y > h->GetYaxis()->GetXmax())
    return GetSFFromHisto(h,x,h->GetYaxis()->GetXmax() - 1e-3,option);
#elif defined(Y2017) || defined(Y2018)
  /* 2017/2018 SFMuonID, SFMuonTrigger provides x(pt) and y(eta)  */
  assert(y < h->GetYaxis()->GetXmax() && y > h->GetYaxis()->GetXmin());
  if(x > h->GetXaxis()->GetXmax())
    return GetSFFromHisto(h,h->GetXaxis()->GetXmax() - 1e-3,y,option);
#endif
  Int_t nbin = h->FindBin(x,y);
  Double_t sf = h->GetBinContent(nbin);
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

  assert(sf>0.);
  return sf;
}
#endif
#ifndef CMSDATA
Float_t PreSelector::GetSFFromHisto(TH1* h, const Int_t& npv){
  if(!h) return 1.;
  return h->GetBinContent(h->FindBin(npv));
}
#endif
#ifndef CMSDATA
Double_t PreSelector::GetElectronSF(const Float_t& eta, const Float_t& pt,
                                   const Int_t& option) const{

  assert(abs(eta)<2.5);

  Double_t sf = -1;

#if defined(Y2016)
  /* 2 bins in pt */
  if( pt < 175.){
    sf = GetSFFromGraph(SFElectronTrigger1,eta,option);
  } else {
    sf = GetSFFromGraph(SFElectronTrigger2,eta,option);
  }
#elif defined(Y2017) || defined(Y2018)
  /* 2 bins in Pt */
  if (pt < 200.){
    sf = GetSFFromGraph(SFElectronTrigger1,eta,option);
  } else {
    sf = GetSFFromGraph(SFElectronTrigger2,eta,option);
  }
#endif

  return sf;

}
#endif
#ifndef CMSDATA
Double_t PreSelector::GetMuonSF(const Float_t& eta, const Float_t& pt,
                               const Int_t& option) const{
  /* Option 0: Central Value, -1: Low, +1: up */

  assert(abs(eta)<2.4);

  Double_t sf = -1;

#if defined(Y2016)
  /*

    B->F : 5.746 + 2.573 + 4.242 + 4.025 + 3.104 //fb-1
    G->H : 7.576 + 8.651                         //fb-1
    From AN_2019_245_v12
  */
  const Double_t LumiBF = 19.689;
  const Double_t LumiGH = 16.227;

  Double_t SFTriggerBF = GetSFFromHisto(SFMuonTriggerBF,abs(eta),pt,option);
  Double_t SFTriggerGH = GetSFFromHisto(SFMuonTriggerGH,abs(eta),pt,option);
  Double_t SFIDBF = GetSFFromHisto(SFMuonIDBF,eta,pt,option);
  Double_t SFIDGH = GetSFFromHisto(SFMuonIDGH,eta,pt,option);

  sf = (LumiBF*SFTriggerBF+LumiGH*SFTriggerGH)/(LumiBF+LumiGH);
  sf *=(LumiBF*SFIDBF+LumiGH*SFIDGH)/(LumiBF+LumiGH);

#elif defined(Y2017) || defined(Y2018)
  sf = GetSFFromHisto(SFMuonTrigger,pt,abs(eta),option);
  sf *= GetSFFromHisto(SFMuonID,pt,abs(eta),option);
#endif

  return sf;

}
#endif

void PreSelector::Begin(TTree *tree) {

  if (fInput->FindObject("SampleName")) {
    // Lesson: TString can't be in TCollection
    TNamed *p = dynamic_cast<TNamed *>(fInput->FindObject("SampleName"));
    SampleName = p->GetTitle();
  }
}

template<typename T, typename... Args>
void PreSelector::InitHVec(std::vector<T*>& vec,
                           std::string_view name,
                           Args... args){

  std::vector<std::string> idst = {
    "A", "B", "C", "D",
    "A_Up","B_Up","C_Up","D_Up", /* +4 */
    "A_Down","B_Down","C_Down","D_Down" /* +8 */
  };

  for(auto id: idst){
    vec.emplace_back(new T(Form("%s_%s",name.data(),id.c_str()),
                           name.data(),
                           args...));
  }

  for(auto h: vec){
    fOutput->Add(h);
  }

}

void PreSelector::SlaveBegin(TTree *tree) {

  TH1::SetDefaultSumw2();

  auto copyHisto = [](TH1F** hcopy, TH1F *h, const std::string& idstr){
    *hcopy = static_cast<TH1F*>(h->Clone());
    (*hcopy)->SetName(Form("%s_%s",h->GetName(),idstr.c_str()));
    return *hcopy;
  };

  const Double_t MaxMet = 600.;
  const Double_t MinMet = 0.;
  const Int_t MetBins = 60;

  InitHVec<TH1F>(HMet_,"HMet",MetBins,MinMet,MaxMet);

  const Int_t DistBins = 100;
  const Float_t MaxDist = 2.01*TMath::Pi();

  InitHVec<TH1F>(HDistl1l2,"HDistl1l2",DistBins,0.,MaxDist);

  HDistl1l3 = new TH1F("HDistl1l3","Eta-Phi Distance l1,l3",DistBins,0.,MaxDist);
  fOutput->Add(HDistl1l3);
  HDistl2l3 = new TH1F("HDistl2l3","Eta-Phi Distance l2,l3",DistBins,0.,MaxDist);
  fOutput->Add(HDistl2l3);


  HWZDist = new TH1F("HWZDist","HWZDist",DistBins,0.,MaxDist);
  fOutput->Add(HWZDist);

  HWZPtDist = new TH2F("HWZPtDist","HWZPtDist",100,0.,1400.,DistBins,0.,MaxDist);
  fOutput->Add(HWZPtDist);

  HWZPt = new TH1F("HWZPt","HWZPt",100,0.,1e3);
  fOutput->Add(HWZPt);

  const Double_t MaxnLep = 7;
  const Double_t MinnLep = 0;
  const Int_t nLepBins = 7;

  InitHVec<TH1F>(HnEl,"HnEl",nLepBins,MinnLep,MaxnLep);
  InitHVec<TH1F>(HnMu,"HnMu",nLepBins,MinnLep,MaxnLep);

  const Int_t nJetBins = 15;
  HnJet = new TH1F("HnJet","",nJetBins,0,(float)nJetBins);
  fOutput->Add(HnJet);

  const Float_t MinMass = 0.;
  const Float_t MaxMass = 2200.;
  const Int_t MassBins = 22;

  const Float_t MinWMass = 0.;
  const Float_t MaxWMass = 1e3;

  InitHVec<TH1F>(HMassW,"HMassW",MassBins,MinWMass,MaxWMass);

  const Float_t HMaxZMass = 120.;
  const Float_t HMinZMass = 60.;
  const Int_t ZMassBins = 30;
  InitHVec<TH1F>(HMassZ_,"HMassZ",ZMassBins,HMinZMass,HMaxZMass);

  const Float_t MaxTWMass = 250.;
  const Int_t TWMassBins = 50;
  InitHVec<TH1F>(HMassTW_,"HMassTW",TWMassBins,0.,MaxTWMass);

  const Float_t MaxWZMass = 8500.;
  const Int_t WZMassBins = 85;
  InitHVec<TH1F>(HMassWZ,"HMassWZ",WZMassBins,0.,MaxWZMass);

  const Float_t MaxLt = 2000.;
  const Int_t NLtBins = 20;
  InitHVec<TH1F>(HLt,"HLt",NLtBins,0.,MaxLt);

  InitHVec<TH2F>(HMassZWZ,"HMassZWZ",MassBins,0.,1.5*HMaxZMass,MassBins,0.,MaxMass);

  const Float_t MaxDeltaR = 6.f;
  const Int_t NBinsDeltaR = 10;
  const Float_t MaxWZM = 5000.f;
  const Float_t MinWZM = 500.f;
  const Int_t NBinsWZM = 45;
  const Int_t ZPtBins = 20;
  const Float_t ZPtMax = 2000;
  HMassZTW =  new TH2F("HMassZTW","MassZ Vs Mass Wt",
                       ZMassBins, HMinZMass, HMaxZMass,
                       TWMassBins,0.,MaxTWMass);
  fOutput->Add(HMassZTW);
  HDeltaRPtZ = new TH2F("HDeltaRPtZ","Lep1Lep3DeltaR vs ZPt",
                        NBinsDeltaR,0.,MaxDeltaR,
                        ZPtBins,0.,ZPtMax);
  fOutput->Add(HDeltaRPtZ);
  HPtWPtZ = new TH2F("HPtWPtZ","WPt vs ZPt",
                     110,0.,1100.,
                     ZPtBins,0.,ZPtMax);
  fOutput->Add(HPtWPtZ);
  HDeltaRMWZ = new TH2F("HDeltaRMWZ","Lep1Lep3DeltaR Vs MWz",
                        NBinsDeltaR,0.,MaxDeltaR,
                        NBinsWZM,MinWZM,MaxWZM);
  fOutput->Add(HDeltaRMWZ);
  HLtMWZ = new TH2F("HLtMWZ","Lt vs MWZ",
                    20,0.,2000.,
                    NBinsWZM,MinWZM,MaxWZM);
  fOutput->Add(HLtMWZ);

  HOverlap = new TH1F("HOverlap","Overlapping events."
                      " -1: l<3 0:None 1: NoOverlap",6,-1,5);
  fOutput->Add(HOverlap);

  const Int_t nPvsBins = 100;
  const Float_t minPvs = 0.;
  const Float_t maxPvs = 100.;

  HPileup = new TH1D("HPileup","PV_npvs",
                     nPvsBins,minPvs,maxPvs);
  fOutput->Add(HPileup);


  InitHVec<TH1F>(HPileup_,"HPileup_",nPvsBins,minPvs,maxPvs);

  const UInt_t BinsPdgId = 100;
  const Float_t PdgIdMin = -50.;
  const Float_t PdgIdMax = 50.;

#ifndef CMSDATA
  ELPass = new TEntryList("ELPass","Events Passing Full Selection");
  fOutput->Add(ELPass);

  InitHVec<TH1F>(HGenPartZ,"HGenPartZ",BinsPdgId,PdgIdMin,PdgIdMax);
  InitHVec<TH1F>(HGenPartW,"HGenPartW",BinsPdgId,PdgIdMin,PdgIdMax);
  InitHVec<TH1F>(HGenPartF,"HGenPartF",BinsPdgId,PdgIdMin,PdgIdMax);

  HLog = new TH1F("HLog","",50,0,50.); /* Limits are meaningless here */
  fOutput->Add(HLog);

#endif
  HCutFlow = new TH1D("HCutFlow","",50,0.,50.);  /* Limits are meaningless here */
  fOutput->Add(HCutFlow);

  const Int_t BinsLep = 10;
  const Float_t MinLep = 0.;
  const Float_t MaxLep = 10.;

  InitHVec<TH2I>(HNLep,"HNLep",BinsLep,MinLep,MaxLep,BinsLep,MinLep,MaxLep);

  HNEl = new TH2I("HNEl","",BinsLep,MinLep,MaxLep,
                  BinsLep,MinLep,MaxLep);
  HNMu = new TH2I("HNMu","",BinsLep,MinLep,MaxLep,
                  BinsLep,MinLep,MaxLep);

  fOutput->Add(HNEl);
  fOutput->Add(HNMu);

  const Float_t MaxPt = 1000;

  HPtl1 = new TH1F("HPtl1","Z Leading lepton Pt",
                   MetBins,MinMet,MaxPt);
  fOutput->Add(HPtl1);

  HPtl2 = new TH1F("HPtl2","Z Subleading lepton Pt",
                   MetBins,MinMet,MaxPt);
  fOutput->Add(HPtl2);

  HPtl3 = new TH1F("HPtl3","W lepton pt",
                     MetBins,MinMet,MaxPt);
  fOutput->Add(HPtl3);

  HMetPt = new TH1F("HMetPt","MET Pt",
                    MetBins,MinMet,MaxMet);
  fOutput->Add(HMetPt);

  const Float_t MaxEta = 3.;
  const Int_t EtaBins = 100;

  HEtal1 = new TH1F("HEtal1","Eta l1",
                    EtaBins,-1*MaxEta,MaxEta);
  fOutput->Add(HEtal1);

  HEtal2 = new TH1F("HEtal2","Eta l2",
                    EtaBins,-1*MaxEta,MaxEta);
  fOutput->Add(HEtal2);

  HEtal3 = new TH1F("HEtal3","Eta l3",
                    EtaBins,-1*MaxEta,MaxEta);
  fOutput->Add(HEtal3);

  HPtEtal1 = new TH2F("HPtEtal1","PtEta l1",
                      MetBins,0, MaxPt,
                      EtaBins,-1*MaxEta,MaxEta);
  fOutput->Add(HPtEtal1);
  HPtEtal2 = new TH2F("HPtEtal2","PtEta l2",
                      MetBins,0, MaxPt,
                      EtaBins,-1*MaxEta,MaxEta);
  fOutput->Add(HPtEtal2);
  HPtEtal3 = new TH2F("HPtEtal3","PtEta l3",
                      MetBins,0, MaxPt,
                      EtaBins,-1*MaxEta,MaxEta);
  fOutput->Add(HPtEtal3);

  const Float_t MaxPhi = TMath::Pi();
  const Int_t PhiBins = 50;

  HPhil1 = new TH1F("HPhil1","Phi l1",
                    PhiBins,-1*MaxPhi,MaxPhi);
  fOutput->Add(HPhil1);

  HPhil2 = new TH1F("HPhil2","Phi l2",
                    PhiBins,-1*MaxPhi,MaxPhi);
  fOutput->Add(HPhil2);

  HPhil3 = new TH1F("HPhil3","Phi l3",
                    PhiBins,-1*MaxPhi,MaxPhi);
  fOutput->Add(HPhil3);

  HMetPhi = new TH1F("HMetPhi","MET Phi",
                     PhiBins,-1*MaxPhi,MaxPhi);
  fOutput->Add(HMetPhi);


#ifndef CMSDATA

  if (fInput->FindObject("SampleName")) {
    // Lesson: TString can't be in TCollection
    TNamed *p = dynamic_cast<TNamed *>(fInput->FindObject("SampleName"));
    SampleName = p->GetTitle();
  }

  if(fInput->FindObject("SFDb")){
    SFDb = dynamic_cast<TList*>(fInput->FindObject("SFDb"));
  }

#ifdef Y2016
  SFElectronTrigger1 = static_cast<TGraphAsymmErrors*>(SFDb->FindObject("SFElectronTrigger1"));
  SFElectronTrigger2 = static_cast<TGraphAsymmErrors*>(SFDb->FindObject("SFElectronTrigger2"));
  SFMuonTriggerBF = static_cast<TH2F*>(SFDb->FindObject("SFMuonTriggerBF"));
  SFMuonTriggerGH = static_cast<TH2F*>(SFDb->FindObject("SFMuonTriggerGH"));
  SFMuonIDBF = static_cast<TH2D*>(SFDb->FindObject("SFMuonIDBF"));
  SFMuonIDGH = static_cast<TH2D*>(SFDb->FindObject("SFMuonIDGH"));
  auto l = static_cast<TList*>(SFDb->FindObject("PileupSFList"));
  SFPileup = static_cast<TH1D*>(l->FindObject(Form("%s_2016",SampleName.Data())));
#elif defined(Y2017)
  SFElectronTrigger1 = static_cast<TGraphAsymmErrors*>(SFDb->FindObject("SFElectronTrigger1"));
  SFElectronTrigger2 = static_cast<TGraphAsymmErrors*>(SFDb->FindObject("SFElectronTrigger2"));
  SFMuonTrigger = static_cast<TH2F*>(SFDb->FindObject("SFMuonTrigger"));
  SFMuonID = static_cast<TH2D*>(SFDb->FindObject("SFMuonID"));
  auto l = static_cast<TList*>(SFDb->FindObject("PileupSFList"));
  SFPileup = static_cast<TH1D*>(l->FindObject(Form("%s_2017",SampleName.Data())));
#elif defined(Y2018)
  SFElectronTrigger1 = static_cast<TGraphAsymmErrors*>(SFDb->FindObject("SFElectronTrigger1"));
  SFElectronTrigger2 = static_cast<TGraphAsymmErrors*>(SFDb->FindObject("SFElectronTrigger2"));
  SFMuonTrigger = static_cast<TH2F*>(SFDb->FindObject("SFMuonTrigger"));
  SFMuonID = static_cast<TH2D*>(SFDb->FindObject("SFMuonID"));
  auto l = static_cast<TList*>(SFDb->FindObject("PileupSFList"));
  SFPileup = static_cast<TH1D*>(l->FindObject(Form("%s_2018",SampleName.Data())));
#endif
  if(!SFPileup)
    std::clog << Form("WARNING: Pileup %s SF histogram not found!\nPileup weight will be taken as 1.\n",SampleName.Data());


  HScaleFactors = new TH1F("HScaleFactors","HScaleFactors",60,0.,6.);
  fOutput->Add(HScaleFactors);
#endif

}

std::vector<UInt_t> PreSelector::GetGoodMuon(const Muons& Mu){
  std::vector<UInt_t> GoodIndex = {};
  if(!MuonTest()) return GoodIndex;
  if(*Mu.n == 0) return GoodIndex;
  if(Mu.pt[0] < 52.)  /* HLT_Mu50_OR_HLT_TkMu50 lower pt limit from SFDB*/
    return GoodIndex;
  const Float_t MaxEta = 2.4;
  const Float_t MinPt = 20.;
  if(abs(Mu.eta[0]) >= MaxEta){
    HCutFlow->FillS("LeadingMuOut");
    return GoodIndex;
  }
  GoodIndex.reserve(10);
  for (UInt_t i=0; i<*Mu.n;i++){
    if( Muon_highPtId[i] >=1 && Mu.pt[i]>MinPt && abs(Mu.eta[i])<MaxEta)
      GoodIndex.emplace_back(i);
  }
  return GoodIndex;
};

std::vector<UInt_t> PreSelector::GetGoodElectron(const Electrons& El){
  const Float_t MaxEta = 2.5;

#if defined(Y2016)
  const Float_t MinPt = 27.;
#elif defined(Y2017)
  const Float_t MinPt = 35.;
#elif defined(Y2018)
  const Float_t MinPt = 32.;
#endif

  std::vector<UInt_t> GoodIndex = {};
  if(!ElectronTest()) return GoodIndex;
  if(*El.n == 0) return GoodIndex; /*Photon Trigger*/

  if( El.pt[0] < MinPt ) return GoodIndex;

  if(abs(El.eta[0]) >= MaxEta){
    HCutFlow->FillS("LeadingElOut");
    return GoodIndex;
  }
  GoodIndex.reserve(10);

  UInt_t index = 0;
  for (UInt_t i = 0; i< *El.n; i++){
    if(El.cutBased[i]==4 && El.pt[i]>MinPt &&
       abs(El.eta[i])<MaxEta)
      GoodIndex.emplace_back(i);
  }
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


std::pair<UInt_t,UInt_t> PreSelector::GetLeptonPair(const Leptons& l,
                                                    const std::vector<UInt_t>& GoodIndex) const{

  // return leading pair sorted by Pt

  static const UInt_t size = 5;

  std::vector<UInt_t> positive;
  positive.reserve(size);
  std::vector<UInt_t> negative;
  negative.reserve(size);

  for (const UInt_t& lepIdx : GoodIndex) {
    if (l.charge[lepIdx] == 1) {
      positive.emplace_back(lepIdx);
    } else {
      negative.emplace_back(lepIdx);
    }
  }

  std::pair<UInt_t,UInt_t> couple;

  if(positive.size() == 0 || negative.size() == 0)
    return couple; /*empty*/

  if(l.pt[positive[0]] > l.pt[negative[0]]){
    couple = std::make_pair(positive[0],negative[0]);
  } else {
    couple = std::make_pair(negative[0],positive[0]);
  }

  return couple;

}



ZPairInfo PreSelector::FindZ(const Leptons& l, const std::vector<UInt_t>& GoodLepton) const{

  std::pair<UInt_t,UInt_t> Pair ;

  Pair = PreSelector::GetLeptonPair(l,GoodLepton);

  // ZMassDistance, ZMass, Pair
  ZPairInfo ZMassTuple;

  if(Pair.first == Pair.second) /*empty*/
    return ZMassTuple;

  const Double_t ZNominalMass = 91.1876;

  UInt_t i = Pair.first;
  UInt_t j = Pair.second;
  Float_t m = PreSelector::MassRecoZ(l.pt[i],l.eta[i],l.phi[i],l.mass,
                                     l.pt[j],l.eta[j],l.phi[j],l.mass);

  ZMassTuple.Delta = abs(ZNominalMass-m);
  ZMassTuple.Mass = m;
  ZMassTuple.Pair = std::make_pair(i,j);

  return ZMassTuple;

}

#ifndef CMSDATA
std::pair<Int_t,Int_t> PreSelector::GetMother(std::pair<Int_t,Int_t> Daughter) const{
  return PreSelector::GetMother(Daughter.first,Daughter.second);
}
#endif
#ifndef CMSDATA
std::pair<Int_t,Int_t> PreSelector::GetMother(Int_t GenPartIdx, Int_t PdgId /*\Mu or e*/) const{

  if(GenPartIdx == -1 ) return std::make_pair(GenPartIdx,9999);

  std::pair<Int_t,Int_t> Mother;
  Mother.first = GenPart_genPartIdxMother[GenPartIdx];
  Mother.second = GenPart_pdgId[Mother.first];

  if(Mother.second == PdgId){
    Mother = PreSelector::GetMother(Mother);
  }

  return Mother;
}
#endif

std::vector<ROOT::Math::PxPyPzMVector> PreSelector::GetNu4VFix(const ROOT::Math::PtEtaPhiMVector& lep,
                                                               const Float_t& Wmt){

  std::vector<ROOT::Math::PxPyPzMVector> s;
  Float_t pz = (lep.Pz()/lep.Pt())*(*MET_pt);
  s.emplace_back(Get4V(pz));
  return s;

}

std::vector<ROOT::Math::PxPyPzMVector> PreSelector::GetNu4V(const ROOT::Math::PtEtaPhiMVector& lep,
                                                            const Float_t& Wmt){

  std::vector<ROOT::Math::PxPyPzMVector> s;
  Float_t NuPz = 0.;
  const Float_t Mw = 80.379;
  Float_t a = 2.*lep.Pt()*(*MET_pt);
  Float_t k = Mw*Mw - Wmt*Wmt;
  Float_t b = (k + a);
  Float_t c = k*(k + 4.*lep.Pt()*(*MET_pt));
  if (c<0) return GetNu4VFix(lep,Wmt);
  Float_t d = lep.P()*TMath::Sqrt(c);
  NuPz = (lep.Pz()*b - d)/(2.*lep.Pt()*lep.Pt());
  s.emplace_back(Get4V(NuPz));
  NuPz = (lep.Pz()*b + d)/(2.*lep.Pt()*lep.Pt());
  s.emplace_back(Get4V(NuPz));
  return s;

}

ROOT::Math::PxPyPzMVector PreSelector::Get4V(const Float_t& Pz){

  const Float_t MNu = 0.;

  return ROOT::Math::PxPyPzMVector((*MET_pt)*TMath::Cos(*MET_phi),
                                   (*MET_pt)*TMath::Sin(*MET_phi),
                                   Pz,MNu);

}

std::vector<ROOT::Math::PxPyPzMVector> PreSelector::GetNu4VAlt(ROOT::Math::PtEtaPhiMVector lep,
                                                               Float_t Wmt){
  const Float_t Mw = 80.379;
  const Float_t MNu = 0.;

  Float_t dphi = *MET_phi-lep.Phi();
  Float_t a,b;

  Float_t pz = 0.;

  std::vector<ROOT::Math::PxPyPzMVector> s;

  a = pow(Mw,2) + 2.*lep.Pt()*(*MET_pt)*TMath::Cos(dphi);
  b = pow(a,2) - 4*pow(lep.Pt(),2)*pow((*MET_pt),2);

  if (b < 0) return GetNu4VFix(lep, Wmt);

  pz = lep.Pz()*a + lep.P()*TMath::Sqrt(b);
  pz = pz/(2.*pow(lep.Pt(),2.));

  s.emplace_back(PreSelector::Get4V(pz));

  pz = lep.Pz()*a - lep.P()*TMath::Sqrt(b);
  pz = pz/(2.*pow(lep.Pt(),2));

  s.emplace_back(PreSelector::Get4V(pz));


  return s;

}


Float_t PreSelector::GetEtaPhiDistance(const float& eta1, const float& phi1,
                                        const float& eta2, const float& phi2) const{
  Double_t dphi = TVector2::Phi_mpi_pi(phi1-phi2);
  return sqrt(pow(eta2-eta1,2.)+pow(dphi,2.));
}

void PreSelector::FillCommon(const Leptons& lz,const Leptons& lw){
  HWZPtDist->Fill((wb+zb).Pt(),GetEtaPhiDistance(wb.Eta(),wb.Phi(),zb.Eta(),zb.Phi()));
  HWZPt->Fill((wb+zb).Pt());
  HWZDist->Fill(GetEtaPhiDistance(wb.Eta(),wb.Phi(),zb.Eta(),zb.Phi()));
  HPtl1->Fill(lz.pt[l1]);
  HPtl2->Fill(lz.pt[l2]);
  HPtl3->Fill(lw.pt[l3]);
  HEtal1->Fill(lz.eta[l1]);
  HEtal2->Fill(lz.eta[l2]);
  HEtal3->Fill(lw.eta[l3]);
  HPtEtal1->Fill(lz.pt[l1],lz.eta[l1]);
  HPtEtal2->Fill(lz.pt[l2],lz.eta[l2]);
  HPtEtal3->Fill(lz.pt[l3],lz.eta[l3]);
  HPhil1->Fill(lz.phi[l1]);
  HPhil2->Fill(lz.phi[l2]);
  HPhil3->Fill(lw.phi[l3]);

  HnJet->Fill(*nJet);

  HDistl1l3->Fill(GetEtaPhiDistance(lz.eta[l1],lz.phi[l1],
                                      lw.eta[l3],lw.phi[l3]));
  HDistl2l3->Fill(GetEtaPhiDistance(lz.eta[l2],lz.phi[l2],
                                      lw.eta[l3],lw.phi[l3]));
  HMetPt->Fill(*MET_pt);
  HMetPhi->Fill(*MET_phi);

  HMassZTW->Fill(zb.M(),wmt);
  HDeltaRPtZ->Fill(GetEtaPhiDistance(lep1.Eta(),lep1.Eta(),
                                     lep3.Phi(),lep3.Phi()),zb.Pt());
  HPtWPtZ->Fill(wb.Pt(),zb.Pt());
  HDeltaRMWZ->Fill(GetEtaPhiDistance(lep1.Eta(),lep1.Eta(),
                                     lep3.Phi(),lep3.Phi()),(wb+zb).M());
  HLtMWZ->Fill(lep1.Pt()+lep2.Pt()+lep3.Pt(),(wb+zb).M());
}

void PreSelector::FillA(){
  Float_t lt = lep1.Pt()+lep2.Pt()+lep3.Pt();
  HNLep[0]->Fill(GoodMuon.size(),GoodElectron.size());
  HnEl[0]->Fill(GoodElectron.size());
  HnMu[0]->Fill(GoodMuon.size());
  HMassZWZ[0]->Fill(PairZMass,(wb+zb).M());
  HCutFlow->FillS("3e0mu");

  HDistl1l2[0]->Fill(GetEtaPhiDistance(lep1.Pt(),lep1.Phi(),
                                     lep2.Pt(),lep2.Phi()));


#ifndef CMSDATA
  Double_t wup = 1.;
  Double_t wdown = 1.;

  wup = GetElectronSF(Electron_eta[0], Electron_pt[0],1);
  if (wup < 0.) HLog->FillS("ElectronSFUpFail_A");
  wup *= GetSFFromHisto(SFPileup,*PV_npvs);

  wdown = GetElectronSF(Electron_eta[0], Electron_pt[0],-1);
  if (wdown < 0.) HLog->FillS("ElectronSFDownFail_A");
  wdown *= GetSFFromHisto(SFPileup,*PV_npvs);

  HScaleFactors->Fill(wup);
  HScaleFactors->Fill(wdown);
  HGenPartF[0]->FillS(Form("%d",GenPart_pdgId[Electron_genPartIdx[l1]]));
  HGenPartF[0]->FillS(Form("%d",GenPart_pdgId[Electron_genPartIdx[l2]]));
  HGenPartF[0]->FillS(Form("%d",GenPart_pdgId[Electron_genPartIdx[l3]]));

  HGenPartZ[0]->FillS(Form("%d",GetMother(Electron_genPartIdx[l1],
                                       Electron_pdgId[l1]).second));
  HGenPartZ[0]->FillS(Form("%d",GetMother(Electron_genPartIdx[l2],
                                        Electron_pdgId[l2]).second));
  HGenPartW[0]->FillS(Form("%d",GetMother(Electron_genPartIdx[l3],
                                        Electron_pdgId[l3]).second));
  /* Default to 1. if it fails */
  if( wup < 0 ) wup = 1.;
  if( wdown < 0 ) wdown = 1.;

  HPileup_[0+4]->Fill(*PV_npvs,wup);
  HMet_[0+4]->Fill(*MET_pt,wup);
  HMassW[0+4]->Fill(wb.M(),wup);
  HMassWZ[0+4]->Fill((wb+zb).M(),wup);
  HLt[0+4]->Fill(lt,wup);
  HMassZ_[0+4]->Fill(PairZMass,wup);
  HMassTW_[0+4]->Fill(wmt,wup);

  HPileup_[0+8]->Fill(*PV_npvs,wdown);
  HMet_[0+8]->Fill(*MET_pt,wdown);
  HMassW[0+8]->Fill(wb.M(),wdown);
  HMassWZ[0+8]->Fill((wb+zb).M(),wdown);
  HLt[0+8]->Fill(lt,wdown);
  HMassZ_[0+8]->Fill(PairZMass,wdown);
  HMassTW_[0+8]->Fill(wmt,wdown);

#endif

  HPileup_[0]->Fill(*PV_npvs);
  HMet_[0]->Fill(*MET_pt);
  HMassW[0]->Fill(wb.M());
  HMassWZ[0]->Fill((wb+zb).M());
  HMassZ_[0]->Fill(PairZMass);
  HMassTW_[0]->Fill(wmt);
  HLt[0]->Fill(lt);
}

void PreSelector::FillB(){
  Float_t lt = lep1.Pt()+lep2.Pt()+lep3.Pt();
  HNLep[1]->Fill(GoodMuon.size(),GoodElectron.size());
  HnEl[1]->Fill(GoodElectron.size());
  HnMu[1]->Fill(GoodMuon.size());
  HMassZWZ[1]->Fill(PairZMass,(wb+zb).M());
  HCutFlow->FillS("2e1mu");

  HDistl1l2[1]->Fill(GetEtaPhiDistance(lep1.Pt(),lep1.Phi(),
                                     lep2.Pt(),lep2.Phi()));


#ifndef CMSDATA
  Double_t wup = 1.;
  Double_t wdown = 1.;

  wup = GetElectronSF(Electron_eta[0],Electron_pt[0],1);
  if (wup < 0.) HLog->FillS("ElectronSFUpFail_B");
  wup *= GetMuonSF(Muon_eta[0],Muon_pt[0],1);
  if (wup < 0.) HLog->FillS("MuonSFUpFail_B");
  wup *= GetSFFromHisto(SFPileup,*PV_npvs);

  wdown = GetElectronSF(Electron_eta[0],Electron_pt[0],-1);
  if (wdown < 0.) HLog->FillS("ElectronSFDownFail_B");
  wdown *= GetMuonSF(Muon_eta[0],Muon_pt[0],-1);
  if (wdown < 0.) HLog->FillS("MuonSFDownFail_B");
  wdown *= GetSFFromHisto(SFPileup,*PV_npvs);

  HScaleFactors->Fill(wup);
  HScaleFactors->Fill(wdown);
  HGenPartF[1]->FillS(Form("%d",GenPart_pdgId[Electron_genPartIdx[l1]]));
  HGenPartF[1]->FillS(Form("%d",GenPart_pdgId[Electron_genPartIdx[l2]]));
  HGenPartF[1]->FillS(Form("%d",GenPart_pdgId[Muon_genPartIdx[l3]]));

  HGenPartZ[1]->FillS(Form("%d",GetMother(Electron_genPartIdx[l1],
                                       Electron_pdgId[l1]).second));
  HGenPartZ[1]->FillS(Form("%d",GetMother(Electron_genPartIdx[l2],
                                       Electron_pdgId[l2]).second));
  HGenPartW[1]->FillS(Form("%d",GetMother(Muon_genPartIdx[l3],
                                       Muon_pdgId[l3]).second));
  /* Default to 1. if it fails */
  if( wup < 0 ) wup = 1.;
  if( wdown < 0 ) wdown = 1.;

  HPileup_[1+4]->Fill(*PV_npvs,wup);
  HMet_[1+4]->Fill(*MET_pt,wup);
  HMassW[1+4]->Fill(wb.M(),wup);
  HMassWZ[1+4]->Fill((wb+zb).M(),wup);
  HLt[1+4]->Fill(lt,wup);
  HMassZ_[1+4]->Fill(PairZMass,wup);
  HMassTW_[1+4]->Fill(wmt,wup);

  HPileup_[1+8]->Fill(*PV_npvs,wdown);
  HMet_[1+8]->Fill(*MET_pt,wdown);
  HMassW[1+8]->Fill(wb.M(),wdown);
  HMassWZ[1+8]->Fill((wb+zb).M(),wdown);
  HLt[1+8]->Fill(lt,wdown);
  HMassZ_[1+8]->Fill(PairZMass,wdown);
  HMassTW_[1+8]->Fill(wmt,wdown);
#endif
  HPileup_[1]->Fill(*PV_npvs);
  HMet_[1]->Fill(*MET_pt);
  HMassW[1]->Fill(wb.M());
  HMassWZ[1]->Fill((wb+zb).M());
  HMassZ_[1]->Fill(PairZMass);
  HMassTW_[1]->Fill(wmt);
  HLt[1]->Fill(lt);
}

void PreSelector::FillC(){
  Float_t lt = lep1.Pt()+lep2.Pt()+lep3.Pt();
  HNLep[2]->Fill(GoodMuon.size(),GoodElectron.size());
  HnEl[2]->Fill(GoodElectron.size());
  HnMu[2]->Fill(GoodMuon.size());
  HMassZWZ[2]->Fill(PairZMass,(wb+zb).M());
  HCutFlow->FillS("1e2mu");

  HDistl1l2[2]->Fill(GetEtaPhiDistance(lep1.Pt(),lep1.Phi(),
                                     lep2.Pt(),lep2.Phi()));


#ifndef CMSDATA
  Double_t wup = 1.;
  Double_t wdown = 1.;

  wup = GetMuonSF(Muon_eta[0],Muon_pt[0],1);
  if (wup < 0.) HLog->FillS("MuonSFUpFail_C");
  wup *= GetElectronSF(Electron_eta[0],Electron_pt[0],1);
  if (wup < 0.) HLog->FillS("ElectronSFUpFail_C");
  wup *= GetSFFromHisto(SFPileup,*PV_npvs);

  wdown = GetMuonSF(Muon_eta[0],Muon_pt[0],-1);
  if (wdown < 0.) HLog->FillS("MuonSFDownFail_C");
  wdown *= GetElectronSF(Electron_eta[0],Electron_pt[0],-1);
  if (wdown < 0.) HLog->FillS("ElectronSFDownFail_C");
  wdown *= GetSFFromHisto(SFPileup,*PV_npvs);

  HScaleFactors->Fill(wup);
  HScaleFactors->Fill(wdown);
  HGenPartF[2]->FillS(Form("%d",GenPart_pdgId[Muon_genPartIdx[l1]]));
  HGenPartF[2]->FillS(Form("%d",GenPart_pdgId[Muon_genPartIdx[l2]]));
  HGenPartF[2]->FillS(Form("%d",GenPart_pdgId[Electron_genPartIdx[l3]]));

  HGenPartZ[2]->FillS(Form("%d",GetMother(Muon_genPartIdx[l1],
                                        Muon_pdgId[l1]).second));
  HGenPartZ[2]->FillS(Form("%d",GetMother(Muon_genPartIdx[l2],
                                        Muon_pdgId[l2]).second));
  HGenPartW[2]->FillS(Form("%d",GetMother(Electron_genPartIdx[l3],
                                        Electron_pdgId[l3]).second));

  /* Default to 1. if it fails */
  if( wup < 0 ) wup = 1.;
  if( wdown < 0 ) wdown = 1.;

  HPileup_[2+4]->Fill(*PV_npvs,wup);
  HMet_[2+4]->Fill(*MET_pt,wup);
  HMassW[2+4]->Fill(wb.M(),wup);
  HMassWZ[2+4]->Fill((wb+zb).M(),wup);
  HLt[2+4]->Fill(lt,wup);
  HMassZ_[2+4]->Fill(PairZMass,wup);
  HMassTW_[2+4]->Fill(wmt,wup);

  HPileup_[2+8]->Fill(*PV_npvs,wdown);
  HMet_[2+8]->Fill(*MET_pt,wdown);
  HMassW[2+8]->Fill(wb.M(),wdown);
  HMassWZ[2+8]->Fill((wb+zb).M(),wdown);
  HLt[2+8]->Fill(lt,wdown);
  HMassZ_[2+8]->Fill(PairZMass,wdown);
  HMassTW_[2+8]->Fill(wmt,wdown);
#endif
  HPileup_[2]->Fill(*PV_npvs);
  HMet_[2]->Fill(*MET_pt);
  HMassW[2]->Fill(wb.M());
  HMassWZ[2]->Fill((wb+zb).M());
  HMassZ_[2]->Fill(PairZMass);
  HMassTW_[2]->Fill(wmt);
  HLt[2]->Fill(lt);
}

bool PreSelector::DefineW(Leptons l){

  assert(l.pt[l3] > 20.); // MinPt

  lep3 = PtEtaPhiMVector(l.pt[l3], l.eta[l3], l.phi[l3], l.mass);
  wmt = PreSelector::MassRecoW(lep3.Pt(), lep3.Phi(), *MET_pt, *MET_phi);
  nu = PreSelector::GetNu4V(lep3, wmt);
  wb = (lep3 + nu[0]);

  return true;

}

void PreSelector::FillD(){
  Float_t lt = lep1.Pt()+lep2.Pt()+lep3.Pt();
  HNLep[3]->Fill(GoodMuon.size(),GoodElectron.size());
  HnEl[3]->Fill(GoodElectron.size());
  HnMu[3]->Fill(GoodMuon.size());
  HMassZWZ[3]->Fill(PairZMass,(wb+zb).M());
  HCutFlow->FillS("0e3mu");

  HDistl1l2[3]->Fill(GetEtaPhiDistance(lep1.Pt(),lep1.Phi(),
                                     lep2.Pt(),lep2.Phi()));

#ifndef CMSDATA
  Double_t wup = 1.;
  Double_t wdown = 1.;

  wup = GetMuonSF(Muon_eta[0],Muon_pt[0],1);
  if (wup < 0.) HLog->FillS("MuonSFUpFail_D");
  wup *= GetSFFromHisto(SFPileup,*PV_npvs);

  wdown = GetMuonSF(Muon_eta[0],Muon_pt[0],-1);
  if (wdown < 0.) HLog->FillS("MuonSFUpFail_D");
  wdown *= GetSFFromHisto(SFPileup,*PV_npvs);
  HScaleFactors->Fill(wup);
  HScaleFactors->Fill(wdown);
  HGenPartF[3]->FillS(Form("%d",GenPart_pdgId[Muon_genPartIdx[l1]]));
  HGenPartF[3]->FillS(Form("%d",GenPart_pdgId[Muon_genPartIdx[l2]]));
  HGenPartF[3]->FillS(Form("%d",GenPart_pdgId[Muon_genPartIdx[l3]]));

  HGenPartZ[3]->FillS(Form("%d",GetMother(Muon_genPartIdx[l1],
                                        Muon_pdgId[l1]).second));
  HGenPartZ[3]->FillS(Form("%d",GetMother(Muon_genPartIdx[l2],
                                        Muon_pdgId[l2]).second));
  HGenPartW[3]->FillS(Form("%d",GetMother(Muon_genPartIdx[l3],
                                        Muon_pdgId[l3]).second));

  /* Default to 1. if it fails */
  if( wup < 0 ) wup = 1.;
  if( wdown < 0 ) wdown = 1.;

  HPileup_[3+4]->Fill(*PV_npvs,wup);
  HMet_[3+4]->Fill(*MET_pt,wup);
  HMassW[3+4]->Fill(wb.M(),wup);
  HMassWZ[3+4]->Fill((wb+zb).M(),wup);
  HLt[3+4]->Fill(lt,wup);
  HMassZ_[3+4]->Fill(PairZMass,wup);
  HMassTW_[3+4]->Fill(wmt,wup);

  HPileup_[3+8]->Fill(*PV_npvs,wdown);
  HMet_[3+8]->Fill(*MET_pt,wdown);
  HMassW[3+8]->Fill(wb.M(),wdown);
  HMassWZ[3+8]->Fill((wb+zb).M(),wdown);
  HLt[3+8]->Fill(lt,wdown);
  HMassZ_[3+8]->Fill(PairZMass,wdown);
  HMassTW_[3+8]->Fill(wmt,wdown);

#endif
  HPileup_[3]->Fill(*PV_npvs);
  HMet_[3]->Fill(*MET_pt);
  HMassW[3]->Fill(wb.M());
  HMassWZ[3]->Fill((wb+zb).M());
  HMassZ_[3]->Fill(PairZMass);
  HMassTW_[3]->Fill(wmt);
  HLt[3]->Fill(lt);
}

Bool_t PreSelector::CheckElectronPair(const std::pair<UInt_t,UInt_t>& p) const{

#if defined(Y2016)
  const Float_t MinPt = 27.;
#elif defined(Y2017)
  const Float_t MinPt = 35.;
#elif defined(Y2018)
  const Float_t MinPt = 32.;
#endif

  if (Electron_pt[p.first] < MinPt || Electron_pt[p.second] < MinPt) return kFALSE;
  return kTRUE;
}

Bool_t PreSelector::CheckMuonPair(const std::pair<UInt_t,UInt_t>& p) const{
  const Float_t MinSubleadPt = 10.;
  if (Muon_pt[p.second] < MinSubleadPt) return kFALSE;
  return kTRUE;
}

Bool_t PreSelector::Process(Long64_t entry) {

  ReadEntry(entry);

  HCutFlow->FillS("NoCuts");
#ifndef CMSDATA
  HCutFlow->Fill("genWeight",*genWeight);
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
  if (*MET_pt < 30.){
    HCutFlow->FillS("MET_pt<30");
    return kFALSE;
  }

  if(!MinLeptonsTest()) {
    HCutFlow->FillS("lep<3");
    return kFALSE;
  }

  Muons Mus(nMuon,Muon_pt,Muon_eta,Muon_phi,
            Muon_charge,Muon_dxy,Muon_dz,
            Muon_tightId);

  Electrons Els(nElectron,Electron_pt,Electron_eta,Electron_phi,
                Electron_charge,Electron_dxy,Electron_dz,
                Electron_cutBased);

  GoodElectron = PreSelector::GetGoodElectron(Els);
  GoodMuon = PreSelector::GetGoodMuon(Mus);

  HNEl->Fill(*nElectron,GoodElectron.size());
  HNMu->Fill(*nMuon,GoodMuon.size());

  if( (GoodElectron.size() + GoodMuon.size()) <3 ){
    HCutFlow->FillS("goodLep<3");
    return kFALSE;
  }


  Bool_t IsA{},IsB{},IsC{},IsD{};
  Bool_t PairEl{}, PairMu{};

  const Float_t MinZMass = 70.;
  const Float_t MaxZMass = 111.;

  ZPairInfo *zt = nullptr;
  ZPairInfo ztel;
  ZPairInfo ztmu;

  ztmu = FindZ(Mus,GoodMuon);
  if(!ztmu.empty() && CheckMuonPair(ztmu.Pair)) PairMu = true;

  ztel = FindZ(Els,GoodElectron);
  if(!ztel.empty() && CheckElectronPair(ztel.Pair)) PairEl = true;

  if (!PairEl && !PairMu) {
    HCutFlow->FillS("NoPairs");
    return kFALSE;
  }

  if (PairEl && PairMu) {
    if (ztel.Delta > ztmu.Delta) {
      PairEl = false;
    } else {
      PairMu = false;
    }
  }

  if(PairEl){
    zt = &ztel;
  } else { //PairMu
    zt = &ztmu;
  }

  if(PairMu) HCutFlow->FillS("PairMu");
  if(PairEl) HCutFlow->FillS("PairEl");

  std::vector<UInt_t> WCand;

  PairZMass = (*zt).Mass;
  Bool_t IsZMassOk = (PairZMass > MinZMass) && (PairZMass < MaxZMass);
  if(!IsZMassOk){
    HCutFlow->FillS("FailZMassWindow");
    return kFALSE;
  }

  l1 = (*zt).Pair.first;
  l2 = (*zt).Pair.second;

  if(PairEl){
    lep1 = PtEtaPhiMVector(Electron_pt[l1],Electron_eta[l1],
                           Electron_phi[l1],Electrons::mass);
    lep2 = PtEtaPhiMVector(Electron_pt[l2],Electron_eta[l2],
                           Electron_phi[l2],Electrons::mass);
  } else { //PairMu
    lep1 = PtEtaPhiMVector(Muon_pt[l1],Muon_eta[l1],
                           Muon_phi[l1],Muons::mass);
    lep2 = PtEtaPhiMVector(Muon_pt[l2],Muon_eta[l2],
                           Muon_phi[l2],Muons::mass);
  }

  zb   = lep1 + lep2;

  Bool_t Zl1PtCut = lep1.Pt() < 50.;
  if(Zl1PtCut){
    HCutFlow->FillS("FailZl1PtCut");
    return kFALSE;
  }

  const float_t l1l2Dist = GetEtaPhiDistance(lep1.Eta(),lep1.Phi(),lep2.Eta(),lep2.Phi());
  Bool_t ZDistCut = l1l2Dist > 1.5;
  if(ZDistCut){
    HCutFlow->FillS("FailZDistCut");
    return kFALSE;
  }

  if(PairMu){

    Bool_t GlobalHighPtl1 = (Muon_highPtId[l1] == 2);
    Bool_t GlobalHighPtl2 = (Muon_highPtId[l2] == 2);

    if (!(GlobalHighPtl1 or GlobalHighPtl2)) {
      HCutFlow->FillS("FailZGlbHighPt");
      return kFALSE;
    } else {
      if (GlobalHighPtl1 and !Muon_isPFcand[l1]) {
        HCutFlow->FillS("FailZGlbHighPt&PFCand");
        return kFALSE;
      } else if (GlobalHighPtl2 and !Muon_isPFcand[l2]) {
        HCutFlow->FillS("FailZGlbHighPt&PFCand");
        return kFALSE;
      }
    }

    for(auto i: GoodMuon){
      if(i!=l1 && i!=l2)
        if(Muon_highPtId[i] == 2)
          WCand.emplace_back(i);
    }

    if(WCand.size() > 0) {
      if(GoodElectron.size() > 0){
        if(Muon_pt[WCand[0]] > Electron_pt[GoodElectron[0]]){
          IsD = true;
          l3 = WCand[0];
        } else {
          IsC = true;
          l3 = GoodElectron[0];
        }
      } else {
        IsD = true;
        l3 = WCand[0];
      }
    } else if (GoodElectron.size()>0) {
      IsC = true;
      l3 = GoodElectron[0];
    }

    if(IsC)
      if(!DefineW(Els))
        return kFALSE;

    if(IsD)
      if(!DefineW(Mus))
        return kFALSE;

#ifndef CMSDATA
    ELPass->Enter(entry);
#endif

    // 0e3mu
    if(IsD){
      if(Muon_highPtId[l3] == 2){
        FillCommon(Mus,Mus);
        FillD();
      } else {
        IsD = false;
        HCutFlow->FillS("FailWMuonGlbHighPt");
      }
    }

    // 1e2Mu
    if(IsC){
      FillCommon(Mus,Els);
      FillC();
    }
  }


  if(PairEl){

    auto WMuonOk = [&](){
      Bool_t ok{};
      for (const auto& i: GoodMuon) {
        if (Muon_highPtId[GoodMuon[i]] == 2) {
          l3 = GoodMuon[i];
          ok = true;
          break;
        }
      }
      return ok;
    };

    for(auto i: GoodElectron){
      if(i!=l1 && i!=l2) WCand.emplace_back(i);
    }

    if(WCand.size() > 0){
      if(GoodMuon.size() > 0){
        if(Muon_pt[GoodMuon[0]] > Electron_pt[WCand[0]] ){
          IsB = true;
          if (!WMuonOk()) {
            HCutFlow->FillS("FailWMuonGlbHighPt");
            return kFALSE;
          }
        } else {
          IsA = true;
          l3 = WCand[0];
        }
      } else {
        IsA = true;
        l3 = WCand[0];
      }
    } else if (GoodMuon.size()>0) {
      IsB = true;
      l3 = GoodMuon[0];
    }

    if (IsA)
      if (!DefineW(Els))
        return kFALSE;
    if (IsB)
      if (!DefineW(Mus))
        return kFALSE;

#ifndef CMSDATA
    ELPass->Enter(entry);
#endif

    // 2e1mu
    if(IsB){
      if(Muon_highPtId[l3] == 2){
        FillCommon(Els,Mus);
        FillB();
      } else {
        IsB = false;
        HCutFlow->FillS("Fail2e1muHighPtId");
      }
    }

    //3e0mu
    if(IsA){
      FillCommon(Els,Els);
      FillA();
    }
  }

  // 3leptons
  HOverlap->Fill(IsA+IsB+IsC+IsD);

  return kTRUE;
}

void PreSelector::Terminate() {

#ifdef Y2016
  const Int_t Year = 2016;
#elif defined(Y2017)
  const Int_t Year = 2017;
#elif defined(Y2018)
  const Int_t Year = 2018;
#endif

  auto getFullPath = [&](const std::string name){
    const std::string dir = "plots";
    return Form("%s/%d/%s_%s.png",dir.c_str(),Year,SampleName.Data(),name.c_str());
  };

#ifndef CMSDATA
  std::unique_ptr<TFile> fELOut(TFile::Open("MCEntryLists.root","UPDATE"));
  ELPass->SetName(SampleName.Data());
  ELPass->Write(SampleName.Data());
  fELOut->Close();
#endif

  gStyle->SetOptStat(1111111);
  std::unique_ptr<TCanvas> ch(new TCanvas("ch","ch",1200,800));
  std::unique_ptr<TCanvas> chc(new TCanvas("chc","chc",1200,800));

  std::unique_ptr<TFile> fOut(TFile::Open("WprimeHistos.root","UPDATE"));
  fOut->mkdir(Form("%d",Year));
  fOut->mkdir(Form("%d/%s",Year,SampleName.Data()));
  fOut->cd(Form("%d/%s",Year,SampleName.Data()));

  TObjLink *lnk = fOutput->FirstLink();
  while (lnk) {
    std::cout << lnk->GetObject()->GetName() << std::endl;
    lnk->GetObject()->Write();
    lnk = lnk->Next();
  }

  ch->cd();
  HCutFlow->LabelsDeflate();
  gPad->SetLogy();
  HCutFlow->Draw("HIST TEXT45");
  ch->Print(getFullPath("HCutFlow"));

#ifndef CMSDATA

  ch->Clear();
  HLog->Draw("HIST TEXT45");
  ch->Print(getFullPath("HLog"));
#endif


  ch->cd(0);
  HOverlap->Draw();
  ch->Print(getFullPath("Overlap"));

  ch->Clear();
  gStyle->SetOptStat(0);
  ch->Divide(2,1);
  ch->cd(1);
  gPad->SetLogz();
  HNEl->SetTitle("nElectron vs size(GoodElectron); nElectron; size(GoodElectron)");
  HNEl->Draw("COL,TEXT");
  ch->cd(2);
  gPad->SetLogz();
  HNMu->SetTitle("nMuon vs size(GoodMuon); nMuon; size(GoodMuon)");
  HNMu->Draw("COL,TEXT");
  ch->Print(getFullPath("CheckGoodLeptons"));

  fOut->Write();
  fOut->Close();

}
