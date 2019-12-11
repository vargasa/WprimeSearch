#include "TFile.h"
#include "DYPreSelector.h"

DYPreSelector::DYPreSelector(TTree *)
{
  hnMuon=0;
  hTauPt = 0;
  hTauMass = 0;
  hTauMass2 = 0;
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
  hTauPt = new TH1F("hTauPt","",50,0.,150.);
  hTauMass = new TH1F("hTauMass","",50,0,2.5);
  hTauMass2 = new TH1F("hTauMass2","",100,0.13,0.15);
  fOutput->Add(hnMuon);
  fOutput->Add(hTauPt);
  fOutput->Add(hTauMass);
  fOutput->Add(hTauMass2);
}



Bool_t DYPreSelector::Process(Long64_t entry) {

   fReader.SetEntry(entry);

   if ( /* JetMetFlags */
       *Flag_goodVertices &
       *Flag_globalSuperTightHalo2016Filter &
       *Flag_HBHENoiseFilter &
       *Flag_HBHENoiseIsoFilter &
       *Flag_EcalDeadCellTriggerPrimitiveFilter &
       *Flag_BadPFMuonFilter &
       // 0L_2taus category
       *nTau >= 2 &
       // Taus are sorted by descending pt
       ( Tau_charge[0] != Tau_charge[1] ) &
       ( Tau_pt[0] > 40. & Tau_pt[1] > 40.) &
       ( Tau_eta[0] < 2.1 & Tau_eta[1] < 2.1 ) /*&
       ( Tau_mass[0] + Tau_mass[1] >= 12. ) */) {

     //Int_t n = *nMuon;
     //hnMuon->Fill(n);
     for (auto m: Tau_mass) {
       hTauMass->Fill(m);
       if (m<0.25) hTauMass2->Fill(m);
     }
     for (auto pt: Tau_pt) hTauPt->Fill(pt);
   }

   return kTRUE;
}

void DYPreSelector::Terminate() {

  ch = new TCanvas("ch");
  hnMuon->Draw();
  ch->Print("nMuon.png");
  hTauPt->Draw();
  ch->Print("TauPt.png");
  hTauMass->Draw();
  ch->Print("TauMass.png");
  hTauMass2->Draw();
  ch->Print("TauMass2.png");

}
