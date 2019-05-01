# Synchronization excercise

## NanoAOD production from MiniAOD

Note: Using `CMSSW_10_2_10` release:

`cmsDriver.py` execution to produce NanoAOD file:

```
cmsDriver.py testNanoAOD -s NANO --mc \
             --n -1 \
             --eventcontent NANOAODSIM \
             --datatier NANOAODSIM \
             --conditions 94X_mcRun2_asymptotic_v3 \
             --filein /store/mc/RunIISummer16MiniAODv3/ttHToNonbb_M125_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8/MINIAODSIM/PUMoriond17_94X_mcRun2_asymptotic_v3-v2/120000/F24F2D5E-DDEC-E811-AF50-90B11C08AD7D.root 
```