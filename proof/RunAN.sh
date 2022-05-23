#!/bin/bash
YEARP=$1 #2016->2018
TYPEP=$2 #MC or DATA
SAMPLEFILENAME=$3 # ULX.txt
OutputLabel=$4
NFSTART=${5:0} #FromFile
NFEND=${6:-1} #ToFile
source /cvmfs/cms.cern.ch/cmsset_default.sh
export SCRAM_ARCH=slc7_amd64_gcc900
export NCORES=4
scram project CMSSW CMSSW_12_2_1
cd CMSSW_12_2_1/src
eval `scram runtime -sh`
echo  "CMSSW Dir: "$CMSSW_BASE
git clone https://github.com/vargasa/WprimeSearch.git Wprime_$YEARP
WprimeDir=$PWD/Wprime_$YEARP/
echo "Analysis Dir: "$WprimeDir
#sed -i 's/cmsxrootd.fnal.gov/xrootd-cms.infn.it/' $WprimeDir/proof/Selector.C
cd $WprimeDir/proof
wget -c https://avargash.web.cern.ch/avargash/WprimeSearch/x509up_u114404
export X509_USER_PROXY=$PWD/x509up_u114404
voms-proxy-info -all
voms-proxy-info -all -file $X509_USER_PROXY


DownloadSFs () {
    SFDir=$WprimeDir/proof/files/mc/$YEARP/UL/sf/
    xrdcp root://cmseos.fnal.gov//store/user/avargash/WprimeSearch/proof/ULPileup.root $WprimeDir/proof/
    mkdir -vp proof/files/mc/2016/sf/
    # KFactors are shared amoung Run2 years
    xrdcp root://cmseos.fnal.gov//store/user/avargash/WprimeSearch/proof/files/mc/2016/sf/merged_kfactors_zjets.root $WprimeDir/proof/files/mc/2016/sf/
    # HLT Muon SF combined file
    xrdcp root://cmseos.fnal.gov//store/user/avargash/WprimeSearch/proof/files/mc/2016/UL/sf/OutFile-v20190510-Combined-Run2016BtoH_Run2017BtoF_Run2018AtoD-M120to10000.root $WprimeDir/proof/files/mc/2016/UL/sf/
    if [[ $YEARP -eq 2016 ]]; then
        SF_FILES="2016_postVFP_reco_egammaEffi_ptBelow20.txt_EGM2D_UL2016postVFP.root 2016_preVFP_reco_egammaEffi_ptBelow20.txt_EGM2D_UL2016preVFP.root  egammaEffi_ptAbove20.txt_EGM2D_UL2016postVFP.root egammaEffi_ptAbove20.txt_EGM2D_UL2016preVFP.root egammaEffiHLT.txt_EGM2D_2016PreVFPTight.root egammaEffiHLT.txt_EGM2D_2016PreVFPMedium.root egammaEffiHLT.txt_EGM2D_2016PreVFPLoose.root egammaEffiHLT.txt_EGM2D_2016PostVFPTight.root egammaEffiHLT.txt_EGM2D_2016PostVFPMedium.root egammaEffiHLT.txt_EGM2D_2016PostVFPLoose.root egammaEffi.txt_Ele_Tight_preVFP_EGM2D.root egammaEffi.txt_Ele_Tight_postVFP_EGM2D.root egammaEffi.txt_Ele_Medium_preVFP_EGM2D.root egammaEffi.txt_Ele_Medium_postVFP_EGM2D.root  egammaEffi.txt_Ele_Loose_preVFP_EGM2D.root  egammaEffi.txt_Ele_Loose_postVFP_EGM2D.root  EfficienciesStudies_UL2016_preVFP_Trigger_rootfiles_Efficiencies_muon_generalTracks_Z_Run2016_UL_HIPM_SingleMuonTriggers.root  EfficienciesStudies_UL2016_postVFP_Trigger_rootfiles_Efficiencies_muon_generalTracks_Z_Run2016_UL_SingleMuonTriggers.root  EfficienciesStudies_UL2016_postVFP_DEN_TrackerMuons_rootfiles_Efficiencies_muon_generalTracks_Z_Run2016_UL_ID.root  Efficiencies_muon_generalTracks_Z_Run2016_UL_ID.root  Efficiencies_muon_generalTracks_Z_Run2016_UL_HIPM_ID.root"
    elif [[ $YEARP -eq 2017 ]]; then
        SF_FILES="2017_reco_egammaEffi_ptBelow20.txt_EGM2D_UL2017.root egammaEffi_ptAbove20.txt_EGM2D_UL2017.root Efficiencies_muon_generalTracks_Z_Run2017_UL_ID.root EfficienciesStudies_UL2017_Trigger_rootfiles_Efficiencies_muon_generalTracks_Z_Run2017_UL_SingleMuonTriggers.root egammaEffi.txt_EGM2D_Loose_UL17.root egammaEffi.txt_EGM2D_Medium_UL17.root egammaEffi.txt_EGM2D_Tight_UL17.root egammaEffiHLT.txt_EGM2D_2017Loose.root egammaEffiHLT.txt_EGM2D_2017Medium.root egammaEffiHLT.txt_EGM2D_2017Tight.root"
    elif [[ $YEARP -eq 2018 ]]; then
        SF_FILES="2018_reco_egammaEffi_ptBelow20.txt_EGM2D_UL2018.root egammaEffi_ptAbove20.txt_EGM2D_UL2018.root Efficiencies_muon_generalTracks_Z_Run2018_UL_ID.root egammaEffi.txt_Ele_Loose_EGM2D.root egammaEffi.txt_Ele_Medium_EGM2D.root egammaEffi.txt_Ele_Tight_EGM2D.root egammaEffiHLT.txt_EGM2D_2018Loose.root egammaEffiHLT.txt_EGM2D_2018Medium.root egammaEffiHLT.txt_EGM2D_2018Tight.root"
    fi
    for i in $SF_FILES; do
        xrdcp root://cmseos.fnal.gov//store/user/avargash/WprimeSearch/proof/files/mc/$YEARP/UL/sf/$i $SFDir/$i
    done
}

