{

  std::vector<Int_t> masses = {600, 800, 1000, 1200, 1400, 1600,
                               1800, 2000, 2500, 3000, 3500, 4000};

  const int year = 0;
  const uint n = 12;

  std::unordered_map<int, float> luminosity = {
    {2016, 36.33},
    {2017, 41.53},
    {2018, 59.74},
    {0, 137.60}
  };

  std::vector<std::pair<int,double>> modelA = {
    {600,4.170265262},
    {800,1.258355576},
    {1000,0.4923069193},
    {1200,0.223867993},
    {1400,0.1123265839},
    {1600,0.06027153041},
    {1800,0.03393427094},
    {2000,0.01979120707},
    {2500,0.005701670225},
    {3000,0.001787435636},
    {3500,0.0005839192986},
    {4000,0.0001929943997},
    {4500,0.00006365368358}
  };

  TGraph* gModelA = new TGraph();
  gModelA->SetLineColor(kRed);
  gModelA->SetLineStyle(7);

  int counter = 0;
  for (auto pair: modelA) {
    gModelA->SetPoint(counter,pair.first,pair.second);
    ++counter;
  }

  std::vector<std::pair<int,double>> modelB = {
    {800,0.6807755239},
    {1000,0.4541593453},
    {1200,0.2503529114},
    {1400,0.1389695229},
    {1600,0.07926705884},
    {1800,0.04645925411},
    {2000,0.0278648144},
    {2500,0.008370229002},
    {3000,0.002682794712},
    {3500,0.0008880528633},
    {4000,0.0003019010608},
    {4500,0.00009917207371}
  };


  TGraph* gModelB = new TGraph();
  gModelB->SetLineColor(kBlue);
  gModelB->SetLineStyle(5);

  counter = 0;
  for (auto pair: modelB) {
    gModelB->SetPoint(counter,pair.first,pair.second);
    ++counter;
  }

  TGraph* gObs = new TGraph();

  Double_t x[n];
  Double_t yGreenLow[n];
  Double_t yGreenHigh[n];
  Double_t yCentral[n];
  Double_t yYellowLow[n];
  Double_t yYellowHigh[n];

  counter = 0;
  for (const auto& m: masses) {

    std::string fileName =  Form("CombineFiles/Combine_%d/higgsCombine.%d.AsymptoticLimits.mH125.root",year,m);

    TFile* f = TFile::Open(fileName.c_str());
    auto t = static_cast<TTree*>(f->Get("limit"));

    TTreeReader fReader("limit", f);
    TTreeReaderValue<Double_t> limit = { fReader, "limit" };
    x[counter] = m;
    std::cout << Form("M: %d\n", m);

    fReader.Next(); // 0 2.5%
    yYellowLow[counter] = (*limit) * (modelA[counter].second);
    std::cout << "YellowLow: " << yYellowLow[counter] << std::endl;

    fReader.Next(); // 1 16.0%
    yGreenLow[counter] = (*limit) * modelA[counter].second;
    std::cout << "GreenLow: " << yGreenLow[counter] << std::endl;

    fReader.Next(); // 2 50.0% Expected
    yCentral[counter] = (*limit) * modelA[counter].second;
    std::cout << "Central: " << yCentral[counter] << std::endl;

    fReader.Next(); // 3 84.0%
    yGreenHigh[counter] = (*limit) * modelA[counter].second;
    std::cout << "GreenHigh: " << yGreenHigh[counter] << std::endl;

    fReader.Next(); // 4 97.5%
    yYellowHigh[counter] = (*limit) * modelA[counter].second;
    std::cout << "YellowHigh: " << yYellowHigh[counter]  << std::endl;

    fReader.Next(); // 5 Observed
    gObs->SetPoint(counter, m, (*limit) * modelA[counter].second);
    ++counter;
    std::cout << "\n\n\n" ;
  }

  for(int i = 0; i < n; ++i){
    yGreenLow[i] = yCentral[i] - yGreenLow[i];
    yGreenHigh[i] -= yCentral[i];
    yYellowLow[i] = yCentral[i] - yYellowLow[i];
    yYellowHigh[i] -= yCentral[i];
  }

  auto gGreen = new TGraphAsymmErrors(n,x,yCentral,0,0,yGreenLow,yGreenHigh);
  gGreen->SetFillColor(kGreen);
  gGreen->SetLineColor(kBlack);
  gGreen->SetFillStyle(1504);

  auto gYellow = new TGraphAsymmErrors(n,x,yCentral,0,0,yYellowLow,yYellowHigh);
  gYellow->SetFillColor(kYellow);
  gYellow->SetFillStyle(1504);

  auto gCentral = new TGraph(n,x,yCentral);
  gCentral->SetLineColor(kBlack);
  gCentral->SetLineStyle(7);

  auto legend = new TLegend(0.6,0.6,0.89,0.88);
  //auto legend = new TLegend(0.6,0.15,0.9,0.38);
  legend->SetBorderSize(0);
  legend->AddEntry(gCentral,"Expected 95% upper limit");
  legend->AddEntry(gGreen, "Expected Limit (#pm1#sigma)");
  legend->AddEntry(gYellow, "Expected Limit (#pm2#sigma)");
  legend->AddEntry(gModelA, "HVT Model A");
  legend->AddEntry(gModelB, "HVT Model B");

  TCanvas* c1 = new TCanvas("c1","Brazilian Band Plot");
  auto mg  = new TMultiGraph();
  mg->SetTitle(Form("95%% CL Upper Limits [Run%d]  L = %.2f fb^{-1}; m_{WZ} (GeV);#sigma(W')#timesBr(W'#rightarrowWZ)(pb))",year,luminosity[year]));
  //mg->SetTitle(Form("95%% CL Upper Limits [Run%d]  L = %.2f fb^{-1}; m_{WZ} (GeV);#sigma_{95%%CL}/#sigma_{Signal}",year,luminosity[year]));
  c1->SetLogy();
  mg->Add(gYellow,"A3");
  mg->Add(gGreen,"A3");
  mg->Add(gCentral,"L");
  mg->Add(gModelA,"L");
  mg->Add(gModelB,"L");
  mg->GetYaxis()->SetRangeUser(1e-4,50.);
  mg->GetXaxis()->SetRangeUser(0.,7000.);

  gObs->SetMarkerColor(kBlack);
  gObs->SetMarkerStyle(20);

  mg->Draw("A");
  legend->Draw();

  //gObs->Draw("SAME PC");

  c1->Print(Form("ExclusionLimits_%d.pdf",year));

}
