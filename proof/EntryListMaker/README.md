
### Build 1st TEntryList and TTrees

```bash
echo -e "#define CMSDATA\n#define Y2016">../IsData.h
FILES=../files/data/2016/*.txt
for i in $FILES
do
 root -l -b -q "MakeEventIDTree.C(\"$i\",4)"
done

echo -e "#define CMSDATA\n#define Y2017">../IsData.h
FILES=../files/data/2017/*.txt
for i in $FILES
do
 root -l -b -q "MakeEventIDTree.C(\"$i\",4)"
done

echo -e "#define CMSDATA\n#define Y2018">../IsData.h
FILES=../files/data/2018/*.txt
for i in $FILES
do
 root -l -b -q "MakeEventIDTree.C(\"$i\",4)"
done
```

For running with UL:

```bash
echo -e "#define CMSDATA\n#define Y2016\n#define ULSAMPLE">../IsData.h
FILES=../files/data/2016/UL/*.txt
for i in $FILES
do
 root -l -b -q "MakeEventIDTree.C(\"$i\",4)"
done

echo -e "#define CMSDATA\n#define Y2017\n#define ULSAMPLE">../IsData.h
FILES=../files/data/2017/UL/*.txt
for i in $FILES
do
 root -l -b -q "MakeEventIDTree.C(\"$i\",4)"
done

echo -e "#define CMSDATA\n#define Y2018#define ULSAMPLE">../IsData.h
FILES=../files/data/2018/UL/*.txt
for i in $FILES
do
 root -l -b -q "MakeEventIDTree.C(\"$i\",4)"
done
```


This will create two ROOT files `EventIDTree.root` and `EntryLists.root`, the former
contains a `TTree` per dataset (separated in a different `TDirectory`) with all the 
events id (`str(run)+str(event)`) and the later contains `TEntryList`s of the different
events in each dataset passing the basic event selection, these lists may contain 
repeated events so a second step to build a curated `TEntryList` containing unique 
entries for a collection of datasets is necessary.

### Build unique TEntryList

We will take `SinglePhoton` as a base dataset, meaning we will take all the events passing
the basic selection on this dataset, then we compare it to `SingleElectron` and discard all
the repeated events in it and finally take `SingleMuon` and discard all the events
found already in the previous two datasets. This order is based on the number of events
in each dataset as we need to make sure the index object fits in memory.

```bash
## 2016
echo -e "#define CMSDATA\n#define Y2016">../IsData.h
root -l -b -q "MakeUniqueEntryList.C(\"../files/data/2016/SingleElectron.txt\",4)"
root -l -b -q "MakeUniqueEntryList.C(\"../files/data/2016/SingleMuon.txt\",4)"

## UL2016
echo -e "#define CMSDATA\n#define Y2016\n#define ULSAMPLE">../IsData.h
root -l -b -q "MakeUniqueEntryList.C(\"../files/data/2016/UL/ULSingleElectron.txt\",4)"
root -l -b -q "MakeUniqueEntryList.C(\"../files/data/2016/UL/ULSingleMuon.txt\",4)"

## 2017
echo -e "#define CMSDATA\n#define Y2017">../IsData.h
root -l -b -q "MakeUniqueEntryList.C(\"../files/data/2017/SingleElectron.txt\",4)"
root -l -b -q "MakeUniqueEntryList.C(\"../files/data/2017/SingleMuon.txt\",2)"

## UL2017
echo -e "#define CMSDATA\n#define Y2017\n#define ULSAMPLE">../IsData.h
root -l -b -q "MakeUniqueEntryList.C(\"../files/data/2017/UL/SingleElectron.txt\",4)"
root -l -b -q "MakeUniqueEntryList.C(\"../files/data/2017/UL/SingleMuon.txt\",2)"

## 2018
# We take all the events from SingleMuon and filter out duplicated events in EGamma
echo -e "#define CMSDATA\n#define Y2018">../IsData.h
root -l -b -q "MakeUniqueEntryList.C(\"../files/data/2018/EGamma.txt\",2)"

## UL2018
# We take all the events from SingleMuon and filter out duplicated events in EGamma
echo -e "#define CMSDATA\n#define Y2018\n#define ULSAMPLE">../IsData.h
root -l -b -q "MakeUniqueEntryList.C(\"../files/data/2018/UL/ULEGamma.txt\",2)"

```

Creating one file `EntryLists_Unique.root` containing `TEntryLists` for each dataset
with one unique entry per event.

We reduce the number of jobs as the index keeping track of the events already included
grows in size.

It is possible to concatenate the list to get a single `TEntryList` that is used
when processing data:

```cpp
TFile f0("EntryLists.root","READ");
TFile f1("EntryLists_Unique.root","UPDATE");

const char* PhotonSample = "SinglePhoton";
const char* ElectronSample = "SingleElectron";
const char* MuonSample = "SingleMuon";
const char* EGammaSample = "EGamma";

const char* PhotonSample = "ULSinglePhoton";
const char* ElectronSample = "ULSingleElectron";
const char* MuonSample = "ULSingleMuon";
const char* EGammaSample = "ULEGamma";

// 2016
auto t1 = (TEntryList*)f0.Get(Form("%s_2016/EntryList;1",PhotonSample));
auto t2 = (TEntryList*)f1.Get(Form("%s_2016/EntryList;1",ElectronSample));
auto t3 = (TEntryList*)f1.Get(Form("%s_2016/EntryList;1",MuonSample));
TEntryList *t4 = new TEntryList("EntryList",Form("%s+%s+%s",PhotonSample,ElectronSample,MuonSample));
t4->Add(t1);
t4->Add(t2);
t4->Add(t3);
f1.mkdir(Form("%s%s%s_2016",PhotonSample,ElectronSample,MuonSample));
f1.cd(Form("%s%s%s_2016",PhotonSample,ElectronSample,MuonSample));
t4->Write();
f1.Close();
f0.Close();

// 2017
auto t1 = (TEntryList*)f0.Get(Form("%s_2017/EntryList;1",PhotonSample));
auto t2 = (TEntryList*)f1.Get(Form("%s_2017/EntryList;1",ElectronSample));
auto t3 = (TEntryList*)f1.Get(Form("%s_2017/EntryList;1",MuonSample));
TEntryList *t4 = new TEntryList("EntryList",Form("%s+%s+%s",PhotonSample,ElectronSample,MuonSample));
t4->Add(t1);
t4->Add(t2);
t4->Add(t3);
f1.mkdir(Form("%s%s%s_2017",PhotonSample,ElectronSample,MuonSample));
f1.cd(Form("%s%s%s_2017",PhotonSample,ElectronSample,MuonSample));
t4->Write();
f1.Close();
f0.Close();

//2018
auto t1 = (TEntryList*)f0.Get(Form("%s_2018/EntryList;1",MuonSample));
auto t2 = (TEntryList*)f1.Get(Form("%s_2018/EntryList;1",EGammeSample);
TEntryList *t4 = new TEntryList("EntryList",Form("%s+%s",MuonSample,EGammaSample));
t4->Add(t1);
t4->Add(t2);
f1.mkdir(Form("%s%s_2018",MuonSample,EGammaSample));
f1.cd(Form("%s%s_2018",MuonSample,EGammaSample));
t4->Write();
```

