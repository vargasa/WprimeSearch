#!/bin/bash
OUTPUTFOLDER="/uscms/home/avargash/store/WprimeSearchCondorOutput/EntryListMaker/"
YEARP=$1
BRANCHNAME=$2
SAMPLE=$3
NFEND=0
NSPLIT=5
PATHTOSAMPLE=$PWD/../files/data/$YEARP/UL/${SAMPLE}.txt
NLINES=`wc -l < $PATHTOSAMPLE`
if [[ $NFSTART -eq 0 ]]; then NFSTART=0; fi
PASS=true
while true; do
    NFEND=$(( $NFSTART + $NSPLIT ))
    if [ $NFEND -gt $NLINES ]; then NFEND=$NLINES; fi
    if [ $NFEND -eq $NFSTART ]; then break; fi
    #EntryLists_2018_ULEGamma_0_10.root  
    FILE1=`printf "%s/EventIDTree_%d_%s_%d_%d.root" $OUTPUTFOLDER $YEARP $SAMPLE $NFSTART $NFEND`
    #file $FILE1
    if [ ! -f "$FILE1" ]; then
        #./SubmitCondorMakeTrees.sh 2018 ULEGamma OnlyGT36 451 461
        printf "\n./SubmitCondorMakeTrees.sh %d %s %s %d %d" $YEARP $SAMPLE $BRANCHNAME $NFSTART $NFEND
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
