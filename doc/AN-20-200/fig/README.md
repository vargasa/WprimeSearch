#### TODO


```cpp
auto f = TFile::Open("files/mc/2016/sf/EfficienciesStudies_UL2016_postVFP_DEN_TrackerMuons_rootfiles_Efficiencies_muon_generalTracks_Z_Run2016_UL_ID.root");
auto h = static_cast<TH2F*>(f->Get("NUM_TrkHighPtID_DEN_TrackerMuons_abseta_pt")->Clone());
h->SetTitle("TrkHighPtID SF 2016; |#eta|;Muon Pt; SF")
TCanvas* c1 = new TCanvas("c1","c1", 1200,1200);
gStyle->SetOptStat(0);
h->Draw("COLZH TEXT");
h->GetXaxis()->SetLabelOffset(0.01);
h->GetXaxis()->SetLabelSize(0.03);
h->GetXaxis()->SetTitleSize(0.03);
h->GetXaxis()->SetTitleOffset(1.6);
h->GetYaxis()->SetLabelOffset(0.01);
h->GetYaxis()->SetLabelSize(0.03);
h->GetYaxis()->SetTitleSize(0.03);
h->GetYaxis()->SetTitleOffset(1.6);
h->GetZaxis()->SetLabelOffset(0.01);
h->GetZaxis()->SetLabelSize(0.03);
h->GetZaxis()->SetTitleSize(0.03);
h->GetZaxis()->SetTitleOffset(1.6);
c1->Print("2016Muon_TrkHighPtID.pdf");
```

```cpp
auto f = TFile::Open("files/mc/2016/sf/EfficienciesStudies_UL2016_postVFP_DEN_TrackerMuons_rootfiles_Efficiencies_muon_generalTracks_Z_Run2016_UL_ID.root");
auto h = static_cast<TH2F*>(f->Get("NUM_HighPtID_DEN_TrackerMuons_abseta_pt")->Clone());
h->SetTitle("GlobalHighPtID SF 2016; |#eta|;Muon Pt; SF")
TCanvas* c1 = new TCanvas("c1","c1", 1200,1200);
gStyle->SetOptStat(0);
h->Draw("COLZH TEXT");
h->GetXaxis()->SetLabelOffset(0.01);
h->GetXaxis()->SetLabelSize(0.03);
h->GetXaxis()->SetTitleSize(0.03);
h->GetXaxis()->SetTitleOffset(1.6);
h->GetYaxis()->SetLabelOffset(0.01);
h->GetYaxis()->SetLabelSize(0.03);
h->GetYaxis()->SetTitleSize(0.03);
h->GetYaxis()->SetTitleOffset(1.6);
h->GetZaxis()->SetLabelOffset(0.01);
h->GetZaxis()->SetLabelSize(0.03);
h->GetZaxis()->SetTitleSize(0.03);
h->GetZaxis()->SetTitleOffset(1.6);
c1->Print("2016Muon_GlobalHighPtID.pdf");
```

```cpp
auto f = TFile::Open("files/mc/2016/sf/2016LegacyReReco_ElectronLoose.root");
auto h = static_cast<TH2F*>(f->Get("EGamma_SF2D")->Clone());
h->SetTitle("Electron Loose ID SF 2016; #eta;Electron Pt; SF")
TCanvas* c1 = new TCanvas("c1","c1", 1200,1200);
gStyle->SetOptStat(0);
h->Draw("COLZH TEXT");
h->GetXaxis()->SetLabelOffset(0.01);
h->GetXaxis()->SetLabelSize(0.03);
h->GetXaxis()->SetTitleSize(0.03);
h->GetXaxis()->SetTitleOffset(1.6);
h->GetYaxis()->SetLabelOffset(0.01);
h->GetYaxis()->SetLabelSize(0.03);
h->GetYaxis()->SetTitleSize(0.03);
h->GetYaxis()->SetTitleOffset(1.6);
h->GetZaxis()->SetLabelOffset(0.01);
h->GetZaxis()->SetLabelSize(0.03);
h->GetZaxis()->SetTitleSize(0.03);
h->GetZaxis()->SetTitleOffset(1.6);
c1->Print("2016Electron_LooseID.pdf");
```
