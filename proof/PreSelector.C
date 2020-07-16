#include "TFile.h"
#include "PreSelector.h"
#include "TMath.h"
#include "TLegend.h"
#include "TError.h"

#define FillS(xx) Fill(xx,1.)

PreSelector::PreSelector(TTree *)
{

  gErrorIgnoreLevel = kError;
  HMetA=0;
  HMetB=0;
  HMetC=0;
  HMetD=0;

  HnElA=0;
  HnElB=0;
  HnElC=0;
  HnElD=0;

  HnMuA=0;
  HnMuB=0;
  HnMuC=0;
  HnMuD=0;

  HMassWA=0;
  HMassWB=0;
  HMassWC=0;
  HMassWD=0;

  HMassZA=0;
  HMassZB=0;
  HMassZC=0;
  HMassZD=0;

  HMassTWA=0;
  HMassTWB=0;
  HMassTWC=0;
  HMassTWD=0;

  HMassWZA=0;
  HMassWZB=0;
  HMassWZC=0;
  HMassWZD=0;

  HMassZWZA=0;
  HMassZWZB=0;
  HMassZWZC=0;
  HMassZWZD=0;

  HOverlap=0;

  HDistl1l2=0;
  HDistl1l3=0;
  HDistl2l3=0;

#ifndef CMSDATA

  HGenPartZA=0;
  HGenPartZB=0;
  HGenPartZC=0;
  HGenPartZD=0;

  HGenPartWA=0;
  HGenPartWB=0;
  HGenPartWC=0;
  HGenPartWD=0;

  HGenPartFA=0;
  HGenPartFB=0;
  HGenPartFC=0;
  HGenPartFD=0;
  HScaleFactors = 0;
  SFPileup = 0;
#endif
  HCutFlow = 0;

  HPtl1 = 0;
  HPtl2 = 0;
  HPtl3 = 0;

  HNLepA=0;
  HNLepB=0;
  HNLepC=0;
  HNLepD=0;

  HNEl = 0;
  HNMu = 0;

  HWZDist = 0;
  HWZPtDist = 0;

  HPileup = 0;
  HPileupA = 0;
  HPileupB = 0;
  HPileupC = 0;
  HPileupD = 0;

  HWZPt = 0;
}