if [ "$TYPEP" =  "MC" ]; then
    DownloadSFs $YEARP
    cd $WprimeDir/proof/
    SAMPLEFILE=$WprimeDir/proof/files/mc/$YEARP/UL/$SAMPLEFILENAME
    echo "Processing Year:"$YEARP
    echo -e "#define Y"$YEARP"\n#define ULSAMPLE\n" > IsData.h # Make sure CMSDATA is undefined
    root -l -b -q "Selector.C(\""$OutputLabel"\",\""$SAMPLEFILE"\","$NCORES",\"\",$NFSTART,$NFEND)";
elif [ "$TYPEP" = "DATA" ]; then
    cd $WprimeDir/proof/
    ENTRYLISTFILE="root://cmseos.fnal.gov//store/user/avargash/WprimeSearch/proof/EntryListMaker/EntryLists_Unique.root"
    echo -e "#define Y"$YEARP"\n#define CMSDATA\n#define ULSAMPLE" > IsData.h
    SAMPLEFILE=$WprimeDir/proof/files/data/$YEARP/UL/$SAMPLEFILENAME
    ROOTCommand="\""$OutputLabel"\",\""$SAMPLEFILE"\","$NCORES",\""$ENTRYLISTFILE"\",$NFSTART,$NFEND";
    echo -e "#define Y"$YEARP"\n#define CMSDATA\n#define ULSAMPLE" > IsData.h
    echo $ROOTCommand
    root -l -b -q "Selector.C("$ROOTCommand")";
fi

cd $WprimeDir/proof/
for i in `ls WprimeHistos_*.root`;
do
    xrdcp -vf $i root://cmseos.fnal.gov//store/user/avargash/WprimeSearchCondorOutput/$i
done
