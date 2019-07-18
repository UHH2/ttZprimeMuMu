#include "UHH2/ttZPrime/include/ttZPrimeMuPreselectionHists.h"
#include "UHH2/core/include/Event.h"
#include "UHH2/common/include/Utils.h"
#include "UHH2/core/include/LorentzVector.h"
#include <math.h>

#include "TH1F.h"
#include <iostream>
#include <vector>


using namespace std;
using namespace uhh2;

ttZPrimeMuPreselectionHists::ttZPrimeMuPreselectionHists(Context & ctx, const string & dirname): Hists(ctx, dirname){

   book<TH1F>("M_mumu", "M_{#mu#mu} [GeV^2]",100 , 0, 2000);
   book<TH1F>("delta_phi_mumu","#Delta#phi_{#mu#mu}",50,0,TMath::Pi());
   book<TH1F>("phi_mumu","#phi_{#mu#mu}",50,-TMath::Pi(),TMath::Pi());
   book<TH1F>("eta_mumu","#eta_{#mu#mu}",50,-3,3);
   book<TH1F>("Pt_mumu", "P_{T,#mu#mu} [GeV]",100 , 0, 1000);
   book<TH1F>("M_mu1mu2", "M_{#mu_{1}#mu_{2}} [GeV^2]",100 , 0, 2000);



 }

 void ttZPrimeMuPreselectionHists::fill(const Event & event){
   // fill the histograms. Please note the comments in the header file:
   // 'hist' is used here a lot for simplicity, but it will be rather
   // slow when you have many histograms; therefore, better
   // use histogram pointers as members as in 'UHH2/common/include/ElectronHists.h'
  double weight = event.weight;
  int Nmuons = event.muons->size();
  double M_mumu;
  LorentzVector muons[Nmuons];
  for(int i=0; i<Nmuons; i++){
    muons[i] = event.muons->at(i).v4();
  }
  hist("M_mu1mu2")->Fill((muons[0]+muons[1]).M());
  for(int i=0; i<Nmuons; i++){
    for(int j=0; j<Nmuons; j++){
      if(j > i){
          M_mumu = (muons[i] + muons[j]).M();
          hist("M_mumu")->Fill(M_mumu, weight);
          hist("delta_phi_mumu")->Fill(abs(deltaPhi(muons[i],muons[j])));
          hist("phi_mumu")->Fill((muons[i] + muons[j]).phi(),weight);
          hist("eta_mumu")->Fill((muons[i] + muons[j]).eta(),weight);
          hist("Pt_mumu")->Fill((muons[i] + muons[j]).pt(),weight);
        }
      }
    }
  }
