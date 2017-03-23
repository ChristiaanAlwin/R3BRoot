// This hearder file contains the definition of a structure
// containing all the inputs from the Inputs.root-file:

// Written by C. A. Douma:

// Inclusion of required hearders:
#include "TString.h"

// Definition of the structure:
struct Inputs_Structure {
  
  // PSP Geometry parameters:
  Double_t PSP_geo_det1_Xpos;
  Double_t PSP_geo_det1_Ypos;
  Double_t PSP_geo_det1_Zpos;
  Double_t PSP_geo_det1_RotX;
  Double_t PSP_geo_det1_RotY;
  Double_t PSP_geo_det1_RotZ;
  Double_t PSP_geo_det2_Xpos;
  Double_t PSP_geo_det2_Ypos;
  Double_t PSP_geo_det2_Zpos;
  Double_t PSP_geo_det2_RotX;
  Double_t PSP_geo_det2_RotY;
  Double_t PSP_geo_det2_RotZ;
  Double_t PSP_geo_det3_Xpos;
  Double_t PSP_geo_det3_Ypos;
  Double_t PSP_geo_det3_Zpos;
  Double_t PSP_geo_det3_RotX;
  Double_t PSP_geo_det3_RotY;
  Double_t PSP_geo_det3_RotZ;
  
  // TARGET paremeters: Material & Geometry:
  TString Target_Material;
  Double_t Target_geo_Xpos;
  Double_t Target_geo_Ypos;
  Double_t Target_geo_Zpos;
  Double_t Target_geo_ThetaRotX;
  Double_t Target_geo_ThetaRotY;
  Double_t Target_geo_ThetaRotZ;
  Double_t Target_geo_PhiRotX;
  Double_t Target_geo_PhiRotY;
  Double_t Target_geo_PhiRotZ;
  
  // ALADIN Geometry parameters:
  Double_t ALADIN_geo_Xpos;
  Double_t ALADIN_geo_Ypos;
  Double_t ALADIN_geo_Zpos;
  Double_t ALADIN_geo_RotX;
  Double_t ALADIN_geo_RotY;
  Double_t ALADIN_geo_RotZ;
  
  // GFI Geometry parameters:
  Double_t GFI_geo_Xpos;
  Double_t GFI_geo_Ypos;
  Double_t GFI_geo_Zpos;
  Double_t GFI_geo_RotX;
  Double_t GFI_geo_RotY;
  Double_t GFI_geo_RotZ;
  Double_t GFI_geo_Dist;
  
  // GLAD Geometry parameters:
  Double_t GLAD_geo_Xpos;
  Double_t GLAD_geo_Ypos;
  Double_t GLAD_geo_Zpos;
  Double_t GLAD_geo_RotX;
  Double_t GLAD_geo_RotY;
  Double_t GLAD_geo_RotZ;
  
  // CALIFA Geometry parameters:
  Double_t CALIFA_geo_Xpos;
  Double_t CALIFA_geo_Ypos;
  Double_t CALIFA_geo_Zpos;
  Double_t CALIFA_geo_RotX;
  Double_t CALIFA_geo_RotY;
  Double_t CALIFA_geo_RotZ;
  
  // Crystal Ball parameters:
  Double_t CrystalBall_geo_Xpos;
  Double_t CrystalBall_geo_Ypos;
  Double_t CrystalBall_geo_Zpos;
  Double_t CrystalBall_geo_RotX;
  Double_t CrystalBall_geo_RotY;
  Double_t CrystalBall_geo_RotZ;
  
  // TOF wall Geometry parameters:
  Double_t TOF_geo_Xpos;
  Double_t TOF_geo_Ypos;
  Double_t TOF_geo_Zpos;
  Double_t TOF_geo_RotX;
  Double_t TOF_geo_RotY;
  Double_t TOF_geo_RotZ;
  
