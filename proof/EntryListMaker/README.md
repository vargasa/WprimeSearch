
### Build 1st TEntryList and TTrees

```bash
FILES=../files/2016/data/*.txt
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
root -l -b -q "MakeUniqueEntryList.C(\"../files/2016/data/SingleElectron.txt\",4)"
root -l -b -q "MakeUniqueEntryList.C(\"../files/2016/data/SingleMuon.txt\",2)"
```

Creating one file `EntryLists_Unique.root` containing `TEntryLists` for each dataset
with one unique entry per event. 

We reduce the number of jobs as the index keeping track of the events already included
grows in size.

It is possible to concatenate the list:

```cpp
TFile f0("EntryLists.root","READ");
auto t1 = (TEntryList*)f0.Get("SinglePhoton_2016/EntryList;1");
TFile f1("EntryLists_Unique.root","UPDATE");
auto t2 = (TEntryList*)f1.Get("SingleElectron/EntryList;1");
auto t3 = (TEntryList*)f1.Get("SingleMuon/EntryList;1");
TEntryList *t4 = new TEntryList("EntryList","SingleElectron+SingleMuon+SinglePhoton");
t4->Add(t1);
t4->Add(t2);
t4->Add(t3);
f1.mkdir("SinglePhotonSingleElectronSingleMuon");
f1.cd("SinglePhotonSingleElectronSingleMuon");
t4->Write();
f1.Close();
f0.Close();
```

