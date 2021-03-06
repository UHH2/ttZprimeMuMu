#include <iostream>
#include <memory>



#include "UHH2/core/include/AnalysisModule.h"
#include "UHH2/core/include/Event.h"

#include "UHH2/common/include/CommonModules.h"
#include "UHH2/common/include/CleaningModules.h"
#include "UHH2/common/include/JetCorrections.h"
#include "UHH2/common/include/JetHists.h"
#include "UHH2/common/include/ElectronHists.h"
#include "UHH2/common/include/ElectronIds.h"
#include "UHH2/common/include/MuonHists.h"
#include "UHH2/common/include/MuonIds.h"
#include "UHH2/common/include/EventHists.h"
#include "UHH2/common/include/LuminosityHists.h"
#include "UHH2/common/include/LumiSelection.h"
#include "UHH2/common/include/NSelections.h"
#include "UHH2/common/include/PrintingModules.h"
#include "UHH2/common/include/TriggerSelection.h"
#include "UHH2/common/include/MCWeight.h"
#include "UHH2/common/include/AdditionalSelections.h"

#include "UHH2/ttZPrime/include/AndHists.h"
// #include "UHH2/ttZPrime/include/MyEventVariables.h"
#include "UHH2/ttZPrime/include/ttZPrimeSelections.h"




using namespace std;
using namespace uhh2;


namespace uhh2examples {

  class ttZPrimePreselectionModule: public AnalysisModule {
  public:

    explicit ttZPrimePreselectionModule(Context & ctx);
    virtual bool process(Event & event) override;

  private:

    unique_ptr<CommonModules> common;
    //unique_ptr<AnalysisModule> Muon_printer, Electron_printer, Jet_printer;

    unique_ptr<JetCleaner> jetcleaner;
    unique_ptr<JetLeptonCleaner> jetleptoncleaner;
    unique_ptr<MuonCleaner> muoncleaner;
    unique_ptr<MuonCleaner> muoncleaner_iso;
    unique_ptr<ElectronCleaner> electroncleaner;

    unique_ptr<AnalysisModule> genjet_cleaner,  syst_module, my_st, my_htlep; //mc_lumi_weight;
        // declare the Selections to use.
    unique_ptr<Selection> njet_sel, nmuon_sel, n_gen_muon_sel, nele_sel, n_gen_ele_sel, st_sel, lumi_sel, mu1_sel, trigger_sel, trigger_sel1, trigger_sel2, mttbargen_sel,nbjet_sel;

    // store the Hists collection as member variables.
    unique_ptr<Hists> h_nocuts,h_trigger, h_lumi,h_cleaner,h_1mu, h_2jets,h_2bjets, h_st;




    MuonId MuId, MuIdLoose;
    ElectronId EleId;
    JetId Btag_loose, Jet_ID;
    JetId Btag_medium, Btag_tight;


    bool is_mc, is_foreff;
    TString Sys_EleFakeRate, path_EleFakeRate, Sys_MuFakeRate, path_MuFakeRate;

    uhh2::Event::Handle<vector<Jet>> h_raw_jets_ele, h_raw_jets_mu;
    uhh2::Event::Handle<vector<Particle>> h_raw_genjets_ele, h_raw_genjets_mu;


  };

  ttZPrimePreselectionModule::ttZPrimePreselectionModule(Context & ctx){
    cout << "Hello from ttZPrimePreselectionModule!" << endl;

    for(auto & kv : ctx.get_all()){
        cout << " " << kv.first << " = " << kv.second << endl;
        }

    // is_mc = ctx.get("dataset_type") == "MC";
    // double st_min = 600.;


    // mc_lumi_weight.reset(new MCLumiWeight(ctx));

    EleId = AndId<Electron>(ElectronID_Summer16_loose, PtEtaCut(30.0, 2.4));
    MuIdLoose = AndId<Muon>(MuonID(Muon::CutBasedIdLoose), PtEtaCut(30.0, 2.4), MuonIso(0.25));
    MuId = AndId<Muon>(MuonID(Muon::CutBasedIdTight), PtEtaCut(30.0, 2.4), MuonIso(0.15));

    Jet_ID = AndId<Jet>(JetPFID(JetPFID::WP_TIGHT_CHS), PtEtaCut(30.0, 2.4));

    Btag_loose = DeepJetBTag(DeepJetBTag::wp::WP_LOOSE);
    Btag_medium = DeepJetBTag(DeepJetBTag::wp::WP_MEDIUM);
    Btag_tight = DeepJetBTag(DeepJetBTag::wp::WP_TIGHT);


    common.reset(new CommonModules());
    common->switch_jetlepcleaner(true);
    common->set_jet_id(Jet_ID);
    common->set_electron_id(EleId);
    common->set_muon_id(MuIdLoose);
    common->switch_jetPtSorter();
    common->init(ctx);
    // genjet_cleaner.reset(new GenJetCleaner(ctx, 30.0, 2.4));
    // jetcleaner.reset(new JetCleaner(ctx,Jet_ID));
    // electroncleaner.reset(new ElectronCleaner(EleId));
    // muoncleaner.reset(new MuonCleaner(MuId));
    // syst_module.reset(new MCScaleVariation(ctx));
    // my_st.reset(new STCalculator(ctx));
    // my_htlep.reset(new HTlepCalculator(ctx));

    // 2. set up selections

    //Preselection
    // trigger_sel1.reset(new TriggerSelection("HLT_IsoMu24_v*")); //original: IsoMu24
    // trigger_sel2.reset(new TriggerSelection("HLT_IsoTkMu24_v*")); //original: IsoMu24
    njet_sel.reset(new NJetSelection(2, -1));
    mu1_sel.reset(new NMuonSelection(1, -1, MuId));
    nbjet_sel.reset(new NJetSelection(2,-1,Btag_loose ));
    // st_sel.reset(new STSelection(st_min));
    //nmuon_sel.reset(new NMuonSelection(2, -1));

    // 3. Set up Hists classes:
    h_nocuts.reset(new AndHists(ctx, "NoCuts"));
    // h_trigger.reset(new AndHists(ctx, "Trigger"));
    h_cleaner.reset(new AndHists(ctx, "Cleaner"));
    h_1mu.reset(new AndHists(ctx, "1Mu"));
    h_2jets.reset(new AndHists(ctx, "2Jets"));
    h_2bjets.reset(new AndHists(ctx,"2BJets"));
    // h_st.reset(new AndHists(ctx,"StSel"));
  }
   bool ttZPrimePreselectionModule::process(Event & event) {
   // mc_lumi_weight->process(event);

   h_nocuts->fill(event);

    // trigger
    // if(!(trigger_sel1->passes(event) || trigger_sel2->passes(event))) return false;
    //
    // h_trigger->fill(event);


    bool pass_common = common->process(event);
    if(!pass_common) return false;

    // if(is_mc) genjet_cleaner->process(event);

    // jetcleaner->process(event);

    // muoncleaner->process(event);

    // electroncleaner->process(event);

    h_cleaner->fill(event);

    if(!mu1_sel->passes(event)) return false;
    h_1mu->fill(event);


    if(!njet_sel->passes(event)) return false;
    h_2jets->fill(event);
    // my_st->process(event);
    // my_htlep->process(event);

    if(!nbjet_sel->passes(event)) return false;
    h_2bjets->fill(event);

    // if(!st_sel->passes(event)) return false;
    // h_st->fill(event);


    return true;


    }
     UHH2_REGISTER_ANALYSIS_MODULE(ttZPrimePreselectionModule)

}
