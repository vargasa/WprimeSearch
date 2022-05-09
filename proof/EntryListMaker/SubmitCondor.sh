#!/bin/bash
export NCORES=3
export jdlString="universe = vanilla\nExecutable = MakeTrees.sh\nshould_transfer_files = YES\nwhen_to_transfer_output = ON_EXIT\nrequest_cpus = $NCORES\nrequest_memory = 6000\nOutput = MakeTrees_\$(Cluster)_\$(Process)_$1_$2.stdout\nError = RunAN_\$(Cluster)_\$(Process)_$1_$2.stderr\nLog = RunAN_\$(Cluster)_\$(Process)_$1_$2.log\nArguments = $1 $2\nQueue 1"
echo -e $jdlString > /tmp/condor_job_$1_$2.jdl
condor_submit /tmp/condor_job_$1_$2.jdl
