#include "TSelector.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TParameter.h"
#include <memory>
#include "TEntryList.h"
#include "TH1D.h"

class UniqueEntryListMaker : public TSelector {

 private :

  TTreeReader fReader;

  TTreeReaderValue<UInt_t> run = {fReader, "run"};
  TTreeReaderValue<UInt_t> luminosityBlock = {fReader, "luminosityBlock"};
  TTreeReaderValue<ULong64_t> event = {fReader, "event"};

  Long64_t GetEventIndex(const UInt_t& run,const ULong64_t& event);
  Bool_t IsGold(const UInt_t& Run, const UInt_t& LuminosityBlock);

  TEntryList *EntryList;

  std::string EventIDTreePath;
  TFile* fEventIDTree;
  Long64_t EventID;
  
  std::unordered_set<Long64_t> EventIndex;
  void AddTreeToEventIndex(std::string_view treeName);

  int Year;
  TH1D *hlog;

 public :

  std::string SampleName;

  UniqueEntryListMaker(TTree * = 0);
  ~UniqueEntryListMaker() { }
  void    Begin(TTree *tree);
  void    Init(TTree *tree);
  void    SlaveBegin(TTree *tree);
  Bool_t  Process(Long64_t entry);
  void    Terminate();
  Int_t   Version() const { return 2; }
  Bool_t  Notify();
  void BuildGoldenJson();


  ClassDef(UniqueEntryListMaker,0);
};

