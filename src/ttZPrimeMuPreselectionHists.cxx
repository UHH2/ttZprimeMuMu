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

   book<TH1F>("M_mumu", "M_{#mu#mu} [GeV]",50 , 0, 2500);
   book<TH1F>("delta_phi_mumu","#Delta#phi_{#mu#mu}",50,0,TMath::Pi());
   book<TH1F>("phi_mumu","#phi_{#mu#mu}",50,-TMath::Pi(),TMath::Pi());
   book<TH1F>("eta_mumu","#eta_{#mu#mu}",50,-3,3);
   book<TH1F>("Pt_mumu", "P_{T,#mu#mu} [GeV]",100 , 0, 1000);
   book<TH1F>("M_mu1mu2", "M_{#mu_{1}#mu_{2}} [GeV]",50 , 0, 2500);



 }

 void ttZPrimeMuPreselectionHists::fill(const Event & event){
   // fill the histograms. Please note the comments in the header file:
   // 'hist' is used here a lot for simplicity, but it will be rather
   // slow when you have many histograms; therefore, better
   // use histogram pointers as members as in 'UHH2/common/include/ElectronHists.h'
  double weight = event.weight;
  assert(event.muons);
  int Nmuons = event.muons->size();
  vector<Muon> muons;
  for(const auto & muon : *event.muons){
    muons.push_back(muon);
  }
  if(Nmuons > 1){
    if((muons[0].charge()+muons[1].charge())== 0) hist("M_mu1mu2")->Fill((muons[0].v4()+muons[1].v4()).M(),weight);
  }
  if(Nmuons > 1){
    // cout << "Nmu=" << Nmuons << endl;
    for(int i=0; i<Nmuons; i++){
      for(int j=0; j<Nmuons; j++){
        if(j > i && ((muons[i].charge()+muons[j].charge())== 0)){
          // cout << "i=" << i << " ,j=" << j << endl;
          LorentzVector mumu = (muons[i].v4() + muons[j].v4());
          hist("M_mumu")->Fill(mumu.M(), weight);
          hist("delta_phi_mumu")->Fill(abs(deltaPhi(muons[i].v4(),muons[j].v4())),weight);
          hist("phi_mumu")->Fill(mumu.phi(),weight);
          hist("eta_mumu")->Fill(mumu.eta(),weight);
          hist("Pt_mumu")->Fill(mumu.pt(),weight);
        }
      }
    }
  }
}
