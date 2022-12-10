#include "TFile.h"
#include "UniqueEntryListMaker.h"
#include "TError.h"
#include "TCanvas.h"

UniqueEntryListMaker::UniqueEntryListMaker(TTree *)
{
  EntryList = 0;
  hlog = 0;
}

void UniqueEntryListMaker::Init(TTree *tree)
{
  //Called every time a new TTree is attached.
  fReader.SetTree(tree);
}

Bool_t UniqueEntryListMaker::Notify() {
  std::clog << Form("Processing: %s\n",(fReader.GetTree())->GetCurrentFile()->GetEndpointUrl()->GetUrl());
  return true;
}

void UniqueEntryListMaker::Begin(TTree *tree) {

  if (fInput->FindObject("Year")) {
    TParameter<Int_t> *p = dynamic_cast<TParameter<Int_t>*>(fInput->FindObject("Year"));
    Year = p->GetVal();
  }

  if (fInput->FindObject("SampleName")) {
    // Lesson: TString can't be in TCollection
    TNamed *p = dynamic_cast<TNamed *>(fInput->FindObject("SampleName"));
    SampleName = p->GetTitle();
  }

  EntryList = new TEntryList("EntryList","Entry Number");
  fInput->Add(EntryList);

  std::clog << "SampleName: " << SampleName << std::endl;
  std::clog << "EntryList: " << EntryList << std::endl;

  if (fInput->FindObject("OutputLabel")) {
    // Lesson: TString can't be in TCollection
    TNamed *p = dynamic_cast<TNamed *>(fInput->FindObject("OutputLabel"));
    OutputLabel = p->GetTitle();
  }
  assert(OutputLabel.size()>0);

}

void UniqueEntryListMaker::SlaveBegin(TTree *tree) {

  hlog = new TH1D("hlog","hlog",100,0.,100.);
  fOutput->Add(hlog);

  if (fInput->FindObject("Year")) {
    TParameter<Int_t> *p = dynamic_cast<TParameter<Int_t>*>(fInput->FindObject("Year"));
    Year = p->GetVal();
  }

  if (fInput->FindObject("SampleName")) {
    TNamed *p = dynamic_cast<TNamed *>(fInput->FindObject("SampleName"));
    SampleName = p->GetTitle();
  }

  if (fInput->FindObject("EventIDTreePath")) {
    TNamed *p = dynamic_cast<TNamed *>(fInput->FindObject("EventIDTreePath"));
    EventIDTreePath = p->GetTitle();
    fEventIDTree = TFile::Open(EventIDTreePath.c_str(),"READ");
    assert(fEventIDTree != nullptr);
  }

  if((EntryList = (TEntryList*) fInput->FindObject("EntryList")))
    EntryList = (TEntryList *) EntryList->Clone();

  if(EntryList)
    fOutput->Add(EntryList);

  assert( Year == 2016 or Year == 2017 or Year == 2018);

  assert( SampleName.find("ULSingleElectron") != std::string::npos or
          SampleName.find("ULSingleMuon") != std::string::npos or
          SampleName.find("ULEGamma") != std::string::npos );

  if (Year == 2016 or Year == 2017) {
    AddTreeToEventIndex(Form("ULSinglePhoton_%d/eTree;1",Year));
    if (SampleName.find("ULSingleMuon") != std::string::npos){
      AddTreeToEventIndex(Form("ULSingleElectron_%d/eTree;1",Year));
    }
  } else if (Year == 2018) {
    AddTreeToEventIndex(Form("ULSingleMuon_%d/eTree;1",Year));
    assert(SampleName.find("ULEGamma") != std::string::npos);
  }

}

void UniqueEntryListMaker::AddTreeToEventIndex(std::string_view treeName){


  std::clog << "Adding TTree to main EventIndex : " << treeName.data() << '\t' << fEventIDTree->Get(treeName.data()) << "\n";

  TTree* EventIndexTree = dynamic_cast<TTree*>(fEventIDTree->Get(treeName.data()));
  assert(EventIndexTree != nullptr);
  TTreeReader fReader1(EventIndexTree);
  TTreeReaderValue<Long64_t> EvID(fReader1,"EventID");

  while(fReader1.Next()){
    hlog->Fill(treeName.data(),1.);
    EventIndex.insert(*EvID);
    //if (!(EventIndex.insert(*EvID).second))
    //  std::clog << Form("\tDuplicated EvID: %s %lld\n",treeName.data(),*EvID) ;
  }
}
Long64_t UniqueEntryListMaker::GetEventIndex(const UInt_t& run,const ULong64_t& event) {
  Long64_t uid = std::stoll(std::to_string(run)+std::to_string(event));
  assert(uid<numeric_limits<Long64_t>::max());
  return uid;
}

Bool_t UniqueEntryListMaker::Process(Long64_t entry) {

   fReader.SetEntry(entry);

   EventID = GetEventIndex(*run,*event);
   if(EventIndex.find(EventID) == EventIndex.end ()) {
     hlog->Fill(Form("UniqueEvent_%s",SampleName.c_str()),1.);
     EntryList->Enter(entry);
   } else {
     hlog->Fill(Form("DuplicatedEvent_%s",SampleName.c_str()),1.);
     std::clog << Form("\tDuplicated EvID: %lld\n",EventID);
   }

   return kTRUE;
}

void UniqueEntryListMaker::Terminate() {

  EntryList = dynamic_cast<TEntryList*>(fOutput->FindObject("EntryList"));
  std::unique_ptr<TFile> fEntryList(TFile::Open(Form("EntryListsUnique_%s.root",OutputLabel.c_str()),"UPDATE"));
  fEntryList->mkdir(Form("%s_%d",SampleName.c_str(),Year));
  fEntryList->cd(Form("%s_%d",SampleName.c_str(),Year));
  hlog->SetName(Form("hlog_%s_%d",SampleName.c_str(),Year));
  hlog->Write();
  EntryList->Write();
  fEntryList->Close();

}
