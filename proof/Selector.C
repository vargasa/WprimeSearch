#include "TProof.h"
#include <array>
#include <string>

Int_t Selector(TString rootfile = "", Int_t fWorkers = 4){

  TString filename = "/store/mc/RunIISummer16NanoAODv6/WprimeToWZToWlepZlep_narrow_M-1400_13TeV-madgraph/NANOAODSIM/PUMoriond17_Nano25Oct2019_102X_mcRun2_asymptotic_v7-v1/270000/825800D8-EB97-004F-AC28-39739C59C975.root";




  TChain* fChain = new TChain("Events");

  fChain->AddFile("root://cmsxrootd.fnal.gov//"+ filename);

  TProof *fProof = TProof::Open(Form("workers=%d",fWorkers));
  fProof->SetProgressDialog(false);
  fProof->SetParameter("Mass",(Int_t)mass);

  fChain->SetProof();
  fChain->Process("PreSelector.C+");
  fProof->Print("a");

  return 0;
}
