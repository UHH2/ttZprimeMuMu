#include "UHH2/core/include/AnalysisModule.h"
#include "UHH2/core/include/Event.h"
#include "UHH2/common/include/CleaningModules.h"

#include "UHH2/ttZPrime/include/AndHists.h"
#include "UHH2/common/include/MCWeight.h"
#include "UHH2/common/include/MuonIds.h"
#include "UHH2/common/include/NSelections.h"

#include "UHH2/ttZPrime/include/ttZPrimeControl3LHists.h"
#include "UHH2/ttZPrime/include/ttZPrimeGenBHists.h"
#include "UHH2/ttZPrime/include/ttZPrimeSelections.h"
#include "UHH2/ttZPrime/include/ttZPrimeWeights.h"



using namespace std;
using namespace uhh2;

namespace uhh2examples {
  class ttZPrimeControlModule: public AnalysisModule {
  public:
    explicit ttZPrimeControlModule(Context & ctx);
    virtual bool process(Event & event) override;

  private:
    bool is_mc;
    unique_ptr<AnalysisModule> mc_lumi_weight, mc_pu_reweight, mu_fake_rate_weight;
    unique_ptr<Selection> nmu_tight_sel, mu1mu2_sel, m_mumu_sel, mu3_loose_sel, mu3_tight_sel;

    unique_ptr<Hists> h_2mu;
    unique_ptr<Hists> h_3mu, h_3mu_loose, h_3mu_tight,  h_3mu_control, h_3mu_control_loose, h_3mu_control_tight;
    unique_ptr<Hists> h_bgen, h_3mu_loose_bgen, h_3mu_tight_bgen;
    unique_ptr<Hists> h_emu, hemu_control, h_emumu_tight,  h_emumu_tight_control;
    unique_ptr<MuonCleaner> mouncleaner_tight;
    MuonId MuIdTight, MuIdLoose;
    ElectronId EleId;
  };


  ttZPrimeControlModule::ttZPrimeControlModule(Context & ctx){
        ctx.undeclare_all_event_output();

        is_mc = ctx.get("dataset_type") == "MC";
        if (is_mc)
        {
          mc_lumi_weight.reset(new MCLumiWeight(ctx));
          mc_pu_reweight.reset(new MCPileupReweight(ctx));
        }
        mu_fake_rate_weight.reset(new MuFakeRateWeight("/nfs/dust/cms/user/tiedemab/CMSSW_10_2_X/CMSSW_10_2_10/src/UHH2/ttZPrime/scripts/TTBar_FakeRateMap.txt",2));

        MuIdTight = AndId<Muon>(MuonID(Muon::CutBasedIdTight), PtEtaCut(30.0, 2.4), MuonIso(0.15));
        MuIdLoose = AndId<Muon>(MuonID(Muon::CutBasedIdLoose), PtEtaCut(30.0, 2.4), MuonIso(0.15));
        mouncleaner_tight.reset(new MuonCleaner(MuIdTight));



        nmu_tight_sel.reset(new NMuonSelection(2, -1, MuIdTight));
        mu1mu2_sel.reset(new NMuIDSelection(MuIdTight, 2));
        m_mumu_sel.reset(new MMuMUSelection(120.));
        mu3_loose_sel.reset(new NMuonSelection(3, 3, MuIdLoose));
        mu3_tight_sel.reset(new NMuonSelection(3, 3, MuIdTight));


        h_bgen.reset(new ttZPrimeGenBHists(ctx, "GenBQuarks"));
        h_3mu_tight_bgen.reset(new ttZPrimeGenBHists(ctx, "GenBQuarksTight"));
        h_3mu_loose_bgen.reset(new ttZPrimeGenBHists(ctx, "GenBQuarksLoose"));

        h_2mu.reset(new AndHists(ctx, "2Mu"));




        h_3mu_loose.reset(new AndHists(ctx, "3MuLoose"));
        h_3mu_tight.reset(new AndHists(ctx, "3MuTight"));


        h_3mu_control_tight.reset(new ttZPrimeControl3LHists(ctx,"3MuControlTight"));
        h_3mu_control_loose.reset(new ttZPrimeControl3LHists(ctx,"3MuControlLoose"));





  }


  bool ttZPrimeControlModule::process(Event & event) {

   if(is_mc)
   {
     mc_lumi_weight->process(event);
     mc_pu_reweight->process(event);
   }



   if(!nmu_tight_sel->passes(event)) return false;
   if(!mu1mu2_sel->passes(event)) return false;
   if(!m_mumu_sel->passes(event)) return false;

   h_2mu->fill(event);
   h_bgen->fill(event);
  
  
   if(!mu3_loose_sel->passes(event)) return false;
   auto old_weight = event.weight;  
   std::cout << "Original event weight: " << event.weight << "\n";
   mu_fake_rate_weight->process(event);
   std::cout << "Scaled event weight: " << event.weight << "\n";
  
   h_3mu_control_loose->fill(event);
   h_3mu_loose->fill(event);
   h_3mu_loose_bgen->fill(event);
  

   event.weight = old_weight;
   std::cout << "Back scaled event weight: " << event.weight << "\n";

   if(!mu3_tight_sel->passes(event)) return false;  

   h_3mu_control_tight->fill(event);
   h_3mu_tight->fill(event);
   h_3mu_tight_bgen->fill(event);
  



   return true;


  }
  UHH2_REGISTER_ANALYSIS_MODULE(ttZPrimeControlModule)
}
