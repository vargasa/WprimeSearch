#include "TProof.h"
#include "IsData.h"
#include <array>
#include <string>
#include <fstream>

Int_t Selector(std::string files = "", Int_t fWorkers = 4, std::string elistfile = "", Int_t year = 2016){

  TChain* fChain = new TChain("Events");

  istringstream f(files);
  std::string file;
  std::string sample;

  while(std::getline(f,file,'+')){
    std::ifstream infile(file);
    std::string line;
    while(std::getline(infile, line)){
      line = Form("root://cmsxrootd.fnal.gov/%s",line.c_str());
      std::cout << "Chaining " << line << std::endl;
      fChain->AddFile(line.c_str());
    }
    sample += file.substr(file.rfind("/")+1);
    sample.resize(sample.size()-4);
  }

  TEntryList *EList;
  TFile *FileEList;
  TProof *fProof = TProof::Open(Form("workers=%d",fWorkers));

  if(!elistfile.empty()){
    fProof->SetParameter("EntryListSet",1);
    fProof->SetParameter("MakeEventIDTree",1);
    FileEList = TFile::Open(elistfile.c_str(),"READ");
    EList = (TEntryList*)FileEList->Get(Form("%s/EntryList",sample.c_str()));
    std::cout << Form("Info: Using EntryList from %s\n",elistfile.c_str());
    fChain->SetEntryList(EList);
  }

  fProof->SetProgressDialog(false);
  fProof->SetParameter("Year",year);
  fProof->SetParameter("SampleName",sample.c_str());

#ifndef CMSDATA
  TFile *f1 = TFile::Open("EfficienciesAndSF_RunBtoF.root","READ");
  auto SFMuonTriggerBF = static_cast<TH2F*>(f1->Get("Mu50_OR_TkMu50_PtEtaBins/efficienciesMC/abseta_pt_MC"));
  SFMuonTriggerBF->SetName("SFMuonTriggerBF");
  TList *SFDb = new TList();
  SFDb->SetName("SFDb");
  SFDb->Add(SFMuonTriggerBF);
  TFile *f2 = TFile::Open("EfficienciesAndSF_Period4.root","READ");
  auto SFMuonTriggerGH = static_cast<TH2F*>(f2->Get("Mu50_OR_TkMu50_PtEtaBins/efficienciesMC/abseta_pt_MC"));
  SFMuonTriggerGH->SetName("SFMuonTriggerGH");
  SFDb->Add(SFMuonTriggerGH);
  fProof->AddInputData(SFDb);
  TFile *f3 = TFile::Open("EfficienciesStudies_2016_legacy_rereco_rootfiles_RunBCDEF_SF_ID.root","READ");
  auto SFMuonIDBF = static_cast<TH2D*>(f3->Get("NUM_TightID_DEN_genTracks_eta_pt"));
  SFMuonIDBF->SetName("SFMuonIDBF");
  SFDb->Add(SFMuonIDBF);
  TFile *f4 = TFile::Open("EfficienciesStudies_2016_legacy_rereco_rootfiles_RunGH_SF_ID.root","READ");
  auto SFMuonIDGH = static_cast<TH2D*>(f4->Get("NUM_TightID_DEN_genTracks_eta_pt"));
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
#endif

  fChain->SetProof();
  fChain->Process("PreSelector.C+");
  fProof->Print("a");

  return 0;
}
