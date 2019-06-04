#include "UHH2/core/include/Hists.h"
#include "UHH2/core/include/Event.h"
#include "UHH2/ttZPrime/include/ZPrimeGen.h"

/** \brief Histograms for LQLQbar quantities on generator (parton) level
 *
 * LQGen container has to be filled before calling this histogram class
 */
class ZPrimeGenHists: public uhh2::Hists {
public:
    ZPrimeGenHists(uhh2::Context & ctx, const std::string & dirname);

    virtual void fill(const uhh2::Event & ev) override;

protected:
    TH1F *M_ZPrime_gen, *Pt_ZPrime_gen, *phi_ZPrime, *eta_ZPrime, *Pt_mu, *Pt_antimu, *eta_mu, *eta_antimu, *phi_mu, *phi_antimu
      //, *cosThetastar_LQ_LQLQframe, *cosThetastar_antiLQ_LQLQframe, *Pt_LQ_LQLQframe, *Pt_antiLQ_LQLQframe, *DecayChannel
;

    // TH2F* M_LQLQbar_vs_shat, *M_LQLQbar_vs_deltaR_LQ, *M_LQLQbar_vs_deltaR_antiLQ, *shat_vs_deltaR_LQ, *shat_vs_deltaR_antiLQ, *Pt_LQ_vs_deltaR_LQ, *Pt_antiLQ_vs_deltaR_antiLQ, *M_LQLQbar_vs_deltaR_top, *M_LQLQbar_vs_deltaR_antitop, *M_LQLQbar_vs_Pt_LQ, *M_LQLQbar_vs_Pt_antiLQ, *shat_vs_Pt_LQ, *shat_vs_Pt_antiLQ, *Pt_LQ_vs_Pt_antiLQ,/* *M_LQLQbar_vs_Pt_LQ_LQLQframe, *M_LQLQbar_vs_Pt_antiLQ_LQLQframe,*/ *M_LQLQbar_vs_eta_LQ, *M_LQLQbar_vs_eta_antiLQ;
    uhh2::Event::Handle<ZPrimeGen> h_ZPrimeGen;
};
