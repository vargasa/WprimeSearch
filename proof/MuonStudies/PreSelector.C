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
  HNMu = 0;
}

#ifndef CMSDATA
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

  const Double_t PtBins[13] = {
    120,200,300,400,600,800,1000,1300,1600,2000,2500,3100,4500
  };

  const Double_t PResBins[54] = {
    -0.26,-0.25,-0.24,-0.23,-0.22,-0.21,
    -0.20,-0.19,-0.18,-0.17,-0.16,-0.15,
    -0.14,-0.13,-0.12,-0.11,-0.10,-0.09,
    -0.08,-0.07,-0.06,-0.05,-0.04,-0.03,
    -0.02,-0.01, 0.00, 0.01, 0.02, 0.03,
    0.04, 0.05, 0.06, 0.07, 0.08, 0.09,
    0.10, 0.11, 0.12, 0.13, 0.14, 0.15,
    0.16, 0.17, 0.18, 0.19, 0.20, 0.21,
    0.22, 0.23, 0.24, 0.25, 0.26, 0.27
  };

  HPResidualB_TG = new TH2F("HPResidualB_TG","",12,PtBins,53,PResBins);
  HPResidualO_TG = static_cast<TH2F*>(HPResidualB_TG->Clone());
  HPResidualO_TG->SetName("HPResidualO_TG");
  HPResidualE_TG = static_cast<TH2F*>(HPResidualB_TG->Clone());
  HPResidualE_TG->SetName("HPResidualE_TG");

  HPResidualB_GG = static_cast<TH2F*>(HPResidualB_TG->Clone());
  HPResidualB_GG->SetName("HPResidualB_GG");
  HPResidualO_GG = static_cast<TH2F*>(HPResidualB_TG->Clone());
  HPResidualO_GG->SetName("HPResidualO_GG");
  HPResidualE_GG = static_cast<TH2F*>(HPResidualB_TG->Clone());
  HPResidualE_GG->SetName("HPResidualE_GG");

  HPResidualB_TT = static_cast<TH2F*>(HPResidualB_TG->Clone());
  HPResidualB_TT->SetName("HPResidualB_TT");
  HPResidualO_TT = static_cast<TH2F*>(HPResidualB_TG->Clone());
  HPResidualO_TT->SetName("HPResidualO_TT");
  HPResidualE_TT = static_cast<TH2F*>(HPResidualB_TG->Clone());
  HPResidualE_TT->SetName("HPResidualE_TT");

  fOutput->Add(HPResidualB_TG);
  fOutput->Add(HPResidualO_TG);
  fOutput->Add(HPResidualE_TG);
  fOutput->Add(HPResidualB_GG);
  fOutput->Add(HPResidualO_GG);
  fOutput->Add(HPResidualE_GG);
  fOutput->Add(HPResidualB_TT);
  fOutput->Add(HPResidualO_TT);
  fOutput->Add(HPResidualE_TT);

  HMuonPt = new TH1F("HMuonPt","",50,0,5000);
  fOutput->Add(HMuonPt);

  HMassZ = new TH1F("HMassZ","",62,0,3100);
  fOutput->Add(HMassZ);

  HNMu = new TH1I("HNMu","",7,0,7);
  fOutput->Add(HNMu);

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

  Muons Mus(nMuon,Muon_pt,Muon_eta,Muon_phi,
            Muon_charge,Muon_dxy,Muon_dz,Muon_pfRelIso03_all,
            Muon_ip3d,Muon_sip3d,
            Muon_tightId, Muon_genPartIdx, Muon_pdgId);


  GoodMuon = PreSelector::GetGoodMuon(Mus);

  HNMu->Fill(*nMuon,GoodMuon.size());

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

  // PairZMass = ztmu.Mass;
  // Bool_t IsZMassOk = (PairZMass > MinZMass) && (PairZMass < MaxZMass);
  // if(!IsZMassOk){
  //   HCutFlow->FillS("FailZMassWindow");
  //   return kFALSE;
  // }

   l1 = ztmu.Pair.first;
   l2 = ztmu.Pair.second;

   lep1 = PtEtaPhiMVector(Muon_pt[l1]*Muon_tunepRelPt[l1],Muon_eta[l1],
                          Muon_phi[l1],Muons::mass);
   lep2 = PtEtaPhiMVector(Muon_pt[l2]*Muon_tunepRelPt[l2],Muon_eta[l2],
                          Muon_phi[l2],Muons::mass);

   zb   = lep1 + lep2;

   HMuonPt->Fill(lep1.Pt());
   HMuonPt->Fill(lep2.Pt());

   HCutFlow->FillS("ZCandidate");


   HMassZ->Fill(zb.M());

   auto FillHistos = [&](TH2F* HPResidualB,
                        TH2F* HPResidualO,
                        TH2F* HPResidualE) {

     std::pair<float,float> MuonB = { 0., 0.9};
     std::pair<float,float> MuonO = { 0.9, 1.2};
     std::pair<float,float> MuonC = { 1.2, 2.4};

     if( abs(lep1.Eta()) < MuonB.second ) {
       HPResidualB->Fill(lep1.P(),GetPResidual(Mus,l1));
     } else if (abs(lep1.Eta()) > MuonO.first and abs(lep1.Eta()) < MuonO.second) {
       HPResidualO->Fill(lep1.P(),GetPResidual(Mus,l1));
     } else {
       HPResidualE->Fill(lep1.P(),GetPResidual(Mus,l1));
     }

     if( abs(lep2.Eta()) < MuonB.second ) {
       HPResidualB->Fill(lep2.P(),GetPResidual(Mus,l2));
     } else if (abs(lep2.Eta()) > MuonO.first and abs(lep2.Eta()) < MuonO.second) {
       HPResidualO->Fill(lep2.P(),GetPResidual(Mus,l2));
     } else {
       HPResidualE->Fill(lep2.P(),GetPResidual(Mus,l2));
     }
   };

   if( Muon_highPtId[l1] == 1 xor Muon_highPtId[l2] == 1 ){
     HCutFlow->FillS("Trk+Glb");
     FillHistos(HPResidualB_TG, HPResidualO_TG, HPResidualE_TG);
   } else if ( Muon_highPtId[l1] == 2 and Muon_highPtId[l2] == 2 ){
     HCutFlow->FillS("Glb+Glb");
     FillHistos(HPResidualB_GG, HPResidualO_GG, HPResidualE_GG);
   } else {
     HCutFlow->FillS("Trk+Trk");
     FillHistos(HPResidualB_TT, HPResidualO_TT, HPResidualE_TT);
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

  std::unique_ptr<TFile> fOut(TFile::Open("MuonStudies.root","UPDATE"));
  fOut->mkdir(Form("%d",Year));
  fOut->mkdir(Form("%d/%s",Year,SampleName.Data()));
  fOut->cd(Form("%d/%s",Year,SampleName.Data()));

  TObjLink *lnk = fOutput->FirstLink();
  while (lnk) {
    lnk->GetObject()->Write();
    lnk = lnk->Next();
  }

  fOut->Write();
  fOut->Close();

}
