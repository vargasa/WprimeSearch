#include "TFile.h"
#include "PreSelector.h"
#include "TMath.h"
#include "TLegend.h"
#include "TError.h"
#include "TVector2.h"

#define FillS(xx) Fill(xx,1.)

PreSelector::PreSelector(TTree *)
{

  HMetA=0;
  HMetB=0;
  HMetC=0;
  HMetD=0;

  HMetPt=0;

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

  HLtA=0;
  HLtB=0;
  HLtC=0;
  HLtD=0;

  HMassZWZA=0;
  HMassZWZB=0;
  HMassZWZC=0;
  HMassZWZD=0;

  HMassZTW=0;
  HDeltaRPtZ=0;
  HPtWPtZ=0;
  HDeltaRMWZ=0;
  HLtMWZ=0;

  HOverlap=0;

  HDistl1l2=0;
  HDistl1l3=0;
  HDistl2l3=0;

#ifndef CMSDATA
  ELPass=0;
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

  HPileupA_SFUp = 0;
  HPileupB_SFUp = 0;
  HPileupC_SFUp = 0;
  HPileupD_SFUp = 0;

  HMetA_SFUp=0;
  HMetB_SFUp=0;
  HMetC_SFUp=0;
  HMetD_SFUp=0;

  HMassWA_SFUp=0;
  HMassWB_SFUp=0;
  HMassWC_SFUp=0;
  HMassWD_SFUp=0;

  HMassWZA_SFUp=0;
  HMassWZB_SFUp=0;
  HMassWZC_SFUp=0;
  HMassWZD_SFUp=0;

  HLtA_SFUp=0;
  HLtB_SFUp=0;
  HLtC_SFUp=0;
  HLtD_SFUp=0;

  HMassZA_SFUp=0;
  HMassZB_SFUp=0;
  HMassZC_SFUp=0;
  HMassZD_SFUp=0;

  HMassTWA_SFUp=0;
  HMassTWB_SFUp=0;
  HMassTWC_SFUp=0;
  HMassTWD_SFUp=0;

  HPileupA_SFDown = 0;
  HPileupB_SFDown = 0;
  HPileupC_SFDown = 0;
  HPileupD_SFDown = 0;

  HMetA_SFDown=0;
  HMetB_SFDown=0;
  HMetC_SFDown=0;
  HMetD_SFDown=0;

  HMassWA_SFDown=0;
  HMassWB_SFDown=0;
  HMassWC_SFDown=0;
  HMassWD_SFDown=0;

  HMassWZA_SFDown=0;
  HMassWZB_SFDown=0;
  HMassWZC_SFDown=0;
  HMassWZD_SFDown=0;

  HLtA_SFDown=0;
  HLtB_SFDown=0;
  HLtC_SFDown=0;
  HLtD_SFDown=0;

  HMassZA_SFDown=0;
  HMassZB_SFDown=0;
  HMassZC_SFDown=0;
  HMassZD_SFDown=0;

  HMassTWA_SFDown=0;
  HMassTWB_SFDown=0;
  HMassTWC_SFDown=0;
  HMassTWD_SFDown=0;

  HLog = 0;
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
  assert(x < h->GetXaxis()->GetXmax() && x > h->GetXaxis()->GetXmin());
  if(y > h->GetYaxis()->GetXmax())
    return GetSFFromHisto(h,x,h->GetYaxis()->GetXmax() - 1e-3,option);
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

  Double_t sf = -1;

  if(Year == 2016){
    /* 2 bins in pt */
    if( pt < 175.){
      sf = GetSFFromGraph(SFElectronTrigger1,eta,option);
    } else {
      sf = GetSFFromGraph(SFElectronTrigger2,eta,option);
    }
  }

  return sf;

}
#endif
#ifndef CMSDATA
Double_t PreSelector::GetMuonSF(const Float_t& eta, const Float_t& pt,
                               const Int_t& option) const{
  /* Option 0: Central Value, -1: Low, +1: up */

  Double_t sf = -1;

  if (Year == 2016) {
    const Double_t LumiBF = 3.11; //fb-1
    const Double_t LumiGH = 5.54;

    Double_t SFTriggerBF = GetSFFromHisto(SFMuonTriggerBF,abs(eta),pt,option);
    Double_t SFTriggerGH = GetSFFromHisto(SFMuonTriggerGH,abs(eta),pt,option);
    Double_t SFIDBF = GetSFFromHisto(SFMuonIDBF,eta,pt,option);
    Double_t SFIDGH = GetSFFromHisto(SFMuonIDGH,eta,pt,option);

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

  TH1::SetDefaultSumw2();

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

  const Float_t MinMass = 0.;
  const Float_t MaxMass = 2200.;
  const Int_t MassBins = 22;

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
  const Int_t ZMassBins = 30;
  HMassZA = new TH1F("HMassZA","",ZMassBins,HMinZMass,HMaxZMass);
  HMassZB = new TH1F("HMassZB","",ZMassBins,HMinZMass,HMaxZMass);
  HMassZC = new TH1F("HMassZC","",ZMassBins,HMinZMass,HMaxZMass);
  HMassZD = new TH1F("HMassZD","",ZMassBins,HMinZMass,HMaxZMass);

  const Float_t MaxTWMass = 250.;
  const Int_t TWMassBins = 50;
  HMassTWA = new TH1F("HMassTWA","",TWMassBins,0.,MaxTWMass);
  HMassTWB = new TH1F("HMassTWB","",TWMassBins,0.,MaxTWMass);
  HMassTWC = new TH1F("HMassTWC","",TWMassBins,0.,MaxTWMass);
  HMassTWD = new TH1F("HMassTWD","",TWMassBins,0.,MaxTWMass);

  fOutput->Add(HMassTWA);
  fOutput->Add(HMassTWB);
  fOutput->Add(HMassTWC);
  fOutput->Add(HMassTWD);

  const Float_t MaxWZMass = 2000.;
  const Int_t WZMassBins = 20;
  HMassWZA = new TH1F("HMassWZA","",WZMassBins,0.,MaxWZMass);
  HMassWZB = new TH1F("HMassWZB","",WZMassBins,0.,MaxWZMass);
  HMassWZC = new TH1F("HMassWZC","",WZMassBins,0.,MaxWZMass);
  HMassWZD = new TH1F("HMassWZD","",WZMassBins,0.,MaxWZMass);

  fOutput->Add(HMassWZA);
  fOutput->Add(HMassWZB);
  fOutput->Add(HMassWZC);
  fOutput->Add(HMassWZD);

  const Float_t MaxLt = 2000.;
  const Int_t NLtBins = 20;
  HLtA = new TH1F("HLtA","",NLtBins,0.,MaxLt);
  fOutput->Add(HLtA);
  HLtB = new TH1F("HLtB","",NLtBins,0.,MaxLt);
  fOutput->Add(HLtB);
  HLtC = new TH1F("HLtC","",NLtBins,0.,MaxLt);
  fOutput->Add(HLtC);
  HLtD = new TH1F("HLtD","",NLtBins,0.,MaxLt);
  fOutput->Add(HLtD);

  HMassZWZA = new TH2F("HMassZWZA","3e0mu;Z Mass;WZ Mass",MassBins,0.,1.5*HMaxZMass,MassBins,0.,MaxMass);
  fOutput->Add(HMassZWZA);
  HMassZWZB = new TH2F("HMassZWZB","2e1mu;Z Mass;WZ Mass",MassBins,0.,1.5*HMaxZMass,MassBins,0.,MaxMass);
  fOutput->Add(HMassZWZB);
  HMassZWZC = new TH2F("HMassZWZC","1e2mu;Z Mass;WZ Mass",MassBins,0.,1.5*HMaxZMass,MassBins,0.,MaxMass);
  fOutput->Add(HMassZWZC);
  HMassZWZD = new TH2F("HMassZWZD","0e3mu;Z Mass;WZ Mass",MassBins,0.,1.5*HMaxZMass,MassBins,0.,MaxMass);
  fOutput->Add(HMassZWZD);


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

  HOverlap = new TH1I("HOverlap","Overlapping events."
                      " -1: l<3 0:None 1: NoOverlap",6,-1,5);
  fOutput->Add(HOverlap);

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

  const UInt_t BinsPdgId = 100;
  const Float_t PdgIdMin = -50.;
  const Float_t PdgIdMax = 50.;

#ifndef CMSDATA
  ELPass = new TEntryList("ELPass","Events Passing Full Selection");
  fOutput->Add(ELPass);

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

  auto copyHisto = [](TH1F** hcopy, TH1F *h, const std::string& idstr){
    *hcopy = static_cast<TH1F*>(h->Clone());
    (*hcopy)->SetName(Form("%s_%s",h->GetName(),idstr.c_str()));
    return *hcopy;
  };

  fOutput->Add(copyHisto(&HMetA_SFUp,HMetA,"SFUp"));
  fOutput->Add(copyHisto(&HMetB_SFUp,HMetB,"SFUp"));
  fOutput->Add(copyHisto(&HMetC_SFUp,HMetC,"SFUp"));
  fOutput->Add(copyHisto(&HMetD_SFUp,HMetD,"SFUp"));

  fOutput->Add(copyHisto(&HPileupA_SFUp,HPileupA,"SFUp"));
  fOutput->Add(copyHisto(&HPileupB_SFUp,HPileupB,"SFUp"));
  fOutput->Add(copyHisto(&HPileupC_SFUp,HPileupC,"SFUp"));
  fOutput->Add(copyHisto(&HPileupD_SFUp,HPileupD,"SFUp"));

  fOutput->Add(copyHisto(&HMassWA_SFUp,HMassWA,"SFUp"));
  fOutput->Add(copyHisto(&HMassWB_SFUp,HMassWB,"SFUp"));
  fOutput->Add(copyHisto(&HMassWC_SFUp,HMassWC,"SFUp"));
  fOutput->Add(copyHisto(&HMassWD_SFUp,HMassWD,"SFUp"));

  fOutput->Add(copyHisto(&HMassWZA_SFUp,HMassWZA,"SFUp"));
  fOutput->Add(copyHisto(&HMassWZB_SFUp,HMassWZB,"SFUp"));
  fOutput->Add(copyHisto(&HMassWZC_SFUp,HMassWZC,"SFUp"));
  fOutput->Add(copyHisto(&HMassWZD_SFUp,HMassWZD,"SFUp"));

  fOutput->Add(copyHisto(&HLtA_SFUp,HLtA,"SFUp"));
  fOutput->Add(copyHisto(&HLtB_SFUp,HLtB,"SFUp"));
  fOutput->Add(copyHisto(&HLtC_SFUp,HLtC,"SFUp"));
  fOutput->Add(copyHisto(&HLtD_SFUp,HLtD,"SFUp"));

  fOutput->Add(copyHisto(&HMassZA_SFUp,HMassZA,"SFUp"));
  fOutput->Add(copyHisto(&HMassZB_SFUp,HMassZB,"SFUp"));
  fOutput->Add(copyHisto(&HMassZC_SFUp,HMassZC,"SFUp"));
  fOutput->Add(copyHisto(&HMassZD_SFUp,HMassZD,"SFUp"));

  fOutput->Add(copyHisto(&HMassTWA_SFUp,HMassTWA,"SFUp"));
  fOutput->Add(copyHisto(&HMassTWB_SFUp,HMassTWB,"SFUp"));
  fOutput->Add(copyHisto(&HMassTWC_SFUp,HMassTWC,"SFUp"));
  fOutput->Add(copyHisto(&HMassTWD_SFUp,HMassTWD,"SFUp"));

  fOutput->Add(copyHisto(&HMetA_SFDown,HMetA,"SFDown"));
  fOutput->Add(copyHisto(&HMetB_SFDown,HMetB,"SFDown"));
  fOutput->Add(copyHisto(&HMetC_SFDown,HMetC,"SFDown"));
  fOutput->Add(copyHisto(&HMetD_SFDown,HMetD,"SFDown"));

  fOutput->Add(copyHisto(&HPileupA_SFDown,HPileupA,"SFDown"));
  fOutput->Add(copyHisto(&HPileupB_SFDown,HPileupB,"SFDown"));
  fOutput->Add(copyHisto(&HPileupC_SFDown,HPileupC,"SFDown"));
  fOutput->Add(copyHisto(&HPileupD_SFDown,HPileupD,"SFDown"));

  fOutput->Add(copyHisto(&HMassWA_SFDown,HMassWA,"SFDown"));
  fOutput->Add(copyHisto(&HMassWB_SFDown,HMassWB,"SFDown"));
  fOutput->Add(copyHisto(&HMassWC_SFDown,HMassWC,"SFDown"));
  fOutput->Add(copyHisto(&HMassWD_SFDown,HMassWD,"SFDown"));

  fOutput->Add(copyHisto(&HMassWZA_SFDown,HMassWZA,"SFDown"));
  fOutput->Add(copyHisto(&HMassWZB_SFDown,HMassWZB,"SFDown"));
  fOutput->Add(copyHisto(&HMassWZC_SFDown,HMassWZC,"SFDown"));
  fOutput->Add(copyHisto(&HMassWZD_SFDown,HMassWZD,"SFDown"));

  fOutput->Add(copyHisto(&HLtA_SFDown,HLtA,"SFDown"));
  fOutput->Add(copyHisto(&HLtB_SFDown,HLtB,"SFDown"));
  fOutput->Add(copyHisto(&HLtC_SFDown,HLtC,"SFDown"));
  fOutput->Add(copyHisto(&HLtD_SFDown,HLtD,"SFDown"));

  fOutput->Add(copyHisto(&HMassZA_SFDown,HMassZA,"SFDown"));
  fOutput->Add(copyHisto(&HMassZB_SFDown,HMassZB,"SFDown"));
  fOutput->Add(copyHisto(&HMassZC_SFDown,HMassZC,"SFDown"));
  fOutput->Add(copyHisto(&HMassZD_SFDown,HMassZD,"SFDown"));

  fOutput->Add(copyHisto(&HMassTWA_SFDown,HMassTWA,"SFDown"));
  fOutput->Add(copyHisto(&HMassTWB_SFDown,HMassTWB,"SFDown"));
  fOutput->Add(copyHisto(&HMassTWC_SFDown,HMassTWC,"SFDown"));
  fOutput->Add(copyHisto(&HMassTWD_SFDown,HMassTWD,"SFDown"));

  HLog = new TH1F("HLog","",50,0,50.); /* Limits are meaningless here */
  fOutput->Add(HLog);

#endif
  HCutFlow = new TH1F("HCutFlow","",BinsPdgId,PdgIdMin,PdgIdMax);  /* Limits are meaningless here */
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
    if(!SFPileup) 
      std::clog << Form("WARNING: Pileup %s SF histogram not found!\nPileup weight will be taken as 1.\n",SampleName.Data());
  }

  HScaleFactors = new TH1F("HScaleFactors","HScaleFactors",60,0.,6.);
  fOutput->Add(HScaleFactors);
#endif

  std::clog << Form("PreSelector::SlaveBegin Year: %d\n",Year);

}

