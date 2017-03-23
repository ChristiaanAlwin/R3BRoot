// This hearder file contains the definition of a structure
// containing all the leafs of the Analysis Tree.

// Written by C. A. Douma:

// Inclusion of required hearders:
#include "TTree.h"
#include "TLeaf.h"

// Definition of the structure:
struct AnaLeafs_Structure {

   // The file:
   TFile* TheFile;
  
   // The AnalysisTree:
   TTree* TheTree;
   
   // Trigger:
   TLeaf* TOF_HorizontalHit;      // Average hit position in horizontal direction along the TOF plane. [cm].
   TLeaf* Full_Trigger;           // Overall setup trigger. kTRUE means that the trigger fired.

   // The leafs: 
   TLeaf* Nprims_ParticleGun;	  // Number of particles fired from the beam
   TLeaf* Nprims_NeuLAND;	  // Number of particles determined as primary w.r.t. NeuLAND. These are NOT the primaries from the beam!
   TLeaf* Prim_Detected;	  // NOTE: Dynamic array for NeuLAND primary particles: detected by NeuLAND yes/no.
   TLeaf* Prim_Resolved;	  // NOTE: Dynamic array for NeuLAND primary particles: Resolved by NeuLAND NeutronTracker yes/no.
   TLeaf* Prim_TrackID;		  // NOTE: Dynamic array for NeuLAND primary particles: Monte Carlo trackID of these particles.
   TLeaf* Prim_PDG;		  // NOTE: Dynamic array for NeuLAND primary particles: PDG code of these particles.
   TLeaf* Prim_E;		  // NOTE: Dynamic array for NeuLAND primary particles: Energy of these particles [MeV].
   TLeaf* Prim_IsVETOed;          // NOTE: Dynamic array for NeuLAND primary particles: Whether or not the particle is VETOed.
   TLeaf* Prim_IsFromTarget;      // NOTE: Dynamic array for NeuLAND primary particles: Whether or not the particle comes directly from the target.
   TLeaf* Prim_E_Detected;        // NOTE: Dynamic array for NeuLAND primary particles: Reconstructed energy of these particles [MeV]. For segmented VETO condition this is Sum(Edep per bar). Bars are tracked by G4MotherId.
   TLeaf* Prim_MCinteraction_T;   // NOTE: Dynamic array for NeuLAND primary particles: The first Monte Carlo interaction point of that primary particle (time [ns]).
   TLeaf* Prim_MCinteraction_X;   // NOTE: Dynamic array for NeuLAND primary particles: The first Monte Carlo interaction point of that primary particle (x-point [cm]).
   TLeaf* Prim_MCinteraction_Y;   // NOTE: Dynamic array for NeuLAND primary particles: The first Monte Carlo interaction point of that primary particle (y-point [cm]).
   TLeaf* Prim_MCinteraction_Z;   // NOTE: Dynamic array for NeuLAND primary particles: The first Monte Carlo interaction point of that primary particle (z-point [cm]). 
   TLeaf* Prim_MCproduction_T;    // NOTE: Dynamic array for NeuLAND primary particles: The first Monte Carlo production point of that primary particle (time [ns]).
   TLeaf* Prim_MCproduction_X;    // NOTE: Dynamic array for NeuLAND primary particles: The first Monte Carlo production point of that primary particle (x-point [cm]).
   TLeaf* Prim_MCproduction_Y;    // NOTE: Dynamic array for NeuLAND primary particles: The first Monte Carlo production point of that primary particle (y-point [cm]).
   TLeaf* Prim_MCproduction_Z;    // NOTE: Dynamic array for NeuLAND primary particles: The first Monte Carlo production point of that primary particle (z-point [cm]). 
 
   // Identification of backscattering events:
   TLeaf* PrimInteraction;	  // Static array for all bars in the VETO: This bar contains a primary interaction yes/no
   TLeaf* BackInteraction;        // Static array for all bars in the VETO: This bar contains a backscattering interaction yes/no
   TLeaf* LoopInteraction;        // Static array for all bars in the VETO: This bar contains an interaction that leaves the VETO at the side of the target, but then loops back to NeuLAND yes/no
   TLeaf* Energetic_Prim;         // Static array for all bars in the VETO: This bar contains a primary interaction with energy deposition above thhreshold yes/no. 
   TLeaf* Energetic_Back;         // Static array for all bars in the VETO: This bar contains backscattering or loop interaction with energy deposition above thhreshold yes/no.
   TLeaf* Overal_Prim;            // This boolian is kTRUE if one of the boolians in PrimInteraction is kTRUE.
   TLeaf* Overal_Back;            // This boolian is kTRUE if one of the boolians in PrimInteraction OR in LoopInteraction is kTRUE.
   TLeaf* Overal_EPrim;           // This boolain is kTRUE if one of the boolians in Energetic_Prim is kTRUE;
   TLeaf* Overal_EBack;           // This boolain is kTRUE if one of the boolians in Energetic Back is kTRUE;
   TLeaf* VETO_TrackTOF;          // Static array for all bars in the VETO: TOF per bar taking primary/backscattering into account [ns]. Equals -100.0 for bars without energetic interacton.
   TLeaf* VETO_TrackTOF_Res;      // Static array for all bars in the VETO: TOF with time resolution per bar taking primary/backscattering into account [ns]. Equals -100.0 for bars without energetic interacton.
   TLeaf* VETO_TotalTrackTOF;     // Full VETO TOF taking primary/backscattering into account [ns]. Equals -100.0 when there are no interactions.
   TLeaf* VETO_TotalTrackTOF_Res; // Full VETO TOF with time resolution taking primary/backscattering into account [ns]. Equals -100.0 when there are no interactions.
  
