#include "EntryListMaker.h"
#include "TFile.h"
#define MAX_EVENTS 4294967295

EntryListMaker::EntryListMaker(TTree *){

}

void EntryListMaker::Init(TTree *tree){
  fReader.SetTree(tree);
}

void EntryListMaker::Begin(TTree *tree){

  if(fInput){
    EventBitmap = new TBits(MAX_EVENTS);
    fInput->Add(EventBitmap);
  }

}

void EntryListMaker::SlaveBegin(TTree *tree){
  if(fInput){
    if((EventBitmap = (TBits*) fInput->FindObject("TBits")))
      EventBitmap = (TBits *) EventBitmap->Clone();
    if(EventBitmap)
      fOutput->Add(EventBitmap);
  }
}

UInt_t EntryListMaker::FindIndex(long double run, long double event){
  return (UInt_t)((run/event)*MAX_EVENTS);
}

Bool_t EntryListMaker::Process(Long64_t entry){

  fReader.SetEntry(entry);

  EventBitmap->SetBitNumber(FindIndex(*run,*event));
  
  return kTRUE;
}

void EntryListMaker::Terminate(){

  TFile *f1 = TFile::Open("eListRunEventNumber.root","RECREATE");
  EventBitmap->Write("test");
  f1->Close();
}
