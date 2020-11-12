#ifndef Leptons_h
#include "Leptons.h"
#endif

#include "IsData.h"

using RVUI = TTreeReaderValue<UInt_t>&;
using RAF = TTreeReaderArray<Float_t>&;
using RAI = TTreeReaderArray<Int_t>&;

class Electrons : public Leptons{
 public:
  Electrons(RVUI, RAF, RAF, RAF, RAI, RAF,
            RAF, RAI
#ifndef CMSDATA
            , RAI, RAI
#endif
            );
  ~Electrons() {};
  RAI cutBased;
  static constexpr Float_t mass = 0.510998950;
};

Electrons::Electrons(RVUI n, RAF pt, RAF eta, RAF phi,
                     RAI charge, RAF dxy, RAF dz, RAI cutBased
#ifndef CMSDATA
                     , RAI gpIdx, RAI pdgId
#endif
                     ) :
  Leptons(n,mass,pt,eta,phi,charge,dxy,dz
#ifndef CMSDATA
  , gpIdx, pdgId
#endif
          ), cutBased(cutBased)
{

}
