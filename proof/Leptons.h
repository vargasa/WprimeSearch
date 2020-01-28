#ifndef Leptons_h
#define Leptons_h

using RVUI = TTreeReaderValue<UInt_t>&;
using RAF = TTreeReaderArray<Float_t>&;
using RVI = TTreeReaderArray<Int_t>&;

class Leptons{
 public:
  RVUI n;
  RAF pt;
  RAF eta;
  RAF phi;
  RVI charge;
  RAF dxy;
  RAF dz;
  Float_t mass;

 protected:
  Leptons(RVUI n, Float_t mass, RAF pt, RAF eta, RAF phi,
          RVI charge, RAF dxy, RAF dz);
};

Leptons::Leptons(RVUI n, Float_t mass, RAF pt, RAF eta, RAF phi,
                 RVI charge, RAF dxy, RAF dz ) :
n(n),mass(mass),pt(pt),eta(eta),phi(phi),charge(charge),dxy(dxy),dz(dz)
{

}

#endif

