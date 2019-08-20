#include "UHH2/ttZPrime/include/ttZPrimeControlHists.h"
#include "UHH2/core/include/Event.h"
#include "UHH2/common/include/Utils.h"
#include "UHH2/core/include/LorentzVector.h"
#include <math.h>

#include "TH1F.h"
#include <iostream>
#include <vector>


using namespace std;
using namespace uhh2;

ttZPrimeControlHists::ttZPrimeControlHists(Context & ctx, const string & dirname): Hists(ctx, dirname){

   book<TH1F>("M_emu", "M_{e#mu} [GeV]",50 , 0, 2000);
   book<TH1F>("delta_phi_emu","#Delta#phi_{e#mu}",50,0,TMath::Pi());
   book<TH1F>("phi_emu","#phi_{e#mu}",50,-TMath::Pi(),TMath::Pi());
   book<TH1F>("eta_emu","#eta_{e#mu}",50,-3,3);
   book<TH1F>("Pt_emu", "P_{T,e#mu} [GeV]",50 , 0, 1000);
 }

 void ttZPrimeControlHists::fill(const Event & event){
   // fill the histograms. Please note the comments in the header file:
   // 'hist' is used here a lot for simplicity, but it will be rather
   // slow when you have many histograms; therefore, better
   // use histogram pointers as members as in 'UHH2/common/include/ElectronHists.h'
  double weight = event.weight;
  assert(event.muons);
  assert(event.electrons);
  vector<Muon> muons;
  vector<Electron> electrons;
  for(const auto & electron : *event.electrons){
    electrons.push_back(electron);
  }
  for(const auto & muon : *event.muons){
    muons.push_back(muon);
  }

    // cout << "Nmu=" << Nmuons << endl;

          // cout << "i=" << i << " ,j=" << j << endl;
  LorentzVector emu = (muons[0].v4() + electrons[0].v4());
  hist("M_emu")->Fill(emu.M(), weight);
  hist("delta_phi_emu")->Fill(abs(deltaPhi(muons[0].v4(),electrons[0].v4())),weight);
  hist("phi_emu")->Fill(emu.phi(),weight);
  hist("eta_emu")->Fill(emu.eta(),weight);
  hist("Pt_emu")->Fill(emu.pt(),weight);
}
