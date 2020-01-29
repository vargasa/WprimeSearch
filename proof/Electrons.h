#ifndef Leptons_h
#include "Leptons.h"
#endif

using RVUI = TTreeReaderValue<UInt_t>&;
using RAF = TTreeReaderArray<Float_t>&;
using RAI = TTreeReaderArray<Int_t>&;

class Electrons : public Leptons{
 public:
  Electrons(RVUI, RAF, RAF, RAF, RAI, RAF,
            RAF, RAI, RAF);
  ~Electrons() {};
  RAI cutBased;
  RAF miniPFRelIso_all;
  static constexpr Double_t mass = 0.510998950;
};

Electrons::Electrons(RVUI n, RAF pt, RAF eta, RAF phi,
                     RAI charge, RAF dxy, RAF dz, RAI cutBased,
                     RAF miniPFRelIso_all) :
Leptons(n,mass,pt,eta,phi,charge,dxy,dz), cutBased(cutBased), miniPFRelIso_all(miniPFRelIso_all)
{

}
