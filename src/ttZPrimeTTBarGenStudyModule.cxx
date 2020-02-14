#include "UHH2/ttZPrime/include/ttZPrimeTTBarGenStudyHists.h"
#include "UHH2/core/include/AnalysisModule.h"
#include "UHH2/common/include/TTbarGen.h"
#include "UHH2/common/include/MCWeight.h"
#include "UHH2/common/include/NSelections.h"
#include "UHH2/ttZPrime/include/TTbarRecoHadHypothesisDiscriminators.h"
#include "UHH2/ttZPrime/include/TTbarRecoHad.h"
#include "UHH2/ttZPrime/include/TTbarRecoHadHypothesisHists.h"
#include "UHH2/ttZPrime/include/ttZPrimeTTBarGenStudyHists.h"
#include "UHH2/common/include/NSelections.h"
#include "UHH2/common/include/MuonIds.h"
#include "UHH2/ttZPrime/include/ttZPrimeSelections.h"

using namespace std;
using namespace uhh2;


namespace uhh2examples {
  class ttZPrimeTTBarGenStudyModule: public AnalysisModule {
    public:
      explicit ttZPrimeTTBarGenStudyModule(Context & ctx);
      virtual bool process(Event & event) override;
    private:
      unique_ptr<AnalysisModule> ttgenprod, mc_lumi_weight, reco_tt_had, disc_tt_had_cm, disc_tt_had_chi2,disc_tt_had_cm4jets,disc_tt_had_chi24jets,disc_tt_had_cm5jets,disc_tt_had_chi25jets, disc_tt_had_dr;
      Event::Handle<TTbarGen> h_ttbargen;
      unique_ptr<Selection> m_topdrmc_sel,m_topdrmc4jets_sel, m_topdrmc5jets_sel, njet_sel,nmuon_sel, jetQ_sel,met_2mu_sel;
      unique_ptr<Hists> h_ttbargen_study, h_2MuTopHadRecoCM, h_2MuTopHadRecoChi2,h_2MuTopHadRecoCM4Jets,h_2MuTopHadRecoChi24Jets,h_ttbargen_study4jets,h_2MuTopHadRecoCM5Jets,h_2MuTopHadRecoChi25Jets,h_ttbargen_study5jets, h_2MuTopHadRecoDR;
      MuonId MuId;
  };

