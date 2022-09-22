#include <iostream>
#include <fstream>
#include <string.h>
#include "THStack.h"

void Stack(std::string FileName = "WprimeHistos_all.root"){

  std::string fileLabel;
  std::regex rexp("(WprimeHistos_)([A-Za-z0-9_-]+)(.root)");
  std::smatch sm;
  if(std::regex_search(FileName,sm,rexp))
    fileLabel = sm[2];

  TH1::SetDefaultSumw2();

  TFile* f1 = TFile::Open(FileName.c_str(),"READ");
  TFile* fOutput = TFile::Open(Form("WprimeStack_%s.root",fileLabel.c_str()),"RECREATE");

  std::unordered_map<int, float> luminosity = {
    // Run2 Luminosity
    // https://twiki.cern.ch/twiki/bin/view/CMS/TWikiLUM#CurRec
    {2016, 36.33},
    {2017, 41.48},
    {2018, 59.83},
    {0, 137.65}
  };

  std::unordered_map<std::string,std::string> channels = {
    {"A", "eee"},
    {"B", "eemu"},
    {"C", "mumue"},
    {"D", "mumumu"},
  };

  struct BackgroundInfo {
    std::string shortName;
    std::string legendName;
    std::string folderName;
    UInt_t color;
    Float_t xsec;
  };

  std::unordered_map<int, std::string> DataSampleNamesLegacy = { //Legacy
    {2016, "SinglePhotonSingleElectronSingleMuon"},
    {2017, "SinglePhotonSingleElectronSingleMuon"},
    {2018, "SingleMuonEGamma"}
  };

  std::unordered_map<int, std::string> DataSampleNames = { //UltraLegacy
    {2016, "ULSinglePhotonULSingleElectronULSingleMuon"},
    {2017, "ULSinglePhotonULSingleElectronULSingleMuon"},
    {2018, "ULSingleMuonULEGamma"}
  };


  // ShortName, DasName, kColor, Style, XSection
  std::unordered_map<int, std::vector<BackgroundInfo>> BgNames = //UltraLegacy
    {
      {
        2016,
        {
          BackgroundInfo{"WZ","WZ","WZTo3LNu_mllmin4p0_TuneCP5_13TeV-powheg-pythia8",kOrange,4.664e+00},
          BackgroundInfo{"ZZ","ZZ","ZZTo4L_M-1toInf_TuneCP5_13TeV_powheg_pythia8",kBlue,1.381e+01},
          //BackgroundInfo{"t#bar{t}","TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8",kBlue-2,7.592e+02},
          //BackgroundInfo{"t#bar{t}","TTJets_DiLept_TuneCP5_13TeV-madgraphMLM-pythia8",kBlue-2,5.675e1},
          BackgroundInfo{"ZG","Z#gamma","ZGToLLG_01J_5f_TuneCP5_13TeV-amcatnloFXFX-pythia8",kRed+3,5.148e+01},
          BackgroundInfo{"DYJets","DY","DYJetsToLL_M-50_HT-70to100_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8",kOrange+7,1.399e+02},
          BackgroundInfo{"DYJets","DY","DYJetsToLL_M-50_HT-100to200_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8",kOrange+7,1.401e+02},
          BackgroundInfo{"DYJets","DY","DYJetsToLL_M-50_HT-200to400_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8",kOrange+7,3.839e+01},
          BackgroundInfo{"DYJets","DY","DYJetsToLL_M-50_HT-400to600_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8",kOrange+7,5.217e+00},
          BackgroundInfo{"DYJets","DY","DYJetsToLL_M-50_HT-600to800_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8",kOrange+7,1.267e+00},
          BackgroundInfo{"DYJets","DY","DYJetsToLL_M-50_HT-800to1200_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8",kOrange+7,5.682e-01},
          BackgroundInfo{"DYJets","DY","DYJetsToLL_M-50_HT-1200to2500_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8",kOrange+7,1.324e-01},
          BackgroundInfo{"DYJets","DY","DYJetsToLL_M-50_HT-2500toInf_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8",kOrange+7,2.974e-03},
          BackgroundInfo{"TTW","t#bar{t}","TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8",kBlue-2,2.161e-01},
          BackgroundInfo{"TT","t#bar{t}","TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8",kBlue-2,88.29},
          BackgroundInfo{"TTSemiLep","t#bar{t}","TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8",kBlue-2,365.9744},
          BackgroundInfo{"TTZ","t#bar{t}","TTZToLL_M-1to10_TuneCP5_13TeV-amcatnlo-pythia8",kBlue-2,0.283}, /*XSecAnayzer 0.*/
          BackgroundInfo{"ttZJets","t#bar{t}","ttZJets_TuneCP5_13TeV_madgraphMLM_pythia8",kBlue-2,5.916e-01}, // dataset=/ttZJets*/RunIISummer20*/NANOAODSIM
          BackgroundInfo{"WWW","VVV","WWW_4F_TuneCP5_13TeV-amcatnlo-pythia8",14,2.158e-01},
          BackgroundInfo{"WWZ","VVV","WWZ_4F_TuneCP5_13TeV-amcatnlo-pythia8",14,1.707e-01},
          BackgroundInfo{"WZZ","VVV","WZZ_TuneCP5_13TeV-amcatnlo-pythia8",14,5.709e-02},
          BackgroundInfo{"ZZZ","VVV","ZZZ_TuneCP5_13TeV-amcatnlo-pythia8",14,1.476e-02},
          BackgroundInfo{"gg","gg","GluGluToContinToZZTo4e_TuneCP5_13TeV-mcfm701-pythia8",43,2.7e-3},
          BackgroundInfo{"gg","gg","GluGluToContinToZZTo4mu_TuneCP5_13TeV-mcfm701-pythia8",43,2.7e-3},
          BackgroundInfo{"gg","gg","GluGluToContinToZZTo2e2mu_TuneCP5_13TeV-mcfm701-pythia8",43,5.4e-3},
          BackgroundInfo{"gg","gg","GluGluToContinToZZTo4tau_TuneCP5_13TeV-mcfm701-pythia8",43,2.7e-3},
          BackgroundInfo{"STs","ST","ST_s-channel_4f_leptonDecays_TuneCP5_13TeV-amcatnlo-pythia8",kGreen+3,3.549e+00},
          BackgroundInfo{"STtWAntiTop","ST","ST_tW_antitop_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8",kGreen+3,3.251e+01},
          BackgroundInfo{"STtWTop","ST","ST_tW_top_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8",kGreen+3,3.245e+01},
          BackgroundInfo{"STtAntiTop","ST","ST_t-channel_antitop_5f_InclusiveDecays_TuneCP5_13TeV-powheg-pythia8",kGreen+3,7.174e+01},
          BackgroundInfo{"STtTop","ST","ST_t-channel_top_5f_InclusiveDecays_TuneCP5_13TeV-powheg-pythia8",kGreen+3,1.197e+02},
        }
      },
      {
        2017,
        {
          BackgroundInfo{"WZ","WZ","WZTo3LNu_mllmin4p0_TuneCP5_13TeV-powheg-pythia8",kOrange,4.664e+00},
          BackgroundInfo{"ZZ","ZZ","ZZTo4L_M-1toInf_TuneCP5_13TeV_powheg_pythia8",kBlue,1.381e+01},
          //BackgroundInfo{"t#bar{t}","TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8",kBlue-2,7.546e+02},
          //BackgroundInfo{"t#bar{t}","TTJets_DiLept_TuneCP5_13TeV-madgraphMLM-pythia8",kBlue-2,5.434e+1},
          BackgroundInfo{"ZG","Z#gamma","ZGToLLG_01J_5f_TuneCP5_13TeV-amcatnloFXFX-pythia8",kRed+3,5.557e+01},
          BackgroundInfo{"DYJets","DY","DYJetsToLL_M-50_HT-70to100_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8",kOrange+7,1.393e+02},
          BackgroundInfo{"DYJets","DY","DYJetsToLL_M-50_HT-100to200_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8",kOrange+7,},
          BackgroundInfo{"DYJets","DY","DYJetsToLL_M-50_HT-200to400_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8",kOrange+7,3.836e+01},
          BackgroundInfo{"DYJets","DY","DYJetsToLL_M-50_HT-400to600_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8",kOrange+7,5.222e+00},
          BackgroundInfo{"DYJets","DY","DYJetsToLL_M-50_HT-600to800_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8",kOrange+7,1.267e+00},
          BackgroundInfo{"DYJets","DY","DYJetsToLL_M-50_HT-800to1200_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8",kOrange+7,5.692e-01},
          BackgroundInfo{"DYJets","DY","DYJetsToLL_M-50_HT-1200to2500_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8",kOrange+7,1.330e-01},
          BackgroundInfo{"DYJets","DY","DYJetsToLL_M-50_HT-2500toInf_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8",kOrange+7,2.975e-03},
          BackgroundInfo{"TTW","t#bar{t}","TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8",kBlue-2,2.162e-01},
          BackgroundInfo{"TT","t#bar{t}","TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8",kBlue-2,88.29},
          BackgroundInfo{"TTSemiLep","t#bar{t}","TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8",kBlue-2,365.9744},
          BackgroundInfo{"TTZ","t#bar{t}","TTZToLL_M-1to10_TuneCP5_13TeV-amcatnlo-pythia8",kBlue-2,5.324e-2},
          BackgroundInfo{"ttZJets","t#bar{t}","ttZJets_TuneCP5_13TeV_madgraphMLM_pythia8",kBlue-2,5.910e-01},
          BackgroundInfo{"WWW","VVV","WWW_4F_TuneCP5_13TeV-amcatnlo-pythia8",14,2.158e-01},
          BackgroundInfo{"WWZ","VVV","WWZ_4F_TuneCP5_13TeV-amcatnlo-pythia8",14,1.707e-01},
          BackgroundInfo{"WZZ","VVV","WZZ_TuneCP5_13TeV-amcatnlo-pythia8",14,5.709e-02},
          BackgroundInfo{"ZZZ","VVV","ZZZ_TuneCP5_13TeV-amcatnlo-pythia8",14,1.476e-02},
          BackgroundInfo{"gg","gg","GluGluToContinToZZTo2e2mu_TuneCP5_13TeV-mcfm701-pythia8",43,5.4e-3},
          BackgroundInfo{"gg","gg","GluGluToContinToZZTo4e_TuneCP5_13TeV-mcfm701-pythia8",43,2.7e-3},
          BackgroundInfo{"gg","gg","GluGluToContinToZZTo4mu_TuneCP5_13TeV-mcfm701-pythia8",43,2.7e-3},
          BackgroundInfo{"gg","gg","GluGluToContinToZZTo4tau_TuneCP5_13TeV-mcfm701-pythia8",43,2.7e-3},
          BackgroundInfo{"STs","ST","ST_s-channel_4f_leptonDecays_TuneCP5_13TeV-amcatnlo-pythia8",kGreen+3,3.549e+00},
          BackgroundInfo{"STtAntiTop","ST","ST_t-channel_antitop_4f_InclusiveDecays_TuneCP5_13TeV-powheg-madspin-pythia8",kGreen+3,6.793e+01},
          BackgroundInfo{"STtTop","ST","ST_t-channel_top_4f_InclusiveDecays_TuneCP5_13TeV-powheg-madspin-pythia8",kGreen+3,1.134e+02},
          BackgroundInfo{"STtWAntiTop","ST","ST_tW_antitop_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8",kGreen+3,},
          BackgroundInfo{"STtWTop","ST","ST_tW_top_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8",kGreen+3,3.245e+01},
        },
      },
      {
        2018,
        {
          BackgroundInfo{"WZ","WZ","WZTo3LNu_mllmin4p0_TuneCP5_13TeV-powheg-pythia8",kOrange,4.664e+00},
          BackgroundInfo{"ZZ","ZZ","ZZTo4L_M-1toInf_TuneCP5_13TeV_powheg_pythia8",kBlue,1.381e+01},
          //BackgroundInfo{"t#bar{t}","TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8",kBlue-2,7.560e+02},
          BackgroundInfo{"ZG","Z#gamma","ZGToLLG_01J_5f_TuneCP5_13TeV-amcatnloFXFX-pythia8",kRed+3,5.557e+01},
          BackgroundInfo{"DYJets","DY","DYJetsToLL_M-50_HT-70to100_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8",kOrange+7,1.398e+02},
          BackgroundInfo{"DYJets","DY","DYJetsToLL_M-50_HT-100to200_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8",kOrange+7,1.405e+02},
          BackgroundInfo{"DYJets","DY","DYJetsToLL_M-50_HT-200to400_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8",kOrange+7,3.839e+01},
          BackgroundInfo{"DYJets","DY","DYJetsToLL_M-50_HT-400to600_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8",kOrange+7,5.213e+00},
          BackgroundInfo{"DYJets","DY","DYJetsToLL_M-50_HT-600to800_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8",kOrange+7,1.265e+00},
          BackgroundInfo{"DYJets","DY","DYJetsToLL_M-50_HT-800to1200_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8",kOrange+7,5.683e-01},
          BackgroundInfo{"DYJets","DY","DYJetsToLL_M-50_HT-1200to2500_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8",kOrange+7,1.328e-01},
          BackgroundInfo{"DYJets","DY","DYJetsToLL_M-50_HT-2500toInf_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8",kOrange+7,2.987e-03},
          BackgroundInfo{"TTW","t#bar{t}","TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8",kBlue-2,2.166e-01},
          BackgroundInfo{"TT","t#bar{t}","TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8",kBlue-2,88.29},
          BackgroundInfo{"TTSemiLep","t#bar{t}","TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8",kBlue-2,365.9744},
          BackgroundInfo{"TTZ","t#bar{t}","TTZToLL_M-1to10_TuneCP5_13TeV-amcatnlo-pythia8",kBlue-2,5.324e-2},
          BackgroundInfo{"ttZJets","t#bar{t}","ttZJets_TuneCP5_13TeV_madgraphMLM_pythia8",kBlue-2,5.901e-01},
          BackgroundInfo{"WWW","VVV","WWW_4F_TuneCP5_13TeV-amcatnlo-pythia8",14,2.158e-01},
          BackgroundInfo{"WWZ","VVV","WWZ_4F_TuneCP5_13TeV-amcatnlo-pythia8",14,1.707e-01},
          BackgroundInfo{"WZZ","VVV","WZZ_TuneCP5_13TeV-amcatnlo-pythia8",14,5.709e-02},
          BackgroundInfo{"ZZZ","VVV","ZZZ_TuneCP5_13TeV-amcatnlo-pythia8",14,1.476e-02},
          BackgroundInfo{"gg","gg","GluGluToContinToZZTo2e2mu_TuneCP5_13TeV-mcfm701-pythia8",43,5.4e-3},
          BackgroundInfo{"gg","gg","GluGluToContinToZZTo4e_TuneCP5_13TeV-mcfm701-pythia8",43,2.7e-3},
          BackgroundInfo{"gg","gg","GluGluToContinToZZTo4mu_TuneCP5_13TeV-mcfm701-pythia8",43,2.7e-3},
          BackgroundInfo{"gg","gg","GluGluToContinToZZTo4tau_TuneCP5_13TeV-mcfm701-pythia8",43,2.7e-3},
          BackgroundInfo{"STs","ST","ST_s-channel_4f_leptonDecays_TuneCP5_13TeV-amcatnlo-pythia8",kGreen+3,3.549e+00},
          BackgroundInfo{"STtAntitOP","ST","ST_t-channel_antitop_4f_InclusiveDecays_TuneCP5_13TeV-powheg-madspin-pythia8",kGreen+3,6.793e+01},
          BackgroundInfo{"STtTop","ST","ST_t-channel_top_4f_InclusiveDecays_TuneCP5_13TeV-powheg-madspin-pythia8",kGreen+3,1.134e+02},
          BackgroundInfo{"STtWAntiTop","ST","ST_tW_antitop_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8",kGreen+3,3.251e+01},
          BackgroundInfo{"STtWTop","ST","ST_tW_top_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8",kGreen+3,3.245e+01},
        }
      }
    };

  // ShortName, DasName, kColor, Style, XSection

  struct SignalInfo {
    std::string legendName;
    std::string folderName;
    Float_t xsec;
  };

  std::unordered_map<int,int> SignalPos = { /* WpMass, Position in SignalSample vector */
    {600, 0},
    {800, 1},
    {1000,2},
    {1200,3},
    {1400,4},
    {1600,5},
    {1800,6},
    {2000,7},
    {2500,8},
    {3000,9},
    {3500,10},
    {4000,11},
    {4500,12},
  };

  std::unordered_map<int, std::vector<SignalInfo>> SignalSamples = { //UltraLegacy
    {
      2016,
      {
        SignalInfo{"W' (0.6TeV)","WprimeToWZToWlepZlep_narrow_M600_TuneCP5_13TeV-madgraph-pythia8",6.252E-2},
        SignalInfo{"W' (0.8TeV)","WprimeToWZToWlepZlep_narrow_M800_TuneCP5_13TeV-madgraph-pythia8",1.886E-2},
        SignalInfo{"W' (1.0TeV)","WprimeToWZToWlepZlep_narrow_M1000_TuneCP5_13TeV-madgraph-pythia8",7.381E-3},
        SignalInfo{"W' (1.2TeV)","WprimeToWZToWlepZlep_narrow_M1200_TuneCP5_13TeV-madgraph-pythia8",3.356E-3},
        SignalInfo{"W' (1.4TeV)","WprimeToWZToWlepZlep_narrow_M1400_TuneCP5_13TeV-madgraph-pythia8",1.684E-3},
        SignalInfo{"W' (1.6TeV)","WprimeToWZToWlepZlep_narrow_M1600_TuneCP5_13TeV-madgraph-pythia8",9.036E-4},
        SignalInfo{"W' (1.8TeV)","WprimeToWZToWlepZlep_narrow_M1800_TuneCP5_13TeV-madgraph-pythia8",5.087E-4},
        SignalInfo{"W' (2.0TeV)","WprimeToWZToWlepZlep_narrow_M2000_TuneCP5_13TeV-madgraph-pythia8",2.967E-4},
        SignalInfo{"W' (2.5TeV)","WprimeToWZToWlepZlep_narrow_M2500_TuneCP5_13TeV-madgraph-pythia8",8.548E-5},
        SignalInfo{"W' (3.0TeV)","WprimeToWZToWlepZlep_narrow_M3000_TuneCP5_13TeV-madgraph-pythia8",2.680E-5},
        SignalInfo{"W' (3.5TeV)","WprimeToWZToWlepZlep_narrow_M3500_TuneCP5_13TeV-madgraph-pythia8",8.754E-6},
        SignalInfo{"W' (4.0TeV)","WprimeToWZToWlepZlep_narrow_M4000_TuneCP5_13TeV-madgraph-pythia8",2.893E-6},
        SignalInfo{"W' (4.5TeV)","WprimeToWZToWlepZlep_narrow_M4500_TuneCP5_13TeV-madgraph-pythia8",9.543E-7},
      }
    },
    {
      2017,
      {
        SignalInfo{"W' (0.6TeV)","WprimeToWZToWlepZlep_narrow_M600_TuneCP5_13TeV-madgraph-pythia8",6.252E-2},
        SignalInfo{"W' (0.8TeV)","WprimeToWZToWlepZlep_narrow_M800_TuneCP5_13TeV-madgraph-pythia8",1.886E-2},
        SignalInfo{"W' (1.0TeV)","WprimeToWZToWlepZlep_narrow_M1000_TuneCP5_13TeV-madgraph-pythia8",7.381E-3},
        SignalInfo{"W' (1.2TeV)","WprimeToWZToWlepZlep_narrow_M1200_TuneCP5_13TeV-madgraph-pythia8",3.356E-3},
        SignalInfo{"W' (1.4TeV)","WprimeToWZToWlepZlep_narrow_M1400_TuneCP5_13TeV-madgraph-pythia8",1.684E-3},
        SignalInfo{"W' (1.6TeV)","WprimeToWZToWlepZlep_narrow_M1600_TuneCP5_13TeV-madgraph-pythia8",9.036E-4},
        SignalInfo{"W' (1.8TeV)","WprimeToWZToWlepZlep_narrow_M1800_TuneCP5_13TeV-madgraph-pythia8",5.087E-4},
        SignalInfo{"W' (2.0TeV)","WprimeToWZToWlepZlep_narrow_M2000_TuneCP5_13TeV-madgraph-pythia8",2.967E-4},
        SignalInfo{"W' (2.5TeV)","WprimeToWZToWlepZlep_narrow_M2500_TuneCP5_13TeV-madgraph-pythia8",8.548E-5},
        SignalInfo{"W' (3.0TeV)","WprimeToWZToWlepZlep_narrow_M3000_TuneCP5_13TeV-madgraph-pythia8",2.680E-5},
        SignalInfo{"W' (3.5TeV)","WprimeToWZToWlepZlep_narrow_M3500_TuneCP5_13TeV-madgraph-pythia8",8.754E-6},
        SignalInfo{"W' (4.0TeV)","WprimeToWZToWlepZlep_narrow_M4000_TuneCP5_13TeV-madgraph-pythia8",2.893E-6},
        SignalInfo{"W' (4.5TeV)","WprimeToWZToWlepZlep_narrow_M4500_TuneCP5_13TeV-madgraph-pythia8",9.543E-7},
      }
    },
    {
      2018,
      {
        SignalInfo{"W' (0.6TeV)","WprimeToWZToWlepZlep_narrow_M600_TuneCP5_13TeV-madgraph-pythia8",6.252E-2},
        SignalInfo{"W' (0.8TeV)","WprimeToWZToWlepZlep_narrow_M800_TuneCP5_13TeV-madgraph-pythia8",1.886E-2},
        SignalInfo{"W' (1.0TeV)","WprimeToWZToWlepZlep_narrow_M1000_TuneCP5_13TeV-madgraph-pythia8",7.381E-3},
        SignalInfo{"W' (1.2TeV)","WprimeToWZToWlepZlep_narrow_M1200_TuneCP5_13TeV-madgraph-pythia8",3.356E-3},
        SignalInfo{"W' (1.4TeV)","WprimeToWZToWlepZlep_narrow_M1400_TuneCP5_13TeV-madgraph-pythia8",1.684E-3},
        SignalInfo{"W' (1.6TeV)","WprimeToWZToWlepZlep_narrow_M1600_TuneCP5_13TeV-madgraph-pythia8",9.036E-4},
        SignalInfo{"W' (1.8TeV)","WprimeToWZToWlepZlep_narrow_M1800_TuneCP5_13TeV-madgraph-pythia8",5.087E-4},
        SignalInfo{"W' (2.0TeV)","WprimeToWZToWlepZlep_narrow_M2000_TuneCP5_13TeV-madgraph-pythia8",2.967E-4},
        SignalInfo{"W' (2.5TeV)","WprimeToWZToWlepZlep_narrow_M2500_TuneCP5_13TeV-madgraph-pythia8",8.548E-5},
        SignalInfo{"W' (3.0TeV)","WprimeToWZToWlepZlep_narrow_M3000_TuneCP5_13TeV-madgraph-pythia8",2.680E-5},
        SignalInfo{"W' (3.5TeV)","WprimeToWZToWlepZlep_narrow_M3500_TuneCP5_13TeV-madgraph-pythia8",8.754E-6},
        SignalInfo{"W' (4.0TeV)","WprimeToWZToWlepZlep_narrow_M4000_TuneCP5_13TeV-madgraph-pythia8",2.893E-6},
        SignalInfo{"W' (4.5TeV)","WprimeToWZToWlepZlep_narrow_M4500_TuneCP5_13TeV-madgraph-pythia8",9.543E-7},
      }
    }
  };


  std::unordered_map<int, std::vector<SignalInfo>> SignalSamplesLegacy = { //Legacy
    {
      2016,
      {
        SignalInfo{"W' (0.6TeV)","WprimeToWZToWlepZlep_narrow_M-600_13TeV-madgraph",6.252E-2},
        SignalInfo{"W' (0.8TeV)","WprimeToWZToWlepZlep_narrow_M-800_13TeV-madgraph",1.886E-2},
        SignalInfo{"W' (1.0TeV)","WprimeToWZToWlepZlep_narrow_M-1000_13TeV-madgraph",7.381E-3},
        SignalInfo{"W' (1.2TeV)","WprimeToWZToWlepZlep_narrow_M-1200_13TeV-madgraph",3.356E-3},
        SignalInfo{"W' (1.4TeV)","WprimeToWZToWlepZlep_narrow_M-1400_13TeV-madgraph",1.684E-3},
        SignalInfo{"W' (1.6TeV)","WprimeToWZToWlepZlep_narrow_M-1600_13TeV-madgraph",9.036E-4},
        SignalInfo{"W' (1.8TeV)","WprimeToWZToWlepZlep_narrow_M-1800_13TeV-madgraph",5.087E-4},
        SignalInfo{"W' (2.0TeV)","WprimeToWZToWlepZlep_narrow_M-2000_13TeV-madgraph",2.967E-4},
        SignalInfo{"W' (2.5TeV)","WprimeToWZToWlepZlep_narrow_M-2500_13TeV-madgraph",8.548E-5},
        SignalInfo{"W' (3.0TeV)","WprimeToWZToWlepZlep_narrow_M-3000_13TeV-madgraph",2.680E-5},
        SignalInfo{"W' (3.5TeV)","WprimeToWZToWlepZlep_narrow_M-3500_13TeV-madgraph",8.754E-6},
        SignalInfo{"W' (4.0TeV)","WprimeToWZToWlepZlep_narrow_M-4000_13TeV-madgraph",2.893E-6},
        SignalInfo{"W' (4.5TeV)","WprimeToWZToWlepZlep_narrow_M-4500_13TeV-madgraph",9.543E-7},
      }
    },
    {
      2017,
      {
        SignalInfo{"W' (0.6TeV)","WprimeToWZToWlepZlep_narrow_M600_TuneCP5_13TeV-madgraph-pythia8",6.252E-2},
        SignalInfo{"W' (0.8TeV)","WprimeToWZToWlepZlep_narrow_M800_TuneCP5_13TeV-madgraph-pythia8",1.886E-2},
        SignalInfo{"W' (1.0TeV)","WprimeToWZToWlepZlep_narrow_M1000_TuneCP5_13TeV-madgraph-pythia8",7.381E-3},
        SignalInfo{"W' (1.2TeV)","WprimeToWZToWlepZlep_narrow_M1200_TuneCP5_13TeV-madgraph-pythia8",3.356E-3},
        SignalInfo{"W' (1.4TeV)","WprimeToWZToWlepZlep_narrow_M1400_TuneCP5_13TeV-madgraph-pythia8",1.684E-3},
        SignalInfo{"W' (1.6TeV)","WprimeToWZToWlepZlep_narrow_M1600_TuneCP5_13TeV-madgraph-pythia8",9.036E-4},
        SignalInfo{"W' (1.8TeV)","WprimeToWZToWlepZlep_narrow_M1800_TuneCP5_13TeV-madgraph-pythia8",5.087E-4},
        SignalInfo{"W' (2.0TeV)","WprimeToWZToWlepZlep_narrow_M2000_TuneCP5_13TeV-madgraph-pythia8",2.967E-4},
        SignalInfo{"W' (2.5TeV)","WprimeToWZToWlepZlep_narrow_M2500_TuneCP5_13TeV-madgraph-pythia8",8.548E-5},
        SignalInfo{"W' (3.0TeV)","WprimeToWZToWlepZlep_narrow_M3000_TuneCP5_13TeV-madgraph-pythia8",2.680E-5},
        SignalInfo{"W' (3.5TeV)","WprimeToWZToWlepZlep_narrow_M3500_TuneCP5_13TeV-madgraph-pythia8",8.754E-6},
        SignalInfo{"W' (4.0TeV)","WprimeToWZToWlepZlep_narrow_M4000_TuneCP5_13TeV-madgraph-pythia8",2.893E-6},
        SignalInfo{"W' (4.5TeV)","WprimeToWZToWlepZlep_narrow_M4500_TuneCP5_13TeV-madgraph-pythia8",9.543E-7},
      }
    },
    {
      2018,
      {
        SignalInfo{"W' (0.6TeV)","WprimeToWZToWlepZlep_narrow_M600_TuneCP5_13TeV-madgraph-pythia8",6.252E-2},
        SignalInfo{"W' (0.8TeV)","WprimeToWZToWlepZlep_narrow_M800_TuneCP5_13TeV-madgraph-pythia8",1.886E-2},
        SignalInfo{"W' (1.0TeV)","WprimeToWZToWlepZlep_narrow_M1000_TuneCP5_13TeV-madgraph-pythia8",7.381E-3},
        SignalInfo{"W' (1.2TeV)","WprimeToWZToWlepZlep_narrow_M1200_TuneCP5_13TeV-madgraph-pythia8",3.356E-3},
        SignalInfo{"W' (1.4TeV)","WprimeToWZToWlepZlep_narrow_M1400_TuneCP5_13TeV-madgraph-pythia8",1.684E-3},
        SignalInfo{"W' (1.6TeV)","WprimeToWZToWlepZlep_narrow_M1600_TuneCP5_13TeV-madgraph-pythia8",9.036E-4},
        SignalInfo{"W' (1.8TeV)","WprimeToWZToWlepZlep_narrow_M1800_TuneCP5_13TeV-madgraph-pythia8",5.087E-4},
        SignalInfo{"W' (2.0TeV)","WprimeToWZToWlepZlep_narrow_M2000_TuneCP5_13TeV-madgraph-pythia8",2.967E-4},
        SignalInfo{"W' (2.5TeV)","WprimeToWZToWlepZlep_narrow_M2500_TuneCP5_13TeV-madgraph-pythia8",8.548E-5},
        SignalInfo{"W' (3.0TeV)","WprimeToWZToWlepZlep_narrow_M3000_TuneCP5_13TeV-madgraph-pythia8",2.680E-5},
        SignalInfo{"W' (3.5TeV)","WprimeToWZToWlepZlep_narrow_M3500_TuneCP5_13TeV-madgraph-pythia8",8.754E-6},
        SignalInfo{"W' (4.0TeV)","WprimeToWZToWlepZlep_narrow_M4000_TuneCP5_13TeV-madgraph-pythia8",2.893E-6},
        SignalInfo{"W' (4.5TeV)","WprimeToWZToWlepZlep_narrow_M4500_TuneCP5_13TeV-madgraph-pythia8",9.543E-7},
      }
    }
  };


  std::unordered_map<std::string, std::string> Labels = {
    { "HMetUnclUpPt_A","MetUncl_{PtUp} (eee#nu); MetUncl_{PtUp} (eee#nu); Event count" },
    { "HMetUnclUpPt_B","MetUncl_{PtUp} (ee#mu#nu); MetUncl_{PtUp} (ee#mu#nu); Event count" },
    { "HMetUnclUpPt_C","MetUncl_{PtUp} (#mu#mue#nu); MetUncl_{PtUp} (#mu#mue#nu); Event count" },
    { "HMetUnclUpPt_D","MetUncl_{PtUp} (#mu#mu#mu#nu); MetUncl_{PtUp} (#mu#mu#mu#nu); Event count" },
    { "HMetUnclDownPt_A","MetUncl_{PtDown} (eee#nu); MetUncl_{PtDown} (eee#nu); Event count" },
    { "HMetUnclDownPt_B","MetUncl_{PtDown} (ee#mu#nu); MetUncl_{PtDown} (ee#mu#nu); Event count" },
    { "HMetUnclDownPt_C","MetUncl_{PtDown} (#mu#mue#nu); MetUncl_{PtDown} (#mu#mue#nu); Event count" },
    { "HMetUnclDownPt_D","MetUncl_{PtDown} (#mu#mu#mu#nu); MetUncl_{PtDown} (#mu#mu#mu#nu); Event count" },
    { "HMetUnclUpPhi_A","MetUncl_{PhiUp} (eee#nu); MetUncl_{PhiUp} (eee#nu); Event count" },
    { "HMetUnclUpPhi_B","MetUncl_{PhiUp} (ee#mu#nu); MetUncl_{PhiUp} (ee#mu#nu); Event count" },
    { "HMetUnclUpPhi_C","MetUncl_{PhiUp} (#mu#mue#nu); MetUncl_{PhiUp} (#mu#mue#nu); Event count" },
    { "HMetUnclUpPhi_D","MetUncl_{PhiUp} (#mu#mu#mu#nu); MetUncl_{PhiUp} (#mu#mu#mu#nu); Event count" },
    { "HMetUnclDownPhi_A","MetUncl_{PhiDown} (eee#nu); MetUncl_{PhiDown} (eee#nu); Event count" },
    { "HMetUnclDownPhi_B","MetUncl_{PhiDown} (ee#mu#nu); MetUncl_{PhiDown} (ee#mu#nu); Event count" },
    { "HMetUnclDownPhi_C","MetUncl_{PhiDown} (#mu#mue#nu); MetUncl_{PhiDown} (#mu#mue#nu); Event count" },
    { "HMetUnclDownPhi_D","MetUncl_{PhiDown} (#mu#mu#mu#nu); MetUncl_{PhiDown} (#mu#mu#mu#nu); Event count" },
    { "HMassZ_A","M_{Z}^{Z#rightarrow ee};M_{Z}^{eee#nu}(GeV);Event count" },
    { "HMassZ_B","M_{Z}^{Z#rightarrow ee};M_{Z}^{ee#mu#nu}(GeV);Event count" },
    { "HMassZ_C","M_{Z}^{Z#rightarrow #mu#mu};M_{Z}^{#mu#mue#nu}(GeV);Event count"},
    { "HMassZ_D","M_{Z}^{Z#rightarrow #mu#mu};M_{Z}^{#mu#mu#mu#nu}(GeV);Event count"},
    { "HMassZ_+ABCD","M_{Z} lll#nu;M_{Z} (GeV);Event count"},
    { "HMassTW_A","M_{T}^{W}(eee#nu);M_{WT}^{eee#nu} (GeV);Event count"},
    { "HMassTW_B","M_{T}^{W}(ee#mu#nu);M_{WT}^{ee#mu#nu} (GeV);Event count"},
    { "HMassTW_C","M_{T}^{W}(#mu#mue#nu);M_{WT}^{#mu#mue#nu} (GeV);Event count"},
    { "HMassTW_D","M_{T}^{W}(#mu#mu#mu#nu);M_{WT}^{#mu#mu#mu#nu} (GeV);Event count"},
    { "HMassTW_+ABCD","M_{T}^{W}(lll#nu);M_{WT};Event count"},
    { "HMassW_A","M_{W} eee#nu ;M_{W}^{eee#nu} (GeV);Event count"},
    { "HMassW_B","M_{W} ee#mu#nu ;M_{W}^{ee#mu#nu} (GeV);Event count"},
    { "HMassW_C","M_{W} #mu#mue#nu ;M_{W}^{#mu#mue#nu} (GeV);Event count"},
    { "HMassW_D","M_{W} #mu#mu#mu#nu ;M_{W} (GeV);Event count"},
    { "HMassW_+ABCD","M_{W} lll#nu ;M_{W}^{eee#nu} (GeV);Event count"},
    { "HMassWZ_A","M_{WZ} eee#nu ;M_{WZ}^{eee#nu} (GeV);Event count"},
    { "HMassWZ_B","M_{WZ} ee#mu#nu ;M_{WZ}^{ee#mu#nu} (GeV);Event count"},
    { "HMassWZ_C","M_{WZ} #mu#mue#nu ;M_{WZ}^{#mu#mue#nu} (GeV);Event count"},
    { "HMassWZ_D","M_{WZ} #mu#mu#mu#nu ;M_{WZ}^{#mu#mu#mu#nu} (GeV);Event count"},
    { "HMassWZ_+ABCD","M_{WZ} lll#nu ;M_{WZ} (GeV);Event count"},
    { "HPileup_A","nPvs eee#nu;nPvs;Event count"},
    { "HPileup_B","nPvs ee#mu#nu;nPvs;Event count"},
    { "HPileup_C","nPvs #mu#mue#nu;nPvs;Event count"},
    { "HPileup_D","nPvs #mu#mu#mu#nu;nPvs;Event count"},
    { "HPileup_+ABCD","nPvs lll#nu;nPvs;Event count"},
    { "HLt_A","eee#nu; P_{T}^{Lep} (GeV);Event count"},
    { "HLt_B","ee#mu#nu; P_{T}^{Lep} (GeV);Event count"},
    { "HLt_C","#mu#mu e#nu; P_{T}^{Lep} (GeV);Event count"},
    { "HLt_D","#mu#mu#mu#nu;P_{T}^{Lep} (GeV);Event count"},
    { "HLt_+ABCD","lll#nu;P_{T}^{Lep} (GeV);Event count"},
    { "HWZPt_A","Pt(WZ) eee#nu; P_{t}^{WZ} (GeV);Event count"},
    { "HWZPt_B","Pt(WZ) ee#mu#nu; P_{t}^{WZ} (GeV);Event count"},
    { "HWZPt_C","Pt(WZ) #mu#mue#nu; P_{t}^{WZ} (GeV);Event count"},
    { "HWZPt_D","Pt(WZ) #mu#mu#mu#nu; P_{t}^{WZ} (GeV);Event count"},
    { "HWZPt_+ABCD","Pt(WZ) lll#nu; P_{t}^{WZ} (GeV);Event count"},
    { "HDistWZ_A","dR(W,Z) eee#nu; dR(W,Z) (cm); Event count"},
    { "HDistWZ_B","dR(W,Z) ee#mu#nu; dR(W,Z) (cm); Event count"},
    { "HDistWZ_C","dR(W,Z) #mu#mue#nu; dR(W,Z) (cm); Event count"},
    { "HDistWZ_D","dR(W,Z) #mu#mu#mu#nu; dR(W,Z) (cm); Event count"},
    { "HDistl1l2_A","dR eee#nu;dR(e_{1},e_{2}) (cm);Event count"},
    { "HDistl1l2_B","dR ee#mu#nu;dR(e_{1},e_{2}) (cm);Event count"},
    { "HDistl1l2_C","dR #mu#mue#nu;dR(#mu_{1},#mu_{2}) (cm);Event count"},
    { "HDistl1l2_D","dR #mu#mu#mu#nu;dR(#mu_{1},#mu_{2}) (cm);Event count"},
    { "HDistl1l2_+ABCD","dR(l_{1},l_{2}) lll#nu;dR;Event count"},
    { "HDistl1l3_A","dR(e_{1},e_{3}) eee#nu;dR;Event count"},
    { "HDistl1l3_B","dR(e_{1},e_{3}) ee#mu#nu;dR;Event count"},
    { "HDistl1l3_C","dR(#mu_{1},#mu_{3}) #mu#mue#nu;dR;Event count"},
    { "HDistl1l3_D","dR(#mu_{1},#mu_{3}) #mu#mue#nu;dR;Event count"},
    { "HDistl1l3_+ABCD","dR(l_{1},l_{3}) lll#nu;dR;Event count"},
    { "HDistl2l3_A","dR(e_{2},e_{3}) eee#nu;dR;Event count"},
    { "HDistl2l3_B","dR(e_{2},e_{3}) ee#mu#nu;dR;Event count"},
    { "HDistl2l3_C","dR(#mu_{2},#mu_{3}) #mu#mue#nu;dR;Event count"},
    { "HDistl2l3_D","dR(#mu_{2},#mu_{3}) #mu#mue#nu;dR;Event count"},
    { "HDistl2l3_+ABCD","dR(l_{2},l_{3}) lll#nu;dR;Event count"},
    { "HWZDist_A","dr(W,Z) eee#nu;dR;Event count"},
    { "HWZDist_B","dr(W,Z) ee#mu#nu;dR;Event count"},
    { "HWZDist_C","dr(W,Z) ee#mu#nu;dR;Event count"},
    { "HWZDist_D","dr(W,Z) #mu#mu#mu#nu;dR;Event count"},
    { "HWZDist_+ABCD","dR(W,Z) lll#nu;dR;Event count"},
    { "HPtZMWZ_A","eee#nu;P_{t}^{Z}/M_{WZ};Event count"},
    { "HPtZMWZ_B","ee#mu#nu;P_{t}^{Z}/M_{WZ};Event count"},
    { "HPtZMWZ_C","#mu#mue#nu;P_{t}^{Z}/M_{WZ};Event count"},
    { "HPtZMWZ_D","#mu#mu#mu#nu;P_{t}^{Z}/M_{WZ};Event count"},
    { "HPtWMWZ_A","eee#nu;P_{t}^{W}/M_{WZ};Event count"},
    { "HPtWMWZ_B","ee#mu#nu;P_{t}^{W}/M_{WZ};Event count"},
    { "HPtWMWZ_C","#mu#mue#nu;P_{t}^{W}/M_{WZ};Event count"},
    { "HPtWMWZ_D","#mu#mu#mu#nu;P_{t}^{W}/M_{WZ};Event count"},
    { "HDistZl3_A","dr(Z,e_{3});dr(Z,e_{3})(cm);Event count"},
    { "HDistZl3_B","dr(Z,#mu);dr(Z(e,e),#mu)(cm);Event count"},
    { "HDistZl3_C","dr(Z,e);dr(Z(mu,mu),e)(cm);Event count"},
    { "HDistZl3_D","dr(Z,#mu);dr(Z,#mu)(cm);Event count"},
    { "HDistZW_A","dr eee#nu;dr(Z(e,e),W(e,#nu))(cm);Event count"},
    { "HDistZW_B","dr ee#mu#nu;dr(Z(e,e),W(#mu,#nu))(cm);Event count"},
    { "HDistZW_C","dr #mu#mue#nu;dr(Z(#mu,#mu),W)(cm);Event count"},
    { "HDistZW_D","dr #mu#mu#mu#nu;dr(Z,W)(cm);Event count"},
    { "HnEl_A","n_{e};n_{e};Event count"},
    { "HnEl_B","n_{e};n_{e};Event count"},
    { "HnEl_C","n_{e};n_{e};Event count"},
    { "HnEl_D","n_{e};n_{e};Event count"},
    { "HnEl_+ABCD","n_{e};n_{e};Event count"},
    { "HnMu_A","n_{#mu};n_{#mu};Event count"},
    { "HnMu_B","n_{#mu};n_{#mu};Event count"},
    { "HnMu_C","n_{#mu};n_{#mu};Event count"},
    { "HnMu_D","n_{#mu};n_{#mu};Event count"},
    { "HnMu_+ABCD","n_{#mu};n_{#mu};Event count"},
    { "HnJet_A","n_{Jet};n_{Jet};Event count"},
    { "HnJet_B","n_{Jet};n_{Jet};Event count"},
    { "HnJet_C","n_{Jet};n_{Jet};Event count"},
    { "HnJet_D","n_{Jet};n_{Jet};Event count"},
    { "HnJet_+ABCD","n_{Jet};n_{Jet};Event count"},
    { "HnbTag_A","n_{bTag};n_{bTag} medium;Event count"},
    { "HnbTag_B","n_{bTag};n_{bTag} medium;Event count"},
    { "HnbTag_C","n_{bTag};n_{bTag} medium;Event count"},
    { "HnbTag_D","n_{bTag};n_{bTag} medium;Event count"},
    { "HnbTag_+ABCD","n_{bTag};n_{bTag} medium;Event count"},
    { "HZPt_A","Z_{Pt} eee#nu; Z_{Pt} (GeV); Event count"},
    { "HZPt_B","Z_{Pt} ee#mu#nu; Z_{Pt} (GeV); Event count"},
    { "HZPt_C","Z_{Pt} #mu#mue#nu; Z_{Pt} (GeV); Event count"},
    { "HZPt_D","Z_{Pt} #mu#mu#mu#nu; Z_{Pt} (GeV); Event count"},
    { "HWPt_A","W_{Pt} eee#nu; W_{Pt} (GeV); Event count"},
    { "HWPt_B","W_{Pt} ee#mu#nu; W_{Pt} (GeV); Event count"},
    { "HWPt_C","W_{Pt} #mu#mue#nu; W_{Pt} (GeV); Event count"},
    { "HWPt_D","W_{Pt} #mu#mu#mu#nu; W_{Pt} (GeV); Event count"},
    { "HPtl1_A","Pt_{e1} eee#nu; Pt_{e1} (GeV); Event count"},
    { "HPtl1_B","Pt_{e1} ee#mu#nu; Pt_{e1} (GeV); Event count"},
    { "HPtl1_C","Pt_{#mu1} #mu#mue#nu; Pt_{#mu1} (GeV); Event count"},
    { "HPtl1_D","Pt_{#mu1} #mu#mu#mu#nu; Pt_{#mu1} (GeV); Event count"},
    { "HPtl1_+ABCD","Pt_{l1} lll#nu; Pt_{l1} (GeV); Event count"},
    { "HPtl2_A","Pt_{e2} eee#nu; Pt_{e2} (GeV); Event count"},
    { "HPtl2_B","Pt_{e2} ee#mu#nu; Pt_{e2} (GeV); Event count"},
    { "HPtl2_C","Pt_{#mu2} #mu#mue#nu; Pt_{#mu2} (GeV); Event count"},
    { "HPtl2_D","Pt_{#mu2} #mu#mu#mu#nu; Pt_{#mu2} (GeV); Event count"},
    { "HPtl2_+ABCD","Pt_{l2} lll#nu; Pt_{l2} (GeV); Event count"},
    { "HPtl3_A","Pt_{e3} eee#nu; Pt_{e3} (GeV); Event count"},
    { "HPtl3_B","Pt_{#mu} ee#mu#nu; Pt_{#mu} (GeV); Event count"},
    { "HPtl3_C","Pt_{e3} #mu#mue#nu; Pt_{e} (GeV); Event count"},
    { "HPtl3_D","Pt_{#mu3} #mu#mu#mu#nu; Pt_{#mu3} (GeV); Event count"},
    { "HPtl3_+ABCD","Pt_{l3} lll#nu; Pt_{l3} (GeV); Event count"},
    { "HMetPt_A","#slash{E}^{Z#rightarrow ee W#rightarrow e#nu}_{T};#slash{E}^{eee#nu}_{T}(GeV);Event count"},
    { "HMetPt_B","#slash{E}^{Z#rightarrow ee W#rightarrow #mu#nu}_{T};#slash{E}^{ee#mu#nu}_{T}(GeV);Event count"},
    { "HMetPt_C","#slash{E}^{Z#rightarrow #mu#mu W#rightarrow e#nu}_{T}#mu;#slash{E}^{#mu#mue#nu}_{T}(GeV);Event count"},
    { "HMetPt_D","#slash{E}^{Z#rightarrow #mu#mu W#rightarrow #mu#nu}_{T};#slash{E}^{#mu#mu#mu#nu}_{T}(GeV);Event count"},
    { "HMetPt_+ABCD","#slash{E}_{T} lll#nu;#slash{E}_{T}(GeV);Event count"},
    { "HEtal1_A", "#eta_{e1} eee#nu;#eta e_{1};Event count"},
    { "HEtal1_B", "#eta_{e1} ee#mu#nu;#eta e_{1};Event count"},
    { "HEtal1_C", "#eta_{#mu1} #mu#mue#nu;#eta #mu_{1};Event count"},
    { "HEtal1_D", "#eta_{#mu1} #mu#mu#mu#nu;#eta #mu_{1};Event count"},
    { "HEtal1_+ABCD ", "Eta_{l1} lll#nu;#eta e_{2};Event count"},
    { "HEtal2_A", "#eta_{e2} eee#nu;#eta e_{2};Event count"},
    { "HEtal2_B", "#eta_{e2} ee#mu#nu;#eta e_{2};Event count"},
    { "HEtal2_C", "#eta_{#mu2} #mu#mue#nu;#eta #mu_{2};Event count"},
    { "HEtal2_D", "#eta_{#mu2} #mu#mu#mu#nu;#eta #mu_{2};Event count"},
    { "HEtal2_+ABCD ", "Eta_{l2} lll#nu;Eta;Event count"},
    { "HEtal3_A", "#eta_{e3} eee#nu;#eta e_{3};Event count"},
    { "HEtal3_B", "#eta_{#mu} ee#mu#nu;#eta #mu;Event count"},
    { "HEtal3_C", "#eta_{e} #mu#mue#nu;#eta e;Event count"},
    { "HEtal3_D", "#eta_{#mu3} #mu#mu#mu#nu;#eta #mu_{3};Event count"},
    { "HEtal3_+ABCD ", "Eta_{l2} lll#nu;Eta;Event count"},
    { "HPhil1_A", "#phi_{e1} eee#nu;#phi (rad);Event count"},
    { "HPhil1_B", "#phi_{e1} ee#mu#nu;#phi (rad);Event count"},
    { "HPhil1_C", "#phi_{#mu1} #mu#mue#nu;#phi (rad);Event count"},
    { "HPhil1_D", "#phi_{#mu1} #mu#mu#mu#nu;#phi (rad);Event count"},
    { "HPhil1_+ABCD", "#phi_{l1} lll#nu;#phi (rad);Event count"},
    { "HPhil2_A", "#phi_{e2} eee#nu;#phi (rad);Event count"},
    { "HPhil2_B", "#phi_{e2} ee#mu#nu;#phi (rad);Event count"},
    { "HPhil2_C", "#phi_{#mu2} #mu#mue#nu;#phi (rad);Event count"},
    { "HPhil2_D", "#phi_{#mu2} #mu#mu#mu#nu;#phi (rad);Event count"},
    { "HPhil2_+ABCD", "#phi_{l2} lll#nu;#phi (rad);Event count"},
    { "HPhil3_A", "#phi_{e3} eee#nu;#phi (rad);Event count"},
    { "HPhil3_B", "#phi_{#mu} ee#mu#nu;#phi (rad);Event count"},
    { "HPhil3_C", "#phi_{e} #mu#mue#nu;#phi (rad);Event count"},
    { "HPhil3_D", "#phi_{#mu3} #mu#mu#mu#nu;#phi (rad);Event count"},
    { "HPhil3_+ABCD", "#phi_{l3} lll#nu;#phi (rad);Event count"},
    { "HMetPhi_A", "#phi_{#slash{E}} eee#nu;#phi (rad);Event count"},
    { "HMetPhi_B", "#phi_{#slash{E}} ee#mu#nu;#phi (rad);Event count"},
    { "HMetPhi_C", "#phi_{#slash{E}} #mu#mue#nu;#phi (rad);Event count"},
    { "HMetPhi_D", "#phi_{#slash{E}} #mu#mu#mu#nu;#phi (rad);Event count"},
    { "HMetPhi_+ABCD", "#phi_{#slash{E}} lll#nu;#phi (rad);Event count"},
    { "HDxyl1_A","d_{xy} eee#nu ; e_{1} d_{xy} (cm);Event count"},
    { "HDxyl1_B","d_{xy} ee#mu#nu ; e_{1} d_{xy} (cm);Event count"},
    { "HDxyl1_C","d_{xy} #mu#mue#nu ; #mu_{1} d_{xy} (cm);Event count"},
    { "HDxyl1_D","d_{xy} #mu#mu#mu#nu ; #mu_{1} d_{xy} (cm);Event count"},
    { "HDxyl1_+ABCD","d_{xy} lll#nu ; l_{1} d_{xy} (cm);Event count"},
    { "HDxyl2_A","d_{xy} eee#nu ; e_{2} d_{xy} (cm);Event count"},
    { "HDxyl2_B","d_{xy} ee#mu#nu ; e_{2} d_{xy} (cm);Event count"},
    { "HDxyl2_C","d_{xy} #mu#mue#nu ; #mu_{2} d_{xy} (cm);Event count"},
    { "HDxyl2_D","d_{xy} #mu#mu#mu#nu ; #mu_{2} d_{xy} (cm);Event count"},
    { "HDxyl2_+ABCD","d_{xy} lll#nu ; l_{2} d_{xy} (cm);Event count"},
    { "HDxyl3_A","d_{xy} eee#nu ; e_{3} d_{xy} (cm);Event count"},
    { "HDxyl3_B","d_{xy} ee#mu#nu ; #mu_{3} d_{xy} (cm);Event count"},
    { "HDxyl3_C","d_{xy} #mu#mue#nu ; e_{3} d_{xy} (cm);Event count"},
    { "HDxyl3_D","d_{xy} #mu#mu#mu#nu ; #mu_{3} d_{xy} (cm);Event count"},
    { "HDxyl3_+ABCD","d_{xy} lll#nu ; l_{3} d_{xy} (cm);Event count"},
    { "HDzl1_A","d_{z} eee#nu; e_{1} d_{z} (cm);Event count"},
    { "HDzl1_B","d_{z} ee#mu#nu; e_{1} d_{z} (cm);Event count"},
    { "HDzl1_C","d_{z} #mu#mue#nu; #mu_{1} d_{z} (cm);Event count"},
    { "HDzl1_D","d_{z} #mu#mu#mu#nu; #mu_{1} d_{z} (cm);Event count"},
    { "HDzl1_+ABCD","d_{z} lll#nu; l_{1} d_{z} (cm);Event count"},
    { "HDzl2_A","d_{z} eee#nu; e_{2} d_{z} (cm);Event count"},
    { "HDzl2_B","d_{z} ee#mu#nu; e_{2} d_{z} (cm);Event count"},
    { "HDzl2_C","d_{z} #mu#mue#nu; #mu_{2} d_{z} (cm);Event count"},
    { "HDzl2_D","d_{z} #mu#mu#mu#nu; #mu_{2} d_{z} (cm);Event count"},
    { "HDzl2_+ABCD","d_{z} lll#nu; l_{2} d_{z} (cm);Event count"},
    { "HDzl3_A","d_{z} eee#nu; e_{1} d_{z} (cm);Event count"},
    { "HDzl3_B","d_{z} ee#mu#nu; #mu_{1} d_{z} (cm);Event count"},
    { "HDzl3_C","d_{z} #mu#mue#nu; e_{1} d_{z} (cm);Event count"},
    { "HDzl3_D","d_{z} #mu#mu#mu#nu; #mu_{1} d_{z} (cm);Event count"},
    { "HDzl3_+ABCD","d_{z} lll#nu; l_{1} d_{z} (cm);Event count"},
    { "HRelIsol1_A","RelIso eee#nu; RelIso e_{1}; Event count"},
    { "HRelIsol1_B","RelIso ee#mu#nu; RelIso e_{1}; Event count"},
    { "HRelIsol1_C","RelIso #mu#mue#nu; RelIso #mu_{1}; Event count"},
    { "HRelIsol1_D","RelIso #mu#mu#mu#nu; RelIso #mu_{1}; Event count"},
    { "HRelIsol1_+ABCD","RelIso lll#nu; RelIso l_{1}; Event count"},
    { "HRelIsol2_A","RelIso eee#nu; RelIso e_{2}; Event count"},
    { "HRelIsol2_B","RelIso ee#mu#nu; RelIso e_{2}; Event count"},
    { "HRelIsol2_C","RelIso #mu#mue#nu; RelIso #mu_{2}; Event count"},
    { "HRelIsol2_D","RelIso #mu#mu#mu#nu; RelIso #mu_{2}; Event count"},
    { "HRelIsol2_+ABCD","RelIso lll#nu; RelIso l_{2}; Event count"},
    { "HRelIsol3_A","RelIso eee#nu; RelIso e_{3}; Event count"},
    { "HRelIsol3_B","RelIso ee#mu#nu; RelIso #mu; Event count"},
    { "HRelIsol3_C","RelIso #mu#mue#nu; RelIso e; Event count"},
    { "HRelIsol3_D","RelIso #mu#mu#mu#nu; RelIso #mu_{2}; Event count"},
    { "HRelIsol3_+ABCD","RelIso lll#nu; RelIso l_{2}; Event count"},
    { "HElFakeCat_A","eee#nu;-1:R&P. -2:R&U. 1:F&P. 2:F&U. (R)eal,(F)ake,(P)aired,(U)npaired; Event count"},
    { "HElFakeCat_B","ee#mu#nu;-1:R&P. -2:R&U. 1:F&P. 2:F&U. (R)eal,(F)ake,(P)aired,(U)npaired; Event count"},
    { "HElFakeCat_C","#mu#mue#nu;-1:R&P. -2:R&U. 1:F&P. 2:F&U. (R)eal,(F)ake,(P)aired,(U)npaired; Event count"},
    { "HElFakeCat_D","#mu#mu#mu#nu;-1:R&P. -2:R&U. 1:F&P. 2:F&U. (R)eal,(F)ake,(P)aired,(U)npaired; Event count"},
    { "HMuFakeCat_A","eee#nu;-1:R&P. -2:R&U. 1:F&P. 2:F&U. (R)eal,(F)ake,(P)aired,(U)npaired; Event count"},
    { "HMuFakeCat_B","ee#mu#nu;-1:R&P. -2:R&U. 1:F&P. 2:F&U. (R)eal,(F)ake,(P)aired,(U)npaired; Event count"},
    { "HMuFakeCat_C","#mu#mue#nu;-1:R&P. -2:R&U. 1:F&P. 2:F&U. (R)eal,(F)ake,(P)aired,(U)npaired; Event count"},
    { "HMuFakeCat_D","#mu#mu#mu#nu;-1:R&P. -2:R&U. 1:F&P. 2:F&U. (R)eal,(F)ake,(P)aired,(U)npaired; Event count"},
    { "HFakeString_A","eee#nu;; Lepton count" },
    { "HFakeString_B","ee#mu#nu;; Lepton count" },
    { "HFakeString_C","#mu#mue#nu;; Lepton count" },
    { "HFakeString_D","#mu#mu#mu#nu;; Lepton count" },
    { "HIP3Dl1_A","eee#nu;IP3D_{e1} (cm); Event count" },
    { "HIP3Dl1_B","ee#mu#nu;IP3D_{e1} (cm); Event count" },
    { "HIP3Dl1_C","#mu#mue#nu;IP3D_{#mu1} (cm); Event count" },
    { "HIP3Dl1_D","#mu#mu#mu#nu;IP3D_{#mu1} (cm); Event count" },
    { "HIP3Dl2_A","eee#nu;IP3D_{e2} (cm); Event count" },
    { "HIP3Dl2_B","ee#mu#nu;IP3D_{e2} (cm); Event count" },
    { "HIP3Dl2_C","#mu#mue#nu;IP3D_{#mu2} (cm); Event count" },
    { "HIP3Dl2_D","#mu#mu#mu#nu;IP3D_{#mu2} (cm); Event count" },
    { "HIP3Dl3_A","eee#nu;IP3D_{e3} (cm); Event count" },
    { "HIP3Dl3_B","ee#mu#nu;IP3D_{#mu} (cm); Event count" },
    { "HIP3Dl3_C","#mu#mue#nu;IP3D_{e} (cm); Event count" },
    { "HIP3Dl3_D","#mu#mu#mu#nu;IP3D_{#mu3} (cm); Event count" },
    { "HPResidual_A","|#eta| < 0.9 1 Trk + 1 Glb;(1/p-1/p^{gen})/(1/p^{gen});Event count" },
    { "HPResidual_B","|#eta| < 0.9 2 Glb;(1/p-1/p^{gen})/(1/p^{gen});Event count" },
    { "HPResidual_C","0.9<|eta|<1.2 1 Trk + 1Glb;(1/p-1/p^{gen})/(1/p^{gen});Event count" },
    { "HPResidual_D","0.9<|eta|<1.2 2Glb;(1/p-1/p^{gen})/(1/p^{gen});Event count" },
    { "HPResidual_E","|eta|>1.2 1 Trk + 1Glb;(1/p-1/p^{gen})/(1/p^{gen});Event count" },
    { "HPResidual_F","|eta|>1.2 2Glb;(1/p-1/p^{gen})/(1/p^{gen});Event count" },
    { "HPtl1Lt_A","eee#nu;P_{t}^{l1}/P_{t}^{lep};Event count" },
    { "HPtl1Lt_B","ee#mu#nu;P_{t}^{l1}/P_{t}^{lep};Event count" },
    { "HPtl1Lt_C","#mu#mue#nu;P_{t}^{l1}/P_{t}^{lep};Event count" },
    { "HPtl1Lt_D","#mu#mu#mu#nu;P_{t}^{l1}/P_{t}^{lep};Event count" },
    { "HPtl2Lt_A","eee#nu;P_{t}^{l2}/P_{t}^{lep};Event count" },
    { "HPtl2Lt_B","ee#mu#nu;P_{t}^{l2}/P_{t}^{lep};Event count" },
    { "HPtl2Lt_C","#mu#mue#nu;P_{t}^{l2}/P_{t}^{lep};Event count" },
    { "HPtl2Lt_D","#mu#mu#mu#nu;P_{t}^{l2}/P_{t}^{lep};Event count" },
    { "HPtl3Lt_A","eee#nu;P_{t}^{l3}/P_{t}^{lep};Event count" },
    { "HPtl3Lt_B","ee#mu#nu;P_{t}^{l3}/P_{t}^{lep};Event count" },
    { "HPtl3Lt_C","#mu#mue#nu;P_{t}^{l3}/P_{t}^{lep};Event count" },
    { "HPtl3Lt_D","#mu#mu#mu#nu;P_{t}^{l3}/P_{t}^{lep};Event count" },
    { "HMetPtLt_A","eee#nu;P_{t}^{MET}/P_{t}^{lep};Event count" },
    { "HMetPtLt_B","ee#mu#nu;P_{t}^{MET}/P_{t}^{lep};Event count" },
    { "HMetPtLt_C","#mu#mue#nu;P_{t}^{MET}/P_{t}^{lep};Event count" },
    { "HMetPtLt_D","#mu#mu#mu#nu;P_{t}^{MET}/P_{t}^{lep};Event count" },
    { "HPtl3Met_A","eee#nu;P_{t}^{MET}/P_{t}^{l3};Event count" },
    { "HPtl3Met_B","ee#mu#nu;P_{t}^{MET}/P_{t}^{l3};Event count" },
    { "HPtl3Met_C","#mu#mue#nu;P_{t}^{MET}/P_{t}^{l3};Event count" },
    { "HPtl3Met_D","#mu#mu#mu#nu;P_{t}^{MET}/P_{t}^{l3};Event count" },
    { "HCosl3Met_A","eee#nu;cos(#phi_{l3}-#phi_{MET});Event count" },
    { "HCosl3Met_B","ee#mu#nu;cos(#phi_{l3}-#phi_{MET});Event count" },
    { "HCosl3Met_C","#mu#mue#nu;cos(#phi_{l3}-#phi_{MET});Event count" },
    { "HCosl3Met_D","#mu#mu#mu#nu;cos(#phi_{l3}-#phi_{MET});Event count" },
    { "HMuonPtDiff_B","ee#mu#nu;(tunePPt - PFPt)/PFPt;Muon count"},
    { "HMuonPtDiff_C","#mu#mue#nu;(tunePPt - PFPt)/PFPt;Muon count"},
    { "HMuonPtDiff_D","#mu#mu#mu#nu;(tunePPt - PFPt)/PFPt;Muon count"},
  };

  std::vector<std::string> HistNames = {
    /* Another series */
    "HDistl1l2",
    "HDistl1l3",
    "HDistl2l3",
    "HWZDist",
    /* Another series */
    "HnEl",
    "HnMu",
    "HnJet",
    "HnbTag",
    /* Another series */
    "HMassW",
    "HMassZ",
    "HMassTW",
    "HMassWZ",
    /* Another series */
    "HPtl1",
    "HPtl2",
    "HPtl3",
    "HMetPt",
    /* Another series */
    "HPtl1",
    "HMetPt",
    "HLt",
    "HWZPt",
    /* Another series */
    "HPileup",
    "HEtal1",
    "HEtal2",
    "HEtal3",
    /* Another series */
    "HPhil1",
    "HPhil2",
    "HPhil3",
    "HMetPhi",
    /* Another series */
    "HDxyl1",
    "HDxyl2",
    "HDxyl3",
    "HDxyl1",
    /* Another series */
    "HDzl1",
    "HDzl2",
    "HDzl3",
    "HDzl1",
    /* Another series */
    "HRelIsol1",
    "HRelIsol2",
    "HRelIsol3",
    "HRelIsol1",
  };

  struct HistoInfo {
    std::string name;
    std::string title;
  };



  auto getErrorHisto = [](THStack* hst){
    TList* lst = hst->GetHists();
    auto h1 = static_cast<TH1F*>((hst->GetStack()->Last())->Clone());
    /* Empty It */
    for(uint i = 0; i < h1->GetNbinsX(); i++){
      h1->SetBinContent(i,0.);
    }
    TIter next(lst);
    while(auto h = static_cast<TH1*>(next())){
      h1->Add(h);
    }
    h1->SetFillStyle(3004);
    h1->SetFillColor(kBlack);
    return h1;
  };

  auto normalizeHisto = [](TH1* h1){
    float_t maxBinContent= h1->GetBinContent(h1->GetMaximumBin());
    h1->Scale(1./maxBinContent);
  };

  auto getMaxY = [](TH1* h){
    return h->GetBinContent(h->GetMaximumBin());
  };

  auto fixYRange = [&](THStack* hss, Double_t maxY = -1.){
    const Float_t LegendSpace = 1.8; // Log Scale ;/
    if ( maxY<0 ) {
      TH1 *last = (TH1*)hss->GetStack()->Last();
      maxY = getMaxY(last);
    }
    hss->SetMaximum(maxY * LegendSpace);
    std::cout << hss->GetHistogram()->GetName() << "\n";
    // hss->SetMaximum(3500.);
    // if(std::string(hss->GetName()).find("_A") != std::string::npos){
    //   hss->SetMaximum(800.);
    // } else if(std::string(hss->GetName()).find("_B") != std::string::npos) {
    //   hss->SetMaximum(2000.);
    // } else if(std::string(hss->GetName()).find("_C") != std::string::npos) {
    //   hss->SetMaximum(1800.);
    // } else if(std::string(hss->GetName()).find("_D") != std::string::npos) {
    //   hss->SetMaximum(3800.);
    // }
  };

  auto sortStack = [](THStack* hss){
    THStack* hst  = new THStack();

    TIter b = hss->begin();

    std::map<Int_t,std::vector<TH1F*>> m;

    while(b.Next()){
      auto histo = static_cast<TH1F*>(*b);
      auto it = m.find(histo->GetFillColor());
      if( it != m.end()){
        (it->second).emplace_back(histo);
      } else {
        std::vector<TH1F*> vh;
        vh.push_back(histo);
        m.insert({histo->GetFillColor(),vh});
      }
    }

    for(auto const& ckey: m){
      for(auto const& histo: ckey.second){
        hst->Add(histo);
      }
    }

    return hst;
  };


  auto getRatio = [](TH1* hhdata, THStack *hss){
      auto hcdata = static_cast<TH1*>(hhdata->Clone());
      auto hcmc = static_cast<TH1*>(hss->GetStack()->Last());
      auto hratio = new TGraphAsymmErrors(hcdata,hcmc,"pois");
      // for(uint i = 0; i < hratio->GetN(); i++){
      //   hratio->SetPointEXhigh(i,0);
      //   hratio->SetPointEXlow(i,0);
      // }
      hratio->SetMarkerColor(kBlack);
      hratio->SetMarkerStyle(20);
      hratio->SetMarkerSize(.5);
      hratio->SetLineWidth(1);
      const int font = 43;
      const float fontSize = 12.;
      const float labelSize = 0.14;
      double xmin, xmax, ymin, ymax;
      hratio->ComputeRange(xmin,ymin,xmax,ymax);
      hratio->SetTitle("");
      hratio->GetXaxis()->SetTitle(hss->GetHistogram()->GetXaxis()->GetTitle());
      hratio->GetXaxis()->SetTitleFont(font);
      hratio->GetXaxis()->SetTitleSize(fontSize);
      hratio->GetXaxis()->SetLabelSize(labelSize);
      hratio->GetXaxis()->SetTitleOffset(12.0);
      hratio->GetYaxis()->SetTitleFont(font);
      hratio->GetYaxis()->SetTitleSize(fontSize);
      hratio->GetYaxis()->SetLabelSize(labelSize);
      hratio->GetYaxis()->SetTitleOffset(4.0);
      hratio->GetYaxis()->SetNdivisions(6,3,0);
      hratio->GetYaxis()->SetTitle("Data/MC");
      //hratio->SetMinimum(ymin);
      //hratio->SetMaximum(ymax);
      hratio->GetYaxis()->SetRangeUser(0.1,3.);
      hratio->SetFillColor(2);
      hratio->SetFillStyle(3001);
      hratio->SetDrawOption("ap");
      return hratio;
  };

  std::function<double(const std::string&, std::string)> getCutCount = [&] (const std::string& folder, std::string cutLabel){
    double count = 0;

    // Accepts strings like "+a+b+c-d-e-f" not like "+a-b-c+d-e+h"

    while (cutLabel.find("-") != std::string::npos) {
      std::string lastLabel = cutLabel.substr(cutLabel.find_last_of("-")+1,cutLabel.length());
      count -= getCutCount(folder,lastLabel);
      cutLabel = cutLabel.substr(0, cutLabel.find_last_of("-"));

    }

    while (cutLabel.find("+") != std::string::npos){
      std::string lastLabel = cutLabel.substr(cutLabel.find_last_of("+")+1,cutLabel.length());
      count += getCutCount(folder,lastLabel);
      cutLabel = cutLabel.substr(0, cutLabel.find_last_of("+"));
    }

    TH1F* hCutFlow = static_cast<TH1F*>(f1->Get(Form("%s/HCutFlow",folder.c_str())));
    count += hCutFlow->GetBinContent(hCutFlow->GetXaxis()->FindBin(cutLabel.c_str( )));

    return count;
  };

  auto applyLumiSF = [&](TH1* h, std::string folder, const Float_t& xsec){
    Int_t yr = std::stoi(folder.substr(0,folder.rfind("/")+1));
    Double_t nEvents = getCutCount(folder,"genWeight");
    Double_t lumiSF = luminosity[yr]*xsec*1e3/nEvents; /* pico*femto^-1=1e-12*1e15=1e3 */
    h->Scale(lumiSF);
  };

  std::function<TH1*(std::string,std::string)> getHistoFromFile = [&](std::string folder, std::string hName){
    std::string hpath = Form("%s/%s",folder.c_str(),hName.c_str());
    std::clog << "Getting Histo From File: " << hpath << std::endl;

    if(hName.find("_+") != std::string::npos){
      std::vector<std::string> chn;
      chn.reserve(6);
      TH1* hAll = nullptr;
      std::string postfix = "WCentral";
      for (const auto ch: hName.substr(hName.find("_+")+2)) {
        if ( ch == std::string("_") ) break;
        chn.emplace_back(Form("%c_%s",ch,postfix.c_str()));
      }
      for(const auto& ch: chn){
        std::string hn = hName.substr(0, hName.find("_+") + 1);
        std::string hln = hName.substr(hName.find("_+") + 2 + chn.size());
        std::string fhn = hn + ch + hln;
        if(!hAll){
          hAll = static_cast<TH1*>(getHistoFromFile(folder,fhn));
        } else {
          hAll->Add(static_cast<TH1*>(getHistoFromFile(folder,fhn)));
        }
      }
      return hAll;
    } else if (hName.find("+H") != std::string::npos) {
      std::string hn = hName.substr(0,hName.find("+"));
      std::string hnew = hName.substr(hn.size()+1);
      TH1* hAll = getHistoFromFile(folder,hn);
      hAll->Add(static_cast<TH1*>(getHistoFromFile(folder,hnew)));
      return hAll;
    }
    auto h = static_cast<TH1*>(f1->Get(hpath.c_str()));
    if(h) h = static_cast<TH1*>(h->Clone());
    return h;
  };

  std::function<TH1*(std::string,std::string)> getDataStack =
    [&](std::string folder, std::string hName, std::string divider = "UL"){
    std::clog << Form("Building Data Stack %s\n",folder.c_str());
    std::string allSamples = folder.substr(folder.rfind("/")+1,folder.length());
    std::string newSamples = allSamples;
    std::vector<std::string> samples;
    while(newSamples.rfind(divider) != std::string::npos){
      Int_t last = newSamples.rfind(divider);
      std::string tmp = newSamples.substr(last,newSamples.length());
      std::cout << tmp << "\n";
      samples.emplace_back(tmp);
      newSamples = newSamples.substr(0,last);
    }
    assert(samples.size()>0);
    std::string syr = folder.substr(0,folder.rfind("/")+1);
    TH1F* h =
    static_cast<TH1F*>(getHistoFromFile(syr+samples[0],hName)->Clone());
    h->Add(getHistoFromFile(syr+samples[1],hName));
    if(samples.size()>2) h->Add(getHistoFromFile(syr+samples[2],hName));
    return h;
  };


  std::function<void(const std::string)> diffSampleYear = [&] (const std::string& histoName) {

    TCanvas* c =  new TCanvas("c","c",1200,1200);
    TPad* p1 = new TPad("p1","p1",0.0,0.2,0.5,1.); p1->Draw();
    TPad* p2 = new TPad("p2","p2",0.5,0.2,1.,1.); p2->Draw();
    TPad* p3 = new TPad("p3","p3",0.0,0.0,1.0,0.2); p3->Draw();

    std::vector<int> years = { 2016, 2017, 2018 };

    auto getSampleName = [&] (const std::string& legend, const int& yy) {
      for (auto BGN: BgNames[yy]) {
        if (BGN.legendName == legend) return BGN.folderName;
      }
      return std::string("");
    };

    for (auto BGN: BgNames[years[0]]) {
      THStack* hs = new THStack("hs",histoName.c_str());
      TLegend* l = new TLegend(0,0,1,1);
      TH1F* hn = new TH1F("hn","nEvents",3,0.,3.);
      TH1F* hgw = new TH1F("hgw","genWeight",3,0.,3.);
      std::string category = BGN.legendName;
      for(const auto& yy: years){
        std::string sampleName = getSampleName(category,yy);
        if (sampleName.size()==0) continue;
        std::clog << Form("** Printing: %d: %s", yy, sampleName.c_str()) << std::endl;
        std::string folder = Form("%d/%s",yy,sampleName.c_str());
        TH1* h = static_cast<TH1*>(getHistoFromFile(folder,histoName)->Clone());
        applyLumiSF(h, folder, BGN.xsec);
        h->SetLineColor(yy%2015);
        h->SetMarkerColor(yy%2015);
        hn->Fill(Form("%d",yy), getCutCount(Form("%d/%s",yy,sampleName.c_str()),"NoCuts"));
        hgw->Fill(Form("%d",yy), getCutCount(Form("%d/%s",yy,sampleName.c_str()),"genWeight"));
        h->SetTitle(Form("%d: %s",yy,sampleName.c_str()));
        h->SetMarkerStyle(28 + (yy%2016));
        hs->Add(h,"HIST PL");
        l->AddEntry(h);
      }
      ((TH1*)hs->GetStack()->Last())->GetXaxis()->SetRangeUser(0.,2000.);
      p1->cd();
      hs->Draw("NOSTACK");
      p2->cd();
      hn->SetLineColor(kBlack);
      hn->Draw("HIST TEXT45");
      hgw->SetLineColor(kRed);
      hgw->Draw("HIST SAME");
      p3->cd();
      l->SetTextFont(42);
      l->Draw();
      c->Print(Form("plots/%s_SampleDiff_%s_%s.png",fileLabel.c_str(),category.c_str(),histoName.c_str()));
      hgw->Delete();
      hn->Delete();
      delete hs;
    }

    delete c;
  };


  std::set<std::string> MissingSamples;
  auto getBGStack = [&](int yr, std::string hname, TLegend* legend = NULL){
    THStack* hstck = new THStack();
    Int_t prevColor = -1;
    for (auto BGN: BgNames[yr]) {
      auto h = getHistoFromFile(Form("%d/%s",yr,BGN.folderName.c_str()),hname);
      if(!h){
        MissingSamples.insert(Form("%d/%s",yr,BGN.folderName.c_str()));
        continue;
      }
      applyLumiSF(h, Form("%d/%s",yr,BGN.folderName.c_str()), BGN.xsec);
      if(h->Integral() < 1e-6){
        std::cout << "\n\n*****************\n*****WARNING*****\n*****************\n"
                  << "\t Integral for " << BGN.folderName << " is " << h->Integral()
                  << "\n*****************\n";
      }
      std::cout << h->GetName() << "\t" << h->Integral() << "\n";
      h->SetFillStyle(1001);
      h->SetTitle(BGN.legendName.c_str());
      h->SetFillColor(BGN.color);
      h->SetLineWidth(0);
      hstck->Add(h);
      if(legend and (prevColor != BGN.color))
        legend->AddEntry(h,BGN.legendName.c_str(),"F");
      prevColor = BGN.color;
    }
    return hstck;
  };


  auto addCutEff = [&] (TGraph* g, const std::string& sample,
                       const char* cutLabel, const char* totalLabel, const float& sampleMass) {
    Double_t nPass = getCutCount(sample.c_str(),cutLabel);
    Double_t nEvents = getCutCount(sample.c_str(),totalLabel);
    g->SetPoint(g->GetN(), sampleMass, nPass/nEvents);
  };


  std::function<void(const int&, const SignalInfo&)> printCutFlowStack = [&] (const int& year, const SignalInfo& signal){
    auto c2 = new TCanvas("c2","c2");
    THStack *hstc = new THStack("hstc","");
    auto legend = new TLegend();
    legend->SetBorderSize(0);

    auto fillCutH = [&] (TH1* h, std::string& folder){
      h->Fill("nEvents", 1.);
      double pSel = getCutCount(folder,"+NoCuts-FailHLT-FailFlags");
      h->Fill("HLTs & Flags",pSel/getCutCount(folder,"NoCuts"));
      pSel -= getCutCount(folder,"+MET_pt<30+lep<3+goodLep<3");
      h->Fill("PreSelection",pSel/getCutCount(folder,"NoCuts"));
      double zSel = pSel - getCutCount(folder,"+NoPairs+FailZMassWindow");
      h->Fill("ZSelection",zSel/getCutCount(folder,"NoCuts"));
      double wSel =  zSel - getCutCount(folder,"+FailZl1PtCut+FailZDistCut");
      h->Fill("WSelection",wSel/getCutCount(folder,"NoCuts"));
      double wzSel = wSel - getCutCount(folder,"+FailZHighPtIdCut+Faill1l3DistCut+Faill2l3DistCut");
      h->Fill("WZSelection",wzSel/getCutCount(folder,"NoCuts"));
      double nChannel = getCutCount(folder,"+0e3mu+1e2mu+2e1mu+3e0mu");
      h->Fill("InChannel",nChannel/getCutCount(folder,"NoCuts"));
      h->LabelsDeflate();
    };

    for(const auto& BGN: (BgNames[year])){
      TH1D* HTCutFlow = new TH1D(Form("HTCF_%d_%s",year,BGN.folderName.c_str()),"HT",10,0., 10.);
      std::string folder = Form("%d/%s",year,BGN.folderName.c_str());
      fillCutH(HTCutFlow,folder);
      HTCutFlow->SetMarkerColor(BGN.color);
      HTCutFlow->SetMarkerStyle(4);
      HTCutFlow->SetLineWidth(0);
      hstc->Add(HTCutFlow);
      legend->AddEntry(HTCutFlow,BGN.legendName.c_str(),"P");
    }

    TH1D* HTCutFlow = new TH1D(Form("HTCF_%d_%s",year,signal.folderName.c_str()),"HT",10,0., 10.);
    std::string folder = Form("%d/%s",year,signal.folderName.c_str());
    fillCutH(HTCutFlow,folder);
    HTCutFlow->SetMarkerColor(kBlack);
    HTCutFlow->SetMarkerStyle(22);
    HTCutFlow->SetLineWidth(0);
    hstc->Add(HTCutFlow);
    legend->AddEntry(HTCutFlow,signal.legendName.c_str(),"P");

    c2->SetLogy();
    hstc->SetTitle("Ratio [%] of events passing set of cuts");
    hstc->Draw("NOSTACK");
    legend->Draw();
    c2->Print(Form("plots/%d/%s_%s_HTCutFlow.png",year,fileLabel.c_str(),signal.folderName.c_str()));
    delete HTCutFlow;
    delete c2;
  };

  std::function<void(const int&, const SignalInfo&, const std::string&)>
    printH2Comb = [&] (const int& yr, const SignalInfo& signal,
                     const std::string& histoLabel){
    TCanvas* c2 = new TCanvas("c2","c2");
    TH2F* h2BgSum = nullptr;
    for(const auto& BGN: (BgNames[yr])){
      std::string folder = Form("%d/%s",yr,BGN.folderName.c_str());
      auto h2 = static_cast<TH2F*>(getHistoFromFile(folder.c_str(),histoLabel.c_str()));
      if(!h2BgSum) {
        h2BgSum = static_cast<TH2F*>(h2->Clone());
      } else {
        h2BgSum->Add(static_cast<TH2F*>(h2->Clone()));
      }
    }
    h2BgSum->SetMarkerColor(kRed);
    TH2F* h2Sig = static_cast<TH2F*>(getHistoFromFile(Form("%d/%s",yr,signal.folderName.c_str()),histoLabel));
    h2Sig->SetMarkerColor(kGreen);
    TH2F* h2Data = static_cast<TH2F*>(getHistoFromFile(Form("%d/%s",yr,DataSampleNames[yr].c_str()),histoLabel));
    h2Data->SetMarkerColor(kBlack);
    c2->Divide(2,1);
    c2->cd(1);
    h2BgSum->Draw("");
    h2Data->Draw("SAME");
    h2Sig->Draw("SAME");
    c2->cd(2);
    h2BgSum->Draw("");
    h2Data->Draw("SAME");
    c2->Print(Form("plots/%d/%s_%s_%s.png",yr,fileLabel.c_str(),signal.folderName.c_str(),histoLabel.c_str()));
    delete c2;
  };

  std::function<std::map<std::string,double>(THStack*,TH1*)>
    printYieldsTable = [] (THStack* hss, TH1* hdata) {

    std::cout << "%%%% Yields Table %%%%\n";
    std::map<std::string, double> table;

    TList* lst = hss->GetHists();
    TIter next(lst);

    std::string prevLabel = "NULL";

    while(auto h = static_cast<TH1*>(next())){
      table[std::string(h->GetTitle())] += h->Integral();
    }

    table["Data"] = hdata->Integral();

    for(const auto& row : table) {
      std::cout << row.first << "\t" << row.second << "\n";
    }
    std::cout << "%%%%%%%%%%%%%%%%%%%%%%\n";

    return table;

  };

  

  std::function<void(const int&, const int&,
                     const std::pair<std::string,std::string>&, const char*)>
    printDataCard = [&] (const int& year, const int& wpmass,
                         const std::pair<std::string,std::string>& ch,
                         const char* fromHisto = "HMassWZ_SR1") {

    std::clog << Form("Printing datacard: Year[%d], WpMass[%d]\n",year,wpmass);



    ofstream dcFile;
    dcFile.open(Form("DataCard_%s_%s_%d_%d.txt",ch.second.c_str(),fromHisto,year, wpmass));

    std::unordered_map<Int_t, Float_t> lumiSyst = {
      // https://twiki.cern.ch/twiki/bin/viewauth/CMS/TWikiLUM
      { 2016, 0.012 },
      { 2017, 0.023 },
      { 2018, 0.025 },
    };

    std::string rootFilename = Form("CombineFile_%s_%d_%d.root",fromHisto,year, wpmass);
    TFile* fCombine = TFile::Open(rootFilename.c_str(),"UPDATE");

    std::string DYSample = "DYJetsToLL";
    std::string GluGluSample = "GluGluToContinToZZTo";

    std::string bin0 = "bin\t";
    std::string obs  = "observation\t";
    std::string bin1 = "bin\t";
    std::string processn = "process\t";
    std::string process  = "process\t";
    std::string rate     = "rate\t";

    std::vector<std::string> systEl     = { "ElReco" , "ElTrigger","ElID"};
    std::vector<std::string> systMu     = { "MuTrigger","MuID" };
    std::vector<std::string> systK      = { "KFactorEWK","KFactorQCD" };
    std::vector<std::string> systShared = { "Pileup", "MetUncl", "L1Pref"};

    std::map<std::string,std::string> uncs = {
      { "Lumi"       , Form("lumi_13TeV%d\t\tlnN\t",year)       },
      { "ElTrigger"  , Form("CMS_ElTrigger%d\t\tshape\t",year)  },
      { "MuTrigger"  , Form("CMS_MuTrigger%d\t\tshape\t",year)  },
      { "ElID"       , Form("CMS_ElID%d\t\tshape\t",year)       },
      { "MuID"       , Form("CMS_MuID%d\t\tshape\t",year)       },
      { "KFactorQCD" , Form("CMS_KFactorQCD%d\t\tshape\t",year) },
      { "KFactorEWK" , Form("CMS_KFactorEWK%d\t\tshape\t",year) },
      { "MetUncl"    , Form("CMS_MetUncl%d\t\tshape\t",year)    },
      { "ElReco"     , Form("CMS_ElReco%d\t\tshape\t",year)     },
      { "Pileup"     , Form("CMS_Pileup%d\t\tshape\t",year)     },
      { "L1Pref"     , Form("CMS_L1Pref%d\t\tshape\t",year)     },
    };



    for(const auto& sample : BgNames[year]){
      if(sample.folderName.find(GluGluSample) != std::string::npos) continue;
      uncs["LHEPdf_"+sample.folderName] =
      Form("CMS_LHEPdf%s%d\t\tshape\t",sample.shortName.c_str(),year);
      uncs["LHEScale_"+sample.folderName] =
      Form("CMS_LHEScale%s%d\t\tshape\t",sample.shortName.c_str(),year);
    }

    std::function<TH1*(TH1*)> stripNegativeBins = [] (TH1* hneg) {
      TH1* hzero = static_cast<TH1*>(hneg->Clone());
      for(int i = 0; i <= hzero->GetNbinsX() ; ++i){
        double priorValue = hzero->GetBinContent(i);
        if( priorValue < 0. ){
          hzero->SetBinContent(i, 0.);
          hzero->SetBinError(i, 0.);
          std::clog << Form("***saveHisto: Negative value found: %s\tBinNumber: %d\t%.4f\n",
                            hzero->GetName(), i, priorValue);
        }
      }
      return hzero;
    };

    std::function<void(const std::string&,const std::string&, const std::string&, const float&)>
    saveHisto = [&] (const std::string& shortName,
                     const std::string& sampleName,
                     const std::string& ch_,
                     const float& xsec) {

      std::function<float(const std::string&,
                          const std::string&,
                          const std::string&)>
      saveUpDown = [&] (std::string folder, std::string hname, std::string s/*ystematic*/) -> float {
        if( (s.find("L1Pref") != std::string::npos) and
            (year == 2018) ) return 0.; //L1Pref only '16 and '17
        auto hup = stripNegativeBins(getHistoFromFile(folder.c_str(),Form("%s_%s_Up",hname.c_str(),s.c_str())));
        applyLumiSF(hup, folder.c_str(), xsec);
        if(hup->Integral() == 0.) std::clog << Form("\n\tWARNING: %s Up Systematic is 0.: %s\n\n",s.c_str(),folder.c_str());
        hup->Write(
             std::string(
                sampleName+
                Form("_CMS_%s%s%dUp",
                     s.c_str(),
                     s.find("LHE") != std::string::npos ? shortName.c_str() : "",
                     year)
             ).c_str());
        auto hdown = stripNegativeBins(getHistoFromFile(folder.c_str(),Form("%s_%s_Down",hname.c_str(),s.c_str())));
        applyLumiSF(hdown, folder.c_str(), xsec);
        if(hdown->Integral() == 0.) std::clog << Form("\n\tWARNING: %s Down Systematic is 0.: %s\n\n",s.c_str(),folder.c_str());
        hdown->Write(
              std::string(
                 sampleName+
                 Form("_CMS_%s%s%dDown",
                      s.c_str(),
                      s.find("LHE") != std::string::npos ? shortName.c_str() : "",
                      year)
              ).c_str());
        return hup->Integral()*hdown->Integral(); // We just need to know if any of this is zero
      };

      std::string folderName = Form("%d/%s",year,sampleName.c_str());
      std::string hname;
      if(ch_ == "A") {
        for (auto s: systEl) {
          hname = Form("%s_%s",fromHisto,ch_.c_str());
          float rate = saveUpDown(folderName,hname,s);
          uncs[s] += rate > 0. ? "1.0\t" : "-\t";
        }
        for (auto s: systMu) {
          uncs[s] += "-\t";
        }
      } else if (ch_ == "B" or ch_ == "C") {
        for (auto s: systEl) {
          hname = Form("%s_%s",fromHisto,ch_.c_str());
          float rate = saveUpDown(folderName,hname,s);
          uncs[s] += rate > 0. ? "1.0\t" : "-\t";
        }
        for (auto s: systMu) {
          hname = Form("%s_%s",fromHisto,ch_.c_str());
          float rate = saveUpDown(folderName,hname,s);
          uncs[s] += rate > 0. ? "1.0\t" : "-\t";
        }
      } else if (ch_ == "D") {
        for (auto s: systEl) {
          uncs[s] += "-\t";
        }
        for (auto s: systMu) {
          hname = Form("%s_%s",fromHisto,ch_.c_str());
          float rate = saveUpDown(folderName,hname,s);
          uncs[s] += rate > 0. ? "1.0\t" : "-\t";
        }
      }

      for (auto s: systShared){
        hname = Form("%s_%s",fromHisto,ch_.c_str());
        float rate = saveUpDown(folderName,hname,s);
        uncs[s] += rate > 0. ? "1.0\t" : "-\t";
      }

      if(folderName.find(DYSample.c_str()) != std::string::npos){
        for (auto s: systK) {
          hname = Form("%s_%s",fromHisto,ch_.c_str());
          float rate = saveUpDown(folderName,hname,s);
          uncs[s] += rate > 0. ? "1.0\t" : "-\t";
        }
      } else {
        for (auto s: systK) {
          uncs[s] += "-\t";
        }
      }



      if (folderName.find(GluGluSample.c_str()) == std::string::npos){
        float ratePdf = saveUpDown(folderName,hname,"LHEPdf");
        float rateScale = saveUpDown(folderName,hname,"LHEScale");
        for(const auto&[key,value] : uncs){
          if(key.find("LHEPdf") == std::string::npos and
             key.find("LHEScale") == std::string::npos) continue;
          if(key.find("LHEPdf") != std::string::npos and
             key.find(sampleName) != std::string::npos) {
            uncs[key] += ratePdf > 0. ? "1.0\t" : "-\t";
          } else if (key.find("LHEPdf") != std::string::npos and
                     key.find(sampleName) == std::string::npos) {
            uncs[key] += "-\t";
          } else if (key.find("LHEScale") != std::string::npos and
                     key.find(sampleName) != std::string::npos) {
            uncs[key] += rateScale > 0. ? "1.0\t" : "-\t";
          } else if (key.find("LHEScale") != std::string::npos and
                     key.find(sampleName) == std::string::npos) {
            uncs[key] += "-\t";
          }
        }
      } else {
        for(const auto&[key,value] : uncs){
          if(key.find("LHEPdf") == std::string::npos and
             key.find("LHEScale") == std::string::npos) continue;
          uncs[key] += "-\t";
        }
      }
    };

      THStack* hss = getBGStack(year,Form("%s_%s_WCentral",fromHisto,ch.first.c_str()));
      float totalBGYield = ((TH1*)(hss->GetStack()->Last()))->Integral();

      fCombine->cd("/");
      fCombine->mkdir(ch.second.c_str());
      fCombine->cd(ch.second.c_str());

      SignalInfo signal = SignalSamples[year][SignalPos[wpmass]];
      TH1* hsig = getHistoFromFile(Form("%d/%s",year,signal.folderName.c_str()),Form("%s_%s_WCentral",fromHisto,ch.first.c_str()));
      applyLumiSF(hsig, Form("%d/%s",year,signal.folderName.c_str()), signal.xsec);
      hsig->Write(signal.folderName.c_str());
      uncs["Lumi"] += Form("%.4f\t",1. + lumiSyst[year]);
    saveHisto("",signal.folderName, ch.first, signal.xsec);

      bin1 += ch.second + "\t";
      processn += "0\t";
      //process += Form("Wprime%d\t",wpmass);
      std::cout << "SIGNALNAME?\t" << signal.folderName << "\n";
      process += Form("%s\t",signal.folderName.c_str());
      rate += Form("%.2f\t",hsig->Integral());

      auto hdata = getDataStack(Form("%d/%s",year,DataSampleNames[year].c_str()),Form("%s_%s_NoSF",fromHisto,ch.first.c_str()));
      bin0 += Form("%s\t",ch.second.c_str());
      obs += Form("%.2f\t",hdata->Integral());
      hdata->Write("data_obs");

      int counter = 1;
      for (auto BGN: BgNames[year]) {
        bin1 += ch.second + "\t";
        processn += Form("%d\t",counter);
        process += Form("%s\t",BGN.folderName.c_str());
        std::string fname = Form("%d/%s",year,BGN.folderName.c_str());
        auto h = stripNegativeBins(getHistoFromFile(fname.c_str(),Form("%s_%s_WCentral",fromHisto,ch.first.c_str())));
        applyLumiSF(h, Form("%d/%s",year,BGN.folderName.c_str()), BGN.xsec);
        rate += Form("%.2f\t",h->Integral());
        h->Write(BGN.folderName.c_str());
        const float contribTreshold = 0.03;
        const float contribFromSample = h->Integral()/totalBGYield;
        if ( contribFromSample > contribTreshold ) {
          uncs["Lumi"] += Form("%.4f\t",1. + lumiSyst[year]);
        } else {
          uncs["Lumi"] += "-\t";
          std::clog << Form("\tContribution from %s is %.4f%%: Excluding Lumi Systematics\n",BGN.folderName.c_str(),contribFromSample*100.); 
        }

        saveHisto(BGN.shortName,BGN.folderName.c_str(),ch.first,BGN.xsec);

        ++counter;
      }


    dcFile << "imax\t1\njmax\t" << BgNames[year].size() << "\nkmax\t*\n";
    dcFile << "------------\n";
    dcFile << Form("shapes * * %s $CHANNEL/$PROCESS $CHANNEL/$PROCESS_$SYSTEMATIC\n",rootFilename.c_str());
    dcFile << bin0 << std::endl;
    dcFile << obs << std::endl;
    dcFile << "------------\n";
    dcFile << bin1 << std::endl << process << std::endl << processn << std::endl << rate << std::endl;
    dcFile << "------------\n";

    for (const auto&[key, value]: uncs) {
      if(key.find("L1Pref") != std::string::npos) continue;
      dcFile << value << "\n";
    }

    if( year != 2018 ) {
      dcFile << uncs["L1Pref"] << std::endl;
    }

    fCombine->Close();

  };

  // std::vector<std::string> crr_ = { "HMassWZ_CR1", "HMassWZ_CR2", "HMassWZ_SR1" };
  // std::vector<int> yyyy = { 2016, 2017, 2018 };
  // for(auto y: yyyy) {
  //   //for (auto n: SignalPos) {
  //     for (const auto& ch: channels){
  //       for(const auto& cr : crr_){
  //         printDataCard(y,/*n.first*/600,ch,cr.c_str());
  //       }
  //     }
  //     //}
  // }
  // throw;

  std::function<TGraphAsymmErrors*(int, const std::string, const std::string, const std::string)>
    getUpDownErrorGraph = [&](int year, const std::string sample, const std::string ch, const std::string syst){
    auto hUp = (TH1*)f1->Get(Form("%d/%s/HMassWZ_SR1_%s_%s_Up",year,sample.c_str(),ch.c_str(),syst.c_str()));
    std::cout << Form("%d/%s/HMassWZ_SR1_%s_%s_Up",year,sample.c_str(),ch.c_str(),syst.c_str()) << std::endl;
    auto hDown = (TH1*)f1->Get(Form("%d/%s/HMassWZ_SR1_%s_%s_Down",year,sample.c_str(),ch.c_str(),syst.c_str()));
    Int_t nBins = hUp->GetNbinsX();
    auto g1 = new TGraphAsymmErrors(nBins);
    g1->SetName(Form("%d_%s_%s_%s",year,sample.c_str(),ch.c_str(),syst.c_str()));
    float logError = 0.f;
    for(uint i = 1; i < nBins; ++i){
      float xlow = hUp->GetBinLowEdge(i);
      float xhigh = xlow + hUp->GetBinWidth(i);
      float ymax = hUp->GetBinContent(i);
      float ymin = hDown->GetBinContent(i);
      if (ymin > ymax) std::swap(ymin,ymax);
      float xx = (xlow+xhigh)/2.;
      float yy = (ymax+ymin)/2.;
      xlow = xx - xlow;
      xhigh = xhigh - xx;
      ymax = ymax - yy;
      ymin = yy - ymin;
      logError += ymax;
      // std::cout << xx << "\t" << xlow << "\t" <<  xhigh << "\n";
      // std::cout << yy << "\t" << ymax << "\t" <<  ymin << "\n";
      g1->SetPoint(i,xx,yy);
      g1->SetPointError(i,xlow,xhigh,ymin,ymax);
    }
    g1->SetMarkerStyle(21);
    g1->SetFillColor(2);
    g1->SetFillStyle(3001);
    if(logError == 0.f and hUp->Integral() > 0.f and hDown->Integral() > 0.f)
      g1->SetMarkerColor(kRed);
    else
      g1->SetMarkerColor(kBlack);
    // auto c1 = new TCanvas("c1","c1");
    // g1->Draw("A2P");
    // c1->Print("tmp.png");
    return g1;
  };


  
  auto printPostFitPlots = [&](const int& yr, const string& cr, const string& ch,
                               const int& mass, const string fit = "postfit",
                               const string folder = "plots/PostFit"){

    auto file = TFile::Open(Form("%s/PostFit_%s_HMassWZ_%s_%d_%d.root",
                                 folder.c_str(),ch.c_str(),cr.c_str(),
                                 yr,mass));

    auto cc = new TCanvas(Form("can_%s_%s",ch.c_str(),cr.c_str()),"",1000,1000);

    auto mainPad = new TPad(Form("mainPad_%s",ch.c_str()),"mainPad",0.,0.25,1.,1.);
    mainPad->Draw();
    mainPad->SetLeftMargin(0.12);
    mainPad->SetRightMargin(0.12);
    //mainPad->SetLogy();
    mainPad->SetTickx();
    mainPad->SetTicky();
    auto subPad = new TPad(Form("subPad_%s_%s",ch.c_str(),cr.c_str()),"subPad",0.,0.,1.,0.25);
    subPad->Draw();
    subPad->SetTopMargin(0);
    subPad->SetLeftMargin(0.12);
    subPad->SetRightMargin(0.12);
    subPad->SetTopMargin(0);
    subPad->SetBottomMargin(0.5);

    mainPad->cd();

    std::vector<BackgroundInfo>& Bkgs = BgNames[yr];

    THStack* hs = new THStack(Form("hs%s",fit.c_str()),"HMassWZ");
    auto legend = new TLegend(0.3, 0.66, .87, .89);
    legend->SetNColumns(2);

    Int_t prevColor = -1;
    for(const auto& bkg: Bkgs){

      auto h =  dynamic_cast<TH1F*>(file->Get(Form("%s_%s/%s",
                                                   ch.c_str(),fit.c_str()
                                                   ,bkg.folderName.c_str())));
      if(h == nullptr) continue;
      h->SetFillStyle(1001);
      h->SetTitle(bkg.legendName.c_str());
      h->SetFillColor(bkg.color);
      h->SetLineWidth(0);
      hs->Add(h,"HIST PL");
      if(prevColor != bkg.color)
        legend->AddEntry(h,bkg.legendName.c_str(),"F");
      prevColor = bkg.color;
    }


    mainPad->cd();
    hs->SetTitle(Form("M_{WZ} %s ;M_{WZ}^{%s} (GeV);Event count (%s)",ch.c_str(),ch.c_str(),fit.c_str()));
    hs->Draw();

    auto cmsLabel = new TPaveText(0.11,0.93,0.3,1.0,"NDC");
    cmsLabel->SetFillColor(0);
    cmsLabel->SetBorderSize(0);
    cmsLabel->AddText("CMS Preliminary");
    cmsLabel->SetTextAlign(12);
    cmsLabel->Draw();

    auto lumiLabel = new TPaveText(0.6,0.93,0.89,1.0,"NDC");
    lumiLabel->SetFillColor(0);
    lumiLabel->SetBorderSize(0);
    lumiLabel->AddText(Form("#sqrt{s} = 13TeV L = %.2f fb^{-1}",luminosity[yr]));
    lumiLabel->SetTextAlign(12);
    lumiLabel->Draw();

    auto hdata = dynamic_cast<TH1F*>(file->Get(Form("%s_%s/data_obs", ch.c_str(),fit.c_str())));

    fixYRange(hs,getMaxY(hdata));
    hdata->SetMarkerStyle(kFullCircle);
    hdata->Draw("SAME P");

    double minx = hdata->GetBinLowEdge(hdata->FindFirstBinAbove(0.1));
    double maxx = hdata->GetBinLowEdge(hdata->FindLastBinAbove(0.1))+hdata->GetBinWidth(hdata->FindLastBinAbove(0.));
    hs->GetXaxis()->SetRangeUser(minx,maxx);
    legend->AddEntry(hdata, Form("Data %d",yr));
    legend->SetBorderSize(0);
    legend->Draw();


    auto hcdata = getRatio(hdata,hs);

    subPad->cd();
    subPad->SetGrid();
    hcdata->Draw("AP");
    hcdata->GetXaxis()->SetNdivisions(hs->GetHistogram()->GetXaxis()->GetNdivisions());
    hcdata->GetXaxis()->SetLimits(minx,maxx);
    hcdata->GetXaxis()->SetTitleOffset(1.8);
    //hcdata->SetTitle(Form(";%s;Data/MC",hs->GetHistogram()->GetXaxis()->GetTitle()));
    hcdata->Draw("AP");



    cc->SaveAs(Form("%s/%s_%s_HMassWZ_%s_%d_%d.png",
                    folder.c_str(),fit.c_str(),ch.c_str(),cr.c_str(),
                    yr,mass));
  };



  for(const auto& fitType: std::initializer_list<std::string>{ "prefit","postfit" }){
    for(const auto&[massp,pos]: SignalPos){
      if (massp != 600) continue;
      for(const auto&[yr,lumi]: luminosity){
        if (yr == 0) continue;
        for(const auto& cr: std::initializer_list<std::string>{ "CR1","CR2" }){
          for(const auto&[non,ch]: channels){
            printPostFitPlots(yr,cr,ch,massp,fitType);
          }
        }
      }
    }
  };
  throw;


  std::function<void(int, const std::string, const std::string)>
    printUpDown = [&](int year, const std::string sample, const std::string syst){

    auto c = new TCanvas("c","c",2000,2000);
    c->Divide(2,2);
    std::vector<std::string> chs = { "A","B","C","D" };
    uint counter = 1;
    for(auto ch: chs){
      c->cd(counter);
      auto mainPad = new TPad(Form("mainPad_%s_%s",sample.c_str(),ch.c_str()),"mainPad",0.,0.25,1.,1.);
      mainPad->SetTopMargin(0);
      mainPad->SetLeftMargin(0.12);
      mainPad->SetRightMargin(0.12);
      mainPad->SetTopMargin(0);
      mainPad->SetBottomMargin(0.5);
      mainPad->SetBottomMargin(1e-3);
      mainPad->Draw();
      auto subPad = new TPad(Form("subPad_%s_%s",sample.c_str(),ch.c_str()),"subPad",0.,0.,1.,0.25);
      subPad->Draw();
      subPad->SetTopMargin(0);
      subPad->SetLeftMargin(0.12);
      subPad->SetRightMargin(0.12);
      subPad->SetTopMargin(0);
      subPad->SetBottomMargin(0.5);
      mainPad->cd();
      auto hNom = (TH1*)(f1->Get(Form("%d/%s/HMassWZ_SR1_%s_WCentral",year,sample.c_str(),ch.c_str())));
      const auto g = getUpDownErrorGraph(year, sample, ch, syst);
      auto gr = new TGraphAsymmErrors(g->GetN());
      float absmin = 1e3, absmax = -1.f;
      for(int i = 0; i < g->GetN(); ++i){
        float nom = hNom->GetBinContent(i);
        double xv, yv;
        g->GetPoint(i,xv,yv);
        if (nom == 0.f or nom < 0) {
          gr->SetPoint(i,xv,1.);
          gr->SetPointError(i,g->GetErrorXlow(i),g->GetErrorXhigh(i), 0., 0.);
          continue;
        }
        float rhigh = (yv + g->GetErrorYhigh(i))/nom;
        if (rhigh>absmax) absmax = rhigh;
        rhigh = rhigh - 1.f;
        float rlow = (yv - g->GetErrorYlow(i))/nom;

        if (rlow<absmin) absmin = rlow;
        rlow = 1. - rlow;
        gr->SetPoint(i,xv, 1.);
        gr->SetPointError(i,
                          g->GetErrorXlow(i),
                          g->GetErrorXhigh(i),
                          rlow, rhigh);
      }
      std::cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~\n";
      g->Print();
      std::cout << sample.c_str() << "\t" << ch.c_str() << "\t" << syst << "\t" << absmin << "\t" << absmax << "\n";
      std::cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~\n";
      gr->Print();
      g->SetFillColor(2);
      g->SetFillStyle(3001);
      //g->SetMarkerSize();
      g->SetMarkerStyle(21);
      hNom->SetTitle(Form("%d %s %s; ;%s",year,syst.c_str(),channels[ch].c_str(),sample.c_str()));
      hNom->SetStats(kFALSE);
      hNom->GetXaxis()->SetRangeUser(0,5e3);
      hNom->Draw("HIST");
      g->Draw("2P");
      subPad->cd();
      gr->SetFillColor(kGreen);
      gr->SetFillStyle(3001);
      gr->SetMarkerStyle(21);
      gr->Draw("A2");
      //gr->Draw("P");
      gr->GetYaxis()->SetRangeUser( absmin < absmax? absmin : 0., absmin < absmax? absmax: 1.);
      // gr->GetXaxis()->SetRangeUser(0,hNom->GetBinLowEdge(hNom->GetNbinsX()+1)+
      //                               hNom->GetBinWidth(hNom->GetNbinsX()+1));
      gr->GetXaxis()->SetRangeUser(100.,5e3);

      const int font = 43;
      const float fontSize = 8.;
      const float labelSize = 0.14;
      gr->SetTitle("");
      gr->GetXaxis()->SetTitleFont(font);
      gr->GetXaxis()->SetTitleSize(fontSize);
      gr->GetXaxis()->SetLabelSize(labelSize);
      gr->GetXaxis()->SetTitleOffset(12.0);
      gr->GetYaxis()->SetTitleFont(font);
      gr->GetYaxis()->SetTitleSize(fontSize);
      gr->GetYaxis()->SetLabelSize(labelSize);
      gr->GetYaxis()->SetTitleOffset(4.0);
      gr->GetYaxis()->SetNdivisions(6,3,0);

      //gPad->SetLogx();
      ++counter;
    }
    c->SetLogx();
    c->Print(Form("plots/%d_%s_%s.png",year,syst.c_str(),sample.c_str()));
    delete c;
  };


    std::vector<int> yyyy = { /*2016, 2017,*/ 2018 };
  std::vector<std::string> systematics = {
    "KFactorQCD",
    "KFactorEWK",
    "ElReco",
    "ElTrigger","ElID",
    "MuTrigger",
    "MuID",
    "MetUncl",
    "Pileup",
    "LHEPdf",
    "LHEScale"
  };
  for(auto syst: systematics){
    for(auto y: yyyy){
      for (auto BGN: BgNames[y]){
        if(syst.find("KFactor") != std::string::npos and BGN.folderName.find("DYJets") == std::string::npos) continue;
          printUpDown(y,BGN.folderName,syst);
      }
    }
  }
  throw;

  std::function<void(const int&, THStack* hs, TH1*)> printBgContrib = [&](const int& year, THStack* hsbg, TH1* hsig = nullptr) {

    TCanvas* cc = new TCanvas("cc","cc");
    TH1D* hc = new TH1D("hc",Form("BG Contribution %d",year),6,0.,6.);

    TIter b = hsbg->begin();

    Double_t total = 0.;

    while(b.Next()){
      auto histo = static_cast<TH1F*>(*b);
      hc->Fill(histo->GetTitle(),histo->Integral());
      total += histo->Integral();
    }

    if(hsig){
      hc->Fill("W'",hsig->Integral());
      total += hsig->Integral();
    }

    hc->Scale(1/total);
    hc->Draw("HIST TEXT45");
    cc->Print(Form("plots/%d/%s_%d_BGContrib.png",year,fileLabel.c_str(),year));
    delete hc;
    delete cc;
  };

  auto getIntegral = [](TH1* h, const float& xmin, const float& xmax){

    Double_t integral = 0.;
    if (xmax < xmin) throw;

    const int startBin = h->FindBin(xmin);
    const int endBin   = h->FindBin(xmax);
    if (startBin > endBin) throw;

    for(int i = startBin; i <= endBin; ++i){
      integral += h->GetBinContent(i);
    }

    return integral;
  };

  auto getPunziSignificance = [&](THStack* hsbg,
                                  const std::string signalFolder,
                                  const std::string histoName){

    Int_t wpmass;
    std::regex rexp1("(Wprime)([A-Za-z_-]+)([0-9]+)(.*)");
    std::smatch sm;
    if(std::regex_search(signalFolder,sm,rexp1)){
      wpmass = std::stoi(sm[3]);
    } else {
      throw("Punzi w/o signal");
    }

    auto hsig = getHistoFromFile(signalFolder,histoName);
    const float width = 0.15; /* 15% Window */
    const float massMin = wpmass * (1-width);
    const float massMax = wpmass * (1+width);

    TIter b = hsbg->begin();

    Double_t bgIntegral = 0.;

    while(b.Next()){
      auto histo = static_cast<TH1F*>(*b);
      bgIntegral += getIntegral(histo, massMin, massMax);
    }

    Double_t sigIntegral = getIntegral(hsig, massMin, massMax) / getCutCount(signalFolder,"NoCuts");

    return (sigIntegral / ( 1. + sqrt(bgIntegral)));

  };

  auto getWpMassFromName = [] (const std::string& signalName) {
    Int_t WpMass = -1;
    std::regex rexp1("(Wprime)([A-Za-z_-]+)([0-9]+)(.*)");
    std::smatch sm;
    if(std::regex_search(signalName,sm,rexp1)) WpMass = std::stoi(sm[3]);
    return WpMass;
  };

  auto plotPunziSignificance = [&] (const int& yr) {

    const std::string fromHisto = "HMassWZ_SR1_+ABCD";
    TCanvas* c1 = new TCanvas("c1","c1");

    THStack *hs = new THStack("hs","");
    hs = getBGStack(yr, fromHisto);

    TGraph* GPunziS = new TGraph();
    GPunziS->SetTitle(Form("Punzi Significance vs M(WZ) %d; M(WZ); Punzi Significance", yr));

    for (auto& signal: SignalSamples[yr]) {
        GPunziS->SetPoint(GPunziS->GetN(),
                          getWpMassFromName(signal.folderName),
                          getPunziSignificance(hs, Form("%d/%s",yr,signal.folderName.c_str()), fromHisto));
    }

    GPunziS->SetMarkerStyle(20);
    GPunziS->Draw("AP");
    GPunziS->Write(Form("%d_%s_GPunziS",yr,fileLabel.c_str()));
    c1->Print(Form("plots/%d/%s_%d_PonziSignificance.png",yr,fileLabel.c_str(),yr));


    delete hs;
    delete c1;
  };

  std::function<void(const int&)> plotHLTSelectionRatio = [&] (const int& yr) {

    TCanvas* c1 = new TCanvas("c1","c1");

    TMultiGraph* mg = new TMultiGraph("mg",Form("HLT Efficiency;Wprime Mass Point [%d]; Ratio [EventsPassingHLTs/EventsGenerated]",yr));

    TGraph* GElTgEff = new TGraph();
    GElTgEff->SetTitle("Electron HLTs Efficiency");
    TGraph* GMuTgEff = new TGraph();
    GMuTgEff->SetTitle("Muon HLTs Efficiency");
    TGraph* GHLTEff = new TGraph();
    GHLTEff->SetTitle("HLT Efficiency (El OR Mu)");

    for (auto& signal: SignalSamples[yr]) {
      const Int_t WpMass = getWpMassFromName(signal.folderName);
      addCutEff(GElTgEff, Form("%d/%s",yr,signal.folderName.c_str()), "FailElectronHLTs", "NoCuts", WpMass);
      addCutEff(GMuTgEff, Form("%d/%s",yr,signal.folderName.c_str()), "FailMuonHLTs", "NoCuts", WpMass);
      addCutEff(GHLTEff, Form("%d/%s",yr,signal.folderName.c_str()), "FailHLT", "NoCuts", WpMass);
    }

    auto invertGraph = [] (TGraph *g){
      Double_t* xi = g->GetX();
      Double_t* yi = g->GetY();
      Double_t xx = 0.;
      Double_t yy = 0.;
      for(int i = 0; i < g->GetN(); ++i){
        g->GetPoint(i,xx,yy);
        xi[i] = xx;
        yi[i] = 1. - yy;
      }
    };

    GElTgEff->SetMarkerStyle(22);
    GElTgEff->SetMarkerColor(kBlack);
    invertGraph(GElTgEff);
    mg->Add(GElTgEff,"AP");
    GElTgEff->GetYaxis()->SetRangeUser(0, 1.);
    GMuTgEff->SetMarkerStyle(23);
    GElTgEff->SetMarkerSize(0.5);
    GMuTgEff->SetMarkerColor(kBlue);
    invertGraph(GMuTgEff);
    mg->Add(GMuTgEff,"P");
    GMuTgEff->SetMarkerSize(0.5);
    GHLTEff->SetMarkerStyle(24);
    GHLTEff->SetMarkerColor(kRed);
    GHLTEff->SetMarkerSize(0.5);
    invertGraph(GHLTEff);
    mg->Add(GHLTEff,"P");
    mg->Draw("A");
    gPad->BuildLegend();
    c1->Print(Form("plots/%d/%s_%d_SignalTriggerEfficiency.pdf",yr,fileLabel.c_str(),yr));
    c1->Write(Form("%d_%s_SignalTriggerEfficiency",yr,fileLabel.c_str()));

    delete c1;
  };

  std::function<void(const int&)> plotSelectionRatio = [&] (const int& yr) {

    const Int_t nMassPoints = 14;

    TCanvas* c1 = new TCanvas("c1","c1");

    TGraph* GSelRatioA = new TGraph();
    GSelRatioA->SetTitle("eee#nu");
    TGraph* GSelRatioB = new TGraph();
    GSelRatioB->SetTitle("ee#mu#nu");
    TGraph* GSelRatioC = new TGraph();
    GSelRatioC->SetTitle("e#mu#mu#nu");
    TGraph* GSelRatioD = new TGraph();
    GSelRatioD->SetTitle("#mu#mu#mu#nu");

    for (auto& signal: SignalSamples[yr]) {
      const Int_t WpMass = getWpMassFromName(signal.folderName);
      addCutEff(GSelRatioA, Form("%d/%s",yr,signal.folderName.c_str()), "0", "NoCuts", WpMass);
      addCutEff(GSelRatioB, Form("%d/%s",yr,signal.folderName.c_str()), "1", "NoCuts", WpMass);
      addCutEff(GSelRatioC, Form("%d/%s",yr,signal.folderName.c_str()), "2", "NoCuts", WpMass);
      addCutEff(GSelRatioD, Form("%d/%s",yr,signal.folderName.c_str()), "3", "NoCuts", WpMass);
    }

    TGraph GInclusive = TGraph(nMassPoints);
    GInclusive.SetTitle("Combined");

    auto addToInclusive = [&](TGraph* g){
      Double_t* xi = GInclusive.GetX();
      Double_t* yi = GInclusive.GetY();
      Double_t xx = 0.;
      Double_t yy = 0.;
      for(int i = 0; i < g->GetN(); ++i){
        g->GetPoint(i,xx,yy);
        xi[i] = xx;
        yi[i] += yy;
      }
    };

    GSelRatioA->SetMarkerStyle(20);
    GSelRatioA->SetMarkerSize(0.5);
    GSelRatioA->SetMarkerColor(kBlue);
    GSelRatioB->SetMarkerStyle(21);
    GSelRatioB->SetMarkerSize(0.5);
    GSelRatioB->SetMarkerColor(kOrange);
    GSelRatioC->SetMarkerStyle(22);
    GSelRatioC->SetMarkerSize(0.5);
    GSelRatioC->SetMarkerColor(kGreen);
    GSelRatioD->SetMarkerStyle(23);
    GSelRatioD->SetMarkerSize(0.5);
    GSelRatioD->SetMarkerColor(kViolet);

    addToInclusive(GSelRatioA);
    addToInclusive(GSelRatioB);
    addToInclusive(GSelRatioC);
    addToInclusive(GSelRatioD);

    GInclusive.SetMarkerColor(kRed);
    GInclusive.SetMarkerStyle(24);
    GInclusive.SetMarkerSize(0.5);
    GInclusive.GetYaxis()->SetTitle("#EventsPassing/#nEventsInSignal");
    GInclusive.GetXaxis()->SetTitle(Form("Wprime Mass Point %d",yr));
    GInclusive.Draw("AP");
    GInclusive.GetYaxis()->SetRangeUser(0.0,0.4);

    GSelRatioA->Draw("P");
    GSelRatioB->Draw("P");
    GSelRatioC->Draw("P");
    GSelRatioD->Draw("P");
    gPad->BuildLegend(0.1,0.7,0.25,0.9);
    c1->Print(Form("plots/%d/%s_%d_SelectionRatio.png",yr,fileLabel.c_str(),yr));
    c1->Write(Form("%s_%d_SelectionRatio.png",fileLabel.c_str(),yr));

    delete c1;
  };

  std::function<void(std::vector<Int_t>, std::vector<std::string>,bool)>
    canvasStacked = [&] (std::vector<Int_t> WpMass,
                         std::vector<std::string> names,
                         bool includeData = true) {

    const Int_t npads = names.size();
    bool includeAllYears{};

    std::function<void(std::string&)> removeCentral = [&] (std::string& s){
      if (s.find("_WCentral") != std::string::npos){
        s.erase(s.find("_WCentral"),9);
      } else if (s.find("_Up") != std::string::npos) {
        s.erase(s.find("_Up"),3);
      } else if ( s.find("_Down") != std::string::npos){
        s.erase(s.find("_Down"),5);
      } else if (s.find("_NoSF") != std::string:: npos){
        s.erase(s.find("_NoSF"),5);
      }
      
      if (s.find("_WCentral") != std::string::npos)  removeCentral(s);
      if (s.find("_Up")      != std::string::npos)  removeCentral(s);
      if (s.find("_Down")    != std::string::npos)  removeCentral(s);
      s.append("_NoSF");
    };

    TCanvas* c1;
    if ( npads == 1 ){
      c1 = new TCanvas("cs","cs",10,10,600,600);
      c1->Divide(1,1);
    } else if( npads == 2 ){
      c1 = new TCanvas("cs","cs",10,10,1200,600);
      c1->Divide(2,1);
    } else if (npads == 3) {
      c1 = new TCanvas("cs","cs",10,10,1800,600);
      c1->Divide(3,1);
    } else if (npads == 4){
      c1 = new TCanvas("cs","cs",10,10,1500,1200);
      c1->Divide(2,2);
    } else if (npads == 6) {
      c1 = new TCanvas("cs","cs",10,10,1500,1000);
      c1->Divide(3,2);
    } else if (npads == 8) {
      c1 = new TCanvas("cs","cs",10,10,2000,1000);
      c1->Divide(4,2);
    } else {
      std::clog << "Undefined canvas division: " << npads << "\n";
    }

    std::string pngName;
    std::vector<std::map<std::string,double>> yieldsTable;
    Int_t j = 1;

    for (auto hName : names) {
      int year = std::stoi(hName.substr(0,4));
      if (year == 0) includeAllYears = true;
      hName = hName.substr(5,hName.size());
      if(hName.find("HFakeString") == std::string::npos or
         hName.find("HMuonPtDiff") == std::string::npos) 
        hName += "_WCentral"; // Include Central SF and genWeight
      Int_t r = (j-1)%npads;
      c1->cd(r+1);
      std::string dataHName = hName;
      removeCentral(dataHName);
      pngName += hName + "_";
      //pngName = pngName.substr(0,20);
      if(includeAllYears){
        pngName += "Run2_";
      } else {
        pngName += Form("%d_",year);
      }

      const Float_t leftMargin = 0.12;
      const Float_t rightMargin = 0.12;
      const Float_t topMargin = 0.12;
      const Float_t bottomMargin = 0.5;

      TPad *mainPad;

      if (includeData) {
        mainPad = new TPad(Form("mainPad_%s",hName.c_str()),"mainPad",0.,0.25,1.,1.);
        mainPad->SetBottomMargin(1e-3);
      } else {
        mainPad = new TPad(Form("mainPad_%s",hName.c_str()),"mainPad",0.,0.,1.,1.);
      }

      mainPad->Draw();
      mainPad->SetLeftMargin(leftMargin);
      mainPad->SetRightMargin(rightMargin);
      //mainPad->SetLogy();
      mainPad->SetTickx();
      mainPad->SetTicky();

      auto cmsLabel = new TPaveText(0.11,0.93,0.3,1.0,"NDC");
      cmsLabel->SetFillColor(0);
      cmsLabel->SetBorderSize(0);
      cmsLabel->AddText("CMS Preliminary");
      cmsLabel->SetTextAlign(12);
      cmsLabel->Draw();

      auto lumiLabel = new TPaveText(0.6,0.93,0.89,1.0,"NDC");
      lumiLabel->SetFillColor(0);
      lumiLabel->SetBorderSize(0);
      lumiLabel->AddText(Form("#sqrt{s} = 13TeV L = %.2f fb^{-1}",luminosity[year]));
      lumiLabel->SetTextAlign(12);
      lumiLabel->Draw();

      auto legend = new TLegend(0.3, 0.66, .87, .89);
      legend->SetNColumns(2);

      THStack *hs;

      std::vector<int> Run2Years = {2016,2017,2018};
      if(!includeAllYears){
        hs = getBGStack(year,hName,legend);
      } else {
        THStack* hsRun2 = new THStack(Form("hsRun2_%s",hName.c_str()),"");
        std::unordered_map<int/*color*/,std::vector<TH1*>> samples;
        for(auto yr_: Run2Years){
          THStack *hs_ = getBGStack(yr_, hName);
          TList* lst = hs_->GetHists();
          TIter next(lst);
          while(auto h = static_cast<TH1*>(next())){
            try {
              samples[h->GetFillColor()].push_back(h);
            } catch(...) {
              samples.insert(std::map<int,std::vector<TH1*>>::value_type(h->GetFillColor(),{ h }));
            }
          }
        };

        std::vector<int> colorSeen;
        for(auto const& sampleVector : samples){
          for(auto h_: sampleVector.second){
            hsRun2->Add(h_);
            std::vector<int>::iterator it
              = find(colorSeen.begin(), colorSeen.end(), sampleVector.first);
            if(it == colorSeen.end()){
              legend->AddEntry(h_);
              colorSeen.emplace_back(sampleVector.first);
            }
          }
        }

        hs = hsRun2;

      }

      TH1F* last = static_cast<TH1F*>(hs->GetStack()->Last());
      std::vector<std::pair<SignalInfo,TH1*>> signals;

      if(includeAllYears){
        TH1* hsigRun2;
        for(auto m: WpMass){
          for(auto yr_: Run2Years){
            SignalInfo signal = SignalSamples[yr_][SignalPos[m]];
            TH1* hsig = getHistoFromFile(Form("%d/%s",yr_,signal.folderName.c_str()),hName);
            applyLumiSF(hsig, Form("%d/%s",yr_,signal.folderName.c_str()), signal.xsec);
            if (yr_ == 2016) {
              hsigRun2 = hsig;
            } else {
              hsigRun2->Add(hsig);
            }
          }
          SignalInfo tmp_ = { Form("Wprime %.2f TeV",((float)m)/1e3), "", 0.};
          signals.emplace_back(std::make_pair(tmp_,hsigRun2));
        }
      } else {
        for(auto m: WpMass) signals.emplace_back(std::make_pair(SignalSamples[year][SignalPos[m]],nullptr));
        for(auto& signal: signals){
          TH1* hh = getHistoFromFile(Form("%d/%s",year,signal.first.folderName.c_str()),hName);
          applyLumiSF(hh, Form("%d/%s",year,signal.first.folderName.c_str()), signal.first.xsec);
          signal.second = hh;
        }
      }

      std::string labelIdx = dataHName;
      labelIdx.erase(labelIdx.find("_NoSF"),5);
      if (hName.find("CR1_") != std::string::npos) {
        labelIdx.erase(labelIdx.find("CR1_"),4);
      } else if (hName.find("CR2_") != std::string::npos) {
        labelIdx.erase(labelIdx.find("CR2_"),4);
      } else if (hName.find("SR1_") != std::string::npos) {
        labelIdx.erase(labelIdx.find("SR1_"),4);
      } else if (hName.find("SR2_") != std::string::npos) {
        labelIdx.erase(labelIdx.find("SR2_"),4);
      }

      if (labelIdx.find("+") != std::string::npos){
        labelIdx.erase(labelIdx.find("+",8));
      }
      hs->SetTitle(Labels[labelIdx].c_str());

      std::cout << "TITLE::::::::\\n\n\n\n\n\n" << labelIdx.c_str() << "\n\n\n\n\n";

      if (hName.find("CR1") != std::string::npos) {
        hs->SetTitle((Labels[labelIdx].c_str() + std::string(" CR1")).c_str());
      } else if (hName.find("CR2_") != std::string::npos) {
        hs->SetTitle((Labels[labelIdx].c_str() + std::string(" CR2")).c_str());
      } else if (hName.find("SR1_") != std::string::npos) {
      hs->SetTitle((Labels[labelIdx].c_str() + std::string(" SR")).c_str());
      }
      legend->SetBorderSize(0);
      gStyle->SetOptStat(0);

      mainPad->cd();
      hs->Draw("HIST");
      int color_ = kBlack;
      if(hName.find("CR") == std::string::npos){
        for(auto signal: signals){
          signal.second->SetTitle(signal.first.legendName.c_str());
          legend->AddEntry(signal.second,Form("%s",signal.first.legendName.c_str()),"L");
          signal.second->SetLineColor(color_++);
          signal.second->SetLineWidth(3);
          signal.second->SetFillColor(0);
          signal.second->Draw("HIST SAME");
        }
      }
      double maxx = last->GetBinLowEdge(last->FindLastBinAbove(0.9)+1);
      double minx = last->GetBinLowEdge(last->FindFirstBinAbove(0.9));
      TH1*   htmp = hs->GetHistogram();
      TAxis* gtmp = htmp->GetXaxis();
      if(hName.find("HIP3D") != std::string::npos){
        minx = 0.;
        maxx = 0.06;
      } else if (hName.find("HMuonPtDiff") != std::string::npos){
        minx = -0.01;
        maxx = 0.01;
      }
      //gtmp->SetRange(htmp->FindBin(minx),htmp->FindBin(maxx));
      gtmp->SetRangeUser(minx,maxx);

      fixYRange(hs,getMaxY(last));

      if (includeData) {
        auto subPad = new TPad(Form("mainPad_%s",hName.c_str()),"subPad",0.,0.,1.,0.25);
        c1->cd(r+1);
        subPad->Draw();
        subPad->SetLeftMargin(leftMargin);
        subPad->SetRightMargin(rightMargin);
        subPad->SetTopMargin(0);
        subPad->SetBottomMargin(bottomMargin);

        TH1* hdata;
        if(includeAllYears){
          hdata = getDataStack(Form("%d/%s",2016,DataSampleNames[2016].c_str()),dataHName);
          hdata->Add(getDataStack(Form("%d/%s",2017,DataSampleNames[2017].c_str()),dataHName));
          hdata->Add(getDataStack(Form("%d/%s",2018,DataSampleNames[2018].c_str()),dataHName));
        } else {
          hdata = getDataStack(Form("%d/%s",year,DataSampleNames[year].c_str()),dataHName);
        }

        minx = hdata->GetBinLowEdge(hdata->FindFirstBinAbove(0.1));
        maxx = hdata->GetBinLowEdge(hdata->FindLastBinAbove(0.1))+hdata->GetBinWidth(hdata->FindLastBinAbove(0.));
        gtmp->SetRangeUser(minx,maxx);

        yieldsTable.push_back(printYieldsTable(hs, hdata));

        mainPad->cd();
        hdata->SetMarkerStyle(kFullCircle);
        fixYRange(hs,getMaxY(hdata));
        hdata->Draw("SAME P");
        TH1F* herror = getErrorHisto(hs);
        herror->Draw("SAME E2");
        legend->AddEntry(hdata,includeAllYears? "Run 2 Data" : Form("Data %d",year));

        auto hcdata = getRatio(hdata,hs);
        subPad->cd();
        subPad->SetGrid();
        hcdata->GetXaxis()->SetNdivisions(hs->GetHistogram()->GetXaxis()->GetNdivisions());
        hcdata->GetXaxis()->SetLimits(minx,maxx);


        hcdata->Draw("ap");
        subPad->SetFrameLineWidth(1);

      }
      ++j;

      mainPad->cd();
      legend->Draw();
      if( r+1 == npads ){
        c1->Print(Form("plots/%d/%s_%s.png",year,fileLabel.c_str(),pngName.c_str()));
        c1->Write(Form("%d_%s_%s_Wprime_Data",year,fileLabel.c_str(),hName.c_str()));
      }
    }

    ////////////////////////////////////////////////////
    std::map<std::string,std::vector<double>> yieldsTable_;
    for(auto sample: yieldsTable){
      for(auto const&[sampleName, yield] : sample) {
        yieldsTable_[sampleName].push_back(yield);
      }
    }
    for(auto const&[sampleName, yields] : yieldsTable_) {
      std::cout << sampleName << " & ";
      Double_t total = 0.;
      for(auto yield: yields){
        total += yield;
        std::cout << yield << " & ";
      }
      std::cout << total << "\\\\\n";
    }

    double totalBkgA = 0.;
    double totalBkgB = 0.;
    double totalBkgC = 0.;
    double totalBkgD = 0.;

    double totalSigA = 0.;
    double totalSigB = 0.;
    double totalSigC = 0.;
    double totalSigD = 0.;

    for(auto const&[sampleName, yields] : yieldsTable_) {
      if(sampleName == "Data") continue;
      totalBkgA += yields[0];
      totalBkgB += yields[1];
      totalBkgC += yields[2];
      totalBkgD += yields[3];
    }
    std::cout << "Total Background & " << totalBkgA
    << " & " << totalBkgB << " & " << totalBkgC
    << " & " << totalBkgD << " & " << totalBkgA+totalBkgB+totalBkgC+totalBkgD << "\\\\\\hline\n";
    ////////////////////////////////////////////////////

    delete c1;

  };

  std::vector<int> pyear = { 2016, 2017, 2018 };
  std::vector<std::string> chs = { "A","B","C","D" };
  std::vector<std::string> hints = {
    // "HElPt","HMuPt",
    // "HElEta","HMuEta",
    // "HElPhi","HMuPhi",
    // "HPtZMWZ","HPtWMWZ",
    //"HDistZl3",
    //"HDistZW",
    // "HDistl1l2",
    // "HDistl2l3",
    // "HDistl1l3",
    //"HDxyl1","HDxyl2","HDxyl3",
    // "HPtZMWZ",
    // "HPtWMWZ",
    //"HMassTW",
    //,"HWZDist"
    //"HLt",
    //"HIP3Dl1","HIP3Dl2","HIP3Dl3",
    // "HRelIsol1","HRelIsol2","HRelIsol3",
    //"HDzl1","HDzl2","HDzl3",
    "HEtal1","HEtal2","HEtal3",
    "HPhil1","HPhil2","HPhil3",
    // "HDistl1l2"
    //"HIP3Dl1","HIP3Dl2","HIP3Dl3",
    //"HFakeString",
    // "HPileup",
    // "HRelIsol1",
    // "HRelIsol2",
    // "HRelIsol3",
    "HPtl1","HPtl2","HPtl3",
    "HMetPt",
    "HMassZ",
    "HMassWZ",
    //"HPtl1Lt","HPtl2Lt","HPtl3Lt","HMetPtLt","HCosl3Met"
  };

  // // Difference in channels per year
  // for(auto h : hints){
  //   for(auto ch: chs){
  //     std::vector<std::string> hNames;
  //   for(auto yr: pyear){
  //       hNames.emplace_back(Form("%d/%s_SR1_%s",yr,h.c_str(),ch.c_str()));
  //        if(hNames.size() == 3){
  //          canvasStacked(/*s.first*/600,hNames,false);
  //          hNames.clear() ;
  //        }
  //      }
  //    }
  //  }


  //// 4 Channels in same plot
  // for(auto h : hints){
  //   for(auto yr: pyear){
  //     std::vector<std::string> hNames;
  //     for(auto ch: chs){
  //       hNames.emplace_back(Form("%d/%s_CR2_%s",yr,h.c_str(),ch.c_str()));
  //        if(hNames.size() == 4){
  //          canvasStacked(600,hNames,true);
  //          hNames.clear() ;
  //        }
  //      }
  //    }
  //  }

  //plotPunziSignificance(2016);

  //Run2 Plots
  std::vector<std::string> yearp = { "2016","2017","2018","0000" };
  std::vector<std::string> reg_ = { "CR1","CR2","SR1" };
  std::vector<Int_t> signals_ = { 600,800,1000,1200,1400,1600 };
  for(auto rr: reg_){
    for(auto y : yearp){
      for(auto h : hints){
        std::vector<std::string> hNames;
        for(auto ch: chs){
          hNames.emplace_back(Form("%s/%s_%s_%s",y.c_str(),h.c_str(),rr.c_str(),ch.c_str())); //0000 -> Run2
          if(hNames.size() == 4){
            canvasStacked(signals_,hNames,rr != "SR1");
            hNames.clear() ;
          }
        }
      }
    }
  }

  // Combine all Lepton channels Run2 plots
  // std::vector<std::string> yearp = { "2016","2017","2018","0000" };  //0000 -> Run2
  // std::vector<std::string> reg_ = { "CR1","CR2","SR1" };
  // std::vector<Int_t> signals_ = { 600,800,1000,1200,1400,1600,1800,2000,2500,/*3000,3500,4000,4500*/ };
  // for(auto rr: reg_){
  //   for(auto y : yearp){
  //     for(auto h : hints){
  //       std::vector<std::string> hNames;
  //       for(auto ch: chs){
  //         std::string s_ = Form("%s/%s_%s_+ABCD",y.c_str(),h.c_str(),rr.c_str());
  //         hNames.emplace_back(s_);
  //         std::cout << s_ << std::endl;
  //         if(hNames.size() == 1){
  //           canvasStacked(signals_,hNames,rr != "SR1");
  //           hNames.clear() ;
  //         }
  //       }
  //     }
  //   }
  // }

  if(MissingSamples.size()>0){
    for(auto s: MissingSamples){
      std::puts(Form("WARNING! Missing sample: %s", s.c_str()));
    }
  }

  // std::vector<std::string> hNames_1 = {
  //   "2016/HPtZMWZ_SR1_A+HPtZMWZ_SR1_A","2016/HPtZMWZ_SR1_B+HPtZMWZ_SR1_B",
  //   "2016/HPtZMWZ_SR1_C+HPtZMWZ_SR1_C","2016/HPtZMWZ_SR1_D+HPtZMWZ_SR1_D"
  // };

  // canvasStacked(1200,hNames_1,true);


  //plotHLTSelectionRatio(2016);
  //plotSelectionRatio(2016);

  for (auto& item: SignalSamples) {

    const int year = item.first;

    //plotSelectionRatio(year);
    //plotHLTSelectionRatio(year);

    for (auto signal: item.second) {

      const Int_t WpMass = getWpMassFromName(signal.folderName);

      // printCutFlowStack(year,signal);
      // printH2Comb(year, signal, "HPtWPtZ_SR_+ABCD");
      // printH2Comb(year, signal, "HDeltaRPtZ_SR_+ABCD");
      // printH2Comb(year, signal, "HLtMWZ_SR_+ABCD");

    }
  }
}
