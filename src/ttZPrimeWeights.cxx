#include "UHH2/core/include/Event.h"
#include "UHH2/core/include/Utils.h"
#include "UHH2/common/include/Utils.h"
#include "UHH2/ttZPrime/include/ttZPrimeWeights.h"
#include "Riostream.h"
#include "TFile.h"
#include "TH1F.h"

#include <stdexcept>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <sstream>
#include <string>

using namespace uhh2;
using namespace std;


 /*
 std::string split implementation by using delimiter as a character.
 */
 std::vector<double> split(std::string strToSplit, char delimeter)
 {
    std::stringstream ss(strToSplit);
    std::string item;
    std::vector<double> splittedStrings;
    while (std::getline(ss, item, delimeter))
    {
        double value = strtod(item.c_str(), NULL);
        splittedStrings.push_back(value);
    }
    return splittedStrings;
}

MuFakeRateWeight1D::MuFakeRateWeight1D(std::string dirname_, unsigned int MuonAt_):
dirname(dirname_),
MuonAt(MuonAt_)
{}

bool MuFakeRateWeight1D::process(uhh2::Event & event){
    if (event.muons->size() < (MuonAt+1)) throw std::runtime_error("Insufficient muons:"+std::to_string(MuonAt+1)+" muons are required but "+std::to_string(event.muons->size())+" are available" );
    auto muon = event.muons->at(MuonAt);
    double mu_iso = muon.relIso();
//     cout << "MuonIso: " << mu_iso << "MuEta: " << mu_eta << "\n";
    fstream mapfile;
    mapfile.open(dirname,ios::in);
    if(!mapfile.is_open()) throw std::runtime_error("Map file "+dirname+" not found");
    string tp;
    while(getline(mapfile, tp)){ 
         
        
//         cout << tp << "\n";
        vector<double> values = split(tp, ' ');
        if(values.size() != 4) throw std::runtime_error("Number of entries per line is not correct: 4 are expected but "+std::to_string(values.size())+"are found");
//         for(auto value : values)
//         {
//             cout << value << ", ";
//         }        
//         cout << "\n" << "New Line" << "\n";
        bool inIso = mu_iso >= values.at(0) && mu_iso < values.at(1);
        if(inIso) 
        {
            factor = values.at(2);
//             cout << "New Weight" << "\n"; 
            break;
        }
    }
    
//     std::cout<< "Scale factor: " << factor << "\n";
    event.weight *= factor; 
    mapfile.close();
    return true;
}

MuFakeRateWeight::MuFakeRateWeight(uhh2::Context & ctx, std::string dirname_, unsigned int MuonAt_, const std::string & sys_uncert):
dirname(dirname_),
MuonAt(MuonAt_),
  h_muon_weight_      (ctx.declare_event_output<float>("weight_mufake")),
  h_muon_weight_up_   (ctx.declare_event_output<float>("weight_mufake_up")),
  h_muon_weight_down_ (ctx.declare_event_output<float>("weight_mufake_down"))
{
  sys_direction_ = 0;
  if (sys_uncert == "up") {
    sys_direction_ = 1;
  } else if (sys_uncert == "down") {
    sys_direction_ = -1;
  }
}

bool MuFakeRateWeight::process(uhh2::Event & event){
    if (event.muons->size() < (MuonAt+1)) throw std::runtime_error("Insufficient muons:"+std::to_string(MuonAt+1)+" muons are required but "+std::to_string(event.muons->size())+" are available" );
    auto muon = event.muons->at(MuonAt);
    double mu_iso = muon.relIso(), mu_eta = muon.eta();
//     cout << "MuonIso: " << mu_iso << "MuEta: " << mu_eta << "\n";
    fstream mapfile;
    mapfile.open(dirname,ios::in);
    if(!mapfile.is_open()) throw std::runtime_error("Map file "+dirname+" not found");
    string tp;
    while(getline(mapfile, tp)){ 
         
        
//         cout << tp << "\n";
        vector<double> values = split(tp, ' ');
        if(values.size() != 6) throw std::runtime_error("Number of entries per line is not correct: 6 are expected but "+std::to_string(values.size())+"are found");
//         for(auto value : values)
//         {
//             cout << value << ", ";
//         }        
//         cout << "\n" << "New Line" << "\n";
        bool inIso = mu_iso >= values.at(0) && mu_iso < values.at(1);
        bool inEta = mu_eta >= values.at(2) && mu_eta < values.at(3);
        if(inIso && inEta) 
        {
            factor = values.at(4);
            factor_up = values.at(4) + values.at(5);
            factor_down = values.at(4) - values.at(5);
//             cout << "New Weight" << "\n"; 
            break;
        }
    }
    
//     std::cout<< "Scale factor: " << factor << "\n";
    
    event.set(h_muon_weight_,       event.weight*factor);
    event.set(h_muon_weight_up_,    event.weight*factor_up);
    event.set(h_muon_weight_down_,  event.weight*factor_down);
    
    if (sys_direction_ == 1) {
        event.weight *= factor_up;
    }
    else if (sys_direction_ == -1) {
        event.weight *= factor_down;
    } 
    else {
        event.weight *= factor;
    }
    
    mapfile.close();
    return true;
}


