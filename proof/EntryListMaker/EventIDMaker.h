#include "TSelector.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TParameter.h"
#include <memory>
#include "TEntryList.h"
#include "EventSelection.h"
#include "TH1F.h"

class EventIDMaker : public EventSelection {

 private :

  TTreeReaderValue<UInt_t> run = {fReader, "run"};
  TTreeReaderValue<UInt_t> luminosityBlock = {fReader, "luminosityBlock"};
  TTreeReaderValue<ULong64_t> event = {fReader, "event"};

  TTreeReaderValue<Float_t> MET_pt = {fReader, "MET_pt"};

  std::unordered_map<Int_t, std::vector<std::pair<UInt_t,UInt_t>>> GoldenJson;
  Long64_t GetEventIndex(const UInt_t& run,const ULong64_t& event);
  TEntryList *EntryList;
  TTree *eTree;
  Long64_t EventID;
  TTree *EventIndexTree;
  std::unordered_set<Long64_t> EventIndex;

  TH1F *hlog;

  Bool_t BrokenTree{};

 public :

  TString SampleName;

  EventIDMaker(TTree * = 0);
  ~EventIDMaker() { }
  void    Begin(TTree *tree);
  void    SlaveBegin(TTree *tree);
  Bool_t  Process(Long64_t entry);
  void    Terminate();
  Int_t   Version() const { return 2; }
  void BuildGoldenJson();

  Bool_t IsGold(const UInt_t& Run,const UInt_t& LuminosityBlock);

  ClassDef(EventIDMaker,0);
};

