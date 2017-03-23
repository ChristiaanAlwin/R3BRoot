// This function performs the connection to the AnalysisTree.
// It loads all the leafs into the AnaLeafs_Structure so that values from 
// the tree can be accessed from the structure in the rest of the process.
// A check will be performed whether the connection to the tree and
// to the leafs was succesful.

// Written by C. A. Douma, for the R3B-collaboration.

// inclusion of C++ headers:
#include <iostream>

// Inclusion of required hearders:
#include "TTree.h"
#include "TLeaf.h"
#include "TH1D.h"
#include "TH1I.h"
#include "TObjString.h"
#include "TString.h"
#include "TFile.h"
#include "TSystem.h"

// Inclusion of the AnaLeafs_Structure header:
#include "AnaLeafs_Structure.h"

// Standard namespace:
using namespace std;

// Definition of the function:
Bool_t AnaLeafs_Connect(AnaLeafs_Structure* AnaLeafs, Int_t const Counter)
{
   // The idea is that a pointer to a big Leaf_Structure called AnaLeafs is
   // passed as argument to this function. NOTE: only a pointer should be
   // passed, not the structure itself. This causes the content of the 
   // structure to be modified outside of the scope of this function, exactly
   // as we like it!
  
   // The second argument is the counter. This identifies which .root-file
   // we exactly whish to connect to. Since only a pointer is passed and
   // not the structure itself, no outputs are required.
  
   // We do however give a boolian as output that tells us whether the 
   // connection was succesful or not.
  
   // All we do in this function is to fill the pointers to the leafs so that the tree
   // can be read out convieniently.
  
   // ================================================================================================
   // We begin with defining the boolian for the test:
   Bool_t Connection = kFALSE;
   cout << "=============== Creating connection the AnalysisTree...\n\n";
   
   // ================================================================================================
   // Now, our first task is to connect to the trees itself.
  
   // We begin by connecting to the Inputs.root-file. Check if this file exists:
   FileStat_t stom1;
   FileStat_t stom2;
   Int_t inputs_test = gSystem->GetPathInfo("../../Data/Inputs/Inputs.root",stom1);
   
   if (inputs_test==1)
   {
     cout << "### ERROR: The Inputs.root-file does not exist! Create it first!\n\n";
   }
   else
   {
   
   // Connect to the Inputs.root file:
   TFile* inputs = new TFile("../../Data/Inputs/Inputs.root","read");
   cout << "Connection to the Inputs.root file was succesful.\n\n";
   
   // Get the data from this file:
   TH1D* Doubles = (TH1D*) inputs->Get("Doubles");
   TH1I* Integers = (TH1I*) inputs->Get("Integers");
   
   // Then we must obtain the string for the output-filename:
   TObjString* Output_Name_Obj = (TObjString*) inputs->Get("Output_Name");
   TString Output_Name = Output_Name_Obj->GetString();
   
   // Then we also need a string for the run-number:
   TString st = "";
   TString Nr_Run_str = st.Itoa(Counter,10);
   
   // Finally we can then build the complete filename:
   TString OutFile = "../../Data/Analysis/" + Output_Name + "_run=" + Nr_Run_str + ".root";
   
   // So now we can connect to the Analysis Tree. First check if it exists:
   Int_t f_test = gSystem->GetPathInfo(OutFile,stom2);
   
   if (f_test>0)
   {
     cout << "### ERROR: The Analysis file you want does not exist!\n\n";
   }
   else
   {
       
   //Then connect to the file:
   AnaLeafs->TheFile = new TFile(OutFile,"read");
   cout << "Connection to the Analysis file was succesful.\n\n";
   
   // Now connect to the trees inside these files:
   AnaLeafs->TheTree = (TTree*) AnaLeafs->TheFile->Get("AnalysisTree");
   
   // Now if this is succesful, we can finally continue with connecting to the leafs:
   if (AnaLeafs->TheTree==0)
   {
     cout << "### ERROR: The AnalysisTree inside the file does not exist!\n\n";
   }
   else
   {
     cout << "Connection to the AnalysisTree was succesful.\n\n";
     
   // Now before we connect, we will first define a counter for the errors:
   Int_t Errors = 0;
   
   // =================================================================================
   // Now we will connect to the leafs of the analysis tree:

   // Trigger:
   AnaLeafs->TOF_HorizontalHit = AnaLeafs->TheTree->FindLeaf("TOF_HorizontalHit");
   AnaLeafs->Full_Trigger = AnaLeafs->TheTree->FindLeaf("Full_Trigger");
   
   // Identification of primary particles:
   AnaLeafs->Nprims_ParticleGun = AnaLeafs->TheTree->FindLeaf("Nprims_ParticleGun");
   AnaLeafs->Nprims_NeuLAND = AnaLeafs->TheTree->FindLeaf("Nprims_NeuLAND");
   AnaLeafs->Prim_Detected = AnaLeafs->TheTree->FindLeaf("Prim_Detected");
   AnaLeafs->Prim_Resolved = AnaLeafs->TheTree->FindLeaf("Prim_Resolved");
   AnaLeafs->Prim_TrackID = AnaLeafs->TheTree->FindLeaf("Prim_TrackID");
   AnaLeafs->Prim_PDG = AnaLeafs->TheTree->FindLeaf("Prim_PDG");
   AnaLeafs->Prim_E = AnaLeafs->TheTree->FindLeaf("Prim_E");
   AnaLeafs->Prim_IsVETOed = AnaLeafs->TheTree->FindLeaf("Prim_IsVETOed");
   AnaLeafs->Prim_IsFromTarget = AnaLeafs->TheTree->FindLeaf("Prim_IsFromTarget");
   AnaLeafs->Prim_E_Detected = AnaLeafs->TheTree->FindLeaf("Prim_E_Detected");
   AnaLeafs->Prim_MCinteraction_T = AnaLeafs->TheTree->FindLeaf("Prim_MCinteraction_T");
   AnaLeafs->Prim_MCinteraction_X = AnaLeafs->TheTree->FindLeaf("Prim_MCinteraction_X");
   AnaLeafs->Prim_MCinteraction_Y = AnaLeafs->TheTree->FindLeaf("Prim_MCinteraction_Y");
   AnaLeafs->Prim_MCinteraction_Z = AnaLeafs->TheTree->FindLeaf("Prim_MCinteraction_Z");
   AnaLeafs->Prim_MCproduction_T = AnaLeafs->TheTree->FindLeaf("Prim_MCproduction_T");
   AnaLeafs->Prim_MCproduction_X = AnaLeafs->TheTree->FindLeaf("Prim_MCproduction_X");
   AnaLeafs->Prim_MCproduction_Y = AnaLeafs->TheTree->FindLeaf("Prim_MCproduction_Y");
   AnaLeafs->Prim_MCproduction_Z = AnaLeafs->TheTree->FindLeaf("Prim_MCproduction_Z");

   // Identification of backscattering events:
   AnaLeafs->PrimInteraction = AnaLeafs->TheTree->FindLeaf("PrimInteraction");
   AnaLeafs->BackInteraction = AnaLeafs->TheTree->FindLeaf("BackInteraction");
   AnaLeafs->LoopInteraction = AnaLeafs->TheTree->FindLeaf("LoopInteraction");
   AnaLeafs->Energetic_Prim = AnaLeafs->TheTree->FindLeaf("Energetic_Prim");
   AnaLeafs->Energetic_Back = AnaLeafs->TheTree->FindLeaf("Energetic_Back");
   AnaLeafs->Overal_Prim = AnaLeafs->TheTree->FindLeaf("Overal_Prim");
   AnaLeafs->Overal_Back = AnaLeafs->TheTree->FindLeaf("Overal_Back");
   AnaLeafs->Overal_EPrim = AnaLeafs->TheTree->FindLeaf("Overal_EPrim");
   AnaLeafs->Overal_EBack = AnaLeafs->TheTree->FindLeaf("Overal_EBack");
   AnaLeafs->VETO_TrackTOF = AnaLeafs->TheTree->FindLeaf("VETO_TrackTOF");
   AnaLeafs->VETO_TrackTOF_Res = AnaLeafs->TheTree->FindLeaf("VETO_TrackTOF_Res");
   AnaLeafs->VETO_TotalTrackTOF = AnaLeafs->TheTree->FindLeaf("VETO_TotalTrackTOF");
   AnaLeafs->VETO_TotalTrackTOF_Res = AnaLeafs->TheTree->FindLeaf("VETO_TotalTrackTOF_Res");
  
   // QDC and TDC of NeuLAND and VETO:
   AnaLeafs->NbarsFired_NeuLAND = AnaLeafs->TheTree->FindLeaf("NbarsFired_NeuLAND");
   AnaLeafs->Neu_BarNumber_Fired = AnaLeafs->TheTree->FindLeaf("Neu_BarNumber_Fired");
   AnaLeafs->Neu_Edep_Fired = AnaLeafs->TheTree->FindLeaf("Neu_Edep_Fired");
   AnaLeafs->Neu_TOF_Fired = AnaLeafs->TheTree->FindLeaf("Neu_TOF_Fired");
   AnaLeafs->Neu_TOF_Fired_Res = AnaLeafs->TheTree->FindLeaf("Neu_TOF_Fired_Res");
   AnaLeafs->Neu_Primary_TrackID = AnaLeafs->TheTree->FindLeaf("Neu_Primary_TrackID");
   AnaLeafs->Neu_Bar_IsVETOed = AnaLeafs->TheTree->FindLeaf("Neu_Bar_IsVETOed");
   AnaLeafs->Neu_TotalTOF = AnaLeafs->TheTree->FindLeaf("Neu_TotalTOF");
   AnaLeafs->Neu_TotalTOF_Res = AnaLeafs->TheTree->FindLeaf("Neu_TotalTOF_Res");

   AnaLeafs->NbarsFired_VETO = AnaLeafs->TheTree->FindLeaf("NbarsFired_VETO");
   AnaLeafs->VETO_BarNumber_Fired = AnaLeafs->TheTree->FindLeaf("VETO_BarNumber_Fired");
   AnaLeafs->VETO_Edep_Fired = AnaLeafs->TheTree->FindLeaf("VETO_Edep_Fired");
   AnaLeafs->VETO_TOF_Fired = AnaLeafs->TheTree->FindLeaf("VETO_TOF_Fired");
   AnaLeafs->VETO_TOF_Fired_Res = AnaLeafs->TheTree->FindLeaf("VETO_TOF_Fired_Res");
   AnaLeafs->VETO_Primary_TrackID = AnaLeafs->TheTree->FindLeaf("VETO_Primary_TrackID");
   AnaLeafs->VETO_TOF_index = AnaLeafs->TheTree->FindLeaf("VETO_TOF_index");
   AnaLeafs->VETO_TotalTOF = AnaLeafs->TheTree->FindLeaf("VETO_TotalTOF");
   AnaLeafs->VETO_TotalTOF_Res = AnaLeafs->TheTree->FindLeaf("VETO_TotalTOF_Res");
   
   // Quantification of secondary particles:
   AnaLeafs->Secondary_Nprotons = AnaLeafs->TheTree->FindLeaf("Secondary_Nprotons");
   AnaLeafs->Secondary_Nneutrons = AnaLeafs->TheTree->FindLeaf("Secondary_Nneutrons");
   AnaLeafs->Secondary_Nelectrons = AnaLeafs->TheTree->FindLeaf("Secondary_Nelectrons");
   AnaLeafs->Secondary_Ngammas = AnaLeafs->TheTree->FindLeaf("Secondary_Ngammas");
   AnaLeafs->Secondary_Npions = AnaLeafs->TheTree->FindLeaf("Secondary_Npions");
   AnaLeafs->Secondary_Nmuons = AnaLeafs->TheTree->FindLeaf("Secondary_Nmuons");
   AnaLeafs->Secondary_Nalphas = AnaLeafs->TheTree->FindLeaf("Secondary_Nalphas");
   AnaLeafs->Secondary_NCions = AnaLeafs->TheTree->FindLeaf("Secondary_NCions");
   AnaLeafs->Secondary_Nfragments = AnaLeafs->TheTree->FindLeaf("Secondary_Nfragments");
   AnaLeafs->Secondary_NallParticles = AnaLeafs->TheTree->FindLeaf("Secondary_NallParticles");
   AnaLeafs->Secondary_Particles = AnaLeafs->TheTree->FindLeaf("Secondary_Particles");
   
   // Processing the ClusterFinder Results:
   AnaLeafs->NeuLAND_Nclusters = AnaLeafs->TheTree->FindLeaf("NeuLAND_Nclusters");
   AnaLeafs->NeuLAND_ClusterX = AnaLeafs->TheTree->FindLeaf("NeuLAND_ClusterX");
   AnaLeafs->NeuLAND_ClusterY = AnaLeafs->TheTree->FindLeaf("NeuLAND_ClusterY");
   AnaLeafs->NeuLAND_ClusterZ = AnaLeafs->TheTree->FindLeaf("NeuLAND_ClusterZ");
   AnaLeafs->NeuLAND_ClusterTime = AnaLeafs->TheTree->FindLeaf("NeuLAND_ClusterTime");
   AnaLeafs->NeuLAND_ClusterEnergy = AnaLeafs->TheTree->FindLeaf("NeuLAND_ClusterEnergy");
   AnaLeafs->NeuLAND_ClusterSize = AnaLeafs->TheTree->FindLeaf("NeuLAND_ClusterSize"); 
   AnaLeafs->NeuLAND_Cluster_TrackID = AnaLeafs->TheTree->FindLeaf("NeuLAND_Cluster_TrackID");
   AnaLeafs->NeuLAND_Cluster_IsVETOed = AnaLeafs->TheTree->FindLeaf("NeuLAND_Cluster_IsVETOed");
   
   // Processing the R3BNeutronTracker Results:
   AnaLeafs->N_Reconstructed_PrimPoints = AnaLeafs->TheTree->FindLeaf("N_Reconstructed_PrimPoints");
   AnaLeafs->T_Reconstructed_PrimPoints = AnaLeafs->TheTree->FindLeaf("T_Reconstructed_PrimPoints");
   AnaLeafs->X_Reconstructed_PrimPoints = AnaLeafs->TheTree->FindLeaf("X_Reconstructed_PrimPoints");
   AnaLeafs->Y_Reconstructed_PrimPoints = AnaLeafs->TheTree->FindLeaf("Y_Reconstructed_PrimPoints");
   AnaLeafs->Z_Reconstructed_PrimPoints = AnaLeafs->TheTree->FindLeaf("Z_Reconstructed_PrimPoints");
   AnaLeafs->TrackID_Reconstructed_PrimPoints = AnaLeafs->TheTree->FindLeaf("TrackID_Reconstructed_PrimPoints");
   AnaLeafs->Reconstructed_PrimPoints_IsVETOed = AnaLeafs->TheTree->FindLeaf("Reconstructed_PrimPoints_IsVETOed");
   
   // Overall VETO condition:
   AnaLeafs->AllBars_IsVETOed = AnaLeafs->TheTree->FindLeaf("AllBars_IsVETOed");
   
   // NOTE: add your new leafs here!
   
   // =================================================================================
   // Now check for possible errors:
   
   if (AnaLeafs->TOF_HorizontalHit==0) {Errors = Errors + 1;}
   if (AnaLeafs->Full_Trigger==0) {Errors = Errors + 1;}
   
   if (AnaLeafs->Nprims_ParticleGun==0) {Errors = Errors + 1;}
   if (AnaLeafs->Nprims_NeuLAND==0) {Errors = Errors + 1;}
   if (AnaLeafs->Prim_Detected==0) {Errors = Errors + 1;}
   if (AnaLeafs->Prim_Resolved==0) {Errors = Errors + 1;}
   if (AnaLeafs->Prim_TrackID==0) {Errors = Errors + 1;}
   if (AnaLeafs->Prim_PDG==0) {Errors = Errors + 1;}
   if (AnaLeafs->Prim_E==0) {Errors = Errors + 1;}
   if (AnaLeafs->Prim_IsVETOed==0) {Errors = Errors + 1;}
   if (AnaLeafs->Prim_IsFromTarget==0) {Errors = Errors + 1;}
   if (AnaLeafs->Prim_E_Detected==0) {Errors = Errors + 1;}
   if (AnaLeafs->Prim_MCinteraction_T==0) {Errors = Errors + 1;}
   if (AnaLeafs->Prim_MCinteraction_X==0) {Errors = Errors + 1;}
   if (AnaLeafs->Prim_MCinteraction_Y==0) {Errors = Errors + 1;}
   if (AnaLeafs->Prim_MCinteraction_Z==0) {Errors = Errors + 1;}
   if (AnaLeafs->Prim_MCproduction_T==0) {Errors = Errors + 1;}
   if (AnaLeafs->Prim_MCproduction_X==0) {Errors = Errors + 1;}
   if (AnaLeafs->Prim_MCproduction_Y==0) {Errors = Errors + 1;}
   if (AnaLeafs->Prim_MCproduction_Z==0) {Errors = Errors + 1;}
   
   if (AnaLeafs->PrimInteraction==0) {Errors = Errors + 1;}
   if (AnaLeafs->BackInteraction==0) {Errors = Errors + 1;}
   if (AnaLeafs->LoopInteraction==0) {Errors = Errors + 1;}
   if (AnaLeafs->Energetic_Prim==0) {Errors = Errors + 1;}
   if (AnaLeafs->Energetic_Back==0) {Errors = Errors + 1;}
   if (AnaLeafs->Overal_Prim==0) {Errors = Errors + 1;}
   if (AnaLeafs->Overal_Back==0) {Errors = Errors + 1;}
   if (AnaLeafs->Overal_EPrim==0) {Errors = Errors + 1;}
   if (AnaLeafs->Overal_EBack==0) {Errors = Errors + 1;}
   if (AnaLeafs->VETO_TrackTOF==0) {Errors = Errors + 1;}
   if (AnaLeafs->VETO_TrackTOF_Res==0) {Errors = Errors + 1;}
   if (AnaLeafs->VETO_TotalTrackTOF==0) {Errors = Errors + 1;}
   if (AnaLeafs->VETO_TotalTrackTOF_Res==0) {Errors = Errors + 1;}
   
   if (AnaLeafs->NbarsFired_NeuLAND==0) {Errors = Errors + 1;}
   if (AnaLeafs->Neu_BarNumber_Fired==0) {Errors = Errors + 1;}
   if (AnaLeafs->Neu_Edep_Fired==0) {Errors = Errors + 1;}
   if (AnaLeafs->Neu_TOF_Fired==0) {Errors = Errors + 1;}
   if (AnaLeafs->Neu_TOF_Fired_Res==0) {Errors = Errors + 1;}
   if (AnaLeafs->Neu_Primary_TrackID==0) {Errors = Errors + 1;}
   if (AnaLeafs->Neu_Bar_IsVETOed==0) {Errors = Errors + 1;}
   if (AnaLeafs->Neu_TotalTOF==0) {Errors = Errors + 1;}
   if (AnaLeafs->Neu_TotalTOF_Res==0) {Errors = Errors + 1;}
   
   if (AnaLeafs->NbarsFired_VETO==0) {Errors = Errors + 1;}
   if (AnaLeafs->VETO_BarNumber_Fired==0) {Errors = Errors + 1;}
   if (AnaLeafs->VETO_Edep_Fired==0) {Errors = Errors + 1;}
   if (AnaLeafs->VETO_TOF_Fired==0) {Errors = Errors + 1;}
   if (AnaLeafs->VETO_TOF_Fired_Res==0) {Errors = Errors + 1;}
   if (AnaLeafs->VETO_Primary_TrackID==0) {Errors = Errors + 1;}
   if (AnaLeafs->VETO_TOF_index==0) {Errors = Errors + 1;}
   if (AnaLeafs->VETO_TotalTOF==0) {Errors = Errors + 1;}
   if (AnaLeafs->VETO_TotalTOF_Res==0) {Errors = Errors + 1;}
   
   if (AnaLeafs->Secondary_Nprotons==0) {Errors = Errors + 1;}
   if (AnaLeafs->Secondary_Nneutrons==0) {Errors = Errors + 1;}
   if (AnaLeafs->Secondary_Nelectrons==0) {Errors = Errors + 1;}
   if (AnaLeafs->Secondary_Ngammas==0) {Errors = Errors + 1;}
   if (AnaLeafs->Secondary_Npions==0) {Errors = Errors + 1;}
   if (AnaLeafs->Secondary_Nmuons==0) {Errors = Errors + 1;}
   if (AnaLeafs->Secondary_Nalphas==0) {Errors = Errors + 1;}
   if (AnaLeafs->Secondary_NCions==0) {Errors = Errors + 1;}
   if (AnaLeafs->Secondary_Nfragments==0) {Errors = Errors + 1;}
   if (AnaLeafs->Secondary_NallParticles==0) {Errors = Errors + 1;}
   if (AnaLeafs->Secondary_Particles==0) {Errors = Errors + 1;}
   
   if (AnaLeafs->NeuLAND_Nclusters==0) {Errors = Errors + 1;} 
   if (AnaLeafs->NeuLAND_ClusterX==0) {Errors = Errors + 1;}  
   if (AnaLeafs->NeuLAND_ClusterY==0) {Errors = Errors + 1;}
   if (AnaLeafs->NeuLAND_ClusterZ==0) {Errors = Errors + 1;}  
   if (AnaLeafs->NeuLAND_ClusterTime==0) {Errors = Errors + 1;}  
   if (AnaLeafs->NeuLAND_ClusterEnergy==0) {Errors = Errors + 1;}  
   if (AnaLeafs->NeuLAND_ClusterSize==0) {Errors = Errors + 1;}    
   if (AnaLeafs->NeuLAND_Cluster_TrackID==0) {Errors = Errors + 1;}  
   if (AnaLeafs->NeuLAND_Cluster_IsVETOed==0) {Errors = Errors + 1;} 
   
   if (AnaLeafs->N_Reconstructed_PrimPoints==0) {Errors = Errors + 1;}
   if (AnaLeafs->T_Reconstructed_PrimPoints==0) {Errors = Errors + 1;}
   if (AnaLeafs->X_Reconstructed_PrimPoints==0) {Errors = Errors + 1;}
   if (AnaLeafs->Y_Reconstructed_PrimPoints==0) {Errors = Errors + 1;}
   if (AnaLeafs->Z_Reconstructed_PrimPoints==0) {Errors = Errors + 1;}
   if (AnaLeafs->TrackID_Reconstructed_PrimPoints==0) {Errors = Errors + 1;}
   if (AnaLeafs->Reconstructed_PrimPoints_IsVETOed==0) {Errors = Errors + 1;}
   
//   if (AnaLeafs->AllBars_IsVETOed==0) {Errors = Errors + 1;}
   
   // NOTE: add your new leafs here!
   
   // ================================================================================
   // Now define the boolian:
   if (Errors==0) {Connection = kTRUE;}
   
   // Give an error report:
   if (Connection) 
   {
       cout << "Connection to all leafs in the AnalysisTree is succesful.\n\n";
   }
   else
   {
       cout << "### ERROR: There are " << Errors << " Leafs in the AnalysisTree\n"
            << "### ERROR: that we could not connect to! Recheck your code!\n\n";
   }
   
   // Now close all blocks:
   }
   }
   }
     
   // Now return the boolian:
   return Connection;

   // Done!   
   
}




