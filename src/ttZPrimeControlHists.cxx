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

   book<TH1F>("M_ee", "M_{ee} [GeV]",50 , 0, 2500);
   book<TH1F>("delta_phi_ee","#Delta#phi_{ee}",50,0,TMath::Pi());
   book<TH1F>("phi_ee","#phi_{ee}",50,-TMath::Pi(),TMath::Pi());
   book<TH1F>("eta_ee","#eta_{ee}",50,-3,3);
   book<TH1F>("Pt_ee", "P_{T,ee} [GeV]",50 , 0, 1000);
   book<TH1F>("M_mue", "M_{#mue} [GeV]",50 , 0, 2500);
   book<TH1F>("delta_phi_mue","#Delta#phi_{#mue}",50,0,TMath::Pi());
   book<TH1F>("phi_mue","#phi_{#mue}",50,-TMath::Pi(),TMath::Pi());
   book<TH1F>("eta_mue","#eta_{#mue}",50,-3,3);
   book<TH1F>("Pt_mue", "P_{T,#mue} [GeV]",50 , 0, 1000);
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
  // if(electrons.size() > 1)
  // {
  //   if((electrons[0].charge()+electrons[1].charge()) == 0)
  //   {
  //     LorentzVector ee = (electrons[0].v4() + electrons[1].v4());
  //     hist("M_ee")->Fill(ee.M(), weight);
  //     hist("delta_phi_ee")->Fill(abs(deltaPhi(electrons[0].v4(),electrons[1].v4())),weight);
  //     hist("phi_ee")->Fill(ee.phi(),weight);
  //     hist("eta_ee")->Fill(ee.eta(),weight);
  //     hist("Pt_ee")->Fill(ee.pt(),weight);
  //   }
  // }
  if(electrons.size() > 0 && muons.size() > 0)
  {
    if((electrons[0].charge()+muons[0].charge()) == 0)
    {
      LorentzVector mue = (electrons[0].v4() + muons[0].v4());
      hist("M_mue")->Fill(mue.M(), weight);
      hist("delta_phi_mue")->Fill(abs(deltaPhi(muons[0].v4(),electrons[0].v4())),weight);
      hist("phi_mue")->Fill(mue.phi(),weight);
      hist("eta_mue")->Fill(mue.eta(),weight);
      hist("Pt_mue")->Fill(mue.pt(),weight);
    }
  }
}