  // mTOF Geometry parameters:
  Double_t mTOF_geo_Xpos;
  Double_t mTOF_geo_Ypos;
  Double_t mTOF_geo_Zpos;
  Double_t mTOF_geo_ThetaRotX;
  Double_t mTOF_geo_ThetaRotY;
  Double_t mTOF_geo_ThetaRotZ;
  Double_t mTOF_geo_PhiRotX;
  Double_t mTOF_geo_PhiRotY;
  Double_t mTOF_geo_PhiRotZ;
			 
  // DCH Geometry parameters:
  Double_t DCH_geo_Xpos;
  Double_t DCH_geo_Ypos;
  Double_t DCH_geo_Zpos;
  Double_t DCH_geo_RotX;
  Double_t DCH_geo_RotY;
  Double_t DCH_geo_RotZ;
  
  // StaRTrack Geometry parameters:
  Double_t STaRTrack_geo_Xpos;
  Double_t STaRTrack_geo_Ypos;
  Double_t STaRTrack_geo_Zpos;
  Double_t STaRTrack_geo_RotX;
  Double_t STaRTrack_geo_RotY;
  Double_t STaRTrack_geo_RotZ;
  
  // Vacuum Vessel Geometry parameters:
  Double_t VACVESSEL_geo_Xpos;
  Double_t VACVESSEL_geo_Ypos;
  Double_t VACVESSEL_geo_Zpos;
  Double_t VACVESSEL_geo_RotX;
  Double_t VACVESSEL_geo_RotY;
  Double_t VACVESSEL_geo_RotZ;
  
  // MFI Geometry parameters:
  Double_t MFI_geo_Xpos;
  Double_t MFI_geo_Ypos;
  Double_t MFI_geo_Zpos;
  Double_t MFI_geo_RotX;
  Double_t MFI_geo_RotY;
  Double_t MFI_geo_RotZ;
  Double_t MFI_geo_Dist;
  
  // Vacuum Chamber parameters:
  Double_t VACchamber_geo_Xpos;
  Double_t VACchamber_geo_Ypos;
  Double_t VACchamber_geo_Zpos;
  Double_t VACchamber_geo_RotX;
  Double_t VACchamber_geo_RotY;
  Double_t VACchamber_geo_RotZ;
  
  // NEBULA Geometry parameters:
  Double_t NEBULA_geo_Xpos;
  Double_t NEBULA_geo_Ypos;
  Double_t NEBULA_geo_Zpos;
  Double_t NEBULA_geo_RotX;
  Double_t NEBULA_geo_RotY;
  Double_t NEBULA_geo_RotZ;
  Double_t NEBULA_geo_Dist;
  Int_t NEBULA_geo_Nparts;
  Double_t NEBULA_geo_VetoDist;
  
  // NeuLAND Geometry parameters:
  Double_t NeuLAND_geo_Xpos;
  Double_t NeuLAND_geo_Ypos;
  Double_t NeuLAND_geo_Zpos;
  Double_t NeuLAND_geo_RotX;
  Double_t NeuLAND_geo_RotY;
  Double_t NeuLAND_geo_RotZ;
  Int_t NeuLAND_geo_NsinglePlanes;
  
  // VETO Geometry parameters:
  Double_t VETO_geo_Xpos;
  Double_t VETO_geo_Ypos;
  Double_t VETO_geo_Zpos;
  Double_t VETO_geo_RotX;
  Double_t VETO_geo_RotY;
  Double_t VETO_geo_RotZ;
  Double_t VETO_geo_TotalBarLength;
  Double_t VETO_geo_TotalBarWidth;
  Double_t VETO_geo_TotalBarThickness;
  Double_t VETO_geo_TotalGab;
  Double_t VETO_geo_FoilThickness;
  Double_t VETO_geo_TapeThickness;
  Int_t VETO_geo_NsinglePlanes;
  Int_t VETO_geo_NbarsPerPlane;
  
