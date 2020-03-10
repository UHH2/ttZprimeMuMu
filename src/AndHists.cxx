#include "UHH2/ttZPrime/include/AndHists.h"
#include "UHH2/common/include/LuminosityHists.h"
#include "UHH2/common/include/EventHists.h"
#include "UHH2/common/include/ElectronHists.h"
#include "UHH2/common/include/MuonHists.h"
#include "UHH2/common/include/MuonIds.h"
#include "UHH2/common/include/JetHists.h"
#include "UHH2/common/include/GenJetsHists.h"
#include "UHH2/ttZPrime/include/MyEventHists.h"
#include "UHH2/ttZPrime/include/ttZPrimeMuPreselectionHists.h"



using namespace std;
using namespace uhh2;
AndHists::AndHists(Context &ctx, const string & dirname):
  Hists(ctx, dirname+"_Counter")
{
  bool is_mc = ctx.get("dataset_type") == "MC", is_blind =true;
  // Counting Hist
  nevt = book<TH1F>("NEvt", "", 1,0,1);

  //
  // MuonID MuIdTight = MuonID(Muon::CutBasedIdTight);
  // MuonID MuIdLoose = MuonID(Muon::CutBasedIdLoose);
  // Add common hists to vector

  hists_vector.push_back(new LuminosityHists(ctx, dirname + "_Lumi"));
  hists_vector.push_back(new EventHists(ctx, dirname + "_Event"));
  hists_vector.push_back(new MuonHists(ctx, dirname + "_Muon"));
  hists_vector.push_back(new ElectronHists(ctx, dirname + "_Electron"));
  hists_vector.push_back(new JetHists(ctx, dirname + "_Jet"));
  // hists_vector.push_back(new TopJetHists(ctx, dirname + "_Topjets"));
  hists_vector.push_back(new MyEventHists(ctx, dirname + "_MyEvent"));
  if(is_mc || !is_blind)
  {
    hists_vector.push_back(new ttZPrimeMuPreselectionHists(ctx, dirname+"_ZPrimeMuMu"));
  }
  hists_vector.push_back(new GenJetsHists(ctx, dirname+"_GenJets"));

  JetHists* bJetLooseHists = new JetHists(ctx, dirname + "_bJet_loose", 2);
  JetId btag_loose = DeepJetBTag(DeepJetBTag::WP_LOOSE);
  bJetLooseHists->set_JetId(btag_loose);
  hists_vector.push_back(bJetLooseHists);

  JetHists* bJetMediumHists = new JetHists(ctx, dirname + "_bJet_medium", 2);
  JetId btag_medium = DeepJetBTag(DeepJetBTag::WP_MEDIUM);
  bJetMediumHists->set_JetId(btag_medium);
  hists_vector.push_back(bJetMediumHists);

  JetHists* bJetTightHists = new JetHists(ctx, dirname + "_bJet_tight", 2);
  JetId btag_tight = DeepJetBTag(DeepJetBTag::WP_TIGHT);
  bJetTightHists->set_JetId(btag_tight);
  hists_vector.push_back(bJetTightHists);

}

void AndHists::fill(const Event & event) {
  nevt->Fill(0., event.weight);
  for (Hists *hist : hists_vector)
    {
      hist->fill(event);
    }
}

void AndHists::add_hist(Hists *hist) {
  hists_vector.push_back(hist);
}

AndHists::~AndHists() {
  for (unsigned int i = 0; i < hists_vector.size(); ++i)
    {
      delete hists_vector[i];
    }
  hists_vector.clear();
}