std::vector<UInt_t> PreSelector::GetGoodMuon(const Muons& Mu){
  std::vector<UInt_t> GoodIndex = {};
  if(!MuonTest())  /* HLT_Mu50_OR_HLT_TkMu50 lower pt limit from SFDB*/
    return GoodIndex;
  if(Mu.pt[0] < 52.)
    return GoodIndex;
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
  HDistl1l2->Fill(GetEtaPhiDistance(lz.eta[l1],lz.phi[l1],
                                      lz.eta[l2],lz.phi[l2]));
  HDistl1l3->Fill(GetEtaPhiDistance(lz.eta[l1],lz.phi[l1],
                                      lw.eta[l3],lw.phi[l3]));
  HDistl2l3->Fill(GetEtaPhiDistance(lz.eta[l2],lz.phi[l2],
                                      lw.eta[l3],lw.phi[l3]));
  HMetPt->Fill(*MET_pt);

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
  HNLepA->Fill(GoodMuon.size(),GoodElectron.size());
  HnElA->Fill(GoodElectron.size());
  HnMuA->Fill(GoodMuon.size());
  HMassZWZA->Fill(PairZMass,(wb+zb).M());
  HCutFlow->FillS("3e0mu");

#ifndef CMSDATA
  Double_t wup = 1.;
  Double_t wdown = 1.;

  wup = GetElectronSF(lep1.Eta(), lep1.Pt(),1);
  if (wup < 0.) HLog->FillS("ElectronSFUpFail_A");
  wup *= GetSFFromHisto(SFPileup,*PV_npvs);

  wdown = GetElectronSF(lep1.Eta(), lep1.Pt(),-1);
  if (wdown < 0.) HLog->FillS("ElectronSFDownFail_A");
  wdown *= GetSFFromHisto(SFPileup,*PV_npvs);

  HScaleFactors->Fill(wup);
  HScaleFactors->Fill(wdown);
  HGenPartFA->FillS(Form("%d",GenPart_pdgId[Electron_genPartIdx[l1]]));
  HGenPartFA->FillS(Form("%d",GenPart_pdgId[Electron_genPartIdx[l2]]));
  HGenPartFA->FillS(Form("%d",GenPart_pdgId[Electron_genPartIdx[l3]]));

  HGenPartZA->FillS(Form("%d",GetMother(Electron_genPartIdx[l1],
                                       Electron_pdgId[l1]).second));
  HGenPartZA->FillS(Form("%d",GetMother(Electron_genPartIdx[l2],
                                        Electron_pdgId[l2]).second));
  HGenPartWA->FillS(Form("%d",GetMother(Electron_genPartIdx[l3],
                                        Electron_pdgId[l3]).second));

  HPileupA_SFUp->Fill(*PV_npvs,wup);
  HMetA_SFUp->Fill(*MET_pt,wup);
  HMassWA_SFUp->Fill(wb.M(),wup);
  HMassWZA_SFUp->Fill((wb+zb).M(),wup);
  HLtA_SFUp->Fill(lt,wup);
  HMassZA_SFUp->Fill(PairZMass,wup);
  HMassTWA_SFUp->Fill(wmt,wup);

  HPileupA_SFDown->Fill(*PV_npvs,wdown);
  HMetA_SFDown->Fill(*MET_pt,wdown);
  HMassWA_SFDown->Fill(wb.M(),wdown);
  HMassWZA_SFDown->Fill((wb+zb).M(),wdown);
  HLtA_SFDown->Fill(lt,wdown);
  HMassZA_SFDown->Fill(PairZMass,wdown);
  HMassTWA_SFDown->Fill(wmt,wdown);

#endif

  HPileupA->Fill(*PV_npvs);
  HMetA->Fill(*MET_pt);
  HMassWA->Fill(wb.M());
  HMassWZA->Fill((wb+zb).M());
  HMassZA->Fill(PairZMass);
  HMassTWA->Fill(wmt);
  HLtA->Fill(lt);
}

