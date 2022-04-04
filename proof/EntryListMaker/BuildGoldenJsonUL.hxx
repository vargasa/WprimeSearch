#include "../IsData.h"

void EventIDMaker::BuildGoldenJson(){
  std::vector<std::pair<UInt_t,UInt_t>> tvv1;
#ifdef Y2016
  std::clog << "Building 2016 UL Golden Map\n";
#include "files/UL_Legacy2016.C"
#elif defined(Y2017)
  std::clog << "Building 2017 UL Golden Map\n";
#include "files/UL_UL2017.C"
#elif defined(Y2018)
  std::clog << "Building 2018 UL Golden Map\n";
#include "files/UL_Legacy2018.C"
#endif
}
