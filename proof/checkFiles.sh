#!/bin/bash
#WprimeHistos_ANv5LPC_2018_MC_ZZTo4L_M-1toInf_TuneCP5_13TeV_powheg_pythia8.txt__0000_0005.root
#./checkFiles 2018 MC ANv5LPC ZZTo4L_M-1toInf_TuneCP5_13TeV_powheg_pythia8.txt

OUTPUTFOLDER="/uscms/home/avargash/store/WprimeSearchCondorOutput/"
YEARP=$1
TYPEP=$2
OUTPUTLABEL=$3
SAMPLEFILENAME=$4
NSPLIT=10
if [ $TYPEP = "MC" ]; then export PATHTOSAMPLE=$PWD/files/mc/$YEARP/UL/$SAMPLEFILENAME; fi
if [ $TYPEP = "DATA" ]; then export PATHTOSAMPLE=$PWD/files/data/$YEARP/UL/$SAMPLEFILENAME; fi
NLINES=`wc -l < $PATHTOSAMPLE`
if [[ $NFSTART -eq 0 ]]; then NFSTART=0; fi
PASS=true
while true; do
    NFEND=$(( $NFSTART + $NSPLIT ))
    if [ $NFEND -gt $NLINES ]; then NFEND=$NLINES; fi
    if [ $NFEND -eq $NFSTART ]; then break; fi
    FILE=`printf "%s/WprimeHistos_%s_%d_%s_%s__%04d_%04d.root" $OUTPUTFOLDER $OUTPUTLABEL $YEARP $TYPEP $SAMPLEFILENAME $NFSTART $NFEND`
    if [ ! -f "$FILE" ]; then
        printf "./SubmitCondor.sh %d %s %s %d %d\n" $YEARP $TYPEP $SAMPLEFILENAME $NFSTART $NFEND
        PASS=false
    fi
    if [ $NFEND -eq $NLINES ]; then break; fi
    NFSTART=$(( $NFEND + 1 ))
done

if $PASS; then
    hadd `printf "%s/WprimeHistos_%d_%s_%s_Merged.root" $OUTPUTFOLDER $YEARP $TYPEP $SAMPLEFILENAME` `printf "%s/WprimeHistos_%s_%d_%s_%s*.root" $OUTPUTFOLDER $OUTPUTLABEL $YEARP $TYPEP $SAMPLEFILENAME`
fi
