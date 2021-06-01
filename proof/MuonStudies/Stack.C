#include "RooTFnBinding.h"

using namespace RooFit ;

Double_t DCBFunction(Double_t *x, Double_t *par){

  Double_t alpha_l = par[0];
  Double_t alpha_h = par[1];
  Double_t n_l     = par[2];
  Double_t n_h     = par[3];
  Double_t mean    = par[4];
  Double_t sigma   = par[5];
  Double_t N       = par[6];
  Double_t result;

  if (  x[0] >= alpha_l and x[0] <= alpha_h ) {
    result = exp(-0.5*pow(x[0]-mean,2.)/pow(sigma,2.));
  } else if (x[0] < alpha_l) {
    result = pow(n_l/alpha_l,n_l)*exp(-0.5*pow(alpha_l,2.))*pow( n_l/alpha_l + alpha_l + x[0], -n_l);
  } else if (x[0] > alpha_h) {
    result = pow(n_h/alpha_h,n_h)*exp(-0.5*pow(alpha_h,2.))*pow( n_h/alpha_h + alpha_h + x[0], -n_h);
  }

  return N*result;
}

double DSCB_ROOTForum(double *x, double *par){

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

TGraphAsymmErrors* GetResolutionGraph(const int year, const int& etaBins_) {

  TFile* f1 = TFile::Open("MuonStudies.root");

  std::vector<std::string> etaBins = {
    "HPResidualB_G", "HPResidualO_G", "HPResidualE_G",
    "HPResidualB_T", "HPResidualO_T", "HPResidualE_T"
  };


  std::unordered_map<int,std::vector<std::pair<std::string,Double_t>>> samples =
    {
      {
        2016,
        {
          // {"ZToMuMu_NNPDF30_13TeV-powheg_M_50_120", 2.116e+03},
          // {"ZToMuMu_NNPDF30_13TeV-powheg_M_120_200", 2.058e+01},
          // {"ZToMuMu_NNPDF30_13TeV-powheg_M_200_400", 2.890e+00},
          {"ZToMuMu_NNPDF30_13TeV-powheg_M_400_800", 2.515e-01},
          {"ZToMuMu_NNPDF30_13TeV-powheg_M_800_1400", 1.709e-02},
          {"ZToMuMu_NNPDF30_13TeV-powheg_M_1400_2300", 1.370e-03},
          {"ZToMuMu_NNPDF30_13TeV-powheg_M_2300_3500", 8.282e-05},
          {"ZToMuMu_NNPDF30_13TeV-powheg_M_3500_4500", 3.414e-06},
          {"ZToMuMu_NNPDF30_13TeV-powheg_M_4500_6000", 3.650e-07},
          {"ZToMuMu_NNPDF30_13TeV-powheg_M_6000_Inf", 2.526e-08},
        },
      },
      {
        2017,
        {
          // { "ZToMuMu_NNPDF31_TuneCP5_13TeV-powheg-pythia8_M_50_120", 2.116e+03},
          // { "ZToMuMu_NNPDF31_TuneCP5_13TeV-powheg-pythia8_M_120_200", 2.058e+01},
          // { "ZToMuMu_NNPDF31_TuneCP5_13TeV-powheg-pythia8_M_200_400", 2.890e+00},
          { "ZToMuMu_NNPDF31_TuneCP5_13TeV-powheg-pythia8_M_400_800", 2.515e-01},
          { "ZToMuMu_NNPDF31_TuneCP5_13TeV-powheg-pythia8_M_800_1400", 1.709e-02},
          { "ZToMuMu_NNPDF31_TuneCP5_13TeV-powheg-pythia8_M_1400_2300", 1.370e-03},
          { "ZToMuMu_NNPDF31_TuneCP5_13TeV-powheg-pythia8_M_2300_3500", 8.282e-05},
          { "ZToMuMu_NNPDF31_TuneCP5_13TeV-powheg-pythia8_M_3500_4500", 3.414e-06},
          { "ZToMuMu_NNPDF31_TuneCP5_13TeV-powheg-pythia8_M_4500_6000", 3.650e-07},
          { "ZToMuMu_NNPDF31_TuneCP5_13TeV-powheg-pythia8_M_6000_Inf", 2.526e-08}
        }
      },
      {
        2018,
        {
          // { "ZToMuMu_NNPDF31_TuneCP5_13TeV-powheg-pythia8_M_50_120", 2.116e+03},
          // { "ZToMuMu_NNPDF31_TuneCP5_13TeV-powheg-pythia8_M_120_200", 2.058e+01},
          // { "ZToMuMu_NNPDF31_TuneCP5_13TeV-powheg-pythia8_M_200_400", 2.890e+00},
          { "ZToMuMu_NNPDF31_TuneCP5_13TeV-powheg-pythia8_M_400_800", 2.515e-01},
          { "ZToMuMu_NNPDF31_TuneCP5_13TeV-powheg-pythia8_M_800_1400", 1.709e-02},
          { "ZToMuMu_NNPDF31_TuneCP5_13TeV-powheg-pythia8_M_1400_2300", 1.370e-03},
          { "ZToMuMu_NNPDF31_TuneCP5_13TeV-powheg-pythia8_M_2300_3500", 8.282e-05},
          { "ZToMuMu_NNPDF31_TuneCP5_13TeV-powheg-pythia8_M_3500_4500", 3.414e-06},
          { "ZToMuMu_NNPDF31_TuneCP5_13TeV-powheg-pythia8_M_4500_6000", 3.650e-07},
          { "ZToMuMu_NNPDF31_TuneCP5_13TeV-powheg-pythia8_M_6000_Inf", 2.526e-08},
        }
      }
    };

  TH2D* hp = static_cast<TH2D*>(f1->Get(Form("%d/%s/%s",year,samples[year][0].first.c_str(),etaBins[etaBins_].c_str())));
  hp->Scale(samples[year][0].second);
  for (int i = 1; i < samples[year].size(); ++i) {
    TH2D* h = static_cast<TH2D*>(f1->Get(Form("%d/%s/%s",year,samples[year][i].first.c_str(),etaBins[etaBins_].c_str())));
    hp->Add(h,samples[year][i].second);
    hp->Add(h);
  }

  TCanvas* c1 = new TCanvas("c1","c1",4*500,3*500);
  c1->Divide(4,3);
  TCanvas* cPull = new TCanvas("cPull","cPull",4*500,3*500);
  cPull->Divide(4,3);
  TCanvas* cResidual = new TCanvas("cResidual","cResidual",4*500,3*500);
  cResidual->Divide(4,3);
  Int_t nParams = 7;

  gStyle->SetOptFit(1111);

  std::vector<Double_t> sigmas;
  std::vector<Double_t> sigmaErrors;
  std::vector<Double_t> ptBins;

  Double_t deltaL = 1. - 0.01;
  Double_t deltaR = 1. + 0.01;

  Double_t prevSigma = 0.;

  for(uint i = 1; i < 12; ++i){
    c1->cd(i);

    Double_t ptBinMin = hp->GetXaxis()->GetBinLowEdge(i);
    Double_t ptBinMax = hp->GetXaxis()->GetBinLowEdge(i+1);

    ptBins.emplace_back(hp->GetXaxis()->GetBinLowEdge(i));

    TH1D* h = hp->ProjectionY("_h",i);
    h->SetName(Form("h_%d_%s",year,etaBins[etaBins_].c_str()));
    h->Sumw2();

    Double_t xmin = -0.5;// -2.*h->GetRMS();
    Double_t xmax = 0.5;//1.3*h->GetRMS();

    if (etaBins_ >= 3) {
      xmin = -0.35;
      xmax = 0.5;
    }

    TF1 *fxDCB = new TF1(Form("fxDCB_%d_%s",year,etaBins[etaBins_].c_str()),
                         DSCB_ROOTForum, xmin, xmax, nParams);
    fxDCB->SetParNames ("#alpha_{low}","#alpha_{high}","n_{low}", "n_{high}", "#mu", "#sigma", "N");
    if(etaBins_ < 3){
      Double_t lowMean = h->GetMean() - (abs(h->GetMean())*0.8);
      Double_t highMean = h->GetMean();
      Double_t lowN = h->GetMaximum()*0.8;
      Double_t highN = h->GetMaximum();
      fxDCB->SetParameters(1., 1., 10, 10, lowMean, h->GetRMS(), h->GetMaximum());
      fxDCB->SetParLimits(4,lowMean,highMean);
      fxDCB->SetParLimits(6,lowN,highN);
    } else {
      Double_t lowMean = h->GetMean() - (abs(h->GetMean()));
      Double_t highMean = h->GetMean();
      Double_t lowN = h->GetMaximum()*0.8;
      Double_t highN = h->GetMaximum();
      fxDCB->SetParameters(1., 1., 10, 10, lowMean, h->GetRMS(), h->GetMaximum());
      fxDCB->SetParLimits(4, lowMean, highMean);
      fxDCB->SetParLimits(5, 0., 100.);
      fxDCB->SetParLimits(6,lowN,highN);
      if(prevSigma > 0.){
        fxDCB->SetParLimits(5,prevSigma*0.9,prevSigma*2.0);
      }
      if(year == 2016 and ptBinMin > 1999.){
        fxDCB->SetParameters(2., 0.5, -1e6, 1e4, -0.12, h->GetRMS(), lowN);
        fxDCB->SetParLimits(4,-0.2,-0.06);
      }
      if(year == 2017 and ptBinMin == 2500.){
        fxDCB->SetParameters(10., 1., 0.1, 1., 0.16, h->GetRMS(), lowN);
      }
      if(year == 2018){
        if(ptBinMin == 2000.){
          if(etaBins_ == 4 or etaBins_ == 5){
            fxDCB->SetParLimits(5,prevSigma,prevSigma*1.2);
          }
        }
        if(ptBinMin == 2500.){
          fxDCB->SetParameters(0.5, 0.2, 8e5, 5., -0.05, h->GetRMS(), lowN);
          fxDCB->SetParLimits(4,-0.2,0.05);
          if(etaBins_ == 4){
            fxDCB->SetParLimits(4,-0.2,-0.05);
            fxDCB->SetParLimits(5,0.10,0.11);
          }
        }
      }
    }

    h->Fit(fxDCB,"M R","",xmin,xmax);

    auto getParameter =[](Double_t p) {
      Double_t fx;
      if (p < 0){
        fx = p * 1.1;
      } else {
        fx = p * 0.9;
      }
      return fx;
    };

    fxDCB->SetParameters(getParameter(fxDCB->GetParameter(0)),
                         getParameter(fxDCB->GetParameter(1)),
                         getParameter(fxDCB->GetParameter(2)),
                         getParameter(fxDCB->GetParameter(3)),
                         getParameter(fxDCB->GetParameter(4)),
                         getParameter(fxDCB->GetParameter(5)),
                         getParameter(fxDCB->GetParameter(6)));

    // h->Fit(fxDCB,"M R ","",xmin,xmax);

    RooRealVar pres("pres","P Residual",xmin,xmax);
    RooAbsPdf* dcb = RooFit::bindPdf(fxDCB,pres);
    RooDataHist dh1("dh1","dh1",pres,h);
    RooPlot* frame = pres.frame(Title(Form("[%.1f:%.1f] GeV %s [%d];(1/p-1/p^{GEN})/(1/p^{GEN});Event Count",ptBinMin,ptBinMax,etaBins[etaBins_].c_str(),year)));

    dcb->fitTo(dh1);
    dh1.plotOn(frame);
    dcb->plotOn(frame);

    RooHist* hpull = frame->pullHist();
    hpull->SetName(Form("hPull_%.0f_%.0f_%s_%d",ptBinMin,ptBinMax,etaBins[etaBins_].c_str(),year));
    RooPlot* framePull = pres.frame(Title(Form("Pull [%.1f:%.1f] GeV %s [%d]",ptBinMin,ptBinMax,etaBins[etaBins_].c_str(),year)));
    framePull->SetName(Form("fPull_%.0f_%.0f_%s_%d",ptBinMin,ptBinMax,etaBins[etaBins_].c_str(),year));
    framePull->addPlotable(hpull,"P");
    RooHist* hresid = frame->residHist();
    RooPlot* frameResid= pres.frame(Title(Form("Residual [%.1f:%.1f] GeV %s [%d]",ptBinMin,ptBinMax,etaBins[etaBins_].c_str(),year)));
    frameResid->addPlotable(hresid,"P");

    prevSigma = fxDCB->GetParameter(5);

    h->SetTitle(Form("[%.1f:%.1f] GeV %s [%d];(1/p-1/p^{GEN})/(1/p^{GEN});Event Count",ptBinMin,ptBinMax,etaBins[etaBins_].c_str(),year));

    sigmas.emplace_back(fxDCB->GetParameter(5));
    sigmaErrors.emplace_back(fxDCB->GetParError(5));

    std::string gaussian = Form("%.8f*exp(-0.5*pow((x-(%.8f))/%.8f,2.))",
                                fxDCB->GetParameter(6),
                                fxDCB->GetParameter(4),
                                fxDCB->GetParameter(5));


    std::cout << gaussian << std::endl;

    std::string exp1 = Form("%.8f*exp(-0.5*%.8f*%.8f)*pow((%.8f/%.8f)*((%.8f/%.8f) - (%.8f) - (x-(%.8f))/%.8f),-(%.8f))",
                            fxDCB->GetParameter(6), // N
                            fxDCB->GetParameter(0), fxDCB->GetParameter(0), // alpha_L*alpha_L
                            fxDCB->GetParameter(0), fxDCB->GetParameter(2), // alphaL / n_L
                            fxDCB->GetParameter(2), fxDCB->GetParameter(0), // n_L / alpha_L
                            fxDCB->GetParameter(0), // alpha_L
                            fxDCB->GetParameter(4), fxDCB->GetParameter(5), // (x-mean)/sigma
                            fxDCB->GetParameter(2)  // n_L
                            );

    std::cout << exp1 << std::endl;

    std::string exp2 = Form("%.8f*exp(-0.5*%.8f*%.8f)*pow((%.8f/%.8f)*((%.8f/%.8f) - (%.8f) + (x-(%.8f))/%.8f),-(%.8f))",
                            fxDCB->GetParameter(6), // N
                            fxDCB->GetParameter(1), fxDCB->GetParameter(1), // alpha_H*alpha_H
                            fxDCB->GetParameter(1), fxDCB->GetParameter(3), // alpha_H / n_H
                            fxDCB->GetParameter(3), fxDCB->GetParameter(1), // n_H / alpha_H
                            fxDCB->GetParameter(1), // alpha_H
                            fxDCB->GetParameter(4), fxDCB->GetParameter(5), // (x-mean)/sigma
                            fxDCB->GetParameter(3)  // n_H
                            );



    //std::cout << exp2 << std::endl;

    TF1 *gausFx = new TF1(Form("gausFx_%d_%s",year,etaBins[etaBins_].c_str()),
                          gaussian.c_str(), xmin, xmax);
    TF1 *exp1Fx = new TF1(Form("exp1Fx_%d_%s",year,etaBins[etaBins_].c_str()),
                          exp1.c_str(), xmin, xmax);
    TF1 *exp2Fx = new TF1(Form("exp2Fx_%d_%s",year,etaBins[etaBins_].c_str()),
                          exp2.c_str(), 5e-2, xmax);

    if(etaBins_  < 3) {
      h->GetXaxis()->SetRangeUser(-0.2,0.2);
    } else {
      h->GetXaxis()->SetRangeUser(-0.35,0.5);
    }
    //h->Draw();
    frame->Draw();
    //fxDCB->Draw("SAME");
    gausFx->SetLineColor(kBlue);
    //gausFx->SetLineStyle(7);
    //gausFx->Draw("SAME");
    exp1Fx->SetLineColor(kYellow);
    exp1Fx->SetLineStyle(7);
    //exp1Fx->Draw("SAME");
    exp2Fx->SetLineColor(kGreen);
    exp2Fx->SetLineStyle(7);
    //exp2Fx->Draw("SAME");
    cPull->cd(i);
    framePull->Draw();
    cResidual->cd(i);
    frameResid->Draw();
    delete gausFx;
    //delete fxDCB;
    delete exp1Fx;
    delete exp2Fx;
  }

  c1->Print(Form("%d_%s_.png",year,etaBins[etaBins_].c_str()));
  cPull->Print(Form("%d_%s_Pull.png",year,etaBins[etaBins_].c_str()));
  cResidual->Print(Form("%d_%s_Residual.png",year,etaBins[etaBins_].c_str()));

  ptBins.emplace_back(hp->GetXaxis()->GetBinLowEdge(12));

  TGraphAsymmErrors* g = new TGraphAsymmErrors(11);

  for(int i = 0; i < 11; ++i){
    Double_t mid = (ptBins[i]+ptBins[i+1])/2.;
    Double_t dx = mid - ptBins[i];
    g->SetPoint(i,mid,sigmas[i]);
    g->SetPointError(i,dx,dx,sigmaErrors[i],sigmaErrors[i]);
    std::cout << i << "\t" << ptBins[i] << "\t" << ptBins[i+1] << "\t" << sigmas[i] <<  "\t" << sigmaErrors[i] << std::endl;
  }

  c1->Clear();
  c1->SetGrid();
  c1->GetFrame()->SetFillColor(21);
  c1->GetFrame()->SetBorderSize(12);
  g->SetMarkerColor(4);
  g->SetMarkerStyle(21);
  g->SetTitle(Form("%s [%d]; P; Resolution",etaBins[etaBins_].c_str(),year));
  g->Draw("AP");
  g->GetXaxis()->SetRangeUser(0,3100);
  if(etaBins_ <3){
    g->GetYaxis()->SetRangeUser(0,0.1);
  } else {
    g->GetYaxis()->SetRangeUser(0,0.25);
  }
  c1->Print(Form("%d_%d_PResolution.png",year,etaBins_));
  delete c1;
  //delete cPull;

  return g;

}

int Stack() {

  std::vector<int> etaBins = { 0, 1, 2, 3, 4, 5};

  TCanvas* c = new TCanvas("c","c",1500,1000);
  c->Divide(3,2);

  std::vector<std::string> titleEtaBins = {
    "Barrel [globalHighPtId]; P [GeV]; Resolution [%]",
    "Overlap [globalHighPtId]; P [GeV]; Resolution [%]",
    "Endcap [globalHighPtId]; P [GeV]; Resolution [%]",
    "Barrel [trackerHighPtId]; P [GeV]; Resolution [%]",
    "Overlap [trackerHighPtId]; P [GeV]; Resolution [%]",
    "Endcap [trackerHighPtId]; P [GeV]; Resolution [%]",
  };

  for(auto etaBins_: etaBins){
    std::cout << "cd etaBins+1 " << etaBins_+1 << std::endl;
    TMultiGraph *mg = new TMultiGraph();
    mg->SetTitle(titleEtaBins[etaBins_].c_str());
    TGraphAsymmErrors *g17 = GetResolutionGraph(2017,etaBins_);
    g17->Print();
    g17->SetMarkerStyle(27);
    g17->SetMarkerColor(kRed);
    g17->SetLineColor(kRed);
    mg->Add(g17,"P");
    TGraphAsymmErrors *g18 = GetResolutionGraph(2018,etaBins_);
    g18->Print();
    g18->SetLineColor(kBlack);
    g18->SetMarkerColor(kRed);
    g18->SetMarkerStyle(28);
    mg->Add(g18,"P");
    TGraphAsymmErrors *g16 = GetResolutionGraph(2016,etaBins_);
    g16->Print();
    g16->SetLineColor(kGreen);
    g16->SetMarkerColor(kGreen);
    g16->SetMarkerStyle(26);
    mg->Add(g16,"P");
    c->cd(etaBins_+1);
    mg->Draw("AP");
    mg->GetXaxis()->SetRangeUser(0,3100);
    if(etaBins_ < 3){
      mg->GetYaxis()->SetRangeUser(0.03,0.1);
    } else {
      mg->GetYaxis()->SetRangeUser(0.03,0.25);
    }


    TLegend *l = new TLegend();
    l->AddEntry(g16,"2016");
    l->AddEntry(g17,"2017");
    l->AddEntry(g18,"2018");
    l->Draw();
  }

  c->Print(Form("ResolutionMeasurement.png"));

  return 0;
}
