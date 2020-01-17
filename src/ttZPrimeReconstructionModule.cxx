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
    unique_ptr<AnalysisModule> my_st, my_htlep, mc_lumi_weight, reco_tt_had, disc_tt_had_chi26jets, ttgenprod, disc_tt_had_chi24jets, disc_tt_had_chi25jets;
    unique_ptr<Hists> h_2mu, h_2mu_4jets , h_2mu_chi2, h_3mu, h_4mu, h_2MuTopHadReco, h_2MuTopHadRecoChi26Jets,h_2MuTopHadRecoChi24Jets,h_2MuTopHadRecoChi25Jets, h_ttgenhists,h_DRMuJet;
    unique_ptr<Hists>  h_control, h_control_4jets , h_control_chi2,  h_control_TopHadReco, h_control_TopHadReco_chi2;
    unique_ptr<Selection> m_top4jets_sel,m_top5jets_sel,m_top6jets_sel, deltaR_mu_jet_sel,njet_sel;
    Event::Handle<TTbarGen> h_ttbargen;
  };


  ttZPrimeReconstructionModule::ttZPrimeReconstructionModule(Context & ctx){
        double m_dr_max = 10.;
        double deltaR_min = 0.2;
        mc_lumi_weight.reset(new MCLumiWeight(ctx));
        my_st.reset(new STCalculator(ctx));
        my_htlep.reset(new HTlepCalculator(ctx));

        deltaR_mu_jet_sel.reset(new DRMuJetSelection(deltaR_min));
        njet_sel.reset(new NJetSelection(4, -1));

        h_DRMuJet.reset(new AndHists(ctx,"DRMuJet"));
        h_2mu.reset(new AndHists(ctx, "2Mu"));
        h_2mu_chi2.reset(new AndHists(ctx, "2MuChi2"));
        h_2mu_4jets.reset(new AndHists(ctx, "2Mu4Jets"));

//         h_3mu.reset(new AndHists(ctx, "3Mu"));
//         h_4mu.reset(new AndHists(ctx, "4MuAndMore"));
        // h_control.reset(new ttZPrimeControlHists(ctx, "Control"));
        // h_control_4jets.reset(new ttZPrimeControlHists(ctx, "Control4Jets"));
        // h_control_chi2.reset(new ttZPrimeControlHists(ctx, "ControlChi2"));
        // h_control_TopHadReco.reset(new TTbarRecoHadHypothesisHists(ctx,"Control_TopRecoHad","TTbarRecoHad","Chi2Had"));
        // h_control_TopHadReco_chi2.reset(new TTbarRecoHadHypothesisHists(ctx,"Control_TopRecoHad_chi2","TTbarRecoHad","Chi2Had"));

        reco_tt_had.reset(new TTbarRecoHad(ctx, "TTbarRecoHad"));

        disc_tt_had_chi26jets.reset(new Chi2Discriminator6Jets(ctx, "TTbarRecoHad"));
        disc_tt_had_chi24jets.reset(new Chi2Discriminator4Jets(ctx, "TTbarRecoHad"));
        disc_tt_had_chi25jets.reset(new Chi2Discriminator5Jets(ctx, "TTbarRecoHad"));
        h_2MuTopHadRecoChi26Jets.reset(new TTbarRecoHadHypothesisHists(ctx,"2Mu_TopRecoHadChi26Jets","TTbarRecoHad","Chi26Jets"));
        // h_2MuTopHadReco_chi2.reset(new TTbarRecoHadHypothesisHists(ctx,"2Mu_TopRecoHad_chi2","TTbarRecoHad","Chi2Had"));
        h_2MuTopHadRecoChi24Jets.reset(new TTbarRecoHadHypothesisHists(ctx,"2Mu_TopRecoHadChi24Jets","TTbarRecoHad","Chi24Jets"));
        h_2MuTopHadRecoChi25Jets.reset(new TTbarRecoHadHypothesisHists(ctx,"2Mu_TopRecoHadChi25Jets","TTbarRecoHad","Chi25Jets"));


        // disc_tt_had.reset(new TopDRMCDiscriminatorHad(ctx, "TTbarRecoHad"));
        // h_2MuTopHadReco.reset(new TTbarRecoHadHypothesisHists(ctx,"2Mu_TopRecoHad","TTbarRecoHad","TopDRMCHad"));
        m_top4jets_sel.reset(new TopDRMCHadSelection(ctx,5,"TTbarRecoHad","Chi24Jets"));
        m_top5jets_sel.reset(new TopDRMCHadSelection(ctx,10,"TTbarRecoHad","Chi25Jets"));
        m_top6jets_sel.reset(new TopDRMCHadSelection(ctx,20,"TTbarRecoHad","Chi26Jets"));

        ttgenprod.reset(new TTbarGenProducer(ctx, "ttbargen", false));
        h_ttgenhists.reset(new TTbarGenHists(ctx, "ttgenhists"));

        h_ttbargen = ctx.get_handle<TTbarGen>("ttbargen");

  }


  bool ttZPrimeReconstructionModule::process(Event & event) {
    mc_lumi_weight->process(event);
    if(!deltaR_mu_jet_sel->passes(event)) return false;
    h_DRMuJet->fill(event);


    // if (event.muons->size() ==1 && event.electrons->size() == 1){
    //   h_control->fill(event);
    //   if(!njet_sel->passes(event)) return false;
    //   h_control_4jets->fill(event);
    //   reco_tt_had->process(event);
    //   disc_tt_had->process(event);
    //   h_control_TopHadReco->fill(event);
    //   if(!m_topdrmc_sel->passes(event)) return false;
    //   h_control_chi2->fill(event);
    //   h_control_TopHadReco_chi2->fill(event);
    // }

    my_st->process(event);
    my_htlep->process(event);
    //
    int Nmuons = event.muons->size();
    if(Nmuons == 2){
        h_2mu->fill(event);
        if(!njet_sel->passes(event)) return false;
        h_2mu_4jets->fill(event);
        reco_tt_had->process(event);
        if(event.jets->size() >= 6)
        {

          disc_tt_had_chi26jets->process(event);
          h_2MuTopHadRecoChi26Jets->fill(event);
          // ttgenprod->process(event);
          // h_ttgenhists->fill(event);
          if(!m_top6jets_sel->passes(event)) return false;

          // h_2MuTopHadReco_chi2->fill(event);
        }
        if(event.jets->size() == 5){
          disc_tt_had_chi25jets->process(event);
          h_2MuTopHadRecoChi25Jets->fill(event);
          if(!m_top6jets_sel->passes(event)) return false;
        }
        if(event.jets->size() == 4){
          disc_tt_had_chi24jets->process(event);
          h_2MuTopHadRecoChi24Jets->fill(event);
          if(!m_top6jets_sel->passes(event)) return false;
        }
        h_2mu_chi2->fill(event);


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
