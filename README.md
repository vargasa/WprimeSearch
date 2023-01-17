## Instructions for running on cms-lpc7.fnal.gov

```bash
source /cvmfs/cms.cern.ch/cmsset_default.sh
source /cvmfs/cms.cern.ch/slc7_amd64_gcc820/lcg/root/6.18.04-nmpfii/bin/thisroot.sh
voms-proxy-init --rfc --valid 172:00 --voms cms --debug #Set up proxy
```

### Cleanup

```bash
rm -rf plots/2016/*.png
rm -rf plots/2017/*.png
rm -rf plots/2018/*.png

# WARNING: Deletes Everything!
echo "TFile *f = TFile::Open(\"WprimeHistos.root\",\"UPDATE\");gDirectory->rmdir(\"2018;1\");throw" | root -l -b
echo "TFile *f = TFile::Open(\"WprimeHistos.root\",\"UPDATE\");gDirectory->rmdir(\"2017;1\");throw" | root -l -b
echo "TFile *f = TFile::Open(\"WprimeHistos.root\",\"UPDATE\");gDirectory->rmdir(\"2016;1\");throw" | root -l -b
```

```cpp
TFile *f = TFile::Open("WprimeHistos_AddSystCRDraft.root","UPDATE");
std::vector<int> years = {2016,2017,2018};
for(const auto& year: years){
  f->cd(Form("%d",year));
  for (auto i: *(gDirectory->GetListOfKeys())) {
    if ( std::string(i->GetName()).find("Single") == std::string::npos 
         and std::string(i->GetName()).find("EGamma") == std::string::npos ){ /*Do not delete CMSDATA plots just MC*/
      std::cout << Form("%d/%s;1",year,i->GetName()) << std::endl;
      gDirectory->Delete(Form("%s;1",i->GetName()));
    }
  }
}
```

```cpp
//Delete specific samples from file:

TFile *f = TFile::Open("WprimeHistos_NewPileupWeight.root","UPDATE");
std::vector<std::string> histos = {
   "2017/TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8",
   "2018/ZZTo4L_13TeV_powheg_pythia8_TuneCP5",
   "2018/TTJets_DiLept_TuneCP5_13TeV-madgraphMLM-pythia8"
};

for(const auto& hs: histos){
  f->cd(Form("%s",hs.c_str()));
  for (auto i: *(gDirectory->GetListOfKeys())) {
    std::cout << Form("%s/%s;1",hs.c_str(),i->GetName()) << std::endl;
    gDirectory->Delete(Form("%s;1",i->GetName()));
  }
}

```

### Running Analysis Framework

