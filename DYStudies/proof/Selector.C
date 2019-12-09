#include "TProof.h"
#include <array>
#include <string>

Int_t Selector(TString rootfile = "", Int_t fWorkers = 4){

  std::array<TString, 8> files = {
    "/store/mc/RunIIAutumn18NanoAODv6/DYJetsToLL_M-5to50_TuneCP5_13TeV-madgraphMLM-pythia8/NANOAODSIM/Nano25Oct2019_102X_upgrade2018_realistic_v20-v1/260000/B2B54714-B3AE-1B4B-B5DB-C701FCA6F833.root",
    "/store/mc/RunIIAutumn18NanoAODv6/DYJetsToLL_M-5to50_TuneCP5_13TeV-madgraphMLM-pythia8/NANOAODSIM/Nano25Oct2019_102X_upgrade2018_realistic_v20-v1/260000/BD9CDF80-1923-024B-A5ED-6DE4878A1777.root",
    "/store/mc/RunIIAutumn18NanoAODv6/DYJetsToLL_M-5to50_TuneCP5_13TeV-madgraphMLM-pythia8/NANOAODSIM/Nano25Oct2019_102X_upgrade2018_realistic_v20-v1/260000/3F973F73-EE2C-E74C-ABA1-99E7014D1BC1.root",
    "/store/mc/RunIIAutumn18NanoAODv6/DYJetsToLL_M-5to50_TuneCP5_13TeV-madgraphMLM-pythia8/NANOAODSIM/Nano25Oct2019_102X_upgrade2018_realistic_v20-v1/260000/B7A5D469-4393-F942-8AF5-39EDB8A0E77A.root",
    "/store/mc/RunIIAutumn18NanoAODv6/DYJetsToLL_M-5to50_TuneCP5_13TeV-madgraphMLM-pythia8/NANOAODSIM/Nano25Oct2019_102X_upgrade2018_realistic_v20-v1/260000/51332710-0D2B-9F4A-9D76-722E8BFCCB16.root",
    "/store/mc/RunIIAutumn18NanoAODv6/DYJetsToLL_M-5to50_TuneCP5_13TeV-madgraphMLM-pythia8/NANOAODSIM/Nano25Oct2019_102X_upgrade2018_realistic_v20-v1/250000/6C1EF991-95F4-9D4E-A62D-3D7678C213F6.root",
    "/store/mc/RunIIAutumn18NanoAODv6/DYJetsToLL_M-5to50_TuneCP5_13TeV-madgraphMLM-pythia8/NANOAODSIM/Nano25Oct2019_102X_upgrade2018_realistic_v20-v1/250000/145E9857-E318-B84B-859D-6EF652C40BEE.root",
    "/store/mc/RunIIAutumn18NanoAODv6/DYJetsToLL_M-5to50_TuneCP5_13TeV-madgraphMLM-pythia8/NANOAODSIM/Nano25Oct2019_102X_upgrade2018_realistic_v20-v1/250000/400A50F8-386A-C64C-A5C2-A6731A3F6F45.root"
  };

  TChain* fChain = new TChain("Events");

  for (auto& filename : files){
    fChain->AddFile("root://cmsxrootd.fnal.gov//"+ filename);
  }

  TProof *fProof = TProof::Open(Form("workers=%d",fWorkers));
  fProof->SetProgressDialog(false);

  fChain->SetProof();
  fChain->Process("DYPreSelector.C+");

  return 0;
}
