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

void cutflow()
{
  TString pathPre = "/nfs/dust/cms/user/tiedemab/ttZPrime/Run2_10x/Preselection/NOMINAL/";
  TString pathReco = "/nfs/dust/cms/user/tiedemab/ttZPrime/Run2_10x/Reconstruction/NOMINAL/test/";
  TString filePerfix = "uhh2.AnalysisModuleRunner.MC.";
  std::cout <<pathReco << std::endl;
  std::vector<TString> filenamesPre;
  filenamesPre.push_back("DYJets");
  filenamesPre.push_back("Other");
  filenamesPre.push_back("TTBar");
  filenamesPre.push_back("TTZ");
  filenamesPre.push_back("ZPrimeM300");
  filenamesPre.push_back("ZPrimeM700");
  filenamesPre.push_back("ZPrimeM1000");
  filenamesPre.push_back("ZPrimeM1400");

  std::vector<TString> filenamesReco;
  filenamesReco.push_back("TTBar_2016v3");
  filenamesReco.push_back("ZPrimeM300_2016v3");

  std::vector<TString> selectionsPre;
  selectionsPre.push_back("Cleaner");
  selectionsPre.push_back("1Mu");
  selectionsPre.push_back("2Jets");
  selectionsPre.push_back("1BJet");
  selectionsPre.push_back("StSel");

  std::vector<TString> selectionsReco;
  selectionsReco.push_back("2Mu");
  selectionsReco.push_back("2Mu4Jets");
  selectionsReco.push_back("2MuChi2");

  Int_t nPre = selectionsPre.size() , nReco =selectionsReco.size();
  Int_t n = nPre+nReco;

  std::vector<TFile*> filesPre;
  for(auto filename : filenamesPre)
  {
    TFile* file = new TFile(pathPre+filePerfix+filename+".root","READ");
    filesPre.push_back(file);
  }

  std::vector<TFile*> filesReco;
  std::vector<TH1F*> hists;
  for(auto filename : filenamesReco)
  {
    TFile* file = new TFile(pathReco+filePerfix+filename+".root","READ");
    TH1F* hist = new TH1F(filename,"Cut Flow",n,0,n-1);
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

  for(Int_t k = 0; k <filesPre.size(); k++)
  {
    TString name = selectionsPre.at(0)+"_Counter/NEvt";
    TH1F* hist =(TH1F*) filesPre.at(k)->Get(name);
    Double_t nEventsTotal = hist->Integral();


    Double_t y[n], x[n];
    for(Int_t i = 0; i< nPre; i++)
    {
      name = selectionsPre.at(i)+"_Counter/NEvt";
      hist = (TH1F*) filesPre.at(k)->Get(name);
      Double_t nEvents = hist->Integral();
      hists.at(k)->SetBinContent(i+1,nEvents/nEventsTotal);
      hists.at(k)->GetXaxis()->SetBinLabel(i+1,selectionsPre.at(i));
      hists.at(k)->GetXaxis()->SetTitle("Cuts");
      hists.at(k)->GetYaxis()->SetTitle("Efficiency");
      std::cout << name << "-Eff = " << nEvents/nEventsTotal << std::endl;
    }
    for(Int_t i = nPre; i< n; i++)
    {
      name = selectionsReco.at(i-nPre)+"_Counter/NEvt";
      hist = (TH1F*) filesReco.at(k)->Get(name);
      hists.at(k)->GetXaxis()->SetBinLabel(i+1,selectionsReco.at(i-nPre));
      Double_t nEvents = hist->Integral();
      hists.at(k)->SetBinContent(i+1,nEvents/nEventsTotal);
      std::cout << name << "-Eff = " << nEvents/nEventsTotal << std::endl;
    }
    // gr = new TGraph (n, x, y);
    // //gr->SetTitle(filenamesPre.at(k));
    // gr->SetMarkerStyle(20);
    hists.at(k)->SetMarkerColor(k+1);
    // gr->Draw("APsame");
    // c1->Update();
    legend->AddEntry(hists.at(k),filenamesPre.at(k),"p");
    // std::string dummy;
    // std::cout << "Enter to continue..." << std::endl;
    // std::getline(std::cin, dummy);
  }

  hists.at(0)->Draw("p");
  hists.at(1)->Draw("psame");
  legend->Draw();
  c1->SaveAs("cutflow.png");


}
