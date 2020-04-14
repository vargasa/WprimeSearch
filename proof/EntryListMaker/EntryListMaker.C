#include "EntryListMaker.h"
#include "TFile.h"

EntryListMaker::EntryListMaker(TTree *){
  eTree = 0;
}

void EntryListMaker::Init(TTree *tree){
  fReader.SetTree(tree);
}

void EntryListMaker::Begin(TTree *tree){

}

void EntryListMaker::SlaveBegin(TTree *tree){

  eTree = new TTree("eTree","eTree");
  eTree->Branch("eTreeName",&eTreeName);
  eTree->Branch("eRun",&eRun);
  eTree->Branch("eEvent",&eEvent);
  eTree->Branch("eLumi",&eLumi);

  fOutput->Add(eTree);

}

Bool_t EntryListMaker::Process(Long64_t entry){

  fReader.SetEntry(entry);

  eRun = *run;
  eEvent = *event;
  eLumi = *luminosityBlock;
  TTree *thisTree = fReader.GetTree();
  eTreeName = TString::Format("%s/%s",thisTree->GetDirectory()->GetName(),thisTree->GetName()).Data();

  eTree->Fill();

  return kTRUE;
}

void EntryListMaker::Terminate(){

  TFile *f1 = TFile::Open("eListRunEventNumber.root","RECREATE");
  eTree->Write();
  f1->Close();
}
