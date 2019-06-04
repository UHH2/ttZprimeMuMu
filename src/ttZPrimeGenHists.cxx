#include "UHH2/ttZPrime/include/ZPrimeGenHists.h"
#include "TH1F.h"
#include "TH2F.h"

using namespace uhh2;

ZPrimeGenHists::ZPrimeGenHists(uhh2::Context & ctx, const std::string & dirname): Hists(ctx, dirname){

    M_ZPrime_gen =  book<TH1F>( "M_ZPrime_gen", "M_{Z'} [GeV/c^{2}]", 1000, 0, 5000 ) ;
    Pt_ZPrime_gen = book< TH1F>( "Pt_ZPrime_gen", "P_{T,Z'} [GeV/c]", 600, 0, 600 ) ;
    //shat = book< TH1F>( "shat", "#hat{s} [GeV]", 1000, 0, 5000 ) ;
    //DecayChannel = book< TH1F>( "DecayChannel", "decay channel", 11, 0, 11 ) ;


    phi_ZPrime = book< TH1F>( "phi_ZPrime", "#phi_{Z'} [GeV/c]", 25, -M_PI, M_PI ) ;
    eta_ZPrime = book< TH1F>( "eta_ZPrime", "#eta_{Z'}", 1000, -5, 5 ) ;


     Pt_mu = book<TH1F>( "Pt_mu","P_{T,mu} [GeV/c]",250,0,1000);
     Pt_antimu = book<TH1F>( "Pt_antimu","P_{T,#bar{#mu}} [GeV/c]",250,0,1000);
     eta_mu = book<TH1F>( "eta_mu","#eta_{#mu}",100,-5,5);
     eta_antimu = book<TH1F>( "eta_antimu","#eta_{#bar{#mu}}",100,-5,5);
    // y_mu = book<TH1F>( "y_mu","y_{mu}",1000,-5,5);
    // y_antimu = book<TH1F>( "y_antimu","y_{#bar{mu}}",1000,-5,5);
    phi_mu = book< TH1F>( "phi_mu", "#phi_{#mu}", 25, -M_PI, M_PI ) ;
    phi_antimu = book< TH1F>( "phi_antimu", "#phi_{#bar{#mu}}", 25, -M_PI, M_PI ) ;
    // M_mu = book<TH1F>( "M_mu","M_{mu} [GeV/c^{2}]",100,0,1);
    // M_antimu = book<TH1F>( "M_antimu","M_{#bar{mu}} [GeV/c^{2}]",100,0,1);
    //
    // Pt_top = book<TH1F>( "Pt_top","P_{T,t} [GeV/c]",500,0,2000);
    // Pt_antitop = book<TH1F>( "Pt_antitop","P_{T,#bar{t}} [GeV/c]",500,0,2000);
    // eta_top = book<TH1F>( "eta_top","#eta_{t}",100,-5,5);
    // eta_antitop = book<TH1F>( "eta_antitop","#eta_{#bar{t}}",100,-5,5);
    // y_top = book<TH1F>( "y_top","y_{t}",1000,-5,5);
    // y_antitop = book<TH1F>( "y_antitop","y_{#bar{t}}",1000,-5,5);
    // phi_top = book< TH1F>( "phi_top", "#phi_{t}", 25, -M_PI, M_PI ) ;
    // phi_antitop = book< TH1F>( "phi_antitop", "#phi_{#bar{t}}", 25, -M_PI, M_PI ) ;
    // M_top = book<TH1F>( "M_top","M_{t} [GeV/c^{2}]",1000,150,200);
    // M_antitop = book<TH1F>( "M_antitop","M_{#bar{t}} [GeV/c^{2}]",1000,150,200);

    /*cosThetastar_top_ttframe = book< TH1F>( "cosThetastar_top_ttframe", "cos(#Theta*)_{t}",1000,-1,1);
    cosThetastar_antitop_ttframe = book< TH1F>( "cosThetastar_antitop_ttframe", "cos(#Theta*)_{#bar{t}}",1000,-1,1);
    Pt_top_ttframe = book< TH1F>( "Pt_top_ttframe", "P_{T,t}* [GeV/c]",1000,0,2000);
    Pt_antitop_ttframe = book< TH1F>( "Pt_antitop_ttframe", "P_{T,#bar{t}}* [GeV/c]",1000,0,2000);
    M_ttbar_vs_Pt_top_ttframe = book<TH2F>( "M_ttbar_vs_Pt_top_ttframe", "M_{t#bar{t}} [GeV/c^{2}] vs P_{T,t}* [GeV/c]",500,0,5000,500,0,2000);
    M_ttbar_vs_Pt_antitop_ttframe = book<TH2F>( "M_ttbar_vs_Pt_antitop_ttframe", "M_{t#bar{t}} [GeV/c^{2}] vs P_{T,#bar{t}}* [GeV/c]",500,0,5000,500,0,2000);*/


    h_ZPrimeGen = ctx.get_handle<ZPrimeGen>("ZPrimeGen");
}


