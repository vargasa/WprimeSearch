#!/bin/bash
MASS=$1
CR=$2
YEAR=$3
CHANNEL=$4
RMAXLIM=$5
xrdcp -vf root://cmseos.fnal.gov//store/user/avargash/WprimeSearch/CMSSW_10_2_13_Combine.tgz ./
source /cvmfs/cms.cern.ch/cmsset_default.sh
tar -xf CMSSW_10_2_13_Combine.tgz
rm CMSSW_10_2_13_Combine.tgz
cd CMSSW_10_2_13/src
scramv1 b ProjectRename
eval `scramv1 runtime -sh`
COMBINELIMITDIR=$CMSSW_BASE/src/HiggsAnalysis/CombinedLimit/
cd $COMBINELIMITDIR

mkdir $COMBINELIMITDIR/datacards
for i in `xrdfs root://eosuser.cern.ch/ ls /eos/user/a/avargash/www/WprimeSearch/datacards/|grep .txt`; do
    FNAME=`echo ${i}| xargs -n1 basename`
    xrdcp root://eosuser.cern.ch/${i} $COMBINELIMITDIR/datacards/${FNAME}
done

for i in `xrdfs root://eosuser.cern.ch/ ls /eos/user/a/avargash/www/WprimeSearch/datacards/|grep CombineFile`; do
    FNAME=`echo ${i}| xargs -n1 basename`
    xrdcp root://eosuser.cern.ch/${i} $COMBINELIMITDIR/datacards/${FNAME}
done


DCARDIR=$COMBINELIMITDIR/datacards/
ALLMASSP="600"
RMAXLIM=20
i=$MASS
j=$CR
k=$YEAR
l=$CHANNEL
LABEL=${l}_HMassWZ_${j}_${k}_${i}
DCARD=${DCARDIR}/DataCard_${LABEL}
combineTool.py -M T2W -o ${DCARD}.root -i ${DCARD}.txt
DCARD=${DCARDIR}/DataCard_${LABEL}.root
RMINLIM=-${RMAXLIM}
combineTool.py -n ${j}_${k} -M Impacts -d $DCARD -m $i --rMin ${RMINLIM} --rMax ${RMAXLIM} --doInitialFit --robustFit 1
combineTool.py -n ${j}_${k} -M Impacts -d $DCARD -m $i --rMin ${RMINLIM} --rMax ${RMAXLIM} --robustFit 1 --doFits
combineTool.py -n ${j}_${k} -M Impacts -d $DCARD -m $i --rMin ${RMINLIM} --rMax ${RMAXLIM} -o Impacts_${LABEL}.json
plotImpacts.py --label-size 0.05 --cms-label ${LABEL} -i Impacts_${LABEL}.json -o ImpactsPlot_${LABEL}_${RMAXLIM}

cd $COMBINELIMITDIR
for i in `ls *.pdf *png *json`;
do
    xrdcp -vf $i root://cmseos.fnal.gov//store/user/avargash/WprimeSearchCondorOutput/ImpactTest/$i
done
