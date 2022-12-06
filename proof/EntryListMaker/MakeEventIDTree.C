#include "TProof.h"
#include <array>
#include <string>
#include <fstream>
#include "../IsData.h"

Int_t MakeEventIDTree(std::string file = "", Int_t fWorkers = 2){

  int year;

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
    if(    line.find("1FC6B7E7-1051-5947-8044-B6EAE3CB0721") != std::string::npos
        or line.find("B3137030-E3B2-9E4A-A365-87447A16A207") != std::string::npos
        or line.find("0C2EC37F-D444-D04E-82FF-4F2B0766E9B8") != std::string::npos ) {
        line = Form("root://cms-xrd-global.cern.ch//%s",line.c_str());
    } else {
      line = Form("root://cmsxrootd.fnal.gov/%s",line.c_str());
    }
    std::cout << "Chaining " << line << std::endl;
    fChain->AddFile(line.c_str());
  }
  sample = file.substr(file.rfind("/")+1);
  sample.resize(sample.size()-4);

  gEnv->SetValue("Proof.Sandbox","/tmp/");
  gEnv->SetValue("ProofLite.Sandbox","/tmp/proof/");
  gEnv->SetValue("Proof.CacheDir","/tmp/proof/");
  gEnv->SetValue("Proof.DataSetDir","/tmp/proof/");
  gEnv->SetValue("Proof.PackageDir","/tmp/");
  gEnv->SetValue("ProofLite.SubPath","last-session");

  TProof *fProof = TProof::Open(Form("workers=%d",fWorkers));

  fProof->SetProgressDialog(false);
  fProof->SetParameter("SampleName",sample.c_str());
  fProof->SetParameter("Year", year);

  fChain->SetProof();
  fChain->Process("EventIDMaker.C+g");
  fProof->Print("a");

  return 0;
}
