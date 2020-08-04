#include "TProof.h"
#include <array>
#include <string>
#include <fstream>


Int_t OfflineSelector(TString rootfile = "", Int_t fWorkers = 4, Int_t year = 2016){

  TChain* fChain = new TChain("Events");

  std::cout << Form("Chaining %s\n",rootfile.Data()) ;
  fChain->AddFile(rootfile.Data());

  TProof *fProof = TProof::Open(Form("workers=%d",fWorkers));
  fProof->SetProgressDialog(false);
  fProof->SetParameter("Year",year);
  fProof->SetParameter("SampleName",rootfile.Data());

  TFile *f1 = TFile::Open("EfficienciesAndSF_RunBtoF.root","READ");
  auto SFMuonTriggerBF = (TH2F*)f1->Get("IsoMu24_OR_IsoTkMu24_PtEtaBins/efficienciesMC/abseta_pt_MC");
  SFMuonTriggerBF->SetName("SFMuonTriggerBF");

  TList *SFDb = new TList();
  SFDb->SetName("SFDb");
  SFDb->Add(SFMuonTriggerBF);

  TFile *f2 = TFile::Open("EfficienciesAndSF_Period4.root","READ");
  auto SFMuonTriggerGH = (TH2F*)f2->Get("IsoMu24_OR_IsoTkMu24_PtEtaBins/efficienciesMC/abseta_pt_MC");
  SFMuonTriggerGH->SetName("SFMuonTriggerGH");
  SFDb->Add(SFMuonTriggerGH);

  TFile *f3 = TFile::Open("EfficienciesStudies_2016_legacy_rereco_rootfiles_RunBCDEF_SF_ID.root","READ");
  auto SFMuonIDBF = (TH2D*)f3->Get("NUM_TightID_DEN_genTracks_eta_pt");
  SFMuonIDBF->SetName("SFMuonIDBF");
  SFDb->Add(SFMuonIDBF);

  TFile *f4 = TFile::Open("EfficienciesStudies_2016_legacy_rereco_rootfiles_RunGH_SF_ID.root","READ");
  auto SFMuonIDGH = (TH2D*)f4->Get("NUM_TightID_DEN_genTracks_eta_pt");
  SFMuonIDGH->SetName("SFMuonIDGH");
  SFDb->Add(SFMuonIDGH);

  TFile *f5 = TFile::Open("ElectronTriggerScaleFactors_eta_ele_binned_official_pt30to175_withsyst.root","READ");
  auto SFElectronTrigger1 = static_cast<TGraphAsymmErrors*>(f5->Get("ScaleFactors"));
  SFElectronTrigger1->SetName("SFElectronTrigger1");
  SFDb->Add(SFElectronTrigger1);

  TFile *f6 = TFile::Open("ElectronTriggerScaleFactors_eta_ele_binned_official_pt175toInf.root","READ");
  auto SFElectronTrigger2 = static_cast<TGraphAsymmErrors*>(f6->Get("ScaleFactors"));
  SFElectronTrigger2->SetName("SFElectronTrigger2");
  SFDb->Add(SFElectronTrigger2);

  TFile *f7 = TFile::Open("PileupWeights.root","READ");
  auto SFPileup = static_cast<TList*>(f7->Get("PileupSFList"));
  SFDb->Add(SFPileup);

  fProof->AddInputData(SFDb);

  fChain->SetProof();
  fChain->Process("PreSelector.C+g");
  fProof->Print("a");

  return 0;
}
