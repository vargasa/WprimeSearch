#include "TProof.h"
#include <array>
#include <string>
#include <fstream>

Int_t Selector(std::string files = "", Int_t fWorkers = 4, std::string elistfile = ""){

  TChain* fChain = new TChain("Events");

  istringstream f(files);
  std::string file;
  std::string sample;

  while(std::getline(f,file,'+')){
    std::ifstream infile(file);
    std::string line;
    while(std::getline(infile, line)){
      line = Form("root://cmsxrootd.fnal.gov/%s",line.c_str());
      std::cout << "Chaining " << line << std::endl;
      fChain->AddFile(line.c_str());
    }
    sample += file.substr(file.rfind("/")+1);
    sample.resize(sample.size()-4);
  }

  TEntryList *EList;
  TFile *FileEList;

  if(!elistfile.empty()){
    FileEList = TFile::Open(elistfile.c_str(),"READ");
    EList = (TEntryList*)FileEList->Get(Form("%s/EntryList",sample.c_str()));
    std::cout << Form("Info: Using EntryList from %s\n",elistfile.c_str());
    fChain->SetEntryList(EList);
  }

  TProof *fProof = TProof::Open(Form("workers=%d",fWorkers));
  fProof->SetProgressDialog(false);
  fProof->SetParameter("SampleName",sample.c_str());

  fChain->SetProof();
  fChain->Process("PreSelector.C+");
  fProof->Print("a");

  return 0;
}
