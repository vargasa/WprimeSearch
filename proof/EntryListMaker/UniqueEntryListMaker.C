#include "TFile.h"
#include "UniqueEntryListMaker.h"
#include "TError.h"
#include "TCanvas.h"

UniqueEntryListMaker::UniqueEntryListMaker(TTree *)
{
  EntryList = 0;
}

void UniqueEntryListMaker::Init(TTree *tree)
{
  //Called every time a new TTree is attached.
  fReader.SetTree(tree);
}

void UniqueEntryListMaker::Begin(TTree *tree) {

  if (fInput->FindObject("SampleName")) {
    // Lesson: TString can't be in TCollection
    TNamed *p = dynamic_cast<TNamed *>(fInput->FindObject("SampleName"));
    SampleName = p->GetTitle();
  }
  if (fInput){
    EntryList = new TEntryList("EntryList","Entry Number");
    fInput->Add(EntryList);
  }

  std::clog << "SampleName: " << SampleName << std::endl;
  std::clog << "EntryList: " << EntryList << std::endl;
}

void UniqueEntryListMaker::SlaveBegin(TTree *tree) {

  hlog = new TH1F("hlog","hlog",100,0,100);
  fOutput->Add(hlog);

  if (fInput->FindObject("SampleName")) {
    TNamed *p = dynamic_cast<TNamed *>(fInput->FindObject("SampleName"));
    SampleName = p->GetTitle();
  }

  if((EntryList = (TEntryList*) fInput->FindObject("EntryList")))
    EntryList = (TEntryList *) EntryList->Clone();
  
  if(EntryList)
    fOutput->Add(EntryList);
    
  if (fInput->FindObject("EventIndexTree1"))
    AddTreeToEventIndex("EventIndexTree1");
  if (fInput->FindObject("EventIndexTree2"))
    AddTreeToEventIndex("EventIndexTree2");
  
} 

void UniqueEntryListMaker::AddTreeToEventIndex(std::string treeName){
  EventIndexTree = dynamic_cast<TTree *>(fInput->FindObject(treeName.c_str()));
  TTreeReader fReader1(EventIndexTree);
  TTreeReaderValue<Long64_t> EvID(fReader1,"EventID");
    
  while(fReader1.Next()){
    hlog->Fill(treeName.c_str(),1.);
    EventIndex.insert(*EvID);
  }
}
Long64_t UniqueEntryListMaker::GetEventIndex(UInt_t run,ULong64_t event) {
  // run < 285500 && event < 5e9
  return std::stol(std::to_string(run)+std::to_string(event));
}

Bool_t UniqueEntryListMaker::Process(Long64_t entry) {

   fReader.SetEntry(entry);

   EventID = GetEventIndex(*run,*event);
   if(EventIndex.find(EventID) == EventIndex.end ()) {
     hlog->Fill(Form("UniqueEvent_%s",SampleName.Data());
     EntryList->Enter(entry);
   } else {
     hlog->Fill(Form("DuplicatedEvent_%s",SampleName.Data());
   }

   return kTRUE;
}

void UniqueEntryListMaker::Terminate() {

  std::unique_ptr<TCanvas> ch(new TCanvas("ch","ch",1200,800));
  gPad->SetLogy();
  hlog->LabelsDeflate();
  hlog->Draw("HIST TEXT45");
  ch->Print(Form("UniqueEntryListMaker_%s_hlog.png",SampleName.Data()));

  EntryList = dynamic_cast<TEntryList*>(fOutput->FindObject("EntryList"));
  std::unique_ptr<TFile> fEntryList(TFile::Open("EntryLists_Unique.root","UPDATE"));
  fEntryList->mkdir(SampleName);
  fEntryList->cd(SampleName);
  EntryList->Write();
  fEntryList->Close();

}
