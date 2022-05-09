#!/bin/bash
export NCORES=3
export jdlString="universe = vanilla\nExecutable = RunAN.sh\nshould_transfer_files = YES\nwhen_to_transfer_output = ON_EXIT\nrequest_cpus = $NCORES\nrequest_memory = 6500\nOutput = RunAN_\$(Cluster)_\$(Process)_$1_$2_$3.stdout\nError = RunAN_\$(Cluster)_\$(Process)_$1_$2_$3.stderr\nLog = RunAN_\$(Cluster)_\$(Process)_$1_$2_$3.log\nArguments = $1 $2 $3\nQueue 1"
echo -e $jdlString > /tmp/condor_job_$1_$2_$3.jdl
condor_submit /tmp/condor_job_$1_$2_$3.jdl
