#include "UHH2/ttZPrime/include/TTbarRecoHadHypothesisHists.h"
#include "UHH2/common/include/JetIds.h"
#include "TH1F.h"
#include "TH2F.h"

using namespace uhh2;

TTbarRecoHadHypothesisHists::TTbarRecoHadHypothesisHists(uhh2::Context & ctx, const std::string & dirname, const std::string & hyps_name, const std::string & discriminator_name ): Hists(ctx, dirname){

  TString name = discriminator_name;
    double min=0;
    double max=500;
    if(discriminator_name=="Chi2Had"){
      name = "#Chi^{2}";
    }
    else{
      name += " discriminator";
    }

    if( discriminator_name=="CorrectMatch"){
      min=0;
      max=2;
    }
    if( discriminator_name=="TopDRMCHad"){
      min=0;
      max=6;
    }

    Discriminator = book<TH1F>("Discriminator",name,100,min,max);
    // Discriminator_2 = book<TH1F>("Discriminator_2",name,50,0,10);
    // Discriminator_3 = book<TH1F>("Discriminator_3",name,300,0,30);
    //
    // M_ttbar_rec = book<TH1F>( "M_ttbar_rec", "M_{t#bar{t}}^{rec} [GeV/c^{2}]", 100, 0, 5000 ) ;
    // M_ttbar_gen = book<TH1F>( "M_ttbar_gen", "M_{t#bar{t}}^{gen} [GeV/c^{2}]", 100, 0, 5000 ) ;
    //
    M_tophad1_rec = book<TH1F>( "M_tophad1_rec", "M^{top,had1} [GeV/c^{2}]", 50, 0, 500 ) ;
    M_tophad2_rec = book<TH1F>( "M_tophad2_rec", "M^{top,had2} [GeV/c^{2}]", 50, 0, 500 ) ;
    DeltaM_rec = book<TH1F>("DeltaM_rec","#DeltaM_{top1,top2} [Gev]", 25,0,250);

    NJet_tophad1_rec = book<TH1F>("NJet_tophad1_rec","NJet^{top,had1}",7,0,7);
    NJet_tophad2_rec = book<TH1F>("NJet_tophad2_rec","NJet^{top,had2}",7,0,7);

    DeltaR_Mu1_Jets = book<TH1F>("DeltaR_Mu1_Jets","#DeltaR_{jet,#mu1}",100,0,5);
    DeltaR_Mu2_Jets = book<TH1F>("DeltaR_Mu2_Jets","#DeltaR_{jet,#mu2}",100,0,5);
    DeltaTop_gen_rec = book<TH1F>("DeltaRSumTop_gen_rec","#DeltaRSumTop_{gen,rec}",100,0,5);

    DeepCSV_tophad1_rec = book<TH2F>("DeepCSV_tophad1_rec","CSV_{jet,had1}",5,0.,1.,7,0,7);
    DeepCSV_tophad2_rec = book<TH1F>("DeepCSV_tophad2_rec","CSV_{jet,had2}",100,0,1);
    //
    M_tophad1_rec_1jet = book<TH1F>( "M_tophad1_rec_1jet", "M^{top,had} [GeV/c^{2}]", 50, 0, 500 ) ;
    M_tophad1_rec_2jet = book<TH1F>( "M_tophad1_rec_2jet", "M^{top,had} [GeV/c^{2}]", 50, 0, 500 ) ;
    M_tophad1_rec_3jet = book<TH1F>( "M_tophad1_rec_3jet", "M^{top,had} [GeV/c^{2}]", 50, 0, 500 ) ;

    Pt_tophad1_rec = book<TH1F>( "Pt_tophad1_rec", "P_{T}^{top,lep} [GeV/c]", 60, 0, 1200 ) ;
    Pt_tophad2_rec = book<TH1F>( "Pt_tophad2_rec", "P_{T}^{top,had} [GeV/c]", 60, 0, 1200 ) ;

    // Pt_ttbar_rec = book<TH1F>( "Pt_ttbar_rec", "P_{T,t#bar{t}}^{rec} [GeV/c]", 60, 0, 600 ) ;
    // Pt_ttbar_gen = book<TH1F>( "Pt_ttbar_gen", "P_{T,t#bar{t}}^{gen} [GeV/c]", 60, 0, 600 ) ;

    // Pt_ttbar_rec_vs_Pt_ttbar_gen = book<TH2F>( "Pt_ttbar_rec_vs_Pt_ttbar_gen", "P_{T,t#bar{t}}^{rec} [GeV/c] vs P_{T,t#bar{t}}^{gen} [GeV/c]", 60, 0, 600 ,60, 0, 600);

    // Discriminator_vs_M_ttbar = book<TH2F>("Discriminator_vs_M_ttbar", name+" vs M_{t#bar{t}}^{rec}" , 50, min,max, 100,0,5000) ;

    // M_ttbar_rec_vs_M_ttbar_gen = book<TH2F>("M_ttbar_rec_vs_M_ttbar_gen","M_{t#bar{t}}^{rec} [GeV/c^{2}] vs M_{t#bar{t}}^{gen} [GeV/c^{2}]",100,0,5000,100,0,5000);
    // M_ttbar_gen_vs_M_ttbar_rec = book<TH2F>("M_ttbar_gen_vs_M_ttbar_rec","M_{t#bar{t}}^{gen} [GeV/c^{2}] vs M_{t#bar{t}}^{rec} [GeV/c^{2}]",100,0,5000,100,0,5000);

    // M_ttbar_resolution = book<TH1F>("M_ttbar_resolution", "(M_{t#bar{t}}^{gen} - M_{t#bar{t}}^{rec})/M_{t#bar{t}}^{rec}", 100, -5,5) ;
    //
    // Pt_toplep_rec_vs_Pt_toplep_gen = book<TH2F>("Pt_toplep_rec_vs_Pt_toplep_gen","P_{T}^{top,lep,rec} [GeV/c] vs P_{T}^{top,lep,gen} [GeV/c]",60, 0, 1200,60, 0, 1200);
    // Pt_tophad_rec_vs_Pt_tophad_gen = book<TH2F>("Pt_tophad_rec_vs_Pt_tophad_gen","P_{T}^{top,had,rec} [GeV/c] vs P_{T}^{top,had,gen} [GeV/c]",60, 0, 1200,60, 0, 1200);
    // Pt_lepton_rec_vs_Pt_lepton_gen = book<TH2F>("Pt_lepton_rec_vs_Pt_lepton_gen","P_{T}^{lepton,rec} [GeV/c] vs P_{T}^{lepton,gen} [GeV/c]",60, 0, 800,60, 0, 800);
    // Pt_blep_rec_vs_Pt_blep_gen = book<TH2F>("Pt_blep_rec_vs_Pt_blep_gen","P_{T}^{blep,rec} [GeV/c] vs P_{T}^{blep,gen} [GeV/c]",60, 0, 800,60, 0, 800);
    // Pt_neutrino_rec_vs_Pt_neutrino_gen = book<TH2F>("Pt_neutrino_rec_vs_Pt_neutrino_gen","P_{T}^{#nu,rec} [GeV/c] vs P_{T}^{#nu,gen} [GeV/c]",60, 0, 800,60, 0, 800);
    //
    // eta_toplep_rec_vs_eta_toplep_gen = book<TH2F>("eta_toplep_rec_vs_eta_toplep_gen","#eta^{top,lep,rec} vs #eta^{top,lep,gen}",100,-5,5,100,-5,5);
    // eta_tophad_rec_vs_eta_tophad_gen = book<TH2F>("eta_tophad_rec_vs_eta_tophad_gen","#eta^{top,had,rec} vs #eta^{top,had,gen}",100,-5,5,100,-5,5);
    // eta_lepton_rec_vs_eta_lepton_gen = book<TH2F>("eta_lepton_rec_vs_eta_lepton_gen","#eta^{lepton,rec} vs #eta^{lepton,gen}",100,-5,5,100,-5,5);
    // eta_blep_rec_vs_eta_blep_gen = book<TH2F>("eta_blep_rec_vs_eta_blep_gen","#eta^{blep,rec} vs #eta^{blep,gen}",100,-5,5,100,-5,5 );
    // eta_neutrino_rec_vs_eta_neutrino_gen = book<TH2F>("eta_neutrino_rec_vs_eta_neutrino_gen","#eta^{#nu,rec} vs #eta^{#nu,gen}",100,-5,5,100,-5,5 );

    h_hyps = ctx.get_handle<std::vector<TTbarRecoHadHypothesis>>(hyps_name);
    h_ttbargen = ctx.get_handle<TTbarGen>("ttbargen");
    m_discriminator_name = discriminator_name;
}


