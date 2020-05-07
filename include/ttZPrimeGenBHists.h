#pragma once

#include "UHH2/core/include/Hists.h"
#include "UHH2/core/include/Event.h"

#include "TH1F.h"
#include <iostream>

class ttZPrimeGenBHists: public uhh2::Hists{
public:
  ttZPrimeGenBHists(uhh2::Context & ctx, const std::string & dirname);
  virtual void fill(const uhh2::Event & e) override;
protected:
  TH1F *BQuarksGenPt, *BQuarksGenPhi, *BQuarksGenEta, *NBQuarksGen;
};
