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


#### PileUp profiles

```cpp
TFile* f1 = TFile::Open("PileupHistogram-goldenJSON-13tev-2016-69200ub.root","READ");
TFile* f2 = TFile::Open("PileupHistogram-goldenJSON-13tev-2017-69200ub.root","READ");
TFile* f3 = TFile::Open("PileupHistogram-goldenJSON-13tev-2018-69200ub.root","READ");

TH1F* p6 = (TH1F*)f1->Get("pileup");
p6->SetTitle("2016");
p6->SetLineColor(kBlue);
TH1F* p7 = (TH1F*)f2->Get("pileup");
p7->SetTitle("2017");
p7->SetLineColor(kGreen);
TH1F* p8 = (TH1F*)f3->Get("pileup");
p8->SetTitle("2018");
p8->SetLineColor(kRed);

p6->SetTitle("Pileup Distribution; NPvx; Events")

TLegend* legend = new TLegend(0.7, 0.75, .9, .9);
legend->AddEntry(p6,"2016");
legend->AddEntry(p7,"2017");
legend->AddEntry(p8,"2018");

p6->Draw("LC");
p7->Draw("LC SAME");
p8->Draw("LC SAME");

gStyle->SetOptStat(0);
legend->Draw();

c1->Print("GoldenPileup_Linear.pdf");
c1->SetLogy();
c1->Print("GoldenPileup_LogScale.pdf");
```

#### MC Pileup profiles

```cpp
TFile* f1 = TFile::Open("WprimeHistos_KFactorIncluded.root","READ");
TH1F* h6 = (TH1F*)f1->Get("2016/WZTo3LNu_mllmin01_13TeV-powheg-pythia8/HTruePileup");
h6->SetTitle("MC Pileup Distribution; NPvx; Normalized Events");
h6->SetLineColor(kBlue);
TH1F* h7 = (TH1F*)f1->Get("2017/WZTo3LNu_mllmin01_NNPDF31_TuneCP5_13TeV_powheg_pythia8/HTruePileup");
h7->SetLineColor(kGreen);
TH1F* h8 = (TH1F*)f1->Get("2018/WZTo3LNu_mllmin01_NNPDF31_TuneCP5_13TeV_powheg_pythia8/HTruePileup");
h8->SetLineColor(kRed);

h6->Scale(1/h6->GetMaximum());
h7->Scale(1/h7->GetMaximum());
h8->Scale(1/h8->GetMaximum());

TLegend* legend = new TLegend(0.7, 0.75, .9, .9);
legend->AddEntry(h6,"MC WZ 2016");
legend->AddEntry(h7,"MC WZ 2017");
legend->AddEntry(h8,"MC WZ 2018");

h6->Draw("LC");
h7->Draw("LC SAME");
h8->Draw("LC SAME");

gStyle->SetOptStat(0);
legend->Draw();

c1->Print("MC_Pileup_Linear.pdf");
c1->SetLogy();
c1->Print("MC_Pileup_LogScale.pdf");
```
