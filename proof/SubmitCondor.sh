#!/bin/bash
YEARP=$1
TYPEP=$2
SAMPLEFILENAME=$3
NCORES=2
MEMORY=2048
SAMPLEFILE=$PWD"/files/data/"$YEARP"/UL/"$SAMPLEFILENAME
NLINES=`wc -l < $SAMPLEFILE`
NSPLIT=50;
NFSTART=0
OutputLabel="ElRecoSFFixCERN_"$1"_"$2"_"$3"_"
while true; do
    NFEND=$(( $NFSTART + $NSPLIT ))
    if [ $NFEND -gt $NLINES ]; then NFEND=$NLINES; fi
    OutputLabel_=$OutputLabel"_"$NFSTART"_"$NFEND
    #echo $OutputLabel_
    export jdlString="universe = vanilla
+JobFlavour = \"tomorrow\"
Executable = RunAN.sh
should_transfer_files = YES
when_to_transfer_output = ON_EXIT
request_cpus = $NCORES
request_memory = $MEMORY
Output = RunAN_\$(Cluster)_\$(Process)_$1_$2_$3.stdout
Error = RunAN_\$(Cluster)_\$(Process)_$1_$2_$3.stderr
Log = RunAN_\$(Cluster)_\$(Process)_$1_$2_$3.log
Arguments = $YEARP $TYPEP $SAMPLEFILENAME $OutputLabel_ $NFSTART $NFEND
Queue 1"
    echo "$jdlString" > /tmp/condor_job_$OutputLabel_.jdl
    echo /tmp/condor_job_$OutputLabel_.jdl
    #condor_submit /tmp/condor_job_$OutputLabel_.jdl
    NFSTART=$(( $NFEND + 1 ))
    if [ $NFEND -eq $NLINES ]; then break; fi
done
