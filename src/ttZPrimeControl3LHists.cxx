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
  drminmugenreco = book<TH1F> ("drminmugenreco","#DeltaR_{min}(#mu gen,reco)",25,0.,5.);
  drminmugenrecomugenb = book<TH1F> ("drminrecomugenb","#DeltaR_{min}(reco #mu, gen b)",25,0.,5.);
  drminmufakegenb =  book<TH1F> ("drminmufakegenb","#DeltaR_{min}(fake #mu,b gen)",25,0.,5.);
  drminmufakejet = book<TH1F> ("drminmufakejet","#DeltaR(#mu,Jet)",25,0.,5.);
  drminmufakebjet = book<TH1F> ("drminmufakebjet","#DeltaR_{min}(#mu fake,bjet)",25,0.,5.);
  drminmufakegenp = book<TH1F> ("drminmufakegenp","#DeltaR(#mu,GenP)",25,0.,5.);
  mmumureal = book<TH1F>("M_mu1mu2_Real", "M_{#mu_{1}#mu_{2}} Real [GeV]",50 , 0, 2500);
  mmumufake = book<TH1F>("M_mu1mu2_Fake", "M_{#mu_{1}#mu_{2}} Fake [GeV]",50 , 0, 2500);
  memureal = book<TH1F>("M_emu_Real", "M_{e#mu} Real [GeV]",50 , 0, 2500);
  memufake = book<TH1F>("M_emu_Fake", "M_{e#mu} Fake [GeV]",50 , 0, 2500);
  NMuonsGen = book<TH1F>("NMuonsGen","N_{#mu} Gen",10, 0, 10);
  NBQuarksGen = book<TH1F>("NBQuarksGen","N_{b} Gen",10, 0, 10);
  MuFakePhi = book<TH1F>("phi_fakemu","#phi fake #mu",50,-M_PI,M_PI);
  MuFakePt =  book<TH1F>("pt_fakemu","p_{T} fake #mu",50,0,500);
  MuFakeEta = book<TH1F>("eta_fakemu","#eta fake #mu",50,-3,3);
  MuFakeIso  = book<TH1F>("isolationFakeMu","relIso fake muon",50,0,0.5);
  MuPromtIso = book<TH1F>("isolationPromtMu","relIso promt muon",50,0,0.5);
  // mmumumix = book<TH1F>("M_mu1mu2_Mix", "M_{#mu_{1}#mu_{2}} Mix [GeV]",50 , 0, 2500);
  nevtFake = book<TH1F>("NEvtFake", "NEvtFake", 1,0,1);
  // nevtMix = book<TH1F>("NEvtMix", "", 1,0,1);
  nevtReal = book<TH1F>("NEvtReal", "NEvtPromt", 1,0,1);
  NFakeMuons = book <TH1F> ("NFakeMuons", "N_{#mu} Fake",10,0,10);
  NPromtMuons = book <TH1F> ("NPromtMuons", "N_{#mu} Promt",10,0,10);







}


