#include <vector>
#include <iostream>

#include "UHH2/ttZPrime/include/MyEventHists.h"
#include "TH1F.h"

using namespace uhh2;

MyEventHists::MyEventHists(uhh2::Context & ctx, const std::string & dirname): Hists(ctx, dirname){
    ST = book<TH1F>("ST", "S_{T} [GeV]", 100, 0, 5000);
    HTLep = book <TH1F>("HTLep","H_{T,Lep} [Gev]",100,0,2000);

    std::vector<float> log_bins;
    for (float f=1e-10; f<1e+11; f*=10) {
        log_bins.push_back(1.*f);
        log_bins.push_back(2.*f);
        log_bins.push_back(5.*f);
    }

    //h_ht = ctx.get_handle<double>("HT");
    h_st = ctx.get_handle<double>("ST");
    h_htlep = ctx.get_handle<double>("HTLep");
}


void MyEventHists::fill(const uhh2::Event & e){
    assert(e.met);
    assert(e.pvs);
    //ht = e.get(h_ht);
    double st =e.get(h_st);
    double htlep = e.get(h_htlep);
    if (e.is_valid(h_st)) {
        ST->Fill(st, e.weight);
    }
    if(e.is_valid(h_htlep)){
      HTLep->Fill(htlep,e.weight);
    }
}
