#include "TFile.h"
#include "PreSelector.h"
#include "Math/Vector4D.h"
#include "Math/Vector4Dfwd.h"


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

  HnElA=0;
  HnElB=0;
  HnElC=0;
  HnElD=0;

  HnMuA=0;
  HnMuB=0;
  HnMuC=0;
  HnMuD=0;

  HMassA=0;
  HMassB=0;
  HMassC=0;
  HMassD=0;

  HOverlap=0;

}

void PreSelector::Init(TTree *tree)
{
  //Called every time a new TTree is attached.
  fReader.SetTree(tree);

}
void PreSelector::Begin(TTree *tree) {

  if (fInput->FindObject("Mass")) {
    TParameter<Int_t> *p = dynamic_cast<TParameter<Int_t>*>(fInput->FindObject("Mass"));
    Mass = p->GetVal();
  }

}

void PreSelector::SlaveBegin(TTree *tree) {

  const Double_t MaxMet = 1000.;
  const Double_t MinMet = 0.;
  const Int_t MetBins = 100;

  HMetA = new TH1F("HMetA","",MetBins,MinMet,MaxMet);
  HMetB = new TH1F("HMetB","",MetBins,MinMet,MaxMet);
  HMetC = new TH1F("HMetC","",MetBins,MinMet,MaxMet);
  HMetD = new TH1F("HMetD","",MetBins,MinMet,MaxMet);

  const Double_t MaxnLep = 7;
  const Double_t MinnLep = 0;
  const Int_t nLepBins = 7;

  HnElA = new TH1I("HnElA","",nLepBins,MinnLep,MaxnLep);
  HnElB = new TH1I("HnElB","",nLepBins,MinnLep,MaxnLep);
  HnElC = new TH1I("HnElC","",nLepBins,MinnLep,MaxnLep);
  HnElD = new TH1I("HnElD","",nLepBins,MinnLep,MaxnLep);

  HnMuA = new TH1I("HnMuA","",nLepBins,MinnLep,MaxnLep);
  HnMuB = new TH1I("HnMuB","",nLepBins,MinnLep,MaxnLep);
  HnMuC = new TH1I("HnMuC","",nLepBins,MinnLep,MaxnLep);
  HnMuD = new TH1I("HnMuD","",nLepBins,MinnLep,MaxnLep);

  const Double_t MinMass = 0.;
  const Double_t MaxMass = 150.;
  const Int_t MassBins = 60;

  HMassA = new TH1F("HMassA","",MassBins,MinMass,MaxMass);
  HMassB = new TH1F("HMassB","",MassBins,MinMass,MaxMass);
  HMassC = new TH1F("HMassC","",MassBins,MinMass,MaxMass);
  HMassD = new TH1F("HMassD","",MassBins,MinMass,MaxMass);

  HOverlap = new TH1I("HOverlap","Overlapping events."
                      " -1: l<3 0:None 1: NoOverlap",6,-1,5);
  fOutput->Add(HOverlap);

  fOutput->Add(HMassA);
  fOutput->Add(HMassB);
  fOutput->Add(HMassC);
  fOutput->Add(HMassD);

  fOutput->Add(HMetA);
  fOutput->Add(HMetB);
  fOutput->Add(HMetC);
  fOutput->Add(HMetD);

  fOutput->Add(HnElA);
  fOutput->Add(HnElB);
  fOutput->Add(HnElC);
  fOutput->Add(HnElD);

  fOutput->Add(HnMuA);
  fOutput->Add(HnMuB);
  fOutput->Add(HnMuC);
  fOutput->Add(HnMuD);

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
   if ( (*HLT_DoubleEle33_CaloIdL_MW || *HLT_IsoMu20) &&
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

     auto MassReco = [](Double_t pt1, Double_t eta1, Double_t phi1, Double_t m1,
                        Double_t pt2, Double_t eta2, Double_t phi2, Double_t m2){

       ROOT::Math::PtEtaPhiMVector l1(pt1, eta1, phi1, m1);
       ROOT::Math::PtEtaPhiMVector l2(pt2, eta2, phi2, m2);

       return (l1+l2).M();

     };

     std::vector<UInt_t> GoodElectron = GetGoodElectron(Els);
     std::vector<UInt_t> GoodMuon = GetGoodMuon(Mus);

     const Double_t Electron_mass = 0.510998950;
     const Double_t Muon_mass = 0.105658755;

     Bool_t IsA{},IsB{},IsC{},IsD{};

     // 0e3Mu
     if(*nMuon>=3 && GoodMuon.size()>=3){
       IsD = true;
       HMetD->Fill(*MET_pt);
       HnElD->Fill(GoodElectron.size());
       HnMuD->Fill(GoodMuon.size());

       UInt_t i = GoodMuon[0];
       UInt_t j = GoodMuon[1];
       if(Muon_charge[i] != Muon_charge[j]){
         Double_t m = MassReco(Muon_pt[i],Muon_eta[i],Muon_phi[i],Muon_mass,
                               Muon_pt[j],Muon_eta[j],Muon_phi[j],Muon_mass);
         HMassD->Fill(m);
       }
     }

     // 1e2Mu
     if(*nElectron!=0 && *nMuon>=2 &&
        GoodElectron.size()>=1 &&
        GoodMuon.size()>=2){
       IsC = true;
       HMetC->Fill(*MET_pt);
       HnElC->Fill(GoodElectron.size());
       HnMuC->Fill(GoodMuon.size());

       UInt_t i = GoodMuon[0];
       UInt_t j = GoodMuon[1];
       if(Muon_charge[i] != Muon_charge[j]){
         Double_t m = MassReco(Muon_pt[i],Muon_eta[i],Muon_phi[i],Muon_mass,
                      Muon_pt[j],Muon_eta[j],Muon_phi[j],Muon_mass);
         HMassC->Fill(m);
       }
     }

     // 2e1Mu
     if(*nElectron>=2 && *nMuon>=1 &&
        GoodElectron.size()>=2 &&
        GoodMuon.size()>=1){
       IsB = true;
       HMetB->Fill(*MET_pt);
       HnElB->Fill(GoodElectron.size());
       HnMuB->Fill(GoodMuon.size());

       UInt_t i = GoodElectron[0];
       UInt_t j = GoodElectron[1];
       if(Electron_charge[i] != Electron_charge[j]){
         Double_t m = MassReco(Electron_pt[i],Electron_eta[i], Electron_phi[i],Electron_mass,
                               Electron_pt[j],Electron_eta[j],Electron_phi[j],Electron_mass);
         HMassB->Fill(m);
       }
     }

     // 3e0Mu
     if(*nElectron>=3 && GoodElectron.size()>=3){
       IsA = true;
       HMetA->Fill(*MET_pt);
       HnElA->Fill(GoodElectron.size());
       HnMuA->Fill(GoodMuon.size());

       UInt_t i = GoodElectron[0];
       UInt_t j = GoodElectron[1];
       if(Electron_charge[i] != Electron_charge[j]){
         Double_t m = MassReco(Electron_pt[i],Electron_eta[i],Electron_phi[i],Electron_mass,
                               Electron_pt[j],Electron_eta[j],Electron_phi[j],Electron_mass);
         HMassA->Fill(m);
       }
     }

     // 3leptons

     if( (*nElectron + *nMuon) >= 3){
       HOverlap->Fill(IsA+IsB+IsC+IsD);
     } else {
       HOverlap->Fill(-1);
     }

   }
   return kTRUE;
}

