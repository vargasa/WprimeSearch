#!/bin/bash
export NCORES=3
export jdlString="universe = vanilla
Executable = MakeTrees.sh
should_transfer_files = YES
when_to_transfer_output = ON_EXIT
request_cpus = $NCORES
request_memory = 6000
Output = MakeTrees_\$(Cluster)_\$(Process)_$1_$2.stdout
Error = MakeTrees_\$(Cluster)_\$(Process)_$1_$2.stderr
Log = MakeTrees_\$(Cluster)_\$(Process)_$1_$2.log
Arguments = $1 $2
Queue 1"
echo "${jdlString}" > /tmp/condor_job_$1_$2.jdl
condor_submit /tmp/condor_job_$1_$2.jdl
