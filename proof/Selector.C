#include "TProof.h"
#include "IsData.h"
#include <array>
#include <string>
#include <fstream>

Int_t Selector(std::string files = "", Int_t fWorkers = 4, std::string elistfile = ""){

  int Year;
#ifdef Y2016
  Year = 2016;
#elif defined(Y2017)
  Year = 2017;
#elif defined(Y2018)
  Year = 2018;
#endif

  TChain* fChain = new TChain("Events");

  istringstream f(files);
  std::string file;
  std::string sample;

  while(std::getline(f,file,'+')){
    std::ifstream infile(file);
    std::string line;
    while(std::getline(infile, line)){
      if(line.find("#") == 0) continue;
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
#ifdef CMSDATA
    FileEList = TFile::Open(elistfile.c_str(),"READ");
    EList = (TEntryList*)FileEList->Get(Form("%s_%d/EntryList",sample.c_str(),Year));
#endif
    if(EList){
      std::clog << Form("Info: Using EntryList %s_%d from %s\n",sample.c_str(),Year,elistfile.c_str());
      fChain->SetEntryList(EList);
    } else {
      std::clog << "EntryList empty, looping through all events\n";
    }
  }

  fProof->SetProgressDialog(false);
  fProof->SetParameter("SampleName",sample.c_str());



#ifndef CMSDATA

  TList *SFDb = new TList();

#ifdef Y2016
  TFile *f1 = TFile::Open("files/mc/2016/sf/EfficienciesAndSF_RunBtoF.root","READ");
  auto SFMuonTriggerBF = static_cast<TH2F*>(f1->Get("Mu50_OR_TkMu50_PtEtaBins/abseta_pt_ratio"));
  SFMuonTriggerBF->SetName("SFMuonTriggerBF");
  SFDb->SetName("SFDb");
  SFDb->Add(SFMuonTriggerBF);
  TFile *f2 = TFile::Open("files/mc/2016/sf/EfficienciesAndSF_Period4.root","READ");
  auto SFMuonTriggerGH = static_cast<TH2F*>(f2->Get("Mu50_OR_TkMu50_PtEtaBins/abseta_pt_ratio"));
  SFMuonTriggerGH->SetName("SFMuonTriggerGH");
  SFDb->Add(SFMuonTriggerGH);
  fProof->AddInputData(SFDb);
  TFile *f3 = TFile::Open("files/mc/2016/sf/RunBCDEF_SF_ID.root","READ");
  auto SFMuonIDBF = static_cast<TH2D*>(f3->Get("NUM_TightID_DEN_genTracks_eta_pt"));
  SFMuonIDBF->SetName("SFMuonIDBF");
  SFDb->Add(SFMuonIDBF);
  TFile *f4 = TFile::Open("files/mc/2016/sf/RunGH_SF_ID.root","READ");
  auto SFMuonIDGH = static_cast<TH2D*>(f4->Get("NUM_TightID_DEN_genTracks_eta_pt"));
  SFMuonIDGH->SetName("SFMuonIDGH");
  SFDb->Add(SFMuonIDGH);
  TFile *f5 = TFile::Open("files/mc/2016/sf/ElectronTriggerScaleFactors_eta_ele_binned_official_pt30to175_withsyst.root","READ");
  auto SFElectronTrigger1 = static_cast<TGraphAsymmErrors*>(f5->Get("ScaleFactors"));
  SFElectronTrigger1->SetName("SFElectronTrigger1");
  SFDb->Add(SFElectronTrigger1);
  TFile *f6 = TFile::Open("files/mc/2016/sf/ElectronTriggerScaleFactors_eta_ele_binned_official_pt175toInf.root","READ");
  auto SFElectronTrigger2 = static_cast<TGraphAsymmErrors*>(f6->Get("ScaleFactors"));
  SFElectronTrigger2->SetName("SFElectronTrigger2");
  SFDb->Add(SFElectronTrigger2);
#elif defined(Y2017)
  TFile *f1 = TFile::Open("files/mc/2017/sf/EfficienciesAndSF_RunBtoF_Nov17Nov2017.root","READ");
  // To be in sync with SFMuonID we provide also TH2 with x(pt) y(abseta)
  auto SFMuonTrigger = static_cast<TH2F*>(f1->Get("Mu50_PtEtaBins/pt_abseta_ratio"));
  SFMuonTrigger->SetName("SFMuonTrigger");
  SFDb->SetName("SFDb");
  SFDb->Add(SFMuonTrigger);
  TFile *f3 = TFile::Open("files/mc/2017/sf/RunBCDEF_SF_ID_2017.root","READ");
  // SDMuonID provides TH2 x(pt) y(abseta)
  auto SFMuonID = static_cast<TH2D*>(f3->Get("NUM_TightID_DEN_genTracks_pt_abseta"));
  SFMuonID->SetName("SFMuonID");
  SFDb->Add(SFMuonID);
  TFile *f5 = TFile::Open("files/mc/2017/sf/ElectronTriggerScaleFactors_eta_ele_binned_official_pt30to200_withsyst.root","READ");
  auto SFElectronTrigger1 = static_cast<TGraphAsymmErrors*>(f5->Get("ScaleFactors"));
  SFElectronTrigger1->SetName("SFElectronTrigger1");
  SFDb->Add(SFElectronTrigger1);
  TFile *f6 = TFile::Open("files/mc/2017/sf/ElectronTriggerScaleFactors_eta_ele_binned_official_pt200toInf.root","READ");
  auto SFElectronTrigger2 = static_cast<TGraphAsymmErrors*>(f6->Get("ScaleFactors"));
  SFElectronTrigger2->SetName("SFElectronTrigger2");
  SFDb->Add(SFElectronTrigger2);
#elif defined(Y2018)
  TFile *f1 = TFile::Open("files/mc/2018/sf/EfficienciesAndSF_2018Data_AfterMuonHLTUpdate.root","READ");
  auto SFMuonTrigger = static_cast<TH2F*>(f1->Get("Mu50_OR_OldMu100_OR_TkMu100_PtEtaBins/pt_abseta_ratio"));
  SFMuonTrigger->SetName("SFMuonTrigger");
  SFDb->SetName("SFDb");
  SFDb->Add(SFMuonTrigger);
  TFile *f3 = TFile::Open("files/mc/2017/sf/RunBCDEF_SF_ID_2017.root","READ");
  auto SFMuonID = static_cast<TH2D*>(f3->Get("NUM_TightID_DEN_genTracks_pt_abseta"));
  SFMuonID->SetName("SFMuonID");
  SFDb->Add(SFMuonID);
  TFile *f5 = TFile::Open("files/mc/2018/sf/ElectronTriggerScaleFactors_eta_ele_binned_official_pt30to200_withsyst.root","READ");
  auto SFElectronTrigger1 = static_cast<TGraphAsymmErrors*>(f5->Get("ScaleFactors"));
  SFElectronTrigger1->SetName("SFElectronTrigger1");
  SFDb->Add(SFElectronTrigger1);
  TFile *f6 = TFile::Open("files/mc/2018/sf/ElectronTriggerScaleFactors_eta_ele_binned_official_pt200toInf.root","READ");
  auto SFElectronTrigger2 = static_cast<TGraphAsymmErrors*>(f6->Get("ScaleFactors"));
  SFElectronTrigger2->SetName("SFElectronTrigger2");
  SFDb->Add(SFElectronTrigger2);
#endif
#endif
  TFile *f7 = TFile::Open("PileupWeights.root","READ");
  auto SFPileup = static_cast<TList*>(f7->Get("PileupSFList"));
  SFDb->Add(SFPileup);
  fProof->AddInputData(SFDb);

  fChain->SetProof();
  fChain->Process("PreSelector.C+g");
  fProof->Print("a");

  return 0;
}
