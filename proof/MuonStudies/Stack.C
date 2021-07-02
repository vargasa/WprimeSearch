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

TGraphAsymmErrors* GetResolutionGraph(const int year, const int& etaBins_) {

  std::cout << "=============\t" << year << "\t=============\t" << etaBins_ << "\t=============\n";

  TFile* f1 = TFile::Open("MuonStudies_PtHistos.root","READ");

  std::vector<std::string> etaBins = {
    "HPResB_G", "HPResO_G", "HPResE_G",
    "HPResB_T", "HPResO_T",
  };

  std::pair<float,float> MuonB = { 0., 1.2 };
  std::pair<float,float> MuonO = { 1.2, 2.1 };
  std::pair<float,float> MuonE = { 2.1, 2.4 };

  std::vector<std::string> titleEtaBins = {
    Form("%.1f <=|#eta|<= %.1f [globalHighPtId]; (1/p-1/p^{GEN})/(1/p^{GEN}); Event Count", MuonB.first, MuonB.second),  //B_G
    Form("%.1f <|#eta|<= %.1f [globalHighPtId]; (1/p-1/p^{GEN})/(1/p^{GEN}); Event Count", MuonO.first, MuonO.second),   //O_G
    Form("%.1f <|#eta|<= %.1f [globalHighPtId]; (1/p-1/p^{GEN})/(1/p^{GEN}); Event Count", MuonE.first, MuonE.second),   //E_G
    Form("%.1f <=|#eta|<= %.1f [trackerHighPtId]; (1/p-1/p^{GEN})/(1/p^{GEN}); Event Count", MuonB.first, MuonB.second), //B_T
    Form("%.1f <|#eta|<= %.1f [trackerHighPtId]; (1/p-1/p^{GEN})/(1/p^{GEN}); Event Count", MuonO.first, MuonE.second),  //O_T
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

  // TH3D: P,Pt,PRes
  // "zx" z(PRes)-> Y; x(P) -> X;
  // "zy" z(PRes)-> Y; y(Pt) -> X;
  std::string projection = "zx";
  std::string hpath = Form("%d/%s/%s",year,samples[year][0].first.c_str(),etaBins[etaBins_].c_str());
  std::clog << hpath << "\n";
  auto hp =
    static_cast<TH2D*>((static_cast<TH3D*>(f1->Get(hpath.c_str())))->Project3D(projection.c_str()));
  hp->SetName(Form("%d_%s_%s",year,samples[year][0].first.c_str(),etaBins[etaBins_].c_str()));
  //hp->Scale(samples[year][0].second);
  for (int i = 1; i < samples[year].size(); ++i) {
    TH2D* h =
      static_cast<TH2D*>((static_cast<TH3D*>(f1->Get(Form("%d/%s/%s",year,samples[year][i].first.c_str(),etaBins[etaBins_].c_str()))))->Project3D(projection.c_str()));
    //hp->Add(h,samples[year][i].second);
    hp->Add(h);
  }

  TCanvas* c1 = new TCanvas("c1","c1",5*500,3*500);
  c1->Divide(5,3);
  TCanvas* cPull = new TCanvas("cPull","cPull",5*500,3*500);
  cPull->Divide(5,3);
  TCanvas* cResidual = new TCanvas("cResidual","cResidual",5*500,3*500);
  cResidual->Divide(5,3);

  Int_t nParams = 7;

  gStyle->SetOptFit(1111);

  std::vector<Double_t> sigmas;
  std::vector<Double_t> sigmaErrors;
  std::vector<Double_t> ptBins;

  std::vector<Double_t> means;
  std::vector<Double_t> meanErrors;

  Double_t prevSigma = 0.;

  Double_t YLimit = 0.;

  TCanvas* c0 = new TCanvas("c0","c0",500,500);

  for(uint i = 1; i <= 15; ++i){
    c1->cd(i);

    Double_t ptBinMin = hp->GetXaxis()->GetBinLowEdge(i);
    Double_t ptBinMax = hp->GetXaxis()->GetBinLowEdge(i+1);

    ptBins.emplace_back(hp->GetXaxis()->GetBinLowEdge(i));

    TH1D* h = hp->ProjectionY("_h",i);
    h->SetName(Form("h_%d_%s_%d",year,etaBins[etaBins_].c_str(),i));
    h->Sumw2();

    if (i == 1)
      YLimit = h->GetMaximum() * 1.1;

    Double_t xmin = -0.5;// -2.*h->GetRMS();
    Double_t xmax = 0.5;//1.3*h->GetRMS();

    if (etaBins_ >= 3) {
      xmin = -0.35;
      xmax = 0.5;
    }

    TF1 *fxDCB = new TF1(Form("fxDCB_%d_%s",year,etaBins[etaBins_].c_str()),
                         DSCB, xmin, xmax, nParams);
    fxDCB->SetParNames ("#alpha_{low}","#alpha_{high}","n_{low}", "n_{high}", "#mu", "#sigma", "N");

    std::string fitOption = "";
    if(etaBins_ < 3){
      Double_t lowMean = -0.2;
      Double_t highMean = h->GetMean() + 0.1;
      Double_t lowN = h->GetMaximum()*0.8;
      Double_t highN = h->GetMaximum()*1.01;
      fxDCB->SetParameters(1., 1., 10, 10, h->GetMean(), h->GetRMS(), h->GetMaximum());
      fxDCB->SetParLimits(4,lowMean,highMean);
      fxDCB->SetParLimits(5,0,0.5);
      fxDCB->SetParLimits(6,lowN,highN);
      fitOption = "M R";
    } else {
      Double_t lowN = h->GetMaximum()*0.75;
      Double_t highN = h->GetMaximum();
      fxDCB->SetParameters(1., 1., 10, 10, h->GetMean(), h->GetRMS() , h->GetMaximum());
      fxDCB->SetParLimits(5, prevSigma > 0.? prevSigma : 0.04, 100.);
      fxDCB->SetParLimits(6,lowN,highN);
      if ( highN < 10. ) {
        fitOption = "M R WL";
      } else {
        fitOption = "M R";
      }
    }

    for(int i = 0; i < 8; ++i){
      h->Fit(fxDCB,fitOption.c_str(),"",xmin,xmax);
    }


    RooRealVar pres("pres","P Residual",xmin,xmax);
    pres.setBins(10000);
    RooAbsPdf* dcb = RooFit::bindPdf(fxDCB,pres);
    RooDataHist dh1("dh1","dh1",pres,h);
    RooPlot* frame = pres.frame(Title(Form("[%.1f:%.1f] GeV %s [%d];(1/p-1/p^{GEN})/(1/p^{GEN});Event Count",ptBinMin,ptBinMax,titleEtaBins[etaBins_].c_str(),year)));


    dcb->fitTo(dh1,SumW2Error(true));
    dh1.plotOn(frame);
    dcb->plotOn(frame);

    RooHist* hpull = frame->pullHist();
    hpull->SetName(Form("hPull_%.0f_%.0f_%s_%d",ptBinMin,ptBinMax,etaBins[etaBins_].c_str(),year));
    RooPlot* framePull = pres.frame(Title(Form("Pull %s",titleEtaBins[etaBins_].c_str())));
    framePull->SetName(Form("fPull_%.0f_%.0f_%s_%d",ptBinMin,ptBinMax,etaBins[etaBins_].c_str(),year));
    framePull->addPlotable(hpull,"P");
    RooHist* hresid = frame->residHist();
    RooPlot* frameResid= pres.frame(Title(Form("Residual [%.1f:%.1f] GeV %s [%d]",ptBinMin,ptBinMax,etaBins[etaBins_].c_str(),year)));
    frameResid->addPlotable(hresid,"P");

    prevSigma = fxDCB->GetParameter(5);

    h->SetTitle(Form("[%.1f:%.1f] GeV %s [%d];(1/p-1/p^{GEN})/(1/p^{GEN});Event Count",ptBinMin,ptBinMax,etaBins[etaBins_].c_str(),year));

    means.emplace_back(fxDCB->GetParameter(4));
    sigmas.emplace_back(fxDCB->GetParameter(5));

    meanErrors.emplace_back(fxDCB->GetParError(4));
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
    gPad->SetLeftMargin(0.15);
    gPad->SetBottomMargin(0.15);
    frame->Draw();
    frame->GetYaxis()->SetRangeUser(0.,YLimit);
    //fxDCB->Draw("SAME");
    gausFx->SetLineColor(kBlue);
    gausFx->SetLineStyle(7);
    //gausFx->Draw("SAME");
    exp1Fx->SetLineColor(kBlack);
    exp1Fx->SetLineStyle(7);
    //exp1Fx->Draw("SAME");
    //exp2Fx->SetLineColor(kRed);
    exp2Fx->SetLineStyle(7);
    ///exp2Fx->Draw("SAME");
    cPull->cd(i);
    framePull->Draw();
    cResidual->cd(i);
    frameResid->Draw();
    //delete gausFx;
    //delete fxDCB;
    //delete exp1Fx;
    //delete exp2Fx;
  }

  c1->Print(Form("%d_%s_.png",year,etaBins[etaBins_].c_str()));
  cPull->Print(Form("%d_%s_Pull.png",year,etaBins[etaBins_].c_str()));
  //cResidual->Print(Form("%d_%s_Residual.png",year,etaBins[etaBins_].c_str()));

  ptBins.emplace_back(hp->GetXaxis()->GetBinLowEdge(12));

  const int nPoints = 11;

  std::vector<TGraphAsymmErrors*> plots;

  TGraphAsymmErrors* g = new TGraphAsymmErrors(nPoints);
  TGraphAsymmErrors* gMeans = new TGraphAsymmErrors(nPoints);

  for(int i = 0; i < nPoints; ++i){
    Double_t mid = (ptBins[i]+ptBins[i+1])/2.;
    Double_t dx = mid - ptBins[i];
    g->SetPoint(i,mid,sigmas[i]);
    gMeans->SetPoint(i,mid,means[i]);
    g->SetPointError(i,dx,dx,sigmaErrors[i],sigmaErrors[i]);
    gMeans->SetPointError(i,dx,dx,meanErrors[i],meanErrors[i]);
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
    g->GetYaxis()->SetRangeUser(0,0.2);
  } else {
    g->GetYaxis()->SetRangeUser(0,0.25);
  }
  c1->Print(Form("%d_%d_PResolution.png",year,etaBins_));


  c1->Clear();
  gMeans->SetMarkerColor(4);
  gMeans->SetMarkerStyle(21);
  gMeans->SetTitle(Form("%s [%d]; P; #mu",etaBins[etaBins_].c_str(),year));
  gMeans->Draw("AP")
  c1->Print(Form("%d_%d_P_Bias.png",year,etaBins_));
  plots.emplace_back(gMeans);

  delete c1;

  return g;

}

int Stack() {

  std::vector<int> etaBins = { 0, 1, 2, 3, 4};

  TCanvas* c = new TCanvas("c","c",3*500,2*500);
  c->Divide(3,2);

  std::vector<int>  Years = { 2016, 2017, 2018 };

  std::pair<float,float> MuonB = { 0., 1.2 };
  std::pair<float,float> MuonO = { 1.2, 2.1 };
  std::pair<float,float> MuonE = { 2.1, 2.4 };

  std::vector<std::string> titleEtaBins = {
    Form("%.1f <=|#eta|<= %.1f [globalHighPtId]; P [GeV]; Resolution [%%]", MuonB.first, MuonB.second),
    Form("%.1f <|#eta|<= %.1f [globalHighPtId]; P [GeV]; Resolution [%%]", MuonO.first, MuonO.second),
    Form("%.1f <|#eta|<= %.1f [globalHighPtId]; P [GeV]; Resolution [%%]", MuonE.first, MuonE.second),
    Form("%.1f <=|#eta|<= %.1f [trackerHighPtId]; P [GeV]; Resolution [%%]", MuonB.first, MuonB.second),
    Form("%.1f <|#eta|<= %.1f [trackerHighPtId]; P [GeV]; Resolution [%%]", MuonO.first, MuonE.second),
  };

  std::vector<Int_t> colorEtaBins = {
    kRed, kBlack, kBlue, kRed, kBlack
  };

  std::vector<std::string> legendEtaBins = {
    Form("%.1f <=|#eta|<= %.1f", MuonB.first, MuonB.second),
    Form("%.1f <|#eta|<= %.1f", MuonO.first, MuonO.second),
    Form("%.1f <|#eta|<= %.1f", MuonE.first, MuonE.second),
    Form("%.1f <=|#eta|<= %.1f", MuonB.first, MuonB.second),
    Form("%.1f <|#eta|<= %.1f", MuonO.first, MuonE.second),
  };

  for(auto yr: Years){

    TMultiGraph *mgG = new TMultiGraph();
    mgG->SetName(Form("mgG_%d",yr));
    mgG->SetTitle(Form("P Resolution [%d] [globalHighPt]; P Reco; P Resolution [#sigma]",yr));

    TMultiGraph *mgT = new TMultiGraph();
    mgT->SetName(Form("mgT_%d",yr));
    mgT->SetTitle(Form("P Resolution [%d] [trackerHighPt]; P Reco; P Resolution [#sigma]",yr));

    TLegend *lG = new TLegend(0.7,0.2,0.95,0.3);
    lG->SetName(Form("lG_%d",yr));

    TLegend *lT = new TLegend(0.7,0.2,0.95,0.3);
    lG->SetName(Form("lT_%d",yr));

    for(auto etaBins_: etaBins){
      TGraphAsymmErrors *g = GetResolutionGraph(yr,etaBins_);
      g->Print();
      g->SetLineColor(colorEtaBins[etaBins_]);
      g->SetMarkerColor(colorEtaBins[etaBins_]);
      g->SetMarkerStyle(23);
      if (etaBins_ < 3){
        lG->AddEntry(g,legendEtaBins[etaBins_].c_str());
        mgG->Add(g,"P");
      } else {
        mgT->Add(g,"P");
        lT->AddEntry(g,legendEtaBins[etaBins_].c_str());
      }
    }

    c->cd(yr%2015);
    gPad->SetLeftMargin(0.15);
    gPad->SetBottomMargin(0.15);
    mgG->Draw("AP");
    lG->Draw();
    mgG->GetXaxis()->SetRangeUser(0,3100);
    mgG->GetYaxis()->SetRangeUser(0.,0.18);

    c->cd((yr%2015)+3);
    gPad->SetLeftMargin(0.15);
    gPad->SetBottomMargin(0.15);
    mgT->Draw("AP");
    lT->Draw();
    mgT->GetXaxis()->SetRangeUser(0,3100);
    mgT->GetYaxis()->SetRangeUser(0.,0.18);
  }

  c->Print(Form("ResolutionMeasurement.png"));

  return 0;
}
