#include "TFile.h"
#include "TH1.h"
#include "TGraph.h"
#include "TLegend.h"
#include "TStyle.h"

#include <iostream>

void ROC_Curve()
{
TString path = "/nfs/dust/cms/user/tiedemab/ttZPrime/Run2_10x/Reconstruction/NOMINAL/test/";
TFile* file1 = new TFile(path+"uhh2.AnalysisModuleRunner.MC.ZPrimeM300_2016v3.root","READ");
TFile* file2 = new TFile(path+"uhh2.AnalysisModuleRunner.MC.TTBar_2016v3.root","READ");

TH1F *hist1 = (TH1F*) file1->Get("2Mu_TopRecoHad/Discriminator");
TH1F *hist2 = (TH1F*) file2->Get("2Mu_TopRecoHad/Discriminator");

double rate1 = hist1->Integral();
double rate2 = hist2->Integral();

Int_t n =hist1->GetNbinsX();

Double_t x[n], y1[n], y2[n] , y3[n];
for (Int_t i=0; i<n; i++) {
   x[i] = hist1->GetXaxis()->GetBinCenter(i);
   y1[i] = (hist1->Integral(0,i)/rate1);
   y2[i] = 1-(hist2->Integral(0,i)/rate2);
   y3[i] = (hist1->Integral(0,i+1)/rate1)/sqrt((hist1->Integral(0,i+1)/rate1)+(hist2->Integral(0,i+1)/rate2));
}
TGraph *gr1 = new TGraph (n, x, y1);
TGraph *gr2 = new TGraph (n, x, y2);
TGraph *gr3 = new TGraph (n, x, y3);


gr1->SetMarkerStyle(20);
gr2->SetMarkerStyle(21);
gr1->SetMarkerColor(4);
gr2->SetMarkerColor(2);
gr3->SetMarkerStyle(22);
gr3->SetMarkerColor(2);


gr1->SetTitle("ROC Curve Top Reco.");
gr1->GetXaxis()->SetTitle("#Chi^{2}_{Cut}");
gr1->GetYaxis()->SetTitle("Efficiency/Rejection");

TLegend* legend;
legend = new TLegend(0.65,0.50,0.87,0.66);
legend->SetNColumns(2);
legend->SetBorderSize(1);
legend->SetTextSize(0.032);
legend->SetLineColor(0);
legend->SetFillColor(0);
legend->SetFillStyle(1001);
legend->AddEntry(gr1,"ZPrime_{300GeV}","p");
legend->AddEntry(gr2,"TTBar","p");

auto c1= new TCanvas();
gr1->Draw("AP");
gr2->Draw("SameP");
legend->Draw();
c1->SaveAs("TopReco_ROC_Curve.png");

std::string dummy;
std::cout << "Enter to continue..." << std::endl;
std::getline(std::cin, dummy);
gr3->Draw("AP");


}
