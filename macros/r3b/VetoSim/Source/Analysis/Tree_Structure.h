// This hearder file contains the definition of a structure
// containing all the variables needed to fill the Analysis
// tree.

// Wriiten by C. A. Douma:

// Inclusion of required hearders:
#include "TTree.h"
#include "TString.h"
#include "TH3D.h" 

// Definition of the structure:
struct Tree_Structure {
  
  // NOTE: Dynamic arrays should be connected to the branch each time during runtime!
  
  // pointer to the file in which the analysis tree is stored:
  TFile* TheFile;
  
  // pointer to the analysis tree:
  TTree* TheTree;
  
  // Filename:
  TString FileName;
  
  // Identification of primary particles:
  Int_t Nprims_ParticleGun;	// Number of particles fired from the beam
  Int_t Nprims_NeuLAND;		// Number of particles determined as primary w.r.t. NeuLAND. These are NOT the primaries from the beam!
  Bool_t* Prim_Detected;	// NOTE: Dynamic array for NeuLAND primary particles: detected by NeuLAND yes/no.
  Bool_t* Prim_Resolved;	// NOTE: Dynamic array for NeuLAND primary particles: Resolved by NeuLAND NeutronTracker yes/no.
  Int_t* Prim_TrackID;		// NOTE: Dynamic array for NeuLAND primary particles: Monte Carlo trackID of these particles.
  Int_t* Prim_PDG;		// NOTE: Dynamic array for NeuLAND primary particles: PDG code of these particles.
  Double_t* Prim_E;		// NOTE: Dynamic array for NeuLAND primary particles: Energy of these particles [MeV].
  Bool_t* Prim_IsVETOed;        // NOTE: Dynamic array for NeuLAND primary particles: Whether or not the particle is VETOed.
  Bool_t* Prim_IsFromTarget;    // NOTE: Dynamic array for NeuLAND primary particles: wether or not the particle track is produced within a radius of 7 cm from the center point of the target.
  Double_t* Prim_E_Detected;    // NOTE: Dynamic array for NeuLAND primary particles: Reconstructed energy of these particles [MeV]. For segmented VETO condition this is Sum(Edep per bar). Bars are tracked by G4MotherId.
  Double_t* Prim_MCinteraction_T;   // NOTE: Dynamic array for NeuLAND primary particles: The first Monte Carlo interaction point of that primary particle (time [ns]).
  Double_t* Prim_MCinteraction_X;   // NOTE: Dynamic array for NeuLAND primary particles: The first Monte Carlo interaction point of that primary particle (x-point [cm]).
  Double_t* Prim_MCinteraction_Y;   // NOTE: Dynamic array for NeuLAND primary particles: The first Monte Carlo interaction point of that primary particle (y-point [cm]).
  Double_t* Prim_MCinteraction_Z;   // NOTE: Dynamic array for NeuLAND primary particles: The first Monte Carlo interaction point of that primary particle (z-point [cm]). 
  
  // Identification of backscattering events:
  Bool_t* PrimInteraction;	// Static array for all bars in the VETO: This bar contains a primary interaction yes/no
  Bool_t* BackInteraction;      // Static array for all bars in the VETO: This bar contains a backscattering interaction yes/no
  Bool_t* LoopInteraction;      // Static array for all bars in the VETO: This bar contains an interaction that leaves the VETO at the side of the target, but then loops back to NeuLAND yes/no
  Bool_t* Energetic_Prim;       // Static array for all bars in the VETO: This bar contains a primary interaction with energy deposition above thhreshold yes/no. 
  Bool_t* Energetic_Back;       // Static array for all bars in the VETO: This bar contains backscattering or loop interaction with energy deposition above thhreshold yes/no.
  Bool_t Overal_Prim;           // This boolian is kTRUE if one of the boolians in PrimInteraction is kTRUE.
  Bool_t Overal_Back;           // This boolian is kTRUE if one of the boolians in PrimInteraction OR in LoopInteraction is kTRUE.
  Bool_t Overal_EPrim;          // This boolain is kTRUE if one of the boolians in Energetic_Prim is kTRUE;
  Bool_t Overal_EBack;          // This boolain is kTRUE if one of the boolians in Energetic Back is kTRUE;
  Double_t* VETO_TrackTOF;      // Static array for all bars in the VETO: TOF per bar taking primary/backscattering into account [ns]. Equals -100.0 for bars without energetic interacton.
  Double_t* VETO_TrackTOF_Res;  // Static array for all bars in the VETO: TOF with time resolution per bar taking primary/backscattering into account [ns]. Equals -100.0 for bars without energetic interacton.
  Double_t VETO_TotalTrackTOF;  // Full VETO TOF taking primary/backscattering into account [ns]. Equals -100.0 when there are no interactions.
  Double_t VETO_TotalTrackTOF_Res;  // Full VETO TOF with time resolution taking primary/backscattering into account [ns]. Equals -100.0 when there are no interactions.