   // QDC and TDC of NeuLAND and VETO:
   TLeaf* NbarsFired_NeuLAND;     // Number of bars inside NeuLAND that came above the energy threshold.
   TLeaf* Neu_BarNumber_Fired;    // NOTE: Dynamic array for NeuLAND numberof bars that fired: NeuLAND number of the specific bar that fired. [int]
   TLeaf* Neu_Edep_Fired;         // NOTE: Dynamic array for NeuLAND numberof bars that fired: Total enegy deposition of the specific bar that fired. [MeV]
   TLeaf* Neu_TOF_Fired;          // NOTE: Dynamic array for NeuLAND numberof bars that fired: TOF of the specific bar that fired. [ns]
   TLeaf* Neu_TOF_Fired_Res;      // NOTE: Dynamic array for NeuLAND numberof bars that fired: TOF of the specific bar that fired. [ns]
   TLeaf* Neu_Primary_TrackID;	  // NOTE: Dynamic array for NeuLAND number of bars that fired: TrackID of the Primary particle belonging to the FIRST particle that hits the fired bar (on TOF basis).
   TLeaf* Neu_Bar_IsVETOed;       // NOTE: Dynamic array for NeuLAND number of bars that fired: Determine Whether this bar is VETOed yes/no.
   TLeaf* Neu_TotalTOF;           // Minimum TOF over all Fired bars for NeuLAND [ns].
   TLeaf* Neu_TotalTOF_Res;       // Minimum TOF over all Fired bars for NeuLAND with time resolution [ns].

   TLeaf* NbarsFired_VETO;        // Number of bars inside VETO that came above the energy threshold.
   TLeaf* VETO_BarNumber_Fired;   // NOTE: Dynamic array for VETO numberof bars that fired: VETO number of the specific bar that fired. [int]
   TLeaf* VETO_Edep_Fired;        // NOTE: Dynamic array for VETO numberof bars that fired: Total enegy deposition of the specific bar that fired. [MeV]
   TLeaf* VETO_TOF_Fired;         // NOTE: Dynamic array for VETO numberof bars that fired: TOF of the specific bar that fired. [ns]
   TLeaf* VETO_TOF_Fired_Res;     // NOTE: Dynamic array for VETO numberof bars that fired: TOF of the specific bar that fired. [ns]
   TLeaf* VETO_Primary_TrackID;	  // NOTE: Dynamic array for VETO number of bars that fired: TrackID of the Primary particle belonging to the FIRST particle that hits the fired bar (on TOF basis).
   TLeaf* VETO_TOF_index;         // NOTE: Dynamic array for VETO number of bars that fired: C++ index of veto_segPoint leafs that is used to compute TOF.
   TLeaf* VETO_TotalTOF;          // Minimum TOF over all Fired bars for VETO [ns].
   TLeaf* VETO_TotalTOF_Res;      // Minimum TOF over all Fired bars for VETO with time resolution [ns].
   
