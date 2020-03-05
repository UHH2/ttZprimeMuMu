#include <iostream>
#include <memory>



#include "UHH2/core/include/AnalysisModule.h"
#include "UHH2/core/include/Event.h"

#include "UHH2/common/include/CommonModules.h"
#include "UHH2/common/include/MuonIds.h"
#include "UHH2/common/include/ElectronIds.h"
#include "UHH2/common/include/NSelections.h"
#include "UHH2/common/include/TriggerSelection.h"

#include "UHH2/ttZPrime/include/AndHists.h"
#include "UHH2/ttZPrime/include/ttZPrimeSelections.h"
#include "UHH2/ttZPrime/include/ttZPrimeMuPreselectionHists.h"




using namespace std;
using namespace uhh2;


namespace uhh2examples {

  class ttZPrimeFullSelectionModule: public AnalysisModule {
  public:

    explicit ttZPrimeFullSelectionModule(Context & ctx);
    virtual bool process(Event & event) override;

  private:
    bool is_mc, is_blind;
    unique_ptr<CommonModules> common;




    // declare the Selections to use.
    unique_ptr<Selection> trigger_sel1, trigger_sel2, mu2_sel, m_mumu_sel;

    // store the Hists collection as member variables.
    unique_ptr<Hists> h_trigger,h_cleaner,h_2mu, h_m_mumu_120, h_mumu;




    MuonId MuId;
    ElectronId EleId;







  };

  ttZPrimeFullSelectionModule::ttZPrimeFullSelectionModule(Context & ctx){
    cout << "Hello from ttZPrimeFullSelectionModule!" << endl;

    for(auto & kv : ctx.get_all()){
        cout << " " << kv.first << " = " << kv.second << endl;
        }

    is_mc = ctx.get("dataset_type") == "MC";
    is_blind = true;

    EleId = AndId<Electron>(ElectronID_Summer16_tight, PtEtaCut(30.0, 2.4));
    MuId = AndId<Muon>(MuonID(Muon::CutBasedIdTight), PtEtaCut(30.0, 2.4), MuonIso(0.15));



    // 1. set up CommonModules
    common.reset(new CommonModules());
    common->disable_lumisel();
    common->disable_jersmear();
    common->disable_jec();
    common->set_electron_id(EleId);
    common->set_muon_id(MuId);
    common->init(ctx);

    // 2. set up selections

    //FullSelection
    trigger_sel1.reset(new TriggerSelection("HLT_IsoMu24_v*")); //original: IsoMu24
    trigger_sel2.reset(new TriggerSelection("HLT_IsoTkMu24_v*")); //original: IsoMu24
    mu2_sel.reset(new NMuonSelection(2, -1, MuId));
    m_mumu_sel.reset(new MMuMUSelection(120.));


    // 3. Set up Hists classes:

    h_trigger.reset(new AndHists(ctx, "Trigger"));
    h_cleaner.reset(new AndHists(ctx, "Cleaner"));
    h_2mu.reset(new AndHists(ctx, "2Mu"));
    h_m_mumu_120.reset(new AndHists(ctx, "MMuMU120"));
    h_mumu.reset(new ttZPrimeMuPreselectionHists(ctx, "ZPrimeMuMu"));


  }
   bool ttZPrimeFullSelectionModule::process(Event & event) {

     bool pass_common = common->process(event);
     if(!pass_common) return false;
     h_cleaner->fill(event);



    // trigger
    if(!(trigger_sel1->passes(event) || trigger_sel2->passes(event))) return false;
    h_trigger->fill(event);




    if(!mu2_sel->passes(event)) return false;
    h_2mu->fill(event);


    if(!m_mumu_sel->passes(event)) return false;
    h_m_mumu_120->fill(event);
    if(is_mc || !is_blind) h_mumu->fill(event);


    return true;


    }
     UHH2_REGISTER_ANALYSIS_MODULE(ttZPrimeFullSelectionModule)

}
