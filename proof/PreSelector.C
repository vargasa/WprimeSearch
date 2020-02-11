#include "TFile.h"
#include "PreSelector.h"
#include "Math/Vector4D.h"
#include "Math/Vector4Dfwd.h"
#include "TMath.h"


PreSelector::PreSelector(TTree *)
{
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

  HMassWA=0;
  HMassWB=0;
  HMassWC=0;
  HMassWD=0;

  HOverlap=0;

  HGenPartA=0;
  HGenPartB=0;
  HGenPartC=0;
  HGenPartD=0;

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

  if (fInput->FindObject("SampleName")) {
    // Lesson: TString can't be in TCollection
    TNamed *p = dynamic_cast<TNamed *>(fInput->FindObject("SampleName"));
    SampleName = p->GetTitle();
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
  const Double_t MaxMass = 200.;
  const Int_t MassBins = 50;

  HMassA = new TH1F("HMassA","",MassBins,MinMass,MaxMass);
  HMassB = new TH1F("HMassB","",MassBins,MinMass,MaxMass);
  HMassC = new TH1F("HMassC","",MassBins,MinMass,MaxMass);
  HMassD = new TH1F("HMassD","",MassBins,MinMass,MaxMass);

  HMassWA = new TH1F("HMassWA","",MassBins,MinMass,MaxMass);
  HMassWB = new TH1F("HMassWB","",MassBins,MinMass,MaxMass);
  HMassWC = new TH1F("HMassWC","",MassBins,MinMass,MaxMass);
  HMassWD = new TH1F("HMassWD","",MassBins,MinMass,MaxMass);

  fOutput->Add(HMassWA);
  fOutput->Add(HMassWB);
  fOutput->Add(HMassWC);
  fOutput->Add(HMassWD);

  HOverlap = new TH1I("HOverlap","Overlapping events."
                      " -1: l<3 0:None 1: NoOverlap",6,-1,5);
  fOutput->Add(HOverlap);

  const UInt_t BinsPdgId = 51;
  const Float_t PdgIdMin = -25.;
  const Float_t PdgIdMax = 25.;

  HGenPartA = new TH1I("HGenPartA","",BinsPdgId,PdgIdMin,PdgIdMax);
  HGenPartB = new TH1I("HGenPartB","",BinsPdgId,PdgIdMin,PdgIdMax);
  HGenPartC = new TH1I("HGenPartC","",BinsPdgId,PdgIdMin,PdgIdMax);
  HGenPartD = new TH1I("HGenPartD","",BinsPdgId,PdgIdMin,PdgIdMax);

  fOutput->Add(HGenPartA);
  fOutput->Add(HGenPartB);
  fOutput->Add(HGenPartC);
  fOutput->Add(HGenPartD);

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


std::vector<UInt_t> PreSelector::GetGoodMuon(Muons Mu){
  std::vector<UInt_t> GoodIndex = {};
  const Float_t MaxEta = 2.4;
  const Float_t MinPt = 10.;
  const Float_t MaxDxy = 0.2; // 2mm
  const Float_t MaxDz = 0.5; // 5mm
  for (UInt_t i=0; i<*Mu.n;i++){
    if(Mu.isGlobal[i] && Mu.looseId[i] &&
       Mu.pt[i]>MinPt && abs(Mu.eta[i])<MaxEta &&
       abs(Mu.dxy[i])<MaxDxy && abs(Mu.dz[i])<MaxDz)
      GoodIndex.emplace_back(i);
  }
  return GoodIndex;
};

std::vector<UInt_t> PreSelector::GetGoodElectron(Electrons El){
  const Float_t MaxEta = 2.5;
  const Float_t MinPt = 20.;
  const Float_t MaxRelIso = 0.15;
  std::vector<UInt_t> GoodIndex = {};
  UInt_t index = 0;
  for (UInt_t i = 0; i< *El.n; i++){
    if(El.cutBased[i]>=1 && El.pt[i]>MinPt &&
       abs(El.eta[i])<MaxEta &&
       El.miniPFRelIso_all[i]<MaxRelIso)
      GoodIndex.emplace_back(i);
  }
  return GoodIndex;
};

Double_t PreSelector::MassRecoZ(Double_t pt1, Double_t eta1, Double_t phi1, Double_t m1,
                                Double_t pt2, Double_t eta2, Double_t phi2, Double_t m2){

  ROOT::Math::PtEtaPhiMVector l1(pt1, eta1, phi1, m1);
  ROOT::Math::PtEtaPhiMVector l2(pt2, eta2, phi2, m2);

  return (l1+l2).M();
};

Double_t PreSelector::MassRecoW(Double_t ptl, Double_t etal, Double_t phil, Double_t ml,
                                Double_t ptmet, Double_t phimet){
  return TMath::Sqrt(2.*ptl*ptmet*(1-TMath::Cos(phil-phimet)));
};


std::vector<std::pair<UInt_t,UInt_t>> PreSelector::GetLeptonPairs(Leptons l, std::vector<UInt_t> GoodIndex){

  std::vector<UInt_t> positive;
  std::vector<UInt_t> negative;

  for(UInt_t i=0; i< GoodIndex.size(); i++){
    if (l.charge[GoodIndex[i]] == 1){
      positive.emplace_back(GoodIndex[i]);
    } else {
      negative.emplace_back(GoodIndex[i]);
    }
  }

  if(positive.size() == 0 || negative.size() == 0) throw std::range_error("No pairs found");

  std::vector<std::pair<UInt_t,UInt_t>> couples;

  for(UInt_t i=0; i< positive.size(); i++){
    for(UInt_t j=0; j< negative.size();j++){
      couples.emplace_back(std::make_pair(positive[i],negative[j]));
    }
  }

  return couples;

}

std::vector<std::tuple<Double_t,Double_t,std::pair<UInt_t,UInt_t>>> PreSelector::FindZ(Leptons l,std::vector<UInt_t> GoodLepton){

  std::vector<std::pair<UInt_t,UInt_t>> Pairs ;

  try {
    Pairs = PreSelector::GetLeptonPairs(l,GoodLepton);
  } catch (const std::exception& e){
    throw std::range_error("No pairs found!");
  }

  // ZMassDistance, ZMass, Pair
  std::vector<std::tuple<Double_t,Double_t,std::pair<UInt_t,UInt_t>>> ZMassTuple;
  const Double_t ZNominalMass = 91.1876;

  for(UInt_t k = 0; k< Pairs.size(); k++){
    UInt_t i = Pairs[k].first;
    UInt_t j = Pairs[k].second;
    Double_t m = PreSelector::MassRecoZ(l.pt[i],l.eta[i],l.phi[i],l.mass,
                                        l.pt[j],l.eta[j],l.phi[j],l.mass);
    ZMassTuple.emplace_back(std::make_tuple(abs(ZNominalMass-m),m,std::make_pair(i,j)));
  }

  std::sort(ZMassTuple.begin(),ZMassTuple.end()); //By ZMassDistance
  return ZMassTuple;

}

Bool_t PreSelector::Process(Long64_t entry) {

   fReader.SetEntry(entry);

   // Event Selection
   if ( (*HLT_DoubleEle33_CaloIdL_MW || *HLT_IsoMu20) &&
        *Flag_HBHENoiseFilter &&
        *Flag_HBHENoiseIsoFilter &&
        *Flag_EcalDeadCellTriggerPrimitiveFilter &&
        *Flag_globalTightHalo2016Filter &&
        *Flag_BadPFMuonSummer16Filter &&
        *PV_npvsGood > 0 &&
        *MET_pt > 30
        ) {

     Muons Mus(nMuon,Muon_pt,Muon_eta,Muon_phi,
               Muon_charge,Muon_dxy,Muon_dz,
               Muon_looseId,Muon_isGlobal);

     Electrons Els(nElectron,Electron_pt,Electron_eta,Electron_phi,
                   Electron_charge,Electron_dxy,Electron_dz,
                   Electron_cutBased,Electron_miniPFRelIso_all);

     std::vector<UInt_t> GoodElectron = PreSelector::GetGoodElectron(Els);
     std::vector<UInt_t> GoodMuon = PreSelector::GetGoodMuon(Mus);

     const Double_t MinRemPt = 20.;

     Bool_t IsA{},IsB{},IsC{},IsD{};

     // 0e3Mu
     if(*nMuon>=3 && GoodMuon.size()>=3){
       IsD = true;
       Bool_t NoPairs{};

       std::vector<std::tuple<Double_t,Double_t,std::pair<UInt_t,UInt_t>>> zt;

       try {
         zt = PreSelector::FindZ(Mus,GoodMuon);
       } catch (const std::exception& e) {
         NoPairs = true;
       }

       if (!NoPairs){
         HMetD->Fill(*MET_pt);
         HnElD->Fill(GoodElectron.size());
         HnMuD->Fill(GoodMuon.size());

         Double_t BestMass = std::get<1>(zt[0]);

         UInt_t l1 = (std::get<2>(zt[0])).first;
         UInt_t l2 = (std::get<2>(zt[0])).second;

         std::vector<UInt_t> WCand;
         for(auto i: GoodMuon){
           if(i!=l1 && i!=l2) WCand.emplace_back(i);
         }

         UInt_t lead = WCand[0];
         if(Mus.pt[lead]>MinRemPt && Muon_highPtId[lead] == 2){
           Double_t wm =
             PreSelector::MassRecoW(Mus.pt[lead], Mus.eta[lead],
                                    Mus.phi[lead], Mus.mass,
                                    *MET_pt, *MET_phi);
           HGenPartD->Fill(GenPart_pdgId[Muon_genPartIdx[l1]]);
           HGenPartD->Fill(GenPart_pdgId[Muon_genPartIdx[l2]]);
           HGenPartD->Fill(GenPart_pdgId[Muon_genPartIdx[lead]]);
           HMassD->Fill(BestMass);
           HMassWD->Fill(wm);
         }
       }
     }

     // 1e2Mu
     if(*nElectron!=0 && *nMuon>=2 &&
        GoodElectron.size()>=1 &&
        GoodMuon.size()>=2){
       IsC = true;
       Bool_t NoPairs{};

       std::vector<std::tuple<Double_t,Double_t,std::pair<UInt_t,UInt_t>>> zt;

       try {
         zt = PreSelector::FindZ(Mus,GoodMuon);
       } catch (const std::exception& e) {
         NoPairs = true;
       }

       if (!NoPairs){
         HMetC->Fill(*MET_pt);
         HnElC->Fill(GoodElectron.size());
         HnMuC->Fill(GoodMuon.size());

         Double_t BestMass = std::get<1>(zt[0]);

         UInt_t l1 = (std::get<2>(zt[0])).first;
         UInt_t l2 = (std::get<2>(zt[0])).second;

         UInt_t lead = GoodElectron[0];
         if(Els.pt[lead]>MinRemPt){
           Double_t wm =
             PreSelector::MassRecoW(Els.pt[lead], Els.eta[lead],
                                    Els.phi[lead], Els.mass,
                                    *MET_pt, *MET_phi);
           HGenPartC->Fill(GenPart_pdgId[Muon_genPartIdx[l1]]);
           HGenPartC->Fill(GenPart_pdgId[Muon_genPartIdx[l2]]);
           HGenPartC->Fill(GenPart_pdgId[Electron_genPartIdx[lead]]);
           HMassC->Fill(BestMass);
           HMassWC->Fill(wm);
         }
       }
     }

     // 2e1Mu
     if(*nElectron>=2 && *nMuon>=1 &&
        GoodElectron.size()>=2 &&
        GoodMuon.size()>=1){
       IsB = true;
       Bool_t NoPairs{};

       std::vector<std::tuple<Double_t,Double_t,std::pair<UInt_t,UInt_t>>> zt;

       try {
         zt = PreSelector::FindZ(Els,GoodElectron);
       } catch (const std::exception& e) {
         NoPairs = true;
       }

       if (!NoPairs){
         HMetB->Fill(*MET_pt);
         HnElB->Fill(GoodElectron.size());
         HnMuB->Fill(GoodMuon.size());

         Double_t BestMass = std::get<1>(zt[0]);

         UInt_t l1 = (std::get<2>(zt[0])).first;
         UInt_t l2 = (std::get<2>(zt[0])).second;

         UInt_t lead = GoodMuon[0];
         if(Mus.pt[lead]>MinRemPt && Muon_highPtId[lead] == 2){
           Double_t wm =
             PreSelector::MassRecoW(Mus.pt[lead], Mus.eta[lead],
                                    Mus.phi[lead], Mus.mass,
                                    *MET_pt, *MET_phi);
           HGenPartB->Fill(GenPart_pdgId[Electron_genPartIdx[l1]]);
           HGenPartB->Fill(GenPart_pdgId[Electron_genPartIdx[l2]]);
           HGenPartB->Fill(GenPart_pdgId[Muon_genPartIdx[lead]]);

           HMassB->Fill(BestMass);
           HMassWB->Fill(wm);
         }

       }



     }

     // 3e0Mu
     if(*nElectron>=3 && GoodElectron.size()>=3){
       IsA = true;
       Bool_t NoPairs{};

       std::vector<std::tuple<Double_t,Double_t,std::pair<UInt_t,UInt_t>>> zt;

       try {
         zt = PreSelector::FindZ(Els,GoodElectron);
       } catch (const std::exception& e) {
         NoPairs = true;
       }

       if (!NoPairs){
         HMetA->Fill(*MET_pt);
         HnElA->Fill(GoodElectron.size());
         HnMuA->Fill(GoodMuon.size());

         Double_t BestMass = std::get<1>(zt[0]);

         UInt_t l1 = (std::get<2>(zt[0])).first;
         UInt_t l2 = (std::get<2>(zt[0])).second;

         std::vector<UInt_t> WCand;
         for(auto i: GoodElectron){
           if(i!=l1 && i!=l2) WCand.emplace_back(i);
         }

         UInt_t lead = WCand[0];
         if(Els.pt[lead]>MinRemPt){
           Double_t wm =
             PreSelector::MassRecoW(Els.pt[lead], Els.eta[lead],
                                    Els.phi[lead], Els.mass,
                                    *MET_pt, *MET_phi);
           HGenPartA->Fill(GenPart_pdgId[Electron_genPartIdx[l1]]);
           HGenPartA->Fill(GenPart_pdgId[Electron_genPartIdx[l2]]);
           HGenPartA->Fill(GenPart_pdgId[Electron_genPartIdx[lead]]);
           HMassA->Fill(BestMass);
           HMassWA->Fill(wm);
         }
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

  std::unique_ptr<TCanvas> ch(new TCanvas("ch","ch",1200,800));
  ch->Divide(2,2);

  std::unique_ptr<TFile> fOut(TFile::Open("WprimeHistos.root","UPDATE"));
  fOut->mkdir(SampleName);
  fOut->cd(SampleName);

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
  HMetA->Write("HMetA");
  ch->cd(2);
  HMetB->SetTitle("2e1#mu;#slash{M}^{2e1#mu}_{T};Event count");
  SetStyle(HMetB);
  HMetB->Draw();
  HMetB->Write("HMetB");
  ch->cd(3);
  HMetC->SetTitle("1e2#mu;#slash{M}^{1e2#mu}_{T};Event count");
  SetStyle(HMetC);
  HMetC->Draw();
  HMetC->Write("HMetC");
  ch->cd(4);
  HMetD->SetTitle("0e3#mu;#slash{M}^{0e3#mu}_{T};Event count");
  SetStyle(HMetD);
  HMetD->Draw();
  HMetD->Write("HMetD");
  ch->Print(Form("%s_PlotMet_%d.png",SampleName.Data(),Mass));

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
  ch->Print(Form("%s_nGoodLeptons_%d.png",SampleName.Data(),Mass));

  ch->cd(1);
  HMassA->SetTitle("Z Mass (3e0#mu);M_{Z}^{3e0#mu};Event count");
  HMassA->Draw();
  HMassA->Write("HMassA");
  ch->cd(2);
  HMassB->SetTitle("Z Mass (2e1#mu);M_{Z}^{2e1#mu};Event count");
  HMassB->Draw();
  HMassB->Write("HMassB");
  ch->cd(3);
  HMassC->SetTitle("Z Mass (1e2#mu);M_{Z}^{1e2#mu};Event count");
  HMassC->Draw();
  HMassC->Write("HMassC");
  ch->cd(4);
  HMassD->SetTitle("Z Mass (0e3#mu);M_{Z}^{0e3#mu};Event count");
  HMassD->Draw();
  HMassD->Write("HMassD");
  ch->Print(Form("%s_HMass_%d.png",SampleName.Data(),Mass));

  ch->cd(1);
  HMassWA->SetTitle("M_{T}^{W}(3e0#mu);M_{WT}^{3e0#mu};Event count");
  HMassWA->Draw();
  HMassWA->Write("HMassWA");
  ch->cd(2);
  HMassWB->SetTitle("M_{T}^{W}(2e1#mu);M_{WT}^{2e1#mu};Event count");
  HMassWB->Draw();
  HMassWB->Write("HMassWB");
  ch->cd(3);
  HMassWC->SetTitle("M_{T}^{W}(1e2#mu);M_{WT}^{1e2#mu};Event count");
  HMassWC->Draw();
  HMassWC->Write("HMassWC");
  ch->cd(4);
  HMassWD->SetTitle("M_{T}^{W}(0e3#mu);M_{WT}^{0e3#mu};Event count");
  HMassWD->Draw();
  HMassWD->Write("HMassWD");
  ch->Print(Form("%s_HMassWT_%d.png",SampleName.Data(),Mass));

  ch->Clear();
  ch->Divide(2,2);
  ch->cd(1);
  HGenPartA->SetTitle("PdgId (3e0#mu); PdgId; Event count");
  HGenPartA->Draw();
  ch->cd(2);
  HGenPartB->SetTitle("PdgId (2e1#mu); PdgId; Event count");
  HGenPartB->Draw();
  ch->cd(3);
  HGenPartC->SetTitle("PdgId (1e2#mu); PdgId; Event count");
  HGenPartC->Draw();
  ch->cd(4);
  HGenPartD->SetTitle("PdgId (0e3#mu); PdgId; Event count");
  HGenPartD->Draw();
  ch->Print(Form("%s_GenPart_%d.png",SampleName.Data(),Mass));

  ch->cd(0);
  HOverlap->Draw();
  HOverlap->Write("HOverlap");
  ch->Print(Form("%s_Overlap_%d.png",SampleName.Data(),Mass));

  fOut->Write();
  fOut->Close();

}
