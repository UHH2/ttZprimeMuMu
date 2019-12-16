#include "UHH2/ttZPrime/include/ttZPrimeTTBarGenStudyHists.h"
#include "TH1F.h"
#include "TH2F.h"
#include <iostream>
#include <vector>

using namespace uhh2;
using namespace std;

ttZPrimeTTBarGenStudyHists::ttZPrimeTTBarGenStudyHists(uhh2::Context & ctx, const std::string & dirname, const std::string & hyps_name, const std::string & discriminator_name_reco,  const std::string & discriminator_name_match ): Hists(ctx, dirname){
    mindrqq = book<TH1F> ("GenPMinQQ","#GenP_{q_{i},q_{j}}",60,0,6);
    matchedevents = book<TH1F> ("RightRecoEvents","RightRecoEvents",5,0,4);
    matchedbjets = book<TH1F> ("RightRecoBJets","RightRecoBJets",5,0,4);
    matchedjets = book<TH1F> ("RightRecoJets","RightRecoJets",5,0,4);
    matchedw = book<TH1F> ("RightRecoW","RightRecoW",5,0,4);
    mindrqj = book<TH1F> ("GenPMinQJet","GenP_{jet_{i},q_{j}}",40,0,2);
    matchsperjet = book<TH1F> ("MatchsPerJet","MatchsPerJet",5,0,5);
    ptResolution = book<TH1F> ("ptResolutionQJ", "#DeltaPt_{q,Jet} [GeV]",100,-50,50);
    h_ttbargen = ctx.get_handle<TTbarGen>("ttbargen");
    h_hyps = ctx.get_handle<std::vector<TTbarRecoHadHypothesis>>(hyps_name);
    m_discriminator_name_reco = discriminator_name_reco;
    m_discriminator_name_match = discriminator_name_match;

};
void ttZPrimeTTBarGenStudyHists::fill(const uhh2::Event & e){
  if(e.isRealData) return;
  std::vector<TTbarRecoHadHypothesis> hyps = e.get(h_hyps);
  const TTbarRecoHadHypothesis* hyp_reco = get_best_hypothesis( hyps, m_discriminator_name_reco );
  const TTbarRecoHadHypothesis* hyp_match = get_best_hypothesis( hyps, m_discriminator_name_match );
  if(!hyp_reco && !hyp_match) return;
  auto  jets = e.jets;
 // auto bjets = hyp->tophad1_jets();
  //jets.insert(jets.end(),hyp->tophad2_jets().begin(), hyp->tophad2_jets().end());
  //jets.insert(jets.end(),hyp->tophad1_bjet().begin(), hyp->tophad1_bjet().end());
  //jets.insert(jets.end(),hyp->tophad2_bjet().begin(), hyp->tophad2_bjet().end());
  const auto & ttbargen = e.get(h_ttbargen);
  const auto & weight = e.weight;
  float chi2_tt_match = hyp_match->chi2_tt();
  float chi2_tt_reco = hyp_reco->chi2_tt();
  auto jets1_match = hyp_match->tophad1_jets();
  auto jets1_reco = hyp_reco->tophad1_jets();
  auto jets2_match = hyp_match->tophad2_jets();
  auto jets2_reco = hyp_reco->tophad2_jets();
  if (jets1_match.size() == jets1_reco.size())
  {
    for (unsigned int i = 0; i < jets1_match.size(); i++)
    {
      if(deltaR(jets1_match.at(i),jets1_reco.at(i))< 0.4) matchedjets->Fill(1, weight);
      else matchedjets->Fill(3, weight);
    }
  }
  if (jets2_match.size() == jets2_reco.size())
  {
    for (unsigned int i = 0; i < jets2_match.size(); i++)
    {
      if(deltaR(jets2_match.at(i),jets2_reco.at(i))< 0.4) matchedjets->Fill(1, weight);
      else matchedjets->Fill(3, weight);
    }
  }
  if(deltaR(hyp_match->tophad1_bjet().at(0),hyp_reco->tophad1_bjet().at(0))< 0.4) matchedbjets->Fill(1, weight);
  else matchedbjets->Fill(3, weight);
  if(deltaR(hyp_match->tophad2_bjet().at(0),hyp_reco->tophad2_bjet().at(0))< 0.4) matchedbjets->Fill(1, weight);
  else matchedbjets->Fill(3, weight);
  if(deltaR(hyp_match->w1_v4(),hyp_reco->w1_v4())< 0.4) matchedw->Fill(1, weight);
  else matchedw->Fill(3, weight);
  if(deltaR(hyp_match->w2_v4(),hyp_reco->w2_v4())< 0.4) matchedw->Fill(1, weight);
  else matchedw->Fill(3, weight);
  if(chi2_tt_match == chi2_tt_reco) matchedevents->Fill(1, weight);
  else matchedevents->Fill(3, weight);
  vector<GenParticle> GenP;
  GenP.push_back(ttbargen.bTop());
  GenP.push_back(ttbargen.bAntitop());
  GenP.push_back(ttbargen.Wdecay1());
  GenP.push_back(ttbargen.Wdecay2());
  GenP.push_back(ttbargen.WMinusdecay1());
  GenP.push_back(ttbargen.WMinusdecay2());

  for(unsigned int k = 0; k<GenP.size(); k++){
    double drmin = std::numeric_limits<double>::infinity();
    for(unsigned int i = 0; i< k ; i++)
    {
        double dr = uhh2::deltaR(GenP.at(k), GenP.at(i));
        if (dr < drmin) drmin = dr;
    }
    mindrqq->Fill(drmin, weight);
  }

  for(unsigned int k = 0; k< jets->size(); k++){
    double drmin = std::numeric_limits<double>::infinity();
    int nmatchs = 0;
    // std::cout << "QPt:" << GenP.at(k).v4().pt() << " QEta:" << GenP.at(k).v4().eta() << " QPhi:" << GenP.at(k).v4().phi() <<endl;
    for(unsigned int i = 0; i< GenP.size(); i++)
    {
        // std::cout << "JetPt:" << jets->at(i).v4().pt() << " JetEta:" << jets->at(i).v4().eta() << " JetPhi:" << jets->at(i).v4().phi() <<endl;
        double dr = uhh2::deltaR(GenP.at(i), jets->at(k));
        if (dr < drmin) drmin = dr;
        if (dr < 0.4)
        {
          nmatchs++;
          ptResolution->Fill(jets->at(k).v4().pt()-GenP.at(i).v4().pt());
        }
    }
    mindrqj->Fill(drmin, weight);
    matchsperjet->Fill(nmatchs, weight);
    // std::cout <<"Next Quark" <<endl;
  }
}
