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
  /* 2017/2018 SFMuonHighPtID, SFMuonTrigger provides x(pt) and y(eta)  */
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
  assert(copysign(1.,h->GetBinContent(h->FindBin(npv))) > 0);
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
  assert(sf>0);

  return sf;

}
#endif
#ifndef CMSDATA
Double_t PreSelector::GetKFactor(const Double_t& ZGenPt, const int& option) const{
  assert(ApplyKFactors);
  double sf = 1.;

  if( ZGenPt < KSFMinPt or ZGenPt > KSFMaxPt)
    return sf;

  const int nbinQCD = SFDYKFactorQCD->FindBin(ZGenPt);
  const int nbinEWK = SFDYKFactorEWK->FindBin(ZGenPt);

  sf *= SFDYKFactorQCD->GetBinContent(nbinQCD);
  sf *= SFDYKFactorEWK->GetBinContent(nbinEWK);

  switch(option){
  case -1:
    sf -= SFDYKFactorQCD->GetBinErrorLow(nbinQCD);
    sf -= SFDYKFactorEWK->GetBinErrorLow(nbinEWK);
    break;
  case 1:
    sf += SFDYKFactorQCD->GetBinErrorUp(nbinQCD);
    sf += SFDYKFactorQCD->GetBinErrorUp(nbinEWK);
    break;
  case 0:
    break;
  }

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
  Double_t SFIDBF = GetSFFromHisto(SFMuonHighPtIDBF,eta,pt,option);
  Double_t SFIDGH = GetSFFromHisto(SFMuonHighPtIDGH,eta,pt,option);

  sf = (LumiBF*SFTriggerBF+LumiGH*SFTriggerGH)/(LumiBF+LumiGH);
  sf *=(LumiBF*SFIDBF+LumiGH*SFIDGH)/(LumiBF+LumiGH);

#elif defined(Y2017) || defined(Y2018)
  sf = GetSFFromHisto(SFMuonTrigger,pt,abs(eta),option);
  sf *= GetSFFromHisto(SFMuonHighPtID,pt,abs(eta),option);
#endif
  assert(sf>0);

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
    "A",             "B",             "C",             "D",             /*+4*/
    "Central_A",     "Central_B",     "Central_C",     "Central_D",     /*+8*/
    "Up_A",          "Up_B",          "Up_C",          "Up_D",          /*+12*/
    "Down_A",        "Down_B",        "Down_C",        "Down_D",        /*+16*/
    "CR1_A",         "CR1_B",         "CR1_C",         "CR1_D",         /*+20*/
    "CR1_Central_A", "CR1_Central_B", "CR1_Central_C", "CR1_Central_D", /*+24*/
    "CR1_Up_A",      "CR1_Up_B",      "CR1_Up_C",      "CR1_Up_D",      /*+28*/
    "CR1_Down_A",    "CR1_Down_B",    "CR1_Down_C",    "CR1_Down_D",    /*+32*/
    "CR2_A",         "CR2_B",         "CR2_C",         "CR2_D",         /*+36*/
    "CR2_Central_A", "CR2_Central_B", "CR2_Central_C", "CR2_Central_D", /*+40*/
    "CR2_Up_A",      "CR2_Up_B",      "CR2_Up_C",      "CR2_Up_D",      /*+44*/
    "CR2_Down_A",    "CR2_Down_B",    "CR2_Down_C",    "CR2_Down_D"     /*+46*/
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

  const Int_t DistBins = 30;
  const Float_t MaxDist = 2.01*TMath::Pi();

  InitHVec<TH1F>(HDistl1l2,"HDistl1l2",DistBins,0.,MaxDist);
  InitHVec<TH1F>(HDistl1l3,"HDistl1l3",DistBins,0.,MaxDist);
  InitHVec<TH1F>(HDistl2l3,"HDistl2l3",DistBins,0.,MaxDist);

  InitHVec<TH1F>(HWZDist,"HWZDist",DistBins,0.,MaxDist);
  InitHVec<TH2F>(HWZPtDist,"HWZPtDist",100,0.,1400.,DistBins,0.,MaxDist);
  InitHVec<TH1F>(HWZPt,"HWZPt",60,0.,1e3);

  InitHVec<TH1F>(HDxyl1,"HDxyl1",200,-0.5,0.5);
  InitHVec<TH1F>(HDxyl2,"HDxyl2",200,-0.5,0.5);
  InitHVec<TH1F>(HDxyl3,"HDxyl3",200,-0.5,0.5);

  InitHVec<TH1F>(HDzl1,"HDzl1",200,-0.5,0.5);
  InitHVec<TH1F>(HDzl2,"HDzl2",200,-0.5,0.5);
  InitHVec<TH1F>(HDzl3,"HDzl3",200,-0.5,0.5);

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
  InitHVec<TH1F>(HnbTag,"HnbTag",nJetBins,0.,(float)nJetBins);

  const Float_t MinMass = 0.;
  const Float_t MaxMass = 2200.;
  const Int_t MassBins = 22;

  const Float_t MinWMass = 0.;
  const Float_t MaxWMass = 500;

  InitHVec<TH1F>(HMassW,"HMassW",MassBins,MinWMass,MaxWMass);

  const Float_t HMaxZMass = 120.;
  const Float_t HMinZMass = 60.;
  const Int_t ZMassBins = 30;
  InitHVec<TH1F>(HMassZ,"HMassZ",ZMassBins,HMinZMass,HMaxZMass);

  const Float_t MaxTWMass = 250.;
  const Int_t TWMassBins = 30;
  InitHVec<TH1F>(HMassTW,"HMassTW",TWMassBins,0.,MaxTWMass);

  const Float_t MaxWZMass = 8500.;
  const Int_t WZMassBins = 85;
  InitHVec<TH1F>(HMassWZ,"HMassWZ",WZMassBins,0.,MaxWZMass);

  const Float_t MaxLt = 1000.;
  const Int_t NLtBins = 50;
  InitHVec<TH1F>(HLt,"HLt",NLtBins,0.,MaxLt);

  InitHVec<TH2F>(HMassZWZ,"HMassZWZ",MassBins,0.,1.5*HMaxZMass,MassBins,0.,MaxMass);

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

  const Int_t nPvsBins = 100;
  const Float_t minPvs = 0.;
  const Float_t maxPvs = 50.;

  HPileup = new TH1D("HPileup","PV_npvs",
                     nPvsBins,minPvs,maxPvs);
  fOutput->Add(HPileup);

  HTruePileup = new TH1D("HTruePileup","PV_npvs",
                         nPvsBins,minPvs,maxPvs);

  fOutput->Add(HTruePileup);

  InitHVec<TH1F>(HPileup_,"HPileup",nPvsBins,minPvs,maxPvs);

  const UInt_t BinsPdgId = 100;
  const Float_t PdgIdMin = -50.;
  const Float_t PdgIdMax = 50.;

#ifndef CMSDATA
  InitHVec<TH1F>(HGenPartZ,"HGenPartZ",BinsPdgId,PdgIdMin,PdgIdMax);
  InitHVec<TH1F>(HGenPartW,"HGenPartW",BinsPdgId,PdgIdMin,PdgIdMax);
  InitHVec<TH1F>(HGenPartF,"HGenPartF",BinsPdgId,PdgIdMin,PdgIdMax);
  InitHVec<TH2F>(HGenPartChgF,"HGenPartChgF",6,-2.,2.,BinsPdgId,PdgIdMin,PdgIdMax);

#endif
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

  const Float_t MaxPt = 1000;
  const Double_t MaxMet = 600.;
  const Double_t MinMet = 0.;
  const Int_t MetBins = 60;

  InitHVec<TH1F>(HPtl1,"HPtl1",25,0.,250.);
  InitHVec<TH1F>(HPtl2,"HPtl2",10.,0.,100.);
  InitHVec<TH1F>(HPtl3,"HPtl3",40,0.,400.);
  InitHVec<TH1F>(HMetPt,"HMetPt",MetBins,MinMet,MaxMet);

  const Float_t MaxEta = 3.;
  const Int_t EtaBins = 25;

  InitHVec<TH1F>(HEtal1,"HEtal1",EtaBins,-1*MaxEta,MaxEta);
  InitHVec<TH1F>(HEtal2,"HEtal2",EtaBins,-1*MaxEta,MaxEta);
  InitHVec<TH1F>(HEtal3,"HEtal3",EtaBins,-1*MaxEta,MaxEta);

  InitHVec<TH2F>(HPtEtal1,"HPtEtal1",
                 MetBins,0, MaxPt,
                 EtaBins,-1*MaxEta,MaxEta);

  InitHVec<TH2F>(HPtEtal2,"HPtEtal2",
                 MetBins,0, MaxPt,
                 EtaBins,-1*MaxEta,MaxEta);


  InitHVec<TH2F>(HPtEtal3,"HPtEtal3",
                 MetBins,0, MaxPt,
                 EtaBins,-1*MaxEta,MaxEta);

  const Float_t MaxPhi = TMath::Pi();
  const Int_t PhiBins = 20;

  InitHVec<TH1F>(HPhil1,"HPhil1",PhiBins,-1*MaxPhi,MaxPhi);
  InitHVec<TH1F>(HPhil2,"HPhil2",PhiBins,-1*MaxPhi,MaxPhi);
  InitHVec<TH1F>(HPhil3,"HPhil3",PhiBins,-1*MaxPhi,MaxPhi);
  InitHVec<TH1F>(HMetPhi,"HMetPhi",PhiBins,-1*MaxPhi,MaxPhi);

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
  SFDYKFactorQCD = static_cast<TH1F*>(SFDb->FindObject("SFDYKFactorQCD"));
  SFDYKFactorEWK = static_cast<TH1F*>(SFDb->FindObject("SFDYKFactorEWK"));
  SFElectronTrigger1 = static_cast<TGraphAsymmErrors*>(SFDb->FindObject("SFElectronTrigger1"));
  SFElectronTrigger2 = static_cast<TGraphAsymmErrors*>(SFDb->FindObject("SFElectronTrigger2"));
  SFMuonTriggerBF = static_cast<TH2F*>(SFDb->FindObject("SFMuonTriggerBF"));
  SFMuonTriggerGH = static_cast<TH2F*>(SFDb->FindObject("SFMuonTriggerGH"));
  SFMuonHighPtIDBF = static_cast<TH2D*>(SFDb->FindObject("SFMuonHighPtIDBF"));
  SFMuonHighPtIDGH = static_cast<TH2D*>(SFDb->FindObject("SFMuonHighPtIDGH"));
  auto l = static_cast<TList*>(SFDb->FindObject("PileupSFList"));
  SFPileup = static_cast<TH1D*>(l->FindObject(Form("%s_2016",SampleName.Data())));

  if(SampleName.Contains("DYJetsToLL")){
    ApplyKFactors = true;
    KSFMinPt = SFDYKFactorQCD->GetBinLowEdge(1);
    KSFMaxPt = SFDYKFactorQCD->GetBinLowEdge(SFDYKFactorQCD->GetNbinsX())
      + SFDYKFactorQCD->GetBinWidth(SFDYKFactorQCD->GetNbinsX());
    std::clog << Form("Apply kFactors to sample: %s\n",SampleName.Data());
  }

#elif defined(Y2017)
  SFElectronTrigger1 = static_cast<TGraphAsymmErrors*>(SFDb->FindObject("SFElectronTrigger1"));
  SFElectronTrigger2 = static_cast<TGraphAsymmErrors*>(SFDb->FindObject("SFElectronTrigger2"));
  SFMuonTrigger = static_cast<TH2F*>(SFDb->FindObject("SFMuonTrigger"));
  SFMuonHighPtID = static_cast<TH2D*>(SFDb->FindObject("SFMuonHighPtID"));
  auto l = static_cast<TList*>(SFDb->FindObject("PileupSFList"));
  SFPileup = static_cast<TH1D*>(l->FindObject(Form("%s_2017",SampleName.Data())));
#elif defined(Y2018)
  SFElectronTrigger1 = static_cast<TGraphAsymmErrors*>(SFDb->FindObject("SFElectronTrigger1"));
  SFElectronTrigger2 = static_cast<TGraphAsymmErrors*>(SFDb->FindObject("SFElectronTrigger2"));
  SFMuonTrigger = static_cast<TH2F*>(SFDb->FindObject("SFMuonTrigger"));
  SFMuonHighPtID = static_cast<TH2D*>(SFDb->FindObject("SFMuonHighPtID"));
  auto l = static_cast<TList*>(SFDb->FindObject("PileupSFList"));
  SFPileup = static_cast<TH1D*>(l->FindObject(Form("%s_2018",SampleName.Data())));
#endif
  if(!SFPileup)
    std::clog << Form("WARNING: Pileup %s SF histogram not found!\nPileup weight will be taken as 1.\n",SampleName.Data());

  InitHVec<TH1F>(HScaleFactors,"HScaleFactors",70,-1.,6.);
#endif

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

#if defined(Y2016)
  const Float_t MinPt = 27.;
#elif defined(Y2017)
  const Float_t MinPt = 35.;
#elif defined(Y2018)
  const Float_t MinPt = 32.;
#endif

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

  UInt_t index = 0;
  for (UInt_t i = 0; i< *El.n; ++i){
    double abseta =  abs(El.eta[i]);
    if(El.cutBased[i]==4 and
       abseta < MaxEta and
       ( abseta < etaGap.first or abseta > etaGap.second))
      GoodIndex.emplace_back(i);
  }

  SortByDescPt(GoodIndex,El);

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


std::vector<std::pair<UInt_t,UInt_t>> PreSelector::GetLeptonPairs(const Leptons& l,
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

  std::vector<std::pair<UInt_t,UInt_t>> pairs;

  if(positive.size() == 0 || negative.size() == 0)
    return pairs; /*empty*/

  std::pair<UInt_t,UInt_t> couple;

  for(const uint& i: positive){
    for(const uint& j: negative){
      if (l.pt[i] > l.pt[j]) {
        couple = std::make_pair(i,j);
      } else {
        couple = std::make_pair(j,i);
      }
      pairs.push_back(couple);
    }
  }

  return pairs;

};

ZPairInfo PreSelector::FindZ(const Leptons& l, const std::vector<UInt_t>& GoodLepton) const{

  std::vector<std::pair<UInt_t,UInt_t>> Pairs ;
  Pairs = PreSelector::GetLeptonPairs(l,GoodLepton);

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

void PreSelector::FillH1(std::vector<TH1F*>& h1, const Int_t& nh, const Double_t& binContent){

#ifdef CMSDATA
  assert( wcentral == 1.);
#endif

  enum {
    kNoSf = 0,
    kCentral = 4,
    kUp = 8,
    kDown = 12,
  };
  h1[nh+kNoSf]->Fill(binContent);
#ifndef CMSDATA
  h1[nh+kDown]->Fill(binContent,wdown);
  h1[nh+kCentral]->Fill(binContent,wcentral);
  h1[nh+kUp]->Fill(binContent,wup);
#endif
}
#ifndef CMSDATA
void PreSelector::DefineSFs(){

  wup = -1.;
  wdown = -1.;
  wcentral = -1.;

  if(IsA_){
    wcentral = GetElectronSF(Electron_eta[leadElIdx], Electron_pt[leadElIdx],0);
    wcentral *= GetSFFromHisto(SFPileup,*PV_npvs);
    wup = GetElectronSF(Electron_eta[leadElIdx], Electron_pt[leadElIdx],1);
    wup *= GetSFFromHisto(SFPileup,*PV_npvs);
    wdown = GetElectronSF(Electron_eta[leadElIdx], Electron_pt[leadElIdx],-1);
    wdown *= GetSFFromHisto(SFPileup,*PV_npvs);
  } else if (IsB) {
    wcentral = GetElectronSF(Electron_eta[leadElIdx],Electron_pt[leadElIdx],0);
    wcentral *= GetMuonSF(Muon_eta[leadMuIdx],Muon_pt[leadMuIdx],0);
    wcentral *= GetSFFromHisto(SFPileup,*PV_npvs);
    wup = GetElectronSF(Electron_eta[leadElIdx],Electron_pt[leadElIdx],1);
    wup *= GetMuonSF(Muon_eta[leadMuIdx],Muon_pt[leadMuIdx],1);
    wup *= GetSFFromHisto(SFPileup,*PV_npvs);
    wdown = GetElectronSF(Electron_eta[leadElIdx],Electron_pt[leadElIdx],-1);
    wdown *= GetMuonSF(Muon_eta[leadMuIdx],Muon_pt[leadMuIdx],-1);
    wdown *= GetSFFromHisto(SFPileup,*PV_npvs);
  } else if (IsC) {
    wcentral = GetMuonSF(Muon_eta[leadMuIdx],Muon_pt[leadMuIdx],0);
    wcentral *= GetElectronSF(Electron_eta[leadElIdx],Electron_pt[leadElIdx],0);
    wcentral *= GetSFFromHisto(SFPileup,*PV_npvs);
    wup = GetMuonSF(Muon_eta[leadMuIdx],Muon_pt[leadMuIdx],1);
    wup *= GetElectronSF(Electron_eta[leadElIdx],Electron_pt[leadElIdx],1);
    wup *= GetSFFromHisto(SFPileup,*PV_npvs);
    wdown = GetMuonSF(Muon_eta[leadMuIdx],Muon_pt[leadMuIdx],-1);
    wdown *= GetElectronSF(Electron_eta[leadElIdx],Electron_pt[leadElIdx],-1);
    wdown *= GetSFFromHisto(SFPileup,*PV_npvs);
  } else if (IsD) {
    wcentral = GetMuonSF(Muon_eta[leadMuIdx],Muon_pt[leadMuIdx],0);
    wcentral *= GetSFFromHisto(SFPileup,*PV_npvs);
    wup = GetMuonSF(Muon_eta[leadMuIdx],Muon_pt[leadMuIdx],1);
    wup *= GetSFFromHisto(SFPileup,*PV_npvs);
    wdown = GetMuonSF(Muon_eta[leadMuIdx],Muon_pt[leadMuIdx],-1);
    wdown *= GetSFFromHisto(SFPileup,*PV_npvs);
  } else {
    assert (false);
  }

  Double_t ksf = 1.;
  Double_t ksfup = 1.;
  Double_t ksfdown = 1.;

  if (ApplyKFactors) {
    Double_t zpt = GetZPtFromGen();
    if(zpt > 0.){
      ksf = GetKFactor(zpt,0);
      ksfup = GetKFactor(zpt,1);
      ksfdown = GetKFactor(zpt,-1);
    }
    wcentral *= ksf;
    wup *= ksfup;
    wdown *= ksfdown;
  }

  assert( wup > 0.);
  assert( wcentral > 0.);
  assert( wdown > 0.);

}
#endif
void PreSelector::FillCategory(const Int_t& crOffset, const Leptons& lz,const Leptons& lw){

  assert(IsA_ xor IsB xor IsC xor IsD);

  wdown = wcentral = wup = 1.;
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
  Double_t wzdist = GetEtaPhiDistance(wb.Eta(),wb.Phi(),zb.Eta(),zb.Phi());
  Double_t l1l2dist = GetEtaPhiDistance(lep1.Eta(),lep1.Phi(),lep2.Eta(),lep2.Phi());
  Double_t l1l3dist = GetEtaPhiDistance(lep1.Eta(),lep1.Phi(),lep3.Eta(),lep3.Phi());
  Double_t l2l3dist = GetEtaPhiDistance(lep2.Eta(),lep2.Phi(),lep3.Eta(),lep3.Phi());

  // 2DHistos
  HWZPtDist[nh]->Fill((wb+zb).Pt(),wzdist);
  HPtEtal1[nh]->Fill(lep1.Pt(),lep1.Eta());
  HPtEtal2[nh]->Fill(lep2.Pt(),lep2.Eta());
  HPtEtal3[nh]->Fill(lep3.Pt(),lep3.Eta());
  HMassZTW[nh]->Fill(zb.M(),wmt);
  HDeltaRPtZ[nh]->Fill(l1l2dist,zb.Pt());
  HPtWPtZ[nh]->Fill(wb.Pt(),zb.Pt());
  HDeltaRMWZ[nh]->Fill(l1l2dist,(wb+zb).M());
  HLtMWZ[nh]->Fill(lt,(wb+zb).M());
  HNLep[nh]->Fill(GoodMuon.size(),GoodElectron.size());
  HMassZWZ[nh]->Fill(PairZMass,(wb+zb).M());

  HCutFlow->FillS(Form("%d",nh));

#ifndef CMSDATA

  DefineSFs();

  HScaleFactors[nh]->Fill(wup);
  HScaleFactors[nh]->Fill(wdown);

  HGenPartChgF[nh]->Fill(lz.charge[l1],GenPart_pdgId[lz.genPartIdx[l1]]);
  HGenPartChgF[nh]->Fill(lz.charge[l2],GenPart_pdgId[lz.genPartIdx[l2]]);
  HGenPartF[nh]->FillS(Form("%d",GenPart_pdgId[lz.genPartIdx[l1]]));
  HGenPartF[nh]->FillS(Form("%d",GenPart_pdgId[lz.genPartIdx[l2]]));
  HGenPartF[nh]->FillS(Form("%d",GenPart_pdgId[lw.genPartIdx[l3]]));

  HGenPartZ[nh]->FillS(Form("%d",GetMother(lz.genPartIdx[l1],
                                           lz.pdgId[l1]).second));
  HGenPartZ[nh]->FillS(Form("%d",GetMother(lz.genPartIdx[l2],
                                           lz.pdgId[l2]).second));
  HGenPartW[nh]->FillS(Form("%d",GetMother(lw.genPartIdx[l3],
                                           lw.pdgId[l3]).second));
  
#endif

  // Eta histos
  FillH1(HPileup_,nh,(double)*PV_npvs);
  FillH1(HEtal1,nh,lep1.Eta());
  FillH1(HEtal2,nh,lep2.Eta());
  FillH1(HEtal3,nh,lep3.Eta());

  // Pt histos
  FillH1(HPtl1,nh,lep1.Pt());
  FillH1(HPtl2,nh,lep2.Pt());
  FillH1(HPtl3,nh,lep3.Pt());
  FillH1(HMetPt,nh,*MET_pt);
  FillH1(HWZPt,nh,(wb+zb).Pt());
  FillH1(HLt,nh,lt);

  // n Histos
  FillH1(HnEl,nh,(double)GoodElectron.size());
  FillH1(HnMu,nh,(double)GoodMuon.size());
  FillH1(HnJet,nh,(double)*nJet);
  FillH1(HnbTag,nh,(double)nbQ);

  // Mass Histos
  FillH1(HMassW,nh,wb.M());
  FillH1(HMassZ,nh,PairZMass);
  FillH1(HMassTW,nh,wmt);
  FillH1(HMassWZ,nh,(wb+zb).M());

  // dR Histos
  FillH1(HWZDist,nh,wzdist);
  FillH1(HDistl1l2,nh,l1l2dist);
  FillH1(HDistl1l3,nh,l1l3dist);
  FillH1(HDistl2l3,nh,l2l3dist);

  // Phi Histos
  FillH1(HPhil1,nh,lep1.Phi());
  FillH1(HPhil2,nh,lep2.Phi());
  FillH1(HPhil3,nh,lep3.Phi());
  FillH1(HMetPhi,nh,*MET_phi);

  // Dxyz RelIso
  FillH1(HDxyl1,nh,lz.dxy[l1]);
  FillH1(HDxyl2,nh,lz.dxy[l2]);
  FillH1(HDxyl3,nh,lw.dxy[l3]);
  FillH1(HDzl1,nh,lz.dz[l1]);
  FillH1(HDzl2,nh,lz.dz[l2]);
  FillH1(HDzl3,nh,lw.dz[l3]);
  FillH1(HRelIsol1,nh,lz.relIso[l1]);
  FillH1(HRelIsol2,nh,lz.relIso[l2]);
  FillH1(HRelIsol3,nh,lz.relIso[l3]);
}


bool PreSelector::DefineW(const int& idx, const Leptons& l){

  l3 = idx;
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
  wmt = PreSelector::MassRecoW(lep3.Pt(), lep3.Phi(), *MET_pt, *MET_phi);
  nu = PreSelector::GetNu4V(lep3, wmt);
  wb = (lep3 + nu[0]);

  return true;

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

Int_t PreSelector::nbTag(){
#if defined(Y2016)
  const float medium = 0.3093;
#elif defined(Y2017)
  const float medium = 0.3033;
#elif defined(Y2018)
  const float medium = 0.2770;
#endif
  Int_t nbtag = 0;
  for(uint i = 0; i < *nJet; ++i){
    if( Jet_btagDeepFlavB[i] > medium){
      ++nbtag;
    }
  }
  return nbtag;
}

Bool_t PreSelector::Process(Long64_t entry) {

  IsA_ = IsB = IsC = IsD = false;
  PairMu = PairEl = false;
  l1 = l2 = l3 = -1;

  ReadEntry(entry);

  HCutFlow->FillS("NoCuts");
#ifndef CMSDATA
  HCutFlow->Fill("genWeight",*genWeight);
  HTruePileup->Fill(static_cast<Double_t>(*Pileup_nTrueInt));
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

  if (*MET_pt < 30.){
    HCutFlow->FillS("MET<30");
    return kFALSE;
  }

#ifndef CMSDATA
  Muons Mus(nMuon,Muon_pt,Muon_eta,Muon_phi,
            Muon_charge,Muon_dxy,Muon_dz,Muon_pfRelIso03_all,
            Muon_tightId, Muon_genPartIdx, Muon_pdgId);

  Electrons Els(nElectron,Electron_pt,Electron_eta,Electron_phi,
                Electron_charge,Electron_dxy,Electron_dz,Electron_pfRelIso03_all,
                Electron_cutBased, Electron_genPartIdx, Electron_pdgId);
#else
  Muons Mus(nMuon,Muon_pt,Muon_eta,Muon_phi,
            Muon_charge,Muon_dxy,Muon_dz,Muon_pfRelIso03_all,
            Muon_tightId);

  Electrons Els(nElectron,Electron_pt,Electron_eta,Electron_phi,
                Electron_charge,Electron_dxy,Electron_dz,Electron_pfRelIso03_all,
                Electron_cutBased);
#endif

  GoodElectron = PreSelector::GetGoodElectron(Els);
  GoodMuon = PreSelector::GetGoodMuon(Mus);

  HNEl->Fill(*nElectron,GoodElectron.size());
  HNMu->Fill(*nMuon,GoodMuon.size());

  if( (GoodElectron.size() + GoodMuon.size()) != 3 ){
    HCutFlow->FillS("goodLep!=3");
    return kFALSE;
  }

  ////////////// Define Z //////////////

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
    zt = &ztel;
  } else { //PairMu
    zt = &ztmu;
  }

  if(PairMu) HCutFlow->FillS("PairMu");
  if(PairEl) HCutFlow->FillS("PairEl");

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

  Bool_t Zl1PtCut = lep1.Pt() < 50.;
  if(Zl1PtCut){
    HCutFlow->FillS("FailZl1PtCut");
    return kFALSE;
  }

  zb   = lep1 + lep2;

  ////////////// Define W //////////////

  assert(PairEl xor PairMu);

  if(PairMu){
    if(GoodMuon.size() == 3){
      for(const int& i: GoodMuon){
        if( (i!=l1 && i!=l2) and Muon_highPtId[i] == 2 ){
          if(!DefineW(i,Mus)) return kFALSE;
          IsD = true;
        }
      }
      if(l3 == -1){
        HCutFlow->FillS("PairMu_NoWlepCand");
        return kFALSE;
      }
    } else {
      assert(GoodElectron.size() == 1);
      if(!DefineW(GoodElectron[0],Els)) return kFALSE;
      IsC = true;
    }
  } else { // PairEl
    if(GoodElectron.size() == 3){
      for(const int& i: GoodElectron){
        if(i!=l1 && i!=l2){
          if(!DefineW(i,Els)) return kFALSE;
          IsA_ = true;
        }
      }
      assert(l3 != -1);
    } else {
      assert(GoodMuon.size() == 1);
      if (Muon_highPtId[0] == 2) {
        if(!DefineW(GoodMuon[0],Mus)) return kFALSE;
        IsB = true;
      } else {
        HCutFlow->FillS("PairEl_NoWlepCand");
        return kFALSE;
      }
    }
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

  const float_t l1l2Dist = GetEtaPhiDistance(lep1.Eta(),lep1.Phi(),lep2.Eta(),lep2.Phi());
  Bool_t ZDistCut = l1l2Dist < 1.5;
  if(ZDistCut){
    HCutFlow->FillS("SignalRegion");
    FillRegion(0,Els,Mus); // 0 -> Signal Region
  } else {
    HCutFlow->FillS("dRControlRegion");
    FillRegion(16,Els,Mus); // 16 -> CR1 Slot
  }

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


  gStyle->SetOptStat(1111111);
  std::unique_ptr<TCanvas> ch(new TCanvas("ch","ch",1200,800));
  std::unique_ptr<TCanvas> chc(new TCanvas("chc","chc",1200,800));

  std::unique_ptr<TFile> fOut(TFile::Open("WprimeHistos_2CR.root","UPDATE"));
  fOut->mkdir(Form("%d",Year));
  fOut->mkdir(Form("%d/%s",Year,SampleName.Data()));
  fOut->cd(Form("%d/%s",Year,SampleName.Data()));

  TObjLink *lnk = fOutput->FirstLink();
  while (lnk) {
    lnk->GetObject()->Write();
    lnk = lnk->Next();
  }

  ch->cd();
  HCutFlow->LabelsDeflate();
  gPad->SetLogy();
  HCutFlow->Draw("HIST TEXT45");
  ch->Print(getFullPath("HCutFlow"));

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
