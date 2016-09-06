#include <fstream>
#include "Masses.h"

void SetParameters()
{
  // This script contains all the input parameters for the VETO simulations.
  // All Double_t and Int_t inputs are stored in a histogram. These histograms
  // together with all other possible inputs will be stored in a file called
  // Inputs.root located in /VetoSim/Data/Inputs/
  
  // The function has no inputs or outputs. This is because outputs are 
  // written to the file and the purpose of this function is to specify
  // inputs, not accepting them.
  
  // Wriiten by C. A. Douma, for R3B collaboration
  
  // ==========================================================================
  // Creation of the structures for saving:
  
  TH1D* Doubles = new TH1D("Doubles","Doubles",500,0.0,1.0);
  TH1D* Integers = new TH1D("Integers","Integers",500,0.0,1.0);

  // ==========================================================================
  // Specification of the input parameters:
  // NOTE: The user is only supposed to make changes to this section!!!
  
  // --------------------------------------------------------------------------
  // Geometry parameters:
  // ----------------------------------- CAVE:
  Integers->SetBinContent(252,0);               // If this bin is equal to unity, the cave will be vacuum. Default = 0 (Air).
  
  // ----------------------------------- PSP:
  Doubles->SetBinContent(81,0.0); 		// PSP detector 1 global x position [cm]. Default:    0.0
  Doubles->SetBinContent(82,0.0);		// PSP detector 1 global y position [cm]. Default:    0.0
  Doubles->SetBinContent(83,-221.0);		// PSP detector 1 global z position [cm]. Default: -221.0
  Doubles->SetBinContent(84,0.0);		// PSP detector 1 rotation around x-axis [deg.] Default:  0.0
  Doubles->SetBinContent(85,0.0);		// PSP detector 1 rotation around y-axis [deg.] Default:  0.0
  Doubles->SetBinContent(86,0.0);		// PSP detector 1 rotation around z-axis [deg.] Default:  0.0
  
  Doubles->SetBinContent(87,0.0); 		// PSP detector 2 global x position [cm]. Default:    0.0
  Doubles->SetBinContent(88,0.0);		// PSP detector 2 global y position [cm]. Default:    0.0
  Doubles->SetBinContent(89,-89.0);		// PSP detector 2 global z position [cm]. Default:  -89.0
  Doubles->SetBinContent(90,0.0);		// PSP detector 2 rotation around x-axis [deg.] Default:  0.0
  Doubles->SetBinContent(91,0.0);		// PSP detector 2 rotation around y-axis [deg.] Default:  0.0
  Doubles->SetBinContent(92,0.0);		// PSP detector 2 rotation around z-axis [deg.] Default:  0.0
  
  Doubles->SetBinContent(93,0.0); 		// PSP detector 3 global x position [cm]. Default:    0.0
  Doubles->SetBinContent(94,0.0);		// PSP detector 3 global y position [cm]. Default:    0.0
  Doubles->SetBinContent(95,94.1);		// PSP detector 3 global z position [cm]. Default:   94.1
  Doubles->SetBinContent(96,0.0);		// PSP detector 3 rotation around x-axis [deg.] Default:  0.0
  Doubles->SetBinContent(97,0.0);		// PSP detector 3 rotation around y-axis [deg.] Default:  0.0
  Doubles->SetBinContent(98,0.0);		// PSP detector 3 rotation around z-axis [deg.] Default:  0.0
  
  // ----------------------------------- TARGET:
  TObjString* Target_Material = new TObjString("LiH"); // Target Target_Material choice. Replace the string with one of the following options;
						           // "LeadTarget" "Para" "Para45" "LiH" "CTarget" "CH2Target" "ETTarget" "Sn"(CTarget, but content is Sn, not C)
  
  Doubles->SetBinContent(21,0.0); 		// Target global x position [cm]. Default:  0.0
  Doubles->SetBinContent(22,200.0);		// Target global y position [cm]. Default:  0.0
  Doubles->SetBinContent(23,0.0);		// Target global z position [cm]. Default:  0.0
  
  Doubles->SetBinContent(24,90.0);		// Target global Theta rotation with polar x-axis [deg.] Default:  90.0
  Doubles->SetBinContent(25,90.0);		// Target global Theta rotation with polar y-axis [deg.] Default:  90.0
  Doubles->SetBinContent(26,0.0);		// Target global Theta rotation with polar z-axis [deg.] Default:   0.0
  
  Doubles->SetBinContent(27,0.0);		// Target global Phi rotation with polar x-axis [deg.] Default:   0.0
  Doubles->SetBinContent(28,90.0);		// Target global Phi rotation with polar y-axis [deg.] Default:  90.0
  Doubles->SetBinContent(29,0.0);		// Target global Phi rotation with polar z-axis [deg.] Default:   0.0 
  
  // ----------------------------------- GLAD:
  Doubles->SetBinContent(101,0.0); 		// GLAD global x position [cm]. Default:   0.0
  Doubles->SetBinContent(102,0.0);		// GLAD global y position [cm]. Default:   0.0
  Doubles->SetBinContent(103,0.0);		// GLAD global z position [cm]. Default:   0.0
  
  Doubles->SetBinContent(104,0.0);		// GLAD rotation around x-axis [deg.] Default:   0.0
  Doubles->SetBinContent(105,90.0);		// GLAD rotation around y-axis [deg.] Default:  90.0
  Doubles->SetBinContent(106,-90.0);		// GLAD rotation around z-axis [deg.] Default: -90.0
  
  // ----------------------------------- CALIFA:
  Doubles->SetBinContent(11,0.0); 		// CALIFA global x position [cm]. Default:  0.0
  Doubles->SetBinContent(12,0.0);		// CALIFA global y position [cm]. Default:  0.0
  Doubles->SetBinContent(13,0.0);		// CALIFA global z position [cm]. Default:  0.0
  
  Doubles->SetBinContent(14,0.0);		// CALIFA rotation around x-axis [deg.] Default:  0.0
  Doubles->SetBinContent(15,0.0);		// CALIFA rotation around y-axis [deg.] Default:  0.0
  Doubles->SetBinContent(16,0.0);		// CALIFA rotation around z-axis [deg.] Default:  0.0
  
  // ----------------------------------- TOF wall:
  Doubles->SetBinContent(41,-417.359574); 	// TOF wall global x position [cm]. Default: -417.359574
  Doubles->SetBinContent(42,2.4);		// TOF wall global y position [cm]. Default:    2.4
  Doubles->SetBinContent(43,960.777114);	// TOF wall global z position [cm]. Default:  960.777114
  
  Doubles->SetBinContent(44,0.0);		// TOF wall rotation around x-axis [deg.] Default:   0.0
  Doubles->SetBinContent(45,-31.0);		// TOF wall rotation around y-axis [deg.] Default: -31.0
  Doubles->SetBinContent(46,0.0);		// TOF wall rotation around z-axis [deg.] Default:   0.0
  
  // ----------------------------------- mTOF:
  Doubles->SetBinContent(1,-155.824045); 	// mTOF global x position [cm]. Default: -155.824045
  Doubles->SetBinContent(2,0.523976);		// mTOF global y position [cm]. Default:    0.523976
  Doubles->SetBinContent(3,761.870345);		// mTOF global z position [cm]. Default:  761.870345
  
  Doubles->SetBinContent(4,-106.7);		// mTOF global Theta rotation with polar x-axis [deg.] Default: -106.7
  Doubles->SetBinContent(5,90.0);		// mTOF global Theta rotation with polar y-axis [deg.] Default:   90.0
  Doubles->SetBinContent(6,-16.7);		// mTOF global Theta rotation with polar z-axis [deg.] Default:  -16.7
  
  Doubles->SetBinContent(7,0.0);		// mTOF global Phi rotation with polar x-axis [deg.] Default:   0.0
  Doubles->SetBinContent(8,90.0);		// mTOF global Phi rotation with polar y-axis [deg.] Default:  90.0
  Doubles->SetBinContent(9,0.0);		// mTOF global Phi rotation with polar z-axis [deg.] Default:   0.0 
			 
  // ----------------------------------- DCH:
  Doubles->SetBinContent(31,0.0); 		// DCH global x position [cm]. Default:  0.0
  Doubles->SetBinContent(32,0.0);		// DCH global y position [cm]. Default:  0.0
  Doubles->SetBinContent(33,0.0);		// DCH global z position [cm]. Default:  0.0
  
  Doubles->SetBinContent(34,0.0);		// DCH rotation around x-axis [deg.] Default:  0.0
  Doubles->SetBinContent(35,0.0);		// DCH rotation around y-axis [deg.] Default:  0.0
  Doubles->SetBinContent(36,0.0);		// DCH rotation around z-axis [deg.] Default:  0.0
  
  // ------------------------------------ StaRTrack:
  Doubles->SetBinContent(51,0.0); 		// StaRTrack global x position [cm]. Default:  0.0
  Doubles->SetBinContent(52,0.0);		// StaRTrack global y position [cm]. Default:  0.0
  Doubles->SetBinContent(53,0.0);		// StaRTrack global z position [cm]. Default:  0.0
  
  Doubles->SetBinContent(54,0.0);		// StaRTrack rotation around x-axis [deg.] Default:  0.0
  Doubles->SetBinContent(55,0.0);		// StaRTrack rotation around y-axis [deg.] Default:  0.0
  Doubles->SetBinContent(56,0.0);		// StaRTrack rotation around z-axis [deg.] Default:  0.0
  
  // ------------------------------------ Vacuum Vessel around scattering chamber:
  Doubles->SetBinContent(61,0.0); 		// Vacuum Vessel global x position [cm]. Default:  0.0
  Doubles->SetBinContent(62,0.0);		// Vacuum Vessel global y position [cm]. Default:  0.0
  Doubles->SetBinContent(63,0.0);		// Vacuum Vessel global z position [cm]. Default:  0.0
  
  Doubles->SetBinContent(64,0.0);		// Vacuum Vessel rotation around x-axis [deg.] Default:  0.0
  Doubles->SetBinContent(65,0.0);		// Vacuum Vessel rotation around y-axis [deg.] Default:  0.0
  Doubles->SetBinContent(66,0.0);		// Vacuum Vessel rotation around z-axis [deg.] Default:  0.0
  
  // ------------------------------------- MFI:
  Doubles->SetBinContent(71,-63.82); 		// MFI global x position [cm]. Default:  -63.82
  Doubles->SetBinContent(72,0.0);		// MFI global y position [cm]. Default:    0.0
  Doubles->SetBinContent(73,520.25);		// MFI global z position [cm]. Default:  520.25
  
  Doubles->SetBinContent(74,0.0);		// MFI rotation around x-axis [deg.] Default:   0.0
  Doubles->SetBinContent(75,-13.5);		// MFI rotation around y-axis [deg.] Default: -13.5
  Doubles->SetBinContent(76,0.0);		// MFI rotation around z-axis [deg.] Default:   0.0
    
  // ------------------------------------- Vacuum Chamber:
  Doubles->SetBinContent(151,0.0);              // Vacuum Chamber global x position [cm]. Default:  0.0
  Doubles->SetBinContent(152,0.0);              // Vacuum Chamber global y position [cm]. Default:  0.0
  Doubles->SetBinContent(153,0.0);              // Vacuum Chamber global z position [cm]. Default:  0.0

  Doubles->SetBinContent(154,0.0);              // Vacuum Chamber rotation around x-axis [deg.] Default:  0.0
  Doubles->SetBinContent(155,0.0);              // Vacuum Chamber rotation around y-axis [deg.] Default:  0.0
  Doubles->SetBinContent(156,0.0);              // Vacuum Chamber rotation around z-axis [deg.] Default:  0.0
  
  // ------------------------------------- NEBULA:
  Doubles->SetBinContent(141,0.0); 		// NEBULA global x position [cm]. Default:  0.0
  Doubles->SetBinContent(142,0.0);		// NEBULA global y position [cm]. Default:  0.0
  Doubles->SetBinContent(143,1800.0);		// NEBULA global z position [cm]. NOTE: This is front-end of detector! Default:  1800.0
  
  Doubles->SetBinContent(144,0.0);		// NEBULA rotation around x-axis [deg.] Default:  0.0
  Doubles->SetBinContent(145,0.0);		// NEBULA rotation around y-axis [deg.] Default:  0.0
  Doubles->SetBinContent(146,0.0);		// NEBULA rotation around z-axis [deg.] Default:  0.0
  
  Doubles->SetBinContent(147,80.0);		// NEBULA Distance between the parts [cm]. Default:  80.0
  
  Integers->SetBinContent(5,4);		        // NEBULA number of parts. Default = 2.
  
  // NOTE: NEBULA will be added to LandPoints at higher fSector. The NEBULA veto is added also to LandPoints at higher indices then ordinary NEBULA.
  
  // ------------------------------------- NEBULA VETO:
  Doubles->SetBinContent(148,25.0);             // NEBULA Distance between VETO and NEBULA (within one part) [cm]. Default = 25.0
  
  // ------------------------------------- NeuLAND:
  Doubles->SetBinContent(111,0.0); 		// NeuLAND global x position [cm]. Default:  0.0
  Doubles->SetBinContent(112,0.0);		// NeuLAND global y position [cm]. Default:  0.0
  Doubles->SetBinContent(113,1550.0);		// NeuLAND global z position [cm]. Default:  1550.0
  
  Doubles->SetBinContent(114,0.0);		// NeuLAND rotation around x-axis [deg.] Default:  0.0
  Doubles->SetBinContent(115,0.0);		// NeuLAND rotation around y-axis [deg.] Default:  0.0
  Doubles->SetBinContent(116,0.0);		// NeuLAND rotation around z-axis [deg.] Default:  0.0
  
  Integers->SetBinContent(2,60);		// NeuLAND number of Single Planes you want to use [int]. Default = 60
  
  // ------------------------------------- VETO:
  Doubles->SetBinContent(121,0.0);		// VETO global x position [cm]. Default:     0.0
  Doubles->SetBinContent(122,0.0);		// VETO global y position [cm]. Default:     0.0
  Doubles->SetBinContent(123,1399.4);		// VETO global z position [cm]. Default:  1200.0
  
  Doubles->SetBinContent(124,0.0);		// VETO rotation around x-axis [deg.] Default: 0.0
  Doubles->SetBinContent(125,0.0);		// VETO rotation around y-axis [deg.] Default: 0.0
  Doubles->SetBinContent(126,90.0);		// VETO rotation around z-axis [deg.] Default: 0.0
  
  Doubles->SetBinContent(127,250.0);		// VETO Scintillator bar length [cm].      Default:  250.0 (NeuLAND)
  Doubles->SetBinContent(128,250.0/8.0);	// VETO Scintillator bar width [cm].       Default:   15.6 (NeuLAND)
  Doubles->SetBinContent(129,1.2);		// VETO Scintillator bar thickness [cm].   Default:    1.2 (NeuLAND)
  Doubles->SetBinContent(130,0.06);		// VETO TOTAL gab size between bars [cm].  Default:   0.06 (NeuLAND)
  Doubles->SetBinContent(131,0.02);		// VETO thickness of reflection foil [cm]. Default:   0.02 (NeuLAND)
  Doubles->SetBinContent(132,0.05);		// VETO thickness of wrapping tape [cm].   Default:   0.05 (NeuLAND)
  
  Integers->SetBinContent(3,1); 		// VETO Number of single planes you want to use [int].     Default = 1
  Integers->SetBinContent(4,8); 		// VETO Number of bars within a single plane [int].        Default = 16
  
  // -------------------------------------------------------------------------
  // Simulation parameters:
  // NOTE: do not place the particle guns inside a detector!
  // NOTE: Ion PDG codes can only be used for ASCII event generator!
  // NOTE: For Ion PDG, Emin and Emax are Kinetic energy per nucleon [GeV]. For particle PDG, they are total kinetic energy!
  
  // --------------------------- First Beam properties: NOTE: neutrons should be shot with this beam!
  Integers->SetBinContent(218,2212);     	// Particle ID code: particle species [int]. Default = 2112 (neutrons) NOTE: Check if the mass is added at the end of this file!
  Integers->SetBinContent(219,1); 		// Number of primary particles per event [int]. Default = 1
  Doubles->SetBinContent(202,0.999);		// Lower Bound op particle total energy! [GeV] Default = 0.0
  Doubles->SetBinContent(203,1.001);		// Upper Bound op particle total energy! [GeV] Default = 1.0
  Doubles->SetBinContent(204,0.0);		// Lower bound of angle with z-axis at which particles are emitted [deg.] Default = 0.0
  Doubles->SetBinContent(205,4.584);	        // Upper bound of angle with z-axis at which particles are emitted [deg.] Default = 4.584 (80 mrad)
  Doubles->SetBinContent(206,0.0);		// Lower bound of azimuthal angle at which particles are emitted [deg.] Default = 0.0
  Doubles->SetBinContent(207,360.0);		// Upper bound of azimuthal angle at which particles are emitted [deg.] Default = 360.0
  Doubles->SetBinContent(208,0.0);		// x-coordinate of location of particle gun [cm]. Default = 0.0
  Doubles->SetBinContent(209,0.0);		// y-coordinate of location of particle gun [cm]. Default = 0.0
  Doubles->SetBinContent(210,0.0);		// z-coordinate of location of particle gun [cm]. Default = -200.0
  Doubles->SetBinContent(232,0.0);		// Rotation of particle gun around X-axis [deg] (only for ASCII generator)
  Doubles->SetBinContent(233,0.0);		// Rotation of particle gun around Y-axis [deg] (only for ASCII generator)
  Doubles->SetBinContent(234,0.0);		// Rotation of particle gun around Z-axis [deg] (only for ASCII generator)
  
  // --------------------------- Second Beam properties:
  Integers->SetBinContent(221,0);		// A second beam is only simulated if this bin is set equal to unity. Default = 0
  Integers->SetBinContent(222,2212);     	// Particle ID code: particle species [int]. Default = 2212 (Protons) NOTE: Check if the mass is added at the end of this file!
  Integers->SetBinContent(223,1); 		// Number of primary particles per event [int]. Default = 1
  Doubles->SetBinContent(212,0.999);		// Lower Bound op particle total energy! [GeV] Default = 0.0
  Doubles->SetBinContent(213,1.001);		// Upper Bound op particle total energy! [GeV] Default = 1.0
  Doubles->SetBinContent(214,0.0);		// Lower bound of angle with z-axis at which particles are emitted [deg.] Default = 0.0
  Doubles->SetBinContent(215,4.584);	        // Upper bound of angle with z-axis at which particles are emitted [deg.] Default = 4.584 (80 mrad)
  Doubles->SetBinContent(216,0.0);		// Lower bound of azimuthal angle at which particles are emitted [deg.] Default = 0.0
  Doubles->SetBinContent(217,360.0);		// Upper bound of azimuthal angle at which particles are emitted [deg.] Default = 360.0
  Doubles->SetBinContent(218,0.0);		// x-coordinate of location of particle gun [cm]. Default = 0.0
  Doubles->SetBinContent(219,0.0);		// y-coordinate of location of particle gun [cm]. Default = 0.0
  Doubles->SetBinContent(220,0.0);		// z-coordinate of location of particle gun [cm]. Default = -200.0
  Doubles->SetBinContent(235,0.0);		// Rotation of particle gun around X-axis [deg] (only for ASCII generator)
  Doubles->SetBinContent(236,0.0);		// Rotation of particle gun around Y-axis [deg] (only for ASCII generator)
  Doubles->SetBinContent(237,0.0);		// Rotation of particle gun around Z-axis [deg] (only for ASCII generator)
  
  // --------------------------- Third Beam properties:
  Integers->SetBinContent(225,0);		// A Third beam is only simulated if this bin is set equal to unity. Default = 0
  Integers->SetBinContent(226,1000080240);	// Particle ID code: particle species [int]. Default = 11 (Electrons) NOTE: Check if the mass is added at the end of this file!
  Integers->SetBinContent(227,1); 		// Number of primary particles per event [int]. Default = 1
  Doubles->SetBinContent(223,0.999);		// Lower Bound op particle total energy! [GeV] Default = 0.0
  Doubles->SetBinContent(224,1.001);		// Upper Bound op particle total energy! [GeV] Default = 1.0
  Doubles->SetBinContent(225,0.0);		// Lower bound of angle with z-axis at which particles are emitted [deg.] Default = 0.0
  Doubles->SetBinContent(226,4.584);	        // Upper bound of angle with z-axis at which particles are emitted [deg.] Default = 4.584 (80 mrad)
  Doubles->SetBinContent(227,0.0);		// Lower bound of azimuthal angle at which particles are emitted [deg.] Default = 0.0
  Doubles->SetBinContent(228,360.0);		// Upper bound of azimuthal angle at which particles are emitted [deg.] Default = 360.0
  Doubles->SetBinContent(229,0.0);		// x-coordinate of location of particle gun [cm]. Default = 0.0
  Doubles->SetBinContent(230,0.0);		// y-coordinate of location of particle gun [cm]. Default = 0.0
  Doubles->SetBinContent(231,0.0);		// z-coordinate of location of particle gun [cm]. Default = -200.0
  Doubles->SetBinContent(238,0.0);		// Rotation of particle gun around X-axis [deg] (only for ASCII generator)
  Doubles->SetBinContent(239,0.0);		// Rotation of particle gun around Y-axis [deg] (only for ASCII generator)
  Doubles->SetBinContent(240,0.0);		// Rotation of particle gun around Z-axis [deg] (only for ASCII generator)
  
  // ---------------------------Target properties:
  // NOTE: Target material is defined at the Target geometry.
  Integers->SetBinContent(232,1000090290);      // For Boosting ASCII-generator: PDG-code of the mother beam from Super-FRS.
  Integers->SetBinContent(233,2212);            // For Boosting ASCII-generator: PDG-code of the fixed target.
 
  // ---------------------------Number of Repetions: Runs, Events, etc.
  Integers->SetBinContent(201,25000);   	// Number of events in a single run [int]. Default = 100
  
  // ---------------------------Neutron Calibration options:
  Integers->SetBinContent(240,50000);		// Number of events used in the Neutron calibration simulations. Default = 10000
  Integers->SetBinContent(244,5);               // Number of neutron tracks the calibration should search for. Default = 5
  Integers->SetBinContent(246,4);               // Number of charged tracks the calibration should search for. Default = 2
  Integers->SetBinContent(251,2212);            // PDG code that we assume the charged tracks to be. Default = 2212 (protons). NOTE: all we know that the VETO fires yes/no. We do not know PDG, we should assume it!!!
  Integers->SetBinContent(247,0);               // If this is 1, we will estimate proton energy from TOF. 0 means that we use original energy of Beam 2. Default = 0
  Integers->SetBinContent(245,4);               // Selection of Minimization procedure for the Calibration Cuts.
                                                // 0 = no minimization, just a hard-coded default answer (default). If The events are <= 900 this option is always selected. Default = 0
                                                // 1 = Jan Mayer his calibr_mini.C option. This uses equidistant cuts (same kappa) and minimizes the sum of wasted efficiency.
                                                // 2 = Same as 1, but now the cuts do not have to be equidistant.
                                                // 3 = Same as 2, but now we minimize the max of all (1-efficiency), not the sum.
                                                // 4 = same as 2, but now we minimize the quadratic sum of (1-efficiency).
  
  // ---------------------------Other settings:
  TObjString* Monte_Carlo_Engine = new TObjString("TGeant3");	// Decides which software is used to compute the simulation.
								// Replace the string with "TGeant3" "TGeant4" "TFluka"
								// Default = "TGeant3"
  TObjString* Event_Generator = new TObjString("ascii");	// Decides which software is used to generate the primary particles.
								// Replace the string with "box" "ascii" "r3b"
								// Default = "box"
  TObjString* Output_Name = new TObjString("Pos_p");   	// Determines the name of the output files of the simulation. Replace the string
							// with anything you want (except an empty string or blank characters!) Default = "VetoSim"
  Integers->SetBinContent(203,1); 		// 0 or 1: 1 allows the use of visualization, 0 does not. Default = 1
  Integers->SetBinContent(204,1); 		// 0 or 1: 1 allows secondary interactions in Geant4, 0 does not. Default = 1
  Integers->SetBinContent(205,0); 		// 0 or 1: 1 switches on the magnetic field of GLAD, 0 switches it off. Default = 1
  Doubles->SetBinContent(201,4000.0);		// Magnetic field current [A]. Default = 4000.0 (for 1 GeV beams). 1920.0 works nice for 300 MeV beams.
  Integers->SetBinContent(224,1);		// Choice between GLAD or ALADIN magnetic field. 0=ALADIN and 1=GLAD. Delault = 1
  Integers->SetBinContent(249,0);               // 1 = Check for Geant3 for digitizer etc. 0 = no checks. Default = 1.
  Integers->SetBinContent(248,1);               // NOTE: ASCII-generator only. Choice of how to generate the ASCII-file.
                                                // 0 = same principle as box generator, but now through ASCII-mechanism. Needed for heavy ions. (Default)
                                                // 1 = same as 0, but now the distributions have a uniform distribution over the NeuLAND surface.
                                                // 2 = Distributions are from a Simulated collision between target & mother beam. Target is at rest and Mother beam has energy of Beam 1 per nucleon.
                                                //     We then boost to the CM-frame. There, all energy is equally distributed among all nucleons. Nucleons are emitted uniformly over the unit sphere.
                                                //     Then we boost back to the LAB-frame. You get error reports if the Mother beam energy is too low to perform the event. So pay attention!!!
  Integers->SetBinContent(242,1);		// Choice whether or not you run the R3BRoot NeutronTracker along with the simulations.
						// 0 means that you do NOT run the tracker (Default).
						// 1 means that you run the tracker wit manual calibration. This means that YOU as a user have to define the Energy cuts that distinguish between the amount of neutrons and protons (FIXME only protons?)
						// You can do this by modifying the Energy_Cuts-files located in the Calibration folder here in the UI-directory. These .txt-file should consist ONLY of n+2 numbers. All starting at the begin
						// of the line and all directly below one another. The first number is the kappa-slope from Dima his multi-neutron simulations. The other n+1 numbers are energy cuts in [MeV].
						// everything between cut 1&2 is called 1 particles, everything between 2&3 is called 2 neutrons, etc. The first number should always be zero and the last should always be 1e6. 
                                                // The amount of numbers determines up to how many particles will be reconstructed (this is n). You as the user can either manually
						// set the cuts or you can run the ./calibrate.sh script from the UI-directory (without any inputs). This script will override all existsing Energy_Cuts-files. This script should be run after
						// the ./run.sh script. (re-run ./input.sh once more to reset the counter.) After you executed ./calibrated.sh, you should re-run your ./run.sh script to allow the R3BRoot Neutrontracker to use the new energy cuts.
                                                // Note that this script will run np*nn simulations wit 1,...,nn neutrons per event and 1,...,np protons per event. Hence this is a very time-consuming script.
						// in the end pictures from the cuts are also written to the Calibration-directory. The new cuts are now completely tuned to the parameters you set the last time you ran the ./input.sh
						// script. Also notice that the ./calibrate.sh script will only work properly if the primary beam is aimed at NeuLAND. If you run the script like ./calibrate.sh digi it will NOT rerun the
						// Monte Carlo simulations, but only the digitizer and the calibration. This should only be done if the number of events set here for neutron calibration is <= the number of events in the
						// Monte Carlo simulation files (these files should still exist!). Notice that this will NOT take modifications in geometry and simulation parameters into account! Hence it is advicable
						// not to do this and just to run ./calibrate.sh entirely. If you run ./calibrate.sh cal it will only calculate the cuts and just assume that all MonteCarlo files and digitizer files are still there.
						// 2 means that you run the Tracker with automatic calibration. Basically this means that after every run but before the tracker, the ./calibrate.sh script is executed automatically. Of course this will
						// produce the best data, especially for simulation series, but it is also VERY VERY VERY time consuming!
                                                // 3 means that you run the tracker with PERFECT calibration. We die-hard force the tracker to look for the exact number of particles that we fired.
  Integers->SetBinContent(243,1);		// Choice for Geant4 physics list:
						// 0 means standard R3BRoot Physics list. (Default) NOTE: This list is changes & updated all the time, it might not work! Use option 1 instead if this happens.
						// 1 means our own copy of the starndard R3BRoot physics list
						// 2 means our own physics list that cuts of delta-electrons. Specially designed by Dima for running heavy ion simulations. Reproduces TrackerVETO condition reasonable!
  Doubles->SetBinContent(242,7.5);              // Search Radius for the NeuLAND Clusterfinder in the spatial dimensions x, y, z [cm]. Default = 7.5
  Doubles->SetBinContent(243,1.0);              // Search Radius for the NeuLAND Clusterfinder in the time dimension t [ns]. Default = 1.0
  Integers->SetBinContent(250,1);               // STaRTrack Track reconstruction mode. 0 = elastic scattering, 1 = knockout reaction.
  
  // ---------------------------Detectors you whish to use:
  Integers->SetBinContent(206,0); 		// 0 or 1: 1 allows to use the Target, 0 does not. Default = 1
  Integers->SetBinContent(207,0); 		// 0 or 1: 1 allows to use StaRTracker, 0 does not. Default = 1
  Integers->SetBinContent(208,0); 		// 0 or 1: 1 allows to use PSP, 0 does not. Default = 1
  Integers->SetBinContent(209,0); 		// 0 or 1: 1 allows to use the Vacuum Vessel, 0 does not. Default = 1
  Integers->SetBinContent(210,0); 		// 0 or 1: 1 allows to use CALIFA, 0 does not. Default = 1
  Integers->SetBinContent(211,0); 		// 0 or 1: 1 allows to use mTOF, 0 does not. Default = 1
  Integers->SetBinContent(212,0); 		// 0 or 1: 1 allows to use the TOF wall, 0 does not. Default = 1
  Integers->SetBinContent(213,0); 		// 0 or 1: 1 allows to use MFI, 0 does not. Default = 1
  Integers->SetBinContent(214,0); 		// 0 or 1: 1 allows to use DCH, 0 does not. Default = 1
  Integers->SetBinContent(215,0); 		// 0 or 1: 1 allows to use GLAD, 0 does not. Default = 1
  Integers->SetBinContent(216,1); 		// 0 or 1 or 2: 1 allows to use NeuLAND with land-class, 0 does not allow NeuLAND. 2 allows to use NeuLAND with neuland-class. Default = 1
  Integers->SetBinContent(230,0);               // 0 or 1: 1 allows to use NEBULA, 0 does not. Default = 0
  Integers->SetBinContent(217,1); 		// 0 or 1 or 2: 1 allows to use the VETO with land-class, 0 does not allow VETO. 2 allows to use VETO with neuland-class. Default = 1
  Integers->SetBinContent(231,0);               // 0 or 1: 1 allows to use NEBULA VETO, 0 does not. Default = 0
  Integers->SetBinContent(253,0);               // 0 or 1: 1 allows to use the Dummy Vacuum chamber behind GLAD. Default = 0.
  
  // ---------------------------Simulation series parameters:
  Doubles->SetBinContent(133,3.0);		// VETO step size to reduce the z-coordinate of the VETO position [cm]. Default = 10.0
  Doubles->SetBinContent(134,0.0);		// VETO step size to increase the thickness of a single scintillator bar of the VETO [cm]. Default = 0.1
  Integers->SetBinContent(220,30);		// Total number of runs the simulation will be repeated after adapting a parameter. 4 means 4 runs in total [int]. Default = 1
  // NOTE: There is no increase parameter for the paddles: we simply multiply it by 2 every time.  
  
  // -------------------------------------------------------------------------
  // Analysis parameters:
  
  // Detector properties:
  Doubles->SetBinContent(301,160.0);		// NeuLAND energy threshold [keV] Default = 160 NOTE: Also used by digitizer!!!
  Doubles->SetBinContent(302,160.0);		// VETO energy threshold [keV] Default = 160 NOTE: Also used by digitizer!!!
  Doubles->SetBinContent(309,150.0);            // NeuLAND time resolution [ps]. Default = 150 NOTE: Also used by digitizer!!!
  Doubles->SetBinContent(310,300.0);            // VETO time resolution [ps]. Default = 300 NOTE: Also used by digitizer!!!
  Doubles->SetBinContent(334,14.0);		// Effective speed of light in VETO scintillator material [cm/ns]. Default = 14.0 NOTE: Also used by digitizer!!!
  Doubles->SetBinContent(335,0.008);		// light transport attenuation factor [1/cm] for transport to PMTs. Default = 0.008 NOTE: Also used by digitizer!!!
  Doubles->SetBinContent(330,10.0e10);          // Resolving Boundary [cm]. if x^2+y^2+z^2+(ct)^2<bounary^2 between MC and Reconstructed prim. interaction, we call the MC point Resolved.
  
  // Primary volume: NOTE: all particle guns should be outside this volume! If NeuLAND or NEBULA (veto) is there, it should be inside this volume!
  Doubles->SetBinContent(303,1300.0);		// z-location of front XY-plane for identification of primary particles [cm] Default = 600.0
  Doubles->SetBinContent(304,1701.0);		// z-location of back XY-plane for identification of primary particles [cm] Default = 1700.0
  Doubles->SetBinContent(305,-130.0);	        // y-location of lower XZ-plane for identification of primary particles [cm] Default = -150.0
  Doubles->SetBinContent(306,130.0);		// y-location of upper XZ-plane for identification of primary particles [cm] Default = 150.0
  Doubles->SetBinContent(307,-130.0);   	// x-location of YZ-plane at TOF side for identification of primary particles [cm] Default = -150.0
  Doubles->SetBinContent(308,130.0);		// x-location of YZ-plane at other side for identification of primary particles [cm] Default = 150.0
  Doubles->SetBinContent(341,7.0);             // Maximum radius a particle can be produced from the target. [cm]. Default = 5.0
  
  // Analysis settings:
  Integers->SetBinContent(304,0);		// If this bin equals 0, No bug reports are given. Default = 0
						// 1 gives bug reports ONLY for identification of primary particles.
						// 2 gives bug reports ONLY for the analysis on secondary particles.
                                                // 3 gives bug reports during the QTDC procedure: comparison with primaries.
                                                // 4 gives bug reports during the processing of the clusters.
                                                // 5 gives bug reports on the Tracker VETO condition.
                                                // 6 gives bug reports on the Tracker Cluster condition.
                                                // 7 gives bug reports during the processing of the tracker.
  
  Integers->SetBinContent(305,1);		// If this bin is set to unity, the identification of primary particles is performed. Otherwise not. Default = 1
  Integers->SetBinContent(306,1);		// If this bin is set to unity, the identification of primary interaction VS backscattering is performed. Otherwise not. Default = 1
  Integers->SetBinContent(307,1);		// If this bin is set to unity, the TOF and Edep per scintillator bar is computed. Otherwise not. Default = 1
  Integers->SetBinContent(308,0);		// If this bin is set to unity, the analysis of secondary particles is performed. Otherwise not. Default = 1
  Integers->SetBinContent(314,1);		// If this bin is set to unity, the Clusters of NeuLAND are processed, Otherwise not. The Digitizer should exist for this. Default = 1
  Integers->SetBinContent(313,1);		// If this bin is set to unity, the NeutronTracker is processed, Otherwise not. The tracker should exist for this! Default = 1
  Integers->SetBinContent(310,1);               // If this bin is set to unity, all bars of NeuLAND are kept for the VETO condition. Default = 0
  Integers->SetBinContent(309,2);               // Selects the VETO condition:
                                                // 0 (any anything else) is no VETO condition: we let everything pass (Default)
                                                // 1 = Naive VETO condition: we VETO everyting if the VETO fired, otherwise not.
                                                // 2 = TOF VETO condition. We VETO everything if the VETO fired BEFORE NeuLAND (on Total_TOF_Resolution based). Otherwise we let it pass.
                                                // 3 = segmented VETO condition. We VETO bars.
                                                // 4 = Tracker VETO condition (only works in combination with the Tracker!) We VETO reconstructed primary interaction points.
                                                // 5 = Igor his cluster VETO condition. We VETO clusters.
                                                // 6 = Tracker+Cluster VETO condition. We VETO reconstructed primary interaction points based on cluster structure.
                                                // 7 = Full VETO condition: we simply VETO every event.
                                                // 8 = Combined VETO condition; Time Cut, coincidence with STaRTrack||CALIFA & VETO condition 4.
                                                // 9 = Perfect Tracker VETO condition. We simply VETO every vertex linked to a particle that is no neutron from the target. This is for comparison purposes.
                                                // 10 = Combined VETO condition; Time Cut, coincidence with STaRTrack||CALIFA & VETO condition 6.
                                                // 11 = Perfect Tracker VETO condition. We simply VETO every vertex linked to a particle that is not a neutron. This is for comparison purposes.
                                                // 12 = Combined VETO condition; Time Cut, coincidence with STaRTrack||CALIFA & No VETO condition.
                                                // 13 = Combined VETO condition; Time Cut, coincidence with STaRTrack||CALIFA & Perfect VETO condition 9.
                                                // 14 = Combined VETO condition; Time Cut, coincidence with STaRTrack||CALIFA & Perfect VETO condition 11.
                                                // 15 = Combined VETO condition; Time Cut, coincidence with STaRTrack||CALIFA & Naive VETO condition.
                                                // 16 = Combined VETO condition; Time Cut, coincidence with STaRTrack||CALIFA & TOF VETO condition.
                                                // 17 = Combined VETO condition; Time Cut, coincidence with STaRTrack||CALIFA & Perfect VETO condition 18, but this perfect condition is only applied when the VETO actually fired!
                                                // 18 = Perfect Tracker VETO condition. We simply VETO every vertex linked to a particle that is not a neutron, but only if the VETO fired. This is for comparison purposes.
  Integers->SetBinContent(312,1);               // If this bin is set to unity, the Digitizer is used when possible, else we use LandPoints. NOTE that the digitizer should exist in order to use it!
                                                // If this bin equals 2, The Digitizer is used with uniform smearing: the the non-Tdiff-directions we use a Unform distribution instead of the bar center!
                                                // NOTE: choice for yes/no uniform smearing means that one actually has to re-run the entire simulation!
  
  // Secondary particle model:
  Doubles->SetBinContent(314,21.0);		// Mean travel distance for Gamma particles according to basic law of radiation [cm]. Default = 0.21
  Doubles->SetBinContent(315,54.0);		// Mean travel distance for Neutrons according to basic law of radiation [cm]. Default = 0.54
  Doubles->SetBinContent(316,1.0);		// Bethe-Bloch maximum step size for Protons [mm]. Default = 1.0;
  Doubles->SetBinContent(317,0.1);		// Bethe-Bloch maximum step size for Electrons [mm]. Default = 0.1;
  Doubles->SetBinContent(318,1.0);		// Bethe-Bloch maximum step size for Pions [mm]. Default = 1.0;
  Doubles->SetBinContent(319,1.0);		// Bethe-Bloch maximum step size for Muons [mm]. Default = 1.0;
  Doubles->SetBinContent(320,0.5);		// Bethe-Bloch maximum step size for Alphas [mm]. Default = 0.5;
  Doubles->SetBinContent(321,0.05);		// Bethe-Bloch maximum step size for C-ions [mm]. Default = 0.05;
  Integers->SetBinContent(301,200);		// Number of bins for secondary particle histograms in energy direction. [int] Default = 100
  Integers->SetBinContent(302,100);		// Number of bins for secondary particle histograms in z direction.  [int] Default = 100
  Integers->SetBinContent(303,100);		// Number of bins for secondary particle histograms in travel distance direction. [int] Default = 100
  Doubles->SetBinContent(311,1300.0);		// Starting z-coordinate of secondary particle histogram [cm]. Default = 1200.0
  Doubles->SetBinContent(312,1700.0);		// Ending z-coordinate of secondary particle histogram [cm]. Defaul = 1700.0
  Doubles->SetBinContent(313,500.0);		// Maximal travel distance in secondary particle histogram [cm]. Default = 500.0
  Doubles->SetBinContent(322,0.03);		// Low energy cuttoff: particles below this energy will NEVER reach the VETO, we cut them out manually. [MeV] Default = 0.03
						// the justification of this is that the air distance for electrons has a dip at very very low energies.
  
  // VETO condition settings:
  Doubles->SetBinContent(323,25.5);             // Shower radius [cm]. All bars of NeuLAND inside this radius around a hit in the VETO are VETOed. Default = 15.0
  Doubles->SetBinContent(324,185.0);            // Particles with a reconstructed energy above this cut are VETOed. [MeV] Default = 100.0
  Doubles->SetBinContent(342,100.0);            // Vertices with a TOF above this cut are VETOed by definition in the combined VETO condition. [ns]. Default = 200.0
  Doubles->SetBinContent(336,14.0);             // Effective speed of light that is used when matching points in processing the tracker and using the tracker VETO condition. Default = 14.0
  Doubles->SetBinContent(325,1.0);              // Z-component of Direction to VETO bars in segmented & cluster VETO condition. [dim. less] It only specifies direction. Default = 1.0
  Doubles->SetBinContent(326,0.0);              // X-component of Direction to VETO bars in segmented & cluster VETO condition. [dim. less] It only specifies direction. Default = 0.0
  Doubles->SetBinContent(327,0.0);              // Y-component of Direction to VETO bars in segmented & cluster VETO condition. [dim. less] It only specifies direction. Default = 0.0
  Doubles->SetBinContent(331,0.0);              // X-coordinate of where the neutrons came from (Tracker VETO condition) [cm]. Default = 0.0 NOTE: Also used by the R3BNeutronTracker2D!
  Doubles->SetBinContent(332,0.0);              // Y-coordinate of where the neutrons came from (Tracker VETO condition) [cm]. Default = 0.0 NOTE: Also used by the R3BNeutronTracker2D!
  Doubles->SetBinContent(333,0.0);              // Z-coordinate of where the neutrons came from (Tracker VETO condition) [cm]. Default = 0.0 NOTE: Also used by the R3BNeutronTracker2D!
  Doubles->SetBinContent(337,0.0);              // X-coordinate of where the PROTONS came from (Tracker VETO condition) [cm]. Default = 0.0
  Doubles->SetBinContent(338,0.0);              // Y-coordinate of where the PROTONS came from (Tracker VETO condition) [cm]. Default = 0.0
  Doubles->SetBinContent(339,0.0);              // Z-coordinate of where the PROTONS came from (Tracker VETO condition) [cm]. Default = 0.0
  
  // Analysis series parameters:
  Integers->SetBinContent(311,1);               // Amount of steps the analysis should be repeated. When this bin is 4, we do 4 analyses. The first time with parameters as above and then
                                                // the next time with one parameter increased/decreased, etc. Which parameter depends on the argument that you give to ./analyse.sh Default = 1
  Doubles->SetBinContent(328,0.0);              // Shower radius increase per step [cm]. Default = 5.0
  Doubles->SetBinContent(343,0.0);              // TimeCut increase per step [ns]. Default = 5.0
  Doubles->SetBinContent(329,0.0);		// Increase in reconstructed energy cut [MeV]. Default = 10.0
  Doubles->SetBinContent(340,0.0);		// Decrease in X-coordinate of the protons starting point in the Tracker VETO condition [cm]. Default = 20.0;
  
  // -------------------------------------------------------------------------
  // Graphics parameters:
  Doubles->SetBinContent(401,2.1);              // Pictures scale factor. [int] Default = 1
  Integers->SetBinContent(401,2);	        // selects whitch TOF is used in the VETO performance picture. (./foto perf) This picture only uses
					        // overall TOF of the entire detector. But you can either add the time resolution before taking the
						// TOF simply takes the minimum of all times of the interaction points. If a VETO bar however contains
						// both primary interaction and backscattering but the primary interaction stays below the energy
						// threshold, the TOF should be the minimum time of only the backscattering points. This is what TrackTOF does.
						// 0 or anything else= No TOF at all. [This is Default value]
						// 1=ordinary TOF and add time resolution AFTER minimizing (old program, incompatible with digitizer!).
						// 2=ordinary TOF and add time resolution BEFORE minimizing (only thing that makes sense with digitizer!)
						// 3=TrackTOF and add time resolution AFTER minimizing
						// 4=TrackTOF and add time resolution BEFORE minimizing (most physical situation).
  Integers->SetBinContent(402,1);		// Selects the type of projection for histograms of secondary particles. 1 plots Energy VS z-coordinate, 2 plots Energy VS traveldistance. Anything else is not accepted. Default = 1
  Integers->SetBinContent(403,100);		// Number of bins in the histograms for reconstructed energy. Default = 100
  Integers->SetBinContent(404,300);		// Number of bins in the Ken Miki histograms. Default = 200
  Integers->SetBinContent(405,1);		// If this bin is set to unity, only tracks aimed at the VETO are plotted. Default = 0
  Integers->SetBinContent(406,2212);		// PDG code that we select on in the Ken Miki plots and also in the Fired Bars OpenGL pictures (0 means all particles!) Default = 0
  Integers->SetBinContent(407,0);		// Selects the plane number for the NeuLAND content plots and the TOFvsEdep plots (also from Ken Miki). 0 = Default = entire NeuLAND
  Integers->SetBinContent(408,200);		// Number of bins in the TOFvsEdep plots. [int] Default = 200
  Doubles->SetBinContent(402,600.0);		// Emax in TOFvsEdep plots. [MeV] Default = 100.0
  Doubles->SetBinContent(403,0.0);		// TOF min. value in TOFvsEdep plots. [ns] Default = -50.0;
  Doubles->SetBinContent(404,150.0);		// TOF max. value in TOFvsEdep plots. [ns] Default = 200.0;
  Integers->SetBinContent(409,0);		// If this bin equals 1 then the full energy of the particle is plotted instead of the reconstructed energy. Default = 0
  Doubles->SetBinContent(405,1200.0);		// Emax in reconstructed Energy plots [MeV]. Default = 1200.0
  Integers->SetBinContent(410,3);		// Vertical number of plots in the neutron calibration pictures. Default = 2
  Integers->SetBinContent(411,3);		// Horizontal number of plots in the neutron calibration pictures. Default = 3
  Integers->SetBinContent(412,1);               // Number of neutrons that you want to detect. Is used in counting good events. Default = 4.
  
  // ==========================================================================
  // Other tasks:
  Integers->SetBinContent(1,1);			// NOTE: This bin represents the run number. Do not change it! This parameter is set to unity
						// in the initialization (therefore here). It will be updated later on, so changing its value
						// might give trouble.
  
  Integers->SetBinContent(241,0);		// NOTE: This bin denotes if we have a neutron calibration run yes/no. This will be taken care of
						// by the program automatically, hence you should not touch it!
  
  // Specification of particle masses in [GeV]:
  Doubles->SetBinContent(211,DefineParticleMass(Integers->GetBinContent(218)));             // First beam mass:
  Doubles->SetBinContent(221,DefineParticleMass(Integers->GetBinContent(222)));             // Second beam mass:
  Doubles->SetBinContent(241,DefineParticleMass(Integers->GetBinContent(226)));             // Third beam mass:
  Doubles->SetBinContent(244,DefineParticleMass(Integers->GetBinContent(232)));             // Mother beam mass:
  Doubles->SetBinContent(245,DefineParticleMass(Integers->GetBinContent(233)));             // Fixed Target mass:
  Doubles->SetBinContent(246,DefineParticleMass(Integers->GetBinContent(251)));             // Mass for Charged tracks in calibration
 
  // ==========================================================================
  // Writing all data to a file:
  
  // Creation of the Inputs.root file... NOTE: this script is located in the UI
  // directory, hence the path must be specified from there:
  TFile* inputs = new TFile("../Data/Inputs/Inputs.root","recreate");
  
  // Write the histograms to the file:
  Doubles->Write("Doubles",2);
  Integers->Write("Integers",2);
  
  // Write the other data to the file:
  Target_Material->Write("Target_Material",2);
  Monte_Carlo_Engine->Write("Monte_Carlo_Engine",2);
  Event_Generator->Write("Event_Generator",2);
  Output_Name->Write("Output_Name",2);
  
  // Write our choice for the neutron tracker to a file:
  std::ofstream txt_file ("../Data/Inputs/Tracker_Choice.txt", std::ofstream::out); // recreate file
  txt_file << Integers->GetBinContent(242);
  txt_file.close();
  
  std::ofstream txt_file2 ("../Data/Inputs/ASCII_Choice.txt", std::ofstream::out); // recreate file
  txt_file2 << Integers->GetBinContent(248);
  txt_file2.close();
  
  std::ofstream txt_file3 ("../Data/Inputs/Cave_Medium.txt", std::ofstream::out); // recreate file
  txt_file3 << Integers->GetBinContent(252);
  txt_file3.close();
  
  // ==========================================================================
  // After all inputs have been written, our task is complete!
  inputs->Close();
  
}