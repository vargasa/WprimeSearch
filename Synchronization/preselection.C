{

  TFile *f = TFile::Open("testNanoAOD_NANO.root");
  auto t1 = (TTree*)f->Get("Events");
  auto c1 = new TCanvas("c1","c1");

  // Preselection for Muon and Electrons from:
  // https://gitlab.cern.ch/ttH_leptons/doc/blob/master/Legacy/objects.md#2151-preselection

  TCut ePt = "Electron_pt>7";
  TCut eEta = "abs(Electron_eta)<2.5";
  TCut eDxy = "Electron_dxy<=0.05";
  TCut eDz = "Electron_dz<0.1";
  TCut eMIso = "Electron_miniPFRelIso_all<0.4";
  TCut eSip3d = "Electron_sip3d<8.";
  TCut eWPLoose = "HLT_Ele25_eta2p1_WPLoose_Gsf";

  t1->Draw("Electron_pt",
           ePt && eEta && eDxy && eDz && eMIso && eSip3d && eWPLoose);
  TH1F *hEle = (TH1F*)gPad->GetPrimitive("htemp");
  hEle->GetXaxis()->SetRangeUser(0.,300.);
  hEle->SetTitle("Electron Pt after Preselection;Pt;Counts");

  c1->Print("electron.png");

  TCut muPt = "Muon_pt>5";
  TCut muEta = "abs(Muon_eta)<2.4";
  TCut muDxy = "Muon_dxy<=0.05";
  TCut muDz = "Muon_dz<0.1";
  TCut muMIso = "";
  TCut muSip3d = "Muon_sip3d<8.";

  t1->Draw("Muon_pt",
           muPt && muEta && muDxy && muDz && muMIso && muSip3d);
  TH1F *hMu = (TH1F*)gPad->GetPrimitive("htemp");
  hMu->GetXaxis()->SetRangeUser(0.,300.);
  hMu->SetTitle("Muon Pt after Preselection;Pt;Counts");

  c1->Print("muon.png");

}