void PreSelector::FillB(){
  Float_t lt = lep1.Pt()+lep2.Pt()+lep3.Pt();
  HNLepB->Fill(GoodMuon.size(),GoodElectron.size());
  HnElB->Fill(GoodElectron.size());
  HnMuB->Fill(GoodMuon.size());
  HMassZWZB->Fill(PairZMass,(wb+zb).M());
  HCutFlow->FillS("2e1mu");

#ifndef CMSDATA
  Double_t wup = 1.;
  Double_t wdown = 1.;

  wup = GetElectronSF(lep1.Eta(),lep1.Pt(),1);
  if (wup < 0.) HLog->FillS("ElectronSFUpFail_B");
  wup *= GetMuonSF(lep3.Eta(),lep3.Pt(),1);
  if (wup < 0.) HLog->FillS("MuonSFUpFail_B");
  wup *= GetSFFromHisto(SFPileup,*PV_npvs);

  wdown = GetElectronSF(lep1.Eta(),lep1.Pt(),-1);
  if (wdown < 0.) HLog->FillS("ElectronSFDownFail_B");
  wdown *= GetMuonSF(lep3.Eta(),lep3.Pt(),-1);
  if (wdown < 0.) HLog->FillS("MuonSFDownFail_B");
  wdown *= GetSFFromHisto(SFPileup,*PV_npvs);

  HScaleFactors->Fill(wup);
  HScaleFactors->Fill(wdown);
  HGenPartFB->FillS(Form("%d",GenPart_pdgId[Electron_genPartIdx[l1]]));
  HGenPartFB->FillS(Form("%d",GenPart_pdgId[Electron_genPartIdx[l2]]));
  HGenPartFB->FillS(Form("%d",GenPart_pdgId[Muon_genPartIdx[l3]]));

  HGenPartZB->FillS(Form("%d",GetMother(Electron_genPartIdx[l1],
                                       Electron_pdgId[l1]).second));
  HGenPartZB->FillS(Form("%d",GetMother(Electron_genPartIdx[l2],
                                       Electron_pdgId[l2]).second));
  HGenPartWB->FillS(Form("%d",GetMother(Muon_genPartIdx[l3],
                                       Muon_pdgId[l3]).second));

  HPileupB_SFUp->Fill(*PV_npvs,wup);
  HMetB_SFUp->Fill(*MET_pt,wup);
  HMassWB_SFUp->Fill(wb.M(),wup);
  HMassWZB_SFUp->Fill((wb+zb).M(),wup);
  HLtB_SFUp->Fill(lt,wup);
  HMassZB_SFUp->Fill(PairZMass,wup);
  HMassTWB_SFUp->Fill(wmt,wup);

  HPileupB_SFDown->Fill(*PV_npvs,wdown);
  HMetB_SFDown->Fill(*MET_pt,wdown);
  HMassWB_SFDown->Fill(wb.M(),wdown);
  HMassWZB_SFDown->Fill((wb+zb).M(),wdown);
  HLtB_SFDown->Fill(lt,wdown);
  HMassZB_SFDown->Fill(PairZMass,wdown);
  HMassTWB_SFDown->Fill(wmt,wdown);
#endif
  HPileupB->Fill(*PV_npvs);
  HMetB->Fill(*MET_pt);
  HMassWB->Fill(wb.M());
  HMassWZB->Fill((wb+zb).M());
  HMassZB->Fill(PairZMass);
  HMassTWB->Fill(wmt);
  HLtB->Fill(lt);
}

