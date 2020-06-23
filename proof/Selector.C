#include "TProof.h"
#include <array>
#include <string>
#include <fstream>

Int_t Selector(std::string files = "", Int_t fWorkers = 4, std::string elistfile = "", Int_t year = 2016,
               TString SFFileBF = "EfficienciesAndSF_RunBtoF.root",
               TString SFFileGH = "EfficienciesAndSF_Period4.root"){

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
  fProof->AddInputData(SFDb);

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
