#!/bin/bash
BRANCHNAME=$1
NCORES=2
YEARP=$2 #2016->2018
TYPEP=$3 #MC or DATA
SAMPLEFILENAME=$4 # ULX.txt
OutputLabel=$5
NFSTART=${6:0} #FromFile
NFEND=${7:-1} #ToFile
source /cvmfs/cms.cern.ch/cmsset_default.sh
export SCRAM_ARCH=slc7_amd64_gcc900
scram project CMSSW CMSSW_12_2_1
cd CMSSW_12_2_1/src
eval `scram runtime -sh`
echo  "CMSSW Dir: "$CMSSW_BASE
git clone --branch $BRANCHNAME https://github.com/vargasa/WprimeSearch.git Wprime_$YEARP
WprimeDir=$PWD/Wprime_$YEARP/
echo "Analysis Dir: "$WprimeDir
sed -i 's/cmsxrootd.fnal.gov/xrootd-cms.infn.it/' $WprimeDir/proof/Selector.C
#sed -i 's/cmsxrootd.fnal.gov/xrootd.unl.edu/' $WprimeDir/proof/Selector.C
#sed -i 's/cmsxrootd.fnal.gov/cms-xrd-global.cern.ch/' $WprimeDir/proof/Selector.C
cd $WprimeDir/proof
wget -c https://avargash.web.cern.ch/avargash/WprimeSearch/x509up_u114404
export X509_USER_PROXY=$PWD/x509up_u114404
voms-proxy-info -all
voms-proxy-info -all -file $X509_USER_PROXY

cd $WprimeDir/proof/

if [ "$TYPEP" =  "MC" ]; then
    export SAMPLEFILE=$WprimeDir/proof/files/mc/$YEARP/UL/$SAMPLEFILENAME
    export ENTRYLISTFILE=""
    echo -e "#define Y"$YEARP"\n#define ULSAMPLE\n" > IsData.h # Make sure CMSDATA is undefined
elif [ "$TYPEP" = "DATA" ]; then
    export SAMPLEFILE=$WprimeDir/proof/files/data/$YEARP/UL/$SAMPLEFILENAME
    export ENTRYLISTFILE="root://cmseos.fnal.gov//store/user/avargash/WprimeSearch/proof/EntryListMaker/EntryLists_Unique.root"
    echo -e "#define Y"$YEARP"\n#define CMSDATA\n#define ULSAMPLE" > IsData.h
fi
ROOTCommand="\""$OutputLabel"\",\""$SAMPLEFILE"\","$NCORES",\""$ENTRYLISTFILE"\",$NFSTART,$NFEND";
root -l -b -q "Selector.C("$ROOTCommand")";


cd $WprimeDir/proof/
for i in `ls WprimeHistos_*.root`;
do
    xrdcp -vf $i root://cmseos.fnal.gov//store/user/avargash/WprimeSearchCondorOutput/$i
done
