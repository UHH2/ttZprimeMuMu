#include "UHH2/ttZPrime/include/ttZPrimeSelections.h"
#include "UHH2/core/include/Event.h"
#include "UHH2/common/include/Utils.h"
#include "UHH2/core/include/LorentzVector.h"

#include <stdexcept>
#include <vector>

using namespace uhh2examples;
using namespace uhh2;


DijetSelection::DijetSelection(float dphi_min_, float third_frac_max_): dphi_min(dphi_min_), third_frac_max(third_frac_max_){}

bool DijetSelection::passes(const Event & event){
    assert(event.jets); // if this fails, it probably means jets are not read in
    if(event.jets->size() < 2) return false;
    const auto & jet0 = event.jets->at(0);
    const auto & jet1 = event.jets->at(1);
    auto dphi = deltaPhi(jet0, jet1);
    if(dphi < dphi_min) return false;
    if(event.jets->size() == 2) return true;
    const auto & jet2 = event.jets->at(2);
    auto third_jet_frac = jet2.pt() / (0.5 * (jet0.pt() + jet1.pt()));
    return third_jet_frac < third_frac_max;
}
STSelection::STSelection(double st_min):
  m_st_min(st_min) {}


bool STSelection::passes(const Event & event) {
  double st = 0.0;
  if(event.jets){
      for(const auto & jet : *event.jets){
          st += jet.pt();
      }
  }
  if(event.electrons){
    for(const auto & electron : *event.electrons){
      st += electron.pt();

    }
  }
  if(event.muons){
    for(const auto & muon : *event.muons){
      st += muon.pt();
    }
}
  if(event.met) {
    st += event.met->pt();
  }


  return st > m_st_min;
}

MMuMUSelection::MMuMUSelection(double mmumu_min):
  m_mmumu_min(mmumu_min) {}

bool MMuMUSelection::passes(const Event & event){
  assert(event.muons);
  std::vector<Muon> muons = *event.muons;
  if((event.muons->size() > 1) && ((muons[0].charge()+muons[1].charge())== 0)){
    double m = (muons[0].v4() + muons[1].v4()).M();
    return m > m_mmumu_min;
  }
  else{
    return false;
  }
}
