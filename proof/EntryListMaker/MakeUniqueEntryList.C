#include "TProof.h"
#include <array>
#include <string>
#include <fstream>

Int_t MakeUniqueEntryList(std::string file = "", Int_t fWorkers = 2){

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
  // Order taken DoubleEG -> SingleElectron -> SingleMuon
  // Takes sizeof(tree) memory PER worker
  TFile *f1 = TFile::Open("EventIDTree.root","READ");
  TTree *EventTree = (TTree*)f1->Get("DoubleEG/eTree;1");
  //TTree *EventTree2 = (TTree*)f1->Get("SingleElectron/eTree;1");
  EventTree->SetName("EventIndexTree1");
  //EventTree2->SetName("EventIndexTree2");
  fProof->AddInput(EventTree);
  //fProof->AddInput(EventTree2);


  fProof->SetProgressDialog(false);
  fProof->SetParameter("SampleName",sample.c_str());

  fChain->SetProof();
  fChain->Process("UniqueEntryListMaker.C+");
  fProof->Print("a");

  return 0;
}
