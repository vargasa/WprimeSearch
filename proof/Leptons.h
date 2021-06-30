#ifndef Leptons_h
#define Leptons_h
#include "IsData.h"

using RVUI = TTreeReaderValue<UInt_t>&;
using RAF = TTreeReaderArray<Float_t>&;
using RAI = TTreeReaderArray<Int_t>&;

class Leptons{
 public:
  RVUI n;
  std::vector<Double_t> pt;
  RAF eta;
  RAF phi;
  RAI charge;
  RAF dxy;
  RAF dz;
  RAF relIso;
  RAF ip3d;
  RAF sip3d;
  Float_t mass;
#ifndef CMSDATA
  RAI genPartIdx;
  RAI pdgId;
#endif

 protected:
  std::vector<Double_t> InitializePtEls(RAF pti);
  std::vector<Double_t> InitializePtMus(RAF pti, RAF relPt);

  //Leptons constructor for Electrons
  Leptons(RVUI n, Float_t mass, RAF pt, RAF eta, RAF phi,
          RAI charge, RAF dxy, RAF dz, RAF relIso,
          RAF ip3d, RAF sip3d
#ifndef CMSDATA
          , RAI gpIdx, RAI pdgId
#endif
          );
  //Leptons constructor for Muons
  Leptons(RVUI n, Float_t mass, RAF relPt, RAF pt, RAF eta, RAF phi,
          RAI charge, RAF dxy, RAF dz, RAF relIso,
          RAF ip3d, RAF sip3d
#ifndef CMSDATA
          , RAI gpIdx, RAI pdgId
#endif
          );
};


std::vector<Double_t> Leptons::InitializePtEls(RAF pti) {

  std::vector<Double_t> ptf;

  for (size_t i = 0; i < *n/*pti.GetSize()*/; ++i) {
    ptf.emplace_back(pti[i]);
  }

  return ptf;

}

std::vector<Double_t> Leptons::InitializePtMus(RAF pti, RAF relPt){

  std::vector<Double_t> ptf;

  for(size_t i = 0; i < *n/*pti.GetSize()*/; ++i){
    pt.emplace_back(relPt[i]*pti[i]);
  }

  return ptf;
}

Leptons::Leptons(RVUI n, Float_t mass, RAF pt, RAF eta, RAF phi,
                 RAI charge, RAF dxy, RAF dz, RAF relIso,
                 RAF ip3d, RAF sip3d
#ifndef CMSDATA
                 , RAI gpIdx, RAI pdgId
#endif
                 ):
  n(n),mass(mass),pt(InitializePtEls(pt)),eta(eta),phi(phi),charge(charge),
    dxy(dxy),dz(dz),relIso(relIso),ip3d(ip3d),sip3d(sip3d)
#ifndef CMSDATA
  ,genPartIdx(gpIdx), pdgId(pdgId)
#endif
{

}



Leptons::Leptons(RVUI n, Float_t mass, RAF relPt, RAF pt, RAF eta, RAF phi,
                 RAI charge, RAF dxy, RAF dz, RAF relIso,
                 RAF ip3d, RAF sip3d
#ifndef CMSDATA
                 , RAI gpIdx, RAI pdgId
#endif
                 ):
  n(n),mass(mass),pt(InitializePtMus(pt,relPt)),eta(eta),phi(phi),charge(charge),
    dxy(dxy),dz(dz),relIso(relIso),ip3d(ip3d),sip3d(sip3d)
#ifndef CMSDATA
  ,genPartIdx(gpIdx), pdgId(pdgId)
#endif
{

}

#endif

