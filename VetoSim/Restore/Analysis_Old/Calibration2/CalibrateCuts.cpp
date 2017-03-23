// Inclusion of C++ headers:
#include <iostream>
#include <fstream>

// inclusion of Math headers:
#include "Math/Minimizer.h"
#include "Math/Factory.h"
#include "Math/Functor.h"

// Inclusion of ROOT headers
#include "TFile.h"
#include "TH2F.h"
#include "TLine.h"
#include "TCanvas.h"
//#include "TPostScript.h"

// Inclusion of own headers:
#include "DrawHistDouma_2D_flatArray.h"

// Inluding the standard C++ variables:
using namespace std;

// Declaration of global variables. 
static const int N_MAX = 5;
TH2F* TheHists [N_MAX];

// NOTE: These variables HAVE to be defined as global because they are inputs to the function that is minimized
//       with TMinuit from ROOT. Just like fitting functions to histograms, this package requires a very special 
//       structure in input variables. Hence inputs that should stay const, etc should be put in as global
//       variables, not as inputs. This is just like the peak boundaries for random subtraction. If you want to
//       modify these inputs, then do it with sed.

// Inclusion of the headers for this algorithm:
#include "Integral_2D.h"
#include "Efficiency.h"
#include "GetCuts.h"
#include "WastedEfficiency.h"
#include "NumericalMinimization.h"
#include "DrawResults.h"
#include "WriteCalibrationFile.h"

void CalibrateCuts()
{
  // This is the main function to obtain the calibration cuts. Therefore it has no inputs or outputs.
  // One can just call it and it will do everything by itself.
    
  // The task that it does is that it takes the 1n, 2n, ... N_MAX n histograms and imposes cuts on these
  // histograms to decide how many neutrons there are. The cuts are defined as y = kappa*(Ecut-x)
  // Where kappa is the slope of the line and Ecut is the energy cut value of that specific cut.
  // Hence all cuts have the same slope. The Energy cuts are defined as Ecut(k) = Step*k
  // Where Step is a certain Energy difference. k = 0, ... , N_MAX (we need 1 more cut then histograms).
  // This means that the cuts are equidistant. Hence Only kappa and Step should be optimized. This is done
  // in this function.
    
  // Designed by Jan Mayer and optimized by C. A. Douma.

  // ================================================================================================================
  // We begin by connecting to our Inputs.root-file:
  TFile* inputs = new TFile("../../Data/Inputs/Inputs.root","update");
  TH1D* Doubles = (TH1D*) inputs->Get("Doubles");
  TH1I* Integers = (TH1I*) inputs->Get("Integers");
  
  // Then we obtain the information that we need:
  Double_t BeamE = 500.0*(Doubles->GetBinContent(202) + Doubles->GetBinContent(203)); // NOTE: MeV!
  Double_t Erel = ((1000.0*Doubles->GetBinContent(203)) - BeamE)*1000.0; // NOTE: keV!
  Int_t beam_energy = (Int_t) BeamE;
  Int_t relative_energy = (Int_t) Erel;

  // NOTE: The Integers bin nr. 244 should be put in here by sed, not from this file. Otherwise
  // this method will not work. Hence we do not collect it here.
  
  // Now we have to connect to the output files of our calibration simulations. They
  // also hold the histograms that we need.
  
  // Define an array of TFiles:
  TFile** file = new TFile*[N_MAX];
  
  // Then use sprintf to correctly define the filenames. With that we can fill the TFile-array
  // and then obtain our histograms, which we will load into our global variables.
  char str_buf[1000];
  static const char BASE_DIR[] = "../../Data/Digitizer";
  Int_t distance = 14;
  TString str = "";
  for (Int_t k = 0; k<N_MAX; ++k) 
  {
    sprintf(str_buf,"%s/%1dAMeV.%1dn.%1dkeV.%1dm.root",BASE_DIR,beam_energy,k+1,relative_energy,distance);
    str = (TString) str_buf;
    file[k] = new TFile(str,"read");
    TheHists[k] = (TH2F*) file[k]->Get("h_ncl_etot");
  }

  // Then now declare our array to hold the cuts and declare the slope kappa:
  Double_t kappa = 0.0;
  Double_t* TheCuts = new Double_t[N_MAX+1];
  
  // Then execute the minimization. But only if there are sufficient events!
  if (TheHists[0]->GetEntries()>1000) 
  {
      NumericalMinimization(TheCuts,kappa);
  }
  else 
  {
      kappa = 0.07; 
      for (Int_t k = 0; k<N_MAX; ++k)
      {
          TheCuts[k] = ((Int_t) k)*0.6*BeamE;
      }
  } // Less then 1000 events, we take this default!

  // Then draw our results:
  DrawResults(TheCuts,kappa);

  // Then, finally, write the calibration file so that the tracker can use it.
  sprintf(str_buf, "../../UI/User_Energy_Cuts.txt");
  TString out_file = (TString) str_buf;
  WriteCalibrationFile(TheCuts,kappa,out_file);

  // Done!
}

// Now define the main for stand-alone compilation:
#ifndef __CINT__
#include <TApplication.h>
int main(int argc, char** argv)
{
  TApplication* TheApp = new TApplication("CalibrateCuts",&argc,argv);
  CalibrateCuts();
  return 0;
}
#endif