void ttZPrimeControl3LHists::fill(const uhh2::Event & e){
  bool debug = false;
  if(debug) std::cout << "New Event!!!!!!!!!!!!!!!!!!!!!" << '\n';
  if(e.isRealData) return;
  double weight = e.weight;
  // assert(e.muons);
  // assert(e.electrons);
  vector<Electron> electrons;
  bool withOneEle = false;

  if(e.electrons){
    for(const auto & electron : *e.electrons){
      electrons.push_back(electron);
    }
    if(electrons.size() > 0) withOneEle = true;
  }
  vector<Muon> muonsR;
  vector<GenParticle> muonsG, bQuarks, genParticles;
  std::vector<Jet> bjets, jets;
  for(const auto & jet : *e.jets)
  {
    jets.push_back(jet);
    // WORKS ONLY FOR 2016v3 !!!!!!!!!!!!!!
    if(jet.btag_DeepJet() < 0.0614) bjets.push_back(jet);
  }
  for(const auto & muon : *e.muons){
    muonsR.push_back(muon);
  }
  for(auto & muon : *e.genparticles){
    // if(debug && abs(muon.pdgId()) == 13) std::cout << "GenParticleStatus:" << muon.status() << '\n';
    if(abs(muon.pdgId()) != 13 && abs(muon.pdgId()) != 5 && muon.status() ==1) genParticles.push_back(muon);
    if(abs(muon.pdgId()) == 13 && (muon.status() ==1 || || muon.status() == 23)) muonsG.push_back(muon);
    if(abs(muon.pdgId()) == 5 ) bQuarks.push_back(muon);
  }
  if(debug)
  {
    std::cout << "N Gen Muons: " << muonsG.size() <<'\n';
    std::cout << "N Reco Muons: " << muonsR.size() <<'\n'<< '\n';
  }

 if(debug)
 {
   std::cout << "RecoMu (Pt, Eta, Phi, Iso):" << '\n';
   for(const auto muon : muonsR)
   {
     std::cout << muon.pt() << ", " << muon.eta() << ", " << muon.phi() << ", " << muon.relIso() << '\n';
   }
   if(debug) std::cout  << '\n' << "GenMu (Pt, Eta, Phi):" <<'\n';
   for(const auto muon : muonsG)
   {
     std::cout << muon.pt() << ", " << muon.eta() << ", " << muon.phi() << '\n';
   }
   std::cout << '\n';
 }
 unsigned int NRecoMuons = muonsR.size();
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

      NMuonsGen->Fill(muonsG.size(), weight);
      bool muons_used_already[muonsG.size()] = {false};
      bool bquarks_used_already[bQuarks.size()] = {false};
      bool bjets_used_already[bjets.size()] = {false};
      unsigned int n_rmuon = 0, nfakes =0;
      if(debug)std::cout << "DeltaR:" << '\n';
      for(const auto & rmuon : muonsR){
        // std::cout << "SimType:" << rmuon.simType() << '\n';
        // if(debug)std::cout << "New Reco Muon !!!!!!!!!!!!!!!!!!" << '\n';
        // if(debug)
        // {
        //   std::cout << "RecoMuPt:" << rmuon.pt() << '\n';
        //   std::cout << "RecoMuEta:" << rmuon.eta() << '\n';
        //   std::cout << "RecoMuPhi:" << rmuon.phi() << '\n';
        // }
        double drmin_muons=10 , drmin_bquarks = 10, drmin_bjets =10;

        for(unsigned int i = 0; i < muonsG.size(); i++){
          double dr = deltaR(rmuon.v4(),muonsG.at(i).v4());
          if(dr < drmin_muons && !muons_used_already [i])
          {
           drmin_muons = dr;
          }
          // if(debug)
          // {
          //   // std::cout << "MotherId:" << (muonsG.at(i).mother(e.genparticles))->pdgId() << '\n';
          //   std::cout << "GenMuPt:" << muonsG.at(i).pt() << '\n';
          //   std::cout << "GenMuEta:" << muonsG.at(i).eta() << '\n';
          //   std::cout << "GenMuPhi:" << muonsG.at(i).phi() << '\n';
          // }
          if(debug) std::cout << dr << ", ";
          if(dr < 0.2) muons_used_already [i] = true;
          // if(debug) std::cout <<"Used muons_used_already:"<< muons_used_already [i]  << '\n';
        }
        if(debug) std::cout  << '\n' ;
        drminmugenreco->Fill(drmin_muons,weight);
        for (unsigned int i = 0; i < bQuarks.size(); i++)
        {
          double dr = deltaR(rmuon.v4(),bQuarks.at(i).v4());
          if(dr < drmin_bquarks && !bquarks_used_already[i]) drmin_bquarks = dr;
          if(dr <0.2) bquarks_used_already[i] = true;
        }

        // if(debug) std::cout << "Min. DR (b,mu):" << drmin_bquarks << '\n';
        drminmugenrecomugenb->Fill(drmin_bquarks, weight);

        if(drmin_muons < 0.2)
        {
         n_rmuon++;
         MuPromtIso->Fill(rmuon.relIso(), weight);
        }
        else{
          nfakes ++;
          if(debug)
          {
            std::cout<< '\n'<<"FakeDeltaRMin: "<< drmin_muons << '\n';
            // std::cout << "FakeMuPt:" << rmuon.pt() << '\n';
            // std::cout << "FakeMuEta:" << rmuon.eta() << '\n';
            // std::cout << "FakeMuPhi:" << rmuon.phi() << '\n';
          }
          MuFakePt->Fill(rmuon.pt(),weight);
          MuFakeEta->Fill(rmuon.eta(), weight);
          MuFakePhi->Fill(rmuon.phi(),weight);
          MuFakeIso->Fill(rmuon.relIso(),weight);
          drminmufakegenb->Fill(drmin_bquarks, weight);
          for (unsigned int i = 0; i < bjets.size(); i++)
          {
            double dr =deltaR(rmuon.v4(),bjets.at(i).v4());
            if(dr < drmin_bjets && !bjets_used_already[i]) drmin_bjets = dr;
            if(dr < 0.2) bjets_used_already[i] = true;
          }
          drminmufakebjet->Fill(drmin_bjets, weight);
          // std::cout << "Nbquarks:"<<bQuarks.size() << '\n';

          for(const auto & jet: jets)
          {
            drminmufakejet->Fill(deltaR(rmuon.v4(),jet.v4()), weight);
          }
          for(const auto &  genp : genParticles)
          {
            drminmufakegenp->Fill(deltaR(rmuon.v4(),genp.v4()), weight);
          }

      }
    }
      NFakeMuons->Fill(nfakes, weight);
      NPromtMuons->Fill(n_rmuon, weight);
      // if(debug) std::cout << "NFakeMuons+NPromtMuons: "<< nfakes+n_rmuon << '\n';
      double m_mumu = 0., m_emu = 0.;
      if(NRecoMuons > 1) m_mumu = (muonsR.at(0).v4()+muonsR.at(1).v4()).M();
      if(withOneEle) m_emu =(muonsR.at(0).v4()+electrons.at(0).v4()).M();
      if(debug) std::cout <<'\n'<< "N Real Muons:"<< n_rmuon << '\n';

      if(n_rmuon == NRecoMuons)
      {
        if(NRecoMuons > 1) mmumureal->Fill(m_mumu,weight);
        nevtReal->Fill(0.5,weight);
        if(withOneEle) memureal->Fill(m_emu, weight);

      }
      // else if (drmin_muons1 > 0.2 || drmin_muons2 > 0.2)
      // {
      //   mmumumix->Fill(m_mumu,weight);
      //   nevtMix->Fill(0.,weight);
      // }
      else
      {
        if(NRecoMuons > 1) mmumufake->Fill(m_mumu,weight);
        nevtFake->Fill(0.5,weight);
        if(withOneEle) memufake->Fill(m_emu, weight);
      }




  }
