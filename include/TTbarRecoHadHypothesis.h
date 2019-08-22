#pragma once

#include "UHH2/core/include/Jet.h"
#include "UHH2/core/include/LorentzVector.h"
#include <map>

/**
 *  @short container class to store the results of the top quark reconstruction
 *
 * The top quarks reconstruction only applied to semileptonic ttbar events. A
 * TTbarRecoHadHypothesis then consists of finding the lepton, and assigning the jets of the event
 * to either the leptonically decaying top or the hadronically decaying top. In addition
 * to accessing these information (i.e., which jets are assigned to which side, etc.), each
 * hypothesis can have a number of associated *discriminators*. A discriminator is identified
 * by name and is a floating point value meant to measure how 'good' the hypothesis is according to some criterion;
 * see TTbarRecoHadHypothesisDiscriminators.h for different criteria and to fill the discriminators.
 */
class TTbarRecoHadHypothesis {
public:
  explicit TTbarRecoHadHypothesis(){};

  LorentzVector tophad1_v4() const{return m_tophad1_v4;}
  LorentzVector tophad2_v4() const{return m_tophad2_v4;}
  const std::vector<Jet>& tophad1_jets() const{return m_tophad1_jets;}
  const std::vector<Jet>& tophad2_jets() const{return m_tophad2_jets;}


// get the discriminator value for this hypothesis; thows a runtime_error if it does not exist.
  float discriminator(const std::string & l) const {
      auto it = m_discriminators.find(l);
      if(it == m_discriminators.end()){
          throw std::runtime_error("TTbarRecoHadHypothesis::discriminator: discriminator with label '" + l + "' not set");
      }
      return it->second;
  }

// test if a discriminator value with a certian label has already been added
  bool has_discriminator(const std::string & label) const {
      return m_discriminators.find(label) != m_discriminators.end();
  }

  void set_tophad1_v4(LorentzVector v4){m_tophad1_v4=v4;}
  void set_tophad2_v4(LorentzVector v4){m_tophad2_v4=v4;}
  void add_tophad1_jet(const Jet& j){m_tophad1_jets.push_back(j);}
  void add_tophad2_jet(const Jet& j){m_tophad2_jets.push_back(j);}
  void set_discriminator(const std::string & label, float discr){
      m_discriminators[label] = discr;
  }

private:
  LorentzVector m_tophad1_v4;
  LorentzVector m_tophad2_v4;

  std::vector<Jet> m_tophad1_jets;
  std::vector<Jet> m_tophad2_jets;



  std::map<std::string, float> m_discriminators;
};
