#include "TFile.h"
#include "EventIDMaker.h"
#include "TError.h"
#include "BuildGoldenJson.hxx"
#include "TCanvas.h"

#define FillS(ss) Fill(ss,1.);

EventIDMaker::EventIDMaker(TTree *)
{
  eTree = 0;
  EntryList = 0;
  EventIndexTree = 0;
  hlog = 0;
}

void EventIDMaker::Init(TTree *tree)
{

  //Called every time a new TTree is attached.

  for(auto brn: BranchNamesList){
    TBranch *b = tree->FindBranch(brn);
    if(b == nullptr){
      std::cerr << "EventIDMaker::Init Error: Tree " << tree->GetName() 
		<< " Branch: " << brn << " not found" << std::endl;
      std::cerr << "URL: " << tree->GetCurrentFile()->GetEndpointUrl()->GetUrl() <<std::endl;
      BrokenTree = true;
    }
  }

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

  std::clog << "EventIDMaker::Begin SampleName:" << SampleName 
	    << "Year " << Year;

}

void EventIDMaker::SlaveBegin(TTree *tree) {

  BuildGoldenJson();

  EntryList = new TEntryList("EntryList","Entry Number");
  fOutput->Add(EntryList);
  
  eTree = new TTree("eTree","eTree");
  eTree->Branch("EventID",&EventID);
  fOutput->Add(eTree);

  if (fInput->FindObject("Year")) {
    TParameter<Int_t> *p = dynamic_cast<TParameter<Int_t>*>(fInput->FindObject("Year"));
    Year = p->GetVal();
  }

  std::clog << "EventIDMaker::SlaveBegin Year " << Year;

  hlog = new TH1F("hlog","hlog",100,0,100); /*Ranges are meaningless here*/
  fOutput->Add(hlog);

} 

Bool_t EventIDMaker::IsGold(UInt_t Run, UInt_t LuminosityBlock){
  for (auto LumiRange: GoldenJson[Run]) {
    if (LuminosityBlock >= LumiRange.first && LuminosityBlock <= LumiRange.second) return true;
  }
  return false;
}

Long64_t EventIDMaker::GetEventIndex(UInt_t run,ULong64_t event) {
  if ( run > 3e5 or event > 6e9) {
    hlog->FillS("EventIDOutOfRange");
    std::cerr << Form("EventIDMaker::GetEventIndex() Unexpected range for run[%d] or event[%llu]\n",
		      run,event);
  }
  return std::stol(std::to_string(run)+std::to_string(event));
}

Bool_t EventIDMaker::Process(Long64_t entry) {

  if(BrokenTree){
    hlog->FillS("BrokenTree");
    Abort("BrokenTree, Check logs!");
    return kFALSE;
  }

  ReadEntry(entry,Year);

  hlog->FillS("Total");

  if (!MinLeptonsTest()){
    hlog->FillS("FailMinLeptonsTest");
    return kFALSE;
  }
  
  if (!IsGold(*run,*luminosityBlock)){
    hlog->FillS("FailGoldenJsonTest");
    return kFALSE;
  }

  if (ElectronTest()){
    hlog->FillS("FailElectronTest");
  }

  if (MuonTest()){
    hlog->FillS("FailMuonTest");
  }

  if (*MET_pt<=30.){
    hlog->FillS("FailsMETPtTest");
  }

   // Event Selection
  if ( (ElectronTest() || MuonTest()) && *MET_pt > 30 ) {
    hlog->FillS("Passed");
    EventID = GetEventIndex(*run,*event);
    eTree->Fill();
    EntryList->Enter(entry);
  }
  return kTRUE;
}

void EventIDMaker::Terminate() {

  std::unique_ptr<TCanvas> ch(new TCanvas("ch","ch",1200,800));
  gPad->SetLogy();
  hlog->LabelsDeflate();
  hlog->Draw("HIST TEXT45");
  ch->Print(Form("EventIDMaker_hlog_%s_%d.png",SampleName.Data(),Year));

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
