#include "TProof.h"
#include <array>
#include <string>
#include <fstream>

Int_t EntryList(std::string files = "", Int_t fWorkers = 4){

  TChain* fChain = new TChain("Events");

  istringstream f(files);
  std::string file;

  while(std::getline(f,file,'+')){
    std::ifstream infile(file);
    std::string line;
    while(std::getline(infile, line)){
      line = Form("root://cmsxrootd.fnal.gov/%s",line.c_str());
      std::cout << "Chaining " << line << std::endl;
      fChain->AddFile(line.c_str());
    }
  }

  TProof *fProof = TProof::Open(Form("workers=%d",fWorkers));
  fProof->SetProgressDialog(false);

  fChain->SetProof();
  fChain->Process("EntryListMaker.C+");

  return 0;
}
