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
    if ( i->GetName()[0] != 'S' ){ /*Do not delete CMSDATA plots just MC*/
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
# [2016 - TightId]
wget -P proof/files/mc/2016/sf/ -c https://avargash.web.cern.ch/avargash/analysisFiles/scaleFactors/2016/2016LegacyReReco_ElectronTight_Fall17V2.root
# [2017 - TightId]
wget -P proof/files/mc/2017/sf/ -c https://avargash.web.cern.ch/avargash/analysisFiles/scaleFactors/2017/2017_ElectronTight.root
# [2018 - TightId]
wget -P proof/files/mc/2018/sf/ -c https://avargash.web.cern.ch/avargash/analysisFiles/scaleFactors/2018/2018_ElectronTight.root
```

#### Muon Trigger:

```bash
#[2016 GH]
wget -P proof/files/mc/2016/sf/ -c https://avargash.web.cern.ch/avargash/analysisFiles/scaleFactors/2016/EfficienciesAndSF_Period4.root
#[2016 B-F]
wget -P proof/files/mc/2016/sf/ -c https://avargash.web.cern.ch/avargash/analysisFiles/scaleFactors/2016/EfficienciesAndSF_RunBtoF.root
#[2017 BCDEF]
wget -P proof/files/mc/2017/sf/ -c https://avargash.web.cern.ch/avargash/analysisFiles/scaleFactors/2017/EfficienciesAndSF_RunBtoF_Nov17Nov2017.root
#[2018 ABCD]
wget -P proof/files/mc/2018/sf/ -c https://avargash.web.cern.ch/avargash/analysisFiles/scaleFactors/2018/EfficienciesAndSF_2018Data_AfterMuonHLTUpdate.root
````

#### Muon ID:

Download filename differs from the one shown in the URL

```bash
#[2016 GH]
wget -P proof/files/mc/2016/sf/ -c https://avargash.web.cern.ch/avargash/analysisFiles/scaleFactors/2016/RunGH_SF_ID.root
#[2016 B-F]
wget -P proof/files/mc/2016/sf/ -c https://avargash.web.cern.ch/avargash/analysisFiles/scaleFactors/2016/RunBCDEF_SF_ID.root
#[2017 BCDEF]
wget -P proof/files/mc/2017/sf/ -c https://avargash.web.cern.ch/avargash/analysisFiles/scaleFactors/2017/RunBCDEF_SF_ID_2017.root
#[2018 ABCD]
wget -P proof/files/mc/2018/sf/ -c https://avargash.web.cern.ch/avargash/analysisFiles/scaleFactors/2018/RunABCD_SF_ID.root
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

### References:

* [Analysis update B2G Workshop May/18/2020](https://indico.cern.ch/event/891751/timetable/)
* [Analysis update B2G Diboson meetings Feb/7/2020](https://indico.cern.ch/event/886464/)
* [8 TeV Search - EXO-12-025](http://cms.cern.ch/iCMS/analysisadmin/cadilines?line=EXO-12-025)
* [8 TeV Search paper](https://arxiv.org/pdf/1407.3476.pdf)
* [7 TeV Search - EXO-11-041](http://cms.cern.ch/iCMS/analysisadmin/cadilines?line=EXO-11-041)
* [7 TeV Search paper](https://arxiv.org/pdf/1206.0433.pdf)