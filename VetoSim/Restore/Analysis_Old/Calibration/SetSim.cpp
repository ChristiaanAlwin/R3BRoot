// Include C++ headers:
#include <iostream>
#include <unistd.h> // NOTE: Linux-specific! Allows us to use chdir-command:
#include <fstream>

// Include ROOT headers:
#include "TFile.h"
#include "TH1D.h"
#include "TH1I.h"
#include "TString.h"
#include "TObjString.h"
#include "TVector3.h"
#include "TMath.h"

// Include Own headers:

// Use standard Namespace:
using namespace std;

void SetSim()
{
  // This function will correctly set the parameters for a neutron calibration simulation.
  // NOTE: This is not a simulation that should be analyzed and plotted, it is a simulation that 
  // is used by R3BRoot to generate the 2D cuts on the number of neutrons. These 2D cuts are the input
  // that is required by the R3BRoot Neutron Tracker that reconstructs the events.
  
  // This function will do nothing more then set the input parameters correctly for this. We return a
  // boolian that states if the primary beam is aimed at NeuLAND yes/no.
  
  // Written by C. A. Douma.
  // =================================================================================================
  // Before we do anything: get the number of neutrons from sed:
  Int_t N_neutrons = 0;
N_neutrons = 5;
  
  // =================================================================================================
  // We begin by connecting to the Inputs.root-file. Notice that in this way we do not destroy any
  // updates made to the Inputs.root-file during simulation tests etc.
  TFile* inputs = new TFile("../../Data/Inputs/Inputs.root","update");
   
  // And then Get the histograms from this file:
  TH1D* Doubles = (TH1D*) inputs->Get("Doubles");
  TH1I* Integers = (TH1I*) inputs->Get("Integers");
  
  // Now notice that we want a simulation for the geometry that the user defined. Hence we will not change
  // the geometry parameters. We only should make sure that we fire indeed a neutron beam. 
  
  // So adapt the primary beam:
  Integers->SetBinContent(218,2112);
  Integers->SetBinContent(219,N_neutrons);
  
  // switch off the other beams:
  Integers->SetBinContent(221,0);
  Integers->SetBinContent(225,0);
  
  // Set the correct number of events:
  Int_t NEvents = Integers->GetBinContent(240);
  Integers->SetBinContent(201,NEvents);
  
  // Use the right detectors:
  Integers->SetBinContent(216,1); 		// We do use NeuLAND!
  Integers->SetBinContent(230,0);               // We do NOT use NEBULA!
  Integers->SetBinContent(231,0);               // We do NOT use NEBULA VETO!
  
  // set the other settings:
  Integers->SetBinContent(203,1); // visualization
  Integers->SetBinContent(204,1); // fUserPList
  Integers->SetBinContent(205,1); // Magnetic field on
  Integers->SetBinContent(220,1); // 1 run, not more
  Integers->SetBinContent(241,1); // This is a Neutron Calibration run!
  
  // Define the correct filename:
  TString FileName = "";
  TString st = "";
  Double_t BeamE = 500.0*(Doubles->GetBinContent(202) + Doubles->GetBinContent(203)); // NOTE: MeV!
  Double_t Erel = ((1000.0*Doubles->GetBinContent(203)) - BeamE)*1000.0; // NOTE: keV!
  Int_t BeamE_int = (Int_t) BeamE;
  Int_t Erel_int = (Int_t) Erel;
  TString BeamE_str = st.Itoa(BeamE_int,10);
  TString Erel_str = st.Itoa(Erel_int,10);
  TString N_neutrons_str = st.Itoa(N_neutrons,10);
  TString d_str = st.Itoa(14,10); // always pretend that d = 14 for the Dima macros!
  FileName = BeamE_str + "AMeV." + N_neutrons_str + "n." + Erel_str + "keV." + d_str + "m.root";
  
  cout << "\n" << FileName << "\n\n";
    
  // Set the corrent simulation engines:
  TObjString* Monte_Carlo_Engine = new TObjString("TGeant3");
  TObjString* Event_Generator = new TObjString("ascii");
  TObjString* Output_Name = new TObjString(FileName);
  TObjString* Target_Material = new TObjString("LiH");
  
  // ==================== Now we want to check if the neutron beam is actually aimed at NeuLAND.
  Double_t NeuLAND_Xpos = Doubles->GetBinContent(111);
  Double_t NeuLAND_Ypos = Doubles->GetBinContent(112);
  Double_t NeuLAND_Zpos = Doubles->GetBinContent(113);
  
  // Now we have the global position on NeuLAND. Shift it so that the beam is in the center:
  Double_t Beam_Xpos = Doubles->GetBinContent(208);
  Double_t Beam_Ypos = Doubles->GetBinContent(209);
  Double_t Beam_Zpos = Doubles->GetBinContent(210);
  
  NeuLAND_Xpos = NeuLAND_Xpos - Beam_Xpos;
  NeuLAND_Ypos = NeuLAND_Ypos - Beam_Ypos;
  NeuLAND_Zpos = NeuLAND_Zpos - Beam_Zpos;
  
  // Then rotate it so that the beam fires straight along +z-axis:
  TVector3 NeuLAND;
  NeuLAND.SetX(NeuLAND_Xpos);
  NeuLAND.SetY(NeuLAND_Ypos);
  NeuLAND.SetZ(NeuLAND_Zpos);
  
  Double_t Beam_RotX = (TMath::Pi()/180.0)*Doubles->GetBinContent(232);
  Double_t Beam_RotY = (TMath::Pi()/180.0)*Doubles->GetBinContent(233);
  Double_t Beam_RotZ = (TMath::Pi()/180.0)*Doubles->GetBinContent(234);
  
  NeuLAND.RotateZ(-1.0*Beam_RotZ);
  NeuLAND.RotateY(-1.0*Beam_RotY);
  NeuLAND.RotateX(-1.0*Beam_RotX);
  
  NeuLAND_Xpos = NeuLAND.X();
  NeuLAND_Ypos = NeuLAND.Y();
  NeuLAND_Zpos = NeuLAND.Z();
  
  // So now this is the center of NeuLAND when the beam fires from the origin along +z.
  // Then the beam spread is
  Double_t theta = (TMath::Pi()/180.0)*Doubles->GetBinContent(205);
  Double_t Spread = TMath::Abs(NeuLAND_Zpos)*TMath::Tan(theta);
  
  // We define the beam to be aimed at NeuLAND if the center is within half of the spread:
  Double_t Center_Dislocation = TMath::Sqrt(NeuLAND_Xpos*NeuLAND_Xpos + NeuLAND_Ypos*NeuLAND_Ypos);
  Bool_t Aimed = kFALSE;
  if (Center_Dislocation<0.5*Spread) {Aimed = kTRUE;}
  
  if (!Aimed) {cout << "### ERROR: The primary beam is NOT aimed at NeuLAND! These parameters are not suited for calibration!\n\n";}  
  
  // Write the test to an output file:
  std::ofstream txt_file ("../../Data/Inputs/Aimed.txt", std::ofstream::out); // recreate file
  if (Aimed) {txt_file << "1";} else {txt_file << "0";}
  txt_file.close();
  
  // Then finally, we have to write everything to the file:
  Doubles->Write("Doubles",2);
  Integers->Write("Integers",2);
  Target_Material->Write("Target_Material",2);
  Monte_Carlo_Engine->Write("Monte_Carlo_Engine",2);
  Event_Generator->Write("Event_Generator",2);
  Output_Name->Write("Output_Name",2);
  
  // Then close it:
  inputs->Close();  
}

// Now define the main for stand-alone compilation:
#ifndef __CINT__
#include <TApplication.h>
int main(int argc, char** argv)
{
  TApplication* TheApp = new TApplication("SetSim",&argc,argv);
  SetSim();
  return 0;
}
#endif