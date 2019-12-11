#include "TSelector.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"
#include "TCanvas.h"
#include "TH1I.h"

class DYPreSelector : public TSelector {

 private :

  TTreeReader fReader;
  TTreeReaderValue<UInt_t> nMuon = {fReader, "nMuon"};
  TTreeReaderValue<Bool_t> Flag_goodVertices = {fReader, "Flag_goodVertices"};
  TTreeReaderValue<Bool_t> Flag_globalSuperTightHalo2016Filter = {fReader, "Flag_globalSuperTightHalo2016Filter"};
  TTreeReaderValue<Bool_t> Flag_HBHENoiseFilter = {fReader, "Flag_HBHENoiseFilter"};
  TTreeReaderValue<Bool_t> Flag_HBHENoiseIsoFilter = {fReader, "Flag_HBHENoiseIsoFilter"};
  TTreeReaderValue<Bool_t> Flag_EcalDeadCellTriggerPrimitiveFilter =  {fReader, "Flag_EcalDeadCellTriggerPrimitiveFilter"};
  TTreeReaderValue<Bool_t> Flag_BadPFMuonFilter {fReader, "Flag_BadPFMuonFilter"};

  TTreeReaderValue<UInt_t> nTau = {fReader, "nTau"};
  TTreeReaderArray<Float_t> Tau_pt = {fReader, "Tau_pt"};
  TTreeReaderArray<Float_t> Tau_dxy = {fReader, "Tau_dxy"};
  TTreeReaderArray<Float_t> Tau_dz = {fReader, "Tau_dz"};
  TTreeReaderArray<Float_t> Tau_eta = {fReader, "Tau_eta"};
  TTreeReaderArray<Float_t> Tau_mass = {fReader, "Tau_mass"};
  TTreeReaderArray<Float_t> Tau_phi = {fReader, "Tau_phi"};
  TTreeReaderArray<Int_t> Tau_charge = {fReader, "Tau_charge"};


  TH1I *hnMuon;
  TH1F *hTauPt;
  TH1F *hTauMass;
  TH1F *hTauMass2;
  TCanvas *ch;
  

 public :

  DYPreSelector(TTree * = 0);
  virtual ~DYPreSelector() { }
  virtual void    Begin(TTree *tree);
  virtual void    Init(TTree *tree);
  virtual void    SlaveBegin(TTree *tree);
  virtual Bool_t  Process(Long64_t entry);
  virtual void    Terminate();
  virtual Int_t   Version() const { return 2; }

  ClassDef(DYPreSelector,0);
};

