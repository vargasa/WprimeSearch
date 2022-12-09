#!/bin/bash
YEAR=$1
SAMPLE=$2
BRANCHNAME=$3
NFSTART=${4:0} #FromFile
NFEND=${5:0} #ToFile
MEMORY=1024
NSPLIT=5
NCORES=1

SubmitSingle () {
    START=$1
    END=$2
    OutputLabel=${YEAR}_${SAMPLE}_${START}_${END}
    export jdlString="universe = vanilla
Executable = MakeTrees.sh
should_transfer_files = YES
when_to_transfer_output = ON_EXIT
request_cpus = ${NCORES}
request_memory = ${MEMORY}
Output = EventIDTree_${OutputLabel}.root___\$(Cluster).stdout
Error = EventIDTree_${OutputLabel}.root___\$(Cluster).stdout
Log = EventIDTree_${OutputLabel}.root___\$(Cluster).log
Arguments = ${YEAR} ${SAMPLE} ${BRANCHNAME} ${START} ${END}
Queue 1"
    echo "${jdlString}" > /tmp/condor_job_${YEAR}_${SAMPLE}_${START}_${END}.jdl
    condor_submit /tmp/condor_job_${YEAR}_${SAMPLE}_${START}_${END}.jdl

}

SplitAndSubmit () {
    NLINES=`wc -l < $1`
    if [[ $NFSTART -eq 0 ]]; then START=0; fi
    while true; do
        END=$(( $START + $NSPLIT ))
        if [ $END -gt $NLINES ]; then END=$NLINES; fi
        SubmitSingle $START $END 
        if [ $END -eq $NLINES ]; then break; fi
        START=$(( $END + 1 ))
    done
}

if [[ $NFEND -ne 0 ]]; then SubmitSingle $NFSTART $NFEND; exit; fi

export SAMPLEFILE="${PWD}/../files/data/${YEAR}/UL/${SAMPLE}.txt"

SplitAndSubmit $SAMPLEFILE
