#include "TProof.h"
#include <array>
#include <string>
#include <fstream>

Int_t MakeEventIDTree(std::string file = "", Int_t fWorkers = 2, Int_t year = 2016){

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

  fProof->SetProgressDialog(false);
  fProof->SetParameter("SampleName",sample.c_str());
  fProof->SetParameter("Year", year);

  fChain->SetProof();
  fChain->Process("EventIDMaker.C+");
  fProof->Print("a");

  return 0;
}
