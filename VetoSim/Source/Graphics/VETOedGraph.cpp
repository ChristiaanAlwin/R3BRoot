// include the required C++ headers:
#include <iostream>

// include the required root headers:
#include "TSystem.h"
#include "TH1D.h"
#include "TH1I.h"
#include "TFile.h"
#include "TGraphErrors.h"

// include own headers:
#include "VETOed1Run.cpp"
#include "Emean1Run.cpp"
#include "DrawGraphDouma.h"

// use the standard C++ variables:
using namespace std;

void VETOedGraph()
{
    // This function paints the results of the VETO condition on the screen in
    // the form of a TGraphErrors.
    
    // Written by C. A. Douma
    
    // ==================================================================================
    // First we obtain the number of runs and the Series from sed:
    Int_t Series = 0;
Series = 0;

    Int_t Nruns = 1;
Nruns = 1;

    // We also want to know which graphs we should paint:
    Int_t Select = 1;
Select = 1;

    // ==================================================================================
    // Now we will define arrays for the TGraphErrors:
    
    Double_t** Arrays = new Double_t*[15];
    Double_t** Errors = new Double_t*[15];
    for (Int_t k = 0; k<15; ++k)
    {
        Arrays[k] = new Double_t[Nruns];
        Errors[k] = new Double_t[Nruns];
    }
    
    // And now we can fill those arrays:
    std::vector<Double_t> Run;
    Run.clear();
    for (Int_t n = 0; n<Nruns; ++n)
    {
        cout << "#############################################################################################\n\n"
	     << "We will now process run nr. " << n+1 << "...\n\n";
      
        if (Select==4) {Run = Emean1Run(n+1,Series);}
        else {Run = VETOed1Run(n+1,Series,Select,-2);} // set last one to -2 so nothing happens!
        
        for (Int_t k = 0; k<15; ++k)
        {
            Arrays[k][n] = Run.at(2*k);
            Errors[k][n] = Run.at(2*k+1);
        }
    }
    
    // Now define the array for the x-axis:
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
    
      // obtain the steps:
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
    
    Double_t* XArray = new Double_t[Nruns];
    Double_t* Err_XArray = new Double_t[Nruns];
    
    for (Int_t k = 0; k<Nruns; ++k)
    {
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
    
    // =============================================================================
    // Now we have all of our arrays. Now create our TGraphErrors.
    
    // We first search for the maximum:
    Double_t max = 0.0;
    for (Int_t k = 0; k<15; ++k)
    {
        for (Int_t n = 0; n<Nruns; ++n)
        {
            if (max<Arrays[k][n]) {max = Arrays[k][n];}
        }
    }
    
    // Then create our TGraphErrors:
    TGraphErrors** TheGraphs = new TGraphErrors*[16];
    for (Int_t k = 0; k<15; ++k)
    {
        TheGraphs[k] = new TGraphErrors(Nruns,XArray,Arrays[k],Err_XArray,Errors[k]);
        TheGraphs[k]->SetMaximum(max);
        TheGraphs[k]->SetMinimum(0.0);
    }
    
    // compute a difference:
    Double_t* Diff = new Double_t[Nruns];
    Double_t* Err_Diff = new Double_t[Nruns];
    for (Int_t k = 0; k<Nruns; ++k)
    {
      Diff[k] = TMath::Abs(Arrays[1][k] - Arrays[0][k]); // neutrons - protons:
      Err_Diff[k] = TMath::Sqrt(Errors[0][k]*Errors[0][k] + Errors[1][k]*Errors[1][k]);
    }
    TheGraphs[15] = new TGraphErrors(Nruns,XArray,Diff,Err_XArray,Err_Diff);
    TheGraphs[15]->SetMaximum(max);
    TheGraphs[15]->SetMinimum(0.0);
    
    
    // Then we can paint all TGraphErrors:
    TString Title = "";
    TString st = "";
    TString Nbar = st.Itoa(N_Paddles_start,10);
    if (Select == 1) {Title = "VETOed Particle Percentage";}
    if (Select == 2) {Title = "Kept Particle Percentage";}
    if (Select == 3) {Title = "Detected Particle Percentage";}
    if (Select == 4) {Title = "Mean of Reconstructed Energy";}
    
    TString Xlabel = "Counter [Dim. Less]";
    if (Series==1) {Xlabel = "VETO distance to NeuLAND [cm]";}
    if (Series==2) {Xlabel = "VETO condition Shower Radius [cm]";}
    if (Series==3) {Xlabel = "VETO Number of bars = 2^(x-" + Nbar + ")";}
    if (Series==4) {Xlabel = "VETO Scintillator bar total thickness [cm]";}
    if (Series==5) {Xlabel = "VETO Energy Cut [MeV]";}
    if (Series==6) {Xlabel = "Proton X-starting position [cm]";}
    if (Series==7) {Xlabel = "Upper TOF cut on Vertices [ns]";}
    if (Series==8) {Xlabel = "VETO condition Number";}
    
    TString YLabel = "";
    if (Select==4) {YLabel = "Reconstructed Energy [MeV]";}
    else {YLabel = "Number of Particles [%]";}
    
    TCanvas* c = DrawGraphDouma(TheGraphs[0],Title,Xlabel,YLabel,2); // red color.
    
    Int_t Color = 0;
    for (Int_t k = 1; k<16; ++k)
    {
        if (k==1) {Color = 8;}
        if (k==2) {Color = 7;}
        if (k==3) {Color = 3;}
        if (k==4) {Color = 9;}
        if (k==5) {Color = 6;}
        if (k==6) {Color = 4;}
        if (k==7) {Color = 14;}
        if (k==8) {Color = 28;}
        if (k==9) {Color = 5;}
        if (k==10) {Color = 12;}
        if (k==11) {Color = 32;} // n from target.
        
        if (k==12) {Color = 1;} // true good events
        if (k==13) {Color = 29;} // apearent good events
        if (k==14) {Color = 38;} // Acceptable events
        if (k==15) {Color = 43;} // p - n diff.
        
        TheGraphs[k]->UseCurrentStyle();
        TheGraphs[k]->SetLineColor(Color);
        TheGraphs[k]->SetLineWidth(2.0);
        TheGraphs[k]->Draw("same");
    }
    //TheGraphs[1]->Draw("same");
    //TheGraphs[11]->Draw("same");
    //TheGraphs[12]->Draw("same");
    
    // =============================================================================
    // Then finally, we have to build a legenda:
    
    TString Legenda[16];
    Legenda[0] = "Protons";
    Legenda[1] = "Neutrons";
    Legenda[2] = "Electrons";
    Legenda[3] = "Gammas";
    Legenda[4] = "Pions";
    Legenda[5] = "Muons";
    Legenda[6] = "Alphas";
    Legenda[7] = "C-ions";
    Legenda[8] = "Fragments";
    Legenda[9] = "Positrons";
    Legenda[10] = "All Particles";
    Legenda[11] = "Neutrons from Target";
    Legenda[12] = "Truely Good Events";
    Legenda[13] = "Appearent Good Event";
    Legenda[14] = "Acceptably Good Events";
    Legenda[15] = "Protons-Neutrons";
    
    TPaveText** text = new TPaveText*[16];
    Double_t Xmin = XArray[0];
    Double_t Xmax = XArray[Nruns-1];
    Double_t Xlength = Xmax - Xmin;
    Double_t Ymin = 0.0;
    Double_t Ymax = max;
    Double_t Ylength = Ymax - Ymin;
    
    for (Int_t k = 0; k<16; ++k)
    {   
      text[k] = new TPaveText(Xmin+Xlength*1.13,Ymin+Ylength*(1.05 - 0.05*((Int_t) k)),Xmin+Xlength*1.14,Ymin+Ylength*(1.06 - 0.05*((Int_t) k)),"NB");
      text[k]->SetFillColor(0);
      text[k]->AddText(Legenda[k]); 
      text[k]->SetTextSize(0.03); 
      text[k]->SetTextFont(1); 
      text[k]->SetTextAngle(0.0);
    
      if (k==0) {text[k]->SetTextColor(2);}
      if (k==1) {text[k]->SetTextColor(8);}
      if (k==2) {text[k]->SetTextColor(7);}
      if (k==3) {text[k]->SetTextColor(3);}
      if (k==4) {text[k]->SetTextColor(9);}
      if (k==5) {text[k]->SetTextColor(6);}
      if (k==6) {text[k]->SetTextColor(4);}
      if (k==7) {text[k]->SetTextColor(14);}
      if (k==8) {text[k]->SetTextColor(28);}
      if (k==9) {text[k]->SetTextColor(5);}
      if (k==10) {text[k]->SetTextColor(12);}
      if (k==11) {text[k]->SetTextColor(32);}
      if (k==12) {text[k]->SetTextColor(1);}
      if (k==13) {text[k]->SetTextColor(29);}
      if (k==14) {text[k]->SetTextColor(38);}
      if (k==15) {text[k]->SetTextColor(43);}
      
      text[k]->Draw("same");
    }
    
    // Done! Print it:
    c->SaveAs("../../UI/Pictures/" + Title + ".png");
}

// Now define the main for stand-alone compilation:
#ifndef __CINT__
#include <TApplication.h>
int main(int argc, char** argv)
{
  TApplication* TheApp = new TApplication("VETOedGraph",&argc,argv);
  VETOedGraph();
  TheApp->Run();
  return 0;
}
#endif
      