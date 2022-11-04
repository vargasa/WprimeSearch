#!/bin/bash
#WprimeHistos_ANv5LPC_2018_MC_ZZTo4L_M-1toInf_TuneCP5_13TeV_powheg_pythia8.txt__0000_0005.root
#./checkFiles 2018 MC ANv5LPC ZZTo4L_M-1toInf_TuneCP5_13TeV_powheg_pythia8.txt

OUTPUTFOLDER="/uscms/home/avargash/store/WprimeSearchCondorOutput/"
YEARP=$1
TYPEP=$2
OUTPUTLABEL=$3
SAMPLEFILENAME=$4
SPACING_FACTOR=${5:-1.15}
FIRSTBIN_LEFTEDGE=${6:-60.0}
FIRSTBIN_RIGHTEDGE=${7:-70.0}
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
    #WprimeHistos_master_2018_MC_WprimeToWZToWlepZlep_narrow_M1200_TuneCP5_13TeV-madgraph-pythia8.txt_1.2_60.0_70.0___0006_0009.root
    FILE=`printf "%s/WprimeHistos_%s_%d_%s_%s_%s_%s_%s___%04d_%04d.root" $OUTPUTFOLDER $OUTPUTLABEL $YEARP $TYPEP $SAMPLEFILENAME $SPACING_FACTOR $FIRSTBIN_LEFTEDGE $FIRSTBIN_RIGHTEDGE $NFSTART $NFEND`
    #file $FILE
    if [ ! -f "$FILE" ]; then
        printf "\n./SubmitCondor.sh %s %d %s %s %d %d %s %s %s" $OUTPUTLABEL $YEARP $TYPEP $SAMPLEFILENAME $NFSTART $NFEND $SPACING_FACTOR $FIRSTBIN_LEFTEDGE $FIRSTBIN_RIGHTEDGE
        PASS=false
    fi
    if [ $NFEND -eq $NLINES ]; then break; fi
    NFSTART=$(( $NFEND + 1 ))
done

#echo -e $FILESMISSING

#if $PASS; then
#    echo "COMPLETED: "$SAMPLEFILENAME
#    hadd `printf "%s/WprimeHistos_%d_%s_%s_Merged.root" $OUTPUTFOLDER $YEARP $TYPEP $SAMPLEFILENAME` `printf "%s/WprimeHistos_%s_%d_%s_%s*.root" $OUTPUTFOLDER $OUTPUTLABEL $YEARP $TYPEP $SAMPLEFILENAME`
#fi
