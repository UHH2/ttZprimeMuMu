#pragma once

#include "UHH2/core/include/fwd.h"
#include "UHH2/core/include/Selection.h"
#include "UHH2/common/include/ObjectIdUtils.h"
#include "UHH2/ttZPrime/include/TTbarRecoHadHypothesis.h"
#include "UHH2/ttZPrime/include/TTbarRecoHadHypothesisDiscriminators.h"
#include "UHH2/common/include/TTbarGen.h"
#include <iostream>
#include <vector>


namespace uhh2examples {

/* Select events with at least two jets in which the leading two jets have deltaphi > 2.7 and the third jet pt is
 * below 20% of the average of the leading two jets, where the minimum deltaphi and
 * maximum third jet pt fraction can be changed in the constructor.
 * The jets are assumed to be sorted in pt.
 */

 class METSelection: public uhh2::Selection {
 public:
   METSelection(double met_max_);
   virtual bool passes(const uhh2::Event &event) override;
 private:
   double met_max;
 };


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
  const double m_mmumu_min;

};

class MEleMuSelection: public uhh2::Selection {
public:
  MEleMuSelection( double melemu_min);
  virtual bool passes(const uhh2::Event & event) override;
private:
  const double m_melemu_min;

};

class NMuIDSelection: public uhh2::Selection {
public:
  NMuIDSelection(const boost::optional<MuonId> & muid = boost::none, unsigned int NMu = 2);
  virtual bool passes(const uhh2::Event & event) override;
private:
   boost::optional<MuonId> muid;
   const unsigned int NMu;


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
class DRMuJetSelection: public uhh2::Selection{
public:
  DRMuJetSelection(double deltaR_min);
  virtual bool passes(const uhh2::Event & event) override;
private:
  double m_deltaR_min;
};
class JetQuarkMatchingSelection: public uhh2::Selection{
public:
  JetQuarkMatchingSelection(uhh2::Context & ctx, int NMatchs, const std::string & hyps_name, const std::string & discriminator_name);
  virtual bool passes (const uhh2::Event & event) override;
private:
  int m_nmatchs;
  std::string m_discriminator_name;
  uhh2::Event::Handle<TTbarGen> h_ttbargen;
  std::vector<GenParticle> GenP;
  uhh2::Event::Handle<std::vector<TTbarRecoHadHypothesis>> h_hyps;
  std::vector<TTbarRecoHadHypothesis> hyps;
  const TTbarRecoHadHypothesis* hyp;
  std::vector<Jet> jets;
};

}
