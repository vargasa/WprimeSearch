#ifndef Leptons_h
#define Leptons_h
#include "IsData.h"

using RVUI = TTreeReaderValue<UInt_t>&;
using RAF = TTreeReaderArray<Float_t>&;
using RAI = TTreeReaderArray<Int_t>&;

class Leptons{
 public:
  RVUI n;
  RAF pt;
  RAF eta;
  RAF phi;
  RAI charge;
  RAF dxy;
  RAF dz;
  Float_t mass;
#ifndef CMSDATA
  RAI genPartIdx;
  RAI pdgId;
#endif

 protected:
  Leptons(RVUI n, Float_t mass, RAF pt, RAF eta, RAF phi,
          RAI charge, RAF dxy, RAF dz
#ifndef CMSDATA
          , RAI gpIdx, RAI pdgId
#endif
          );
};

Leptons::Leptons(RVUI n, Float_t mass, RAF pt, RAF eta, RAF phi,
                 RAI charge, RAF dxy, RAF dz
#ifndef CMSDATA
                 , RAI gpIdx, RAI pdgId
#endif
                 ):
  n(n),mass(mass),pt(pt),eta(eta),phi(phi),charge(charge),
  dxy(dxy),dz(dz)
#ifndef CMSDATA
  ,genPartIdx(gpIdx), pdgId(pdgId)
#endif
{

}

#endif

