#pragma once

#include "UHH2/core/include/fwd.h"
#include "UHH2/core/include/Selection.h"

#include "UHH2/ttZPrime/include/TTbarRecoHadHypothesis.h"
#include "UHH2/ttZPrime/include/TTbarRecoHadHypothesisDiscriminators.h"

namespace uhh2examples {

/* Select events with at least two jets in which the leading two jets have deltaphi > 2.7 and the third jet pt is
 * below 20% of the average of the leading two jets, where the minimum deltaphi and
 * maximum third jet pt fraction can be changed in the constructor.
 * The jets are assumed to be sorted in pt.
 */
class DijetSelection: public uhh2::Selection {
public:
    DijetSelection(float dphi_min = 2.7f, float third_frac_max = 0.2f);
    virtual bool passes(const uhh2::Event & event) override;
private:
    float dphi_min, third_frac_max;
};
class STSelection: public uhh2::Selection {
public:
  STSelection(double st_min);
  virtual bool passes(const uhh2::Event & event) override;
private:
  double m_st_min;

};
class MMuMUSelection: public uhh2::Selection {
public:
  MMuMUSelection(double mmumu_min);
  virtual bool passes(const uhh2::Event & event) override;
private:
  double m_mmumu_min;

};

class TopDRMCHadSelection: public uhh2::Selection{
public:
  TopDRMCHadSelection(uhh2::Context &ctx, double dr_max, const std::string & hyps_name, const std::string & discriminator_name);
  virtual bool passes(const uhh2::Event & event) override;
private:
  double m_dr_max;
  uhh2::Event::Handle<std::vector<TTbarRecoHadHypothesis>> h_hyps;
  std::string m_discriminator_name;
};

}
