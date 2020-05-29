#pragma once
#include "UHH2/core/include/Hists.h"
#include "UHH2/core/include/Event.h"

#include <vector>
#include <string>

class ThirdMuonHists: public uhh2::Hists {
public:
  ThirdMuonHists(uhh2::Context & ctx, const std::string & dirname);

  virtual void fill(const uhh2::Event & ev) override;

protected:
     TH1F *pt_3, *eta_3, *phi_3, *isolation_3, *charge_3, *M_mu2mu3;
     TH2F *ThirdMuIsoVsEta;
};
