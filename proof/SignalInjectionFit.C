
//std::string label = "OnlyGT36BG10X"

std::map<int,std::vector<float>> rMap = { 
    {600,  {0.0, 0.0251799, 0.0346679, 0.0493158}},
    {1000, {0.0, 0.0597894, 0.0854492, 0.1229156}},
    {1400, {0.0, 0.1413164, 0.2041015, 0.2976606}},
};

std::vector<std::string> rLabels = {"r = 0.", "lower edge 68% band", "expected limit", "upper edge 68% band"};

gStyle->SetOptFit(0111);
gStyle->SetOptStat(0);


for(const auto&[ mass, rVec ] : rMap){

    int i = 0;
    for(const auto& expectSignal: rVec){
        double r, rErr, rHiErr, rLoErr; 
        int fit_status;

        TFile *f = new TFile(Form("fitDiagnosticsRunII_%d_%.7f.root", mass, expectSignal));
        TTree* tree_fit_sb=(TTree*)(f->Get("tree_fit_sb")); 
        TH1F *h_bias = new TH1F("h_bias","h_bias",12*2,-10,10);
        tree_fit_sb->SetBranchAddress("r",&r); 
        tree_fit_sb->SetBranchAddress("rErr",&rErr); 
        tree_fit_sb->SetBranchAddress("rHiErr",&rHiErr); 
        tree_fit_sb->SetBranchAddress("rLoErr",&rLoErr); 
        tree_fit_sb->SetBranchAddress("fit_status",&fit_status);

        double biasMean = h_bias->GetMean();
        double biasRms = h_bias->GetRMS();

        for (int i=0; i<tree_fit_sb->GetEntries(); i++) {
            tree_fit_sb->GetEntry(i);
            if(fit_status>-1){
                if(r > expectSignal) {
                    h_bias->Fill((r-expectSignal)/rLoErr);
                } else {
                    h_bias->Fill((r-expectSignal)/rHiErr);
                }
            }
        }

        TCanvas* c1 = new TCanvas("c1","c1",800,800);

        TF1* gaussFit = new TF1("gaussFit", "[0]*exp(-0.5*((x-[1])/[2])^2)", -2.,2.);
        gaussFit->SetParameter(0, h_bias->GetMaximum());
        gaussFit->SetParName(0, "Fit_N");
        gaussFit->SetParName(1, "Fit_#mu");
        gaussFit->SetParName(2, "Fit_#sigma");
        //gaussFit->SetParameter(1, h_bias->GetMean());
        //gaussFit->SetParameter(2, h_bias->GetRMS());

        h_bias->Fit(gaussFit, "MWL");
        h_bias->SetMarkerStyle(20);
        h_bias->SetMarkerSize(1);
        h_bias->SetMarkerColor(kBlack);
        h_bias->SetTitle(Form("Signal Injection Test WZ_{Mass} = %d GeV Run 2 - Combined Channels; (r-r_{inj})/r_{Err} [%s]; Toy Datasets r = %.3f (%s)", mass, label.c_str(), expectSignal, rLabels[i].c_str()));
        h_bias->Draw("E");
        c1->Update();
        h_bias->GetYaxis()->SetRangeUser(0, h_bias->GetMaximum()*1.5);
        TPaveStats *ps = (TPaveStats*)c1->GetPrimitive("stats");
        ps->SetName("Fit Results");
        TList *listOfLines = ps->GetListOfLines();

        TLatex *myt = new TLatex(0,0,Form("r = %.7f", expectSignal));
        myt ->SetTextFont(42);
        myt ->SetTextSize(0.04);
        myt ->SetTextColor(kRed);
        listOfLines->Add(myt);
        h_bias->Draw("E");
        c1->Update();

        c1->SaveAs(Form("bias_%d_%d_%s.pdf", mass, i, label.c_str()));
        ++i;
    }
}