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

  TList *SFDb = new TList();
  SFDb->SetName("SFDb");
  SFDb->Add(SFTriggerBF);

  TFile *f2 = TFile::Open(SFFileGH.Data(),"READ");
  auto SFTriggerGH = (TH2F*)f2->Get("IsoMu24_OR_IsoTkMu24_PtEtaBins/efficienciesMC/abseta_pt_MC");
  SFTriggerGH->SetName("SFTriggerGH");
  SFDb->Add(SFTriggerGH);

  TFile *f3 = TFile::Open("EfficienciesStudies_2016_legacy_rereco_rootfiles_RunBCDEF_SF_ID.root","READ");
  auto SFMuonIDBF = (TH2D*)f3->Get("NUM_TightID_DEN_genTracks_eta_pt");
  SFMuonIDBF->SetName("SFMuonIDBF");
  SFDb->Add(SFMuonIDBF);

  TFile *f4 = TFile::Open("EfficienciesStudies_2016_legacy_rereco_rootfiles_RunGH_SF_ID.root","READ");
  auto SFMuonIDGH = (TH2D*)f4->Get("NUM_TightID_DEN_genTracks_eta_pt");
  SFMuonIDGH->SetName("SFMuonIDGH");
  SFDb->Add(SFMuonIDGH);

  fProof->AddInputData(SFDb);

  fChain->SetProof();
  fChain->Process("PreSelector.C+");
  fProof->Print("a");

  return 0;
}
