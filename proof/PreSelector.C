#include "TFile.h"
#include "PreSelector.h"

PreSelector::PreSelector(TTree *)
{
  HMuon_ptA=0;
  HElectron_ptA=0;

  HMuon_ptB=0;
  HElectron_ptB=0;

  HMuon_ptC=0;
  HElectron_ptC=0;

  HMuon_ptD=0;
  HElectron_ptD=0;

  HMetZA=0;
  HMetZB=0;
  HMetZC=0;
  HMetZD=0;

}

void PreSelector::Init(TTree *tree)
{
  //Called every time a new TTree is attached.
  fReader.SetTree(tree);

}
void PreSelector::Begin(TTree *tree) {

}

void PreSelector::SlaveBegin(TTree *tree) {

  HMuon_ptA = new TH1F("HMuon_ptA","",250,0,1200);
  HElectron_ptA = new TH1F("HElectron_ptA","",250,0,1200);

  HMuon_ptB = new TH1F("HMuon_ptB","",250,0,1200);
  HElectron_ptB = new TH1F("HElectron_ptB","",250,0,1200);

  HMuon_ptC = new TH1F("HMuon_ptC","",250,0,1200);
  HElectron_ptC = new TH1F("HElectron_ptC","",250,0,1200);

  HMuon_ptD = new TH1F("HMuon_ptD","",250,0,1200);
  HElectron_ptD = new TH1F("HElectron_ptD","",250,0,1200);

  HMetZA = new TH1F("HMetZA","",50,0,500);
  HMetZB = new TH1F("HMetZB","",50,0,500);
  HMetZC = new TH1F("HMetZC","",50,0,500);
  HMetZD = new TH1F("HMetZD","",50,0,500);

  fOutput->Add(HMuon_ptA);
  fOutput->Add(HElectron_ptA);

  fOutput->Add(HMuon_ptB);
  fOutput->Add(HElectron_ptB);

  fOutput->Add(HMuon_ptC);
  fOutput->Add(HElectron_ptC);

  fOutput->Add(HMuon_ptD);
  fOutput->Add(HElectron_ptD);

  fOutput->Add(HMetZA);
  fOutput->Add(HMetZB);
  fOutput->Add(HMetZC);
  fOutput->Add(HMetZD);

}




Bool_t PreSelector::Process(Long64_t entry) {

   fReader.SetEntry(entry);

   struct Muons{
     TTreeReaderValue<UInt_t> &n;
     TTreeReaderArray<Bool_t> &looseId;
     TTreeReaderArray<Float_t> &pt;
     TTreeReaderArray<Float_t> &eta;
     TTreeReaderArray<Float_t> &phi;
   };

   struct Electrons{
     TTreeReaderValue<UInt_t> &n;
     TTreeReaderArray<Int_t> &cutBased;
     TTreeReaderArray<Float_t> &pt;
     TTreeReaderArray<Float_t> &eta;
     TTreeReaderArray<Float_t> &phi;
   };

   // Event Selection
   if ( *HLT_DoubleEle33_CaloIdL_MW &&
        *Flag_HBHENoiseFilter &&
        *Flag_HBHENoiseIsoFilter &&
        *Flag_EcalDeadCellTriggerPrimitiveFilter &&
        *Flag_globalTightHalo2016Filter &&
        *Flag_BadPFMuonSummer16Filter &&
        *PV_npvsGood > 1 &&
        *PV_ndof > 4
        ) {

     Muons Mus{nMuon,Muon_looseId,
         Muon_pt,Muon_eta,Muon_phi};

     Electrons Els{nElectron,Electron_cutBased,
         Electron_pt,Electron_eta,Electron_phi};

     auto GetLooseMuon = [](Muons Mu){
       std::vector<UInt_t> LooseIndex = {};
       UInt_t index = 0;
       for (const auto loose: Mu.looseId){
         if(loose) LooseIndex.emplace_back(index);
         index++;
       }
       return LooseIndex;
     };

     auto GetLooseElectron = [](Electrons El){
       std::vector<UInt_t> LooseIndex = {};
       UInt_t index = 0;
       for (const auto id: El.cutBased){
         if(id>=1) LooseIndex.emplace_back(index);
         index++;
       }
       return LooseIndex;
     };

     std::vector<UInt_t> LooseElectron = GetLooseElectron(Els);
     std::vector<UInt_t> LooseMuon = GetLooseMuon(Mus);

     // 0e3Mu
     if(*nMuon>=3 && LooseMuon.size()>=3){
       HMetZA->Fill(*MET_pt);
     }

     // 1e2Mu
     if(*nElectron!=0 && *nMuon>=2 &&
        LooseElectron.size()>=1 &&
        LooseMuon.size()>=2){
       HMetZB->Fill(*MET_pt);
     }

     // 2e1Mu
     if(*nElectron>=2 && *nMuon>=1 &&
        LooseElectron.size()>=2 &&
        LooseMuon.size()>=1){
       HMetZC->Fill(*MET_pt);
     }

     // 3e0Mu
     if(*nElectron>=3 && LooseElectron.size()>=3){
       HMetZD->Fill(*MET_pt);
     }

     // 3leptons

     if( (*nElectron + *nMuon) == 3){

     }

   }
   return kTRUE;
}

void PreSelector::Terminate() {

  ch = new TCanvas("ch","ch",1200,600);
  ch->Divide(2,2);

  ch->cd(1);
  HMetZA->SetTitle("0e3Mu;MET_pt;Event count");
  HMetZA->Draw();
  ch->cd(2);
  HMetZB->SetTitle("1e2Mu;MET_pt;Event count");
  HMetZB->Draw();
  ch->cd(3);
  HMetZC->SetTitle("2e1Mu;MET_pt;Event count");
  HMetZC->Draw();
  ch->cd(4);
  HMetZD->SetTitle("3e0Mu;MET_pt;Event count");
  HMetZD->Draw();
  ch->Print("PlotMet.png");
}
