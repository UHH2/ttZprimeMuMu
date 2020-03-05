#pragma once

#include "UHH2/core/include/Hists.h"
#include "UHH2/core/include/Event.h"

#include "TH1F.h"
#include <iostream>

class ttZPrimeControl3LHists: public uhh2::Hists{
public:
  ttZPrimeControl3LHists(uhh2::Context & ctx, const std::string & dirname);
  virtual void fill(const uhh2::Event & e) override;
protected:
  TH1F *drminmugenreco, *mmumureal, *mmumufake, *mmumumix, *nevtFake ,*nevtMix ,*nevtReal, *NFakeMuons;
  TH1F *MuFakePhi, *MuFakePt, *MuFakeEta;
};
