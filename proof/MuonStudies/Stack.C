
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

int Stack() {

  TFile* f1 = TFile::Open("MuonStudies_ALT.root");

  std::vector<std::string> etaBins = {
    //    "HPResidualB_TT", "HPResidualO_TT", "HPResidualE_TT",
    "HPResidualB_TG", "HPResidualO_TG", "HPResidualE_TG",
    "HPResidualB_GG", "HPResidualO_GG", "HPResidualE_GG"
  };

  int etaBins_ = 3;

  std::vector<std::pair<std::string,Double_t>> samples = {
    { "ZToMuMu_NNPDF31_TuneCP5_13TeV-powheg-pythia8_M_50_120", 2.116e+03},
    { "ZToMuMu_NNPDF31_TuneCP5_13TeV-powheg-pythia8_M_120_200", 2.058e+01},
    { "ZToMuMu_NNPDF31_TuneCP5_13TeV-powheg-pythia8_M_200_400", 2.890e+00},
    { "ZToMuMu_NNPDF31_TuneCP5_13TeV-powheg-pythia8_M_400_800", 2.515e-01},
    { "ZToMuMu_NNPDF31_TuneCP5_13TeV-powheg-pythia8_M_800_1400", 1.709e-02},
    { "ZToMuMu_NNPDF31_TuneCP5_13TeV-powheg-pythia8_M_1400_2300", 1.370e-03},
    { "ZToMuMu_NNPDF31_TuneCP5_13TeV-powheg-pythia8_M_2300_3500", 8.282e-05},
    { "ZToMuMu_NNPDF31_TuneCP5_13TeV-powheg-pythia8_M_3500_4500", 3.414e-06},
    { "ZToMuMu_NNPDF31_TuneCP5_13TeV-powheg-pythia8_M_4500_6000", 3.650e-07},
    { "ZToMuMu_NNPDF31_TuneCP5_13TeV-powheg-pythia8_M_6000_Inf", 2.526e-08},
  };

  TH2D* hp = static_cast<TH2D*>(f1->Get(Form("2017/%s/%s",samples[0].first.c_str(),etaBins[etaBins_].c_str())));
  //hp->Scale(samples[0].second);
  for (int i = 1; i < samples.size(); ++i) {
    TH2D* h = static_cast<TH2D*>(f1->Get(Form("2017/%s/%s",samples[i].first.c_str(),etaBins[etaBins_].c_str())));
    //hp->Add(h,samples[i].second);
    hp->Add(h);
  }

  TCanvas* c1 = new TCanvas();
  Int_t nParams = 7;

  gStyle->SetOptFit(1);

  std::vector<Double_t> sigmas;
  std::vector<Double_t> sigmaErrors;
  std::vector<Double_t> ptBins;

  //TGraphAsymmErrors

  for(uint i = 1; i < 12; ++i){

    Double_t ptBinMin = hp->GetXaxis()->GetBinLowEdge(i);
    Double_t ptBinMax = hp->GetXaxis()->GetBinLowEdge(i+1);

    ptBins.emplace_back(hp->GetXaxis()->GetBinLowEdge(i));

    TH1D* h = hp->ProjectionY("_h",i);

    Double_t xmin = -0.2;// -2.*h->GetRMS();
    Double_t xmax = 0.2;//1.3*h->GetRMS();

    TF1 *fxDCB = new TF1("fxDCB", DSCB_ROOTForum, xmin, xmax, nParams);
    fxDCB->SetParNames ("#alpha_{low}","#alpha_{high}","n_{low}", "n_{high}", "#mu", "#sigma", "N");
    //fxDCB->SetParameters(1, 1,10, 10, h->GetMean(), h->GetRMS(), h->Integral(xmin, xmax));
    //fxDCB->SetParameters( -0.2,0.2 , 20, 20, h->GetMean(), h->GetRMS(), h->Integral(xmin, xmax));
    fxDCB->SetParameters(2, 5, 20, 20, h->GetMean(), h->GetRMS(), h->Integral(xmin, xmax));
    h->SetTitle(Form("[%.1f:%.1f] GeV %s;(1/p-1/p^{GEN})/(1/p^{GEN});Event Count",ptBinMin,ptBinMax,etaBins[etaBins_].c_str()));
    h->Fit(fxDCB,"","",xmin,xmax);
    sigmas.emplace_back(fxDCB->GetParameter(5));
    sigmaErrors.emplace_back(fxDCB->GetParError(5));

    std::string gaussian = Form("%.8f*exp(-0.5*((x - %.8f)**2)/(%.8f**2))",
                                fxDCB->GetParameter(6),fxDCB->GetParameter(4),fxDCB->GetParameter(5));

    //std::cout << gaussian << std::endl;

    std::string exp1 = Form("%.8f*pow(%.8f/%.8f,%.8f)*exp(-0.5*(%.8f**2))/pow((%.8f/%.8f)-%.8f-x,%.8f)",
                            fxDCB->GetParameter(6),
                            fxDCB->GetParameter(2),fxDCB->GetParameter(0),fxDCB->GetParameter(2),
                            fxDCB->GetParameter(0),
                            fxDCB->GetParameter(2),fxDCB->GetParameter(0),fxDCB->GetParameter(2),fxDCB->GetParameter(2));

    //std::cout << exp1 << std::endl;

    TF1 *gausFx = new TF1("gausFx", gaussian.c_str(), xmin, xmax);
    TF1 *exp1Fx = new TF1("exp1Fx", exp1.c_str(), xmin, xmax);

    h->Draw();
    fxDCB->Draw("SAME");
    gausFx->SetLineColor(kBlue);
    //gausFx->Draw("SAME");
    exp1Fx->SetLineColor(kYellow);
    //exp1Fx->Draw("SAME");
    c1->Print(Form("%s_%.0f-%.0f.png",etaBins[etaBins_].c_str(),ptBinMin,ptBinMax));
    delete gausFx;
    delete fxDCB;
    delete exp1Fx;
    //break;
  }

  //TGraphAsymmErrors* g = new TGraphAsymmErrors(12);

  for(int i = 0; i < 12; ++i){
    std::cout << ptBins[i] << "\t" << ptBins[i+1] << "\t" << sigmas[i] << std::endl;
  }

  return 0;
}
