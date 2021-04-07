## Instructions for running on cms-lpc7.fnal.gov

```bash
source /cvmfs/cms.cern.ch/cmsset_default.sh
source /cvmfs/cms.cern.ch/slc7_amd64_gcc820/lcg/root/6.18.04-nmpfii/bin/thisroot.sh
voms-proxy-init --valid 72:00 --voms cms --debug #Set up proxy
```

### Cleanup

```bash
rm -rf plots/2016/*.png
rm -rf plots/2017/*.png
rm -rf plots/2018/*.png

# WARNING: Deletes Everything!
echo "TFile *f = TFile::Open(\"WprimeHistos.root\",\"UPDATE\");gDirectory->rmdir(\"2018;1\");throw" | root -l -b
echo "TFile *f = TFile::Open(\"WprimeHistos.root\",\"UPDATE\");gDirectory->rmdir(\"2017;1\");throw" | root -l -b
echo "TFile *f = TFile::Open(\"WprimeHistos.root\",\"UPDATE\");gDirectory->rmdir(\"2016;1\");throw" | root -l -b
```

```cpp
TFile *f = TFile::Open("WprimeHistos.root","UPDATE");
std::vector<int> years = {2016,2017,2018};
for(const auto& year: years){
  f->cd(Form("%d",year));
  for (auto i: *(gDirectory->GetListOfKeys())) {
    if ( std::string(i->GetName()).find("Single") == std::string::npos ){ /*Do not delete CMSDATA plots just MC*/
      std::cout << Form("%d/%s;1",year,i->GetName()) << std::endl;
      gDirectory->Delete(Form("%s;1",i->GetName()));
    }
  }
}
```


### Setup for MC

