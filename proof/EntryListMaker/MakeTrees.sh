#!/bin/bash
YEAR=$1
SAMPLE=$2
BRANCHNAME=$3
source /cvmfs/cms.cern.ch/cmsset_default.sh
export SCRAM_ARCH=slc7_amd64_gcc900
export NCORES=3
scram project CMSSW CMSSW_12_2_1
cd CMSSW_12_2_1/src
eval `scram runtime -sh`
echo $CMSSW_BASE
git clone --branch $BRANCHNAME https://github.com/vargasa/WprimeSearch.git Wprime_$YEAR
WprimeDir=$CMSSW_BASE/src/Wprime_$YEAR/
#sed -i 's/cmsxrootd.fnal.gov/xrootd-cms.infn.it/' $WprimeDir/proof/EntryListMaker/MakeEventIDTree.C
#sed -i 's/cmsxrootd.fnal.gov/xrootd.unl.edu/' $WprimeDir/proof/EntryListMaker/MakeEventIDTree.C
#sed -i 's/cmsxrootd.fnal.gov/cms-xrd-global.cern.ch/' $WprimeDir/proof/EntryListMaker/MakeEventIDTree.C
OutputLabel="EntryListMaker_"$YEAR"_"$SAMPLE
cd $WprimeDir/proof/EntryListMaker/
export FILES=$WprimeDir/proof/files/data/$YEAR/UL/$SAMPLE.txt

echo -e "#define Y"$YEAR"\n#define CMSDATA\n#define ULSAMPLE">../IsData.h
root -l -b -q "MakeEventIDTree.C(\""$FILES"\","$NCORES")"

cd $WprimeDir/proof/EntryListMaker/
for i in `ls *.root`
do
    xrdcp -vf $i root://cmseos.fnal.gov//store/user/avargash/WprimeSearchCondorOutput/EntryListMaker/$i
done
