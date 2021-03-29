
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

For running with UL, replace:

```
-#include "BuildGoldenJson.hxx"
+#include "BuildGoldenJsonUL.hxx"
```

Rename also ROOT File for output to include UL label and then:

```bash
echo -e "#define CMSDATA\n#define Y2016">../IsData.h
FILES=../files/data/2016/UL/*.txt
for i in $FILES
do
 root -l -b -q "MakeEventIDTree.C(\"$i\",4)"
done

echo -e "#define CMSDATA\n#define Y2017">../IsData.h
FILES=../files/data/2017/UL/*.txt
for i in $FILES
do
 root -l -b -q "MakeEventIDTree.C(\"$i\",4)"
done

echo -e "#define CMSDATA\n#define Y2018">../IsData.h
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

## 2017
echo -e "#define CMSDATA\n#define Y2017">../IsData.h
root -l -b -q "MakeUniqueEntryList.C(\"../files/data/2017/SingleElectron.txt\",4)"
root -l -b -q "MakeUniqueEntryList.C(\"../files/data/2017/SingleMuon.txt\",2)"

## 2018
# We take all the events from SingleMuon and filter out duplicated events in EGamma
echo -e "#define CMSDATA\n#define Y2018">../IsData.h
root -l -b -q "MakeUniqueEntryList.C(\"../files/data/2018/EGamma.txt\",2)"
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

// 2016
auto t1 = (TEntryList*)f0.Get("SinglePhoton_2016/EntryList;1");
auto t2 = (TEntryList*)f1.Get("SingleElectron_2016/EntryList;1");
auto t3 = (TEntryList*)f1.Get("SingleMuon_2016/EntryList;1");
TEntryList *t4 = new TEntryList("EntryList","SingleElectron+SingleMuon+SinglePhoton");
t4->Add(t1);
t4->Add(t2);
t4->Add(t3);
f1.mkdir("SinglePhotonSingleElectronSingleMuon_2016");
f1.cd("SinglePhotonSingleElectronSingleMuon_2016");
t4->Write();
f1.Close();
f0.Close();

// 2017
auto t1 = (TEntryList*)f0.Get("SinglePhoton_2017/EntryList;1");
auto t2 = (TEntryList*)f1.Get("SingleElectron_2017/EntryList;1");
auto t3 = (TEntryList*)f1.Get("SingleMuon_2017/EntryList;1");
TEntryList *t4 = new TEntryList("EntryList","SingleElectron+SingleMuon+SinglePhoton");
t4->Add(t1);
t4->Add(t2);
t4->Add(t3);
f1.mkdir("SinglePhotonSingleElectronSingleMuon_2017");
f1.cd("SinglePhotonSingleElectronSingleMuon_2017");
t4->Write();
f1.Close();
f0.Close();

//2018
auto t1 = (TEntryList*)f0.Get("SingleMuon_2018/EntryList;1");
auto t2 = (TEntryList*)f1.Get("EGamma_2018/EntryList;1");
TEntryList *t4 = new TEntryList("EntryList","SingleMuon+EGamma");
t4->Add(t1);
t4->Add(t2);
f1.mkdir("SingleMuonEGamma_2018");
f1.cd("SingleMuonEGamma_2018");
t4->Write();
```

