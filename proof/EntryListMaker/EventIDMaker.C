#include "TFile.h"
#include "EventIDMaker.h"
#include "TError.h"
#include "BuildGoldenJson.hxx"

EventIDMaker::EventIDMaker(TTree *)
{
  eTree = 0;
  EntryList = 0;
  EventIndexTree = 0;
}

void EventIDMaker::Init(TTree *tree)
{
  //Called every time a new TTree is attached.
  fReader.SetTree(tree);
}

void EventIDMaker::Begin(TTree *tree) {

  if (fInput->FindObject("SampleName")) {
    // Lesson: TString can't be in TCollection
    TNamed *p = dynamic_cast<TNamed *>(fInput->FindObject("SampleName"));
    SampleName = p->GetTitle();
  }

  if (fInput->FindObject("Year")) {
    TParameter<Int_t> *p = dynamic_cast<TParameter<Int_t>*>(fInput->FindObject("Year"));
    Year = p->GetVal();
  }
}

void EventIDMaker::SlaveBegin(TTree *tree) {

  BuildGoldenJson();

  EntryList = new TEntryList("EntryList","Entry Number");
  fOutput->Add(EntryList);
  
  eTree = new TTree("eTree","eTree");
  eTree->Branch("EventID",&EventID);
  fOutput->Add(eTree);

} 

Bool_t EventIDMaker::IsGold(UInt_t Run, UInt_t LuminosityBlock){
  for (auto LumiRange: GoldenJson[Run]) {
    if (LuminosityBlock >= LumiRange.first && LuminosityBlock <= LumiRange.second) return true;
  }
  return false;
}

Long64_t EventIDMaker::GetEventIndex(UInt_t run,ULong64_t event) {
  if ( run > 3e5 or event > 6e9) {
    std::cerr << Form("EventIDMaker::GetEventIndex() Unexpected range for run[%d] or event[%llu]\n",
		      run,event);
  }
  return std::stol(std::to_string(run)+std::to_string(event));
}

Bool_t EventIDMaker::Process(Long64_t entry) {

  ReadEntry(entry,Year);

  if (!MinLeptonsTest()) return kFALSE;
  
  if (!IsGold(*run,*luminosityBlock)) return kFALSE;

   // Event Selection
  if ( (ElectronTest() || MuonTest()) && *MET_pt > 30 ) {
    EventID = GetEventIndex(*run,*event);
    eTree->Fill();
    EntryList->Enter(entry);
  }
  return kTRUE;
}

void EventIDMaker::Terminate() {

  std::string dirName = Form("%s_%d",SampleName.Data(),Year);
  std::unique_ptr<TFile> fEventIDTree(TFile::Open("EventIDTree.root","UPDATE"));
  fEventIDTree->mkdir(dirName.c_str());
  fEventIDTree->cd(dirName.c_str());
  eTree = dynamic_cast<TTree*>(fOutput->FindObject("eTree"));
  eTree->Write("eTree");
  fEventIDTree->Close();
  
  std::unique_ptr<TFile> fEntryList(TFile::Open("EntryLists.root","UPDATE"));
  fEntryList->mkdir(dirName.c_str());
  fEntryList->cd(dirName.c_str());
  EntryList->Write("EntryList");
  fEntryList->Close();

}
