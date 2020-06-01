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

  if (fInput){
    EntryList = new TEntryList("EntryList","Entry Number");
    fInput->Add(EntryList);
    eTree = new TTree("eTree","eTree");
    fInput->Add(eTree);
  }
}

void EventIDMaker::SlaveBegin(TTree *tree) {

  BuildGoldenJson();

  if((EntryList = (TEntryList*) fInput->FindObject("EntryList")))
    EntryList = (TEntryList *) EntryList->Clone();
  
  if(EntryList)
    fOutput->Add(EntryList);
  
  if((eTree = (TTree*) fInput->FindObject("eTree"))){
    eTree = (TTree*) eTree->Clone();
    eTree->Branch("EventID",&EventID);
  }
  

  if(eTree)
    fOutput->Add(eTree);
  
} 

Bool_t EventIDMaker::IsGold(UInt_t Run, UInt_t LuminosityBlock){
  for (auto LumiRange: GoldenJson[Run]) {
    if (LuminosityBlock >= LumiRange.first && LuminosityBlock <= LumiRange.second) return true;
  }
  return false;
}

Long64_t EventIDMaker::GetEventIndex(UInt_t run,ULong64_t event) {
  // run < 285500 && event < 5e9
  return std::stol(std::to_string(run)+std::to_string(event));
}

Bool_t EventIDMaker::Process(Long64_t entry) {

  this->ReadEntry(entry,Year);
  
  if (!IsGold(*run,*luminosityBlock)) return kFALSE;

   // Event Selection
  if ( (this->ElectronTest() || this->MuonTest()) && *MET_pt > 30 ) {
    EventID = GetEventIndex(*run,*event);
    eTree->Fill();
    EntryList->Enter(entry);
  }
  return kTRUE;
}

void EventIDMaker::Terminate() {

  std::unique_ptr<TFile> fEventIDTree(TFile::Open("EventIDTree.root","UPDATE"));
  fEventIDTree->mkdir(Form("%s_%d",SampleName.Data(),Year));
  fEventIDTree->cd(SampleName);
  eTree = dynamic_cast<TTree*>(fOutput->FindObject("eTree"));
  eTree->Write();
  fEventIDTree->Close();
  
  EntryList = dynamic_cast<TEntryList*>(fOutput->FindObject("EntryList"));
  std::unique_ptr<TFile> fEntryList(TFile::Open("EntryLists.root","UPDATE"));
  fEntryList->mkdir(Form("%s_%d",SampleName.Data(),Year));
  fEntryList->cd(SampleName);
  EntryList->Write();
  fEntryList->Close();

}
