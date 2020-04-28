#include "UHH2/core/include/AnalysisModule.h"
#include "UHH2/core/include/Event.h"
#include "UHH2/common/include/CleaningModules.h"
#include "UHH2/common/include/MCWeight.h"
#include "UHH2/common/include/MuonIds.h"
#include "UHH2/common/include/NSelections.h"
#include "UHH2/common/include/ElectronIds.h"

#include "UHH2/ttZPrime/include/AndHists.h"
#include "UHH2/ttZPrime/include/ttZPrimeControlHists.h"
#include "UHH2/ttZPrime/include/ttZPrimeSelections.h"
#include "UHH2/ttZPrime/include/ttZPrimeControl3LHists.h"

using namespace std;
using namespace uhh2;

namespace uhh2examples {
  class ttZPrimeMuFakeRateModule: public AnalysisModule{
  public:
    explicit ttZPrimeMuFakeRateModule(Context & ctx);
    virtual bool process(Event & event) override;
  private:
    bool is_mc;
    unique_ptr<AnalysisModule> mc_lumi_weight, mc_pu_reweight;
    unique_ptr<Selection>  nele_sel, oneTightMuSel, m_muele_sel, nmu_loose_sel, nmu_tight_sel;
    unique_ptr<Hists> h_e_control, h_emu, h_emu_control, h_emumu_tight,  h_emumu_tight_control, h_emumu_loose,  h_emumu_loose_control;
    unique_ptr<Hists> h_emumu_loose_control_gen, h_emumu_tight_control_gen;
    // unique_ptr<ElectronCleaner> electroncleaner;
    // unique_ptr<MuonCleaner> mouncleaner_tight;
    MuonId MuIdTight, MuIdLoose;
    ElectronId EleId;
  };

  ttZPrimeMuFakeRateModule::ttZPrimeMuFakeRateModule(Context & ctx){
    ctx.undeclare_all_event_output();
    is_mc = ctx.get("dataset_type") == "MC";
    if (is_mc)
    {
      mc_lumi_weight.reset(new MCLumiWeight(ctx));
      mc_pu_reweight.reset(new MCPileupReweight(ctx));
    }
    MuIdTight = AndId<Muon>(MuonID(Muon::CutBasedIdTight), PtEtaCut(30.0, 2.4), MuonIso(0.15));
    MuIdLoose = AndId<Muon>(MuonID(Muon::CutBasedIdLoose), PtEtaCut(30.0, 2.4), MuonIso(0.25));
    EleId = AndId<Electron>(ElectronID_Summer16_tight, PtEtaCut(30.0, 2.4));

    // electroncleaner.reset(new ElectronCleaner(EleId));
    // mouncleaner_tight.reset(new MuonCleaner(MuIdTight));

    nele_sel.reset(new NElectronSelection(1, 1, EleId));
    oneTightMuSel.reset(new NMuIDSelection(MuIdTight,1));
    // m_muele_sel.reset(new MEleMuSelection(120.));
    nmu_loose_sel.reset(new NMuonSelection(2, 2, MuIdLoose));
    nmu_tight_sel.reset(new NMuonSelection(2, 2, MuIdTight));

    h_e_control.reset(new AndHists(ctx, "ElectronControl"));
    h_emu_control.reset(new AndHists(ctx, "ElectronPlusMuControl"));
    h_emu.reset(new ttZPrimeControlHists(ctx,"ElectronPlusMu"));
    h_emumu_loose_control.reset(new AndHists(ctx, "ElectronPlusMuMuLooseControl"));
    h_emumu_loose_control_gen.reset(new ttZPrimeControl3LHists(ctx, "ElectronPlusMuMuLooseControlGen"));
    h_emumu_loose.reset(new ttZPrimeControlHists(ctx,"ElectronPlusMuMuLoose"));
    h_emumu_tight_control.reset(new AndHists(ctx, "ElectronPlusMuMuTightControl"));
    h_emumu_tight_control_gen.reset(new ttZPrimeControl3LHists(ctx, "ElectronPlusMuMuTightControlGen"));
    h_emumu_tight.reset(new ttZPrimeControlHists(ctx,"ElectronPlusMuMuTight"));

  }


  bool ttZPrimeMuFakeRateModule::process(Event & event){
    if(is_mc)
    {
      mc_lumi_weight->process(event);
      mc_pu_reweight->process(event);
    }

    if(!nele_sel->passes(event)) return false;
    // electroncleaner->process(event);

    h_e_control->fill(event);

    if(!oneTightMuSel->passes(event)) return false;
    // if(!m_muele_sel->passes(event)) return false;
    h_emu_control->fill(event);
    h_emu->fill(event);

    if(!nmu_loose_sel->passes(event)) return false; //Möglicherweise muss der Cut geändert werden
    h_emumu_loose_control->fill(event);
    h_emumu_loose->fill(event);
    if(is_mc)h_emumu_loose_control_gen->fill(event);

    // mouncleaner_tight->process(event);
    if(!nmu_tight_sel->passes(event)) return false;
    h_emumu_tight_control->fill(event);
    h_emumu_tight->fill(event);
    if(is_mc)h_emumu_tight_control_gen->fill(event);

    return true;

  }
  UHH2_REGISTER_ANALYSIS_MODULE(ttZPrimeMuFakeRateModule)
}
