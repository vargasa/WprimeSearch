
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
          { "ZToMuMu_NNPDF30_13TeV-powheg_M_120_200",1.},
          {"ZToMuMu_NNPDF30_13TeV-powheg_M_1400_2300",1.},
          {"ZToMuMu_NNPDF30_13TeV-powheg_M_200_400",1.},
          {"ZToMuMu_NNPDF30_13TeV-powheg_M_2300_3500",1.},
          {"ZToMuMu_NNPDF30_13TeV-powheg_M_3500_4500",1.},
          {"ZToMuMu_NNPDF30_13TeV-powheg_M_400_800",1.},
          {"ZToMuMu_NNPDF30_13TeV-powheg_M_4500_6000",1.},
          {"ZToMuMu_NNPDF30_13TeV-powheg_M_50_120",1.},
          {"ZToMuMu_NNPDF30_13TeV-powheg_M_6000_Inf",1.},
          {"ZToMuMu_NNPDF30_13TeV-powheg_M_800_1400",1.}
        },
      },
      {
        2017,
        {
          //{ "ZToMuMu_NNPDF31_TuneCP5_13TeV-powheg-pythia8_M_50_120", 2.116e+03},
          //{ "ZToMuMu_NNPDF31_TuneCP5_13TeV-powheg-pythia8_M_120_200", 2.058e+01},
          //{ "ZToMuMu_NNPDF31_TuneCP5_13TeV-powheg-pythia8_M_200_400", 2.890e+00},
          { "ZToMuMu_NNPDF31_TuneCP5_13TeV-powheg-pythia8_M_400_800", 2.515e-01},
          { "ZToMuMu_NNPDF31_TuneCP5_13TeV-powheg-pythia8_M_800_1400", 1.709e-02},
          { "ZToMuMu_NNPDF31_TuneCP5_13TeV-powheg-pythia8_M_1400_2300", 1.370e-03},
          { "ZToMuMu_NNPDF31_TuneCP5_13TeV-powheg-pythia8_M_2300_3500", 8.282e-05},
          { "ZToMuMu_NNPDF31_TuneCP5_13TeV-powheg-pythia8_M_3500_4500", 3.414e-06},
          { "ZToMuMu_NNPDF31_TuneCP5_13TeV-powheg-pythia8_M_4500_6000", 3.650e-07},
          { "ZToMuMu_NNPDF31_TuneCP5_13TeV-powheg-pythia8_M_6000_Inf", 2.526e-08},
        }
      },
      {
        2018,
        {
          //{ "ZToMuMu_NNPDF31_TuneCP5_13TeV-powheg-pythia8_M_50_120", 2.116e+03},
          //{ "ZToMuMu_NNPDF31_TuneCP5_13TeV-powheg-pythia8_M_120_200", 2.058e+01},
          //{ "ZToMuMu_NNPDF31_TuneCP5_13TeV-powheg-pythia8_M_200_400", 2.890e+00},
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

  TCanvas* c1 = new TCanvas();
  Int_t nParams = 7;

  gStyle->SetOptFit(1);

  std::vector<Double_t> sigmas;
  std::vector<Double_t> sigmaErrors;
  std::vector<Double_t> ptBins;

  Double_t deltaL = 1. - 0.01;
  Double_t deltaR = 1. + 0.01;

  for(uint i = 1; i < 12; ++i){

    Double_t ptBinMin = hp->GetXaxis()->GetBinLowEdge(i);
    Double_t ptBinMax = hp->GetXaxis()->GetBinLowEdge(i+1);

    ptBins.emplace_back(hp->GetXaxis()->GetBinLowEdge(i));

    TH1D* h = hp->ProjectionY("_h",i);

    Double_t xmin = -0.5;// -2.*h->GetRMS();
    Double_t xmax = 0.5;//1.3*h->GetRMS();

    TF1 *fxDCB = new TF1("fxDCB", DSCB_ROOTForum, xmin, xmax, nParams);
    fxDCB->SetParNames ("#alpha_{low}","#alpha_{high}","n_{low}", "n_{high}", "#mu", "#sigma", "N");
    if(etaBins_ < 3){
      fxDCB->SetParameters(1., 1., 10, 10, h->GetMean(), h->GetRMS(), h->GetMaximum());
      fxDCB->SetParLimits(4,h->GetMean()*(deltaL+0.05),h->GetMean()*(deltaR-0.05));
      fxDCB->SetParLimits(6,h->GetMaximum()*deltaL,h->GetMaximum()*deltaR);
    } else {
      Double_t mm = h->GetMean()*0.6;
      fxDCB->SetParameters(1., 1., 100, 10, mm, h->GetRMS(), h->GetMaximum());
      fxDCB->SetParLimits(4,h->GetMean()*-1.,h->GetMean()*deltaR);
      fxDCB->SetParLimits(6,h->GetMaximum()*0.8,h->GetMaximum()*deltaR);
    }

    h->SetTitle(Form("[%.1f:%.1f] GeV %s;(1/p-1/p^{GEN})/(1/p^{GEN});Event Count",ptBinMin,ptBinMax,etaBins[etaBins_].c_str()));
    h->Fit(fxDCB,"MB","",xmin,xmax);
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

    TF1 *gausFx = new TF1("gausFx", gaussian.c_str(), xmin, xmax);
    TF1 *exp1Fx = new TF1("exp1Fx", exp1.c_str(), xmin, xmax);
    TF1 *exp2Fx = new TF1("exp2Fx", exp2.c_str(), 5e-2, xmax);

    if(etaBins_  < 3) {
      h->GetXaxis()->SetRangeUser(-0.2,0.2);
    } else {
      h->GetXaxis()->SetRangeUser(-0.5,0.5);
    }
    h->Draw();
    fxDCB->Draw("SAME");
    gausFx->SetLineColor(kBlue);
    gausFx->Draw("SAME");
    exp1Fx->SetLineColor(kYellow);
    exp1Fx->Draw("SAME");
    exp2Fx->SetLineColor(kGreen);
    exp2Fx->Draw("SAME");
    c1->Print(Form("%s_%.0f-%.0f.png",etaBins[etaBins_].c_str(),ptBinMin,ptBinMax));
    delete gausFx;
    delete fxDCB;
    delete exp1Fx;
    delete exp2Fx;
  }

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
  g->SetTitle("P Resolution [globalHighPtId]; P; Resolution");
  g->Draw("AP");
  g->GetXaxis()->SetRangeUser(0,3100);
  g->GetYaxis()->SetRangeUser(0,0.1);
  c1->Print(Form("%d_%d_PResolution.png",year,etaBins_));
  delete c1;

  return g;

}

int Stack() {

  std::vector<int> etaBins = { 0, 1, 2,/* 3, 4, 5*/ };

  TCanvas* c2 = new TCanvas("c2","c2",1500,500);
  c2->Divide(3,1);

  for(auto etaBins_: etaBins){
    std::cout << "cd etaBins+1 " << etaBins_+1 << std::endl;
    TMultiGraph *mg = new TMultiGraph();
    mg->SetTitle("P Resolution [globalHighPtId]; P; Resolution");
    TGraphAsymmErrors *g17 = GetResolutionGraph(2017,etaBins_);
    g17->Print();
    g17->SetLineColor(kRed);
    mg->Add(g17,"P");
    mg->GetXaxis()->SetRangeUser(0,3100);
    mg->GetYaxis()->SetRangeUser(0.03,0.1);
    TGraphAsymmErrors *g18 = GetResolutionGraph(2018,etaBins_);
    g18->Print();
    g18->SetLineColor(kBlack);
    mg->Add(g18,"P");
    TGraphAsymmErrors *g16 = GetResolutionGraph(2016,etaBins_);
    g16->Print();
    g16->SetLineColor(kGreen);
    mg->Add(g16,"P");
    c2->cd(etaBins_+1);
    mg->Draw("AP");
    c2->Print(Form("ResolutionMeasurement%d.png",etaBins_));
  }

  c2->Print(Form("ResolutionMeasurement.png"));

  return 0;
}
