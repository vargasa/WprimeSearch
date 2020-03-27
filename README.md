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
```
This will create `WprimeHistos.root` file which will contain all the histograms
created on `PreSelector.C` classified by the sample name as a root directory.

```bash
root -l -b -1 "Stack.C(\"WprimeHistos.root\")";
```

### Setup for Data
```bash
echo "#define CMSDATA" > proof/IsData.h # Make sure CMSDATA is defined
FILES=files/2016/data/*.txt
```

### Run:
```
for i in $FILES
do
 root -l -b -q "proof/Selector.C(\"$i\", 10)"; # 10 Workers
done
```