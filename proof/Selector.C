#include "TProof.h"
#include "IsData.h"
#include <array>
#include <string>
#include <fstream>
#include <type_traits>

template<class T>
void AddSFHistoSFDb(std::string newLabel,
                std::string filename, std::string histoname,
                TList* SFDb){
  TFile* f = TFile::Open(filename.c_str());
  T* h = dynamic_cast<T*>((f->Get(histoname.c_str()))->Clone());
  h->SetName(newLabel.c_str());
  std::clog << "\tAdding " << h->GetName() << " to SFDB: " << filename << ":" << histoname <<  ":" << h  <<" \n";
  if constexpr (std::is_same_v<T,TH2D> or std::is_same_v<T,TH2F>)
                 std::clog << "\t\tx=[" << h->GetXaxis()->GetXmin() << ":" << h->GetXaxis()->GetXmax() << "]"
                           << "\ty=[" << h->GetYaxis()->GetXmin() << ":" << h->GetYaxis()->GetXmax() << "]\n" ;
  SFDb->Add(h);
}



TList* CreateSFDb(Int_t Year, Bool_t IsUL, TList* SFDb){

  std::clog << "Creating SFDb: " << Year << " IsUL? " << IsUL << "\n";

  // Year, SFKind, File, HistogramName

  if (Year == 2016){
    if (!IsUL) {
      AddSFHistoSFDb<TH1F>("SFDYKFactorQCD","files/mc/2016/sf/merged_kfactors_zjets.root","kfactor_monojet_qcd",SFDb);
      AddSFHistoSFDb<TH1F>("SFDYKFactorEWK","files/mc/2016/sf/merged_kfactors_zjets.root","kfactor_monojet_ewk",SFDb);
      AddSFHistoSFDb<TH2F>("SFMuonTriggerBF","files/mc/2016/sf/EfficienciesAndSF_RunBtoF.root","Mu50_OR_TkMu50_PtEtaBins/abseta_pt_ratio",SFDb);
      AddSFHistoSFDb<TH2F>("SFMuonTriggerGH","files/mc/2016/sf/EfficienciesAndSF_Period4.root","Mu50_OR_TkMu50_PtEtaBins/abseta_pt_ratio",SFDb);
      AddSFHistoSFDb<TH2D>("SFMuonHighPtIDBF","files/mc/2016/sf/RunBCDEF_SF_ID.root","NUM_HighPtID_DEN_genTracks_eta_pair_newTuneP_probe_pt",SFDb);
      AddSFHistoSFDb<TH2D>("SFMuonHighPtIDGH","files/mc/2016/sf/RunGH_SF_ID.root","NUM_HighPtID_DEN_genTracks_eta_pair_newTuneP_probe_pt",SFDb);
      // SFMuonTrkHighPt ?
      AddSFHistoSFDb<TGraphAsymmErrors>("SFElectronTrigger1","files/mc/2016/sf/ElectronTriggerScaleFactors_eta_ele_binned_official_pt30to175_withsyst.root","ScaleFactors",SFDb);
      AddSFHistoSFDb<TGraphAsymmErrors>("SFElectronTrigger2","files/mc/2016/sf/ElectronTriggerScaleFactors_eta_ele_binned_official_pt175toInf.root","ScaleFactors",SFDb);
      AddSFHistoSFDb<TH2F>("SFElectronLooseID","files/mc/2016/sf/2016LegacyReReco_ElectronLoose.root","EGamma_SF2D",SFDb);
      AddSFHistoSFDb<TH2F>("SFElectronMediumID","files/mc/2016/sf/2016LegacyReReco_ElectronLoose.root","EGamma_SF2D",SFDb); //FixMe
      AddSFHistoSFDb<TH2F>("SFElectronTightID","files/mc/2016/sf/2016LegacyReReco_ElectronTight_Fall17V2.root","EGamma_SF2D",SFDb);
    } else {
      AddSFHistoSFDb<TH1F>("SFDYKFactorQCD","files/mc/2016/sf/merged_kfactors_zjets.root","kfactor_monojet_qcd",SFDb);
      AddSFHistoSFDb<TH1F>("SFDYKFactorEWK","files/mc/2016/sf/merged_kfactors_zjets.root","kfactor_monojet_ewk",SFDb);
      AddSFHistoSFDb<TH2F>("SFMuonTrigger","files/mc/2016/UL/sf/OutFile-v20190510-Combined-Run2016BtoH_Run2017BtoF_Run2018AtoD-M120to10000.root","SF_2016_var",SFDb);
      AddSFHistoSFDb<TH2F>("SFMuonHighPtIDpreVFP","files/mc/2016/UL/sf/Efficiencies_muon_generalTracks_Z_Run2016_UL_HIPM_ID.root","NUM_HighPtID_DEN_TrackerMuons_abseta_pt",SFDb);
      AddSFHistoSFDb<TH2F>("SFMuonHighPtIDpostVFP","files/mc/2016/UL/sf/Efficiencies_muon_generalTracks_Z_Run2016_UL_ID.root","NUM_HighPtID_DEN_TrackerMuons_abseta_pt",SFDb);
      AddSFHistoSFDb<TH2F>("SFMuonTrkHighPtIDpreVFP","files/mc/2016/UL/sf/Efficiencies_muon_generalTracks_Z_Run2016_UL_HIPM_ID.root","NUM_TrkHighPtID_DEN_TrackerMuons_abseta_pt",SFDb);
      AddSFHistoSFDb<TH2F>("SFMuonTrkHighPtIDpostVFP","files/mc/2016/UL/sf/Efficiencies_muon_generalTracks_Z_Run2016_UL_ID.root","NUM_TrkHighPtID_DEN_TrackerMuons_abseta_pt",SFDb);
      AddSFHistoSFDb<TH2F>("SFElectronHLTLoosepreVFP","files/mc/2016/UL/sf/egammaEffiHLT.txt_EGM2D_2016PreVFPLoose.root","EGamma_SF2D",SFDb);
      AddSFHistoSFDb<TH2F>("SFElectronRecopreVFP","files/mc/2016/UL/sf/egammaEffi_ptAbove20.txt_EGM2D_UL2016preVFP.root","EGamma_SF2D",SFDb);
      AddSFHistoSFDb<TH2F>("SFElectronRecopostVFP","files/mc/2016/UL/sf/egammaEffi_ptAbove20.txt_EGM2D_UL2016postVFP.root","EGamma_SF2D",SFDb);
      AddSFHistoSFDb<TH2F>("SFElectronHLTLoosepostVFP","files/mc/2016/UL/sf/egammaEffiHLT.txt_EGM2D_2016PostVFPLoose.root","EGamma_SF2D",SFDb);
      AddSFHistoSFDb<TH2F>("SFElectronHLTMediumpreVFP","files/mc/2016/UL/sf/egammaEffiHLT.txt_EGM2D_2016PreVFPMedium.root","EGamma_SF2D",SFDb);
      AddSFHistoSFDb<TH2F>("SFElectronHLTMediumpostVFP","files/mc/2016/UL/sf/egammaEffiHLT.txt_EGM2D_2016PostVFPMedium.root","EGamma_SF2D",SFDb);
      AddSFHistoSFDb<TH2F>("SFElectronHLTTightpreVFP","files/mc/2016/UL/sf/egammaEffiHLT.txt_EGM2D_2016PreVFPTight.root","EGamma_SF2D",SFDb);
      AddSFHistoSFDb<TH2F>("SFElectronHLTTightpostVFP","files/mc/2016/UL/sf/egammaEffiHLT.txt_EGM2D_2016PostVFPTight.root","EGamma_SF2D",SFDb);
      AddSFHistoSFDb<TH2F>("SFElectronLooseIDpreVFP","files/mc/2016/UL/sf/egammaEffi.txt_Ele_Loose_preVFP_EGM2D.root","EGamma_SF2D",SFDb);
      AddSFHistoSFDb<TH2F>("SFElectronLooseIDpostVFP","files/mc/2016/UL/sf/egammaEffi.txt_Ele_Loose_postVFP_EGM2D.root","EGamma_SF2D",SFDb);
      AddSFHistoSFDb<TH2F>("SFElectronMediumIDpreVFP","files/mc/2016/UL/sf/egammaEffi.txt_Ele_Medium_preVFP_EGM2D.root","EGamma_SF2D",SFDb);
      AddSFHistoSFDb<TH2F>("SFElectronMediumIDpostVFP","files/mc/2016/UL/sf/egammaEffi.txt_Ele_Medium_postVFP_EGM2D.root","EGamma_SF2D",SFDb);
      AddSFHistoSFDb<TH2F>("SFElectronTightIDpreVFP","files/mc/2016/UL/sf/egammaEffi.txt_Ele_Tight_preVFP_EGM2D.root","EGamma_SF2D",SFDb);
      AddSFHistoSFDb<TH2F>("SFElectronTightIDpostVFP","files/mc/2016/UL/sf/egammaEffi.txt_Ele_Tight_postVFP_EGM2D.root","EGamma_SF2D",SFDb);
    }
  } else if (Year == 2017) {
    if (!IsUL) {
      AddSFHistoSFDb<TH1F>("SFDYKFactorQCD","files/mc/2016/sf/merged_kfactors_zjets.root","kfactor_monojet_qcd",SFDb);
      AddSFHistoSFDb<TH1F>("SFDYKFactorEWK","files/mc/2016/sf/merged_kfactors_zjets.root","kfactor_monojet_ewk",SFDb);
      AddSFHistoSFDb<TH2F>("SFMuonTrigger","files/mc/2017/sf/EfficienciesAndSF_RunBtoF_Nov17Nov2017.root","Mu50_PtEtaBins/abseta_pt_ratio",SFDb);
      AddSFHistoSFDb<TH2F>("SFMuonHighPt","files/mc/2017/sf/EfficienciesStudies_UL2017_DEN_TrackerMuons_rootfiles_Efficiencies_muon_generalTracks_Z_Run2017_UL_ID.root","NUM_HighPtID_DEN_TrackerMuons_abseta_pt",SFDb);
      AddSFHistoSFDb<TH2F>("SFMuonTrkHighPtID","files/mc/2017/sf/EfficienciesStudies_UL2017_DEN_TrackerMuons_rootfiles_Efficiencies_muon_generalTracks_Z_Run2017_UL_ID.root","NUM_TrkHighPtID_DEN_TrackerMuons_abseta_pt",SFDb);
      AddSFHistoSFDb<TGraphAsymmErrors>("SFElectronTrigger1","files/mc/2017/sf/ElectronTriggerScaleFactors_eta_ele_binned_official_pt30to200_withsyst.root","ScaleFactors",SFDb);
      AddSFHistoSFDb<TGraphAsymmErrors>("SFElectronTrigger2","files/mc/2017/sf/ElectronTriggerScaleFactors_eta_ele_binned_official_pt200toInf.root","ScaleFactors",SFDb);
      AddSFHistoSFDb<TH2F>("SFElectronLooseID","files/mc/2017/sf/2017_ElectronLoose.root","EGamma_SF2D",SFDb);
      AddSFHistoSFDb<TH2F>("SFElectronMediumID","files/mc/2017/sf/2017_ElectronLoose.root","EGamma_SF2D",SFDb); //FixMe
      AddSFHistoSFDb<TH2F>("SFElectronTightID","files/mc/2017/sf/2017_ElectronTight.root","EGamma_SF2D",SFDb);
    } else {
      AddSFHistoSFDb<TH1F>("SFDYKFactorQCD","files/mc/2016/sf/merged_kfactors_zjets.root","kfactor_monojet_qcd",SFDb);
      AddSFHistoSFDb<TH1F>("SFDYKFactorEWK","files/mc/2016/sf/merged_kfactors_zjets.root","kfactor_monojet_ewk",SFDb);
      AddSFHistoSFDb<TH2F>("SFMuonTrigger","files/mc/2016/UL/sf/OutFile-v20190510-Combined-Run2016BtoH_Run2017BtoF_Run2018AtoD-M120to10000.root","SF_2017_var",SFDb);
      AddSFHistoSFDb<TH2F>("SFMuonHighPt","files/mc/2017/UL/sf/Efficiencies_muon_generalTracks_Z_Run2017_UL_ID.root","NUM_HighPtID_DEN_TrackerMuons_abseta_pt",SFDb);
      AddSFHistoSFDb<TH2F>("SFMuonTrkHighPtID","files/mc/2017/UL/sf/Efficiencies_muon_generalTracks_Z_Run2017_UL_ID.root","NUM_TrkHighPtID_DEN_TrackerMuons_abseta_pt",SFDb);
      AddSFHistoSFDb<TH2F>("SFElectronReco","files/mc/2017/UL/sf/egammaEffi_ptAbove20.txt_EGM2D_UL2017.root","EGamma_SF2D",SFDb);
      AddSFHistoSFDb<TH2F>("SFElectronHLTLoose","files/mc/2017/UL/sf/egammaEffiHLT.txt_EGM2D_2017Loose.root","EGamma_SF2D",SFDb);
      AddSFHistoSFDb<TH2F>("SFElectronHLTMedium","files/mc/2017/UL/sf/egammaEffiHLT.txt_EGM2D_2017Medium.root","EGamma_SF2D",SFDb);
      AddSFHistoSFDb<TH2F>("SFElectronHLTTight","files/mc/2017/UL/sf/egammaEffiHLT.txt_EGM2D_2017Tight.root","EGamma_SF2D",SFDb);
      AddSFHistoSFDb<TH2F>("SFElectronLooseID","files/mc/2017/UL/sf/egammaEffi.txt_EGM2D_Loose_UL17.root","EGamma_SF2D",SFDb);
      AddSFHistoSFDb<TH2F>("SFElectronMediumID","files/mc/2017/UL/sf/egammaEffi.txt_EGM2D_Medium_UL17.root","EGamma_SF2D",SFDb);
      AddSFHistoSFDb<TH2F>("SFElectronTightID","files/mc/2017/UL/sf/egammaEffi.txt_EGM2D_Tight_UL17.root","EGamma_SF2D",SFDb);
    }
  } else if (Year == 2018) {
    if (!IsUL) {
      AddSFHistoSFDb<TH1F>("SFDYKFactorQCD","files/mc/2016/sf/merged_kfactors_zjets.root","kfactor_monojet_qcd",SFDb);
      AddSFHistoSFDb<TH1F>("SFDYKFactorEWK","files/mc/2016/sf/merged_kfactors_zjets.root","kfactor_monojet_ewk",SFDb);
      AddSFHistoSFDb<TH2F>("SFMuonTrigger","files/mc/2018/sf/EfficienciesAndSF_2018Data_AfterMuonHLTUpdate.root","Mu50_OR_OldMu100_OR_TkMu100_PtEtaBins/abseta_pt_ratio",SFDb);
      AddSFHistoSFDb<TH2F>("SFMuonHighPt","files/mc/2018/sf/EfficienciesStudies_UL2018_DEN_TrackerMuons_rootfiles_Efficiencies_muon_generalTracks_Z_Run2018_UL_ID.root","NUM_HighPtID_DEN_TrackerMuons_abseta_pt",SFDb);
      AddSFHistoSFDb<TH2F>("SFMuonTrkHighPtID","files/mc/2018/sf/EfficienciesStudies_UL2018_DEN_TrackerMuons_rootfiles_Efficiencies_muon_generalTracks_Z_Run2018_UL_ID.root","NUM_TrkHighPtID_DEN_TrackerMuons_abseta_pt",SFDb);
      AddSFHistoSFDb<TGraphAsymmErrors>("SFElectronTrigger1","files/mc/2018/sf/ElectronTriggerScaleFactors_eta_ele_binned_official_pt30to200_withsyst.root","ScaleFactors",SFDb);
      AddSFHistoSFDb<TGraphAsymmErrors>("SFElectronTrigger2","files/mc/2018/sf/ElectronTriggerScaleFactors_eta_ele_binned_official_pt200toInf.root","ScaleFactors",SFDb);
      AddSFHistoSFDb<TH2F>("SFElectronLooseID","files/mc/2018/sf/2018_ElectronLoose.root","EGamma_SF2D",SFDb);
      AddSFHistoSFDb<TH2F>("SFElectronMediumID","files/mc/2018/sf/2018_ElectronLoose.root","EGamma_SF2D",SFDb); //FixMe
      AddSFHistoSFDb<TH2F>("SFElectronTightID","files/mc/2018/sf/2018_ElectronTight.root","EGamma_SF2D",SFDb);
    } else {
      AddSFHistoSFDb<TH1F>("SFDYKFactorQCD","files/mc/2016/sf/merged_kfactors_zjets.root","kfactor_monojet_qcd",SFDb);
      AddSFHistoSFDb<TH1F>("SFDYKFactorEWK","files/mc/2016/sf/merged_kfactors_zjets.root","kfactor_monojet_ewk",SFDb);
      AddSFHistoSFDb<TH2F>("SFMuonTrigger","files/mc/2016/UL/sf/OutFile-v20190510-Combined-Run2016BtoH_Run2017BtoF_Run2018AtoD-M120to10000.root","SF_2018_var",SFDb);
      AddSFHistoSFDb<TH2F>("SFMuonHighPt","files/mc/2018/UL/sf/Efficiencies_muon_generalTracks_Z_Run2018_UL_ID.root","NUM_HighPtID_DEN_TrackerMuons_abseta_pt",SFDb);
      AddSFHistoSFDb<TH2F>("SFMuonTrkHighPtID","files/mc/2018/UL/sf/Efficiencies_muon_generalTracks_Z_Run2018_UL_ID.root","NUM_TrkHighPtID_DEN_TrackerMuons_abseta_pt",SFDb);
      AddSFHistoSFDb<TH2F>("SFElectronReco","files/mc/2018/UL/sf/egammaEffi_ptAbove20.txt_EGM2D_UL2018.root","EGamma_SF2D",SFDb);
      AddSFHistoSFDb<TH2F>("SFElectronHLTLoose","files/mc/2018/UL/sf/egammaEffiHLT.txt_EGM2D_2018Loose.root","EGamma_SF2D",SFDb);
      AddSFHistoSFDb<TH2F>("SFElectronHLTMedium","files/mc/2018/UL/sf/egammaEffiHLT.txt_EGM2D_2018Medium.root","EGamma_SF2D",SFDb);
      AddSFHistoSFDb<TH2F>("SFElectronHLTTight","files/mc/2018/UL/sf/egammaEffiHLT.txt_EGM2D_2018Tight.root","EGamma_SF2D",SFDb);
      AddSFHistoSFDb<TH2F>("SFElectronLooseID","files/mc/2018/UL/sf/egammaEffi.txt_Ele_Loose_EGM2D.root","EGamma_SF2D",SFDb);
      AddSFHistoSFDb<TH2F>("SFElectronMediumID","files/mc/2018/UL/sf/egammaEffi.txt_Ele_Medium_EGM2D.root","EGamma_SF2D",SFDb);
      AddSFHistoSFDb<TH2F>("SFElectronTightID","files/mc/2018/UL/sf/egammaEffi.txt_Ele_Tight_EGM2D.root","EGamma_SF2D",SFDb);
    }
  };

  if(!IsUL){
    AddSFHistoSFDb<TList>("PileupSFList","PileupWeights.root","PileupSFList",SFDb);
  } else {
    AddSFHistoSFDb<TList>("PileupSFList","PileupWeights_UL.root","PileupSFList",SFDb);
  }

  return SFDb;
}

