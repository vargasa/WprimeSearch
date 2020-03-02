#include "THStack.h"

void Stack(){

  std::string FileName = "WprimeHistos_all.root";
  std::vector<std::string> DirNames = {
    "DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8",
    "TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
    "ZGToLLG_01J_LoosePtlPtg_5f_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8",
    "DYJetsToLL_Zpt-100to200_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
    "WZTo3LNu_TuneCUETP8M1_13TeV-powheg-pythia8",
    "ZZTo4L_13TeV_powheg_pythia8",
    "DYJetsToLL_Zpt-200toInf_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"
  };

  auto f1 = TFile::Open(FileName.c_str());

  std::vector<std::string> HistNames = {
    "HMassWA",
    "HMassWB",
    "HMassWC",
    "HMassWD"
  };

  auto c1 = new TCanvas("cs","cs",10,10,700,900);
  c1->SetLogy();


  for (auto HN : HistNames){
    Int_t color = 5;
    THStack *hs = new THStack("hs","");
    for (auto DN: DirNames) {
      auto h = (TH1F*)f1->Get(Form("%s/%s",DN.c_str(),HN.c_str()));
      h->SetTitle(DN.c_str());
      h->SetFillColor(color);
      hs->Add(h);
      ++color;
    }
    hs->SetTitle(HN.c_str());
    hs->Draw();
    gPad->BuildLegend(0.5,0.5,0.95,0.95,"");
    c1->Print(Form("Stack_%s.png",HN.c_str()));
    delete hs;
  }





}
