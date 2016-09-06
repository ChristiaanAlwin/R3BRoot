// Include C++ headers:
#include <iostream>

// Include required ROOT headers:
#include "TGraphErrors.h"

// Include our own required headers:
#include "DrawGraphDoumaArray.h"
#include "GetSecondaryParticles.cpp"

// Use standard C++ variables:
using namespace std;

void GetTableGraphics()
{
  // This function creates the TGraphErrors for the table of secondary particles.
  // The readout of the tree is done by a seperate function and the painting is
  // also done by a seperate function. All we have to do here is to provide
  // an interface between these two functions.
  
  // This function is ment to run stand-alone, so it has no inputs or outputs.
  
  // Written by C. A. Douma, for R3B collaboration.
  
  // ===========================================================================
  // Before we do anything, we need to obtain the Series argument from the sed:
  Int_t Series = 0;
Series = 0;

  // We also need from sed the amount of files that we will analyze:
  Int_t Nfiles = 1;
Nfiles = 1;

  // ===========================================================================
  // Now the first thing that we must do is to obtain our table for all of these
  // files:

  Int_t*** Overall_Table = new Int_t**[Nfiles];
  for (Int_t k = 0; k<Nfiles; ++k)
  {
    cout << "#############################################################################\n\n"
         << "### NOTE: We now read out Analysis Tree nr. " << k+1 << "\n\n";
	 
    Overall_Table[k] = GetSecondaryParticles(k+1,Series);
  }
  
  // So notice that that the first index is VETO position or files, the second
  // index is particle species and the third index is stituation: produces, aimed,
  // reached, hit, detected. Now we must build our TGraphErrors from this.
  
  // ===========================================================================
  // For this purpose we will need a few values from the inputs-file:
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
  Int_t nEvents = 1;
  FileStat_t stom;
  Int_t inputs_test = gSystem->GetPathInfo("../../Data/Inputs/Inputs.root",stom);
   
  if (inputs_test==0)
  {
    // Connect to the Inputs.root file:
    TFile* inputs = new TFile("../../Data/Inputs/Inputs.root","read");
  
    // And then Get the histograms from this file:
    TH1D* Doubles = (TH1D*) inputs->Get("Doubles");
    TH1I* Integers = (TH1I*) inputs->Get("Integers");
    
    // obtain our values:
    StepBegin = Doubles->GetBinContent(113) - Doubles->GetBinContent(123) - 0.5*Doubles->GetBinContent(129)*((Int_t) Integers->GetBinContent(3)) - 2.5*((Int_t) Integers->GetBinContent(2));
    StepSize = Doubles->GetBinContent(133);
    ShowerStart = Doubles->GetBinContent(323);
    ShowerStep = Doubles->GetBinContent(328);
    N_Paddles_start = Integers->GetBinContent(4);
    ThickBegin = Doubles->GetBinContent(129);
    ThickStep = Doubles->GetBinContent(134);
    nEvents = Integers->GetBinContent(201);
    Ebegin = Doubles->GetBinContent(324);
    Estep = Doubles->GetBinContent(329);
    Xbegin = Doubles->GetBinContent(337);
    Xstep = Doubles->GetBinContent(340);
    Tcut = Doubles->GetBinContent(342);
    TcutStep = Doubles->GetBinContent(343);
  }
  
  // ============================================================================
  // Now let's build the TGraphErrors. For this purpose we need several arrays
  // of doubles. We will declare all y-values as a big 3D-array.  
  // Notice that the length of a single y-array should be the number of
  // files: the veto position. Hence in a big 3D array this should be the third index.
  // The second index should be particle species and the first index should then be
  // the situation. So build it:
  
  Double_t*** TheArrays = new Double_t**[5];
  Double_t*** ErrArrays = new Double_t**[5];
  for (Int_t k1 = 0; k1<5; ++k1)
  {
    TheArrays[k1] = new Double_t*[10];
    ErrArrays[k1] = new Double_t*[10];
    for (Int_t k2 = 0; k2<10; ++k2)
    {
      TheArrays[k1][k2] = new Double_t[Nfiles];
      ErrArrays[k1][k2] = new Double_t[Nfiles];
      for (Int_t k3 = 0; k3<Nfiles; ++k3)
      {
	// Now we will load our table and normalize against the total number of events:
	TheArrays[k1][k2][k3] = 100.0*((Int_t) Overall_Table[k3][k2][k1])/((Int_t) nEvents);
	ErrArrays[k1][k2][k3] = 100.0*(TMath::Sqrt((Int_t) Overall_Table[k3][k2][k1]))/((Int_t) nEvents);
      }
    }
  }
  
  // So now we have our y-values. Now we will get our X-values:
  Double_t* Xarray = new Double_t[Nfiles];
  Double_t* Err_Xarrray = new Double_t[Nfiles];
  for (Int_t k = 0; k<Nfiles; ++k)
  {
    // Default values:
    Err_Xarrray[k] = 0.0;
    Xarray[k] = k+1;
    
    // Depending on the series:
    if (Series==1) {Xarray[k] = StepBegin + StepSize*((Int_t) k);}
    if (Series==2) {Xarray[k] = ShowerStart + ShowerStep*((Int_t) k);}
    //if (Series==3) {Xarray[k] = ((Int_t) N_Paddles_start)*TMath::Power(2.0,((Int_t) k));}
    if (Series==3) {(Int_t) (k+1);}
    if (Series==4) {Xarray[k] = ThickBegin + ThickStep*((Int_t) k);}
    if (Series==5) {Xarray[k] = Ebegin + Estep*((Int_t) k);}
    if (Series==6) {Xarray[k] = Xbegin - Xstep*((Int_t) k);}
    if (Series==7) {Xarray[k] = Tcut + TcutStep*((Int_t) k);}
    if (Series==8) {Xarray[k] = ((Int_t) (k+1));}
  }
  
  // ============================================================================
  // Now build our TGraphErrors:
  TGraphErrors*** TheGraphs = new TGraphErrors**[5];
  for (Int_t ksit = 0; ksit<5; ++ksit) // ksit of k_situation
  {
    TheGraphs[ksit] = new TGraphErrors*[10]; // kpart of k_particles
    for (Int_t kpart = 0; kpart<10; ++kpart)
    {
      TheGraphs[ksit][kpart] = new TGraphErrors(Nfiles,Xarray,TheArrays[ksit][kpart],Err_Xarrray,ErrArrays[ksit][kpart]);
    }
  }
  
  // ============================================================================
  // Now that we have our TGraphErrors, we can paint it. 
  
  // Define titles and labels:
  TString Title [5];
  Title[0] = "Produced Secondary Particles";
  Title[1] = "Secondary Particles Aimed at VETO";
  Title[2] = "Secondary Particles that Reached the VETO";
  Title[3] = "Secondary Particles that Hit the VETO";
  Title[4] = "Secondary Particles Detected by the VETO";
  
  TString xlabel = "File number";
  TString st = "";
  TString Nbar = st.Itoa(N_Paddles_start,10);
  if (Series==1) {xlabel = "VETO distance to NeuLAND [cm]";}
  if (Series==2) {xlabel = "VETO condition Shower Radius [cm]";}
  if (Series==3) {xlabel = "VETO Number of bars = 2^(x-" + Nbar + ")";}
  if (Series==4) {xlabel = "VETO Scintillator bar total thickness [cm]";}
  if (Series==5) {xlabel = "VETO Energy Cut [MeV]";}
  if (Series==6) {xlabel = "Proton X-starting position [cm]";}
  if (Series==7) {xlabel = "Upper TOF cut on Vertices [ns]";}
  if (Series==8) {xlabel = "VETO condition Number";}
  
  TString ylabel = "[%] Events";
  
  // Define the colors of our graphs:
  Int_t Colors [10];
  Colors[0] = 2; // protons: red.
  Colors[1] = 8; // neutrons: dark green.
  Colors[2] = 7; // electrons: light blue.
  Colors[3] = 3; // gammas: bright green.
  Colors[4] = 9; // pions: purple.
  Colors[5] = 6; // muons: magenta.
  Colors[6] = 4; // Alphas: dark blue.
  Colors[7] = 13; // Cions: grey.
  Colors[8] = 28; // other fragments: brown.
  Colors[9] = 1; // the sum: black.
  
  // Define our legenda:
  TString Legenda [10];
  Legenda[0] = "Protons";
  Legenda[1] = "Neutrons";
  Legenda[2] = "Electrons";
  Legenda[3] = "Gammas";
  Legenda[4] = "Pions";
  Legenda[5] = "Muons";
  Legenda[6] = "Alphas";
  Legenda[7] = "C-ions";
  Legenda[8] = "Other fragments";
  Legenda[9] = "Total";
  
  // Number of graphs:
  Int_t Npads = 5; // We want 5 figures in parallel:
  Int_t Ngraphs = 10; // We want 10 graphs per figure.
  Int_t Nhor = 3; // 3 figures in horizontal direction.
  Int_t Nvert = 2; // 2 figures in vertical direction.
  
  // Now fire the plotter:
  DrawGraphDoumaArray(Npads,Ngraphs,Nhor,Nvert,TheGraphs,Title,xlabel,ylabel,Colors,Legenda,kTRUE,kFALSE);
  
  // ============================================================================
  
  // Now that should do it. We are done!
  
}

// Now define the main for stand-alone compilation:
#ifndef __CINT__
#include <TApplication.h>
int main(int argc, char** argv)
{
  TApplication* TheApp = new TApplication("GetTableGraphics",&argc,argv);
  GetTableGraphics();
  TheApp->Run();
  return 0;
}
#endif