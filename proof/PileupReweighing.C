// It requires the Pileup profiles from data
// Named as: Form("PileupHistogram-goldenJSON-13tev-%d-69200ub.root",year)
// and WprimeHistos.root/Year/SampleName

Int_t PileupReweighing(){

  TFile *mcFile = TFile::Open("WprimeHistos_ANv5Mixv2v9.root","READ");

  std::multimap<int, std::vector<std::string>> BgNamesUL =
    {
      {
        2016,
        {
          "DYJetsToLL_M-50_HT-100to200_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8",
          "DYJetsToLL_M-50_HT-1200to2500_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8",
          "DYJetsToLL_M-50_HT-200to400_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8",
          "DYJetsToLL_M-50_HT-2500toInf_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8",
          "DYJetsToLL_M-50_HT-400to600_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8",
          "DYJetsToLL_M-50_HT-600to800_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8",
          "DYJetsToLL_M-50_HT-70to100_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8",
          "DYJetsToLL_M-50_HT-800to1200_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8",
          "GluGluToContinToZZTo2e2mu_TuneCP5_13TeV-mcfm701-pythia8",
          "GluGluToContinToZZTo2e2nu_TuneCP5_13TeV-mcfm701-pythia8",
          "GluGluToContinToZZTo2e2tau_TuneCP5_13TeV-mcfm701-pythia8",
          "GluGluToContinToZZTo2mu2nu_TuneCP5_13TeV-mcfm701-pythia8",
          "GluGluToContinToZZTo2mu2tau_TuneCP5_13TeV-mcfm701-pythia8",
          "GluGluToContinToZZTo4e_TuneCP5_13TeV-mcfm701-pythia8",
          "GluGluToContinToZZTo4mu_TuneCP5_13TeV-mcfm701-pythia8",
          "GluGluToContinToZZTo4tau_TuneCP5_13TeV-mcfm701-pythia8",
          "ST_s-channel_4f_leptonDecays_TuneCP5_13TeV-amcatnlo-pythia8",
          "ST_t-channel_antitop_5f_InclusiveDecays_TuneCP5_13TeV-powheg-pythia8",
          "ST_t-channel_top_5f_InclusiveDecays_TuneCP5_13TeV-powheg-pythia8",
          "ST_tW_antitop_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8",
          "ST_tW_top_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8",
          "TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8",
          "TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8",
          "TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8",
          "TTZToLLNuNu_M-10_TuneCP5_13TeV-amcatnlo-pythia8",
          "WWW_4F_TuneCP5_13TeV-amcatnlo-pythia8",
          "WWZ_4F_TuneCP5_13TeV-amcatnlo-pythia8",
          "WZTo3LNu_TuneCP5_13TeV-amcatnloFXFX-pythia8",
          "WZTo3LNu_mllmin4p0_TuneCP5_13TeV-powheg-pythia8",
          "WZZ_TuneCP5_13TeV-amcatnlo-pythia8",
          "WprimeToWZToWlepZlep_narrow_M1000_TuneCP5_13TeV-madgraph-pythia8",
          "WprimeToWZToWlepZlep_narrow_M1200_TuneCP5_13TeV-madgraph-pythia8",
          "WprimeToWZToWlepZlep_narrow_M1400_TuneCP5_13TeV-madgraph-pythia8",
          "WprimeToWZToWlepZlep_narrow_M1600_TuneCP5_13TeV-madgraph-pythia8",
          "WprimeToWZToWlepZlep_narrow_M1800_TuneCP5_13TeV-madgraph-pythia8",
          "WprimeToWZToWlepZlep_narrow_M2000_TuneCP5_13TeV-madgraph-pythia8",
          "WprimeToWZToWlepZlep_narrow_M2500_TuneCP5_13TeV-madgraph-pythia8",
          "WprimeToWZToWlepZlep_narrow_M3000_TuneCP5_13TeV-madgraph-pythia8",
          "WprimeToWZToWlepZlep_narrow_M3500_TuneCP5_13TeV-madgraph-pythia8",
          "WprimeToWZToWlepZlep_narrow_M4000_TuneCP5_13TeV-madgraph-pythia8",
          "WprimeToWZToWlepZlep_narrow_M4500_TuneCP5_13TeV-madgraph-pythia8",
          "WprimeToWZToWlepZlep_narrow_M600_TuneCP5_13TeV-madgraph-pythia8",
          "WprimeToWZToWlepZlep_narrow_M800_TuneCP5_13TeV-madgraph-pythia8",
          "ZGToLLG_01J_5f_TuneCP5_13TeV-amcatnloFXFX-pythia8",
          "ZZTo4L_M-1toInf_TuneCP5_13TeV_powheg_pythia8",
          "ZZZ_TuneCP5_13TeV-amcatnlo-pythia8",
          "ttZJets_TuneCP5_13TeV_madgraphMLM_pythia8",
        }
      },
      {
        2017,
        {
          "DYJetsToLL_M-50_HT-100to200_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8",
          "DYJetsToLL_M-50_HT-1200to2500_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8",
          "DYJetsToLL_M-50_HT-200to400_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8",
          "DYJetsToLL_M-50_HT-2500toInf_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8",
          "DYJetsToLL_M-50_HT-400to600_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8",
          "DYJetsToLL_M-50_HT-600to800_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8",
          "DYJetsToLL_M-50_HT-70to100_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8",
          "DYJetsToLL_M-50_HT-800to1200_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8",
          "GluGluToContinToZZTo2e2mu_TuneCP5_13TeV-mcfm701-pythia8",
          "GluGluToContinToZZTo2e2nu_TuneCP5_13TeV-mcfm701-pythia8",
          "GluGluToContinToZZTo2e2tau_TuneCP5_13TeV-mcfm701-pythia8",
          "GluGluToContinToZZTo2mu2nu_TuneCP5_13TeV-mcfm701-pythia8",
          "GluGluToContinToZZTo2mu2tau_TuneCP5_13TeV-mcfm701-pythia8",
          "GluGluToContinToZZTo4e_TuneCP5_13TeV-mcfm701-pythia8",
          "GluGluToContinToZZTo4mu_TuneCP5_13TeV-mcfm701-pythia8",
          "GluGluToContinToZZTo4tau_TuneCP5_13TeV-mcfm701-pythia8",
          "ST_s-channel_4f_leptonDecays_TuneCP5_13TeV-amcatnlo-pythia8",
          "ST_t-channel_antitop_4f_InclusiveDecays_TuneCP5_13TeV-powheg-madspin-pythia8",
          "ST_t-channel_top_4f_InclusiveDecays_TuneCP5_13TeV-powheg-madspin-pythia8",
          "ST_tW_antitop_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8",
          "ST_tW_top_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8",
          "TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8",
          "TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8",
          "TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8",
          "TTZToLLNuNu_M-10_TuneCP5_13TeV-amcatnlo-pythia8",
          "WWW_4F_TuneCP5_13TeV-amcatnlo-pythia8",
          "WWZ_4F_TuneCP5_13TeV-amcatnlo-pythia8",
          "WZTo3LNu_mllmin01_NNPDF31_TuneCP5_13TeV_powheg_pythia8",
          "WZTo3LNu_mllmin4p0_TuneCP5_13TeV-powheg-pythia8",
          "WZZ_TuneCP5_13TeV-amcatnlo-pythia8",
          "WprimeToWZToWlepZlep_narrow_M1000_TuneCP5_13TeV-madgraph-pythia8",
          "WprimeToWZToWlepZlep_narrow_M1200_TuneCP5_13TeV-madgraph-pythia8",
          "WprimeToWZToWlepZlep_narrow_M1400_TuneCP5_13TeV-madgraph-pythia8",
          "WprimeToWZToWlepZlep_narrow_M1600_TuneCP5_13TeV-madgraph-pythia8",
          "WprimeToWZToWlepZlep_narrow_M1800_TuneCP5_13TeV-madgraph-pythia8",
          "WprimeToWZToWlepZlep_narrow_M2000_TuneCP5_13TeV-madgraph-pythia8",
          "WprimeToWZToWlepZlep_narrow_M2500_TuneCP5_13TeV-madgraph-pythia8",
          "WprimeToWZToWlepZlep_narrow_M3000_TuneCP5_13TeV-madgraph-pythia8",
          "WprimeToWZToWlepZlep_narrow_M3500_TuneCP5_13TeV-madgraph-pythia8",
          "WprimeToWZToWlepZlep_narrow_M4000_TuneCP5_13TeV-madgraph-pythia8",
          "WprimeToWZToWlepZlep_narrow_M600_TuneCP5_13TeV-madgraph-pythia8",
          "WprimeToWZToWlepZlep_narrow_M800_TuneCP5_13TeV-madgraph-pythia8",
          "ZGToLLG_01J_5f_TuneCP5_13TeV-amcatnloFXFX-pythia8",
          "ZZTo4L_M-1toInf_TuneCP5_13TeV_powheg_pythia8",
          "ZZZ_TuneCP5_13TeV-amcatnlo-pythia8",
          "ttZJets_TuneCP5_13TeV_madgraphMLM_pythia8",
        },
      },
      {
        2018,
        {
          "DYJetsToLL_M-50_HT-100to200_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8",
          "DYJetsToLL_M-50_HT-1200to2500_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8",
          "DYJetsToLL_M-50_HT-200to400_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8",
          "DYJetsToLL_M-50_HT-2500toInf_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8",
          "DYJetsToLL_M-50_HT-400to600_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8",
          "DYJetsToLL_M-50_HT-600to800_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8",
          "DYJetsToLL_M-50_HT-70to100_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8",
          "DYJetsToLL_M-50_HT-800to1200_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8",
          "GluGluToContinToZZTo2e2mu_TuneCP5_13TeV-mcfm701-pythia8",
          "GluGluToContinToZZTo2e2nu_TuneCP5_13TeV-mcfm701-pythia8",
          "GluGluToContinToZZTo2e2tau_TuneCP5_13TeV-mcfm701-pythia8",
          "GluGluToContinToZZTo2mu2nu_TuneCP5_13TeV-mcfm701-pythia8",
          "GluGluToContinToZZTo2mu2tau_TuneCP5_13TeV-mcfm701-pythia8",
          "GluGluToContinToZZTo4e_TuneCP5_13TeV-mcfm701-pythia8",
          "GluGluToContinToZZTo4mu_TuneCP5_13TeV-mcfm701-pythia8",
          "GluGluToContinToZZTo4tau_TuneCP5_13TeV-mcfm701-pythia8",
          "ST_s-channel_4f_leptonDecays_TuneCP5_13TeV-amcatnlo-pythia8",
          "ST_t-channel_antitop_4f_InclusiveDecays_TuneCP5_13TeV-powheg-madspin-pythia8",
          "ST_t-channel_top_4f_InclusiveDecays_TuneCP5_13TeV-powheg-madspin-pythia8",
          "ST_tW_antitop_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8",
          "ST_tW_top_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8",
          "TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8",
          "TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8",
          "TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8",
          "TTZToLLNuNu_M-10_TuneCP5_13TeV-amcatnlo-pythia8",
          "WWW_4F_TuneCP5_13TeV-amcatnlo-pythia8",
          "WWZ_4F_TuneCP5_13TeV-amcatnlo-pythia8",
          "WZTo3LNu_mllmin4p0_TuneCP5_13TeV-powheg-pythia8",
          "WZZ_TuneCP5_13TeV-amcatnlo-pythia8",
          "WprimeToWZToWlepZlep_narrow_M1000_TuneCP5_13TeV-madgraph-pythia8",
          "WprimeToWZToWlepZlep_narrow_M1200_TuneCP5_13TeV-madgraph-pythia8",
          "WprimeToWZToWlepZlep_narrow_M1400_TuneCP5_13TeV-madgraph-pythia8",
          "WprimeToWZToWlepZlep_narrow_M1600_TuneCP5_13TeV-madgraph-pythia8",
          "WprimeToWZToWlepZlep_narrow_M1800_TuneCP5_13TeV-madgraph-pythia8",
          "WprimeToWZToWlepZlep_narrow_M2000_TuneCP5_13TeV-madgraph-pythia8",
          "WprimeToWZToWlepZlep_narrow_M2500_TuneCP5_13TeV-madgraph-pythia8",
          "WprimeToWZToWlepZlep_narrow_M3000_TuneCP5_13TeV-madgraph-pythia8",
          "WprimeToWZToWlepZlep_narrow_M3500_TuneCP5_13TeV-madgraph-pythia8",
          "WprimeToWZToWlepZlep_narrow_M4000_TuneCP5_13TeV-madgraph-pythia8",
          "WprimeToWZToWlepZlep_narrow_M4500_TuneCP5_13TeV-madgraph-pythia8",
          "WprimeToWZToWlepZlep_narrow_M600_TuneCP5_13TeV-madgraph-pythia8",
          "WprimeToWZToWlepZlep_narrow_M800_TuneCP5_13TeV-madgraph-pythia8",
          "ZGToLLG_01J_5f_TuneCP5_13TeV-amcatnloFXFX-pythia8",
          "ZZTo4L_M-1toInf_TuneCP5_13TeV_powheg_pythia8",
          "ZZZ_TuneCP5_13TeV-amcatnlo-pythia8",
          "ttZJets_TuneCP5_13TeV_madgraphMLM_pythia8",
        },
      }
    };

  std::multimap<int, std::vector<std::string>> BgNames =
    {
      {
        2016,
        {
          "DYJetsToLL_M-50_HT-100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
          "DYJetsToLL_M-50_HT-1200to2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
          "DYJetsToLL_M-50_HT-200to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
          "DYJetsToLL_M-50_HT-2500toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
          "DYJetsToLL_M-50_HT-400to600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
          "DYJetsToLL_M-50_HT-600to800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
          "DYJetsToLL_M-50_HT-800to1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
          "GluGluToContinToZZTo2e2mu_13TeV_MCFM701_pythia8",
          "GluGluToContinToZZTo4e_13TeV_MCFM701_pythia8",
          "GluGluToContinToZZTo4mu_13TeV_MCFM701_pythia8",
          "GluGluToContinToZZTo4tau_13TeV_MCFM701_pythia8",
          "ST_s-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1",
          "ST_t-channel_antitop_4f_inclusiveDecays_13TeV-powhegV2-madspin-pythia8_TuneCUETP8M1",
          "ST_t-channel_top_4f_inclusiveDecays_13TeV-powhegV2-madspin-pythia8_TuneCUETP8M1",
          "ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1",
          "ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1",
          "TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
          "TTWJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8",
          "TTZToLL_M-1to10_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
          "WWW_4F_TuneCUETP8M1_13TeV-amcatnlo-pythia8",
          "WWZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8",
          "WZTo3LNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8",
          "WZTo3LNu_TuneCUETP8M1_13TeV-powheg-pythia8",
          "WZTo3LNu_mllmin01_13TeV-powheg-pythia8",
          "WZZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8",
          "WprimeToWZToWlepZlep_narrow_M-1000_13TeV-madgraph",
          "WprimeToWZToWlepZlep_narrow_M-1200_13TeV-madgraph",
          "WprimeToWZToWlepZlep_narrow_M-1400_13TeV-madgraph",
          "WprimeToWZToWlepZlep_narrow_M-1600_13TeV-madgraph",
          "WprimeToWZToWlepZlep_narrow_M-1800_13TeV-madgraph",
          "WprimeToWZToWlepZlep_narrow_M-2000_13TeV-madgraph",
          "WprimeToWZToWlepZlep_narrow_M-2500_13TeV-madgraph",
          "WprimeToWZToWlepZlep_narrow_M-3000_13TeV-madgraph",
          "WprimeToWZToWlepZlep_narrow_M-3500_13TeV-madgraph",
          "WprimeToWZToWlepZlep_narrow_M-4000_13TeV-madgraph",
          "WprimeToWZToWlepZlep_narrow_M-4500_13TeV-madgraph",
          "WprimeToWZToWlepZlep_narrow_M-600_13TeV-madgraph",
          "WprimeToWZToWlepZlep_narrow_M-800_13TeV-madgraph",
          "ZGTo2LG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8",
          "ZZTo4L_13TeV_powheg_pythia8",
          "ZZZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8",
          "ttZJets_13TeV_madgraphMLM-pythia8"
        }
      },
      {
        2017,
        {
          "DYJetsToLL_M-50_HT-100to200_TuneCP5_13TeV-madgraphMLM-pythia8",
          "DYJetsToLL_M-50_HT-1200to2500_TuneCP5_13TeV-madgraphMLM-pythia8",
          "DYJetsToLL_M-50_HT-200to400_TuneCP5_13TeV-madgraphMLM-pythia8",
          "DYJetsToLL_M-50_HT-2500toInf_TuneCP5_13TeV-madgraphMLM-pythia8",
          "DYJetsToLL_M-50_HT-400to600_TuneCP5_13TeV-madgraphMLM-pythia8",
          "DYJetsToLL_M-50_HT-600to800_TuneCP5_13TeV-madgraphMLM-pythia8",
          "DYJetsToLL_M-50_HT-800to1200_TuneCP5_13TeV-madgraphMLM-pythia8",
          "DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8_NEWPMX",
          "GluGluToContinToZZTo2e2mu_13TeV_MCFM701_pythia8",
          "GluGluToContinToZZTo2e2mu_13TeV_TuneCP5_MCFM701_pythia8",
          "GluGluToContinToZZTo4e_13TeV_MCFM701_pythia8",
          "GluGluToContinToZZTo4e_13TeV_TuneCP5_MCFM701_pythia8",
          "GluGluToContinToZZTo4mu_13TeV_MCFM701_pythia8",
          "GluGluToContinToZZTo4mu_13TeV_TuneCP5_MCFM701_pythia8",
          "GluGluToContinToZZTo4tau_13TeV_MCFM701_pythia8",
          "GluGluToContinToZZTo4tau_13TeV_TuneCP5_MCFM701_pythia8",
          "ST_s-channel_4f_leptonDecays_TuneCP5_13TeV-amcatnlo-pythia8",
          "ST_t-channel_antitop_4f_inclusiveDecays_TuneCP5_13TeV-powhegV2-madspin-pythia8",
          "ST_t-channel_top_4f_inclusiveDecays_TuneCP5_13TeV-powhegV2-madspin-pythia8",
          "ST_tW_antitop_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8",
          "ST_tW_top_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8",
          "TTJets_DiLept_TuneCP5_13TeV-madgraphMLM-pythia8",
          "TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8_NEWPMX",
          "TTTo2L2Nu_TuneCP5_PSweights_13TeV-powheg-pythia8",
          "TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8",
          "TTZToLL_M-1to10_TuneCP5_13TeV-amcatnlo-pythia8",
          "WWW_4F_TuneCP5_13TeV-amcatnlo-pythia8",
          "WWZ_4F_TuneCP5_13TeV-amcatnlo-pythia8",
          "WZTo3LNu_13TeV-powheg-pythia8",
          "WZTo3LNu_TuneCP5_13TeV-amcatnloFXFX-pythia8",
          "WZTo3LNu_mllmin01_NNPDF31_TuneCP5_13TeV_powheg_pythia8",
          "WZZ_TuneCP5_13TeV-amcatnlo-pythia8",
          "WprimeToWZToWlepZlep_narrow_M1000_TuneCP5_13TeV-madgraph-pythia8",
          "WprimeToWZToWlepZlep_narrow_M1200_TuneCP5_13TeV-madgraph-pythia8",
          "WprimeToWZToWlepZlep_narrow_M1400_TuneCP5_13TeV-madgraph-pythia8",
          "WprimeToWZToWlepZlep_narrow_M1600_TuneCP5_13TeV-madgraph-pythia8",
          "WprimeToWZToWlepZlep_narrow_M1800_TuneCP5_13TeV-madgraph-pythia8",
          "WprimeToWZToWlepZlep_narrow_M2000_TuneCP5_13TeV-madgraph-pythia8",
          "WprimeToWZToWlepZlep_narrow_M2500_TuneCP5_13TeV-madgraph-pythia8",
          "WprimeToWZToWlepZlep_narrow_M3000_TuneCP5_13TeV-madgraph-pythia8",
          "WprimeToWZToWlepZlep_narrow_M3500_TuneCP5_13TeV-madgraph-pythia8",
          "WprimeToWZToWlepZlep_narrow_M4000_TuneCP5_13TeV-madgraph-pythia8",
          "WprimeToWZToWlepZlep_narrow_M600_TuneCP5_13TeV-madgraph-pythia8",
          "WprimeToWZToWlepZlep_narrow_M800_TuneCP5_13TeV-madgraph-pythia8",
          "ZGToLLG_01J_5f_TuneCP5_13TeV-amcatnloFXFX-pythia8",
          "ZZTo4L_13TeV_powheg_pythia8",
          "ZZZ_TuneCP5_13TeV-amcatnlo-pythia8",
          "ttZJets_TuneCP5_13TeV_madgraphMLM_pythia8"
        }
      },
      {
        2018,
        {
          "DYJetsToLL_M-50_HT-100to200_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8",
          "DYJetsToLL_M-50_HT-1200to2500_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8",
          "DYJetsToLL_M-50_HT-200to400_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8",
          "DYJetsToLL_M-50_HT-2500toInf_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8",
          "DYJetsToLL_M-50_HT-400to600_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8",
          "DYJetsToLL_M-50_HT-600to800_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8",
          "DYJetsToLL_M-50_HT-800to1200_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8",
          "DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8",
          "GluGluToContinToZZTo2e2mu_13TeV_MCFM701_pythia8",
          "GluGluToContinToZZTo2e2mu_13TeV_TuneCP5_MCFM701_pythia8",
          "GluGluToContinToZZTo4e_13TeV_MCFM701_pythia8",
          "GluGluToContinToZZTo4e_13TeV_TuneCP5_MCFM701_pythia8",
          "GluGluToContinToZZTo4mu_13TeV_MCFM701_pythia8",
          "GluGluToContinToZZTo4mu_13TeV_TuneCP5_MCFM701_pythia8",
          "GluGluToContinToZZTo4tau_13TeV_MCFM701_pythia8",
          "GluGluToContinToZZTo4tau_13TeV_TuneCP5_MCFM701_pythia8",
          "ST_s-channel_4f_leptonDecays_TuneCP5_13TeV-amcatnlo-pythia8",
          "ST_tW_antitop_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8",
          "ST_tW_top_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8",
          "TTJets_DiLept_TuneCP5_13TeV-madgraphMLM-pythia8",
          "TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8",
          "TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8",
          "TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8",
          "TTZToLL_M-1to10_TuneCP5_13TeV-amcatnlo-pythia8",
          "WWW_4F_TuneCP5_13TeV-amcatnlo-pythia8",
          "WWZ_TuneCP5_13TeV-amcatnlo-pythia8",
          "WZTo3LNu_TuneCP5_13TeV-amcatnloFXFX-pythia8",
          "WZTo3LNu_TuneCP5_13TeV-powheg-pythia8",
          "WZTo3LNu_mllmin01_NNPDF31_TuneCP5_13TeV_powheg_pythia8",
          "WZZ_TuneCP5_13TeV-amcatnlo-pythia8",
          "WZ_TuneCP5_13TeV-pythia8",
          "WprimeToWZToWlepZlep_narrow_M1000_TuneCP5_13TeV-madgraph-pythia8",
          "WprimeToWZToWlepZlep_narrow_M1200_TuneCP5_13TeV-madgraph-pythia8",
          "WprimeToWZToWlepZlep_narrow_M1400_TuneCP5_13TeV-madgraph-pythia8",
          "WprimeToWZToWlepZlep_narrow_M1600_TuneCP5_13TeV-madgraph-pythia8",
          "WprimeToWZToWlepZlep_narrow_M1800_TuneCP5_13TeV-madgraph-pythia8",
          "WprimeToWZToWlepZlep_narrow_M2000_TuneCP5_13TeV-madgraph-pythia8",
          "WprimeToWZToWlepZlep_narrow_M2500_TuneCP5_13TeV-madgraph-pythia8",
          "WprimeToWZToWlepZlep_narrow_M3000_TuneCP5_13TeV-madgraph-pythia8",
          "WprimeToWZToWlepZlep_narrow_M3500_TuneCP5_13TeV-madgraph-pythia8",
          "WprimeToWZToWlepZlep_narrow_M4000_TuneCP5_13TeV-madgraph-pythia8",
          "WprimeToWZToWlepZlep_narrow_M600_TuneCP5_13TeV-madgraph-pythia8",
          "WprimeToWZToWlepZlep_narrow_M800_TuneCP5_13TeV-madgraph-pythia8",
          "ZGToLLG_01J_5f_TuneCP5_13TeV-amcatnloFXFX-pythia8",
          "ZZTo4L_13TeV_powheg_pythia8_TuneCP5",
          "ZZTo4L_TuneCP5_13TeV-amcatnloFXFX-pythia8",
          "ZZZ_TuneCP5_13TeV-amcatnlo-pythia8",
          "ttZJets_TuneCP5_13TeV_madgraphMLM_pythia8",
        }
      }
    };

  int year;
  auto list = new TList();
  list->SetName("PileupSFList");
  auto f = std::make_unique<TFile>("PileupWeights_UL.root","RECREATE");

  for( std::multimap<int,std::vector<std::string>>::iterator i = BgNamesUL.begin();
        i != BgNamesUL.end(); ++i) {
    year = (*i).first;

    TFile *dataFile = TFile::Open(Form("files/mc/%d/sf/PileupHistogram-goldenJSON-13tev-%d-69200ub-99bins.root",year,year));

    auto dataPileup = static_cast<TH1D*>(dataFile->Get("pileup"));
    TH1D* mcPileup;
    TH1D* weights;

    f->mkdir(Form("%d", year));

    for(auto& sample: (*i).second) {
      std::clog << Form("Processing: %d/%s:\n", year, sample.c_str());
      mcPileup = static_cast<TH1D*>(mcFile->Get(Form("%d/%s/HTruePileup",year,sample.c_str())));
      mcPileup = static_cast<TH1D*>(mcPileup->Clone());
      mcPileup->Scale(1./mcPileup->Integral());

      dataPileup = static_cast<TH1D*>(dataPileup->Clone());
      dataPileup->Scale(1./dataPileup->Integral());

      weights = static_cast<TH1D*>(dataPileup->Clone());
      weights->Divide(mcPileup);
      weights->SetName("lumiWeights");

      f->mkdir(Form("%d/%s", year, sample.c_str()));
      f->cd(Form("%d/%s", year, sample.c_str()));
      weights->Write();

      weights->SetName(Form("%s_%d",sample.c_str(),year));
      list->Add(weights);
    }
  }

  f->cd("/");
  list->Write("PileupSFList",1);

  return 0;

}