void TTbarRecoHadHypothesisHists::fill(const uhh2::Event & e){
  std::vector<TTbarRecoHadHypothesis> hyps = e.get(h_hyps);
  const TTbarRecoHadHypothesis* hyp = get_best_hypothesis( hyps, m_discriminator_name );
  if(hyp){
  double weight = e.weight;
  // double mttbar_rec = 0;
  // if( (hyp->tophad1_v4()+hyp->tophad2_v4()).isTimelike() )
  //   mttbar_rec = (hyp->tophad1_v4()+hyp->tophad2_v4()).M();
  // else{
  //   mttbar_rec = sqrt( -(hyp->tophad1_v4()+hyp->tophad2_v4()).mass2());
  // }
  // double ptttbar_rec = (hyp->tophad1_v4()+hyp->tophad2_v4()).Pt();
  // double mttbar_gen = 0;
  // double ptttbar_gen = 0;
  //
  // M_ttbar_rec->Fill(mttbar_rec, weight);
  // Pt_ttbar_rec->Fill ( ptttbar_rec, weight);
  //
  if(e.is_valid(h_ttbargen)){
    const auto & ttbargen = e.get(h_ttbargen);
    double dr_gen_reco1 = deltaR(hyp->tophad1_v4(),ttbargen.Top().v4())+ deltaR(hyp->tophad2_v4(),ttbargen.Antitop().v4());
    double dr_gen_reco2 = deltaR(hyp->tophad1_v4(),ttbargen.Antitop().v4())+ deltaR(hyp->tophad2_v4(),ttbargen.Top().v4());
    if(dr_gen_reco1 < dr_gen_reco2)
    {
      DeltaTop_gen_rec->Fill(dr_gen_reco1,weight);
    }
    else{
      DeltaTop_gen_rec->Fill(dr_gen_reco2,weight);
    }

  //   mttbar_gen = ( ttbargen.Top().v4() + ttbargen.Antitop().v4()).M();
  //   ptttbar_gen = ( ttbargen.Top().v4() + ttbargen.Antitop().v4()).Pt();
  //
  //   M_ttbar_gen->Fill(mttbar_gen, weight);
  //
  //   M_ttbar_rec_vs_M_ttbar_gen->Fill(mttbar_rec, mttbar_gen,weight);
  //   M_ttbar_gen_vs_M_ttbar_rec->Fill(mttbar_gen, mttbar_rec,weight);
  //
  //   M_ttbar_resolution->Fill( (mttbar_gen-mttbar_rec)/mttbar_gen, weight);
  //
  //   Pt_ttbar_gen->Fill ( ptttbar_gen, weight);
  //   Pt_ttbar_rec_vs_Pt_ttbar_gen->Fill(ptttbar_rec, ptttbar_gen,weight);
  //
  //   if(ttbargen.DecayChannel()==TTbarGen::e_ehad || ttbargen.DecayChannel()==TTbarGen::e_muhad){
  //     double lep_pt_gen = ttbargen.ChargedLepton().pt();
  //     double lep_eta_gen = ttbargen.ChargedLepton().eta();
  //     double nu_pt_gen = ttbargen.Neutrino().pt();
  //     double nu_eta_gen = ttbargen.Neutrino().eta();
  //     double blep_pt_gen = ttbargen.BLep().pt();
  //     double blep_eta_gen = ttbargen.BLep().eta();
  //     double toplep_pt_gen = ttbargen.TopLep().pt();
  //     double toplep_eta_gen = ttbargen.TopLep().eta();
  //     double tophad_pt_gen = ttbargen.TopHad().pt();
  //     double tophad_eta_gen = ttbargen.TopHad().eta();
  //
  //     Pt_tophad_rec_vs_Pt_tophad_gen->Fill(hyp->tophad_v4().Pt(), tophad_pt_gen, weight);
  //     Pt_toplep_rec_vs_Pt_toplep_gen->Fill(hyp->toplep_v4().Pt(), toplep_pt_gen, weight);
  //     Pt_lepton_rec_vs_Pt_lepton_gen->Fill(hyp->lepton().pt(), lep_pt_gen, weight);
  //     Pt_neutrino_rec_vs_Pt_neutrino_gen->Fill(hyp->neutrino_v4().Pt(), nu_pt_gen, weight);
  //     Pt_blep_rec_vs_Pt_blep_gen->Fill(hyp->blep_v4().Pt(), blep_pt_gen, weight);
  //     eta_tophad_rec_vs_eta_tophad_gen->Fill(hyp->tophad_v4().eta(), tophad_eta_gen, weight);
  //     eta_toplep_rec_vs_eta_toplep_gen->Fill(hyp->toplep_v4().eta(), toplep_eta_gen, weight);
  //     eta_lepton_rec_vs_eta_lepton_gen->Fill(hyp->lepton().eta(), lep_eta_gen, weight);
  //     eta_neutrino_rec_vs_eta_neutrino_gen->Fill(hyp->neutrino_v4().eta(), nu_eta_gen, weight);
  //     eta_blep_rec_vs_eta_blep_gen->Fill(hyp->blep_v4().eta(), blep_eta_gen, weight);
  //   }
  }

  Discriminator->Fill(hyp->discriminator(m_discriminator_name) ,weight);
  // Discriminator_2->Fill(hyp->discriminator(m_discriminator_name) ,weight);
  // Discriminator_3->Fill(hyp->discriminator(m_discriminator_name) ,weight);

  // Discriminator_vs_M_ttbar->Fill(hyp->discriminator(m_discriminator_name), mttbar_rec, weight);

  double mtophad1=0;
  double mtophad2=0;
  auto muon = *e.muons;
  if(hyp->tophad1_v4().isTimelike()) mtophad1 = hyp->tophad1_v4().M();
  if(hyp->tophad2_v4().isTimelike()) mtophad2 = hyp->tophad2_v4().M();


  M_tophad1_rec->Fill(mtophad1,weight);
  M_tophad2_rec->Fill(mtophad2,weight);
  DeltaM_rec->Fill(abs(mtophad1-mtophad2));
  NJet_tophad1_rec->Fill(hyp->tophad1_jets().size(),weight);
  NJet_tophad2_rec->Fill(hyp->tophad2_jets().size(),weight);
  int NJet_CSV20 = 0;
  int NJet_CSV40 = 0;
  int NJet_CSV60 = 0;
  int NJet_CSV80 = 0;
  int NJet_CSV100 = 0;
  for(auto & jet : hyp->tophad1_jets()){
    double csv = jet.btag_DeepCSV();
    if(csv <= 0.2) NJet_CSV20++;
    if(csv <= 0.4 && csv > 0.2) NJet_CSV40++;
    if(csv <= 0.6 && csv > 0.4) NJet_CSV60++;
    if(csv <= 0.8 && csv > 0.6) NJet_CSV80++;
    if(csv > 0.8) NJet_CSV100++;

    DeltaR_Mu1_Jets->Fill(deltaR(muon[0],jet),weight);
    DeltaR_Mu2_Jets->Fill(deltaR(muon[1],jet),weight);

  }
  DeepCSV_tophad1_rec->Fill(0.1,NJet_CSV20,weight);
  DeepCSV_tophad1_rec->Fill(0.3,NJet_CSV40,weight);
  DeepCSV_tophad1_rec->Fill(0.5,NJet_CSV60,weight);
  DeepCSV_tophad1_rec->Fill(0.7,NJet_CSV80,weight);
  DeepCSV_tophad1_rec->Fill(0.9,NJet_CSV100,weight);
  for(auto & jet : hyp->tophad2_jets()){
    DeepCSV_tophad2_rec->Fill(jet.btag_DeepCSV(),weight);
    DeltaR_Mu1_Jets->Fill(deltaR(muon[0],jet),weight);
    DeltaR_Mu2_Jets->Fill(deltaR(muon[1],jet),weight);

  }
  if(hyp->tophad1_jets().size()==1) M_tophad1_rec_1jet->Fill(mtophad1,weight);
  if(hyp->tophad1_jets().size()==2) M_tophad1_rec_2jet->Fill(mtophad1,weight);
  if(hyp->tophad1_jets().size()>=3) M_tophad1_rec_3jet->Fill(mtophad1,weight);
  //
  Pt_tophad1_rec->Fill( hyp->tophad1_v4().Pt(),weight );
  Pt_tophad2_rec->Fill( hyp->tophad2_v4().Pt(),weight );
 }

}
