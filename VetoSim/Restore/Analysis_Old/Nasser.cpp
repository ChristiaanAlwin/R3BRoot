#include <iostream>

#include "DrawHistDouma.h"

using namespace std;

void Nasser()
{
    TFile* f;
    
    // ========================================================================================
    
    f = new TFile("P_200MeV_NewCluster_run=1_NeuLAND.root","read");
    TH1D* P200New = (TH1D*) f->Get("hClusters");
    
    f = new TFile("N_200MeV_NewCluster_run=1_NeuLAND.root","read");
    TH1D* N200New = (TH1D*) f->Get("hClusters");
    
    f = new TFile("P_200MeV_OldCluster_run=1_NeuLAND.root","read");
    TH2D* P200Old_xy = (TH2D*) f->Get("h_ncl_etot");
    TH1D* P200Old = (TH1D*) P200Old_xy->ProjectionY();
    
    f = new TFile("N_200MeV_OldCluster_run=1_NeuLAND.root","read");
    TH2D* N200Old_xy = (TH2D*) f->Get("h_ncl_etot");
    TH1D* N200Old = (TH1D*) N200Old_xy->ProjectionY();
    
    TCanvas* c200 = DrawHistDouma(P200New,"Number of Clusters for 200 MeV","Number of Clusters","Counts",2);
   
    N200New->UseCurrentStyle();
    N200New->SetLineWidth(2.0);
    N200New->SetLineColor(8);
    N200New->Draw("same");
    
    P200Old->UseCurrentStyle();
    P200Old->SetLineWidth(2.0);
    P200Old->SetLineColor(46);
    P200Old->Draw("same");
    
    N200Old->UseCurrentStyle();
    N200Old->SetLineWidth(2.0);
    N200Old->SetLineColor(32);
    N200Old->Draw("same");
    
    TPaveText** text = new TPaveText*[4];
    Double_t Xmin = -0.5;
    Double_t Xmax = 105.5;
    Double_t Ymin = 0.0;
    Double_t Ymax = 900.0;
    Double_t Xlength = Xmax - Xmin;
    Double_t Ylength = Ymax - Ymin;
    for (Int_t k = 0; k<4; ++k)
    {   
      text[k] = new TPaveText(Xmin+Xlength*0.72,Ymin+Ylength*(0.93-0.05*((Int_t) (k+1))),Xmin+Xlength*0.73,Ymin+Ylength*(0.93-0.05*((Int_t) (k+1))+0.01),"NB");
      text[k]->SetFillColor(0); // Color of the box.
      text[k]->SetTextSize(0.03); // Define text size
      text[k]->SetTextFont(1); // Define text font
      text[k]->SetTextAngle(0.0); // Defines the angle at which the text is displayed.
    }
  
    // Define the text:
    text[0]->AddText("Protons with merging");
    text[1]->AddText("Neutrons with Merging");
    text[2]->AddText("Protons No merging");
    text[3]->AddText("Neutrons No merging");
  
    // Set the colors:
    text[0]->SetTextColor(2);
    text[1]->SetTextColor(8);
    text[2]->SetTextColor(46);
    text[3]->SetTextColor(32);
    
    text[0]->Draw("same");
    text[1]->Draw("same");
    text[2]->Draw("same");
    text[3]->Draw("same");
    
    c200->SaveAs("200MeV");
    
    // ========================================================================================
    
}