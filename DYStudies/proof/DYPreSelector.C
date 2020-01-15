#include "TFile.h"
#include "DYPreSelector.h"

DYPreSelector::DYPreSelector(TTree *)
{
  hnMuon=0;
}

void DYPreSelector::Init(TTree *tree)
{
  //Called every time a new TTree is attached.
  fReader.SetTree(tree);

}
void DYPreSelector::Begin(TTree *tree) {

}

void DYPreSelector::SlaveBegin(TTree *tree) {

  hnMuon = new TH1I("hnMuon","",10,0,10);
  fOutput->Add(hnMuon);
}



Bool_t DYPreSelector::Process(Long64_t entry) {

   fReader.SetEntry(entry);

   // Event Selection
   if ( HLT_DoubleEle33_CaloIdL_MW &&
        Flag_HBHENoiseFilter &&
        Flag_HBHENoiseIsoFilter &&
        Flag_EcalDeadCellTriggerPrimitiveFilter &&
        Flag_globalTightHalo2016Filter &&
        Flag_BadPFMuonSummer16Filter &&
        PV_npvsGood > 1 &&
        PV_ndof > 4
        ) {

     //hTauMass->Fill(m);

     // 0e3Mu
     if(*nElectron>=0 && *nMuon>=3){

     }

     // 1e2Mu
     if(*nElectron!=0 && *nMuon>=2){

     }

     // 2e1Mu
     if(*nElectron>=2 && *nMuon>=1){

     }

     // 3e0Mu
     if(*nElectron>=3 && *nMuon>=0){

     }

   }

   return kTRUE;
}

void DYPreSelector::Terminate() {

  ch = new TCanvas("ch");
  hnMuon->Draw();
}
