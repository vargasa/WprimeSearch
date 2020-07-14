#include "THStack.h"

void Stack(std::string FileName = "WprimeHistos_all.root"){

  const Float_t luminosity = 35.9e15; /* 35.9fb^-1 2016 */
  const Float_t pbFactor = 1e-12; /*pico prefix*/

  const char* DataName = "SinglePhotonSingleElectronSingleMuon";

  // ShortName, DasName, kColor, Style, XSection, nEvents
  std::vector<std::tuple<std::string,std::string,Int_t,Float_t>> BgNames = {
    std::make_tuple("WZ","WZTo3LNu_TuneCUETP8M1_13TeV-powheg-pythia8",
                    kOrange,4.43),
    std::make_tuple("DYJetsToLL_A","DYJetsToLL_Zpt-100to200_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
                    kOrange+7,57.3),
    std::make_tuple("DYJetsToLL_B","DYJetsToLL_Zpt-200toInf_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
                    kOrange+7,6.733),
    std::make_tuple("t#bar{t}","TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
                    kBlue-2,56.86),
    std::make_tuple("Z#gamma","ZGToLLG_01J_LoosePtlPtg_5f_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8",
                    kRed+3,124.9), /* AN2019_029_v3 */
    std::make_tuple("ZZ","ZZTo4L_13TeV_powheg_pythia8",
                    kBlue,1.256)
  };

  std::vector<std::tuple<std::string,std::string,Float_t>> SignalSamples = {
    std::make_tuple("W' (0.6TeV)","WprimeToWZToWlepZlep_narrow_M-600_13TeV-madgraph",14490.0),
    std::make_tuple("W' (0.8TeV)","WprimeToWZToWlepZlep_narrow_M-800_13TeV-madgraph",3197.0),
    std::make_tuple("W' (1.0TeV)","WprimeToWZToWlepZlep_narrow_M-1000_13TeV-madgraph",1227.0),
    std::make_tuple("W' (1.2TeV)","WprimeToWZToWlepZlep_narrow_M-1200_13TeV-madgraph",589.6),
    std::make_tuple("W' (1.4TeV)","WprimeToWZToWlepZlep_narrow_M-1400_13TeV-madgraph",319.6),
    std::make_tuple("W' (1.6TeV)","WprimeToWZToWlepZlep_narrow_M-1600_13TeV-madgraph",185.9),
    std::make_tuple("W' (1.8TeV)","WprimeToWZToWlepZlep_narrow_M-1800_13TeV-madgraph",113.5),
    std::make_tuple("W' (2.0TeV)","WprimeToWZToWlepZlep_narrow_M-2000_13TeV-madgraph",71.67),
    std::make_tuple("W' (2.5TeV)","WprimeToWZToWlepZlep_narrow_M-2500_13TeV-madgraph",24.66),
    std::make_tuple("W' (3.0TeV)","WprimeToWZToWlepZlep_narrow_M-3000_13TeV-madgraph",8.971),
    std::make_tuple("W' (3.5TeV)","WprimeToWZToWlepZlep_narrow_M-3500_13TeV-madgraph",3.304),
    std::make_tuple("W' (4.0TeV)","WprimeToWZToWlepZlep_narrow_M-4000_13TeV-madgraph",1.221),
    std::make_tuple("W' (4.5TeV)","WprimeToWZToWlepZlep_narrow_M-4500_13TeV-madgraph",0.4574)
  };

  auto f1 = TFile::Open(FileName.c_str(),"READ");

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
    std::make_tuple("HMassWZD","WZ Mass (0e3#mu);M_{Z}^{0e3#mu};Event count"),
    /* Another series */
    std::make_tuple("HPileupA","Number of Good Primary Vertices;nPvs;Event count"),
    std::make_tuple("HPileupB","Number of Good Primary Vertices;nPvs;Event count"),
    std::make_tuple("HPileupC","Number of Good Primary Vertices;nPvs;Event count"),
    std::make_tuple("HPileupD","Number of Good Primary Vertices;nPvs;Event count"),
  };

  std::vector<std::pair<std::string,std::string>> NonStackedHistos = {
    std::make_pair("HPtl1","Z Lepton highest pt;Pt;Normalized Events"),
    std::make_pair("HPtl2","Z Second Lepton pt;Pt;Normalized Events"),
    std::make_pair("HPtl3","W Lepton Pt;Pt;Normalized Events"),
    std::make_pair("HMetPt","Met Pt;Pt;Normalized Events"),
    /*Next series*/
    std::make_pair("HDistl1l2","Distance in Eta-Phi Plane; Distance in Eta-Phi Plane (l1,l2); Normalized Events"),
    std::make_pair("HDistl1l3","Distance in Eta-Phi Plane; Distance in Eta-Phi Plane (l1,l3); Normalized Events"),
    std::make_pair("HDistl2l3","Distance in Eta-Phi Plane; Distance in Eta-Phi Plane (l2,l3); Normalized Events"),
    std::make_pair("HWZDist","Distance in Eta-Phi Plane; Distance in Eta-Phi Plane (W,Z); Normalized Events"),
    /*Next series*/
    std::make_pair("HScaleFactors","Scale Factors; Scale Factor; Normalized events"),
    std::make_pair("HPileup","Number of primary vertices; Npvs; Normalized events"),
    std::make_pair("HPileup","Number of primary vertices; Npvs; Normalized events"),
    std::make_pair("HPileup","Number of primary vertices; Npvs; Normalized events"),

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

  auto c1 = new TCanvas("cs","cs",10,10,1196,772);

  for (auto signal: SignalSamples) {

    Int_t WpMass;
    std::regex rexp1("(Wprime)([A-Za-z_-]+)([0-9]+)(.*)");
    std::smatch sm;
    if(std::regex_search(std::get<1>(signal),sm,rexp1)){
      WpMass = std::stoi(sm[3]);
    }

    c1->Clear();
    c1->Divide(2,2);
    Int_t j = 1;
    TH1F* hCutFlow;
    Float_t nEvents;

    THStack* hscomb = new THStack("hscomb","");
    THStack* hsdummy = new THStack("hsdummy","");
    TH1F* hcombData;

    for (auto HN : HistNames) {
      Int_t r = (j-1)%4;
      c1->cd(r+1);
      const char *hName = std::get<0>(HN).c_str();

      THStack *hs = new THStack("hs","");

      auto legend = new TLegend(0.3, 0.66, .89, .89);
      legend->SetNColumns(2);

      for (auto BGN: BgNames) {
        auto h = (TH1F*)f1->Get(Form("%s/%s",(std::get<1>(BGN)).c_str(),hName));
        h = (TH1F*)h->Clone();
        hCutFlow =
          (TH1F*)f1->Get(Form("%s/%s",(std::get<1>(BGN)).c_str(),"HCutFlow"));
        nEvents = (Float_t)hCutFlow->GetBinContent(1);
        h->SetFillStyle(1001);
        h->SetTitle((std::get<0>(BGN)).c_str());
        h->SetFillColor(std::get<2>(BGN));
        h->Scale(luminosity*std::get<3>(BGN)*pbFactor/nEvents);
        h->SetLineWidth(0);
        hs->Add(h);
        hscomb->Add(static_cast<TH1F*>(h->Clone()));
        legend->AddEntry(h,(std::get<0>(BGN)).c_str(),"F");
      }

      auto hsig = (TH1F*)f1->Get(Form("%s/%s",(std::get<1>(signal)).c_str(),hName));
      hsig = (TH1F*)hsig->Clone();
      hCutFlow =
        (TH1F*)f1->Get(Form("%s/%s",(std::get<1>(signal)).c_str(),"HCutFlow"));
      nEvents = (Float_t)hCutFlow->GetBinContent(1);
      hsig->Scale(luminosity*std::get<2>(signal)*pbFactor/nEvents);
      legend->AddEntry(hsig,(std::get<0>(signal)).c_str(),"L");
      hs->Add(hsig);
      hs->SetTitle((std::get<0>(HN)).c_str());

      gPad->SetLogy();
      gPad->SetTickx();
      gPad->SetTicky();
      fixYRange(hs);
      legend->SetBorderSize(0);
      hs->Draw("HIST");

      auto hdata = (TH1F*)f1->Get(Form("%s/%s",DataName,hName));
      hdata->SetMarkerStyle(kFullCircle);
      hdata->Draw("SAME P");
      legend->AddEntry(hdata, "Data2016");
      if( r+1 == 1 ){
        hcombData = static_cast<TH1F*>(hdata->Clone());
      } else {
        hcombData->Add(hdata);
      }

      ++j;
      legend->Draw();
      if( r+1 == 4 ){
        c1->Print(Form("Stack_%s_Wprime%d_Data.png",hName,WpMass));
        c1->Clear();
        gPad->SetLogy();
        hscomb->Draw("HIST");
        std::clog << Form("\thscomb integral: %s %f\n",hName,((TH1*)hscomb->GetStack()->Last())->Integral());;
        hcombData->Draw("SAME P");
        fixYRange(hscomb);
        legend->Draw();
        c1->Print(Form("Stack_Combined_%s_Wprime%d.png",hName,WpMass));
        c1->Clear();
        c1->Divide(2,2);
        hscomb = static_cast<THStack*>(hsdummy->Clone());
        hscomb->SetName("hscomb");
      }
    }

    c1->Clear();
    c1->Divide(2,2);
    UInt_t canvasPos = 1;
    gStyle->SetOptStat(0);

    j = 1;
    for(auto hp: NonStackedHistos){
      std::string histoName = hp.first;
      Int_t r = (j-1)%4;
      auto legend = new TLegend(0.11, 0.7, .89, .89);
      legend->SetNColumns(3);
      c1->cd(r+1);
      for(auto BGN: BgNames){
        std::string folder = std::get<1>(BGN);
        std::cout << Form("Getting %s/%s\n",folder.c_str(),histoName.c_str());
        auto h = (TH1F*)f1->Get(Form("%s/%s",folder.c_str(),histoName.c_str()));
        h = (TH1F*)h->Clone();
        legend->AddEntry(h,(std::get<0>(BGN)).c_str(), "L");
        legend->SetBorderSize(0);
        h->SetTitle(hp.second.c_str());
        h->SetLineColor(std::get<2>(BGN));
        h->GetXaxis()->SetRangeUser(0., 800.);
        normalizeHisto(h);
        h->SetMaximum(1.5);
        h->Draw("HIST SAME");
      }

      auto hsig = (TH1F*)f1->Get(Form("%s/%s",(std::get<1>(signal)).c_str(),(std::get<0>(hp).c_str())));
      hsig = (TH1F*)hsig->Clone();
      hsig->SetLineColor(kBlack);
      hsig->SetLineWidth(2);
      normalizeHisto(hsig);
      hsig->Draw("HIST SAME");
      legend->AddEntry(hsig,(std::get<0>(signal)).c_str(), "L");

      if(hp.first == "HPileup"){
        auto hdata = (TH1D*)f1->Get(Form("%s/%s",DataName,histoName.c_str()));
        hdata = (TH1D*)hdata->Clone();
        hdata->SetLineColor(kRed);
        hdata->SetLineWidth(2);
        normalizeHisto(hdata);
        hdata->Draw("HIST SAME");
        legend->AddEntry(hdata,"Data","L");
      }

      ++j;
      legend->Draw();
      if( r+1 == 4 ){
        c1->Print(Form("Stack_%s_Wprime%d.png",(std::get<0>(hp)).c_str(),WpMass));
        c1->Clear();
        c1->Divide(2,2);
      }
    }
  }

}
