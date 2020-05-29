#include "UHH2/core/include/Event.h"
#include "UHH2/common/include/Utils.h"
#include "UHH2/core/include/LorentzVector.h"

#include "UHH2/ttZPrime/include/ttZPrimeSelections.h"
#include "UHH2/ttZPrime/include/TTbarRecoHadHypothesisDiscriminators.h"

#include <iostream>
#include <stdexcept>
#include <vector>

using namespace uhh2examples;
using namespace uhh2;

METSelection::METSelection(double met_max_) {
  met_max = met_max_;
}

bool METSelection::passes(const Event &event) {
  return (event.met->pt() < met_max);
}



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

DRMuJetSelection::DRMuJetSelection(double deltaR_min):
m_deltaR_min(deltaR_min) {}
bool DRMuJetSelection::passes(const Event & event){
  assert(event.muons);
  assert(event.jets);
  std::vector<Jet> jets = *event.jets;
  std::vector<Muon> muons = *event.muons;
  for(auto& muon : muons)
  {
    for(auto & jet : jets)
    {
      auto dr = deltaR(jet,muon);
      if(dr < m_deltaR_min) return false;
    }
  }
  return true;
}

MMuMUSelection::MMuMUSelection(double mmumu_min):
  m_mmumu_min(mmumu_min) {}

bool MMuMUSelection::passes(const Event & event){
  assert(event.muons);
  if (2 > event.muons->size()) throw std::runtime_error("Insufficient muons: Two muons are required but "+std::to_string(event.muons->size())+" are available" );
  std::vector<Muon> muons = *event.muons;
  double m = (muons[0].v4() + muons[1].v4()).M();
  return m > m_mmumu_min;
}

MMuMuMaxSelection::MMuMuMaxSelection(double mmumu_max):
  m_mmumu_max(mmumu_max) {}

bool MMuMuMaxSelection::passes(const Event & event){
  assert(event.muons);
  if (2 > event.muons->size()) throw std::runtime_error("Insufficient muons: Two muons are required but "+std::to_string(event.muons->size())+" are available" );
  std::vector<Muon> muons = *event.muons;
  double m = (muons[0].v4() + muons[1].v4()).M();
  return m < m_mmumu_max;
}


MEleMuSelection::MEleMuSelection(double melemu_min):
  m_melemu_min(melemu_min) {}

bool MEleMuSelection::passes(const Event & event){
  assert(event.muons);
  assert(event.electrons);
  std::vector<Muon> muons = *event.muons;
  std::vector<Electron> electrons = *event.electrons;
  double m = (muons.at(0).v4() + electrons.at(0).v4()).M();
  return m > m_melemu_min;
}


NMuIDSelection::NMuIDSelection(const boost::optional<MuonId> & muid_, unsigned int NMu_):
muid(muid_),
NMu(NMu_){}

bool NMuIDSelection::passes(const uhh2::Event & event){
  bool rightID = true;
  if (NMu > event.muons->size()) throw std::runtime_error("Insufficient muons: "+std::to_string(NMu)+" muons are required but "+std::to_string(event.muons->size())+" are available" );
    for (unsigned int k = 0; k < NMu; k++){
    rightID &= (*muid)(event.muons->at(k), event);
  }
  return (rightID);
}



TopDRMCHadSelection::TopDRMCHadSelection(uhh2::Context &ctx, double dr_max, const std::string & hyps_name, const std::string & discriminator_name):
  m_dr_max(dr_max),
  h_hyps(ctx.get_handle<std::vector<TTbarRecoHadHypothesis>>(hyps_name)),
  m_discriminator_name(discriminator_name) {}
bool TopDRMCHadSelection::passes(const Event &event) {
  const TTbarRecoHadHypothesis* hyp = get_best_hypothesis(event.get(h_hyps), m_discriminator_name );
  if(hyp)
  {
    double dr = hyp->discriminator(m_discriminator_name);
    return dr < m_dr_max;
  }
  else
  {
    return false;
  }
}

JetQuarkMatchingSelection::JetQuarkMatchingSelection(uhh2::Context & ctx, int NMatchs,const std::string & hyps_name, const std::string & discriminator_name):
  m_nmatchs(NMatchs),
  h_ttbargen(ctx.get_handle<TTbarGen>("ttbargen")),
  h_hyps (ctx.get_handle<std::vector<TTbarRecoHadHypothesis>>(hyps_name)),
  m_discriminator_name (discriminator_name){}
bool JetQuarkMatchingSelection::passes(const Event & event){
  hyps = event.get(h_hyps);
  hyp = get_best_hypothesis( hyps, m_discriminator_name );
  if(!hyp) return false;
  jets.insert(jets.end(),hyp->tophad1_jets().begin(), hyp->tophad1_jets().end());
  jets.insert(jets.end(),hyp->tophad2_jets().begin(), hyp->tophad2_jets().end());
  jets.insert(jets.end(),hyp->tophad1_bjet());
  jets.insert(jets.end(),hyp->tophad2_bjet());

  // auto  jets = event.jets;
  const auto & ttbargen = event.get(h_ttbargen);

  GenP.push_back(ttbargen.bTop());
  GenP.push_back(ttbargen.bAntitop());
  GenP.push_back(ttbargen.Wdecay1());
  GenP.push_back(ttbargen.Wdecay2());
  GenP.push_back(ttbargen.WMinusdecay1());
  GenP.push_back(ttbargen.WMinusdecay2());

  for(unsigned int k = 0; k<GenP.size(); k++){
    int n = 0;
    for(unsigned int i = 0; i< jets.size(); i++)
    {
        double dr = uhh2::deltaR(GenP.at(k), jets.at(i));
        if (dr < 0.4) n++;

    }
    if(n != m_nmatchs) return false;
  }
  return true;
}
