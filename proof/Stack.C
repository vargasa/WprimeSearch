#include "THStack.h"

void Stack(){

  std::string FileName = "WprimeHistos_all.root";
  std::map<std::string,std::pair<std::string,int>> BgNames = {
    {"DYJetsToLL",{"DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8",kYellow}},
    {"TTJets_DiLept",{"TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",kGreen}},
    {"ZGToLLG",{"ZGToLLG_01J_LoosePtlPtg_5f_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8",kOrange}},
    {"DYJetsToLL_Zpt-100to200",{"DYJetsToLL_Zpt-100to200_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",kRed}},
    {"WZTo3LNu",{"WZTo3LNu_TuneCUETP8M1_13TeV-powheg-pythia8",kMagenta}},
    {"ZZTo4L",{"ZZTo4L_13TeV_powheg_pythia8",kBlue}},
    {"DYJetsToLL",{"DYJetsToLL_Zpt-200toInf_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",kCyan}}
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
    THStack *hs = new THStack("hs","");
    for (auto BGN: BgNames) {
      auto h = (TH1F*)f1->Get(Form("%s/%s",BGN.second.first.c_str(),HN.c_str()));
      h->SetTitle(BGN.first.c_str());
      h->SetFillColor(BGN.second.second);
      hs->Add(h);
    }
    hs->SetTitle(HN.c_str());
    hs->Draw();
    gPad->BuildLegend(0.5,0.5,0.95,0.95,"");
    c1->Print(Form("Stack_%s.png",HN.c_str()));
    delete hs;
  }
}