  // QDC and TDC of NeuLAND and VETO:
  Int_t NbarsFired_NeuLAND;     // Number of bars inside NeuLAND that came above the energy threshold.
  Int_t* Neu_BarNumber_Fired;   // NOTE: Dynamic array for NeuLAND number of bars that fired: NeuLAND number of the specific bar that fired. [int]
  Double_t* Neu_Edep_Fired;     // NOTE: Dynamic array for NeuLAND number of bars that fired: Total enegy deposition of the specific bar that fired. [MeV]
  Double_t* Neu_TOF_Fired;      // NOTE: Dynamic array for NeuLAND number of bars that fired: TOF of the specific bar that fired. [ns]
  Double_t* Neu_TOF_Fired_Res;  // NOTE: Dynamic array for NeuLAND number of bars that fired: TOF with time resolution of the specific bar that fired. [ns]
  Int_t* Neu_Primary_TrackID;	// NOTE: Dynamic array for NeuLAND number of bars that fired: TrackID of the Primary particle belonging to the FIRST particle that hits the fired bar (on TOF basis).
  Bool_t* Neu_Bar_IsVETOed;     // NOTE: Dynamic array for NeuLAND number of bars that fired: Determine Whether this bar is VETOed yes/no.
  Double_t Neu_TotalTOF;        // Minimum TOF over all Fired bars for NeuLAND [ns].
  Double_t Neu_TotalTOF_Res;    // Minimum TOF over all Fired bars for NeuLAND with time resolution [ns].

  Int_t NbarsFired_VETO;        // Number of bars inside VETO that came above the energy threshold.
  Int_t* VETO_BarNumber_Fired;  // NOTE: Dynamic array for VETO number of bars that fired: VETO number of the specific bar that fired. [int]
  Double_t* VETO_Edep_Fired;    // NOTE: Dynamic array for VETO number of bars that fired: Total enegy deposition of the specific bar that fired. [MeV]
  Double_t* VETO_TOF_Fired;     // NOTE: Dynamic array for VETO number of bars that fired: TOF of the specific bar that fired. [ns]
  Double_t* VETO_TOF_Fired_Res; // NOTE: Dynamic array for VETO number of bars that fired: TOF with time resolution of the specific bar that fired. [ns]
  Int_t* VETO_Primary_TrackID;	// NOTE: Dynamic array for VETO number of bars that fired: TrackID of the Primary particle belonging to the FIRST particle that hits the fired bar (on TOF basis).
  Int_t* VETO_TOF_index;        // NOTE: Dynamic array for VETO number of bars that fired: C++ index of veto_segPoint leafs that is used to compute TOF.
  Double_t VETO_TotalTOF;       // Minimum TOF over all Fired bars for VETO [ns].
  Double_t VETO_TotalTOF_Res;   // Minimum TOF over all Fired bars for VETO with time resolution [ns].
  
  // Quantification of secondary particles:
  Int_t* Secondary_Nprotons;     // 1. Static array of size 5: Amount of registered secondary protons in this specific event. Values 1=all, 2=aimed, 3=reached, 4=hit, 5=detected.
  Int_t* Secondary_Nneutrons;    // 2. Static array of size 5: Amount of registered secondary neutrons in this specific event. Values 1=all, 2=aimed, 3=reached, 4=hit, 5=detected.
  Int_t* Secondary_Nelectrons;   // 3. Static array of size 5: Amount of registered secondary electrons in this specific event. Values 1=all, 2=aimed, 3=reached, 4=hit, 5=detected.
  Int_t* Secondary_Ngammas;      // 4. Static array of size 5: Amount of registered secondary gammas in this specific event. Values 1=all, 2=aimed, 3=reached, 4=hit, 5=detected.
  Int_t* Secondary_Npions;       // 5. Static array of size 5: Amount of registered secondary pions in this specific event. Values 1=all, 2=aimed, 3=reached, 4=hit, 5=detected.
  Int_t* Secondary_Nmuons;       // 6. Static array of size 5: Amount of registered secondary muons in this specific event. Values 1=all, 2=aimed, 3=reached, 4=hit, 5=detected.
  Int_t* Secondary_Nalphas;      // 7. Static array of size 5: Amount of registered secondary alphas in this specific event. Values 1=all, 2=aimed, 3=reached, 4=hit, 5=detected.
  Int_t* Secondary_NCions;       // 8. Static array of size 5: Amount of registered secondary Cions in this specific event. Values 1=all, 2=aimed, 3=reached, 4=hit, 5=detected.
  Int_t* Secondary_Nfragments;   // 9. Static array of size 5: Amount of registered secondary fragments in this specific event. Values 1=all, 2=aimed, 3=reached, 4=hit, 5=detected.
  Int_t* Secondary_NallParticles;// 10.Static array of size 5: Amount of registered secondary particles in this specific event. Values 1=all, 2=aimed, 3=reached, 4=hit, 5=detected.
  TH3D* Harray[10][5];         	 // NOTE: These histograms are NOT stored in the tree, but seperately in the files! they are sums over all events. The fisrt index marks the
                                 //       particle type (see first number in above comments) and the second is the same as the arrays of size 5 in the above comments.
                                 // Hence the array will always have 1-10 (or 0-9) as first index and 1-5 or 0-4 as second index.
  Bool_t Secondary_Particles;	 // This boolian states whether the event registered secondary particles in the VETO yes/no.
  
