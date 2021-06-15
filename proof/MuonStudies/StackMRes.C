#include "RooTFnBinding.h"

using namespace RooFit ;

double DSCB(double *x, double *par){

  double alpha_l = par[0];
  double alpha_h = par[1];
  double n_l     = par[2];
  double n_h     = par[3];
  double mean    = par[4];
  double sigma   = par[5];
  double N       = par[6];
  double t = (x[0]-mean)/sigma;
  double result;

  double fAlphaL = alpha_l/n_l;
  double f2AlphaL = (n_l/alpha_l) - alpha_l - t;
  double fAlphaH = alpha_h/n_h;
  double f2AlphaH = (n_h/alpha_h) - alpha_h + t;

  if (-alpha_l <= t && alpha_h >= t){
    result = exp(-0.5*t*t);
  } else if (t < -alpha_l) {
    result = exp(-0.5*alpha_l*alpha_l)*pow(fAlphaL*f2AlphaL, -n_l);
  } else if (t > alpha_h) {
    result = exp(-0.5*alpha_h*alpha_h)*pow(fAlphaH*f2AlphaH, -n_h);
  }

  return N*result;

}

TGraphAsymmErrors* plotFits(Int_t year, std::string hname, Bool_t isData = false){

  TFile *f1 = TFile::Open("MuonStudies.root");

  TCanvas* c1 = new TCanvas("c1","c1",2000,1000);
  c1->Divide(4,2);

  TCanvas* cPull = new TCanvas("cPull","cPull",4*500,2*500);
  cPull->Divide(4,2);

  std::string histopath = Form("%d/DYJetsToMuMu_M-50_Zpt-150toInf_TuneCP5_13TeV-madgraphMLM_pdfwgt_F-pythia8/%s",year,hname.c_str());
  if(isData)
    histopath = Form("%d/ULSingleMuon/%s",year,hname.c_str());

  std::cout << histopath << "\n";
  
  std::vector<Float_t> sigmas;
  std::vector<Float_t> sigmaErrors;
  std::vector<Float_t> ptBins;

  TH2F* h2 = static_cast<TH2F*>(f1->Get(histopath.c_str()));

  Double_t YLimit = 0.;

  TH1D *h;

  for(int k = 1; k < 8; ++k){
    c1->cd(k);

    Float_t ptBinLow = h2->GetXaxis()->GetBinLowEdge(k);
    if( ptBinLow > 500.) break;

    Float_t ptBinHigh = h2->GetXaxis()->GetBinLowEdge(k+1);
    ptBins.emplace_back(ptBinLow);
    if(k==7)
      ptBins.emplace_back(ptBinHigh);

    h = static_cast<TH1D*>(h2->ProjectionY(Form("%s_%.0f",hname.c_str(),ptBinHigh),k));
    h->SetTitle(Form("%s [%.0f:%.0f];Dimuon Mass [GeV];Event Count", hname.c_str(),ptBinLow, ptBinHigh));

    if (k == 1)
      YLimit = h->GetMaximum()*1.1;

    std::pair<float,float> MassWindow = {75., 105};

    RooRealVar *mass = new RooRealVar("mass","m_{Z} (GeV)",MassWindow.first,MassWindow.second);
    mass->setBins(10000,"cache");
    RooRealVar *massZdpg = new RooRealVar("massZdpg","DPG Mass Z", 91.1855, 91.1897);
    RooRealVar *widthZdpg =  new RooRealVar("widthZdpg","DPG Width Z", 2.4929, 2.4975);
    RooBreitWigner *breitW = new RooBreitWigner("breitW","Fit PDF",*mass,*massZdpg,*widthZdpg);

    TF1 *fxDCB = new TF1(Form("fxDCB_%.0f_%s",ptBinLow,hname.c_str()),
                         DSCB,MassWindow.first, MassWindow.second, 7 /*nParams*/);
    fxDCB->SetParameters(1., 1., 10, 10, 90., h->GetRMS(), h->GetMaximum()*0.9);
    fxDCB->SetParLimits(4, 89.,93.);
    h->Fit(fxDCB,"MB","",MassWindow.first,MassWindow.second);
    h->Fit(fxDCB,"MB","",MassWindow.first,MassWindow.second);

    RooAbsPdf* dcb = RooFit::bindPdf(fxDCB,*mass);
    RooDataHist dh1("dh1","dh1",*mass,h);
    std::string title = Form("%s [%.0f:%.0f] MC [%d];Pt [GeV];Event Count", hname.c_str(),ptBinLow, ptBinHigh,year);
    if(isData)
      title = Form("%s [%.0f:%.0f] Data [%d];Pt [GeV];Event Count", hname.c_str(),ptBinLow, ptBinHigh,year);
    
    RooPlot* frame = mass->frame(Title(title.c_str()));
    //RooFFTConvPdf* bwdcb = new RooFFTConvPdf("bwdcb","BreitWigner DCB", *mass, *breitW, *dcb);

    dh1.plotOn(frame);

    //bwdcb->fitTo(dh1);
    std::cout << "calling fitTo\n";
    RooFitResult* fResult = dcb->fitTo(dh1,Range(MassWindow.first,MassWindow.second),Save(true),Minos(true));
    sigmas.emplace_back(fxDCB->GetParameter(5));
    sigmaErrors.emplace_back(fxDCB->GetParError(5));
    //bwdcb->plotOn(frame);

    dcb->plotOn(frame);
    //h->Draw();

    RooPlot* framePull = mass->frame(Title(Form("Pull %s",title.c_str())));
    framePull->SetName(Form("fPull_%.0f_%.0f_%s_%d",ptBinLow,ptBinHigh,hname.c_str(),year));
    RooHist* hpull = frame->pullHist();
    hpull->SetName(Form("hPull_%.0f_%.0f_%s_%d",ptBinLow,ptBinHigh,hname.c_str(),year));
    framePull->addPlotable(hpull,"P");


    frame->Draw();
    frame->GetYaxis()->SetRangeUser(0.,YLimit);
    if( ptBinLow >= 150. ){
      frame->GetYaxis()->SetRangeUser(0.1,YLimit*10.);
      gPad->SetLogy();
      gPad->Modified();
      gPad->Update();
    }

    cPull->cd(k);
    framePull->Draw();

    //fxDCB->Draw("SAME");
  }

  std::string fname = Form("%d_%s_Fits.png",year,hname.c_str());

  if(isData)
    fname = Form("%d_%s_Fits_Data.png",year,hname.c_str());

  c1->Print(fname.c_str());

  if(!isData){
    cPull->Print(Form("%d_%s_Pull_Fits.png",year,hname.c_str()));
  }else{
    cPull->Print(Form("%d_%s_Pull_Fits_Data.png",year,hname.c_str()));
  }


  TGraphAsymmErrors* g = new TGraphAsymmErrors(7);
  g->SetName(Form("%d_%s_g",year,hname.c_str()));

  for(int i = 0; i < 7; ++i){
    Double_t mid = (ptBins[i]+ptBins[i+1])/2.;
    Double_t dx = mid - ptBins[i];
    g->SetPoint(i,mid,sigmas[i]);
    g->SetPointError(i,dx,dx,sigmaErrors[i],sigmaErrors[i]);
  }

  c1->Clear();
  g->Print();
  g->SetMarkerColor(4);
  g->SetMarkerStyle(21);
  g->SetTitle(Form("Mass Resolution [%d]; Pt; Mass Resolution",year));
  g->Draw("AP");
  //g->GetXaxis()->SetRangeUser(0,3100);
  fname = Form("%d_%s.png",year,hname.c_str());
  if(isData)
    fname = Form("%d_%s_Data.png",year,hname.c_str());
  c1->Print(fname.c_str());
  delete c1;

  return g;

}

