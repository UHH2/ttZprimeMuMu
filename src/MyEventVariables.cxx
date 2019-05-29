#include "UHH2/ttZPrime/include/MyEventVariables.h"

using namespace uhh2;

STCalculator::STCalculator(Context & ctx, const boost::optional<JetId> & jetid_, const boost::optional<ElectronId> & electronid_, const boost::optional<MuonId> & muonid_, const std::string & name):
        jetid(jetid_){
    h_st = ctx.get_handle<double>(name);
    h_htlep = ctx.get_handle<double>(name);
}


bool STCalculator::process(Event & event){
    double st = 0.0;
    double htlep =0.0;
    if(event.jets){
        for(const auto & jet : *event.jets){
            if(jetid && !(*jetid)(jet, event)) continue;
	    st += jet.pt();
        }
    }
    if(event.electrons){
      for(const auto & electron : *event.electrons){
        if(electronid && !(*electronid)(electron, event)) continue;
        st += electron.pt();
        htlep += electron.pt();

      }
    }
    if(event.muons){
      for(const auto & muon : *event.muons){
        if(muonid && !(*muonid)(muon, event)) continue;
        st += muon.pt();
        htlep += muon.pt();
      }
}
    if(event.met) {
      st += event.met->pt();
      htlep += event.met->pt();
    }



    event.set(h_st, st);
    event.set( h_htlep,htlep);
    return true;
}
