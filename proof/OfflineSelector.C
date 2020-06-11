#include "TProof.h"
#include <array>
#include <string>
#include <fstream>


Int_t OfflineSelector(TString rootfile = "", Int_t fWorkers = 4, Int_t year = 2016,
                      TString SFFileBF = "EfficienciesAndSF_RunBtoF.root",
                      TString SFFileGH = "EfficienciesAndSF_Period4.root"){

  TChain* fChain = new TChain("Events");

  std::cout << Form("Chaining %s\n",rootfile.Data()) ;
  fChain->AddFile(rootfile.Data());

  TProof *fProof = TProof::Open(Form("workers=%d",fWorkers));
  fProof->SetProgressDialog(false);
  fProof->SetParameter("Year",year);
  fProof->SetParameter("SampleName",rootfile.Data());

  TFile *f1 = TFile::Open(SFFileBF.Data(),"READ");
  auto SFTriggerBF = (TH2F*)f1->Get("IsoMu24_OR_IsoTkMu24_PtEtaBins/efficienciesMC/abseta_pt_MC");
  SFTriggerBF->SetName("SFTriggerBF");
  fProof->AddInputData(SFTriggerBF);

  TFile *f2 = TFile::Open(SFFileGH.Data(),"READ");
  auto SFTriggerGH = (TH2F*)f2->Get("IsoMu24_OR_IsoTkMu24_PtEtaBins/efficienciesMC/abseta_pt_MC");
  SFTriggerGH->SetName("SFTriggerGH");
  fProof->AddInputData(SFTriggerGH);

  fChain->SetProof();
  fChain->Process("PreSelector.C+");
  fProof->Print("a");

  return 0;
}
