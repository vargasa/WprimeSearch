#include "THStack.h"

void Stack(){

  std::string FileName = "WprimeHistos_all.root";
  const Int_t FillStyle = 1001;
  // ShortName, DasName, kColor, Style, isSignal?, XSection
  std::vector<std::tuple<std::string,std::string,Int_t,Int_t,Bool_t,Float_t>> BgNames = {
    std::make_tuple("WZ","WZTo3LNu_TuneCUETP8M1_13TeV-powheg-pythia8",kOrange,FillStyle,false,4.102),
    std::make_tuple("DYJetsToLL_Zpt-100to200","DYJetsToLL_Zpt-100to200_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",kOrange+7,FillStyle,false,5.795e02),
    std::make_tuple("DYJetsToLL","DYJetsToLL_Zpt-200toInf_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",kOrange+7,FillStyle,false,1.030e+02),
    std::make_tuple("t#bar{t}","TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",kBlue-2,FillStyle,false,2.412e02),
    std::make_tuple("Z#gamma","ZGToLLG_01J_LoosePtlPtg_5f_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8",kRed+3,FillStyle,false,7.564e01),
    std::make_tuple("ZZ","ZZTo4L_13TeV_powheg_pythia8",kBlue,FillStyle,false,1.256),
    std::make_tuple("W' (1.4TeV)","WprimeToWZToWlepZlep_narrow_M-1400_13TeV-madgraph",kBlack,0,true,3.196e02)
  };

  auto f1 = TFile::Open(FileName.c_str());

  std::vector<std::tuple<std::string,std::string>> HistNames = {
    std::make_tuple("HMassZA","3e0#mu;#slash{E}^{3e0#mu}_{T};Event count"),
    std::make_tuple("HMassZB","2e1#mu;#slash{E}^{2e1#mu}_{T};Event count"),
    std::make_tuple("HMassZC","1e2#mu;#slash{E}^{1e2#mu}_{T};Event count"),
    std::make_tuple("HMassZD","0e3#mu;#slash{E}^{0e3#mu}_{T};Event count")
  };

  auto c1 = new TCanvas("cs","cs",10,10,700,900);
  c1->SetLogy();


  for (auto HN : HistNames){
    THStack *hs = new THStack("hs","");
    for (auto BGN: BgNames) {
      auto h = (TH1F*)f1->Get(Form("%s/%s",(std::get<1>(BGN)).c_str(),(std::get<0>(HN).c_str())));
      h->SetFillStyle(std::get<3>(BGN));
      h->SetTitle((std::get<0>(BGN)).c_str());
      h->SetFillColor(std::get<2>(BGN));
      h->Scale(std::get<5>(BGN));
      if(!std::get<4>(BGN)) h->SetLineWidth(0);
      hs->Add(h);
    }
    hs->SetTitle((std::get<1>(HN)).c_str());
    hs->Draw("HIST");
    gPad->BuildLegend(0.6,0.6,0.90,0.90,"");
    c1->Print(Form("Stack_%s.png",(std::get<0>(HN)).c_str()));
    delete hs;
  }
}
