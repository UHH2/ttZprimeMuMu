
#include "UHH2/ttZPrime/include/TTbarRecoHad.h"
#include "UHH2/core/include/LorentzVector.h"
#include "UHH2/common/include/Utils.h"
#include "UHH2/core/include/Utils.h"

#include <cassert>

using namespace uhh2;
using namespace std;

TTbarRecoHad::TTbarRecoHad(Context & ctx, const string & label) {
    h_recohyps = ctx.get_handle<vector<TTbarRecoHadHypothesis>>(label);
}

TTbarRecoHad::~TTbarRecoHad() {}

bool TTbarRecoHad::process(uhh2::Event & event) {
    assert(event.jets);
    std::vector<TTbarRecoHadHypothesis> recoHyps;
    unsigned int n_jets = event.jets->size();
    if(n_jets>7) n_jets=7; //avoid crashes in events with many jets
    // idea: loop over 3^Njet possibilities and write the current loop
    // index j in the 3-base system. The Njets digits represent whether
    // to assign each jet to the hadronic side (0), leptonic side (1),
    // or none of them (2).
    const unsigned int max_j = pow(3, n_jets);

    for (unsigned int j=0; j < max_j; j++) {
            LorentzVector tophad1_v4;
            LorentzVector tophad2_v4;
            int hadjets1=0;
            int hadjets2=0;
            int num = j;
            TTbarRecoHadHypothesis hyp;
            for (unsigned int k=0; k<n_jets; k++) {
                if(num%3==0) {
                    tophad1_v4 = tophad1_v4 + event.jets->at(k).v4();
                    hyp.add_tophad1_jet(event.jets->at(k));
                    hadjets1++;
                }

                if(num%3==1) {
                    tophad2_v4 = tophad2_v4 + event.jets->at(k).v4();
                    hyp.add_tophad2_jet(event.jets->at(k));
                    hadjets2++;
                }
                //in case num%3==2 do not take this jet at all
                //shift the trigits of num to the right:
                num /= 3;
            }
            //fill only hypotheses with at least one jet assigned to each top quark
            if(hadjets1>0 && hadjets2>0) {
                hyp.set_tophad1_v4(tophad1_v4);
                hyp.set_tophad2_v4(tophad2_v4);
                recoHyps.emplace_back(std::move(hyp));

            }
    } // 3^n_jets jet combinations
    event.set(h_recohyps, std::move(recoHyps));
    return true;
}
