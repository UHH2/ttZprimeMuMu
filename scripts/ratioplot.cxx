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
#include <math.h> 

using namespace std;

void ratioplot() {


   TString filePerfix = "uhh2.AnalysisModuleRunner.";
   
   TString pathOut = "/nfs/dust/cms/user/tiedemab/ttZPrime/Run2_10x/Control/NOMINAL/";
   TFile* input = new TFile(pathOut+filePerfix+"MC.TT_2L2Nu_2016v3.root","READ");
   
  std::vector<TString> histNamesCR;
  histNamesCR.push_back("3MuThirdMuonLooseScaled/pt_3");
  histNamesCR.push_back("3MuThirdMuonLooseScaled/isolation_3");
  histNamesCR.push_back("3MuThirdMuonLooseScaled/eta_3");
  histNamesCR.push_back("3MuLooseScaled_ZPrimeMuMu/M_mu1mu2");
  

  
  std::vector<TString> histNamesCR_Up;
  histNamesCR_Up.push_back("3MuThirdMuonLooseScaled_Up/pt_3");
  histNamesCR_Up.push_back("3MuThirdMuonLooseScaled_Up/isolation_3");
  histNamesCR_Up.push_back("3MuThirdMuonLooseScaled_Up/eta_3");
  histNamesCR_Up.push_back("3MuLooseScaled_Up_ZPrimeMuMu/M_mu1mu2");
//   

  std::vector<TString> histNamesSR;
  histNamesSR.push_back("3MuThirdMuonTight/pt_3");
  histNamesSR.push_back("3MuThirdMuonTight/isolation_3");
  histNamesSR.push_back("3MuThirdMuonTight/eta_3");
  histNamesSR.push_back("3MuTight_ZPrimeMuMu/M_mu1mu2");
  
  std::vector<TString> histLabel;
  histLabel.push_back("Mu_Pt_3");
  histLabel.push_back("Mu_Iso_3");
  histLabel.push_back("Mu_Eta_3");
  histLabel.push_back("M_mu1mu2");

//    TFile* output = new TFile(pathOut+filePerfix+"MC.TT_2L2Nu_2016v3.root","READ");

/*  TString pathCon = "/nfs/dust/cms/user/tiedemab/ttZPrime/Run2_10x/Fake/NOMINAL/";
  TFile* input = new TFile(pathCon+filePerfix+"MC.TT_2L2Nu_2016v3.root","READ");  */ 
//   TH1F* h1 =(TH1F*) input->Get("ElectronPlusMuMuTightControl_Muon/pt_2");
//   TH1F* h2 =(TH1F*) input->Get("ElectronPlusMuMuLooseControlScaled_Muon/pt_2");

//   TH1F* h1 =(TH1F*) input->Get("ElectronPlusMuMuTight/M_mue");
//   TH1F* h2 =(TH1F*) input->Get("ElectronPlusMuMuLooseScaled/M_mue");
//   
for(Int_t i = 0; i < histNamesCR.size() ;i++){
  TH1F* h1 =(TH1F*) input->Get(histNamesSR.at(i));
  TH1F* h2 =(TH1F*) input->Get(histNamesCR.at(i));
  TH1F* h2_up =(TH1F*) input->Get(histNamesCR_Up.at(i));

  
  std::cout << h1->Integral() << "\n";
  std::cout << h2->Integral() << "\n";
  
  Int_t nbins = h2->GetNbinsX();
  for(Int_t k = 1; k <= nbins; k++)
      
  {   
      if(h2->GetBinContent(k) <= 0.) continue;
      auto error = h2_up->GetBinContent(k)-h2->GetBinContent(k);
      h2->SetBinError(k,error);     
      
  }
  
  if(histLabel.at(i) != "M_mu1mu2")
  {
    h1->Rebin(5);
    h2->Rebin(5);
  }

   // Define the Canvas
   TCanvas *c = new TCanvas("c", "canvas", 800, 800);



   // Upper plot will be in pad1
   TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
   pad1->SetBottomMargin(0); // Upper and lower plot are joined
   pad1->SetGridx();         // Vertical grid
//    pad1->SetLogy();
   pad1->Draw();             // Draw the upper pad: pad1
   pad1->cd();               // pad1 becomes the current pad


   h1->SetStats(0);          // No statistics on upper plot
   h1->Draw();               // Draw h1
   h2->Draw("same");         // Draw h2 on top of h1

   // Do not draw the Y axis label on the upper plot and redraw a small
   // axis instead, in order to avoid the first label (0) to be clipped.
   // h1->GetYaxis()->SetLabelSize(0.);
   TGaxis *axis = new TGaxis( -5, 20, -5, 220, 20,220,510,"");
   axis->SetLabelFont(43); // Absolute font size in pixel (precision 3)
   axis->SetLabelSize(15);
   axis->Draw();

   // lower plot will be in pad
   c->cd();          // Go back to the main canvas before defining pad2
   TPad *pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
   pad2->SetTopMargin(0);
   pad2->SetBottomMargin(0.2);
   pad2->SetGridx(); // vertical grid
   pad2->Draw();
   pad2->cd();       // pad2 becomes the current pad

   // Define the ratio plot
   TH1F *h3 = (TH1F*)h1->Clone("h3");
   h3->SetLineColor(kBlack);
   h3->SetMinimum(0.01);  // Define Y ..
   h3->SetMaximum(2); // .. range
   h3->Sumw2();
   h3->SetStats(0);      // No statistics on lower plot
   h3->Divide(h2);
   h3->SetMarkerStyle(21);
   h3->Draw("ep");       // Draw the ratio plot

   // h1 settings
   h1->SetLineColor(kBlue+1);
   h1->SetLineWidth(2);

   // Y axis h1 plot settings
   h1->GetYaxis()->SetTitleSize(20);
   h1->GetYaxis()->SetTitleFont(43);
   h1->GetYaxis()->SetTitleOffset(1.55);

   // h2 settings
   h2->SetLineColor(kRed);
   h2->SetLineWidth(2);
   // Ratio plot (h3) settings
   h3->SetTitle(""); // Remove the ratio title

   // Y axis ratio plot settings
   h3->GetYaxis()->SetTitle("ratio");
   h3->GetYaxis()->SetNdivisions(505);
   h3->GetYaxis()->SetTitleSize(20);
   h3->GetYaxis()->SetTitleFont(43);
   h3->GetYaxis()->SetTitleOffset(1.55);
   h3->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
   h3->GetYaxis()->SetLabelSize(15);

   // X axis ratio plot settings
   h3->GetXaxis()->SetTitleSize(20);
   h3->GetXaxis()->SetTitleFont(43);
   h3->GetXaxis()->SetTitleOffset(4.);
   h3->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
   h3->GetXaxis()->SetLabelSize(15);

   pad1->cd();
   TLegend* legend;
   legend = new TLegend(0.3,0.7,0.8,0.9);
   legend->SetNColumns(2);
   legend->SetBorderSize(1);
   legend->SetTextSize(0.032);
   legend->SetLineColor(0);
   legend->SetFillColor(0);
   legend->SetFillStyle(1001);
   legend->AddEntry(h1,"TTbar","l");
   legend->AddEntry(h2,"FakeRateScaled","l");
   legend->Draw();

    

   c->Update();
   c->SaveAs("plots/"+histLabel.at(i)+"_test.png");
   h3->Reset();
   h2->Reset();
   h2_up->Reset();
   h1->Reset();
 }
input->Close();
}