  // Detectors that should be used:
  Int_t Detectors_TARGET_Exists;
  Int_t Detectors_STaRTracker_Exists;
  Int_t Detectors_PSP_Exists;
  Int_t Detectors_VacVessel_Exists;
  Int_t Detectors_CALIFA_Exists;
  Int_t Detectors_mTOF_Exists;
  Int_t Detectors_TOFwall_Exists;
  Int_t Detectors_MFI_Exists;
  Int_t Detectors_DCH_Exists;
  Int_t Detectors_GFI_Exists;
  Int_t Detectors_ALADIN_Exists;
  Int_t Detectors_GLAD_Exists;
  Int_t Detectors_NeuLAND_Exists;
  Int_t Detectors_NEBULA_Exists;
  Int_t Detectors_VETO_Exists;
  Int_t Detectors_NebVETO_Exists;
  Int_t Detectors_VACchamber_Exists;
  
  // First Beam properties:
  Int_t Beam_PDG;
  Double_t Beam_Mass;
  Int_t Beam_Nparticles;
  Double_t Beam_Emin;
  Double_t Beam_Emax;
  Double_t Beam_ThetaMin;
  Double_t Beam_ThetaMax;
  Double_t Beam_PhiMin;
  Double_t Beam_PhiMax;
  Double_t Beam_Xpos;
  Double_t Beam_Ypos;
  Double_t Beam_Zpos;
  Double_t Beam_Xrot_ASCII;
  Double_t Beam_Yrot_ASCII;
  Double_t Beam_Zrot_ASCII;
  
  // Second Beam properties:
  Int_t Beam2_Exe;
  Int_t Beam2_PDG;
  Double_t Beam2_Mass;
  Int_t Beam2_Nparticles;
  Double_t Beam2_Emin;
  Double_t Beam2_Emax;
  Double_t Beam2_ThetaMin;
  Double_t Beam2_ThetaMax;
  Double_t Beam2_PhiMin;
  Double_t Beam2_PhiMax;
  Double_t Beam2_Xpos;
  Double_t Beam2_Ypos;
  Double_t Beam2_Zpos;
  Double_t Beam2_Xrot_ASCII;
  Double_t Beam2_Yrot_ASCII;
  Double_t Beam2_Zrot_ASCII;
  
  // Third Beam properties:
  Int_t Beam3_Exe;
  Int_t Beam3_PDG;
  Double_t Beam3_Mass;
  Int_t Beam3_Nparticles;
  Double_t Beam3_Emin;
  Double_t Beam3_Emax;
  Double_t Beam3_ThetaMin;
  Double_t Beam3_ThetaMax;
  Double_t Beam3_PhiMin;
  Double_t Beam3_PhiMax;
  Double_t Beam3_Xpos;
  Double_t Beam3_Ypos;
  Double_t Beam3_Zpos;
  Double_t Beam3_Xrot_ASCII;
  Double_t Beam3_Yrot_ASCII;
  Double_t Beam3_Zrot_ASCII;
  
  // Boosting ASCII-generator:
  Int_t MotherBeamPDG;
  Int_t Target_PDG;
  Double_t MotherBeamMass;
  Double_t Target_Mass;
  
  // Other simulation parameters:
  Int_t nEvents;
  Int_t nEvents_NeutronCalibration;
  Int_t nTracks_NeutronCalibration;
  Int_t pTracks_NeutronCalibration;
  Int_t pTracksPDG_NeutronCalibration;
  Double_t pTracksMass_NeutronCalibration;
  Int_t UseEnergyEstimate;
  TString Monte_Carlo_Engine;
  TString Event_Generator;
  TString Output_Name;
  Int_t VIS;
  Int_t SEC;
  Int_t MAG;
  Double_t MAGcurrent;
  Int_t FieldMap;
  Int_t ThisCounter;
  Int_t ASCIIchoice;
  Int_t Geant3Test;
  Int_t CalibrationID;
  Int_t Tracker_Exe;
  Int_t G4_PhysicsList;
  Int_t Calibr_Minimization;
  Double_t ClusterSpaceRadius;
  Double_t ClusterTimeRadius;
  Int_t STaRTrackMode;
  Int_t CaveMedium; 
  