void ZPrimeGenHists::fill(const uhh2::Event & e){
    //do not fill histograms if LQLQbargen information has not been filled
    if(!e.is_valid(h_ZPrimeGen)){
      //std::cout <<"Wrong" << '\n';
      return;
    }
    const auto & ZPrimeGen = e.get(h_ZPrimeGen);

    LorentzVector ZPrime = ZPrimeGen.ZPrime().v4();
    LorentzVector MuZPrime = ZPrimeGen.MuZPrime().v4();
    LorentzVector MuAntiZPrime = ZPrimeGen.MuAntiZPrime().v4();


    //double sh = (e.genparticles->at(0).v4()+ e.genparticles->at(1).v4()).M();

    //DecayChannel->Fill(LQLQbargen.DecayChannel(), e.weight);
    //if(LQ.M() == 0 || antiLQ.M() == 0){return;}

    //std::cout << "MLQLQ = " << mLQLQbar_gen << " & eventweight = " << e.weight << std::endl;
    //std::cout << "MLQ = " << LQ.M() << " & MAntiLQ = " << antiLQ.M() << std::endl << std::endl;
    //    if (LQ.M() > 0 && antiLQ.M() > 0){
    M_ZPrime_gen->Fill( ZPrime.M(),e.weight);   //}
    //shat->Fill(sh, e.weight);
    //std::cout <<"ZPrimeMass=" << ZPrime.M() << '\n';
    //   if (LQ.M() > 0 && antiLQ.M() > 0){
    Pt_ZPrime_gen->Fill( ZPrime.Pt(), e.weight);

    eta_ZPrime->Fill( ZPrime.eta(), e.weight);

    phi_ZPrime->Fill( ZPrime.phi(), e.weight);





    /* double deltaR_Top = uhh2::deltaR(LQLQbargen.Topdecay1(), LQLQbargen.Topdecay2());
    double deltaR_Antitop = uhh2::deltaR(LQLQbargen.Antitopdecay1(), LQLQbargen.Antitopdecay2());
    deltaR_Top_decays->Fill(deltaR_Top,e.weight);
    deltaR_Antitop_decays->Fill(deltaR_Antitop,e.weight);
    M_LQLQbar_vs_deltaR_Top->Fill(mLQLQbar_gen, deltaR_Top, e.weight);
    M_LQLQbar_vs_deltaR_Antitop->Fill(mLQLQbar_gen, deltaR_Antitop, e.weight);*/


    Pt_mu->Fill( MuZPrime.pt(), e.weight);
    Pt_antimu->Fill( MuAntiZPrime.pt(), e.weight);
    eta_mu->Fill( MuZPrime.eta(), e.weight);
    eta_antimu->Fill( MuAntiZPrime.eta(), e.weight);
    // y_mu->Fill( LQLQbargen.muLQ().v4().Rapidity(), e.weight);
    // y_antimu->Fill( LQLQbargen.muAntiLQ().v4().Rapidity(), e.weight);
    phi_mu->Fill( MuZPrime.phi(), e.weight);
    phi_antimu->Fill( MuAntiZPrime.phi(), e.weight);
    // M_mu->Fill( LQLQbargen.muLQ().v4().M(), e.weight);
    // M_antimu->Fill( LQLQbargen.muAntiLQ().v4().M(), e.weight);
    //
    // Pt_top->Fill( LQLQbargen.TopLQ().pt(), e.weight);
    // Pt_antitop->Fill( LQLQbargen.TopAntiLQ().pt(), e.weight);
    // eta_top->Fill( LQLQbargen.TopLQ().eta(), e.weight);
    // eta_antitop->Fill( LQLQbargen.TopAntiLQ().eta(), e.weight);
    // y_top->Fill( LQLQbargen.TopLQ().v4().Rapidity(), e.weight);
    // y_antitop->Fill( LQLQbargen.TopAntiLQ().v4().Rapidity(), e.weight);
    // phi_top->Fill( LQLQbargen.TopLQ().phi(), e.weight);
    // phi_antitop->Fill( LQLQbargen.TopAntiLQ().phi(), e.weight);
    // if(LQLQbargen.TopLQ().v4().isTimelike())
    //   M_top->Fill( LQLQbargen.TopLQ().v4().M(), e.weight);
    // if(LQLQbargen.TopAntiLQ().v4().isTimelike())
    //   M_antitop->Fill( LQLQbargen.TopAntiLQ().v4().M(), e.weight);

    /*  TLorentzVector LQ_LQLQframe(0,0,0,0);
    LQ_LQLQframe.SetPtEtaPhiE(LQ.pt(), LQ.eta(), LQ.phi(), LQ.E());
    TLorentzVector antiLQ_LQLQframe(0,0,0,0);
    antiLQ_LQLQframe.SetPtEtaPhiE(antiLQ.pt(), antiLQ.eta(), antiLQ.phi(), antiLQ.E());
    TLorentzVector LQLQbar(0,0,0,0);
    LQLQbar.SetPtEtaPhiE((LQ+antiLQ).pt(), (LQ+antiLQ).eta(), (LQ+antiLQ).phi(), (LQ+antiLQ).E());

    LQ_LQLQframe.Boost(-LQLQbar.BoostVector());
    antiLQ_LQLQframe.Boost(-LQLQbar.BoostVector());
    cosThetastar_LQ_LQLQframe->Fill(cos(LQ_LQLQframe.Theta()) ,e.weight);
    cosThetastar_antiLQ_LQLQframe->Fill(cos(antiLQ_LQLQframe.Theta()) ,e.weight);
    Pt_LQ_LQLQframe->Fill(LQ_LQLQframe.Pt(),e.weight);
    Pt_antiLQ_LQLQframe->Fill(antiLQ_LQLQframe.Pt(),e.weight);
    M_LQLQbar_vs_Pt_LQ_LQLQframe->Fill(mLQLQbar_gen, LQ_LQLQframe.Pt(),e.weight );
    M_LQLQbar_vs_Pt_antiLQ_LQLQframe->Fill(mLQLQbar_gen, antiLQ_LQLQframe.Pt(),e.weight ); */

}
