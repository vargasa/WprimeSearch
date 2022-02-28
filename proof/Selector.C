#include "TProof.h"
#include "IsData.h"
#include <array>
#include <string>
#include <fstream>

template<class T>
void AddSFHisto(std::string& newLabel,
                std::string& filename, std::string& histoname,
                TList* SFDb){
  TFile* f = TFile::Open(filename.c_str());
  T* h = static_cast<T*>(f->Get(histoname.c_str()));
  h->SetName(newLabel.c_str());
  SFDb->Add(h);
}

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
  std::string fileNameOut = "WprimeHistos_ULSFs.root";

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

  // Year, SFKind, File, HistogramName

  if (Year == 2016){
#ifndef ULSAMPLE
    AddSFHisto<TH1F>("SFDYKFactorQCD","files/mc/2016/sf/merged_kfactors_zjets.root","kfactor_monojet_qcd");
    AddSFHisto<TH1F>("SFDYKFactorEWK","files/mc/2016/sf/merged_kfactors_zjets.root","kfactor_monojet_ewk");
    AddSFHisto<TH2F>("SFMuonTriggerBF","files/mc/2016/sf/EfficienciesAndSF_RunBtoF.root","Mu50_OR_TkMu50_PtEtaBins/abseta_pt_ratio");
    AddSFHisto<TH2F>("SFMuonTriggerGH","files/mc/2016/sf/EfficienciesAndSF_Period4.root","Mu50_OR_TkMu50_PtEtaBins/abseta_pt_ratio");
    AnddSFHisto<TH2F>("SFMuonHighPtIDBF","files/mc/2016/sf/RunBCDEF_SF_ID.root","NUM_HighPtID_DEN_genTracks_eta_pair_newTuneP_probe_pt");
    AnddSFHisto<TH2F>("SFMuonHighPtIDGH","files/mc/2016/sf/RunGH_SF_ID.root","NUM_HighPtID_DEN_genTracks_eta_pair_newTuneP_probe_pt");
    // SFMuonTrkHighPt ?
    AddSFHisto<TGraphAsymmErrors>("SFElectronTrigger1","files/mc/2016/sf/ElectronTriggerScaleFactors_eta_ele_binned_official_pt30to175_withsyst.root","ScaleFactors");
    AddSFHisto<TGraphAsymmErrors>("SFElectronTrigger2","files/mc/2016/sf/ElectronTriggerScaleFactors_eta_ele_binned_official_pt175toInf.root","ScaleFactors");
    AddSFHisto<TH2F>("SFElectronLooseID","files/mc/2016/sf/2016LegacyReReco_ElectronLoose.root","EGamma_SF2D");
    AddSFHisto<TH2F>("SFElectronTightID","files/mc/2016/sf/2016LegacyReReco_ElectronTight_Fall17V2.root","EGamma_SF2D");
#elif defined(ULSAMPLE)
    AddSFHisto<TH1F>("SFDYKFactorQCD","files/mc/2016/sf/merged_kfactors_zjets.root","kfactor_monojet_qcd");
    AddSFHisto<TH1F>("SFDYKFactorEWK","files/mc/2016/sf/merged_kfactors_zjets.root","kfactor_monojet_ewk");
    AddSFHisto<TH2F>("SFMuonTrigger","files/mc/2016/UL/sf/OutFile-v20190510-Combined-Run2016BtoH_Run2017BtoF_Run2018AtoD-M120to10000.root","SF_2016_var");
    AddSFHisto<TH2F>("SFMuonHighPtIDpreVFP","files/mc/2016/UL/sf/Efficiencies_muon_generalTracks_Z_Run2016_UL_HIPM_ID.root","NUM_HighPtID_DEN_TrackerMuons_abseta_pt");
    AddSFHisto<TH2F>("SFMuonHighPtIDpostVFP","files/mc/2016/UL/sf/Efficiencies_muon_generalTracks_Z_Run2016_UL_ID.root","NUM_HighPtID_DEN_TrackerMuons_abseta_pt");
    AddSFHisto<TH2F>("SFMuonTrackerHighPtIDpreVFP","files/mc/2016/UL/sf/Efficiencies_muon_generalTracks_Z_Run2016_UL_HIPM_ID.root","NUM_TrkHighPtID_DEN_TrackerMuons_abseta_pt");
    AddSFHisto<TH2F>("SFMuonTrackerHighPtIDpostVFP","files/mc/2016/UL/sf/Efficiencies_muon_generalTracks_Z_Run2016_UL_ID.root","NUM_TrkHighPtID_DEN_TrackerMuons_abseta_pt");
    // AddSFHisto<>(SFElectronTrigger,"","");
    AddSFHisto<TH2F>("SFElectronLooseIDpreVFP","files/mc/2016/UL/sf/egammaEffi.txt_Ele_Loose_preVFP_EGM2D.root","EGamma_SF2D");
    AddSFHisto<TH2F>("SFElectronLooseIDpostVFP","files/mc/2016/UL/sf/egammaEffi.txt_Ele_Loose_postVFP_EGM2D.root","EGamma_SF2D");
    AddSFHisto<TH2F>("SFElectronTightIDpreVFP","files/mc/2016/UL/sf/egammaEffi.txt_Ele_Tight_preVFP_EGM2D.root","EGamma_SF2D");
    AddSFHisto<TH2F>("SFElectronTightIDpostVFP","files/mc/2016/UL/sf/egammaEffi.txt_Ele_Tight_postVFP_EGM2D.root","EGamma_SF2D");
#endif
  } else if (Year == 2017) {
#ifndef ULSAMPLE
    AddSFHisto<TH1F>("SFDYKFactorQCD","files/mc/2017/sf/SF_QCD_NLO_DYJetsToLL.root","kfac_dy_filter");
    AddSFHisto<TH1F>("SFDYKFactorEWK","files/mc/2016/sf/merged_kfactors_zjets.root","kfactor_monojet_ewk");
    AddSFHisto<TH2F>("SFMuonTrigger","files/mc/2017/sf/EfficienciesAndSF_RunBtoF_Nov17Nov2017.root","Mu50_PtEtaBins/abseta_pt_ratio");
    AddSFHisto<TH2F>("SFMuonHighPt","files/mc/2017/sf/EfficienciesStudies_UL2017_DEN_TrackerMuons_rootfiles_Efficiencies_muon_generalTracks_Z_Run2017_UL_ID.root","NUM_HighPtID_DEN_TrackerMuons_abseta_pt");
    AddSFHisto<TH2F>("SFMuonTrkHighPtID","files/mc/2017/sf/EfficienciesStudies_UL2017_DEN_TrackerMuons_rootfiles_Efficiencies_muon_generalTracks_Z_Run2017_UL_ID.root","NUM_TrkHighPtID_DEN_TrackerMuons_abseta_pt");
    AddSFHisto<TGraphAsymmErrors>("SFElectronTrigger1","files/mc/2017/sf/ElectronTriggerScaleFactors_eta_ele_binned_official_pt30to200_withsyst.root","ScaleFactors","SFElectronTrigger1");
    AddSFHisto<TGraphAsymmErrors>("SFElectronTrigger2","files/mc/2017/sf/ElectronTriggerScaleFactors_eta_ele_binned_official_pt200toInf.root","ScaleFactors","SFElectronTrigger2");
    AddSFHisto<TH2F>("SFElectronLooseID","files/mc/2017/sf/2017_ElectronLoose.root","EGamma_SF2D");
    AddSFHisto<TH2F>("SFElectronTightID","files/mc/2017/sf/2017_ElectronTight.root","EGamma_SF2D");
#elif defined(ULSAMPLE)
    AddSFHisto<TH1F>("SFDYKFactorQCD","files/mc/2016/sf/merged_kfactors_zjets.root","kfactor_monojet_qcd");
    AddSFHisto<TH1F>("SFDYKFactorEWK","files/mc/2016/sf/merged_kfactors_zjets.root","kfactor_monojet_ewk");
    AddSFHisto<TH2F>("SFMuonTrigger","files/mc/2016/UL/sf/OutFile-v20190510-Combined-Run2016BtoH_Run2017BtoF_Run2018AtoD-M120to10000.root","SF_2017_var");
    AddSFHisto<TH2D>("SFMuonHighPt","files/mc/2016/UL/sf/OutFile-v20190510-Combined-Run2016BtoH_Run2017BtoF_Run2018AtoD-M120to10000.root","NUM_HighPtID_DEN_TrackerMuons_abseta_pt");
    AddSFHisto<TH2D>("SFMuonTrackerHighPtID","files/mc/2016/UL/sf/OutFile-v20190510-Combined-Run2016BtoH_Run2017BtoF_Run2018AtoD-M120to10000.root","NUM_TrkHighPtID_DEN_TrackerMuons_abseta_pt");
    // AddSFHisto<>(SFElectronTrigger,"","");
    AddSFHisto<TH2F>("SFElectronLooseID","files/mc/2017/UL/sf/egammaEffi.txt_EGM2D_Loose_UL17.root","EGamma_SF2D");
    AddSFHisto<TH2F>("SFElectronTightID","files/mc/2017/UL/sf/egammaEffi.txt_EGM2D_Tight_UL17.root","EGamma_SF2D");
#endif
  } else if (Year == 2018) {
#ifndef ULSAMPLE
    AddSFHisto<TH1F>("SFDYKFactorQCD","files/mc/2017/sf/SF_QCD_NLO_DYJetsToLL.root","kfac_dy_filter");
    AddSFHisto<TH1F>("SFDYKFactorEWK","files/mc/2016/sf/merged_kfactors_zjets.root","kfactor_monojet_ewk");
    AddSFHisto<TH2F>("SFMuonTrigger","files/mc/2018/sf/EfficienciesAndSF_2018Data_AfterMuonHLTUpdate.root","Mu50_OR_OldMu100_OR_TkMu100_PtEtaBins/abseta_pt_ratio");
    AddSFHisto<TH2D>("SFMuonHighPt","files/mc/2018/sf/EfficienciesStudies_UL2018_DEN_TrackerMuons_rootfiles_Efficiencies_muon_generalTracks_Z_Run2018_UL_ID.root","NUM_HighPtID_DEN_TrackerMuons_abseta_pt");
    AddSFHisto<TH2D>("SFMuonTrkHighPtID","files/mc/2018/sf/EfficienciesStudies_UL2018_DEN_TrackerMuons_rootfiles_Efficiencies_muon_generalTracks_Z_Run2018_UL_ID.root","NUM_TrkHighPtID_DEN_TrackerMuons_abseta_pt");
    AddSFHisto<TGraphAsymmErrors>("SFElectronTrigger1","files/mc/2018/sf/ElectronTriggerScaleFactors_eta_ele_binned_official_pt30to200_withsyst.root","ScaleFactors");
    AddSFHisto<TGraphAsymmErrors>("SFElectronTrigger2","files/mc/2018/sf/ElectronTriggerScaleFactors_eta_ele_binned_official_pt200toInf.root","ScaleFactors");
    AddSFHisto<TH2F>("SFElectronLooseID","files/mc/2018/sf/2018_ElectronLoose.root","EGamma_SF2D");
    AddSFHisto<TH2F>("SFElectronTightID","files/mc/2018/sf/2018_ElectronTight.root","EGamma_SF2D");
#elif defined(ULSAMPLE)
    AddSFHisto<TH1F>("SFDYKFactorQCD","files/mc/2016/sf/merged_kfactors_zjets.root","kfactor_monojet_qcd");
    AddSFHisto<TH1F>("SFDYKFactorEWK","files/mc/2016/sf/merged_kfactors_zjets.root","kfactor_monojet_ewk");
    AddSFHisto<>(SFMuonTrigger,"files/mc/2016/UL/sf/OutFile-v20190510-Combined-Run2016BtoH_Run2017BtoF_Run2018AtoD-M120to10000.root","SF_2018_var");
    AddSFHisto<>(SFMuonHighPt,"files/mc/2018/UL/sf/Efficiencies_muon_generalTracks_Z_Run2018_UL_ID.root","NUM_HighPtID_DEN_TrackerMuons_abseta_pt");
    AddSFHisto<>(SFMuonTrackerHighPtID,"files/mc/2018/UL/sf/Efficiencies_muon_generalTracks_Z_Run2018_UL_ID.root","NUM_TrkHighPtID_DEN_TrackerMuons_abseta_pt");
    // AddSFHisto<>(SFElectronTrigger,"","");
    AddSFHisto<TH2F>("SFElectronLooseID","files/mc/2018/UL/sf/egammaEffi.txt_Ele_Loose_EGM2D.root","EGamma_SF2D");
    AddSFHisto<TH2F>("SFElectronTightID","files/mc/2018/UL/sf/egammaEffi.txt_Ele_Tight_EGM2D.root","EGamma_SF2D");
#endif
  };

  AddSFHisto<TH1F>("SFPileup","PileupWeights.root","PileupSFList");

  fChain->SetProof();
  fChain->Process("PreSelector.C+g");
  fProof->Print("a");

  return 0;
}
