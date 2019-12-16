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

  Mthad_mean_  = 174.;
  Mthad_sigma_ =  20.;
  Mw_mean_ = 84.;
  Mw_sigma_ = 13.;

}

bool Chi2DiscriminatorHad::process(uhh2::Event& event){

  auto& hyps = event.get(h_hyps);

  for(auto& hyp : hyps){
    float chi2_w1 = numeric_limits<float>::infinity();
    float chi2_w2 = numeric_limits<float>::infinity();
    auto& wjets1 = hyp.tophad1_jets();
    auto& wjets2 = hyp.tophad2_jets();
    LorentzVector wmass1, wmass2;
    for(auto & wjet : wjets1)
    {
      wmass1 += wjet.v4();
      float chi2 = pow((wmass1.M() - Mw_mean_) / Mw_sigma_, 2);
      if(chi2 < chi2_w1)
      {
          chi2_w1 = chi2;
          hyp.set_w1_v4(wmass1);
      }
    }
    for(auto & wjet : wjets2)
    {
      wmass2 += wjet.v4();
      float chi2 = pow((wmass2.M() - Mw_mean_) / Mw_sigma_, 2);
      if(chi2 < chi2_w1)
      {
          chi2_w2 = chi2;
          hyp.set_w2_v4(wmass2);
      }
    }

    const float Mthad_reco_1 = inv_mass(hyp.tophad1_v4());
    const float Mthad_reco_2 = inv_mass(hyp.tophad2_v4());

    const double chi2_thad_1 = pow((Mthad_reco_1 - Mthad_mean_) / Mthad_sigma_, 2);
    const double chi2_thad_2 = pow((Mthad_reco_2 - Mthad_mean_) / Mthad_sigma_, 2);
    hyp.set_chi2_tt(chi2_thad_1 + chi2_thad_2);
    hyp.set_discriminator(config.discriminator_label+"_thad1", chi2_thad_1);
    hyp.set_discriminator(config.discriminator_label+"_thad2", chi2_thad_2);

    hyp.set_discriminator(config.discriminator_label+"_whad2", chi2_w2);
    hyp.set_discriminator(config.discriminator_label         , chi2_thad_1 + chi2_thad_2+chi2_w1+chi2_w2);

  }

  return true;
}

Chi2Discriminator4Jets::Chi2Discriminator4Jets(Context & ctx, const std::string & rechyps_name, const cfg & config_): config(config_){

  h_hyps = ctx.get_handle<vector<TTbarRecoHadHypothesis>>(rechyps_name);

  Mthad_mean_  = 120.;
  Mthad_sigma_ =  35.;

}

bool Chi2Discriminator4Jets::process(uhh2::Event& event){

  auto& hyps = event.get(h_hyps);

  for(auto& hyp : hyps){
    const float Mthad_reco_1 = inv_mass(hyp.tophad1_v4());
    const float Mthad_reco_2 = inv_mass(hyp.tophad2_v4());
    const double chi2_thad_1 = pow((Mthad_reco_1 - Mthad_mean_) / Mthad_sigma_, 2);
    const double chi2_thad_2 = pow((Mthad_reco_2 - Mthad_mean_) / Mthad_sigma_, 2);
    hyp.set_chi2_tt(chi2_thad_1 + chi2_thad_2);
    hyp.set_discriminator(config.discriminator_label+"_thad1", chi2_thad_1);
    hyp.set_discriminator(config.discriminator_label+"_thad2", chi2_thad_2);
    hyp.set_discriminator(config.discriminator_label         , chi2_thad_1 + chi2_thad_2);

  }

  return true;
}



