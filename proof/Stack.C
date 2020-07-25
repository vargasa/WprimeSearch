#include "THStack.h"

void Stack(std::string FileName = "WprimeHistos_all.root"){

  TH1::SetDefaultSumw2();

  const Float_t luminosity = 35.9e15; /* 35.9fb^-1 2016 */
  const Float_t pbFactor = 1e-12; /*pico prefix*/

  const char* DataName = "SinglePhotonSingleElectronSingleMuon";

  // ShortName, DasName, kColor, Style, XSection, nEvents
  std::vector<std::tuple<std::string,std::string,Int_t,Float_t>> BgNames = {
    std::make_tuple("WZ","WZTo3LNu_TuneCUETP8M1_13TeV-powheg-pythia8",
                    kOrange,4.43), /*XSDB 2nd Sample is 0*/
    std::make_tuple("DYJetsToLL","DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8",
                    kOrange+7,5941.0),
    std::make_tuple("t#bar{t}","TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
                    kBlue-2,56.86),
    std::make_tuple("t#bar{t} Ext1","TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_EXT1",
                    kBlue-4,56.86),
    std::make_tuple("Z#gamma","ZGTo2LG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8",
                    kRed+3,123.8), /*AN2019_252_v1*/
    std::make_tuple("Z#gamma Ext1","ZGTo2LG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_EXT1",
                    kRed+3,123.8),
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
    std::make_tuple("HMassZA","3e0#mu;M_{Z}^{3e0#mu}(GeV);Event count/2GeV"),
    std::make_tuple("HMassZB","2e1#mu;M_{Z}^{2e1#mu}(GeV);Event count/2GeV"),
    std::make_tuple("HMassZC","1e2#mu;M_{Z}^{1e2#mu}(GeV);Event count/2GeV"),
    std::make_tuple("HMassZD","0e3#mu;M_{Z}^{0e3#mu}(GeV);Event count/2GeV"),
    /* Another series */
    std::make_tuple("HMetA","3e0#mu;#slash{E}^{3e0#mu}_{T}(GeV);Event count/10GeV"),
    std::make_tuple("HMetB","2e1#mu;#slash{E}^{2e1#mu}_{T}(GeV);Event count/10GeV"),
    std::make_tuple("HMetC","1e2#mu;#slash{E}^{1e2#mu}_{T}(GeV);Event count/10GeV"),
    std::make_tuple("HMetD","0e3#mu;#slash{E}^{0e3#mu}_{T}(GeV);Event count/10GeV"),
    /* Another series */
    std::make_tuple("HMassTWA","M_{T}^{W}(3e0#mu);M_{WT}^{3e0#mu};Event count/5GeV"),
    std::make_tuple("HMassTWB","M_{T}^{W}(2e1#mu);M_{WT}^{2e1#mu};Event count/5GeV"),
    std::make_tuple("HMassTWC","M_{T}^{W}(1e2#mu);M_{WT}^{1e2#mu};Event count/5GeV"),
    std::make_tuple("HMassTWD","M_{T}^{W}(0e3#mu);M_{WT}^{0e3#mu};Event count/5GeV"),
    /* Another series */
    std::make_tuple("HMassWZA","WZ Mass (3e0#mu);M_{Z}^{3e0#mu};Event count/100GeV"),
    std::make_tuple("HMassWZB","WZ Mass (2e1#mu);M_{Z}^{2e1#mu};Event count/100GeV"),
    std::make_tuple("HMassWZC","WZ Mass (1e2#mu);M_{Z}^{1e2#mu};Event count/100GeV"),
    std::make_tuple("HMassWZD","WZ Mass (0e3#mu);M_{Z}^{0e3#mu};Event count/100GeV"),
    /* Another series */
    std::make_tuple("HPileupA","Number of Good Primary Vertices;nPvs;Event count"),
    std::make_tuple("HPileupB","Number of Good Primary Vertices;nPvs;Event count"),
    std::make_tuple("HPileupC","Number of Good Primary Vertices;nPvs;Event count"),
    std::make_tuple("HPileupD","Number of Good Primary Vertices;nPvs;Event count"),
    /* Another series*/
    std::make_tuple("HMassZA_NoSF","3e0#mu;M_{Z}^{3e0#mu}(GeV);Event count"),
    std::make_tuple("HMassZB_NoSF","2e1#mu;M_{Z}^{2e1#mu}(GeV);Event count"),
    std::make_tuple("HMassZC_NoSF","1e2#mu;M_{Z}^{1e2#mu}(GeV);Event count"),
    std::make_tuple("HMassZD_NoSF","0e3#mu;M_{Z}^{0e3#mu}(GeV);Event count"),
    /* Another series */
    std::make_tuple("HMetA_NoSF","3e0#mu;#slash{E}^{3e0#mu}_{T}(GeV);Event count"),
    std::make_tuple("HMetB_NoSF","2e1#mu;#slash{E}^{2e1#mu}_{T}(GeV);Event count"),
    std::make_tuple("HMetC_NoSF","1e2#mu;#slash{E}^{1e2#mu}_{T}(GeV);Event count"),
    std::make_tuple("HMetD_NoSF","0e3#mu;#slash{E}^{0e3#mu}_{T}(GeV);Event count"),
    /* Another series */
    std::make_tuple("HMassTWA_NoSF","M_{T}^{W}(3e0#mu);M_{WT}^{3e0#mu};Event count"),
    std::make_tuple("HMassTWB_NoSF","M_{T}^{W}(2e1#mu);M_{WT}^{2e1#mu};Event count"),
    std::make_tuple("HMassTWC_NoSF","M_{T}^{W}(1e2#mu);M_{WT}^{1e2#mu};Event count"),
    std::make_tuple("HMassTWD_NoSF","M_{T}^{W}(0e3#mu);M_{WT}^{0e3#mu};Event count"),
    /* Another series */
    std::make_tuple("HMassWZA_NoSF","WZ Mass (3e0#mu);M_{Z}^{3e0#mu};Event count"),
    std::make_tuple("HMassWZB_NoSF","WZ Mass (2e1#mu);M_{Z}^{2e1#mu};Event count"),
    std::make_tuple("HMassWZC_NoSF","WZ Mass (1e2#mu);M_{Z}^{1e2#mu};Event count"),
    std::make_tuple("HMassWZD_NoSF","WZ Mass (0e3#mu);M_{Z}^{0e3#mu};Event count"),
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

  auto getBGStack = [&](std::string hname, THStack *hscomb = NULL,TLegend* legend = NULL){
    THStack* hstck = new THStack();
    for (auto BGN: BgNames) {
      std::string hpath = Form("%s/%s",(std::get<1>(BGN)).c_str(),hname.c_str());
      std::clog << "Reading: " << hpath << std::endl;
      auto h = static_cast<TH1F*>(f1->Get(hpath.c_str()));
      h = static_cast<TH1F*>(h->Clone());
      auto hCutFlow = static_cast<TH1F*>(f1->Get(Form("%s/%s",(std::get<1>(BGN)).c_str(),"HCutFlow")));
      Float_t nEvents = (Float_t)hCutFlow->GetBinContent(1);
      h->SetFillStyle(1001);
      h->SetTitle((std::get<1>(BGN)).c_str());
      h->SetFillColor(std::get<2>(BGN));
      h->Scale(luminosity*std::get<3>(BGN)*pbFactor/nEvents);
      h->SetLineWidth(0);
      hstck->Add(h);
      if(hscomb) hscomb->Add(h);
      if(legend)
        legend->AddEntry(h,(std::get<0>(BGN)).c_str(),"F");
    }
    return hstck;
  };

  auto c1 = new TCanvas("cs","cs",10,10,2400,1200);

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

    THStack* hscomb = new THStack("hscomb","");
    THStack* hsdummy = new THStack("hsdummy","");
    TH1F* hcombData;

    auto hratio = new TH1F("hratio","DataMCRatios",50,0.,10.);
    TH1F* hfdummy = new TH1F("","DataMCRatios",50,0.,10.);

    for (auto HN : HistNames) {
      Int_t r = (j-1)%4;
      c1->cd(r+1);
      const char *hName = std::get<0>(HN).c_str();

      THStack *hs = new THStack("hs","");

      auto legend = new TLegend(0.3, 0.66, .87, .89);
      legend->SetNColumns(2);

      hs = getBGStack(hName,hscomb,legend);

      auto hsig = (TH1F*)f1->Get(Form("%s/%s",(std::get<1>(signal)).c_str(),hName));
      hsig = (TH1F*)hsig->Clone();
      TH1F* hCutFlow =
        (TH1F*)f1->Get(Form("%s/%s",(std::get<1>(signal)).c_str(),"HCutFlow"));
      auto nEvents = (Float_t)hCutFlow->GetBinContent(1);
      hsig->Scale(luminosity*std::get<2>(signal)*pbFactor/nEvents);
      legend->AddEntry(hsig,(std::get<0>(signal)).c_str(),"L");
      hs->Add(hsig);
      hs->SetTitle((std::get<1>(HN)).c_str());

      gPad->SetLogy();
      gPad->SetTickx();
      gPad->SetTicky();
      fixYRange(hs);
      legend->SetBorderSize(0);

      Float_t leftMargin = 0.12;
      Float_t rightMargin = 0.12;
      Float_t topMargin = 0.12;
      Float_t bottomMargin = 0.5;

      auto mainPad = new TPad("subPad","subPad",0.,0.25,1.,1.);
      mainPad->Draw();
      mainPad->SetLeftMargin(leftMargin);
      mainPad->SetRightMargin(rightMargin);
      mainPad->SetBottomMargin(1e-3);
      mainPad->SetLogy();
      auto subPad = new TPad("mainPad","mainPad",0.,0.,1.,0.25);
      subPad->Draw();
      subPad->SetLeftMargin(leftMargin);
      subPad->SetRightMargin(rightMargin);
      subPad->SetTopMargin(1e-3);
      subPad->SetBottomMargin(bottomMargin);

      gStyle->SetOptStat(0);

      mainPad->cd();
      hs->Draw("HIST");

      auto hdata = (TH1F*)f1->Get(Form("%s/%s",DataName,hName));
      if (!hdata) {
        std::string tmp = hName;
        tmp = tmp.substr(0,tmp.find("_NoSF"));
        std::clog << Form("Printing data on %s_NoSF plot\n",tmp.c_str());
        hdata = static_cast<TH1F*>(f1->Get(Form("%s/%s",DataName,tmp.c_str())));
      }

      hdata->SetMarkerStyle(kFullCircle);
      mainPad->cd();
      hdata->Draw("SAME HISTO P");
      TH1F* herror = getErrorHisto(hs);
      herror->Draw("SAME E2");
      legend->AddEntry(hdata, "Data2016");

      auto hcdata = getRatio(hdata,hs);
      subPad->cd();
      hcdata->SetMaximum(2.);
      hcdata->SetMinimum(0.);
      hcdata->Draw();
      TLine *line = new TLine(hdata->GetXaxis()->GetXmin(),1.,
                              hdata->GetXaxis()->GetXmax(),1.);
      line->SetLineColor(kBlack);
      line->Draw();


      for(uint i = 0; i < hcdata->GetNbinsX(); i++){
        float xx = hcdata->GetBinContent(i);
        if(xx > 1e-3) hratio->Fill(xx);
      }

      if( r+1 == 1 ){
        hcombData = static_cast<TH1F*>(hdata->Clone());
      } else {
        hcombData->Add(hdata);
      }

      ++j;
      mainPad->cd();
      legend->Draw();
      if( r+1 == 4 ){
        c1->Print(Form("Stack_%s_Wprime%d_Data.png",hName,WpMass));
        c1->Clear();
        gPad->SetLogy();
        hscomb = sortStack(hscomb);
        hscomb->Draw("HIST");
        std::clog << Form("\thscomb integral: %s %f\n",hName,((TH1*)hscomb->GetStack()->Last())->Integral());
        hcombData->Draw("SAME P");
        fixYRange(hscomb);
        legend->Draw();
        c1->Print(Form("Stack_Combined_%s_Wprime%d.png",hName,WpMass));
        hratio->Draw("HIST");
        c1->Print(Form("Stack_Combined_DataMCRatios_%s_Wprime%d.png",hName,WpMass));
        hratio = static_cast<TH1F*>(hfdummy->Clone());
        hratio->SetName("hratio");
        c1->Clear();
        c1->Divide(2,2);
        hscomb = static_cast<THStack*>(hsdummy->Clone());
        hscomb->SetName("hscomb");
      }
    }

    /*** HPileup ***/
    c1->Clear();
    auto legend = new TLegend();
    THStack *hspileup = getBGStack("HPileup",NULL,legend);
    hspileup->Draw("HIST E2");
    legend->Draw();
    auto hdata = (TH1D*)f1->Get(Form("%s/HPileup",DataName));
    hdata = (TH1D*)hdata->Clone();
    hdata->SetLineColor(kRed);
    hdata->SetLineWidth(2);
    hdata->Draw("HIST SAME P");
    c1->Print("HPileup.png");


    /*** NonStackedHistos ***/

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
