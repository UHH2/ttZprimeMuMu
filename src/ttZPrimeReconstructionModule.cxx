#include "UHH2/core/include/AnalysisModule.h"
#include "UHH2/core/include/Event.h"
#include "UHH2/ttZPrime/include/AndHists.h"
#include "UHH2/ttZPrime/include/MyEventVariables.h"
#include "UHH2/common/include/MCWeight.h"
#include "UHH2/ttZPrime/include/ttZPrimeSelections.h"
#include "UHH2/ttZPrime/include/ttZPrimeControlHists.h"

using namespace std;
using namespace uhh2;

namespace uhh2examples {
  class ttZPrimeReconstructionModule: public AnalysisModule {
  public:
    explicit ttZPrimeReconstructionModule(Context & ctx);
    virtual bool process(Event & event) override;

  private:
    unique_ptr<AnalysisModule> my_st, my_htlep, mc_lumi_weight;
    unique_ptr<Hists> h_2mu, h_3mu, h_4mu, h_control;
    unique_ptr<Selection> m_mu1mu2_sel;
  };
  ttZPrimeReconstructionModule::ttZPrimeReconstructionModule(Context & ctx){
        double m_mu1mu2_min = 110. ;
        mc_lumi_weight.reset(new MCLumiWeight(ctx));
        my_st.reset(new STCalculator(ctx));
        my_htlep.reset(new HTlepCalculator(ctx));
        h_2mu.reset(new AndHists(ctx, "2Mu"));
        h_3mu.reset(new AndHists(ctx, "3Mu"));
        h_4mu.reset(new AndHists(ctx, "4MuAndMore"));
        h_control.reset(new ttZPrimeControlHists(ctx, "Control"));
        m_mu1mu2_sel.reset(new MMuMUSelection(m_mu1mu2_min));
  }
  bool ttZPrimeReconstructionModule::process(Event & event) {
    mc_lumi_weight->process(event);
    if (event.muons->size() ==1 && event.electrons->size() == 1){
      h_control->fill(event);
    }
    if(!m_mu1mu2_sel->passes(event)) return false;
    my_st->process(event);
    my_htlep->process(event);
    int Nmuons = event.muons->size();
    if(Nmuons == 2){
      h_2mu->fill(event);
    }
    else if(Nmuons == 3)
    {
      h_3mu->fill(event);
    }
    else if(Nmuons >= 4)
    {
      h_4mu->fill(event);
    }

    return true;
  }
  UHH2_REGISTER_ANALYSIS_MODULE(ttZPrimeReconstructionModule)
}
