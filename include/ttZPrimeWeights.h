#pragma once

#include "UHH2/core/include/AnalysisModule.h"
#include "UHH2/core/include/Event.h"

#include "TH2.h"

class MuFakeRateWeight: public uhh2::AnalysisModule {
public:
    explicit MuFakeRateWeight(std::string dirname_, unsigned int MuonAt_);

    virtual bool process(uhh2::Event & event) override;

private:
    double factor = 1.0;
    std::string dirname;
    unsigned int MuonAt;
};
