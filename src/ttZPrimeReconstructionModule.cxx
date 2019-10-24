#include "UHH2/core/include/AnalysisModule.h"
#include "UHH2/core/include/Event.h"
#include "UHH2/ttZPrime/include/AndHists.h"
#include "UHH2/ttZPrime/include/MyEventVariables.h"
#include "UHH2/common/include/MCWeight.h"
#include "UHH2/ttZPrime/include/ttZPrimeSelections.h"
#include "UHH2/ttZPrime/include/ttZPrimeControlHists.h"
#include "UHH2/common/include/TTbarGen.h"
#include "UHH2/common/include/TTbarGenHists.h"
#include "UHH2/common/include/NSelections.h"
#include "UHH2/ttZPrime/include/TTbarRecoHadHypothesisDiscriminators.h"
#include "UHH2/ttZPrime/include/TTbarRecoHad.h"
#include "UHH2/ttZPrime/include/TTbarRecoHadHypothesisHists.h"




using namespace std;
using namespace uhh2;

namespace uhh2examples {
  class ttZPrimeReconstructionModule: public AnalysisModule {
  public:
    explicit ttZPrimeReconstructionModule(Context & ctx);
    virtual bool process(Event & event) override;

  private:
    unique_ptr<AnalysisModule> my_st, my_htlep, mc_lumi_weight, reco_tt_had, disc_tt_had, ttgenprod;
    unique_ptr<Hists> h_2mu, h_2mu_6jets , h_2mu_chi2, h_3mu, h_4mu, h_control, h_2MuTopHadReco, h_2MuTopHadReco_chi2, h_ttgenhists;
    unique_ptr<Selection> m_topdrmc_sel, deltaR_mu_jet_sel,njet_sel;
    Event::Handle<TTbarGen> h_ttbargen;
  };


  ttZPrimeReconstructionModule::ttZPrimeReconstructionModule(Context & ctx){
        double m_dr_max = 20.;
        double deltaR_min = 0.2;
        mc_lumi_weight.reset(new MCLumiWeight(ctx));
        my_st.reset(new STCalculator(ctx));
        my_htlep.reset(new HTlepCalculator(ctx));

        deltaR_mu_jet_sel.reset(new DRMuJetSelection(deltaR_min));
        njet_sel.reset(new NJetSelection(6, -1));

        h_2mu.reset(new AndHists(ctx, "2Mu"));
        h_2mu_chi2.reset(new AndHists(ctx, "2MuChi2"));
        h_2mu_6jets.reset(new AndHists(ctx, "2Mu6Jets"));
//         h_3mu.reset(new AndHists(ctx, "3Mu"));
//         h_4mu.reset(new AndHists(ctx, "4MuAndMore"));
        h_control.reset(new ttZPrimeControlHists(ctx, "Control"));

        reco_tt_had.reset(new TTbarRecoHad(ctx, "TTbarRecoHad"));

        disc_tt_had.reset(new Chi2DiscriminatorHad(ctx, "TTbarRecoHad"));
        h_2MuTopHadReco.reset(new TTbarRecoHadHypothesisHists(ctx,"2Mu_TopRecoHad","TTbarRecoHad","Chi2Had"));
        h_2MuTopHadReco_chi2.reset(new TTbarRecoHadHypothesisHists(ctx,"2Mu_TopRecoHad_chi2","TTbarRecoHad","Chi2Had"));


        // disc_tt_had.reset(new TopDRMCDiscriminatorHad(ctx, "TTbarRecoHad"));
        // h_2MuTopHadReco.reset(new TTbarRecoHadHypothesisHists(ctx,"2Mu_TopRecoHad","TTbarRecoHad","TopDRMCHad"));
        m_topdrmc_sel.reset(new TopDRMCHadSelection(ctx,m_dr_max,"TTbarRecoHad","Chi2Had"));

        ttgenprod.reset(new TTbarGenProducer(ctx, "ttbargen", false));
        // h_ttgenhists.reset(new TTbarGenHists(ctx, "ttgenhists"));

        h_ttbargen = ctx.get_handle<TTbarGen>("ttbargen");

  }


  bool ttZPrimeReconstructionModule::process(Event & event) {
    if(!deltaR_mu_jet_sel->passes(event)) return false;
    mc_lumi_weight->process(event);

    if (event.muons->size() ==1 && event.electrons->size() == 1){
      h_control->fill(event);
    }

    my_st->process(event);
    my_htlep->process(event);
    ttgenprod->process(event);
    int Nmuons = event.muons->size();
    if(Nmuons == 2){
        h_2mu->fill(event);
        if(!njet_sel->passes(event)) return false;
        h_2mu_6jets->fill(event);
        reco_tt_had->process(event);
        disc_tt_had->process(event);

        h_2MuTopHadReco->fill(event);
        // h_ttgenhists->fill(event);
        if(!m_topdrmc_sel->passes(event)) return false;
        h_2mu_chi2->fill(event);
        h_2MuTopHadReco_chi2->fill(event);

    }
//     else if(Nmuons == 3)
//     {
//       h_3mu->fill(event);
//     }
//     else if(Nmuons >= 4)
//     {
//       h_4mu->fill(event);
//     }

    return true;


  }
  UHH2_REGISTER_ANALYSIS_MODULE(ttZPrimeReconstructionModule)
}
