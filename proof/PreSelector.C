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

  HMetA=0;
  HMetB=0;
  HMetC=0;
  HMetD=0;

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

  HMetA = new TH1F("HMetA","",50,0,500);
  HMetB = new TH1F("HMetB","",50,0,500);
  HMetC = new TH1F("HMetC","",50,0,500);
  HMetD = new TH1F("HMetD","",50,0,500);

  fOutput->Add(HMuon_ptA);
  fOutput->Add(HElectron_ptA);

  fOutput->Add(HMuon_ptB);
  fOutput->Add(HElectron_ptB);

  fOutput->Add(HMuon_ptC);
  fOutput->Add(HElectron_ptC);

  fOutput->Add(HMuon_ptD);
  fOutput->Add(HElectron_ptD);

  fOutput->Add(HMetA);
  fOutput->Add(HMetB);
  fOutput->Add(HMetC);
  fOutput->Add(HMetD);

}




Bool_t PreSelector::Process(Long64_t entry) {

   fReader.SetEntry(entry);

   struct Muons{
     TTreeReaderValue<UInt_t> &n;
     TTreeReaderArray<Bool_t> &looseId;
     TTreeReaderArray<Float_t> &pt;
     TTreeReaderArray<Float_t> &eta;
     TTreeReaderArray<Float_t> &phi;
     TTreeReaderArray<Bool_t> &isGlobal;
     TTreeReaderArray<Float_t> &dxy;
     TTreeReaderArray<Float_t> &dz;
   };

   struct Electrons{
     TTreeReaderValue<UInt_t> &n;
     TTreeReaderArray<Int_t> &cutBased;
     TTreeReaderArray<Float_t> &pt;
     TTreeReaderArray<Float_t> &eta;
     TTreeReaderArray<Float_t> &phi;
     TTreeReaderArray<Float_t> &miniPFRelIso_all;
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
         Muon_pt,Muon_eta,Muon_phi,
         Muon_isGlobal,Muon_dxy,Muon_dz};

     Electrons Els{nElectron,Electron_cutBased,
         Electron_pt,Electron_eta,Electron_phi,
         Electron_miniPFRelIso_all};

     auto GetGoodMuon = [](Muons Mu){
       std::vector<UInt_t> GoodIndex = {};
       const Float_t MaxEta = 2.4;
       const Float_t MinPt = 10.;
       const Float_t MaxDxy = 0.2; // 2mm
       const Float_t MaxDz = 0.5; // 5mm
       UInt_t index = 0;
       for (UInt_t i=0; i<*Mu.n;i++){
         if(Mu.isGlobal[i] && Mu.looseId[i] && Mu.eta[i]<MaxEta &&
            Mu.pt[i]>MinPt && Mu.dxy[i]<MaxDxy && Mu.dz[i]<MaxDz)
           GoodIndex.emplace_back(index);
         index++;
       }
       return GoodIndex;
     };

     auto GetGoodElectron = [](Electrons El){
       const Float_t MaxEta = 2.5;
       const Float_t MinPt = 20.;
       const Float_t MaxRelIso = 0.15;
       std::vector<UInt_t> GoodIndex = {};
       UInt_t index = 0;
       for (UInt_t i = 0; i< *El.n; i++){
         if(El.cutBased[i]>=1 && El.eta[i]<MaxEta && El.pt[i]>MinPt &&
            El.miniPFRelIso_all[i]<MaxRelIso)
           GoodIndex.emplace_back(i);
       }
       return GoodIndex;
     };

     std::vector<UInt_t> GoodElectron = GetGoodElectron(Els);
     std::vector<UInt_t> GoodMuon = GetGoodMuon(Mus);

     // 0e3Mu
     if(*nMuon>=3 && GoodMuon.size()>=3){
       HMetA->Fill(*MET_pt);
     }

     // 1e2Mu
     if(*nElectron!=0 && *nMuon>=2 &&
        GoodElectron.size()>=1 &&
        GoodMuon.size()>=2){
       HMetB->Fill(*MET_pt);
     }

     // 2e1Mu
     if(*nElectron>=2 && *nMuon>=1 &&
        GoodElectron.size()>=2 &&
        GoodMuon.size()>=1){
       HMetC->Fill(*MET_pt);
     }

     // 3e0Mu
     if(*nElectron>=3 && GoodElectron.size()>=3){
       HMetD->Fill(*MET_pt);
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

  auto SetStyle = [](TH1 *h) {
    gPad->SetGrid();
    gPad-> SetLogy();
    h->SetFillColor(16);
    h->SetFillStyle(4050);
  };

  ch->cd(1);
  HMetA->SetTitle("0e3Mu;MET_pt;Event count");
  SetStyle(HMetA);
  HMetA->Draw();
  ch->cd(2);
  HMetB->SetTitle("1e2Mu;MET_pt;Event count");
  SetStyle(HMetB);
  HMetB->Draw();
  ch->cd(3);
  HMetC->SetTitle("2e1Mu;MET_pt;Event count");
  SetStyle(HMetC);
  HMetC->Draw();
  ch->cd(4);
  HMetD->SetTitle("3e0Mu;MET_pt;Event count");
  SetStyle(HMetD);
  HMetD->Draw();
  ch->Print("PlotMet.png");
}
