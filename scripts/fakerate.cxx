#include "TFile.h"
#include "TGraph.h"
#include "TStyle.h"
#include "TPaveText.h"
#include "TH1.h"
#include "TPad.h"
#include "TLegend.h"
#include "TVirtualPad.h"
#include <iostream>
#include <fstream>
#include "TCanvas.h"
#include <vector>
#include "TString.h"
#include <array>

using namespace std;


void fakerate ()
{
  bool debug = true;
//   TString pathCon = "/nfs/dust/cms/user/tiedemab/ttZPrime/Run2_10x/Fake/NOMINAL/";
  TString pathCon = "/nfs/dust/cms/user/tiedemab/ttZPrime/Run2_10x/Control/NOMINAL/";
  TString filePerfix = "uhh2.AnalysisModuleRunner.MC.";

  std::vector<TString> filenamesCon;
//   filenamesCon.push_back("TTBar");
  filenamesCon.push_back("TT_2L2Nu_2016v3");
//   filenamesCon.push_back("TTZ");
//   filenamesCon.push_back("DYJets");
//   filenamesCon.push_back("Diboson");


  std::vector<TString> histNamesCR;
//   histNamesCR.push_back("ElectronPlusMuMuLoose/M_mue");
//   histNamesCR.push_back("ElectronPlusMuMuLooseControl_Muon/pt_1");
//   histNamesCR.push_back("ElectronPlusMuMuLooseControl_Muon/isolation_1");
//   histNamesCR.push_back("ElectronPlusMuMuLooseControl_Muon/eta_1");
//   histNamesCR.push_back("ElectronPlusMuMuLooseControl_Muon/pt_2");
//   histNamesCR.push_back("ElectronPlusMuMuLooseControl_Muon/isolation_2");
//   histNamesCR.push_back("ElectronPlusMuMuLooseControl_Muon/eta_2");
//   histNamesCR.push_back("ElectronPlusMuMuLooseControl_Muon/pt");
//   histNamesCR.push_back("ElectronPlusMuMuLooseControl_Muon/isolation");
//   histNamesCR.push_back("ElectronPlusMuMuLooseControl_Muon/eta");
  histNamesCR.push_back("3MuThirdMuonLoose/pt_3");
  histNamesCR.push_back("3MuThirdMuonLoose/isolation_3");
  histNamesCR.push_back("3MuThirdMuonLoose/eta_3");

  std::vector<TString> histNamesSR;
//   histNamesSR.push_back("ElectronPlusMuMuTight/M_mue");
//   histNamesSR.push_back("ElectronPlusMuMuTightControl_Muon/pt_1");
//   histNamesSR.push_back("ElectronPlusMuMuTightControl_Muon/isolation_1");
//   histNamesSR.push_back("ElectronPlusMuMuTightControl_Muon/eta_1");
//   histNamesSR.push_back("ElectronPlusMuMuTightControl_Muon/pt_2");
//   histNamesSR.push_back("ElectronPlusMuMuTightControl_Muon/isolation_2");
//   histNamesSR.push_back("ElectronPlusMuMuTightControl_Muon/eta_2");
//   histNamesSR.push_back("ElectronPlusMuMuTightControl_Muon/pt");
//   histNamesSR.push_back("ElectronPlusMuMuTightControl_Muon/isolation");
//   histNamesSR.push_back("ElectronPlusMuMuTightControl_Muon/eta");
  histNamesSR.push_back("3MuThirdMuonTight/pt_3");
  histNamesSR.push_back("3MuThirdMuonTight/isolation_3");
  histNamesSR.push_back("3MuThirdMuonTight/eta_3");

  std::vector<TString> histLabel;
//   histLabel.push_back("M_mue");
//   histLabel.push_back("Mu_Pt_1");
//   histLabel.push_back("Mu_Iso_1");
//   histLabel.push_back("Mu_Eta_1");
//   histLabel.push_back("Mu_Pt_2");
//   histLabel.push_back("Mu_Iso_2");
//   histLabel.push_back("Mu_Eta_2");
//   histLabel.push_back("Mu_Pt");
//   histLabel.push_back("Mu_Iso");
//   histLabel.push_back("Mu_Eta");
  histLabel.push_back("Mu_Pt_3");
  histLabel.push_back("Mu_Iso_3");
  histLabel.push_back("Mu_Eta_3");

  // TFile* input = new TFile(pathOut+filePerfix+"TTBar.root","READ");
  // TH1F* outhist =(TH1F*) input->Get("3MuLoose_ZPrimeMuMu/M_mu1mu2");
  //
  // TFile* output = new TFile(pathOut+filePerfix+"TTBar_Fake.root","RECREATE");


  TCanvas *c1 = new TCanvas("c1","demo quantiles",10,10,800,600);
  // c1->SetLogy();

  std::vector<TFile*> files;
  for(auto filename : filenamesCon){
    TFile* file = new TFile(pathCon+filePerfix+filename+".root","READ");
    files.push_back(file);
  }





  for(Int_t k = 0; k < files.size(); k++)//Loop over files
  {
    for(Int_t i = 0; i < histNamesSR.size(); i++)
    {
//       ofstream myfile;
//       myfile.open (filenamesCon.at(k)+"_"+histLabel.at(i)+"_1DMap.txt");
      TH1F* histCR = (TH1F*) files.at(k)->Get(histNamesCR.at(i));
      TH1F* histSR = (TH1F*) files.at(k)->Get(histNamesSR.at(i));
      histCR->Rebin(5);
      histSR->Rebin(5);
      TH1F* hist = (TH1F*)histSR->Clone(filenamesCon.at(k)+"_"+histLabel.at(i)+"_FakeRate");
      hist->Divide(histCR);
      hist->Draw("E");
      c1->Update();
      c1->SaveAs("plots/"+filenamesCon.at(k)+"_"+histLabel.at(i)+"_FakeRate.png");
//       Int_t nbins = hist->GetNbinsX();
//       for(Int_t n =1; n < nbins; n++){
//             auto value = hist->GetBinContent(n);
//             auto error = hist->GetBinError(n);
//             auto xmin = hist->GetBinLowEdge(n), xmax = (hist->GetBinLowEdge(n) + hist->GetBinWidth(n));
//             if(value > 0.)  myfile << xmin <<" " << xmax<<  " " << value << " " << error <<"\n";
//       }
//       myfile.close();
      hist->Reset();
      histCR->Reset();
      histSR->Reset();
    }


    files.at(k)->Close();


  }
  // output->cd();
  // outhist->Write();
  // output->Write();
  // output->Close();
  // input->Close();
}
