## Instructions for running on cms-lpc7.fnal.gov

```
source /cvmfs/cms.cern.ch/cmsset_default.sh
source /cvmfs/cms.cern.ch/slc7_amd64_gcc820/lcg/root/6.18.04-nmpfii/bin/thisroot.sh
voms-proxy-init --voms cms --debug #Set up proxy
```

### Setup for MC

Scale factors: Additional notes [here](https://avargash.web.cern.ch/avargash/analysisFiles/scaleFactors/README.txt)

#### Electron Trigger

```
#[2016 - LowPt Bin]
wget -c https://avargash.web.cern.ch/avargash/analysisFiles/scaleFactors/ElectronTriggerScaleFactors_eta_ele_binned_official_pt30to175_withsyst.root
#[2016 - HighPt Bin]
wget -c https://avargash.web.cern.ch/avargash/analysisFiles/scaleFactors/ElectronTriggerScaleFactors_eta_ele_binned_official_pt175toInf.root
```

#### Muon Trigger:

```
#[2016 GH]
wget -c https://avargash.web.cern.ch/avargash/analysisFiles/scaleFactors/EfficienciesAndSF_Period4.root
#[2016 B-F]
wget -c https://avargash.web.cern.ch/avargash/analysisFiles/scaleFactors/EfficienciesAndSF_RunBtoF.root
````
#### Muon ID:

Download filename differs from the one shown in the URL

```
#[2016 GH]
wget -c https://avargash.web.cern.ch/avargash/analysisFiles/scaleFactors/RunGH_SF_ID.root
#[2016 B-F]
wget -c https://avargash.web.cern.ch/avargash/analysisFiles/scaleFactors/RunBCDEF_SF_ID.root
```

### Pileup distribution
```
#2016
wget -c https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions16/13TeV/PileUp/PileupHistogram-goldenJSON-13tev-2016-69200ub.root

```

```bash
cd proof/
echo "" > IsData.h # Make sure CMSDATA is undefined
FILES=files/mc/2016/*.txt #Loop over set of the list files
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
cd proof/
echo "#define CMSDATA" > IsData.h # Make sure CMSDATA is defined
root -l -b -q "Selector.C(\"files/data/2016/SinglePhoton.txt+files/data/2016/SingleElectron.txt+files/data/2016/SingleMuon.txt\", 8, \"EntryLists_Unique.root\")"; # 8 Workers
```

This will create `WprimeHistos.root` file which will contain all the histograms
created on `PreSelector.C` classified by the sample name as a root directory.

### Stack

```bash
root -l -b -1 "Stack.C(\"WprimeHistos_all.root\")";
```

Where "_all" stands for MC + Data.

### References:

* [Analysis update B2G Workshop May/18/2020](https://indico.cern.ch/event/891751/timetable/)
* [Analysis update B2G Diboson meetings Feb/7/2020](https://indico.cern.ch/event/886464/)
* [8 TeV Search - EXO-12-025](http://cms.cern.ch/iCMS/analysisadmin/cadilines?line=EXO-12-025)
* [8 TeV Search paper](https://arxiv.org/pdf/1407.3476.pdf)
* [7 TeV Search - EXO-11-041](http://cms.cern.ch/iCMS/analysisadmin/cadilines?line=EXO-11-041)
* [7 TeV Search paper](https://arxiv.org/pdf/1206.0433.pdf)