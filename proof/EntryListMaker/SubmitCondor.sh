#!/bin/bash
YEAR=$1
SAMPLE=$2
BRANCHNAME=$3
export NCORES=3
export jdlString="universe = vanilla
Executable = MakeTrees.sh
should_transfer_files = YES
when_to_transfer_output = ON_EXIT
request_cpus = $NCORES
request_memory = 6000
Output = MakeTrees_\$(Cluster)_\$(Process)_${YEAR}_${SAMPLE}_${BRANCHNAME}.stdout
Error = MakeTrees_\$(Cluster)_\$(Process)_${YEAR}_${SAMPLE}_${BRANCHNAME}.stderr
Log = MakeTrees_\$(Cluster)_\$(Process)_${YEAR}_${SAMPLE}_${BRANCHNAME}.log
Arguments = ${YEAR} ${SAMPLE} ${BRANCHNAME}
Queue 1"
echo "${jdlString}" > /tmp/condor_job_${YEAR}_${SAMPLE}_${BRANCHNAME}.jdl
condor_submit /tmp/condor_job_${YEAR}_${SAMPLE}_${BRANCHNAME}.jdl
