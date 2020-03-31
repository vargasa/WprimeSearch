#include "TProof.h"
#include <array>
#include <string>
#include <fstream>

Int_t Selector(std::string rootlist = "", Int_t fWorkers = 4){

  std::ifstream infile(rootlist);

  TChain* fChain = new TChain("Events");

  std::string line;
  while(std::getline(infile, line)){
    line = Form("root://cmsxrootd.fnal.gov/%s",line.c_str());
    std::cout << "Chaining " << line << std::endl;
    fChain->AddFile(line.c_str());
  }
  std::string sample = rootlist.substr(rootlist.rfind("/")+1);
  sample.resize(sample.size()-4);

  TProof *fProof = TProof::Open(Form("workers=%d",fWorkers));
  fProof->SetProgressDialog(false);
  fProof->SetParameter("SampleName",sample.c_str());

  const char *fGoldenTreeName = "goldenTree";
  TFile *GoldenJson = TFile::Open("GoldenJson.root","READ");
  TTree *GoldenTree = (TTree*)GoldenJson->Get(fGoldenTreeName);
  fProof->SetParameter("GoldenTreeName",fGoldenTreeName);
  fProof->AddInputData(GoldenTree);

  fChain->SetProof();
  fChain->Process("PreSelector.C+");
  fProof->Print("a");

  return 0;
}
