#!/bin/bash
source /cvmfs/cms.cern.ch/cmsset_default.sh
export SCRAM_ARCH=slc7_amd64_gcc900
scram project CMSSW CMSSW_12_2_1
cd CMSSW_12_2_1/src
eval `scram runtime -sh`
echo $CMSSW_BASE
git clone https://github.com/vargasa/WprimeSearch.git Wprime_$1
WprimeDir=$CMSSW_BASE/src/Wprime_$1/
OutputLabel="ANv5Mixv2v9_"$1"_"$2
cd $WprimeDir

DownloadSFs () {
    SFDir=$WprimeDir/proof/files/mc/$1/UL/sf/
    xrdcp root://cmseos.fnal.gov//store/user/avargash/WprimeSearch/proof/PileupWeights_UL.root $WprimeDir/proof/
    mkdir -vp proof/files/mc/2016/sf/
    # KFactors are shared amoung Run2 years
    xrdcp root://cmseos.fnal.gov//store/user/avargash/WprimeSearch/proof/files/mc/2016/sf/merged_kfactors_zjets.root $WprimeDir/proof/files/mc/2016/sf/
    # HLT Muon SF combined file
    xrdcp root://cmseos.fnal.gov//store/user/avargash/WprimeSearch/proof/files/mc/2016/UL/sf/OutFile-v20190510-Combined-Run2016BtoH_Run2017BtoF_Run2018AtoD-M120to10000.root $WprimeDir/proof/files/mc/2016/UL/sf/
    if [[ $1 -eq 2016 ]]; then
        SF_FILES="egammaEffi_ptAbove20.txt_EGM2D_UL2016postVFP.root egammaEffi_ptAbove20.txt_EGM2D_UL2016preVFP.root egammaEffiHLT.txt_EGM2D_2016PreVFPTight.root egammaEffiHLT.txt_EGM2D_2016PreVFPMedium.root egammaEffiHLT.txt_EGM2D_2016PreVFPLoose.root egammaEffiHLT.txt_EGM2D_2016PostVFPTight.root egammaEffiHLT.txt_EGM2D_2016PostVFPMedium.root egammaEffiHLT.txt_EGM2D_2016PostVFPLoose.root egammaEffi.txt_Ele_Tight_preVFP_EGM2D.root egammaEffi.txt_Ele_Tight_postVFP_EGM2D.root egammaEffi.txt_Ele_Medium_preVFP_EGM2D.root egammaEffi.txt_Ele_Medium_postVFP_EGM2D.root  egammaEffi.txt_Ele_Loose_preVFP_EGM2D.root  egammaEffi.txt_Ele_Loose_postVFP_EGM2D.root  EfficienciesStudies_UL2016_preVFP_Trigger_rootfiles_Efficiencies_muon_generalTracks_Z_Run2016_UL_HIPM_SingleMuonTriggers.root  EfficienciesStudies_UL2016_postVFP_Trigger_rootfiles_Efficiencies_muon_generalTracks_Z_Run2016_UL_SingleMuonTriggers.root  EfficienciesStudies_UL2016_postVFP_DEN_TrackerMuons_rootfiles_Efficiencies_muon_generalTracks_Z_Run2016_UL_ID.root  Efficiencies_muon_generalTracks_Z_Run2016_UL_ID.root  Efficiencies_muon_generalTracks_Z_Run2016_UL_HIPM_ID.root"
    elif [[ $1 -eq 2017 ]]; then
        SF_FILES="egammaEffi_ptAbove20.txt_EGM2D_UL2017.root Efficiencies_muon_generalTracks_Z_Run2017_UL_ID.root EfficienciesStudies_UL2017_Trigger_rootfiles_Efficiencies_muon_generalTracks_Z_Run2017_UL_SingleMuonTriggers.root egammaEffi.txt_EGM2D_Loose_UL17.root egammaEffi.txt_EGM2D_Medium_UL17.root egammaEffi.txt_EGM2D_Tight_UL17.root egammaEffiHLT.txt_EGM2D_2017Loose.root egammaEffiHLT.txt_EGM2D_2017Medium.root egammaEffiHLT.txt_EGM2D_2017Tight.root"
    elif [[ $1 -eq 2018 ]]; then
        SF_FILES="egammaEffi_ptAbove20.txt_EGM2D_UL2018.root Efficiencies_muon_generalTracks_Z_Run2018_UL_ID.root egammaEffi.txt_Ele_Loose_EGM2D.root egammaEffi.txt_Ele_Medium_EGM2D.root egammaEffi.txt_Ele_Tight_EGM2D.root egammaEffiHLT.txt_EGM2D_2018Loose.root egammaEffiHLT.txt_EGM2D_2018Medium.root egammaEffiHLT.txt_EGM2D_2018Tight.root"
    fi
    for i in $SF_FILES; do
        xrdcp root://cmseos.fnal.gov//store/user/avargash/WprimeSearch/proof/files/mc/$1/UL/sf/$i $SFDir/$i
    done
}

if [[ "$2" -eq "MC" ]]; then
    DownloadSFs $1
    cd $WprimeDir/proof/

    FILES=$WprimeDir/proof/files/mc/$1/UL/*.txt #Loop over set of the list files

    echo "Processing Year:"$1

    for i in $FILES
    do
        echo -e "#define Y"$1"\n#define ULSAMPLE\n" > IsData.h # Make sure CMSDATA is undefined
        root -l -b -q "Selector.C(\""$OutputLabel"\",\"$i\", 6)";
    done

elif [[ "$2" -eq "DATA" ]]; then
    xrdcp root://cmseos.fnal.gov//store/user/avargash/WprimeSearch/proof/EntryListMaker/EntryLists_Unique.root $WprimeDir/proof/EntryListMaker/
    echo -e "#define Y"$1"\n#define CMSDATA\n#define ULSAMPLE" > IsData.h
    if [ $1 -eq 2016 ] || [ $1 -eq 2017 ]; then
        ROOTCommand="Selector.C(\""$OutputLabel"\",\"files/data/"$1"/UL/ULSinglePhoton.txt+files/data/"$1"/UL/ULSingleElectron.txt+files/data/"$1"/UL/ULSingleMuon.txt\", 8, \"EntryListMaker/EntryLists_Unique.root\")";
    elif [[ $1 -eq 2018 ]]; then
        ROOTCommand="Selector.C(\""$OutputLabel"\",\"files/data/2018/UL/ULSingleMuon.txt+files/data/2018/UL/ULEGamma.txt\", 8, \"EntryListMaker/EntryLists_Unique.root\")";
    fi
    echo -e "#define Y"$1"\n#define CMSDATA\n#define ULSAMPLE" > IsData.h
    root -l -b -q $ROOTCommand;
fi

xrdcp -f $WprimeDir/proof/WprimeHistos_$OutputLabel.root root://cmseos.fnal.gov//store/user/avargash/WprimeSearchCondorOutput/WprimeHistos_$OutputLabel.root 2>&1
XRDEXIT=$?
if [[ $XRDEXIT -ne 0 ]]; then
    rm *.root
    echo "exit code $XRDEXIT, failure in xrdcp"
    exit $XRDEXIT
fi
