struct Muons{
  TTreeReaderValue<UInt_t> &n;
  TTreeReaderArray<Bool_t> &looseId;
  TTreeReaderArray<Float_t> &pt;
  TTreeReaderArray<Float_t> &eta;
  TTreeReaderArray<Float_t> &phi;
  TTreeReaderArray<Bool_t> &isGlobal;
  TTreeReaderArray<Float_t> &dxy;
  TTreeReaderArray<Float_t> &dz;
};