  ttZPrimeTTBarGenStudyModule::ttZPrimeTTBarGenStudyModule(Context & ctx){
    ctx.undeclare_all_event_output();

    double deltaR_max = 1.0;
    // int nmatchs = 1;
    MuId = AndId<Muon>(MuonID(Muon::CutBasedIdTight), PtEtaCut(30.0, 2.4), MuonIso(0.15));

    mc_lumi_weight.reset(new MCLumiWeight(ctx));

    ttgenprod.reset(new TTbarGenProducer(ctx, "ttbargen", false));
    njet_sel.reset(new NJetSelection(4, -1));
    nmuon_sel.reset(new NMuonSelection(2, 2));
    met_2mu_sel.reset(new METSelection(100.));
    h_ttbargen = ctx.get_handle<TTbarGen>("ttbargen");
    h_ttbargen_study.reset(new ttZPrimeTTBarGenStudyHists(ctx, "TTbarGenStudy","TTbarRecoHad","Chi2Had","CorrectMatchHad"));

    reco_tt_had.reset(new TTbarRecoHad(ctx, "TTbarRecoHad"));

    // disc_tt_had.reset(new TopDRMCDiscriminatorHad(ctx, "TTbarRecoHad"));

    disc_tt_had_dr.reset(new TopDRMCDiscriminatorHad(ctx, "TTbarRecoHad"));
    h_2MuTopHadRecoDR.reset(new TTbarRecoHadHypothesisHists(ctx,"MCTopRecoHadDR","TTbarRecoHad","TopDRMCHad"));
    disc_tt_had_cm.reset(new CorrectMatchDiscriminatorHad(ctx, "TTbarRecoHad"));
    h_2MuTopHadRecoCM.reset(new TTbarRecoHadHypothesisHists(ctx,"2Mu_TopRecoHadCM","TTbarRecoHad","CorrectMatchHad"));

    disc_tt_had_cm4jets.reset(new CorrectMatchDiscriminator4Jets(ctx, "TTbarRecoHad"));
    h_2MuTopHadRecoCM4Jets.reset(new TTbarRecoHadHypothesisHists(ctx,"2Mu_TopRecoHadCM4Jets","TTbarRecoHad","CorrectMatch4Jets"));
    disc_tt_had_chi24jets.reset(new Chi2Discriminator4Jets(ctx, "TTbarRecoHad"));
    h_2MuTopHadRecoChi24Jets.reset(new TTbarRecoHadHypothesisHists(ctx,"2Mu_TopRecoHadChi24Jets","TTbarRecoHad","Chi24Jets"));
    h_ttbargen_study4jets.reset(new ttZPrimeTTBarGenStudyHists(ctx, "TTbarGenStudy4Jets","TTbarRecoHad","Chi24Jets","CorrectMatch4Jets"));
    m_topdrmc4jets_sel.reset(new TopDRMCHadSelection(ctx,deltaR_max,"TTbarRecoHad","CorrectMatch4Jets"));

    disc_tt_had_cm5jets.reset(new CorrectMatchDiscriminator5Jets(ctx, "TTbarRecoHad"));
    h_2MuTopHadRecoCM5Jets.reset(new TTbarRecoHadHypothesisHists(ctx,"2Mu_TopRecoHadCM5Jets","TTbarRecoHad","CorrectMatch5Jets"));
    disc_tt_had_chi25jets.reset(new Chi2Discriminator5Jets(ctx, "TTbarRecoHad"));
    h_2MuTopHadRecoChi25Jets.reset(new TTbarRecoHadHypothesisHists(ctx,"2Mu_TopRecoHadChi25Jets","TTbarRecoHad","Chi25Jets"));
    h_ttbargen_study5jets.reset(new ttZPrimeTTBarGenStudyHists(ctx, "TTbarGenStudy5Jets","TTbarRecoHad","Chi25Jets","CorrectMatch5Jets"));
    m_topdrmc5jets_sel.reset(new TopDRMCHadSelection(ctx,deltaR_max,"TTbarRecoHad","CorrectMatch5Jets"));

    m_topdrmc_sel.reset(new TopDRMCHadSelection(ctx,deltaR_max,"TTbarRecoHad","CorrectMatchHad"));

    disc_tt_had_chi2.reset(new Chi2Discriminator6Jets(ctx, "TTbarRecoHad"));
    h_2MuTopHadRecoChi2.reset(new TTbarRecoHadHypothesisHists(ctx,"2Mu_TopRecoHadChi2","TTbarRecoHad","Chi2Had"));
    // jetQ_sel.reset(new JetQuarkMatchingSelection(ctx, nmatchs,"TTbarRecoHad","Chi2Had"));



  }

  bool ttZPrimeTTBarGenStudyModule::process(Event & event){
    if(!nmuon_sel->passes(event)) return false;
    if(!met_2mu_sel->passes(event)) return false;
    if(!njet_sel->passes(event)) return false;
    ttgenprod->process(event);
    if(!event.is_valid(h_ttbargen)) return false;

    // mc_lumi_weight->process(event);
    reco_tt_had->process(event);
    if(event.jets->size() == 4)
    {
      disc_tt_had_dr->process(event);
      h_2MuTopHadRecoDR->fill(event);
      disc_tt_had_cm4jets->process(event);
      h_2MuTopHadRecoCM4Jets->fill(event);
      if(!m_topdrmc4jets_sel->passes(event)) return false;
      disc_tt_had_chi24jets->process(event);
      h_2MuTopHadRecoChi24Jets->fill(event);
      h_ttbargen_study4jets->fill(event);
    }
    if(event.jets->size() == 5)
    {
      disc_tt_had_cm5jets->process(event);
      h_2MuTopHadRecoCM5Jets->fill(event);
      if(!m_topdrmc5jets_sel->passes(event)) return false;
      disc_tt_had_chi25jets->process(event);
      h_2MuTopHadRecoChi25Jets->fill(event);
      h_ttbargen_study5jets->fill(event);
    }


    if(event.jets->size() >= 6)
    {
      disc_tt_had_cm->process(event);
      // if(!jetQ_sel->passes(event))return false;
      h_2MuTopHadRecoCM->fill(event);

      if(!m_topdrmc_sel->passes(event)) return false;
      disc_tt_had_chi2->process(event);
      h_2MuTopHadRecoChi2->fill(event);
      h_ttbargen_study->fill(event);
    }

	// std::cout << "##############" << endl;
	// std::cout << "NEXT EVENT!!!!" <<endl;
  // std::cout << "##############" << endl;
    return true;
  }


  UHH2_REGISTER_ANALYSIS_MODULE(ttZPrimeTTBarGenStudyModule)
}
