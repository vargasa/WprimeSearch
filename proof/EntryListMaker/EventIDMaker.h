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
  Long64_t GetEventIndex(UInt_t run,ULong64_t event);
  TEntryList *EntryList;
  TTree *eTree;
  Int_t Year;
  Long64_t EventID;
  TTree *EventIndexTree;
  std::unordered_set<Long64_t> EventIndex;

  TH1F *hlog;

 public :

  TString SampleName;

  EventIDMaker(TTree * = 0);
  virtual ~EventIDMaker() { }
  virtual void    Begin(TTree *tree);
  virtual void    Init(TTree *tree);
  virtual void    SlaveBegin(TTree *tree);
  virtual Bool_t  Process(Long64_t entry);
  virtual void    Terminate();
  virtual Int_t   Version() const { return 2; }
  void BuildGoldenJson();

  Bool_t IsGold(UInt_t Run, UInt_t LuminosityBlock);

  ClassDef(EventIDMaker,0);
};

