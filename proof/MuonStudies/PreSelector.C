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

  HCutFlow = 0;
  HPResB_T = 0;
  HPResO_T = 0;
  HPResB_G = 0;
  HPResO_G = 0;
  HPResE_G = 0;

  HPtResB_T = 0;
  HPtResO_T = 0;
  HPtResB_G = 0;
  HPtResO_G = 0;
  HPtResE_G = 0;

  HMassZPt_A_G = 0;
  HMassZPt_B_G = 0;
  HMassZPt_C_G = 0;
  HMassZPt_A_T = 0;
  HMassZPt_B_T = 0;
  HMassZPt_C_T = 0;

  HMuonPtl1 = 0;
  HMuonPtl2 = 0;

  HMassZ = 0;
}

#ifndef CMSDATA
Double_t PreSelector::GetPtResidual(const Leptons& l, const int& idx) const{

  const int gpidx = l.genPartIdx[idx];

  PtEtaPhiMVector v4 = PtEtaPhiMVector(l.pt[idx],l.eta[idx],l.phi[idx],l.mass);
  PtEtaPhiMVector v4Gen = PtEtaPhiMVector(GenPart_pt[gpidx],
                                          GenPart_eta[gpidx],
                                          GenPart_phi[gpidx],
                                          GenPart_mass[gpidx]);

  return ( (1 / v4.Pt()) - (1/v4Gen.Pt()) ) / ( 1/v4Gen.Pt() );

}

