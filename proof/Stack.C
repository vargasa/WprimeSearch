#include "THStack.h"

void Stack(std::string FileName = "WprimeHistos_all.root"){

  const Float_t luminosity = 35.9e15; /* 35.9fb^-1 2016 */
  const Float_t pbFactor = 1e-12; /*pico prefix*/

  const Int_t FillStyle = 1001;
  // ShortName, DasName, kColor, Style, isSignal?, XSection, nEvents
  std::vector<std::tuple<std::string,std::string,Int_t,Int_t,Bool_t,Float_t,Int_t>> BgNames = {
    std::make_tuple("WZ","WZTo3LNu_TuneCUETP8M1_13TeV-powheg-pythia8",
                    kOrange,FillStyle,false,4.102,1993200),
    std::make_tuple("DYJetsToLL_A","DYJetsToLL_Zpt-100to200_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
                    kOrange+7,FillStyle,false,5.795e02,6584581),
    std::make_tuple("DYJetsToLL_B","DYJetsToLL_Zpt-200toInf_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
                    kOrange+7,FillStyle,false,1.030e+02,2903064),
    std::make_tuple("t#bar{t}","TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
                    kBlue-2,FillStyle,false,2.412e02,6068369),
    std::make_tuple("Z#gamma","ZGToLLG_01J_LoosePtlPtg_5f_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8",
                    kRed+3,FillStyle,false,7.564e01,18134601),
    std::make_tuple("ZZ","ZZTo4L_13TeV_powheg_pythia8",
                    kBlue,FillStyle,false,1.256,6669988),
    std::make_tuple("W' (1.4TeV)","WprimeToWZToWlepZlep_narrow_M-1400_13TeV-madgraph",
                    kBlack,0,true,3.196e02,50000)
  };

  std::string DataName = "DoubleEGSingleElectronSingleMuon";

  auto f1 = TFile::Open(FileName.c_str());

  std::vector<std::tuple<std::string,std::string>> HistNames = {
    std::make_tuple("HMassZA","3e0#mu;M_{Z}^{3e0#mu}(GeV);Event count"),
    std::make_tuple("HMassZB","2e1#mu;M_{Z}^{2e1#mu}(GeV);Event count"),
    std::make_tuple("HMassZC","1e2#mu;M_{Z}^{1e2#mu}(GeV);Event count"),
    std::make_tuple("HMassZD","0e3#mu;M_{Z}^{0e3#mu}(GeV);Event count"),
    /* Another series */
    std::make_tuple("HMetA","3e0#mu;#slash{E}^{3e0#mu}_{T}(GeV);Event count"),
    std::make_tuple("HMetB","2e1#mu;#slash{E}^{2e1#mu}_{T}(GeV);Event count"),
    std::make_tuple("HMetC","1e2#mu;#slash{E}^{1e2#mu}_{T}(GeV);Event count"),
    std::make_tuple("HMetD","0e3#mu;#slash{E}^{0e3#mu}_{T}(GeV);Event count"),
    /* Another series */
    std::make_tuple("HMassTWA","M_{T}^{W}(3e0#mu);M_{WT}^{3e0#mu};Event count"),
    std::make_tuple("HMassTWB","M_{T}^{W}(2e1#mu);M_{WT}^{2e1#mu};Event count"),
    std::make_tuple("HMassTWC","M_{T}^{W}(1e2#mu);M_{WT}^{1e2#mu};Event count"),
    std::make_tuple("HMassTWD","M_{T}^{W}(0e3#mu);M_{WT}^{0e3#mu};Event count"),
    /* Another series */
    std::make_tuple("HMassWZA","WZ Mass (3e0#mu);M_{Z}^{3e0#mu};Event count"),
    std::make_tuple("HMassWZB","WZ Mass (2e1#mu);M_{Z}^{2e1#mu};Event count"),
    std::make_tuple("HMassWZC","WZ Mass (1e2#mu);M_{Z}^{1e2#mu};Event count"),
    std::make_tuple("HMassWZD","WZ Mass (0e3#mu);M_{Z}^{0e3#mu};Event count")
  };

  auto c1 = new TCanvas("cs","cs",10,10,1196,772);
  c1->Divide(2,2);

  Int_t j = 1;
  for (auto HN : HistNames){
    Int_t r = (j-1)%4;
    c1->cd(r+1);
    THStack *hs = new THStack("hs","");
    THStack *hsdata = new THStack("hsdata","");
    auto legend = new TLegend(0.3, 0.66, .89, .89);
    legend->SetNColumns(2);
    auto h = (TH1F*)f1->Get(Form("%s/%s",DataName.c_str(),(std::get<0>(HN).c_str())));
    hsdata->Add(h);
    for (auto BGN: BgNames) {
      Bool_t IsSignal = std::get<4>(BGN);
      auto h = (TH1F*)f1->Get(Form("%s/%s",(std::get<1>(BGN)).c_str(),(std::get<0>(HN).c_str())));
      h->SetFillStyle(std::get<3>(BGN));
      h->SetTitle((std::get<0>(BGN)).c_str());
      h->SetFillColor(std::get<2>(BGN));
      h->Scale(luminosity*std::get<5>(BGN)*pbFactor/((Float_t)std::get<6>(BGN)));
      if(!IsSignal) h->SetLineWidth(0);
      hs->Add(h);
      legend->AddEntry(h,(std::get<0>(BGN)).c_str(),IsSignal? "L":"F");
    }
    hs->SetTitle((std::get<1>(HN)).c_str());
    gPad->SetLogy();
    gPad->SetTickx();
    gPad->SetTicky();
    TH1 *last = (TH1*)hs->GetStack()->Last();
    const Float_t LegendSpace = 10.; // Log Scale ;/
    const Float_t MaxY = last->GetBinContent(last->GetMaximumBin()) * LegendSpace;
    legend->SetBorderSize(0);
    hs->SetMaximum(MaxY);
    hs->Draw("HIST");
    auto hdata = (TH1F*)hsdata->GetStack()->Last();
    hdata->SetMarkerStyle(kFullCircle);
    hdata->Draw("HIST SAME P");

    ++j;
    legend->Draw();
    if( r+1 == 4 ){
      c1->Print(Form("Stack_%s.png",(std::get<0>(HN)).c_str()));
      c1->Clear();
      c1->Divide(2,2);
    }
  }
}
