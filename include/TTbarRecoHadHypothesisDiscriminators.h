#pragma once

#include "UHH2/core/include/AnalysisModule.h"
#include "UHH2/core/include/Event.h"

#include "UHH2/ttZPrime/include/TTbarRecoHadHypothesis.h"
#include "UHH2/common/include/TTbarGen.h"

/**  @short This file defines analysis modules which add discriminator values to reconstruction hypotheses
 *
 * As input, all modules need a vector of TTbarRecoHadHypothesis which must be in the event
 * when the module is called. The name of this vector can be specified at time of construction.
 * Further parameter can be defined by passing a 'cfg' object which typically controls the name of the
 * discriminator in th TTbarRecoHadHypothesis or the name of additional event input.
 *
 * The convention is that smaller values of the discriminator flag are better (as e.g. naturally the case
 * for chi-square). The exact meaning of the discriminators depends on the method, though.
 */


/** \brief Get the best hypothesis, i.e. the one with the smallest discriminator value
 *
 * If no hypothesis exists with that name or if the smallest discriminator is infinite, returns
 * nullptr.
 *
 * label is the discriminator label, e.g. "Chi2".
 */
const TTbarRecoHadHypothesis * get_best_hypothesis(const std::vector<TTbarRecoHadHypothesis> & hyps, const std::string & label);


/** \brief Calculate the chi-square reconstruction discriminator
 *
 * The Chi-square value is calculated from leptonic and hadronic reconstructed top-quark masses. This
 * is the default reconstruction-level method used in the 8TeV semi-leptonic high-mass CMS analyses.
 *
 * Per default, fills discriminators "Chi2", "Chi2_tlep" and "Chi2_thad" which are the over chi-square,
 * the chi-square only for the leptonic lep and the chi-square only for the hadronic leg, resp.
 * The name / prefix "Chi2" can be overridden via cfg::discriminator_label.
 *
 * For numeric values of the means and widths for the masses used see the implementation in the .cxx file;
 * they are the 8TeV values.
 */
class Chi2DiscriminatorHad: public uhh2::AnalysisModule {

 public:
  struct cfg {

   std::string discriminator_label;
   cfg(): discriminator_label("Chi2Had"){}
  };

  Chi2DiscriminatorHad(uhh2::Context&, const std::string&, const cfg& config=cfg());
  virtual bool process(uhh2::Event&) override;


  virtual void set_Mthad_mean (const float m){ Mthad_mean_  = m; }
  virtual void set_Mthad_sigma(const float s){ Mthad_sigma_ = s;


    if(s <= 0.) throw std::runtime_error("Chi2DiscriminatorHad::set_Mthad_sigma -- logic error: non-positive input value: "+std::to_string(s));
  }
  virtual void set_DMthad_mean (const float m){ DMthad_mean_  = m; }
  virtual void set_DMthad_sigma(const float s){ DMthad_sigma_ = s;


    if(s <= 0.) throw std::runtime_error("Chi2DiscriminatorHad::set_Mthad_sigma -- logic error: non-positive input value: "+std::to_string(s));
  }

  virtual float Mthad_mean () const { return Mthad_mean_; }
  virtual float Mthad_sigma() const { return Mthad_sigma_; }
  virtual float DMthad_mean () const { return DMthad_mean_; }
  virtual float DMthad_sigma() const { return DMthad_sigma_; }
 private:
  cfg config;
  uhh2::Event::Handle<std::vector<TTbarRecoHadHypothesis>> h_hyps;
  float Mthad_mean_, Mthad_sigma_, Mw_mean, Mw_sigma, DMthad_sigma_, DMthad_mean_;
};


/** \brief Top-DeltaR quality flag for Monte-Carlo
 *
 * Requires a TTbarGen object in the event (see TTbarGen.h).
 *
 * Writes "TopDRMC" quality flags to the reconstruction hypotheses, which is the sum of Delta R values
 * between the generated and reconstructed top quarks for the top and anti-top.
 *
 * NOTE: This used to be called "BestPossible" in UHHAnalysis.
 */

class TopDRMCDiscriminatorHad: public uhh2::AnalysisModule {
public:
    struct cfg {
        std::string ttbargen_name;
        std::string discriminator_label;
        cfg(): ttbargen_name("ttbargen"), discriminator_label("TopDRMCHad"){}
    };

    TopDRMCDiscriminatorHad(uhh2::Context & ctx, const std::string & rechyps_name, const cfg & config = cfg());
    virtual bool process(uhh2::Event & event) override;

private:

    uhh2::Event::Handle<std::vector<TTbarRecoHadHypothesis>> h_hyps;
    uhh2::Event::Handle<TTbarGen> h_ttbargen;
    cfg config;
};


/** \brief Try to match the reconstruction hypotheses to Monte-Carlo truth, jet-by-jet
 *
 * Requires a TTbarGen object in the event (see TTbarGen.h).
 *
 * Writes a "CorrectMatch" quality flags to the reconstruction hypotheses, which is the sum of Delta R values
 * between the four generated and reconstructed matrix-element final-state partons and the DeltaR between the
 * true neutrino and the reconstructed neutrino. The discriminator is set to infinity
 * if one of the final-state partons could not be matched to a jet within Delta R < 0.3 (note that no
 * such match is done for the neutrino).
 *
 * NOTE: This class only works for events which are (on gen-level) electron+jets or muon+jets. Otherwise, all discriminator
 * values are set to +infinity. The reconstructed lepton is ignored in this discriminator criterion.
 */
/*
class CorrectMatchDiscriminator: public uhh2::AnalysisModule {
public:
    struct cfg {
        std::string ttbargen_name;
        std::string discriminator_label;
        cfg(): ttbargen_name("ttbargen"), discriminator_label("CorrectMatch"){}
    };

    CorrectMatchDiscriminator(uhh2::Context & ctx, const std::string & rechyps_name, const cfg & config = cfg());
    virtual bool process(uhh2::Event & event) override;

private:

    uhh2::Event::Handle<std::vector<TTbarRecoHadHypothesis>> h_hyps;
    uhh2::Event::Handle<TTbarGen> h_ttbargen;
    cfg config;
};
*/
