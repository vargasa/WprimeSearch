#include "TProof.h"

Int_t Selector(TString rootfile = "", Int_t fWorkers = 4){

  TChain* fChain = new TChain("Events");
  fChain->AddFile("root://cmsxrootd.fnal.gov///store/mc/RunIIAutumn18NanoAODv6/DYJetsToLL_M-5to50_TuneCP5_13TeV-madgraphMLM-pythia8/NANOAODSIM/Nano25Oct2019_102X_upgrade2018_realistic_v20-v1/260000/BD9CDF80-1923-024B-A5ED-6DE4878A1777.root");
  fChain->AddFile("root://cmsxrootd.fnal.gov///store/mc/RunIIAutumn18NanoAODv6/DYJetsToLL_M-5to50_TuneCP5_13TeV-madgraphMLM-pythia8/NANOAODSIM/Nano25Oct2019_102X_upgrade2018_realistic_v20-v1/260000/B7A5D469-4393-F942-8AF5-39EDB8A0E77A.root");
  fChain->AddFile("root://cmsxrootd.fnal.gov///store/mc/RunIIAutumn18NanoAODv6/DYJetsToLL_M-5to50_TuneCP5_13TeV-madgraphMLM-pythia8/NANOAODSIM/Nano25Oct2019_102X_upgrade2018_realistic_v20-v1/260000/B2B54714-B3AE-1B4B-B5DB-C701FCA6F833.root");


  TProof *fProof = TProof::Open(Form("workers=%d",fWorkers));
  fProof->SetProgressDialog(false);

  fChain->SetProof();
  fChain->Process("DYPreSelector.C+");

  return 0;
}