Int_t Selector(std::string outputLabel, std::string files = "", Int_t fWorkers = 4, std::string elistfile = ""){


  Int_t Year = 0;
#if defined(Y2016)
  Year = 2016;
#elif defined(Y2017)
  Year = 2017;
#elif defined(Y2018)
  Year = 2018;
#endif

  Bool_t IsUL{};
#if defined(ULSAMPLE)
  IsUL = true;
#endif

  Bool_t IsData{};
#if defined(CMSDATA)
  IsData = true;
#endif

  TChain* fChain = new TChain("Events");

  istringstream f(files);
  std::string file;
  std::string sample;
  std::string fileNameOut = "WprimeHistos_"+outputLabel+".root";

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
      std::cout << "Chaining [" << fChain->GetNtrees() << "]\t" << line << std::endl;
      fChain->AddFile(line.c_str());
    }
  }


  TEntryList *EList;
  TFile *FileEList;
  TProof *fProof = TProof::Open(Form("workers=%d",fWorkers));

  if(!elistfile.empty()){
    fProof->SetParameter("EntryListSet",1);
    fProof->SetParameter("MakeEventIDTree",1);
    if(IsData){
      FileEList = TFile::Open(elistfile.c_str(),"READ");
      EList = (TEntryList*)FileEList->Get(Form("%s_%d/EntryList",sample.c_str(),Year));
    }
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
  fProof->SetParameter("IsData", IsData? "true":"false");
  fProof->SetParameter("IsUL",IsUL? "true":"false");
  fProof->SetParameter("Year",std::to_string(Year).c_str());

  TList* SFDb = new TList();
  SFDb->SetName("SFDb");
  if (!IsData) {
    SFDb = CreateSFDb(Year,IsUL, SFDb);
    fProof->AddInputData(SFDb);
  }

  fChain->SetProof();
  fChain->Process("PreSelector.C+g");
  fProof->Print("a");

  return 0;
}
