
Int_t PileupReweighing(){

  TFile *mcFile = TFile::Open("WprimeHistos.root","READ");
  TFile *dataFile = TFile::Open("PileupHistogram-goldenJSON-13tev-2016-69200ub.root");
  auto dataPileup = static_cast<TH1D*>(dataFile->Get("pileup"));
  TH1D* mcPileup;
  TH1D* weights;

  std::vector<std::string> BgNames =
    {
      "DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8",
      "DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8",
      "WZTo3LNu_TuneCUETP8M1_13TeV-powheg-pythia8",
      "DYJetsToLL_Zpt-100to200_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
      "DYJetsToLL_Zpt-200toInf_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
      "TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
      "ZGToLLG_01J_LoosePtlPtg_5f_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8",
      "ZZTo4L_13TeV_powheg_pythia8",
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
      "WZ1400.root"
    };

  auto list = new TList();
  list->SetName("PileupSFList");
  auto f = std::make_unique<TFile>("PileupWeights.root","RECREATE");

  for(auto sample: BgNames){
    mcPileup = static_cast<TH1D*>(mcFile->Get(Form("%s/HPileup",sample.c_str())));
    mcPileup = static_cast<TH1D*>(mcPileup->Clone());
    mcPileup->Scale(1./mcPileup->Integral());

    dataPileup = static_cast<TH1D*>(dataPileup->Clone());
    dataPileup->Scale(1./dataPileup->Integral());

    weights = static_cast<TH1D*>(dataPileup->Clone());
    weights->Divide(mcPileup);
    weights->SetName("lumiWeights");

    f->mkdir(Form("%s", sample.c_str()));
    f->cd(Form("%s", sample.c_str()));
    weights->Write();

    weights->SetName(sample.c_str());
    list->Add(weights);
  }

  f->cd("/");
  list->Write("PileupSFList",1);

  return 0;

}
