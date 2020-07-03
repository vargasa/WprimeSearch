#include "TSelector.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TParameter.h"
#include <memory>
#include "TEntryList.h"
#include "TH1F.h"

class UniqueEntryListMaker : public TSelector {

 private :

  TTreeReader fReader;

  TTreeReaderValue<UInt_t> run = {fReader, "run"};
  TTreeReaderValue<UInt_t> luminosityBlock = {fReader, "luminosityBlock"};
  TTreeReaderValue<ULong64_t> event = {fReader, "event"};

  Long64_t GetEventIndex(const UInt_t& run,const ULong64_t& event);
  Bool_t IsGold(const UInt_t& Run, const UInt_t& LuminosityBlock);

  TEntryList *EntryList;

  Long64_t EventID;
  TTree *EventIndexTree;
  std::unordered_set<Long64_t> EventIndex;
  void AddTreeToEventIndex(std::string_view treeName);

  TH1F *hlog;

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


  ClassDef(UniqueEntryListMaker,0);
};

