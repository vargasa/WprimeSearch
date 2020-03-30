import json
from ROOT import TFile, TTree
from array import array

f1 = TFile("GoldenJson.root", "RECREATE");

goldenFile = "Cert_271036-284044_13TeV_ReReco_07Aug2017_Collisions16_JSON.txt"

with open(goldenFile) as f:
  data = json.load(f)

t1 = TTree("goldenTree","from Golden JSON");

nRun = array( 'i', [ 0 ] )
nLumiStart = array( 'i', [ 0 ] )
nLumiEnd = array( 'i', [ 0 ] )

t1.Branch("run", nRun,"run/I")
t1.Branch("lumiStart", nLumiStart, "lumiStart/I")
t1.Branch("lumiEnd", nLumiEnd, "lumiEnd/I")

for run in data:
  for lumiBlock in data[run]:
    nRun[0] = int(run)
    nLumiStart[0] = int(lumiBlock[0])
    nLumiEnd[0] = int(lumiBlock[1])
    t1.Fill()

t1.Write()
f1.Close()
