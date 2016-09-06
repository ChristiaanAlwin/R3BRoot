// This function performs the loading of the inputs from Inputs.root
// into the Inputs_Structure called inputs. In that way,
// // the inputs can be accessed from the structure in the rest of the analysis.
// A check will be performed whether this loading was succesful.

// Written by C. A. Douma, for the R3B-collaboration.

// inclusion of C++ headers:
#include <iostream>

// Inclusion of required headers:
#include "TMath.h"
#include "TH1D.h"
#include "TH1I.h"
#include "TObjString.h"
#include "TString.h"
#include "TFile.h"
#include "TSystem.h"

// Inclusion of the Leaf_Structure header:
#include "Inputs_Structure.h"

// Standard namespace:
using namespace std;

// Definition of the fucntion:
Bool_t Inputs_Load(Inputs_Structure* Inputs, Int_t const Counter, Int_t const Series)
{
   // The idea is that a pointer to a big Inputs_Structure called Inputs is
   // passed as argument to this function. NOTE: only a pointer should be
   // passed, not the structure itself. This causes the content of the 
   // structure to be modified outside of the scope of this function, exactly
   // as we like it!
  
   // The second argument is the counter. This identifies which .root-file
   // we exactly whish to connect to. Since only a pointer is passed and
   // not the structure itself, no outputs are required.
  
   // The third argument is a integer that states if we should use the
   // value of the inputs as they are, or adapt them from the counter.
   // it also states which values should be adapted.
  
   // We do however give a boolian as output that tells us whether the 
   // loading was succesful or not.
  
   // All we do in this function is to fill the valiables in the structure, so that
   // they can be used conveniently in the rest of the analysis.
    
   // NOTE: In geoemtry macros all rotations are in degrees. That is also
   // NOTE: how they are specified in SetParameters.cpp When we however
   // NOTE: rotate vectors in our analysis, the arguments must be supplied
   // NOTE: in radians. Hence here all angles are converted from degrees
   // NOTE: into radians!
  
   // ================================================================================================
   // We begin with defining the boolian for the test:
   Bool_t Loading = kFALSE;
   cout << "=============== Loading all inputs from Inputs.root-file...\n\n";
   
   // ================================================================================================
   // We begin by connecting to the Inputs.root-file. 
   
   // Check if this file exists:
   FileStat_t stom;
   Int_t inputs_test = gSystem->GetPathInfo("../../Data/Inputs/Inputs.root",stom);
   
   if (inputs_test==1)
   {
     cout << "### ERROR: The Inputs.root-file does not exist! Create it first!\n\n";
   }
   else
   {
   
   // Connect to the Inputs.root file:
   TFile* inputs = new TFile("../../Data/Inputs/Inputs.root","read");
   cout << "Connection to the Inputs.root file was succesful.\n\n";
   
   // And then Get the histograms from this file:
   TH1D* Doubles = (TH1D*) inputs->Get("Doubles");
   TH1I* Integers = (TH1I*) inputs->Get("Integers");
   
   // ================================================================================================
   // Now that we have the Inputs.root-file and the two key histograms in there, we can begin with
   // loading the actual inputs. First we have to test however that the histograms are actually there:
   
   if (!((Doubles!=0)&&(Integers!=0)))
   {
     cout << "### ERROR: Recheck your code! somehow the Integers and/or Doubles histogram do not exist!\n\n";
   }
   else
   {
     cout << "The Doubles and Integers histograms are found.\n\n";
     
  // Now aslo check that all the required object strings exist:
  TObjString* Target_Material_Obj = (TObjString*) inputs->Get("Target_Material");
  TObjString* Monte_Carlo_Engine_Obj = (TObjString*) inputs->Get("Monte_Carlo_Engine");
  TObjString* Event_Generator_Obj = (TObjString*) inputs->Get("Event_Generator");
  TObjString* Output_Name_Obj = (TObjString*) inputs->Get("Output_Name");
  
  if (!((Target_Material_Obj!=0)&&(Monte_Carlo_Engine_Obj!=0)&&(Event_Generator_Obj!=0)&&(Output_Name_Obj!=0)))
  {
    cout << "### ERROR: Rexcheck your code! some of the TObjString* are missing!\n\n";
  }
  else
  {
    cout << "All TObjString* are found as well.\n\n";
  
  // Now we can begin with loading the inputs:
   
  // PSP Geometry parameters:
  Inputs->PSP_geo_det1_Xpos = Doubles->GetBinContent(81); 	                 	// PSP detector 1 global x position [cm]. Default:    0.0
  Inputs->PSP_geo_det1_Ypos = Doubles->GetBinContent(82);		                // PSP detector 1 global y position [cm]. Default:    0.0
  Inputs->PSP_geo_det1_Zpos = Doubles->GetBinContent(83);		                // PSP detector 1 global z position [cm]. Default: -221.0
  Inputs->PSP_geo_det1_RotX = ((TMath::Pi())/180.0)*Doubles->GetBinContent(84);		// PSP detector 1 rotation around x-axis [deg.] Default:  0.0
  Inputs->PSP_geo_det1_RotY = ((TMath::Pi())/180.0)*Doubles->GetBinContent(85);		// PSP detector 1 rotation around y-axis [deg.] Default:  0.0
  Inputs->PSP_geo_det1_RotZ = ((TMath::Pi())/180.0)*Doubles->GetBinContent(86);		// PSP detector 1 rotation around z-axis [deg.] Default:  0.0
  Inputs->PSP_geo_det2_Xpos = Doubles->GetBinContent(87); 		                // PSP detector 2 global x position [cm]. Default:   0.0
  Inputs->PSP_geo_det2_Ypos = Doubles->GetBinContent(88);		                // PSP detector 2 global y position [cm]. Default:   0.0
  Inputs->PSP_geo_det2_Zpos = Doubles->GetBinContent(89);		                // PSP detector 2 global z position [cm]. Default: -89.0
  Inputs->PSP_geo_det2_RotX = ((TMath::Pi())/180.0)*Doubles->GetBinContent(90);		// PSP detector 2 rotation around x-axis [deg.] Default:  0.0
  Inputs->PSP_geo_det2_RotY = ((TMath::Pi())/180.0)*Doubles->GetBinContent(91);		// PSP detector 2 rotation around y-axis [deg.] Default:  0.0
  Inputs->PSP_geo_det2_RotZ = ((TMath::Pi())/180.0)*Doubles->GetBinContent(92);		// PSP detector 2 rotation around z-axis [deg.] Default:  0.0
  Inputs->PSP_geo_det3_Xpos = Doubles->GetBinContent(93); 	                 	// PSP detector 3 global x position [cm]. Default:   0.0
  Inputs->PSP_geo_det3_Ypos = Doubles->GetBinContent(94);	                  	// PSP detector 3 global y position [cm]. Default:   0.0
  Inputs->PSP_geo_det3_Zpos = Doubles->GetBinContent(95);	                  	// PSP detector 3 global z position [cm]. Default:  94.0
  Inputs->PSP_geo_det3_RotX = ((TMath::Pi())/180.0)*Doubles->GetBinContent(96);		// PSP detector 3 rotation around x-axis [deg.] Default:  0.0
  Inputs->PSP_geo_det3_RotY = ((TMath::Pi())/180.0)*Doubles->GetBinContent(97);		// PSP detector 3 rotation around y-axis [deg.] Default:  0.0
  Inputs->PSP_geo_det3_RotZ = ((TMath::Pi())/180.0)*Doubles->GetBinContent(98);		// PSP detector 3 rotation around z-axis [deg.] Default:  0.0
  
  // TARGET paremeters: Material & Geometry:
  Inputs->Target_Material = Target_Material_Obj->GetString();	                  	// Choice of target material [TString]
  Inputs->Target_geo_Xpos = Doubles->GetBinContent(21); 	                   	// Target global x position [cm]. Default:  0.0
  Inputs->Target_geo_Ypos = Doubles->GetBinContent(22);		                   	// Target global y position [cm]. Default:  0.0
  Inputs->Target_geo_Zpos = Doubles->GetBinContent(23);		                       	// Target global z position [cm]. Default:  0.0
  Inputs->Target_geo_ThetaRotX = ((TMath::Pi())/180.0)*Doubles->GetBinContent(24);	// Target global Theta rotation with polar x-axis [deg.] Default:  90.0
  Inputs->Target_geo_ThetaRotY = ((TMath::Pi())/180.0)*Doubles->GetBinContent(25);	// Target global Theta rotation with polar y-axis [deg.] Default:  90.0
  Inputs->Target_geo_ThetaRotZ = ((TMath::Pi())/180.0)*Doubles->GetBinContent(26);	// Target global Theta rotation with polar z-axis [deg.] Default:   0.0
  Inputs->Target_geo_PhiRotX = ((TMath::Pi())/180.0)*Doubles->GetBinContent(27);	// Target global Phi rotation with polar x-axis [deg.] Default:   0.0
  Inputs->Target_geo_PhiRotY = ((TMath::Pi())/180.0)*Doubles->GetBinContent(28);	// Target global Phi rotation with polar y-axis [deg.] Default:  90.0
  Inputs->Target_geo_PhiRotZ = ((TMath::Pi())/180.0)*Doubles->GetBinContent(29);	// Target global Phi rotation with polar z-axis [deg.] Default:   0.0 
  
  // GLAD Geometry parameters:
  Inputs->GLAD_geo_Xpos = Doubles->GetBinContent(101); 		                   	// GLAD global x position [cm]. Default:   0.0
  Inputs->GLAD_geo_Ypos = Doubles->GetBinContent(102);		                	// GLAD global y position [cm]. Default:   0.0
  Inputs->GLAD_geo_Zpos = Doubles->GetBinContent(103);		                	// GLAD global z position [cm]. Default:   0.0
  Inputs->GLAD_geo_RotX = ((TMath::Pi())/180.0)*Doubles->GetBinContent(104);    	// GLAD rotation around x-axis [deg.] Default:   0.0
  Inputs->GLAD_geo_RotY = ((TMath::Pi())/180.0)*Doubles->GetBinContent(105);		// GLAD rotation around y-axis [deg.] Default:  90.0
  Inputs->GLAD_geo_RotZ = ((TMath::Pi())/180.0)*Doubles->GetBinContent(106);		// GLAD rotation around z-axis [deg.] Default: -90.0
  
  // CALIFA Geometry parameters:
  Inputs->CALIFA_geo_Xpos = Doubles->GetBinContent(11); 		                // CALIFA global x position [cm]. Default:  0.0
  Inputs->CALIFA_geo_Ypos = Doubles->GetBinContent(12);		                   	// CALIFA global y position [cm]. Default:  0.0
  Inputs->CALIFA_geo_Zpos = Doubles->GetBinContent(13);		                   	// CALIFA global z position [cm]. Default:  0.0
  Inputs->CALIFA_geo_RotX = ((TMath::Pi())/180.0)*Doubles->GetBinContent(14);		// CALIFA rotation around x-axis [deg.] Default:  0.0
  Inputs->CALIFA_geo_RotY = ((TMath::Pi())/180.0)*Doubles->GetBinContent(15);		// CALIFA rotation around y-axis [deg.] Default:  0.0
  Inputs->CALIFA_geo_RotZ = ((TMath::Pi())/180.0)*Doubles->GetBinContent(16);		// CALIFA rotation around z-axis [deg.] Default:  0.0
  
  // TOF wall Geometry parameters:
  Inputs->TOF_geo_Xpos = Doubles->GetBinContent(41); 	                  		// TOF wall global x position [cm]. Default: -417.359574
  Inputs->TOF_geo_Ypos = Doubles->GetBinContent(42);		                  	// TOF wall global y position [cm]. Default:    2.4
  Inputs->TOF_geo_Zpos = Doubles->GetBinContent(43);			                // TOF wall global z position [cm]. Default:  960.777114
  Inputs->TOF_geo_RotX = ((TMath::Pi())/180.0)*Doubles->GetBinContent(44);		// TOF wall rotation around x-axis [deg.] Default:   0.0
  Inputs->TOF_geo_RotY = ((TMath::Pi())/180.0)*Doubles->GetBinContent(45);		// TOF wall rotation around y-axis [deg.] Default: -31.0
  Inputs->TOF_geo_RotZ = ((TMath::Pi())/180.0)*Doubles->GetBinContent(46);		// TOF wall rotation around z-axis [deg.] Default:   0.0
  
  // mTOF Geometry parameters:
  Inputs->mTOF_geo_Xpos = Doubles->GetBinContent(1); 	                  		// mTOF global x position [cm]. Default: -155.824045
  Inputs->mTOF_geo_Ypos = Doubles->GetBinContent(2);			                // mTOF global y position [cm]. Default:    0.523976
  Inputs->mTOF_geo_Zpos = Doubles->GetBinContent(3);		                  	// mTOF global z position [cm]. Default:  761.870345
  Inputs->mTOF_geo_ThetaRotX = ((TMath::Pi())/180.0)*Doubles->GetBinContent(4);		// mTOF global Theta rotation with polar x-axis [deg.] Default: -106.7
  Inputs->mTOF_geo_ThetaRotY = ((TMath::Pi())/180.0)*Doubles->GetBinContent(5);		// mTOF global Theta rotation with polar y-axis [deg.] Default:   90.0
  Inputs->mTOF_geo_ThetaRotZ = ((TMath::Pi())/180.0)*Doubles->GetBinContent(6);		// mTOF global Theta rotation with polar z-axis [deg.] Default:  -16.7
  Inputs->mTOF_geo_PhiRotX = ((TMath::Pi())/180.0)*Doubles->GetBinContent(7);		// mTOF global Phi rotation with polar x-axis [deg.] Default:   0.0
  Inputs->mTOF_geo_PhiRotY = ((TMath::Pi())/180.0)*Doubles->GetBinContent(8);		// mTOF global Phi rotation with polar y-axis [deg.] Default:  90.0
  Inputs->mTOF_geo_PhiRotZ = ((TMath::Pi())/180.0)*Doubles->GetBinContent(9);		// mTOF global Phi rotation with polar z-axis [deg.] Default:   0.0 
			 
  // DCH Geometry parameters:
  Inputs->DCH_geo_Xpos = Doubles->GetBinContent(31); 	                      		// DCH global x position [cm]. Default:  0.0
  Inputs->DCH_geo_Ypos = Doubles->GetBinContent(32);		                  	// DCH global y position [cm]. Default:  0.0
  Inputs->DCH_geo_Zpos = Doubles->GetBinContent(33);		                  	// DCH global z position [cm]. Default:  0.0
  Inputs->DCH_geo_RotX = ((TMath::Pi())/180.0)*Doubles->GetBinContent(34);		// DCH rotation around x-axis [deg.] Default:  0.0
  Inputs->DCH_geo_RotY = ((TMath::Pi())/180.0)*Doubles->GetBinContent(35);		// DCH rotation around y-axis [deg.] Default:  0.0
  Inputs->DCH_geo_RotZ = ((TMath::Pi())/180.0)*Doubles->GetBinContent(36);		// DCH rotation around z-axis [deg.] Default:  0.0
  
  // StaRTrack Geometry parameters:
  Inputs->STaRTrack_geo_Xpos = Doubles->GetBinContent(51); 	                	// StaRTrack global x position [cm]. Default:  0.0
  Inputs->STaRTrack_geo_Ypos = Doubles->GetBinContent(52);	                 	// StaRTrack global y position [cm]. Default:  0.0
  Inputs->STaRTrack_geo_Zpos = Doubles->GetBinContent(53);	                 	// StaRTrack global z position [cm]. Default:  0.0
  Inputs->STaRTrack_geo_RotX = ((TMath::Pi())/180.0)*Doubles->GetBinContent(54);	// StaRTrack rotation around x-axis [deg.] Default:  0.0
  Inputs->STaRTrack_geo_RotY = ((TMath::Pi())/180.0)*Doubles->GetBinContent(55);        // StaRTrack rotation around y-axis [deg.] Default:  0.0
  Inputs->STaRTrack_geo_RotZ = ((TMath::Pi())/180.0)*Doubles->GetBinContent(56);	// StaRTrack rotation around z-axis [deg.] Default:  0.0
  
  // Vacuum Vessel Geometry parameters:
  Inputs->VACVESSEL_geo_Xpos = Doubles->GetBinContent(61); 	                	// Vacuum Vessel global x position [cm]. Default:  0.0
  Inputs->VACVESSEL_geo_Ypos = Doubles->GetBinContent(62);	                 	// Vacuum Vessel global y position [cm]. Default:  0.0
  Inputs->VACVESSEL_geo_Zpos = Doubles->GetBinContent(63);	                 	// Vacuum Vessel global z position [cm]. Default:  0.0
  Inputs->VACVESSEL_geo_RotX = ((TMath::Pi())/180.0)*Doubles->GetBinContent(64);	// Vacuum Vessel rotation around x-axis [deg.] Default:  0.0
  Inputs->VACVESSEL_geo_RotY = ((TMath::Pi())/180.0)*Doubles->GetBinContent(65);	// Vacuum Vessel rotation around y-axis [deg.] Default:  0.0
  Inputs->VACVESSEL_geo_RotZ = ((TMath::Pi())/180.0)*Doubles->GetBinContent(66);	// Vacuum Vessel rotation around z-axis [deg.] Default:  0.0
  
  // MFI Geometry parameters:
  Inputs->MFI_geo_Xpos = Doubles->GetBinContent(71);            	 		// MFI global x position [cm]. Default:  -63.82
  Inputs->MFI_geo_Ypos = Doubles->GetBinContent(72);	                   		// MFI global y position [cm]. Default:    0.0
  Inputs->MFI_geo_Zpos = Doubles->GetBinContent(73);			                // MFI global z position [cm]. Default:  520.25
  Inputs->MFI_geo_RotX = ((TMath::Pi())/180.0)*Doubles->GetBinContent(74);		// MFI rotation around x-axis [deg.] Default:   0.0
  Inputs->MFI_geo_RotY = ((TMath::Pi())/180.0)*Doubles->GetBinContent(75);		// MFI rotation around y-axis [deg.] Default: -13.5
  Inputs->MFI_geo_RotZ = ((TMath::Pi())/180.0)*Doubles->GetBinContent(76);		// MFI rotation around z-axis [deg.] Default:   0.0
  
  // Vacuum Chamber Geometry parameters:
  Inputs->VACchamber_geo_Xpos = Doubles->GetBinContent(151); 		                // Vacuum Chamber global x position [cm]. Default:  0.0
  Inputs->VACchamber_geo_Ypos = Doubles->GetBinContent(152);		                // Vacuum Chamber global y position [cm]. Default:  0.0
  Inputs->VACchamber_geo_Zpos = Doubles->GetBinContent(153);		                // Vacuum Chamber global z position [cm]. Default:  0.0
  
  Inputs->VACchamber_geo_RotX = ((TMath::Pi())/180.0)*Doubles->GetBinContent(154);	// Vacuum Chamber rotation around x-axis [deg.] Default:  0.0
  Inputs->VACchamber_geo_RotY = ((TMath::Pi())/180.0)*Doubles->GetBinContent(155);	// Vacuum Chamber rotation around y-axis [deg.] Default:  0.0
  Inputs->VACchamber_geo_RotZ = ((TMath::Pi())/180.0)*Doubles->GetBinContent(156);	// Vacuum Chamber rotation around z-axis [deg.] Default:  0.0
  
  // NEBULA Geometry Parameters:
  Inputs->NEBULA_geo_Xpos = Doubles->GetBinContent(141); 		                // NEBULA global x position [cm]. Default:  0.0
  Inputs->NEBULA_geo_Ypos = Doubles->GetBinContent(142);	                   	// NEBULA global y position [cm]. Default:  0.0
  Inputs->NEBULA_geo_Zpos = Doubles->GetBinContent(143);	                	// NEBULA global z position [cm]. Default:  1800.0
  Inputs->NEBULA_geo_RotX = ((TMath::Pi())/180.0)*Doubles->GetBinContent(144);	        // NEBULA rotation around x-axis [deg.] Default:  0.0
  Inputs->NEBULA_geo_RotY = ((TMath::Pi())/180.0)*Doubles->GetBinContent(145);	        // NEBULA rotation around y-axis [deg.] Default:  0.0
  Inputs->NEBULA_geo_RotZ = ((TMath::Pi())/180.0)*Doubles->GetBinContent(146);	        // NEBULA rotation around z-axis [deg.] Default:  0.0
  Inputs->NEBULA_geo_Dist = Doubles->GetBinContent(147);		                // NEBULA Distance between the parts [cm]. Default:  80.0
  Inputs->NEBULA_geo_Nparts = Integers->GetBinContent(5);		                // NEBULA number of parts. Default = 2.
  Inputs->NEBULA_geo_VetoDist = Doubles->GetBinContent(148);                            // NEBULA Distance between VETO and NEBULA (within one part) [cm]. Default = 25.0
  
  // NeuLAND Geometry parameters:
  Inputs->NeuLAND_geo_Xpos = Doubles->GetBinContent(111); 	                 	// NeuLAND global x position [cm]. Default:  0.0
  Inputs->NeuLAND_geo_Ypos = Doubles->GetBinContent(112);               		// NeuLAND global y position [cm]. Default:  0.0
  Inputs->NeuLAND_geo_Zpos = Doubles->GetBinContent(113);	                  	// NeuLAND global z position [cm]. Default:  1550.0
  Inputs->NeuLAND_geo_RotX = ((TMath::Pi())/180.0)*Doubles->GetBinContent(114);		// NeuLAND rotation around x-axis [deg.] Default:  0.0
  Inputs->NeuLAND_geo_RotY = ((TMath::Pi())/180.0)*Doubles->GetBinContent(115);		// NeuLAND rotation around y-axis [deg.] Default:  0.0
  Inputs->NeuLAND_geo_RotZ = ((TMath::Pi())/180.0)*Doubles->GetBinContent(116);		// NeuLAND rotation around z-axis [deg.] Default:  0.0
  Inputs->NeuLAND_geo_NsinglePlanes = Integers->GetBinContent(2);               	// NeuLAND number of Single Planes you want to use [int]. Default = 60
  
  // VETO Geometry parameters:
  Inputs->VETO_geo_Xpos = Doubles->GetBinContent(121);	                 		// VETO global x position [cm]. Default:     0.0
  Inputs->VETO_geo_Ypos = Doubles->GetBinContent(122);	                 		// VETO global y position [cm]. Default:     0.0
  Inputs->VETO_geo_Zpos = Doubles->GetBinContent(123);	                 		// VETO global z position [cm]. Default:  1000.0
  Inputs->VETO_geo_RotX = ((TMath::Pi())/180.0)*Doubles->GetBinContent(124);		// VETO rotation around x-axis [deg.] Default: 0.0
  Inputs->VETO_geo_RotY = ((TMath::Pi())/180.0)*Doubles->GetBinContent(125);		// VETO rotation around y-axis [deg.] Default: 0.0
  Inputs->VETO_geo_RotZ = ((TMath::Pi())/180.0)*Doubles->GetBinContent(126);		// VETO rotation around z-axis [deg.] Default: 0.0
  Inputs->VETO_geo_TotalBarLength = Doubles->GetBinContent(127);                	// VETO Scintillator bar length [cm].      Default:  250.0 (NeuLAND)
  Inputs->VETO_geo_TotalBarWidth = Doubles->GetBinContent(128);	                	// VETO Scintillator bar width [cm].       Default:    5.0 (NeuLAND)
  Inputs->VETO_geo_TotalBarThickness = Doubles->GetBinContent(129);             	// VETO Scintillator bar thickness [cm].   Default:    5.0 (NeuLAND)
  Inputs->VETO_geo_TotalGab = Doubles->GetBinContent(130);		                // VETO total gab size between bars [cm].  Default:   0.06 (NeuLAND)
  Inputs->VETO_geo_FoilThickness = Doubles->GetBinContent(131);	                	// VETO thickness of reflection foil [cm]. Default:   0.02 (NeuLAND)
  Inputs->VETO_geo_TapeThickness = Doubles->GetBinContent(132);	                	// VETO thickness of wrapping tape [cm].   Default:   0.05 (NeuLAND)
  Inputs->VETO_geo_NsinglePlanes = Integers->GetBinContent(3); 	                	// VETO Number of single planes you want to use [int].     Default = 2
  Inputs->VETO_geo_NbarsPerPlane = Integers->GetBinContent(4); 	                	// VETO Number of bars within a single plane [int].        Default = 50 (NeuLAND)
  
  // First Beam properties:
  Inputs->Beam_PDG = Integers->GetBinContent(218);			// Particle ID code: particle species [int]. Default = 2112 (neutrons) NOTE: Check if the mass is added at the end of this file!
  Inputs->Beam_Mass = Doubles->GetBinContent(211);			// Particle total mass [GeV].
  Inputs->Beam_Nparticles = Integers->GetBinContent(219); 		// Number of primary particles per event [int]. Default = 1
  Inputs->Beam_Emin = Doubles->GetBinContent(202);			// Lower Bound op particle total energy! [GeV]
  Inputs->Beam_Emax = Doubles->GetBinContent(203);			// Upper Bound op particle total energy! [GeV]
  Inputs->Beam_ThetaMin = Doubles->GetBinContent(204);			// Lower bound of angle with z-axis at which particles are emitted [deg.] Default = 0.0
  Inputs->Beam_ThetaMax = Doubles->GetBinContent(205);			// Upper bound of angle with z-axis at which particles are emitted [deg.] Default = 4.584 (80 mrad)
  Inputs->Beam_PhiMin = Doubles->GetBinContent(206);			// Lower bound of azimuthal angle at which particles are emitted [deg.] Default = 0.0
  Inputs->Beam_PhiMax = Doubles->GetBinContent(207);			// Upper bound of azimuthal angle at which particles are emitted [deg.] Default = 360.0
  Inputs->Beam_Xpos = Doubles->GetBinContent(208);			// x-coordinate of location of particle gun [cm]. Default = 0.0
  Inputs->Beam_Ypos = Doubles->GetBinContent(209);			// y-coordinate of location of particle gun [cm]. Default = 0.0
  Inputs->Beam_Zpos = Doubles->GetBinContent(210);			// z-coordinate of location of particle gun [cm]. Default = -200.0
  Inputs->Beam_Xrot_ASCII = ((TMath::Pi())/180.0)*Doubles->GetBinContent(232);		// Rotation of particle gun around X-axis [deg] (only for ASCII generator)
  Inputs->Beam_Yrot_ASCII = ((TMath::Pi())/180.0)*Doubles->GetBinContent(233);		// Rotation of particle gun around Y-axis [deg] (only for ASCII generator)
  Inputs->Beam_Zrot_ASCII = ((TMath::Pi())/180.0)*Doubles->GetBinContent(234);		// Rotation of particle gun around Z-axis [deg] (only for ASCII generator)
  
  // Second beam properties:
  Inputs->Beam2_Exe = Integers->GetBinContent(221);			// A second beam is only simulated if this bin is set equal to unity.
  Inputs->Beam2_PDG = Integers->GetBinContent(222);			// Particle ID code: particle species [int]. Default = 2112 (neutrons) NOTE: Check if the mass is added at the end of this file!
  Inputs->Beam2_Mass = Doubles->GetBinContent(221);			// Particle total mass [GeV].
  Inputs->Beam2_Nparticles = Integers->GetBinContent(223); 		// Number of primary particles per event [int]. Default = 1
  Inputs->Beam2_Emin = Doubles->GetBinContent(212);			// Lower Bound op particle total energy! [GeV]
  Inputs->Beam2_Emax = Doubles->GetBinContent(213);			// Upper Bound op particle total energy! [GeV]
  Inputs->Beam2_ThetaMin = Doubles->GetBinContent(214);			// Lower bound of angle with z-axis at which particles are emitted [deg.] Default = 0.0
  Inputs->Beam2_ThetaMax = Doubles->GetBinContent(215);	       	 	// Upper bound of angle with z-axis at which particles are emitted [deg.] Default = 4.584 (80 mrad)
  Inputs->Beam2_PhiMin = Doubles->GetBinContent(216);			// Lower bound of azimuthal angle at which particles are emitted [deg.] Default = 0.0
  Inputs->Beam2_PhiMax = Doubles->GetBinContent(217);			// Upper bound of azimuthal angle at which particles are emitted [deg.] Default = 360.0
  Inputs->Beam2_Xpos = Doubles->GetBinContent(218);			// x-coordinate of location of particle gun [cm]. Default = 0.0
  Inputs->Beam2_Ypos = Doubles->GetBinContent(219);			// y-coordinate of location of particle gun [cm]. Default = 0.0
  Inputs->Beam2_Zpos = Doubles->GetBinContent(220);			// z-coordinate of location of particle gun [cm]. Default = -200.0
  Inputs->Beam2_Xrot_ASCII = ((TMath::Pi())/180.0)*Doubles->GetBinContent(232);		// Rotation of particle gun around X-axis [deg] (only for ASCII generator)
  Inputs->Beam2_Yrot_ASCII = ((TMath::Pi())/180.0)*Doubles->GetBinContent(233);		// Rotation of particle gun around Y-axis [deg] (only for ASCII generator)
  Inputs->Beam2_Zrot_ASCII = ((TMath::Pi())/180.0)*Doubles->GetBinContent(234);		// Rotation of particle gun around Z-axis [deg] (only for ASCII generator)
  
  // Third beam properties:
  Inputs->Beam3_Exe = Integers->GetBinContent(225);			// A second beam is only simulated if this bin is set equal to unity.
  Inputs->Beam3_PDG = Integers->GetBinContent(226);			// Particle ID code: particle species [int]. Default = 2112 (neutrons) NOTE: Check if the mass is added at the end of this file!
  Inputs->Beam3_Mass = Doubles->GetBinContent(241);			// Particle total mass [GeV].
  Inputs->Beam3_Nparticles = Integers->GetBinContent(227); 		// Number of primary particles per event [int]. Default = 1
  Inputs->Beam3_Emin = Doubles->GetBinContent(223);			// Lower Bound op particle total energy! [GeV]
  Inputs->Beam3_Emax = Doubles->GetBinContent(224);			// Upper Bound op particle total energy! [GeV]
  Inputs->Beam3_ThetaMin = Doubles->GetBinContent(225);			// Lower bound of angle with z-axis at which particles are emitted [deg.] Default = 0.0
  Inputs->Beam3_ThetaMax = Doubles->GetBinContent(226);	       	 	// Upper bound of angle with z-axis at which particles are emitted [deg.] Default = 4.584 (80 mrad)
  Inputs->Beam3_PhiMin = Doubles->GetBinContent(227);			// Lower bound of azimuthal angle at which particles are emitted [deg.] Default = 0.0
  Inputs->Beam3_PhiMax = Doubles->GetBinContent(228);			// Upper bound of azimuthal angle at which particles are emitted [deg.] Default = 360.0
  Inputs->Beam3_Xpos = Doubles->GetBinContent(229);			// x-coordinate of location of particle gun [cm]. Default = 0.0
  Inputs->Beam3_Ypos = Doubles->GetBinContent(230);			// y-coordinate of location of particle gun [cm]. Default = 0.0
  Inputs->Beam3_Zpos = Doubles->GetBinContent(231);			// z-coordinate of location of particle gun [cm]. Default = -200.0
  Inputs->Beam3_Xrot_ASCII = ((TMath::Pi())/180.0)*Doubles->GetBinContent(238);		// Rotation of particle gun around X-axis [deg] (only for ASCII generator)
  Inputs->Beam3_Yrot_ASCII = ((TMath::Pi())/180.0)*Doubles->GetBinContent(239);		// Rotation of particle gun around Y-axis [deg] (only for ASCII generator)
  Inputs->Beam3_Zrot_ASCII = ((TMath::Pi())/180.0)*Doubles->GetBinContent(240);		// Rotation of particle gun around Z-axis [deg] (only for ASCII generator)
  
  // Boosting-ASCII generator:
  Inputs->MotherBeamPDG = Integers->GetBinContent(232);                 // For Boosting ASCII-generator: PDG-code of the mother beam from Super-FRS.
  Inputs->Target_PDG = Integers->GetBinContent(233);                    // For Boosting ASCII-generator: PDG-code of the fixed target.
  Inputs->MotherBeamMass = Doubles->GetBinContent(244);                 // For Boosting ASCII-generator: Mass of the mother beam from Super-FRS.
  Inputs->Target_Mass = Doubles->GetBinContent(245);                    // For Boosting ASCII-generator: Mass of the fixed target.
  
  // Detectors that should be used:
  Inputs->Detectors_TARGET_Exists = Integers->GetBinContent(206); 	// 0 or 1: 1 allows to use the Target, 0 does not. Default = 1
  Inputs->Detectors_STaRTracker_Exists = Integers->GetBinContent(207); 	// 0 or 1: 1 allows to use StaRTracker, 0 does not. Default = 1
  Inputs->Detectors_PSP_Exists = Integers->GetBinContent(208); 		// 0 or 1: 1 allows to use PSP, 0 does not. Default = 1
  Inputs->Detectors_VacVessel_Exists = Integers->GetBinContent(209); 	// 0 or 1: 1 allows to use the Vacuum Vessel, 0 does not. Default = 1
  Inputs->Detectors_CALIFA_Exists = Integers->GetBinContent(210); 	// 0 or 1: 1 allows to use CALIFA, 0 does not. Default = 1
  Inputs->Detectors_mTOF_Exists = Integers->GetBinContent(211); 	// 0 or 1: 1 allows to use mTOF, 0 does not. Default = 1
  Inputs->Detectors_TOFwall_Exists = Integers->GetBinContent(212); 	// 0 or 1: 1 allows to use the TOF wall, 0 does not. Default = 1
  Inputs->Detectors_MFI_Exists = Integers->GetBinContent(213); 		// 0 or 1: 1 allows to use MFI, 0 does not. Default = 1
  Inputs->Detectors_DCH_Exists = Integers->GetBinContent(214); 		// 0 or 1: 1 allows to use DCH, 0 does not. Default = 1
  Inputs->Detectors_GLAD_Exists = Integers->GetBinContent(215); 	// 0 or 1: 1 allows to use GLAD, 0 does not. Default = 1
  Inputs->Detectors_NeuLAND_Exists = Integers->GetBinContent(216); 	// 0 or 1 or 2: 1 allows to use NeuLAND with land-class, 0 does not allow NeuLAND. 2 allows to use NeuLAND with neuland-class. Default = 1
  Inputs->Detectors_NEBULA_Exists = Integers->GetBinContent(230);       // 0 or 1: 1 allows to use NEBULA, 0 does not. Default = 0
  Inputs->Detectors_VETO_Exists = Integers->GetBinContent(217); 	// 0 or 1 or 2: 1 allows to use the VETO with land-class, 0 does not allow VETO. 2 allows to use VETO with neuland-class. Default = 1
  Inputs->Detectors_NebVETO_Exists = Integers->GetBinContent(231);      // 0 or 1: 1 allows to use NEBULA VETO, 0 does not. Default = 0
  Inputs->Detectors_VACchamber_Exists = Integers->GetBinContent(253);   // 0 or 1: 1 allows to use the Dummy Vacuum chamber behind GLAD. Default = 0.
  
  // Other simulation parameters:
  Inputs->nEvents = Integers->GetBinContent(201); 			// Number of events in a single run [int]. Default = 1
  Inputs->nEvents_NeutronCalibration = Integers->GetBinContent(240);	// Numebr of events used in the Neutron calibration simulations. Default = 10000
  Inputs->nTracks_NeutronCalibration = Integers->GetBinContent(244);    // Number of neutron tracks the calibration should search for.
  Inputs->pTracks_NeutronCalibration = Integers->GetBinContent(246);    // Number of charged tracks the calibration should search for.
  Inputs->pTracksPDG_NeutronCalibration = Integers->GetBinContent(251); // PDG code that we assume the charged tracks to be. Default = 2212 (protons). NOTE: all we know that the VETO fires yes/no. We do not know PDG, we should assume it!!!
  Inputs->pTracksMass_NeutronCalibration = Doubles->GetBinContent(246); // Mass of the charged tracks that we assume.
  Inputs->UseEnergyEstimate = Integers->GetBinContent(247);             // If this is 1, we will estimate proton energy from TOF. 0 means that we use original energy of Beam 2. Default = 0
  Inputs->Monte_Carlo_Engine = Monte_Carlo_Engine_Obj->GetString();	// Choice of Monte Carlo Engine [TString]
  Inputs->Event_Generator = Event_Generator_Obj->GetString();		// Choice of Event Generator [TString]
  Inputs->Output_Name = Output_Name_Obj->GetString();			// Choice of Output FileName Tag [TString]
  Inputs->VIS = Integers->GetBinContent(203); 				// 0 or 1: 1 allows the use of visualization, 0 does not. Default = 1
  Inputs->SEC = Integers->GetBinContent(204); 				// 0 or 1: 1 allows secondary interactions in Geant4, 0 does not. Default = 1
  Inputs->MAG = Integers->GetBinContent(205); 				// 0 or 1: 1 switches on the magnetic field of GLAD, 0 switches it off. Default = 1
  Inputs->MAGcurrent = Doubles->GetBinContent(201);			// Magnetic field current [A]. Default = 4000.0
  Inputs->FieldMap = Integers->GetBinContent(224);			// 1=GLAD field, 0 or anything else is ALADIN field. Default = 1
  Inputs->ThisCounter = Integers->GetBinContent(1);			// Simulation counter (not very relevant, since it is updated all the time).
  Inputs->CalibrationID = Integers->GetBinContent(241);			// 0=ordinary sim, 1=calibration run.
  Inputs->Geant3Test = Integers->GetBinContent(249);                    // 1 = Check for Geant3 for digitizer etc. 0 = no checks. Default = 1.
  Inputs->ASCIIchoice = Integers->GetBinContent(248);                   // NOTE: ASCII-generator only. Choice of how to generate the ASCII-file.
  Inputs->Tracker_Exe = Integers->GetBinContent(242);			// 0 = no tracker, 1 = tracker with manual calibration, 2 = tarcker with autocalibration.
  Inputs->G4_PhysicsList = Integers->GetBinContent(243);		// Selects Geant4 physics list. 0 = default.
  Inputs->Calibr_Minimization = Integers->GetBinContent(245);           // Selection of Minimization procedure for teh Calibration Cuts.
  Inputs->ClusterSpaceRadius = Doubles->GetBinContent(242);             // Search Radius for the NeuLAND Clusterfinder in the spatial dimensions x, y, z [cm]. Default = 7.5
  Inputs->ClusterTimeRadius = Doubles->GetBinContent(243);              // Search Radius for the NeuLAND Clusterfinder in the time dimension t [ns]. Default = 1.0
  Inputs->STaRTrackMode = Integers->GetBinContent(250);                 // STaRTrack Track reconstruction mode. 0 = elastic scattering, 1 = knockout reaction.
  Inputs->CaveMedium = Integers->GetBinContent(252);                    // If this bin is equal to unity, the cave will be vacuum. Default = 0 (Air).
  
  // Batch mode parameters:
  Inputs->VETO_stepsize = Doubles->GetBinContent(133);			// VETO step size to reduce the z-coordinate of the VETO position [cm]. Default = 12.5
  Inputs->VETO_thickstep = Doubles->GetBinContent(134);			// VETO step size to increase the thickness of a single scintillator bar of the VETO [cm]. Default = 0.1
  Inputs->Simulation_Repetition = Integers->GetBinContent(220);		// Total number of runs the simulation will be repeated aftere adapting a parameter. 4 means 4 runs in total [int].
  
  // Detector properties:  
  Inputs->Neu_Ethreshold = Doubles->GetBinContent(301);			// NeuLAND energy threshold [keV] Default = 160 NOTE: Also used by digitizer!!!
  Inputs->Veto_Ethreshold = Doubles->GetBinContent(302);		// VETO energy threshold [keV] Default = 160 NOTE: Also used by digitizer!!!
  Inputs->Neu_TimeResolution = Doubles->GetBinContent(309);             // NeuLAND Time resolution [ps]. Default = 150.0 NOTE: Also used by digitizer!!!
  Inputs->VETO_TimeResolution = Doubles->GetBinContent(310);            // VETO Time resolution [ps]. Default = 300.0 NOTE: Also used by digitizer!!!
  Inputs->cEff_VETOscint = Doubles->GetBinContent(334);			// Effective speed of light in VETO scintillator material [cm/ns]. Default = 14.0 NOTE: Also used by digitizer!!!
  Inputs->ExpAttenuation = Doubles->GetBinContent(335);			// light transport attenuation factor [1/cm] for transport to PMTs. Default = 0.008 NOTE: Also used by digitizer!!!
  Inputs->ResolveBoundary = Doubles->GetBinContent(330);                // Resolving Boundary [cm]. if x^2+y^2+z^2+(ct)^2<bounary^2 between MC and Reconstructed prim. interaction, we call the MC point Resolved.
  
  // Primary Volume:
  Inputs->PrimIDplane_Zfront = Doubles->GetBinContent(303);		// z-location of front XY-plane for identification of primary particles [cm] Default = 600.0
  Inputs->PrimIDplane_Zback = Doubles->GetBinContent(304);		// z-location of back XY-plane for identification of primary particles [cm] Default = 1700.0
  Inputs->PrimIDplane_Ylow = Doubles->GetBinContent(305);		// y-location of lower XZ-plane for identification of primary particles [cm] Default = -150.0
  Inputs->PrimIDplane_Yup = Doubles->GetBinContent(306);		// y-location of upper XZ-plane for identification of primary particles [cm] Default = 150.0
  Inputs->PrimIDplane_Xtof = Doubles->GetBinContent(307);		// x-location of YZ-plane at TOF side for identification of primary particles [cm] Default = -150.0
  Inputs->PrimIDplane_Xother = Doubles->GetBinContent(308);		// x-location of YZ-plane at other side for identification of primary particles [cm] Default = 150.0
  Inputs->AllowedTargetRadius = Doubles->GetBinContent(341);            // Maximum radius a particle can be produced from the target. [cm]. Default = 5.0
  
  // Analysis settings:
  Inputs->Bug_Report = Integers->GetBinContent(304);			// Decides which bug reports are printed on the screen. 0=no bug reports.
  Inputs->PrimParticles_Exe = Integers->GetBinContent(305);		// If this bin is set to unity, the identification of primary particles is performed. Otherwise not.
  Inputs->TrackEvents_Exe = Integers->GetBinContent(306);		// If this bin is set to unity, the identification of primary interaction VS backscattering is performed. Otherwise not.
  Inputs->QTDC_Exe = Integers->GetBinContent(307);			// If this bin is set to unity, the computation of Edep and TOF per bar is performed. Otherwise not.
  Inputs->Secondaries_Exe = Integers->GetBinContent(308);		// If this bin is set to unity, the analysis on secondary particles is performed. Otherwise not.
  Inputs->Cluster_Exe = Integers->GetBinContent(314);   		// If this bin is set to unity, the Clusters of NeuLAND are processed, Otherwise not. The Digitizer should exist for this. Default = 1
  Inputs->AnaTracker_Exe = Integers->GetBinContent(313);	        // If this bin is set to unity, the NeutronTracker is processed, Otherwise not. The tracker should exist for this! Default = 1
  Inputs->VETO_select = Integers->GetBinContent(309);                   // Selects the type of VETO condition.
  Inputs->Store_AllBars = Integers->GetBinContent(310);                 // If this bin is set to unity, all bars of NeuLAND are stored whether VETOed or not.
  Inputs->UseDigitizer = Integers->GetBinContent(312);                  // If this bin is set to unity, the Digitizer is used when possible, else we use LandPoints. NOTE that the digitizer should exist in order to use it!
  
  // Secondary particle model:
  Inputs->Z_mean_gamma = Doubles->GetBinContent(314);			// Mean travel distance for gamma particles according to basic law of radiation [cm].
  Inputs->Z_mean_neutron = Doubles->GetBinContent(315);	   		// Mean travel distance for neutrons according to basic law of radiation [cm].
  Inputs->Bethe_Dx_Protons = Doubles->GetBinContent(316);		// Bethe-Bloch maximum step size for Protons [mm].
  Inputs->Bethe_Dx_Electrons = Doubles->GetBinContent(317);		// Bethe-Bloch maximum step size for Electrons [mm].
  Inputs->Bethe_Dx_Pions = Doubles->GetBinContent(318);			// Bethe-Bloch maximum step size for Pions [mm].
  Inputs->Bethe_Dx_Muons = Doubles->GetBinContent(319);			// Bethe-Bloch maximum step size for Muons [mm].
  Inputs->Bethe_Dx_Alphas = Doubles->GetBinContent(320);		// Bethe-Bloch maximum step size for Alphas [mm].
  Inputs->Bethe_Dx_Cions = Doubles->GetBinContent(321);			// Bethe-Bloch maximum step size for C-ions [mm].
  Inputs->Nbins_Esec = Integers->GetBinContent(301);			// Number of bins in energy direction for secondary particle histograms [int] Default = 100
  Inputs->Nbins_Zsec = Integers->GetBinContent(302);			// Number of bins in z direction for secondary particle histograms: [int] Default = 100
  Inputs->Nbins_Dsec = Integers->GetBinContent(303);			// Number of bins in travel distance direction for secondary particle histograms: [int] Default = 100
  Inputs->ZsecMin = Doubles->GetBinContent(311);			// Starting z-coordinate of secondary particle histogram [cm]. Default = 1200.0
  Inputs->ZsecMax = Doubles->GetBinContent(312);			// Ending z-coordinate of secondary particle histogram [cm]. Defaul = 1700.0
  Inputs->DistMax = Doubles->GetBinContent(313);			// Maximal travel distance in secondary particle histogram [cm]. Default = 500.0
  Inputs->Ecutoff = Doubles->GetBinContent(322);			// Low energy cuttoff: particles below this energy will NEVER reach the VETO, we cut them out manually. [MeV] Default = 0.03
  
  // VETO condition settings:
  Inputs->Shower_Radius = Doubles->GetBinContent(323);                  // Shower radius [cm]. All bars of NeuLAND insie this radius around a hit in the VETO are VETOed.
  Inputs->VETO_Ecut = Doubles->GetBinContent(324);                      // Particles with a reconstructed energy above this cut are VETOed. [MeV]
  Inputs->TimeCut = Doubles->GetBinContent(342);                        // Vertices with a TOF above this cut are VETOed by definition in the combined VETO condition. [ns]. Default = 200.0
  Inputs->EffectiveC = Doubles->GetBinContent(336);                     // Effective speed of light that is used when matching points in processing the tracker and using the tracker VETO condition.
  Inputs->VETO_Zdir = Doubles->GetBinContent(325);                      // Z-component of Direction to VETO bars in segmented VETO condition. [dim. less] It only specifies direction.
  Inputs->VETO_Xdir = Doubles->GetBinContent(326);                      // X-component of Direction to VETO bars in segmented VETO condition. [dim. less] It only specifies direction.
  Inputs->VETO_Ydir = Doubles->GetBinContent(327);                      // Y-component of Direction to VETO bars in segmented VETO condition. [dim. less] It only specifies direction.
  Inputs->NeuBegin_X = Doubles->GetBinContent(331);                     // X-coordinate of where the neutrons came from (Tracker VETO condition). Default = 0.0
  Inputs->NeuBegin_Y = Doubles->GetBinContent(332);                     // Y-coordinate of where the neutrons came from (Tracker VETO condition). Default = 0.0
  Inputs->NeuBegin_Z = Doubles->GetBinContent(333);                     // Z-coordinate of where the neutrons came from (Tracker VETO condition). Default = 0.0
  Inputs->NeuProtonBegin_X = Doubles->GetBinContent(337);               // X-coordinate of where the PROTONS came from (Tracker VETO condition). Default = 0.0
  Inputs->NeuProtonBegin_Y = Doubles->GetBinContent(338);               // Y-coordinate of where the PROTONS came from (Tracker VETO condition). Default = 0.0
  Inputs->NeuProtonBegin_Z = Doubles->GetBinContent(339);               // Z-coordinate of where the PROTONS came from (Tracker VETO condition). Default = 0.0
  
  // Analysis series inputs.
  Inputs->N_Analysis = Integers->GetBinContent(311);                    // Amount of steps the analysis should be repeated. When this bin is 4, we do 4 analyses. 
                                                                        // The first time with parameters as above and then the next time with one parameter increased/decreased, etc. 
                                                                        // Which parameter depends on the argument that you give to ./analyse.sh
  Inputs->Showerstep = Doubles->GetBinContent(328);                     // Shower radius increase per step [cm].
  Inputs->EcutStep = Doubles->GetBinContent(329);			// Increase in reconstructed energy cut [MeV]. Default = 10.0
  Inputs->TimeCutStep = Doubles->GetBinContent(343);                    // TimeCut increase per step [ns]. Default = 5.0
  Inputs->NeuPstart_Step = Doubles->GetBinContent(340); 		// Decrease in X-coordinate of the protons starting point in the Tracker VETO condition [cm]. Default = 20.0;
   
  // Graphics parameters:
  Inputs->ScaleFactor = Doubles->GetBinContent(401);                    // Scale factor to paint the pictures.
  Inputs->TOFchoice = Integers->GetBinContent(401);			// selects how to compute total TOF.
  Inputs->Projection = Integers->GetBinContent(402);			// Selects the type of projection for histograms of secondary particles. 1 plots Energy VS z-coordinate, 2 plots Energy VS traveldistance. Anything else is not accepted.
  Inputs->Nbins_Eres = Integers->GetBinContent(403);			// Number of bins in the histograms for reconstructed energy.
  Inputs->Nbins_KenMiki = Integers->GetBinContent(404);			// Number of bins in Ken Miki histograms.
  Inputs->KenMiki_Aim = Integers->GetBinContent(405);			// if this is unity, only tracks aimed at the VETO are taken along.
  Inputs->PDG_KenMiki = Integers->GetBinContent(406);			// PDG code that we select on in the Ken Miki plots.
  Inputs->NeuContent_Plane = Integers->GetBinContent(407);		// Selects the plane number for the NeuLAND content plots (also from Ken Miki). 0 = entire NeuLAND
  Inputs->Nbins_ETOF = Integers->GetBinContent(408);			// Number of bins in the TOFvsEdep plots. [int] Default = 200
  Inputs->ETOF_Emax = Doubles->GetBinContent(402);			// Emax in TOFvsEdep plots. [MeV] Default = 100.0
  Inputs->ETOF_Tmin = Doubles->GetBinContent(403);			// TOF min. value in TOFvsEdep plots. [ns] Default = -50.0;
  Inputs->ETOF_Tmax = Doubles->GetBinContent(404);			// TOF max. value in TOFvsEdep plots. [ns] Default = 200.0;
  Inputs->Eres_full = Integers->GetBinContent(409);			// If this bin equals 1 then the full energy of the particle is plotted instead of the reconstructed energy. Default = 0
  Inputs->Eres_Emax = Doubles->GetBinContent(405);		        // Emax in reconstructed Energy plots [MeV]. Default = 1000.0
  Inputs->CalNvert = Integers->GetBinContent(410);			// Vertical number of plots in the neutron calibration pictures.
  Inputs->CalNhor = Integers->GetBinContent(411);			// Horizontal number of plots in the neutron calibration pictures.
  Inputs->NneutronsGood = Integers->GetBinContent(412);                 // Number of neutrons that you want to detect. Is used in counting good events. Default = 4.
  
  // NOTE: The inputs about which detectors exist are not required here. Those inputs are processed to boolians
  //       during the connection process of the Leaf_Structure. Hence they are redundant here.
  
  // Now we are in principle done. But if the integer Series is different from zero; then 
  // we have to update the zome of the parameters:
  
  // Normalize the VETO_X,Y,Zdir to a Z-value of unity:
  if (Inputs->VETO_Zdir==0.0) {Inputs->VETO_Zdir = 1e-10;} 
  Inputs->VETO_Xdir = Inputs->VETO_Xdir/Inputs->VETO_Zdir;
  Inputs->VETO_Ydir = Inputs->VETO_Ydir/Inputs->VETO_Zdir;
  Inputs->VETO_Zdir = 1.0;
  
  if (Series==1)
  {
    // Analyze for different positions:
    Inputs->VETO_geo_Zpos = Inputs->VETO_geo_Zpos - (Inputs->VETO_stepsize)*((Int_t) (Counter - 1));
  }
  if (Series==2)
  {
    // Analyze for different shower radii
    Inputs->Shower_Radius = Inputs->Shower_Radius + (Inputs->Showerstep)*((Int_t) (Counter - 1));
  }
  if (Series==3)
  {
    // analyze for different number of paddles:
    Double_t increase = TMath::Power(2.0,((Int_t) (Counter - 1)));
    Int_t increase_int = (Double_t) increase;
    Int_t bars = Inputs->VETO_geo_NbarsPerPlane;
    Int_t total = increase_int*bars;
    Inputs->VETO_geo_NbarsPerPlane = total;
    Inputs->VETO_geo_TotalBarWidth = Inputs->VETO_geo_TotalBarWidth*TMath::Power(0.5,((Int_t) (Counter - 1)));
  }
  if (Series==4)
  {
    Inputs->VETO_geo_TotalBarThickness = Inputs->VETO_geo_TotalBarThickness + (Inputs->VETO_thickstep)*((Int_t) (Counter - 1));
  }
  if (Series==5)
  {
    Inputs->VETO_Ecut = Inputs->VETO_Ecut + (Inputs->EcutStep)*((Int_t) (Counter - 1));
  }
  if (Series==6)
  {
    Inputs->NeuProtonBegin_X = Inputs->NeuProtonBegin_X - (Inputs->NeuPstart_Step)*((Int_t) (Counter - 1));
  }
  if (Series==7)
  {
    Inputs->TimeCut = Inputs->TimeCut + (Inputs->TimeCutStep)*((Int_t) (Counter - 1));
  }
  if (Series==8)
  {
    Inputs->VETO_select = Counter - 1;
  }
  
  // ... add more options if you like... (NOTE: also add them to the Tree_Definition.h!)
  
  // =================================================================================================
  // We will now also load all results of the Bethe-Bloch calcuulations into the Inputs-structure.
  // It is actually not an input, but we need it here so that we have access to it during the analysis.
  // We do this unless Series<0:
  
  Bool_t test = kTRUE;
  if ((!(Series<0))&&(Inputs->Secondaries_Exe==1))
  {
  // We begin with opening up another file:
  FileStat_t stom2;
  Int_t inputs_test2 = gSystem->GetPathInfo("../../Data/Analysis/Bethe.root",stom2);
   
  if (inputs_test2==1)
  {
    cout << "### ERROR: The Bethe.root-file does not exist! Re-Check your code!\n\n";
    test = kFALSE;
  }
  else
  {
   
  // Connect to the Inputs.root file:
  TFile* Bethe = new TFile("../../Data/Analysis/Bethe.root","read");
  cout << "Connection to the file Bethe.root was succesful.\n\n";
  
  // Now charge the histograms into the pointers:
  Inputs->Protons_TravelDist = (TH1D*) Bethe->Get("Protons_TravelDist");
  Inputs->Electrons_TravelDist = (TH1D*) Bethe->Get("Electrons_TravelDist");
  Inputs->Pions_TravelDist = (TH1D*) Bethe->Get("Pions_TravelDist");
  Inputs->Muons_TravelDist = (TH1D*) Bethe->Get("Muons_TravelDist");
  Inputs->Alphas_TravelDist = (TH1D*) Bethe->Get("Alphas_TravelDist");
  Inputs->Cions_TravelDist = (TH1D*) Bethe->Get("Cions_TravelDist");
  
  // Check if it worked:
  test = kTRUE;
  if (Inputs->Protons_TravelDist==0) {test = kFALSE;}
  if (Inputs->Electrons_TravelDist==0) {test = kFALSE;}
  if (Inputs->Pions_TravelDist==0) {test = kFALSE;}
  if (Inputs->Muons_TravelDist==0) {test = kFALSE;}
  if (Inputs->Alphas_TravelDist==0) {test = kFALSE;}
  if (Inputs->Cions_TravelDist==0) {test = kFALSE;}
  
  if (test)
  {
    cout << "All Bethe-Bloch histograms are loaded.\n\n";
  }
  else
  {
    cout << "### ERROR: Some of the Bethe-Bloch histograms failed to load! Re-check your code!\n\n";
  }
  }
  }
  else
  {
  test = kTRUE;
  }
  
  // =================================================================================================
  // At this point everything is OK! We cannot check whether all individual numbers have been
  // loaded succesfully, since they are no pointers that can be checked to be NULL yes/no.
  // Hence we just assume that if the histograms and TObjString* exist, everythings works OK.
  // Hence all that is left to do is to set the Loading Boolian. But we can check for the above
  // histograms:
  if (test) {Loading = kTRUE;}
  cout << "All inputs are loaded succesfully.\n\n";
 
   // ================================================================================================
   // Done! Now we just close all remaining blocks:
   }
   }
   }
   
   // And finally, return the loading boolian:
   return Loading;
   
}
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   