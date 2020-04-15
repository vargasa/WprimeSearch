#include "TSelector.h"
#include "TTree.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"

class EntryListMaker : public TSelector {

 private:

  TTreeReader fReader;
  TTreeReaderValue<UInt_t> run = {fReader, "run"};
  TTreeReaderValue<UInt_t> luminosityBlock = {fReader, "luminosityBlock"};
  TTreeReaderValue<ULong64_t> event = {fReader, "event"};

  TBits *EventBitmap;

 public:
  UInt_t FindIndex(long double run, long double event);
  EntryListMaker(TTree * = 0);
  virtual ~EntryListMaker() {}
  virtual void Begin(TTree *tree);
  virtual void Init(TTree *tree);
  virtual Bool_t Process(Long64_t entry);
  virtual void SlaveBegin(TTree *tree);
  virtual void Terminate();
  virtual Int_t Version() const { return 2; }

  ClassDef(EntryListMaker,0);
};
