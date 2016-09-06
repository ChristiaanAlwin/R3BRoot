// include the required C++ headers:
#include <iostream>

// include the required root headers:
#include "TMath.h"
#include "TGraphErrors.h"
#include "TSystem.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH1I.h"

// include own headers:
#include "GetSimplePerformance.cpp"
#include "DrawGraphDouma.h"

// use the standard C++ variables:
using namespace std;

void SimplePerformanceSeries()
{
    // This function creates a picture of the simple VETO
    // performance. It does not require inputs our ourputs,
    // since the inputs are the AnalysisTrees and the
    // output is on the screen.
    
    // Written by C. A. Douma.
    
    // ======================================================================
    // We begin with defining what kind of series we are talking about.
    // first define an integer that states this. Its value can then be
    // modified with sed by the shell-script running this program.
    Int_t Series = 0;
Series = 0;
    
    // Now we also require the amount of files that we should process.
    // This will again be given with sed:
    Int_t Nfiles = 0;
Nfiles = 1;

    // And we also require how many plots we actually should make. We do
    // count the first plot as plot nr. 1:
    Int_t Nplots = 1;
Nplots = 10;

    // ======================================================================
    // We also require some of the Input variables here:
    Double_t StepSize = 0.0;
    Double_t StepBegin = 0.0;
    Double_t ShowerStart = 0.0;
    Double_t ShowerStep = 0.0;
    Double_t ThickStep = 0.0;
    Double_t ThickBegin = 0.0;
    Double_t Estep = 0.0;
    Double_t Ebegin = 0.0;
    Double_t Xbegin = 0.0;
    Double_t Xstep = 0.0;
    Double_t Tcut = 0.0;
    Double_t TcutStep = 0.0;
    Int_t N_Paddles_start = 1;
    FileStat_t stom;
    Int_t inputs_test = gSystem->GetPathInfo("../../Data/Inputs/Inputs.root",stom);
   
    if (inputs_test==0)
    {
      // Connect to the Inputs.root file:
      TFile* inputs = new TFile("../../Data/Inputs/Inputs.root","read");
  
      // And then Get the histograms from this file:
      TH1D* Doubles = (TH1D*) inputs->Get("Doubles");
      TH1I* Integers = (TH1I*) inputs->Get("Integers");
    
      // obtain the scale factor:
      StepBegin = Doubles->GetBinContent(113) - Doubles->GetBinContent(123) - 0.5*Doubles->GetBinContent(129)*((Int_t) Integers->GetBinContent(3)) - 2.5*((Int_t) Integers->GetBinContent(2));
      StepSize = Doubles->GetBinContent(133);
      ShowerStart = Doubles->GetBinContent(323);
      ShowerStep = Doubles->GetBinContent(328);
      N_Paddles_start = Integers->GetBinContent(4);
      ThickBegin = Doubles->GetBinContent(129);
      ThickStep = Doubles->GetBinContent(134);
      Ebegin = Doubles->GetBinContent(324);
      Estep = Doubles->GetBinContent(329);
      Xbegin = Doubles->GetBinContent(337);
      Xstep = Doubles->GetBinContent(340);
      Tcut = Doubles->GetBinContent(342);
      TcutStep = Doubles->GetBinContent(343);
    }

    // ======================================================================
    // Now that we know this, define arrays that hold the information for the plots:
    Double_t* XArray = new Double_t[Nfiles];
    Double_t* Err_XArray = new Double_t[Nfiles];
    
    Double_t* Ethres = new Double_t[Nfiles];
    Double_t* EthresBack = new Double_t [Nfiles];
    Double_t* EthresPrim = new Double_t [Nfiles];
    Double_t* EthresBoth = new Double_t [Nfiles];
    Double_t* EthresTOF = new Double_t [Nfiles];
    Double_t* EthresTOF_Res = new Double_t [Nfiles];
    Double_t* EthresTOFPrim = new Double_t [Nfiles];
    Double_t* EthresTOFPrim_Res = new Double_t [Nfiles];
    Double_t* SecParts = new Double_t [Nfiles];
    Double_t* SecPartics = new Double_t [Nfiles];
    
    Double_t* Err_Ethres = new Double_t[Nfiles];
    Double_t* Err_EthresBack = new Double_t [Nfiles];
    Double_t* Err_EthresPrim = new Double_t [Nfiles];
    Double_t* Err_EthresBoth = new Double_t [Nfiles];
    Double_t* Err_EthresTOF = new Double_t [Nfiles];
    Double_t* Err_EthresTOF_Res = new Double_t [Nfiles];
    Double_t* Err_EthresTOFPrim = new Double_t [Nfiles];
    Double_t* Err_EthresTOFPrim_Res = new Double_t [Nfiles];
    Double_t* Err_SecParts = new Double_t [Nfiles];
    Double_t* Err_SecPartics = new Double_t [Nfiles];
    
    // =================================================================
    // Now we must fill the arrays:
    
    std::vector<Int_t> Results;
    for (Int_t k = 0; k<Nfiles; ++k)
    {
        // read out the AnalysisTree:
        cout << "#######################################################################################################\n\n"
             << "### NOTE: Read out Analysis Tree Nr. " << k+1 << "...\n\n";
        
        Results = GetSimplePerformance(k+1,Series);
        
        // Now fill the arrays:
        Ethres[k] = 100.0*((Int_t) Results.at(2))/((Int_t) Results.at(1));
        EthresBack[k] = 100.0*((Int_t) Results.at(3))/((Int_t) Results.at(1));
        EthresPrim[k] = 100.0*((Int_t) Results.at(4))/((Int_t) Results.at(1));
        EthresBoth[k] = 100.0*((Int_t) Results.at(5))/((Int_t) Results.at(1));
        EthresTOF[k] = 100.0*((Int_t) Results.at(6))/((Int_t) Results.at(1));
        EthresTOF_Res[k] = 100.0*((Int_t) Results.at(7))/((Int_t) Results.at(1));
        EthresTOFPrim[k] = 100.0*((Int_t) Results.at(8))/((Int_t) Results.at(1));
        EthresTOFPrim_Res[k] = 100.0*((Int_t) Results.at(9))/((Int_t) Results.at(1));
	SecParts[k] = 100.0*((Int_t) Results.at(10))/((Int_t) Results.at(1));
	SecPartics[k] = 100.0*((Int_t) Results.at(11))/((Int_t) Results.at(1));
        
        Err_Ethres[k] = 100.0*(TMath::Sqrt((Int_t) Results.at(2)))/((Int_t) Results.at(1));
        Err_EthresBack[k] = 100.0*(TMath::Sqrt((Int_t) Results.at(3)))/((Int_t) Results.at(1));
        Err_EthresPrim[k] = 100.0*(TMath::Sqrt((Int_t) Results.at(4)))/((Int_t) Results.at(1));
        Err_EthresBoth[k] = 100.0*(TMath::Sqrt((Int_t) Results.at(5)))/((Int_t) Results.at(1));
        Err_EthresTOF[k] = 100.0*(TMath::Sqrt((Int_t) Results.at(6)))/((Int_t) Results.at(1));
        Err_EthresTOF_Res[k] = 100.0*(TMath::Sqrt((Int_t) Results.at(7)))/((Int_t) Results.at(1));
        Err_EthresTOFPrim[k] = 100.0*(TMath::Sqrt((Int_t) Results.at(8)))/((Int_t) Results.at(1));
        Err_EthresTOFPrim_Res[k] = 100.0*(TMath::Sqrt((Int_t) Results.at(9)))/((Int_t) Results.at(1));
	Err_SecParts[k] = 100.0*(TMath::Sqrt((Int_t) Results.at(10)))/((Int_t) Results.at(1));
	Err_SecPartics[k] = 100.0*(TMath::Sqrt((Int_t) Results.at(11)))/((Int_t) Results.at(1));
        
        // Finally, we also have to fill the X-array:
        Err_XArray[k] = 0.0;
        XArray[k] = (Int_t) (k+1); // Default values
        if (Series==1) {XArray[k] = StepBegin + StepSize*((Int_t) k);}
        if (Series==2) {XArray[k] = ShowerStart + ShowerStep*((Int_t) k);}
        //if (Series==3) {XArray[k] = ((Int_t) N_Paddles_start)*TMath::Power(2.0,((Int_t) k));}
        if (Series==3) {(Int_t) (k+1);}
        if (Series==4) {XArray[k] = ThickBegin + ThickStep*((Int_t) k);}
        if (Series==5) {XArray[k] = Ebegin + Estep*((Int_t) k);}
        if (Series==6) {XArray[k] = Xbegin - Xstep*((Int_t) k);}
        if (Series==7) {XArray[k] = Tcut + TcutStep*((Int_t) k);}
        if (Series==8) {XArray[k] = ((Int_t) (k+1));}
    }
    
    // ===========================================================================
    // And now that we have done this, we can declare the TGraphErrors:
    
    TGraphErrors* Graph_Ethres = new TGraphErrors(Nfiles,&XArray[0],&Ethres[0],&Err_XArray[0],&Err_Ethres[0]);
    TGraphErrors* Graph_EthresBack = new TGraphErrors(Nfiles,&XArray[0],&EthresBack[0],&Err_XArray[0],&Err_EthresBack[0]);
    TGraphErrors* Graph_EthresPrim = new TGraphErrors(Nfiles,&XArray[0],&EthresPrim[0],&Err_XArray[0],&Err_EthresPrim[0]);
    TGraphErrors* Graph_EthresBoth = new TGraphErrors(Nfiles,&XArray[0],&EthresBoth[0],&Err_XArray[0],&Err_EthresBoth[0]);
    TGraphErrors* Graph_EthresTOF = new TGraphErrors(Nfiles,&XArray[0],&EthresTOF[0],&Err_XArray[0],&Err_EthresTOF[0]);
    TGraphErrors* Graph_EthresTOF_Res = new TGraphErrors(Nfiles,&XArray[0],&EthresTOF_Res[0],&Err_XArray[0],&Err_EthresTOF_Res[0]);
    TGraphErrors* Graph_EthresTOFPrim = new TGraphErrors(Nfiles,&XArray[0],&EthresTOFPrim[0],&Err_XArray[0],&Err_EthresTOFPrim[0]);
    TGraphErrors* Graph_EthresTOFPrim_Res = new TGraphErrors(Nfiles,&XArray[0],&EthresTOFPrim_Res[0],&Err_XArray[0],&Err_EthresTOFPrim_Res[0]);
    TGraphErrors* Graph_SecParts = new TGraphErrors(Nfiles,&XArray[0],&SecParts[0],&Err_XArray[0],&Err_SecParts[0]);
    TGraphErrors* Graph_SecPartics = new TGraphErrors(Nfiles,&XArray[0],&SecPartics[0],&Err_XArray[0],&Err_SecPartics[0]);
    
    // Then now we can do the painting:
    TString Xlabel = "Counter [Dim. less]";
    TString Ylabel = "% Events [Dim. less]";
    TString Title = "Veto Performance";
    TString st = "";
    TString Nbar = st.Itoa(N_Paddles_start,10);
    
    if (Series==1) {Xlabel = "VETO distance to NeuLAND [cm]";}
    if (Series==2) {Xlabel = "VETO condition Shower Radius [cm]";}
    if (Series==3) {Xlabel = "VETO Number of bars = 2^(x-"+ Nbar + ")";}
    if (Series==4) {Xlabel = "VETO Scintillator bar total thickness [cm]";}
    if (Series==5) {Xlabel = "VETO Energy Cut [MeV]";}
    if (Series==6) {Xlabel = "Proton X-starting position [cm]";}
    if (Series==7) {Xlabel = "Upper TOF cut on Vertices [ns]";}
    if (Series==8) {Xlabel = "VETO condition Number";}
    
    TCanvas* c = DrawGraphDouma(Graph_Ethres,Title,Xlabel,Ylabel,1); // black color
    
    if (Nplots>1) {
    Graph_EthresBack->UseCurrentStyle();
    Graph_EthresBack->SetLineWidth(2);
    Graph_EthresBack->SetLineColor(2); // Red
    Graph_EthresBack->Draw("same");}
    
    if (Nplots>2) {
    Graph_EthresPrim->UseCurrentStyle();
    Graph_EthresPrim->SetLineWidth(2);
    Graph_EthresPrim->SetLineColor(14); // Grey
    Graph_EthresPrim->Draw("same");}
    
    if (Nplots>3) {
    Graph_EthresBoth->UseCurrentStyle();
    Graph_EthresBoth->SetLineWidth(2);
    Graph_EthresBoth->SetLineColor(28); // Brown
    Graph_EthresBoth->Draw("same");}
    
    if (Nplots>4) {
    Graph_EthresTOF->UseCurrentStyle();
    Graph_EthresTOF->SetLineWidth(2);
    Graph_EthresTOF->SetLineColor(3); // Green
    Graph_EthresTOF->Draw("same");}
    
    if (Nplots>5) {
    Graph_EthresTOF_Res->UseCurrentStyle();
    Graph_EthresTOF_Res->SetLineWidth(2);
    Graph_EthresTOF_Res->SetLineColor(4); // Blue
    Graph_EthresTOF_Res->Draw("same");}
    
    if (Nplots>6) {
    Graph_EthresTOFPrim->UseCurrentStyle();
    Graph_EthresTOFPrim->SetLineWidth(2);
    Graph_EthresTOFPrim->SetLineColor(6); // Magenta
    Graph_EthresTOFPrim->Draw("same");}
    
    if (Nplots>7) {
    Graph_EthresTOFPrim_Res->UseCurrentStyle();
    Graph_EthresTOFPrim_Res->SetLineWidth(2);
    Graph_EthresTOFPrim_Res->SetLineColor(7); // Cyaan
    Graph_EthresTOFPrim_Res->Draw("same");}
    
    if (Nplots>8) {
    Graph_SecParts->UseCurrentStyle();
    Graph_SecParts->SetLineWidth(2);
    Graph_SecParts->SetLineColor(46); // transparent red
    Graph_SecParts->Draw("same");}
    
    if (Nplots>9) {
    Graph_SecPartics->UseCurrentStyle();
    Graph_SecPartics->SetLineWidth(2);
    Graph_SecPartics->SetLineColor(5); // Yellow
    Graph_SecPartics->Draw("same");}
    
    // =====================================================================
    // Then finally we should plot a legenda:
  
    Double_t Xmin = Graph_Ethres->GetXaxis()->GetXmin(); // X-axis
    Double_t Xmax = Graph_Ethres->GetXaxis()->GetXmax();
    Double_t Xlength = Xmax - Xmin;
    Double_t Ymin = 0.0;
    Double_t Ymax = Graph_Ethres->GetYaxis()->GetXmax();
    Double_t Ylength = Ymax - Ymin;

    TPaveText* text [10];
    
    for (Int_t k = 0; k<10; ++k)
    {
      text[k] = new TPaveText(Xmin+Xlength*0.72,Ymin+Ylength*(0.93-0.025*((Int_t) (k+1))),Xmin+Xlength*0.73,Ymin+Ylength*(0.93-0.025*((Int_t) (k+1))+0.01),"NB");
      text[k]->SetFillColor(0); // Color of the box.
      text[k]->SetTextSize(0.02); // Define text size
      text[k]->SetTextFont(1); // Define text font
      text[k]->SetTextAngle(0.0); // Defines the angle at which the text is displayed.
    }
  
    // Define the text:
    text[0]->AddText("Energy threshold condition");
    text[1]->AddText("Pure backscattering events (tracking) in Energy threshold condition");
    text[2]->AddText("Primary events (tracking) in Energy threshold condition, possibly with backscattering");
    text[3]->AddText("Events in Energy threshold condition that are both primary and backscattering (tracking)");
    text[4]->AddText("Energy threshold and TOF condition (delta spike)");
    text[5]->AddText("Energy threshold and TOF condition (Gaussian)");
    text[6]->AddText("Primary events (tracking) in Energy threshold and TOF condition (delta spike)");
    text[7]->AddText("Primary events (tracking) in Energy threshold and TOF condition (Gaussian)");
    text[8]->AddText("Events for which the VETO detected secondary particles");
    text[9]->AddText("Pure number of Secondary Particles detected by the VETO");
  
    // Set the colors:
    text[0]->SetTextColor(1);
    text[1]->SetTextColor(2);
    text[2]->SetTextColor(14);
    text[3]->SetTextColor(28);
    text[4]->SetTextColor(3);
    text[5]->SetTextColor(4);
    text[6]->SetTextColor(6);
    text[7]->SetTextColor(7);
    text[8]->SetTextColor(46);
    text[9]->SetTextColor(5);
  
    // Draw the legenda (plot only untill Nplots):
    for (Int_t k = 0; k<Nplots; ++k) {text[k]->Draw("same");}
    
    // =====================================================================
    // Now print a picture to the pictures folder:
    c->SaveAs("../../UI/Pictures/"+Title+".png");
    
    // And then we are done!
  
}
  
  
// Now define the main for stand-alone compilation:
#ifndef __CINT__
#include <TApplication.h>
int main(int argc, char** argv)
{
  TApplication* TheApp = new TApplication("SimplePerformanceSeries",&argc,argv);
  SimplePerformanceSeries();
  TheApp->Run();
  return 0;
}
#endif
    
             
    