```bash

## Unique EntryList
wget -P proof/EntryListMaker/ https://avargash.web.cern.ch/avargash/analysisFiles/EntryLists_Unique.root

### UL Samples

#### MC
##### Running using condor:
cd proof/
YEAR=2018;for i in `ls -1 files/mc/$YEAR/UL/*.txt |xargs -n1 basename`; do ./SubmitCondor.sh $YEAR MC $i; sleep 1; done
LABEL=Anv5Mixv2v9;for i in `ls *$LABEL*`; do root -l -b -q "copyFiles.C(\""$i"\")"; done

```

### Setup for Data

Turns out the different datasets have overlapping events, meaning the same event is found
in different root files and chaining them results in counting the same event multiple times
towards the analysis. To solve this problem, as a first step a `TEntryList` is created for each
dataset with the events passing the so called "Event selection" (i.e `HLTs` and `Flags`), a
`TTree` is also created containing information about the `*run` and `*event` which may used to
identify uniquely events. As a second step, a new set of `TEntryList` is created using this
information, by loading the identification of the events in an `unordered_set` we can go 
sequentially through the datasets and discard the insertion of repeated events in the 
new `TEntryList`. Check `proof/EntryListMaker/README.md` for further details on how to
create the required `EntryLists.root` file.

```bash
#### Ultra-Legacy Samples

```bash

# Submit Condor Jobs
BRANCHNAME=RestoreWZBins
for k in {2016..2018}; do
  for j in {data,mc}; do
    YEAR=$k;TYPEP=$j;for i in `ls -1 files/${TYPEP,,}/$YEAR/UL/*.txt |xargs -n1 basename`; do ./SubmitCondor.sh $BRANCHNAME $YEAR ${TYPEP^^} $OUTPUTLABEL $i; done 
  done
done;

# Delete Condor Output if errors occurred
BRANCHNAME=RestoreWZBins;OUTDIR=/uscms/home/avargash/store/WprimeSearchCondorOutput/;for i in `grep -l -E "(error|crash|already deleted)" *_$BRANCHNAME*`; do FILE=`grep -oE 'WprimeHistos.*root' <<< $i`; rm -vf $OUTDIR/$FILE; rm -vf $i;done

# Check which files are missing
OUTPUTLABEL=RestoreWZBins
for k in {2016..2018}; do
  for j in {data,mc}; do
    YEAR=$k;TYPEP=$j;for i in `ls -1 files/${TYPEP,,}/$YEAR/UL/*.txt |xargs -n1 basename`; do ./checkFiles.sh $YEAR ${TYPEP^^} $OUTPUTLABEL $i; done 
  done
done;
```

This will create `WprimeHistos.root` file which will contain all the histograms
created on `PreSelector.C` classified by the sample name as a root directory.


### Stack

```bash
root -l -b -q "Stack.C(\"WprimeHistos_all.root\")";
```

Where "_all" stands for MC + Data. It creates a file `WprimeStack_all.root` (or `_label`)
where all the stack plots are produced. Plots are also saved in `png` format in the
`plots/[year]` directory.


#### For reference

```cpp
// Copy Data histos to different root file
TFile* fileFrom = TFile::Open("WprimeHistos_ApplyMETPtPhiCorrectionv5.root");
TFile* fileTo = TFile::Open("WprimeHistos_AddSystCR.root","UPDATE");
std::vector<std::string> dirNames = {
   "2016/ULSinglePhoton",
   "2016/ULSingleElectron",
   "2016/ULSingleMuon",
   "2017/ULSinglePhoton",
   "2017/ULSingleElectron",
   "2017/ULSingleMuon",
   "2018/ULSingleMuon",
   "2018/ULEGamma"
}

for(const auto& dir: dirNames){
  fileTo->mkdir(dir.c_str());
  fileFrom->cd(dir.c_str());
  TObjLink* l = gDirectory->GetListOfKeys()->FirstLink();
  fileTo->cd(dir.c_str());
  while(l){
    std::cout << fileFrom->Get(Form("%s/%s",dir.c_str(),l->GetObject()->GetName()))->Write() << std::endl;
    l = l->Next();
  }
}
```

### Combine

```bash
# Fix names for DataCards
sed -i 's/t#bar{t}/TT/' *.txt
sed -i 's/Z#gamma/ZG/' *.txt
```

### Datacards location

```bash
scp DataCard*.txt      avargash@lxplus.cern.ch:/afs/cern.ch/user/a/avargash/eos/www/WprimeSearch/datacards/
scp CombineFile*.root  avargash@lxplus.cern.ch:/afs/cern.ch/user/a/avargash/eos/www/WprimeSearch/datacards/
```

### Combining Datacards



#### Compute Exclusion Limits

```bash
FLAG=Bin30BG100X
for MASS in 600 800 1000 1200 \
    1400 1600 1800 2000 2500 \
    3000 3500 4000 4500
do
    DCARDIR=/afs/cern.ch/user/a/avargash/eos/www/WprimeSearch/datacards/${FLAG}/
    Y16_eee=DataCard_eee_HMassWZ_SR1_2016_${MASS}_${FLAG}.txt
    Y17_eee=DataCard_eee_HMassWZ_SR1_2017_${MASS}_${FLAG}.txt
    Y18_eee=DataCard_eee_HMassWZ_SR1_2018_${MASS}_${FLAG}.txt
    Y16_eemu=DataCard_eemu_HMassWZ_SR1_2016_${MASS}_${FLAG}.txt
    Y17_eemu=DataCard_eemu_HMassWZ_SR1_2017_${MASS}_${FLAG}.txt
    Y18_eemu=DataCard_eemu_HMassWZ_SR1_2018_${MASS}_${FLAG}.txt
    Y16_mumue=DataCard_mumue_HMassWZ_SR1_2016_${MASS}_${FLAG}.txt
    Y17_mumue=DataCard_mumue_HMassWZ_SR1_2017_${MASS}_${FLAG}.txt
    Y18_mumue=DataCard_mumue_HMassWZ_SR1_2018_${MASS}_${FLAG}.txt
    Y16_mumumu=DataCard_mumumu_HMassWZ_SR1_2016_${MASS}_${FLAG}.txt
    Y17_mumumu=DataCard_mumumu_HMassWZ_SR1_2017_${MASS}_${FLAG}.txt
    Y18_mumumu=DataCard_mumumu_HMassWZ_SR1_2018_${MASS}_${FLAG}.txt
    combineCards.py Y16_eee=${DCARDIR}/${Y16_eee} Y17_eee=${DCARDIR}/${Y17_eee} Y18_eee=${DCARDIR}/${Y18_eee} \
        Y16_eemu=${DCARDIR}/${Y16_eemu} Y17_eemu=${DCARDIR}/${Y17_eemu} Y18_eemu=${DCARDIR}/${Y18_eemu} \
        Y16_mumue=${DCARDIR}/${Y16_mumue} Y17_mumue=${DCARDIR}/${Y17_mumue} Y18_mumue=${DCARDIR}/${Y18_mumue} \
        Y16_mumumu=${DCARDIR}/${Y16_mumumu} Y17_mumumu=${DCARDIR}/${Y17_mumumu} Y18_mumumu=${DCARDIR}/${Y18_mumumu} \
        > ${DCARDIR}/RunII_HMassWZ_SR1_${MASS}_${FLAG}.txt
    combine -m $MASS -n ".${MASS}_${FLAG}" -M AsymptoticLimits -d ${DCARDIR}/RunII_HMassWZ_SR1_${MASS}_${FLAG}.txt
done
```

# Combine datacards for CR2

```bash
FLAG=RollbackBins
for i in 600 800 1000 1200 \
    1400 1600 1800 2000 2500 \
    3000 3500 4000 4500
do
    DCARDIR=/afs/cern.ch/user/a/avargash/eos/www/WprimeSearch/datacards/${FLAG}/
    Y16_eee=DataCard_eee_HMassWZ_CR2_2016_${i}_${FLAG}.txt
    Y17_eee=DataCard_eee_HMassWZ_CR2_2017_${i}_${FLAG}.txt
    Y18_eee=DataCard_eee_HMassWZ_CR2_2018_${i}_${FLAG}.txt
    Y16_eemu=DataCard_eemu_HMassWZ_CR2_2016_${i}_${FLAG}.txt
    Y17_eemu=DataCard_eemu_HMassWZ_CR2_2017_${i}_${FLAG}.txt
    Y18_eemu=DataCard_eemu_HMassWZ_CR2_2018_${i}_${FLAG}.txt
    Y16_mumue=DataCard_mumue_HMassWZ_CR2_2016_${i}_${FLAG}.txt
    Y17_mumue=DataCard_mumue_HMassWZ_CR2_2017_${i}_${FLAG}.txt
    Y18_mumue=DataCard_mumue_HMassWZ_CR2_2018_${i}_${FLAG}.txt
    Y16_mumumu=DataCard_mumumu_HMassWZ_CR2_2016_${i}_${FLAG}.txt
    Y17_mumumu=DataCard_mumumu_HMassWZ_CR2_2017_${i}_${FLAG}.txt
    Y18_mumumu=DataCard_mumumu_HMassWZ_CR2_2018_${i}_${FLAG}.txt
    combineCards.py Y16_eee=${DCARDIR}/${Y16_eee} Y17_eee=${DCARDIR}/${Y17_eee} Y18_eee=${DCARDIR}/${Y18_eee} \
        Y16_eemu=${DCARDIR}/${Y16_eemu} Y17_eemu=${DCARDIR}/${Y17_eemu} Y18_eemu=${DCARDIR}/${Y18_eemu} \
        Y16_mumue=${DCARDIR}/${Y16_mumue} Y17_mumue=${DCARDIR}/${Y17_mumue} Y18_mumue=${DCARDIR}/${Y18_mumue} \
        Y16_mumumu=${DCARDIR}/${Y16_mumumu} Y17_mumumu=${DCARDIR}/${Y17_mumumu} Y18_mumumu=${DCARDIR}/${Y18_mumumu} \
        > ${DCARDIR}/RunII_HMassWZ_CR2_${i}_${FLAG}.txt
done
```


### Workspace from Datacards (per channel

```bash
FLAG=OnlyGT36
DCARDIR=/afs/cern.ch/user/a/avargash/eos/www/WprimeSearch/datacards/${FLAG}/
ALLMASSP="600 800 1000 \
  1200 1400 1600 1800 \
  2000 2500 3000 3500 \
  4000 4500"
for MASS in $ALLMASSP; do
  for CR in {CR1,CR2}; do
    for YEAR in {2016,2017,2018}; do
      for CH in {eee,eemu,mumue,mumumu}; do
        LABEL=${CH}_HMassWZ_${CR}_${YEAR}_${MASS}_${FLAG}
        DCARD=${DCARDIR}DataCard_${LABEL}
        file ${DCARD}.txt
        combineTool.py -M T2W -o ${DCARD}.root -i ${DCARD}.txt
        file ${DCARD}.root
      done
    done
  done
done
```


### Pulls Asimov Dataset

#### PrintPulls.C

```cpp
int PrintPulls(std::string label){

  const char* ext = "pdf";
  const float margin = 0.3;
  TFile* f1 = TFile::Open(Form("Plot%s.root",label.c_str()));
  TCanvas* nuisances = (TCanvas*)f1->Get("nuisances");
  nuisances->SetBottomMargin(margin);
  TH1F* h = (TH1F*)nuisances->GetPrimitive("prefit_nuisancs");
  h->SetTitle(Form("%s [%s]",h->GetTitle(),label.c_str()));
  //h->LabelsDeflate("X");
  h->LabelsOption("v");
  nuisances->Print(Form("Plot%s_nuisances.%s",label.c_str(),ext));


  TCanvas* post_fit_errs = (TCanvas*)f1->Get("post_fit_errs");
  h = (TH1F*)post_fit_errs->GetPrimitive("errors_b");
  post_fit_errs->SetBottomMargin(margin);
  h->SetTitle(Form("%s [%s]",h->GetTitle(),label.c_str()));
  h->LabelsDeflate("X");
  h->LabelsOption("v","X");
  post_fit_errs->Print(Form("Plot%s_post_fit_errs.%s",label.c_str(),ext));
  post_fit_errs->Print(Form("Plot%s_post_fit_errs.png",label.c_str()));
  return 0;
}
```

```bash
FLAG=RollbackBins
DCARDIR=/afs/cern.ch/user/a/avargash/eos/www/WprimeSearch/datacards/${FLAG}/
COMBINEFOLDER=/afs/cern.ch/user/a/avargash/eos/Combine/CMSSW_10_2_13/src/HiggsAnalysis/CombinedLimit/
ALLMASSP=600
for MASS in $ALLMASSP; do
  for CR in {CR1,CR2}; do
    for YEAR in {2016,2017,2018}; do
      for CH in {eee,eemu,mumue,mumumu}; do
        LABEL=${CH}_HMassWZ_${CR}_${YEAR}_${MASS}_${FLAG}
        DCARD=${DCARDIR}/DataCard_${LABEL}.root
        combineTool.py -M T2W -i ${DCARDIR}/DataCard_${LABEL}.txt -o ${DCARDIR}/DataCard_${LABEL}.root
        combineTool.py -M FitDiagnostics -t -1 --saveToys $DCARD -n Asimov.${LABEL} --verbose 4
        python $CMSSW_BASE/src/HiggsAnalysis/CombinedLimit/test/diffNuisances.py fitDiagnosticsAsimov.${LABEL}.root --sortBy=correlation -g PlotAsimov_${LABEL}.root
        root -l -b -q $COMBINEFOLDER/PrintPulls.C\(\"Asimov_${LABEL}\"\)
        convert -border 2 -density 200 -quality 100  PlotAsimov_${LABEL}_nuisances.pdf -trim PlotAsimov_${LABEL}_nuisances.png
        convert -border 2 -density 200 -quality 100  PlotAsimov_${LABEL}_post_fit_errs.pdf -trim PlotAsimov_${LABEL}_post_fit_errs.png
      done
      montage -geometry +0+0 -tile 2x2 \
      PlotAsimov*_${LABEL}_nuisances.png \
      PlotAsimovMerged_${LABEL}_nuisances.png
    done
  done
done



#FLAG=OnlyGT36
FLAG=PosGenWv3
#FLAG=1p10b1600
DCARDIR=/afs/cern.ch/user/a/avargash/eos/www/WprimeSearch/datacards/${FLAG}/
COMBINEFOLDER=/afs/cern.ch/user/a/avargash/eos/Combine/CMSSW_10_2_13/src/HiggsAnalysis/CombinedLimit/
ALLMASSP=600
#for MASS in $ALLMASSP; do
MASS=600
  #for CR in {CR1,CR2}; do
  CR=CR2
    #for YEAR in {2016,2017,2018}; do
    YEAR=2018
      #for CH in {eee,eemu,mumue,mumumu}; do
      CH=mumue
        LABEL=${CH}_HMassWZ_${CR}_${YEAR}_${MASS}_${FLAG}
        DCARD=${DCARDIR}/DataCard_${LABEL}.root
        #combineTool.py -M T2W -i ${DCARDIR}/DataCard_${LABEL}.txt -o ${DCARDIR}/DataCard_${LABEL}.root
        combineTool.py -M FitDiagnostics -t -1 --saveToys $DCARD -n Asimov.${LABEL} --verbose 4
        python $CMSSW_BASE/src/HiggsAnalysis/CombinedLimit/test/diffNuisances.py fitDiagnosticsAsimov.${LABEL}.root --sortBy=correlation -g PlotAsimov_${LABEL}.#root --all
        root -l -b -q $COMBINEFOLDER/PrintPulls.C\(\"Asimov_${LABEL}\"\)
        #convert -border 2 -density 200 -quality 100  PlotAsimov_${LABEL}_nuisances.pdf -trim PlotAsimov_${LABEL}_nuisances.png
        #convert -border 2 -density 200 -quality 100  PlotAsimov_${LABEL}_post_fit_errs.pdf -trim PlotAsimov_${LABEL}_post_fit_errs.png
      #done
      #montage -geometry +0+0 -tile 2x2 \
      #PlotAsimov*_${LABEL}_nuisances.png \
      #PlotAsimovMerged_${LABEL}_nuisances.png
    #done
  #done
#done
```

### Pulls

```bash
FLAG=RollbackBins
DCARDIR=/afs/cern.ch/user/a/avargash/eos/www/WprimeSearch/datacards/${FLAG}/
COMBINEFOLDER=/afs/cern.ch/user/a/avargash/eos/Combine/CMSSW_10_2_13/src/HiggsAnalysis/CombinedLimit/
ALLMASSP=600
for MASS in $ALLMASSP; do
#MASS=600
  for CR in {CR1,CR2}; do
  #CR=CR2
    for YEAR in {2016,2017,2018}; do
    #YEAR=2018
      for CH in {eee,eemu,mumue,mumumu}; do
      #CH=mumue
        LABEL=${CH}_HMassWZ_${CR}_${YEAR}_${MASS}_${FLAG}
        DCARD=${DCARDIR}/DataCard_${LABEL}.root
        file $DCARD
        combineTool.py -M FitDiagnostics $DCARD -n $LABEL
        python $CMSSW_BASE/src/HiggsAnalysis/CombinedLimit/test/diffNuisances.py fitDiagnostics${LABEL}.root --sortBy=correlation -g PlotPulls_${LABEL}.root --all
        root -l -b -q $COMBINEFOLDER/PrintPulls.C\(\"Pulls_${LABEL}\"\)
        convert -border 2 -density 200 -quality 100  PlotPulls_${LABEL}_nuisances.pdf -trim PlotPulls_${LABEL}_nuisances.png
        convert -border 2 -density 200 -quality 100  PlotPulls_${LABEL}_post_fit_errs.pdf -trim PlotPulls_${LABEL}_post_fit_errs.png
      done
      montage -geometry +0+0 -tile 2x2 \
      PlotPulls*_${LABEL}_nuisances.png \
      PlotPullsMerged_${LABEL}_nuisances.png
    done
  done
done
```

### Impacts
```bash
FLAG=RollbackBins
DCARDIR=/afs/cern.ch/user/a/avargash/eos/www/WprimeSearch/datacards/${FLAG}/
ALLMASSP=600
for i in $ALLMASSP; do
  for j in {CR1,CR2}; do
    for k in {2016,2017,2018}; do
      for l in {eee,eemu,mumue,mumumu}; do
        LABEL=${l}_HMassWZ_${j}_${k}_${i}_${FLAG}
        DCARD=${DCARDIR}/DataCard_${LABEL}.root
        RLIM=50
        combineTool.py -n ${j}_${k} -M Impacts -d $DCARD -m $i --rMin -${RLIM}. --rMax ${RLIM}. --doInitialFit --robustFit 1
        combineTool.py -n ${j}_${k} -M Impacts -d $DCARD -m $i --rMin -${RLIM}. --rMax ${RLIM}. --robustFit 1 --doFits --parallel 8
        combineTool.py -n ${j}_${k} -M Impacts -d $DCARD -m $i --rMin -${RLIM}. --rMax ${RLIM}. -o Impacts_${LABEL}.json
        plotImpacts.py --blind --per-page 50 --label-size 0.03 --cms-label ${LABEL} -i Impacts_${LABEL}.json -o ImpactsPlot_${LABEL}
        convert -border 2 -density 200 -quality 100  ImpactsPlot_${LABEL}.pdf -trim ImpactsPlot_${LABEL}.png 
      done
      echo "Creating montage for ${j}_${k}"
      montage -geometry +0+0 -tile 2x2 \
        ImpactsPlot_*_HMassWZ_${j}_${k}_${i}_${RLIM}.png \
        ImpactsPlotMerged_${j}_${k}_${i}_${RLIM}.png
      done
    done
  done
done
```

### Goodness of Fit

```bash
FLAG=RollbackBins
ALLMASSP=600
DCARDIR=/afs/cern.ch/user/a/avargash/eos/www/WprimeSearch/datacards/${FLAG}/
ALGO="saturated"

function processGOF {
  WSPACE=$1
  file $WSPACE
  LABEL=$2
  MASS=$3
  NTOYS=1000
  NCORES=10
  NTOYS=$((NTOYS / NCORES)) # 1000 toys 10 cores
  RSEED=12345
  echo -e "\n=======GOODNESS OF FIT==${LABEL}=======\n"
  echo "Processing GOF for ${WSPACE} ${LABEL} ${MASS}"
  ALGO="saturated"
  file $WSPACE
  combineTool.py -M GoodnessOfFit --algorithm $ALGO -m $MASS -d ${WSPACE} -n .${ALGO}.${LABEL} \
    --plots --cminDefaultMinimizerStrategy 0
  combineTool.py -M GoodnessOfFit --algorithm $ALGO -m $MASS \
    -d ${WSPACE} \
    -n .${ALGO}.toys.${LABEL} --saveToys --toysFreq \
    -t ${NTOYS} -s ${RSEED}:$((RSEED+NCORES-1)):1 --parallel $NCORES --verbose 0 \
    --cminDefaultMinimizerStrategy 0
  combineTool.py -M CollectGoodnessOfFit \
    --input higgsCombine.${ALGO}.${LABEL}.GoodnessOfFit.mH${MASS}.root \
    higgsCombine.saturated.toys.${LABEL}.GoodnessOfFit.mH${MASS}.*.root \
    -o saturated.${LABEL}.json
  python $CMSSW_BASE/src/CombineHarvester/CombineTools/scripts/plotGof.py \
    --x-title "Test statistic" --y-title "nToys" --title-left $LABEL\
    --bins 80 --statistic ${ALGO} --mass ${MASS}.0 \
    --output GoF_${LABEL} ${ALGO}.${LABEL}.json --range 0 80
}

for MASS in $ALLMASSP; do
  for CR in {CR1,CR2}; do
    for YEAR in {2016,2017,2018}; do
      for CH in {eee,eemu,mumue,mumumu}; do
        LABEL=${CH}_HMassWZ_${CR}_${YEAR}_${MASS}_${FLAG}
        combineTool.py -M T2W --channel-masks -o ${DCARDIR}DataCard_${LABEL}.root -i ${DCARDIR}DataCard_${LABEL}.txt
        processGOF ${DCARDIR}DataCard_${LABEL}.root ${LABEL} ${MASS}
      done   
      montage -geometry +1+1 -tile 2x2 \
        GoF_*_${LABEL}.png \
        GoFMerged_${LABEL}.png
    done
  done
done
```

### Signal Injection tests


#### Masked workspace

```bash
# Datacard store
FLAG=RollbackBins
for MASS in {600,1000,1400}; do
  #MASS=1400
  DCARDIR=/afs/cern.ch/user/a/avargash/eos/www/WprimeSearch/datacards/${FLAG}/
  PREVDIR=`pwd`
  cd ${DCARDIR}
  CR2=RunII_HMassWZ_CR2_${MASS}_${FLAG}
  file ${CR2}.txt
  SR1=RunII_HMassWZ_SR1_${MASS}_${FLAG}
  file ${SR1}.txt
  CFILE=CombinedCR2SR1_RunII_${MASS}_${FLAG}
  combineCards.py SR1_RunII_${MASS}=${SR1}.txt CR2_RunII_${MASS}=${CR2}.txt>${CFILE}.txt
  file ${CFILE}.txt
  cd $PREVDIR
  CFILE=${DCARDIR}/${CFILE}
  combineTool.py -M T2W --channel-masks -o ${CFILE}.root -i ${CFILE}.txt
  file ${CFILE}.root
  combineTool.py -d ${CFILE}.root -M FitDiagnostics --saveShapes --saveWithUncertainties \
      -n RunII_${MASS} --setParameters mask_SR1_RunII_${MASS}_Y16_eee=1,mask_SR1_RunII_${MASS}_Y17_eee=1,mask_SR1_RunII_${MASS}_Y18_eee=1,mask_SR1_RunII_${MASS}_Y16_eemu=1,mask_SR1_RunII_${MASS}_Y17_eemu=1,mask_SR1_RunII_${MASS}_Y18_eemu=1,mask_SR1_RunII_${MASS}_Y16_mumue=1,mask_SR1_RunII_${MASS}_Y17_mumue=1,mask_SR1_RunII_${MASS}_Y18_mumue=1,mask_SR1_RunII_${MASS}_Y16_mumumu=1,mask_SR1_RunII_${MASS}_Y17_mumumu=1,mask_SR1_RunII_${MASS}_Y18_mumumu=1
  file fitDiagnosticsRunII_${MASS}.root
done
```

#### M = 600

```bash
FLAG="RollbackBins"
DCARDIR=/afs/cern.ch/user/a/avargash/eos/www/WprimeSearch/datacards/${FLAG}/

declare -A RMASS
RMASS["600"]="0.0 0.0244706 0.0336914 0.0479267"
RMASS["1000"]="0.0 0.0723099 0.1015625 0.1440699"
RMASS["1400"]="0.0 0.1226263 0.1777343 0.2620407"
wget -O importPars.py https://raw.githubusercontent.com/lcorcodilos/2DAlphabet/bstar/importPars.py
for MASS in ${!RMASS[@]}; do
  echo ${MASS}
  for R in ${RMASS[${MASS}]}; do
    SR1=RunII_HMassWZ_SR1_${MASS}_${FLAG}
    python importPars.py ${DCARDIR}/${SR1}.txt fitDiagnosticsRunII_${MASS}.root
    combineTool.py  -M GenerateOnly -d morphedWorkspace.root \
        -n RunII_${MASS}_${R} -m ${MASS} --saveToys --toysFrequentist --bypassFrequentistFit \
        --expectSignal ${R} -t 1000 -s 1234 \
        --cminDefaultMinimizerStrategy 0
    combineTool.py  -M FitDiagnostics -d morphedWorkspace.root \
        -n RunII_${MASS}_${R} \
        --toysFile higgsCombineRunII_${MASS}_${R}.GenerateOnly.mH${MASS}.1234.root -t 1000 \
        --rMin -100 --rMax 100 --expectSignal ${R}
  done
done
```


### PostFit plots

```bash
FLAG=1p10b1600
DCARDIR=/afs/cern.ch/user/a/avargash/eos/www/WprimeSearch/datacards/${FLAG}/
ALLMASSP=600
for i in $ALLMASSP; do
  for j in {CR1,CR2}; do
    for k in {2016,2017,2018}; do
      for l in {eee,eemu,mumue,mumumu}; do
        LABEL=${l}_HMassWZ_${j}_${k}_${i}_${FLAG}
        DCARD=${DCARDIR}/DataCard_${LABEL}
        combine -M FitDiagnostics $DCARD.root -n ${LABEL} \
            --expectSignal 0 --minos=all \
            --cminDefaultMinimizerStrategy 0 --forceRecreateNLL
        PostFitShapesFromWorkspace -w ${DCARD}.root -d ${DCARD}.txt \
            -f fitDiagnostics${LABEL}.root:fit_b --postfit \
            --sampling --samples 300 -o PostFit_${LABEL}.root
      done
    done
  done
done

#### Locally

scp avargash@lxplus.cern.ch:/afs/cern.ch/user/a/avargash/eos/Combine/CMSSW_10_2_13/src/HiggsAnalysis/CombinedLimit/PostFit/PostFit*.root ./WprimeSearch/proof/plots/PostFit/

#### Run printPostfitPlots

root -l -b -q Stack.C\(\"WprimeHistos_1p10b1600.root\"\)
cd plots/PostFit/

ALLMASSP=600
for i in $ALLMASSP; do
    for j in {CR1,CR2}; do
        for k in {2016,2017,2018}; do
            for l in {eee,eemu,mumue,mumumu}; do
                LABEL=${l}_HMassWZ_${j}_${k}_${i}
                montage -geometry +0+0 -tile 2x1 \
                    prefit_${LABEL}.png \
                    postfit_${LABEL}.png \
                    FitMerged_${LABEL}.png
            done
            montage -geometry +0+0 -tile 2x2 \
            FitMerged_*_${j}_${k}_${i}.png \
            ${k}_${j}_${i}.png
        done
    done
done
```


### Limits

```bash
cd /eos/home-a/avargash/Combine/CMSSW_10_2_13/src/HiggsAnalysis/CombinedLimit/

for i $ALLMASSP
  for j in 0.025 0.16 0.5 0.84 0.975; do
     ./SubmitLimitJob.sh $i $j
  done
done

scp avargash@lxplus.cern.ch:/eos/home-a/avargash/Combine/CMSSW_10_2_13/src/HiggsAnalysis/CombinedLimit/higgsCombine*.root
```

### Comparing graphs for different set of cuts:

```cpp
TFile* f1 = TFile::Open("WprimeStack_TightId.root");
TFile* f2 = TFile::Open("WprimeStack_highPtId.root");

auto c1 = new TCanvas("c1","c1");

auto l = new TLegend(0.69,0.16,0.87,0.26);
l->SetLineWidth(0);

auto g1 = static_cast<TGraph*>(f1->Get("2016_TightId_GPunziS"));
g1->SetTitle("Muon TightId");
g1->SetLineColor(kBlue);
g1->SetMarkerStyle(20);
g1->SetMarkerColor(kBlack);
g1->SetLineWidth(2);
l->AddEntry(g1);

auto g2 = static_cast<TGraph*>(f2->Get("2016_highPtId_GPunziS"));
g2->SetTitle("Muon highPtId");
g2->SetLineColor(kRed);
g2->SetMarkerStyle(20);
g2->SetMarkerColor(kBlack);
g2->SetLineWidth(2);
l->AddEntry(g2);

auto mg = new TMultiGraph();
mg->SetTitle("Punzi Significance ; M(WZ); #frac{#epsilon}{1+#sqrt{B}}");
mg->Add(g1)
mg->Add(g2)

mg->Draw("APL");
c1->SetGridx();
c1->SetGridy();
c1->SetTickx();
c1->SetTicky();
l->Draw();

c1->Print("2016_PunziTest.png")
```

### Efficiency graphs

```c++
std::vector<int> wm = { 800, 1000, 1200, 1400, 1600, 1800, 2000, 2500, 3000, 3500, 4000 };

std::vector<std::pair<std::string,std::string>> hname= {
 {"HDistl1l2_SR","dR(l1,l2) (cm)"},
 {"HLt_SR","#SigmaPt(l)"},
 {"HPtl1_SR","Pt(l1)"},
 {"HPtl2_SR","Pt(l2)"},
 {"HPtl3_SR","Pt(l3)"},
 {"HEtal1_SR","Eta(l1)"},
 {"HEtal2_SR","Eta(l2)"},
 {"HEtal3_SR","Eta(l3)"}
};

auto GetHisto = [&](const std::string filename, const std::string hs) {
 auto f = TFile::Open(filename.c_str());
 // Take 600 as a base and add the rest on top of it
 auto h = static_cast<TH1F*>(f->Get(
  Form("2016/WprimeToWZToWlepZlep_narrow_M-600_13TeV-madgraph/%s_C",hs.c_str())
 )->Clone());
 h->Add(static_cast<TH1D*>(f->Get(
  Form("2016/WprimeToWZToWlepZlep_narrow_M-600_13TeV-madgraph/%s_D",hs.c_str())
 )));
 for(const int& m: wm){
  h->Add(static_cast<TH1D*>(f->Get(Form("2016/WprimeToWZToWlepZlep_narrow_M-%d_13TeV-madgraph/%s_C",m,hs.c_str()))));
  h->Add(static_cast<TH1D*>(f->Get(Form("2016/WprimeToWZToWlepZlep_narrow_M-%d_13TeV-madgraph/%s_D",m,hs.c_str()))));
 }
 return h;
};

for(auto hs: hname){

 TH1F* htotal = GetHisto("WprimeHistos_NoMuonID.root",hs.first);

 TH1F* hpass1 = GetHisto("WprimeHistos_GlobalPF.root",hs.first);
 TGraphAsymmErrors *g1 = new TGraphAsymmErrors(hpass1,htotal);
 g1->SetTitle("GlobalMuon is PF;dR;Efficiency");
 g1->SetLineColor(kGreen);

 TH1F* hpass2 = GetHisto("WprimeHistos_NoMuonPF.root",hs.first);
 TGraphAsymmErrors *g2 = new TGraphAsymmErrors(hpass2,htotal);
 g2->SetTitle("No PF Requirement");
 g2->SetLineColor(kBlack);

 TH1F* hpass3 = GetHisto("WprimeHistos_EitherPF.root",hs.first);
 TGraphAsymmErrors *g3 = new TGraphAsymmErrors(hpass3,htotal);
 g3->SetTitle("Either Muon is PF;dR;Efficiency");
 g3->SetLineColor(kRed);


 TMultiGraph* mg = new TMultiGraph();
 std::vector<TGraph*> gs = { g2,g1,g3 };
 for(auto& g: gs){
   g->SetLineWidth(2);
   mg->Add(g,"P");
 }

 auto c1 = new TCanvas("c1","c1",1200,1200);
 mg->SetTitle(Form("Efficiency; %s Z#rightarrow#mu#mu; Efficiency;",hs.second.c_str()));
 mg->GetYaxis()->SetRangeUser(0.,1.05);
 mg->Draw("A");
 mg->GetYaxis()->SetRangeUser(0.8,1.01);
 gPad->BuildLegend();
 c1->Print(Form("HighPtPF_%s.png",hs.second.c_str()));
}
```

#### Signature Examples

```bash
# 1000     Run: 319579     Event: 4487890911       Lumiblock: 2957 mll: 86.8262
xrdcp -v root://cmsxrootd.fnal.gov//store/data/Run2018C/EGamma/MINIAOD/12Nov2019_UL2018-v2/100000/AFCAE2E8-4264-0D4F-A80C-516B47323CE3.root ./A_319579_2957_4487890911.root
# 0100     Run: 325000     Event: 346288517        Lumiblock: 188  mll: 91.8193
xrdcp -v root://cmsxrootd.fnal.gov//store/data/Run2018D/EGamma/MINIAOD/12Nov2019_UL2018-v8/280002/BF1FA64B-89BB-F14B-8DF6-E2CFB1CAE6EC.root ./B_325000_188_346288517.root
# 0010     Run: 321295     Event: 873839127        Lumiblock: 553  mll: 91.1356
xrdcp -v root://cmsxrootd.fnal.gov//store/data/Run2018D/SingleMuon/MINIAOD/12Nov2019_UL2018-v8/270001/F9F6EB22-345F-9C49-88BF-3D9B7F442CDD.root ./C_321295_553_873839127.root
# 0001     Run: 320024     Event: 355149645        Lumiblock: 211  mll: 91.4401
xrdcp -v root://cmsxrootd.fnal.gov//store/data/Run2018C/SingleMuon/MINIAOD/12Nov2019_UL2018-v3/110000/ECDCC359-CB8D-6A44-97EC-2A04895DF1F3.root ./D_320024_211_355149645.root
# 0010     Run: 323790     Event: 646065569        Lumiblock: 376  mll: 91.6585
xrdcp -v root://cmsxrootd.fnal.gov//store/data/Run2018D/SingleMuon/MINIAOD/12Nov2019_UL2018-v8/270002/136E0085-023C-B34F-8501-1805588399B0.root ./C_323790_376_646065569.root
```

### References:

* [Analysis update B2G Diboson meeting Feb/11th/2022](https://indico.cern.ch/event/1127839/contributions/4733861/attachments/2389961/4085434/WprimeUpdate%20Feb%2011th.pdf)
* [Analysis update B2G Diboson meeting Sep/24th/2021](https://indico.cern.ch/event/1079124/contributions/4539114/attachments/2315745/3942026/WprimeUpdate%20Sept%2024th.pdf)
* [Muon POG Momentum Resolution update Jul/26th/2021](https://indico.cern.ch/event/1061707/contributions/4464423/attachments/2287342/3887855/MomentumResolution%20Update.pdf)
* [Analysis update B2G Diboson meeting Jun/18th/2021](https://indico.cern.ch/event/1049401/contributions/4409124/attachments/2267179/3849537/Wprime%20Update%20June%202021.pdf)
* [Analysis update B2G Diboson meeting Apr/12th/2021](https://indico.cern.ch/event/1017483/contributions/4304551/attachments/2224394/3767255/Wprime%20Update%20April%202021.pdf)
* [Analysis update B2G Diboson meeting Jan/15th/2021](https://indico.cern.ch/event/993670/contributions/4181199/attachments/2172439/3667916/Wprime%20DIB%20Jan%202021%20%281%29.pdf)
* [Analysis update B2G Diboson meeting Oct/30th/2020](https://indico.cern.ch/event/970049/contributions/4083907/attachments/2134087/3594291/Wprime%20DIB%20Update%20Oct%202020.pdf)
* [Analysis update B2G Diboson meeting Sep/18th/2020](https://indico.cern.ch/event/952830/contributions/4003368/attachments/2105217/3540240/Wprime_DIB_Meeting_Sept2020.pdf)
* [Analysis update B2G Workshop May/18/2020](https://indico.cern.ch/event/891751/timetable/)
* [Analysis update B2G Diboson meeting Feb/7/2020](https://indico.cern.ch/event/886464/)
* [8 TeV Search - EXO-12-025](http://cms.cern.ch/iCMS/analysisadmin/cadilines?line=EXO-12-025)
* [8 TeV Search paper](https://arxiv.org/pdf/1407.3476.pdf)
* [7 TeV Search - EXO-11-041](http://cms.cern.ch/iCMS/analysisadmin/cadilines?line=EXO-11-041)
* [7 TeV Search paper](https://arxiv.org/pdf/1206.0433.pdf)