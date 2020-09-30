#include "TProof.h"
#include <array>
#include <string>
#include <fstream>
#include "../IsData.h"

Int_t MakeUniqueEntryList(std::string file = "", Int_t fWorkers = 2){

  Int_t year;

#ifdef Y2016
  year = 2016;
#elif defined(Y2017)
  year = 2017;
#elif defined(Y2018)
  year = 2018;
#endif

  TChain* fChain = new TChain("Events");

  istringstream f(file);
  std::string sample;

  std::ifstream infile(file);
  std::string line;
  while(std::getline(infile, line)){
    line = Form("root://cmsxrootd.fnal.gov/%s",line.c_str());
    std::cout << "Chaining " << line << std::endl;
    fChain->AddFile(line.c_str());
  }
  sample = file.substr(file.rfind("/")+1);
  sample.resize(sample.size()-4);

  TProof *fProof = TProof::Open(Form("workers=%d",fWorkers));

  // Send TTrees to build EventIndex
  // Order taken 2016/2017 SingleElectron -> SingleMuon -> SinglePhoton
  // Takes sizeof(tree) memory PER worker
  // As it is: Works for SingleMuon and Commented: SinglePhoton
  TFile *f1 = TFile::Open("EventIDTree.root","READ");
  TFile *f2 = TFile::Open("EntryLists.root","READ");
  TTree *EventTree = (TTree*)f1->Get(Form("SinglePhoton_%d/eTree;1",year));
  //TTree *EventTree2 = (TTree*)f1->Get(Form("SingleElectron_%d/eTree;1",year));
  TEntryList *l2 = (TEntryList*)f2->Get(Form("SingleElectron_%d/EntryList;1",year));
  //TEntryList *l3 = (TEntryList*)f2->Get(Form("SingleMuon_%d/EntryList;1",year));
  EventTree->SetName("EventIndexTree1");
  //EventTree2->SetName("EventIndexTree2");
  fProof->AddInput(EventTree);
  //fProof->AddInput(EventTree2);

  // Orden taken 2018  SingleMuon -> EGamma
  // TTree *EventTree = (TTree*)f1->Get(Form("SingleMuon_%d/eTree;1",year));
  // TEntryList *l2 = (TEntryList*)f2->Get(Form("EGamma_%d/EntryList;1",year));
  // EventTree->SetName("EventIndexTree1");
  // fProof->AddInput(EventTree);

  fProof->SetProgressDialog(false);
  fProof->SetParameter("SampleName",sample.c_str());
  fProof->SetParameter("Year", year);

  fChain->SetEntryList(l2);
  //fChain->SetEntryList(l3);
  fChain->SetProof();
  fChain->Process("UniqueEntryListMaker.C+g");
  fProof->Print("a");

  return 0;
}
