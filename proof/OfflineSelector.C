#include "TProof.h"
#include <array>
#include <string>
#include <fstream>

Int_t OfflineSelector(TString rootfile = "", Int_t fWorkers = 4){

  TChain* fChain = new TChain("Events");

  std::cout << Form("Chaining %s\n",rootfile.Data()) ;
  fChain->AddFile(rootfile.Data());

  TProof *fProof = TProof::Open(Form("workers=%d",fWorkers));
  fProof->SetProgressDialog(false);
  fProof->SetParameter("SampleName",rootfile.Data());

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