void PreSelector::FillC(){
  Float_t lt = lep1.Pt()+lep2.Pt()+lep3.Pt();
  HNLepC->Fill(GoodMuon.size(),GoodElectron.size());
  HnElC->Fill(GoodElectron.size());
  HnMuC->Fill(GoodMuon.size());
  HMassZWZC->Fill(PairZMass,(wb+zb).M());
  HCutFlow->FillS("1e2mu");

#ifndef CMSDATA
  Double_t wup = 1.;
  Double_t wdown = 1.;

  wup = GetMuonSF(lep1.Eta(),lep1.Pt(),1);
  if (wup < 0.) HLog->FillS("MuonSFUpFail_C");
  wup *= GetElectronSF(lep3.Eta(),lep3.Pt(),1);
  if (wup < 0.) HLog->FillS("ElectronSFUpFail_C");
  wup *= GetSFFromHisto(SFPileup,*PV_npvs);

  wdown = GetMuonSF(lep1.Eta(),lep1.Pt(),-1);
  if (wdown < 0.) HLog->FillS("MuonSFDownFail_C");
  wdown *= GetElectronSF(lep3.Eta(),lep3.Pt(),-1);
  if (wdown < 0.) HLog->FillS("ElectronSFDownFail_C");
  wdown *= GetSFFromHisto(SFPileup,*PV_npvs);

  HScaleFactors->Fill(wup);
  HScaleFactors->Fill(wdown);
  HGenPartFC->FillS(Form("%d",GenPart_pdgId[Muon_genPartIdx[l1]]));
  HGenPartFC->FillS(Form("%d",GenPart_pdgId[Muon_genPartIdx[l2]]));
  HGenPartFC->FillS(Form("%d",GenPart_pdgId[Electron_genPartIdx[l3]]));

  HGenPartZC->FillS(Form("%d",GetMother(Muon_genPartIdx[l1],
                                        Muon_pdgId[l1]).second));
  HGenPartZC->FillS(Form("%d",GetMother(Muon_genPartIdx[l2],
                                        Muon_pdgId[l2]).second));
  HGenPartWC->FillS(Form("%d",GetMother(Electron_genPartIdx[l3],
                                        Electron_pdgId[l3]).second));
  HPileupC_SFUp->Fill(*PV_npvs,wup);
  HMetC_SFUp->Fill(*MET_pt,wup);
  HMassWC_SFUp->Fill(wb.M(),wup);
  HMassWZC_SFUp->Fill((wb+zb).M(),wup);
  HLtC_SFUp->Fill(lt,wup);
  HMassZC_SFUp->Fill(PairZMass,wup);
  HMassTWC_SFUp->Fill(wmt,wup);

  HPileupC_SFDown->Fill(*PV_npvs,wdown);
  HMetC_SFDown->Fill(*MET_pt,wdown);
  HMassWC_SFDown->Fill(wb.M(),wdown);
  HMassWZC_SFDown->Fill((wb+zb).M(),wdown);
  HLtC->Fill(lt,wdown);
  HMassZC_SFDown->Fill(PairZMass,wdown);
  HMassTWC_SFDown->Fill(wmt,wdown);
#endif
  HPileupC->Fill(*PV_npvs);
  HMetC->Fill(*MET_pt);
  HMassWC->Fill(wb.M());
  HMassWZC->Fill((wb+zb).M());
  HMassZC->Fill(PairZMass);
  HMassTWC->Fill(wmt);
  HLtC->Fill(lt);
}

