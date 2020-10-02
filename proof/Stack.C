#include "THStack.h"

void Stack(std::string FileName = "WprimeHistos_all.root"){

  TH1::SetDefaultSumw2();

  const Float_t luminosity = 35.9e15; /* 35.9fb^-1 2016 */
  const Float_t pbFactor = 1e-12; /*pico prefix*/

  struct BackgroundInfo {
    std::string legendName;
    std::string folderName;
    UInt_t color;
    Float_t xsec;
  };

  // ShortName, DasName, kColor, Style, XSection, nEvents
  std::multimap<int, std::vector<BackgroundInfo>> BgNames =
    {
      {
        2016,
        {
          BackgroundInfo{"WZ","WZTo3LNu_TuneCUETP8M1_13TeV-powheg-pythia8",
            kOrange,4.43}, /*XSDB 2nd Sample is 0*/
          BackgroundInfo{"WZ EXT1","WZTo3LNu_TuneCUETP8M1_13TeV-powheg-pythia8_EXT1",
            kOrange,4.43}, /*XSDB 2nd Sample is 0*/
          BackgroundInfo{"DYJetsToLL_A","DYJetsToLL_Zpt-100to200_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
            kOrange+7,57.3},
          BackgroundInfo{"DYJetsToLL_AEXT1","DYJetsToLL_Zpt-100to200_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_EXT1",
            kOrange+7,57.3},
          BackgroundInfo{"DYJetsToLL_B","DYJetsToLL_Zpt-200toInf_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
            kOrange+7,6.733},
          BackgroundInfo{"DYJetsToLL_BEXT1","DYJetsToLL_Zpt-200toInf_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
            kOrange+7,6.733},
          BackgroundInfo{"t#bar{t}","TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
            kBlue-2,56.86},
          BackgroundInfo{"t#bar{t} EXT1","TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_EXT1",
            kBlue-4,56.86},
          BackgroundInfo{"Z#gamma","ZGTo2LG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8",
            kRed+3,123.8}, /*AN2019_252_v1*/
          BackgroundInfo{"Z#gamma EXT1","ZGTo2LG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_EXT1",
            kRed+3,123.8},
          BackgroundInfo{"ZZ","ZZTo4L_13TeV_powheg_pythia8",
            kBlue,1.256}
        }
      },
      {
        2017,
        {
          BackgroundInfo{"DYJetsToLL","DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8",
            kOrange+7,1.},
          BackgroundInfo{"TTJets","TTJets_DiLept_TuneCP5_13TeV-madgraphMLM-pythia8",
            kBlue-2,1.},
          BackgroundInfo{"WZ","WZTo3LNu_0Jets_MLL-50_TuneCP5_13TeV-madgraphMLM-pythia8",
            kOrange,1.},
          BackgroundInfo{"Z#gamma","ZGToLLG_01J_5f_TuneCP5_13TeV-amcatnloFXFX-pythia8",
            kRed+3,1.},
          BackgroundInfo{"ZZ","ZZTo4L_13TeV_powheg_pythia8",
            kBlue,1.}
        }
      },
      {
        2018,
        {
          BackgroundInfo{"DYJetsToLL","DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8",
            kOrange+7,1.},
          BackgroundInfo{"TTJets","TTJets_DiLept_TuneCP5_13TeV-madgraphMLM-pythia8",
            kBlue-2,1.},
          BackgroundInfo{"WZ","WZTo3LNu_0Jets_MLL-50_TuneCP5_13TeV-madgraphMLM-pythia8",
            kOrange,1.},
          BackgroundInfo{"Z#gamma","ZGToLLG_01J_LoosePtlPtg_5f_TuneCP5_13TeV-amcatnloFXFX-pythia8",
            kRed+3,1.},
          BackgroundInfo{"ZZ","ZZTo4L_13TeV_powheg_pythia8_TuneCP5",
            kBlue,1.}
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
        SignalInfo{"W' (0.6TeV)","WprimeToWZToWlepZlep_narrow_M-600_13TeV-madgraph",14490.0},
        SignalInfo{"W' (0.8TeV)","WprimeToWZToWlepZlep_narrow_M-800_13TeV-madgraph",3197.0},
        SignalInfo{"W' (1.0TeV)","WprimeToWZToWlepZlep_narrow_M-1000_13TeV-madgraph",1227.0},
        SignalInfo{"W' (1.2TeV)","WprimeToWZToWlepZlep_narrow_M-1200_13TeV-madgraph",589.6},
        SignalInfo{"W' (1.4TeV)","WprimeToWZToWlepZlep_narrow_M-1400_13TeV-madgraph",319.6},
        SignalInfo{"W' (1.6TeV)","WprimeToWZToWlepZlep_narrow_M-1600_13TeV-madgraph",185.9},
        SignalInfo{"W' (1.8TeV)","WprimeToWZToWlepZlep_narrow_M-1800_13TeV-madgraph",113.5},
        SignalInfo{"W' (2.0TeV)","WprimeToWZToWlepZlep_narrow_M-2000_13TeV-madgraph",71.67},
        SignalInfo{"W' (2.5TeV)","WprimeToWZToWlepZlep_narrow_M-2500_13TeV-madgraph",24.66},
        SignalInfo{"W' (3.0TeV)","WprimeToWZToWlepZlep_narrow_M-3000_13TeV-madgraph",8.971},
        SignalInfo{"W' (3.5TeV)","WprimeToWZToWlepZlep_narrow_M-3500_13TeV-madgraph",3.304},
        SignalInfo{"W' (4.0TeV)","WprimeToWZToWlepZlep_narrow_M-4000_13TeV-madgraph",1.221},
        SignalInfo{"W' (4.5TeV)","WprimeToWZToWlepZlep_narrow_M-4500_13TeV-madgraph",0.4574}
      }
    },
    {
      2017,
      {
        SignalInfo{"W' (0.6TeV)","WprimeToWZToWlepZlep_narrow_M600_TuneCP5_13TeV-madgraph-pythia8",1.},
        SignalInfo{"W' (0.8TeV)","WprimeToWZToWlepZlep_narrow_M800_TuneCP5_13TeV-madgraph-pythia8",1.},
        SignalInfo{"W' (1.0TeV)","WprimeToWZToWlepZlep_narrow_M1000_TuneCP5_13TeV-madgraph-pythia8",1.},
        SignalInfo{"W' (1.2TeV)","WprimeToWZToWlepZlep_narrow_M1200_TuneCP5_13TeV-madgraph-pythia8",1.},
        SignalInfo{"W' (1.4TeV)","WprimeToWZToWlepZlep_narrow_M1400_TuneCP5_13TeV-madgraph-pythia8",1.},
        SignalInfo{"W' (1.6TeV)","WprimeToWZToWlepZlep_narrow_M1600_TuneCP5_13TeV-madgraph-pythia8",1.},
        SignalInfo{"W' (1.8TeV)","WprimeToWZToWlepZlep_narrow_M1800_TuneCP5_13TeV-madgraph-pythia8",1.},
        SignalInfo{"W' (2.0TeV)","WprimeToWZToWlepZlep_narrow_M2000_TuneCP5_13TeV-madgraph-pythia8",1.},
        SignalInfo{"W' (2.5TeV)","WprimeToWZToWlepZlep_narrow_M2500_TuneCP5_13TeV-madgraph-pythia8",1.},
        SignalInfo{"W' (3.0TeV)","WprimeToWZToWlepZlep_narrow_M3000_TuneCP5_13TeV-madgraph-pythia8",1.},
        SignalInfo{"W' (3.5TeV)","WprimeToWZToWlepZlep_narrow_M3500_TuneCP5_13TeV-madgraph-pythia8",1.},
        SignalInfo{"W' (4.0TeV)","WprimeToWZToWlepZlep_narrow_M4000_TuneCP5_13TeV-madgraph-pythia8",1.},
      }
    },
    {
      2018,
      {
        SignalInfo{"W' (0.6TeV)","WprimeToWZToWlepZlep_narrow_M600_TuneCP5_13TeV-madgraph-pythia8",1.},
        SignalInfo{"W' (0.8TeV)","WprimeToWZToWlepZlep_narrow_M800_TuneCP5_13TeV-madgraph-pythia8",1.},
        SignalInfo{"W' (1.0TeV)","WprimeToWZToWlepZlep_narrow_M1000_TuneCP5_13TeV-madgraph-pythia8",1.},
        SignalInfo{"W' (1.2TeV)","WprimeToWZToWlepZlep_narrow_M1200_TuneCP5_13TeV-madgraph-pythia8",1.},
        SignalInfo{"W' (1.4TeV)","WprimeToWZToWlepZlep_narrow_M1400_TuneCP5_13TeV-madgraph-pythia8",1.},
        SignalInfo{"W' (1.6TeV)","WprimeToWZToWlepZlep_narrow_M1600_TuneCP5_13TeV-madgraph-pythia8",1.},
        SignalInfo{"W' (1.8TeV)","WprimeToWZToWlepZlep_narrow_M1800_TuneCP5_13TeV-madgraph-pythia8",1.},
        SignalInfo{"W' (2.0TeV)","WprimeToWZToWlepZlep_narrow_M2000_TuneCP5_13TeV-madgraph-pythia8",1.},
        SignalInfo{"W' (2.5TeV)","WprimeToWZToWlepZlep_narrow_M2500_TuneCP5_13TeV-madgraph-pythia8",1.},
        SignalInfo{"W' (3.0TeV)","WprimeToWZToWlepZlep_narrow_M3000_TuneCP5_13TeV-madgraph-pythia8",1.},
        SignalInfo{"W' (3.5TeV)","WprimeToWZToWlepZlep_narrow_M3500_TuneCP5_13TeV-madgraph-pythia8",1.},
        SignalInfo{"W' (4.0TeV)","WprimeToWZToWlepZlep_narrow_M4000_TuneCP5_13TeV-madgraph-pythia8",1.},
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
    HistoInfo{"HScaleFactors","Scale Factors; Scale Factor; Normalized events"},
    HistoInfo{"HPileup","Number of primary vertices; Npvs; Normalized events"},
    HistoInfo{"HPileup","Number of primary vertices; Npvs; Normalized events"},
    HistoInfo{"HPileup","Number of primary vertices; Npvs; Normalized events"},
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

  auto getMCHisto = [&](std::string folder, std::string hName, Float_t xsec){
    std::string hpath = Form("%s/%s",folder.c_str(),hName.c_str());
    std::clog << "Getting MCHisto: " << hpath << std::endl;
    auto h = static_cast<TH1F*>(f1->Get(hpath.c_str()));
    h = static_cast<TH1F*>(h->Clone());
    TH1F* hCutFlow = static_cast<TH1F*>(f1->Get(Form("%s/HCutFlow",folder.c_str())));
    auto nEvents = (Float_t)hCutFlow->GetBinContent(1);
    std::clog << "\tnEvents Processed :" << nEvents << std::endl;
    h->Scale(luminosity*xsec*pbFactor/nEvents);
    std::clog << "\tIntegral: " << h->Integral() << std::endl;
    return h;
  };

  auto getBGStack = [&](int yr, std::string hname, TLegend* legend = NULL){
    THStack* hstck = new THStack();
    for (auto BGN: BgNames.find(yr)->second) {
      auto h = getMCHisto(Form("%d/%s",yr,BGN.folderName.c_str()),hname,BGN.xsec);
      h->SetFillStyle(1001);
      h->SetTitle(BGN.legendName.c_str());
      h->SetFillColor(BGN.color);
      h->SetLineWidth(0);
      hstck->Add(h);
      if(legend)
        legend->AddEntry(h,BGN.legendName.c_str(),"F");
    }
    return hstck;
  };

  auto getDataHisto = [&](int yr, std::string hName){
    std::string DataName;
    if (yr == 2016 or yr == 2017){
      DataName = "SinglePhotonSingleElectronSingleMuon";
    } else if ( yr == 2018 ){
      DataName = "SingleMuonEGamma";
    }
    std::cout <<  Form("Getting CMSDATA: %d/%s/%s\n",yr,DataName.c_str(),hName.c_str());
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



  for (auto& item: SignalSamples){
    const int year = item.first;

    auto c1 = new TCanvas("cs","cs",10,10,2400,1200);

    for (auto signal: item.second) {
      Int_t WpMass;
      std::regex rexp1("(Wprime)([A-Za-z_-]+)([0-9]+)(.*)");
      std::smatch sm;
      if(std::regex_search(signal.folderName,sm,rexp1)){
        WpMass = std::stoi(sm[3]);
      }

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

        auto hsig = getMCHisto(Form("%d/%s",year,signal.folderName.c_str()),hName,signal.xsec);
        legend->AddEntry(hsig,signal.legendName.c_str(),"L");
        hsig->Draw("SAME");
        hs->SetTitle(HN.title.c_str());

        fixYRange(hs);
        legend->SetBorderSize(0);

        gStyle->SetOptStat(0);

        mainPad->cd();
        hs->Draw("HIST");

        auto hdata = getDataHisto(year,hName);
        hdata->SetMarkerStyle(kFullCircle);
        hdata->Draw("SAME P");
        TH1F* herror = getErrorHisto(hs);
        herror->Draw("SAME E2");
        legend->AddEntry(hdata, Form("Data%d",year));

        auto hcdata = getRatio(hdata,hs);
        subPad->cd();
        hcdata->SetMaximum(2.);
        hcdata->SetMinimum(0.);
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
  }

}
