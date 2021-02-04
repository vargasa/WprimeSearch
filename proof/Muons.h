
#ifndef Leptons_h
#include "Leptons.h"
#endif

#include "IsData.h"

using RVUI = TTreeReaderValue<UInt_t>&;
using RAF = TTreeReaderArray<Float_t>&;
using RAI = TTreeReaderArray<Int_t>&;
using RAB = TTreeReaderArray<Bool_t>&;

class Muons : public Leptons{
 public:
  Muons(RVUI, RAF, RAF, RAF, RAI, RAF,
        RAF, RAF, RAF, RAF, RAB
#ifndef CMSDATA
        , RAI, RAI
#endif
        );
  ~Muons() {};

  RAB tightId;

  static constexpr Float_t mass = 0.105658755;
};

Muons::Muons(TTreeReaderValue<UInt_t>& n, RAF pt, RAF eta,
             RAF phi, RAI charge,
             RAF dxy, RAF dz, RAF relIso,
             RAF ip3d, RAF sip3d,
             RAB tightId
#ifndef CMSDATA
             , RAI gpIdx, RAI pdgId
#endif
             ) :
  Leptons(n,mass,pt,eta,phi,charge,dxy,dz,relIso,ip3d,sip3d
#ifndef CMSDATA
  , gpIdx, pdgId
#endif
          ), tightId(tightId)
{

}