  // Processing the NeuLAND Clusters:
  Int_t NeuLAND_Nclusters;          // Total number of clusters found in this event.
  Double_t* NeuLAND_ClusterX;       // NOTE: Dynamical array for the number of Clusters: x-coordinate of the starting point of this cluster. [cm]
  Double_t* NeuLAND_ClusterY;       // NOTE: Dynamical array for the number of Clusters: y-coordinate of the starting point of this cluster. [cm]
  Double_t* NeuLAND_ClusterZ;       // NOTE: Dynamical array for the number of Clusters: z-coordinate of the starting point of this cluster. [cm]
  Double_t* NeuLAND_ClusterTime;    // NOTE: Dynamical array for the number of Clusters: t-coordinate of the starting point of this cluster. [ns]
  Double_t* NeuLAND_CStopX;         // NOTE: Dynamical array for the number of Clusters: x-coordinate of the stopping point of this cluster. [cm]
  Double_t* NeuLAND_CStopY;         // NOTE: Dynamical array for the number of Clusters: y-coordinate of the stopping point of this cluster. [cm]
  Double_t* NeuLAND_CStopZ;         // NOTE: Dynamical array for the number of Clusters: z-coordinate of the stopping point of this cluster. [cm]
  Double_t* NeuLAND_CStopTime;      // NOTE: Dynamical array for the number of Clusters: t-coordinate of the stopping point of this cluster. [ns]
  Double_t* NeuLAND_ClusterEnergy;  // NOTE: Dynamical array for the number of Clusters: Total Energy inside this cluster. [MeV]
  Int_t* NeuLAND_ClusterSize;       // NOTE: Dynamical array for the number of Clusters: Number of bars/hits/digis inside this cluster [int]   
  Int_t* NeuLAND_Cluster_TrackID;   // NOTE: Dynamical array for the number of Clusters: TrackID of the primary particle this cluster belongs to [int].
  Bool_t* NeuLAND_Cluster_IsVETOed; // NOTE: Dynamical array for the number of Clusters: Tells us if this cluster is VETOed yes/no for VETO cluster condition.
  
  // Processing the R3BNeutronTracker Results:
  Int_t N_Reconstructed_PrimPoints;         // Total number of primary interaction points by the tracker within a single event.
  Double_t* T_Reconstructed_PrimPoints;     // NOTE: Dynamical array for Amount of reconstructed points: Time coordinate of that point in LAB-frame [ns].
  Double_t* X_Reconstructed_PrimPoints;     // NOTE: Dynamical array for Amount of reconstructed points: X coordinate of that point in LAB-frame [cm].
  Double_t* Y_Reconstructed_PrimPoints;     // NOTE: Dynamical array for Amount of reconstructed points: Y coordinate of that point in LAB-frame [cm].
  Double_t* Z_Reconstructed_PrimPoints;     // NOTE: Dynamical array for Amount of reconstructed points: Z coordinate of that point in LAB-frame [cm].
  Bool_t* Reconstructed_PrimPoints_IsVETOed;// NOTE: Dynamical array for Amount of reconstructed points: This reconstructed point is VETOed yes/no.
  Int_t* TrackID_Reconstructed_PrimPoints;  // NOTE: Dynamical array for Amount of reconstructed points: TrackID of the primary particle that has its primary MC interaction point clostest to this point.
                                            // Closest means that x^2 + y^2 + z^2 + (ct)^2 is minimal. for each reconstructed point a single primary interaction point is found by minimizing
                                            // this distance. If a primary interaction point is assigned to multiple reconstructed points, the smallest distance is kept and the other reconstructed
                                            // points are re-assigned. If no primary interaction point can be found for a reconstructed point, then this value becomes -1.
  
  // Leafs on other detectors:
  Int_t Ntracks_STaRTrack;                  // Number of tracks in STaRTRack for this event [int]
  Int_t Nhits_CALIFA;                       // Number of hits in CALIFA for this event. [int]
  
    // Full VETO condition:
  Bool_t* AllBars_IsVETOed;      // Static array for all bars in NeuLAND: This bar states if the bar is VETOed yes/no.
  
  // NOTE: Add new leafs to the tree here!
  
};