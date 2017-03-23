// C++ headers:
#include <iostream>
#include <fstream>

// ROOT headers:
#include "TFile.h"
#include "TH1D.h"
#include "TH1I.h"
#include "TObjString.h"
#include "TString.h"
#include "TMath.h"
#include "TRandom3.h"
#include "TVector3.h"

// Own headers:

// Use standard namespace:
using namespace std;

void Create_ASCII_File()
{
  // This function creates an input file (.dat-file)
  // to use for the R3B ASCII event generator. The format of such an input is as follows:
  
  // eventId nTracks pBeam b
  // iPid  Z  A  Px  Py  Pz  X  Y  Z  Mass
  
  // and this has to be repeated for ALL events that you whish to simulate. Hence you get a pretty
  // big text file. If you whish to generate multiple particles per event, then the second line
  // has to be repeated for each particle! Then meaning of the variables is:
  
  // eventId ==> the event number. It has to start at 0 and then increase by 1 for each new event.
  // nTracks: total number of particles for this event.
  // pBeam ==> always 0.0
  // b ==> always 0.0
  // iPid ==> always +/-1 ==> -1 means you specify Z and A manually. Hence you will fire a nucleus.
  //			  ==> +1 ==> Then Z=0 and A becomes the full PDG code of the particle (not only a nucleus).
  // Px, Py, Pz ==> total momenta of the particle in GeV. Notice relativity!
  // X, Y, Z ==> starting point of the track in cm.
  // Mass of the particle you fire.
  
  // We will now generate an ASCII-file appropriate for what we specify
  // in our 2 particle guns in SetParameters.cpp. We will do this from random number generation.
  
  // written by C. A. Douma.
  
  // ===================================================================
  // We begin by connecting to the Inputs.root-file:
  TFile* inputs = new TFile("../../Data/Inputs/Inputs.root","read");
  TH1D* Doubles = (TH1D*) inputs->Get("Doubles");
  TH1I* Integers = (TH1I*) inputs->Get("Integers");
  
  // Then the first thing we should do is decide whether we should create a file or not:
  TObjString* Event_Generator_Obj = (TObjString*) inputs->Get("Event_Generator");
  TString Event_Generator = Event_Generator_Obj->GetString();
  
  // Also get our nametag:
  TObjString* Output_Name_Obj = (TObjString*) inputs->Get("Output_Name");
  TString Output_Name = Output_Name_Obj->GetString();
  
  if (Event_Generator == "ascii")
  {
  
  // Then we obtain our data on the particle guns:
  
  // First Beam properties:
  Int_t Particle_ID = Integers->GetBinContent(218);
  Int_t Num_Prim_Particle = Integers->GetBinContent(219); 
  Double_t Particle_Mass = Doubles->GetBinContent(211);
  Double_t Emin = Doubles->GetBinContent(202);
  Double_t Emax = Doubles->GetBinContent(203);
  Double_t ThetaMin = Doubles->GetBinContent(204);
  Double_t ThetaMax = Doubles->GetBinContent(205);
  Double_t PhiMin = Doubles->GetBinContent(206);
  Double_t PhiMax = Doubles->GetBinContent(207);
  Double_t X_Beam = Doubles->GetBinContent(208);
  Double_t Y_Beam = Doubles->GetBinContent(209);
  Double_t Z_Beam = Doubles->GetBinContent(210);
  Double_t RotationX_Beam = (TMath::Pi()/180.0)*Doubles->GetBinContent(232);
  Double_t RotationY_Beam = (TMath::Pi()/180.0)*Doubles->GetBinContent(233);
  Double_t RotationZ_Beam = (TMath::Pi()/180.0)*Doubles->GetBinContent(234);
  
  // Second Beam properties:
  Int_t Beam2_Exists = Integers->GetBinContent(221);
  Int_t Particle_ID2 = Integers->GetBinContent(222);
  Int_t Num_Prim_Particle2 = Integers->GetBinContent(223); 
  Double_t Particle_Mass2 = Doubles->GetBinContent(221);
  Double_t Emin2 = Doubles->GetBinContent(212);
  Double_t Emax2 = Doubles->GetBinContent(213);
  Double_t ThetaMin2 = Doubles->GetBinContent(214);
  Double_t ThetaMax2 = Doubles->GetBinContent(215);
  Double_t PhiMin2 = Doubles->GetBinContent(216);
  Double_t PhiMax2 = Doubles->GetBinContent(217);
  Double_t X_Beam2 = Doubles->GetBinContent(218);
  Double_t Y_Beam2 = Doubles->GetBinContent(219);
  Double_t Z_Beam2 = Doubles->GetBinContent(220);
  Double_t RotationX_Beam2 = (TMath::Pi()/180.0)*Doubles->GetBinContent(235);
  Double_t RotationY_Beam2 = (TMath::Pi()/180.0)*Doubles->GetBinContent(236);
  Double_t RotationZ_Beam2 = (TMath::Pi()/180.0)*Doubles->GetBinContent(237);
  
  // Third Beam properties:
  Int_t Beam3_Exists = Integers->GetBinContent(225);
  Int_t Particle_ID3 = Integers->GetBinContent(226);
  Int_t Num_Prim_Particle3 = Integers->GetBinContent(227); 
  Double_t Particle_Mass3 = Doubles->GetBinContent(241);
  Double_t Emin3 = Doubles->GetBinContent(223);
  Double_t Emax3 = Doubles->GetBinContent(224);
  Double_t ThetaMin3 = Doubles->GetBinContent(225);
  Double_t ThetaMax3 = Doubles->GetBinContent(226);
  Double_t PhiMin3 = Doubles->GetBinContent(227);
  Double_t PhiMax3 = Doubles->GetBinContent(228);
  Double_t X_Beam3 = Doubles->GetBinContent(229);
  Double_t Y_Beam3 = Doubles->GetBinContent(230);
  Double_t Z_Beam3 = Doubles->GetBinContent(231);
  Double_t RotationX_Beam3 = (TMath::Pi()/180.0)*Doubles->GetBinContent(238);
  Double_t RotationY_Beam3 = (TMath::Pi()/180.0)*Doubles->GetBinContent(239);
  Double_t RotationZ_Beam3 = (TMath::Pi()/180.0)*Doubles->GetBinContent(240);
  
  // See if this is a calibration run:
  Int_t RunID = Integers->GetBinContent(241);
  Int_t Counter = Integers->GetBinContent(1);
  Int_t UseEnergyEstimate = Integers->GetBinContent(247);
  TString st = "";
  TString kstr = st.Itoa(Counter,10);
  TFile* ProtonE_file = 0;
  TH1D* ProtonE = 0;
  if (RunID==1)
  {
      // Then all of this should exist...
      TFile* Safe_inputs = new TFile("../../Data/Inputs/Safe_Inputs.root","read");
      TObjString* Prev_Name_Obj = (TObjString*) Safe_inputs->Get("Output_Name"); // get original data! orginal nametag!
      TString Prev_Name = Prev_Name_Obj->GetString();

      ProtonE_file = new TFile("../../Data/Analysis/Proton_Estimation_" + Prev_Name + "_run=" + kstr + ".root","read");
      ProtonE = (TH1D*) ProtonE_file->Get("Energy_Estimates");
  }
  
  
  // multiplying enery with the number of nucleons:
  Int_t Abeam = -1.0;
  Int_t Zbeam = -1.0;
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
    
    Emin = Emin*Abeam;
    Emax = Emax*Abeam;
  }
  
  Int_t Abeam2 = -1.0;
  Int_t Zbeam2 = -1.0;
  if (TMath::Abs(Particle_ID2)>1000000000)
  {
    Int_t PDGsmall2 = 0;
    if (Particle_ID2>0) {PDGsmall2 = Particle_ID2 - 1000000000;}
    if (Particle_ID2<0) {PDGsmall2 = Particle_ID2 + 1000000000;}
    Double_t PDG_d2 = (Int_t) TMath::Abs(PDGsmall2);

    Double_t Z_d2 = PDG_d2/10.0;
    Double_t R_d2 = PDG_d2/10000.0;
    
    Double_t z_Z_d2 = PDG_d2/10000.0;
    Double_t z_R_d2 = PDG_d2/10000000.0;
    
    Int_t Z2 = (Double_t) Z_d2;
    Int_t R2 = (Double_t) R_d2;
    Z_d2 = Z_d2 - 1000.0*((Int_t) R2);
    Z2 = (Double_t) Z_d2;
    Z_d2 = (Int_t) Z2;
    
    Int_t z_Z2 = (Double_t) z_Z_d2;
    Int_t z_R2 = (Double_t) z_R_d2;
    z_Z_d2 = z_Z_d2 - 1000.0*((Int_t) z_R2);
    z_Z2 = (Double_t) z_Z_d2;
    z_Z_d2 = (Int_t) z_Z2;
    
    Abeam2 = Z_d2;
    Zbeam2 = z_Z_d2;
    
    Emin2 = Emin2*Abeam2;
    Emax2 = Emax2*Abeam2;
  }
  
  Int_t Abeam3 = -1.0;
  Int_t Zbeam3 = -1.0;
  if (TMath::Abs(Particle_ID3)>1000000000)
  {
    Int_t PDGsmall3 = 0;
    if (Particle_ID3>0) {PDGsmall3 = Particle_ID3 - 1000000000;}
    if (Particle_ID3<0) {PDGsmall3 = Particle_ID3 + 1000000000;}
    Double_t PDG_d3 = (Int_t) TMath::Abs(PDGsmall3);

    Double_t Z_d3 = PDG_d3/10.0;
    Double_t R_d3 = PDG_d3/10000.0;
    
    Double_t z_Z_d3 = PDG_d3/10000.0;
    Double_t z_R_d3 = PDG_d3/10000000.0;
    
    Int_t Z3 = (Double_t) Z_d3;
    Int_t R3 = (Double_t) R_d3;
    Z_d3 = Z_d3 - 1000.0*((Int_t) R3);
    Z3 = (Double_t) Z_d3;
    Z_d3 = (Int_t) Z3;
    
    Int_t z_Z3 = (Double_t) z_Z_d3;
    Int_t z_R3 = (Double_t) z_R_d3;
    z_Z_d3 = z_Z_d3 - 1000.0*((Int_t) z_R3);
    z_Z3 = (Double_t) z_Z_d3;
    z_Z_d3 = (Int_t) z_Z3;
    
    Abeam3 = Z_d3;
    Zbeam3 = z_Z_d3;
    
    Emin3 = Emin3*Abeam3;
    Emax3 = Emax3*Abeam3;
  }
  
  // Correct for relativity: We specified kinetic energy, not total momentum!
  Emin = Emin + Particle_Mass;
  Emax = Emax + Particle_Mass;
  Emin = TMath::Sqrt(Emin*Emin - Particle_Mass*Particle_Mass);
  Emax = TMath::Sqrt(Emax*Emax - Particle_Mass*Particle_Mass);
  
  Emin2 = Emin2 + Particle_Mass2;
  Emax2 = Emax2 + Particle_Mass2;
  Emin2 = TMath::Sqrt(Emin2*Emin2 - Particle_Mass2*Particle_Mass2);
  Emax2 = TMath::Sqrt(Emax2*Emax2 - Particle_Mass2*Particle_Mass2);
  
  Emin3 = Emin3 + Particle_Mass3;
  Emax3 = Emax3 + Particle_Mass3;
  Emin3 = TMath::Sqrt(Emin3*Emin3 - Particle_Mass3*Particle_Mass3);
  Emax3 = TMath::Sqrt(Emax3*Emax3 - Particle_Mass3*Particle_Mass3);
  
  // ====================================================================
  // Now the trick is that we specify Px, Py and Pz according to a
  // random distribution. The other properties can be kept fixed as they are:
  
  // Define the event properties:
  Int_t nTracks = Num_Prim_Particle;
  if (Beam2_Exists==1) {nTracks = nTracks + Num_Prim_Particle2;}
  if (Beam3_Exists==1) {nTracks = nTracks + Num_Prim_Particle3;}
  Double_t pBeam = 0.0;
  Double_t b = 0.0;
  
  // Define the first beam properties:
  Int_t iPid1 = 1;
  if (TMath::Abs(Particle_ID)>1000000000) {iPid1 = -1;}
  Int_t Z1 = 0;
  Int_t A1 = Particle_ID; // specify by PDG code!
  if (TMath::Abs(Particle_ID)>1000000000) {Z1 = Zbeam; A1 = Abeam;}
  Double_t Px1 = 0.0;
  Double_t Py1 = 0.0;
  Double_t Pz1 = 0.0;
  Double_t Xpos1 = X_Beam;
  Double_t Ypos1 = Y_Beam;
  Double_t Zpos1 = Z_Beam;
  Double_t Mass1 = Particle_Mass;
  
  // Define the second beam properties:
  Int_t iPid2 = 1;
  if (TMath::Abs(Particle_ID2)>1000000000) {iPid2 = -1;}
  Int_t Z2 = 0;
  Int_t A2 = Particle_ID2; // specify by PDG code!
  if (TMath::Abs(Particle_ID2)>1000000000) {Z2 = Zbeam2; A2 = Abeam2;}
  Double_t Px2 = 0.0;
  Double_t Py2 = 0.0;
  Double_t Pz2 = 0.0;
  Double_t Xpos2 = X_Beam2;
  Double_t Ypos2 = Y_Beam2;
  Double_t Zpos2 = Z_Beam2;
  Double_t Mass2 = Particle_Mass2;
  
  // Define the third beam properties:
  Int_t iPid3 = 1;
  if (TMath::Abs(Particle_ID3)>1000000000) {iPid3 = -1;}
  Int_t Z3 = 0;
  Int_t A3 = Particle_ID3; // specify by PDG code!
  if (TMath::Abs(Particle_ID3)>1000000000) {Z3 = Zbeam3; A3 = Abeam3;}
  Double_t Px3 = 0.0;
  Double_t Py3 = 0.0;
  Double_t Pz3 = 0.0;
  Double_t Xpos3 = X_Beam3;
  Double_t Ypos3 = Y_Beam3;
  Double_t Zpos3 = Z_Beam3;
  Double_t Mass3 = Particle_Mass3;
  
  // Define the random generator that we will need:
  TRandom3* generator = new TRandom3(0);
  
  // Define the theta, phi and energy:
  Double_t theta = 0.0;
  Double_t phi = 0.0;
  Double_t Energy;
  
  // Define the TVector3 that we need:
  TVector3 Beam1;
  TVector3 Beam2;
  TVector3 Beam3;
  
  // ====================================================================
  // Now we must create our file. Create the plain file:
  std::ofstream Our_ascii ("../../Data/Inputs/ASCII_Generator.dat", std::ofstream::out); // recreate file
  
  // Obtain the number of events:
  Int_t nEvents = Integers->GetBinContent(201);
  
  // Then loop over the number of events to write to the file:
  for (Int_t k = 0; k<nEvents; ++k)
  {
    // Begin by writing the first line:
    Our_ascii << "    " << k << "    " << nTracks << "    " << pBeam << "    " << b << "\n";
    
    // Now in order to write the next lines, we have to compute Px, Py, Pz for each particle
    // from random numbers.
    
    // Loop over the number of primary particles for the first beam:
    for (Int_t n = 0; n<Num_Prim_Particle; ++n)
    {
      // generate theta and phi:
      theta = ThetaMin + generator->Uniform(ThetaMax - ThetaMin); // [deg]
      phi = PhiMin + generator->Uniform(PhiMax - PhiMin); // [deg]
      Energy = Emin + generator->Uniform(Emax - Emin); // [GeV] NOTE: Relativity has already been taken care of in Emin and Emax!
      
      // convert to radians:
      theta = (TMath::Pi()/180.0)*theta;
      phi = (TMath::Pi()/180.0)*phi;
      
      // Generate the impulse vector:
      Beam1.SetX(Energy*TMath::Sin(theta)*TMath::Cos(phi));
      Beam1.SetY(Energy*TMath::Sin(theta)*TMath::Sin(phi));
      Beam1.SetZ(Energy*TMath::Cos(theta));
      
      // Rotate the impulse vector as we want it:
      Beam1.RotateX(RotationX_Beam);
      Beam1.RotateY(RotationY_Beam);
      Beam1.RotateZ(RotationZ_Beam);
      
      // Then finally, we can extract the momentum:
      Px1 = Beam1.X();
      Py1 = Beam1.Y();
      Pz1 = Beam1.Z();
      
      // Then now we can write our next line:
      Our_ascii << "    " << iPid1 << "    " << Z1 << "    " << A1 << "    " << Px1 << "    " << Py1 << "    " << Pz1 << "    " << Xpos1 << "    " << Ypos1 << "    " << Zpos1 << "    " << Mass1 << "\n";
      
      // That's it!      
    }
    
    // Now do everything again for the second beam, but only if the second beam exists:
    if (Beam2_Exists==1)
    {
      for (Int_t n = 0; n<Num_Prim_Particle2; ++n)
      {
	// generate theta and phi:
	theta = ThetaMin2 + generator->Uniform(ThetaMax2 - ThetaMin2); // [deg]
	phi = PhiMin2 + generator->Uniform(PhiMax2 - PhiMin2); // [deg]
	Energy = Emin2 + generator->Uniform(Emax2 - Emin2); // [GeV] NOTE: Relativity has already been taken care of in Emin and Emax!
        
        // If This is a calibration run, then energy should be distributed accoding to (TH1D*) ProtonE. Check it and decide if we use it.
        if ((RunID==1)&&(UseEnergyEstimate==1)) {Energy = 0.001*ProtonE->GetRandom();} // NOTE: Conversion to [GeV]!
      
	// convert to radians:
	theta = (TMath::Pi()/180.0)*theta;
	phi = (TMath::Pi()/180.0)*phi;
      
	// Generate the impulse vector:
	Beam2.SetX(Energy*TMath::Sin(theta)*TMath::Cos(phi));
	Beam2.SetY(Energy*TMath::Sin(theta)*TMath::Sin(phi));
	Beam2.SetZ(Energy*TMath::Cos(theta));
      
	// Rotate the impulse vector as we want it:
	Beam2.RotateX(RotationX_Beam2);
	Beam2.RotateY(RotationY_Beam2);
	Beam2.RotateZ(RotationZ_Beam2);
      
	// Then finally, we can extract the momentum:
	Px2 = Beam2.X();
	Py2 = Beam2.Y();
	Pz2 = Beam2.Z();
      
	// Then now we can write our next line:
	Our_ascii << "    " << iPid2 << "    " << Z2 << "    " << A2 << "    " << Px2 << "    " << Py2 << "    " << Pz2 << "    " << Xpos2 << "    " << Ypos2 << "    " << Zpos2 << "    " << Mass2 << "\n";
      
	// That's it!      
      }
    }
    
    // Now do everything again for the third beam, but only if the third beam exists:
    if (Beam3_Exists==1)
    {
      for (Int_t n = 0; n<Num_Prim_Particle3; ++n)
      {
	// generate theta and phi:
	theta = ThetaMin3 + generator->Uniform(ThetaMax3 - ThetaMin3); // [deg]
	phi = PhiMin3 + generator->Uniform(PhiMax3 - PhiMin3); // [deg]
	Energy = Emin3 + generator->Uniform(Emax3 - Emin3); // [GeV] NOTE: Relativity has already been taken care of in Emin and Emax!
      
	// convert to radians:
	theta = (TMath::Pi()/180.0)*theta;
	phi = (TMath::Pi()/180.0)*phi;
      
	// Generate the impulse vector:
	Beam3.SetX(Energy*TMath::Sin(theta)*TMath::Cos(phi));
	Beam3.SetY(Energy*TMath::Sin(theta)*TMath::Sin(phi));
	Beam3.SetZ(Energy*TMath::Cos(theta));
      
	// Rotate the impulse vector as we want it:
	Beam3.RotateX(RotationX_Beam3);
	Beam3.RotateY(RotationY_Beam3);
	Beam3.RotateZ(RotationZ_Beam3);
      
	// Then finally, we can extract the momentum:
	Px3 = Beam3.X();
	Py3 = Beam3.Y();
	Pz3 = Beam3.Z();
      
	// Then now we can write our next line:
	Our_ascii << "    " << iPid3 << "    " << Z3 << "    " << A3 << "    " << Px3 << "    " << Py3 << "    " << Pz3 << "    " << Xpos3 << "    " << Ypos3 << "    " << Zpos3 << "    " << Mass3 << "\n";
      
	// That's it!      
      }
    }
    
    
    
    
  }
  
  // Now that we wrote everything to this file, close it:
  Our_ascii.close();
  
  // And then we are done!
  }
}

// Now define the main for stand-alone compilation:
#ifndef __CINT__
#include <TApplication.h>
int main(int argc, char** argv)
{
  TApplication* TheApp = new TApplication("Create_ASCII_File",&argc,argv);
  Create_ASCII_File();
  return 0;
}
#endif
