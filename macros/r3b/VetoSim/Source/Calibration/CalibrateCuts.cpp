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
static const Int_t N_MAX = 4;
static const Int_t N_PROT = 2;
static const Int_t N_TOT = 2 + 3;

TH2F* TheHists [N_MAX];

// NOTE: These variables HAVE to be defined as global because they are inputs to the function that is minimized
//       with TMinuit from ROOT. Just like fitting functions to histograms, this package requires a very special 
//       structure in input variables. Hence inputs that should stay const, etc should be put in as global
//       variables, not as inputs. This is just like the peak boundaries for random subtraction. If you want to
//       modify these inputs, then do it with sed.

// Inclusion of the headers for this algorithm:
#include "Integral_2D.h"
#include "Efficiency.h"
#include "GetCuts_EquiDist.h"
#include "WastedEfficiency_EquiDist.h"
#include "NumericalMinimization_EquiDist.h"
#include "GetCuts_VarDist.h"
#include "WastedEfficiency_VarDist.h"
#include "WastedEfficiency_VarDist2.h"
#include "NumericalMinimization_VarDist.h"
#include "WastedEfficiency_MaxEff.h"
#include "NumericalMinimization_VarDist2.h"
#include "NumericalMinimization_MaxEff.h"
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
  TFile* inputs = new TFile("../../Data/Inputs/Inputs.root","read");
  TH1D* Doubles = (TH1D*) inputs->Get("Doubles");
  TH1I* Integers = (TH1I*) inputs->Get("Integers");
  
  // Then we obtain the information that we need:
  Int_t Cut_Type = Integers->GetBinContent(245); // Defines the type of minimization we will perform.
  Int_t nEvents_Cal = Integers->GetBinContent(240);
  Double_t BeamE = 500.0*(Doubles->GetBinContent(202) + Doubles->GetBinContent(203)); // NOTE: MeV!
  Int_t BeamE_int = (Int_t) BeamE;

  // NOTE: The Integers bin nr. 244 should be put in here by sed, not from this file. Otherwise
  // this method will not work. Hence we do not collect it here.
  
  // Check for ion calibrations:
  Int_t Particle_ID = Integers->GetBinContent(251);
  Int_t Abeam = -1;
  Int_t Zbeam = -1;
  if (TMath::Abs(Particle_ID)>1000000000)
  {
    Int_t PDGsmall = 0;
    if (Particle_ID>0) {PDGsmall = Particle_ID - 1000000000;}
    if (Particle_ID<0) {PDGsmall = Particle_ID + 1000000000;}
    Double_t PDG_d = (Int_t) TMath::Abs(PDGsmall);

    Double_t Z_d = PDG_d/10.0;
    Double_t R_d = PDG_d/10000.0;
    
    Double_t z_Z_d = PDG_d/10000.0;
    Double_t z_R_d = PDG_d/10000000.0;
    
    Int_t Z = (Double_t) Z_d;
    Int_t R = (Double_t) R_d;
    Z_d = Z_d - 1000.0*((Int_t) R);
    Z = (Double_t) Z_d;
    Z_d = (Int_t) Z;
    
    Int_t z_Z = (Double_t) z_Z_d;
    Int_t z_R = (Double_t) z_R_d;
    z_Z_d = z_Z_d - 1000.0*((Int_t) z_R);
    z_Z = (Double_t) z_Z_d;
    z_Z_d = (Int_t) z_Z;
    
    Abeam = Z_d;
    Zbeam = z_Z_d;
  }
  
  // Now we have to connect to the output files of our calibration simulations. They
  // also hold the histograms that we need.
  
  // Now we will load the histograms. We need filenames for that, hence we 
  // will now construct them.
  TFile** file = new TFile*[N_MAX];
  TString FileName = "";
  TString st = "";
  TString nstr = "";
  TString pstr = "";
  
  if (N_PROT>0)
  {
    for (Int_t kp = 0; kp<N_PROT; ++kp)
    {
      nstr = st.Itoa(0,10);
      pstr = st.Itoa(kp+1,10); // start at 1 proton!
      FileName = "../../Data/Digitizer/CalibrationFile_" + pstr + "p_" + nstr + "n.root";
      file[kp] = new TFile(FileName,"read");
      TheHists[kp] = (TH2F*) file[kp]->Get("h_ncl_etot");
      TheHists[kp]->SetMinimum(0.0);
      TheHists[kp]->SetMaximum(20.0*((Int_t) nEvents_Cal)/50000.0);
    }
    
    for (Int_t kn = 0; kn<(N_MAX-N_PROT); ++kn)
    {
      nstr = st.Itoa(kn+1,10);
      pstr = st.Itoa(N_PROT,10); // start at 0 neutron!
      FileName = "../../Data/Digitizer/CalibrationFile_" + pstr + "p_" + nstr + "n.root";
      file[kn+N_PROT] = new TFile(FileName,"read");
      TheHists[kn+N_PROT] = (TH2F*) file[kn+N_PROT]->Get("h_ncl_etot");
      TheHists[kn+N_PROT]->SetMinimum(0.0);
      TheHists[kn+N_PROT]->SetMaximum(20.0*((Int_t) nEvents_Cal)/50000.0);
    }
  }
  else
  {
    for (Int_t kn = 0; kn<N_MAX; ++kn)
    {
      nstr = st.Itoa(kn+1,10); // start at 1 neutron!
      pstr = st.Itoa(0,10);
      FileName = "../../Data/Digitizer/CalibrationFile_" + pstr + "p_" + nstr + "n.root";
      file[kn] = new TFile(FileName,"read");
      TheHists[kn] = (TH2F*) file[kn]->Get("h_ncl_etot");
      TheHists[kn]->SetMinimum(0.0);
      TheHists[kn]->SetMaximum(20.0*((Int_t) nEvents_Cal)/50000.0);
    }
  }
      
  // NOTE: After this we do not need to worry anymore about the difference
  // between neutrons and protons (except mayby in the titles of the plots)
  // we simply make N_MAX plots and optimize the cuts. N_MAX has already
  // been set OK for us.

  // Then now declare our array to hold the cuts and declare the slope kappa:
  Double_t kappa = 0.0;
  Double_t* TheCuts = new Double_t[N_MAX+1];
  
  // Then execute the minimization. But only if there are sufficient events!
  cout << "\n\n### PROGRESS REPORT: Your cuts are now being optimized. This may take\n### PROGRESS REPORT: some time, so please be a little patient!\n\n\n";
  
  if (TheHists[0]->GetEntries()>900)
  {
      if (Cut_Type == 1) {NumericalMinimization_EquiDist(TheCuts,kappa,Abeam,Zbeam);}
      else if (Cut_Type == 2) {NumericalMinimization_VarDist(TheCuts,kappa,Abeam,Zbeam);}      
      else if (Cut_Type == 3) {NumericalMinimization_MaxEff(TheCuts,kappa,Abeam,Zbeam);}  
      else if (Cut_Type == 4) {NumericalMinimization_VarDist2(TheCuts,kappa,Abeam,Zbeam);}  
      else {kappa = 0.03; Double_t Step = 0.8*BeamE; GetCuts_EquiDist(Step,TheCuts);}
  }
  else {kappa = 0.07; Double_t Step = 0.7*BeamE; GetCuts_EquiDist(Step,TheCuts);} // Our default when there are too little events or when the Cut_Type is wrong.

  // Then draw our results:
  DrawResults(TheCuts,kappa);

  // Then, finally, write the calibration file so that the tracker can use it.
  pstr = st.Itoa(N_PROT,10);
  TString out_file = "../../UI/Calibration/Energy_Cuts_" + pstr + "p.txt";
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
