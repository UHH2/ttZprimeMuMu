#include <iostream>
#include <memory>

#include "UHH2/core/include/AnalysisModule.h"
#include "UHH2/core/include/Event.h"
#include "UHH2/ttZPrime/include/ZPrimeGen.h"
//#include "UHH2/common/include/PrintingModules.h"
#include "UHH2/ttZPrime/include/ZPrimeGenHists.h"


using namespace std;
using namespace uhh2;

namespace uhh2examples {
/** \brief Example for calculating and accessing the TTbarGen interpretation
 *
 */
class ZPrimeGenModule: public AnalysisModule {
public:

    explicit ZPrimeGenModule(Context & ctx);
    virtual bool process(Event & event) override;

private:
  // std::unique_ptr<AnalysisModule> printer;
  std::unique_ptr<AnalysisModule> ZPrimegenprod;
  std::unique_ptr<Hists> h_ZPrimegenhists;
  Event::Handle<ZPrimeGen> h_ZPrimegen;
};


ZPrimeGenModule::ZPrimeGenModule(Context & ctx){


  // printer.reset(new GenParticlesPrinter(ctx));
  ZPrimegenprod.reset(new ZPrimeGenProducer(ctx, "ZPrimeGen", false));
  h_ZPrimegen = ctx.get_handle<ZPrimeGen>("ZPrimegGen");
  h_ZPrimegenhists.reset(new ZPrimeGenHists(ctx, "ZPrimeGenHists"));
}


bool ZPrimeGenModule::process(Event & event) {
  //printer->process(event);
  ZPrimegenprod->process(event);
  bool isAnZPrime = false;
  //cout << "Test" << endl;

  h_ZPrimegenhists->fill(event);

    return true;
}

UHH2_REGISTER_ANALYSIS_MODULE(ZPrimeGenModule)
}
