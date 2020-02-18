
#ifndef Leptons_h
#include "Leptons.h"
#endif


using RVUI = TTreeReaderValue<UInt_t>&;
using RAF = TTreeReaderArray<Float_t>&;
using RAI = TTreeReaderArray<Int_t>&;
using RAB = TTreeReaderArray<Bool_t>&;

class Muons : public Leptons{
 public:
  Muons(RVUI, RAF, RAF, RAF, RAI, RAF,
        RAF, RAB, RAB);
  ~Muons() {};

  RAB looseId;
  RAB isGlobal;

  static constexpr Float_t mass = 0.105658755;
};

Muons::Muons(TTreeReaderValue<UInt_t>& n, RAF pt, RAF eta,
             RAF phi, RAI charge,
             RAF dxy, RAF dz, RAB looseId,
             RAB isGlobal) :
Leptons(n,mass,pt,eta,phi,charge,dxy,dz), looseId(looseId), isGlobal(isGlobal){

}
