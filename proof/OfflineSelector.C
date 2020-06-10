#include "TProof.h"
#include <array>
#include <string>
#include <fstream>

Int_t OfflineSelector(TString rootfile = "", Int_t fWorkers = 4, Int_t year = 2016, TString SFFile = "EfficienciesAndSF_Period4.root"){

  TChain* fChain = new TChain("Events");

  std::cout << Form("Chaining %s\n",rootfile.Data()) ;
  fChain->AddFile(rootfile.Data());

  TProof *fProof = TProof::Open(Form("workers=%d",fWorkers));
  fProof->SetProgressDialog(false);
  fProof->SetParameter("Year",year);
  fProof->SetParameter("SampleName",rootfile.Data());

  TFile *f = TFile::Open(SFFile.Data(),"READ");
  auto SFTrigger = (TH2F*)f->Get("IsoMu24_OR_IsoTkMu24_PtEtaBins/efficienciesMC/abseta_pt_MC");
  SFTrigger->SetName("SFTrigger");
  fProof->AddInputData(SFTrigger);

  fChain->SetProof();
  fChain->Process("PreSelector.C+");
  fProof->Print("a");

  return 0;
}
