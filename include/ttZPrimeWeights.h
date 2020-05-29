#pragma once

#include "UHH2/core/include/AnalysisModule.h"
#include "UHH2/core/include/Event.h"

#include "TH2.h"

class MuFakeRateWeight: public uhh2::AnalysisModule {
public:
    explicit MuFakeRateWeight(uhh2::Context & ctx, std::string dirname_, unsigned int MuonAt_,const std::string & sys_uncert="nominal");

    virtual bool process(uhh2::Event & event) override;

private:
    double factor = 0.0, factor_up = 0.0, factor_down = 0.0;
    std::string dirname;
    unsigned int MuonAt;
    int sys_direction_;
    uhh2::Event::Handle<float> h_muon_weight_;
    uhh2::Event::Handle<float> h_muon_weight_up_;
    uhh2::Event::Handle<float> h_muon_weight_down_;
};

class MuFakeRateWeight1D: public uhh2::AnalysisModule {
public:
    explicit MuFakeRateWeight1D(std::string dirname_, unsigned int MuonAt_);

    virtual bool process(uhh2::Event & event) override;

private:
    double factor = 0.0;
    std::string dirname;
    unsigned int MuonAt;

};
