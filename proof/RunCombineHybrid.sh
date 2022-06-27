#!/bin/bash
# RunCombineHybrid MASS RLIMIT
# requires:
# cp /afs/cern.ch/user/a/avargash/eos/Combine/CMSSW_10_2_13/src/HiggsAnalysis/CombinedLimit/datacards/*.* /eos/user/a/avargash/www/WprimeSearch/datacards/
source /cvmfs/cms.cern.ch/cmsset_default.sh
SCRAM_ARCH=slc6_amd64_gcc700
MASS=$1
LIMIT=$2
scram project CMSSW_10_2_13
cd CMSSW_10_2_13/src
eval `scram runtime -sh`
git clone https://github.com/cms-analysis/HiggsAnalysis-CombinedLimit.git HiggsAnalysis/CombinedLimit
cd $CMSSW_BASE/src/HiggsAnalysis/CombinedLimit
git fetch origin
git checkout v8.2.0
cd $CMSSW_BASE/src/
bash <(curl -s https://raw.githubusercontent.com/cms-analysis/CombineHarvester/master/CombineTools/scripts/sparse-checkout-ssh.sh)
scramv1 b clean; scramv1 b --ignore-arch
cd $CMSSW_BASE/src/
mkdir datacards
cd $CMSSW_BASE/src/datacards
cd datacards
wget -c "https://avargash.web.cern.ch/avargash/WprimeSearch/datacards/RunII_SR1_"$MASS"_Datacard.root"
cd $CMSSW_BASE/src/
cmsenv
combine -v 2 -M HybridNew --datacard "datacards/RunII_SR1_"$MASS"_Datacard.root" \
        --LHCmode LHC-limits --saveHybridResult \
        -m $MASS -n "Wprime_"$MASS"_r"$LIMIT \
        -T 100 --saveToys --expectedFromGrid=$LIMIT
for i in `ls *.root`;
do
    xrdcp -vf $i root://cmseos.fnal.gov//store/user/avargash/WprimeSearchCondorOutput/$i
done