void PreSelector::DefineW(Leptons l){
  lep3 = PtEtaPhiMVector(l.pt[l3], l.eta[l3], l.phi[l3], l.mass);
  wmt = PreSelector::MassRecoW(lep3.Pt(), lep3.Phi(), *MET_pt, *MET_phi);
  nu = PreSelector::GetNu4V(lep3, wmt);
  wb = (lep3 + nu[0]);
}

void PreSelector::FillD(){
  Float_t lt = lep1.Pt()+lep2.Pt()+lep3.Pt();
  HNLepD->Fill(GoodMuon.size(),GoodElectron.size());
  HnElD->Fill(GoodElectron.size());
  HnMuD->Fill(GoodMuon.size());
  HMassZWZD->Fill(PairZMass,(wb+zb).M());
  HCutFlow->FillS("0e3mu");

#ifndef CMSDATA
  Double_t wup = 1.;
  Double_t wdown = 1.;

  wup = GetMuonSF(lep1.Eta(),lep1.Pt(),1);
  if (wup < 0.) HLog->FillS("MuonSFUpFail_D");
  wup *= GetSFFromHisto(SFPileup,*PV_npvs);

  wdown = GetMuonSF(lep1.Eta(),lep1.Pt(),-1);
  if (wdown < 0.) HLog->FillS("MuonSFUpFail_D");
  wdown *= GetSFFromHisto(SFPileup,*PV_npvs);
  HScaleFactors->Fill(wup);
  HScaleFactors->Fill(wdown);
  HGenPartFD->FillS(Form("%d",GenPart_pdgId[Muon_genPartIdx[l1]]));
  HGenPartFD->FillS(Form("%d",GenPart_pdgId[Muon_genPartIdx[l2]]));
  HGenPartFD->FillS(Form("%d",GenPart_pdgId[Muon_genPartIdx[l3]]));

  HGenPartZD->FillS(Form("%d",GetMother(Muon_genPartIdx[l1],
                                        Muon_pdgId[l1]).second));
  HGenPartZD->FillS(Form("%d",GetMother(Muon_genPartIdx[l2],
                                        Muon_pdgId[l2]).second));
  HGenPartWD->FillS(Form("%d",GetMother(Muon_genPartIdx[l3],
                                        Muon_pdgId[l3]).second));
  HPileupD_SFUp->Fill(*PV_npvs,wup);
  HMetD_SFUp->Fill(*MET_pt,wup);
  HMassWD_SFUp->Fill(wb.M(),wup);
  HMassWZD_SFUp->Fill((wb+zb).M(),wup);
  HLtD_SFUp->Fill(lt,wup);
  HMassZD_SFUp->Fill(PairZMass,wup);
  HMassTWD_SFUp->Fill(wmt,wup);

  HPileupD_SFDown->Fill(*PV_npvs,wdown);
  HMetD_SFDown->Fill(*MET_pt,wdown);
  HMassWD_SFDown->Fill(wb.M(),wdown);
  HMassWZD_SFDown->Fill((wb+zb).M(),wdown);
  HLtD_SFDown->Fill(lt,wdown);
  HMassZD_SFDown->Fill(PairZMass,wdown);
  HMassTWD_SFDown->Fill(wmt,wdown);

#endif
  HPileupD->Fill(*PV_npvs);
  HMetD->Fill(*MET_pt);
  HMassWD->Fill(wb.M());
  HMassWZD->Fill((wb+zb).M());
  HMassZD->Fill(PairZMass);
  HMassTWD->Fill(wmt);
  HLtD->Fill(lt);
}

