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

void Map ()
{
  bool debug = true;
  TString pathCon = "/nfs/dust/cms/user/tiedemab/ttZPrime/Run2_10x/Fake/NOMINAL/";
  TString filePerfix = "uhh2.AnalysisModuleRunner.";

  std::vector<TString> filenamesCon;
  filenamesCon.push_back("MC.TT_2L2Nu_2016v3");
//   filenamesCon.push_back("TTZ");
//   filenamesCon.push_back("DYJets");
//   filenamesCon.push_back("Diboson");


  std::vector<TString> histNamesCR;
  histNamesCR.push_back("ElectronPlusMuMuLoose/SecoendMuIsoVsEta");


  std::vector<TString> histNamesSR;
  histNamesSR.push_back("ElectronPlusMuMuTight/SecoendMuIsoVsEta");






  TCanvas *c1 = new TCanvas("c1","demo quantiles",10,10,800,600);
  gStyle->SetOptStat(0);
  // c1->SetLogy();

  std::vector<TFile*> files;
  for(auto filename : filenamesCon){
    TFile* file = new TFile(pathCon+filePerfix+filename+".root","READ");
    files.push_back(file);
  }





  for(Int_t k = 0; k < files.size(); k++)//Loop over files
  {
    ofstream myfile;
    myfile.open (filenamesCon.at(k)+"_FakeRateMap.txt");
    TH2F* histCR = (TH2F*) files.at(k)->Get(histNamesCR.at(0));
    TH2F* histSR = (TH2F*) files.at(k)->Get(histNamesSR.at(0));
    TH2F* hist = (TH2F*)histSR->Clone(filenamesCon.at(k)+"_SecoendMuIsoVsEta");
    hist->Divide(histCR);
    Int_t ybins = hist->GetNbinsY(), xbins = hist->GetNbinsX();
    for(Int_t x = 1; x <= xbins; x++)
    {
      for(Int_t y = 1; y <= ybins; y++)
      {
        auto value = hist->GetBinContent(x,y);
        auto error = hist->GetBinError(x,y);
        double xmin = hist->GetXaxis()->GetBinLowEdge(x), xmax = hist->GetXaxis()->GetBinLowEdge(x)+hist->GetXaxis()->GetBinWidth(x);
        double ymin = hist->GetYaxis()->GetBinLowEdge(y), ymax = hist->GetYaxis()->GetBinLowEdge(y)+hist->GetYaxis()->GetBinWidth(y); 
        if(value > 0.) myfile << xmin <<" " << xmax<<" " << ymin << " " << ymax << " " << value << " " << error <<"\n";
      }
    }
    hist->Draw("COLZ");
    c1->Update();
    c1->SaveAs("plots/"+filenamesCon.at(k)+"_SecoendMuIsoVsEtaRatioTest.png");
    hist->Reset();
    histCR->Reset();
    histSR->Reset();
    files.at(k)->Close();
    myfile.close();

  }
}
