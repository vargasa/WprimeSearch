#!/bin/bash
#./SubmitImpact.sh RMAXLIM

# RunImpacts.sh $MASS $CR $YEAR $CHANNEL $RMAXLIM $BRANCHNAME
MASS=600
MEMORY=1024
NCORES=1
SubmitSingle () {
    MASS=$1
    CR=$2
    YEAR=$3
    CHANNEL=$4
    RMAXLIM=$5
    BRANCHNAME=$6
    OUTPUTLABEL="ImpactsTest_RMAX_${MASS}_${CR}_${YEAR}_${CHANNEL}_${RMAXLIM}_${BRANCHNAME}"
    jdlString="universe = vanilla
+JobFlavour = \"longlunch\"
+MaxRuntime = 9000
Executable = RunImpacts.sh
should_transfer_files = YES
when_to_transfer_output = ON_EXIT
request_cpus = $NCORES
request_memory = $MEMORY
Error = ${OUTPUTLABEL}_\$(Cluster).stderr
Log = ${OUTPUTLABEL}_\$(Cluster).log

Arguments = $MASS $CR $YEAR $CHANNEL $RMAXLIM $BRANCHNAME

Queue 1"
    echo "$jdlString" > /tmp/condor_job_$OUTPUTLABEL.jdl
    echo /tmp/condor_job_$OUTPUTLABEL.jdl
    condor_submit /tmp/condor_job_$OUTPUTLABEL.jdl
}

BRANCHNAME="16Bins"
for RMAXLIM in `seq -f "%.2f" 90. 10. 110.0`; do
    for CHANNEL in {eee,eemu,mumue,mumumu}; do
        for YEAR in {2016,2017,2018}; do
            for CR in {CR1,CR2}; do
                SubmitSingle $MASS $CR $YEAR $CHANNEL $RMAXLIM $BRANCHNAME
            done
        done
    done
done
