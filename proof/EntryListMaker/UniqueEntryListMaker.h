#include "TSelector.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TParameter.h"
#include <memory>
#include "TEntryList.h"

class UniqueEntryListMaker : public TSelector {

 private :

  TTreeReader fReader;

  TTreeReaderValue<UInt_t> run = {fReader, "run"};
  TTreeReaderValue<UInt_t> luminosityBlock = {fReader, "luminosityBlock"};
  TTreeReaderValue<ULong64_t> event = {fReader, "event"};

  Long64_t GetEventIndex(UInt_t run,ULong64_t event);
  TEntryList *EntryList;

  Long64_t EventID;
  TTree *EventIndexTree;
  std::unordered_set<Long64_t> EventIndex;
  void AddTreeToEventIndex(std::string treeName);

 public :

  TString SampleName;

  UniqueEntryListMaker(TTree * = 0);
  virtual ~UniqueEntryListMaker() { }
  virtual void    Begin(TTree *tree);
  virtual void    Init(TTree *tree);
  virtual void    SlaveBegin(TTree *tree);
  virtual Bool_t  Process(Long64_t entry);
  virtual void    Terminate();
  virtual Int_t   Version() const { return 2; }
  void BuildGoldenJson();

  Bool_t IsGold(UInt_t Run, UInt_t LuminosityBlock);

  ClassDef(UniqueEntryListMaker,0);
};

