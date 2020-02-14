#include "UHH2/ttZPrime/include/TTbarRecoHadHypothesisHists.h"
#include "UHH2/common/include/JetIds.h"
#include "TH1F.h"
#include "TH2F.h"

using namespace uhh2;

TTbarRecoHadHypothesisHists::TTbarRecoHadHypothesisHists(uhh2::Context & ctx, const std::string & dirname, const std::string & hyps_name, const std::string & discriminator_name ): Hists(ctx, dirname){

  TString name = discriminator_name;
    double min=0;
    double max = 100;
    int nbins = 50;
    if(discriminator_name=="Chi26Jets" ||discriminator_name=="Chi25Jets" || discriminator_name=="Chi24Jets"){
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

    Discriminator = book<TH1F>("Discriminator",name,nbins,min,max);
    Discriminator_top1 = book<TH1F>("Discriminator_top1",name+"Top1",nbins,min,max);
    Discriminator_top2 = book<TH1F>("Discriminator_top2",name+"Top2",nbins,min,max);
    Discriminator_w1 = book<TH1F>("Discriminator_W1",name+"W1",nbins,min,max);
    Discriminator_w2 = book<TH1F>("Discriminator_W2",name+"W2",nbins,min,max);

    //
    // M_ttbar_rec = book<TH1F>( "M_ttbar_rec", "M_{t#bar{t}}^{rec} [GeV/c^{2}]", 100, 0, 5000 ) ;
    // M_ttbar_gen = book<TH1F>( "M_ttbar_gen", "M_{t#bar{t}}^{gen} [GeV/c^{2}]", 100, 0, 5000 ) ;
    //
    M_topshad_rec = book<TH1F>( "M_topshad_rec", "M^{tops,had} [GeV/c^{2}]", 50, 0, 500 ) ;
    M_tophad1_rec = book<TH1F>( "M_tophad1_rec", "M^{top,had1} [GeV/c^{2}]", 50, 0, 500 ) ;
    M_tophad2_rec = book<TH1F>( "M_tophad2_rec", "M^{top,had2} [GeV/c^{2}]", 50, 0, 500 ) ;
    Mw_tophad_rec = book<TH1F>( "Mw_tophad_rec", "M^{W,had} [GeV/c^{2}]", 50, 0, 500 ) ;
    Mw_tophad1_rec = book<TH1F>( "Mw_tophad1_rec", "M^{W,had1} [GeV/c^{2}]", 50, 0, 500 ) ;
    Mw_tophad2_rec = book<TH1F>( "Mw_tophad2_rec", "M^{W,had2} [GeV/c^{2}]", 50, 0, 500 ) ;


    DeltaM_rec = book<TH1F>("DeltaM_rec","#DeltaM_{top1,top2} [Gev]", 100,-500,500);
    DeltaM_rec_W = book<TH1F>("DeltaM_rec_W","#DeltaM_{W1,W2} [Gev]", 100,-500,500);

    NJet_tophad1_rec = book<TH1F>("NJet_tophad1_rec","NJet^{top,had1}",7,0,7);
    NJet_tophad2_rec = book<TH1F>("NJet_tophad2_rec","NJet^{top,had2}",7,0,7);
    NJet_sum_rec = book<TH1F>("NJet_sum_rec","NJet^{top,sum}",10,0,10);
    NWJets_sum_rec = book<TH1F>("NWJets_sum_rec","NWJet^{top,sum}",5,0,5);
    NWJet_tophad1_rec = book<TH1F>("NWJet_tophad1_rec","NWJet^{top,had1}",5,0,5);
    NWJet_tophad2_rec = book<TH1F>("NWJet_tophad2_rec","NWJet^{top,had2}",5,0,5);

    DeltaR_Mu1_Jets = book<TH1F>("DeltaR_Mu1_Jets","#DeltaR_{jet,#mu1}",100,0,5);
    DeltaR_Mu2_Jets = book<TH1F>("DeltaR_Mu2_Jets","#DeltaR_{jet,#mu2}",100,0,5);
    DeltaTop_gen_rec = book<TH1F>("DeltaRSumTop_gen_rec","#DeltaRSumTop_{gen,rec}",100,0,5);

    DeepCSV_tophad1_rec = book<TH1F>("DeepCSV_tophad1_rec","CSV_{bjet,had1}",50,0,1);
    DeepCSV_tophad2_rec = book<TH1F>("DeepCSV_tophad2_rec","CSV_{bjet,had2}",50,0,1);
    //
    M_tophad1_rec_1jet = book<TH1F>( "M_tophad1_rec_1jet", "M^{top,had} [GeV/c^{2}]", 50, 0, 500 ) ;
    M_tophad1_rec_2jet = book<TH1F>( "M_tophad1_rec_2jet", "M^{top,had} [GeV/c^{2}]", 50, 0, 500 ) ;
    M_tophad1_rec_3jet = book<TH1F>( "M_tophad1_rec_3jet", "M^{top,had} [GeV/c^{2}]", 50, 0, 500 ) ;
    M_tophad1_rec_4jet = book<TH1F>( "M_tophad1_rec_4jet", "M^{top,had} [GeV/c^{2}]", 50, 0, 500 ) ;

    M_tophad2_rec_1jet = book<TH1F>( "M_tophad2_rec_1jet", "M^{top,had} [GeV/c^{2}]", 50, 0, 500 ) ;
    M_tophad2_rec_2jet = book<TH1F>( "M_tophad2_rec_2jet", "M^{top,had} [GeV/c^{2}]", 50, 0, 500 ) ;
    M_tophad2_rec_3jet = book<TH1F>( "M_tophad2_rec_3jet", "M^{top,had} [GeV/c^{2}]", 50, 0, 500 ) ;
    M_tophad2_rec_4jet = book<TH1F>( "M_tophad2_rec_4jet", "M^{top,had} [GeV/c^{2}]", 50, 0, 500 ) ;

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
  double maxbin = 299.99;
  if(hyp->discriminator(m_discriminator_name) > maxbin)
  {
    Discriminator->Fill(maxbin ,weight);
  }
  else
  {
    Discriminator->Fill(hyp->discriminator(m_discriminator_name) ,weight);
  }
  if(m_discriminator_name =="Chi2Had"){
    if(hyp->discriminator(m_discriminator_name + "_thad1") > maxbin)
    {
    Discriminator_top1->Fill(maxbin ,weight);
    }
    else
    {
      Discriminator_top1->Fill(hyp->discriminator(m_discriminator_name+"_thad1") ,weight);
    }
    if(hyp->discriminator(m_discriminator_name + "_thad2") > maxbin)
    {
    Discriminator_top2->Fill(maxbin ,weight);
    }
    else
    {
      Discriminator_top2->Fill(hyp->discriminator(m_discriminator_name+"_thad2") ,weight);
    }
    // Discriminator_w1->Fill(hyp->discriminator(m_discriminator_name+"_whad1") ,weight);
    // Discriminator_w2->Fill(hyp->discriminator(m_discriminator_name+"_whad2") ,weight);
  }
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
  M_topshad_rec->Fill(mtophad1,weight);
  M_topshad_rec->Fill(mtophad2,weight);
  DeltaM_rec->Fill(mtophad1-mtophad2,weight);








  for(auto & jet : hyp->tophad1_jets()){
    DeltaR_Mu1_Jets->Fill(deltaR(muon[0],jet),weight);
    DeltaR_Mu2_Jets->Fill(deltaR(muon[1],jet),weight);
  }
  // DeepCSV_tophad1_rec->Fill(hyp->tophad1_bjet().btag_DeepJet(),weight);
  // DeepCSV_tophad2_rec->Fill(hyp->tophad2_bjet().btag_DeepJet(),weight);
  for(auto & jet : hyp->tophad2_jets()){
    DeltaR_Mu1_Jets->Fill(deltaR(muon[0],jet),weight);
    DeltaR_Mu2_Jets->Fill(deltaR(muon[1],jet),weight);
  }

  LorentzVector mwhad1 = hyp->w1_v4();
  LorentzVector mwhad2 = hyp->w2_v4();
  Mw_tophad1_rec->Fill(mwhad1.M(),weight);
  Mw_tophad2_rec->Fill(mwhad2.M(),weight);
  Mw_tophad_rec->Fill(mwhad1.M(), weight);
  Mw_tophad_rec->Fill(mwhad2.M(), weight);
  DeltaM_rec_W->Fill(mwhad1.M()-mwhad2.M(),weight);

  auto NJets1 = hyp->tophad1_jets().size();
  auto NJets2 = hyp->tophad2_jets().size();


  NJet_tophad1_rec->Fill(NJets1,weight);
  NJet_tophad2_rec->Fill(NJets2,weight);
  NJet_sum_rec->Fill(NJets1+NJets2,weight);

  auto NWJets1 = hyp->tophad1_wjets().size();
  auto NWJets2 = hyp->tophad2_wjets().size();
  NWJet_tophad1_rec->Fill(NWJets1,weight);
  NWJet_tophad2_rec->Fill(NWJets2,weight);
  NWJets_sum_rec->Fill(NWJets1+NWJets2,weight);

  if(NJets1==1) M_tophad1_rec_1jet->Fill(mtophad1,weight);
  if(NJets1==2) M_tophad1_rec_2jet->Fill(mtophad1,weight);
  if(NJets1==3) M_tophad1_rec_3jet->Fill(mtophad1,weight);
  if(NJets1>=4) M_tophad1_rec_4jet->Fill(mtophad1,weight);

  if(NJets2==1) M_tophad2_rec_1jet->Fill(mtophad2,weight);
  if(NJets2==2) M_tophad2_rec_2jet->Fill(mtophad2,weight);
  if(NJets2==3) M_tophad2_rec_3jet->Fill(mtophad2,weight);
  if(NJets2>=4) M_tophad2_rec_4jet->Fill(mtophad2,weight);




  Pt_tophad1_rec->Fill( hyp->tophad1_v4().Pt(),weight );
  Pt_tophad2_rec->Fill( hyp->tophad2_v4().Pt(),weight );

  // std::cout << m_discriminator_name << ":" << std::endl;
  // std::cout << "NJetSum:" << NJets1+NJets2 << ", Chi2:" << chi2_thad_1 + chi2_thad_2 << ", M(Top1):" << mtophad1 << ", M(Top2):" << mtophad2 <<std::endl;

 }

}
