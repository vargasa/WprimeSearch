struct Electrons{
  TTreeReaderValue<UInt_t> &n;
  TTreeReaderArray<Int_t> &cutBased;
  TTreeReaderArray<Float_t> &pt;
  TTreeReaderArray<Float_t> &eta;
  TTreeReaderArray<Float_t> &phi;
  TTreeReaderArray<Int_t> &charge;
  TTreeReaderArray<Float_t> &miniPFRelIso_all;
};
