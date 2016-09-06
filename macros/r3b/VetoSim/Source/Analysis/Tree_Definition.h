// This function performs the creation and definition of the analysis
// tree that contains the results of our analysis. It is performed
// in such a way that loading the values of your choice into the 
// members of the Tree_Structure will automatically fill the tree.
// NOTE: during runtime one has to re-set the branch addresses every time
// for the dynamical arrays. A test will be performed to see if the
// tree definition was succesful.

// Written by C. A. Douma, for the R3B-collaboration.

// inclusion of C++ headers:
#include <iostream>

// Inclusion of required hearders:
#include "TTree.h"
#include "TBranch.h"
#include "TLeaf.h"
#include "TH1D.h"
#include "TH1I.h"
#include "TObjString.h"
#include "TString.h"
#include "TFile.h"
#include "TSystem.h"

// Inclusion of the Tree_Structure header:
#include "Tree_Structure.h"

// Standard namespace:
using namespace std;

// Definition of the function:
Bool_t Tree_Definition(Tree_Structure* AnaTree, Int_t const Counter, Int_t const Series)
{
   // The idea is that a pointer to a big Tree_Structure called AnaTree is
   // passed as argument to this function. NOTE: only a pointer should be
   // passed, not the structure itself. This causes the content of the 
   // structure to be modified outside of the scope of this function, exactly
   // as we like it!
  
   // The second argument is the counter. This identifies which .root-file
   // we exactly whish to analyze. Since only a pointer is passed and
   // not the structure itself, no outputs are required. Series indicates
   // how we analyze the different simulation runs.
  
   // We do however give a boolian as output that tells us whether the 
   // definition of the tree was succesful.
  
   // All we do in this function is to create the tree and all branches and leafs.
   // the filling is done somewhere else.
  
   // ================================================================================================
   // We begin with defining the boolian for the test:
   Bool_t Definition = kFALSE;
   cout << "=============== Defining the analysis Tree...\n\n";
   
   // ================================================================================================
  
   // Now we connect to the Inputs.root-file to obtain some information that we need for
   // the creation of the tree:
   
   // Check if this file exists:
   FileStat_t stom1;
   Int_t inputs_test = gSystem->GetPathInfo("../../Data/Inputs/Inputs.root",stom1);
   
   if (inputs_test==1)
   {
     cout << "### ERROR: The Inputs.root-file does not exist! Create it first!\n\n";
   }
   else
   {
   
   // Now Connect to the Inputs.root file:
   TFile* inputs = new TFile("../../Data/Inputs/Inputs.root","read");
   cout << "Connection to the Inputs.root file was succesful.\n\n";
   
   // Get the data from this file:
   TH1D* Doubles = (TH1D*) inputs->Get("Doubles");
   TH1I* Integers = (TH1I*) inputs->Get("Integers");
   
   // The we must obtain the string for the output-filename:
   TObjString* Output_Name_Obj = (TObjString*) inputs->Get("Output_Name");
   TString Output_Name = Output_Name_Obj->GetString();
   
   // Then we also need a string for the run-number:
   TString st = "";
   TString Nr_Run_str = st.Itoa(Counter,10);
   
   // Finally we can then build the complete filename:
   TString OutFile = "../../Data/Analysis/" + Output_Name + "_run=" + Nr_Run_str + ".root";
   
   // Get some other inputs:
   Int_t Store_AllBars = Integers->GetBinContent(310);
   
   // ================================================================================================
   // Now we can begin with the creation of our tree. The first thing we do is to define the
   // filename that it should be stored under:
   AnaTree->FileName = OutFile;
   
   // Then we create the tree:
   AnaTree->TheFile = new TFile(OutFile,"recreate");
   AnaTree->TheTree = new TTree("AnalysisTree","AnalysisTree");
   
   // ================================================================================================
   // Next thing to do is to create our static arrays. We will create them with a "new" command.
   // We call them static, because their length does not change from event to event. Hence it is
   // better to create them now:
   
   // Get the total number of VETO bars:
   Int_t Nbars_Neu = (Integers->GetBinContent(2))*50;
   Int_t Nbars_VETO = (Integers->GetBinContent(3))*(Integers->GetBinContent(4));
   if (Series==3)
   {
     Double_t increase = TMath::Power(2.0,((Int_t) (Counter - 1)));
     Int_t increase_int = (Double_t) increase;
     Nbars_VETO = Nbars_VETO*increase_int;
   }
  
   TString Nbars_Neu_str = st.Itoa(Nbars_Neu,10);
   TString Nbars_VETO_str = st.Itoa(Nbars_VETO,10);
   
   // Create the arrays:
   AnaTree->PrimInteraction = new Bool_t[Nbars_VETO];
   AnaTree->BackInteraction = new Bool_t[Nbars_VETO];
   AnaTree->LoopInteraction = new Bool_t[Nbars_VETO];
   AnaTree->Energetic_Prim = new Bool_t[Nbars_VETO];
   AnaTree->Energetic_Back = new Bool_t[Nbars_VETO];
   AnaTree->VETO_TrackTOF = new Double_t[Nbars_VETO];
   AnaTree->VETO_TrackTOF_Res = new Double_t[Nbars_VETO];
   
   AnaTree->Secondary_Nprotons = new Int_t[5];
   AnaTree->Secondary_Nneutrons = new Int_t[5];
   AnaTree->Secondary_Nelectrons = new Int_t[5];
   AnaTree->Secondary_Ngammas = new Int_t[5];
   AnaTree->Secondary_Npions = new Int_t[5];
   AnaTree->Secondary_Nmuons = new Int_t[5];
   AnaTree->Secondary_Nalphas = new Int_t[5];
   AnaTree->Secondary_NCions = new Int_t[5];
   AnaTree->Secondary_Nfragments = new Int_t[5];
   AnaTree->Secondary_NallParticles = new Int_t[5];
   
   if (Store_AllBars==1)
   {
       AnaTree->AllBars_IsVETOed = new Bool_t[Nbars_Neu];
   }
   else
   {
       AnaTree->AllBars_IsVETOed = new Bool_t[0];
   }
  
   // NOTE: Add your static arrays here if you want!
   
   // ================================================================================================
   // Finally, we have to create the branches inside the tree and link them to the 
   // right members of the AnaTree structure:
   
   // Identification of primary particles:
   AnaTree->TheTree->Branch("Nprims_ParticleGun",&(AnaTree->Nprims_ParticleGun),"Nprims_ParticleGun/I");
   AnaTree->TheTree->Branch("Nprims_NeuLAND",&(AnaTree->Nprims_NeuLAND),"Nprims_NeuLAND/I");
   AnaTree->TheTree->Branch("Prim_Detected",AnaTree->Prim_Detected,"Prim_Detected[Nprims_NeuLAND]/B");
   AnaTree->TheTree->Branch("Prim_Resolved",AnaTree->Prim_Resolved,"Prim_Resolved[Nprims_NeuLAND]/B");
   AnaTree->TheTree->Branch("Prim_TrackID",AnaTree->Prim_TrackID,"Prim_TrackID[Nprims_NeuLAND]/I");
   AnaTree->TheTree->Branch("Prim_PDG",AnaTree->Prim_PDG,"Prim_PDG[Nprims_NeuLAND]/I");
   AnaTree->TheTree->Branch("Prim_E",AnaTree->Prim_E,"Prim_E[Nprims_NeuLAND]/D");
   AnaTree->TheTree->Branch("Prim_IsVETOed",AnaTree->Prim_IsVETOed,"Prim_IsVETOed[Nprims_NeuLAND]/B");
   AnaTree->TheTree->Branch("Prim_IsFromTarget",AnaTree->Prim_IsFromTarget,"Prim_IsFromTarget[Nprims_NeuLAND]/B");
   AnaTree->TheTree->Branch("Prim_E_Detected",AnaTree->Prim_E_Detected,"Prim_E_Detected[Nprims_NeuLAND]/D");
   AnaTree->TheTree->Branch("Prim_MCinteraction_T",AnaTree->Prim_MCinteraction_T,"Prim_MCinteraction_T[Nprims_NeuLAND]/D");
   AnaTree->TheTree->Branch("Prim_MCinteraction_X",AnaTree->Prim_MCinteraction_X,"Prim_MCinteraction_X[Nprims_NeuLAND]/D");
   AnaTree->TheTree->Branch("Prim_MCinteraction_Y",AnaTree->Prim_MCinteraction_Y,"Prim_MCinteraction_Y[Nprims_NeuLAND]/D");
   AnaTree->TheTree->Branch("Prim_MCinteraction_Z",AnaTree->Prim_MCinteraction_Z,"Prim_MCinteraction_Z[Nprims_NeuLAND]/D");
   
   // Identification of backscattering events:
   AnaTree->TheTree->Branch("PrimInteraction",AnaTree->PrimInteraction,"PrimInteraction["+Nbars_VETO_str+"]/B");
   AnaTree->TheTree->Branch("BackInteraction",AnaTree->BackInteraction,"BackInteraction["+Nbars_VETO_str+"]/B");
   AnaTree->TheTree->Branch("LoopInteraction",AnaTree->LoopInteraction,"LoopInteraction["+Nbars_VETO_str+"]/B");
   AnaTree->TheTree->Branch("Energetic_Prim",AnaTree->Energetic_Prim,"Energetic_Prim["+Nbars_VETO_str+"]/B");
   AnaTree->TheTree->Branch("Energetic_Back",AnaTree->Energetic_Back,"Energetic_Back["+Nbars_VETO_str+"]/B");
   
   AnaTree->TheTree->Branch("Overal_Prim",&(AnaTree->Overal_Prim),"Overal_Prim/B");
   AnaTree->TheTree->Branch("Overal_Back",&(AnaTree->Overal_Back),"Overal_Back/B");
   AnaTree->TheTree->Branch("Overal_EPrim",&(AnaTree->Overal_EPrim),"Overal_EPrim/B");
   AnaTree->TheTree->Branch("Overal_EBack",&(AnaTree->Overal_EBack),"Overal_EBack/B");
   AnaTree->TheTree->Branch("VETO_TrackTOF",AnaTree->VETO_TrackTOF,"VETO_TrackTOF["+Nbars_VETO_str+"]/D");
   AnaTree->TheTree->Branch("VETO_TrackTOF_Res",AnaTree->VETO_TrackTOF_Res,"VETO_TrackTOF_Res["+Nbars_VETO_str+"]/D");
   AnaTree->TheTree->Branch("VETO_TotalTrackTOF",&(AnaTree->VETO_TotalTrackTOF),"VETO_TotalTrackTOF/D");
   AnaTree->TheTree->Branch("VETO_TotalTrackTOF_Res",&(AnaTree->VETO_TotalTrackTOF_Res),"VETO_TotalTrackTOF_Res/D");
   
   // NeuLAND and VETO QDC and TDC:
   AnaTree->TheTree->Branch("NbarsFired_NeuLAND",&(AnaTree->NbarsFired_NeuLAND),"NbarsFired_NeuLAND/I");
   AnaTree->TheTree->Branch("Neu_BarNumber_Fired",AnaTree->Neu_BarNumber_Fired,"Neu_BarNumber_Fired[NbarsFired_NeuLAND]/I");
   AnaTree->TheTree->Branch("Neu_Edep_Fired",AnaTree->Neu_Edep_Fired,"Neu_Edep_Fired[NbarsFired_NeuLAND]/D");
   AnaTree->TheTree->Branch("Neu_TOF_Fired",AnaTree->Neu_TOF_Fired,"Neu_TOF_Fired[NbarsFired_NeuLAND]/D");
   AnaTree->TheTree->Branch("Neu_TOF_Fired_Res",AnaTree->Neu_TOF_Fired_Res,"Neu_TOF_Fired_Res[NbarsFired_NeuLAND]/D");
   AnaTree->TheTree->Branch("Neu_Primary_TrackID",AnaTree->Neu_Primary_TrackID,"Neu_Primary_TrackID[NbarsFired_NeuLAND]/I");
   AnaTree->TheTree->Branch("Neu_Bar_IsVETOed",AnaTree->Neu_Bar_IsVETOed,"Neu_Bar_IsVETOed[NbarsFired_NeuLAND]/B");
   AnaTree->TheTree->Branch("Neu_TotalTOF",&(AnaTree->Neu_TotalTOF),"Neu_TotalTOF/D");
   AnaTree->TheTree->Branch("Neu_TotalTOF_Res",&(AnaTree->Neu_TotalTOF_Res),"Neu_TotalTOF_Res/D");
   
   AnaTree->TheTree->Branch("NbarsFired_VETO",&(AnaTree->NbarsFired_VETO),"NbarsFired_VETO/I");
   AnaTree->TheTree->Branch("VETO_BarNumber_Fired",AnaTree->VETO_BarNumber_Fired,"VETO_BarNumber_Fired[NbarsFired_VETO]/I");
   AnaTree->TheTree->Branch("VETO_Edep_Fired",AnaTree->VETO_Edep_Fired,"VETO_Edep_Fired[NbarsFired_VETO]/D");
   AnaTree->TheTree->Branch("VETO_TOF_Fired",AnaTree->VETO_TOF_Fired,"VETO_TOF_Fired[NbarsFired_VETO]/D");
   AnaTree->TheTree->Branch("VETO_TOF_Fired_Res",AnaTree->VETO_TOF_Fired_Res,"VETO_TOF_Fired_Res[NbarsFired_VETO]/D");
   AnaTree->TheTree->Branch("VETO_Primary_TrackID",AnaTree->VETO_Primary_TrackID,"VETO_Primary_TrackID[NbarsFired_VETO]/I");
   AnaTree->TheTree->Branch("VETO_TOF_index",AnaTree->VETO_TOF_index,"VETO_TOF_index[NbarsFired_VETO]/I");
   AnaTree->TheTree->Branch("VETO_TotalTOF",&(AnaTree->VETO_TotalTOF),"VETO_TotalTOF/D");
   AnaTree->TheTree->Branch("VETO_TotalTOF_Res",&(AnaTree->VETO_TotalTOF_Res),"VETO_TotalTOF_Res/D");
   
   // Quantification of secondary particles:
   AnaTree->TheTree->Branch("Secondary_Nprotons",AnaTree->Secondary_Nprotons,"Secondary_Nprotons[5]/I");
   AnaTree->TheTree->Branch("Secondary_Nneutrons",AnaTree->Secondary_Nneutrons,"Secondary_Nneutrons[5]/I");
   AnaTree->TheTree->Branch("Secondary_Nelectrons",AnaTree->Secondary_Nelectrons,"Secondary_Nelectrons[5]/I");
   AnaTree->TheTree->Branch("Secondary_Ngammas",AnaTree->Secondary_Ngammas,"Secondary_Ngammas[5]/I");
   AnaTree->TheTree->Branch("Secondary_Npions",AnaTree->Secondary_Npions,"Secondary_Npions[5]/I");
   AnaTree->TheTree->Branch("Secondary_Nmuons",AnaTree->Secondary_Nmuons,"Secondary_Nmuons[5]/I");
   AnaTree->TheTree->Branch("Secondary_Nalphas",AnaTree->Secondary_Nalphas,"Secondary_Nalphas[5]/I");
   AnaTree->TheTree->Branch("Secondary_NCions",AnaTree->Secondary_NCions,"Secondary_NCions[5]/I");
   AnaTree->TheTree->Branch("Secondary_Nfragments",AnaTree->Secondary_Nfragments,"Secondary_Nfragments[5]/I");
   AnaTree->TheTree->Branch("Secondary_NallParticles",AnaTree->Secondary_NallParticles,"Secondary_NallParticles[5]/I");
   AnaTree->TheTree->Branch("Secondary_Particles",&(AnaTree->Secondary_Particles),"Secondary_Particles/B");
   
   // Processing the NeuLAND Clusters:
   AnaTree->TheTree->Branch("NeuLAND_Nclusters",&(AnaTree->NeuLAND_Nclusters),"NeuLAND_Nclusters/I");
   AnaTree->TheTree->Branch("NeuLAND_ClusterX",AnaTree->NeuLAND_ClusterX,"NeuLAND_ClusterX[NeuLAND_Nclusters]/D");
   AnaTree->TheTree->Branch("NeuLAND_ClusterY",AnaTree->NeuLAND_ClusterY,"NeuLAND_ClusterY[NeuLAND_Nclusters]/D");
   AnaTree->TheTree->Branch("NeuLAND_ClusterZ",AnaTree->NeuLAND_ClusterZ,"NeuLAND_ClusterZ[NeuLAND_Nclusters]/D");
   AnaTree->TheTree->Branch("NeuLAND_ClusterTime",AnaTree->NeuLAND_ClusterTime,"NeuLAND_ClusterTime[NeuLAND_Nclusters]/D");
   AnaTree->TheTree->Branch("NeuLAND_CStopX",AnaTree->NeuLAND_CStopX,"NeuLAND_CStopX[NeuLAND_Nclusters]/D");
   AnaTree->TheTree->Branch("NeuLAND_CStopY",AnaTree->NeuLAND_CStopY,"NeuLAND_CStopY[NeuLAND_Nclusters]/D");
   AnaTree->TheTree->Branch("NeuLAND_CStopZ",AnaTree->NeuLAND_CStopZ,"NeuLAND_CStopZ[NeuLAND_Nclusters]/D");
   AnaTree->TheTree->Branch("NeuLAND_CStopTime",AnaTree->NeuLAND_CStopTime,"NeuLAND_CStopTime[NeuLAND_Nclusters]/D");
   AnaTree->TheTree->Branch("NeuLAND_ClusterEnergy",AnaTree->NeuLAND_ClusterEnergy,"NeuLAND_ClusterEnergy[NeuLAND_Nclusters]/D");
   AnaTree->TheTree->Branch("NeuLAND_ClusterSize",AnaTree->NeuLAND_ClusterSize,"NeuLAND_ClusterSize[NeuLAND_Nclusters]/I");
   AnaTree->TheTree->Branch("NeuLAND_Cluster_TrackID",AnaTree->NeuLAND_Cluster_TrackID,"NeuLAND_Cluster_TrackID[NeuLAND_Nclusters]/I");
   AnaTree->TheTree->Branch("NeuLAND_Cluster_IsVETOed",AnaTree->NeuLAND_Cluster_IsVETOed,"NeuLAND_Cluster_IsVETOed[NeuLAND_Nclusters]/B");
   
   // Processing the R3BNeutronTracker Results:
   AnaTree->TheTree->Branch("N_Reconstructed_PrimPoints",&(AnaTree->N_Reconstructed_PrimPoints),"N_Reconstructed_PrimPoints/I");
   AnaTree->TheTree->Branch("T_Reconstructed_PrimPoints",AnaTree->T_Reconstructed_PrimPoints,"T_Reconstructed_PrimPoints[N_Reconstructed_PrimPoints]/D");
   AnaTree->TheTree->Branch("X_Reconstructed_PrimPoints",AnaTree->X_Reconstructed_PrimPoints,"X_Reconstructed_PrimPoints[N_Reconstructed_PrimPoints]/D");
   AnaTree->TheTree->Branch("Y_Reconstructed_PrimPoints",AnaTree->Y_Reconstructed_PrimPoints,"Y_Reconstructed_PrimPoints[N_Reconstructed_PrimPoints]/D");
   AnaTree->TheTree->Branch("Z_Reconstructed_PrimPoints",AnaTree->Z_Reconstructed_PrimPoints,"Z_Reconstructed_PrimPoints[N_Reconstructed_PrimPoints]/D");
   AnaTree->TheTree->Branch("TrackID_Reconstructed_PrimPoints",AnaTree->TrackID_Reconstructed_PrimPoints,"TrackID_Reconstructed_PrimPoints[N_Reconstructed_PrimPoints]/I");
   AnaTree->TheTree->Branch("Reconstructed_PrimPoints_IsVETOed",AnaTree->Reconstructed_PrimPoints_IsVETOed,"Reconstructed_PrimPoints_IsVETOed[N_Reconstructed_PrimPoints]/B");
   
   // Leafs on other detectors:
   AnaTree->TheTree->Branch("Ntracks_STaRTrack",&(AnaTree->Ntracks_STaRTrack),"Ntracks_STaRTrack/I");
   AnaTree->TheTree->Branch("Nhits_CALIFA",&(AnaTree->Nhits_CALIFA),"Nhits_CALIFA/I");
   
   // Full VETO condition:
   if (Store_AllBars==1)
   {
       AnaTree->TheTree->Branch("AllBars_IsVETOed",AnaTree->AllBars_IsVETOed,"AllBars_IsVETOed["+Nbars_Neu_str+"]/B");
   }
   else
   {
       AnaTree->TheTree->Branch("AllBars_IsVETOed",AnaTree->AllBars_IsVETOed,"AllBars_IsVETOed[0]/B");
   }
   
   // NOTE: Add your new branches here!
   
   // ================================================================================================
   // Now our tree is created and all branches are succesfully linked.
   // This means that filling the tree is now really easy. 
   
   // We are however not yet done. We also need to declare our 3D histograms here:
   
   // Obtaining the required inputs:
   Int_t NbinsE = Integers->GetBinContent(301);
   Int_t NbinsZ = Integers->GetBinContent(302);
   Int_t NbinsD = Integers->GetBinContent(303);
   Double_t Emin = 0.0;
   Double_t Emax = Doubles->GetBinContent(203);
   Double_t Zmin = Doubles->GetBinContent(311);
   Double_t Zmax = Doubles->GetBinContent(312);
   Double_t DistMin = 0.0;
   Double_t DistMax = Doubles->GetBinContent(313);
   TString Title = "Secondary";
   
   for (Int_t species = 0; species<10; ++species)
   {
     for (Int_t type = 0; type<5; ++type)
     {
       // Construction of the Title:
       Title = "Secondary";
       
       if (species==0) {Title = Title + "_Protons";}
       if (species==1) {Title = Title + "_Neutrons";}
       if (species==2) {Title = Title + "_Electrons";}
       if (species==3) {Title = Title + "_Gammas";}
       if (species==4) {Title = Title + "_Pions";}
       if (species==5) {Title = Title + "_Muons";}
       if (species==6) {Title = Title + "_Alphas";}
       if (species==7) {Title = Title + "_C-ions";}
       if (species==8) {Title = Title + "_Fragments";}
       if (species==9) {Title = "All_" + Title;}
       
       if (type==0) {Title = Title + "_produced.";}
       if (type==1) {Title = Title + "_aimed_at_VETO.";}
       if (type==2) {Title = Title + "_which_reached_VETO.";}
       if (type==3) {Title = Title + "_which_hit_VETO.";}
       if (type==4) {Title = Title + "_detected_by_VETO.";}
       
       // The actual declaration:
       AnaTree->Harray[species][type] = new TH3D(Title,Title,NbinsE,Emin,Emax,NbinsZ,Zmin,Zmax,NbinsD,DistMin,DistMax);
       
     }
   }

   // ================================================================================================
   // Now this is done, we can just give output:
   
   cout << "=============== Analysis Tree is succesfully created!\n\n";
   Definition = kTRUE;
   
   // close all blocks:
   }
   
   // And done!
   return Definition;  
}