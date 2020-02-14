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






void branching()
{

  TString pathReco = "/nfs/dust/cms/user/tiedemab/ttZPrime/Run2_10x/Reconstruction/NOMINAL/";
  TString filePerfix = "uhh2.AnalysisModuleRunner.MC.";
  std::cout <<pathReco << std::endl;


  std::vector<TString> filenamesReco;
  filenamesReco.push_back("ZPrimeM300");
  filenamesReco.push_back("ZPrimeM700");
  filenamesReco.push_back("ZPrimeM1000");
  filenamesReco.push_back("ZPrimeM1400");


  std::vector<TString> selectionsReco;
  selectionsReco.push_back("DRMuJet");
  selectionsReco.push_back("2Mu");
  selectionsReco.push_back("3Mu");
  selectionsReco.push_back("4MuAndMore");

  std::vector<TString> labels;
  labels.push_back("2#mu");
  labels.push_back("2#mu + l");
  labels.push_back("2#mu + 2l");


 std::vector<TString> legnames;
 legnames.push_back("Z'_{300}");
 legnames.push_back("Z'_{700}");
 legnames.push_back("Z'_{1000}");
 legnames.push_back("Z'_{1400}");

  Int_t nReco =selectionsReco.size();
  Int_t n = nReco;


  std::vector<TFile*> filesReco;
  std::vector<TH1F*> hists;
  std::cout << filenamesReco.size() << std::endl;

  for(auto filename : filenamesReco)
  {

    TFile* file = new TFile(pathReco+filePerfix+filename+"_2016v3.root","READ");
    TH1F* hist = new TH1F(filename,"",n,0,n-1);
    hist->SetMarkerStyle(20);
    hists.push_back(hist);
    filesReco.push_back(file);
  }

  auto c1= new TCanvas();
  c1->SetLogy();
  gStyle->SetOptStat(0);
  gStyle->SetPalette(57);
  TLegend* legend;
  TGraph* gr;
  legend = new TLegend(0.12,0.11,0.5,0.6);
  legend->SetNColumns(2);
  legend->SetBorderSize(1);
  legend->SetTextSize(0.032);
  legend->SetLineColor(0);
  legend->SetFillColor(0);
  legend->SetFillStyle(1001);

  for(Int_t k = 0; k <filenamesReco.size(); k++)
  {
    TString name = selectionsReco.at(0)+"_Counter/NEvt";
    TH1F* hist =(TH1F*) filesReco.at(k)->Get(name);
    Double_t nEventsTotal = hist->Integral();



    for(Int_t i = 1; i< n; i++)
    {
      name = selectionsReco.at(i)+"_Counter/NEvt";
      hist = (TH1F*) filesReco.at(k)->Get(name);
      hists.at(k)->GetXaxis()->SetBinLabel(i+1,labels.at(i-1));
      hists.at(k)->GetXaxis()->SetTitle("Signal Region");
      hists.at(k)->GetXaxis()->SetLabelSize(0.05);
      // hists.at(k)->GetYaxis()->SetLabelSize(0.05);
      hists.at(k)->GetYaxis()->SetTitle("Branching Fraction");
      hists.at(k)->GetXaxis()->SetTitleOffset(1.25);

      Double_t nEvents = hist->Integral();
      hists.at(k)->SetBinContent(i+1,nEvents/nEventsTotal);
      std::cout << name << "-branch = " << nEvents/nEventsTotal << std::endl;
    }
    // gr = new TGraph (n, x, y);
    // //gr->SetTitle(filenamesPre.at(k));
    // gr->SetMarkerStyle(20);
    hists.at(k)->SetMarkerColor(k+1);
    // gr->Draw("APsame");
    // c1->Update();
    legend->AddEntry(hists.at(k),legnames.at(k),"p");
    // std::string dummy;
    // std::cout << "Enter to continue..." << std::endl;
    // std::getline(std::cin, dummy);
  }

  hists.at(0)->Draw("p");
  for(Int_t k = 1; k <filesReco.size(); k++)
  {
      hists.at(k)->Draw("psame");
  }
  legend->Draw();
  c1->SaveAs("branching.png");


}
