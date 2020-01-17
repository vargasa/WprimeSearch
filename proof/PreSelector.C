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

  fOutput->Add(HMuon_ptA);
  fOutput->Add(HElectron_ptA);

  fOutput->Add(HMuon_ptB);
  fOutput->Add(HElectron_ptB);

  fOutput->Add(HMuon_ptC);
  fOutput->Add(HElectron_ptC);

  fOutput->Add(HMuon_ptD);
  fOutput->Add(HElectron_ptD);
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

     //hTauMass->Fill(m);

     // 0e3Mu
     if(*nElectron>0 && *nMuon>=3){
       float pt;
       HElectron_ptA->Fill(Electron_pt[0]);
       HMuon_ptA->Fill(Muon_pt[0]);
     }

     // 1e2Mu
     if(*nElectron!=0 && *nMuon>=2){
       HElectron_ptB->Fill(Electron_pt[0]);
       HMuon_ptB->Fill(Muon_pt[0]);
     }

     // 2e1Mu
     if(*nElectron>=2 && *nMuon>=1){
       HElectron_ptC->Fill(Electron_pt[0]);
       HMuon_ptC->Fill(Muon_pt[0]);
     }

     // 3e0Mu
     if(*nElectron>=3 && *nMuon>0){
       HElectron_ptD->Fill(Electron_pt[0]);
       HMuon_ptD->Fill(Muon_pt[0]);
     }

   }

   return kTRUE;
}

void PreSelector::Terminate() {

  ch = new TCanvas("ch");
  ch->Divide(2,2);

  ch->cd(1);
  HElectron_ptA->Draw();
  ch->cd(2);
  HElectron_ptB->Draw();
  ch->cd(3);
  HElectron_ptC->Draw();
  ch->cd(4);
  HElectron_ptD->Draw();
  ch->Print("LeadingElectronPt.png");

  ch->cd(1);
  HMuon_ptA->Draw();
  ch->cd(2);
  HMuon_ptB->Draw();
  ch->cd(3);
  HMuon_ptC->Draw();
  ch->cd(4);
  HMuon_ptD->Draw();
  ch->Print("LeadingMuonPt.png");

}
