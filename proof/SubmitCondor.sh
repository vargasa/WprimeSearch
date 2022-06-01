#!/bin/bash
#./SubmitCondor 2018 DATA ULEGamma.txt
YEARP=$1
TYPEP=$2
SAMPLEFILENAME=$3
NCORES=2
MEMORY=2048
NSPLIT=20;
NFSTART=${4:0}
NFEND=${5:0}
OUTPUTLABEL="HEMCorrMET_"$YEARP"_"$TYPEP"_"$SAMPLEFILENAME"_"

SubmitSingle () {
    printf "%4d_%4d\n" $1 $2
    OutputLabel=`printf "%s_%04d_%04d" $OUTPUTLABEL $1 $2`
    jdlString="universe = vanilla
+JobFlavour = \"longlunch\"
Executable = RunAN.sh
should_transfer_files = YES
when_to_transfer_output = ON_EXIT
request_cpus = $NCORES
request_memory = $MEMORY
Output = RunAN_"$OutputLabel"_\$(Cluster).stdout
Error = RunAN_"$OutputLabel"_\$(Cluster).stderr
Log = RunAN_"$OutputLabel"_\$(Cluster).log
Arguments = $YEARP $TYPEP $SAMPLEFILENAME $OutputLabel $1 $2
Queue 1"
    echo "$jdlString" > /tmp/condor_job_$OutputLabel.jdl
    echo /tmp/condor_job_$OutputLabel.jdl
    condor_submit /tmp/condor_job_$OutputLabel.jdl
}

SplitAndSubmit () {
    NLINES=`wc -l < $1`
    if [[ $NFSTART -eq 0 ]]; then NFSTART=0; fi
    while true; do
        NFEND=$(( $NFSTART + $NSPLIT ))
        if [ $NFEND -gt $NLINES ]; then NFEND=$NLINES; fi
        SubmitSingle $NFSTART $NFEND
        if [ $NFEND -eq $NLINES ]; then break; fi
        NFSTART=$(( $NFEND + 1 ))
    done
}

if [[ $NFEND -ne 0 ]]; then SubmitSingle $NFSTART $NFEND; exit; fi

if [ "$TYPEP" =  "DATA" ]; then
    export SAMPLEFILE=$PWD"/files/data/"$YEARP"/UL/"$SAMPLEFILENAME
elif [ "$TYPEP" = "MC" ]; then
    export SAMPLEFILE=$PWD"/files/mc/"$YEARP"/UL/"$SAMPLEFILENAME
fi
SplitAndSubmit $SAMPLEFILE