  // Batch mode parameters:
  Double_t VETO_stepsize;
  Double_t VETO_thickstep;
  Int_t Simulation_Repetition;
  
  // Detector properties:
  Double_t Neu_Ethreshold;
  Double_t Veto_Ethreshold; 
  Double_t Neu_TimeResolution;
  Double_t VETO_TimeResolution;
  Double_t ResolveBoundary;
  Double_t cEff_VETOscint;
  Double_t ExpAttenuation;
  
  // Primary Volume:
  Double_t PrimIDplane_Zfront; 
  Double_t PrimIDplane_Zback; 
  Double_t PrimIDplane_Ylow;
  Double_t PrimIDplane_Yup;
  Double_t PrimIDplane_Xtof;
  Double_t PrimIDplane_Xother;
  Double_t AllowedTargetRadius;
  Double_t PrimTcut_up;
  Double_t PrimTcut_low;
  
  // Analysis settings:
  Int_t Bug_Report;
  Int_t Trigger_Exe;
  Int_t PrimParticles_Exe;
  Int_t TrackEvents_Exe;
  Int_t QTDC_Exe;
  Int_t Secondaries_Exe;
  Int_t Cluster_Exe;
  Int_t AnaTracker_Exe;
  Int_t VETO_select;
  Int_t Store_AllBars;
  Int_t UseDigitizer;
  
  // Secondary particle model:
  Double_t Z_mean_gamma;
  Double_t Z_mean_neutron;
  Double_t Bethe_Dx_Protons;
  Double_t Bethe_Dx_Electrons;
  Double_t Bethe_Dx_Pions;
  Double_t Bethe_Dx_Muons;
  Double_t Bethe_Dx_Alphas;
  Double_t Bethe_Dx_Cions;
  Int_t Nbins_Esec;
  Int_t Nbins_Zsec;
  Int_t Nbins_Dsec;
  Double_t ZsecMin;
  Double_t ZsecMax;
  Double_t DistMax;
  Double_t Ecutoff;
  
  // VETO condition parameters:
  Double_t VETO_Ecut;
  Double_t TimeCut;
  Double_t Shower_Radius;
  Double_t EffectiveC;
  Double_t VETO_Zdir;
  Double_t VETO_Xdir;
  Double_t VETO_Ydir;
  Double_t NeuBegin_X;
  Double_t NeuBegin_Y;
  Double_t NeuBegin_Z;
  Double_t NeuProtonBegin_X;
  Double_t NeuProtonBegin_Y;
  Double_t NeuProtonBegin_Z;
  
  // Analysis series parameters:
  Int_t N_Analysis;
  Double_t Showerstep;
  Double_t EcutStep;
  Double_t TimeCutStep;
  Double_t NeuPstart_Step;
  
  // Graphics parameters:
  Double_t ScaleFactor;
  Int_t TOFchoice;
  Int_t Projection;
  Int_t Nbins_Eres;
  Int_t Nbins_KenMiki;
  Int_t KenMiki_Aim;
  Int_t PDG_KenMiki;
  Int_t NeuContent_Plane;
  Int_t Nbins_ETOF;
  Double_t ETOF_Emax;
  Double_t ETOF_Tmin;
  Double_t ETOF_Tmax;
  Int_t Eres_full;
  Double_t Eres_Emax;
  Int_t CalNvert;
  Int_t CalNhor;
  Int_t NneutronsGood;
  Int_t Only_Detected;
  Int_t UseTrigger;
  
  // Bethe-Bloch histograms:
  TH1D* Protons_TravelDist;
  TH1D* Electrons_TravelDist;
  TH1D* Pions_TravelDist;
  TH1D* Muons_TravelDist;
  TH1D* Alphas_TravelDist;
  TH1D* Cions_TravelDist;

};




