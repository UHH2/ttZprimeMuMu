#include "UHH2/common/include/MuonHists.h"
#include "UHH2/common/include/Utils.h"
#include "UHH2/core/include/Event.h"
#include "UHH2/core/include/LorentzVector.h"

#include "UHH2/ttZPrime/include/ttZPrimeMuonHists.h"

#include "TH1F.h"
#include "TH2F.h"

#include <vector>

using namespace uhh2;
using namespace std;


ThirdMuonHists::ThirdMuonHists(Context & ctx, const string & dirname): Hists(ctx, dirname){
  pt_3 = book<TH1F>("pt_3","p_{T} muon 3 [GeV]",100,0,500);
  eta_3 = book<TH1F>("eta_3","#eta muon 3",100,-3,3);
  phi_3 = book<TH1F>("phi_3","#phi muon 3",100,-M_PI,M_PI);
  isolation_3 = book<TH1F>("isolation_3","relIso muon 3",100,0,0.5);
  charge_3 = book<TH1F>("charge_3", "muon 2 charge",3,-1.5,1.5);
  ThirdMuIsoVsEta = book<TH2F>("ThirdMuIsoVsEta",";Iso muon 3; #eta muon 3",20,0.,0.5,20,-3,3);
  M_mu2mu3 = book<TH1F>("M_mu2mu3","M_{#mu_{2}#mu_{3}} [GeV]" ,50 , 0, 2500);
}
void ThirdMuonHists::fill(const Event & event){
    assert(event.muons);    
    auto w = event.weight;
    if(event.muons->size() > 2)
    {
        const auto & muon = (*event.muons)[2];
        pt_3->Fill(muon.pt(), w);
        eta_3->Fill(muon.eta(), w);
        phi_3->Fill(muon.phi(), w);
        isolation_3->Fill(muon.relIso(), w);
        charge_3->Fill(muon.charge(), w);
        ThirdMuIsoVsEta->Fill(muon.relIso(),muon.eta(), w);
        M_mu2mu3->Fill((muon.v4()+event.muons->at(1).v4()).M());
    }
}
