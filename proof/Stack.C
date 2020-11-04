#include <iostream>
#include <fstream>
#include "THStack.h"

void Stack(std::string FileName = "WprimeHistos_all.root"){

  TH1::SetDefaultSumw2();


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

  // ShortName, DasName, kColor, Style, XSection, nEvents
  std::multimap<int, std::vector<BackgroundInfo>> BgNames =
    {
      {
        2016,
        {
          BackgroundInfo{"WZ","WZTo3LNu_TuneCUETP8M1_13TeV-powheg-pythia8",
            kOrange,5.052},
          BackgroundInfo{"ZZ","ZZTo4L_13TeV_powheg_pythia8",
            kBlue,1.325},
          BackgroundInfo{"t#bar{t}","TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
            kBlue-2,87.315},
          BackgroundInfo{"Z#gamma","ZGTo2LG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8",
            kRed+3,123.9},
          BackgroundInfo{"DY","DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
            kOrange+7,5765.},
        }
      },
      {
        2017,
        {
          BackgroundInfo{"WZ","WZTo3LNu_mllmin01_NNPDF31_TuneCP5_13TeV_powheg_pythia8",
            kOrange,62.17},
          BackgroundInfo{"ZZ","ZZTo4L_13TeV_powheg_pythia8",
            kBlue,1.325},
          BackgroundInfo{"t#bar{t}","TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8_NEWPMX",
            kBlue-2,831.76},
          BackgroundInfo{"Z#gamma","ZGToLLG_01J_5f_TuneCP5_13TeV-amcatnloFXFX-pythia8",
            kRed+3,123.9},
          BackgroundInfo{"DY","DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8_NEWPMX",
            kOrange+7,5765.},
        }
      },
      {
        2018,
        {
          BackgroundInfo{"WZ","WZTo3LNu_TuneCP5_13TeV-amcatnloFXFX-pythia8",
            kOrange,5.052},
          BackgroundInfo{"ZZ","ZZTo4L_TuneCP5_13TeV-amcatnloFXFX-pythia8",
            kBlue,1.325},
          BackgroundInfo{"t#bar{t}","TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8",
            kBlue-2,831.76},
          BackgroundInfo{"Z#gamma","ZGToLLG_01J_5f_TuneCP5_13TeV-amcatnloFXFX-pythia8",
            kRed+3,123.9},
          BackgroundInfo{"DY","DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8",
            kOrange+7,5765.},
        }
      }
    };


  struct SignalInfo {
    std::string legendName;
    std::string folderName;
    Float_t xsec;
  };

  std::multimap<int, std::vector<SignalInfo>> SignalSamples = {
    {
      2016,
      {
        SignalInfo{"W' (0.6TeV)","WprimeToWZToWlepZlep_narrow_M-600_13TeV-madgraph",5.988e-2},
        SignalInfo{"W' (0.8TeV)","WprimeToWZToWlepZlep_narrow_M-800_13TeV-madgraph",1.807e-2},
        SignalInfo{"W' (1.0TeV)","WprimeToWZToWlepZlep_narrow_M-1000_13TeV-madgraph",7.069e-3},
        SignalInfo{"W' (1.2TeV)","WprimeToWZToWlepZlep_narrow_M-1200_13TeV-madgraph",3.215e-3},
        SignalInfo{"W' (1.4TeV)","WprimeToWZToWlepZlep_narrow_M-1400_13TeV-madgraph",1.613e-3},
        SignalInfo{"W' (1.6TeV)","WprimeToWZToWlepZlep_narrow_M-1600_13TeV-madgraph",8.655e-4},
        SignalInfo{"W' (1.8TeV)","WprimeToWZToWlepZlep_narrow_M-1800_13TeV-madgraph",4.873e-4},
        SignalInfo{"W' (2.0TeV)","WprimeToWZToWlepZlep_narrow_M-2000_13TeV-madgraph",2.842e-4},
        SignalInfo{"W' (2.5TeV)","WprimeToWZToWlepZlep_narrow_M-2500_13TeV-madgraph",8.187e-5},
        SignalInfo{"W' (3.0TeV)","WprimeToWZToWlepZlep_narrow_M-3000_13TeV-madgraph",2.567e-5},
        SignalInfo{"W' (3.5TeV)","WprimeToWZToWlepZlep_narrow_M-3500_13TeV-madgraph",8.385e-6},
        SignalInfo{"W' (4.0TeV)","WprimeToWZToWlepZlep_narrow_M-4000_13TeV-madgraph",2.771e-6},
      }
    },
    {
      2017,
      {
        SignalInfo{"W' (0.6TeV)","WprimeToWZToWlepZlep_narrow_M600_TuneCP5_13TeV-madgraph-pythia8",5.988e-2},
        SignalInfo{"W' (0.8TeV)","WprimeToWZToWlepZlep_narrow_M800_TuneCP5_13TeV-madgraph-pythia8",1.807e-2},
        SignalInfo{"W' (1.0TeV)","WprimeToWZToWlepZlep_narrow_M1000_TuneCP5_13TeV-madgraph-pythia8",7.069e-3},
        SignalInfo{"W' (1.2TeV)","WprimeToWZToWlepZlep_narrow_M1200_TuneCP5_13TeV-madgraph-pythia8",3.215e-3},
        SignalInfo{"W' (1.4TeV)","WprimeToWZToWlepZlep_narrow_M1400_TuneCP5_13TeV-madgraph-pythia8",1.613e-3},
        SignalInfo{"W' (1.6TeV)","WprimeToWZToWlepZlep_narrow_M1600_TuneCP5_13TeV-madgraph-pythia8",8.655e-4},
        SignalInfo{"W' (1.8TeV)","WprimeToWZToWlepZlep_narrow_M1800_TuneCP5_13TeV-madgraph-pythia8",4.873e-4},
        SignalInfo{"W' (2.0TeV)","WprimeToWZToWlepZlep_narrow_M2000_TuneCP5_13TeV-madgraph-pythia8",2.842e-4},
        SignalInfo{"W' (2.5TeV)","WprimeToWZToWlepZlep_narrow_M2500_TuneCP5_13TeV-madgraph-pythia8",8.187e-5},
        SignalInfo{"W' (3.0TeV)","WprimeToWZToWlepZlep_narrow_M3000_TuneCP5_13TeV-madgraph-pythia8",2.567e-5},
        SignalInfo{"W' (3.5TeV)","WprimeToWZToWlepZlep_narrow_M3500_TuneCP5_13TeV-madgraph-pythia8",8.385e-6},
        SignalInfo{"W' (4.0TeV)","WprimeToWZToWlepZlep_narrow_M4000_TuneCP5_13TeV-madgraph-pythia8",2.771e-6},
      }
    },
    {
      2018,
      {
        SignalInfo{"W' (0.6TeV)","WprimeToWZToWlepZlep_narrow_M600_TuneCP5_13TeV-madgraph-pythia8",5.988e-2},
        SignalInfo{"W' (0.8TeV)","WprimeToWZToWlepZlep_narrow_M800_TuneCP5_13TeV-madgraph-pythia8",1.807e-2},
        SignalInfo{"W' (1.0TeV)","WprimeToWZToWlepZlep_narrow_M1000_TuneCP5_13TeV-madgraph-pythia8",7.069e-3},
        SignalInfo{"W' (1.2TeV)","WprimeToWZToWlepZlep_narrow_M1200_TuneCP5_13TeV-madgraph-pythia8",3.215e-3},
        SignalInfo{"W' (1.4TeV)","WprimeToWZToWlepZlep_narrow_M1400_TuneCP5_13TeV-madgraph-pythia8",1.613e-3},
        SignalInfo{"W' (1.6TeV)","WprimeToWZToWlepZlep_narrow_M1600_TuneCP5_13TeV-madgraph-pythia8",8.655e-4},
        SignalInfo{"W' (1.8TeV)","WprimeToWZToWlepZlep_narrow_M1800_TuneCP5_13TeV-madgraph-pythia8",4.873e-4},
        SignalInfo{"W' (2.0TeV)","WprimeToWZToWlepZlep_narrow_M2000_TuneCP5_13TeV-madgraph-pythia8",2.842e-4},
        SignalInfo{"W' (2.5TeV)","WprimeToWZToWlepZlep_narrow_M2500_TuneCP5_13TeV-madgraph-pythia8",8.187e-5},
        SignalInfo{"W' (3.0TeV)","WprimeToWZToWlepZlep_narrow_M3000_TuneCP5_13TeV-madgraph-pythia8",2.567e-5},
        SignalInfo{"W' (3.5TeV)","WprimeToWZToWlepZlep_narrow_M3500_TuneCP5_13TeV-madgraph-pythia8",8.385e-6},
        SignalInfo{"W' (4.0TeV)","WprimeToWZToWlepZlep_narrow_M4000_TuneCP5_13TeV-madgraph-pythia8",2.771e-6},
      }
    }
  };

  auto f1 = TFile::Open(FileName.c_str(),"READ");

  struct HistoInfo {
    std::string name;
    std::string title;
  };

  std::vector<HistoInfo> HistNames = {
    HistoInfo{"HMassZA","M_{Z}^{Z#rightarrow ee};M_{Z}^{3e0#mu}(GeV);Event count/2GeV"},
    HistoInfo{"HMassZB","M_{Z}^{Z#rightarrow ee};M_{Z}^{2e1#mu}(GeV);Event count/2GeV"},
    HistoInfo{"HMassZC","M_{Z}^{Z#rightarrow #mu#mu};M_{Z}^{1e2#mu}(GeV);Event count/2GeV"},
    HistoInfo{"HMassZD","M_{Z}^{Z#rightarrow #mu#mu};M_{Z}^{0e3#mu}(GeV);Event count/2GeV"},
    /* Another series */
    HistoInfo{"HMassZA_SFUp","M_{Z}^{Z#rightarrow ee};M_{Z}^{3e0#mu}(GeV);Event count/2GeV"},
    HistoInfo{"HMassZB_SFUp","M_{Z}^{Z#rightarrow ee};M_{Z}^{2e1#mu}(GeV);Event count/2GeV"},
    HistoInfo{"HMassZC_SFUp","M_{Z}^{Z#rightarrow #mu#mu};M_{Z}^{1e2#mu}(GeV);Event count/2GeV"},
    HistoInfo{"HMassZD_SFUp","M_{Z}^{Z#rightarrow #mu#mu};M_{Z}^{0e3#mu}(GeV);Event count/2GeV"},
    /* Another series */
    HistoInfo{"HMassZA_SFDown","M_{Z}^{Z#rightarrow ee};M_{Z}^{3e0#mu}(GeV);Event count/2GeV"},
    HistoInfo{"HMassZB_SFDown","M_{Z}^{Z#rightarrow ee};M_{Z}^{2e1#mu}(GeV);Event count/2GeV"},
    HistoInfo{"HMassZC_SFDown","M_{Z}^{Z#rightarrow #mu#mu};M_{Z}^{1e2#mu}(GeV);Event count/2GeV"},
    HistoInfo{"HMassZD_SFDown","M_{Z}^{Z#rightarrow #mu#mu};M_{Z}^{0e3#mu}(GeV);Event count/2GeV"},
    /* Another series */
    HistoInfo{"HMetA","#slash{E}^{Z#rightarrow ee W#rightarrow e#nu}_{T};#slash{E}^{3e0#mu}_{T}(GeV);Event count/10GeV"},
    HistoInfo{"HMetB","#slash{E}^{Z#rightarrow ee W#rightarrow #mu#nu}_{T};#slash{E}^{2e1#mu}_{T}(GeV);Event count/10GeV"},
    HistoInfo{"HMetC","#slash{E}^{Z#rightarrow #mu#mu W#rightarrow e#nu}_{T}#mu;#slash{E}^{1e2#mu}_{T}(GeV);Event count/10GeV"},
    HistoInfo{"HMetD","#slash{E}^{Z#rightarrow #mu#mu W#rightarrow #mu#nu}_{T};#slash{E}^{0e3#mu}_{T}(GeV);Event count/10GeV"},
    /* Another series */
    HistoInfo{"HMetA_SFUp","#slash{E}^{Z#rightarrow ee W#rightarrow e#nu}_{T};#slash{E}^{3e0#mu}_{T}(GeV);Event count/10GeV"},
    HistoInfo{"HMetB_SFUp","#slash{E}^{Z#rightarrow ee W#rightarrow #mu#nu}_{T};#slash{E}^{2e1#mu}_{T}(GeV);Event count/10GeV"},
    HistoInfo{"HMetC_SFUp","#slash{E}^{Z#rightarrow #mu#mu W#rightarrow e#nu}_{T};#slash{E}^{1e2#mu}_{T}(GeV);Event count/10GeV"},
    HistoInfo{"HMetD_SFUp","#slash{E}^{Z#rightarrow #mu#mu W#rightarrow #mu#nu}_{T};#slash{E}^{0e3#mu}_{T}(GeV);Event count/10GeV"},
    /* Another series */
    HistoInfo{"HMetA_SFDown","#slash{E}^{Z#rightarrow ee W#rightarrow e#nu}_{T};#slash{E}^{3e0#mu}_{T}(GeV);Event count/10GeV"},
    HistoInfo{"HMetB_SFDown","#slash{E}^{Z#rightarrow ee W#rightarrow #mu#nu}_{T};#slash{E}^{2e1#mu}_{T}(GeV);Event count/10GeV"},
    HistoInfo{"HMetC_SFDown","#slash{E}^{Z#rightarrow #mu#mu W#rightarrow e#nu}_{T};#slash{E}^{1e2#mu}_{T}(GeV);Event count/10GeV"},
    HistoInfo{"HMetD_SFDown","#slash{E}^{Z#rightarrow #mu#mu W#rightarrow #mu#nu}_{T};#slash{E}^{0e3#mu}_{T}(GeV);Event count/10GeV"},
    /* Another series */
    HistoInfo{"HMassTWA","M_{T}^{W}(3e0#mu);M_{WT}^{3e0#mu};Event count/5GeV"},
    HistoInfo{"HMassTWB","M_{T}^{W}(2e1#mu);M_{WT}^{2e1#mu};Event count/5GeV"},
    HistoInfo{"HMassTWC","M_{T}^{W}(1e2#mu);M_{WT}^{1e2#mu};Event count/5GeV"},
    HistoInfo{"HMassTWD","M_{T}^{W}(0e3#mu);M_{WT}^{0e3#mu};Event count/5GeV"},
    /* Another series */
    HistoInfo{"HMassTWA_SFUp","M_{T}^{W} SFUp (3e0#mu);M_{WT}^{3e0#mu};Event count/5GeV"},
    HistoInfo{"HMassTWB_SFUp","M_{T}^{W} SFUp (2e1#mu);M_{WT}^{2e1#mu};Event count/5GeV"},
    HistoInfo{"HMassTWC_SFUp","M_{T}^{W} SFUp (1e2#mu);M_{WT}^{1e2#mu};Event count/5GeV"},
    HistoInfo{"HMassTWD_SFUp","M_{T}^{W} SFUp (0e3#mu);M_{WT}^{0e3#mu};Event count/5GeV"},
    /* Another series */
    HistoInfo{"HMassTWA_SFDown","M_{T}^{W} SFDown (3e0#mu);M_{WT}^{3e0#mu};Event count/5GeV"},
    HistoInfo{"HMassTWB_SFDown","M_{T}^{W} SFDown (2e1#mu);M_{WT}^{2e1#mu};Event count/5GeV"},
    HistoInfo{"HMassTWC_SFDown","M_{T}^{W} SFDown (1e2#mu);M_{WT}^{1e2#mu};Event count/5GeV"},
    HistoInfo{"HMassTWD_SFDown","M_{T}^{W} SFDown (0e3#mu);M_{WT}^{0e3#mu};Event count/5GeV"},
    /* Another series */
    HistoInfo{"HMassWZA","M_{WZ}^{Z#rightarrow ee W#rightarrow e#nu};M_{WZ}^{3e0#mu};Event count/100GeV"},
    HistoInfo{"HMassWZB","M_{WZ}^{Z#rightarrow ee W#rightarrow #mu#nu};M_{WZ}^{2e1#mu};Event count/100GeV"},
    HistoInfo{"HMassWZC","M_{WZ}^{Z#rightarrow #mu#mu W#rightarrow e#nu};M_{WZ}^{1e2#mu};Event count/100GeV"},
    HistoInfo{"HMassWZD","M_{WZ}^{Z#rightarrow #mu#mu W#rightarrow #mu#nu};M_{WZ}^{0e3#mu};Event count/100GeV"},
    /* Another series */
    HistoInfo{"HMassWZA_SFUp","M_{WZ}^{Z#rightarrow ee W#rightarrow e#nu};M_{Z}^{3e0#mu};Event count/100GeV"},
    HistoInfo{"HMassWZB_SFUp","M_{WZ}^{Z#rightarrow ee W#rightarrow #mu#nu};M_{Z}^{2e1#mu};Event count/100GeV"},
    HistoInfo{"HMassWZC_SFUp","M_{WZ}^{Z#rightarrow #mu#mu W#rightarrow e#nu};M_{Z}^{1e2#mu};Event count/100GeV"},
    HistoInfo{"HMassWZD_SFUp","M_{WZ}^{Z#rightarrow #mu#mu W#rightarrow #mu#nu};M_{Z}^{0e3#mu};Event count/100GeV"}, 
    /* Another series */
    HistoInfo{"HMassWZA_SFDown","M_{WZ}^{Z#rightarrow ee W#rightarrow e#nu};M_{Z}^{2e1#mu};Event count/100GeV"},
    HistoInfo{"HMassWZB_SFDown","M_{WZ}^{Z#rightarrow ee W#rightarrow #mu#nu};M_{Z}^{2e1#mu};Event count/100GeV"},
    HistoInfo{"HMassWZC_SFDown","M_{WZ}^{Z#rightarrow #mu#mu W#rightarrow e#nu};M_{Z}^{1e2#mu};Event count/100GeV"},
    HistoInfo{"HMassWZD_SFDown","M_{WZ}^{Z#rightarrow #mu#mu W#rightarrow #mu#nu};M_{Z}^{0e3#mu};Event count/100GeV"},
    /* Another series */
    HistoInfo{"HPileupA","Number of Good Primary Vertices;nPvs;Event count"},
    HistoInfo{"HPileupB","Number of Good Primary Vertices;nPvs;Event count"},
    HistoInfo{"HPileupC","Number of Good Primary Vertices;nPvs;Event count"},
    HistoInfo{"HPileupD","Number of Good Primary Vertices;nPvs;Event count"},
    /* Another Series */
    HistoInfo{"HMet","MET lll#nu; #slash{E_{T}}(GeV);Event count/10GeV"},
    HistoInfo{"HMassTW","M_{WT}^{lll#nu}; M_{WT}(GeV);Event count/5GeV"},
    HistoInfo{"HMassZ","M_{Z}^{lll#nu}; M_{Z}(GeV);Event count/2GeV"},
    HistoInfo{"HMassWZ","M_{WZ}^{lll#nu}; M_{WZ}(GeV);Event count/100GeV"},
  };

  std::vector<HistoInfo> NonStackedHistos = {
    HistoInfo{"HPtl1","Z Lepton highest pt;Pt;Normalized Events"},
    HistoInfo{"HPtl2","Z Second Lepton pt;Pt;Normalized Events"},
    HistoInfo{"HPtl3","W Lepton Pt;Pt;Normalized Events"},
    HistoInfo{"HMetPt","Met Pt;Pt;Normalized Events"},
    /*Next series*/
    HistoInfo{"HDistl1l2","Distance in Eta-Phi Plane; Distance in Eta-Phi Plane (l1,l2); Normalized Events"},
    HistoInfo{"HDistl1l3","Distance in Eta-Phi Plane; Distance in Eta-Phi Plane (l1,l3); Normalized Events"},
    HistoInfo{"HDistl2l3","Distance in Eta-Phi Plane; Distance in Eta-Phi Plane (l2,l3); Normalized Events"},
    HistoInfo{"HWZDist","Distance in Eta-Phi Plane; Distance in Eta-Phi Plane (W,Z); Normalized Events"},
    /*Next series*/
    HistoInfo{"HEtal1","Eta Distribution l1; Eta; Normalized events"},
    HistoInfo{"HEtal2","Eta Distribution l2; Eta; Normalized events"},
    HistoInfo{"HEtal3","Eta Distribution l3; Eta; Normalized events"},
    HistoInfo{"HScaleFactors","Scale Factors; Scale Factor; Normalized events"},
    /*Next series*/
    HistoInfo{"HPhil1","Phi Distribution l1; Eta; Normalized events"},
    HistoInfo{"HPhil2","Phi Distribution l2; Eta; Normalized events"},
    HistoInfo{"HPhil3","Phi Distribution l3; Eta; Normalized events"},
    HistoInfo{"HMetPhi","Phi Distribution MET; MET; Normalized events"},
    /*Next series*/
    HistoInfo{"HPileup","Number of primary vertices; Npvs; Normalized events"},
    HistoInfo{"HPileup","Number of primary vertices; Npvs; Normalized events"},
    HistoInfo{"HPileup","Number of primary vertices; Npvs; Normalized events"},
    HistoInfo{"HPileup","Number of primary vertices; Npvs; Normalized events"},
  };

  std::function<void(TH1* h,const Double_t&)> blindHisto = [](TH1* h, const Double_t& wpmass) {
    Int_t nBin = h->FindBin(wpmass);
    for(int i = -1; i < 20; ++i){
      h->SetBinContent(nBin+i,0.);
      h->SetBinError(nBin+i,0.);
    }
  };

  auto blindStack = [&](THStack * hst, const Double_t& wpmass) {

    TObjLink *lnk = hst->GetHists()->FirstLink();
    while (lnk) {
      blindHisto(static_cast<TH1*>(lnk->GetObject()),wpmass);
      lnk = lnk->Next();
    }
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


  auto getRatio = [](TH1F* hhdata, THStack *hss){
      auto hcdata = static_cast<TH1*>(hhdata->Clone());
      hcdata->Divide(static_cast<TH1*>(hss->GetStack()->Last()));
      hcdata->SetMarkerColor(kBlack);
      hcdata->SetMarkerStyle(20);
      hcdata->SetMarkerSize(.5);
      hcdata->SetLineWidth(1);
      hcdata->SetTitle("");
      hcdata->GetXaxis()->SetTitleSize(25);
      hcdata->GetXaxis()->SetTitleFont(43);
      hcdata->GetXaxis()->SetTitleOffset(6.0);
      hcdata->GetXaxis()->SetLabelSize(0.17);
      hcdata->GetYaxis()->SetTitleSize(14);
      hcdata->GetYaxis()->SetTitleFont(43);
      hcdata->GetYaxis()->SetLabelSize(0.17);
      hcdata->GetYaxis()->SetTitleOffset(5.0);
      hcdata->GetYaxis()->SetNdivisions(6,3,0);
      hcdata->GetYaxis()->SetTitle("Data/MC");
      hcdata->GetYaxis()->SetLimits(0.,2.);
      return hcdata;
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
    count += hCutFlow->GetBinContent(hCutFlow->GetXaxis()->FindBin(cutLabel.c_str()));

    return count;
  };

  auto applyLumiSF = [&](TH1* h, std::string folder, const Float_t& xsec){
    Int_t yr = std::stoi(folder.substr(0,folder.rfind("/")+1));
    Double_t nEvents = getCutCount(folder,"NoCuts");
    Double_t lumiSF = luminosity[yr]*xsec*1e3/nEvents; /* pico*femto^-1=1e-12*1e15=1e3 */
    h->Scale(lumiSF);
  };

  auto getMCHisto = [&](std::string folder, std::string hName){
    std::string hpath = Form("%s/%s",folder.c_str(),hName.c_str());
    std::clog << "Getting MCHisto: " << hpath << std::endl;
    auto h = static_cast<TH1F*>(f1->Get(hpath.c_str()));
    h = static_cast<TH1F*>(h->Clone());
    return h;
  };

  std::function<void(const std::string)> diffSampleYear = [&] (const std::string& histoName) {

    TCanvas* c =  new TCanvas("c","c",1200,1200);
    TPad* p1 = new TPad("p1","p1",0.0,0.2,0.5,1.); p1->Draw();
    TPad* p2 = new TPad("p2","p2",0.5,0.2,1.,1.); p2->Draw();
    TPad* p3 = new TPad("p3","p3",0.0,0.0,1.0,0.2); p3->Draw();

    std::vector<int> years = { 2016, 2017, 2018 };

    auto getSampleName = [&] (const std::string& legend, const int& yy) {
      for (auto BGN: BgNames.find(yy)->second) {
        if (BGN.legendName == legend) return BGN.folderName;
      }
      return std::string("");
    };

    for (auto BGN: BgNames.find(years[0])->second) {
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
        TH1* h = static_cast<TH1*>(getMCHisto(folder,histoName)->Clone());
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
      c->Print(Form("plots/SampleDiff_%s_%s.png",category.c_str(),histoName.c_str()));
      hgw->Delete();
      hn->Delete();
      delete hs;
    }

    delete c;
  };

  diffSampleYear("HMassWZ");

  auto getBGStack = [&](int yr, std::string hname, TLegend* legend = NULL){
    THStack* hstck = new THStack();
    Int_t prevColor = -1;
    for (auto BGN: BgNames.find(yr)->second) {
      auto h = getMCHisto(Form("%d/%s",yr,BGN.folderName.c_str()),hname);
      applyLumiSF(h, Form("%d/%s",yr,BGN.folderName.c_str()), BGN.xsec);
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

  auto getDataHisto = [&](const int& yr, const std::string& hName){
    std::string DataName = DataSampleNames[yr];
    std::clog <<  Form("Getting CMSDATA: %d/%s/%s\n",yr,DataName.c_str(),hName.c_str());
    auto hdata = dynamic_cast<TH1F*>(f1->Get(Form("%d/%s/%s",yr,DataName.c_str(),hName.c_str())));
    if (!hdata) {
      std::string tmp = hName;
      tmp = tmp.substr(0,tmp.find("_SF"));
      std::clog << Form("Printing data on %s (from %s) plot\n",tmp.c_str(),hName.c_str());
      hdata = static_cast<TH1F*>(f1->Get(Form("%d/%s/%s",yr,DataName.c_str(),tmp.c_str())));
    }
    hdata = static_cast<TH1F*>(hdata->Clone());
    return hdata;
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

    for(const auto& BGN: (BgNames.find(year))->second){
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
    c2->Print(Form("plots/%d/%s_HTCutFlow.png",year,signal.folderName.c_str()));
    delete HTCutFlow;
    delete c2;
  };

  std::function<void(const int&, const SignalInfo&, const std::string&)>
    printH2Comb = [&] (const int& yr, const SignalInfo& signal,
                     const std::string& histoLabel){
    TCanvas* c2 = new TCanvas("c2","c2");
    TH2F* h2BgSum = nullptr;
    for(const auto& BGN: (BgNames.find(yr))->second){
      std::string folder = Form("%d/%s",yr,BGN.folderName.c_str());
      auto h2 = static_cast<TH2F*>(f1->Get(Form("%s/%s",folder.c_str(),histoLabel.c_str())));
      if(!h2BgSum) {
        h2BgSum = static_cast<TH2F*>(h2->Clone());
      } else {
        h2BgSum->Add(static_cast<TH2F*>(h2->Clone()));
      }
    }
    h2BgSum->SetMarkerColor(kRed);
    TH2F* h2Sig = static_cast<TH2F*>(f1->Get(Form("%d/%s/%s",yr,signal.folderName.c_str(),histoLabel.c_str()))->Clone());
    h2Sig->SetMarkerColor(kGreen);
    TH2F* h2Data = static_cast<TH2F*>(f1->Get(Form("%d/%s/%s",yr,DataSampleNames[yr].c_str(),histoLabel.c_str()))->Clone());
    h2Data->SetMarkerColor(kBlack);
    c2->Divide(2,1);
    c2->cd(1);
    h2BgSum->Draw("");
    h2Data->Draw("SAME");
    h2Sig->Draw("SAME");
    c2->cd(2);
    h2BgSum->Draw("");
    h2Data->Draw("SAME");
    c2->Print(Form("plots/%d/%s_%s.png",yr,signal.folderName.c_str(),histoLabel.c_str()));
    delete c2;
  };

  auto printDataCard = [&] (const int& year, const int& wpmass,
                            THStack* hsbg, TH1* hsig, TH1* hdata) {
    ofstream dcFile;
    dcFile.open(Form("plots/%d/%d_%d_DataCard.txt", year, year, wpmass));

    std::unordered_map<Int_t, Float_t> lumiSyst = {
      { 2016, 0.025 }, /*AN2018_298_v14*/
      { 2017, 0.023 },
      { 2018, 0.025 },
    };

    std::string binName = "modA";
    std::string bin = "bin\t" + binName + "\t";
    std::string process  = Form("process\t%s_%d\t","Wp",wpmass);
    std::string processn = "process\t0\t";
    std::string rate     = Form("rate\t%.2f\t",hsig->Integral());
    std::string unc1     = Form("lumi_13TeV\tlnN\t%.4f\t",1. + lumiSyst[year]);

    for(uint i = 1; i <= hsbg->GetNhists(); ++i){
      bin += binName + "\t";
      processn += Form("%d\t",i);
      unc1 += Form("%.4f\t",1. + lumiSyst[year]);
    }

    TIter b = hsbg->begin();

    while(b.Next()){
      auto histo = static_cast<TH1F*>(*b);
      process += Form("%s\t",histo->GetTitle());
      rate += Form("%.2f\t", histo->Integral());
    }

    dcFile << "imax\t1\njmax\t" << hsbg->GetNhists() << "\nkmax\t1\n";
    dcFile << "------------\n";
    dcFile<< "bin\t" << binName << std::endl;
    dcFile << "observation\t" << hdata->Integral() << std::endl;
    dcFile << "------------\n";
    dcFile << bin << std::endl << process << std::endl
    << processn << std::endl << rate << std::endl;
    dcFile << "------------\n";
    dcFile << unc1 << std::endl;

  };

  std::function<void(const int&, THStack* hs, TH1*)> printBgContrib = [](const int& year, THStack* hsbg, TH1* hsig = nullptr) {

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
    cc->Print(Form("plots/%d/%d_BGContrib.png",year,year));
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

    auto hsig = getMCHisto(signalFolder,histoName);
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

    const std::string fromHisto = "HMassWZ";
    TCanvas* c1 = new TCanvas("c1","c1");

    THStack *hs = new THStack("hs","");
    hs = getBGStack(yr, "HMassWZ");

    TGraph* GPunziS = new TGraph();
    GPunziS->SetTitle(Form("Punzi Significance vs M(WZ) %d; M(WZ); Punzi Significance", yr));

    for (auto& signal: SignalSamples.find(yr)->second) {
        GPunziS->SetPoint(GPunziS->GetN(),
                          getWpMassFromName(signal.folderName),
                          getPunziSignificance(hs, Form("%d/%s",yr,signal.folderName.c_str()), fromHisto));
    }

    GPunziS->SetMarkerStyle(20);
    GPunziS->Draw("AP");
    c1->Print(Form("plots/%d/%d_PonziSignificance.png",yr,yr));

    delete hs;
    delete c1;
  };


  for (auto& item: SignalSamples) {
    const int year = item.first;

    auto c1 = new TCanvas("cs","cs",10,10,1500,1200);

    plotPunziSignificance(year);

    TGraph* GSelRatioA = new TGraph();
    GSelRatioA->SetTitle("eee#nu");
    TGraph* GSelRatioB = new TGraph();
    GSelRatioB->SetTitle("ee#mu#nu");
    TGraph* GSelRatioC = new TGraph();
    GSelRatioC->SetTitle("e#mu#mu#nu");
    TGraph* GSelRatioD = new TGraph();
    GSelRatioD->SetTitle("#mu#mu#mu#nu");

    TGraph* GElTgEff = new TGraph();
    GElTgEff->SetTitle(Form("Electron HLTs Efficiency on Signal;Wprime Mass Point %d;Ratio [EventsPassingHLTs/EventsGenerated]",year));
    TGraph* GMuTgEff = new TGraph();
    GMuTgEff->SetTitle(Form("Muon HLTs Efficiency on Signal;Wprime Mass Point %d;Ratio",year));
    TGraph* GHLTEff = new TGraph();
    GHLTEff->SetTitle(Form("HLT Efficiency (El OR Mu) on Signal;Wprime Mass Point %d;Ratio",year));


    for (auto signal: item.second) {
      Int_t WpMass = getWpMassFromName(signal.folderName);
      printCutFlowStack(year,signal);

      addCutEff(GSelRatioA, Form("%d/%s",year,signal.folderName.c_str()), "3e0mu", "NoCuts", WpMass);
      addCutEff(GSelRatioB, Form("%d/%s",year,signal.folderName.c_str()), "2e1mu", "NoCuts", WpMass);
      addCutEff(GSelRatioC, Form("%d/%s",year,signal.folderName.c_str()), "1e2mu", "NoCuts", WpMass);
      addCutEff(GSelRatioD, Form("%d/%s",year,signal.folderName.c_str()), "0e3mu", "NoCuts", WpMass);
      addCutEff(GElTgEff, Form("%d/%s",year,signal.folderName.c_str()), "FailElectronHLTs", "NoCuts", WpMass);
      addCutEff(GMuTgEff, Form("%d/%s",year,signal.folderName.c_str()), "FailMuonHLTs", "NoCuts", WpMass);
      addCutEff(GHLTEff, Form("%d/%s",year,signal.folderName.c_str()), "FailHLT", "NoCuts", WpMass);

      printH2Comb(year, signal, "HPtWPtZ");
      printH2Comb(year, signal, "HDeltaRPtZ");
      printH2Comb(year, signal, "HLtMWZ");

      c1->Clear();
      c1->Divide(2,2);
      Int_t j = 1;

      for (auto HN : HistNames) {

        Int_t r = (j-1)%4;
        c1->cd(r+1);
        const char *hName = HN.name.c_str();

        const Float_t leftMargin = 0.12;
        const Float_t rightMargin = 0.12;
        const Float_t topMargin = 0.12;
        const Float_t bottomMargin = 0.5;

        auto mainPad = new TPad(Form("mainPad_%s",HN.name.c_str()),"mainPad",0.,0.25,1.,1.);
        mainPad->Draw();
        mainPad->SetLeftMargin(leftMargin);
        mainPad->SetRightMargin(rightMargin);
        mainPad->SetBottomMargin(1e-3);
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

        auto subPad = new TPad(Form("mainPad_%s",HN.name.c_str()),"subPad",0.,0.,1.,0.25);
        subPad->Draw();
        subPad->SetLeftMargin(leftMargin);
        subPad->SetRightMargin(rightMargin);
        subPad->SetTopMargin(1e-3);
        subPad->SetBottomMargin(bottomMargin);
 
        auto legend = new TLegend(0.3, 0.66, .87, .89);
        legend->SetNColumns(2);

        THStack *hs = new THStack("hs","");
        hs = getBGStack(year,hName,legend);
        //blindStack(hs,WpMass);
        TH1F* last = static_cast<TH1F*>(hs->GetStack()->Last());

        auto hsig = getMCHisto(Form("%d/%s",year,signal.folderName.c_str()),hName);
        applyLumiSF(hsig, Form("%d/%s",year,signal.folderName.c_str()), signal.xsec);
        hsig->SetTitle(signal.legendName.c_str());
        legend->AddEntry( hsig,signal.legendName.c_str(),"L");
        hsig->SetLineColor(kBlack);
        hsig->SetLineWidth(3);
        hsig->SetFillColor(0);
        hs->SetTitle(HN.title.c_str());

        legend->SetBorderSize(0);

        gStyle->SetOptStat(0);

        mainPad->cd();
        hs->Draw("HIST");
        hsig->Draw("HIST SAME");
        double maxx = last->GetXaxis()->GetBinWidth(0) * (last->FindLastBinAbove(0.25)+1);
        double minx = last->GetXaxis()->GetBinWidth(0) * (last->FindFirstBinAbove(0.25)-1);
        hs->GetHistogram()->GetXaxis()->SetRangeUser(minx,maxx);

        auto hdata = getDataHisto(year,hName);
        hdata->SetMarkerStyle(kFullCircle);
        fixYRange(hs,getMaxY(hdata));
        hdata->Draw("SAME P");
        TH1F* herror = getErrorHisto(hs);
        herror->Draw("SAME E2");
        blindHisto(hdata,WpMass);
        legend->AddEntry(hdata, Form("Data%d",year));

        if (std::string(hName).compare("HMassWZ")==0){
          printBgContrib(year,hs,hsig);
          printDataCard(year,WpMass,hs,hsig,hdata);
        }


        auto hcdata = getRatio(hdata,hs);
        subPad->cd();
        subPad->SetGrid();
        hcdata->SetMaximum(1.9);
        hcdata->SetMinimum(0.1);
        hcdata->Draw();
        subPad->SetFrameLineWidth(1);
        hcdata->GetXaxis()->SetRangeUser(minx,maxx);
        hcdata->GetYaxis()->SetLabelSize(0.15);
        hcdata->GetYaxis()->SetTitleOffset(2.5);
        hcdata->GetXaxis()->SetTitleOffset(9.0);

        ++j;
        mainPad->cd();
        legend->Draw();
        if( r+1 == 4 ){
          c1->Print(Form("plots/%d/Stack_%s_Wprime%d_Data.png",year,hName,WpMass));
          c1->Clear();
          c1->Divide(2,2);
        }
      }

      /*** NonStackedHistos ***/

      c1->Clear();
      c1->Divide(2,2);
      UInt_t canvasPos = 1;
      gStyle->SetOptStat(0);

      j = 1;
      for(auto hp: NonStackedHistos){
        Int_t r = (j-1)%4;
        auto legend = new TLegend(0.11, 0.7, .89, .89);
        legend->SetNColumns(3);
        c1->cd(r+1);
        for(auto BGN: (BgNames.find(year))->second ){
          std::cout << Form("Getting %d/%s/%s\n",year,BGN.folderName.c_str(),hp.name.c_str());
          auto h = (TH1F*)f1->Get(Form("%d/%s/%s",year,BGN.folderName.c_str(),hp.name.c_str()));
          h = (TH1F*)h->Clone();
          legend->AddEntry(h,BGN.legendName.c_str(), "L");
          legend->SetBorderSize(0);
          h->SetTitle(hp.title.c_str());
          h->SetLineColor(BGN.color);
          h->GetXaxis()->SetRangeUser(0., 800.);
          normalizeHisto(h);
          h->SetMaximum(1.5);
          h->Draw("HIST SAME");
        }

        auto hsig = (TH1F*)f1->Get(Form("%d/%s/%s",year,signal.folderName.c_str(),hp.name.c_str()));
        hsig = (TH1F*)hsig->Clone();
        hsig->SetLineColor(kBlack);
        hsig->SetLineWidth(2);
        normalizeHisto(hsig);
        hsig->Draw("HIST SAME");
        legend->AddEntry(hsig,signal.legendName.c_str(), "L");

        if(hp.name == "HPileup"){
           auto hdata = getDataHisto(year,hp.name);
           hdata->SetLineColor(kRed);
           hdata->SetLineWidth(2);
           normalizeHisto(hdata);
           hdata->Draw("HIST SAME");
           legend->AddEntry(hdata,"Data","L");
        }

        ++j;
        legend->Draw();
        if( r+1 == 4 ){
          c1->Print(Form("plots/%d/Stack_%s_Wprime%d.png",year,hp.name.c_str(),WpMass));
          c1->Clear();
          c1->Divide(2,2);
        }
      }
    }


    // Selection Efficiency Plot
    c1->Clear();
    c1->cd();
    const Int_t nMassPoints = 14;
    TGraph GInclusive = TGraph(nMassPoints);
    GInclusive.SetTitle("Inclusive");
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
    GSelRatioA->SetMarkerColor(kBlue);
    GSelRatioB->SetMarkerStyle(21);
    GSelRatioB->SetMarkerColor(kOrange);
    GSelRatioC->SetMarkerStyle(22);
    GSelRatioC->SetMarkerColor(kGreen);
    GSelRatioD->SetMarkerStyle(23);
    GSelRatioD->SetMarkerColor(kViolet);
    addToInclusive(GSelRatioA);
    addToInclusive(GSelRatioB);
    addToInclusive(GSelRatioC);
    addToInclusive(GSelRatioD);
    GInclusive.SetMarkerColor(kRed);
    GInclusive.SetMarkerStyle(24);
    GInclusive.GetYaxis()->SetTitle("#epsilon");
    GInclusive.GetXaxis()->SetTitle("M_{WZ}");
    GInclusive.Draw("AP");
    GInclusive.GetYaxis()->SetRangeUser(0.0,0.4);
    GSelRatioA->Draw("P");
    GSelRatioB->Draw("P");
    GSelRatioC->Draw("P");
    GSelRatioD->Draw("P");
    gPad->BuildLegend(0.7,0.8,0.7,0.8);
    c1->Print(Form("plots/%d/%d_SelectionRatio.png",year,year));

    // Trigger efficiency plot
    c1->Clear();
    c1->cd();
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
    GElTgEff->Draw("AP");
    GElTgEff->GetYaxis()->SetRangeUser(0, 1.);
    GMuTgEff->SetMarkerStyle(23);
    GMuTgEff->SetMarkerColor(kBlue);
    invertGraph(GMuTgEff);
    GMuTgEff->Draw("P");
    GHLTEff->SetMarkerStyle(24);
    GHLTEff->SetMarkerColor(kRed);
    invertGraph(GHLTEff);
    GHLTEff->Draw("P");
    gPad->BuildLegend();
    c1->Print(Form("plots/%d/%d_SignalTriggerEfficiency.png",year,year));
  }

}
