#include "THStack.h"

void Stack(){

  std::string FileName = "WprimeHistos_all.root";
  // ShortName, DasName, kColor, XSection
  std::vector<std::tuple<std::string,std::string,Int_t,Float_t>> BgNames = {
    std::make_tuple("DYJetsToLL","DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8",kYellow,0.1),
    std::make_tuple("TTJets_DiLept","TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",kGreen,0.2),
    std::make_tuple("ZGToLLG","ZGToLLG_01J_LoosePtlPtg_5f_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8",kOrange,0.3),
    std::make_tuple("DYJetsToLL_Zpt-100to200","DYJetsToLL_Zpt-100to200_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",kRed,0.4),
    std::make_tuple("WZTo3LNu","WZTo3LNu_TuneCUETP8M1_13TeV-powheg-pythia8",kMagenta,0.5),
    std::make_tuple("ZZTo4L","ZZTo4L_13TeV_powheg_pythia8",kBlue,0.6),
    std::make_tuple("DYJetsToLL","DYJetsToLL_Zpt-200toInf_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",kCyan,0.7)
  };

  auto f1 = TFile::Open(FileName.c_str());

  std::vector<std::tuple<std::string,std::string>> HistNames = {
    std::make_tuple("HMetA","3e0#mu;#slash{M}^{3e0#mu}_{T};Event count"),
    std::make_tuple("HMetB","2e1#mu;#slash{M}^{2e1#mu}_{T};Event count"),
    std::make_tuple("HMetC","1e2#mu;#slash{M}^{1e2#mu}_{T};Event count"),
    std::make_tuple("HMetD","0e3#mu;#slash{M}^{0e3#mu}_{T};Event count")
  };

  auto c1 = new TCanvas("cs","cs",10,10,700,900);
  c1->SetLogy();


  for (auto HN : HistNames){
    THStack *hs = new THStack("hs","");
    for (auto BGN: BgNames) {
      auto h = (TH1F*)f1->Get(Form("%s/%s",(std::get<1>(BGN)).c_str(),(std::get<0>(HN).c_str())));
      //h->Scale(std::get<3>(BGN));
      h->SetFillStyle(1001);
      h->SetTitle((std::get<0>(BGN)).c_str());
      h->SetFillColor(std::get<2>(BGN));
      hs->Add(h);
    }
    hs->SetTitle((std::get<1>(HN)).c_str());
    hs->Draw();
    gPad->BuildLegend(0.5,0.5,0.95,0.95,"");
    c1->Print(Form("Stack_%s.png",(std::get<0>(HN)).c_str()));
    delete hs;
  }
}
