#include "UHH2/ttZPrime/include/ZPrimeGen.h"

using namespace std;
using namespace uhh2;

ZPrimeGen::ZPrimeGen(const vector<GenParticle> & genparticles, bool throw_on_failure)/*: m_type(e_notfound)*/ {
    int n_ZPrime = 0;
    for(unsigned int i=0; i<genparticles.size(); ++i) {
        const GenParticle & genp = genparticles[i];
        //cout << genp.pdgId() << endl;
        if (abs(genp.pdgId()) ==9000005 && genp.status() == 22){ // nciht nur die Zahl kann spaeter problem machen = ZPrime's
            auto mu = genp.daughter(&genparticles, 1);
            auto muAnti = genp.daughter(&genparticles, 2);
            if(!mu || !muAnti){
                if(throw_on_failure) throw runtime_error("ZPrimeGen: ZPrime has not ==2 daughters");
                return;
            }
            if(abs(mu->pdgId()) != 13){
                std::swap(mu, muAnti);
            }
            if(abs(mu->pdgId()) != 13){
                if(throw_on_failure) throw runtime_error("ZPrimeGen: ZPrime has no Mu daughter");
                return;
            }

            if(abs(muAnti->pdgId()) != 13){
                if(throw_on_failure) throw runtime_error("ZPrimeGen: ZPrime has no MuAnti daughter");
                return;
            }
            //cout << "Status" << genp.status() << endl;
            // now get W daughters:

            // auto topd1 = top->daughter(&genparticles, 1);
            // auto topd2 = top->daughter(&genparticles, 2);
            // if(!topd1 || !topd2){
            //     if(throw_on_failure) throw runtime_error("ZPrimeGen: top has not ==2 daughters");
            //     return;
            // }

            // now that we collected everything, fill the member variables.
            m_ZPrime = genp;
            m_muZPrime = *mu;
            m_muAntiZPrime = *muAnti;
            ++n_ZPrime;
            //cout << "Number of ZPrimes: " << n_ZPrime << endl;

        }
    }
    if(n_ZPrime != 1){
        if(throw_on_failure)  throw runtime_error("ZPrimeGen: did not find exactly one ZPrime in the event");
        return;
    }
}


GenParticle ZPrimeGen::ZPrime() const{
    return m_ZPrime;
}



GenParticle ZPrimeGen::MuZPrime() const{
    return m_muZPrime;
}


GenParticle ZPrimeGen::MuAntiZPrime() const{
    return m_muAntiZPrime;
}




ZPrimeGenProducer::ZPrimeGenProducer(uhh2::Context & ctx, const std::string & name, bool throw_on_failure_): throw_on_failure(throw_on_failure_){
    h_ZPrimeGen = ctx.get_handle<ZPrimeGen>(name);
}

bool ZPrimeGenProducer::process(Event & event){
    event.set(h_ZPrimeGen, ZPrimeGen(*event.genparticles, throw_on_failure));
    return true;
}
