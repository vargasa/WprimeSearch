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
    {2016, 35.92},
    {2017, 41.43},
    {2018, 59.74}
  };

  /*Run2 Luminosity: 137.4 fb^-1*/

  struct BackgroundInfo {
    std::string legendName;
    std::string folderName;
    UInt_t color;
    Float_t xsec;
  };

  std::unordered_map<int, std::string> DataSampleNames = {
    {2016, "SinglePhotonSingleElectronSingleMuon"},
    {2017, "SinglePhotonSingleElectronSingleMuon"},
    {2018, "SingleMuonEGamma"}
  };

  // ShortName, DasName, kColor, Style, XSection
  std::unordered_map<int, std::vector<BackgroundInfo>> BgNamesUL =
    {
      {
        2016,
        {

          BackgroundInfo{"DY","DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8",kOrange+7,6.637e+03},
          BackgroundInfo{"WZ","WZ_TuneCP5_13TeV-pythia8",kOrange,2.750e+01},
          BackgroundInfo{"WWZ","WWZ_4F_TuneCP5_13TeV-amcatnlo-pythia8",15,1.707e-01},
          BackgroundInfo{"ST","ST_s-channel_4f_leptonDecays_TuneCP5_13TeV-amcatnlo-pythia8",kGreen+3,3.549e+00},
          BackgroundInfo{"ST","ST_t-channel_antitop_5f_InclusiveDecays_TuneCP5_13TeV-powheg-pythia8",kGreen+3,7.174e+01},
          BackgroundInfo{"ST","ST_t-channel_top_5f_InclusiveDecays_TuneCP5_13TeV-powheg-pythia8",kGreen+3,1.197e+02},
          BackgroundInfo{"t#bar{t}","TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8",kBlue+3,7.592e+02},
          BackgroundInfo{"TT","TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8",kBlue+3,6.871e+02},
        }
      },
      {
        2017,
        {
          BackgroundInfo{"WZ","WZ_TuneCP5_13TeV-pythia8",kOrange,2.750e+01},
          BackgroundInfo{"WWZ","WWZ_4F_TuneCP5_13TeV-amcatnlo-pythia8",15,1.707e-01},
        }
      }
    };

  // ShortName, DasName, kColor, Style, XSection
  std::unordered_map<int, std::vector<BackgroundInfo>> BgNames =
    {
      {
        2016,
        {
          BackgroundInfo{"WZ","WZTo3LNu_mllmin01_13TeV-powheg-pythia8",
                         kOrange,6.217e+01}, /*XSecAnalyzer 0.*/
          BackgroundInfo{"ZZ","ZZTo4L_13TeV_powheg_pythia8",
                         kBlue,1.256},
          BackgroundInfo{"t#bar{t}","TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
                         kBlue-2,5.675e1},
          BackgroundInfo{"Z#gamma","ZGTo2LG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8",
                         kRed+3,1.24e2},
          BackgroundInfo{"DY","DYJetsToLL_M-50_HT-100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
                         kOrange+7,1.475e2},
          BackgroundInfo{"DY","DYJetsToLL_M-50_HT-200to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
                         kOrange+7,4.104e1},
          BackgroundInfo{"DY","DYJetsToLL_M-50_HT-400to600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
                         kOrange+7,5.676},
          BackgroundInfo{"DY","DYJetsToLL_M-50_HT-600to800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
                         kOrange+7,1.36},
          BackgroundInfo{"DY","DYJetsToLL_M-50_HT-800to1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
                         kOrange+7,6.218e-1},
          BackgroundInfo{"DY","DYJetsToLL_M-50_HT-1200to2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
                         kOrange+7,1.512e-1},
          BackgroundInfo{"DY","DYJetsToLL_M-50_HT-2500toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
                         kOrange+7,3.659e-3},
          BackgroundInfo{"TTV","TTWJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8",
                         kCyan+1,2.007e-1},
          BackgroundInfo{"TTV","TTZToLL_M-1to10_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
                         kCyan+1,0.283}, /*XSecAnayzer 0.*/
          BackgroundInfo{"TTV","ttZJets_13TeV_madgraphMLM-pythia8",
                         kCyan+1,6.559e-1},
          BackgroundInfo{"VVV","WWW_4F_TuneCUETP8M1_13TeV-amcatnlo-pythia8",
                         14,2.086e-1},
          BackgroundInfo{"VVV","WWZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8",
                         14,1.651e-1},
          BackgroundInfo{"VVV","WZZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8",
                         14,5.565e-2},
          BackgroundInfo{"VVV","ZZZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8",
                         14,1.398e-2},
          BackgroundInfo{"gg","GluGluToContinToZZTo4e_13TeV_MCFM701_pythia8",
                         43,2.703e-3}, /*AN2019_252_v1*/ //-> Integral is 0
          BackgroundInfo{"gg","GluGluToContinToZZTo4mu_13TeV_MCFM701_pythia8",
                         43,2.703e-3}, /*AN2019_252_v1*/ //-> Integral is 0
          BackgroundInfo{"gg","GluGluToContinToZZTo2e2mu_13TeV_MCFM701_pythia8",
                         43,5.423e-3}, /*AN2019_252_v1*/ //-> Integral is 0
          BackgroundInfo{"gg","GluGluToContinToZZTo4tau_13TeV_MCFM701_pythia8",
                         43,2.703e-3}, /*AN2019_252_v1*/
          BackgroundInfo{"ST","ST_s-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1",
                         kGreen+3,3.365}, //-> Integral is 0
          BackgroundInfo{"ST","ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1",
                         kGreen+3,3.806e1},
          BackgroundInfo{"ST","ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1",
                         kGreen+3,3.809e1},
          BackgroundInfo{"ST","ST_t-channel_antitop_4f_inclusiveDecays_13TeV-powhegV2-madspin-pythia8_TuneCUETP8M1",
                         kGreen+3,80.95}, /*XSecAnalyser 0.*/ //-> Integral is 0
          //BackgroundInfo{"ST","ST_t-channel_top_4f_inclusiveDecays_13TeV-powhegV2-madspin-pythia8_TuneCUETP8M1",
          //               kGreen+3,136.02}, /*XSecAnalyzer 0.*/ //-> Integral is 0
        }
      },
      {
        2017,
        {
          BackgroundInfo{"WZ","WZTo3LNu_mllmin01_NNPDF31_TuneCP5_13TeV_powheg_pythia8",
                         kOrange, 6.217e+01},
          BackgroundInfo{"ZZ","ZZTo4L_13TeV_powheg_pythia8",
                         kBlue,1.325},
          BackgroundInfo{"t#bar{t}","TTJets_DiLept_TuneCP5_13TeV-madgraphMLM-pythia8",
                         kBlue-2,5.420e+1},
          BackgroundInfo{"Z#gamma","ZGToLLG_01J_5f_TuneCP5_13TeV-amcatnloFXFX-pythia8",
                         kRed+3,5.547e1},
          BackgroundInfo{"DY","DYJetsToLL_M-50_HT-100to200_TuneCP5_13TeV-madgraphMLM-pythia8",
                         kOrange+7,1.61e2},
          BackgroundInfo{"DY","DYJetsToLL_M-50_HT-200to400_TuneCP5_13TeV-madgraphMLM-pythia8",
                         kOrange+7,4.858e1},
          BackgroundInfo{"DY","DYJetsToLL_M-50_HT-400to600_TuneCP5_13TeV-madgraphMLM-pythia8",
                         kOrange+7,6.982},
          BackgroundInfo{"DY","DYJetsToLL_M-50_HT-600to800_TuneCP5_13TeV-madgraphMLM-pythia8",
                         kOrange+7,1.747},
          BackgroundInfo{"DY","DYJetsToLL_M-50_HT-800to1200_TuneCP5_13TeV-madgraphMLM-pythia8",
                         kOrange+7,8.052e-1},
          BackgroundInfo{"DY","DYJetsToLL_M-50_HT-1200to2500_TuneCP5_13TeV-madgraphMLM-pythia8",
                         kOrange+7,1.927e-1},
          BackgroundInfo{"DY","DYJetsToLL_M-50_HT-2500toInf_TuneCP5_13TeV-madgraphMLM-pythia8",
                         kOrange+7,3.478e-3},
          BackgroundInfo{"TTV","TTZToLL_M-1to10_TuneCP5_13TeV-amcatnlo-pythia8",
                         kCyan+1,5.324e-2},
          BackgroundInfo{"TTV","ttZJets_TuneCP5_13TeV_madgraphMLM_pythia8",
                         kCyan+1,5.420e-1},
          BackgroundInfo{"TTV","TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8",
                         kCyan+1,2.144e-1},
          BackgroundInfo{"VVV","WWW_4F_TuneCP5_13TeV-amcatnlo-pythia8",
                         14,2.086e-1},
          BackgroundInfo{"VVV","WWZ_4F_TuneCP5_13TeV-amcatnlo-pythia8",
                         14,1.651e-1},
          BackgroundInfo{"VVV","WZZ_TuneCP5_13TeV-amcatnlo-pythia8",
                         14,5.565e-2},
          BackgroundInfo{"VVV","ZZZ_TuneCP5_13TeV-amcatnlo-pythia8",
                         14,1.398e-2},
          BackgroundInfo{"gg","GluGluToContinToZZTo2e2mu_13TeV_TuneCP5_MCFM701_pythia8",
                         43,5.423e-3},
          BackgroundInfo{"gg","GluGluToContinToZZTo4e_13TeV_TuneCP5_MCFM701_pythia8",
                         43,2.703e-3},
          BackgroundInfo{"gg","GluGluToContinToZZTo4mu_13TeV_TuneCP5_MCFM701_pythia8",
                         43,2.703e-3},
          BackgroundInfo{"gg","GluGluToContinToZZTo4tau_13TeV_TuneCP5_MCFM701_pythia8",
                         43,5.423e-3},
          BackgroundInfo{"ST","ST_s-channel_4f_leptonDecays_TuneCP5_13TeV-amcatnlo-pythia8",
                         kGreen+3,3.740e+0},
          BackgroundInfo{"ST","ST_t-channel_antitop_4f_inclusiveDecays_TuneCP5_13TeV-powhegV2-madspin-pythia8",
                         kGreen+3,6.791e+1},
          BackgroundInfo{"ST","ST_t-channel_top_4f_inclusiveDecays_TuneCP5_13TeV-powhegV2-madspin-pythia8",
                         kGreen+3,1.133e+2},
          BackgroundInfo{"ST","ST_tW_antitop_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8",
                         kGreen+3,3.497e+1},
          BackgroundInfo{"ST","ST_tW_top_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8",
                         kGreen+3,3.491e+1},
        }
      },
      {
        2018,
        {
          BackgroundInfo{"WZ","WZTo3LNu_mllmin01_NNPDF31_TuneCP5_13TeV_powheg_pythia8",
                         kOrange,6.217e+01},
          BackgroundInfo{"ZZ","ZZTo4L_TuneCP5_13TeV-amcatnloFXFX-pythia8",
                         kBlue,1.373},
          BackgroundInfo{"t#bar{t}","TTJets_DiLept_TuneCP5_13TeV-madgraphMLM-pythia8",
                         kBlue-2,5.434e+1},
          BackgroundInfo{"Z#gamma","ZGToLLG_01J_5f_TuneCP5_13TeV-amcatnloFXFX-pythia8",
                         kRed+3,5.548e1},
          BackgroundInfo{"DY","DYJetsToLL_M-50_HT-100to200_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8",
                         kOrange+7,1.608e2},
          BackgroundInfo{"DY","DYJetsToLL_M-50_HT-200to400_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8",
                         kOrange+7,4.864e1},
          BackgroundInfo{"DY","DYJetsToLL_M-50_HT-400to600_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8",
                         kOrange+7,6.982},
          BackgroundInfo{"DY","DYJetsToLL_M-50_HT-600to800_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8",
                         kOrange+7,1.756},
          BackgroundInfo{"DY","DYJetsToLL_M-50_HT-800to1200_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8",
                         kOrange+7,8.096e-1},
          BackgroundInfo{"DY","DYJetsToLL_M-50_HT-1200to2500_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8",
                         kOrange+7,1.931e-1},
          BackgroundInfo{"DY","DYJetsToLL_M-50_HT-2500toInf_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8",
                         kOrange+7,3.513e-3},
          BackgroundInfo{"TTV","TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8",
                         kCyan+1,2.181e-1},
          BackgroundInfo{"TTV","TTZToLL_M-1to10_TuneCP5_13TeV-amcatnlo-pythia8",
                         kCyan+1,5.324e-2},
          BackgroundInfo{"TTV","ttZJets_TuneCP5_13TeV_madgraphMLM_pythia8",
                         kCyan+1,5.924e-1},
          BackgroundInfo{"VVV","WWW_4F_TuneCP5_13TeV-amcatnlo-pythia8",
                         14,2.154e-1},
          BackgroundInfo{"VVV","WWZ_TuneCP5_13TeV-amcatnlo-pythia8",
                         14,1.676e-1},
          BackgroundInfo{"VVV","WZZ_TuneCP5_13TeV-amcatnlo-pythia8",
                         14,5.701e-2},
          BackgroundInfo{"VVV","ZZZ_TuneCP5_13TeV-amcatnlo-pythia8",
                         14,1.473e-2},
          BackgroundInfo{"gg","GluGluToContinToZZTo2e2mu_13TeV_TuneCP5_MCFM701_pythia8",
                         43,5.423e-3},
          BackgroundInfo{"gg","GluGluToContinToZZTo4e_13TeV_TuneCP5_MCFM701_pythia8",
                         43,2.703e-3},
          BackgroundInfo{"gg","GluGluToContinToZZTo4mu_13TeV_TuneCP5_MCFM701_pythia8",
                         43,2.703e-3},
          BackgroundInfo{"gg","GluGluToContinToZZTo4tau_13TeV_TuneCP5_MCFM701_pythia8",
                         43,2.703e-3},
          BackgroundInfo{"ST","ST_s-channel_4f_leptonDecays_TuneCP5_13TeV-amcatnlo-pythia8",
                         kGreen+3,3.740e+0},
          BackgroundInfo{"ST","ST_tW_antitop_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8",
                         kGreen+3,3.497e+1},
          BackgroundInfo{"ST","ST_tW_top_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8",
                         kGreen+3,3.491e+1},

        }
      }
    };


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
  };

  std::unordered_map<int, std::vector<SignalInfo>> SignalSamplesUL = {
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
        SignalInfo{"W' (0.6TeV)","WprimeToWZToWlepZlep_narrow_600_TuneCP5_13TeV-madgraph-pythia8",6.252E-2},
        SignalInfo{"W' (0.8TeV)","WprimeToWZToWlepZlep_narrow_800_TuneCP5_13TeV-madgraph-pythia8",1.886E-2},
        SignalInfo{"W' (1.0TeV)","WprimeToWZToWlepZlep_narrow_1000_TuneCP5_13TeV-madgraph-pythia8",7.381E-3},
        SignalInfo{"W' (1.2TeV)","WprimeToWZToWlepZlep_narrow_1200_TuneCP5_13TeV-madgraph-pythia8",3.356E-3},
        SignalInfo{"W' (1.4TeV)","WprimeToWZToWlepZlep_narrow_1400_TuneCP5_13TeV-madgraph-pythia8",1.684E-3},
        SignalInfo{"W' (1.6TeV)","WprimeToWZToWlepZlep_narrow_1600_TuneCP5_13TeV-madgraph-pythia8",9.036E-4},
        SignalInfo{"W' (1.8TeV)","WprimeToWZToWlepZlep_narrow_1800_TuneCP5_13TeV-madgraph-pythia8",5.087E-4},
        SignalInfo{"W' (2.0TeV)","WprimeToWZToWlepZlep_narrow_2000_TuneCP5_13TeV-madgraph-pythia8",2.967E-4},
        SignalInfo{"W' (2.5TeV)","WprimeToWZToWlepZlep_narrow_2500_TuneCP5_13TeV-madgraph-pythia8",8.548E-5},
        SignalInfo{"W' (3.0TeV)","WprimeToWZToWlepZlep_narrow_3000_TuneCP5_13TeV-madgraph-pythia8",2.680E-5},
        SignalInfo{"W' (3.5TeV)","WprimeToWZToWlepZlep_narrow_3500_TuneCP5_13TeV-madgraph-pythia8",8.754E-6},
        SignalInfo{"W' (4.0TeV)","WprimeToWZToWlepZlep_narrow_4000_TuneCP5_13TeV-madgraph-pythia8",2.893E-6},
        SignalInfo{"W' (4.5TeV)","WprimeToWZToWlepZlep_narrow_4500_TuneCP5_13TeV-madgraph-pythia8",9.543E-7},
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


  std::unordered_map<int, std::vector<SignalInfo>> SignalSamples = {
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
    { "HDistl1l2_A","dR eee#nu;dR(e_{1},e_{2}) (cm);Event count"},
    { "HDistl1l2_B","dR ee#mu#nu;dR(e_{1},e_{2}) (cm);Event count"},
    { "HDistl1l2_C","dR #mu#mue#nu;dR(#mu_{1},#mu_{2}) (cm);Event count"},
    { "HDistl1l2_D","dR #mu#mue#nu;dR(#mu_{1},#mu_{2}) (cm);Event count"},
    { "HDistl1l2_+ABCD","dR(l_{1},l_{2}) lll#nu;dR (cm);Event count"},
    { "HDistl1l3_A","dR(e_{1},e_{3}) eee#nu;dR (cm);Event count"},
    { "HDistl1l3_B","dR(e_{1},e_{3}) ee#mu#nu;dR (cm);Event count"},
    { "HDistl1l3_C","dR(#mu_{1},#mu_{3}) #mu#mue#nu;dR (cm);Event count"},
    { "HDistl1l3_D","dR(#mu_{1},#mu_{3}) #mu#mue#nu;dR (cm);Event count"},
    { "HDistl1l3_+ABCD","dR(l_{1},l_{3}) lll#nu;dR (cm);Event count"},
    { "HDistl2l3_A","dR(e_{2},e_{3}) eee#nu;dR (cm);Event count"},
    { "HDistl2l3_B","dR(e_{2},e_{3}) ee#mu#nu;dR (cm);Event count"},
    { "HDistl2l3_C","dR(#mu_{2},#mu_{3}) #mu#mue#nu;dR (cm);Event count"},
    { "HDistl2l3_D","dR(#mu_{2},#mu_{3}) #mu#mue#nu;dR (cm);Event count"},
    { "HDistl2l3_+ABCD","dR(l_{2},l_{3}) lll#nu;dR (cm);Event count"},
    { "HWZDist_A","dr(W,Z) eee#nu;dR (cm);Event count"},
    { "HWZDist_B","dr(W,Z) ee#mu#nu;dR (cm);Event count"},
    { "HWZDist_C","dr(W,Z) ee#mu#nu;dR (cm);Event count"},
    { "HWZDist_D","dr(W,Z) #mu#mu#mu#nu;dR (cm);Event count"},
    { "HWZDist_+ABCD","dR(W,Z) lll#nu;dR (cm);Event count"},
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
    { "HEtal1_A", "#eta_{e1} eee#nu;Eta e_{1};Event count"},
    { "HEtal1_B", "#eta_{e1} ee#mu#nu;Eta;Event count"},
    { "HEtal1_C", "#eta_{#mu1} #mu#mue#nu;Eta;Event count"},
    { "HEtal1_D", "#eta_{#mu1} #mu#mu#mu#nu;Eta;Event count"},
    { "HEtal1_+ABCD ", "Eta_{l1} lll#nu;Eta e_{2};Event count"},
    { "HEtal2_A", "#eta_{e2} eee#nu;Eta;Event count"},
    { "HEtal2_B", "#eta_{e2} ee#mu#nu;Eta;Event count"},
    { "HEtal2_C", "#eta_{#mu2} #mu#mue#nu;Eta;Event count"},
    { "HEtal2_D", "#eta_{#mu2} #mu#mu#mu#nu;Eta;Event count"},
    { "HEtal2_+ABCD ", "Eta_{l2} lll#nu;Eta;Event count"},
    { "HEtal3_A", "#eta_{e3} eee#nu;Eta e_{3};Event count"},
    { "HEtal3_B", "#eta_{#mu} ee#mu#nu;Eta #mu;Event count"},
    { "HEtal3_C", "#eta_{e} #mu#mue#nu;Eta e;Event count"},
    { "HEtal3_D", "#eta_{#mu3} #mu#mu#mu#nu;Eta #mu_{3};Event count"},
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
    const Float_t LegendSpace = 10.; // Log Scale ;/
    if ( maxY<0 ) {
      TH1 *last = (TH1*)hss->GetStack()->Last();
      maxY = getMaxY(last);
    }
    hss->SetMaximum(maxY * LegendSpace);
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
      for(uint i = 0; i < hratio->GetN(); i++){
        hratio->SetPointEXhigh(i,0);
        hratio->SetPointEXlow(i,0);
      }
      hratio->SetMarkerColor(kBlack);
      hratio->SetMarkerStyle(20);
      hratio->SetMarkerSize(.5);
      hratio->SetLineWidth(1);
      const int font = 43;
      const float fontSize = 17.;
      const float labelSize = 0.17;
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
      hratio->SetMinimum(0.1);
      hratio->SetMaximum(1.9);
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
      for (const auto ch: hName.substr(hName.find("_+")+2)) {
        if ( ch == std::string("_") ) break;
        chn.emplace_back(Form("%c",ch));
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
    h = static_cast<TH1*>(h->Clone());
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


  auto getBGStack = [&](int yr, std::string hname, TLegend* legend = NULL){
    THStack* hstck = new THStack();
    Int_t prevColor = -1;
    for (auto BGN: BgNames[yr]) {
      auto h = getHistoFromFile(Form("%d/%s",yr,BGN.folderName.c_str()),hname);
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




  std::function<void(const int&, const int&)>
    printDataCard = [&] (const int& year, const int& wpmass) {

    const char* fromHisto = "HMassWZ_SR1";

    std::unordered_map<std::string,std::string> channels = {
      {"A", "eee"},
      {"B", "eemu"},
      {"C", "mumue"},
      {"D", "mumumu"},
    };

    ofstream dcFile;
    dcFile.open(Form("plots/%d/%d_%d_DataCard.txt", year, year, wpmass));

    std::unordered_map<Int_t, Float_t> lumiSyst = {
      { 2016, 0.025 }, /*AN2018_298_v14*/
      { 2017, 0.023 },
      { 2018, 0.025 },
    };

    std::string rootFilename = Form("CombineFile_%d_%d.root",year,wpmass);
    TFile* fCombine = TFile::Open(rootFilename.c_str(),"UPDATE");

    std::string DYSample = "DYJetsToLL";

    std::string bin0 = "bin\t";
    std::string obs  = "observation\t";
    std::string bin1 = "bin\t";
    std::string processn = "process\t";
    std::string process  = "process\t";
    std::string rate     = "rate\t";
    std::string unc1     = "lumi_13TeV\tlnN\t";
    std::vector<std::string> systEl = { "ElTrigger","ElID" };
    std::vector<std::string> systMu = { "MuTrigger","MuID" };
    std::vector<std::string> systK = { "EWK","QCD" };
    std::string unc2     = "CMS_ElTrigger\tshape\t";
    std::string unc3     = "CMS_MuTrigger\tshape\t";
    std::string unc4     = "CMS_ElID\tshape\t";
    std::string unc5     = "CMS_MuID\tshape\t";
    std::string unc6     = "CMS_KFactorQCD\tshape\t";
    std::string unc7     = "CMS_KFactorEWK\tshape\t";

    std::function<void(const std::string&, const std::string&, const float&)>
    saveHisto = [&] (const std::string& sampleName,
                     const std::string& ch,
                     const float& xsec) {

      std::function<void(const std::string&,
                         const std::string&,
                         const std::string&)>
      saveUpDown = [&] (std::string folder, std::string hname, std::string s/*ystematic*/){
        auto hup = getHistoFromFile(folder.c_str(),Form("%s_Up",hname.c_str()));
        applyLumiSF(hup, folder.c_str(), xsec);
        hup->Write(std::string(sampleName+Form("_CMS_%sUp",s.c_str())).c_str());
        auto hdown = getHistoFromFile(folder.c_str(),Form("%s_Down",hname.c_str()));
        applyLumiSF(hdown, folder.c_str(), xsec);
        hup->Write(std::string(sampleName+Form("_CMS_%sDown",s.c_str())).c_str());
      };

      std::string folderName = Form("%d/%s",year,sampleName.c_str());
      std::string hname;
      if(ch == "A"){
        for (auto s: systEl) {
          hname = Form("%s_%s_%s",fromHisto,ch.c_str(),s.c_str());
          saveUpDown(folderName,hname,s);
        }
      } else if (ch == "B" or ch == "C"){
        for (auto s: systEl) {
          hname = Form("%s_%s_%s",fromHisto,ch.c_str(),s.c_str());
          saveUpDown(folderName,hname,s);
        }
        for (auto s: systMu) {
          hname = Form("%s_%s_%s",fromHisto,ch.c_str(),s.c_str());
          saveUpDown(folderName,hname,s);
        }
      } else if (ch == "D"){
        for (auto s: systMu) {
          hname = Form("%s_%s_%s",fromHisto,ch.c_str(),s.c_str());
          saveUpDown(folderName,hname,s);
        }
      }

      if(folderName.find(DYSample.c_str()) != std::string::npos){
        for (auto s: systK) {
          hname = Form("%s_%s_%s",fromHisto,ch.c_str(),Form("KFactor_%s",s.c_str()));
          saveUpDown(folderName,hname,Form("KFactor%s",s.c_str()));
        }
      }

    };

    for(auto ch: channels){

      fCombine->cd("/");
      fCombine->mkdir(ch.second.c_str());
      fCombine->cd(ch.second.c_str());

      SignalInfo signal = SignalSamples[year][SignalPos[wpmass]];
      TH1* hsig = getHistoFromFile(Form("%d/%s",year,signal.folderName.c_str()),Form("%s_%s_Central",fromHisto,ch.first.c_str()));
      applyLumiSF(hsig, Form("%d/%s",year,signal.folderName.c_str()), signal.xsec);
      hsig->Write(Form("Wprime%d",wpmass));

      saveHisto(signal.folderName, ch.first, signal.xsec);

      bin1 += ch.second + "\t";
      processn += "0\t";
      process += Form("Wprime%d\t",wpmass);
      rate += Form("%.2f\t",hsig->Integral());
      unc1 += "--\t";
      unc2 += "--\t";
      unc3 += "--\t";
      unc4 += "--\t";
      unc5 += "--\t";
      unc6 += "--\t";
      unc7 += "--\t";

      auto hdata = getHistoFromFile(Form("%d/%s",year,DataSampleNames[year].c_str()),
                                    Form("%s_%s",fromHisto,ch.first.c_str()));
      bin0 += Form("%s\t",ch.second.c_str());
      obs += Form("%.2f\t",hdata->Integral());
      hdata->Write("data_obs");

      int counter = 1;
      for (auto BGN: BgNames[year]) {
        bin1 += ch.second + "\t";
        processn += Form("%d\t",counter);
        process += Form("%s\t",BGN.folderName.c_str());
        std::string fname = Form("%d/%s",year,BGN.folderName.c_str());
        auto h = getHistoFromFile(fname.c_str(),Form("%s_%s_Central",fromHisto,ch.first.c_str()));
        applyLumiSF(h, Form("%d/%s",year,BGN.folderName.c_str()), BGN.xsec);
        rate += Form("%.2f\t",h->Integral());
        h->Write(BGN.folderName.c_str());
        unc1 += Form("%.4f\t",1. + lumiSyst[year]);

        if (ch.first == "A") {
          unc2 += "1.0\t";
          unc3 += "-\t";
          unc4 += "1.0\t";
          unc5 += "-\t";
          saveHisto(BGN.folderName.c_str(),ch.first,BGN.xsec);
        } else if (ch.first == "B") {
          unc2 += "1.0\t";
          unc3 += "1.0\t";
          unc4 += "1.0\t";
          unc5 += "1.0\t";
          saveHisto(BGN.folderName.c_str(),ch.first,BGN.xsec);
        } else if (ch.first == "C") {
          unc2 += "1.0\t";
          unc3 += "1.0\t";
          unc4 += "1.0\t";
          unc5 += "1.0\t";
          saveHisto(BGN.folderName.c_str(),ch.first,BGN.xsec);
        } else if (ch.first == "D") {
          unc2 += "-\t";
          unc3 += "1.0\t";
          unc4 += "-\t";
          unc5 += "1.0\t";
          saveHisto(BGN.folderName.c_str(),ch.first,BGN.xsec);
        }

        if (BGN.folderName.find(DYSample.c_str()) != std::string::npos){
          unc6 += "1.0\t";
          unc7 += "1.0\t";
          saveHisto(BGN.folderName.c_str(),ch.first,BGN.xsec);
        } else {
          unc6 += "-\t";
          unc7 += "-\t";
        }
        ++counter;
      }
    }


    dcFile << "imax\t4\njmax\t" << BgNames[year].size() << "\nkmax\t7\n";
    dcFile << "------------\n";
    dcFile << Form("shapes * * %s $CHANNEL/$PROCESS $CHANNEL/$PROCESS_$SYSTEMATIC\n",rootFilename.c_str());
    dcFile << bin0 << std::endl;
    dcFile << obs << std::endl;
    dcFile << "------------\n";
    dcFile << bin1 << std::endl << process << std::endl << processn << std::endl << rate << std::endl;
    dcFile << "------------\n";
    dcFile << unc1 << std::endl;
    dcFile << unc2 << std::endl;
    dcFile << unc3 << std::endl;
    dcFile << unc4 << std::endl;
    dcFile << unc5 << std::endl;
    dcFile << unc6 << std::endl;
    dcFile << unc7 << std::endl;

    fCombine->Close();

  };

  for(auto n: SignalPos){
    printDataCard(2016,n.first);
  }


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


  std::function<void(const Int_t, std::vector<std::string>,bool)>
    canvasStacked = [&] (const Int_t WpMass,
                         std::vector<std::string> names,
                         bool includeData = true) {

    const Int_t npads = names.size();

    std::function<void(std::string&)> removeCentral = [&] (std::string& s){
      if (s.find("_Central") != std::string::npos){
        s.erase(s.find("_Central"),8);

      } else if (s.find("_Up") != std::string::npos) {
        s.erase(s.find("_Up"),3);
      } else if ( s.find("_Down") != std::string::npos){
        s.erase(s.find("_Down"),5);
      }
      if (s.find("_Central") != std::string::npos)  removeCentral(s);
      if (s.find("_Up")      != std::string::npos)  removeCentral(s);
      if (s.find("_Down")    != std::string::npos)  removeCentral(s);
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

    Int_t j = 1;

    std::string pngname;

    for (auto hName : names) {
      int year = std::stoi(hName.substr(0,4));
      hName = hName.substr(5,hName.size());
      SignalInfo signal = SignalSamples[year][SignalPos[WpMass]];
      Int_t r = (j-1)%npads;
      c1->cd(r+1);
      std::string dataHName = hName;
      removeCentral(dataHName);
      pngname += dataHName + "_";

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
      mainPad->SetLogy();
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

      THStack *hs = new THStack("hs","");
      hs = getBGStack(year,hName,legend);
      TH1F* last = static_cast<TH1F*>(hs->GetStack()->Last());

      auto hsig = getHistoFromFile(Form("%d/%s",year,signal.folderName.c_str()),hName);
      applyLumiSF(hsig, Form("%d/%s",year,signal.folderName.c_str()), signal.xsec);
      hsig->SetTitle(signal.legendName.c_str());
      legend->AddEntry( hsig,signal.legendName.c_str(),"L");
      hsig->SetLineColor(kBlack);
      hsig->SetLineWidth(3);
      hsig->SetFillColor(0);
      std::string labelIdx = dataHName;
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
      if (hName.find("CR1_") != std::string::npos) {
        hs->SetTitle((Labels[labelIdx].c_str() + std::string(" CR")).c_str());
      }

      legend->SetBorderSize(0);
      gStyle->SetOptStat(0);

      mainPad->cd();
      hs->Draw("HIST");
      hsig->Draw("HIST SAME");
      double maxx = last->GetBinLowEdge(last->FindLastBinAbove(0.25)+1);
      double minx = last->GetBinLowEdge(last->FindFirstBinAbove(0.25)-1);
      hs->GetHistogram()->GetXaxis()->SetRangeUser(minx,maxx);
      fixYRange(hs,getMaxY(last));

      if (includeData) {
        auto subPad = new TPad(Form("mainPad_%s",hName.c_str()),"subPad",0.,0.,1.,0.25);
        c1->cd(r+1);
        subPad->Draw();
        subPad->SetLeftMargin(leftMargin);
        subPad->SetRightMargin(rightMargin);
        subPad->SetTopMargin(1e-3);
        subPad->SetBottomMargin(bottomMargin);

        auto hdata = getHistoFromFile(Form("%d/%s",year,DataSampleNames[year].c_str()),dataHName);
        mainPad->cd();
        hdata->SetMarkerStyle(kFullCircle);
        fixYRange(hs,getMaxY(hdata));
        hdata->Draw("SAME P");
        TH1F* herror = getErrorHisto(hs);
        herror->Draw("SAME E2");
        legend->AddEntry(hdata, Form("Data%d",year));

        auto hcdata = getRatio(hdata,hs);
        subPad->cd();
        subPad->SetGrid();
        hcdata->Draw("AP");
        subPad->SetFrameLineWidth(1);
        hcdata->GetXaxis()->SetRangeUser(minx,maxx);
      }
      ++j;

      mainPad->cd();
      legend->Draw();
      if( r+1 == npads ){
        c1->Print(Form("plots/%d/%s_%sM%d.png",year,fileLabel.c_str(),pngname.c_str(),WpMass));
        c1->Write(Form("%d_%s_%s_Wprime%d_Data",year,fileLabel.c_str(),hName.c_str(),WpMass));
      }
    }

    delete c1;

  };

  std::vector<int> pyear = { 2016, 2017, 2018 };
  std::vector<std::string> chs = { "A","B","C","D" };
  std::vector<std::string> hints = {
    "HMassZ_CR1_+ABCD","HMassWZ_CR1_+ABCD",
    "HMassTW_CR1_+ABCD","HMetPt_CR1_+ABCD"
  };

  for(auto yr: pyear){
    std::vector<std::string> hNames;
    for(auto h : hints){
      for(auto ch: chs){
        hNames.emplace_back(Form("%d/%s_CR1_%s",yr,h.c_str(),ch.c_str()));
        if(hNames.size() == 4){
          canvasStacked(600,hNames,true);
          hNames.clear();
        }
      }
    }
  }


  for (auto& item: SignalSamples) {

    const int year = item.first;

    // plotPunziSignificance(year);
    // plotSelectionRatio(year);
    // plotHLTSelectionRatio(year);

    for (auto signal: item.second) {

      const Int_t WpMass = getWpMassFromName(signal.folderName);

      // printCutFlowStack(year,signal);
      // printH2Comb(year, signal, "HPtWPtZ_SR_+ABCD");
      // printH2Comb(year, signal, "HDeltaRPtZ_SR_+ABCD");
      // printH2Comb(year, signal, "HLtMWZ_SR_+ABCD");

    }
  }
}
