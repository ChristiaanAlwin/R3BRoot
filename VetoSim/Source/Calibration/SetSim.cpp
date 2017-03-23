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

  // And also the number of protons. FIXME: Are they only protons?
  Int_t N_protons = 1;
N_protons = 4;
  
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
  Doubles->SetBinContent(211,0.939565379); // neutron mass.
  
  // Compute distance between the primary beam and the target:
  Double_t dist = 0.0;
  dist = dist + (Doubles->GetBinContent(208) - Doubles->GetBinContent(21))*(Doubles->GetBinContent(208) - Doubles->GetBinContent(21));
  dist = dist + (Doubles->GetBinContent(209) - Doubles->GetBinContent(22))*(Doubles->GetBinContent(209) - Doubles->GetBinContent(22));
  dist = dist + (Doubles->GetBinContent(210) - Doubles->GetBinContent(23))*(Doubles->GetBinContent(210) - Doubles->GetBinContent(23));
  dist = TMath::Sqrt(dist);
  
  // The primary beam should always come from the target:
  Doubles->SetBinContent(208,Doubles->GetBinContent(21));
  Doubles->SetBinContent(209,Doubles->GetBinContent(22));
  Doubles->SetBinContent(210,Doubles->GetBinContent(23));
  
  // Set the angular spread for the neutrons:
  Doubles->SetBinContent(204,0.0);
  Doubles->SetBinContent(205,4.584);
  Doubles->SetBinContent(206,0.0);
  Doubles->SetBinContent(207,360.0);
  
  // switch off the other beams:
  Integers->SetBinContent(221,0);
  Integers->SetBinContent(225,0);
  
  // Set the beam angular spread:
  Integers->SetBinContent(248,1);
    
  // We calibrate protons with the second beam (unchanged!)
  if (N_protons>0)
  {
    Integers->SetBinContent(221,1);
    Integers->SetBinContent(222,Integers->GetBinContent(251));
    Doubles->SetBinContent(221,Doubles->GetBinContent(246)); // charged track mass.
    Integers->SetBinContent(223,N_protons);
    // NOTE: we always set the generator to ascii for calibration! The ascii-generator is
    // written in such a way that it ALWAYS will generate proton from the histogram!
  }
  
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
  Integers->SetBinContent(205,0); // Magnetic field is off, so we do not spoil the protons!
  Integers->SetBinContent(220,1); // 1 run, not more
  Integers->SetBinContent(253,0); // We do NOT use the dummy vacuum chamber, since protons will hit the neutron window and spoil the calibration!
  Integers->SetBinContent(241,1); // This is a Neutron Calibration run!
  
  // Adapt the time cuts for the distance between the beam and the target:
  Double_t Ekin = 0.5*(Doubles->GetBinContent(202) + Doubles->GetBinContent(203));
  Double_t Mass = Doubles->GetBinContent(211);
  Double_t gamma = (Ekin+Mass)/Mass;
  Double_t beta = TMath::Sqrt(1.0 - 1.0/(gamma*gamma));
  Double_t cspeed = 29.9792458;
  Double_t Tdiff = dist/(cspeed*beta);
  
  Double_t Tup = Doubles->GetBinContent(344);
  Double_t Tlow = Doubles->GetBinContent(345);
  Doubles->SetBinContent(344,Tup - Tdiff);
  Doubles->SetBinContent(345,Tlow - Tdiff);
  
  // Define the correct filename:
  TString FileName = "";
  TString st = "";
  TString N_neutrons_str = st.Itoa(N_neutrons,10);
  TString N_protons_str = st.Itoa(N_protons,10);
  FileName = "CalibrationFile_" + N_protons_str + "p_" + N_neutrons_str + "n.root";
  
  cout << "\n" << FileName << "\n\n";
    
  // Set the corrent simulation engines:
  Int_t G3Test = Integers->GetBinContent(249);
  TObjString* Monte_Carlo_Engine = new TObjString("TGeant3");
  if (G3Test==0) {Monte_Carlo_Engine = (TObjString*) inputs->Get("Monte_Carlo_Engine");}
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