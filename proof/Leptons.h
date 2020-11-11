#ifndef Leptons_h
#define Leptons_h

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
  RAI genPartIdx;
  RAI pdgId;

 protected:
  Leptons(RVUI n, Float_t mass, RAF pt, RAF eta, RAF phi,
          RAI charge, RAF dxy, RAF dz, RAI gpIdx, RAI pdgId);
};

Leptons::Leptons(RVUI n, Float_t mass, RAF pt, RAF eta, RAF phi,
                 RAI charge, RAF dxy, RAF dz, RAI gpIdx, RAI pdgId ) :
  n(n),mass(mass),pt(pt),eta(eta),phi(phi),charge(charge),
  dxy(dxy),dz(dz),genPartIdx(gpIdx),pdgId(pdgId)
{

}

#endif

