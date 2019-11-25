#pragma once

#include "UHH2/core/include/Hists.h"
#include "UHH2/core/include/Event.h"
#include "UHH2/ttZPrime/include/TTbarRecoHadHypothesisDiscriminators.h"
#include "UHH2/ttZPrime/include/TTbarRecoHadHypothesis.h"
#include "UHH2/common/include/TTbarGen.h"


class ttZPrimeTTBarGenStudyHists: public uhh2::Hists {
public:
    ttZPrimeTTBarGenStudyHists(uhh2::Context & ctx, const std::string & dirname, const std::string & hyps_name, const std::string & discriminator_name_reco,  const std::string & discriminator_name_match);

    virtual void fill(const uhh2::Event & ev) override;

protected:
  std::string m_discriminator_name_reco, m_discriminator_name_match;
  uhh2::Event::Handle<std::vector<TTbarRecoHadHypothesis>> h_hyps;
  uhh2::Event::Handle<TTbarGen> h_ttbargen;

  TH1F* mindrqq, *mindrqj, *matchsperjet, *ptResolution, *matchedevents, *matchedbjets;
};
