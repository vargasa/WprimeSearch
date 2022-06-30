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

scp DataCard*.txt avargash@lxplus.cern.ch:/eos/home-a/avargash/Combine/CMSSW_10_2_13/src/HiggsAnalysis/CombinedLimit/datacards/

scp CombineFile*.root avargash@lxplus.cern.ch:/eos/home-a/avargash/Combine/CMSSW_10_2_13/src/HiggsAnalysis/CombinedLimit/datacards/

cd /eos/home-a/avargash/Combine/CMSSW_10_2_13/src/HiggsAnalysis/CombinedLimit/

for i in 600 800 1000 \
  1200 1400 1600 1800 2000 \
  2500 3000 3500 4000 4500; do
  for j in 0.025 0.16 0.5 0.84 0.975; do
     ./SubmitLimitJob.sh $i $j
  done
done

for i in 600 800 1000 1200 \
  1400 1600 1800 2000 2500 \
  3000 3500 4000
do
  combineCards.py Y16="datacards/2016_"$i"_DataCard.txt" Y17="datacards/2017_"$i"_DataCard.txt" Y18="datacards/2018_"$i"_DataCard.txt">"datacards/RunII_"$i"_Datacard.txt"
  combine  -m 125 -n "."$i -M AsymptoticLimits -d "datacards/RunII_"$i"_Datacard.txt"
  text2workspace.py "datacards/RunII_"$i"_Datacard.txt"
  combineTool.py -M Impacts -d "datacards/RunII_"$i"_Datacard.root" -m $i --doInitialFit --robustFit 1
  combineTool.py -M Impacts -d "datacards/RunII_"$i"_Datacard.root" -m $i --robustFit 1 --doFits --parallel 4
  combineTool.py -M Impacts -d "datacards/RunII_"$i"_Datacard.root" -m $i -o "impacts_"$i".json"
  plotImpacts.py -i "impacts_"$i".json" -o "ImpactsPlot"$i".json"
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