void PreSelector::Terminate() {

  ch = new TCanvas("ch","ch",1200,800);
  ch->Divide(2,2);

  THStack *hsA = new THStack("hsA","");
  THStack *hsB = new THStack("hsB","");
  THStack *hsC = new THStack("hsC","");
  THStack *hsD = new THStack("hsD","");

  gStyle->SetOptStat(1111111);
  auto SetStyle = [](TH1 *h) {
    gPad->SetGrid();
    gPad->SetLogy();
    h->SetFillColor(16);
    h->SetFillStyle(4050);
  };

  ch->cd(1);
  HMetA->SetTitle("3e0#mu;#slash{M}^{3e0#mu}_{T};Event count");
  SetStyle(HMetA);
  HMetA->Draw();
  ch->cd(2);
  HMetB->SetTitle("2e1#mu;#slash{M}^{2e1#mu}_{T};Event count");
  SetStyle(HMetB);
  HMetB->Draw();
  ch->cd(3);
  HMetC->SetTitle("1e2#mu;#slash{M}^{1e2#mu}_{T};Event count");
  SetStyle(HMetC);
  HMetC->Draw();
  ch->cd(4);
  HMetD->SetTitle("0e3#mu;#slash{M}^{0e3#mu}_{T};Event count");
  SetStyle(HMetD);
  HMetD->Draw();
  ch->Print(Form("PlotMet_%d.png",Mass));

  ch->cd(1);
  gStyle->SetOptStat(1111111);
  HnElA->SetFillColor(kGreen);
  HnMuA->SetFillColor(kBlue);
  hsA->Add(HnMuA);
  hsA->Add(HnElA);
  hsA->SetTitle("3e0#mu;n;Event count");
  hsA->Draw();
  ch->cd(2);
  HnElB->SetFillColor(kGreen);
  HnMuB->SetFillColor(kBlue);
  hsB->Add(HnMuB);
  hsB->Add(HnElB);
  hsB->SetTitle("2e1#mu;n;Event count");
  hsB->Draw();
  ch->cd(3);
  HnElC->SetFillColor(kGreen);
  HnMuC->SetFillColor(kBlue);
  hsC->Add(HnMuC);
  hsC->Add(HnElC);
  hsC->SetTitle("1e2#mu;n;Event count");
  hsC->Draw();
  ch->cd(4);
  HnElD->SetFillColor(kGreen);
  HnMuD->SetFillColor(kBlue);
  hsD->Add(HnMuD);
  hsD->Add(HnElD);
  hsD->SetTitle("0e3#mu;n;Event count");
  hsD->Draw();
  ch->Print(Form("nGoodLeptons_%d.png",Mass));

  ch->cd(1);
  HMassA->SetTitle("Z Mass;M_{Z}^{3e0#mu};Event count");
  HMassA->Draw();
  ch->cd(2);
  HMassB->SetTitle("Z Mass;M_{Z}^{2e1#mu};Event count");
  HMassB->Draw();
  ch->cd(3);
  HMassC->SetTitle("Z Mass;M_{Z}^{1e2#mu};Event count");
  HMassC->Draw();
  ch->cd(4);
  HMassD->SetTitle("Z Mass;M_{Z}^{0e3#mu};Event count");
  HMassD->Draw();
  ch->Print(Form("HMass_%d.png",Mass));

  ch->cd(0);
  HOverlap->Draw();
  ch->Print(Form("Overlap_%d.png",Mass));

}
