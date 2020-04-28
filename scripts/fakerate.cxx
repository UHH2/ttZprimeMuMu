#include "TFile.h"
#include "TGraph.h"
#include "TStyle.h"
#include "TPaveText.h"
#include "TH1.h"
#include "TPad.h"
#include "TLegend.h"
#include "TVirtualPad.h"
#include <iostream>
#include "TCanvas.h"
#include <vector>
#include "TString.h"
#include <array>


void fakerate ()
{
  bool debug = true;
  TString pathCon = "/nfs/dust/cms/user/tiedemab/ttZPrime/Run2_10x/Fake/NOMINAL/";
  TString pathOut = "/nfs/dust/cms/user/tiedemab/ttZPrime/Run2_10x/Control/NOMINAL/";
  TString filePerfix = "uhh2.AnalysisModuleRunner.MC.";

  std::vector<TString> filenamesCon;
  filenamesCon.push_back("TTBar");
  filenamesCon.push_back("TTZ");
  filenamesCon.push_back("DYJets");
  filenamesCon.push_back("Diboson");


  std::vector<TString> histNamesCR;
  histNamesCR.push_back("ElectronPlusMuMuLoose/M_mue");
  histNamesCR.push_back("ElectronPlusMuMuLooseControl_Muon/pt_1");
  histNamesCR.push_back("ElectronPlusMuMuLooseControl_Muon/isolation_1");
  histNamesCR.push_back("ElectronPlusMuMuLooseControl_Muon/eta_1");
  histNamesCR.push_back("ElectronPlusMuMuLooseControl_Muon/pt_2");
  histNamesCR.push_back("ElectronPlusMuMuLooseControl_Muon/isolation_2");
  histNamesCR.push_back("ElectronPlusMuMuLooseControl_Muon/eta_2");
  histNamesCR.push_back("ElectronPlusMuMuLooseControl_Muon/pt");
  histNamesCR.push_back("ElectronPlusMuMuLooseControl_Muon/isolation");
  histNamesCR.push_back("ElectronPlusMuMuLooseControl_Muon/eta");

  std::vector<TString> histNamesSR;
  histNamesSR.push_back("ElectronPlusMuMuTight/M_mue");
  histNamesSR.push_back("ElectronPlusMuMuTightControl_Muon/pt_1");
  histNamesSR.push_back("ElectronPlusMuMuTightControl_Muon/isolation_1");
  histNamesSR.push_back("ElectronPlusMuMuTightControl_Muon/eta_1");
  histNamesSR.push_back("ElectronPlusMuMuTightControl_Muon/pt_2");
  histNamesSR.push_back("ElectronPlusMuMuTightControl_Muon/isolation_2");
  histNamesSR.push_back("ElectronPlusMuMuTightControl_Muon/eta_2");
  histNamesSR.push_back("ElectronPlusMuMuTightControl_Muon/pt");
  histNamesSR.push_back("ElectronPlusMuMuTightControl_Muon/isolation");
  histNamesSR.push_back("ElectronPlusMuMuTightControl_Muon/eta");

  std::vector<TString> histLabel;
  histLabel.push_back("M_mue");
  histLabel.push_back("Mu_Pt_1");
  histLabel.push_back("Mu_Iso_1");
  histLabel.push_back("Mu_Eta_1");
  histLabel.push_back("Mu_Pt_2");
  histLabel.push_back("Mu_Iso_2");
  histLabel.push_back("Mu_Eta_2");
  histLabel.push_back("Mu_Pt");
  histLabel.push_back("Mu_Iso");
  histLabel.push_back("Mu_Eta");
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
      TH1F* histCR = (TH1F*) files.at(k)->Get(histNamesCR.at(i));
      TH1F* histSR = (TH1F*) files.at(k)->Get(histNamesSR.at(i));
      TH1F* hist = (TH1F*)histSR->Clone(filenamesCon.at(k)+"_"+histLabel.at(i)+"_FakeRate");
      hist->Divide(histCR);
      hist->Draw("E");
      c1->Update();
      c1->SaveAs("plots/"+filenamesCon.at(k)+"_"+histLabel.at(i)+"_FakeRate.png");
      hist->Reset();
      histCR->Reset();
      histSR->Reset();
    }

    // Int_t NBins = histLoose->GetNbinsX();

    // outhist->Multiply(hist);

    // TH1F* hist = new TH1F("FakeRate",filenamesCon.at(k)+"_FakeRate",NBins,histLoose->GetBinLowEdge(1), (histLoose->GetBinLowEdge(NBins)+histLoose->GetBinWidth(NBins)));
    // for (Int_t k = 1; k <= NBins; k++) //Loop over bins
    // {
    //   Double_t NFakeLoose = histLoose->GetBinContent(k), NFakeTight = histTight->GetBinContent(k);
    //   Double_t ErrorLoose = histLoose->GetBinError(k), ErrorTight = histTight->GetBinError(k);
    //   if(debug) std::cout << "NLoose" << NFakeLoose << '\n';
    //   if(debug) std::cout << "NTight" << NFakeTight << '\n';
    //   Double_t fakeRate, error = 0.;
    //   if(NFakeLoose > 0.)
    //   {
    //     fakeRate = NFakeTight/NFakeLoose;
    //     error = 1/NFakeLoose*(ErrorTight-fakeRate*ErrorLoose);
    //   }
    //   else fakeRate = 1.;
    //   hist->SetBinContent(k,fakeRate);
    //   hist->SetBinError(k,error);
    //   if(debug) std::cout << "FakeRate:" << fakeRate<< '\n';
    //   Double_t NEventOut = (outhist->GetBinContent(k))*fakeRate;
    //   Double_t ErrorOut = (outhist->GetBinError(k))*fakeRate + (outhist->GetBinContent(k))*error;
    //   outhist->SetBinContent(k,NEventOut);
    //   outhist->SetBinError(k,ErrorOut);
    // }

    files.at(k)->Close();


  }
  // output->cd();
  // outhist->Write();
  // output->Write();
  // output->Close();
  // input->Close();
}