Chi2Discriminator5Jets::Chi2Discriminator5Jets(Context & ctx, const std::string & rechyps_name, const cfg & config_): config(config_){

  h_hyps = ctx.get_handle<vector<TTbarRecoHadHypothesis>>(rechyps_name);

  Mthad_mean_2jets_  = 120.;
  Mthad_sigma_2jets_ =  35.;
  Mthad_mean_3jets_  = 174.;
  Mthad_sigma_3jets_ =  20.;
  Mw_mean_3jets_ = 84.;
  Mw_sigma_3jets_ = 13.;
}

bool Chi2Discriminator5Jets::process(uhh2::Event& event){
  if(event.jets->size() != 5) return true;
  auto& hyps = event.get(h_hyps);

  for(auto& hyp : hyps){
    auto NJets1 = hyp.tophad1_jets().size() + hyp.tophad1_bjet().size();
    auto NJets2 = hyp.tophad2_jets().size() + hyp.tophad2_bjet().size();
    if(NJets1+NJets2 != 5)
    {
      hyp.set_discriminator(config.discriminator_label,  infinity);
      continue;
    }

    double  chi2_thad_1 = numeric_limits<float>::infinity();
    double  chi2_thad_2 = numeric_limits<float>::infinity();
    if(NJets1 == 2)
    {
      const float Mthad_reco_1 = inv_mass(hyp.tophad1_v4());
      chi2_thad_1 = pow((Mthad_reco_1 - Mthad_mean_2jets_) / Mthad_sigma_2jets_, 2);

    }
    else if (NJets1 == 3)
    {
        float chi2_w1 = numeric_limits<float>::infinity();
        auto& wjets1 = hyp.tophad1_jets();
        LorentzVector wmass1;
        for(auto & wjet : wjets1)
        {
          wmass1 += wjet.v4();
          float chi2 = pow((wmass1.M() - Mw_mean_3jets_) / Mw_sigma_3jets_, 2);
          if(chi2 < chi2_w1)
          {
              chi2_w1 = chi2;
              hyp.set_w1_v4(wmass1);
          }
        }
        const float Mthad_reco_1 = inv_mass(hyp.tophad1_v4());
        chi2_thad_1 = pow((Mthad_reco_1 - Mthad_mean_3jets_) / Mthad_sigma_3jets_, 2);
        hyp.set_discriminator(config.discriminator_label+"_whad1", chi2_w1);

    }
    if(NJets2 == 2)
    {
      const float Mthad_reco_2 = inv_mass(hyp.tophad2_v4());
      chi2_thad_2 = pow((Mthad_reco_2 - Mthad_mean_2jets_) / Mthad_sigma_2jets_, 2);

    }
    else if (NJets2 == 3)
    {
        float chi2_w2 = numeric_limits<float>::infinity();
        auto& wjets2 = hyp.tophad2_jets();
        LorentzVector wmass2;
        for(auto & wjet : wjets2)
        {
          wmass2 += wjet.v4();
          float chi2 = pow((wmass2.M() - Mw_mean_3jets_) / Mw_sigma_3jets_, 2);
          if(chi2 < chi2_w2)
          {
              chi2_w2 = chi2;
              hyp.set_w1_v4(wmass2);
          }
        }
        const float Mthad_reco_2 = inv_mass(hyp.tophad2_v4());
        chi2_thad_2 = pow((Mthad_reco_2 - Mthad_mean_3jets_) / Mthad_sigma_3jets_, 2);
        hyp.set_discriminator(config.discriminator_label+"_whad1", chi2_w2);

    }
    hyp.set_chi2_tt(chi2_thad_1 + chi2_thad_2);
    hyp.set_discriminator(config.discriminator_label+"_thad1", chi2_thad_1);
    hyp.set_discriminator(config.discriminator_label+"_thad2", chi2_thad_2);
    hyp.set_discriminator(config.discriminator_label         ,  chi2_thad_1 + chi2_thad_2);

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
        // auto& wjets1 = hyp.tophad1_wjets();
        // auto& wjets2 = hyp.tophad2_wjets();
        // if(wjets1.size() > 0 && wjets2.size() > 0)
        // {
        //   LorentzVector wmass1;
        //   LorentzVector wmass2;
        //   int w1_njets = 0;
        //   int w2_njets = 0;
        //   double deltaRW1 = deltaR(ttbargen.WTop().v4(),hyp.tophad1_wjets().at(0).v4());
        //   double deltaRW2 = deltaR(ttbargen.WTop().v4(),hyp.tophad2_wjets().at(0).v4());
        //   for(auto & jet : wjets1)
        //   {
        //     wmass1 += jet.v4();
        //     w1_njets ++;
        //     if(deltaRW1 < deltaR(wmass1,ttbargen.WTop().v4()) && w1_njets > 0)
        //     {
        //       deltaRW1 = deltaR(wmass1,ttbargen.WTop().v4());
        //       hyp.set_w1_v4(wmass1);
        //     }
        //     else if(deltaRW1 < deltaR(wmass1,ttbargen.WAntitop().v4()) && w2_njets > 0)
        //     {
        //       deltaRW1 = deltaR(wmass1,ttbargen.WAntitop().v4());
        //       hyp.set_w1_v4(wmass1);
        //     }
        //   }
        //   for(auto & jet : wjets2)
        //   {
        //     wmass2 += jet.v4();
        //     w2_njets ++;
        //     if(deltaRW2 < deltaR(wmass2,ttbargen.WTop().v4()) && w2_njets > 0)
        //     {
        //       deltaRW2 = deltaR(wmass2,ttbargen.WTop().v4());
        //       hyp.set_w2_v4(wmass2);
        //     }
        //     else if(deltaRW2 < deltaR(wmass2,ttbargen.WAntitop().v4()) && w2_njets > 0)
        //     {
        //       deltaRW2 = deltaR(wmass2,ttbargen.WAntitop().v4());
        //       hyp.set_w2_v4(wmass2);
        //     }
        //   }
        // }
        auto deltar_sum1 = deltaR(ttbargen.Top().v4(), hyp.tophad1_v4()) + deltaR(ttbargen.Antitop().v4(), hyp.tophad2_v4());
        auto deltar_sum2 = deltaR(ttbargen.Top().v4(), hyp.tophad2_v4()) + deltaR(ttbargen.Antitop().v4(), hyp.tophad1_v4());
        double deltar_sum = deltar_sum1;
        if(deltar_sum2 < deltar_sum1) deltar_sum = deltar_sum2;
        hyp.set_discriminator(config.discriminator_label, deltar_sum);
    }
    return true;
}



