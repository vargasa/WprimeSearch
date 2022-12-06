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

#ifndef ULSAMPLE
  const char *ElectronSample = "SingleElectron";
  const char *MuonSample = "SingleMuon";
  const char *PhotonSample = "SinglePhoton";
  const char *EGammaSample = "EGamma";
#elif defined(ULSAMPLE)
  const char *ElectronSample = "ULSingleElectron";
  const char *MuonSample = "ULSingleMuon";
  const char *PhotonSample = "ULSinglePhoton";
  const char *EGammaSample = "ULEGamma";
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
  TFile *f1 = TFile::Open("root://cmseos.fnal.gov///store/user/avargash/WprimeSearchCondorOutput/EntryListMaker/EventIDTree.root","READ");
  TFile *f2 = TFile::Open("root://cmseos.fnal.gov///store/user/avargash/WprimeSearchCondorOutput/EntryListMaker/EntryLists.root","READ");

  TTree *EventTree = nullptr;
  TTree *EventTree2 = nullptr;

  TEntryList *l2 = nullptr;
  TEntryList *l3 = nullptr;

  if ( (year == 2016) or (year == 2017) ) {
    EventTree = (TTree*)f1->Get(Form("%s_%d/eTree;1",PhotonSample,year));
    if (!sample.compare(ElectronSample)){
      l2 = (TEntryList*)f2->Get(Form("%s_%d/EntryList;1",ElectronSample,year));
      std::clog << Form("Setting EntryList: %s %s\n",ElectronSample,l2->GetName());
      fChain->SetEntryList(l2);
    } else if (!sample.compare(MuonSample)) {
      EventTree2 = (TTree*)f1->Get(Form("%s_%d/eTree;1",ElectronSample,year));
      l3 = (TEntryList*)f2->Get(Form("%s_%d/EntryList;1",MuonSample,year));
      std::clog << Form("Setting EntryList: %s %s\n",MuonSample,l3->GetName());
      fChain->SetEntryList(l3);
    }
  } else if ( year == 2018 and (!sample.compare(EGammaSample)) ) {
    EventTree = (TTree*)f1->Get(Form("%s_%d/eTree;1",MuonSample,year));
    l2 = (TEntryList*)f2->Get(Form("%s_%d/EntryList;1",EGammaSample,year));
  }

  EventTree->SetName("EventIndexTree1");
  std::clog << Form("Adding EventIDTree: %s\n",EventTree->GetName());
  fProof->AddInput(EventTree);

  if (EventTree2) {
    std::clog << Form("Adding EventIDTree: %s\n",EventTree2->GetName());
    EventTree2->SetName("EventIndexTree2");
    fProof->AddInput(EventTree2);
  }

  fProof->SetProgressDialog(false);
  fProof->SetParameter("SampleName",sample.c_str());
  fProof->SetParameter("Year", year);


  fChain->SetProof();
  fChain->Process("UniqueEntryListMaker.C+g");
  fProof->Print("a");

  return 0;
}
