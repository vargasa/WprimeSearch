## Instructions for running on cms-lpc7.fnal.gov

```
source /cvmfs/cms.cern.ch/cmsset_default.sh
source /cvmfs/cms.cern.ch/slc7_amd64_gcc820/lcg/root/6.18.04-nmpfii/bin/thisroot.sh
voms-proxy-init --voms cms --debug #Set up proxy
```

### Setup for MC
```bash
echo "" > proof/IsData.h # Make sure CMSDATA is undefined
FILES=files/2016/*.txt #Loop over set of the list files
for i in $FILES
do
 root -l -b -q "proof/Selector.C(\"$i\", 10)"; # 10 Workers
done

```

This will create `WprimeHistos.root` file which will contain all the histograms
created on `PreSelector.C` classified by the sample name as a root directory.

### Setup for Data

Turns out the different datasets have overlapping events, meaning the same event is found
in different root files and chaining them results in counting the same event multiple times
towards the analysis. To solve this problem, as a first step a `TEntryList` is created for each
dataset with the events passing the so called "Event selection" (i.e `HLTs` and `Flags`), a
`TTree` is also created containing information about the `*run` and `*event` which may used to
identify uniquely events. As a second step, a new set of `TEntryList` is created using this
information, by loading the identification of the events in an `unordered_set` we can go 
sequentially through the datasets and discard the insertion of repeated events in the 
new `TEntrlList`. 

```bash
echo "#define CMSDATA" > proof/IsData.h # Make sure CMSDATA is defined
root -l -b -q "proof/Selector.C(\"files/2016/data/DoubleEG.txt+files/2016/data/SingleMuon.txt\", 10)"; # 10 Workers
```

### Stack

```bash
root -l -b -1 "Stack.C(\"WprimeHistos.root\")";
```