CorrectMatchDiscriminatorHad::CorrectMatchDiscriminatorHad(Context & ctx, const std::string & rechyps_name, const cfg & config_): config(config_){
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

bool CorrectMatchDiscriminatorHad::process(uhh2::Event & event){
    auto & hyps = event.get(h_hyps);
    const auto & ttbargen = event.get(h_ttbargen);
    auto dec = ttbargen.DecayChannel();
    if(dec != TTbarGen::e_had){
        for(auto & hyp: hyps){
            hyp.set_discriminator(config.discriminator_label, infinity);
        }
        return true;
    }

    // note that it is allowed that two partons from the hadronic ttbar decay match the same jet.
    for(auto & hyp: hyps){
        auto had_jets1 = hyp.tophad1_jets();
        auto had_jets2 = hyp.tophad2_jets();
        auto had_bjets1 = hyp.tophad1_bjet();
        auto had_bjets2 =  hyp.tophad2_bjet();
        if(had_bjets1.size() != 1 || had_bjets2.size() != 1)
        {
            hyp.set_discriminator(config.discriminator_label, infinity);
            continue;
        }

        if(had_jets1.size() != 2 || had_jets2.size() != 2){ // < 3 is allowed ...
            hyp.set_discriminator(config.discriminator_label, infinity);
            continue;
        }

        //index lists of jets that can be matched to partons
        std::set<int> matched_hadr_jets1,matched_hadr_jets2, matched_hadr_bjets1,matched_hadr_bjets2;

        // match b jets
        int index_h1, index_h2;
        float correct_dr;
        float dr1 = match_dr(ttbargen.bTop(), had_bjets1, index_h1) + match_dr(ttbargen.bAntitop(),had_bjets2, index_h2);
        float dr2 = match_dr(ttbargen.bTop(), had_bjets2, index_h2) + match_dr(ttbargen.bAntitop(),had_bjets1, index_h1);

        if( dr1 < dr2)
        {
          correct_dr = match_dr(ttbargen.bTop(), had_bjets1, index_h1) + match_dr(ttbargen.bAntitop(),had_bjets2, index_h2);

          if(index_h1 >= 0) matched_hadr_bjets1.insert(index_h1);
          if(index_h2 >= 0) matched_hadr_bjets2.insert(index_h2);

          //match quarks from W decays
          correct_dr += match_dr(ttbargen.Wdecay1(), had_jets1, index_h1);
          if(index_h1 >= 0) matched_hadr_jets1.insert(index_h1);
          correct_dr += match_dr(ttbargen.Wdecay2(), had_jets1, index_h1);

          correct_dr += match_dr(ttbargen.WMinusdecay1(), had_jets2, index_h2);
          if(index_h2 >= 0) matched_hadr_jets2.insert(index_h2);
          correct_dr += match_dr(ttbargen.WMinusdecay2(), had_jets2, index_h2);
          if(index_h2 >= 0) matched_hadr_jets2.insert(index_h2);


        }
        else
        {
          correct_dr = match_dr(ttbargen.bTop(), had_bjets2, index_h2) + match_dr(ttbargen.bAntitop(),had_bjets1, index_h1);
          if(index_h1 >= 0) matched_hadr_bjets1.insert(index_h1);
          if(index_h2 >= 0) matched_hadr_bjets2.insert(index_h2);

          //match quarks from W decays
          correct_dr += match_dr(ttbargen.WMinusdecay1(), had_jets1, index_h1);
          if(index_h1 >= 0) matched_hadr_jets1.insert(index_h1);
          correct_dr += match_dr(ttbargen.WMinusdecay2(), had_jets1, index_h1);
          if(index_h1 >= 0) matched_hadr_jets1.insert(index_h1);
          correct_dr += match_dr(ttbargen.Wdecay1(), had_jets2, index_h2);
          if(index_h2 >= 0) matched_hadr_jets2.insert(index_h2);
          correct_dr += match_dr(ttbargen.Wdecay2(), had_jets2, index_h2);
          if(index_h2 >= 0) matched_hadr_jets2.insert(index_h2);
        }

        // if not all jets of the hadronic side of the reconstruction could be matched: infinite
        // value:
        if(matched_hadr_bjets1.size() != had_bjets1.size() || matched_hadr_bjets2.size() != had_bjets2.size()){
            hyp.set_discriminator(config.discriminator_label, infinity);
            continue;
        }
        if(matched_hadr_jets1.size() != had_jets1.size() || matched_hadr_jets2.size() != had_jets2.size()){
            hyp.set_discriminator(config.discriminator_label, infinity);
            continue;
        }
        LorentzVector w1,w2;
        for (auto jets :had_jets1)
        {
          w1 += jets.v4();
        }
        for (auto jets :had_jets2)
        {
          w2 += jets.v4();
        }
        hyp.set_w1_v4(w1);
        hyp.set_w2_v4(w2);
        const float Mthad_mean_  = 174.;
        const float Mthad_sigma_ =  20.;
        const float Mthad_reco_1 = inv_mass(hyp.tophad1_v4());
        const float Mthad_reco_2 = inv_mass(hyp.tophad2_v4());
        const double chi2_thad_1 = pow((Mthad_reco_1 - Mthad_mean_) / Mthad_sigma_, 2);
        const double chi2_thad_2 = pow((Mthad_reco_2 - Mthad_mean_) / Mthad_sigma_, 2);
        hyp.set_chi2_tt(chi2_thad_1+chi2_thad_2);
        hyp.set_discriminator(config.discriminator_label, correct_dr);
    }
    return true;

}


CorrectMatchDiscriminator4Jets::CorrectMatchDiscriminator4Jets(Context & ctx, const std::string & rechyps_name, const cfg & config_): config(config_){
    h_hyps = ctx.get_handle<vector<TTbarRecoHadHypothesis>>(rechyps_name);
    h_ttbargen = ctx.get_handle<TTbarGen>(config.ttbargen_name);
}
bool CorrectMatchDiscriminator4Jets::process(uhh2::Event & event){
    auto & hyps = event.get(h_hyps);
    const auto & ttbargen = event.get(h_ttbargen);
    auto dec = ttbargen.DecayChannel();
    if(dec != TTbarGen::e_had && event.jets->size() != 4){
        for(auto & hyp: hyps){
            hyp.set_discriminator(config.discriminator_label, infinity);
        }
        return true;
    }

    // note that it is allowed that two partons from the hadronic ttbar decay match the same jet.
    for(auto & hyp: hyps){
        auto had_jets1 = hyp.tophad1_jets();
        auto had_jets2 = hyp.tophad2_jets();
        auto had_bjets1 = hyp.tophad1_bjet();
        auto had_bjets2 =  hyp.tophad2_bjet();

        if(had_bjets1.size() != 1 || had_bjets2.size() != 1)
        {
            hyp.set_discriminator(config.discriminator_label, infinity);
            continue;
        }

        if(had_jets1.size() != 1 || had_jets2.size() != 1){ // < 3 is allowed ...
            hyp.set_discriminator(config.discriminator_label, infinity);
            continue;
        }

        //index lists of jets that can be matched to partons
        std::set<int> matched_hadr_jets1,matched_hadr_jets2, matched_hadr_bjets1,matched_hadr_bjets2;

        // match b jets
        int index_h1, index_h2;
        float correct_dr;
        float dr1 = match_dr(ttbargen.bTop(), had_bjets1, index_h1) + match_dr(ttbargen.bAntitop(),had_bjets2, index_h2);
        float dr2 = match_dr(ttbargen.bTop(), had_bjets2, index_h2) + match_dr(ttbargen.bAntitop(),had_bjets1, index_h1);

        if( dr1 < dr2)
        {
          correct_dr = match_dr(ttbargen.bTop(), had_bjets1, index_h1) + match_dr(ttbargen.bAntitop(),had_bjets2, index_h2);
          if(index_h1 >= 0) matched_hadr_bjets1.insert(index_h1);
          if(index_h2 >= 0) matched_hadr_bjets2.insert(index_h2);

          //match quarks from W decays
          if(match_dr(ttbargen.Wdecay1(), had_jets1, index_h1) < match_dr(ttbargen.Wdecay2(), had_jets1, index_h1))
          {
            correct_dr += match_dr(ttbargen.Wdecay1(), had_jets1, index_h1);
            if(index_h1 >= 0) matched_hadr_jets1.insert(index_h1);
          }
          else
          {
            correct_dr += match_dr(ttbargen.Wdecay2(), had_jets1, index_h1);
            if(index_h1 >= 0) matched_hadr_jets1.insert(index_h1);
          }
          if(match_dr(ttbargen.WMinusdecay1(), had_jets2, index_h2) < match_dr(ttbargen.WMinusdecay2(), had_jets2, index_h2))
          {
            correct_dr += match_dr(ttbargen.WMinusdecay1(), had_jets2, index_h2);
            if(index_h2 >= 0) matched_hadr_jets2.insert(index_h2);
          }
          else
          {
            correct_dr += match_dr(ttbargen.WMinusdecay2(), had_jets2, index_h2);
            if(index_h2 >= 0) matched_hadr_jets2.insert(index_h2);
          }



        }
        else
        {
          correct_dr = match_dr(ttbargen.bTop(), had_bjets2, index_h2) + match_dr(ttbargen.bAntitop(),had_bjets1, index_h1);
          if(index_h1 >= 0) matched_hadr_bjets1.insert(index_h1);
          if(index_h2 >= 0) matched_hadr_bjets2.insert(index_h2);

          if(match_dr(ttbargen.WMinusdecay1(), had_jets1, index_h1) < match_dr(ttbargen.WMinusdecay2(), had_jets1, index_h1))
          {
            correct_dr += match_dr(ttbargen.WMinusdecay1(), had_jets1, index_h1);
            if(index_h1 >= 0) matched_hadr_jets1.insert(index_h1);
          }
          else
          {
            correct_dr += match_dr(ttbargen.WMinusdecay2(), had_jets1, index_h1);
            if(index_h1 >= 0) matched_hadr_jets1.insert(index_h1);
          }
          if(match_dr(ttbargen.Wdecay1(), had_jets2, index_h2) < match_dr(ttbargen.Wdecay2(), had_jets2, index_h2))
          {
            correct_dr += match_dr(ttbargen.Wdecay1(), had_jets2, index_h2);
            if(index_h2 >= 0) matched_hadr_jets2.insert(index_h2);
          }
          else
          {
          correct_dr += match_dr(ttbargen.Wdecay2(), had_jets2, index_h2);
          if(index_h2 >= 0) matched_hadr_jets2.insert(index_h2);
          }
        }

        // if not all jets of the hadronic side of the reconstruction could be matched: infinite
        // value:
        if(matched_hadr_bjets1.size() != had_bjets1.size() || matched_hadr_bjets2.size() != had_bjets2.size()){
            hyp.set_discriminator(config.discriminator_label, infinity);
            continue;
        }
        if(matched_hadr_jets1.size() != had_jets1.size() || matched_hadr_jets2.size() != had_jets2.size()){
            hyp.set_discriminator(config.discriminator_label, infinity);
            continue;
        }
        const float Mthad_mean_  = 120.;
        const float Mthad_sigma_ =  35.;
        const float Mthad_reco_1 = inv_mass(hyp.tophad1_v4());
        const float Mthad_reco_2 = inv_mass(hyp.tophad2_v4());
        const double chi2_thad_1 = pow((Mthad_reco_1 - Mthad_mean_) / Mthad_sigma_, 2);
        const double chi2_thad_2 = pow((Mthad_reco_2 - Mthad_mean_) / Mthad_sigma_, 2);
        hyp.set_chi2_tt(chi2_thad_1+chi2_thad_2);
        hyp.set_discriminator(config.discriminator_label, correct_dr);
    }
    return true;

}

CorrectMatchDiscriminator5Jets::CorrectMatchDiscriminator5Jets(Context & ctx, const std::string & rechyps_name, const cfg & config_): config(config_){
    h_hyps = ctx.get_handle<vector<TTbarRecoHadHypothesis>>(rechyps_name);
    h_ttbargen = ctx.get_handle<TTbarGen>(config.ttbargen_name);
}
bool CorrectMatchDiscriminator5Jets::process(uhh2::Event & event){
    auto & hyps = event.get(h_hyps);
    const auto & ttbargen = event.get(h_ttbargen);
    auto dec = ttbargen.DecayChannel();
    if(dec != TTbarGen::e_had && event.jets->size() != 5){
        for(auto & hyp: hyps){
            hyp.set_discriminator(config.discriminator_label, infinity);
        }
        return true;
    }
    // note that it is allowed that two partons from the hadronic ttbar decay match the same jet.
    for(auto & hyp: hyps){
        auto had_jets1 = hyp.tophad1_jets();
        auto had_jets2 = hyp.tophad2_jets();
        auto had_bjets1 = hyp.tophad1_bjet();
        auto had_bjets2 =  hyp.tophad2_bjet();

        if(had_bjets1.size() != 1 || had_bjets2.size() != 1)
        {
            hyp.set_discriminator(config.discriminator_label, infinity);
            continue;
        }

        if(had_jets1.size() + had_jets2.size() != 3)  {
            hyp.set_discriminator(config.discriminator_label, infinity);
            continue;
        }
        //
        // //index lists of jets that can be matched to partons
        std::set<int> matched_hadr_jets1,matched_hadr_jets2, matched_hadr_bjets1,matched_hadr_bjets2;

        // match b jets
        int index_h1, index_h2;
        float correct_dr;
        float dr1 = match_dr(ttbargen.bTop(), had_bjets1, index_h1) + match_dr(ttbargen.bAntitop(),had_bjets2, index_h2);
        float dr2 = match_dr(ttbargen.bTop(), had_bjets2, index_h2) + match_dr(ttbargen.bAntitop(),had_bjets1, index_h1);

        if( dr1 < dr2)
        {
          correct_dr = match_dr(ttbargen.bTop(), had_bjets1, index_h1) + match_dr(ttbargen.bAntitop(),had_bjets2, index_h2);
          if(index_h1 >= 0) matched_hadr_bjets1.insert(index_h1);
          if(index_h2 >= 0) matched_hadr_bjets2.insert(index_h2);

          //match quarks from W decays
          if(had_jets1.size() == 2)
          {
            correct_dr += match_dr(ttbargen.Wdecay1(), had_jets1, index_h1);
            if(index_h1 >= 0) matched_hadr_jets1.insert(index_h1);
            correct_dr += match_dr(ttbargen.Wdecay2(), had_jets1, index_h1);
            if(index_h1 >= 0) matched_hadr_jets1.insert(index_h1);
            if(match_dr(ttbargen.WMinusdecay1(), had_jets2, index_h2) < match_dr(ttbargen.WMinusdecay2(), had_jets2, index_h2))
            {
              correct_dr += match_dr(ttbargen.WMinusdecay1(), had_jets2, index_h2);
              if(index_h2 >= 0) matched_hadr_jets2.insert(index_h2);
            }
            else
            {
              correct_dr += match_dr(ttbargen.WMinusdecay2(), had_jets2, index_h2);
              if(index_h2 >= 0) matched_hadr_jets2.insert(index_h2);
            }

          }
          else
          {
            if(match_dr(ttbargen.Wdecay1(), had_jets1, index_h1) < match_dr(ttbargen.Wdecay2(), had_jets1, index_h1))
            {
              correct_dr += match_dr(ttbargen.Wdecay1(), had_jets1, index_h1);
              if(index_h1 >= 0) matched_hadr_jets1.insert(index_h1);
            }
            else
            {
              correct_dr += match_dr(ttbargen.Wdecay2(), had_jets1, index_h1);
              if(index_h1 >= 0) matched_hadr_jets1.insert(index_h1);
            }
            if(index_h1 >= 0) matched_hadr_jets1.insert(index_h1);
            correct_dr += match_dr(ttbargen.WMinusdecay1(), had_jets2, index_h2);
            if(index_h2 >= 0) matched_hadr_jets2.insert(index_h2);
            correct_dr += match_dr(ttbargen.WMinusdecay2(), had_jets2, index_h2);
            if(index_h2 >= 0) matched_hadr_jets2.insert(index_h2);
          }



        }


        else
        {
          correct_dr = match_dr(ttbargen.bTop(), had_bjets2, index_h2) + match_dr(ttbargen.bAntitop(),had_bjets1, index_h1);
          if(index_h1 >= 0) matched_hadr_bjets1.insert(index_h1);
          if(index_h2 >= 0) matched_hadr_bjets2.insert(index_h2);
          if(had_jets1.size() == 2)
          {
            correct_dr += match_dr(ttbargen.WMinusdecay1(), had_jets1, index_h1);
            if(index_h1 >= 0) matched_hadr_jets1.insert(index_h1);
            correct_dr += match_dr(ttbargen.WMinusdecay2(), had_jets1, index_h1);
            if(index_h1 >= 0) matched_hadr_jets1.insert(index_h1);
            if(match_dr(ttbargen.Wdecay1(), had_jets2, index_h2) < match_dr(ttbargen.Wdecay2(), had_jets2, index_h2))
            {
              correct_dr += match_dr(ttbargen.Wdecay1(), had_jets2, index_h2);
              if(index_h2 >= 0) matched_hadr_jets2.insert(index_h2);
            }
            else
            {
            correct_dr += match_dr(ttbargen.Wdecay2(), had_jets2, index_h2);
            if(index_h2 >= 0) matched_hadr_jets2.insert(index_h2);
            }
          }
          else
          {
            if(match_dr(ttbargen.WMinusdecay1(), had_jets1, index_h1) < match_dr(ttbargen.WMinusdecay2(), had_jets1, index_h1))
            {
              correct_dr += match_dr(ttbargen.WMinusdecay1(), had_jets1, index_h1);
              if(index_h1 >= 0) matched_hadr_jets1.insert(index_h1);
            }
            else
            {
              correct_dr += match_dr(ttbargen.WMinusdecay2(), had_jets1, index_h1);
              if(index_h1 >= 0) matched_hadr_jets1.insert(index_h1);
            }
            correct_dr += match_dr(ttbargen.Wdecay1(), had_jets2, index_h2);
            if(index_h2 >= 0) matched_hadr_jets2.insert(index_h2);
            correct_dr += match_dr(ttbargen.Wdecay2(), had_jets2, index_h2);
            if(index_h2 >= 0) matched_hadr_jets2.insert(index_h2);
          }

          // std::cout << "DR1:"<< correct_dr << endl;

        }

        // if not all jets of the hadronic side of the reconstruction could be matched: infinite
        // value:

        if(matched_hadr_bjets1.size() != had_bjets1.size() || matched_hadr_bjets2.size() != had_bjets2.size()){
            hyp.set_discriminator(config.discriminator_label, infinity);
            continue;
        }
        if((matched_hadr_jets1.size() + matched_hadr_jets2.size()) != (had_jets1.size() + had_jets2.size())){
            hyp.set_discriminator(config.discriminator_label, infinity);
            continue;
        }
        const float Mthad_mean_2jets_  = 120.;
        const float Mthad_sigma_2jets_ =  35.;
        const float Mthad_mean_3jets_  = 174.;
        const float Mthad_sigma_3jets_ =  20.;
        const float Mthad_reco_1 = inv_mass(hyp.tophad1_v4());
        const float Mthad_reco_2 = inv_mass(hyp.tophad2_v4());
        double chi2_thad_1, chi2_thad_2;
        if(had_jets1.size() == 1)
        {
          chi2_thad_1 = pow((Mthad_reco_1 - Mthad_mean_2jets_) / Mthad_sigma_2jets_, 2);
          chi2_thad_2 = pow((Mthad_reco_2 - Mthad_mean_3jets_) / Mthad_sigma_3jets_, 2);
        }
        else
        {
          chi2_thad_1 = pow((Mthad_reco_1 - Mthad_mean_3jets_) / Mthad_sigma_3jets_, 2);
          chi2_thad_2 = pow((Mthad_reco_2 - Mthad_mean_2jets_) / Mthad_sigma_2jets_, 2);
        }
        hyp.set_chi2_tt(chi2_thad_1+chi2_thad_2);
        hyp.set_discriminator(config.discriminator_label, correct_dr);
    }
    return true;

}
