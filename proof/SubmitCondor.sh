#!/bin/bash
#./SubmitCondor BRANCHNAME 2018 DATA ULEGamma.txt NFILESTART NFILEEND SPACING_FACTOR
BRANCHNAME=$1
YEARP=$2
TYPEP=$3
SAMPLEFILENAME=$4
NCORES=1
MEMORY=1024
NSPLIT=10
SPACING_FACTOR=${5:-1.15}
NFSTART=${6:0}
NFEND=${7:0}
FIRSTBIN_LEFTEDGE=${8:-60.0}
FIRSTBIN_RIGHTEDGE=${9:-70.0}
OUTPUTLABEL="${BRANCHNAME}_${YEARP}_${TYPEP}_${SAMPLEFILENAME}_"

SubmitSingle () {
    OUTPUTLABEL="${BRANCHNAME}_${YEARP}_${TYPEP}_${SAMPLEFILENAME}_${SPACING_FACTOR}_${FIRSTBIN_LEFTEDGE}_${FIRSTBIN_RIGHTEDGE}__"
    NFSTART=$1
    NFEND=$2
    echo $OUTPUTLABEL
    printf "%4d_%4d_%s\n" $NFSTART $NFEND $SPACING_FACTOR
    OutputLabel=`printf "%s_%04d_%04d" $OUTPUTLABEL $NFSTART $NFEND`
    jdlString="universe = vanilla
+JobFlavour = \"longlunch\"
+MaxRuntime = 9000
Executable = RunAN.sh
should_transfer_files = YES
when_to_transfer_output = ON_EXIT
request_cpus = $NCORES
request_memory = $MEMORY
Output = WprimeHistos_"$OutputLabel".root___\$(Cluster).stdout
Error = WprimeHistos_"$OutputLabel".root___\$(Cluster).stderr
Log = WprimeHistos_"$OutputLabel".root___\$(Cluster).log
Arguments = $BRANCHNAME $YEARP $TYPEP $SAMPLEFILENAME $OutputLabel $NFSTART $NFEND $SPACING_FACTOR $FIRSTBIN_LEFTEDGE $FIRSTBIN_RIGHTEDGE
Queue 1"
    echo "$jdlString" > /tmp/condor_job_$OutputLabel.jdl
    echo /tmp/condor_job_$OutputLabel.jdl
    condor_submit /tmp/condor_job_$OutputLabel.jdl
}

SplitAndSubmit () {
    NLINES=`wc -l < $1`
    SPACING_FACTOR=$2
    if [[ $NFSTART -eq 0 ]]; then NFSTART=0; fi
    while true; do
        NFEND=$(( $NFSTART + $NSPLIT ))
        if [ $NFEND -gt $NLINES ]; then NFEND=$NLINES; fi
        SubmitSingle $NFSTART $NFEND 
        if [ $NFEND -eq $NLINES ]; then break; fi
        NFSTART=$(( $NFEND + 1 ))
    done
}

if [[ $NFEND -ne 0 ]]; then SubmitSingle $NFSTART $NFEND $SPACING_FACTOR; exit; fi

if [ "$TYPEP" =  "DATA" ]; then
    export SAMPLEFILE=$PWD"/files/data/"$YEARP"/UL/"$SAMPLEFILENAME
elif [ "$TYPEP" = "MC" ]; then
    export SAMPLEFILE=$PWD"/files/mc/"$YEARP"/UL/"$SAMPLEFILENAME
fi

SplitAndSubmit $SAMPLEFILE $SPACING_FACTOR

