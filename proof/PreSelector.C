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

}




Bool_t PreSelector::Process(Long64_t entry) {

   fReader.SetEntry(entry);

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

     // 0e3Mu
     if(*nMuon>=3){

     }

     // 1e2Mu
     if(*nElectron!=0 && *nMuon>=2){

     }

     // 2e1Mu
     if(*nElectron>=2 && *nMuon>=1){

     }

     // 3e0Mu
     if(*nElectron>=3){

     }

     // 3leptons

     if( (*nElectron + *nMuon) == 3){

     }

   }
   return kTRUE;
}

void PreSelector::Terminate() {

  ch = new TCanvas("ch","ch",1200,600);
  ch->Divide(3,1);

  ch->cd(1);
  HElectron_ptA->Draw();
  ch->cd(2);
  HMuon_ptA->Draw();
  
  ch->cd(3);
  HElectron_ptB->Add(HElectron_ptA);
  HElectron_ptB->Add(HMuon_ptA);
  HElectron_ptB->Draw();
  
  ch->Print("LeadingLeptonPt.png");

  

  ch->cd(1);
  HMetZA->Draw();
  ch->cd(2);
  HMetZB->Draw();
  ch->Print("HMetZ.png");
}
