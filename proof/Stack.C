#include "THStack.h"

void Stack(std::string FileName = "WprimeHistos_all.root"){

  TH1::SetDefaultSumw2();

  const Float_t pbFactor = 1e-12; /*pico prefix*/

  std::unordered_map<int, float> luminosity = {
    {2016, 35.92e15},
    {2017, 41.43e15},
    {2018, 59.74e15}
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
          BackgroundInfo{"DY","DYJetsToLL_M-10to50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8",
            kOrange+7,18610.},
          BackgroundInfo{"DY","DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
            kOrange+7,5765.},
          BackgroundInfo{"t#bar{t}","TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
            kBlue-2,87.315},
          BackgroundInfo{"Z#gamma","ZGTo2LG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8",
            kRed+3,123.9},
          BackgroundInfo{"ZZ","ZZTo4L_13TeV_powheg_pythia8",
            kBlue,1.325}
        }
      },
      {
        2017,
        {
          BackgroundInfo{"DYJetsToLL","DYJetsToLL_M-10to50_TuneCP5_13TeV-madgraphMLM-pythia8",
            kOrange+7,18610.},
          BackgroundInfo{"DYJetsToLL","DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8",
            kOrange+7,5765.},
          BackgroundInfo{"TTJets","TTJets_DiLept_TuneCP5_13TeV-madgraphMLM-pythia8",
            kBlue-2,87.315},
          BackgroundInfo{"WZ","WZTo3LNu_TuneCP5_13TeV-amcatnloFXFX-pythia8",
            kOrange,5.052},
          BackgroundInfo{"Z#gamma","ZGToLLG_01J_5f_TuneCP5_13TeV-amcatnloFXFX-pythia8",
            kRed+3,123.9},
          BackgroundInfo{"ZZ","ZZTo4L_13TeV_powheg_pythia8",
            kBlue,0.1325},
        }
      },
      {
        2018,
        {
          BackgroundInfo{"DYJetsToLL","DYJetsToLL_M-10to50_TuneCP5_13TeV-madgraphMLM-pythia8",
            kOrange+7,18610.},
          BackgroundInfo{"DYJetsToLL","DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8",
            kOrange+7,5765.},
          BackgroundInfo{"TTJets","TTJets_DiLept_TuneCP5_13TeV-madgraphMLM-pythia8",
            kBlue-2,87.315},
          BackgroundInfo{"WZ","WZTo3LNu_TuneCP5_13TeV-amcatnloFXFX-pythia8",
            kOrange,5.052},
          BackgroundInfo{"Z#gamma","ZGToLLG_01J_5f_TuneCP5_13TeV-amcatnloFXFX-pythia8",
            kRed+3,123.9},
          BackgroundInfo{"ZZ","ZZTo4L_TuneCP5_13TeV-amcatnloFXFX-pythia8",
            kBlue,1.325}
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
        SignalInfo{"W' (0.6TeV)","WprimeToWZToWlepZlep_narrow_M-600_13TeV-madgraph",0.009817},
        SignalInfo{"W' (0.8TeV)","WprimeToWZToWlepZlep_narrow_M-800_13TeV-madgraph",9.8e-3},
        SignalInfo{"W' (1.0TeV)","WprimeToWZToWlepZlep_narrow_M-1000_13TeV-madgraph",6.54e-3},
        SignalInfo{"W' (1.2TeV)","WprimeToWZToWlepZlep_narrow_M-1200_13TeV-madgraph",3.61e-3},
        SignalInfo{"W' (1.4TeV)","WprimeToWZToWlepZlep_narrow_M-1400_13TeV-madgraph",2.0e-3},
        SignalInfo{"W' (1.6TeV)","WprimeToWZToWlepZlep_narrow_M-1600_13TeV-madgraph",1.14e-3},
        SignalInfo{"W' (1.8TeV)","WprimeToWZToWlepZlep_narrow_M-1800_13TeV-madgraph",6.69e-4},
        SignalInfo{"W' (2.0TeV)","WprimeToWZToWlepZlep_narrow_M-2000_13TeV-madgraph",4.01e-4},
        SignalInfo{"W' (2.5TeV)","WprimeToWZToWlepZlep_narrow_M-2500_13TeV-madgraph",1.21e-4},
        SignalInfo{"W' (3.0TeV)","WprimeToWZToWlepZlep_narrow_M-3000_13TeV-madgraph",3.86e-5},
        SignalInfo{"W' (3.5TeV)","WprimeToWZToWlepZlep_narrow_M-3500_13TeV-madgraph",1.31e-5},
        SignalInfo{"W' (4.0TeV)","WprimeToWZToWlepZlep_narrow_M-4000_13TeV-madgraph",4.26e-6},
      }
    },
    {
      2017,
      {
        SignalInfo{"W' (0.6TeV)","WprimeToWZToWlepZlep_narrow_M600_TuneCP5_13TeV-madgraph-pythia8",0.009817},
        SignalInfo{"W' (0.8TeV)","WprimeToWZToWlepZlep_narrow_M800_TuneCP5_13TeV-madgraph-pythia8",9.8e-3},
        SignalInfo{"W' (1.0TeV)","WprimeToWZToWlepZlep_narrow_M1000_TuneCP5_13TeV-madgraph-pythia8",6.54e-3},
        SignalInfo{"W' (1.2TeV)","WprimeToWZToWlepZlep_narrow_M1200_TuneCP5_13TeV-madgraph-pythia8",3.61e-3},
        SignalInfo{"W' (1.4TeV)","WprimeToWZToWlepZlep_narrow_M1400_TuneCP5_13TeV-madgraph-pythia8",2.0e-3},
        SignalInfo{"W' (1.6TeV)","WprimeToWZToWlepZlep_narrow_M1600_TuneCP5_13TeV-madgraph-pythia8",1.14e-3},
        SignalInfo{"W' (1.8TeV)","WprimeToWZToWlepZlep_narrow_M1800_TuneCP5_13TeV-madgraph-pythia8",6.69e-4},
        SignalInfo{"W' (2.0TeV)","WprimeToWZToWlepZlep_narrow_M2000_TuneCP5_13TeV-madgraph-pythia8",4.01e-4},
        SignalInfo{"W' (2.5TeV)","WprimeToWZToWlepZlep_narrow_M2500_TuneCP5_13TeV-madgraph-pythia8",1.21e-4},
        SignalInfo{"W' (3.0TeV)","WprimeToWZToWlepZlep_narrow_M3000_TuneCP5_13TeV-madgraph-pythia8",3.86e-5},
        SignalInfo{"W' (3.5TeV)","WprimeToWZToWlepZlep_narrow_M3500_TuneCP5_13TeV-madgraph-pythia8",1.31e-5},
        SignalInfo{"W' (4.0TeV)","WprimeToWZToWlepZlep_narrow_M4000_TuneCP5_13TeV-madgraph-pythia8",4.26e-6},
      }
    },
    {
      2018,
      {
        SignalInfo{"W' (0.6TeV)","WprimeToWZToWlepZlep_narrow_M600_TuneCP5_13TeV-madgraph-pythia8",0.009817},
        SignalInfo{"W' (0.8TeV)","WprimeToWZToWlepZlep_narrow_M800_TuneCP5_13TeV-madgraph-pythia8",9.8e-3},
        SignalInfo{"W' (1.0TeV)","WprimeToWZToWlepZlep_narrow_M1000_TuneCP5_13TeV-madgraph-pythia8",6.54e-3},
        SignalInfo{"W' (1.2TeV)","WprimeToWZToWlepZlep_narrow_M1200_TuneCP5_13TeV-madgraph-pythia8",3.61e-3},
        SignalInfo{"W' (1.4TeV)","WprimeToWZToWlepZlep_narrow_M1400_TuneCP5_13TeV-madgraph-pythia8",2.0e-3},
        SignalInfo{"W' (1.6TeV)","WprimeToWZToWlepZlep_narrow_M1600_TuneCP5_13TeV-madgraph-pythia8",1.14e-3},
        SignalInfo{"W' (1.8TeV)","WprimeToWZToWlepZlep_narrow_M1800_TuneCP5_13TeV-madgraph-pythia8",6.69e-4},
        SignalInfo{"W' (2.0TeV)","WprimeToWZToWlepZlep_narrow_M2000_TuneCP5_13TeV-madgraph-pythia8",4.01e-4},
        SignalInfo{"W' (2.5TeV)","WprimeToWZToWlepZlep_narrow_M2500_TuneCP5_13TeV-madgraph-pythia8",1.21e-4},
        SignalInfo{"W' (3.0TeV)","WprimeToWZToWlepZlep_narrow_M3000_TuneCP5_13TeV-madgraph-pythia8",3.86e-5},
        SignalInfo{"W' (3.5TeV)","WprimeToWZToWlepZlep_narrow_M3500_TuneCP5_13TeV-madgraph-pythia8",1.31e-5},
        SignalInfo{"W' (4.0TeV)","WprimeToWZToWlepZlep_narrow_M4000_TuneCP5_13TeV-madgraph-pythia8",4.26e-6},
      }
    }
  };

  auto f1 = TFile::Open(FileName.c_str(),"READ");

  struct HistoInfo {
    std::string name;
    std::string title;
  };

  std::vector<HistoInfo> HistNames = {
    HistoInfo{"HMassZA","3e0#mu;M_{Z}^{3e0#mu}(GeV);Event count/2GeV"},
    HistoInfo{"HMassZB","2e1#mu;M_{Z}^{2e1#mu}(GeV);Event count/2GeV"},
    HistoInfo{"HMassZC","1e2#mu;M_{Z}^{1e2#mu}(GeV);Event count/2GeV"},
    HistoInfo{"HMassZD","0e3#mu;M_{Z}^{0e3#mu}(GeV);Event count/2GeV"},
    /* Another series */
    HistoInfo{"HMassZA_SFUp","3e0#mu SFUp;M_{Z}^{3e0#mu}(GeV);Event count/2GeV"},
    HistoInfo{"HMassZB_SFUp","2e1#mu SFUp;M_{Z}^{2e1#mu}(GeV);Event count/2GeV"},
    HistoInfo{"HMassZC_SFUp","1e2#mu SFUp;M_{Z}^{1e2#mu}(GeV);Event count/2GeV"},
    HistoInfo{"HMassZD_SFUp","0e3#mu SFUp;M_{Z}^{0e3#mu}(GeV);Event count/2GeV"},
    /* Another series */
    HistoInfo{"HMassZA_SFDown","3e0#mu SFDown;M_{Z}^{3e0#mu}(GeV);Event count/2GeV"},
    HistoInfo{"HMassZB_SFDown","2e1#mu SFDown;M_{Z}^{2e1#mu}(GeV);Event count/2GeV"},
    HistoInfo{"HMassZC_SFDown","1e2#mu SFDown;M_{Z}^{1e2#mu}(GeV);Event count/2GeV"},
    HistoInfo{"HMassZD_SFDown","0e3#mu SFDown;M_{Z}^{0e3#mu}(GeV);Event count/2GeV"},
    /* Another series */
    HistoInfo{"HMetA","3e0#mu;#slash{E}^{3e0#mu}_{T}(GeV);Event count/10GeV"},
    HistoInfo{"HMetB","2e1#mu;#slash{E}^{2e1#mu}_{T}(GeV);Event count/10GeV"},
    HistoInfo{"HMetC","1e2#mu;#slash{E}^{1e2#mu}_{T}(GeV);Event count/10GeV"},
    HistoInfo{"HMetD","0e3#mu;#slash{E}^{0e3#mu}_{T}(GeV);Event count/10GeV"},
    /* Another series */
    HistoInfo{"HMetA_SFUp","3e0#mu SFUp;#slash{E}^{3e0#mu}_{T}(GeV);Event count/10GeV"},
    HistoInfo{"HMetB_SFUp","2e1#mu SFUp;#slash{E}^{2e1#mu}_{T}(GeV);Event count/10GeV"},
    HistoInfo{"HMetC_SFUp","1e2#mu SFUp;#slash{E}^{1e2#mu}_{T}(GeV);Event count/10GeV"},
    HistoInfo{"HMetD_SFUp","0e3#mu SFUp;#slash{E}^{0e3#mu}_{T}(GeV);Event count/10GeV"},
    /* Another series */
    HistoInfo{"HMetA_SFDown","3e0#mu SFDown;#slash{E}^{3e0#mu}_{T}(GeV);Event count/10GeV"},
    HistoInfo{"HMetB_SFDown","2e1#mu SFDown;#slash{E}^{2e1#mu}_{T}(GeV);Event count/10GeV"},
    HistoInfo{"HMetC_SFDown","1e2#mu SFDown;#slash{E}^{1e2#mu}_{T}(GeV);Event count/10GeV"},
    HistoInfo{"HMetD_SFDown","0e3#mu SFDown;#slash{E}^{0e3#mu}_{T}(GeV);Event count/10GeV"},
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
    HistoInfo{"HMassWZA","WZ Mass (3e0#mu);M_{Z}^{3e0#mu};Event count/100GeV"},
    HistoInfo{"HMassWZB","WZ Mass (2e1#mu);M_{Z}^{2e1#mu};Event count/100GeV"},
    HistoInfo{"HMassWZC","WZ Mass (1e2#mu);M_{Z}^{1e2#mu};Event count/100GeV"},
    HistoInfo{"HMassWZD","WZ Mass (0e3#mu);M_{Z}^{0e3#mu};Event count/100GeV"},
    /* Another series */
    HistoInfo{"HMassWZA_SFUp","WZ Mass SFUp (3e0#mu);M_{Z}^{3e0#mu};Event count/100GeV"},
    HistoInfo{"HMassWZB_SFUp","WZ Mass SFUp (2e1#mu);M_{Z}^{2e1#mu};Event count/100GeV"},
    HistoInfo{"HMassWZC_SFUp","WZ Mass SFUp (1e2#mu);M_{Z}^{1e2#mu};Event count/100GeV"},
    HistoInfo{"HMassWZD_SFUp","WZ Mass SFUp (0e3#mu);M_{Z}^{0e3#mu};Event count/100GeV"}, 
    /* Another series */
    HistoInfo{"HMassWZA_SFDown","WZ Mass SFDown (3e0#mu);M_{Z}^{3e0#mu};Event count/100GeV"},
    HistoInfo{"HMassWZB_SFDown","WZ Mass SFDown (2e1#mu);M_{Z}^{2e1#mu};Event count/100GeV"},
    HistoInfo{"HMassWZC_SFDown","WZ Mass SFDown (1e2#mu);M_{Z}^{1e2#mu};Event count/100GeV"},
    HistoInfo{"HMassWZD_SFDown","WZ Mass SFDown(0e3#mu);M_{Z}^{0e3#mu};Event count/100GeV"},
    /* Another series */
    HistoInfo{"HPileupA","Number of Good Primary Vertices;nPvs;Event count"},
    HistoInfo{"HPileupB","Number of Good Primary Vertices;nPvs;Event count"},
    HistoInfo{"HPileupC","Number of Good Primary Vertices;nPvs;Event count"},
    HistoInfo{"HPileupD","Number of Good Primary Vertices;nPvs;Event count"},
    /* Another Series */
    HistoInfo{"HMet","MET (Combined);#slash{E}_{T}(GeV);Event count/10GeV"},
    HistoInfo{"HMassTW","M_{T}^{W} (Combined);M_{WT};Event count/5GeV"},
    HistoInfo{"HMassZ","Z Mass (Combined);M_{Z}(GeV);Event count/2GeV"},
    HistoInfo{"HMassWZ","WZ Mass (Combined);M_{Z}(GeV);Event count/100GeV"},
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
    h->SetBinContent(nBin,0.);
    h->SetBinContent(nBin+1,0.);
    h->SetBinContent(nBin-1,0.);
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

  auto fixYRange = [](THStack* hss){
    TH1 *last = (TH1*)hss->GetStack()->Last();
    const Float_t LegendSpace = 10.; // Log Scale ;/
    const Float_t MaxY = last->GetBinContent(last->GetMaximumBin()) * LegendSpace;
    hss->SetMaximum(MaxY);
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

  auto getMCHisto = [&](std::string folder, std::string hName, Float_t xsec){
    Int_t yr = std::stoi(folder.substr(0,folder.rfind("/")+1));
    std::string hpath = Form("%s/%s",folder.c_str(),hName.c_str());
    std::clog << "Getting MCHisto: " << hpath << std::endl;
    auto h = static_cast<TH1F*>(f1->Get(hpath.c_str()));
    h = static_cast<TH1F*>(h->Clone());

    Double_t nEvents = getCutCount(folder,"genWeight");
    Double_t lumiSF = luminosity[yr]*xsec*pbFactor/nEvents;
    h->Scale(lumiSF);

    std::cout << "Sample: " << folder.substr(0,30)
    << "\t" << hName.substr(0,10)
    << "\tnEvents :" << (long int)nEvents
    << "\txsec: " << xsec
    << "\tLumiSF: " << lumiSF
    << "\tIntegral: " << h->Integral() << std::endl;

    double zSel = getCutCount(folder,"+NoCuts-FailHLT-FailFlags-MET_pt<30-lep<3-goodLep<3-NoPairs-FailZMassWindow");
    double wSel =  zSel - getCutCount(folder,"+FailZl1PtCut+FailZDistCut");
    double wzSel = wSel - getCutCount(folder,"+FailZHighPtIdCut+Faill1l3DistCut+Faill2l3DistCut");
    double nChannel = getCutCount(folder,"+0e3mu+1e2mu+2e1mu+3e0mu");
    std::cout << "CutFlow: " << folder.substr(0,30)
    << "\tZ Selection: " << zSel/getCutCount(folder,"NoCuts")
    << "\tW Selection: " << wSel/getCutCount(folder,"NoCuts")
    << "\tWZ Selection: " << wzSel/getCutCount(folder,"NoCuts")
    << "\tnChannel: " << nChannel/getCutCount(folder,"NoCuts") << std::endl;

    return h;
  };

  auto getBGStack = [&](int yr, std::string hname, TLegend* legend = NULL){
    THStack* hstck = new THStack();
    Int_t prevColor = -1;
    for (auto BGN: BgNames.find(yr)->second) {
      auto h = getMCHisto(Form("%d/%s",yr,BGN.folderName.c_str()),hname,BGN.xsec);
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
    std::cout << "Data Integral " << hName << "\t" << hdata->Integral() << std::endl;
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
      double zSel = getCutCount(folder,"+NoCuts-FailHLT-FailFlags-MET_pt<30-lep<3-goodLep<3-NoPairs-FailZMassWindow");
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
      TH1D* HTCutFlow = new TH1D(Form("HTCutFlow_%s",BGN.folderName.c_str()),"HT",10,0., 10.);
      std::string folder = Form("%d/%s",year,BGN.folderName.c_str());
      std::cout << "Ploting Wirdo: " << folder << std::endl;
      fillCutH(HTCutFlow,folder);
      HTCutFlow->SetMarkerColor(BGN.color);
      HTCutFlow->SetMarkerStyle((BGN.color)%28);
      HTCutFlow->SetLineWidth(0);
      hstc->Add(HTCutFlow);
      legend->AddEntry(HTCutFlow,BGN.legendName.c_str(),"P");
    }

    TH1D* HTCutFlow = new TH1D(Form("HTCutFlow_%s",signal.folderName.c_str()),"HT",10,0., 10.);
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
    c2->Print(Form("%d_%s_HTCutFlow.png",year,signal.folderName.c_str()));
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


  for (auto& item: SignalSamples) {
    const int year = item.first;

    auto c1 = new TCanvas("cs","cs",10,10,2400,1200);

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
      Int_t WpMass;
      std::regex rexp1("(Wprime)([A-Za-z_-]+)([0-9]+)(.*)");
      std::smatch sm;
      printCutFlowStack(year,signal);
      if(std::regex_search(signal.folderName,sm,rexp1)){
        WpMass = std::stoi(sm[3]);
        addCutEff(GSelRatioA, Form("%d/%s",year,signal.folderName.c_str()), "3e0mu", "NoCuts", WpMass);
        addCutEff(GSelRatioB, Form("%d/%s",year,signal.folderName.c_str()), "2e1mu", "NoCuts", WpMass);
        addCutEff(GSelRatioC, Form("%d/%s",year,signal.folderName.c_str()), "1e2mu", "NoCuts", WpMass);
        addCutEff(GSelRatioD, Form("%d/%s",year,signal.folderName.c_str()), "0e3mu", "NoCuts", WpMass);
        addCutEff(GElTgEff, Form("%d/%s",year,signal.folderName.c_str()), "FailElectronHLTs", "NoCuts", WpMass);
        addCutEff(GMuTgEff, Form("%d/%s",year,signal.folderName.c_str()), "FailMuonHLTs", "NoCuts", WpMass);
        addCutEff(GHLTEff, Form("%d/%s",year,signal.folderName.c_str()), "FailHLT", "NoCuts", WpMass);
      }

      printH2Comb(year, signal, "HPtWPtZ");
      printH2Comb(year, signal, "HDeltaRPtZ");
      printH2Comb(year, signal, "HLtMWZ");

      continue;

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
        blindStack(hs,WpMass);
        TH1F* last = static_cast<TH1F*>(hs->GetStack()->Last());


        auto hsig = getMCHisto(Form("%d/%s",year,signal.folderName.c_str()),hName,signal.xsec);
        legend->AddEntry(hsig,signal.legendName.c_str(),"L");
        hsig->Draw("SAME");
        hs->SetTitle(HN.title.c_str());

        fixYRange(hs);
        legend->SetBorderSize(0);

        gStyle->SetOptStat(0);

        mainPad->cd();
        hs->Draw("HIST");
        double maxx = last->GetXaxis()->GetBinWidth(0) * (last->FindLastBinAbove(0.)+1);
        double minx = last->GetXaxis()->GetBinWidth(0) * (last->FindFirstBinAbove(0.)-1);
        hs->GetHistogram()->GetXaxis()->SetRangeUser(minx,maxx);

        auto hdata = getDataHisto(year,hName);
        hdata->SetMarkerStyle(kFullCircle);
        hdata->Draw("SAME P");
        TH1F* herror = getErrorHisto(hs);
        herror->Draw("SAME E2");
        blindHisto(hdata,WpMass);
        legend->AddEntry(hdata, Form("Data%d",year));

        auto hcdata = getRatio(hdata,hs);
        subPad->cd();
        hcdata->SetMaximum(2.);
        hcdata->SetMinimum(0.);
        hcdata->GetXaxis()->SetRangeUser(minx,maxx);
        hcdata->Draw();
        TLine *line = new TLine(hdata->GetXaxis()->GetXmin(),1.,
                              hdata->GetXaxis()->GetXmax(),1.);
        line->SetLineColor(kBlack);
        line->Draw();

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
          // auto hdata = getDataHisto(year,hp.name);
          // hdata->SetLineColor(kRed);
          // hdata->SetLineWidth(2);
          // normalizeHisto(hdata);
          // hdata->Draw("HIST SAME");
          // legend->AddEntry(hdata,"Data","L");
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
