#include <iostream>
#include <memory>

int main()
{
  std::cout << "Hello from ttZPrimePreselectionModule!" << std::endl;
  return 0;
}


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
//#include "UHH2/common/include/TriggerSelection.h"
#include "UHH2/common/include/MCWeight.h"
#include "UHH2/common/include/AdditionalSelections.h"

#include "UHH2/ttZPrime/include/AndHists.h"
#include "UHH2/ttZPrime/include/MyEventVariables.h"



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

    unique_ptr<AnalysisModule> syst_module, my_st, my_htlep;
        // declare the Selections to use.
    unique_ptr<Selection> njet_sel, nmuon_sel, n_gen_muon_sel, nele_sel, n_gen_ele_sel, ht_sel, lumi_sel, mu2_sel, trigger_sel, trigger_sel1, trigger_sel2, mttbargen_sel,nbjet_sel;

    // store the Hists collection as member variables.
    unique_ptr<Hists> h_nocuts,h_trigger, h_lumi,h_cleaner,h_2mu, h_2jets,h_1bjet;




    MuonId MuId;
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

    EleId = AndId<Electron>(ElectronID_Summer16_tight, PtEtaCut(30.0, 2.4));
    MuId = AndId<Muon>(MuonID(Muon::CutBasedIdTight), PtEtaCut(30.0, 2.4), MuonIso(0.15));

    Jet_ID = AndId<Jet>(JetPFID(JetPFID::WP_TIGHT_CHS), PtEtaCut(30.0, 2.4));

    Btag_loose = CSVBTag(CSVBTag::wp::WP_LOOSE);
    Btag_medium = CSVBTag(CSVBTag::wp::WP_MEDIUM);
    Btag_tight = CSVBTag(CSVBTag::wp::WP_TIGHT);

    //common.reset(new CommonModules());
    //common->switch_jetlepcleaner(true);
    //common->set_electron_id(EleId);
    //common->set_muon_id(MuId);
    //common->init(ctx);
    jetcleaner.reset(new JetCleaner(ctx,Jet_ID));
    electroncleaner.reset(new ElectronCleaner(EleId));
    muoncleaner.reset(new MuonCleaner(MuId));
    syst_module.reset(new MCScaleVariation(ctx));
    my_st.reset(new STCalculator(ctx));
    my_htlep.reset(new HTlepCalculator(ctx));

    // 2. set up selections

    //Preselection
    //trigger_sel1.reset(new TriggerSelection("HLT_IsoMu24_v*")); //original: IsoMu24
    //trigger_sel2.reset(new TriggerSelection("HLT_IsoTkMu24_v*")); //original: IsoMu24
    njet_sel.reset(new NJetSelection(2, -1));
    mu2_sel.reset(new NMuonSelection(2, -1));
    nbjet_sel.reset(new NJetSelection(1,-1,Btag_tight ));
    //nmuon_sel.reset(new NMuonSelection(2, -1));

    // 3. Set up Hists classes:
    h_nocuts.reset(new AndHists(ctx, "NoCuts"));
    //h_trigger.reset(new AndHists(ctx, "Trigger"));
    h_cleaner.reset(new AndHists(ctx, "Cleaner"));
    h_2mu.reset(new AndHists(ctx, "1Mu"));
    h_2jets.reset(new AndHists(ctx, "2Jets"));
    h_1bjet.reset(new AndHists(ctx,"1BJet"));
  }
   bool ttZPrimePreselectionModule::process(Event & event) {
   my_st->process(event);
   my_htlep->process(event);

    h_nocuts->fill(event);

    // trigger
    //if(!(trigger_sel1->passes(event))) return false;
    //if(!(trigger_sel1->passes(event) || trigger_sel2->passes(event))) return false;

    //h_trigger->fill(event);


    //bool pass_common = common->process(event);
    //if(!pass_common) return false;
    jetcleaner->process(event);
    muoncleaner->process(event);
    electroncleaner->process(event);


    h_cleaner->fill(event);


    if(!mu2_sel->passes(event)) return false;
    h_2mu->fill(event);


    if(!njet_sel->passes(event)) return false;
    h_2jets->fill(event);

    if(!nbjet_sel->passes(event)) return false;
    h_1bjet->fill(event);


    return true;


    }
     UHH2_REGISTER_ANALYSIS_MODULE(ttZPrimePreselectionModule)

}
