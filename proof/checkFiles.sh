#!/bin/bash

OUTPUTFOLDER="/uscms/home/avargash/store/WprimeSearchCondorOutput/"
YEARP=$1
TYPEP=$2
OUTPUTLABEL=$3
SAMPLEFILENAME=$4
SPACING_FACTOR=${5:-1.15}
LIMIT_EDGE=${6:-1350.0}
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
    FILE=`printf "%s/WprimeHistos_%s_%d_%s_%s_%s_%s____%04d_%04d.root" $OUTPUTFOLDER $OUTPUTLABEL $YEARP $TYPEP $SAMPLEFILENAME $SPACING_FACTOR $LIMIT_EDGE $NFSTART $NFEND`
    #file $FILE
    if [ ! -f "$FILE" ]; then
        printf "\n./SubmitCondor.sh %s %s %s %s %s %s %s %s %s %s" $OUTPUTLABEL $YEARP $TYPEP $SAMPLEFILENAME $SPACING_FACTOR $LIMIT_EDGE $NFSTART $NFEND
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
