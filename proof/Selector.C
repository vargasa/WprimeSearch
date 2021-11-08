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
  std::string fileNameOut = "WprimeHistos_CR2.root";

  while(std::getline(f,file,'+')){
    sample += file.substr(file.rfind("/")+1);
    sample.resize(sample.size()-4);

    TFile* outFile = TFile::Open(fileNameOut.c_str(),"READ");
    if (outFile != NULL){
      Bool_t skip = outFile->cd(Form("%d/%s", Year, sample.c_str()));
      Int_t nHistos = gDirectory->GetNkeys();
      skip = skip and (nHistos > 1);
      outFile->Close();
      if (skip) {
        std::clog << Form("Sample found in output file [%d], skipping : %s \n", nHistos, sample.c_str()) ;
        return 0;
      }
    }
    std::ifstream infile(file);
    std::string line;
    while(std::getline(infile, line)){
      if(line.empty() or line.find("#") == 0) continue;
      line = Form("root://cmsxrootd.fnal.gov/%s",line.c_str());
      //line = Form("root://cms-xrd-global.cern.ch/%s",line.c_str());
      std::cout << "Chaining " << line << std::endl;
      fChain->AddFile(line.c_str());
    }
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
  fProof->SetParameter("FileNameOut", fileNameOut.c_str());



#ifndef CMSDATA

  TList *SFDb = new TList();
  SFDb->SetName("SFDb");

#ifdef Y2016
  TFile *f0 = TFile::Open("files/mc/2016/sf/merged_kfactors_zjets.root","READ");
  auto SFDYKFactorQCD = static_cast<TH1F*>(f0->Get("kfactor_monojet_qcd"));
  SFDYKFactorQCD->SetName("SFDYKFactorQCD");
  SFDb->Add(SFDYKFactorQCD);
  auto SFDYKFactorEWK = static_cast<TH1F*>(f0->Get("kfactor_monojet_ewk"));
  SFDYKFactorEWK->SetName("SFDYKFactorEWK");
  SFDb->Add(SFDYKFactorEWK);
  TFile *f1 = TFile::Open("files/mc/2016/sf/EfficienciesAndSF_RunBtoF.root","READ");
  auto SFMuonTriggerBF = static_cast<TH2F*>(f1->Get("Mu50_OR_TkMu50_PtEtaBins/abseta_pt_ratio"));
  SFMuonTriggerBF->SetName("SFMuonTriggerBF");
  SFDb->Add(SFMuonTriggerBF);
  TFile *f2 = TFile::Open("files/mc/2016/sf/EfficienciesAndSF_Period4.root","READ");
  auto SFMuonTriggerGH = static_cast<TH2F*>(f2->Get("Mu50_OR_TkMu50_PtEtaBins/abseta_pt_ratio"));
  SFMuonTriggerGH->SetName("SFMuonTriggerGH");
  SFDb->Add(SFMuonTriggerGH);
  auto *f3a = TFile::Open("files/mc/2016/sf/EfficienciesStudies_UL2016_postVFP_DEN_TrackerMuons_rootfiles_Efficiencies_muon_generalTracks_Z_Run2016_UL_ID.root","READ");
  auto SFMuonHighPtID = static_cast<TH2F*>(f3a->Get("NUM_HighPtID_DEN_TrackerMuons_abseta_pt"));
  SFMuonHighPtID->SetName("SFMuonHighPtID");
  SFDb->Add(SFMuonHighPtID);
  auto SFMuonTrkHighPtID = static_cast<TH2F*>(f3a->Get("NUM_TrkHighPtID_DEN_TrackerMuons_abseta_pt"));
  SFMuonTrkHighPtID->SetName("SFMuonTrkHighPtID");
  SFDb->Add(SFMuonTrkHighPtID);
  TFile *f5 = TFile::Open("files/mc/2016/sf/ElectronTriggerScaleFactors_eta_ele_binned_official_pt30to175_withsyst.root","READ");
  auto SFElectronTrigger1 = static_cast<TGraphAsymmErrors*>(f5->Get("ScaleFactors"));
  SFElectronTrigger1->SetName("SFElectronTrigger1");
  SFDb->Add(SFElectronTrigger1);
  TFile *f6 = TFile::Open("files/mc/2016/sf/ElectronTriggerScaleFactors_eta_ele_binned_official_pt175toInf.root","READ");
  auto SFElectronTrigger2 = static_cast<TGraphAsymmErrors*>(f6->Get("ScaleFactors"));
  SFElectronTrigger2->SetName("SFElectronTrigger2");
  SFDb->Add(SFElectronTrigger2);
  TFile *f7a = TFile::Open("files/mc/2016/sf/2016LegacyReReco_ElectronLoose.root","READ");
  auto SFElectronLooseID = static_cast<TH2F*>(f7a->Get("EGamma_SF2D"));
  SFElectronLooseID->SetName("SFElectronLooseID");
  SFDb->Add(SFElectronLooseID);
  TFile *f7b = TFile::Open("files/mc/2016/sf/2016LegacyReReco_ElectronTight_Fall17V2.root","READ");
  auto SFElectronTightID = static_cast<TH2F*>(f7b->Get("EGamma_SF2D"));
  SFElectronTightID->SetName("SFElectronTightID");
  SFDb->Add(SFElectronTightID);
#elif defined(Y2017)
  TFile *f0 = TFile::Open("files/mc/2017/sf/SF_QCD_NLO_DYJetsToLL.root","READ");
  auto SFDYKFactorQCD = static_cast<TH1F*>(f0->Get("kfac_dy_filter"));
  SFDYKFactorQCD->SetName("SFDYKFactorQCD");
  SFDb->Add(SFDYKFactorQCD);
  TFile *f0a = TFile::Open("files/mc/2016/sf/merged_kfactors_zjets.root","READ");
  auto SFDYKFactorEWK = static_cast<TH1F*>(f0a->Get("kfactor_monojet_ewk"));
  SFDYKFactorEWK->SetName("SFDYKFactorEWK");
  SFDb->Add(SFDYKFactorEWK);
  TFile *f1 = TFile::Open("files/mc/2017/sf/EfficienciesAndSF_RunBtoF_Nov17Nov2017.root","READ");
  auto SFMuonTrigger = static_cast<TH2F*>(f1->Get("Mu50_PtEtaBins/abseta_pt_ratio"));
  SFMuonTrigger->SetName("SFMuonTrigger");
  SFDb->Add(SFMuonTrigger);
  TFile *f3 = TFile::Open("files/mc/2017/sf/EfficienciesStudies_UL2017_DEN_TrackerMuons_rootfiles_Efficiencies_muon_generalTracks_Z_Run2017_UL_ID.root","READ");
  auto SFMuonHighPtID = static_cast<TH2D*>(f3->Get("NUM_HighPtID_DEN_TrackerMuons_abseta_pt"));
  SFMuonHighPtID->SetName("SFMuonHighPtID");
  SFDb->Add(SFMuonHighPtID);
  auto SFMuonTrkHighPtID = static_cast<TH2D*>(f3->Get("NUM_TrkHighPtID_DEN_TrackerMuons_abseta_pt"));
  SFMuonTrkHighPtID->SetName("SFMuonTrkHighPtID");
  SFDb->Add(SFMuonTrkHighPtID);
  TFile *f5 = TFile::Open("files/mc/2017/sf/ElectronTriggerScaleFactors_eta_ele_binned_official_pt30to200_withsyst.root","READ");
  auto SFElectronTrigger1 = static_cast<TGraphAsymmErrors*>(f5->Get("ScaleFactors"));
  SFElectronTrigger1->SetName("SFElectronTrigger1");
  SFDb->Add(SFElectronTrigger1);
  TFile *f6 = TFile::Open("files/mc/2017/sf/ElectronTriggerScaleFactors_eta_ele_binned_official_pt200toInf.root","READ");
  auto SFElectronTrigger2 = static_cast<TGraphAsymmErrors*>(f6->Get("ScaleFactors"));
  SFElectronTrigger2->SetName("SFElectronTrigger2");
  SFDb->Add(SFElectronTrigger2);
  TFile *f2 = TFile::Open("files/mc/2017/sf/2017_ElectronLoose.root","READ");
  auto SFElectronLooseID = static_cast<TH2F*>(f2->Get("EGamma_SF2D"));
  SFElectronLooseID->SetName("SFElectronLooseID");
  SFDb->Add(SFElectronLooseID);
  TFile *f2b = TFile::Open("files/mc/2017/sf/2017_ElectronTight.root","READ");
  auto SFElectronTightID = static_cast<TH2F*>(f2b->Get("EGamma_SF2D"));
  SFElectronTightID->SetName("SFElectronTightID");
  SFDb->Add(SFElectronTightID);
#elif defined(Y2018)
  TFile *f0 = TFile::Open("files/mc/2017/sf/SF_QCD_NLO_DYJetsToLL.root","READ");
  auto SFDYKFactorQCD = static_cast<TH1F*>(f0->Get("kfac_dy_filter"));
  SFDYKFactorQCD->SetName("SFDYKFactorQCD");
  SFDb->Add(SFDYKFactorQCD);
  TFile *f0a = TFile::Open("files/mc/2016/sf/merged_kfactors_zjets.root","READ");
  auto SFDYKFactorEWK = static_cast<TH1F*>(f0a->Get("kfactor_monojet_ewk"));
  SFDYKFactorEWK->SetName("SFDYKFactorEWK");
  SFDb->Add(SFDYKFactorEWK);
  TFile *f1 = TFile::Open("files/mc/2018/sf/EfficienciesAndSF_2018Data_AfterMuonHLTUpdate.root","READ");
  auto SFMuonTrigger = static_cast<TH2F*>(f1->Get("Mu50_OR_OldMu100_OR_TkMu100_PtEtaBins/abseta_pt_ratio"));
  SFMuonTrigger->SetName("SFMuonTrigger");
  SFDb->Add(SFMuonTrigger);
  TFile *f3 = TFile::Open("files/mc/2018/sf/EfficienciesStudies_UL2018_DEN_TrackerMuons_rootfiles_Efficiencies_muon_generalTracks_Z_Run2018_UL_ID.root","READ");
  auto SFMuonHighPtID = static_cast<TH2D*>(f3->Get("NUM_HighPtID_DEN_TrackerMuons_abseta_pt"));
  SFMuonHighPtID->SetName("SFMuonHighPtID");
  SFDb->Add(SFMuonHighPtID);
  auto SFMuonTrkHighPtID =  static_cast<TH2D*>(f3->Get("NUM_TrkHighPtID_DEN_TrackerMuons_abseta_pt"));
  SFMuonTrkHighPtID->SetName("SFMuonTrkHighPtID");
  SFDb->Add(SFMuonTrkHighPtID);
  TFile *f5 = TFile::Open("files/mc/2018/sf/ElectronTriggerScaleFactors_eta_ele_binned_official_pt30to200_withsyst.root","READ");
  auto SFElectronTrigger1 = static_cast<TGraphAsymmErrors*>(f5->Get("ScaleFactors"));
  SFElectronTrigger1->SetName("SFElectronTrigger1");
  SFDb->Add(SFElectronTrigger1);
  TFile *f6 = TFile::Open("files/mc/2018/sf/ElectronTriggerScaleFactors_eta_ele_binned_official_pt200toInf.root","READ");
  auto SFElectronTrigger2 = static_cast<TGraphAsymmErrors*>(f6->Get("ScaleFactors"));
  SFElectronTrigger2->SetName("SFElectronTrigger2");
  SFDb->Add(SFElectronTrigger2);
  TFile *f7a = TFile::Open("files/mc/2018/sf/2018_ElectronLoose.root","READ");
  auto SFElectronLooseID = static_cast<TH2F*>(f7a->Get("EGamma_SF2D"));
  SFElectronLooseID->SetName("SFElectronLooseID");
  SFDb->Add(SFElectronLooseID);
  TFile *f7b = TFile::Open("files/mc/2018/sf/2018_ElectronTight.root","READ");
  auto SFElectronTightID = static_cast<TH2F*>(f7b->Get("EGamma_SF2D"));
  SFElectronTightID->SetName("SFElectronTightID");
  SFDb->Add(SFElectronTightID);

#endif
  TFile *f7 = TFile::Open("PileupWeights.root","READ");
  auto SFPileup = static_cast<TList*>(f7->Get("PileupSFList"));
  SFPileup->SetName("PileupSFList");
  SFDb->Add(SFPileup);
  fProof->AddInputData(SFDb);
#endif

  fChain->SetProof();
  fChain->Process("PreSelector.C+g");
  fProof->Print("a");

  return 0;
}
