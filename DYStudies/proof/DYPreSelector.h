#include "TSelector.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TCanvas.h"
#include "TH1I.h"

class DYPreSelector : public TSelector {

 private :

  TTreeReader fReader;
  TTreeReaderValue<UInt_t> fnMuon;

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

