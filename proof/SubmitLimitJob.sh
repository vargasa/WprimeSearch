#!/bin/bash
#./SubmitLimitJob.sh MASS RLIMIT
MASS=$1
RLIMIT=$2
NCORES=1
MEMORY=4096
jdlString="universe = vanilla
Executable = RunCombineHybrid.sh
should_transfer_files = YES
when_to_transfer_output = ON_EXIT
request_cpus = $NCORES
request_memory = $MEMORY
Output = CondorLimit_"$MASS"_"$RLIMIT"__\$(Cluster).stdout
Error = CondorLimit_"$MASS"_"$RLIMIT"__\$(Cluster).stdout
Log = CondorLimit_"$MASS"_"$RLIMIT"__\$(Cluster).stdout
Arguments = $MASS $RLIMIT
Queue 1"
echo "$jdlString" > /tmp/condor_job_limit_$MASS"_"$RLIMIT.jdl
echo /tmp/condor_job_limit_$MASS"_"$RLIMIT.jdl
condor_submit /tmp/condor_job_limit_$MASS"_"$RLIMIT.jdl
