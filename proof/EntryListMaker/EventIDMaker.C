#include "../IsData.h"
#include "TFile.h"
#include "EventIDMaker.h"
#include "TError.h"
#ifndef ULSAMPLE
#include "BuildGoldenJson.hxx"
#elif defined(ULSAMPLE)
#include "BuildGoldenJsonUL.hxx"
#endif
#include "TCanvas.h"

#define FillS(ss) Fill(ss,1.);

EventIDMaker::EventIDMaker(TTree *)
{
  eTree = 0;
  EntryList = 0;
  EventIndexTree = 0;
  hlog = 0;
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

  if (fInput->FindObject("OutputLabel")) {
    // Lesson: TString can't be in TCollection
    TNamed *p = dynamic_cast<TNamed *>(fInput->FindObject("OutputLabel"));
    OutputLabel = p->GetTitle();
  }
  assert(OutputLabel.size()>0);

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

  std::clog << Form("EventIDMaker::SlaveBegin Year:%d \n", Year);

  hlog = new TH1D("hlog","hlog",100,0.,100.); /*Ranges are meaningless here*/
  fOutput->Add(hlog);

} 

Bool_t EventIDMaker::IsGold(const UInt_t& Run,const UInt_t& LuminosityBlock) {
  for (auto LumiRange: GoldenJson[Run]) {
    if (LuminosityBlock >= LumiRange.first && LuminosityBlock <= LumiRange.second) return true;
  }
  return false;
}

Long64_t EventIDMaker::GetEventIndex(const UInt_t& run,const ULong64_t& event) const{
  if ( run > 33e4 or event > 8e9) {
    hlog->FillS("EventIDOutOfRange");
    std::cerr << Form("EventIDMaker::GetEventIndex() Unexpected range for run[%d] or event[%llu]\n",
                      run,event);
  }
  Long64_t uid = std::stoll(std::to_string(run)+std::to_string(event));
  assert(uid<numeric_limits<Long64_t>::max());
  return uid;
}

Bool_t EventIDMaker::Process(Long64_t entry) {

  hlog->FillS("Total");

  if(!ReadEntry(entry)){
    hlog->FillS("MissingMultipleBranches");
    return kFALSE;
  }

  if(IsMissingBranch) hlog->FillS("MissingBranch");

  if (!MinLeptonsTest()){
    hlog->FillS("FailMinLeptonsTest");
    return kFALSE;
  }

  if (!IsGold(*run,*luminosityBlock)){
    hlog->FillS("FailGoldenJsonTest");
    return kFALSE;
  }

  if (!ElectronTest()){
    hlog->FillS("FailElectronTest");
  }

  if (!MuonTest()){
    hlog->FillS("FailMuonTest");
  }

   // Event Selection
  if ( (ElectronTest() || MuonTest()) ) {
    hlog->FillS("Passed");
    if (IsMissingBranch) hlog->FillS("MissingBranch&Passed");
    EventID = GetEventIndex(*run,*event);
    eTree->Fill();
    EntryList->Enter(entry);
  }
  return kTRUE;
}



void EventIDMaker::Terminate() {


  assert(OutputLabel.size()>0);

  std::string dirName = Form("%s_%d",SampleName.Data(),Year);
  std::unique_ptr<TFile> fEventIDTree(TFile::Open(Form("EventIDTree_%s.root",OutputLabel.c_str()),"UPDATE"));
  fEventIDTree->mkdir(dirName.c_str());
  fEventIDTree->cd(dirName.c_str());
  eTree = dynamic_cast<TTree*>(fOutput->FindObject("eTree"));
  eTree->Write("eTree");
  EntryList = dynamic_cast<TEntryList*>(fOutput->FindObject("EntryList"));
  EntryList->Write("EntryList");
  hlog->SetName(Form("hlog_%s_%d",SampleName.Data(),Year));
  hlog->Write();
  fEventIDTree->Close();

}