int StackMRes(){

  std::vector<std::string> Histos2D =
    { "HMassZPt_BB_G", "HMassZPt_BE_G",
      "HMassZPt_BB_T","HMassZPt_BE_T" };

  TCanvas* cp1 = new TCanvas("cp1","cp1", 1000, 1000);
  cp1->Divide(2,2);

  Int_t year = 2018;
  Int_t k = 1;

  for(auto hname : Histos2D){
    TMultiGraph *mg = new TMultiGraph();
    mg->SetName(Form("%d_mg_%s",year,hname.c_str()));
    mg->SetTitle(Form("%s [%d];Pt [GeV];Mass Resolution at Z Peak [GeV]",hname.c_str(),year));
    TGraphAsymmErrors* gmc = plotFits(year, hname);
    gmc->SetLineColor(kRed);
    gmc->SetMarkerColor(kRed);
    TGraphAsymmErrors* gdata = plotFits(year, hname, true);
    gdata->SetLineColor(kBlack);
    gdata->SetMarkerColor(kBlack);
    mg->Add(gmc,"P");
    mg->Add(gdata,"P");
    mg->GetYaxis()->SetRangeUser(-1.,10.);
    cp1->cd(k);
    mg->Draw("AP");

    TLegend *l = new TLegend();
    l->SetName(Form("%d",year));
    l->AddEntry(gmc,"MC");
    l->AddEntry(gdata,"Data");
    l->Draw();
    std::cout << k << "\t" << hname << "\n";
    ++k;
  }

  cp1->Print(Form("%d_.png",year));

  return 0;
}
