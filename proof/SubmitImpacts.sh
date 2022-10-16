#!/bin/bash
#./SubmitImpact.sh RMAXLIM
#!/bin/bash
#./SubmitImpact.sh RMAXLIM
MEMORY=1024
NCORES=1
SubmitSingle () {
    RMAXLIM=$1
    OUTPUTLABEL="ImpactsTest_RMAX_${RMAXLIM}"
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
Arguments = $RMAXLIM
Queue 1"
    echo "$jdlString" > /tmp/condor_job_$OUTPUTLABEL.jdl
    echo /tmp/condor_job_$OUTPUTLABEL.jdl
    condor_submit /tmp/condor_job_$OUTPUTLABEL.jdl
}

for i in `seq -f "%.2f" 0.0 0.1 50.0`; do
    SubmitSingle $i
done
