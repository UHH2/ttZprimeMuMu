#include "UHH2/core/include/Event.h"
#include "UHH2/common/include/Utils.h"
#include "UHH2/core/include/LorentzVector.h"
#include "UHH2/ttZPrime/include/ttZPrimeControl3LHists.h"


#include <math.h>

#include "TH1F.h"
#include <iostream>
#include <vector>


using namespace std;
using namespace uhh2;


ttZPrimeControl3LHists::ttZPrimeControl3LHists(Context & ctx, const string & dirname): Hists(ctx, dirname){
  drminmugenreco = book<TH1F> ("drminmugenreco","#DeltaR(#mu)_{gen,reco}",10,0.,1.);
  mmumureal = book<TH1F>("M_mu1mu2_Real", "M_{#mu_{1}#mu_{2}} Real [GeV]",50 , 0, 2500);
  mmumufake = book<TH1F>("M_mu1mu2_Fake", "M_{#mu_{1}#mu_{2}} Fake [GeV]",50 , 0, 2500);
  mmumumix = book<TH1F>("M_mu1mu2_Mix", "M_{#mu_{1}#mu_{2}} Mix [GeV]",50 , 0, 2500);






}
void ttZPrimeControl3LHists::fill(const uhh2::Event & e){

  if(e.isRealData) return;
  double weight = e.weight;
  // assert(e.muons);
  // assert(e.electrons);
  vector<Muon> muonsR;
  vector<GenParticle> muonsG;
  for(const auto & muon : *e.muons){
    muonsR.push_back(muon);
  }
  for(const auto & muon : *e.genparticles){
    if(abs(muon.pdgId()) != 13) muonsG.push_back(muon);
  }

  // int NMuR = muonsR.size(), NMuG = muonsG.size();
  // if(NMuR > 1 && NMuR == NMuG ){
    // std::cout << "test2" << '\n';
    // for(int i = 0; i < NMuR; i++)
    // {
    //   double drmin=deltaR(muonsR.at(i).v4(),muonsG.at(0).v4());
    //   for(int k = 0; k < i; k++)
    //   {
    //     double dr = deltaR(muonsR.at(i).v4(),muonsG.at(k).v4());
    //     if(dr < drmin) drmin = dr;
    //   }
    //     drminmugenreco->Fill(drmin,weight);
    // }
    if((muonsR.at(0).charge()+muonsR.at(1).charge()) == 0){
      double drmin1=deltaR(muonsR.at(0).v4(),muonsG.at(0).v4()), drmin2=deltaR(muonsR.at(1).v4(),muonsG.at(0).v4());
      for(const auto & muon : muonsG){
        double dr1 = deltaR(muonsR.at(0).v4(),muon.v4());
        double dr2 = deltaR(muonsR.at(1).v4(),muon.v4());
        if(dr1 < drmin1) drmin1 = dr1;
        if(dr2 < drmin2) drmin2 = dr2;
      }
      double m_mumu = (muonsR.at(0).v4()+muonsR.at(1).v4()).M();
      if(drmin1 > 0.2 && drmin2 > 0.2)
      {
        mmumufake->Fill(m_mumu,weight);
      }
      else if (drmin1 > 0.2 || drmin2 > 0.2)
      {
        mmumumix->Fill(m_mumu,weight);
      }
      else mmumureal->Fill(m_mumu,weight);
    }
  // }

}
