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

   if ( /* JetMetFlags */
       *Flag_goodVertices &
       *Flag_globalSuperTightHalo2016Filter &
       *Flag_HBHENoiseFilter &
       *Flag_HBHENoiseIsoFilter &
       *Flag_EcalDeadCellTriggerPrimitiveFilter &
       *Flag_BadPFMuonFilter ) {
     Int_t n = *nMuon;
     hnMuon->Fill(n);

   }
   return kTRUE;
}

void DYPreSelector::Terminate() {

  ch = new TCanvas("ch");
  hnMuon->Draw();
  ch->Print("nMuon.png");
}