Scale factors: Additional notes [here](https://avargash.web.cern.ch/avargash/analysisFiles/scaleFactors/README.txt)

#### Electron Trigger

```bash
#[2016 - LowPt Bin]
wget -P proof/files/mc/2016/sf/ -c https://avargash.web.cern.ch/avargash/analysisFiles/scaleFactors/2016/ElectronTriggerScaleFactors_eta_ele_binned_official_pt30to175_withsyst.root 
#[2016 - HighPt Bin]
wget -P proof/files/mc/2016/sf/ -c https://avargash.web.cern.ch/avargash/analysisFiles/scaleFactors/2016/ElectronTriggerScaleFactors_eta_ele_binned_official_pt175toInf.root
#[2017 - LowPt Bin]
wget -P proof/files/mc/2017/sf/ -c https://avargash.web.cern.ch/avargash/analysisFiles/scaleFactors/2017/ElectronTriggerScaleFactors_eta_ele_binned_official_pt30to200_withsyst.root
#[2017 - HighPt Bin]
wget -P proof/files/mc/2017/sf/ -c https://avargash.web.cern.ch/avargash/analysisFiles/scaleFactors/2017/ElectronTriggerScaleFactors_eta_ele_binned_official_pt200toInf.root
#[2018 - LowPt Bin]
wget -P proof/files/mc/2018/sf/ -c https://avargash.web.cern.ch/avargash/analysisFiles/scaleFactors/2018/ElectronTriggerScaleFactors_eta_ele_binned_official_pt30to200_withsyst.root
#[2018 - HighPt Bin]
wget -P proof/files/mc/2018/sf/ -c https://avargash.web.cern.ch/avargash/analysisFiles/scaleFactors/2018/ElectronTriggerScaleFactors_eta_ele_binned_official_pt200toInf.root
```

#### Electron ID

```bash
# [2016]
wget -P proof/files/mc/2016/sf/ -c https://avargash.web.cern.ch/avargash/analysisFiles/scaleFactors/2016/2016LegacyReReco_ElectronLoose.root
wget -P proof/files/mc/2016/sf/ -c https://avargash.web.cern.ch/avargash/analysisFiles/scaleFactors/2016/2016LegacyReReco_ElectronMedium_Fall17V2.root
wget -P proof/files/mc/2016/sf/ -c https://avargash.web.cern.ch/avargash/analysisFiles/scaleFactors/2016/2016LegacyReReco_ElectronTight_Fall17V2.root
# [UL2016]
wget -P proof/files/mc/2016/sf/UL/ -c https://avargash.web.cern.ch/avargash/analysisFiles/scaleFactors/2016/UL/egammaEffi.txt_Ele_Loose_preVFP_EGM2D.root
wget -P proof/files/mc/2016/sf/UL/ -c https://avargash.web.cern.ch/avargash/analysisFiles/scaleFactors/2016/UL/egammaEffi.txt_Ele_Loose_postVFP_EGM2D.root
wget -P proof/files/mc/2016/sf/UL/ -c https://avargash.web.cern.ch/avargash/analysisFiles/scaleFactors/2016/UL/egammaEffi.txt_Ele_Tight_preVFP_EGM2D.root
wget -P proof/files/mc/2016/sf/UL/ -c https://avargash.web.cern.ch/avargash/analysisFiles/scaleFactors/2016/UL/egammaEffi.txt_Ele_Tight_postVFP_EGM2D.root
# [2017]
wget -P proof/files/mc/2017/sf/ -c https://avargash.web.cern.ch/avargash/analysisFiles/scaleFactors/2017/2017_ElectronLoose.root
wget -P proof/files/mc/2017/sf/ -c https://avargash.web.cern.ch/avargash/analysisFiles/scaleFactors/2017/2017_ElectronMedium.root
wget -P proof/files/mc/2017/sf/ -c https://avargash.web.cern.ch/avargash/analysisFiles/scaleFactors/2017/2017_ElectronTight.root
# [UL2017]
wget -P proof/files/mc/2017/sf/UL/ -c https://avargash.web.cern.ch/avargash/analysisFiles/scaleFactors/2017/UL/egammaEffi.txt_EGM2D_PHO_Loose_UL17.root
wget -P proof/files/mc/2017/sf/UL/ -c https://avargash.web.cern.ch/avargash/analysisFiles/scaleFactors/2017/UL/egammaEffi.txt_EGM2D_PHO_Tight_UL17.root
# [2018]
wget -P proof/files/mc/2018/sf/ -c https://avargash.web.cern.ch/avargash/analysisFiles/scaleFactors/2018/2018_ElectronLoose.root
wget -P proof/files/mc/2018/sf/ -c https://avargash.web.cern.ch/avargash/analysisFiles/scaleFactors/2018/2018_ElectronMedium.root
wget -P proof/files/mc/2018/sf/ -c https://avargash.web.cern.ch/avargash/analysisFiles/scaleFactors/2018/2018_ElectronTight.root
# [UL2018]
wget -P proof/files/mc/2018/sf/UL/ -c https://avargash.web.cern.ch/avargash/analysisFiles/scaleFactors/2018/UL/egammaEffi.txt_Ele_Loose_EGM2D.root
wget -P proof/files/mc/2018/sf/UL/ -c https://avargash.web.cern.ch/avargash/analysisFiles/scaleFactors/2018/UL/egammaEffi.txt_Ele_Tight_EGM2D.root
```

#### Muon Trigger:

```bash
#[2016 GH]
wget -P proof/files/mc/2016/sf/ -c https://avargash.web.cern.ch/avargash/analysisFiles/scaleFactors/2016/EfficienciesAndSF_Period4.root
#[2016 B-F]
wget -P proof/files/mc/2016/sf/ -c https://avargash.web.cern.ch/avargash/analysisFiles/scaleFactors/2016/EfficienciesAndSF_RunBtoF.root
#[UL2016]
wget -P proof/files/mc/2016/sf/UL/ -c https://avargash.web.cern.ch/avargash/analysisFiles/scaleFactors/2016/UL/EfficienciesStudies_UL2016_preVFP_Trigger_rootfiles_Efficiencies_muon_generalTracks_Z_Run2016_UL_HIPM_SingleMuonTriggers.root
wget -P proof/files/mc/2016/sf/UL/ -c https://avargash.web.cern.ch/avargash/analysisFiles/scaleFactors/2016/UL/EfficienciesStudies_UL2016_postVFP_Trigger_rootfiles_Efficiencies_muon_generalTracks_Z_Run2016_UL_SingleMuonTriggers.root
#[2017 BCDEF]
wget -P proof/files/mc/2017/sf/ -c https://avargash.web.cern.ch/avargash/analysisFiles/scaleFactors/2017/EfficienciesAndSF_RunBtoF_Nov17Nov2017.root
#[UL2017]
wget -P proof/files/mc/2017/sf/UL/ -c https://avargash.web.cern.ch/avargash/analysisFiles/scaleFactors/2017/UL/EfficienciesStudies_UL2017_Trigger_rootfiles_Efficiencies_muon_generalTracks_Z_Run2017_UL_SingleMuonTriggers.root
#[2018 ABCD]
wget -P proof/files/mc/2018/sf/ -c https://avargash.web.cern.ch/avargash/analysisFiles/scaleFactors/2018/EfficienciesAndSF_2018Data_AfterMuonHLTUpdate.root
#[UL2018]
wget -P proof/files/mc/2018/sf/UL/ -c https://avargash.web.cern.ch/avargash/analysisFiles/scaleFactors/2018/UL/EfficienciesStudies_UL2018_Trigger_rootfiles_Efficiencies_muon_generalTracks_Z_Run2018_UL_SingleMuonTriggers.root
````

#### Muon ID:

Download filename differs from the one shown in the URL

```bash
#[2016 GH]
wget -P proof/files/mc/2016/sf/ -c https://avargash.web.cern.ch/avargash/analysisFiles/scaleFactors/2016/RunGH_SF_ID.root
#[2016 B-F]
wget -P proof/files/mc/2016/sf/ -c https://avargash.web.cern.ch/avargash/analysisFiles/scaleFactors/2016/RunBCDEF_SF_ID.root
#[UL2016]
wget -P proof/files/mc/2016/sf/UL/ -c https://avargash.web.cern.ch/avargash/analysisFiles/scaleFactors/2016/UL/Efficiencies_muon_generalTracks_Z_Run2016_UL_HIPM_ID.root #preVFP
wget -P proof/files/mc/2016/sf/UL/ -c https://avargash.web.cern.ch/avargash/analysisFiles/scaleFactors/2016/UL/EfficienciesStudies_UL2016_postVFP_DEN_TrackerMuons_rootfiles_Efficiencies_muon_generalTracks_Z_Run2016_UL_ID.root #postVFP
#[2017 BCDEF]
wget -P proof/files/mc/2017/sf/ -c https://avargash.web.cern.ch/avargash/analysisFiles/scaleFactors/2017/RunBCDEF_SF_ID_2017.root
#[UL2017]
wget -P proof/files/mc/2017/sf/UL/ -c https://avargash.web.cern.ch/avargash/analysisFiles/scaleFactors/2017/UL/EfficienciesStudies_UL2017_DEN_TrackerMuons_rootfiles_Efficiencies_muon_generalTracks_Z_Run2017_UL_ID.root
#[2018 ABCD]
wget -P proof/files/mc/2018/sf/ -c https://avargash.web.cern.ch/avargash/analysisFiles/scaleFactors/2018/RunABCD_SF_ID.root
#[UL2018]
wget -P proof/files/mc/2018/sf/UL -c https://avargash.web.cern.ch/avargash/analysisFiles/scaleFactors/2018/UL/EfficienciesStudies_UL2018_DEN_TrackerMuons_rootfiles_Efficiencies_muon_generalTracks_Z_Run2018_UL_ID.root 
```

#### Muon ID [UL]:

```bash
#[2016]
wget -P proof/files/mc/2016/sf/ -c https://avargash.web.cern.ch/avargash/analysisFiles/scaleFactors/2016/EfficienciesStudies_UL2016_postVFP_DEN_TrackerMuons_rootfiles_Efficiencies_muon_generalTracks_Z_Run2016_UL_ID.root
#[2017]
wget -P proof/files/mc/2017/sf/ -c https://avargash.web.cern.ch/avargash/analysisFiles/scaleFactors/2017/EfficienciesStudies_UL2017_DEN_TrackerMuons_rootfiles_Efficiencies_muon_generalTracks_Z_Run2017_UL_ID.root
#[2018]
wget -P proof/files/mc/2018/sf/ -c https://avargash.web.cern.ch/avargash/analysisFiles/scaleFactors/2018/EfficienciesStudies_UL2018_DEN_TrackerMuons_rootfiles_Efficiencies_muon_generalTracks_Z_Run2018_UL_ID.root
```

### Pileup distribution

```bash
wget -c https://avargash.web.cern.ch/avargash/analysisFiles/scaleFactors/PileupWeights.root
```

```bash
#2016
wget -P proof/files/mc/2016/sf/ -c https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions16/13TeV/PileUp/PileupHistogram-goldenJSON-13tev-2016-69200ub.root
#2017
wget -P proof/files/mc/2017/sf/ -c https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions17/13TeV/PileUp/PileupHistogram-goldenJSON-13tev-2017-69200ub.root
#2018
wget -P proof/files/mc/2018/sf/ -c https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions18/13TeV/PileUp/PileupHistogram-goldenJSON-13tev-2018-69200ub.root

```
### kFactors
```
#2016
wget -P proof/files/mc/2016/sf -c https://github.com/rishabhCMS/decaf/raw/master/analysis/data/vjets_SFs/merged_kfactors_zjets.root
#2017 / 2018
wget -P proof/files/mc/2017/sf -c https://github.com/rishabhCMS/decaf/raw/master/analysis/data/vjets_SFs/SF_QCD_NLO_DYJetsToLL.root

```


The pileup scalefactors `PileupWeights.root` is computed by using:

```bash
root -l PileupReweighing.C
```

Which takes as input `WprimeHistos.root` and `PileupHistogram-goldenJSON-13tev-2016-69200ub.root`

```bash
cd proof/
# Copy paste formulas:

echo "#define Y2016" > IsData.h # Make sure CMSDATA is undefined
FILES=files/mc/2016/*.txt #Loop over set of the list files
for i in $FILES
do
 root -l -b -q "Selector.C(\"$i\", 8)"; # 8 Workers
done

echo "#define Y2017" > IsData.h # Make sure CMSDATA is undefined
FILES=files/mc/2017/*.txt #Loop over set of the list files
for i in $FILES
do
 root -l -b -q "Selector.C(\"$i\", 8)"; # 8 Workers
done

echo "#define Y2018" > IsData.h # Make sure CMSDATA is undefined
FILES=files/mc/2018/*.txt #Loop over set of the list files
for i in $FILES
do
 root -l -b -q "Selector.C(\"$i\", 8)"; # 8 Workers
done
```

### Setup for Data

Turns out the different datasets have overlapping events, meaning the same event is found
in different root files and chaining them results in counting the same event multiple times
towards the analysis. To solve this problem, as a first step a `TEntryList` is created for each
dataset with the events passing the so called "Event selection" (i.e `HLTs` and `Flags`), a
`TTree` is also created containing information about the `*run` and `*event` which may used to
identify uniquely events. As a second step, a new set of `TEntryList` is created using this
information, by loading the identification of the events in an `unordered_set` we can go 
sequentially through the datasets and discard the insertion of repeated events in the 
new `TEntryList`. Check `proof/EntryListMaker/README.md` for further details on how to
create the required `EntryLists.root` file.

```bash
# 2016
cd proof/
# Make sure CMSDATA is defined and Year is provided
echo -e "#define Y2016\n#define CMSDATA" > IsData.h
root -l -b -q "Selector.C(\"files/data/2016/SinglePhoton.txt+files/data/2016/SingleElectron.txt+files/data/2016/SingleMuon.txt\", 8, \"EntryListMaker/EntryLists_Unique.root\")"; # 8 Workers
# SampleName is not commutative ;)
# It will look for: SinglePhotonSingleElectronSingleMuon/EntryList into EntryLists_Unique

# 2017
echo -e "#define Y2017\n#define CMSDATA" > IsData.h
root -l -b -q "Selector.C(\"files/data/2017/SinglePhoton.txt+files/data/2017/SingleElectron.txt+files/data/2017/SingleMuon.txt\", 8, \"EntryListMaker/EntryLists_Unique.root\")";

# 2018
echo -e "#define Y2018\n#define CMSDATA" > IsData.h
root -l -b -q "Selector.C(\"files/data/2018/SingleMuon.txt+files/data/2018/EGamma.txt\", 8, \"EntryListMaker/EntryLists_Unique.root\")";

```

This will create `WprimeHistos.root` file which will contain all the histograms
created on `PreSelector.C` classified by the sample name as a root directory.


#### For reference
```cpp
// Copy Data histos to different root file
TFile* fileFrom = TFile::Open("WprimeHistos_Data.root");
TFile* fileTo = TFile::Open("WprimeHistos_MC.root","UPDATE");
std::vector<std::string> dirNames = {
   "2016/SinglePhotonSingleElectronSingleMuon",
   "2017/SinglePhotonSingleElectronSingleMuon",
   "2018/SingleMuonEGamma",
}

for(const auto& dir: dirNames){
  fileTo->mkdir(dir.c_str());
  fileFrom->cd(dir.c_str());
  TObjLink* l = gDirectory->GetListOfKeys()->FirstLink();
  fileTo->cd(dir.c_str());
  while(l){
    std::cout << fileFrom->Get(Form("%s/%s",dir.c_str(),l->GetObject()->GetName()))->Write() << std::endl;
    l = l->Next();
  }
}
```


### Stack

```bash
root -l -b -1 "Stack.C(\"WprimeHistos_all.root\")";
```

Where "_all" stands for MC + Data. It creates a file `WprimeStack_all.root` (or `_label`)
where all the stack plots are produced. Plots are also saved in `png` format in the
`plots/[year]` directory.

### Combine

```bash
# Fix names for DataCards
sed -i 's/t#bar{t}/TT/' plots/*/*.txt
sed -i 's/Z#gamma/ZG/' plots/*/*.txt

for i in 600 800 1000 1200 \
  1400 1600 1800 2000 2500 \
  3000 3500 4000
do
  combine  -m 125 -n "."$i -M AsymptoticLimits -d "datacards/2016_"$i"_DataCard.txt"
done
```

### Comparing graphs for different set of cuts:

```cpp
TFile* f1 = TFile::Open("WprimeStack_TightId.root");
TFile* f2 = TFile::Open("WprimeStack_highPtId.root");

auto c1 = new TCanvas("c1","c1");

auto l = new TLegend(0.69,0.16,0.87,0.26);
l->SetLineWidth(0);

auto g1 = static_cast<TGraph*>(f1->Get("2016_TightId_GPunziS"));
g1->SetTitle("Muon TightId");
g1->SetLineColor(kBlue);
g1->SetMarkerStyle(20);
g1->SetMarkerColor(kBlack);
g1->SetLineWidth(2);
l->AddEntry(g1);

auto g2 = static_cast<TGraph*>(f2->Get("2016_highPtId_GPunziS"));
g2->SetTitle("Muon highPtId");
g2->SetLineColor(kRed);
g2->SetMarkerStyle(20);
g2->SetMarkerColor(kBlack);
g2->SetLineWidth(2);
l->AddEntry(g2);

auto mg = new TMultiGraph();
mg->SetTitle("Punzi Significance ; M(WZ); #frac{#epsilon}{1+#sqrt{B}}");
mg->Add(g1)
mg->Add(g2)

mg->Draw("APL");
c1->SetGridx();
c1->SetGridy();
c1->SetTickx();
c1->SetTicky();
l->Draw();

c1->Print("2016_PunziTest.png")
```

### Efficiency graphs

```c++
std::vector<int> wm = { 800, 1000, 1200, 1400, 1600, 1800, 2000, 2500, 3000, 3500, 4000 };

std::vector<std::pair<std::string,std::string>> hname= {
 {"HDistl1l2_SR","dR(l1,l2) (cm)"},
 {"HLt_SR","#SigmaPt(l)"},
 {"HPtl1_SR","Pt(l1)"},
 {"HPtl2_SR","Pt(l2)"},
 {"HPtl3_SR","Pt(l3)"},
 {"HEtal1_SR","Eta(l1)"},
 {"HEtal2_SR","Eta(l2)"},
 {"HEtal3_SR","Eta(l3)"}
};

auto GetHisto = [&](const std::string filename, const std::string hs) {
 auto f = TFile::Open(filename.c_str());
 // Take 600 as a base and add the rest on top of it
 auto h = static_cast<TH1F*>(f->Get(
  Form("2016/WprimeToWZToWlepZlep_narrow_M-600_13TeV-madgraph/%s_C",hs.c_str())
 )->Clone());
 h->Add(static_cast<TH1D*>(f->Get(
  Form("2016/WprimeToWZToWlepZlep_narrow_M-600_13TeV-madgraph/%s_D",hs.c_str())
 )));
 for(const int& m: wm){
  h->Add(static_cast<TH1D*>(f->Get(Form("2016/WprimeToWZToWlepZlep_narrow_M-%d_13TeV-madgraph/%s_C",m,hs.c_str()))));
  h->Add(static_cast<TH1D*>(f->Get(Form("2016/WprimeToWZToWlepZlep_narrow_M-%d_13TeV-madgraph/%s_D",m,hs.c_str()))));
 }
 return h;
};

for(auto hs: hname){

 TH1F* htotal = GetHisto("WprimeHistos_NoMuonID.root",hs.first);

 TH1F* hpass1 = GetHisto("WprimeHistos_GlobalPF.root",hs.first);
 TGraphAsymmErrors *g1 = new TGraphAsymmErrors(hpass1,htotal);
 g1->SetTitle("GlobalMuon is PF;dR;Efficiency");
 g1->SetLineColor(kGreen);

 TH1F* hpass2 = GetHisto("WprimeHistos_NoMuonPF.root",hs.first);
 TGraphAsymmErrors *g2 = new TGraphAsymmErrors(hpass2,htotal);
 g2->SetTitle("No PF Requirement");
 g2->SetLineColor(kBlack);

 TH1F* hpass3 = GetHisto("WprimeHistos_EitherPF.root",hs.first);
 TGraphAsymmErrors *g3 = new TGraphAsymmErrors(hpass3,htotal);
 g3->SetTitle("Either Muon is PF;dR;Efficiency");
 g3->SetLineColor(kRed);


 TMultiGraph* mg = new TMultiGraph();
 std::vector<TGraph*> gs = { g2,g1,g3 };
 for(auto& g: gs){
   g->SetLineWidth(2);
   mg->Add(g,"P");
 }

 auto c1 = new TCanvas("c1","c1",1200,1200);
 mg->SetTitle(Form("Efficiency; %s Z#rightarrow#mu#mu; Efficiency;",hs.second.c_str()));
 mg->GetYaxis()->SetRangeUser(0.,1.05);
 mg->Draw("A");
 mg->GetYaxis()->SetRangeUser(0.8,1.01);
 gPad->BuildLegend();
 c1->Print(Form("HighPtPF_%s.png",hs.second.c_str()));
}
```

### References:

* [Analysis update B2G Diboson meeting Jan/15th/2021](https://indico.cern.ch/event/993670/contributions/4181199/attachments/2172439/3667916/Wprime%20DIB%20Jan%202021%20%281%29.pdf)
* [Analysis update B2G Diboson meeting Oct/30th/2020](https://indico.cern.ch/event/970049/contributions/4083907/attachments/2134087/3594291/Wprime%20DIB%20Update%20Oct%202020.pdf)
* [Analysis update B2G Diboson meeting Sep/18th/2020](https://indico.cern.ch/event/952830/contributions/4003368/attachments/2105217/3540240/Wprime_DIB_Meeting_Sept2020.pdf)
* [Analysis update B2G Workshop May/18/2020](https://indico.cern.ch/event/891751/timetable/)
* [Analysis update B2G Diboson meeting Feb/7/2020](https://indico.cern.ch/event/886464/)
* [8 TeV Search - EXO-12-025](http://cms.cern.ch/iCMS/analysisadmin/cadilines?line=EXO-12-025)
* [8 TeV Search paper](https://arxiv.org/pdf/1407.3476.pdf)
* [7 TeV Search - EXO-11-041](http://cms.cern.ch/iCMS/analysisadmin/cadilines?line=EXO-11-041)
* [7 TeV Search paper](https://arxiv.org/pdf/1206.0433.pdf)