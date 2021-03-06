
#pragma once
#include "UHH2/core/include/Hists.h"
#include "UHH2/core/include/Event.h"

/** \brief Common histograms for some event-wide quantities
 *
 * Filled histograms are:
 *  - number of primary vertices
 *  - weights (note that it uses the range 0 to 2); this is filed without weights (!)
 *  - MET: missing transverse energy
 *  - HTLep: sum of MET and primary lepton pt
 *  - HT: use the "HT" double in the event
 *  - ST: HT + HTLep
 *
 * Note that the primary lepton and HT must be in the
 * event (see TTbarReconstruction.h); otherwise, the
 * histograms will be created, but not be filled.
 */
class MyEventHists: public uhh2::Hists {
public:
    MyEventHists(uhh2::Context & ctx, const std::string & dirname);

    virtual void fill(const uhh2::Event & ev) override;

protected:
    TH1F  *ST;
    TH1F  *HTLep;
    //uhh2::Event::Handle<double> h_ht;
    uhh2::Event::Handle<double> h_st;
    uhh2::Event::Handle<double> h_htlep;
    //uhh2::Event::Handle<FlavorParticle> h_primlep;
};
