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

  const char *ElectronSample = "ULSingleElectron";
  const char *MuonSample = "ULSingleMuon";
  const char *PhotonSample = "ULSinglePhoton";
  const char *EGammaSample = "ULEGamma";

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

  gEnv->SetValue("Proof.Sandbox","/tmp/");
  gEnv->SetValue("ProofLite.Sandbox","/tmp/proof/");
  gEnv->SetValue("Proof.CacheDir","/tmp/proof/");
  gEnv->SetValue("Proof.DataSetDir","/tmp/proof/");
  gEnv->SetValue("Proof.PackageDir","/tmp/");
  gEnv->SetValue("ProofLite.SubPath","last-session");

  TProof *fProof = TProof::Open(Form("workers=%d",fWorkers));

  std::string EventIDTreePath = "root://cmseos.fnal.gov///store/user/avargash/WprimeSearchCondorOutput/EntryListMaker/EventIDTree.root";
  
  TFile *f2 = TFile::Open("root://cmseos.fnal.gov///store/user/avargash/WprimeSearchCondorOutput/EntryListMaker/EntryLists.root","READ");

  TEntryList *l2 = nullptr;
  TEntryList *l3 = nullptr;

  if ( (year == 2016) or (year == 2017) ) {
    if (sample.compare(ElectronSample) == 0){
      l2 = (TEntryList*)f2->Get(Form("%s_%d/EntryList;1",ElectronSample,year));
      std::clog << Form("Setting EntryList: %s %s\n",ElectronSample,l2->GetName());
      fChain->SetEntryList(l2);
    } else if (sample.compare(MuonSample) == 0) {
      l3 = (TEntryList*)f2->Get(Form("%s_%d/EntryList;1",MuonSample,year));
      std::clog << Form("Setting EntryList: %s %s\n",MuonSample,l3->GetName());
      fChain->SetEntryList(l3);
    }
  } else if ( year == 2018 and (!sample.compare(EGammaSample)) ) {
    l2 = (TEntryList*)f2->Get(Form("%s_%d/EntryList;1",EGammaSample,year));
    fChain->SetEntryList(l2);
  }

  fProof->SetProgressDialog(false);
  fProof->SetParameter("SampleName",sample.c_str());
  fProof->SetParameter("Year", year);
  fProof->SetParameter("EventIDTreePath",EventIDTreePath.c_str());

  fChain->SetProof();
  fChain->Process("UniqueEntryListMaker.C+g");
  fProof->Print("a");

  return 0;
}
