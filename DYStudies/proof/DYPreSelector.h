#include "TSelector.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
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

  TH1I *hnMuon;
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

