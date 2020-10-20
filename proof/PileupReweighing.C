// It requires the Pileup profiles from data
// Named as: Form("PileupHistogram-goldenJSON-13tev-%d-69200ub.root",year)
// and WprimeHistos.root/Year/SampleName

Int_t PileupReweighing(){

  TFile *mcFile = TFile::Open("WprimeHistos.root","READ");

  std::multimap<int, std::vector<std::string>> BgNames =
    {
      {
        2016,
        {
          "DYJetsToLL_M-10to50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8",
          "DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
          "TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
          "WZTo3LNu_TuneCUETP8M1_13TeV-powheg-pythia8",
          "WprimeToWZToWlepZlep_narrow_M-600_13TeV-madgraph",
          "WprimeToWZToWlepZlep_narrow_M-800_13TeV-madgraph",
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
          "ZGTo2LG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8",
          "ZZTo4L_13TeV_powheg_pythia8"
        }
      },
      {
        2017,
        {
          "DYJetsToLL_M-10to50_TuneCP5_13TeV-madgraphMLM-pythia8",
          "DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8",
          "TTJets_DiLept_TuneCP5_13TeV-madgraphMLM-pythia8",
          "WZTo3LNu_TuneCP5_13TeV-amcatnloFXFX-pythia8",
          "WprimeToWZToWlepZlep_narrow_M600_TuneCP5_13TeV-madgraph-pythia8",
          "WprimeToWZToWlepZlep_narrow_M800_TuneCP5_13TeV-madgraph-pythia8",
          "WprimeToWZToWlepZlep_narrow_M1000_TuneCP5_13TeV-madgraph-pythia8",
          "WprimeToWZToWlepZlep_narrow_M1200_TuneCP5_13TeV-madgraph-pythia8",
          "WprimeToWZToWlepZlep_narrow_M1400_TuneCP5_13TeV-madgraph-pythia8",
          "WprimeToWZToWlepZlep_narrow_M1600_TuneCP5_13TeV-madgraph-pythia8",
          "WprimeToWZToWlepZlep_narrow_M1800_TuneCP5_13TeV-madgraph-pythia8"
          "WprimeToWZToWlepZlep_narrow_M2000_TuneCP5_13TeV-madgraph-pythia8",
          "WprimeToWZToWlepZlep_narrow_M2500_TuneCP5_13TeV-madgraph-pythia8",
          "WprimeToWZToWlepZlep_narrow_M3000_TuneCP5_13TeV-madgraph-pythia8",
          "WprimeToWZToWlepZlep_narrow_M3500_TuneCP5_13TeV-madgraph-pythia8",
          "WprimeToWZToWlepZlep_narrow_M4000_TuneCP5_13TeV-madgraph-pythia8",
          "ZGToLLG_01J_5f_TuneCP5_13TeV-amcatnloFXFX-pythia8",
          "ZZTo4L_13TeV_powheg_pythia8",
        }
      },
      {
        2018,
        {
          "DYJetsToLL_M-10to50_TuneCP5_13TeV-madgraphMLM-pythia8",
          "DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8",
          "TTJets_DiLept_TuneCP5_13TeV-madgraphMLM-pythia8",
          "WZTo3LNu_0Jets_MLL-50_TuneCP5_13TeV-madgraphMLM-pythia8",
          "WprimeToWZToWlepZlep_narrow_M600_TuneCP5_13TeV-madgraph-pythia8",
          "WprimeToWZToWlepZlep_narrow_M800_TuneCP5_13TeV-madgraph-pythia8",
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
          "ZGToLLG_01J_LoosePtlPtg_5f_TuneCP5_13TeV-amcatnloFXFX-pythia8",
          "ZZTo4L_13TeV_powheg_pythia8_TuneCP5",
        }
      }
    };

  int year;
  auto list = new TList();
  list->SetName("PileupSFList");
  auto f = std::make_unique<TFile>("PileupWeights.root","RECREATE");

  for( std::multimap<int,std::vector<std::string>>::iterator i = BgNames.begin();
        i != BgNames.end(); ++i) {
    year = (*i).first;

    TFile *dataFile = TFile::Open(Form("files/mc/%d/sf/PileupHistogram-goldenJSON-13tev-%d-69200ub.root",year,year));

    auto dataPileup = static_cast<TH1D*>(dataFile->Get("pileup"));
    TH1D* mcPileup;
    TH1D* weights;

    f->mkdir(Form("%d", year));

    for(auto& sample: (*i).second) {
      std::clog << Form("Processing: %d/%s:\n", year, sample.c_str());
      mcPileup = static_cast<TH1D*>(mcFile->Get(Form("%d/%s/HPileup",year,sample.c_str())));
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
