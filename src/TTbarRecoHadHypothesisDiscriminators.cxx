#include "UHH2/ttZPrime/include/TTbarRecoHadHypothesisDiscriminators.h"
#include "UHH2/core/include/Utils.h"

#include <set>

using namespace uhh2;
using namespace std;

namespace {

  // invariant mass of a lorentzVector, but safe for timelike / spacelike vectors
  float inv_mass(const LorentzVector& p4){ return p4.isTimelike() ? p4.mass() : -sqrt(-p4.mass2()); }

}

const TTbarRecoHadHypothesis * get_best_hypothesis(const std::vector<TTbarRecoHadHypothesis> & hyps, const std::string & label){

    const TTbarRecoHadHypothesis * best = nullptr;
    float current_best_disc = numeric_limits<float>::infinity();
    for(const auto & hyp : hyps){
        if(!hyp.has_discriminator(label)) continue;
        auto disc = hyp.discriminator(label);
        if(disc < current_best_disc){
            best = &hyp;
            current_best_disc = disc;
        }
    }
    if(std::isfinite(current_best_disc)){
        return best;
    }
    else{
        return nullptr;
    }
}
////

Chi2DiscriminatorHad::Chi2DiscriminatorHad(Context & ctx, const std::string & rechyps_name, const cfg & config_): config(config_){

  h_hyps = ctx.get_handle<vector<TTbarRecoHadHypothesis>>(rechyps_name);

  Mthad_mean_  = 170.;
  Mthad_sigma_ =  25.;
  Mw_mean = 84.;
  Mw_sigma = 16.;
  DMthad_mean_ = 0.;
  DMthad_sigma_ = 55.;
}

bool Chi2DiscriminatorHad::process(uhh2::Event& event){

  auto& hyps = event.get(h_hyps);

  for(auto& hyp : hyps){
    // auto& wjets1 = tophad1_jets();
    // auto& wjets2 = tophad2_jets();
    // if(wjets1.size() > 0)
    // {
    //   chi2_w1 = pow((wjets1.v4().M() - Mw_mean) / Mw_sigma, 2);
    //   hyp.set_w1_v4((wjets1.v4().M());
    //   LorentzVector wmass;
    //   for(auto & jet : wjets1)
    //  {
    //    wmass1 += jet.v4();
    //    float chi2 = pow((wmass1.M() - Mw_mean) / Mw_sigma, 2);
    //       if(chi2 < chi2_w1)
    //      {
    //         chi2_w1 = chi2;
    //         hyp.set_w1_v4(wmass1);
    //      }
    //    }
    //  }
    // }



    // float chi2_w2 = numeric_limits<float>::infinity();
    // int w1_njets = 0;
    // int w2_njets = 0;
    // LorentzVector wmass1;
    // LorentzVector wmass2;
    // for(auto & jet : wjets1)
    // {
    //   wmass1 += jet.v4();
    //   w1_njets ++;
    //   if( w1_njets > 1)
    //   {
    //     float chi2 = pow((wmass1.M() - Mw_mean) / Mw_sigma, 2);
    //     if(chi2 < chi2_w1)
    //     {
    //        chi2_w1 = chi2;
    //        hyp.set_w1_v4(wmass1);
    //     }
    //   }
    // }
    // for(auto & jet : wjets2)
    // {
    //   wmass2 += jet.v4();
    //   w2_njets ++;
    //   if(w2_njets > 1 )
    //   {
    //     float chi2 = pow((wmass2.M() - Mw_mean) / Mw_sigma, 2);
    //     if(chi2 < chi2_w2)
    //     {
    //       chi2_w2 = chi2;
    //       hyp.set_w2_v4(wmass2);
    //     }
    //   }
    // }
    const float Mthad_reco_1 = inv_mass(hyp.tophad1_v4());
    const float Mthad_reco_2 = inv_mass(hyp.tophad2_v4());

    const double chi2_thad_1 = pow((Mthad_reco_1 - Mthad_mean_) / Mthad_sigma_, 2);
    const double chi2_thad_2 = pow((Mthad_reco_2 - Mthad_mean_) / Mthad_sigma_, 2);
    hyp.set_discriminator(config.discriminator_label+"_thad1", chi2_thad_1);
    hyp.set_discriminator(config.discriminator_label+"_thad2", chi2_thad_2);
    // hyp.set_discriminator(config.discriminator_label+"_whad1", chi2_w1);
    // hyp.set_discriminator(config.discriminator_label+"_whad2", chi2_w2);
    hyp.set_discriminator(config.discriminator_label         , chi2_thad_1 + chi2_thad_2);

  }

  return true;
}
////

TopDRMCDiscriminatorHad::TopDRMCDiscriminatorHad(Context & ctx, const std::string & rechyps_name, const cfg & config_): config(config_){
    h_hyps = ctx.get_handle<vector<TTbarRecoHadHypothesis>>(rechyps_name);
    h_ttbargen = ctx.get_handle<TTbarGen>(config.ttbargen_name);
}