#ifndef CMSDATA
Float_t PreSelector::GetSFFromGraph(TGraphAsymmErrors* g,const Float_t& eta) const {

  Double_t* a;

  for(Int_t i = 0; i < g->GetN(); i++){
    a = (g->GetY()+i);
    /* Test right bin limit*/
    if( eta < (*(g->GetX()+i) + g->GetErrorX(i))) break;
  }

  return static_cast<Float_t>(*a);

}
#endif
#ifndef CMSDATA
Float_t PreSelector::GetSFFromHisto(TH1* h,const Float_t& x, const Float_t& y) const {
  return h->GetBinContent(h->FindBin(x,y));
}
#endif
#ifndef CMSDATA
Float_t PreSelector::GetSFFromHisto(TH1* h, const Int_t& npv){
  return h->GetBinContent(h->FindBin(npv));
}
#endif
#ifndef CMSDATA
Float_t PreSelector::GetElectronSF(const Float_t& eta, const Float_t& pt) const{

  Float_t sf = -1;

  if(Year == 2016){
    /* 2 bins in pt */
    if( pt < 175.){
      sf = GetSFFromGraph(SFElectronTrigger1,eta); 
    } else {
      sf = GetSFFromGraph(SFElectronTrigger2,eta);
    }
  }

  return sf;

}
#endif
#ifndef CMSDATA
Float_t PreSelector::GetMuonSF(const Float_t& eta, const Float_t& pt) const{

  Float_t sf = -1;
  const Float_t epsilon = 1e-2;

  if (Year == 2016) {
    const Float_t LumiBF = 3.11; //fb-1
    const Float_t LumiGH = 5.54;

    Float_t SFTriggerBF = GetSFFromHisto(SFMuonTriggerBF,abs(eta),pt);
    if (SFTriggerBF < epsilon)
      SFTriggerBF = GetSFFromHisto(SFMuonTriggerBF,abs(eta),(SFMuonTriggerBF->GetYaxis()->GetXmax() - epsilon));

    Float_t SFTriggerGH = GetSFFromHisto(SFMuonTriggerGH,abs(eta),pt);
    if (SFTriggerGH < epsilon)
      SFTriggerGH = GetSFFromHisto(SFMuonTriggerGH,abs(eta),(SFMuonTriggerGH->GetYaxis()->GetXmax() - epsilon));

    Float_t SFIDBF = GetSFFromHisto(SFMuonIDBF,eta,pt);
    if (SFIDBF < epsilon)
      SFIDBF = GetSFFromHisto(SFMuonIDBF,eta,(SFMuonIDBF->GetYaxis()->GetXmax() - epsilon));

    Float_t SFIDGH = GetSFFromHisto(SFMuonIDGH,eta,pt);
    if (SFIDGH < epsilon)
      SFIDGH = GetSFFromHisto(SFMuonIDGH,eta,(SFMuonIDGH->GetYaxis()->GetXmax() - epsilon));

    sf = (LumiBF*SFTriggerBF+LumiGH*SFTriggerGH)/(LumiBF+LumiGH);
    sf *=(LumiBF*SFIDBF+LumiGH*SFIDGH)/(LumiBF+LumiGH);
  }

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

void PreSelector::SlaveBegin(TTree *tree) {

  const Double_t MaxMet = 600.;
  const Double_t MinMet = 0.;
  const Int_t MetBins = 60;

  HMetA = new TH1F("HMetA","",MetBins,MinMet,MaxMet);
  HMetB = new TH1F("HMetB","",MetBins,MinMet,MaxMet);
  HMetC = new TH1F("HMetC","",MetBins,MinMet,MaxMet);
  HMetD = new TH1F("HMetD","",MetBins,MinMet,MaxMet);

  const Int_t DistBins = 100;
  const Float_t MaxDist = 2.01*TMath::Pi();

  HDistl1l2 = new TH1F("HDistl1l2","Distance in EtaPhi l1,l2",DistBins,0.,MaxDist);
  fOutput->Add(HDistl1l2);
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

  HnElA = new TH1I("HnElA","",nLepBins,MinnLep,MaxnLep);
  HnElB = new TH1I("HnElB","",nLepBins,MinnLep,MaxnLep);
  HnElC = new TH1I("HnElC","",nLepBins,MinnLep,MaxnLep);
  HnElD = new TH1I("HnElD","",nLepBins,MinnLep,MaxnLep);

  HnMuA = new TH1I("HnMuA","",nLepBins,MinnLep,MaxnLep);
  HnMuB = new TH1I("HnMuB","",nLepBins,MinnLep,MaxnLep);
  HnMuC = new TH1I("HnMuC","",nLepBins,MinnLep,MaxnLep);
  HnMuD = new TH1I("HnMuD","",nLepBins,MinnLep,MaxnLep);

  const Float_t MinMass = -1.;
  const Float_t MaxMass = 2200.;
  const Int_t MassBins = 52;

  const Float_t MinWMass = 0.;
  const Float_t MaxWMass = 1e3;

  HMassWA = new TH1F("HMassWA","",MassBins,MinWMass,MaxWMass);
  HMassWB = new TH1F("HMassWB","",MassBins,MinWMass,MaxWMass);
  HMassWC = new TH1F("HMassWC","",MassBins,MinWMass,MaxWMass);
  HMassWD = new TH1F("HMassWD","",MassBins,MinWMass,MaxWMass);

  fOutput->Add(HMassWA);
  fOutput->Add(HMassWB);
  fOutput->Add(HMassWC);
  fOutput->Add(HMassWD);

  const Float_t HMaxZMass = 120.;
  const Float_t HMinZMass = 60.;

  HMassZA = new TH1F("HMassZA","",MassBins,HMinZMass,HMaxZMass);
  HMassZB = new TH1F("HMassZB","",MassBins,HMinZMass,HMaxZMass);
  HMassZC = new TH1F("HMassZC","",MassBins,HMinZMass,HMaxZMass);
  HMassZD = new TH1F("HMassZD","",MassBins,HMinZMass,HMaxZMass);

  HMassTWA = new TH1F("HMassTWA","",MassBins,MinMass,HMaxZMass);
  HMassTWB = new TH1F("HMassTWB","",MassBins,MinMass,HMaxZMass);
  HMassTWC = new TH1F("HMassTWC","",MassBins,MinMass,HMaxZMass);
  HMassTWD = new TH1F("HMassTWD","",MassBins,MinMass,HMaxZMass);

  fOutput->Add(HMassTWA);
  fOutput->Add(HMassTWB);
  fOutput->Add(HMassTWC);
  fOutput->Add(HMassTWD);

  HMassWZA = new TH1F("HMassWZA","",MassBins,MinMass,MaxMass);
  HMassWZB = new TH1F("HMassWZB","",MassBins,MinMass,MaxMass);
  HMassWZC = new TH1F("HMassWZC","",MassBins,MinMass,MaxMass);
  HMassWZD = new TH1F("HMassWZD","",MassBins,MinMass,MaxMass);

  fOutput->Add(HMassWZA);
  fOutput->Add(HMassWZB);
  fOutput->Add(HMassWZC);
  fOutput->Add(HMassWZD);

  HMassZWZA = new TH2F("HMassZWZA","3e0mu;Z Mass;WZ Mass",MassBins,0.,1.5*HMaxZMass,MassBins,0.,MaxMass);
  fOutput->Add(HMassZWZA);
  HMassZWZB = new TH2F("HMassZWZB","2e1mu;Z Mass;WZ Mass",MassBins,0.,1.5*HMaxZMass,MassBins,0.,MaxMass);
  fOutput->Add(HMassZWZB);
  HMassZWZC = new TH2F("HMassZWZC","1e2mu;Z Mass;WZ Mass",MassBins,0.,1.5*HMaxZMass,MassBins,0.,MaxMass);
  fOutput->Add(HMassZWZC);
  HMassZWZD = new TH2F("HMassZWZD","0e3mu;Z Mass;WZ Mass",MassBins,0.,1.5*HMaxZMass,MassBins,0.,MaxMass);
  fOutput->Add(HMassZWZD);

  HOverlap = new TH1I("HOverlap","Overlapping events."
                      " -1: l<3 0:None 1: NoOverlap",6,-1,5);
  fOutput->Add(HOverlap);

  const UInt_t BinsPdgId = 100;
  const Float_t PdgIdMin = -50.;
  const Float_t PdgIdMax = 50.;

#ifndef CMSDATA

  HGenPartZA = new TH1F("HGenPartZA","",BinsPdgId,PdgIdMin,PdgIdMax);
  HGenPartZB = new TH1F("HGenPartZB","",BinsPdgId,PdgIdMin,PdgIdMax);
  HGenPartZC = new TH1F("HGenPartZC","",BinsPdgId,PdgIdMin,PdgIdMax);
  HGenPartZD = new TH1F("HGenPartZD","",BinsPdgId,PdgIdMin,PdgIdMax);

  fOutput->Add(HGenPartZA);
  fOutput->Add(HGenPartZB);
  fOutput->Add(HGenPartZC);
  fOutput->Add(HGenPartZD);

  HGenPartWA = new TH1F("HGenPartWA","",BinsPdgId,PdgIdMin,PdgIdMax);
  HGenPartWB = new TH1F("HGenPartWB","",BinsPdgId,PdgIdMin,PdgIdMax);
  HGenPartWC = new TH1F("HGenPartWC","",BinsPdgId,PdgIdMin,PdgIdMax);
  HGenPartWD = new TH1F("HGenPartWD","",BinsPdgId,PdgIdMin,PdgIdMax);

  fOutput->Add(HGenPartWA);
  fOutput->Add(HGenPartWB);
  fOutput->Add(HGenPartWC);
  fOutput->Add(HGenPartWD);

  HGenPartFA = new TH1F("HGenPartFA","",BinsPdgId,PdgIdMin,PdgIdMax);
  HGenPartFB = new TH1F("HGenPartFB","",BinsPdgId,PdgIdMin,PdgIdMax);
  HGenPartFC = new TH1F("HGenPartFC","",BinsPdgId,PdgIdMin,PdgIdMax);
  HGenPartFD = new TH1F("HGenPartFD","",BinsPdgId,PdgIdMin,PdgIdMax);

  fOutput->Add(HGenPartFA);
  fOutput->Add(HGenPartFB);
  fOutput->Add(HGenPartFC);
  fOutput->Add(HGenPartFD);

#endif
  HCutFlow = new TH1F("HCutFlow","",BinsPdgId,PdgIdMin,PdgIdMax);
  fOutput->Add(HCutFlow);

  fOutput->Add(HMassZA);
  fOutput->Add(HMassZB);
  fOutput->Add(HMassZC);
  fOutput->Add(HMassZD);

  fOutput->Add(HMetA);
  fOutput->Add(HMetB);
  fOutput->Add(HMetC);
  fOutput->Add(HMetD);

  fOutput->Add(HnElA);
  fOutput->Add(HnElB);
  fOutput->Add(HnElC);
  fOutput->Add(HnElD);

  fOutput->Add(HnMuA);
  fOutput->Add(HnMuB);
  fOutput->Add(HnMuC);
  fOutput->Add(HnMuD);

  const Int_t BinsLep = 10;
  const Float_t MinLep = 0.;
  const Float_t MaxLep = 10.;

  HNLepA = new TH2I("HNLepA","",BinsLep,MinLep,MaxLep,
                    BinsLep,MinLep,MaxLep);
  HNLepB = new TH2I("HNLepB","",BinsLep,MinLep,MaxLep,
                    BinsLep,MinLep,MaxLep);
  HNLepC = new TH2I("HNLepC","",BinsLep,MinLep,MaxLep,
                    BinsLep,MinLep,MaxLep);
  HNLepD = new TH2I("HNLepD","",BinsLep,MinLep,MaxLep,
                    BinsLep,MinLep,MaxLep);

  fOutput->Add(HNLepA);
  fOutput->Add(HNLepB);
  fOutput->Add(HNLepC);
  fOutput->Add(HNLepD);

  HNEl = new TH2I("HNEl","",BinsLep,MinLep,MaxLep,
                  BinsLep,MinLep,MaxLep);
  HNMu = new TH2I("HNMu","",BinsLep,MinLep,MaxLep,
                  BinsLep,MinLep,MaxLep);

  fOutput->Add(HNEl);
  fOutput->Add(HNMu);

  HPtl1 = new TH1F("HPtl1","Z Leading lepton Pt",
                   MetBins,MinMet,MaxMet);
  fOutput->Add(HPtl1);

  HPtl2 = new TH1F("HPtl2","Z Subleading lepton Pt",
                   MetBins,MinMet,MaxMet);
  fOutput->Add(HPtl2);

  HPtl3 = new TH1F("HPtl3","W lepton pt",
                     MetBins,MinMet,MaxMet);
  fOutput->Add(HPtl3);

  HMetPt = new TH1F("HMetPt","MET Pt",
                    MetBins,MinMet,MaxMet);
  fOutput->Add(HMetPt);

  const Int_t nPvsBins = 100;
  const Float_t minPvs = 0.;
  const Float_t maxPvs = 100.;

  HPileup = new TH1D("HPileup","PV_npvs",
                     nPvsBins,minPvs,maxPvs);
  fOutput->Add(HPileup);

  HPileupA = new TH1F("HPileupA","PV_npvs",
                      nPvsBins,minPvs,maxPvs);
  fOutput->Add(HPileupA);

  HPileupB = new TH1F("HPileupB","PV_npvs",
                      nPvsBins,minPvs,maxPvs);
  fOutput->Add(HPileupB);

  HPileupC = new TH1F("HPileupC","PV_npvs",
                      nPvsBins,minPvs,maxPvs);
  fOutput->Add(HPileupC);

  HPileupD = new TH1F("HPileupD","PV_npvs",
                      nPvsBins,minPvs,maxPvs);
  fOutput->Add(HPileupD);

  if (fInput->FindObject("Year")) {
    TParameter<Int_t> *p = dynamic_cast<TParameter<Int_t>*>(fInput->FindObject("Year"));
    Year = p->GetVal();
    std::clog << Form("PreSelector::SlaveBegin() Year Set: %d\n",Year);
  }

#ifndef CMSDATA

  if (fInput->FindObject("SampleName")) {
    // Lesson: TString can't be in TCollection
    TNamed *p = dynamic_cast<TNamed *>(fInput->FindObject("SampleName"));
    SampleName = p->GetTitle();
  }

  if(fInput->FindObject("SFDb")){
    SFDb = dynamic_cast<TList*>(fInput->FindObject("SFDb"));
  }

  if (Year == 2016){
    SFElectronTrigger1 = static_cast<TGraphAsymmErrors*>(SFDb->FindObject("SFElectronTrigger1"));
    SFElectronTrigger2 = static_cast<TGraphAsymmErrors*>(SFDb->FindObject("SFElectronTrigger2"));
    SFMuonTriggerBF = static_cast<TH2F*>(SFDb->FindObject("SFMuonTriggerBF"));
    SFMuonTriggerGH = static_cast<TH2F*>(SFDb->FindObject("SFMuonTriggerGH"));
    SFMuonIDBF = static_cast<TH2D*>(SFDb->FindObject("SFMuonIDBF"));
    SFMuonIDGH = static_cast<TH2D*>(SFDb->FindObject("SFMuonIDGH"));
    auto l = static_cast<TList*>(SFDb->FindObject("PileupSFList"));
    SFPileup = static_cast<TH1D*>(l->FindObject(SampleName.Data()));
  }

  HScaleFactors = new TH1F("HScaleFactors","HScaleFactors",30,0.,1.5);
  fOutput->Add(HScaleFactors);
#endif

  std::clog << Form("PreSelector::SlaveBegin Year: %d\n",Year);

}

std::vector<UInt_t> PreSelector::GetGoodMuon(const Muons& Mu){
  std::vector<UInt_t> GoodIndex = {};
  if(!MuonTest()) return GoodIndex;
  GoodIndex.reserve(10);
  const Float_t MaxEta = 2.4;
  const Float_t MinPt = 10.;
  for (UInt_t i=0; i<*Mu.n;i++){
    if(Mu.tightId[i] &&
       Mu.pt[i]>MinPt && abs(Mu.eta[i])<MaxEta)
      GoodIndex.emplace_back(i);
  }
  return GoodIndex;
};

std::vector<UInt_t> PreSelector::GetGoodElectron(const Electrons& El){
  const Float_t MaxEta = 2.5;
  const Float_t MinPt = 20.;
  std::vector<UInt_t> GoodIndex = {};
  if(!ElectronTest()) return GoodIndex;
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


std::vector<std::pair<UInt_t,UInt_t>> PreSelector::GetLeptonPairs(const Leptons& l, const std::vector<UInt_t>& GoodIndex) const{

  static const UInt_t size = 5;

  std::vector<UInt_t> positive;
  positive.reserve(size);
  std::vector<UInt_t> negative;
  negative.reserve(size);

  for(UInt_t i=0; i< GoodIndex.size(); i++){
    if (l.charge[GoodIndex[i]] == 1){
      positive.emplace_back(GoodIndex[i]);
    } else {
      negative.emplace_back(GoodIndex[i]);
    }
  }

  if(positive.size() == 0 || negative.size() == 0)
    return std::vector<std::pair<UInt_t,UInt_t>>();

  std::vector<std::pair<UInt_t,UInt_t>> couples;
  couples.reserve(size);

  for(UInt_t i=0; i< positive.size(); i++){
    for(UInt_t j=0; j< negative.size();j++){
      std::pair<UInt_t,UInt_t> p;
      if (l.pt[positive[i]] > l.pt[negative[j]]) {
        p = std::make_pair(positive[i],negative[j]);
      } else {
        p = std::make_pair(negative[j],positive[i]);
      }
      couples.emplace_back(p);
    }
  }

  return couples;

}

std::vector<std::tuple<Double_t,Double_t,std::pair<UInt_t,UInt_t>>> PreSelector::FindZ(const Leptons& l, const std::vector<UInt_t>& GoodLepton) const{

  std::vector<std::pair<UInt_t,UInt_t>> Pairs ;

  Pairs = PreSelector::GetLeptonPairs(l,GoodLepton);

  // ZMassDistance, ZMass, Pair
  std::vector<std::tuple<Double_t,Double_t,std::pair<UInt_t,UInt_t>>> ZMassTuple;
  ZMassTuple.reserve(5);

  if(Pairs.empty())
    return ZMassTuple;

  const Double_t ZNominalMass = 91.1876;

  for(UInt_t k = 0; k< Pairs.size(); k++){
    UInt_t i = Pairs[k].first;
    UInt_t j = Pairs[k].second;
    Double_t m = PreSelector::MassRecoZ(l.pt[i],l.eta[i],l.phi[i],l.mass,
                                        l.pt[j],l.eta[j],l.phi[j],l.mass);
    ZMassTuple.emplace_back(std::make_tuple(abs(ZNominalMass-m),m,std::make_pair(i,j)));
  }

  std::sort(ZMassTuple.begin(),ZMassTuple.end()); //By ZMassDistance
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
  NuPz = (lep.Pz()*b + d)/(2.*lep.Pt()*lep.Pt());
  s.emplace_back(Get4V(NuPz));
  NuPz = (lep.Pz()*b - d)/(2.*lep.Pt()*lep.Pt());
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
  return sqrt(pow(eta2-eta1,2.)+pow(phi2-phi1,2.));
}

void PreSelector::FillCommon(const Leptons& lz,const Leptons& lw){
  HWZPtDist->Fill((wb+zb).Pt(),GetEtaPhiDistance(wb.Eta(),wb.Phi(),zb.Eta(),zb.Phi()));
  HWZPt->Fill((wb+zb).Pt());
  HWZDist->Fill(GetEtaPhiDistance(wb.Eta(),wb.Phi(),zb.Eta(),zb.Phi()));
  HPtl1->Fill(lz.pt[l1]);
  HPtl2->Fill(lz.pt[l2]);
  HPtl3->Fill(lw.pt[l3]);
  HDistl1l2->Fill(GetEtaPhiDistance(lz.eta[l1],lz.phi[l1],
                                      lz.eta[l2],lz.phi[l2]));
  HDistl1l3->Fill(GetEtaPhiDistance(lz.eta[l1],lz.phi[l1],
                                      lw.eta[l3],lw.phi[l3]));
  HDistl2l3->Fill(GetEtaPhiDistance(lz.eta[l2],lz.phi[l2],
                                      lw.eta[l3],lw.phi[l3]));
  HMetPt->Fill(*MET_pt);
}

void PreSelector::FillA(){
  Float_t w = 1.f;
  HNLepA->Fill(GoodMuon.size(),GoodElectron.size());
  HnElA->Fill(GoodElectron.size());
  HnMuA->Fill(GoodMuon.size());
  HMassZWZA->Fill(BestZMass,(wb+zb).M());
  HCutFlow->FillS("3e0mu");

#ifndef CMSDATA
  w = GetElectronSF(lep1.Eta(), lep1.Pt());
  w *= GetSFFromHisto(SFPileup,*PV_npvs);
  HScaleFactors->Fill(w);
  HGenPartFA->FillS(Form("%d",GenPart_pdgId[Electron_genPartIdx[l1]]));
  HGenPartFA->FillS(Form("%d",GenPart_pdgId[Electron_genPartIdx[l2]]));
  HGenPartFA->FillS(Form("%d",GenPart_pdgId[Electron_genPartIdx[l3]]));

  HGenPartZA->FillS(Form("%d",GetMother(Electron_genPartIdx[l1],
                                       Electron_pdgId[l1]).second));
  HGenPartZA->FillS(Form("%d",GetMother(Electron_genPartIdx[l2],
                                        Electron_pdgId[l2]).second));
  HGenPartWA->FillS(Form("%d",GetMother(Electron_genPartIdx[l3],
                                        Electron_pdgId[l3]).second));
#endif
  HPileupA->Fill(*PV_npvs,w);
  HMetA->Fill(*MET_pt,w);
  HMassWA->Fill(wb.M(),w);
  HMassWZA->Fill((wb+zb).M(),w);
  HMassZA->Fill(BestZMass,w);
  HMassTWA->Fill(wmt,w);

}

void PreSelector::FillB(){
  Float_t w = 1.f;

  HNLepB->Fill(GoodMuon.size(),GoodElectron.size());
  HnElB->Fill(GoodElectron.size());
  HnMuB->Fill(GoodMuon.size());
  HMassZWZB->Fill(BestZMass,(wb+zb).M());
  HCutFlow->FillS("2e1mu");

#ifndef CMSDATA
  w = GetElectronSF(lep1.Eta(),lep1.Pt());
  w *= GetMuonSF(lep3.Eta(),lep3.Pt());
  w *= GetSFFromHisto(SFPileup,*PV_npvs);
  HScaleFactors->Fill(w);
  HGenPartFB->FillS(Form("%d",GenPart_pdgId[Electron_genPartIdx[l1]]));
  HGenPartFB->FillS(Form("%d",GenPart_pdgId[Electron_genPartIdx[l2]]));
  HGenPartFB->FillS(Form("%d",GenPart_pdgId[Muon_genPartIdx[l3]]));

  HGenPartZB->FillS(Form("%d",GetMother(Electron_genPartIdx[l1],
                                       Electron_pdgId[l1]).second));
  HGenPartZB->FillS(Form("%d",GetMother(Electron_genPartIdx[l2],
                                       Electron_pdgId[l2]).second));
  HGenPartWB->FillS(Form("%d",GetMother(Muon_genPartIdx[l3],
                                       Muon_pdgId[l3]).second));
#endif
  HPileupB->Fill(*PV_npvs,w);
  HMetB->Fill(*MET_pt,w);
  HMassWB->Fill(wb.M(),w);
  HMassWZB->Fill((wb+zb).M(),w);
  HMassZB->Fill(BestZMass,w);
  HMassTWB->Fill(wmt,w);
}

void PreSelector::FillC(){
  Float_t w = 1.f;
  HNLepC->Fill(GoodMuon.size(),GoodElectron.size());
  HnElC->Fill(GoodElectron.size());
  HnMuC->Fill(GoodMuon.size());
  HMassZWZC->Fill(BestZMass,(wb+zb).M());
  HCutFlow->FillS("1e2mu");

#ifndef CMSDATA
  w = GetMuonSF(lep1.Eta(),lep1.Pt());
  w *= GetElectronSF(lep3.Eta(),lep3.Pt());
  w *= GetSFFromHisto(SFPileup,*PV_npvs);
  HScaleFactors->Fill(w);
  HGenPartFC->FillS(Form("%d",GenPart_pdgId[Muon_genPartIdx[l1]]));
  HGenPartFC->FillS(Form("%d",GenPart_pdgId[Muon_genPartIdx[l2]]));
  HGenPartFC->FillS(Form("%d",GenPart_pdgId[Electron_genPartIdx[l3]]));

  HGenPartZC->FillS(Form("%d",GetMother(Muon_genPartIdx[l1],
                                        Muon_pdgId[l1]).second));
  HGenPartZC->FillS(Form("%d",GetMother(Muon_genPartIdx[l2],
                                        Muon_pdgId[l2]).second));
  HGenPartWC->FillS(Form("%d",GetMother(Electron_genPartIdx[l3],
                                        Electron_pdgId[l3]).second));
#endif
  HPileupC->Fill(*PV_npvs,w);
  HMetC->Fill(*MET_pt,w);
  HMassWC->Fill(wb.M(),w);
  HMassWZC->Fill((wb+zb).M(),w);
  HMassZC->Fill(BestZMass,w);
  HMassTWC->Fill(wmt,w);
}

void PreSelector::DefineW(Leptons l){
  lep3 = PtEtaPhiMVector(l.pt[l3], l.eta[l3], l.phi[l3], l.mass);
  wmt = PreSelector::MassRecoW(lep3.Pt(), lep3.Phi(), *MET_pt, *MET_phi);
  nu = PreSelector::GetNu4V(lep3, wmt);
  wb = (lep3 + nu[0]);
}

void PreSelector::FillD(){
  Float_t w = 1.f;
  HNLepD->Fill(GoodMuon.size(),GoodElectron.size());
  HnElD->Fill(GoodElectron.size());
  HnMuD->Fill(GoodMuon.size());
  HMassZWZD->Fill(BestZMass,(wb+zb).M());
  HCutFlow->FillS("0e3mu");

#ifndef CMSDATA
  w = GetMuonSF(lep1.Eta(),lep1.Pt());
  w *= GetSFFromHisto(SFPileup,*PV_npvs);
  HScaleFactors->Fill(w);
  HGenPartFD->FillS(Form("%d",GenPart_pdgId[Muon_genPartIdx[l1]]));
  HGenPartFD->FillS(Form("%d",GenPart_pdgId[Muon_genPartIdx[l2]]));
  HGenPartFD->FillS(Form("%d",GenPart_pdgId[Muon_genPartIdx[l3]]));

  HGenPartZD->FillS(Form("%d",GetMother(Muon_genPartIdx[l1],
                                        Muon_pdgId[l1]).second));
  HGenPartZD->FillS(Form("%d",GetMother(Muon_genPartIdx[l2],
                                        Muon_pdgId[l2]).second));
  HGenPartWD->FillS(Form("%d",GetMother(Muon_genPartIdx[l3],
                                        Muon_pdgId[l3]).second));
#endif
  HPileupD->Fill(*PV_npvs,w);
  HMetD->Fill(*MET_pt,w);
  HMassWD->Fill(wb.M(),w);
  HMassWZD->Fill((wb+zb).M(),w);
  HMassZD->Fill(BestZMass,w);
  HMassTWD->Fill(wmt,w);
}

Bool_t PreSelector::CheckElectronPair(const std::pair<UInt_t,UInt_t>& p) const{
  const Float_t MinPt = 35.;
  if (Electron_pt[p.first] < MinPt || Electron_pt[p.second] < MinPt) return kFALSE;
  return kTRUE;
}

Bool_t PreSelector::CheckMuonPair(const std::pair<UInt_t,UInt_t>& p) const{
  const Float_t MinLeadPt = 25.;
  const Float_t MinSubleadPt = 10.;
  if (Muon_pt[p.first] < MinLeadPt || Muon_pt[p.second] < MinSubleadPt) return kFALSE;
  return kTRUE;
}

Bool_t PreSelector::Process(Long64_t entry) {

  ReadEntry(entry);

  HCutFlow->FillS("NoCuts");

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

  const Double_t MinRemPt = 20.;

  Bool_t IsA{},IsB{},IsC{},IsD{};
  Bool_t PairEl{}, PairMu{};

  Float_t MinZMass = 70.;
  Float_t MaxZMass = 111.;


  std::vector<std::tuple<Double_t,Double_t,std::pair<UInt_t,UInt_t>>> *zt = nullptr;
  std::vector<std::tuple<Double_t,Double_t,std::pair<UInt_t,UInt_t>>> ztel;
  std::vector<std::tuple<Double_t,Double_t,std::pair<UInt_t,UInt_t>>> ztmu;

  ztmu = PreSelector::FindZ(Mus,GoodMuon);
  if(!ztmu.empty() && CheckMuonPair(std::get<2>((ztmu)[0]))) PairMu = true;

  ztel = PreSelector::FindZ(Els,GoodElectron);
  if(!ztel.empty() && CheckElectronPair(std::get<2>((ztel)[0]))) PairEl = true;

  if (!PairEl && !PairMu) {
    HCutFlow->FillS("NoPairs");
    return kFALSE;
  }

  if (PairEl && PairMu) {
    Float_t DeltaZMassEl = std::get<0>(ztel[0]);
    Float_t DeltaZMassMu = std::get<0>(ztmu[0]);

    if (DeltaZMassEl > DeltaZMassMu) {
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

  BestZMass = std::get<1>((*zt)[0]);
  Bool_t IsZMassOk = (BestZMass > MinZMass) && (BestZMass < MaxZMass);
  if(!IsZMassOk){
    HCutFlow->FillS("FailZMassWindow");
    return kFALSE;
  }

  l1 = (std::get<2>((*zt)[0])).first;
  l2 = (std::get<2>((*zt)[0])).second;

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
  Bool_t ZDistCut = l1l2Dist > 1.5 and l1l2Dist < 5.0 ;
  if(ZDistCut){
    HCutFlow->FillS("FailZDistCut");
    return kFALSE;
  }

  if(PairMu){

    for(auto i: GoodMuon){
      if(i!=l1 && i!=l2) WCand.emplace_back(i);
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

    if(IsC) DefineW(Els);
    if(IsD) DefineW(Mus);

    Bool_t ZHighPtIdCut = Muon_highPtId[l1]>=1 && Muon_highPtId[l2]>=2;
    if(!ZHighPtIdCut){
      HCutFlow->FillS("FailZHighPtIdCut");
      return kFALSE;
    }

    const float l1l3Dist = GetEtaPhiDistance(lep1.Eta(),lep1.Phi(),lep3.Eta(),lep3.Phi());
    Bool_t l1l3DistCut = l1l3Dist < 1.4 or l1l3Dist > 5.;
    if(l1l3DistCut){
      HCutFlow->FillS("Faill1l3DistCut");
      return kFALSE;
    }

    const float l2l3Dist = GetEtaPhiDistance(lep2.Eta(),lep2.Phi(),lep3.Eta(),lep3.Phi());
    Bool_t l2l3DistCut = l2l3Dist < 1.2 or l2l3Dist > 5.;
    if(l2l3DistCut){
      HCutFlow->FillS("Faill2l3DistCut");
      return kFALSE;
    }

    // 0e3mu
    if(IsD){
      if(Muon_pt[l3]>MinRemPt){
        if(Muon_highPtId[l3] == 2){
          FillCommon(Mus,Mus);
          FillD();
        } else {
          IsD = false;
          HCutFlow->FillS("Fail0e3muHighPtId");
        }
      } else {
        IsD = false;
        HCutFlow->FillS("Fail0e3muMinRemPt");
      }
    }

    // 1e2Mu
    if(IsC){
      if(Electron_pt[l3]>MinRemPt){
        FillCommon(Mus,Els);
        FillC();
      } else {
        HCutFlow->FillS("Fail1e2muMinRemPt");
        IsC = false;
      }
    }
  }


  if(PairEl){

    for(auto i: GoodElectron){
      if(i!=l1 && i!=l2) WCand.emplace_back(i);
    }

    if(WCand.size() > 0){
      if(GoodMuon.size() > 0){
        if(Muon_pt[GoodMuon[0]] > Electron_pt[WCand[0]] ){
          IsB = true;
          l3 = GoodMuon[0];
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

    if (IsA) DefineW(Els);
    if (IsB) DefineW(Mus);

    const float l1l3Dist = GetEtaPhiDistance(lep1.Eta(),lep1.Phi(),lep3.Eta(),lep3.Phi());
    Bool_t l1l3DistCut = l1l3Dist < 1.4 or l1l3Dist > 5.;
    if(l1l3DistCut){
      HCutFlow->FillS("Faill1l3DistCut");
      return kFALSE;
    }

    const float l2l3Dist = GetEtaPhiDistance(lep2.Eta(),lep2.Phi(),lep3.Eta(),lep3.Phi());
    Bool_t l2l3DistCut = l2l3Dist < 1.2 or l2l3Dist > 5.;
    if(l2l3DistCut){
      HCutFlow->FillS("Faill2l3DistCut");
      return kFALSE;
    }

    // 2e1mu
    if(IsB){
      if(Muon_pt[l3]>MinRemPt){
        if(Muon_highPtId[l3] == 2){
          FillCommon(Els,Mus);
          FillB();
        } else {
          IsB = false;
          HCutFlow->FillS("Fail2e1muHighPtId");
        }
      } else {
        IsB = false;
        HCutFlow->FillS("Fail2e1muMinRemPt");
      }
    }

    //3e0mu
    if(IsA){
      if(Electron_pt[l3]>MinRemPt){
        FillCommon(Els,Els);
        FillA();
      } else {
        IsA = false;
        HCutFlow->FillS("Fail3e0muMinRemPt");
      }
    }
  }

  // 3leptons
  HOverlap->Fill(IsA+IsB+IsC+IsD);

  return kTRUE;
}

void PreSelector::Terminate() {

  gStyle->SetOptStat(1111111);
  std::unique_ptr<TCanvas> ch(new TCanvas("ch","ch",1200,800));
  std::unique_ptr<TCanvas> chc(new TCanvas("chc","chc",1200,800));

  std::unique_ptr<TFile> fOut(TFile::Open("WprimeHistos.root","UPDATE"));
  fOut->mkdir(SampleName);
  fOut->cd(SampleName);

  ch->cd();
  ch->Divide(2,2);
  ch->cd(1);
  HWZDist->Draw("HIST");
  HWZDist->Write("HWZDist");
  ch->cd(2);
  HWZPtDist->Draw("COLZ");
  HWZPtDist->Write("HWZPtDist");
  ch->cd(3);
  HWZPt->Draw("HIST");
  HWZPt->Write("HWZPt");
  ch->Print(Form("%s_HWZDist.png",SampleName.Data()));
  ch->Clear();

  ch->cd();
  HCutFlow->LabelsDeflate();
  gPad->SetLogy();
  HCutFlow->Draw("HIST TEXT45");
  ch->Print(Form("%s_HCutFlow.png",SampleName.Data()));
  HCutFlow->Write("HCutFlow");

  ch->Clear();
  ch->Divide(2,2);
  ch->cd(1);
  HMassZWZA->Draw("COLZ");
  HMassZWZA->Write();
  ch->cd(2);
  HMassZWZB->Draw("COLZ");
  HMassZWZB->Write();
  ch->cd(3);
  HMassZWZC->Draw("COLZ");
  HMassZWZC->Write();
  ch->cd(4);
  HMassZWZD->Draw("COLZ");
  HMassZWZD->Write();
  ch->Print(Form("%s_HMassZWZ.png",SampleName.Data()));

  ch->Clear();
  ch->Divide(2,2);
  ch->cd(1);
  HDistl1l2->Draw("HIST");
  HDistl1l2->Write();
  ch->cd(2);
  HDistl1l3->Draw("HIST");
  HDistl1l3->Write();
  ch->cd(3);
  HDistl2l3->Draw("Hist");
  HDistl2l3->Write();
  ch->Print(Form("%s_HDistLeptons.png",SampleName.Data()));

  ch->Clear();
  ch->Divide(2,2);
  ch->cd(1);
  HPtl1->Draw("HIST");
  HPtl1->Write();
  ch->cd(2);
  HPtl2->Draw("HIST");
  HPtl2->Write();
  ch->cd(3);
  HPtl3->Draw("HIST");
  HPtl3->Write();
  ch->cd(4);
  HMetPt->Draw("HIST");
  HMetPt->Write();
  ch->Print(Form("%s_LeptonsPt.png",SampleName.Data()));

  THStack *hsA = new THStack("hsA","");
  THStack *hsB = new THStack("hsB","");
  THStack *hsC = new THStack("hsC","");
  THStack *hsD = new THStack("hsD","");


  auto SetStyle = [](TH1 *h) {
    gPad->SetGrid();
    gPad->SetLogy();
    h->SetFillColor(16);
    h->SetFillStyle(4050);
  };

  ch->Clear();
  ch->Divide(2,2);


  ch->cd(1);
  HMetA->SetTitle("3e0#mu;#slash{E}^{3e0#mu}_{T};Event count");
  SetStyle(HMetA);
  HMetA->Draw();
  HMetA->Write("HMetA");
  ch->cd(2);
  HMetB->SetTitle("2e1#mu;#slash{E}^{2e1#mu}_{T};Event count");
  SetStyle(HMetB);
  HMetB->Draw();
  HMetB->Write("HMetB");
  ch->cd(3);
  HMetC->SetTitle("1e2#mu;#slash{E}^{1e2#mu}_{T};Event count");
  SetStyle(HMetC);
  HMetC->Draw();
  HMetC->Write("HMetC");
  ch->cd(4);
  HMetD->SetTitle("0e3#mu;#slash{E}^{0e3#mu}_{T};Event count");
  SetStyle(HMetD);
  HMetD->Draw();
  HMetD->Write("HMetD");
  ch->Print(Form("%s_HMet.png",SampleName.Data()));
  chc->cd();
  TH1F HMet = *HMetA + *HMetB + *HMetC + *HMetD;
  HMet.SetTitle("MET;#slash{M}_{T};Event count");
  HMet.SetName("HMet");
  HMet.Write();
  HMet.Draw();
  chc->Print(Form("%s_HMet_SUM.png",SampleName.Data()));

  ch->cd(1);
  gStyle->SetOptStat(1111111);
  HnElA->SetFillColor(kGreen);
  HnMuA->SetFillColor(kBlue);
  hsA->Add(HnMuA);
  hsA->Add(HnElA);
  hsA->SetTitle("3e0#mu;n;Event count");
  hsA->Draw();
  ch->cd(2);
  HnElB->SetFillColor(kGreen);
  HnMuB->SetFillColor(kBlue);
  hsB->Add(HnMuB);
  hsB->Add(HnElB);
  hsB->SetTitle("2e1#mu;n;Event count");
  hsB->Draw();
  ch->cd(3);
  HnElC->SetFillColor(kGreen);
  HnMuC->SetFillColor(kBlue);
  hsC->Add(HnMuC);
  hsC->Add(HnElC);
  hsC->SetTitle("1e2#mu;n;Event count");
  hsC->Draw();
  ch->cd(4);
  HnElD->SetFillColor(kGreen);
  HnMuD->SetFillColor(kBlue);
  hsD->Add(HnMuD);
  hsD->Add(HnElD);
  hsD->SetTitle("0e3#mu;n;Event count");
  hsD->Draw();
  ch->Print(Form("%s_nGoodLeptons.png",SampleName.Data()));

  ch->cd(1);
  HMassWA->SetTitle("W Mass (3e0#mu);M_{W}^{3e0#mu};Event count");
  HMassWA->Draw();
  HMassWA->Write("HMassWA");
  ch->cd(2);
  HMassWB->SetTitle("W Mass (2e1#mu);M_{W}^{2e1#mu};Event count");
  HMassWB->Draw();
  HMassWB->Write("HMassWB");
  ch->cd(3);
  HMassWC->SetTitle("W Mass (1e2#mu);M_{W}^{1e2#mu};Event count");
  HMassWC->Draw();
  HMassWC->Write("HMassWC");
  ch->cd(4);
  HMassWD->SetTitle("W Mass (0e3#mu);M_{W}^{0e3#mu};Event count");
  HMassWD->Draw();
  HMassWD->Write("HMassWD");
  ch->Print(Form("%s_HMassW.png",SampleName.Data()));
  chc->cd();
  TH1F HMassW = *HMassWA + *HMassWB + *HMassWC + *HMassWD;
  HMassW.SetTitle("MassW;#slash{M}_{W};Event count");
  HMassW.SetName("HMassW");
  HMassW.Write();
  HMassW.Draw();
  chc->Print(Form("%s_HMassW_SUM.png",SampleName.Data()));

  ch->cd(1);
  HMassZA->SetTitle("Z Mass (3e0#mu);M_{Z}^{3e0#mu};Event count");
  HMassZA->Draw();
  HMassZA->Write("HMassZA");
  ch->cd(2);
  HMassZB->SetTitle("Z Mass (2e1#mu);M_{Z}^{2e1#mu};Event count");
  HMassZB->Draw();
  HMassZB->Write("HMassZB");
  ch->cd(3);
  HMassZC->SetTitle("Z Mass (1e2#mu);M_{Z}^{1e2#mu};Event count");
  HMassZC->Draw();
  HMassZC->Write("HMassZC");
  ch->cd(4);
  HMassZD->SetTitle("Z Mass (0e3#mu);M_{Z}^{0e3#mu};Event count");
  HMassZD->Draw();
  HMassZD->Write("HMassZD");
  ch->Print(Form("%s_HMassZ.png",SampleName.Data()));
  TH1F HMassZ = *HMassZA + *HMassZB + *HMassZC + *HMassZD;
  HMassZ.SetTitle("MassZ;#slash{M}_{Z};Event count");
  HMassZ.SetName("HMassZ");
  HMassZ.Write();
  HMassZ.Draw();
  chc->Print(Form("%s_HMassZ_SUM.png",SampleName.Data()));

  ch->cd(1);
  HMassTWA->SetTitle("M_{T}^{W}(3e0#mu);M_{WT}^{3e0#mu};Event count");
  HMassTWA->Draw();
  HMassTWA->Write("HMassTWA");
  ch->cd(2);
  HMassTWB->SetTitle("M_{T}^{W}(2e1#mu);M_{WT}^{2e1#mu};Event count");
  HMassTWB->Draw();
  HMassTWB->Write("HMassTWB");
  ch->cd(3);
  HMassTWC->SetTitle("M_{T}^{W}(1e2#mu);M_{WT}^{1e2#mu};Event count");
  HMassTWC->Draw();
  HMassTWC->Write("HMassTWC");
  ch->cd(4);
  HMassTWD->SetTitle("M_{T}^{W}(0e3#mu);M_{WT}^{0e3#mu};Event count");
  HMassTWD->Draw();
  HMassTWD->Write("HMassTWD");
  ch->Print(Form("%s_HMassTW.png",SampleName.Data()));
  TH1F HMassTW = *HMassTWA + *HMassTWB + *HMassTWC + *HMassTWD;
  HMassTW.SetTitle("MassTW;#slash{M}_{WT};Event count");
  HMassTW.Draw();
  HMassTW.SetName("HMassTW");
  HMassTW.Write();
  chc->Print(Form("%s_HMassTW_SUM.png",SampleName.Data()));

#ifndef CMSDATA
  ch->cd(1);
  HGenPartFA->SetTitle("PdgId Final State (3e0#mu); PdgId; Event count");
  HGenPartFA->LabelsDeflate();
  HGenPartFA->Write("HGenPartFA");
  HGenPartFA->Draw();
  ch->cd(2);
  HGenPartFB->SetTitle("PdgId Final State (2e1#mu); PdgId; Event count");
  HGenPartFB->LabelsDeflate();
  HGenPartFB->Write("HGenPartFB");
  HGenPartFB->Draw();
  ch->cd(3);
  HGenPartFC->SetTitle("PdgId Final State (1e2#mu); PdgId; Event count");
  HGenPartFC->LabelsDeflate();
  HGenPartFC->Write("HGenPartFC");
  HGenPartFC->Draw();
  ch->cd(4);
  HGenPartFD->SetTitle("PdgId Final State (0e3#mu); PdgId; Event count");
  HGenPartFD->LabelsDeflate();
  HGenPartFD->Write("HGenPartFD");
  HGenPartFD->Draw();
  ch->Print(Form("%s_PdgIdFinal.png",SampleName.Data()));
#endif

  ch->cd(1);
  HMassWZA->SetTitle("WZ Mass (3e0#mu);M_{WZ}^{3e0#mu};Event count");
  HMassWZA->Draw();
  HMassWZA->Write("HMassWZA");
  ch->cd(2);
  HMassWZB->SetTitle("WZ Mass (2e1#mu);M_{WZ}^{2e1#mu};Event count");
  HMassWZB->Draw();
  HMassWZB->Write("HMassWZB");
  ch->cd(3);
  HMassWZC->SetTitle("WZ Mass (1e2#mu);M_{WZ}^{1e2#mu};Event count");
  HMassWZC->Draw();
  HMassWZC->Write("HMassWZC");
  ch->cd(4);
  HMassWZD->SetTitle("WZ Mass (0e3#mu);M_{WZ}^{0e3#mu};Event count");
  HMassWZD->Draw();
  HMassWZD->Write("HMassWZD");
  ch->Print(Form("%s_HMassWZ.png",SampleName.Data()));
  TH1F HMassWZ = *HMassWZA + *HMassWZB + *HMassWZC + *HMassWZD;
  HMassWZ.SetTitle("MassWZ;#slash{M}_{WZ};Event count");
  HMassWZ.SetName("HMassWZ");
  HMassWZ.Write();
  HMassWZ.Draw();
  chc->Print(Form("%s_HMassWZ_SUM.png",SampleName.Data()));

  auto DrawPdgIdHisto = [](TH1F* hz, TH1F* hw){
    hz->LabelsDeflate();
    hz->SetBarWidth(0.4);
    hz->SetBarOffset(0.55);
    hz->SetFillColor(50);
    hz->Draw("BAR1");
    hw->LabelsDeflate();
    hw->SetBarWidth(0.4);
    hw->SetBarOffset(0.1);
    hw->SetFillColor(49);
    hw->Draw("BAR2,SAME");
    TLegend *legend = new TLegend(0.55,0.65,0.76,0.82);
    legend->AddEntry(hz,"Z pair","f");
    legend->AddEntry(hw,"W lepton","f");
    legend->Draw();
  };

#ifndef CMSDATA
  ch->Clear();
  ch->Divide(2,2);
  ch->cd(1);
  HGenPartZA->SetTitle("PdgId (3e0#mu); PdgId; Event count");
  DrawPdgIdHisto(HGenPartZA,HGenPartWA);
  HGenPartZA->Write("HGenPartZA");
  HGenPartWA->Write("HGenPartWA");
  ch->cd(2);
  HGenPartZB->SetTitle("PdgId (2e1#mu); PdgId; Event count");
  DrawPdgIdHisto(HGenPartZB,HGenPartWB);
  HGenPartZB->Write("HGenPartZB");
  HGenPartWB->Write("HGenPartWB");
  ch->cd(3);
  HGenPartZC->SetTitle("PdgId (1e2#mu); PdgId; Event count");
  DrawPdgIdHisto(HGenPartZC,HGenPartWC);
  HGenPartZC->Write("HGenPartZC");
  HGenPartWC->Write("HGenPartWC");
  ch->cd(4);
  HGenPartZD->SetTitle("PdgId (0e3#mu); PdgId; Event count");
  DrawPdgIdHisto(HGenPartZD,HGenPartWD);
  HGenPartZD->Write("HGenPartZD");
  HGenPartWD->Write("HGenPartWD");
  ch->Print(Form("%s_GenPartMother.png",SampleName.Data()));

  ch->Clear();
  ch->cd();
  HScaleFactors->SetTitle("ScaleFactors Distribution; SF; Event count");
  HScaleFactors->Draw();
  HScaleFactors->Write("HScaleFactors");
  ch->Print(Form("%s_HScaleFactors.png",SampleName.Data()));
#endif

  ch->Clear();
  ch->Divide(2,2);
  ch->cd(1);
  gPad->SetLogz();
  HNLepA->SetTitle("# Leptons (3e0#mu); GoodMuon Size; GoodElectron Size");
  HNLepA->Draw("COLZ");
  ch->cd(2);
  gPad->SetLogz();
  HNLepB->SetTitle("# Leptons (2e1#mu); GoodMuon Size; GoodElectron Size");
  HNLepB->Draw("COLZ");
  ch->cd(3);
  gPad->SetLogz();
  HNLepC->SetTitle("# Leptons (1e2#mu); GoodMuon Size; GoodElectron Size");
  HNLepC->Draw("COLZ");
  ch->cd(4);
  gPad->SetLogz();
  HNLepD->SetTitle("# Leptons (0e3#mu); GoodMuon Size; GoodElectron Size");
  HNLepD->Draw("COLZ");
  ch->Print(Form("%s_NGoodLeptons.png",SampleName.Data()));

  ch->cd(0);
  HOverlap->Draw();
  HOverlap->Write("HOverlap");
  ch->Print(Form("%s_Overlap.png",SampleName.Data()));

  ch->Clear();
  ch->cd();
  HPileup->Draw();
  HPileup->Write("HPileup");
  ch->Print(Form("%s_HPileupPriorToCuts.png",SampleName.Data()));

  ch->Clear();
  ch->Divide(2,2);
  ch->cd(1);
  HPileupA->Draw();
  HPileupA->Write("HPileupA");
  ch->cd(2);
  HPileupB->Draw();
  HPileupA->Write("HPileupB");
  ch->cd(3);
  HPileupC->Draw();
  HPileupA->Write("HPileupC");
  ch->cd(4);
  HPileupD->Draw();
  HPileupA->Write("HPileupD");
  ch->Print(Form("%s_HPileup.png",SampleName.Data()));

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
  ch->Print(Form("%s_CheckGoodLeptons.png",SampleName.Data()));

  fOut->Write();
  fOut->Close();

}
