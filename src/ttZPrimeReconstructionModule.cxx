#include "UHH2/core/include/AnalysisModule.h"
#include "UHH2/core/include/Event.h"
#include "UHH2/ttZPrime/include/AndHists.h"
#include "UHH2/ttZPrime/include/MyEventVariables.h"

using namespace std;
using namespace uhh2;

namespace uhh2examples {
  class ttZPrimeReconstructionModule: public AnalysisModule {
  public:
    explicit ttZPrimeReconstructionModule(Context & ctx);
    virtual bool process(Event & event) override;

  private:
    unique_ptr<AnalysisModule> my_st, my_htlep;
    unique_ptr<Hists> h_2mu, h_3mu, h_4mu;
  };
  ttZPrimeReconstructionModule::ttZPrimeReconstructionModule(Context & ctx){
        my_st.reset(new STCalculator(ctx));
        my_htlep.reset(new HTlepCalculator(ctx));
        h_2mu.reset(new AndHists(ctx, "2Mu"));
        h_3mu.reset(new AndHists(ctx, "3Mu"));
        h_4mu.reset(new AndHists(ctx, "4MuAndMore"));
  }
  bool ttZPrimeReconstructionModule::process(Event & event) {
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
