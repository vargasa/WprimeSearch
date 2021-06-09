#include "TProof.h"
#include "IsData.h"
#include <array>
#include <string>
#include <fstream>


Int_t Selector(std::string files = "", Int_t fWorkers = 4, std::string elistfile = ""){

  int Year;
#ifdef Y2016
  Year = 2016;
#elif defined(Y2017)
  Year = 2017;
#elif defined(Y2018)
  Year = 2018;
#endif

  TChain* fChain = new TChain("Events");

  istringstream f(files);
  std::string file;
  std::string sample;

  while(std::getline(f,file,'+')){
    std::ifstream infile(file);
    std::string line;
    while(std::getline(infile, line)){
      if(line.empty() or line.find("#") == 0) continue;
      line = Form("root://cmsxrootd.fnal.gov/%s",line.c_str());
      //line = Form("root://cms-xrd-global.cern.ch/%s",line.c_str());
      std::cout << "Chaining " << line << std::endl;
      fChain->AddFile(line.c_str());
    }
    sample += file.substr(file.rfind("/")+1);
    sample.resize(sample.size()-4);
  }

  TEntryList *EList;
  TFile *FileEList;
  TProof *fProof = TProof::Open(Form("workers=%d",fWorkers));

  if(!elistfile.empty()){
    fProof->SetParameter("EntryListSet",1);
    fProof->SetParameter("MakeEventIDTree",1);
#ifdef CMSDATA
    FileEList = TFile::Open(elistfile.c_str(),"READ");
    EList = (TEntryList*)FileEList->Get(Form("%s_%d/EntryList",sample.c_str(),Year));
#endif
    if(EList){
      std::clog << Form("Info: Using EntryList %s_%d from %s\n",sample.c_str(),Year,elistfile.c_str());
      fChain->SetEntryList(EList);
    } else {
      std::clog << "EntryList empty, looping through all events\n";
    }
  }

  fProof->SetProgressDialog(false);
  fProof->SetParameter("SampleName",sample.c_str());

  fChain->SetProof();
  fChain->Process("PreSelector.C+g");
  fProof->Print("a");

  return 0;
}
