#include "TProof.h"
#include <array>
#include <string>
#include <fstream>

Int_t Selector(TString rootlist = "", Int_t fWorkers = 4){

  std::ifstream infile(rootlist);

  TChain* fChain = new TChain("Events");

  std::string line;
  while(std::getline(infile, line)){
    line = Form("root://cmsxrootd.fnal.gov/%s",line.c_str());
    std::cout << "Chaining " << line << std::endl;
    fChain->AddFile(line.c_str());
  }

  TProof *fProof = TProof::Open(Form("workers=%d",fWorkers));
  fProof->SetProgressDialog(false);
  fProof->SetParameter("Mass",(Int_t)mass);

  fChain->SetProof();
  fChain->Process("PreSelector.C+");
  fProof->Print("a");

  return 0;
}