Bool_t PreSelector::CheckElectronPair(const std::pair<UInt_t,UInt_t>& p) const{
  const Float_t MinPt = 35.;
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

#ifndef CMSDATA
    ELPass->Enter(entry);
#endif

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

#ifndef CMSDATA
    ELPass->Enter(entry);
#endif

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
  fOut->mkdir(SampleName);
  fOut->cd(SampleName);

  HLtA->Write();
  HLtB->Write();
  HLtC->Write();
  HLtD->Write();

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
  ch->Divide(2,3);
  ch->cd(1);
  HMassZTW->Draw();
  HMassZTW->Write();
  ch->cd(2);
  HDeltaRPtZ->Draw();
  HDeltaRPtZ->Write();
  ch->cd(3);
  HPtWPtZ->Draw();
  HPtWPtZ->Write();
  ch->cd(4);
  HDeltaRMWZ->Draw();
  HDeltaRMWZ->Write();
  ch->cd(5);
  HLtMWZ->Draw();
  HLtMWZ->Write();
  ch->Print(Form("%s_WindowMass.png",SampleName.Data()));

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
  HMassZA_SFUp->SetTitle("Z Mass (3e0#mu);M_{Z}^{3e0#mu};Event count");
  HMassZA_SFUp->Draw();
  HMassZA_SFUp->Write();
  ch->cd(2);
  HMassZB_SFUp->SetTitle("Z Mass (2e1#mu);M_{Z}^{2e1#mu};Event count");
  HMassZB_SFUp->Draw();
  HMassZB_SFUp->Write();
  ch->cd(3);
  HMassZC_SFUp->SetTitle("Z Mass (1e2#mu);M_{Z}^{1e2#mu};Event count");
  HMassZC_SFUp->Draw();
  HMassZC_SFUp->Write();
  ch->cd(4);
  HMassZD_SFUp->SetTitle("Z Mass (0e3#mu);M_{Z}^{0e3#mu};Event count");
  HMassZD_SFUp->Draw();
  HMassZD_SFUp->Write();
  ch->Print(Form("%s_HMassZ_SFUp.png",SampleName.Data()));

  ch->cd(1);
  HMassWA_SFUp->SetTitle("W Mass (3e0#mu);M_{W}^{3e0#mu};Event count");
  HMassWA_SFUp->Draw();
  HMassWA_SFUp->Write();
  ch->cd(2);
  HMassWB_SFUp->SetTitle("W Mass (2e1#mu);M_{W}^{2e1#mu};Event count");
  HMassWB_SFUp->Draw();
  HMassWB_SFUp->Write();
  ch->cd(3);
  HMassWC_SFUp->SetTitle("W Mass (1e2#mu);M_{W}^{1e2#mu};Event count");
  HMassWC_SFUp->Draw();
  HMassWC_SFUp->Write();
  ch->cd(4);
  HMassWD_SFUp->SetTitle("W Mass (0e3#mu);M_{W}^{0e3#mu};Event count");
  HMassWD_SFUp->Draw();
  HMassWD_SFUp->Write();

  ch->cd(1);
  HMassTWA_SFUp->SetTitle("M_{T}^{W}(3e0#mu);M_{WT}^{3e0#mu};Event count");
  HMassTWA_SFUp->Draw();
  HMassTWA_SFUp->Write();
  ch->cd(2);
  HMassTWB_SFUp->SetTitle("M_{T}^{W}(2e1#mu);M_{WT}^{2e1#mu};Event count");
  HMassTWB_SFUp->Draw();
  HMassTWB_SFUp->Write();
  ch->cd(3);
  HMassTWC_SFUp->SetTitle("M_{T}^{W}(1e2#mu);M_{WT}^{1e2#mu};Event count");
  HMassTWC_SFUp->Draw();
  HMassTWC_SFUp->Write();
  ch->cd(4);
  HMassTWD_SFUp->SetTitle("M_{T}^{W}(0e3#mu);M_{WT}^{0e3#mu};Event count");
  HMassTWD_SFUp->Draw();
  HMassTWD_SFUp->Write("");

  ch->cd(1);
  HMetA_SFUp->SetTitle("3e0#mu;#slash{E}^{3e0#mu}_{T};Event count");
  SetStyle(HMetA_SFUp);
  HMetA_SFUp->Draw();
  HMetA_SFUp->Write();
  ch->cd(2);
  HMetB_SFUp->SetTitle("2e1#mu;#slash{E}^{2e1#mu}_{T};Event count");
  SetStyle(HMetB_SFUp);
  HMetB_SFUp->Draw();
  HMetB_SFUp->Write();
  ch->cd(3);
  HMetC_SFUp->SetTitle("1e2#mu;#slash{E}^{1e2#mu}_{T};Event count");
  SetStyle(HMetC_SFUp);
  HMetC_SFUp->Draw();
  HMetC_SFUp->Write();
  ch->cd(4);
  HMetD_SFUp->SetTitle("0e3#mu;#slash{E}^{0e3#mu}_{T};Event count");
  SetStyle(HMetD_SFUp);
  HMetD_SFUp->Draw();
  HMetD_SFUp->Write();

  ch->cd(1);
  HMassWZA_SFUp->SetTitle("WZ Mass (3e0#mu);M_{WZ}^{3e0#mu};Event count");
  HMassWZA_SFUp->Draw();
  HMassWZA_SFUp->Write();
  ch->cd(2);
  HMassWZB_SFUp->SetTitle("WZ Mass (2e1#mu);M_{WZ}^{2e1#mu};Event count");
  HMassWZB_SFUp->Draw();
  HMassWZB_SFUp->Write();
  ch->cd(3);
  HMassWZC_SFUp->SetTitle("WZ Mass (1e2#mu);M_{WZ}^{1e2#mu};Event count");
  HMassWZC_SFUp->Draw();
  HMassWZC_SFUp->Write();
  ch->cd(4);
  HMassWZD_SFUp->SetTitle("WZ Mass (0e3#mu);M_{WZ}^{0e3#mu};Event count");
  HMassWZD_SFUp->Draw();
  HMassWZD_SFUp->Write();

  HLtA_SFUp->Write();
  HLtB_SFUp->Write();
  HLtC_SFUp->Write();
  HLtD_SFUp->Write();

  ch->cd(1);
  HMassZA_SFDown->SetTitle("Z Mass (3e0#mu);M_{Z}^{3e0#mu};Event count");
  HMassZA_SFDown->Draw();
  HMassZA_SFDown->Write();
  ch->cd(2);
  HMassZB_SFDown->SetTitle("Z Mass (2e1#mu);M_{Z}^{2e1#mu};Event count");
  HMassZB_SFDown->Draw();
  HMassZB_SFDown->Write();
  ch->cd(3);
  HMassZC_SFDown->SetTitle("Z Mass (1e2#mu);M_{Z}^{1e2#mu};Event count");
  HMassZC_SFDown->Draw();
  HMassZC_SFDown->Write();
  ch->cd(4);
  HMassZD_SFDown->SetTitle("Z Mass (0e3#mu);M_{Z}^{0e3#mu};Event count");
  HMassZD_SFDown->Draw();
  HMassZD_SFDown->Write();
  ch->Print(Form("%s_HMassZ_SFDown.png",SampleName.Data()));

  HLtA_SFDown->Write();
  HLtB_SFDown->Write();
  HLtC_SFDown->Write();
  HLtD_SFDown->Write();

  ch->cd(1);
  HMassWA_SFDown->SetTitle("W Mass (3e0#mu);M_{W}^{3e0#mu};Event count");
  HMassWA_SFDown->Draw();
  HMassWA_SFDown->Write();
  ch->cd(2);
  HMassWB_SFDown->SetTitle("W Mass (2e1#mu);M_{W}^{2e1#mu};Event count");
  HMassWB_SFDown->Draw();
  HMassWB_SFDown->Write();
  ch->cd(3);
  HMassWC_SFDown->SetTitle("W Mass (1e2#mu);M_{W}^{1e2#mu};Event count");
  HMassWC_SFDown->Draw();
  HMassWC_SFDown->Write();
  ch->cd(4);
  HMassWD_SFDown->SetTitle("W Mass (0e3#mu);M_{W}^{0e3#mu};Event count");
  HMassWD_SFDown->Draw();
  HMassWD_SFDown->Write();

  ch->cd(1);
  HMassTWA_SFDown->SetTitle("M_{T}^{W}(3e0#mu);M_{WT}^{3e0#mu};Event count");
  HMassTWA_SFDown->Draw();
  HMassTWA_SFDown->Write();
  ch->cd(2);
  HMassTWB_SFDown->SetTitle("M_{T}^{W}(2e1#mu);M_{WT}^{2e1#mu};Event count");
  HMassTWB_SFDown->Draw();
  HMassTWB_SFDown->Write();
  ch->cd(3);
  HMassTWC_SFDown->SetTitle("M_{T}^{W}(1e2#mu);M_{WT}^{1e2#mu};Event count");
  HMassTWC_SFDown->Draw();
  HMassTWC_SFDown->Write();
  ch->cd(4);
  HMassTWD_SFDown->SetTitle("M_{T}^{W}(0e3#mu);M_{WT}^{0e3#mu};Event count");
  HMassTWD_SFDown->Draw();
  HMassTWD_SFDown->Write("");

  ch->cd(1);
  HMetA_SFDown->SetTitle("3e0#mu;#slash{E}^{3e0#mu}_{T};Event count");
  SetStyle(HMetA_SFDown);
  HMetA_SFDown->Draw();
  HMetA_SFDown->Write();
  ch->cd(2);
  HMetB_SFDown->SetTitle("2e1#mu;#slash{E}^{2e1#mu}_{T};Event count");
  SetStyle(HMetB_SFDown);
  HMetB_SFDown->Draw();
  HMetB_SFDown->Write();
  ch->cd(3);
  HMetC_SFDown->SetTitle("1e2#mu;#slash{E}^{1e2#mu}_{T};Event count");
  SetStyle(HMetC_SFDown);
  HMetC_SFDown->Draw();
  HMetC_SFDown->Write();
  ch->cd(4);
  HMetD_SFDown->SetTitle("0e3#mu;#slash{E}^{0e3#mu}_{T};Event count");
  SetStyle(HMetD_SFDown);
  HMetD_SFDown->Draw();
  HMetD_SFDown->Write();

  ch->cd(1);
  HMassWZA_SFDown->SetTitle("WZ Mass (3e0#mu);M_{WZ}^{3e0#mu};Event count");
  HMassWZA_SFDown->Draw();
  HMassWZA_SFDown->Write();
  ch->cd(2);
  HMassWZB_SFDown->SetTitle("WZ Mass (2e1#mu);M_{WZ}^{2e1#mu};Event count");
  HMassWZB_SFDown->Draw();
  HMassWZB_SFDown->Write();
  ch->cd(3);
  HMassWZC_SFDown->SetTitle("WZ Mass (1e2#mu);M_{WZ}^{1e2#mu};Event count");
  HMassWZC_SFDown->Draw();
  HMassWZC_SFDown->Write();
  ch->cd(4);
  HMassWZD_SFDown->SetTitle("WZ Mass (0e3#mu);M_{WZ}^{0e3#mu};Event count");
  HMassWZD_SFDown->Draw();
  HMassWZD_SFDown->Write();

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


  ch->Clear();
  HLog->Draw("HIST TEXT45");
  HLog->Write();
  ch->Print(Form("%s_HLog.png",SampleName.Data()));
#endif

  ch->Clear();
  ch->Divide(2,2);
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

  ch->Clear();
  ch->Divide(2,2);
  ch->cd(1);
  HPileupA_SFUp->Draw();
  HPileupA_SFUp->Write();
  ch->cd(2);
  HPileupB_SFUp->Draw();
  HPileupB_SFUp->Write();
  ch->cd(3);
  HPileupC_SFUp->Draw();
  HPileupC_SFUp->Write();
  ch->cd(4);
  HPileupD_SFUp->Draw();
  HPileupD_SFUp->Write();
  ch->Print(Form("%s_HPileup_SFUpSFUp.png",SampleName.Data()));

  ch->cd(1);
  HPileupA_SFDown->Draw();
  HPileupA_SFDown->Write();
  ch->cd(2);
  HPileupB_SFDown->Draw();
  HPileupB_SFDown->Write();
  ch->cd(3);
  HPileupC_SFDown->Draw();
  HPileupC_SFDown->Write();
  ch->cd(4);
  HPileupD_SFDown->Draw();
  HPileupD_SFDown->Write();
  ch->Print(Form("%s_HPileup_SFDown.png",SampleName.Data()));

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
  HPileupB->Write("HPileupB");
  ch->cd(3);
  HPileupC->Draw();
  HPileupC->Write("HPileupC");
  ch->cd(4);
  HPileupD->Draw();
  HPileupD->Write("HPileupD");
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
