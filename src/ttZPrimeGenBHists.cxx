#include "UHH2/core/include/Event.h"
#include "UHH2/common/include/Utils.h"
#include "UHH2/core/include/LorentzVector.h"
#include "UHH2/ttZPrime/include/ttZPrimeGenBHists.h"


#include <math.h>

#include "TH1F.h"
#include <iostream>
#include <vector>


using namespace std;
using namespace uhh2;


ttZPrimeGenBHists::ttZPrimeGenBHists(Context & ctx, const string & dirname): Hists(ctx, dirname){
  NBQuarksGen = book<TH1F>("NBQuarksGen","N_{b} Gen",10, 0, 10);
  BQuarksGenPhi = book<TH1F>("phi_b","#phi gen b",50,-M_PI,M_PI);
  BQuarksGenPt =  book<TH1F>("pt_b","p_{T} gen b",50,0,500);
  BQuarksGenEta = book<TH1F>("eta_b","#eta gen b",50,-3,3);
}

void ttZPrimeGenBHists::fill(const uhh2::Event & e){
  if(e.isRealData) return;
  double weight = e.weight;
  vector<GenParticle>  bQuarks;
  for(const auto & muon : *e.genparticles){
    if(abs(muon.pdgId()) == 5 && muon.status() == 23) bQuarks.push_back(muon);
  }
  NBQuarksGen->Fill(bQuarks.size(), weight);
  for(const auto b : bQuarks)
  {
    BQuarksGenPt->Fill(b.pt(),weight);
    BQuarksGenPhi->Fill(b.phi(),weight);
    BQuarksGenEta->Fill(b.eta(),weight);
  }
}
