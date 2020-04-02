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

  fChain->SetProof();
  fChain->Process("PreSelector.C+");
  fProof->Print("a");

  return 0;
}
