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
  drminmugenreco = book<TH1F> ("drminmugenreco","#DeltaR(#mu)_{gen,reco}",100,0.,5.);
  drminmurecogenb =  book<TH1F> ("drminmurecogenb","#DeltaR(#mu,b)_{reco,gen}",100,0.,5.);
  mmumureal = book<TH1F>("M_mu1mu2_Real", "M_{#mu_{1}#mu_{2}} Real [GeV]",50 , 0, 2500);
  mmumufake = book<TH1F>("M_mu1mu2_Fake", "M_{#mu_{1}#mu_{2}} Fake [GeV]",50 , 0, 2500);
  NMuonsGen = book<TH1F>("NMuonsGen","N_{#mu} Gen",10, 0, 10);
  MuFakePhi = book<TH1F>("phi_fakemu","#phi fake #mu",100,-M_PI,M_PI);
  MuFakePt =  book<TH1F>("pt_fakemu","p_{T} fake #mu",100,0,500);
  MuFakeEta = book<TH1F>("eta_fakemu","#eta fake #mu",100,-3,3);
  // mmumumix = book<TH1F>("M_mu1mu2_Mix", "M_{#mu_{1}#mu_{2}} Mix [GeV]",50 , 0, 2500);
  nevtFake = book<TH1F>("NEvtFake", "", 1,0,1);
  // nevtMix = book<TH1F>("NEvtMix", "", 1,0,1);
  nevtReal = book<TH1F>("NEvtReal", "", 1,0,1);
  NFakeMuons = book <TH1F> ("NFakeMuons", "N_{#mu} Fake",10,0,10);





}
void ttZPrimeControl3LHists::fill(const uhh2::Event & e){
  bool debug = true;
  if(debug) std::cout << "New Event" << '\n';
  if(e.isRealData) return;
  double weight = e.weight;
  // assert(e.muons);
  // assert(e.electrons);
  vector<Muon> muonsR;
  vector<GenParticle> muonsG, bQuarks;
  for(const auto & muon : *e.muons){
    muonsR.push_back(muon);
  }
  for(const auto & muon : *e.genparticles){
    if(debug && abs(muon.pdgId()) == 13) std::cout << "GenParticleStatus:" << muon.status() << '\n';
    if(abs(muon.pdgId()) == 13 && muon.status() == 23) muonsG.push_back(muon);
    if(abs(muon.pdgId()) == 5 && muon.status() == 23) bQuarks.push_back(muon);
  }
  if(debug)
  {
    std::cout << "N Gen Muons: " << muonsG.size() <<'\n';
    std::cout << "N Reco Muons: " << muonsR.size() <<'\n';
  }
  NMuonsGen->Fill(muonsG.size(), weight);

  // int NMuR = muonsR.size(), NMuG = muonsG.size();
  // if(NMuR > 1 && NMuR == NMuG ){
    // std::cout << "test2" << '\n';
    // for(int i = 0; i < NMuR; i++)
    // {
    //   double drmin_muons=deltaR(muonsR.at(i).v4(),muonsG.at(0).v4());
    //   for(int k = 0; k < i; k++)
    //   {
    //     double dr = deltaR(muonsR.at(i).v4(),muonsG.at(k).v4());
    //     if(dr < drmin_muons) drmin_muons = dr;
    //   }
    //     drmin_muonsmugenreco->Fill(drmin_muons,weight);
    // }
      bool muons_used_already[muonsG.size()] = {false};
      bool bquarks_used_already[bQuarks.size()] = {false};
      unsigned int n_rmuon = 0, nfakes =0;
      for(const auto & rmuon : muonsR){
        if(debug)std::cout << "New Reco Muon !!!!!!!!!!!!!!!!!!" << '\n';
        double drmin_muons=10 , drmin_bquarks = 10;
        for(unsigned int i = 0; i < muonsG.size(); i++){
          double dr = deltaR(rmuon.v4(),muonsG.at(i).v4());
          if(dr < drmin_muons && !muons_used_already [i])
          {
           drmin_muons = dr;
          }
          if(debug)
          {
            std::cout << "MotherId:" << (muonsG.at(i).mother(e.genparticles))->pdgId() << '\n';
            std::cout << "GenMuPt:" << muonsG.at(i).pt() << '\n';
            std::cout << "GenMuEta:" << muonsG.at(i).eta() << '\n';
            std::cout << "GenMuPhi:" << muonsG.at(i).phi() << '\n';
          }
          if(debug) std::cout <<"DeltaRMin: "<< dr << '\n';
          if(dr < 0.2) muons_used_already [i] = true;
          if(debug) std::cout <<"Used muons_used_already:"<< muons_used_already [i]  << '\n';
        }

        drminmugenreco->Fill(drmin_muons,weight);
        if(drmin_muons < 0.2) n_rmuon++;
        else{
          for (unsigned int i = 0; i < bQuarks.size(); i++)
          {
            double dr = deltaR(rmuon.v4(),bQuarks.at(i).v4());
            if(dr < drmin_bquarks && !bquarks_used_already[i]) drmin_bquarks = dr;
            if(dr <0.2) bquarks_used_already[i] = true;
          }
          drminmurecogenb->Fill(drmin_bquarks, weight);
          if(drmin_bquarks < 0.2) n_rmuon++;
          else{
            nfakes ++;
            MuFakePt->Fill(rmuon.pt(),weight);
            MuFakeEta->Fill(rmuon.eta(), weight);
            MuFakePhi->Fill(rmuon.phi(),weight);
          }
        }
      }
      NFakeMuons->Fill(nfakes, weight);
      double m_mumu = (muonsR.at(0).v4()+muonsR.at(1).v4()).M();
      if(debug) std::cout << "N Real Muons:"<< n_rmuon << '\n';
      if(n_rmuon == muonsR.size())
      {
        mmumureal->Fill(m_mumu,weight);
        nevtReal->Fill(0.,weight);
      }
      // else if (drmin_muons1 > 0.2 || drmin_muons2 > 0.2)
      // {
      //   mmumumix->Fill(m_mumu,weight);
      //   nevtMix->Fill(0.,weight);
      // }
      else
      {
        mmumufake->Fill(m_mumu,weight);
        nevtFake->Fill(0.,weight);
      }


  // }

}