Double_t PreSelector::GetPResidual(const Leptons& l, const int& idx) const{

  const int gpidx = l.genPartIdx[idx];

  PtEtaPhiMVector v4 = PtEtaPhiMVector(l.pt[idx],l.eta[idx],l.phi[idx],l.mass);
  PtEtaPhiMVector v4Gen = PtEtaPhiMVector(GenPart_pt[gpidx],
                                          GenPart_eta[gpidx],
                                          GenPart_phi[gpidx],
                                          GenPart_mass[gpidx]);

  return ( (1 / v4.P()) - (1/v4Gen.P()) ) / ( 1/v4Gen.P() );
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

  HCutFlow = new TH1D("HCutFlow","",50,0.,50.);  /* Limits are meaningless here */
  fOutput->Add(HCutFlow);

  const Double_t PBins[16] = {
    52.,72.,100.,150.,200.,
    300.,400.,600.,800.,1000.,
    1300.,1600.,2000.,2500,3100,
    4500
  };

  const Double_t PResBins[198] = {
    -0.99,-0.98,-0.97,-0.96,-0.95,-0.94,
    -0.93,-0.92,-0.91,-0.9,-0.89,-0.88,
    -0.87,-0.86,-0.85,-0.84,-0.83,-0.82,
    -0.81,-0.8,-0.79,-0.78,-0.77,-0.76,
    -0.75,-0.74,-0.73,-0.72,-0.71,-0.7,
    -0.69,-0.68,-0.67,-0.66,-0.65,-0.64,
    -0.63,-0.62,-0.61,-0.6,-0.59,-0.58,
    -0.57,-0.56,-0.55,-0.54,-0.53,-0.52,
    -0.51,-0.5,-0.49,-0.48,-0.47,-0.46,
    -0.45,-0.44,-0.43,-0.42,-0.41,-0.4,
    -0.39,-0.38,-0.37,-0.36,-0.35,-0.34,
    -0.33,-0.32,-0.31,-0.3,-0.29,-0.28,
    -0.27,-0.26,-0.25,-0.24,-0.23,-0.22,
    -0.21,-0.2,-0.19,-0.18,-0.17,-0.16,
    -0.15,-0.14,-0.13,-0.12,-0.11,-0.1,
    -0.09,-0.08,-0.07,-0.06,-0.05,-0.04,
    -0.03,-0.02,-0.01,7.5287e-16,0.01,0.02,
    0.03,0.04,0.05,0.06,0.07,0.08,
    0.09,0.1,0.11,0.12,0.13,0.14,
    0.15,0.16,0.17,0.18,0.19,0.2,
    0.21,0.22,0.23,0.24,0.25,0.26,
    0.27,0.28,0.29,0.3,0.31,0.32,
    0.33,0.34,0.35,0.36,0.37,0.38,
    0.39,0.4,0.41,0.42,0.43,0.44,
    0.45,0.46,0.47,0.48,0.49,0.5,
    0.51,0.52,0.53,0.54,0.55,0.56,
    0.57,0.58,0.59,0.6,0.61,0.62,
    0.63,0.64,0.65,0.66,0.67,0.68,
    0.69,0.7,0.71,0.72,0.73,0.74,
    0.75,0.76,0.77,0.78,0.79,0.8,
    0.81,0.82,0.83,0.84,0.85,0.86,
    0.87,0.88,0.89,0.9,0.91,0.92,
    0.93,0.94,0.95,0.96,0.97,0.98,
  };

  HPResB_T = new TH3F("HPResB_T","",15,PBins,15,PBins,197,PResBins);
  HPResO_T = static_cast<TH3F*>(HPResB_T->Clone());
  HPResO_T->SetName("HPResO_T");

  HPtResB_T = static_cast<TH3F*>(HPResB_T->Clone());
  HPtResB_T->SetName("HPtResB_T");
  HPtResO_T = static_cast<TH3F*>(HPtResB_T->Clone());
  HPtResO_T->SetName("HPtResO_T");

  HPResB_G = static_cast<TH3F*>(HPResB_T->Clone());
  HPResB_G->SetName("HPResB_G");
  HPResO_G = static_cast<TH3F*>(HPResB_T->Clone());
  HPResO_G->SetName("HPResO_G");
  HPResE_G = static_cast<TH3F*>(HPResB_T->Clone());
  HPResE_G->SetName("HPResE_G");

  HPtResB_G = static_cast<TH3F*>(HPResB_T->Clone());
  HPtResB_G->SetName("HPtResB_G");
  HPtResO_G = static_cast<TH3F*>(HPResB_T->Clone());
  HPtResO_G->SetName("HPtResO_G");
  HPtResE_G = static_cast<TH3F*>(HPResB_T->Clone());
  HPtResE_G->SetName("HPtResE_G");

  fOutput->Add(HPResB_T);
  fOutput->Add(HPResO_T);
  fOutput->Add(HPResB_G);
  fOutput->Add(HPResO_G);
  fOutput->Add(HPResE_G);

  fOutput->Add(HPtResB_T);
  fOutput->Add(HPtResO_T);
  fOutput->Add(HPtResB_G);
  fOutput->Add(HPtResO_G);
  fOutput->Add(HPtResE_G);

  HMuonPtl1 = new TH1F("HMuonPtl1","",50,0,5000);
  fOutput->Add(HMuonPtl1);

  HMuonPtl2 = new TH1F("HMuonPtl2","",50,0,5000);
  fOutput->Add(HMuonPtl2);


  const Double_t PtBins_MRes[8] = {
    53,75,100,150,200,300,450,800
  };

  const Double_t ZMassBins[19] = {
    70., 72., 74., 76., 78.,
    80., 82., 84., 86., 88.,
    90., 92., 94., 96., 98.,
    100., 102., 104., 106.
  };

  // A - Both Muons are within abs(eta) <= 1.2
  HMassZPt_A_G = new TH2F("HMassZPt_A_G","", 7, PtBins_MRes, 18, ZMassBins);
  fOutput->Add(HMassZPt_A_G);

  // B - Leading Muon is within 1.2 < abs(eta) <= 1.6
  HMassZPt_B_G = static_cast<TH2F*>(HMassZPt_A_G->Clone());
  HMassZPt_B_G->SetName("HMassZPt_B_G");
  fOutput->Add(HMassZPt_B_G);

  // C - Leading Muon is within abs(eta) > 1.6
  HMassZPt_C_G = static_cast<TH2F*>(HMassZPt_A_G->Clone());
  HMassZPt_C_G->SetName("HMassZPt_C_G");
  fOutput->Add(HMassZPt_C_G);

  HMassZPt_A_T = static_cast<TH2F*>(HMassZPt_A_G->Clone());
  HMassZPt_A_T->SetName("HMassZPt_A_T");
  fOutput->Add(HMassZPt_A_T);

  HMassZPt_B_T = static_cast<TH2F*>(HMassZPt_A_G->Clone());
  HMassZPt_B_T->SetName("HMassZPt_B_T");
  fOutput->Add(HMassZPt_B_T);

  HMassZPt_C_T = static_cast<TH2F*>(HMassZPt_A_G->Clone());
  HMassZPt_C_T->SetName("HMassZPt_C_T");
  fOutput->Add(HMassZPt_C_T);

  HMassZ = new TH1F("HMassZ","", 100,50,10000);
  fOutput->Add(HMassZ);


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

  if(Mu.pt[leadMuIdx] < 53.)  /* HLT_Mu50_OR_HLT_TkMu50 lower pt limit from SFDB*/
    return GoodIndex;
  const Float_t MaxEta = 2.4;
  const Float_t MinPt = 53.;
  if(abs(Mu.eta[leadMuIdx]) >= MaxEta){
    HCutFlow->FillS("LeadingMuOut");
    return GoodIndex;
  }
  GoodIndex.reserve(10);
  for (UInt_t i=0; i<*Mu.n;++i){
    Double_t pt = Mu.pt[i]*Muon_tunepRelPt[i];
    if( Muon_highPtId[i] >=1 and
        abs(Mu.eta[i]) < MaxEta and
        pt > MinPt and
        Muon_tkRelIso[i] < 0.1)
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



std::vector<std::pair<UInt_t,UInt_t>> PreSelector::GetMuonPairs(const Muons& m) const{

  // return leading pair sorted by Pt

  std::vector<std::pair<UInt_t,UInt_t>> pairs;

  for(uint i = 0; i < GoodMuon.size(); ++i){
    for(uint j = i+1; j < GoodMuon.size(); ++j){
      if (m.charge[i] != m.charge[j]) {
        if(m.pt[i] > m.pt[j]) {
          pairs.push_back(std::make_pair(GoodMuon[i],GoodMuon[j]));
        } else {
          pairs.push_back(std::make_pair(GoodMuon[j],GoodMuon[i]));
        }
      }
    }
  }

  return pairs;

};

float PreSelector::MassRecoZ(const float& pt1, const float& eta1, const float& phi1, const float& m1,
                             const float& pt2, const float& eta2, const float& phi2, const float& m2) const{

  ROOT::Math::PtEtaPhiMVector l1(pt1, eta1, phi1, m1);
  ROOT::Math::PtEtaPhiMVector l2(pt2, eta2, phi2, m2);

  return (l1+l2).M();
};

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


  for(const auto& pair: Pairs){
    UInt_t i = pair.first;
    UInt_t j = pair.second;
    Float_t m = PreSelector::MassRecoZ(l.pt[i],l.eta[i],l.phi[i],l.mass,
                                       l.pt[j],l.eta[j],l.phi[j],l.mass);
    z1.Mass = m;
    z1.Pair = pair;
    PairsWithMass.emplace_back(z1);
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

  if(abs(Mother.second) == abs(PdgId)){
    Mother = PreSelector::GetMother(Mother);
  }

  return Mother;
}
#endif


Float_t PreSelector::GetEtaPhiDistance(const float& eta1, const float& phi1,
                                        const float& eta2, const float& phi2) const{
  Double_t dphi = TVector2::Phi_mpi_pi(phi1-phi2);
  return sqrt(pow(eta2-eta1,2.)+pow(dphi,2.));
}

Bool_t PreSelector::CheckMuonPair(const std::pair<UInt_t,UInt_t>& p) const{

  const Float_t MinSubleadPt = 10.;
  if (Muon_pt[p.second] < MinSubleadPt) return kFALSE;
  const Float_t farFromPV = 1e-2;
  if (Muon_ip3d[p.first] > farFromPV or Muon_ip3d[p.second] > farFromPV) return kFALSE;

#ifndef CMSDATA
  const uint ZPdgId = 23;

  if ( GetMother(Muon_genPartIdx[p.first],MuPdgId).second != ZPdgId
       or
       GetMother(Muon_genPartIdx[p.second],MuPdgId).second != ZPdgId ){

    return kFALSE;

  };
#endif

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


Bool_t PreSelector::Process(Long64_t entry) {

  l1 = l2 = -1;
  PairMu = false;
  PairZMass = -1.;

  ReadEntry(entry);

  HCutFlow->FillS("NoCuts");

  if (!MuonTest()){
    HCutFlow->FillS("FailMuonHLTs");
    return kFALSE;
  }

  if (!FlagsTest()){
    HCutFlow->FillS("FailFlags");
    return kFALSE;
  }

#ifndef CMSDATA
  Muons Mus(nMuon,Muon_pt,Muon_eta,Muon_phi,
            Muon_charge,Muon_dxy,Muon_dz,Muon_pfRelIso03_all,
            Muon_ip3d,Muon_sip3d,
            Muon_tightId, Muon_genPartIdx, Muon_pdgId);
#endif
#ifdef CMSDATA
  Muons Mus(nMuon,Muon_pt,Muon_eta,Muon_phi,
            Muon_charge,Muon_dxy,Muon_dz,Muon_pfRelIso03_all,
            Muon_ip3d,Muon_sip3d,
            Muon_tightId);
#endif


  GoodMuon = PreSelector::GetGoodMuon(Mus);

  if( (GoodMuon.size()) < 2 ){
    HCutFlow->FillS("goodMu<2");
    return kFALSE;
  }

  ////////////// Define Z //////////////
  const Float_t MinZMass = 70.;
  const Float_t MaxZMass = 111.;


  ZPairInfo ztmu;

  ztmu = FindZ(Mus);
  if(!ztmu.empty() && CheckMuonPair(ztmu.Pair)) PairMu = true;

  if(PairMu){
     HCutFlow->FillS("PairMu");
  } else {
    HCutFlow->FillS("NoPair");
    return kFALSE;
  }

   l1 = ztmu.Pair.first;
   l2 = ztmu.Pair.second;

   lep1 = PtEtaPhiMVector(Muon_pt[l1]*Muon_tunepRelPt[l1],Muon_eta[l1],
                          Muon_phi[l1],Muons::mass);
   lep2 = PtEtaPhiMVector(Muon_pt[l2]*Muon_tunepRelPt[l2],Muon_eta[l2],
                          Muon_phi[l2],Muons::mass);

   zb   = lep1 + lep2;

   Double_t w = 1.;

   HMuonPtl1->Fill(lep1.Pt());
   HMuonPtl2->Fill(lep2.Pt());
   HMassZ->Fill(zb.M());

   HCutFlow->FillS("ZCandidate");

   auto FillHistos53 = [&](TH2F* A_G, TH2F* B_G, TH2F* C_G,
                           TH2F* A_T, TH2F* B_T, TH2F* C_T) {
     TH2F *hl1, *hl2 = NULL;

     std::pair<Float_t,Float_t> etaLimit = { 1.2, 1.6 };
     Int_t globalId = 2;

     if( abs(lep1.Eta()) <= etaLimit.first ) { // A
       if(Muon_highPtId[l1] == globalId){
         hl1 = A_G;
       } else {
         hl1 = A_T;
       }
       if(Muon_highPtId[l2] == globalId){
         hl2 = A_G;
       } else {
         hl2 = A_T;
       }
     } else if ( abs(lep1.Eta()) > etaLimit.first and
                 abs(lep1.Eta()) <= etaLimit.second ) { //B
       if(Muon_highPtId[l1] == globalId){
         hl1 = B_G;
       } else {
         hl1 = B_T;
       }
       if(Muon_highPtId[l2] == globalId){
         hl2 = B_G;
       } else {
         hl2 = B_T;
       }
     } else if (abs(lep1.Eta()) > etaLimit.second) {
       if(Muon_highPtId[l1] == globalId){
         hl1 = C_G;
       } else {
         hl1 = C_T;
       }
       if(Muon_highPtId[l2] == globalId){
         hl2 = C_G;
       } else {
         hl2 = C_T;
       }
     }

#ifndef CMSDATA
     hl1->Fill(lep1.Pt(), zb.M());
     hl2->Fill(lep2.Pt(), zb.M());
#elif defined(CMSDATA)
     hl1->Fill(lep1.Pt(), zb.M());
     hl2->Fill(lep2.Pt(), zb.M());
#endif
   };


   std::pair<float,float> MassWindow = { 75., 110. };

   if ( zb.M() > MassWindow.first and zb.M() < MassWindow.second ){
     FillHistos53(HMassZPt_A_G, HMassZPt_B_G, HMassZPt_C_G,
                  HMassZPt_A_T, HMassZPt_B_T, HMassZPt_C_T);
   }

#ifndef CMSDATA
   auto FillHistos = [&](TH3F* HPB,
                         TH3F* HPO,
                         TH3F* HPE,
                         PtEtaPhiMVector& l,
                         Double_t pRes_) {

     std::pair<float,float> MuonB = { 0., 1.2};
     std::pair<float,float> MuonO = { 1.2, 2.1};
     std::pair<float,float> MuonC = { 2.1, 2.4};

     if( abs(l.Eta()) <= MuonB.second ) {
       HPB->Fill(l.P(), l.Pt(), pRes_);
     } else if (l.Eta() > MuonO.first and l.Eta() <= MuonO.second) {
       HPO->Fill(l.P(), l.Pt(), pRes_);
     } else if (l.Eta() > MuonC.first and l.Eta() <= MuonC.second){
       HPE->Fill(l.P(), l.Pt(), pRes_);
     }
   };

   if( Muon_highPtId[l1] == 1 ){
     FillHistos(HPResB_T, HPResO_T, HPResO_T, lep1, GetPResidual(Mus,11));
     FillHistos(HPtResB_T, HPtResO_T, HPtResO_T, lep1, GetPtResidual(Mus,11));
   } else {
     FillHistos(HPResB_G, HPResO_G, HPResE_G, lep1, GetPResidual(Mus,l1));
     FillHistos(HPtResB_G, HPtResO_G, HPtResE_G, lep1,GetPtResidual(Mus,l1));
   }

   if( Muon_highPtId[l2] == 1){
     FillHistos(HPResB_T, HPResO_T, HPResO_T, lep2, GetPResidual(Mus,l2));
     FillHistos(HPtResB_T, HPtResO_T, HPtResO_T, lep2, GetPtResidual(Mus,l2));
   } else {
     FillHistos(HPResB_G, HPResO_G, HPResE_G, lep2, GetPResidual(Mus,l2));
     FillHistos(HPtResB_G, HPtResO_G, HPtResE_G, lep2, GetPtResidual(Mus,l2));
   }
#endif

   return kTRUE;
}

void PreSelector::Terminate() {

#ifdef Y2016
  const Int_t Year_ = 2016;
#elif defined(Y2017)
  const Int_t Year_ = 2017;
#elif defined(Y2018)
  const Int_t Year_ = 2018;
#endif

  std::unique_ptr<TFile> fOut(TFile::Open("MuonStudies_PtHistos.root","UPDATE"));
  fOut->mkdir(Form("%d",Year_));
  fOut->mkdir(Form("%d/%s",Year_,SampleName.Data()));
  fOut->cd(Form("%d/%s",Year_,SampleName.Data()));

  TObjLink *lnk = fOutput->FirstLink();
  while (lnk) {
    lnk->GetObject()->Write();
    lnk = lnk->Next();
  }

  fOut->Write();
  fOut->Close();

}