bool TopDRMCDiscriminatorHad::process(uhh2::Event & event){
    auto & hyps = event.get(h_hyps);
    const auto & ttbargen = event.get(h_ttbargen);
    for(auto & hyp: hyps){
        auto& wjets1 = hyp.tophad1_wjets();
        auto& wjets2 = hyp.tophad2_wjets();
        if(wjets1.size() > 0 && wjets2.size() > 0)
        {
          LorentzVector wmass1;
          LorentzVector wmass2;
          int w1_njets = 0;
          int w2_njets = 0;
          double deltaRW1 = deltaR(ttbargen.WTop().v4(),hyp.tophad1_wjets().at(0).v4());
          double deltaRW2 = deltaR(ttbargen.WTop().v4(),hyp.tophad2_wjets().at(0).v4());
          for(auto & jet : wjets1)
          {
            wmass1 += jet.v4();
            w1_njets ++;
            if(deltaRW1 < deltaR(wmass1,ttbargen.WTop().v4()) && w1_njets > 0)
            {
              deltaRW1 = deltaR(wmass1,ttbargen.WTop().v4());
              hyp.set_w1_v4(wmass1);
            }
            else if(deltaRW1 < deltaR(wmass1,ttbargen.WAntitop().v4()) && w2_njets > 0)
            {
              deltaRW1 = deltaR(wmass1,ttbargen.WAntitop().v4());
              hyp.set_w1_v4(wmass1);
            }
          }
          for(auto & jet : wjets2)
          {
            wmass2 += jet.v4();
            w2_njets ++;
            if(deltaRW2 < deltaR(wmass2,ttbargen.WTop().v4()) && w2_njets > 0)
            {
              deltaRW2 = deltaR(wmass2,ttbargen.WTop().v4());
              hyp.set_w2_v4(wmass2);
            }
            else if(deltaRW2 < deltaR(wmass2,ttbargen.WAntitop().v4()) && w2_njets > 0)
            {
              deltaRW2 = deltaR(wmass2,ttbargen.WAntitop().v4());
              hyp.set_w2_v4(wmass2);
            }
          }
        }
        auto deltar_sum1 = deltaR(ttbargen.Top().v4(), hyp.tophad1_v4()) + deltaR(ttbargen.Antitop().v4(), hyp.tophad2_v4());
        auto deltar_sum2 = deltaR(ttbargen.Top().v4(), hyp.tophad2_v4()) + deltaR(ttbargen.Antitop().v4(), hyp.tophad1_v4());
        double deltar_sum = deltar_sum1;
        if(deltar_sum2 < deltar_sum1) deltar_sum = deltar_sum2;
        hyp.set_discriminator(config.discriminator_label, deltar_sum);
    }
    return true;
}


/*
CorrectMatchDiscriminator::CorrectMatchDiscriminator(Context & ctx, const std::string & rechyps_name, const cfg & config_): config(config_){
    h_hyps = ctx.get_handle<vector<TTbarRecoHadHypothesis>>(rechyps_name);
    h_ttbargen = ctx.get_handle<TTbarGen>(config.ttbargen_name);
}

namespace {

// match particle p to one of the jets (Delta R < 0.3); return the deltaR
// of the match.
template<typename T> // T should inherit from Particle
float match_dr(const Particle & p, const std::vector<T> & jets, int& index){
  float mindr = infinity;
  index = -1;
  for(unsigned int i=0; i<jets.size(); ++i){
    float dR = deltaR(p, jets.at(i));
    if( dR <0.3 && dR<mindr) {
      mindr=dR;
      index=i;
    }
  }
  return mindr;
}

}

bool CorrectMatchDiscriminator::process(uhh2::Event & event){
    auto & hyps = event.get(h_hyps);
    const auto & ttbargen = event.get(h_ttbargen);
    auto dec = ttbargen.DecayChannel();
    if(dec != TTbarGen::e_ehad && dec != TTbarGen::e_muhad){
        for(auto & hyp: hyps){
            hyp.set_discriminator(config.discriminator_label, infinity);
        }
        return true;
    }

    // note that it is allowed that two partons from the hadronic ttbar decay match the same jet.
    for(auto & hyp: hyps){
        auto hadr_jets = hyp.tophad_jets();
        auto lept_jets = hyp.toplep_jets();

        if(lept_jets.size() != 1){
            hyp.set_discriminator(config.discriminator_label, infinity);
            continue;
        }
        if(hadr_jets.size() > 3){ // < 3 is allowed ...
            hyp.set_discriminator(config.discriminator_label, infinity);
            continue;
        }

        //index lists of jets that can be matched to partons
        std::set<int> matched_hadr_jets;

        // match b jets
        int index_l, index_h;
        float correct_dr = match_dr(ttbargen.BLep(), lept_jets, index_l) + match_dr(ttbargen.BHad(), hadr_jets, index_h);
        if(index_h >= 0) matched_hadr_jets.insert(index_h);
        //match quarks from W decays
        correct_dr += match_dr(ttbargen.Q1(), hadr_jets, index_h);
        if(index_h >= 0) matched_hadr_jets.insert(index_h);
        correct_dr += match_dr(ttbargen.Q2(), hadr_jets, index_h);
        if(index_h >= 0) matched_hadr_jets.insert(index_h);

        // if not all jets of the hadronic side of the reconstruction could be matched: infinite
        // value:
        if(matched_hadr_jets.size() != hadr_jets.size()){
            hyp.set_discriminator(config.discriminator_label, infinity);
            continue;
        }

        //add deltaR between reconstructed and true neutrino
        correct_dr += deltaR(ttbargen.Neutrino(), hyp.neutrino_v4());
        hyp.set_discriminator(config.discriminator_label, correct_dr);
    }
    return true;

} */