   // Quantification of secondary particles:
   TLeaf* Secondary_Nprotons;     // 1. Static array of size 5: Amount of registered secondary protons in this specific event. Values 1=all, 2=aimed, 3=reached, 4=hit, 5=detected.
   TLeaf* Secondary_Nneutrons;    // 2. Static array of size 5: Amount of registered secondary neutrons in this specific event. Values 1=all, 2=aimed, 3=reached, 4=hit, 5=detected.
   TLeaf* Secondary_Nelectrons;   // 3. Static array of size 5: Amount of registered secondary electrons in this specific event. Values 1=all, 2=aimed, 3=reached, 4=hit, 5=detected.
   TLeaf* Secondary_Ngammas;      // 4. Static array of size 5: Amount of registered secondary gammas in this specific event. Values 1=all, 2=aimed, 3=reached, 4=hit, 5=detected.
   TLeaf* Secondary_Npions;       // 5. Static array of size 5: Amount of registered secondary pions in this specific event. Values 1=all, 2=aimed, 3=reached, 4=hit, 5=detected.
   TLeaf* Secondary_Nmuons;       // 6. Static array of size 5: Amount of registered secondary muons in this specific event. Values 1=all, 2=aimed, 3=reached, 4=hit, 5=detected.
   TLeaf* Secondary_Nalphas;      // 7. Static array of size 5: Amount of registered secondary alphas in this specific event. Values 1=all, 2=aimed, 3=reached, 4=hit, 5=detected.
   TLeaf* Secondary_NCions;       // 8. Static array of size 5: Amount of registered secondary Cions in this specific event. Values 1=all, 2=aimed, 3=reached, 4=hit, 5=detected.
   TLeaf* Secondary_Nfragments;   // 9. Static array of size 5: Amount of registered secondary fragments in this specific event. Values 1=all, 2=aimed, 3=reached, 4=hit, 5=detected.
   TLeaf* Secondary_NallParticles;// 10.Static array of size 5: Amount of registered secondary particles in this specific event. Values 1=all, 2=aimed, 3=reached, 4=hit, 5=detected.
   TLeaf* Secondary_Particles;	  // This boolian states whether the event registered secondary particles in the VETO yes/no.
   
   // Processing the ClusterFinder Results:
   TLeaf* NeuLAND_Nclusters;        // Total number of clusters found in this event.
   TLeaf* NeuLAND_ClusterX;         // NOTE: Dynamical array for the number of Clusters: x-coordinate of the starting point of this cluster. [cm]
   TLeaf* NeuLAND_ClusterY;         // NOTE: Dynamical array for the number of Clusters: y-coordinate of the starting point of this cluster. [cm]
   TLeaf* NeuLAND_ClusterZ;         // NOTE: Dynamical array for the number of Clusters: z-coordinate of the starting point of this cluster. [cm]
   TLeaf* NeuLAND_ClusterTime;      // NOTE: Dynamical array for the number of Clusters: t-coordinate of the starting point of this cluster. [ns]
   TLeaf* NeuLAND_ClusterEnergy;    // NOTE: Dynamical array for the number of Clusters: Total Energy inside this cluster. [MeV]
   TLeaf* NeuLAND_ClusterSize;      // NOTE: Dynamical array for the number of Clusters: Number of bars/hits/digis inside this cluster [int]   
   TLeaf* NeuLAND_Cluster_TrackID;  // NOTE: Dynamical array for the number of Clusters: TrackID of the primary particle this cluster belongs to [int].
   TLeaf* NeuLAND_Cluster_IsVETOed; // NOTE: Dynamical array for the number of Clusters: Tells us if this cluster is VETOed yes/no for VETO cluster condition.
   
   // Processing the R3BNeutronTracker Results:
   TLeaf* N_Reconstructed_PrimPoints;     // Total number of primary interaction points by the tracker within a single event.
   TLeaf* T_Reconstructed_PrimPoints;     // NOTE: Dynamical array for Amount of reconstructed points: Time coordinate of that point in LAB-frame [ns].
   TLeaf* X_Reconstructed_PrimPoints;     // NOTE: Dynamical array for Amount of reconstructed points: X coordinate of that point in LAB-frame [cm].
   TLeaf* Y_Reconstructed_PrimPoints;     // NOTE: Dynamical array for Amount of reconstructed points: Y coordinate of that point in LAB-frame [cm].
   TLeaf* Z_Reconstructed_PrimPoints;     // NOTE: Dynamical array for Amount of reconstructed points: Z coordinate of that point in LAB-frame [cm].
   TLeaf* Reconstructed_PrimPoints_IsVETOed; // NOTE: Dynamical array for Amount of reconstructed points: VETOed yes/no.
   TLeaf* TrackID_Reconstructed_PrimPoints;  // NOTE: Dynamical array for Amount of reconstructed points: TrackID of the primary particle that has its primary MC interaction point clostest to this point.
                                             // Closest means that x^2 + y^2 + z^2 + (ct)^2 is minimal. for each reconstructed point a single primary interaction point is found by minimizing
                                             // this distance. If a primary interaction point is assigned to multiple reconstructed points, the smallest distance is kept and the other reconstructed
                                             // points are re-assigned. If no primary interaction point can be found for a reconstructed point, then this value becomes -1.
  
   // Overall VETO:
   TLeaf* AllBars_IsVETOed;       // Static array for all bars in NeuLAND: This bar states if the bar is VETOed yes/no.
   
   // NOTE: Add new leafs here!
};




