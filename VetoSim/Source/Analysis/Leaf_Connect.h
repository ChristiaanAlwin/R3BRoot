// This function performs the connection to the cbmsim-tree of R3BRoot.
// It loads all the leafs into the Leaf_Structure so that values from 
// the tree can be accessed from the structure in the rest of the analysis.
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

// Inclusion of the Leaf_Structure header:
#include "Leaf_Structure.h"

// Standard namespace:
using namespace std;

// Definition of the function:
Bool_t Leaf_Connect(Leaf_Structure* TheLeafs, Int_t const Counter_input, Int_t const Series)
{
   // The idea is that a pointer to a big Leaf_Structure called TheLeafs is
   // passed as argument to this function. NOTE: only a pointer should be
   // passed, not the structure itself. This causes the content of the 
   // structure to be modified outside of the scope of this function, exactly
   // as we like it!
  
   // The second argument is the counter. This identifies which .root-file
   // we exactly whish to connect to. Since only a pointer is passed and
   // not the structure itself, no outputs are required.
    
   // The third argument is the series: it tell us what kind of analysis-series
   // we are processing.
  
   // We do however give a boolian as output that tells us whether the 
   // connection was succesful or not.
  
   // All we do in this function is to fill the pointers to the leafs so that the tree
   // can be read out convieniently.
  
   // NOTE: if a comment about a leaf is marked with "DIMA" then it means that Dima has
   // literally told me that this is what it means. Hence this is completely sure. If a
   // comment is mark with the word probably, it means that it is a guess of mine without
   // any proof. If a comment just states what it means, Then I am sure about what it 
   // means but there hasn't been anyone who told me that this is what it means. For
   // example, Dima has explained what LandPoint_fTrackID means. Other detectors also 
   // have an fTrackID leaf. Hence guess what it means??? but Dima did not tell me this.
   // Hence unmarked comments are like this.
  
   // ================================================================================================
   // before doing anything, define the counter:
   Int_t Counter = Counter_input;
   if (Series==2) {Counter = 1;} // Then we analyze the same file over and over again, exactly what we want!
   if (Series==5) {Counter = 1;} // Then we analyze the same file over and over again, exactly what we want!
   if (Series==6) {Counter = 1;} // Then we analyze the same file over and over again, exactly what we want!
   if (Series==7) {Counter = 1;} // Then we analyze the same file over and over again, exactly what we want!
   if (Series==8) {Counter = 1;} // Then we analyze the same file over and over again, exactly what we want!
    
   // We begin with defining the boolian for the test:
   Bool_t Connection = kFALSE;
   cout << "=============== Creating connection to all leafs in the Trees...\n\n";
   
   // ================================================================================================
   // Now, our first task is to connect to the trees itself.
  
   // We begin by connecting to the Inputs.root-file. Check if this file exists:
   FileStat_t stom1;
   FileStat_t stom2;
   FileStat_t stom3;
   FileStat_t stom4;
   FileStat_t stom5;
   FileStat_t stom6;
   FileStat_t stom7;
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
   
   // The we must obtain the string for the output-filename:
   TObjString* Output_Name_Obj = (TObjString*) inputs->Get("Output_Name");
   TString Output_Name = Output_Name_Obj->GetString();
   
   // Then we also need a string for the run-number:
   TString st = "";
   TString Nr_Run_str = st.Itoa(Counter,10);
   
   // And we need a string for the Monte Carlo Engine:
   TObjString* Monte_Carlo_Engine_Obj = (TObjString*) inputs->Get("Monte_Carlo_Engine");
   TString Monte_Carlo_Engine = Monte_Carlo_Engine_Obj->GetString();
   
   // Put the result in a boolian:
   if (Monte_Carlo_Engine=="TGeant3")
   {
     TheLeafs->Geant3 = kTRUE;
   }
   else
   {
     TheLeafs->Geant3 = kFALSE;
   }
   
   // Finally we can then build the complete filename:
   TString OutFile = "../../Data/MonteCarlo/" + Output_Name + "_run=" + Nr_Run_str + ".root";
   TString DigiNeuFile = "../../Data/Digitizer/" + Output_Name + "_run=" + Nr_Run_str + "_NeuLAND.root";
   TString DigiVetoFile = "../../Data/Digitizer/" + Output_Name + "_run=" + Nr_Run_str + "_VETO.root";
   TString TrackerFile = "../../Data/Digitizer/" + Output_Name + "_run=" + Nr_Run_str + "_NeutronTracker.root";
   TString OtherDigis = "../../Data/Digitizer/" + Output_Name + "_run=" + Nr_Run_str + "_Others.root";
   TString STaRTrackFile = "../../Data/Digitizer/" + Output_Name + "_run=" + Nr_Run_str + "_STaRTrack.root";
   
   // So now we can connect to the Monte Carlo file and to the digitizer files. First check if they exist:
   Int_t MC_test = gSystem->GetPathInfo(OutFile,stom2);
   Int_t DigiN_test = gSystem->GetPathInfo(DigiNeuFile,stom3);
   Int_t DigiV_test = gSystem->GetPathInfo(DigiVetoFile,stom4);
   Int_t Track_test = gSystem->GetPathInfo(TrackerFile,stom5); // NOTE: we will not test for this, we will simply assume that only if this file exists, we should connect to the tracker.
   Int_t Other_Test = gSystem->GetPathInfo(OtherDigis,stom6);
   Int_t Star_Test = gSystem->GetPathInfo(STaRTrackFile,stom7);   
   
   // adapt the tracker test:
   Int_t Texe = Integers->GetBinContent(313);
   if (Texe==0) {Track_test = 1;}
   
   Int_t f_test = 0;
   
   Int_t Neu_Ex = Integers->GetBinContent(216);
   Int_t VETO_Ex = Integers->GetBinContent(217);
   Int_t NEB_Ex = Integers->GetBinContent(230);
   Int_t NEBv_Ex = Integers->GetBinContent(231);
   Int_t G3Test = Integers->GetBinContent(249);
   Int_t Califa_ex = Integers->GetBinContent(210);
   Int_t Star_Ex = Integers->GetBinContent(207);
  
   Bool_t Neu = ((Neu_Ex==1)||(Neu_Ex==2));
   Bool_t Vet = ((VETO_Ex==1)||(VETO_Ex==2));
   Bool_t NEB = (!(NEB_Ex==1));
   Bool_t NEBv = (!(NEBv_Ex==1));
   Bool_t Gean = (Monte_Carlo_Engine=="TGeant3");
   Bool_t cal = (Califa_ex==1);
   Bool_t star = (Star_Ex==1);
   
   if (G3Test==0) {Gean = kTRUE; TheLeafs->Geant3 = kTRUE;} // NOTE!!!
   
   if (Gean&&Neu&&NEB&&NEBv)
   {
     f_test = f_test + DigiN_test;
   }
   if (Gean&&Vet)
   {
       f_test = f_test + DigiV_test;
   }
   f_test = f_test + MC_test;
   if (Gean&&(cal||star))
   {
       f_test = f_test + Other_Test;
   }
   if (Gean&&star)
   {
       f_test = f_test + Star_Test;
   }
   
   if (f_test>0)
   {
     cout << "### ERROR: The Monte Carlo file and/or the Digitizer files you want do not exist!\n\n";
   }
   else
   {
   //Then connect to them:
   TFile* f = new TFile(OutFile,"read");
   TFile* fdigiN = 0;
   TFile* fdigiV = 0;
   TFile* ftrack = 0;
   TFile* fOther = 0;
   TFile* fStar = 0;
   if (Gean&&Neu&&NEB&&NEBv) {fdigiN = new TFile(DigiNeuFile,"read");}
   if (Gean&&Vet) {fdigiV = new TFile(DigiVetoFile,"read");}
   if (Track_test==0) {ftrack = new TFile(TrackerFile,"read");}
   if (Gean&&(cal||star)) {fOther = new TFile(OtherDigis,"read");}
   if (Gean&&star) {fStar = new TFile(STaRTrackFile,"read");}
   cout << "Connection of the Monte Carlo file and to the Digitizer files was succesful.\n\n";
   
   // Now connect to the trees inside these files:
   TheLeafs->TheTree = (TTree*) f->Get("cbmsim");
   if (Gean&&Neu&&NEB&&NEBv) {TheLeafs->NeuDigiTree = (TTree*) fdigiN->Get("cbmsim");}
   if (Gean&&Vet) {TheLeafs->VetoDigiTree = (TTree*) fdigiV->Get("cbmsim");}
   if (Track_test==0) {TheLeafs->TrackTree = (TTree*) ftrack->Get("cbmsim");}
   if (Gean&&(cal||star)) {TheLeafs->OtherTree = (TTree*) fOther->Get("cbmsim");}
   if (Gean&&star) {TheLeafs->StarTree = (TTree*) fStar->Get("STarTrack_Tracks");}
   
   // Now if this is succesful, we can finally continue with connecting to the leafs:
   Bool_t cbmsim_test = kTRUE;
   if (Gean&&Neu&&NEB&&NEBv)
   {
     if ((TheLeafs->NeuDigiTree)==0) {cbmsim_test = kFALSE;}
   }
   if (Gean&&Vet)
   {
     if ((TheLeafs->VetoDigiTree)==0) {cbmsim_test = kFALSE;}
   }
   if (Track_test==0)
   {
     if ((TheLeafs->TrackTree)==0) {Track_test = 1;} // NOTE: Here we simply assume that the tracker is only there if it worked!
   }
   if ((TheLeafs->TheTree)==0) {cbmsim_test = kFALSE;}
   if (Gean&&(cal||star))
   {
       if ((TheLeafs->OtherTree)==0) {cbmsim_test = kFALSE;}
   }
   if (Gean&&star)
   {
       if ((TheLeafs->StarTree)==0) {cbmsim_test = kFALSE;}
   }   
   
   if (!cbmsim_test)
   {
     cout << "### ERROR: Some of the cbmsim Trees inside the .root-file do not exist!\n\n";
   }
   else
   {
     cout << "Connection to the cbmsim Trees was succesful.\n\n";
     
   // ================================================================================================
   // Before we connect to the leafs, we first define the error counters that we use to keep track
   // on which leafs succesfully connected and which did not:
   
   Int_t MC_Errors = 0;
   Int_t GFI_Errors = 0;
   Int_t CALIFA_Errors = 0;
   Int_t CrystalBall_Errors = 0;
   Int_t STaRTRack_Errors = 0;
   Int_t DCH_Errors = 0;
   Int_t TOF_Errors = 0;
   Int_t mTOF_Errors = 0;
   Int_t LandPoint_Errors = 0;
   Int_t LFH_Errors = 0;
   Int_t VetoPoint_Errors = 0;
   Int_t VFH_Errors = 0;
   Int_t MFI_Errors = 0;
   Int_t PSP_Errors = 0;
   Int_t EventHeader_Errors = 0;
   Int_t GeoTracks_Errors = 0;
   Int_t LandDigi_Errors = 0;
   Int_t LandCluster_Errors = 0;
   Int_t VetoDigi_Errors = 0;
   Int_t VetoCluster_Errors = 0;
   Int_t Tracker_Errors = 0;
   Int_t VetoPrimIntPoint_Errors = 0;  
   Int_t NeuPrimIntPoint_Errors = 0;
   Int_t OtherGen_Errors = 0;
   Int_t CALIFAhit_Errors = 0;
   Int_t STaRTrackHits_Errors = 0;
   Int_t STaRTrackDigis_Errors = 0;
   Int_t STaRTrackTracks_Errors = 0;
   
   // ================================================================================================
   // Now we can continue with connecting to the Leafs:
   
   // -----------------------------------
   // MC leafs:
   TheLeafs->MCTrack_fUniqueID = TheLeafs->TheTree->FindLeaf("MCTrack.fUniqueID");	// UInt_t[n] // This is an artifact of FairRoot. FairRoot is designed in such a way that each kind of leafs requires an fUniqueID and an fBits.
   TheLeafs->MCTrack_fBits = TheLeafs->TheTree->FindLeaf("MCTrack.fBits"); 		// UInt_t[n] // This is an artifact of FairRoot. FairRoot is designed in such a way that each kind of leafs requires an fUniqueID and an fBits.
   TheLeafs->MCTrack_fPdgCode  = TheLeafs->TheTree->FindLeaf("MCTrack.fPdgCode"); 	// Int_t[n] // particle ID of the particle performing this specific interaction. This uses the particle PDG code, ions have 1000......-code, so they are much much higher
												    // then elemntary particles. Hence a histogram from TBrowser() does not reveal the structure. for particles (not ions), The PDG code is constructed in the
												    // following way: PDG = 1000*i + 100*j + 10*k + (2*s+1). Here s is the total spin of the particle. i is the PDG code of the first quark of the particle, j is the
												    // PDG code of the second quark of the particle and k is the PDG code of the third quark of the particle (0 if the particle does not have this quark).
												    // u=1, d=2, s=3, c=4, b=5, t=6, b'=7. NB: electrons, gamma's etc have PDG between 10 en 100, so they can be distinguished.
												    // mesons have PDG = (100*max(i,j) + 10*min(i,j) + (2s+1))*sign(i-j)*(-1)^max(i,j) this is because they have an antiquark 
   TheLeafs->MCTrack_fMotherId = TheLeafs->TheTree->FindLeaf("MCTrack.fMotherId"); 	// Int_t[n] // Track-ID of the particle that created this particle (mother particle). The Track-ID of a particle is the C++ index in the MC leaf where this particle is stored.
												    // If the MotherID equals -1, it is a primary particle (Geant4 only!) "DIMA".
   TheLeafs->MCTrack_fPx = TheLeafs->TheTree->FindLeaf("MCTrack.fPx"); 			// Double32_t[n] // Particle momentum in x-direction at a specific interaction point. [GeV]
   TheLeafs->MCTrack_fPy = TheLeafs->TheTree->FindLeaf("MCTrack.fPy"); 			// Double32_t[n] // Particle momentum in y-direction at a specific interaction point. [GeV]
   TheLeafs->MCTrack_fPz = TheLeafs->TheTree->FindLeaf("MCTrack.fPz"); 			// Double32_t[n] // Particle momentum in z-direction at a specific interaction point. [GeV]
   TheLeafs->MCTrack_fStartX = TheLeafs->TheTree->FindLeaf("MCTrack.fStartX"); 		// Double32_t[n] // x-coordinate of the start-point of the particle track this specific interaction point belongs to [cm] (so not x-coordinate of the interaction point itself!)
   TheLeafs->MCTrack_fStartY = TheLeafs->TheTree->FindLeaf("MCTrack.fStartY"); 		// Double32_t[n] // y-coordinate of the start-point of the particle track this specific interaction point belongs to [cm] (so not y-coordinate of the interaction point itself!)
   TheLeafs->MCTrack_fStartZ = TheLeafs->TheTree->FindLeaf("MCTrack.fStartZ"); 		// Double32_t[n] // z-coordinate of the start-point of the particle track this specific interaction point belongs to [cm] (so not z-coordinate of the interaction point itself!)
   TheLeafs->MCTrack_fStartT = TheLeafs->TheTree->FindLeaf("MCTrack.fStartT"); 		// Double32_t[n] // time-coordinate of the start-point of the particle track this specific interaction point belongs to [ns] (so not time-coordinate of the interaction point itself!)
   TheLeafs->MCTrack_fNPoints = TheLeafs->TheTree->FindLeaf("MCTrack.fNPoints"); 	// Int_t[n] //
   TheLeafs->MCTrack_fMass = TheLeafs->TheTree->FindLeaf("MCTrack.fMass"); 		// Double32_t[n] // mass of the particle performing this specific interaction. [GeV]
   
   // Checking if the connection worked:
   if (TheLeafs->MCTrack_fUniqueID == 0) {MC_Errors = MC_Errors + 1;}
   if (TheLeafs->MCTrack_fBits == 0) {MC_Errors = MC_Errors + 1;}
   if (TheLeafs->MCTrack_fPdgCode == 0) {MC_Errors = MC_Errors + 1;}
   if (TheLeafs->MCTrack_fMotherId == 0) {MC_Errors = MC_Errors + 1;}
   if (TheLeafs->MCTrack_fPx == 0) {MC_Errors = MC_Errors + 1;}
   if (TheLeafs->MCTrack_fPy == 0) {MC_Errors = MC_Errors + 1;}
   if (TheLeafs->MCTrack_fPz == 0) {MC_Errors = MC_Errors + 1;}
   if (TheLeafs->MCTrack_fStartX == 0) {MC_Errors = MC_Errors + 1;}
   if (TheLeafs->MCTrack_fStartY == 0) {MC_Errors = MC_Errors + 1;}
   if (TheLeafs->MCTrack_fStartZ == 0) {MC_Errors = MC_Errors + 1;}
   if (TheLeafs->MCTrack_fStartT == 0) {MC_Errors = MC_Errors + 1;}
   if (TheLeafs->MCTrack_fNPoints == 0) {MC_Errors = MC_Errors + 1;}
   if (TheLeafs->MCTrack_fMass == 0) {MC_Errors = MC_Errors + 1;}

   // -----------------------------------
   // CALIFA leafs:
   TheLeafs->CaloCrystalHitSim_fUniqueID = TheLeafs->TheTree->FindLeaf("CaloCrystalHitSim.fUniqueID");				// UInt_t[n] // This is an artifact of FairRoot. FairRoot is designed in such a way that each kind of leafs requires an fUniqueID and an fBits.
   TheLeafs->CaloCrystalHitSim_fBits = TheLeafs->TheTree->FindLeaf("CaloCrystalHitSim.fBits");					// UInt_t[n] // This is an artifact of FairRoot. FairRoot is designed in such a way that each kind of leafs requires an fUniqueID and an fBits.
   TheLeafs->CaloCrystalHitSim_fLinks = TheLeafs->TheTree->FindLeaf("CaloCrystalHitSim.fLinks");				// set<FairLink>[n]
   TheLeafs->CaloCrystalHitSim_fEntryNr_fUniqueID = TheLeafs->TheTree->FindLeaf("CaloCrystalHitSim.fEntryNr.fUniqueID");	// UInt_t[n] // Probably again an artifact of FairRoot, but now specificly for the fEntryNr leafs.
   TheLeafs->CaloCrystalHitSim_fEntryNr_fBits = TheLeafs->TheTree->FindLeaf("CaloCrystalHitSim.fEntryNr.fBits");		// UInt_t[n] // Probably again an artifact of FairRoot, but now specificly for the fEntryNr leafs.
   TheLeafs->CaloCrystalHitSim_fEntryNr_fFile = TheLeafs->TheTree->FindLeaf("CaloCrystalHitSim.fEntryNr.fFile");		// Int_t[n]
   TheLeafs->CaloCrystalHitSim_fEntryNr_fEntry = TheLeafs->TheTree->FindLeaf("CaloCrystalHitSim.fEntryNr.fEntry");		// Int_t[n]
   TheLeafs->CaloCrystalHitSim_fEntryNr_fType = TheLeafs->TheTree->FindLeaf("CaloCrystalHitSim.fEntryNr.fType");		// Int_t[n]
   TheLeafs->CaloCrystalHitSim_fEntryNr_fIndex = TheLeafs->TheTree->FindLeaf("CaloCrystalHitSim.fEntryNr.fIndex");		// Int_t[n]
   TheLeafs->CaloCrystalHitSim_fEntryNr_fWeight = TheLeafs->TheTree->FindLeaf("CaloCrystalHitSim.fEntryNr.fWeight");		// Float_t[n]
   TheLeafs->CaloCrystalHitSim_fDefaultType = TheLeafs->TheTree->FindLeaf("CaloCrystalHitSim.fDefaultType");			// Int_t[n]
   TheLeafs->CaloCrystalHitSim_fEnergy = TheLeafs->TheTree->FindLeaf("CaloCrystalHitSim.fEnergy");				// Double32_t[n]
   TheLeafs->CaloCrystalHitSim_fNf = TheLeafs->TheTree->FindLeaf("CaloCrystalHitSim.fNf");					// Double32_t[n]
   TheLeafs->CaloCrystalHitSim_fNs = TheLeafs->TheTree->FindLeaf("CaloCrystalHitSim.fNs");					// Double32_t[n]
   TheLeafs->CaloCrystalHitSim_fToT_Energy = TheLeafs->TheTree->FindLeaf("CaloCrystalHitSim.fToT_Energy");			// Double32_t[n]
   TheLeafs->CaloCrystalHitSim_fTime = TheLeafs->TheTree->FindLeaf("CaloCrystalHitSim.fTime");					// ULong64_t[n] // probably Time Of Flight since the beginning of the event.
   TheLeafs->CaloCrystalHitSim_fCrystalId = TheLeafs->TheTree->FindLeaf("CaloCrystalHitSim.fCrystalId");			// Int_t[n] // probably the number of the crystal in which the bhit took place (similar to fSector in NeuLAND?)
   TheLeafs->CaloCrystalHitSim_fCrystalType = TheLeafs->TheTree->FindLeaf("CaloCrystalHitSim.fCrystalType");			// Int_t[n]
   TheLeafs->CaloCrystalHitSim_fCrystalCopy = TheLeafs->TheTree->FindLeaf("CaloCrystalHitSim.fCrystalCopy");				// Int_t[n]
   TheLeafs->CaloCrystalHitSim_fNSteps = TheLeafs->TheTree->FindLeaf("CaloCrystalHitSim.fNSteps");				// Int_t[n]
   TheLeafs->CaloCrystalHitSim_fEinc = TheLeafs->TheTree->FindLeaf("CaloCrystalHitSim.fEinc");					// Double32_t[n]
   TheLeafs->CaloCrystalHitSim_fTrackID = TheLeafs->TheTree->FindLeaf("CaloCrystalHitSim.fTrackID");				// Int_t[n] // C++ index of where this track is located in the MC leafs.
   TheLeafs->CaloCrystalHitSim_fVolumeID = TheLeafs->TheTree->FindLeaf("CaloCrystalHitSim.fVolumeID");				// Int_t[n]
   TheLeafs->CaloCrystalHitSim_fParentTrackID = TheLeafs->TheTree->FindLeaf("CaloCrystalHitSim.fParentTrackID");		// Int_t[n] // probably C++ index of where the mother of this track is located in the MC leafs.
   TheLeafs->CaloCrystalHitSim_fTrackPID = TheLeafs->TheTree->FindLeaf("CaloCrystalHitSim.fTrackPID");				// Int_t[n] 
   TheLeafs->CaloCrystalHitSim_fTrackUniqueID = TheLeafs->TheTree->FindLeaf("CaloCrystalHitSim.fTrackUniqueID");		// Int_t[n] // Probably again an artifact of FairRoot, but now specificly for the TrackID.
   
   // Checking if the connection worked:
   if (TheLeafs->CaloCrystalHitSim_fUniqueID == 0) {CALIFA_Errors = CALIFA_Errors + 1;}
   if (TheLeafs->CaloCrystalHitSim_fBits == 0) {CALIFA_Errors = CALIFA_Errors + 1;}
   if (TheLeafs->CaloCrystalHitSim_fLinks == 0) {CALIFA_Errors = CALIFA_Errors + 1;}
   if (TheLeafs->CaloCrystalHitSim_fEntryNr_fUniqueID == 0) {CALIFA_Errors = CALIFA_Errors + 1;}
   if (TheLeafs->CaloCrystalHitSim_fEntryNr_fBits == 0) {CALIFA_Errors = CALIFA_Errors + 1;}
   if (TheLeafs->CaloCrystalHitSim_fEntryNr_fFile == 0) {CALIFA_Errors = CALIFA_Errors + 1;}
   if (TheLeafs->CaloCrystalHitSim_fEntryNr_fEntry == 0) {CALIFA_Errors = CALIFA_Errors + 1;}
   if (TheLeafs->CaloCrystalHitSim_fEntryNr_fType == 0) {CALIFA_Errors = CALIFA_Errors + 1;}
   if (TheLeafs->CaloCrystalHitSim_fEntryNr_fIndex == 0) {CALIFA_Errors = CALIFA_Errors + 1;}
   if (TheLeafs->CaloCrystalHitSim_fEntryNr_fWeight == 0) {CALIFA_Errors = CALIFA_Errors + 1;}
   if (TheLeafs->CaloCrystalHitSim_fDefaultType == 0) {CALIFA_Errors = CALIFA_Errors + 1;}
   if (TheLeafs->CaloCrystalHitSim_fEnergy == 0) {CALIFA_Errors = CALIFA_Errors + 1;}
   if (TheLeafs->CaloCrystalHitSim_fNf == 0) {CALIFA_Errors = CALIFA_Errors + 1;}
   if (TheLeafs->CaloCrystalHitSim_fNs == 0) {CALIFA_Errors = CALIFA_Errors + 1;}
   if (TheLeafs->CaloCrystalHitSim_fToT_Energy == 0) {CALIFA_Errors = CALIFA_Errors + 1;}
   if (TheLeafs->CaloCrystalHitSim_fTime == 0) {CALIFA_Errors = CALIFA_Errors + 1;}
   if (TheLeafs->CaloCrystalHitSim_fCrystalId == 0) {CALIFA_Errors = CALIFA_Errors + 1;}
   if (TheLeafs->CaloCrystalHitSim_fCrystalType == 0) {CALIFA_Errors = CALIFA_Errors + 1;}
   if (TheLeafs->CaloCrystalHitSim_fCrystalCopy == 0) {CALIFA_Errors = CALIFA_Errors + 1;}
   if (TheLeafs->CaloCrystalHitSim_fNSteps == 0) {CALIFA_Errors = CALIFA_Errors + 1;}
   if (TheLeafs->CaloCrystalHitSim_fEinc == 0) {CALIFA_Errors = CALIFA_Errors + 1;}
   if (TheLeafs->CaloCrystalHitSim_fTrackID == 0) {CALIFA_Errors = CALIFA_Errors + 1;}
   if (TheLeafs->CaloCrystalHitSim_fVolumeID == 0) {CALIFA_Errors = CALIFA_Errors + 1;}
   if (TheLeafs->CaloCrystalHitSim_fParentTrackID == 0) {CALIFA_Errors = CALIFA_Errors + 1;}
   if (TheLeafs->CaloCrystalHitSim_fTrackPID == 0) {CALIFA_Errors = CALIFA_Errors + 1;}
   if (TheLeafs->CaloCrystalHitSim_fTrackUniqueID == 0) {CALIFA_Errors = CALIFA_Errors + 1;}
   
   // -----------------------------------
   // Crystal Ball Leafs:
   TheLeafs->XBCrystalPoint_fUniqueID = TheLeafs->TheTree->FindLeaf("XBCrystalPoint.fUniqueID");         // UInt_t[n]
   TheLeafs->XBCrystalPoint_fBits = TheLeafs->TheTree->FindLeaf("XBCrystalPoint.fBits");                 // UInt_t[n]
   TheLeafs->XBCrystalPoint_fLink = TheLeafs->TheTree->FindLeaf("XBCrystalPoint.fLink");                 // FairMultiLinkedData*[n]
   TheLeafs->XBCrystalPoint_fTrackID = TheLeafs->TheTree->FindLeaf("XBCrystalPoint.fTrackID");           // Int_t[n]
   TheLeafs->XBCrystalPoint_fEventId = TheLeafs->TheTree->FindLeaf("XBCrystalPoint.fEventId");           // UInt_t[n]
   TheLeafs->XBCrystalPoint_fPx = TheLeafs->TheTree->FindLeaf("XBCrystalPoint.fPx");                     // Double32_t[n]
   TheLeafs->XBCrystalPoint_fPy = TheLeafs->TheTree->FindLeaf("XBCrystalPoint.fPy");                     // Double32_t[n]
   TheLeafs->XBCrystalPoint_fPz = TheLeafs->TheTree->FindLeaf("XBCrystalPoint.fPz");                     // Double32_t[n]
   TheLeafs->XBCrystalPoint_fTime = TheLeafs->TheTree->FindLeaf("XBCrystalPoint.fTime");                 // Double32_t[n]
   TheLeafs->XBCrystalPoint_fLength = TheLeafs->TheTree->FindLeaf("XBCrystalPoint.fLength");             // Double32_t[n]
   TheLeafs->XBCrystalPoint_fELoss = TheLeafs->TheTree->FindLeaf("XBCrystalPoint.fELoss");               // Double32_t[n]
   TheLeafs->XBCrystalPoint_fDetectorID = TheLeafs->TheTree->FindLeaf("XBCrystalPoint.fDetectorID");     // Int_t[n]
   TheLeafs->XBCrystalPoint_fX = TheLeafs->TheTree->FindLeaf("XBCrystalPoint.fX");                       // Double32_t[n]
   TheLeafs->XBCrystalPoint_fY = TheLeafs->TheTree->FindLeaf("XBCrystalPoint.fY");                       // Double32_t[n]
   TheLeafs->XBCrystalPoint_fZ = TheLeafs->TheTree->FindLeaf("XBCrystalPoint.fZ");                       // Double32_t[n]
   TheLeafs->XBCrystalPoint_fX_out = TheLeafs->TheTree->FindLeaf("XBCrystalPoint.fX_out");               // Double32_t[n]
   TheLeafs->XBCrystalPoint_fY_out = TheLeafs->TheTree->FindLeaf("XBCrystalPoint.fY_out");               // Double32_t[n]
   TheLeafs->XBCrystalPoint_fZ_out = TheLeafs->TheTree->FindLeaf("XBCrystalPoint.fZ_out");               // Double32_t[n]
   TheLeafs->XBCrystalPoint_fPx_out = TheLeafs->TheTree->FindLeaf("XBCrystalPoint.fPx_out");             // Double32_t[n]
   TheLeafs->XBCrystalPoint_fPy_out = TheLeafs->TheTree->FindLeaf("XBCrystalPoint.fPy_out");             // Double32_t[n]
   TheLeafs->XBCrystalPoint_fPz_out = TheLeafs->TheTree->FindLeaf("XBCrystalPoint.fPz_out");             // Double32_t[n]
   TheLeafs->XBCrystalPoint_fCrystalType = TheLeafs->TheTree->FindLeaf("XBCrystalPoint.fCrystalType");   // Int_t[n]
   TheLeafs->XBCrystalPoint_fCrystalNb = TheLeafs->TheTree->FindLeaf("XBCrystalPoint.fCrystalNb");       // Int_t[n]
   
   if (TheLeafs->XBCrystalPoint_fUniqueID == 0) {CrystalBall_Errors = CrystalBall_Errors + 1;}  
   if (TheLeafs->XBCrystalPoint_fBits == 0) {CrystalBall_Errors = CrystalBall_Errors + 1;}      
   if (TheLeafs->XBCrystalPoint_fLink == 0) {CrystalBall_Errors = CrystalBall_Errors + 1;}   
   if (TheLeafs->XBCrystalPoint_fTrackID == 0) {CrystalBall_Errors = CrystalBall_Errors + 1;}   
   if (TheLeafs->XBCrystalPoint_fEventId == 0) {CrystalBall_Errors = CrystalBall_Errors + 1;}   
   if (TheLeafs->XBCrystalPoint_fPx == 0) {CrystalBall_Errors = CrystalBall_Errors + 1;}           
   if (TheLeafs->XBCrystalPoint_fPy == 0) {CrystalBall_Errors = CrystalBall_Errors + 1;}           
   if (TheLeafs->XBCrystalPoint_fPz == 0) {CrystalBall_Errors = CrystalBall_Errors + 1;}           
   if (TheLeafs->XBCrystalPoint_fTime == 0) {CrystalBall_Errors = CrystalBall_Errors + 1;}         
   if (TheLeafs->XBCrystalPoint_fLength == 0) {CrystalBall_Errors = CrystalBall_Errors + 1;}       
   if (TheLeafs->XBCrystalPoint_fELoss == 0) {CrystalBall_Errors = CrystalBall_Errors + 1;}        
   if (TheLeafs->XBCrystalPoint_fDetectorID == 0) {CrystalBall_Errors = CrystalBall_Errors + 1;}    
   if (TheLeafs->XBCrystalPoint_fX == 0) {CrystalBall_Errors = CrystalBall_Errors + 1;}            
   if (TheLeafs->XBCrystalPoint_fY == 0) {CrystalBall_Errors = CrystalBall_Errors + 1;}            
   if (TheLeafs->XBCrystalPoint_fZ == 0) {CrystalBall_Errors = CrystalBall_Errors + 1;}            
   if (TheLeafs->XBCrystalPoint_fX_out == 0) {CrystalBall_Errors = CrystalBall_Errors + 1;}        
   if (TheLeafs->XBCrystalPoint_fY_out == 0) {CrystalBall_Errors = CrystalBall_Errors + 1;}        
   if (TheLeafs->XBCrystalPoint_fZ_out == 0) {CrystalBall_Errors = CrystalBall_Errors + 1;}        
   if (TheLeafs->XBCrystalPoint_fPx_out == 0) {CrystalBall_Errors = CrystalBall_Errors + 1;}       
   if (TheLeafs->XBCrystalPoint_fPy_out == 0) {CrystalBall_Errors = CrystalBall_Errors + 1;}       
   if (TheLeafs->XBCrystalPoint_fPz_out == 0) {CrystalBall_Errors = CrystalBall_Errors + 1;}       
   if (TheLeafs->XBCrystalPoint_fCrystalType == 0) {CrystalBall_Errors = CrystalBall_Errors + 1;}  
   if (TheLeafs->XBCrystalPoint_fCrystalNb == 0) {CrystalBall_Errors = CrystalBall_Errors + 1;}  
   
   // -----------------------------------
   // GFI leafs:
   TheLeafs->GFIPoint_fUniqueID = TheLeafs->TheTree->FindLeaf("GFIPoint.fUniqueID");                    // UInt_t[n]
   TheLeafs->GFIPoint_fBits = TheLeafs->TheTree->FindLeaf("GFIPoint.fBits");                            // UInt_t[n]
   TheLeafs->GFIPoint_fLink = TheLeafs->TheTree->FindLeaf("GFIPoint.fLink");                            // FairMultiLinkedData*[n]
   TheLeafs->GFIPoint_fTrackID = TheLeafs->TheTree->FindLeaf("GFIPoint.fTrackID");                      // Int_t[n]
   TheLeafs->GFIPoint_fEventId = TheLeafs->TheTree->FindLeaf("GFIPoint.fEventId");                      // UInt_t[n]
   TheLeafs->GFIPoint_fPx = TheLeafs->TheTree->FindLeaf("GFIPoint.fPx");                                // Double32_t[n]
   TheLeafs->GFIPoint_fPy = TheLeafs->TheTree->FindLeaf("GFIPoint.fPy");                                // Double32_t[n]
   TheLeafs->GFIPoint_fPz = TheLeafs->TheTree->FindLeaf("GFIPoint.fPz");                                // Double32_t[n]
   TheLeafs->GFIPoint_fTime = TheLeafs->TheTree->FindLeaf("GFIPoint.fTime");                            // Double32_t[n]
   TheLeafs->GFIPoint_fLength = TheLeafs->TheTree->FindLeaf("GFIPoint.fLength");                        // Double32_t[n]
   TheLeafs->GFIPoint_fELoss = TheLeafs->TheTree->FindLeaf("GFIPoint.fELoss");                          // Double32_t[n]
   TheLeafs->GFIPoint_fDetectorID = TheLeafs->TheTree->FindLeaf("GFIPoint.fDetectorID");                // Int_t[n]
   TheLeafs->GFIPoint_fX = TheLeafs->TheTree->FindLeaf("GFIPoint.fX");                                  // Double32_t[n]
   TheLeafs->GFIPoint_fY = TheLeafs->TheTree->FindLeaf("GFIPoint.fY");                                  // Double32_t[n]
   TheLeafs->GFIPoint_fZ = TheLeafs->TheTree->FindLeaf("GFIPoint.fZ");                                  // Double32_t[n]
   TheLeafs->GFIPoint_fX_out = TheLeafs->TheTree->FindLeaf("GFIPoint.fX_out");                          // Double32_t[n]
   TheLeafs->GFIPoint_fY_out = TheLeafs->TheTree->FindLeaf("GFIPoint.fY_out");                          // Double32_t[n]
   TheLeafs->GFIPoint_fZ_out = TheLeafs->TheTree->FindLeaf("GFIPoint.fZ_out");                          // Double32_t[n]
   TheLeafs->GFIPoint_fPx_out = TheLeafs->TheTree->FindLeaf("GFIPoint.fPx_out");                        // Double32_t[n]
   TheLeafs->GFIPoint_fPy_out = TheLeafs->TheTree->FindLeaf("GFIPoint.fPy_out");                        // Double32_t[n]
   TheLeafs->GFIPoint_fPz_out = TheLeafs->TheTree->FindLeaf("GFIPoint.fPz_out");                        // Double32_t[n]
   TheLeafs->GFIPoint_fModule = TheLeafs->TheTree->FindLeaf("GFIPoint.fModule");                        // Int_t[n]
   
   if (TheLeafs->GFIPoint_fUniqueID == 0) {GFI_Errors = GFI_Errors + 1;}             
   if (TheLeafs->GFIPoint_fBits == 0) {GFI_Errors = GFI_Errors + 1;}                
   if (TheLeafs->GFIPoint_fLink == 0) {GFI_Errors = GFI_Errors + 1;}                         
   if (TheLeafs->GFIPoint_fTrackID == 0) {GFI_Errors = GFI_Errors + 1;}                 
   if (TheLeafs->GFIPoint_fEventId == 0) {GFI_Errors = GFI_Errors + 1;}                  
   if (TheLeafs->GFIPoint_fPx == 0) {GFI_Errors = GFI_Errors + 1;}                        
   if (TheLeafs->GFIPoint_fPy == 0) {GFI_Errors = GFI_Errors + 1;}                         
   if (TheLeafs->GFIPoint_fPz == 0) {GFI_Errors = GFI_Errors + 1;}                      
   if (TheLeafs->GFIPoint_fTime == 0) {GFI_Errors = GFI_Errors + 1;}                           
   if (TheLeafs->GFIPoint_fLength == 0) {GFI_Errors = GFI_Errors + 1;}                         
   if (TheLeafs->GFIPoint_fELoss == 0) {GFI_Errors = GFI_Errors + 1;}                          
   if (TheLeafs->GFIPoint_fDetectorID == 0) {GFI_Errors = GFI_Errors + 1;}                     
   if (TheLeafs->GFIPoint_fX == 0) {GFI_Errors = GFI_Errors + 1;}                         
   if (TheLeafs->GFIPoint_fY == 0) {GFI_Errors = GFI_Errors + 1;}                          
   if (TheLeafs->GFIPoint_fZ == 0) {GFI_Errors = GFI_Errors + 1;}                           
   if (TheLeafs->GFIPoint_fX_out == 0) {GFI_Errors = GFI_Errors + 1;}                       
   if (TheLeafs->GFIPoint_fY_out == 0) {GFI_Errors = GFI_Errors + 1;}                        
   if (TheLeafs->GFIPoint_fZ_out == 0) {GFI_Errors = GFI_Errors + 1;}                        
   if (TheLeafs->GFIPoint_fPx_out == 0) {GFI_Errors = GFI_Errors + 1;}                      
   if (TheLeafs->GFIPoint_fPy_out == 0) {GFI_Errors = GFI_Errors + 1;}                       
   if (TheLeafs->GFIPoint_fPz_out == 0) {GFI_Errors = GFI_Errors + 1;}                         
   if (TheLeafs->GFIPoint_fModule == 0) {GFI_Errors = GFI_Errors + 1;}                         
   
   // -----------------------------------
   // STaRTrack leafs:
   TheLeafs->STaRTraPoint_fUniqueID = TheLeafs->TheTree->FindLeaf("STaRTraPoint.fUniqueID");		// UInt_t[n] // This is an artifact of FairRoot. FairRoot is designed in such a way that each kind of leafs requires an fUniqueID and an fBits.
   TheLeafs->STaRTraPoint_fBits = TheLeafs->TheTree->FindLeaf("STaRTraPoint.fBits");			// UInt_t[n] // This is an artifact of FairRoot. FairRoot is designed in such a way that each kind of leafs requires an fUniqueID and an fBits.
   TheLeafs->STaRTraPoint_fLink = TheLeafs->TheTree->FindLeaf("STaRTraPoint.fLink");			// FairMultiLinkedData*[n]
   TheLeafs->STaRTraPoint_fTrackID = TheLeafs->TheTree->FindLeaf("STaRTraPoint.fTrackID");		// Int_t[n] // C++ index of where this track is located in the MC leafs.
   TheLeafs->STaRTraPoint_fEventId = TheLeafs->TheTree->FindLeaf("STaRTraPoint.fEventId");		// UInt_t[n]
   TheLeafs->STaRTraPoint_fPx = TheLeafs->TheTree->FindLeaf("STaRTraPoint.fPx");			// Double32_t[n] // Momentum in x-direction [GeV] at the entrance of the active detector volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->STaRTraPoint_fPy = TheLeafs->TheTree->FindLeaf("STaRTraPoint.fPy");			// Double32_t[n] // Momentum in y-direction [GeV] at the entrance of the active detector volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->STaRTraPoint_fPz = TheLeafs->TheTree->FindLeaf("STaRTraPoint.fPz");			// Double32_t[n] // Momentum in z-direction [GeV] at the entrance of the active detector volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->STaRTraPoint_fTime = TheLeafs->TheTree->FindLeaf("STaRTraPoint.fTime");			// Double32_t[n] // Time Of Flight between the beginning of this event and this specific interaction point [ns].
   TheLeafs->STaRTraPoint_fLength = TheLeafs->TheTree->FindLeaf("STaRTraPoint.fLength");		// Double32_t[n] // Total Track length since creation (averaged over detector) [cm].
   TheLeafs->STaRTraPoint_fELoss = TheLeafs->TheTree->FindLeaf("STaRTraPoint.fELoss");			// Double32_t[n] // Energy deposition at this specific interaction point [GeV].
   TheLeafs->STaRTraPoint_fDetectorID = TheLeafs->TheTree->FindLeaf("STaRTraPoint.fDetectorID");	// Int_t[n] // Identification of the detector. We already know however which detector we are talking about due to the name of the leaf!
   TheLeafs->STaRTraPoint_fX = TheLeafs->TheTree->FindLeaf("STaRTraPoint.fX");				// Double32_t[n] // x-coordinate [cm] at entrance of active volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->STaRTraPoint_fY = TheLeafs->TheTree->FindLeaf("STaRTraPoint.fY");				// Double32_t[n] // y-coordinate [cm] at entrance of active volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->STaRTraPoint_fZ = TheLeafs->TheTree->FindLeaf("STaRTraPoint.fZ");				// Double32_t[n] // z-coordinate [cm] at entrance of active volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->STaRTraPoint_fX_out = TheLeafs->TheTree->FindLeaf("STaRTraPoint.fX_out");			// Double32_t[n] // x-coordinate [cm] at exit of active volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->STaRTraPoint_fY_out = TheLeafs->TheTree->FindLeaf("STaRTraPoint.fY_out");			// Double32_t[n] // y-coordinate [cm] at exit of active volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->STaRTraPoint_fZ_out = TheLeafs->TheTree->FindLeaf("STaRTraPoint.fZ_out");			// Double32_t[n] // z-coordinate [cm] at exit of active volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->STaRTraPoint_fPx_out = TheLeafs->TheTree->FindLeaf("STaRTraPoint.fPx_out");		// Double32_t[n] // Momentum in x-direction [GeV] at the exit of the active detector volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->STaRTraPoint_fPy_out = TheLeafs->TheTree->FindLeaf("STaRTraPoint.fPy_out");			// Double32_t[n] // Momentum in y-direction [GeV] at the exit of the active detector volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->STaRTraPoint_fPz_out = TheLeafs->TheTree->FindLeaf("STaRTraPoint.fPz_out");		// Double32_t[n] // Momentum in z-direction [GeV] at the exit of the active detector volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->STaRTraPoint_fDetCopyID = TheLeafs->TheTree->FindLeaf("STaRTraPoint.fDetCopyID");		// Int_t[n]
   
   // Checking if the connection worked:
   if (TheLeafs->STaRTraPoint_fUniqueID == 0) {STaRTRack_Errors = STaRTRack_Errors + 1;}
   if (TheLeafs->STaRTraPoint_fBits == 0) {STaRTRack_Errors = STaRTRack_Errors + 1;}
   if (TheLeafs->STaRTraPoint_fLink == 0) {STaRTRack_Errors = STaRTRack_Errors + 1;}
   if (TheLeafs->STaRTraPoint_fTrackID == 0) {STaRTRack_Errors = STaRTRack_Errors + 1;}
   if (TheLeafs->STaRTraPoint_fEventId == 0) {STaRTRack_Errors = STaRTRack_Errors + 1;}
   if (TheLeafs->STaRTraPoint_fPx == 0) {STaRTRack_Errors = STaRTRack_Errors + 1;}
   if (TheLeafs->STaRTraPoint_fPy == 0) {STaRTRack_Errors = STaRTRack_Errors + 1;}
   if (TheLeafs->STaRTraPoint_fPz == 0) {STaRTRack_Errors = STaRTRack_Errors + 1;}
   if (TheLeafs->STaRTraPoint_fTime == 0) {STaRTRack_Errors = STaRTRack_Errors + 1;}
   if (TheLeafs->STaRTraPoint_fLength == 0) {STaRTRack_Errors = STaRTRack_Errors + 1;}
   if (TheLeafs->STaRTraPoint_fELoss == 0) {STaRTRack_Errors = STaRTRack_Errors + 1;}
   if (TheLeafs->STaRTraPoint_fDetectorID == 0) {STaRTRack_Errors = STaRTRack_Errors + 1;}
   if (TheLeafs->STaRTraPoint_fX == 0) {STaRTRack_Errors = STaRTRack_Errors + 1;}	
   if (TheLeafs->STaRTraPoint_fY == 0) {STaRTRack_Errors = STaRTRack_Errors + 1;}	
   if (TheLeafs->STaRTraPoint_fZ == 0) {STaRTRack_Errors = STaRTRack_Errors + 1;}	
   if (TheLeafs->STaRTraPoint_fX_out == 0) {STaRTRack_Errors = STaRTRack_Errors + 1;}
   if (TheLeafs->STaRTraPoint_fY_out == 0) {STaRTRack_Errors = STaRTRack_Errors + 1;}
   if (TheLeafs->STaRTraPoint_fZ_out == 0) {STaRTRack_Errors = STaRTRack_Errors + 1;}
   if (TheLeafs->STaRTraPoint_fPx_out == 0) {STaRTRack_Errors = STaRTRack_Errors + 1;}
   if (TheLeafs->STaRTraPoint_fPy_out == 0) {STaRTRack_Errors = STaRTRack_Errors + 1;}
   if (TheLeafs->STaRTraPoint_fPz_out == 0) {STaRTRack_Errors = STaRTRack_Errors + 1;}
   if (TheLeafs->STaRTraPoint_fDetCopyID == 0) {STaRTRack_Errors = STaRTRack_Errors + 1;}
   
   // -----------------------------------
   // DCH leafs:
   TheLeafs->DCHPoint_fUniqueID = TheLeafs->TheTree->FindLeaf("DCHPoint.fUniqueID");			// UInt_t[n] // This is an artifact of FairRoot. FairRoot is designed in such a way that each kind of leafs requires an fUniqueID and an fBits.
   TheLeafs->DCHPoint_fBits = TheLeafs->TheTree->FindLeaf("DCHPoint.fBits");				// UInt_t[n] // This is an artifact of FairRoot. FairRoot is designed in such a way that each kind of leafs requires an fUniqueID and an fBits.
   TheLeafs->DCHPoint_fLink = TheLeafs->TheTree->FindLeaf("DCHPoint.fLink");				// FairMultiLinkedData*[n]
   TheLeafs->DCHPoint_fTrackID = TheLeafs->TheTree->FindLeaf("DCHPoint.fTrackID");			// Int_t[n]  // C++ index of where this track is located in the MC leafs. "DIMA"
   TheLeafs->DCHPoint_fEventId = TheLeafs->TheTree->FindLeaf("DCHPoint.fEventId");			// UInt_t[n]
   TheLeafs->DCHPoint_fPx = TheLeafs->TheTree->FindLeaf("DCHPoint.fPx");				// Double32_t[n] // Momentum in x-direction [GeV] at the entrance of the active detector volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->DCHPoint_fPy = TheLeafs->TheTree->FindLeaf("DCHPoint.fPy");				// Double32_t[n] // Momentum in y-direction [GeV] at the entrance of the active detector volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->DCHPoint_fPz = TheLeafs->TheTree->FindLeaf("DCHPoint.fPz");				// Double32_t[n] // Momentum in z-direction [GeV] at the entrance of the active detector volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->DCHPoint_fTime = TheLeafs->TheTree->FindLeaf("DCHPoint.fTime");				// Double32_t[n] // Time Of Flight between the beginning of this event and this specific interaction point [ns].
   TheLeafs->DCHPoint_fLength = TheLeafs->TheTree->FindLeaf("DCHPoint.fLength");			// Double32_t[n] // Total Track length since creation (averaged over detector) [cm].
   TheLeafs->DCHPoint_fELoss = TheLeafs->TheTree->FindLeaf("DCHPoint.fELoss");				// Double32_t[n] // Energy deposition at this specific interaction point [GeV].
   TheLeafs->DCHPoint_fDetectorID = TheLeafs->TheTree->FindLeaf("DCHPoint.fDetectorID");		// Int_t[n] // Identification of the detector. We already know however which detector we are talking about due to the name of the leaf!
   TheLeafs->DCHPoint_fX = TheLeafs->TheTree->FindLeaf("DCHPoint.fX");					// Double32_t[n] // x-coordinate [cm] at entrance of active volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->DCHPoint_fY = TheLeafs->TheTree->FindLeaf("DCHPoint.fY");					// Double32_t[n] // y-coordinate [cm] at entrance of active volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->DCHPoint_fZ = TheLeafs->TheTree->FindLeaf("DCHPoint.fZ");					// Double32_t[n] // z-coordinate [cm] at entrance of active volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->DCHPoint_fX_out = TheLeafs->TheTree->FindLeaf("DCHPoint.fX_out");				// Double32_t[n] // x-coordinate [cm] at exit of active volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->DCHPoint_fY_out = TheLeafs->TheTree->FindLeaf("DCHPoint.fY_out");				// Double32_t[n] // y-coordinate [cm] at exit of active volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->DCHPoint_fZ_out = TheLeafs->TheTree->FindLeaf("DCHPoint.fZ_out");				// Double32_t[n] // z-coordinate [cm] at exit of active volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->DCHPoint_fPx_out = TheLeafs->TheTree->FindLeaf("DCHPoint.fPx_out");			// Double32_t[n] // Momentum in x-direction [GeV] at the exit of the active detector volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->DCHPoint_fPy_out = TheLeafs->TheTree->FindLeaf("DCHPoint.fPy_out");			// Double32_t[n] // Momentum in y-direction [GeV] at the exit of the active detector volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->DCHPoint_fPz_out = TheLeafs->TheTree->FindLeaf("DCHPoint.fPz_out");			// Double32_t[n] // Momentum in z-direction [GeV] at the exit of the active detector volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->DCHPoint_fLocalX_in = TheLeafs->TheTree->FindLeaf("DCHPoint.fLocalX_in");			// Double32_t[n] // probably x-coordinate [cm] at entrance of a local part of the DCH active volumes. Active volume refers to a Geant4 active detector volume.
   TheLeafs->DCHPoint_fLocalY_in = TheLeafs->TheTree->FindLeaf("DCHPoint.fLocalY_in");			// Double32_t[n] // probably x-coordinate [cm] at entrance of a local part of the DCH active volumes. Active volume refers to a Geant4 active detector volume.
   TheLeafs->DCHPoint_fLocalZ_in = TheLeafs->TheTree->FindLeaf("DCHPoint.fLocalZ_in");			// Double32_t[n] // probably x-coordinate [cm] at entrance of a local part of the DCH active volumes. Active volume refers to a Geant4 active detector volume.
   TheLeafs->DCHPoint_fLocalPx_in = TheLeafs->TheTree->FindLeaf("DCHPoint.fLocalPx_in");		// Double32_t[n] // probably Momentum in x-direction [GeV] at the entrance of a local part of the DCH active detector volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->DCHPoint_fLocalPy_in = TheLeafs->TheTree->FindLeaf("DCHPoint.fLocalPy_in");		// Double32_t[n] // probably Momentum in y-direction [GeV] at the entrance of a local part of the DCH active detector volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->DCHPoint_fLocalPz_in = TheLeafs->TheTree->FindLeaf("DCHPoint.fLocalPz_in");		// Double32_t[n] // probably Momentum in z-direction [GeV] at the entrance of a local part of the DCH active detector volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->DCHPoint_fLocalX_out = TheLeafs->TheTree->FindLeaf("DCHPoint.fLocalX_out");		// Double32_t[n] // probably x-coordinate [cm] at exit of a local part of the DCH active volumes. Active volume refers to a Geant4 active detector volume.
   TheLeafs->DCHPoint_fLocalY_out = TheLeafs->TheTree->FindLeaf("DCHPoint.fLocalY_out");		// Double32_t[n] // probably y-coordinate [cm] at exit of a local part of the DCH active volumes. Active volume refers to a Geant4 active detector volume.
   TheLeafs->DCHPoint_fLocalZ_out = TheLeafs->TheTree->FindLeaf("DCHPoint.fLocalZ_out");		// Double32_t[n] // probably z-coordinate [cm] at exit of a local part of the DCH active volumes. Active volume refers to a Geant4 active detector volume.
   TheLeafs->DCHPoint_fLocalPx_out = TheLeafs->TheTree->FindLeaf("DCHPoint.fLocalPx_out");		// Double32_t[n] // probably Momentum in x-direction [GeV] at the exit of a local part of the DCH active detector volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->DCHPoint_fLocalPy_out = TheLeafs->TheTree->FindLeaf("DCHPoint.fLocalPy_out");		// Double32_t[n] // probably Momentum in y-direction [GeV] at the exit of a local part of the DCH active detector volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->DCHPoint_fLocalPz_out = TheLeafs->TheTree->FindLeaf("DCHPoint.fLocalPz_out");		// Double32_t[n] // probably Momentum in z-direction [GeV] at the exit of a local part of the DCH active detector volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->DCHPoint_fModule = TheLeafs->TheTree->FindLeaf("DCHPoint.fModule");			// Int_t[n]
   TheLeafs->DCHPoint_fLayer = TheLeafs->TheTree->FindLeaf("DCHPoint.fLayer");				// Int_t[n]
   TheLeafs->DCHPoint_fCell = TheLeafs->TheTree->FindLeaf("DCHPoint.fCell");				// Int_t[n]
   
   // Checking if the connection worked:
   if (TheLeafs->DCHPoint_fUniqueID == 0) {DCH_Errors = DCH_Errors + 1;}
   if (TheLeafs->DCHPoint_fBits == 0) {DCH_Errors = DCH_Errors + 1;}
   if (TheLeafs->DCHPoint_fLink == 0) {DCH_Errors = DCH_Errors + 1;}
   if (TheLeafs->DCHPoint_fTrackID == 0) {DCH_Errors = DCH_Errors + 1;}
   if (TheLeafs->DCHPoint_fEventId == 0) {DCH_Errors = DCH_Errors + 1;}	
   if (TheLeafs->DCHPoint_fPx == 0) {DCH_Errors = DCH_Errors + 1;}	
   if (TheLeafs->DCHPoint_fPy == 0) {DCH_Errors = DCH_Errors + 1;}	
   if (TheLeafs->DCHPoint_fPz == 0) {DCH_Errors = DCH_Errors + 1;}	
   if (TheLeafs->DCHPoint_fTime == 0) {DCH_Errors = DCH_Errors + 1;}	
   if (TheLeafs->DCHPoint_fLength == 0) {DCH_Errors = DCH_Errors + 1;}	
   if (TheLeafs->DCHPoint_fELoss == 0) {DCH_Errors = DCH_Errors + 1;}	
   if (TheLeafs->DCHPoint_fDetectorID == 0) {DCH_Errors = DCH_Errors + 1;}
   if (TheLeafs->DCHPoint_fX == 0) {DCH_Errors = DCH_Errors + 1;}	
   if (TheLeafs->DCHPoint_fY == 0) {DCH_Errors = DCH_Errors + 1;}	
   if (TheLeafs->DCHPoint_fZ == 0) {DCH_Errors = DCH_Errors + 1;}	
   if (TheLeafs->DCHPoint_fX_out == 0) {DCH_Errors = DCH_Errors + 1;}	
   if (TheLeafs->DCHPoint_fY_out == 0) {DCH_Errors = DCH_Errors + 1;}	
   if (TheLeafs->DCHPoint_fZ_out == 0) {DCH_Errors = DCH_Errors + 1;}	
   if (TheLeafs->DCHPoint_fPx_out == 0) {DCH_Errors = DCH_Errors + 1;}	
   if (TheLeafs->DCHPoint_fPy_out == 0) {DCH_Errors = DCH_Errors + 1;}		
   if (TheLeafs->DCHPoint_fPz_out == 0) {DCH_Errors = DCH_Errors + 1;}	
   if (TheLeafs->DCHPoint_fLocalX_in == 0) {DCH_Errors = DCH_Errors + 1;}
   if (TheLeafs->DCHPoint_fLocalY_in == 0) {DCH_Errors = DCH_Errors + 1;}
   if (TheLeafs->DCHPoint_fLocalZ_in == 0) {DCH_Errors = DCH_Errors + 1;}
   if (TheLeafs->DCHPoint_fLocalPx_in == 0) {DCH_Errors = DCH_Errors + 1;}
   if (TheLeafs->DCHPoint_fLocalPy_in == 0) {DCH_Errors = DCH_Errors + 1;}
   if (TheLeafs->DCHPoint_fLocalPz_in == 0) {DCH_Errors = DCH_Errors + 1;}
   if (TheLeafs->DCHPoint_fLocalX_out == 0) {DCH_Errors = DCH_Errors + 1;}
   if (TheLeafs->DCHPoint_fLocalY_out == 0) {DCH_Errors = DCH_Errors + 1;}
   if (TheLeafs->DCHPoint_fLocalZ_out == 0) {DCH_Errors = DCH_Errors + 1;}
   if (TheLeafs->DCHPoint_fLocalPx_out == 0) {DCH_Errors = DCH_Errors + 1;}
   if (TheLeafs->DCHPoint_fLocalPy_out == 0) {DCH_Errors = DCH_Errors + 1;}
   if (TheLeafs->DCHPoint_fLocalPz_out == 0) {DCH_Errors = DCH_Errors + 1;}
   if (TheLeafs->DCHPoint_fModule == 0) {DCH_Errors = DCH_Errors + 1;}
   if (TheLeafs->DCHPoint_fLayer == 0) {DCH_Errors = DCH_Errors + 1;}
   if (TheLeafs->DCHPoint_fCell == 0) {DCH_Errors = DCH_Errors + 1;}
   
   // -----------------------------------
   // TOF wall leafs:
   TheLeafs->TOFPoint_fUniqueID = TheLeafs->TheTree->FindLeaf("TOFPoint.fUniqueID");		// UInt_t[n] // This is an artifact of FairRoot. FairRoot is designed in such a way that each kind of leafs requires an fUniqueID and an fBits.
   TheLeafs->TOFPoint_fBits = TheLeafs->TheTree->FindLeaf("TOFPoint.fBits");			// UInt_t[n] // This is an artifact of FairRoot. FairRoot is designed in such a way that each kind of leafs requires an fUniqueID and an fBits.
   TheLeafs->TOFPoint_fLink = TheLeafs->TheTree->FindLeaf("TOFPoint.fLink");			// FairMultiLinkedData*[n]
   TheLeafs->TOFPoint_fTrackID = TheLeafs->TheTree->FindLeaf("TOFPoint.fTrackID");		// Int_t[n] // C++ index of where this track is located in the MC leafs.
   TheLeafs->TOFPoint_fEventId = TheLeafs->TheTree->FindLeaf("TOFPoint.fEventId");		// UInt_t[n]
   TheLeafs->TOFPoint_fPx = TheLeafs->TheTree->FindLeaf("TOFPoint.fPx");			// Double32_t[n] // Momentum in x-direction [GeV] at the entrance of the active detector volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->TOFPoint_fPy = TheLeafs->TheTree->FindLeaf("TOFPoint.fPy");			// Double32_t[n] // Momentum in y-direction [GeV] at the entrance of the active detector volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->TOFPoint_fPz = TheLeafs->TheTree->FindLeaf("TOFPoint.fPz");			// Double32_t[n] // Momentum in z-direction [GeV] at the entrance of the active detector volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->TOFPoint_fTime = TheLeafs->TheTree->FindLeaf("TOFPoint.fTime");			// Double32_t[n] // Time Of Flight between the beginning of this event and this specific interaction point [ns].
   TheLeafs->TOFPoint_fLength = TheLeafs->TheTree->FindLeaf("TOFPoint.fLength");		// Double32_t[n] // Total Track length since creation (averaged over detector) [cm].
   TheLeafs->TOFPoint_fELoss = TheLeafs->TheTree->FindLeaf("TOFPoint.fELoss");			// Double32_t[n] // Energy deposition at this specific interaction point [GeV].
   TheLeafs->TOFPoint_fDetectorID = TheLeafs->TheTree->FindLeaf("TOFPoint.fDetectorID");	// Int_t[n] // Identification of the detector. We already know however which detector we are talking about due to the name of the leaf!
   TheLeafs->TOFPoint_fX = TheLeafs->TheTree->FindLeaf("TOFPoint.fX");				// Double32_t[n] // x-coordinate [cm] at entrance of active volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->TOFPoint_fY = TheLeafs->TheTree->FindLeaf("TOFPoint.fY");				// Double32_t[n] // y-coordinate [cm] at entrance of active volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->TOFPoint_fZ = TheLeafs->TheTree->FindLeaf("TOFPoint.fZ");				// Double32_t[n] // z-coordinate [cm] at entrance of active volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->TOFPoint_fX_out = TheLeafs->TheTree->FindLeaf("TOFPoint.fX_out");			// Double32_t[n] // x-coordinate [cm] at exit of active volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->TOFPoint_fY_out = TheLeafs->TheTree->FindLeaf("TOFPoint.fY_out");			// Double32_t[n] // y-coordinate [cm] at exit of active volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->TOFPoint_fZ_out = TheLeafs->TheTree->FindLeaf("TOFPoint.fZ_out");			// Double32_t[n] // z-coordinate [cm] at exit of active volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->TOFPoint_fPx_out = TheLeafs->TheTree->FindLeaf("TOFPoint.fPx_out");		// Double32_t[n] // Momentum in x-direction [GeV] at the exit of the active detector volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->TOFPoint_fPy_out = TheLeafs->TheTree->FindLeaf("TOFPoint.fPy_out");			// Double32_t[n] // Momentum in y-direction [GeV] at the exit of the active detector volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->TOFPoint_fPz_out = TheLeafs->TheTree->FindLeaf("TOFPoint.fPz_out");		// Double32_t[n] // Momentum in z-direction [GeV] at the exit of the active detector volume. Active volume refers to a Geant4 active detector volume.
   
   // Checking if the connection worked:
   if (TheLeafs->TOFPoint_fUniqueID == 0) {TOF_Errors = TOF_Errors + 1;}
   if (TheLeafs->TOFPoint_fBits == 0) {TOF_Errors = TOF_Errors + 1;}
   if (TheLeafs->TOFPoint_fLink == 0) {TOF_Errors = TOF_Errors + 1;}
   if (TheLeafs->TOFPoint_fTrackID == 0) {TOF_Errors = TOF_Errors + 1;}
   if (TheLeafs->TOFPoint_fEventId == 0) {TOF_Errors = TOF_Errors + 1;}
   if (TheLeafs->TOFPoint_fPx == 0) {TOF_Errors = TOF_Errors + 1;}
   if (TheLeafs->TOFPoint_fPy == 0) {TOF_Errors = TOF_Errors + 1;}
   if (TheLeafs->TOFPoint_fPz == 0) {TOF_Errors = TOF_Errors + 1;}
   if (TheLeafs->TOFPoint_fTime == 0) {TOF_Errors = TOF_Errors + 1;}
   if (TheLeafs->TOFPoint_fLength == 0) {TOF_Errors = TOF_Errors + 1;}
   if (TheLeafs->TOFPoint_fELoss == 0) {TOF_Errors = TOF_Errors + 1;}
   if (TheLeafs->TOFPoint_fDetectorID == 0) {TOF_Errors = TOF_Errors + 1;}
   if (TheLeafs->TOFPoint_fX == 0) {TOF_Errors = TOF_Errors + 1;}	
   if (TheLeafs->TOFPoint_fY == 0) {TOF_Errors = TOF_Errors + 1;}	
   if (TheLeafs->TOFPoint_fZ == 0) {TOF_Errors = TOF_Errors + 1;}	
   if (TheLeafs->TOFPoint_fX_out == 0) {TOF_Errors = TOF_Errors + 1;}
   if (TheLeafs->TOFPoint_fY_out == 0) {TOF_Errors = TOF_Errors + 1;}
   if (TheLeafs->TOFPoint_fZ_out == 0) {TOF_Errors = TOF_Errors + 1;}
   if (TheLeafs->TOFPoint_fPx_out == 0) {TOF_Errors = TOF_Errors + 1;}
   if (TheLeafs->TOFPoint_fPy_out == 0) {TOF_Errors = TOF_Errors + 1;}
   if (TheLeafs->TOFPoint_fPz_out == 0) {TOF_Errors = TOF_Errors + 1;}
   
   // -----------------------------------
   // mTOF leafs:
   TheLeafs->mTOFPoint_fUniqueID = TheLeafs->TheTree->FindLeaf("mTOFPoint.fUniqueID");		// UInt_t[n] // This is an artifact of FairRoot. FairRoot is designed in such a way that each kind of leafs requires an fUniqueID and an fBits.
   TheLeafs->mTOFPoint_fBits = TheLeafs->TheTree->FindLeaf("mTOFPoint.fBits");			// UInt_t[n] // This is an artifact of FairRoot. FairRoot is designed in such a way that each kind of leafs requires an fUniqueID and an fBits.
   TheLeafs->mTOFPoint_fLink = TheLeafs->TheTree->FindLeaf("mTOFPoint.fLink");			// FairMultiLinkedData*[n]
   TheLeafs->mTOFPoint_fTrackID = TheLeafs->TheTree->FindLeaf("mTOFPoint.fTrackID");		// Int_t[n] // C++ index of where this track is located in the MC leafs.
   TheLeafs->mTOFPoint_fEventId = TheLeafs->TheTree->FindLeaf("mTOFPoint.fEventId");		// UInt_t[n]
   TheLeafs->mTOFPoint_fPx = TheLeafs->TheTree->FindLeaf("mTOFPoint.fPx");			// Double32_t[n] // Momentum in x-direction [GeV] at the entrance of the active detector volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->mTOFPoint_fPy = TheLeafs->TheTree->FindLeaf("mTOFPoint.fPy");			// Double32_t[n] // Momentum in y-direction [GeV] at the entrance of the active detector volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->mTOFPoint_fPz = TheLeafs->TheTree->FindLeaf("mTOFPoint.fPz");			// Double32_t[n] // Momentum in z-direction [GeV] at the entrance of the active detector volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->mTOFPoint_fTime = TheLeafs->TheTree->FindLeaf("mTOFPoint.fTime");			// Double32_t[n] // Time Of Flight between the beginning of this event and this specific interaction point [ns].
   TheLeafs->mTOFPoint_fLength = TheLeafs->TheTree->FindLeaf("mTOFPoint.fLength");		// Double32_t[n] // Total Track length since creation (averaged over detector) [cm].
   TheLeafs->mTOFPoint_fELoss = TheLeafs->TheTree->FindLeaf("mTOFPoint.fELoss");		// Double32_t[n] // Energy deposition at this specific interaction point [GeV].
   TheLeafs->mTOFPoint_fDetectorID = TheLeafs->TheTree->FindLeaf("mTOFPoint.fDetectorID");	// Int_t[n] // Identification of the detector. We already know however which detector we are talking about due to the name of the leaf!
   TheLeafs->mTOFPoint_fX = TheLeafs->TheTree->FindLeaf("mTOFPoint.fX");			// Double32_t[n] // x-coordinate [cm] at entrance of active volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->mTOFPoint_fY = TheLeafs->TheTree->FindLeaf("mTOFPoint.fY");			// Double32_t[n] // y-coordinate [cm] at entrance of active volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->mTOFPoint_fZ = TheLeafs->TheTree->FindLeaf("mTOFPoint.fZ");			// Double32_t[n] // z-coordinate [cm] at entrance of active volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->mTOFPoint_fX_out = TheLeafs->TheTree->FindLeaf("mTOFPoint.fX_out");		// Double32_t[n] // x-coordinate [cm] at exit of active volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->mTOFPoint_fY_out = TheLeafs->TheTree->FindLeaf("mTOFPoint.fY_out");		// Double32_t[n] // y-coordinate [cm] at exit of active volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->mTOFPoint_fZ_out = TheLeafs->TheTree->FindLeaf("mTOFPoint.fZ_out");		// Double32_t[n] // z-coordinate [cm] at exit of active volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->mTOFPoint_fPx_out = TheLeafs->TheTree->FindLeaf("mTOFPoint.fPx_out");		// Double32_t[n] // Momentum in x-direction [GeV] at the exit of the active detector volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->mTOFPoint_fPy_out = TheLeafs->TheTree->FindLeaf("mTOFPoint.fPy_out");		// Double32_t[n] // Momentum in y-direction [GeV] at the exit of the active detector volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->mTOFPoint_fPz_out = TheLeafs->TheTree->FindLeaf("mTOFPoint.fPz_out");		// Double32_t[n] // Momentum in z-direction [GeV] at the exit of the active detector volume. Active volume refers to a Geant4 active detector volume.
   
   // Checking if the connection worked:
   if (TheLeafs->mTOFPoint_fUniqueID == 0) {mTOF_Errors = mTOF_Errors + 1;}
   if (TheLeafs->mTOFPoint_fBits == 0) {mTOF_Errors = mTOF_Errors + 1;}
   if (TheLeafs->mTOFPoint_fLink == 0) {mTOF_Errors = mTOF_Errors + 1;}
   if (TheLeafs->mTOFPoint_fTrackID == 0) {mTOF_Errors = mTOF_Errors + 1;}
   if (TheLeafs->mTOFPoint_fEventId == 0) {mTOF_Errors = mTOF_Errors + 1;}
   if (TheLeafs->mTOFPoint_fPx == 0) {mTOF_Errors = mTOF_Errors + 1;}
   if (TheLeafs->mTOFPoint_fPy == 0) {mTOF_Errors = mTOF_Errors + 1;}
   if (TheLeafs->mTOFPoint_fPz == 0) {mTOF_Errors = mTOF_Errors + 1;}
   if (TheLeafs->mTOFPoint_fTime == 0) {mTOF_Errors = mTOF_Errors + 1;}
   if (TheLeafs->mTOFPoint_fLength == 0) {mTOF_Errors = mTOF_Errors + 1;}
   if (TheLeafs->mTOFPoint_fELoss == 0) {mTOF_Errors = mTOF_Errors + 1;}
   if (TheLeafs->mTOFPoint_fDetectorID == 0) {mTOF_Errors = mTOF_Errors + 1;}
   if (TheLeafs->mTOFPoint_fX == 0) {mTOF_Errors = mTOF_Errors + 1;}	
   if (TheLeafs->mTOFPoint_fY == 0) {mTOF_Errors = mTOF_Errors + 1;}	
   if (TheLeafs->mTOFPoint_fZ == 0) {mTOF_Errors = mTOF_Errors + 1;}	
   if (TheLeafs->mTOFPoint_fX_out == 0) {mTOF_Errors = mTOF_Errors + 1;}
   if (TheLeafs->mTOFPoint_fY_out == 0) {mTOF_Errors = mTOF_Errors + 1;}
   if (TheLeafs->mTOFPoint_fZ_out == 0) {mTOF_Errors = mTOF_Errors + 1;}
   if (TheLeafs->mTOFPoint_fPx_out == 0) {mTOF_Errors = mTOF_Errors + 1;}
   if (TheLeafs->mTOFPoint_fPy_out == 0) {mTOF_Errors = mTOF_Errors + 1;}
   if (TheLeafs->mTOFPoint_fPz_out == 0) {mTOF_Errors = mTOF_Errors + 1;}
   
   // -----------------------------------
   // LandPoint leafs:
   
   if (Neu_Ex==2)
   {
   TheLeafs->LandPoint_fUniqueID = TheLeafs->TheTree->FindLeaf("NeulandPoints.fUniqueID");	// UInt_t[n] // This is an artifact of FairRoot. FairRoot is designed in such a way that each kind of leafs requires an fUniqueID and an fBits.
   TheLeafs->LandPoint_fBits = TheLeafs->TheTree->FindLeaf("NeulandPoints.fBits");		// UInt_t[n] // This is an artifact of FairRoot. FairRoot is designed in such a way that each kind of leafs requires an fUniqueID and an fBits.
   TheLeafs->LandPoint_fLink = TheLeafs->TheTree->FindLeaf("NeulandPoints.fLink");		// FairMultiLinkedData*[n]
   TheLeafs->LandPoint_fTrackID = TheLeafs->TheTree->FindLeaf("NeulandPoints.fTrackID");	// Int_t[n] // C++ index of where this track is located in the MC leafs. "DIMA"
   TheLeafs->LandPoint_fEventId = TheLeafs->TheTree->FindLeaf("NeulandPoints.fEventId");	// UInt_t[n]
   TheLeafs->LandPoint_fPx = TheLeafs->TheTree->FindLeaf("NeulandPoints.fPx");			// Double32_t[n] // Momentum in x-direction [GeV] at the entrance of the active detector volume. Active volume refers to a Geant4 active detector volume. "DIMA"
   TheLeafs->LandPoint_fPy = TheLeafs->TheTree->FindLeaf("NeulandPoints.fPy");			// Double32_t[n] // Momentum in y-direction [GeV] at the entrance of the active detector volume. Active volume refers to a Geant4 active detector volume. "DIMA"
   TheLeafs->LandPoint_fPz = TheLeafs->TheTree->FindLeaf("NeulandPoints.fPz");			// Double32_t[n] // Momentum in z-direction [GeV] at the entrance of the active detector volume. Active volume refers to a Geant4 active detector volume. "DIMA"
   TheLeafs->LandPoint_fTime = TheLeafs->TheTree->FindLeaf("NeulandPoints.fTime");		// Double32_t[n] // Time Of Flight between the beginning of this event and this specific interaction point [ns]. "DIMA"
   TheLeafs->LandPoint_fLength = TheLeafs->TheTree->FindLeaf("NeulandPoints.fLength");		// Double32_t[n] // Total Track length since creation (averaged over detector) [cm]. "DIMA"
   TheLeafs->LandPoint_fELoss = TheLeafs->TheTree->FindLeaf("NeulandPoints.fELoss");		// Double32_t[n] // Energy deposition at this specific interaction point [GeV]. "DIMA"
   TheLeafs->LandPoint_fDetectorID = TheLeafs->TheTree->FindLeaf("NeulandPoints.fDetectorID");	// Int_t[n]      // Identification of the detector. We already know however which detector we are talking about due to the name of the leaf! "DIMA"
   TheLeafs->LandPoint_fX = TheLeafs->TheTree->FindLeaf("NeulandPoints.fX");			// Double32_t[n] // x-coordinate [cm] at entrance of active volume. Active volume refers to a Geant4 active detector volume. "DIMA"
   TheLeafs->LandPoint_fY = TheLeafs->TheTree->FindLeaf("NeulandPoints.fY");			// Double32_t[n] // y-coordinate [cm] at entrance of active volume. Active volume refers to a Geant4 active detector volume. "DIMA"
   TheLeafs->LandPoint_fZ = TheLeafs->TheTree->FindLeaf("NeulandPoints.fZ");			// Double32_t[n] // z-coordinate [cm] at entrance of active volume. Active volume refers to a Geant4 active detector volume. "DIMA"
   TheLeafs->LandPoint_fX_out = TheLeafs->TheTree->FindLeaf("NeulandPoints.fX_out");		// Double32_t[n] // x-coordinate [cm] at exit of active volume. Active volume refers to a Geant4 active detector volume. "DIMA"
   TheLeafs->LandPoint_fY_out = TheLeafs->TheTree->FindLeaf("NeulandPoints.fY_out");		// Double32_t[n] // y-coordinate [cm] at exit of active volume. Active volume refers to a Geant4 active detector volume. "DIMA"
   TheLeafs->LandPoint_fZ_out = TheLeafs->TheTree->FindLeaf("NeulandPoints.fZ_out");		// Double32_t[n] // z-coordinate [cm] at exit of active volume. Active volume refers to a Geant4 active detector volume. "DIMA"
   TheLeafs->LandPoint_fPx_out = TheLeafs->TheTree->FindLeaf("NeulandPoints.fPx_out");		// Double32_t[n] // Momentum in x-direction [GeV] at the exit of the active detector volume. Active volume refers to a Geant4 active detector volume. "DIMA"
   TheLeafs->LandPoint_fPy_out = TheLeafs->TheTree->FindLeaf("NeulandPoints.fPy_out");		// Double32_t[n] // Momentum in y-direction [GeV] at the exit of the active detector volume. Active volume refers to a Geant4 active detector volume. "DIMA"
   TheLeafs->LandPoint_fPz_out = TheLeafs->TheTree->FindLeaf("NeulandPoints.fPz_out");		// Double32_t[n] // Momentum in z-direction [GeV] at the exit of the active detector volume. Active volume refers to a Geant4 active detector volume. "DIMA"
   TheLeafs->LandPoint_fSector = TheLeafs->TheTree->FindLeaf("NeulandPoints.fDetectorID");		// Int_t[n] // Number of the specific bar of NeuLAND in which the hit is located. It ranges from 1 to 3000 for full NeuLAND. "DIMA"
   TheLeafs->LandPoint_fPaddleNb = TheLeafs->TheTree->FindLeaf("NeulandPoints.fPaddleNb");	// Int_t[n]
   TheLeafs->LandPoint_fPaddleTyp = TheLeafs->TheTree->FindLeaf("NeulandPoints.fPaddleTyp");	// Int_t[n]
   TheLeafs->LandPoint_fLightYield = TheLeafs->TheTree->FindLeaf("NeulandPoints.fLightYield");	// Double32_t[n] // If the energy deposit (dE/dx) is low, then the produced scintillator light is proportional to dE/dx: dN/dx = c*dE/dx where N stands for number of photons produced
												// (those photons carry a certain energy distribution). If dE/dx becomes higher however, the number of produced photons saturates. This saturation is descibed by Birk's
												// law: dL/dx = (S*dE/dx)(1 + B1*dE/dx). Here S and B1 are constants. dL/dx is now STILL energy deposition, but now with the saturation effect taken into account.
												// Then the produced scintillator light is given by dN/dx = c*dL/dx. This is now valid for all energy ranges. N is the number of produced photons. Hence the quantity
												// LandPoint_fLightYield->GetValue(k) holds the amount of deposited energy at the k-th interaction point in NeuLAND. Its unit is [GeV] and the saturation already is
												// taken into account: The number of produced photons at the k-th interaction point is given by N = c*LandPoint_fLightYield->GetValue(k) where c is a constant that
												// only depends on the scintillator material and the particle. It holds up to all energy ranges, because the saturation effect is taken into account. For fEloss 
												// this is not the case. Hence N = c*LandPoint_fLightYield->GetValue(k) gives the number of produced photons at the k-th interaction point. Each photon carries an energy
												// which is a sample of the same energy distribution, that depends also on LandPoint_fLightYield->GetValue(k) and on the material and particle of course.
												// hence to call a particle detected if a certain amount of light is PRODUCED, one could simply put an energy threshold on fLightYield per scintillator bar. Then 
												// the constant c is absorbed in this threshold. In order to put the light propagation in there however, one has to know c and generate a number of photons. One
												// also has to know the energy distribution of these photons to generate them and then let them propagate...
   
   }
   else
   {
   TheLeafs->LandPoint_fUniqueID = TheLeafs->TheTree->FindLeaf("LandPoint.fUniqueID");		// UInt_t[n] // This is an artifact of FairRoot. FairRoot is designed in such a way that each kind of leafs requires an fUniqueID and an fBits.
   TheLeafs->LandPoint_fBits = TheLeafs->TheTree->FindLeaf("LandPoint.fBits");			// UInt_t[n] // This is an artifact of FairRoot. FairRoot is designed in such a way that each kind of leafs requires an fUniqueID and an fBits.
   TheLeafs->LandPoint_fLink = TheLeafs->TheTree->FindLeaf("LandPoint.fLink");			// FairMultiLinkedData*[n]
   TheLeafs->LandPoint_fTrackID = TheLeafs->TheTree->FindLeaf("LandPoint.fTrackID");		// Int_t[n] // C++ index of where this track is located in the MC leafs. "DIMA"
   TheLeafs->LandPoint_fEventId = TheLeafs->TheTree->FindLeaf("LandPoint.fEventId");		// UInt_t[n]
   TheLeafs->LandPoint_fPx = TheLeafs->TheTree->FindLeaf("LandPoint.fPx");			// Double32_t[n] // Momentum in x-direction [GeV] at the entrance of the active detector volume. Active volume refers to a Geant4 active detector volume. "DIMA"
   TheLeafs->LandPoint_fPy = TheLeafs->TheTree->FindLeaf("LandPoint.fPy");			// Double32_t[n] // Momentum in y-direction [GeV] at the entrance of the active detector volume. Active volume refers to a Geant4 active detector volume. "DIMA"
   TheLeafs->LandPoint_fPz = TheLeafs->TheTree->FindLeaf("LandPoint.fPz");			// Double32_t[n] // Momentum in z-direction [GeV] at the entrance of the active detector volume. Active volume refers to a Geant4 active detector volume. "DIMA"
   TheLeafs->LandPoint_fTime = TheLeafs->TheTree->FindLeaf("LandPoint.fTime");			// Double32_t[n] // Time Of Flight between the beginning of this event and this specific interaction point [ns]. "DIMA"
   TheLeafs->LandPoint_fLength = TheLeafs->TheTree->FindLeaf("LandPoint.fLength");		// Double32_t[n] // Total Track length since creation (averaged over detector) [cm]. "DIMA"
   TheLeafs->LandPoint_fELoss = TheLeafs->TheTree->FindLeaf("LandPoint.fELoss");		// Double32_t[n] // Energy deposition at this specific interaction point [GeV]. "DIMA"
   TheLeafs->LandPoint_fDetectorID = TheLeafs->TheTree->FindLeaf("LandPoint.fDetectorID");	// Int_t[n]      // Identification of the detector. We already know however which detector we are talking about due to the name of the leaf! "DIMA"
   TheLeafs->LandPoint_fX = TheLeafs->TheTree->FindLeaf("LandPoint.fX");			// Double32_t[n] // x-coordinate [cm] at entrance of active volume. Active volume refers to a Geant4 active detector volume. "DIMA"
   TheLeafs->LandPoint_fY = TheLeafs->TheTree->FindLeaf("LandPoint.fY");			// Double32_t[n] // y-coordinate [cm] at entrance of active volume. Active volume refers to a Geant4 active detector volume. "DIMA"
   TheLeafs->LandPoint_fZ = TheLeafs->TheTree->FindLeaf("LandPoint.fZ");			// Double32_t[n] // z-coordinate [cm] at entrance of active volume. Active volume refers to a Geant4 active detector volume. "DIMA"
   TheLeafs->LandPoint_fX_out = TheLeafs->TheTree->FindLeaf("LandPoint.fX_out");		// Double32_t[n] // x-coordinate [cm] at exit of active volume. Active volume refers to a Geant4 active detector volume. "DIMA"
   TheLeafs->LandPoint_fY_out = TheLeafs->TheTree->FindLeaf("LandPoint.fY_out");		// Double32_t[n] // y-coordinate [cm] at exit of active volume. Active volume refers to a Geant4 active detector volume. "DIMA"
   TheLeafs->LandPoint_fZ_out = TheLeafs->TheTree->FindLeaf("LandPoint.fZ_out");		// Double32_t[n] // z-coordinate [cm] at exit of active volume. Active volume refers to a Geant4 active detector volume. "DIMA"
   TheLeafs->LandPoint_fPx_out = TheLeafs->TheTree->FindLeaf("LandPoint.fPx_out");		// Double32_t[n] // Momentum in x-direction [GeV] at the exit of the active detector volume. Active volume refers to a Geant4 active detector volume. "DIMA"
   TheLeafs->LandPoint_fPy_out = TheLeafs->TheTree->FindLeaf("LandPoint.fPy_out");		// Double32_t[n] // Momentum in y-direction [GeV] at the exit of the active detector volume. Active volume refers to a Geant4 active detector volume. "DIMA"
   TheLeafs->LandPoint_fPz_out = TheLeafs->TheTree->FindLeaf("LandPoint.fPz_out");		// Double32_t[n] // Momentum in z-direction [GeV] at the exit of the active detector volume. Active volume refers to a Geant4 active detector volume. "DIMA"
   TheLeafs->LandPoint_fSector = TheLeafs->TheTree->FindLeaf("LandPoint.fSector");		// Int_t[n] // Number of the specific bar of NeuLAND in which the hit is located. It ranges from 1 to 3000 for full NeuLAND. "DIMA"
   TheLeafs->LandPoint_fPaddleNb = TheLeafs->TheTree->FindLeaf("LandPoint.fPaddleNb");		// Int_t[n]
   TheLeafs->LandPoint_fPaddleTyp = TheLeafs->TheTree->FindLeaf("LandPoint.fPaddleTyp");	// Int_t[n]
   TheLeafs->LandPoint_fLightYield = TheLeafs->TheTree->FindLeaf("LandPoint.fLightYield");	// Double32_t[n] // If the energy deposit (dE/dx) is low, then the produced scintillator light is proportional to dE/dx: dN/dx = c*dE/dx where N stands for number of photons produced
												// (those photons carry a certain energy distribution). If dE/dx becomes higher however, the number of produced photons saturates. This saturation is descibed by Birk's
												// law: dL/dx = (S*dE/dx)(1 + B1*dE/dx). Here S and B1 are constants. dL/dx is now STILL energy deposition, but now with the saturation effect taken into account.
												// Then the produced scintillator light is given by dN/dx = c*dL/dx. This is now valid for all energy ranges. N is the number of produced photons. Hence the quantity
												// LandPoint_fLightYield->GetValue(k) holds the amount of deposited energy at the k-th interaction point in NeuLAND. Its unit is [GeV] and the saturation already is
												// taken into account: The number of produced photons at the k-th interaction point is given by N = c*LandPoint_fLightYield->GetValue(k) where c is a constant that
												// only depends on the scintillator material and the particle. It holds up to all energy ranges, because the saturation effect is taken into account. For fEloss 
												// this is not the case. Hence N = c*LandPoint_fLightYield->GetValue(k) gives the number of produced photons at the k-th interaction point. Each photon carries an energy
												// which is a sample of the same energy distribution, that depends also on LandPoint_fLightYield->GetValue(k) and on the material and particle of course.
												// hence to call a particle detected if a certain amount of light is PRODUCED, one could simply put an energy threshold on fLightYield per scintillator bar. Then 
												// the constant c is absorbed in this threshold. In order to put the light propagation in there however, one has to know c and generate a number of photons. One
												// also has to know the energy distribution of these photons to generate them and then let them propagate...
   
   }
   
   // Checking if the connection worked:
   if (TheLeafs->LandPoint_fUniqueID == 0) {LandPoint_Errors = LandPoint_Errors + 1;}
   if (TheLeafs->LandPoint_fBits == 0) {LandPoint_Errors = LandPoint_Errors + 1;}
   if (TheLeafs->LandPoint_fLink == 0) {LandPoint_Errors = LandPoint_Errors + 1;} 
   if (TheLeafs->LandPoint_fTrackID == 0) {LandPoint_Errors = LandPoint_Errors + 1;}
   if (TheLeafs->LandPoint_fEventId == 0) {LandPoint_Errors = LandPoint_Errors + 1;}
   if (TheLeafs->LandPoint_fPx == 0) {LandPoint_Errors = LandPoint_Errors + 1;}
   if (TheLeafs->LandPoint_fPy == 0) {LandPoint_Errors = LandPoint_Errors + 1;}
   if (TheLeafs->LandPoint_fPz == 0) {LandPoint_Errors = LandPoint_Errors + 1;}
   if (TheLeafs->LandPoint_fTime == 0) {LandPoint_Errors = LandPoint_Errors + 1;}
   if (TheLeafs->LandPoint_fLength == 0) {LandPoint_Errors = LandPoint_Errors + 1;}
   if (TheLeafs->LandPoint_fELoss == 0) {LandPoint_Errors = LandPoint_Errors + 1;}
   if (TheLeafs->LandPoint_fDetectorID == 0) {LandPoint_Errors = LandPoint_Errors + 1;}
   if (TheLeafs->LandPoint_fX == 0) {LandPoint_Errors = LandPoint_Errors + 1;}
   if (TheLeafs->LandPoint_fY == 0) {LandPoint_Errors = LandPoint_Errors + 1;}
   if (TheLeafs->LandPoint_fZ == 0) {LandPoint_Errors = LandPoint_Errors + 1;}
   if (Neu_Ex!=2)
   {
   if (TheLeafs->LandPoint_fX_out == 0) {LandPoint_Errors = LandPoint_Errors + 1;}
   if (TheLeafs->LandPoint_fY_out == 0) {LandPoint_Errors = LandPoint_Errors + 1;}
   if (TheLeafs->LandPoint_fZ_out == 0) {LandPoint_Errors = LandPoint_Errors + 1;}
   if (TheLeafs->LandPoint_fPx_out == 0) {LandPoint_Errors = LandPoint_Errors + 1;}
   if (TheLeafs->LandPoint_fPy_out == 0) {LandPoint_Errors = LandPoint_Errors + 1;}
   if (TheLeafs->LandPoint_fPz_out == 0) {LandPoint_Errors = LandPoint_Errors + 1;}
   if (TheLeafs->LandPoint_fPaddleNb == 0) {LandPoint_Errors = LandPoint_Errors + 1;}
   if (TheLeafs->LandPoint_fPaddleTyp == 0) {LandPoint_Errors = LandPoint_Errors + 1;}
   }
   if (TheLeafs->LandPoint_fSector == 0) {LandPoint_Errors = LandPoint_Errors + 1;}
   if (TheLeafs->LandPoint_fLightYield == 0) {LandPoint_Errors = LandPoint_Errors + 1;}
   
   // -----------------------------------
   // LFH-leafs:
   if (Neu_Ex==1)
   {
   TheLeafs->LandFirstHits_fUniqueID = TheLeafs->TheTree->FindLeaf("LandFirstHits.fUniqueID"); 		// UInt_t // This is an artifact of FairRoot. FairRoot is designed in such a way that each kind of leafs requires an fUniqueID and an fBits.
   TheLeafs->LandFirstHits_fBits = TheLeafs->TheTree->FindLeaf("LandFirstHits.fBits"); 			// UInt_t // This is an artifact of FairRoot. FairRoot is designed in such a way that each kind of leafs requires an fUniqueID and an fBits.
   TheLeafs->LandFirstHits_fx0 = TheLeafs->TheTree->FindLeaf("LandFirstHits.fx0"); 			// Double32_t //
   TheLeafs->LandFirstHits_fy0 = TheLeafs->TheTree->FindLeaf("LandFirstHits.fy0"); 			// Double32_t //
   TheLeafs->LandFirstHits_fz0 = TheLeafs->TheTree->FindLeaf("LandFirstHits.fz0"); 			// Double32_t //
   TheLeafs->LandFirstHits_fT0 = TheLeafs->TheTree->FindLeaf("LandFirstHits.fT0"); 			// Double32_t //
   TheLeafs->LandFirstHits_fx1 = TheLeafs->TheTree->FindLeaf("LandFirstHits.fx1"); 			// Double32_t //
   TheLeafs->LandFirstHits_fy1 = TheLeafs->TheTree->FindLeaf("LandFirstHits.fy1"); 			// Double32_t //
   TheLeafs->LandFirstHits_fz1 = TheLeafs->TheTree->FindLeaf("LandFirstHits.fz1"); 			// Double32_t //
   TheLeafs->LandFirstHits_fT1 = TheLeafs->TheTree->FindLeaf("LandFirstHits.fT1"); 			// Double32_t //
   TheLeafs->LandFirstHits_fx2 = TheLeafs->TheTree->FindLeaf("LandFirstHits.fx2"); 			// Double32_t //
   TheLeafs->LandFirstHits_fy2 = TheLeafs->TheTree->FindLeaf("LandFirstHits.fy2"); 			// Double32_t //
   TheLeafs->LandFirstHits_fz2 = TheLeafs->TheTree->FindLeaf("LandFirstHits.fz2"); 			// Double32_t //
   TheLeafs->LandFirstHits_fT2 = TheLeafs->TheTree->FindLeaf("LandFirstHits.fT2"); 			// Double32_t //
   TheLeafs->LandFirstHits_fx3 = TheLeafs->TheTree->FindLeaf("LandFirstHits.fx3"); 			// Double32_t //
   TheLeafs->LandFirstHits_fy3 = TheLeafs->TheTree->FindLeaf("LandFirstHits.fy3"); 			// Double32_t //
   TheLeafs->LandFirstHits_fz3 = TheLeafs->TheTree->FindLeaf("LandFirstHits.fz3"); 			// Double32_t //
   TheLeafs->LandFirstHits_fT3 = TheLeafs->TheTree->FindLeaf("LandFirstHits.fT3"); 			// Double32_t //
   TheLeafs->LandFirstHits_fx4 = TheLeafs->TheTree->FindLeaf("LandFirstHits.fx4"); 			// Double32_t //
   TheLeafs->LandFirstHits_fy4 = TheLeafs->TheTree->FindLeaf("LandFirstHits.fy4"); 			// Double32_t //
   TheLeafs->LandFirstHits_fz4 = TheLeafs->TheTree->FindLeaf("LandFirstHits.fz4"); 			// Double32_t //
   TheLeafs->LandFirstHits_fT4 = TheLeafs->TheTree->FindLeaf("LandFirstHits.fT4"); 			// Double32_t //
   TheLeafs->LandFirstHits_fx5 = TheLeafs->TheTree->FindLeaf("LandFirstHits.fx5"); 			// Double32_t //
   TheLeafs->LandFirstHits_fy5 = TheLeafs->TheTree->FindLeaf("LandFirstHits.fy5"); 			// Double32_t //
   TheLeafs->LandFirstHits_fz5 = TheLeafs->TheTree->FindLeaf("LandFirstHits.fz5"); 			// Double32_t //
   TheLeafs->LandFirstHits_fT5 = TheLeafs->TheTree->FindLeaf("LandFirstHits.fT5"); 			// Double32_t //
   
   // Checking if the connection worked:
   if (TheLeafs->LandFirstHits_fUniqueID == 0) {LFH_Errors = LFH_Errors + 1;}
   if (TheLeafs->LandFirstHits_fBits == 0) {LFH_Errors = LFH_Errors + 1;}
   if (TheLeafs->LandFirstHits_fx0 == 0) {LFH_Errors = LFH_Errors + 1;}
   if (TheLeafs->LandFirstHits_fy0 == 0) {LFH_Errors = LFH_Errors + 1;}
   if (TheLeafs->LandFirstHits_fz0 == 0) {LFH_Errors = LFH_Errors + 1;}
   if (TheLeafs->LandFirstHits_fT0 == 0) {LFH_Errors = LFH_Errors + 1;}
   if (TheLeafs->LandFirstHits_fx1 == 0) {LFH_Errors = LFH_Errors + 1;}
   if (TheLeafs->LandFirstHits_fy1 == 0) {LFH_Errors = LFH_Errors + 1;}
   if (TheLeafs->LandFirstHits_fz1 == 0) {LFH_Errors = LFH_Errors + 1;}
   if (TheLeafs->LandFirstHits_fT1 == 0) {LFH_Errors = LFH_Errors + 1;}
   if (TheLeafs->LandFirstHits_fx2 == 0) {LFH_Errors = LFH_Errors + 1;}
   if (TheLeafs->LandFirstHits_fy2 == 0) {LFH_Errors = LFH_Errors + 1;}
   if (TheLeafs->LandFirstHits_fz2 == 0) {LFH_Errors = LFH_Errors + 1;}
   if (TheLeafs->LandFirstHits_fT2 == 0) {LFH_Errors = LFH_Errors + 1;}
   if (TheLeafs->LandFirstHits_fx3 == 0) {LFH_Errors = LFH_Errors + 1;}
   if (TheLeafs->LandFirstHits_fy3 == 0) {LFH_Errors = LFH_Errors + 1;}
   if (TheLeafs->LandFirstHits_fz3 == 0) {LFH_Errors = LFH_Errors + 1;}
   if (TheLeafs->LandFirstHits_fT3 == 0) {LFH_Errors = LFH_Errors + 1;}
   if (TheLeafs->LandFirstHits_fx4 == 0) {LFH_Errors = LFH_Errors + 1;}
   if (TheLeafs->LandFirstHits_fy4 == 0) {LFH_Errors = LFH_Errors + 1;}
   if (TheLeafs->LandFirstHits_fz4 == 0) {LFH_Errors = LFH_Errors + 1;}
   if (TheLeafs->LandFirstHits_fT4 == 0) {LFH_Errors = LFH_Errors + 1;}
   if (TheLeafs->LandFirstHits_fx5 == 0) {LFH_Errors = LFH_Errors + 1;}
   if (TheLeafs->LandFirstHits_fy5 == 0) {LFH_Errors = LFH_Errors + 1;}
   if (TheLeafs->LandFirstHits_fz5 == 0) {LFH_Errors = LFH_Errors + 1;}
   if (TheLeafs->LandFirstHits_fT5 == 0) {LFH_Errors = LFH_Errors + 1;}
   }
   
   if (Neu_Ex==2)
   {
   TheLeafs->NeuPrimIntPoint_fUniqueID = TheLeafs->TheTree->FindLeaf("NeulandPrimaryNeutronInteractionPoints.fUniqueID");	// UInt_t[n] // This is an artifact of FairRoot. FairRoot is designed in such a way that each kind of leafs requires an fUniqueID and an fBits.
   TheLeafs->NeuPrimIntPoint_fBits = TheLeafs->TheTree->FindLeaf("NeulandPrimaryNeutronInteractionPoints.fBits");		// UInt_t[n] // This is an artifact of FairRoot. FairRoot is designed in such a way that each kind of leafs requires an fUniqueID and an fBits.
   TheLeafs->NeuPrimIntPoint_fLink = TheLeafs->TheTree->FindLeaf("NeulandPrimaryNeutronInteractionPoints.fLink");		// FairMultiLinkedData*[n]
   TheLeafs->NeuPrimIntPoint_fTrackID = TheLeafs->TheTree->FindLeaf("NeulandPrimaryNeutronInteractionPoints.fTrackID");		// Int_t[n] // C++ index of where this track is located in the MC leafs. "DIMA"
   TheLeafs->NeuPrimIntPoint_fEventId = TheLeafs->TheTree->FindLeaf("NeulandPrimaryNeutronInteractionPoints.fEventId");		// UInt_t[n]
   TheLeafs->NeuPrimIntPoint_fPx = TheLeafs->TheTree->FindLeaf("NeulandPrimaryNeutronInteractionPoints.fPx");			// Double32_t[n] // Momentum in x-direction [GeV] at the entrance of the active detector volume. Active volume refers to a Geant4 active detector volume. "DIMA"
   TheLeafs->NeuPrimIntPoint_fPy = TheLeafs->TheTree->FindLeaf("NeulandPrimaryNeutronInteractionPoints.fPy");			// Double32_t[n] // Momentum in y-direction [GeV] at the entrance of the active detector volume. Active volume refers to a Geant4 active detector volume. "DIMA"
   TheLeafs->NeuPrimIntPoint_fPz = TheLeafs->TheTree->FindLeaf("NeulandPrimaryNeutronInteractionPoints.fPz");			// Double32_t[n] // Momentum in z-direction [GeV] at the entrance of the active detector volume. Active volume refers to a Geant4 active detector volume. "DIMA"
   TheLeafs->NeuPrimIntPoint_fTime = TheLeafs->TheTree->FindLeaf("NeulandPrimaryNeutronInteractionPoints.fTime");		// Double32_t[n] // Time Of Flight between the beginning of this event and this specific interaction point [ns]. "DIMA"
   TheLeafs->NeuPrimIntPoint_fLength = TheLeafs->TheTree->FindLeaf("NeulandPrimaryNeutronInteractionPoints.fLength");		// Double32_t[n] // Total Track length since creation (averaged over detector) [cm]. "DIMA"
   TheLeafs->NeuPrimIntPoint_fELoss = TheLeafs->TheTree->FindLeaf("NeulandPrimaryNeutronInteractionPoints.fELoss");		// Double32_t[n] // Energy deposition at this specific interaction point [GeV]. "DIMA"
   TheLeafs->NeuPrimIntPoint_fDetectorID = TheLeafs->TheTree->FindLeaf("NeulandPrimaryNeutronInteractionPoints.fDetectorID");	// Int_t[n]      // Identification of the detector. We already know however which detector we are talking about due to the name of the leaf! "DIMA"
   TheLeafs->NeuPrimIntPoint_fX = TheLeafs->TheTree->FindLeaf("NeulandPrimaryNeutronInteractionPoints.fX");			// Double32_t[n] // x-coordinate [cm] at entrance of active volume. Active volume refers to a Geant4 active detector volume. "DIMA"
   TheLeafs->NeuPrimIntPoint_fY = TheLeafs->TheTree->FindLeaf("NeulandPrimaryNeutronInteractionPoints.fY");			// Double32_t[n] // y-coordinate [cm] at entrance of active volume. Active volume refers to a Geant4 active detector volume. "DIMA"
   TheLeafs->NeuPrimIntPoint_fZ = TheLeafs->TheTree->FindLeaf("NeulandPrimaryNeutronInteractionPoints.fZ");			// Double32_t[n] // z-coordinate [cm] at entrance of active volume. Active volume refers to a Geant4 active detector volume. "DIMA"
   
   // Checking if the connection worked:
   if (TheLeafs->NeuPrimIntPoint_fUniqueID == 0) {NeuPrimIntPoint_Errors = NeuPrimIntPoint_Errors + 1;}
   if (TheLeafs->NeuPrimIntPoint_fBits == 0) {NeuPrimIntPoint_Errors = NeuPrimIntPoint_Errors + 1;}
   if (TheLeafs->NeuPrimIntPoint_fLink == 0) {NeuPrimIntPoint_Errors = NeuPrimIntPoint_Errors + 1;} 
   if (TheLeafs->NeuPrimIntPoint_fTrackID == 0) {NeuPrimIntPoint_Errors = NeuPrimIntPoint_Errors + 1;}
   if (TheLeafs->NeuPrimIntPoint_fEventId == 0) {NeuPrimIntPoint_Errors = NeuPrimIntPoint_Errors + 1;}
   if (TheLeafs->NeuPrimIntPoint_fPx == 0) {NeuPrimIntPoint_Errors = NeuPrimIntPoint_Errors + 1;}
   if (TheLeafs->NeuPrimIntPoint_fPy == 0) {NeuPrimIntPoint_Errors = NeuPrimIntPoint_Errors + 1;}
   if (TheLeafs->NeuPrimIntPoint_fPz == 0) {NeuPrimIntPoint_Errors = NeuPrimIntPoint_Errors + 1;}
   if (TheLeafs->NeuPrimIntPoint_fTime == 0) {NeuPrimIntPoint_Errors = NeuPrimIntPoint_Errors + 1;}
   if (TheLeafs->NeuPrimIntPoint_fLength == 0) {NeuPrimIntPoint_Errors = NeuPrimIntPoint_Errors + 1;}
   if (TheLeafs->NeuPrimIntPoint_fELoss == 0) {NeuPrimIntPoint_Errors = NeuPrimIntPoint_Errors + 1;}
   if (TheLeafs->NeuPrimIntPoint_fDetectorID == 0) {NeuPrimIntPoint_Errors = NeuPrimIntPoint_Errors + 1;}
   if (TheLeafs->NeuPrimIntPoint_fX == 0) {NeuPrimIntPoint_Errors = NeuPrimIntPoint_Errors + 1;}
   if (TheLeafs->NeuPrimIntPoint_fY == 0) {NeuPrimIntPoint_Errors = NeuPrimIntPoint_Errors + 1;}
   if (TheLeafs->NeuPrimIntPoint_fZ == 0) {NeuPrimIntPoint_Errors = NeuPrimIntPoint_Errors + 1;}
   }
   
   // -----------------------------------
   // VETO Point leafs:
   
   if (VETO_Ex==2)
   {
   TheLeafs->veto_segPoint_fUniqueID = TheLeafs->TheTree->FindLeaf("Nveto_segPoints.fUniqueID");	// UInt_t[n] // This is an artifact of FairRoot. FairRoot is designed in such a way that each kind of leafs requires an fUniqueID and an fBits.
   TheLeafs->veto_segPoint_fBits = TheLeafs->TheTree->FindLeaf("Nveto_segPoints.fBits");		// UInt_t[n] // This is an artifact of FairRoot. FairRoot is designed in such a way that each kind of leafs requires an fUniqueID and an fBits.
   TheLeafs->veto_segPoint_fLink = TheLeafs->TheTree->FindLeaf("Nveto_segPoints.fLink");		// FairMultiLinkedData*[n]
   TheLeafs->veto_segPoint_fTrackID = TheLeafs->TheTree->FindLeaf("Nveto_segPoints.fTrackID");		// Int_t[n] // C++ index of where this track is located in the MC leafs. "DIMA"
   TheLeafs->veto_segPoint_fEventId = TheLeafs->TheTree->FindLeaf("Nveto_segPoints.fEventId");		// UInt_t[n]
   TheLeafs->veto_segPoint_fPx = TheLeafs->TheTree->FindLeaf("Nveto_segPoints.fPx");			// Double32_t[n] // Momentum in x-direction [GeV] at the entrance of the active detector volume. Active volume refers to a Geant4 active detector volume. "DIMA"
   TheLeafs->veto_segPoint_fPy = TheLeafs->TheTree->FindLeaf("Nveto_segPoints.fPy");			// Double32_t[n] // Momentum in y-direction [GeV] at the entrance of the active detector volume. Active volume refers to a Geant4 active detector volume. "DIMA"
   TheLeafs->veto_segPoint_fPz = TheLeafs->TheTree->FindLeaf("Nveto_segPoints.fPz");			// Double32_t[n] // Momentum in z-direction [GeV] at the entrance of the active detector volume. Active volume refers to a Geant4 active detector volume. "DIMA"
   TheLeafs->veto_segPoint_fTime = TheLeafs->TheTree->FindLeaf("Nveto_segPoints.fTime");		// Double32_t[n] // Time Of Flight between the beginning of this event and this specific interaction point [ns]. "DIMA"
   TheLeafs->veto_segPoint_fLength = TheLeafs->TheTree->FindLeaf("Nveto_segPoints.fLength");		// Double32_t[n] // Total Track length since creation (averaged over detector) [cm]. "DIMA"
   TheLeafs->veto_segPoint_fELoss = TheLeafs->TheTree->FindLeaf("Nveto_segPoints.fELoss");		// Double32_t[n] // Energy deposition at this specific interaction point [GeV]. "DIMA"
   TheLeafs->veto_segPoint_fDetectorID = TheLeafs->TheTree->FindLeaf("Nveto_segPoints.fDetectorID");	// Int_t[n]      // Identification of the detector. We already know however which detector we are talking about due to the name of the leaf! "DIMA"
   TheLeafs->veto_segPoint_fX = TheLeafs->TheTree->FindLeaf("Nveto_segPoints.fX");			// Double32_t[n] // x-coordinate [cm] at entrance of active volume. Active volume refers to a Geant4 active detector volume. "DIMA"
   TheLeafs->veto_segPoint_fY = TheLeafs->TheTree->FindLeaf("Nveto_segPoints.fY");			// Double32_t[n] // y-coordinate [cm] at entrance of active volume. Active volume refers to a Geant4 active detector volume. "DIMA"
   TheLeafs->veto_segPoint_fZ = TheLeafs->TheTree->FindLeaf("Nveto_segPoints.fZ");			// Double32_t[n] // z-coordinate [cm] at entrance of active volume. Active volume refers to a Geant4 active detector volume. "DIMA"
   TheLeafs->veto_segPoint_fX_out = TheLeafs->TheTree->FindLeaf("Nveto_segPoints.fX_out");		// Double32_t[n] // x-coordinate [cm] at exit of active volume. Active volume refers to a Geant4 active detector volume. "DIMA"
   TheLeafs->veto_segPoint_fY_out = TheLeafs->TheTree->FindLeaf("Nveto_segPoints.fY_out");		// Double32_t[n] // y-coordinate [cm] at exit of active volume. Active volume refers to a Geant4 active detector volume. "DIMA"
   TheLeafs->veto_segPoint_fZ_out = TheLeafs->TheTree->FindLeaf("Nveto_segPoints.fZ_out");		// Double32_t[n] // z-coordinate [cm] at exit of active volume. Active volume refers to a Geant4 active detector volume. "DIMA"
   TheLeafs->veto_segPoint_fPx_out = TheLeafs->TheTree->FindLeaf("Nveto_segPoints.fPx_out");		// Double32_t[n] // Momentum in x-direction [GeV] at the exit of the active detector volume. Active volume refers to a Geant4 active detector volume. "DIMA"
   TheLeafs->veto_segPoint_fPy_out = TheLeafs->TheTree->FindLeaf("Nveto_segPoints.fPy_out");		// Double32_t[n] // Momentum in y-direction [GeV] at the exit of the active detector volume. Active volume refers to a Geant4 active detector volume. "DIMA"
   TheLeafs->veto_segPoint_fPz_out = TheLeafs->TheTree->FindLeaf("Nveto_segPoints.fPz_out");		// Double32_t[n] // Momentum in z-direction [GeV] at the exit of the active detector volume. Active volume refers to a Geant4 active detector volume. "DIMA"
   TheLeafs->veto_segPoint_fSector = TheLeafs->TheTree->FindLeaf("Nveto_segPoints.fDetectorID");		// Int_t[n] // Number of the specific bar of NeuLAND in which the hit is located. It ranges from 1 to 3000 for full NeuLAND. "DIMA"
   TheLeafs->veto_segPoint_fPaddleNb = TheLeafs->TheTree->FindLeaf("Nveto_segPoints.fPaddleNb");	// Int_t[n]
   TheLeafs->veto_segPoint_fPaddleTyp = TheLeafs->TheTree->FindLeaf("Nveto_segPoints.fPaddleTyp");	// Int_t[n]
   TheLeafs->veto_segPoint_fLightYield = TheLeafs->TheTree->FindLeaf("Nveto_segPoints.fLightYield");	// Double32_t[n] // If the energy deposit (dE/dx) is low, then the produced scintillator light is proportional to dE/dx: dN/dx = c*dE/dx where N stands for number of photons produced
													// (those photons carry a certain energy distribution). If dE/dx becomes higher however, the number of produced photons saturates. This saturation is descibed by Birk's
													// law: dL/dx = (S*dE/dx)(1 + B1*dE/dx). Here S and B1 are constants. dL/dx is now STILL energy deposition, but now with the saturation effect taken into account.
													// Then the produced scintillator light is given by dN/dx = c*dL/dx. This is now valid for all energy ranges. N is the number of produced photons. Hence the quantity
													// LandPoint_fLightYield->GetValue(k) holds the amount of deposited energy at the k-th interaction point in NeuLAND. Its unit is [GeV] and the saturation already is
													// taken into account: The number of produced photons at the k-th interaction point is given by N = c*LandPoint_fLightYield->GetValue(k) where c is a constant that
													// only depends on the scintillator material and the particle. It holds up to all energy ranges, because the saturation effect is taken into account. For fEloss 
													// this is not the case. Hence N = c*LandPoint_fLightYield->GetValue(k) gives the number of produced photons at the k-th interaction point. Each photon carries an energy
													// which is a sample of the same energy distribution, that depends also on LandPoint_fLightYield->GetValue(k) and on the material and particle of course.
													// hence to call a particle detected if a certain amount of light is PRODUCED, one could simply put an energy threshold on fLightYield per scintillator bar. Then 
													// the constant c is absorbed in this threshold. In order to put the light propagation in there however, one has to know c and generate a number of photons. One
													// also has to know the energy distribution of these photons to generate them and then let them propagate...
   }
   else
   {
   TheLeafs->veto_segPoint_fUniqueID = TheLeafs->TheTree->FindLeaf("veto_segPoint.fUniqueID");		// UInt_t[n] // This is an artifact of FairRoot. FairRoot is designed in such a way that each kind of leafs requires an fUniqueID and an fBits.
   TheLeafs->veto_segPoint_fBits = TheLeafs->TheTree->FindLeaf("veto_segPoint.fBits");			// UInt_t[n] // This is an artifact of FairRoot. FairRoot is designed in such a way that each kind of leafs requires an fUniqueID and an fBits.
   TheLeafs->veto_segPoint_fLink = TheLeafs->TheTree->FindLeaf("veto_segPoint.fLink");			// FairMultiLinkedData*[n]
   TheLeafs->veto_segPoint_fTrackID = TheLeafs->TheTree->FindLeaf("veto_segPoint.fTrackID");		// Int_t[n] // C++ index of where this track is located in the MC leafs. "DIMA"
   TheLeafs->veto_segPoint_fEventId = TheLeafs->TheTree->FindLeaf("veto_segPoint.fEventId");		// UInt_t[n]
   TheLeafs->veto_segPoint_fPx = TheLeafs->TheTree->FindLeaf("veto_segPoint.fPx");			// Double32_t[n] // Momentum in x-direction [GeV] at the entrance of the active detector volume. Active volume refers to a Geant4 active detector volume. "DIMA"
   TheLeafs->veto_segPoint_fPy = TheLeafs->TheTree->FindLeaf("veto_segPoint.fPy");			// Double32_t[n] // Momentum in y-direction [GeV] at the entrance of the active detector volume. Active volume refers to a Geant4 active detector volume. "DIMA"
   TheLeafs->veto_segPoint_fPz = TheLeafs->TheTree->FindLeaf("veto_segPoint.fPz");			// Double32_t[n] // Momentum in z-direction [GeV] at the entrance of the active detector volume. Active volume refers to a Geant4 active detector volume. "DIMA"
   TheLeafs->veto_segPoint_fTime = TheLeafs->TheTree->FindLeaf("veto_segPoint.fTime");			// Double32_t[n] // Time Of Flight between the beginning of this event and this specific interaction point [ns]. "DIMA"
   TheLeafs->veto_segPoint_fLength = TheLeafs->TheTree->FindLeaf("veto_segPoint.fLength");		// Double32_t[n] // Total Track length since creation (averaged over detector) [cm]. "DIMA"
   TheLeafs->veto_segPoint_fELoss = TheLeafs->TheTree->FindLeaf("veto_segPoint.fELoss");		// Double32_t[n] // Energy deposition at this specific interaction point [GeV]. "DIMA"
   TheLeafs->veto_segPoint_fDetectorID = TheLeafs->TheTree->FindLeaf("veto_segPoint.fDetectorID");	// Int_t[n]      // Identification of the detector. We already know however which detector we are talking about due to the name of the leaf! "DIMA"
   TheLeafs->veto_segPoint_fX = TheLeafs->TheTree->FindLeaf("veto_segPoint.fX");			// Double32_t[n] // x-coordinate [cm] at entrance of active volume. Active volume refers to a Geant4 active detector volume. "DIMA"
   TheLeafs->veto_segPoint_fY = TheLeafs->TheTree->FindLeaf("veto_segPoint.fY");			// Double32_t[n] // y-coordinate [cm] at entrance of active volume. Active volume refers to a Geant4 active detector volume. "DIMA"
   TheLeafs->veto_segPoint_fZ = TheLeafs->TheTree->FindLeaf("veto_segPoint.fZ");			// Double32_t[n] // z-coordinate [cm] at entrance of active volume. Active volume refers to a Geant4 active detector volume. "DIMA"
   TheLeafs->veto_segPoint_fX_out = TheLeafs->TheTree->FindLeaf("veto_segPoint.fX_out");		// Double32_t[n] // x-coordinate [cm] at exit of active volume. Active volume refers to a Geant4 active detector volume. "DIMA"
   TheLeafs->veto_segPoint_fY_out = TheLeafs->TheTree->FindLeaf("veto_segPoint.fY_out");		// Double32_t[n] // y-coordinate [cm] at exit of active volume. Active volume refers to a Geant4 active detector volume. "DIMA"
   TheLeafs->veto_segPoint_fZ_out = TheLeafs->TheTree->FindLeaf("veto_segPoint.fZ_out");		// Double32_t[n] // z-coordinate [cm] at exit of active volume. Active volume refers to a Geant4 active detector volume. "DIMA"
   TheLeafs->veto_segPoint_fPx_out = TheLeafs->TheTree->FindLeaf("veto_segPoint.fPx_out");		// Double32_t[n] // Momentum in x-direction [GeV] at the exit of the active detector volume. Active volume refers to a Geant4 active detector volume. "DIMA"
   TheLeafs->veto_segPoint_fPy_out = TheLeafs->TheTree->FindLeaf("veto_segPoint.fPy_out");		// Double32_t[n] // Momentum in y-direction [GeV] at the exit of the active detector volume. Active volume refers to a Geant4 active detector volume. "DIMA"
   TheLeafs->veto_segPoint_fPz_out = TheLeafs->TheTree->FindLeaf("veto_segPoint.fPz_out");		// Double32_t[n] // Momentum in z-direction [GeV] at the exit of the active detector volume. Active volume refers to a Geant4 active detector volume. "DIMA"
   TheLeafs->veto_segPoint_fSector = TheLeafs->TheTree->FindLeaf("veto_segPoint.fSector");		// Int_t[n] // Number of the specific bar of NeuLAND in which the hit is located. It ranges from 1 to 3000 for full NeuLAND. "DIMA"
   TheLeafs->veto_segPoint_fPaddleNb = TheLeafs->TheTree->FindLeaf("veto_segPoint.fPaddleNb");		// Int_t[n]
   TheLeafs->veto_segPoint_fPaddleTyp = TheLeafs->TheTree->FindLeaf("veto_segPoint.fPaddleTyp");	// Int_t[n]
   TheLeafs->veto_segPoint_fLightYield = TheLeafs->TheTree->FindLeaf("veto_segPoint.fLightYield");	// Double32_t[n] // If the energy deposit (dE/dx) is low, then the produced scintillator light is proportional to dE/dx: dN/dx = c*dE/dx where N stands for number of photons produced
													// (those photons carry a certain energy distribution). If dE/dx becomes higher however, the number of produced photons saturates. This saturation is descibed by Birk's
													// law: dL/dx = (S*dE/dx)(1 + B1*dE/dx). Here S and B1 are constants. dL/dx is now STILL energy deposition, but now with the saturation effect taken into account.
													// Then the produced scintillator light is given by dN/dx = c*dL/dx. This is now valid for all energy ranges. N is the number of produced photons. Hence the quantity
													// LandPoint_fLightYield->GetValue(k) holds the amount of deposited energy at the k-th interaction point in NeuLAND. Its unit is [GeV] and the saturation already is
													// taken into account: The number of produced photons at the k-th interaction point is given by N = c*LandPoint_fLightYield->GetValue(k) where c is a constant that
													// only depends on the scintillator material and the particle. It holds up to all energy ranges, because the saturation effect is taken into account. For fEloss 
													// this is not the case. Hence N = c*LandPoint_fLightYield->GetValue(k) gives the number of produced photons at the k-th interaction point. Each photon carries an energy
													// which is a sample of the same energy distribution, that depends also on LandPoint_fLightYield->GetValue(k) and on the material and particle of course.
													// hence to call a particle detected if a certain amount of light is PRODUCED, one could simply put an energy threshold on fLightYield per scintillator bar. Then 
													// the constant c is absorbed in this threshold. In order to put the light propagation in there however, one has to know c and generate a number of photons. One
													// also has to know the energy distribution of these photons to generate them and then let them propagate...
   }
   
   // Checking if the connection worked:
   if (TheLeafs->veto_segPoint_fUniqueID == 0) {VetoPoint_Errors = VetoPoint_Errors + 1;}
   if (TheLeafs->veto_segPoint_fBits == 0) {VetoPoint_Errors = VetoPoint_Errors + 1;}
   if (TheLeafs->veto_segPoint_fLink == 0) {VetoPoint_Errors = VetoPoint_Errors + 1;} 
   if (TheLeafs->veto_segPoint_fTrackID == 0) {VetoPoint_Errors = VetoPoint_Errors + 1;}
   if (TheLeafs->veto_segPoint_fEventId == 0) {VetoPoint_Errors = VetoPoint_Errors + 1;}
   if (TheLeafs->veto_segPoint_fPx == 0) {VetoPoint_Errors = VetoPoint_Errors + 1;}
   if (TheLeafs->veto_segPoint_fPy == 0) {VetoPoint_Errors = VetoPoint_Errors + 1;}
   if (TheLeafs->veto_segPoint_fPz == 0) {VetoPoint_Errors = VetoPoint_Errors + 1;}
   if (TheLeafs->veto_segPoint_fTime == 0) {VetoPoint_Errors = VetoPoint_Errors + 1;}
   if (TheLeafs->veto_segPoint_fLength == 0) {VetoPoint_Errors = VetoPoint_Errors + 1;}
   if (TheLeafs->veto_segPoint_fELoss == 0) {VetoPoint_Errors = VetoPoint_Errors + 1;}
   if (TheLeafs->veto_segPoint_fDetectorID == 0) {VetoPoint_Errors = VetoPoint_Errors + 1;}
   if (TheLeafs->veto_segPoint_fX == 0) {VetoPoint_Errors = VetoPoint_Errors + 1;}
   if (TheLeafs->veto_segPoint_fY == 0) {VetoPoint_Errors = VetoPoint_Errors + 1;}
   if (TheLeafs->veto_segPoint_fZ == 0) {VetoPoint_Errors = VetoPoint_Errors + 1;}
   if (VETO_Ex!=2)
   {
   if (TheLeafs->veto_segPoint_fX_out == 0) {VetoPoint_Errors = VetoPoint_Errors + 1;}
   if (TheLeafs->veto_segPoint_fY_out == 0) {VetoPoint_Errors = VetoPoint_Errors + 1;}
   if (TheLeafs->veto_segPoint_fZ_out == 0) {VetoPoint_Errors = VetoPoint_Errors + 1;}
   if (TheLeafs->veto_segPoint_fPx_out == 0) {VetoPoint_Errors = VetoPoint_Errors + 1;}
   if (TheLeafs->veto_segPoint_fPy_out == 0) {VetoPoint_Errors = VetoPoint_Errors + 1;}
   if (TheLeafs->veto_segPoint_fPz_out == 0) {VetoPoint_Errors = VetoPoint_Errors + 1;}
   if (TheLeafs->veto_segPoint_fPaddleNb == 0) {VetoPoint_Errors = VetoPoint_Errors + 1;}
   if (TheLeafs->veto_segPoint_fPaddleTyp == 0) {VetoPoint_Errors = VetoPoint_Errors + 1;}
   }
   if (TheLeafs->veto_segPoint_fSector == 0) {VetoPoint_Errors = VetoPoint_Errors + 1;}
   if (TheLeafs->veto_segPoint_fLightYield == 0) {VetoPoint_Errors = VetoPoint_Errors + 1;}
   
   // -----------------------------------
   // VFH-leafs:
   if (VETO_Ex==1)
   {
   TheLeafs->veto_segFirstHits_fUniqueID = TheLeafs->TheTree->FindLeaf("veto_segFirstHits.fUniqueID"); 		// UInt_t // This is an artifact of FairRoot. FairRoot is designed in such a way that each kind of leafs requires an fUniqueID and an fBits.
   TheLeafs->veto_segFirstHits_fBits = TheLeafs->TheTree->FindLeaf("veto_segFirstHits.fBits"); 			// UInt_t // This is an artifact of FairRoot. FairRoot is designed in such a way that each kind of leafs requires an fUniqueID and an fBits.
   TheLeafs->veto_segFirstHits_fx0 = TheLeafs->TheTree->FindLeaf("veto_segFirstHits.fx0"); 			// Double32_t //
   TheLeafs->veto_segFirstHits_fy0 = TheLeafs->TheTree->FindLeaf("veto_segFirstHits.fy0"); 			// Double32_t //
   TheLeafs->veto_segFirstHits_fz0 = TheLeafs->TheTree->FindLeaf("veto_segFirstHits.fz0"); 			// Double32_t //
   TheLeafs->veto_segFirstHits_fT0 = TheLeafs->TheTree->FindLeaf("veto_segFirstHits.fT0"); 			// Double32_t //
   TheLeafs->veto_segFirstHits_fx1 = TheLeafs->TheTree->FindLeaf("veto_segFirstHits.fx1"); 			// Double32_t //
   TheLeafs->veto_segFirstHits_fy1 = TheLeafs->TheTree->FindLeaf("veto_segFirstHits.fy1"); 			// Double32_t //
   TheLeafs->veto_segFirstHits_fz1 = TheLeafs->TheTree->FindLeaf("veto_segFirstHits.fz1"); 			// Double32_t //
   TheLeafs->veto_segFirstHits_fT1 = TheLeafs->TheTree->FindLeaf("veto_segFirstHits.fT1"); 			// Double32_t //
   TheLeafs->veto_segFirstHits_fx2 = TheLeafs->TheTree->FindLeaf("veto_segFirstHits.fx2"); 			// Double32_t //
   TheLeafs->veto_segFirstHits_fy2 = TheLeafs->TheTree->FindLeaf("veto_segFirstHits.fy2"); 			// Double32_t //
   TheLeafs->veto_segFirstHits_fz2 = TheLeafs->TheTree->FindLeaf("veto_segFirstHits.fz2"); 			// Double32_t //
   TheLeafs->veto_segFirstHits_fT2 = TheLeafs->TheTree->FindLeaf("veto_segFirstHits.fT2"); 			// Double32_t //
   TheLeafs->veto_segFirstHits_fx3 = TheLeafs->TheTree->FindLeaf("veto_segFirstHits.fx3"); 			// Double32_t //
   TheLeafs->veto_segFirstHits_fy3 = TheLeafs->TheTree->FindLeaf("veto_segFirstHits.fy3"); 			// Double32_t //
   TheLeafs->veto_segFirstHits_fz3 = TheLeafs->TheTree->FindLeaf("veto_segFirstHits.fz3"); 			// Double32_t //
   TheLeafs->veto_segFirstHits_fT3 = TheLeafs->TheTree->FindLeaf("veto_segFirstHits.fT3"); 			// Double32_t //
   TheLeafs->veto_segFirstHits_fx4 = TheLeafs->TheTree->FindLeaf("veto_segFirstHits.fx4"); 			// Double32_t //
   TheLeafs->veto_segFirstHits_fy4 = TheLeafs->TheTree->FindLeaf("veto_segFirstHits.fy4"); 			// Double32_t //
   TheLeafs->veto_segFirstHits_fz4 = TheLeafs->TheTree->FindLeaf("veto_segFirstHits.fz4"); 			// Double32_t //
   TheLeafs->veto_segFirstHits_fT4 = TheLeafs->TheTree->FindLeaf("veto_segFirstHits.fT4"); 			// Double32_t //
   TheLeafs->veto_segFirstHits_fx5 = TheLeafs->TheTree->FindLeaf("veto_segFirstHits.fx5"); 			// Double32_t //
   TheLeafs->veto_segFirstHits_fy5 = TheLeafs->TheTree->FindLeaf("veto_segFirstHits.fy5"); 			// Double32_t //
   TheLeafs->veto_segFirstHits_fz5 = TheLeafs->TheTree->FindLeaf("veto_segFirstHits.fz5"); 			// Double32_t //
   TheLeafs->veto_segFirstHits_fT5 = TheLeafs->TheTree->FindLeaf("veto_segFirstHits.fT5"); 			// Double32_t //
   
   // Checking if the connection worked:
   if (TheLeafs->veto_segFirstHits_fUniqueID == 0) {VFH_Errors = VFH_Errors + 1;}
   if (TheLeafs->veto_segFirstHits_fBits == 0) {VFH_Errors = VFH_Errors + 1;}
   if (TheLeafs->veto_segFirstHits_fx0 == 0) {VFH_Errors = VFH_Errors + 1;}
   if (TheLeafs->veto_segFirstHits_fy0 == 0) {VFH_Errors = VFH_Errors + 1;}
   if (TheLeafs->veto_segFirstHits_fz0 == 0) {VFH_Errors = VFH_Errors + 1;}
   if (TheLeafs->veto_segFirstHits_fT0 == 0) {VFH_Errors = VFH_Errors + 1;}
   if (TheLeafs->veto_segFirstHits_fx1 == 0) {VFH_Errors = VFH_Errors + 1;}
   if (TheLeafs->veto_segFirstHits_fy1 == 0) {VFH_Errors = VFH_Errors + 1;}
   if (TheLeafs->veto_segFirstHits_fz1 == 0) {VFH_Errors = VFH_Errors + 1;}
   if (TheLeafs->veto_segFirstHits_fT1 == 0) {VFH_Errors = VFH_Errors + 1;}
   if (TheLeafs->veto_segFirstHits_fx2 == 0) {VFH_Errors = VFH_Errors + 1;}
   if (TheLeafs->veto_segFirstHits_fy2 == 0) {VFH_Errors = VFH_Errors + 1;}
   if (TheLeafs->veto_segFirstHits_fz2 == 0) {VFH_Errors = VFH_Errors + 1;}
   if (TheLeafs->veto_segFirstHits_fT2 == 0) {VFH_Errors = VFH_Errors + 1;}
   if (TheLeafs->veto_segFirstHits_fx3 == 0) {VFH_Errors = VFH_Errors + 1;}
   if (TheLeafs->veto_segFirstHits_fy3 == 0) {VFH_Errors = VFH_Errors + 1;}
   if (TheLeafs->veto_segFirstHits_fz3 == 0) {VFH_Errors = VFH_Errors + 1;}
   if (TheLeafs->veto_segFirstHits_fT3 == 0) {VFH_Errors = VFH_Errors + 1;}
   if (TheLeafs->veto_segFirstHits_fx4 == 0) {VFH_Errors = VFH_Errors + 1;}
   if (TheLeafs->veto_segFirstHits_fy4 == 0) {VFH_Errors = VFH_Errors + 1;}
   if (TheLeafs->veto_segFirstHits_fz4 == 0) {VFH_Errors = VFH_Errors + 1;}
   if (TheLeafs->veto_segFirstHits_fT4 == 0) {VFH_Errors = VFH_Errors + 1;}
   if (TheLeafs->veto_segFirstHits_fx5 == 0) {VFH_Errors = VFH_Errors + 1;}
   if (TheLeafs->veto_segFirstHits_fy5 == 0) {VFH_Errors = VFH_Errors + 1;}
   if (TheLeafs->veto_segFirstHits_fz5 == 0) {VFH_Errors = VFH_Errors + 1;}
   if (TheLeafs->veto_segFirstHits_fT5 == 0) {VFH_Errors = VFH_Errors + 1;}
   }
   
   if (VETO_Ex==2)
   {
   TheLeafs->VetoPrimIntPoint_fUniqueID = TheLeafs->TheTree->FindLeaf("Nveto_segPrimaryNeutronInteractionPoints.fUniqueID");	// UInt_t[n] // This is an artifact of FairRoot. FairRoot is designed in such a way that each kind of leafs requires an fUniqueID and an fBits.
   TheLeafs->VetoPrimIntPoint_fBits = TheLeafs->TheTree->FindLeaf("Nveto_segPrimaryNeutronInteractionPoints.fBits");		// UInt_t[n] // This is an artifact of FairRoot. FairRoot is designed in such a way that each kind of leafs requires an fUniqueID and an fBits.
   TheLeafs->VetoPrimIntPoint_fLink = TheLeafs->TheTree->FindLeaf("Nveto_segPrimaryNeutronInteractionPoints.fLink");		// FairMultiLinkedData*[n]
   TheLeafs->VetoPrimIntPoint_fTrackID = TheLeafs->TheTree->FindLeaf("Nveto_segPrimaryNeutronInteractionPoints.fTrackID");	// Int_t[n] // C++ index of where this track is located in the MC leafs. "DIMA"
   TheLeafs->VetoPrimIntPoint_fEventId = TheLeafs->TheTree->FindLeaf("Nveto_segPrimaryNeutronInteractionPoints.fEventId");	// UInt_t[n]
   TheLeafs->VetoPrimIntPoint_fPx = TheLeafs->TheTree->FindLeaf("Nveto_segPrimaryNeutronInteractionPoints.fPx");		// Double32_t[n] // Momentum in x-direction [GeV] at the entrance of the active detector volume. Active volume refers to a Geant4 active detector volume. "DIMA"
   TheLeafs->VetoPrimIntPoint_fPy = TheLeafs->TheTree->FindLeaf("Nveto_segPrimaryNeutronInteractionPoints.fPy");		// Double32_t[n] // Momentum in y-direction [GeV] at the entrance of the active detector volume. Active volume refers to a Geant4 active detector volume. "DIMA"
   TheLeafs->VetoPrimIntPoint_fPz = TheLeafs->TheTree->FindLeaf("Nveto_segPrimaryNeutronInteractionPoints.fPz");		// Double32_t[n] // Momentum in z-direction [GeV] at the entrance of the active detector volume. Active volume refers to a Geant4 active detector volume. "DIMA"
   TheLeafs->VetoPrimIntPoint_fTime = TheLeafs->TheTree->FindLeaf("Nveto_segPrimaryNeutronInteractionPoints.fTime");		// Double32_t[n] // Time Of Flight between the beginning of this event and this specific interaction point [ns]. "DIMA"
   TheLeafs->VetoPrimIntPoint_fLength = TheLeafs->TheTree->FindLeaf("Nveto_segPrimaryNeutronInteractionPoints.fLength");	// Double32_t[n] // Total Track length since creation (averaged over detector) [cm]. "DIMA"
   TheLeafs->VetoPrimIntPoint_fELoss = TheLeafs->TheTree->FindLeaf("Nveto_segPrimaryNeutronInteractionPoints.fELoss");		// Double32_t[n] // Energy deposition at this specific interaction point [GeV]. "DIMA"
   TheLeafs->VetoPrimIntPoint_fDetectorID = TheLeafs->TheTree->FindLeaf("Nveto_segPrimaryNeutronInteractionPoints.fDetectorID");// Int_t[n]      // Identification of the detector. We already know however which detector we are talking about due to the name of the leaf! "DIMA"
   TheLeafs->VetoPrimIntPoint_fX = TheLeafs->TheTree->FindLeaf("Nveto_segPrimaryNeutronInteractionPoints.fX");			// Double32_t[n] // x-coordinate [cm] at entrance of active volume. Active volume refers to a Geant4 active detector volume. "DIMA"
   TheLeafs->VetoPrimIntPoint_fY = TheLeafs->TheTree->FindLeaf("Nveto_segPrimaryNeutronInteractionPoints.fY");			// Double32_t[n] // y-coordinate [cm] at entrance of active volume. Active volume refers to a Geant4 active detector volume. "DIMA"
   TheLeafs->VetoPrimIntPoint_fZ = TheLeafs->TheTree->FindLeaf("Nveto_segPrimaryNeutronInteractionPoints.fZ");			// Double32_t[n] // z-coordinate [cm] at entrance of active volume. Active volume refers to a Geant4 active detector volume. "DIMA"
   
   // Checking if the connection worked:
   if (TheLeafs->VetoPrimIntPoint_fUniqueID == 0) {VetoPrimIntPoint_Errors = VetoPrimIntPoint_Errors + 1;}
   if (TheLeafs->VetoPrimIntPoint_fBits == 0) {VetoPrimIntPoint_Errors = VetoPrimIntPoint_Errors + 1;}
   if (TheLeafs->VetoPrimIntPoint_fLink == 0) {VetoPrimIntPoint_Errors = VetoPrimIntPoint_Errors + 1;} 
   if (TheLeafs->VetoPrimIntPoint_fTrackID == 0) {VetoPrimIntPoint_Errors = VetoPrimIntPoint_Errors + 1;}
   if (TheLeafs->VetoPrimIntPoint_fEventId == 0) {VetoPrimIntPoint_Errors = VetoPrimIntPoint_Errors + 1;}
   if (TheLeafs->VetoPrimIntPoint_fPx == 0) {VetoPrimIntPoint_Errors = VetoPrimIntPoint_Errors + 1;}
   if (TheLeafs->VetoPrimIntPoint_fPy == 0) {VetoPrimIntPoint_Errors = VetoPrimIntPoint_Errors + 1;}
   if (TheLeafs->VetoPrimIntPoint_fPz == 0) {VetoPrimIntPoint_Errors = VetoPrimIntPoint_Errors + 1;}
   if (TheLeafs->VetoPrimIntPoint_fTime == 0) {VetoPrimIntPoint_Errors = VetoPrimIntPoint_Errors + 1;}
   if (TheLeafs->VetoPrimIntPoint_fLength == 0) {VetoPrimIntPoint_Errors = VetoPrimIntPoint_Errors + 1;}
   if (TheLeafs->VetoPrimIntPoint_fELoss == 0) {VetoPrimIntPoint_Errors = VetoPrimIntPoint_Errors + 1;}
   if (TheLeafs->VetoPrimIntPoint_fDetectorID == 0) {VetoPrimIntPoint_Errors = VetoPrimIntPoint_Errors + 1;}
   if (TheLeafs->VetoPrimIntPoint_fX == 0) {VetoPrimIntPoint_Errors = VetoPrimIntPoint_Errors + 1;}
   if (TheLeafs->VetoPrimIntPoint_fY == 0) {VetoPrimIntPoint_Errors = VetoPrimIntPoint_Errors + 1;}
   if (TheLeafs->VetoPrimIntPoint_fZ == 0) {VetoPrimIntPoint_Errors = VetoPrimIntPoint_Errors + 1;}
   }
   
   // -----------------------------------
   // MFI leafs:
   TheLeafs->MFIPoint_fUniqueID = TheLeafs->TheTree->FindLeaf("MFIPoint.fUniqueID");		// UInt_t[n] // This is an artifact of FairRoot. FairRoot is designed in such a way that each kind of leafs requires an fUniqueID and an fBits.
   TheLeafs->MFIPoint_fBits = TheLeafs->TheTree->FindLeaf("MFIPoint.fBits");			// UInt_t[n] // This is an artifact of FairRoot. FairRoot is designed in such a way that each kind of leafs requires an fUniqueID and an fBits.
   TheLeafs->MFIPoint_fLink = TheLeafs->TheTree->FindLeaf("MFIPoint.fLink");			// FairMultiLinkedData*[n]
   TheLeafs->MFIPoint_fTrackID = TheLeafs->TheTree->FindLeaf("MFIPoint.fTrackID");		// Int_t[n] // C++ index of where this track is located in the MC leafs.
   TheLeafs->MFIPoint_fEventId = TheLeafs->TheTree->FindLeaf("MFIPoint.fEventId");		// UInt_t[n]
   TheLeafs->MFIPoint_fPx = TheLeafs->TheTree->FindLeaf("MFIPoint.fPx");			// Double32_t[n] // Momentum in x-direction [GeV] at the entrance of the active detector volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->MFIPoint_fPy = TheLeafs->TheTree->FindLeaf("MFIPoint.fPy");			// Double32_t[n] // Momentum in y-direction [GeV] at the entrance of the active detector volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->MFIPoint_fPz = TheLeafs->TheTree->FindLeaf("MFIPoint.fPz");			// Double32_t[n] // Momentum in z-direction [GeV] at the entrance of the active detector volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->MFIPoint_fTime = TheLeafs->TheTree->FindLeaf("MFIPoint.fTime");			// Double32_t[n] // Time Of Flight between the beginning of this event and this specific interaction point [ns].
   TheLeafs->MFIPoint_fLength = TheLeafs->TheTree->FindLeaf("MFIPoint.fLength");		// Double32_t[n] // Total Track length since creation (averaged over detector) [cm].
   TheLeafs->MFIPoint_fELoss = TheLeafs->TheTree->FindLeaf("MFIPoint.fELoss");			// Double32_t[n] // Energy deposition at this specific interaction point [GeV].
   TheLeafs->MFIPoint_fDetectorID = TheLeafs->TheTree->FindLeaf("MFIPoint.fDetectorID");	// Int_t[n] // Identification of the detector. We already know however which detector we are talking about due to the name of the leaf!
   TheLeafs->MFIPoint_fX = TheLeafs->TheTree->FindLeaf("MFIPoint.fX");				// Double32_t[n] // x-coordinate [cm] at entrance of active volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->MFIPoint_fY = TheLeafs->TheTree->FindLeaf("MFIPoint.fY");				// Double32_t[n] // y-coordinate [cm] at entrance of active volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->MFIPoint_fZ = TheLeafs->TheTree->FindLeaf("MFIPoint.fZ");				// Double32_t[n] // z-coordinate [cm] at entrance of active volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->MFIPoint_fX_out = TheLeafs->TheTree->FindLeaf("MFIPoint.fX_out");			// Double32_t[n] // x-coordinate [cm] at exit of active volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->MFIPoint_fY_out = TheLeafs->TheTree->FindLeaf("MFIPoint.fY_out");			// Double32_t[n] // y-coordinate [cm] at exit of active volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->MFIPoint_fZ_out = TheLeafs->TheTree->FindLeaf("MFIPoint.fZ_out");			// Double32_t[n] // z-coordinate [cm] at exit of active volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->MFIPoint_fPx_out = TheLeafs->TheTree->FindLeaf("MFIPoint.fPx_out");		// Double32_t[n] // Momentum in x-direction [GeV] at the exit of the active detector volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->MFIPoint_fPy_out = TheLeafs->TheTree->FindLeaf("MFIPoint.fPy_out");			// Double32_t[n] // Momentum in y-direction [GeV] at the exit of the active detector volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->MFIPoint_fPz_out = TheLeafs->TheTree->FindLeaf("MFIPoint.fPz_out");		// Double32_t[n] // Momentum in z-direction [GeV] at the exit of the active detector volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->MFIPoint_fModule = TheLeafs->TheTree->FindLeaf("MFIPoint.fModule");		// Int_t[n] // probably something similar as with PSP...
   
   // Checking if the connection worked:
   if (TheLeafs->MFIPoint_fUniqueID == 0) {MFI_Errors = MFI_Errors + 1;}
   if (TheLeafs->MFIPoint_fBits == 0) {MFI_Errors = MFI_Errors + 1;}
   if (TheLeafs->MFIPoint_fLink == 0) {MFI_Errors = MFI_Errors + 1;}
   if (TheLeafs->MFIPoint_fTrackID == 0) {MFI_Errors = MFI_Errors + 1;}
   if (TheLeafs->MFIPoint_fEventId == 0) {MFI_Errors = MFI_Errors + 1;}
   if (TheLeafs->MFIPoint_fPx == 0) {MFI_Errors = MFI_Errors + 1;}
   if (TheLeafs->MFIPoint_fPy == 0) {MFI_Errors = MFI_Errors + 1;}
   if (TheLeafs->MFIPoint_fPz == 0) {MFI_Errors = MFI_Errors + 1;}
   if (TheLeafs->MFIPoint_fTime == 0) {MFI_Errors = MFI_Errors + 1;}
   if (TheLeafs->MFIPoint_fLength == 0) {MFI_Errors = MFI_Errors + 1;}
   if (TheLeafs->MFIPoint_fELoss == 0) {MFI_Errors = MFI_Errors + 1;}
   if (TheLeafs->MFIPoint_fDetectorID == 0) {MFI_Errors = MFI_Errors + 1;}
   if (TheLeafs->MFIPoint_fX == 0) {MFI_Errors = MFI_Errors + 1;}	
   if (TheLeafs->MFIPoint_fY == 0) {MFI_Errors = MFI_Errors + 1;}	
   if (TheLeafs->MFIPoint_fZ == 0) {MFI_Errors = MFI_Errors + 1;}	
   if (TheLeafs->MFIPoint_fX_out == 0) {MFI_Errors = MFI_Errors + 1;}
   if (TheLeafs->MFIPoint_fY_out == 0) {MFI_Errors = MFI_Errors + 1;}
   if (TheLeafs->MFIPoint_fZ_out == 0) {MFI_Errors = MFI_Errors + 1;}
   if (TheLeafs->MFIPoint_fPx_out == 0) {MFI_Errors = MFI_Errors + 1;}
   if (TheLeafs->MFIPoint_fPy_out == 0) {MFI_Errors = MFI_Errors + 1;}
   if (TheLeafs->MFIPoint_fPz_out == 0) {MFI_Errors = MFI_Errors + 1;}
   if (TheLeafs->MFIPoint_fModule == 0) {MFI_Errors = MFI_Errors + 1;}
   
   // -----------------------------------
   // PSP leafs:
   TheLeafs->PSPPoint_fUniqueID = TheLeafs->TheTree->FindLeaf("PSPPoint.fUniqueID");		// UInt_t[n] // This is an artifact of FairRoot. FairRoot is designed in such a way that each kind of leafs requires an fUniqueID and an fBits.
   TheLeafs->PSPPoint_fBits = TheLeafs->TheTree->FindLeaf("PSPPoint.fBits");			// UInt_t[n] // This is an artifact of FairRoot. FairRoot is designed in such a way that each kind of leafs requires an fUniqueID and an fBits.
   TheLeafs->PSPPoint_fLink = TheLeafs->TheTree->FindLeaf("PSPPoint.fLink");			// FairMultiLinkedData*[n]
   TheLeafs->PSPPoint_fTrackID = TheLeafs->TheTree->FindLeaf("PSPPoint.fTrackID");		// Int_t[n] // C++ index of where this track is located in the MC leafs.
   TheLeafs->PSPPoint_fEventId = TheLeafs->TheTree->FindLeaf("PSPPoint.fEventId");		// UInt_t[n]
   TheLeafs->PSPPoint_fPx = TheLeafs->TheTree->FindLeaf("PSPPoint.fPx");			// Double32_t[n] // Momentum in x-direction [GeV] at the entrance of the active detector volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->PSPPoint_fPy = TheLeafs->TheTree->FindLeaf("PSPPoint.fPy");			// Double32_t[n] // Momentum in y-direction [GeV] at the entrance of the active detector volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->PSPPoint_fPz = TheLeafs->TheTree->FindLeaf("PSPPoint.fPz");			// Double32_t[n] // Momentum in z-direction [GeV] at the entrance of the active detector volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->PSPPoint_fTime = TheLeafs->TheTree->FindLeaf("PSPPoint.fTime");			// Double32_t[n] // Time Of Flight between the beginning of this event and this specific interaction point [ns].
   TheLeafs->PSPPoint_fLength = TheLeafs->TheTree->FindLeaf("PSPPoint.fLength");		// Double32_t[n] // Total Track length since creation (averaged over detector) [cm].
   TheLeafs->PSPPoint_fELoss = TheLeafs->TheTree->FindLeaf("PSPPoint.fELoss");			// Double32_t[n] // Energy deposition at this specific interaction point [GeV].
   TheLeafs->PSPPoint_fDetectorID = TheLeafs->TheTree->FindLeaf("PSPPoint.fDetectorID");	// Int_t[n] // Identification of the detector. We already know however which detector we are talking about due to the name of the leaf!
   TheLeafs->PSPPoint_fX = TheLeafs->TheTree->FindLeaf("PSPPoint.fX");				// Double32_t[n] // x-coordinate [cm] at entrance of active volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->PSPPoint_fY = TheLeafs->TheTree->FindLeaf("PSPPoint.fY");				// Double32_t[n] // y-coordinate [cm] at entrance of active volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->PSPPoint_fZ = TheLeafs->TheTree->FindLeaf("PSPPoint.fZ");				// Double32_t[n] // z-coordinate [cm] at entrance of active volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->PSPPoint_fX_out = TheLeafs->TheTree->FindLeaf("PSPPoint.fX_out");			// Double32_t[n] // x-coordinate [cm] at exit of active volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->PSPPoint_fY_out = TheLeafs->TheTree->FindLeaf("PSPPoint.fY_out");			// Double32_t[n] // y-coordinate [cm] at exit of active volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->PSPPoint_fZ_out = TheLeafs->TheTree->FindLeaf("PSPPoint.fZ_out");			// Double32_t[n] // z-coordinate [cm] at exit of active volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->PSPPoint_fPx_out = TheLeafs->TheTree->FindLeaf("PSPPoint.fPx_out");		// Double32_t[n] // Momentum in x-direction [GeV] at the exit of the active detector volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->PSPPoint_fPy_out = TheLeafs->TheTree->FindLeaf("PSPPoint.fPy_out");			// Double32_t[n] // Momentum in y-direction [GeV] at the exit of the active detector volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->PSPPoint_fPz_out = TheLeafs->TheTree->FindLeaf("PSPPoint.fPz_out");		// Double32_t[n] // Momentum in z-direction [GeV] at the exit of the active detector volume. Active volume refers to a Geant4 active detector volume.
   TheLeafs->PSPPoint_fModule = TheLeafs->TheTree->FindLeaf("PSPPoint.fModule");		// Int_t[n] // probably this states which detector is hit: PSP 1, 2 or 3.
   
   // Checking if the connection worked:
   if (TheLeafs->PSPPoint_fUniqueID == 0) {PSP_Errors = PSP_Errors + 1;}
   if (TheLeafs->PSPPoint_fBits == 0) {PSP_Errors = PSP_Errors + 1;}
   if (TheLeafs->PSPPoint_fLink == 0) {PSP_Errors = PSP_Errors + 1;}
   if (TheLeafs->PSPPoint_fTrackID == 0) {PSP_Errors = PSP_Errors + 1;}
   if (TheLeafs->PSPPoint_fEventId == 0) {PSP_Errors = PSP_Errors + 1;}
   if (TheLeafs->PSPPoint_fPx == 0) {PSP_Errors = PSP_Errors + 1;}
   if (TheLeafs->PSPPoint_fPy == 0) {PSP_Errors = PSP_Errors + 1;}
   if (TheLeafs->PSPPoint_fPz == 0) {PSP_Errors = PSP_Errors + 1;}
   if (TheLeafs->PSPPoint_fTime == 0) {PSP_Errors = PSP_Errors + 1;}
   if (TheLeafs->PSPPoint_fLength == 0) {PSP_Errors = PSP_Errors + 1;}
   if (TheLeafs->PSPPoint_fELoss == 0) {PSP_Errors = PSP_Errors + 1;}
   if (TheLeafs->PSPPoint_fDetectorID == 0) {PSP_Errors = PSP_Errors + 1;}
   if (TheLeafs->PSPPoint_fX == 0) {PSP_Errors = PSP_Errors + 1;}	
   if (TheLeafs->PSPPoint_fY == 0) {PSP_Errors = PSP_Errors + 1;}	
   if (TheLeafs->PSPPoint_fZ == 0) {PSP_Errors = PSP_Errors + 1;}	
   if (TheLeafs->PSPPoint_fX_out == 0) {PSP_Errors = PSP_Errors + 1;}
   if (TheLeafs->PSPPoint_fY_out == 0) {PSP_Errors = PSP_Errors + 1;}
   if (TheLeafs->PSPPoint_fZ_out == 0) {PSP_Errors = PSP_Errors + 1;}
   if (TheLeafs->PSPPoint_fPx_out == 0) {PSP_Errors = PSP_Errors + 1;}
   if (TheLeafs->PSPPoint_fPy_out == 0) {PSP_Errors = PSP_Errors + 1;}
   if (TheLeafs->PSPPoint_fPz_out == 0) {PSP_Errors = PSP_Errors + 1;}
   if (TheLeafs->PSPPoint_fModule == 0) {PSP_Errors = PSP_Errors + 1;}
   
   // -----------------------------------
   // MC Eventheader leafs:
   TheLeafs->MCEventHeader_TNamed_fUniqueID = TheLeafs->TheTree->FindLeaf("MCEventHeader.TNamed.fUniqueID");	// UInt_t // This is an artifact of FairRoot. FairRoot is designed in such a way that each kind of leafs requires an fUniqueID and an fBits.
   TheLeafs->MCEventHeader_TNamed_fBits = TheLeafs->TheTree->FindLeaf("MCEventHeader.TNamed.fBits");		// UInt_t // This is an artifact of FairRoot. FairRoot is designed in such a way that each kind of leafs requires an fUniqueID and an fBits.
   TheLeafs->MCEventHeader_TNamed_fName = TheLeafs->TheTree->FindLeaf("MCEventHeader.TNamed.fName");		// TString
   TheLeafs->MCEventHeader_TNamed_fTitle = TheLeafs->TheTree->FindLeaf("MCEventHeader.TNamed.fTitle");		// TString
   TheLeafs->MCEventHeader_fRunId = TheLeafs->TheTree->FindLeaf("MCEventHeader.fRunId");			// UInt_t
   TheLeafs->MCEventHeader_fEventId = TheLeafs->TheTree->FindLeaf("MCEventHeader.fEventId");			// UInt_t
   TheLeafs->MCEventHeader_fX = TheLeafs->TheTree->FindLeaf("MCEventHeader.fX");				// Double32_t
   TheLeafs->MCEventHeader_fY = TheLeafs->TheTree->FindLeaf("MCEventHeader.fY");				// Double32_t
   TheLeafs->MCEventHeader_fZ = TheLeafs->TheTree->FindLeaf("MCEventHeader.fZ");				// Double32_t
   TheLeafs->MCEventHeader_fT = TheLeafs->TheTree->FindLeaf("MCEventHeader.fT");				// Double32_t
   TheLeafs->MCEventHeader_fB = TheLeafs->TheTree->FindLeaf("MCEventHeader.fB");				// Double32_t
   TheLeafs->MCEventHeader_fNPrim = TheLeafs->TheTree->FindLeaf("MCEventHeader.fNPrim");			// Int_t
   TheLeafs->MCEventHeader_fIsSet = TheLeafs->TheTree->FindLeaf("MCEventHeader.fIsSet");			// Bool_t
   TheLeafs->MCEventHeader_fRotX = TheLeafs->TheTree->FindLeaf("MCEventHeader.fRotX");				// Double32_t
   TheLeafs->MCEventHeader_fRotY = TheLeafs->TheTree->FindLeaf("MCEventHeader.fRotY");				// Double32_t
   TheLeafs->MCEventHeader_fRotZ = TheLeafs->TheTree->FindLeaf("MCEventHeader.fRotZ");				// Double32_t
   
   // Checking if the connection worked:
   if (TheLeafs->MCEventHeader_TNamed_fUniqueID == 0) {EventHeader_Errors = EventHeader_Errors + 1;}
   if (TheLeafs->MCEventHeader_TNamed_fBits == 0) {EventHeader_Errors = EventHeader_Errors + 1;}
   if (TheLeafs->MCEventHeader_TNamed_fName == 0) {EventHeader_Errors = EventHeader_Errors + 1;}
   if (TheLeafs->MCEventHeader_TNamed_fTitle == 0) {EventHeader_Errors = EventHeader_Errors + 1;}
   if (TheLeafs->MCEventHeader_fRunId == 0) {EventHeader_Errors = EventHeader_Errors + 1;}
   if (TheLeafs->MCEventHeader_fEventId == 0) {EventHeader_Errors = EventHeader_Errors + 1;}
   if (TheLeafs->MCEventHeader_fX == 0) {EventHeader_Errors = EventHeader_Errors + 1;}
   if (TheLeafs->MCEventHeader_fY == 0) {EventHeader_Errors = EventHeader_Errors + 1;}
   if (TheLeafs->MCEventHeader_fZ == 0) {EventHeader_Errors = EventHeader_Errors + 1;}
   if (TheLeafs->MCEventHeader_fT == 0) {EventHeader_Errors = EventHeader_Errors + 1;}
   if (TheLeafs->MCEventHeader_fB == 0) {EventHeader_Errors = EventHeader_Errors + 1;}
   if (TheLeafs->MCEventHeader_fNPrim == 0) {EventHeader_Errors = EventHeader_Errors + 1;}
   if (TheLeafs->MCEventHeader_fIsSet == 0) {EventHeader_Errors = EventHeader_Errors + 1;}
   if (TheLeafs->MCEventHeader_fRotX == 0) {EventHeader_Errors = EventHeader_Errors + 1;}
   if (TheLeafs->MCEventHeader_fRotY == 0) {EventHeader_Errors = EventHeader_Errors + 1;}
   if (TheLeafs->MCEventHeader_fRotZ == 0) {EventHeader_Errors = EventHeader_Errors + 1;}
   
   // -----------------------------------
   // GeoTracks leafs:
   TheLeafs->GeoTracks_fUniqueID = TheLeafs->TheTree->FindLeaf("GeoTracks.fUniqueID");		// UInt_t[n] // This is an artifact of FairRoot. FairRoot is designed in such a way that each kind of leafs requires an fUniqueID and an fBits.
   TheLeafs->GeoTracks_fBits = TheLeafs->TheTree->FindLeaf("GeoTracks.fBits");			// UInt_t[n] // This is an artifact of FairRoot. FairRoot is designed in such a way that each kind of leafs requires an fUniqueID and an fBits.
   TheLeafs->GeoTracks_fGeoAtt = TheLeafs->TheTree->FindLeaf("GeoTracks.fGeoAtt");		// UInt_t[n]
   TheLeafs->GeoTracks_fLineColor = TheLeafs->TheTree->FindLeaf("GeoTracks.fLineColor");	// Short_t[n]
   TheLeafs->GeoTracks_fLineStyle = TheLeafs->TheTree->FindLeaf("GeoTracks.fLineStyle");	// Short_t[n]
   TheLeafs->GeoTracks_fLineWidth = TheLeafs->TheTree->FindLeaf("GeoTracks.fLineWidth");	// Short_t[n]
   TheLeafs->GeoTracks_fMarkerColor = TheLeafs->TheTree->FindLeaf("GeoTracks.fMarkerColor");	// Short_t[n]
   TheLeafs->GeoTracks_fMarkerStyle = TheLeafs->TheTree->FindLeaf("GeoTracks.fMarkerStyle");	// Short_t[n]
   TheLeafs->GeoTracks_fMarkerSize = TheLeafs->TheTree->FindLeaf("GeoTracks.fMarkerSize");	// Float_t[n]
   TheLeafs->GeoTracks_fPDG = TheLeafs->TheTree->FindLeaf("GeoTracks.fPDG");			// Int_t[n]
   TheLeafs->GeoTracks_fId = TheLeafs->TheTree->FindLeaf("GeoTracks.fId");			// Int_t[n]
   TheLeafs->GeoTracks_fParent = TheLeafs->TheTree->FindLeaf("GeoTracks.fParent");		// TVirtualGeoTrack*[n]
   TheLeafs->GeoTracks_fParticle = TheLeafs->TheTree->FindLeaf("GeoTracks.fParticle");		// TObject*[n]
   TheLeafs->GeoTracks_fTracks = TheLeafs->TheTree->FindLeaf("GeoTracks.fTracks");		// TObjArray*[n]
   TheLeafs->GeoTracks_fPointsSize = TheLeafs->TheTree->FindLeaf("GeoTracks.fPointsSize");	// Int_t[n]
   TheLeafs->GeoTracks_fNpoints = TheLeafs->TheTree->FindLeaf("GeoTracks.fNpoints");		// Int_t[n]
   TheLeafs->GeoTracks_fPoints = TheLeafs->TheTree->FindLeaf("GeoTracks.fPoints");		// Double_t[n]
   
   // Checking if the connection worked:
   if (TheLeafs->GeoTracks_fUniqueID == 0) {GeoTracks_Errors = GeoTracks_Errors + 1;}
   if (TheLeafs->GeoTracks_fBits == 0) {GeoTracks_Errors = GeoTracks_Errors + 1;}
   if (TheLeafs->GeoTracks_fGeoAtt == 0) {GeoTracks_Errors = GeoTracks_Errors + 1;}
   if (TheLeafs->GeoTracks_fLineColor == 0) {GeoTracks_Errors = GeoTracks_Errors + 1;}
   if (TheLeafs->GeoTracks_fLineStyle == 0) {GeoTracks_Errors = GeoTracks_Errors + 1;}
   if (TheLeafs->GeoTracks_fLineWidth == 0) {GeoTracks_Errors = GeoTracks_Errors + 1;}
   if (TheLeafs->GeoTracks_fMarkerColor == 0) {GeoTracks_Errors = GeoTracks_Errors + 1;}
   if (TheLeafs->GeoTracks_fMarkerStyle == 0) {GeoTracks_Errors = GeoTracks_Errors + 1;}
   if (TheLeafs->GeoTracks_fMarkerSize == 0) {GeoTracks_Errors = GeoTracks_Errors + 1;}
   if (TheLeafs->GeoTracks_fPDG == 0) {GeoTracks_Errors = GeoTracks_Errors + 1;}
   if (TheLeafs->GeoTracks_fId == 0) {GeoTracks_Errors = GeoTracks_Errors + 1;}
   if (TheLeafs->GeoTracks_fParent == 0) {GeoTracks_Errors = GeoTracks_Errors + 1;}
   if (TheLeafs->GeoTracks_fParticle == 0) {GeoTracks_Errors = GeoTracks_Errors + 1;}
   if (TheLeafs->GeoTracks_fTracks == 0) {GeoTracks_Errors = GeoTracks_Errors + 1;}
   if (TheLeafs->GeoTracks_fPointsSize == 0) {GeoTracks_Errors = GeoTracks_Errors + 1;}
   if (TheLeafs->GeoTracks_fNpoints == 0) {GeoTracks_Errors = GeoTracks_Errors + 1;}
   if (TheLeafs->GeoTracks_fPoints == 0) {GeoTracks_Errors = GeoTracks_Errors + 1;}
   
   // ===============================================================================================
   // Now we continue with the digitizer leafs:
   
   if (Gean&&Neu&&NEB&&NEBv)
   {
   
   // -----------------------------------
   // NeuLAND digitizer leafs:
   TheLeafs->LandDigi_fUniqueID = TheLeafs->NeuDigiTree->FindLeaf("LandDigi.fUniqueID");		// UInt_t[n] // This is an artifact of FairRoot. FairRoot is designed in such a way that each kind of leafs requires an fUniqueID and an fBits.
   TheLeafs->LandDigi_fBits = TheLeafs->NeuDigiTree->FindLeaf("LandDigi.fBits");			// UInt_t[n] // This is an artifact of FairRoot. FairRoot is designed in such a way that each kind of leafs requires an fUniqueID and an fBits.
														     // NOTE: The leaf size is the number of bars that fired in this particluar event! Information is given to me by Jan Mayer.
   TheLeafs->LandDigi_fPaddleNb = TheLeafs->NeuDigiTree->FindLeaf("LandDigi.fPaddleNb");		// Int_t[n] // Number of the bar from NeuLAND that registered the hit. Remember the mapping table of NeuLAND!
   TheLeafs->LandDigi_fTdcL = TheLeafs->NeuDigiTree->FindLeaf("LandDigi.fTdcL");			// Double32_t[n] // TDC output of the first PMT as coming from TACQUILA [ns]
   TheLeafs->LandDigi_fTdcR = TheLeafs->NeuDigiTree->FindLeaf("LandDigi.fTdcR");			// Double32_t[n] // TDC output of the second PMT as coming from TACQUILA [ns]
   TheLeafs->LandDigi_fTdc = TheLeafs->NeuDigiTree->FindLeaf("LandDigi.fTdc");				// Double32_t[n] // Reconstructed time at the hit: (leftPMT.GetTDC() + rightPMT.GetTDC()) / 2. - fPaddleHalfLength / fcMedium; [ns]
															 // computed in DigitizationEngine.h un Paddle-structure
   TheLeafs->LandDigi_fQdcL = TheLeafs->NeuDigiTree->FindLeaf("LandDigi.fQdcL");			// Double32_t[n] // QDC output of the first PMT as coming from TACQUILA [MeV, but uncalibrated!]
   TheLeafs->LandDigi_fQdcR = TheLeafs->NeuDigiTree->FindLeaf("LandDigi.fQdcR");			// Double32_t[n] // QDC output of the second PMT as coming from TACQUILA [MeV, nut uncalibrated!]
   TheLeafs->LandDigi_fQdc = TheLeafs->NeuDigiTree->FindLeaf("LandDigi.fQdc");				// Double32_t[n] // Reconstructed energy deposition at the hit: It is computed as sqrt(leftPMT.GetEnergy() * rightPMT.GetEnergy()) [MeV, nut uncalibrated!]
															 // computed in DigitizationEngine.h un Paddle-structure
   TheLeafs->LandDigi_fxx = TheLeafs->NeuDigiTree->FindLeaf("LandDigi.fxx");				// Double32_t[n] // Hit X-coordinate detemined with TOF information from TDC: Computed in Paddle Structure as (rightPMT.GetTDC() - leftPMT.GetTDC()) / 2.*fcMedium; [cm]
   TheLeafs->LandDigi_fyy = TheLeafs->NeuDigiTree->FindLeaf("LandDigi.fyy");				// Double32_t[n] // Hit Y-coordinate detemined with TOF information from TDC: Computed in Paddle Structure as (rightPMT.GetTDC() - leftPMT.GetTDC()) / 2.*fcMedium; [cm]
   TheLeafs->LandDigi_fzz = TheLeafs->NeuDigiTree->FindLeaf("LandDigi.fzz");				// Double32_t[n] // Hit Z-coordinate detemined with TOF information from TDC: Computed in Paddle Structure as (rightPMT.GetTDC() - leftPMT.GetTDC()) / 2.*fcMedium; [cm]

   // Checking if the connection worked:
   if (TheLeafs->LandDigi_fUniqueID == 0) {LandDigi_Errors = LandDigi_Errors + 1;}
   if (TheLeafs->LandDigi_fBits == 0) {LandDigi_Errors = LandDigi_Errors + 1;}
   if (TheLeafs->LandDigi_fPaddleNb == 0) {LandDigi_Errors = LandDigi_Errors + 1;}
   if (TheLeafs->LandDigi_fTdcL == 0) {LandDigi_Errors = LandDigi_Errors + 1;}
   if (TheLeafs->LandDigi_fTdcR == 0) {LandDigi_Errors = LandDigi_Errors + 1;}
   if (TheLeafs->LandDigi_fTdc == 0) {LandDigi_Errors = LandDigi_Errors + 1;}
   if (TheLeafs->LandDigi_fQdcL == 0) {LandDigi_Errors = LandDigi_Errors + 1;}
   if (TheLeafs->LandDigi_fQdcR == 0) {LandDigi_Errors = LandDigi_Errors + 1;}
   if (TheLeafs->LandDigi_fQdc == 0) {LandDigi_Errors = LandDigi_Errors + 1;}
   if (TheLeafs->LandDigi_fxx == 0) {LandDigi_Errors = LandDigi_Errors + 1;}
   if (TheLeafs->LandDigi_fyy == 0) {LandDigi_Errors = LandDigi_Errors + 1;}
   if (TheLeafs->LandDigi_fzz == 0) {LandDigi_Errors = LandDigi_Errors + 1;}
   
   // if it doesn work, retry:
   if (LandDigi_Errors==12)
   {
       LandDigi_Errors = 0;
       
   TheLeafs->LandDigi_fUniqueID = TheLeafs->NeuDigiTree->FindLeaf("NeulandDigis.fUniqueID");	
   TheLeafs->LandDigi_fBits = TheLeafs->NeuDigiTree->FindLeaf("NeulandDigis.fBits");
   TheLeafs->LandDigi_fPaddleNb = TheLeafs->NeuDigiTree->FindLeaf("NeulandDigis.fPaddle");
   TheLeafs->LandDigi_fTdcL = TheLeafs->NeuDigiTree->FindLeaf("NeulandDigis.fTdcL");
   TheLeafs->LandDigi_fTdcR = TheLeafs->NeuDigiTree->FindLeaf("NeulandDigis.fTdcR");
   TheLeafs->LandDigi_fTdc = TheLeafs->NeuDigiTree->FindLeaf("NeulandDigis.fT");	
   TheLeafs->LandDigi_fQdcL = TheLeafs->NeuDigiTree->FindLeaf("NeulandDigis.fQdcL");
   TheLeafs->LandDigi_fQdcR = TheLeafs->NeuDigiTree->FindLeaf("NeulandDigis.fQdcR");	
   TheLeafs->LandDigi_fQdc = TheLeafs->NeuDigiTree->FindLeaf("NeulandDigis.fE");		
   TheLeafs->LandDigi_fxx = TheLeafs->NeuDigiTree->FindLeaf("NeulandDigis.fPosition.fX");		
   TheLeafs->LandDigi_fyy = TheLeafs->NeuDigiTree->FindLeaf("NeulandDigis.fPosition.fY");			
   TheLeafs->LandDigi_fzz = TheLeafs->NeuDigiTree->FindLeaf("NeulandDigis.fPosition.fZ");		

   if (TheLeafs->LandDigi_fUniqueID == 0) {LandDigi_Errors = LandDigi_Errors + 1;}
   if (TheLeafs->LandDigi_fBits == 0) {LandDigi_Errors = LandDigi_Errors + 1;}
   if (TheLeafs->LandDigi_fPaddleNb == 0) {LandDigi_Errors = LandDigi_Errors + 1;}
   if (TheLeafs->LandDigi_fTdcL == 0) {LandDigi_Errors = LandDigi_Errors + 1;}
   if (TheLeafs->LandDigi_fTdcR == 0) {LandDigi_Errors = LandDigi_Errors + 1;}
   if (TheLeafs->LandDigi_fTdc == 0) {LandDigi_Errors = LandDigi_Errors + 1;}
   if (TheLeafs->LandDigi_fQdcL == 0) {LandDigi_Errors = LandDigi_Errors + 1;}
   if (TheLeafs->LandDigi_fQdcR == 0) {LandDigi_Errors = LandDigi_Errors + 1;}
   if (TheLeafs->LandDigi_fQdc == 0) {LandDigi_Errors = LandDigi_Errors + 1;}
   if (TheLeafs->LandDigi_fxx == 0) {LandDigi_Errors = LandDigi_Errors + 1;}
   if (TheLeafs->LandDigi_fyy == 0) {LandDigi_Errors = LandDigi_Errors + 1;}
   if (TheLeafs->LandDigi_fzz == 0) {LandDigi_Errors = LandDigi_Errors + 1;}
   }
   
   
   // -----------------------------------
   // NeuLAND ClusterFinder leafs:
   if (Neu_Ex==1)
   {
   TheLeafs->NeuLandCluster_fUniqueID = TheLeafs->NeuDigiTree->FindLeaf("NeuLandCluster.fUniqueID");	// UInt_t[n]
   TheLeafs->NeuLandCluster_fBits = TheLeafs->NeuDigiTree->FindLeaf("NeuLandCluster.fBits");		// UInt_t[n]
   TheLeafs->NeuLandCluster_fStartX = TheLeafs->NeuDigiTree->FindLeaf("NeuLandCluster.fStartX");	// Double32_t[n] // A Cluster contains multiple bars that all fired. These bars will have fired at different times. Hence the fStart-leafs are the time, X, Y and Z
   TheLeafs->NeuLandCluster_fStartY = TheLeafs->NeuDigiTree->FindLeaf("NeuLandCluster.fStartY");	// Double32_t[n] // coordinates of the first hit (= first bar) in that cluster. Likewise, you also have a bar that fired last. The fStop-leafs are the time,
   TheLeafs->NeuLandCluster_fStartZ = TheLeafs->NeuDigiTree->FindLeaf("NeuLandCluster.fStartZ");	// Double32_t[n] // X, Y and Z coordinates of this last bar (=hit) inside that cluster. These times and X, Y and Z coordinates are extracted from the 
   TheLeafs->NeuLandCluster_fStartT = TheLeafs->NeuDigiTree->FindLeaf("NeuLandCluster.fStartT");	// Double32_t[n] // physical location of each bar and the use of the TimeDifference and TOF measurements for each bar in the cluster. Of course the
   TheLeafs->NeuLandCluster_fStopX = TheLeafs->NeuDigiTree->FindLeaf("NeuLandCluster.fStopX");		// Double32_t[n] // physical location of a bar has a resolution of 5 cm, while TimeDifference has a much better position resolution. NOTE: Jan Mayer told me this information!
   TheLeafs->NeuLandCluster_fStopY = TheLeafs->NeuDigiTree->FindLeaf("NeuLandCluster.fStopY");		// Double32_t[n] // NOTE: leaf size is the number of clusters!
   TheLeafs->NeuLandCluster_fStopZ = TheLeafs->NeuDigiTree->FindLeaf("NeuLandCluster.fStopZ");		// Double32_t[n]
   TheLeafs->NeuLandCluster_fStopT = TheLeafs->NeuDigiTree->FindLeaf("NeuLandCluster.fStopT");		// Double32_t[n]
   TheLeafs->NeuLandCluster_fE = TheLeafs->NeuDigiTree->FindLeaf("NeuLandCluster.fE");			// Double32_t[n] // Total energy deposition in the full cluster. This information is computed from the LandDigi_fQdc Leaf, hence it is uncalibrated!
   TheLeafs->NeuLandCluster_fSize = TheLeafs->NeuDigiTree->FindLeaf("NeuLandCluster.fSize");		// Double32_t[n] // ?
   TheLeafs->NeuLandCluster_fMarked = TheLeafs->NeuDigiTree->FindLeaf("NeuLandCluster.fMarked");	// Bool_t[n] // ?
   
   // Checking if the connection worked:
   if (TheLeafs->NeuLandCluster_fUniqueID == 0) {LandCluster_Errors = LandCluster_Errors + 1;}
   if (TheLeafs->NeuLandCluster_fBits == 0) {LandCluster_Errors = LandCluster_Errors + 1;}
   if (TheLeafs->NeuLandCluster_fStartX == 0) {LandCluster_Errors = LandCluster_Errors + 1;}
   if (TheLeafs->NeuLandCluster_fStartY == 0) {LandCluster_Errors = LandCluster_Errors + 1;}
   if (TheLeafs->NeuLandCluster_fStartZ == 0) {LandCluster_Errors = LandCluster_Errors + 1;}
   if (TheLeafs->NeuLandCluster_fStartT == 0) {LandCluster_Errors = LandCluster_Errors + 1;}
   if (TheLeafs->NeuLandCluster_fStopX == 0) {LandCluster_Errors = LandCluster_Errors + 1;}
   if (TheLeafs->NeuLandCluster_fStopY == 0) {LandCluster_Errors = LandCluster_Errors + 1;}
   if (TheLeafs->NeuLandCluster_fStopZ == 0) {LandCluster_Errors = LandCluster_Errors + 1;}
   if (TheLeafs->NeuLandCluster_fStopT == 0) {LandCluster_Errors = LandCluster_Errors + 1;}
   if (TheLeafs->NeuLandCluster_fE == 0) {LandCluster_Errors = LandCluster_Errors + 1;}
   if (TheLeafs->NeuLandCluster_fSize == 0) {LandCluster_Errors = LandCluster_Errors + 1;}
   if (TheLeafs->NeuLandCluster_fMarked == 0) {LandCluster_Errors = LandCluster_Errors + 1;}
   }
   
   if (Neu_Ex==2)
   {
   TheLeafs->NeuLandCluster_fUniqueID = TheLeafs->NeuDigiTree->FindLeaf("NeuLANDCluster.fUniqueID");	// UInt_t[n]
   TheLeafs->NeuLandCluster_fBits = TheLeafs->NeuDigiTree->FindLeaf("NeuLANDCluster.fBits");		// UInt_t[n]
   TheLeafs->NeuLandCluster_fStartX = TheLeafs->NeuDigiTree->FindLeaf("NeuLANDCluster.fStartX");	// Double32_t[n] // A Cluster contains multiple bars that all fired. These bars will have fired at different times. Hence the fStart-leafs are the time, X, Y and Z
   TheLeafs->NeuLandCluster_fStartY = TheLeafs->NeuDigiTree->FindLeaf("NeuLANDCluster.fStartY");	// Double32_t[n] // coordinates of the first hit (= first bar) in that cluster. Likewise, you also have a bar that fired last. The fStop-leafs are the time,
   TheLeafs->NeuLandCluster_fStartZ = TheLeafs->NeuDigiTree->FindLeaf("NeuLANDCluster.fStartZ");	// Double32_t[n] // X, Y and Z coordinates of this last bar (=hit) inside that cluster. These times and X, Y and Z coordinates are extracted from the 
   TheLeafs->NeuLandCluster_fStartT = TheLeafs->NeuDigiTree->FindLeaf("NeuLANDCluster.fStartT");	// Double32_t[n] // physical location of each bar and the use of the TimeDifference and TOF measurements for each bar in the cluster. Of course the
   TheLeafs->NeuLandCluster_fStopX = TheLeafs->NeuDigiTree->FindLeaf("NeuLANDCluster.fStopX");		// Double32_t[n] // physical location of a bar has a resolution of 5 cm, while TimeDifference has a much better position resolution. NOTE: Jan Mayer told me this information!
   TheLeafs->NeuLandCluster_fStopY = TheLeafs->NeuDigiTree->FindLeaf("NeuLANDCluster.fStopY");		// Double32_t[n] // NOTE: leaf size is the number of clusters!
   TheLeafs->NeuLandCluster_fStopZ = TheLeafs->NeuDigiTree->FindLeaf("NeuLANDCluster.fStopZ");		// Double32_t[n] // The fEdge-leafs contain the middle point of a cluster in case we found a kink/pitchfort/crsoo-topology.
   TheLeafs->NeuLandCluster_fStopT = TheLeafs->NeuDigiTree->FindLeaf("NeuLANDCluster.fStopT");		// Double32_t[n]
   TheLeafs->NeuLandCluster_fEdgeX = TheLeafs->NeuDigiTree->FindLeaf("NeuLANDCluster.fEdgeX");		// Double32_t[n] 
   TheLeafs->NeuLandCluster_fEdgeY = TheLeafs->NeuDigiTree->FindLeaf("NeuLANDCluster.fEdgeY");		// Double32_t[n] 
   TheLeafs->NeuLandCluster_fEdgeZ = TheLeafs->NeuDigiTree->FindLeaf("NeuLANDCluster.fEdgeZ");		// Double32_t[n]
   TheLeafs->NeuLandCluster_fEdgeT = TheLeafs->NeuDigiTree->FindLeaf("NeuLANDCluster.fEdgeT");		// Double32_t[n]
   TheLeafs->NeuLandCluster_fE = TheLeafs->NeuDigiTree->FindLeaf("NeuLANDCluster.fE");			// Double32_t[n] // Total energy deposition in the full cluster. This information is computed from the LandDigi_fQdc Leaf, hence it is uncalibrated!
   TheLeafs->NeuLandCluster_fSize = TheLeafs->NeuDigiTree->FindLeaf("NeuLANDCluster.fSize");		// Int_t[n] // Total number of bars in the cluster.
   TheLeafs->NeuLandCluster_fTopology = TheLeafs->NeuDigiTree->FindLeaf("NeuLANDCluster.fTopology");	// Int_t[n] // 1 = line, 2=kink, 3=pitchfork, 4=cross, 5=unidentifiied, 6=singeton, 7=empty ==> Tells you the shape of the cluster!
   TheLeafs->NeuLandCluster_fMarked = TheLeafs->NeuDigiTree->FindLeaf("NeuLANDCluster.fMarked");	// Bool_t[n] // Marker for the Vertex reconstruction.
   TheLeafs->NeuLandCluster_fEliminated = TheLeafs->NeuDigiTree->FindLeaf("NeuLANDCluster.fEliminated");// Bool_t[n] // VETO Eliminator for the Vertex reconstruction.
   TheLeafs->NeuLandCluster_fDigibars = TheLeafs->NeuDigiTree->FindLeaf("NeuLANDCluster.fDigiBars");	// <<std::vector<Int_t>[n] // All bars that belong to this cluster in global bar numbers.
   
   // Checking if the connection worked:
   if (TheLeafs->NeuLandCluster_fUniqueID == 0) {LandCluster_Errors = LandCluster_Errors + 1;}
   if (TheLeafs->NeuLandCluster_fBits == 0) {LandCluster_Errors = LandCluster_Errors + 1;}
   if (TheLeafs->NeuLandCluster_fStartX == 0) {LandCluster_Errors = LandCluster_Errors + 1;}
   if (TheLeafs->NeuLandCluster_fStartY == 0) {LandCluster_Errors = LandCluster_Errors + 1;}
   if (TheLeafs->NeuLandCluster_fStartZ == 0) {LandCluster_Errors = LandCluster_Errors + 1;}
   if (TheLeafs->NeuLandCluster_fStartT == 0) {LandCluster_Errors = LandCluster_Errors + 1;}
   if (TheLeafs->NeuLandCluster_fStopX == 0) {LandCluster_Errors = LandCluster_Errors + 1;}
   if (TheLeafs->NeuLandCluster_fStopY == 0) {LandCluster_Errors = LandCluster_Errors + 1;}
   if (TheLeafs->NeuLandCluster_fStopZ == 0) {LandCluster_Errors = LandCluster_Errors + 1;}
   if (TheLeafs->NeuLandCluster_fStopT == 0) {LandCluster_Errors = LandCluster_Errors + 1;}
   if (TheLeafs->NeuLandCluster_fEdgeX == 0) {LandCluster_Errors = LandCluster_Errors + 1;}
   if (TheLeafs->NeuLandCluster_fEdgeY == 0) {LandCluster_Errors = LandCluster_Errors + 1;}
   if (TheLeafs->NeuLandCluster_fEdgeZ == 0) {LandCluster_Errors = LandCluster_Errors + 1;}
   if (TheLeafs->NeuLandCluster_fEdgeT == 0) {LandCluster_Errors = LandCluster_Errors + 1;}
   if (TheLeafs->NeuLandCluster_fE == 0) {LandCluster_Errors = LandCluster_Errors + 1;}
   if (TheLeafs->NeuLandCluster_fSize == 0) {LandCluster_Errors = LandCluster_Errors + 1;}
   if (TheLeafs->NeuLandCluster_fMarked == 0) {LandCluster_Errors = LandCluster_Errors + 1;}
   if (TheLeafs->NeuLandCluster_fTopology == 0) {LandCluster_Errors = LandCluster_Errors + 1;}
   if (TheLeafs->NeuLandCluster_fEliminated == 0) {LandCluster_Errors = LandCluster_Errors + 1;}
   if (TheLeafs->NeuLandCluster_fDigibars == 0) {LandCluster_Errors = LandCluster_Errors + 1;}
   }
   
   }
   
   if (Gean&&Vet)
   {
       
   // -----------------------------------
   // VETO digitizer leafs:
   TheLeafs->veto_segDigi_fUniqueID = TheLeafs->VetoDigiTree->FindLeaf("veto_segDigi.fUniqueID");		// UInt_t[n] // This is an artifact of FairRoot. FairRoot is designed in such a way that each kind of leafs requires an fUniqueID and an fBits.
   TheLeafs->veto_segDigi_fBits = TheLeafs->VetoDigiTree->FindLeaf("veto_segDigi.fBits");			// UInt_t[n] // This is an artifact of FairRoot. FairRoot is designed in such a way that each kind of leafs requires an fUniqueID and an fBits.
															     // NOTE: The leaf size is the number of bars that fired in this particluar event! Information is given to me by Jan Mayer.
   TheLeafs->veto_segDigi_fPaddleNb = TheLeafs->VetoDigiTree->FindLeaf("veto_segDigi.fPaddleNb");		// Int_t[n] // Number of the bar from VETO that registered the hit.
   TheLeafs->veto_segDigi_fTdcL = TheLeafs->VetoDigiTree->FindLeaf("veto_segDigi.fTdcL");			// Double32_t[n] // TDC output of the first PMT as coming from TACQUILA [ns]
   TheLeafs->veto_segDigi_fTdcR = TheLeafs->VetoDigiTree->FindLeaf("veto_segDigi.fTdcR");			// Double32_t[n] // TDC output of the second PMT as coming from TACQUILA [ns]
   TheLeafs->veto_segDigi_fTdc = TheLeafs->VetoDigiTree->FindLeaf("veto_segDigi.fTdc");				// Double32_t[n] // Reconstructed time at the hit: (leftPMT.GetTDC() + rightPMT.GetTDC()) / 2. - fPaddleHalfLength / fcMedium; [ns]
																 // computed in DigitizationEngine.h un Paddle-structure
   TheLeafs->veto_segDigi_fQdcL = TheLeafs->VetoDigiTree->FindLeaf("veto_segDigi.fQdcL");			// Double32_t[n] // QDC output of the first PMT as coming from TACQUILA [MeV, but uncalibrated!]
   TheLeafs->veto_segDigi_fQdcR = TheLeafs->VetoDigiTree->FindLeaf("veto_segDigi.fQdcR");			// Double32_t[n] // QDC output of the second PMT as coming from TACQUILA [MeV, nut uncalibrated!]
   TheLeafs->veto_segDigi_fQdc = TheLeafs->VetoDigiTree->FindLeaf("veto_segDigi.fQdc");				// Double32_t[n] // Reconstructed energy deposition at the hit: It is computed as sqrt(leftPMT.GetEnergy() * rightPMT.GetEnergy()) [MeV, nut uncalibrated!]
																 // computed in DigitizationEngine.h un Paddle-structure
   TheLeafs->veto_segDigi_fxx = TheLeafs->VetoDigiTree->FindLeaf("veto_segDigi.fxx");				// Double32_t[n] // Hit X-coordinate detemined with TOF information from TDC: Computed in Paddle Structure as (rightPMT.GetTDC() - leftPMT.GetTDC()) / 2.*fcMedium; [cm]
   TheLeafs->veto_segDigi_fyy = TheLeafs->VetoDigiTree->FindLeaf("veto_segDigi.fyy");				// Double32_t[n] // Hit Y-coordinate detemined with TOF information from TDC: Computed in Paddle Structure as (rightPMT.GetTDC() - leftPMT.GetTDC()) / 2.*fcMedium; [cm]
   TheLeafs->veto_segDigi_fzz = TheLeafs->VetoDigiTree->FindLeaf("veto_segDigi.fzz");				// Double32_t[n] // Hit Z-coordinate detemined with TOF information from TDC: Computed in Paddle Structure as (rightPMT.GetTDC() - leftPMT.GetTDC()) / 2.*fcMedium; [cm]

   // Checking if the connection worked:
   if (TheLeafs->veto_segDigi_fUniqueID == 0) {VetoDigi_Errors = VetoDigi_Errors + 1;}
   if (TheLeafs->veto_segDigi_fBits == 0) {VetoDigi_Errors = VetoDigi_Errors + 1;}
   if (TheLeafs->veto_segDigi_fPaddleNb == 0) {VetoDigi_Errors = VetoDigi_Errors + 1;}
   if (TheLeafs->veto_segDigi_fTdcL == 0) {VetoDigi_Errors = VetoDigi_Errors + 1;}
   if (TheLeafs->veto_segDigi_fTdcR == 0) {VetoDigi_Errors = VetoDigi_Errors + 1;}
   if (TheLeafs->veto_segDigi_fTdc == 0) {VetoDigi_Errors = VetoDigi_Errors + 1;}
   if (TheLeafs->veto_segDigi_fQdcL == 0) {VetoDigi_Errors = VetoDigi_Errors + 1;}
   if (TheLeafs->veto_segDigi_fQdcR == 0) {VetoDigi_Errors = VetoDigi_Errors + 1;}
   if (TheLeafs->veto_segDigi_fQdc == 0) {VetoDigi_Errors = VetoDigi_Errors + 1;}
   if (TheLeafs->veto_segDigi_fxx == 0) {VetoDigi_Errors = VetoDigi_Errors + 1;}
   if (TheLeafs->veto_segDigi_fyy == 0) {VetoDigi_Errors = VetoDigi_Errors + 1;}
   if (TheLeafs->veto_segDigi_fzz == 0) {VetoDigi_Errors = VetoDigi_Errors + 1;}
   
   // check if it worked:
   if (VetoDigi_Errors==12)
   {
       VetoDigi_Errors = 0;
   
   TheLeafs->veto_segDigi_fUniqueID = TheLeafs->VetoDigiTree->FindLeaf("Nveto_segDigis.fUniqueID");
   TheLeafs->veto_segDigi_fBits = TheLeafs->VetoDigiTree->FindLeaf("Nveto_segDigis.fBits");		
   TheLeafs->veto_segDigi_fPaddleNb = TheLeafs->VetoDigiTree->FindLeaf("Nveto_segDigis.fPaddle");	
   TheLeafs->veto_segDigi_fTdcL = TheLeafs->VetoDigiTree->FindLeaf("Nveto_segDigis.fTdcL");		
   TheLeafs->veto_segDigi_fTdcR = TheLeafs->VetoDigiTree->FindLeaf("Nveto_segDigis.fTdcR");		
   TheLeafs->veto_segDigi_fTdc = TheLeafs->VetoDigiTree->FindLeaf("Nveto_segDigis.fT");			
   TheLeafs->veto_segDigi_fQdcL = TheLeafs->VetoDigiTree->FindLeaf("Nveto_segDigis.fQdcL");	
   TheLeafs->veto_segDigi_fQdcR = TheLeafs->VetoDigiTree->FindLeaf("Nveto_segDigis.fQdcR");	
   TheLeafs->veto_segDigi_fQdc = TheLeafs->VetoDigiTree->FindLeaf("Nveto_segDigis.fE");	
   TheLeafs->veto_segDigi_fxx = TheLeafs->VetoDigiTree->FindLeaf("Nveto_segDigis.fPosition.fX");		
   TheLeafs->veto_segDigi_fyy = TheLeafs->VetoDigiTree->FindLeaf("Nveto_segDigis.fPosition.fY");	
   TheLeafs->veto_segDigi_fzz = TheLeafs->VetoDigiTree->FindLeaf("Nveto_segDigis.fPosition.fZ");	

   // Checking if the connection worked:
   if (TheLeafs->veto_segDigi_fUniqueID == 0) {VetoDigi_Errors = VetoDigi_Errors + 1;}
   if (TheLeafs->veto_segDigi_fBits == 0) {VetoDigi_Errors = VetoDigi_Errors + 1;}
   if (TheLeafs->veto_segDigi_fPaddleNb == 0) {VetoDigi_Errors = VetoDigi_Errors + 1;}
   if (TheLeafs->veto_segDigi_fTdcL == 0) {VetoDigi_Errors = VetoDigi_Errors + 1;}
   if (TheLeafs->veto_segDigi_fTdcR == 0) {VetoDigi_Errors = VetoDigi_Errors + 1;}
   if (TheLeafs->veto_segDigi_fTdc == 0) {VetoDigi_Errors = VetoDigi_Errors + 1;}
   if (TheLeafs->veto_segDigi_fQdcL == 0) {VetoDigi_Errors = VetoDigi_Errors + 1;}
   if (TheLeafs->veto_segDigi_fQdcR == 0) {VetoDigi_Errors = VetoDigi_Errors + 1;}
   if (TheLeafs->veto_segDigi_fQdc == 0) {VetoDigi_Errors = VetoDigi_Errors + 1;}
   if (TheLeafs->veto_segDigi_fxx == 0) {VetoDigi_Errors = VetoDigi_Errors + 1;}
   if (TheLeafs->veto_segDigi_fyy == 0) {VetoDigi_Errors = VetoDigi_Errors + 1;}
   if (TheLeafs->veto_segDigi_fzz == 0) {VetoDigi_Errors = VetoDigi_Errors + 1;}
   }
   
   // -----------------------------------
   // VETO ClusterFinder leafs:
   TheLeafs->Nveto_segCluster_fUniqueID = TheLeafs->VetoDigiTree->FindLeaf("NVeto_segCluster.fUniqueID");	// UInt_t[n]
   TheLeafs->Nveto_segCluster_fBits = TheLeafs->VetoDigiTree->FindLeaf("NVeto_segCluster.fBits");		// UInt_t[n]
   TheLeafs->Nveto_segCluster_fStartX = TheLeafs->VetoDigiTree->FindLeaf("NVeto_segCluster.fStartX");		// Double32_t[n] // A Cluster contains multiple bars that all fired. These bars will have fired at different times. Hence the fStart-leafs are the time, X, Y and Z
   TheLeafs->Nveto_segCluster_fStartY = TheLeafs->VetoDigiTree->FindLeaf("NVeto_segCluster.fStartY");		// Double32_t[n] // coordinates of the first hit (= first bar) in that cluster. Likewise, you also have a bar that fired last. The fStop-leafs are the time,
   TheLeafs->Nveto_segCluster_fStartZ = TheLeafs->VetoDigiTree->FindLeaf("NVeto_segCluster.fStartZ");		// Double32_t[n] // X, Y and Z coordinates of this last bar (=hit) inside that cluster. These times and X, Y and Z coordinates are extracted from the 
   TheLeafs->Nveto_segCluster_fStartT = TheLeafs->VetoDigiTree->FindLeaf("NVeto_segCluster.fStartT");		// Double32_t[n] // physical location of each bar and the use of the TimeDifference and TOF measurements for each bar in the cluster. Of course the
   TheLeafs->Nveto_segCluster_fStopX = TheLeafs->VetoDigiTree->FindLeaf("NVeto_segCluster.fStopX");		// Double32_t[n] // physical location of a bar has a resolution equal to the BarWidth, while TimeDifference has a much better position resolution.
   TheLeafs->Nveto_segCluster_fStopY = TheLeafs->VetoDigiTree->FindLeaf("NVeto_segCluster.fStopY");		// Double32_t[n] // NOTE: leaf size is the number of clusters! Jan Mayer told me this information!
   TheLeafs->Nveto_segCluster_fStopZ = TheLeafs->VetoDigiTree->FindLeaf("NVeto_segCluster.fStopZ");		// Double32_t[n]
   TheLeafs->Nveto_segCluster_fStopT = TheLeafs->VetoDigiTree->FindLeaf("NVeto_segCluster.fStopT");		// Double32_t[n]
   TheLeafs->Nveto_segCluster_fE = TheLeafs->VetoDigiTree->FindLeaf("NVeto_segCluster.fE");			// Double32_t[n] // Total energy deposition in the full cluster. This information is computed from the veto_segDigi_fQdc Leaf, hence it is uncalibrated!
   TheLeafs->Nveto_segCluster_fSize = TheLeafs->VetoDigiTree->FindLeaf("NVeto_segCluster.fSize");		// Double32_t[n] // ?
   TheLeafs->Nveto_segCluster_fMarked = TheLeafs->VetoDigiTree->FindLeaf("NVeto_segCluster.fMarked");		// Bool_t[n] // ?
   
   // Checking if the connection worked:
   if (TheLeafs->Nveto_segCluster_fUniqueID == 0) {VetoCluster_Errors = VetoCluster_Errors + 1;}
   if (TheLeafs->Nveto_segCluster_fBits == 0) {VetoCluster_Errors = VetoCluster_Errors + 1;}
   if (TheLeafs->Nveto_segCluster_fStartX == 0) {VetoCluster_Errors = VetoCluster_Errors + 1;}
   if (TheLeafs->Nveto_segCluster_fStartY == 0) {VetoCluster_Errors = VetoCluster_Errors + 1;}
   if (TheLeafs->Nveto_segCluster_fStartZ == 0) {VetoCluster_Errors = VetoCluster_Errors + 1;}
   if (TheLeafs->Nveto_segCluster_fStartT == 0) {VetoCluster_Errors = VetoCluster_Errors + 1;}
   if (TheLeafs->Nveto_segCluster_fStopX == 0) {VetoCluster_Errors = VetoCluster_Errors + 1;}
   if (TheLeafs->Nveto_segCluster_fStopY == 0) {VetoCluster_Errors = VetoCluster_Errors + 1;}
   if (TheLeafs->Nveto_segCluster_fStopZ == 0) {VetoCluster_Errors = VetoCluster_Errors + 1;}
   if (TheLeafs->Nveto_segCluster_fStopT == 0) {VetoCluster_Errors = VetoCluster_Errors + 1;}
   if (TheLeafs->Nveto_segCluster_fE == 0) {VetoCluster_Errors = VetoCluster_Errors + 1;}
   if (TheLeafs->Nveto_segCluster_fSize == 0) {VetoCluster_Errors = VetoCluster_Errors + 1;}
   if (TheLeafs->Nveto_segCluster_fMarked == 0) {VetoCluster_Errors = VetoCluster_Errors + 1;}
   
   // retry if it did not work:
   if (VetoCluster_Errors==13)
   {
       VetoCluster_Errors = 0;
    
   TheLeafs->Nveto_segCluster_fUniqueID = TheLeafs->VetoDigiTree->FindLeaf("Nveto_segClusters.fUniqueID");
   TheLeafs->Nveto_segCluster_fBits = TheLeafs->VetoDigiTree->FindLeaf("Nveto_segClusters.fBits");	
   TheLeafs->Nveto_segCluster_fStartX = TheLeafs->VetoDigiTree->FindLeaf("Nveto_segClusters.fStartX");	
   TheLeafs->Nveto_segCluster_fStartY = TheLeafs->VetoDigiTree->FindLeaf("Nveto_segClusters.fStartY");	
   TheLeafs->Nveto_segCluster_fStartZ = TheLeafs->VetoDigiTree->FindLeaf("Nveto_segClusters.fStartZ");		
   TheLeafs->Nveto_segCluster_fStartT = TheLeafs->VetoDigiTree->FindLeaf("Nveto_segClusters.fStartT");	
   TheLeafs->Nveto_segCluster_fStopX = TheLeafs->VetoDigiTree->FindLeaf("Nveto_segClusters.fStopX");	
   TheLeafs->Nveto_segCluster_fStopY = TheLeafs->VetoDigiTree->FindLeaf("Nveto_segClusters.fStopY");		
   TheLeafs->Nveto_segCluster_fStopZ = TheLeafs->VetoDigiTree->FindLeaf("Nveto_segClusters.fStopZ");		
   TheLeafs->Nveto_segCluster_fStopT = TheLeafs->VetoDigiTree->FindLeaf("Nveto_segClusters.fStopT");		
   TheLeafs->Nveto_segCluster_fE = TheLeafs->VetoDigiTree->FindLeaf("Nveto_segClusters.fE");		
   TheLeafs->Nveto_segCluster_fSize = TheLeafs->VetoDigiTree->FindLeaf("Nveto_segClusters.fSize");		
   TheLeafs->Nveto_segCluster_fMarked = TheLeafs->VetoDigiTree->FindLeaf("Nveto_segClusters.fMarked");		
   
   // Checking if the connection worked:
   if (TheLeafs->Nveto_segCluster_fUniqueID == 0) {VetoCluster_Errors = VetoCluster_Errors + 1;}
   if (TheLeafs->Nveto_segCluster_fBits == 0) {VetoCluster_Errors = VetoCluster_Errors + 1;}
   if (TheLeafs->Nveto_segCluster_fStartX == 0) {VetoCluster_Errors = VetoCluster_Errors + 1;}
   if (TheLeafs->Nveto_segCluster_fStartY == 0) {VetoCluster_Errors = VetoCluster_Errors + 1;}
   if (TheLeafs->Nveto_segCluster_fStartZ == 0) {VetoCluster_Errors = VetoCluster_Errors + 1;}
   if (TheLeafs->Nveto_segCluster_fStartT == 0) {VetoCluster_Errors = VetoCluster_Errors + 1;}
   if (TheLeafs->Nveto_segCluster_fStopX == 0) {VetoCluster_Errors = VetoCluster_Errors + 1;}
   if (TheLeafs->Nveto_segCluster_fStopY == 0) {VetoCluster_Errors = VetoCluster_Errors + 1;}
   if (TheLeafs->Nveto_segCluster_fStopZ == 0) {VetoCluster_Errors = VetoCluster_Errors + 1;}
   if (TheLeafs->Nveto_segCluster_fStopT == 0) {VetoCluster_Errors = VetoCluster_Errors + 1;}
   if (TheLeafs->Nveto_segCluster_fE == 0) {VetoCluster_Errors = VetoCluster_Errors + 1;}
   if (TheLeafs->Nveto_segCluster_fSize == 0) {VetoCluster_Errors = VetoCluster_Errors + 1;}
   if (TheLeafs->Nveto_segCluster_fMarked == 0) {VetoCluster_Errors = VetoCluster_Errors + 1;}
   
   // NOTE: The new clusterfinder has a much harder structure, hence since we never use
   //       veto clusters anyway, just skip it:
   VetoCluster_Errors = 0;
   }
   
   }
   
   if (Track_test==0)
   {
     // Then we should connect to the Tracker-leafs:
     if (Neu_Ex==1)
     {
     TheLeafs->LandNeutHits_fUniqueID = TheLeafs->TrackTree->FindLeaf("LandNeutHits.fUniqueID");		// UInt_t[n]
     TheLeafs->LandNeutHits_fBits = TheLeafs->TrackTree->FindLeaf("LandNeutHits.fBits");			// UInt_t[n]
     TheLeafs->LandNeutHits_fX = TheLeafs->TrackTree->FindLeaf("LandNeutHits.fX");				// Double32_t[n] // X-coordinate [cm] of the fully reconstructed Neutron track starting point.
     TheLeafs->LandNeutHits_fY = TheLeafs->TrackTree->FindLeaf("LandNeutHits.fY");				// Double32_t[n] // Y-coordinate [cm] of the fully reconstructed Neutron track starting point.
     TheLeafs->LandNeutHits_fZ = TheLeafs->TrackTree->FindLeaf("LandNeutHits.fZ");				// Double32_t[n] // Z-coordinate [cm] of the fully reconstructed Neutron track starting point.
     TheLeafs->LandNeutHits_fT = TheLeafs->TrackTree->FindLeaf("LandNeutHits.fT");				// Double32_t[n] // Time Of Flight [ns] of the fully reconstructed Neutron track starting point.
     
     if (TheLeafs->LandNeutHits_fUniqueID == 0) {Tracker_Errors = Tracker_Errors + 1;}
     if (TheLeafs->LandNeutHits_fBits == 0) {Tracker_Errors = Tracker_Errors + 1;}
     if (TheLeafs->LandNeutHits_fX == 0) {Tracker_Errors = Tracker_Errors + 1;}
     if (TheLeafs->LandNeutHits_fY == 0) {Tracker_Errors = Tracker_Errors + 1;}
     if (TheLeafs->LandNeutHits_fZ == 0) {Tracker_Errors = Tracker_Errors + 1;}
     if (TheLeafs->LandNeutHits_fT == 0) {Tracker_Errors = Tracker_Errors + 1;}
     }
     
     if (Neu_Ex==2)
     {
     TheLeafs->LandNeutHits_fUniqueID = TheLeafs->TrackTree->FindLeaf("NeuLANDVertex.fUniqueID");		// UInt_t[n]
     TheLeafs->LandNeutHits_fBits = TheLeafs->TrackTree->FindLeaf("NeuLANDVertex.fBits");			// UInt_t[n]
     TheLeafs->LandNeutHits_fX = TheLeafs->TrackTree->FindLeaf("NeuLANDVertex.fReconstructed_X");		// Double32_t[n] // Reconstructed X-coordinate [cm] of the fully reconstructed Neutron track starting point.
     TheLeafs->LandNeutHits_fY = TheLeafs->TrackTree->FindLeaf("NeuLANDVertex.fReconstructed_Y");		// Double32_t[n] // Reconstructed Y-coordinate [cm] of the fully reconstructed Neutron track starting point.
     TheLeafs->LandNeutHits_fZ = TheLeafs->TrackTree->FindLeaf("NeuLANDVertex.fReconstructed_Z");		// Double32_t[n] // Reconstructed Z-coordinate [cm] of the fully reconstructed Neutron track starting point.
     TheLeafs->LandNeutHits_fT = TheLeafs->TrackTree->FindLeaf("NeuLANDVertex.fReconstructed_Time");		// Double32_t[n] // Reconstructed Time Of Flight [ns] of the fully reconstructed Neutron track starting point.
     TheLeafs->LandNeutHits_fE = TheLeafs->TrackTree->FindLeaf("NeuLANDVertex.fReconstructed_E");		// Double32_t[n] // Reconstructed Energy [MeV] of the fully reconstructed Neutron track starting point.
     TheLeafs->LandNeutHits_fMCX = TheLeafs->TrackTree->FindLeaf("NeuLANDVertex.fMC_X");			// Double32_t[n] // Corresponding Monte Carlo X-coordinate [cm] of the fully reconstructed Neutron track starting point.
     TheLeafs->LandNeutHits_fMCY = TheLeafs->TrackTree->FindLeaf("NeuLANDVertex.fMC_Y");			// Double32_t[n] // Corresponding Monte Carlo Y-coordinate [cm] of the fully reconstructed Neutron track starting point.
     TheLeafs->LandNeutHits_fMCZ = TheLeafs->TrackTree->FindLeaf("NeuLANDVertex.fMC_Z");			// Double32_t[n] // Corresponding Monte Carlo Z-coordinate [cm] of the fully reconstructed Neutron track starting point.
     TheLeafs->LandNeutHits_fMCT = TheLeafs->TrackTree->FindLeaf("NeuLANDVertex.fMC_Time");			// Double32_t[n] // Corresponding Monte Carlo Time Of Flight [ns] of the fully reconstructed Neutron track starting point.
     TheLeafs->LandNeutHits_fMCE = TheLeafs->TrackTree->FindLeaf("NeuLANDVertex.fMC_E");			// Double32_t[n] // Corresponding Monte Carlo Energy [MeV] of the fully reconstructed Neutron track starting point.
     TheLeafs->LandNeutHits_fDeltaR = TheLeafs->TrackTree->FindLeaf("NeuLANDVertex.fDeltaR");           	// Double32_t[n] // Spatial difference between MC point & Reconstructed point. [cm]
     TheLeafs->LandNeutHits_fDeltaT = TheLeafs->TrackTree->FindLeaf("NeuLANDVertex.fDeltaT");		        // Double32_t[n] // Time difference between MC point & Reconstructed point. [ns]
     TheLeafs->LandNeutHits_fHasMC = TheLeafs->TrackTree->FindLeaf("NeuLANDVertex.fHasMCpoint");		// Double32_t[n] // Whether the reconstructed point has been linked to an MC point yes/no.
     TheLeafs->LandNeutHits_fIsCompared = TheLeafs->TrackTree->FindLeaf("NeuLANDVertex.fHasBeenCompared");	// Double32_t[n] // Whether one has attempted to link the reconstructed point to an MC point yes/no.
     TheLeafs->LandNeutHits_fSucces = TheLeafs->TrackTree->FindLeaf("NeuLANDVertex.fEventSucces");		// Double32_t[n] // Whether the full event has been succesfully reconstructed yes/no.
     TheLeafs->LandNeutHits_fTrackID = TheLeafs->TrackTree->FindLeaf("NeuLANDVertex.fMC_TrackID");		// Double32_t[n] // TrackID of the corresponding Monte Carlo point.
     TheLeafs->LandNeutHits_fPDG = TheLeafs->TrackTree->FindLeaf("NeuLANDVertex.fMC_PDG");			// Double32_t[n] // PDG of the corresponding Monte Carlo point.
     
     
     if (TheLeafs->LandNeutHits_fUniqueID == 0) {Tracker_Errors = Tracker_Errors + 1;}
     if (TheLeafs->LandNeutHits_fBits == 0) {Tracker_Errors = Tracker_Errors + 1;}
     if (TheLeafs->LandNeutHits_fX == 0) {Tracker_Errors = Tracker_Errors + 1;}
     if (TheLeafs->LandNeutHits_fY == 0) {Tracker_Errors = Tracker_Errors + 1;}
     if (TheLeafs->LandNeutHits_fZ == 0) {Tracker_Errors = Tracker_Errors + 1;}
     if (TheLeafs->LandNeutHits_fT == 0) {Tracker_Errors = Tracker_Errors + 1;}
     if (TheLeafs->LandNeutHits_fE == 0) {Tracker_Errors = Tracker_Errors + 1;}
     if (TheLeafs->LandNeutHits_fMCX == 0) {Tracker_Errors = Tracker_Errors + 1;}
     if (TheLeafs->LandNeutHits_fMCY == 0) {Tracker_Errors = Tracker_Errors + 1;}
     if (TheLeafs->LandNeutHits_fMCZ == 0) {Tracker_Errors = Tracker_Errors + 1;}
     if (TheLeafs->LandNeutHits_fMCT == 0) {Tracker_Errors = Tracker_Errors + 1;}
     if (TheLeafs->LandNeutHits_fMCE == 0) {Tracker_Errors = Tracker_Errors + 1;}
     if (TheLeafs->LandNeutHits_fDeltaR == 0) {Tracker_Errors = Tracker_Errors + 1;}
     if (TheLeafs->LandNeutHits_fDeltaT == 0) {Tracker_Errors = Tracker_Errors + 1;}
     if (TheLeafs->LandNeutHits_fHasMC == 0) {Tracker_Errors = Tracker_Errors + 1;}
     if (TheLeafs->LandNeutHits_fIsCompared == 0) {Tracker_Errors = Tracker_Errors + 1;}
     if (TheLeafs->LandNeutHits_fSucces == 0) {Tracker_Errors = Tracker_Errors + 1;}
     if (TheLeafs->LandNeutHits_fTrackID == 0) {Tracker_Errors = Tracker_Errors + 1;}
     if (TheLeafs->LandNeutHits_fPDG == 0) {Tracker_Errors = Tracker_Errors + 1;}
     }
   }
   
   // Now continue with the other digitizer leafs:
   if (Gean&&(cal||star))
   {
   // General leafs:
   TheLeafs->EventHeader_TNamed_fUniqueID = TheLeafs->OtherTree->FindLeaf("EventHeader.TNamed.fUniqueID");
   TheLeafs->EventHeader_TNamed_fBits = TheLeafs->OtherTree->FindLeaf("EventHeader.TNamed.fBits");
   TheLeafs->EventHeader_TNamed_fName = TheLeafs->OtherTree->FindLeaf("EventHeader.TNamed.fName");
   TheLeafs->EventHeader_TNamed_fTitle = TheLeafs->OtherTree->FindLeaf("EventHeader.TNamed.fTitle");
   TheLeafs->EventHeader_fRunId = TheLeafs->OtherTree->FindLeaf("EventHeader.fRunId");
   TheLeafs->EventHeader_fEventTime = TheLeafs->OtherTree->FindLeaf("EventHeader.fEventTime");
   TheLeafs->EventHeader_fInputFileId = TheLeafs->OtherTree->FindLeaf("EventHeader.fInputFileId");
   TheLeafs->EventHeader_fMCEntryNo = TheLeafs->OtherTree->FindLeaf("EventHeader.fMCEntryNo");
   
   if (TheLeafs->EventHeader_TNamed_fUniqueID==0) {OtherGen_Errors = OtherGen_Errors + 1;}
   if (TheLeafs->EventHeader_TNamed_fBits==0) {OtherGen_Errors = OtherGen_Errors + 1;}
   if (TheLeafs->EventHeader_TNamed_fName==0) {OtherGen_Errors = OtherGen_Errors + 1;}
   if (TheLeafs->EventHeader_TNamed_fTitle==0) {OtherGen_Errors = OtherGen_Errors + 1;}
   if (TheLeafs->EventHeader_fRunId==0) {OtherGen_Errors = OtherGen_Errors + 1;}
   if (TheLeafs->EventHeader_fEventTime==0) {OtherGen_Errors = OtherGen_Errors + 1;}
   if (TheLeafs->EventHeader_fInputFileId==0) {OtherGen_Errors = OtherGen_Errors + 1;}
   if (TheLeafs->EventHeader_fMCEntryNo==0) {OtherGen_Errors = OtherGen_Errors + 1;}

   // califa-leafs:
   if (cal)
   {
       
   TheLeafs->CaloHitSim_fUniqueID = TheLeafs->OtherTree->FindLeaf("CaloHitSim.fUniqueID");
   TheLeafs->CaloHitSim_fBits = TheLeafs->OtherTree->FindLeaf("CaloHitSim.fBits");
   TheLeafs->CaloHitSim_fLinks = TheLeafs->OtherTree->FindLeaf("CaloHitSim.fLinks");
   TheLeafs->CaloHitSim_fEntryNr_fUniqueID = TheLeafs->OtherTree->FindLeaf("CaloHitSim.fEntryNr.fUniqueID");
   TheLeafs->CaloHitSim_fEntryNr_fBits = TheLeafs->OtherTree->FindLeaf("CaloHitSim.fEntryNr.fBits");
   TheLeafs->CaloHitSim_fEntryNr_fFile = TheLeafs->OtherTree->FindLeaf("CaloHitSim.fEntryNr.fFile");
   TheLeafs->CaloHitSim_fEntryNr_fEntry = TheLeafs->OtherTree->FindLeaf("CaloHitSim.fEntryNr.fEntry");
   TheLeafs->CaloHitSim_fEntryNr_fType = TheLeafs->OtherTree->FindLeaf("CaloHitSim.fEntryNr.fType");
   TheLeafs->CaloHitSim_fEntryNr_fIndex = TheLeafs->OtherTree->FindLeaf("CaloHitSim.fEntryNr.fIndex");
   TheLeafs->CaloHitSim_fEntryNr_fWeight = TheLeafs->OtherTree->FindLeaf("CaloHitSim.fEntryNr.fWeight");
   TheLeafs->CaloHitSim_fDefaultType = TheLeafs->OtherTree->FindLeaf("CaloHitSim.fDefaultType");
   TheLeafs->CaloHitSim_fNbOfCrystalHits = TheLeafs->OtherTree->FindLeaf("CaloHitSim.fNbOfCrystalHits");
   TheLeafs->CaloHitSim_fEnergy = TheLeafs->OtherTree->FindLeaf("CaloHitSim.fEnergy");
   TheLeafs->CaloHitSim_fNf = TheLeafs->OtherTree->FindLeaf("CaloHitSim.fNf");
   TheLeafs->CaloHitSim_fNs = TheLeafs->OtherTree->FindLeaf("CaloHitSim.fNs");
   TheLeafs->CaloHitSim_fTheta = TheLeafs->OtherTree->FindLeaf("CaloHitSim.fTheta");
   TheLeafs->CaloHitSim_fPhi = TheLeafs->OtherTree->FindLeaf("CaloHitSim.fPhi");
   TheLeafs->CaloHitSim_fTime = TheLeafs->OtherTree->FindLeaf("CaloHitSim.fTime");
   TheLeafs->CaloHitSim_fEinc = TheLeafs->OtherTree->FindLeaf("CaloHitSim.fEinc");
   
   if (TheLeafs->CaloHitSim_fUniqueID==0) {CALIFAhit_Errors = CALIFAhit_Errors + 1;}
   if (TheLeafs->CaloHitSim_fBits==0) {CALIFAhit_Errors = CALIFAhit_Errors + 1;}
   if (TheLeafs->CaloHitSim_fLinks==0) {CALIFAhit_Errors = CALIFAhit_Errors + 1;}
   if (TheLeafs->CaloHitSim_fEntryNr_fUniqueID==0) {CALIFAhit_Errors = CALIFAhit_Errors + 1;}
   if (TheLeafs->CaloHitSim_fEntryNr_fBits==0) {CALIFAhit_Errors = CALIFAhit_Errors + 1;}
   if (TheLeafs->CaloHitSim_fEntryNr_fFile==0) {CALIFAhit_Errors = CALIFAhit_Errors + 1;}
   if (TheLeafs->CaloHitSim_fEntryNr_fType==0) {CALIFAhit_Errors = CALIFAhit_Errors + 1;}
   if (TheLeafs->CaloHitSim_fEntryNr_fIndex==0) {CALIFAhit_Errors = CALIFAhit_Errors + 1;}
   if (TheLeafs->CaloHitSim_fEntryNr_fWeight==0) {CALIFAhit_Errors = CALIFAhit_Errors + 1;}
   if (TheLeafs->CaloHitSim_fEntryNr_fEntry==0) {CALIFAhit_Errors = CALIFAhit_Errors + 1;}
   if (TheLeafs->CaloHitSim_fDefaultType==0) {CALIFAhit_Errors = CALIFAhit_Errors + 1;}
   if (TheLeafs->CaloHitSim_fNbOfCrystalHits==0) {CALIFAhit_Errors = CALIFAhit_Errors + 1;}
   if (TheLeafs->CaloHitSim_fEnergy==0) {CALIFAhit_Errors = CALIFAhit_Errors + 1;}
   if (TheLeafs->CaloHitSim_fNf==0) {CALIFAhit_Errors = CALIFAhit_Errors + 1;}
   if (TheLeafs->CaloHitSim_fNs==0) {CALIFAhit_Errors = CALIFAhit_Errors + 1;}
   if (TheLeafs->CaloHitSim_fTheta==0) {CALIFAhit_Errors = CALIFAhit_Errors + 1;}
   if (TheLeafs->CaloHitSim_fPhi==0) {CALIFAhit_Errors = CALIFAhit_Errors + 1;}
   if (TheLeafs->CaloHitSim_fTime==0) {CALIFAhit_Errors = CALIFAhit_Errors + 1;}
   if (TheLeafs->CaloHitSim_fEinc==0) {CALIFAhit_Errors = CALIFAhit_Errors + 1;}
   
   }
   
   // STaRTrack hit leafs:
   
   if (star)
   {
       
   TheLeafs->STaRTrackerHit_fUniqueID = TheLeafs->OtherTree->FindLeaf("STaRTrackerHit.fUniqueID");
   TheLeafs->STaRTrackerHit_fBits = TheLeafs->OtherTree->FindLeaf("STaRTrackerHit.fBits");
   TheLeafs->STaRTrackerHit_fLinks = TheLeafs->OtherTree->FindLeaf("STaRTrackerHit.fLinks");
   TheLeafs->STaRTrackerHit_fEntryNr_fUniqueID = TheLeafs->OtherTree->FindLeaf("STaRTrackerHit.fEntryNr.fUniqueID");
   TheLeafs->STaRTrackerHit_fEntryNr_fBits = TheLeafs->OtherTree->FindLeaf("STaRTrackerHit.fEntryNr.fBits");
   TheLeafs->STaRTrackerHit_fEntryNr_fFile = TheLeafs->OtherTree->FindLeaf("STaRTrackerHit.fEntryNr.fFile");
   TheLeafs->STaRTrackerHit_fEntryNr_fEntry = TheLeafs->OtherTree->FindLeaf("STaRTrackerHit.fEntryNr.fEntry");
   TheLeafs->STaRTrackerHit_fEntryNr_fType = TheLeafs->OtherTree->FindLeaf("STaRTrackerHit.fEntryNr.fType");
   TheLeafs->STaRTrackerHit_fEntryNr_fIndex = TheLeafs->OtherTree->FindLeaf("STaRTrackerHit.fEntryNr.fIndex");
   TheLeafs->STaRTrackerHit_fEntryNr_fWeight = TheLeafs->OtherTree->FindLeaf("STaRTrackerHit.fEntryNr.fWeight");
   TheLeafs->STaRTrackerHit_fDefaultType = TheLeafs->OtherTree->FindLeaf("STaRTrackerHit.fDefaultType");
   TheLeafs->STaRTrackerHit_fEnergy = TheLeafs->OtherTree->FindLeaf("STaRTrackerHit.fEnergy");
   TheLeafs->STaRTrackerHit_fDetector = TheLeafs->OtherTree->FindLeaf("STaRTrackerHit.fDetector");
   TheLeafs->STaRTrackerHit_fXHit = TheLeafs->OtherTree->FindLeaf("STaRTrackerHit.fXHit");
   TheLeafs->STaRTrackerHit_fYHit = TheLeafs->OtherTree->FindLeaf("STaRTrackerHit.fYHit");
   TheLeafs->STaRTrackerHit_fZHit = TheLeafs->OtherTree->FindLeaf("STaRTrackerHit.fZHit");
   TheLeafs->STaRTrackerHit_fPx = TheLeafs->OtherTree->FindLeaf("STaRTrackerHit.fPx");
   TheLeafs->STaRTrackerHit_fPy = TheLeafs->OtherTree->FindLeaf("STaRTrackerHit.fPy");
   TheLeafs->STaRTrackerHit_fPz = TheLeafs->OtherTree->FindLeaf("STaRTrackerHit.fPz");
   TheLeafs->STaRTrackerHit_fThetaZero = TheLeafs->OtherTree->FindLeaf("STaRTrackerHit.fThetaZero");
   TheLeafs->STaRTrackerHit_fPhiZero = TheLeafs->OtherTree->FindLeaf("STaRTrackerHit.fPhiZero");
   
   if (TheLeafs->STaRTrackerHit_fUniqueID==0) {STaRTrackHits_Errors = STaRTrackHits_Errors + 1;}
   if (TheLeafs->STaRTrackerHit_fBits==0) {STaRTrackHits_Errors = STaRTrackHits_Errors + 1;}
   if (TheLeafs->STaRTrackerHit_fLinks==0) {STaRTrackHits_Errors = STaRTrackHits_Errors + 1;}
   if (TheLeafs->STaRTrackerHit_fEntryNr_fUniqueID==0) {STaRTrackHits_Errors = STaRTrackHits_Errors + 1;}
   if (TheLeafs->STaRTrackerHit_fEntryNr_fBits==0) {STaRTrackHits_Errors = STaRTrackHits_Errors + 1;}
   if (TheLeafs->STaRTrackerHit_fEntryNr_fFile==0) {STaRTrackHits_Errors = STaRTrackHits_Errors + 1;}
   if (TheLeafs->STaRTrackerHit_fEntryNr_fEntry==0) {STaRTrackHits_Errors = STaRTrackHits_Errors + 1;}
   if (TheLeafs->STaRTrackerHit_fEntryNr_fType==0) {STaRTrackHits_Errors = STaRTrackHits_Errors + 1;}
   if (TheLeafs->STaRTrackerHit_fEntryNr_fIndex==0) {STaRTrackHits_Errors = STaRTrackHits_Errors + 1;}
   if (TheLeafs->STaRTrackerHit_fEntryNr_fWeight==0) {STaRTrackHits_Errors = STaRTrackHits_Errors + 1;}
   if (TheLeafs->STaRTrackerHit_fDefaultType==0) {STaRTrackHits_Errors = STaRTrackHits_Errors + 1;}
   if (TheLeafs->STaRTrackerHit_fEnergy==0) {STaRTrackHits_Errors = STaRTrackHits_Errors + 1;}
   if (TheLeafs->STaRTrackerHit_fDetector==0) {STaRTrackHits_Errors = STaRTrackHits_Errors + 1;}
   if (TheLeafs->STaRTrackerHit_fXHit==0) {STaRTrackHits_Errors = STaRTrackHits_Errors + 1;}
   if (TheLeafs->STaRTrackerHit_fYHit==0) {STaRTrackHits_Errors = STaRTrackHits_Errors + 1;}
   if (TheLeafs->STaRTrackerHit_fZHit==0) {STaRTrackHits_Errors = STaRTrackHits_Errors + 1;}
   if (TheLeafs->STaRTrackerHit_fPx==0) {STaRTrackHits_Errors = STaRTrackHits_Errors + 1;}
   if (TheLeafs->STaRTrackerHit_fPy==0) {STaRTrackHits_Errors = STaRTrackHits_Errors + 1;}
   if (TheLeafs->STaRTrackerHit_fPz==0) {STaRTrackHits_Errors = STaRTrackHits_Errors + 1;}
   if (TheLeafs->STaRTrackerHit_fThetaZero==0) {STaRTrackHits_Errors = STaRTrackHits_Errors + 1;}
   if (TheLeafs->STaRTrackerHit_fPhiZero==0) {STaRTrackHits_Errors = STaRTrackHits_Errors + 1;}
   
   // STaRTrack Digitizer leafs:
   TheLeafs->STaRTrackerDigitHit_fUniqueID = TheLeafs->OtherTree->FindLeaf("STaRTrackerDigitHit.fUniqueID");
   TheLeafs->STaRTrackerDigitHit_fBits = TheLeafs->OtherTree->FindLeaf("STaRTrackerDigitHit.fBits");
   TheLeafs->STaRTrackerDigitHit_fLinks = TheLeafs->OtherTree->FindLeaf("STaRTrackerDigitHit.fLinks");
   TheLeafs->STaRTrackerDigitHit_fEntryNr_fUniqueID = TheLeafs->OtherTree->FindLeaf("STaRTrackerDigitHit.fEntryNr.fUniqueID");
   TheLeafs->STaRTrackerDigitHit_fEntryNr_fBits = TheLeafs->OtherTree->FindLeaf("STaRTrackerDigitHit.fEntryNr.fBits");
   TheLeafs->STaRTrackerDigitHit_fEntryNr_fFile = TheLeafs->OtherTree->FindLeaf("STaRTrackerDigitHit.fEntryNr.fFile");
   TheLeafs->STaRTrackerDigitHit_fEntryNr_fEntry = TheLeafs->OtherTree->FindLeaf("STaRTrackerDigitHit.fEntryNr.fEntry");
   TheLeafs->STaRTrackerDigitHit_fEntryNr_fType = TheLeafs->OtherTree->FindLeaf("STaRTrackerDigitHit.fEntryNr.fType");
   TheLeafs->STaRTrackerDigitHit_fEntryNr_fIndex = TheLeafs->OtherTree->FindLeaf("STaRTrackerDigitHit.fEntryNr.fIndex");
   TheLeafs->STaRTrackerDigitHit_fEntryNr_fWeight = TheLeafs->OtherTree->FindLeaf("STaRTrackerDigitHit.fEntryNr.fWeight");
   TheLeafs->STaRTrackerDigitHit_fDefaultType = TheLeafs->OtherTree->FindLeaf("STaRTrackerDigitHit.fDefaultType");
   TheLeafs->STaRTrackerDigitHit_fEnergy = TheLeafs->OtherTree->FindLeaf("STaRTrackerDigitHit.fEnergy");
   TheLeafs->STaRTrackerDigitHit_fTime = TheLeafs->OtherTree->FindLeaf("STaRTrackerDigitHit.fTime");
   TheLeafs->STaRTrackerDigitHit_fDetector = TheLeafs->OtherTree->FindLeaf("STaRTrackerDigitHit.fDetector");
   TheLeafs->STaRTrackerDigitHit_fChip = TheLeafs->OtherTree->FindLeaf("STaRTrackerDigitHit.fChip");
   TheLeafs->STaRTrackerDigitHit_fSide = TheLeafs->OtherTree->FindLeaf("STaRTrackerDigitHit.fSide");
   TheLeafs->STaRTrackerDigitHit_fStrip = TheLeafs->OtherTree->FindLeaf("STaRTrackerDigitHit.fStrip");
   
   if (TheLeafs->STaRTrackerDigitHit_fUniqueID==0) {STaRTrackDigis_Errors = STaRTrackDigis_Errors + 1;}
   if (TheLeafs->STaRTrackerDigitHit_fBits==0) {STaRTrackDigis_Errors = STaRTrackDigis_Errors + 1;}
   if (TheLeafs->STaRTrackerDigitHit_fLinks==0) {STaRTrackDigis_Errors = STaRTrackDigis_Errors + 1;}
   if (TheLeafs->STaRTrackerDigitHit_fEntryNr_fUniqueID==0) {STaRTrackDigis_Errors = STaRTrackDigis_Errors + 1;}
   if (TheLeafs->STaRTrackerDigitHit_fEntryNr_fBits==0) {STaRTrackDigis_Errors = STaRTrackDigis_Errors + 1;}
   if (TheLeafs->STaRTrackerDigitHit_fEntryNr_fFile==0) {STaRTrackDigis_Errors = STaRTrackDigis_Errors + 1;}
   if (TheLeafs->STaRTrackerDigitHit_fEntryNr_fEntry==0) {STaRTrackDigis_Errors = STaRTrackDigis_Errors + 1;}
   if (TheLeafs->STaRTrackerDigitHit_fEntryNr_fType==0) {STaRTrackDigis_Errors = STaRTrackDigis_Errors + 1;}
   if (TheLeafs->STaRTrackerDigitHit_fEntryNr_fIndex==0) {STaRTrackDigis_Errors = STaRTrackDigis_Errors + 1;}
   if (TheLeafs->STaRTrackerDigitHit_fEntryNr_fWeight==0) {STaRTrackDigis_Errors = STaRTrackDigis_Errors + 1;}
   if (TheLeafs->STaRTrackerDigitHit_fDefaultType==0) {STaRTrackDigis_Errors = STaRTrackDigis_Errors + 1;}
   if (TheLeafs->STaRTrackerDigitHit_fEnergy==0) {STaRTrackDigis_Errors = STaRTrackDigis_Errors + 1;}
   if (TheLeafs->STaRTrackerDigitHit_fTime==0) {STaRTrackDigis_Errors = STaRTrackDigis_Errors + 1;}
   if (TheLeafs->STaRTrackerDigitHit_fDetector==0) {STaRTrackDigis_Errors = STaRTrackDigis_Errors + 1;}
   if (TheLeafs->STaRTrackerDigitHit_fChip==0) {STaRTrackDigis_Errors = STaRTrackDigis_Errors + 1;}
   if (TheLeafs->STaRTrackerDigitHit_fSide==0) {STaRTrackDigis_Errors = STaRTrackDigis_Errors + 1;}
   if (TheLeafs->STaRTrackerDigitHit_fStrip==0) {STaRTrackDigis_Errors = STaRTrackDigis_Errors + 1;}

   }
       
   }
   
   // STaRTrack track leafs:
   if (Gean&&star)
   {
   
   TheLeafs->TrackSize = TheLeafs->StarTree->FindLeaf("TrackSize");
   TheLeafs->Xtrack = TheLeafs->StarTree->FindLeaf("Xtrack");
   TheLeafs->Ytrack = TheLeafs->StarTree->FindLeaf("Ytrack");
   TheLeafs->Ztrack = TheLeafs->StarTree->FindLeaf("Ztrack");
   TheLeafs->ThetaTrack = TheLeafs->StarTree->FindLeaf("ThetaTrack");
   TheLeafs->PhiTrack = TheLeafs->StarTree->FindLeaf("PhiTrack");
   TheLeafs->ElossTrack = TheLeafs->StarTree->FindLeaf("ElossTrack");
   TheLeafs->NumberOfSolvedTracks = TheLeafs->StarTree->FindLeaf("NumberOfSolvedTracks");
   TheLeafs->NumberOfGoodTracks = TheLeafs->StarTree->FindLeaf("NumberOfGoodTracks");
   TheLeafs->MiddleMult = TheLeafs->StarTree->FindLeaf("MiddleMult");
   TheLeafs->theta = TheLeafs->StarTree->FindLeaf("theta");
   TheLeafs->phi = TheLeafs->StarTree->FindLeaf("phi");
   TheLeafs->Ep = TheLeafs->StarTree->FindLeaf("Ep");
   
   if (TheLeafs->TrackSize==0) {STaRTrackTracks_Errors = STaRTrackTracks_Errors + 1;}
   if (TheLeafs->Xtrack==0) {STaRTrackTracks_Errors = STaRTrackTracks_Errors + 1;}
   if (TheLeafs->Ytrack==0) {STaRTrackTracks_Errors = STaRTrackTracks_Errors + 1;}
   if (TheLeafs->Ztrack==0) {STaRTrackTracks_Errors = STaRTrackTracks_Errors + 1;}
   if (TheLeafs->ThetaTrack==0) {STaRTrackTracks_Errors = STaRTrackTracks_Errors + 1;}
   if (TheLeafs->PhiTrack==0) {STaRTrackTracks_Errors = STaRTrackTracks_Errors + 1;}
   if (TheLeafs->ElossTrack==0) {STaRTrackTracks_Errors = STaRTrackTracks_Errors + 1;}
   if (TheLeafs->NumberOfSolvedTracks==0) {STaRTrackTracks_Errors = STaRTrackTracks_Errors + 1;}
   if (TheLeafs->NumberOfGoodTracks==0) {STaRTrackTracks_Errors = STaRTrackTracks_Errors + 1;}
   if (TheLeafs->MiddleMult==0) {STaRTrackTracks_Errors = STaRTrackTracks_Errors + 1;}
   if (TheLeafs->theta==0) {STaRTrackTracks_Errors = STaRTrackTracks_Errors + 1;}
   if (TheLeafs->phi==0) {STaRTrackTracks_Errors = STaRTrackTracks_Errors + 1;}
   if (TheLeafs->Ep==0) {STaRTrackTracks_Errors = STaRTrackTracks_Errors + 1;}
   
   }
   
   // ===============================================================================================
   // Now that finally the connection to the leafs has been performed and checked,
   // we can process the errors and give a report.
   
   // We begin with setting the errors to zero of detectors that aren't there. After all, if a detector
   // is left out of the setup, then obviously all leaf pointers will return zero and the errors will
   // be equal to the amount of leafs. This has to be circumvented.
   
   // So check all detectors one by one:
   Int_t some_int = 0;
   
   // TARGET:
   some_int = Integers->GetBinContent(206);
   if (some_int==1) 
   {
     TheLeafs->TARGET_exists = kTRUE;
   }
   else
   {
     TheLeafs->TARGET_exists = kFALSE;
   }
   
   // STaRTrack:
   some_int = Integers->GetBinContent(207);
   if (some_int==1) 
   {
     TheLeafs->STaRTrack_exists = kTRUE;
   }
   else
   {
     TheLeafs->STaRTrack_exists = kFALSE;
     STaRTRack_Errors = 0;
     STaRTrackDigis_Errors = 0;
     STaRTrackHits_Errors = 0;
     STaRTrackTracks_Errors = 0;
   }
   
   // PSP:
   some_int = Integers->GetBinContent(208);
   if (some_int==1) 
   {
     TheLeafs->PSP_exists = kTRUE;
   }
   else
   {
     TheLeafs->PSP_exists = kFALSE;
     PSP_Errors = 0;
   }
   
   // Vacuum Vessel:
   some_int = Integers->GetBinContent(209);
   if (some_int==1) 
   {
     TheLeafs->VACVESSEL_exists = kTRUE;
   }
   else
   {
     TheLeafs->VACVESSEL_exists = kFALSE;
   }
   
   // CALIFA:
   some_int = Integers->GetBinContent(210);
   if (some_int==1) 
   {
     TheLeafs->CALIFA_exists = kTRUE;
   }
   else
   {
     TheLeafs->CALIFA_exists = kFALSE;
     CALIFA_Errors = 0;
     CALIFAhit_Errors = 0;
   }
   
   // Crystal Ball:
   some_int = Integers->GetBinContent(256);
   if (some_int==1) 
   {
     TheLeafs->CrystalBall_exists = kTRUE;
   }
   else
   {
     TheLeafs->CrystalBall_exists = kFALSE;
     CrystalBall_Errors = 0;
   }
   
   // mTOF:
   some_int = Integers->GetBinContent(211);
   if (some_int==1) 
   {
     TheLeafs->mTOF_exists = kTRUE;
   }
   else
   {
     TheLeafs->mTOF_exists = kFALSE;
     mTOF_Errors = 0;
   }
   
   // TOF wall:
   some_int = Integers->GetBinContent(212);
   if (some_int==1) 
   {
     TheLeafs->TOF_exists = kTRUE;
   }
   else
   {
     TheLeafs->TOF_exists = kFALSE;
     TOF_Errors = 0;
   }
   
   // MFI:
   some_int = Integers->GetBinContent(213);
   if (some_int==1) 
   {
     TheLeafs->MFI_exists = kTRUE;
   }
   else
   {
     TheLeafs->MFI_exists = kFALSE;
     MFI_Errors = 0;
   }
   
   // DCH:
   some_int = Integers->GetBinContent(214);
   if (some_int==1) 
   {
     TheLeafs->DCH_exists = kTRUE;
   }
   else
   {
     TheLeafs->DCH_exists = kFALSE;
     DCH_Errors = 0;
   }
   
   // GLAD:
   some_int = Integers->GetBinContent(215);
   if (some_int==1) 
   {
     TheLeafs->GLAD_exists = kTRUE;
   }
   else
   {
     TheLeafs->GLAD_exists = kFALSE;
   }
   
   // ALADIN:
   some_int = Integers->GetBinContent(254);
   if (some_int==1) 
   {
     TheLeafs->ALADIN_exists = kTRUE;
   }
   else
   {
     TheLeafs->ALADIN_exists = kFALSE;
   }
   
   // GFI:
   some_int = Integers->GetBinContent(255);
   if (some_int==1) 
   {
     TheLeafs->ALADIN_exists = kTRUE;
   }
   else
   {
     TheLeafs->ALADIN_exists = kFALSE;
     GFI_Errors = 0;
   }
   
   // Vacuum Chamber:
   some_int = Integers->GetBinContent(253);
   if (some_int==1) 
   {
     TheLeafs->VacChamber_exists = kTRUE;
   }
   else
   {
     TheLeafs->VacChamber_exists = kFALSE;
   }
   
   // NeuLAND:
   some_int = Integers->GetBinContent(216);
   if ((some_int==1)||(some_int==2)||(Integers->GetBinContent(230)==1)) // If NEBULA is there but NeuLAND not, we should still have the leafs!
   {
     TheLeafs->NeuLAND_exists = kTRUE;
   }
   else
   {
     TheLeafs->NeuLAND_exists = kFALSE;
     LandPoint_Errors = 0;
     LFH_Errors = 0;
   }
   
   // VETO:
   some_int = Integers->GetBinContent(217);
   if ((some_int==1)||(some_int==2)||(Integers->GetBinContent(231)==1)) // If NEBULA VETO is there but ordinary VETO not, we should still have the leafs! 
   {
     TheLeafs->VETO_exists = kTRUE;
   }
   else
   {
     TheLeafs->VETO_exists = kFALSE;
     VetoPoint_Errors = 0;
     VFH_Errors = 0;
   }
   
   // Neutron Tracker:
   if (Track_test==0) {TheLeafs->Tracker_Exists = kTRUE;} else {TheLeafs->Tracker_Exists = kFALSE;}
   
   if (!((Integers->GetBinContent(207)==1)||(Integers->GetBinContent(210)==1))) {OtherGen_Errors = 0;}
   
   // ========================================================================================================
   // Now that we succesfully determined which detectors exist and which do not, we can proceed with the
   // error report.
   
   // Check all errors:
   Bool_t MC = (MC_Errors == 0);
   Bool_t CALIFA = (CALIFA_Errors == 0);
   Bool_t XBall = (CrystalBall_Errors == 0);
   Bool_t CALIFAhit = (CALIFAhit_Errors == 0);
   Bool_t STaRTrack = (STaRTRack_Errors == 0);
   Bool_t STaRTrackh = (STaRTrackHits_Errors == 0);
   Bool_t STaRTrackd = (STaRTrackDigis_Errors == 0);
   Bool_t STaRTrackt = (STaRTrackTracks_Errors == 0);
   Bool_t GFI = (GFI_Errors == 0);
   Bool_t DCH = (DCH_Errors == 0);
   Bool_t TOF = (TOF_Errors == 0);
   Bool_t mTOF = (mTOF_Errors == 0);
   Bool_t LP = (LandPoint_Errors == 0);
   Bool_t LFH = (LFH_Errors == 0);
   Bool_t NLP = (NeuPrimIntPoint_Errors == 0);
   Bool_t VP = (VetoPoint_Errors == 0);
   Bool_t VFH = (VFH_Errors == 0);
   Bool_t VLP = (VetoPrimIntPoint_Errors == 0);
   Bool_t MFI = (MFI_Errors == 0);
   Bool_t PSP = (PSP_Errors == 0);
   Bool_t MCEvent = (EventHeader_Errors == 0);
   Bool_t DigiEvent = (OtherGen_Errors == 0);
   Bool_t Geo = (GeoTracks_Errors == 0);
   Bool_t Ldigi = (LandDigi_Errors == 0);
   Bool_t Lclust = (LandCluster_Errors == 0);
   Bool_t Vdigi = (VetoDigi_Errors == 0);
   Bool_t Vclust = (VetoCluster_Errors == 0);
   Bool_t Ltrack = (Tracker_Errors == 0);
   
   // Make the total boolian:
   Bool_t total = XBall&&GFI&&MC&&CALIFA&&CALIFAhit&&STaRTrack&&STaRTrackh&&STaRTrackd&&STaRTrackt&&DCH&&TOF&&mTOF&&LP&&LFH&&VP&&VFH&&MFI&&PSP&&MCEvent&&Geo&&Ldigi&&Lclust&&Vdigi&&Vclust&&Ltrack&&VLP&&NLP&&DigiEvent;
   
   // And now we write the error report:
   if (total)
   {
     Connection = kTRUE;
     cout << "Connection to all leafs was succesful.\n\n";
   }
   else
   {
     cout << "### ERROR: Not all leafs were connected! The results of our connection check are:\n\n";
     
     cout << "MONTE CARLO connection failures = " << MC_Errors << "\n"
          << "EVENTHEADER connection failures = " << EventHeader_Errors << "\n"
	  << "GEOTRACKS   connection failures = " << GeoTracks_Errors << "\n";
	  
     if (TheLeafs->TARGET_exists)
     {
       cout << "TARGET detector exists. There are no corresponding leafs in the tree.\n";
     }
     else
     {
       cout << "TARGET detector does not exist.\n";
     }
     
     if (TheLeafs->STaRTrack_exists)
     {
       cout << "STaRTrack detector exists. Number of STaRTrack Monte Carlo failures = " << STaRTRack_Errors << "\n";
       cout << "                           Number of STaRTrack Hit failures = " << STaRTrackHits_Errors << "\n"
	    << "                           Number of STaRTrack Digitizer failures = " << STaRTrackDigis_Errors << "\n"
	    << "                           Number of STaRTrack Tracker failures = " << STaRTrackTracks_Errors << "\n";
     }
     else
     {
       cout << "STaRTrack detector does not exist.\n";
     }
     
     if (TheLeafs->PSP_exists)
     {
       cout << "PSP detector exists. Number of connection failures = " << PSP_Errors << "\n";
     }
     else
     {
       cout << "PSP detector does not exist.\n";
     }
     
     if (TheLeafs->VACVESSEL_exists)
     {
       cout << "VACUUM VESSEL detector exists. There are no corresponding leafs in the tree.\n";
     }
     else
     {
       cout << "VACUUM VESSEL detector does not exist.\n";
     }
     
     if (TheLeafs->CALIFA_exists)
     {
       cout << "CALIFA detector exists. Number of Monte Carlo failures = " << CALIFA_Errors << "\n";
       cout << "                        Number of Hitfinder failures = " << CALIFAhit_Errors << "\n";
     }
     else
     {
       cout << "CALIFA detector does not exist.\n";
     }
     
     if (TheLeafs->CrystalBall_exists)
     {
       cout << "Crystal Ball detector exists. Number of connection failures = " << CrystalBall_Errors << "\n";
     }
     else
     {
       cout << "Crystal Ball detector does not exist.\n";
     }
     
     if (TheLeafs->TOF_exists)
     {
       cout << "TOF wall detector exists. Number of connection failures = " << TOF_Errors << "\n";
     }
     else
     {
       cout << "TOF wall detector does not exist.\n";
     }
     
     if (TheLeafs->mTOF_exists)
     {
       cout << "mTOF detector exists. Number of connection failures = " << mTOF_Errors << "\n";
     }
     else
     {
       cout << "mTOF detector does not exist.\n";
     }
     
     if (TheLeafs->MFI_exists)
     {
       cout << "MFI detector exists. Number of connection failures = " << MFI_Errors << "\n";
     }
     else
     {
       cout << "MFI detector does not exist.\n";
     }
     
     if (TheLeafs->DCH_exists)
     {
       cout << "DCH detector exists. Number of connection failures = " << DCH_Errors << "\n";
     }
     else
     {
       cout << "DCH detector does not exist.\n";
     }
     
     if (TheLeafs->GLAD_exists)
     {
       cout << "GLAD magnet exists. There are no corresponding leafs in the tree.\n";
     }
     else
     {
       cout << "GLAD magnet does not exist.\n";
     }
     
     if (TheLeafs->VacChamber_exists)
     {
       cout << "Vacuum Scattering Chamber exists. There are no corresponding leafs in the tree.\n";
     }
     else
     {
       cout << "Vacuum Scattering Chamber does not exist.\n";
     }
     
     if (TheLeafs->ALADIN_exists)
     {
       cout << "ALADIN magnet exists. There are no corresponding leafs in the tree.\n";
     }
     else
     {
       cout << "ALADIN magnet does not exist.\n";
     }
     
     if (TheLeafs->GFI_exists)
     {
       cout << "GFI detector exists. Number of connection failures = " << GFI_Errors << "\n";
     }
     else
     {
       cout << "GFI detector does not exist.\n";
     }
     
     if (TheLeafs->NeuLAND_exists)
     {
       cout << "NeuLAND detector exists. Number of LandPoint failures = " << LandPoint_Errors << "\n";
       if (Neu_Ex==1) {cout << "                         Number of LFH failures = " << LFH_Errors << "\n";}
       else if (Neu_Ex==2) {cout << "                         Number of NeuPrimInt failures = " << NeuPrimIntPoint_Errors << "\n";}
       else {cout << "                         ### ERROR: No NeuLAND option was selected!!!\n";}
       if (TheLeafs->Geant3)
       {
       cout << "                         Number of NeuLAND Digitizer failures = " << LandDigi_Errors << "\n"
	    << "                         Number of NeuLAND ClusterFinder failures = " << LandCluster_Errors << "\n"
	    << "                         Number of NeuLAND Neutron Tracker failures = " << Tracker_Errors << "\n";
       }
       else
       {
       cout << "                         NeuLAND Digitizer does not exist.\n";
       }
     }
     else
     {
       cout << "NeuLAND detector does not exist.\n";
     }
     
     if (TheLeafs->VETO_exists)
     {
       cout << "VETO detector exists. Number of veto_segPoint failures = " << VetoPoint_Errors << "\n";
       if (VETO_Ex==1) {cout << "                      Number of VFH failures = " << VFH_Errors << "\n";}
       else if (VETO_Ex==2) {cout << "                         Number of VetoPrimInt failures = " << VetoPrimIntPoint_Errors << "\n";}
       else {cout << "                         ### ERROR: No VETO option was selected!!!\n";}
       if (TheLeafs->Geant3)
       {
       cout << "                      Number of VETO Digitizer failures = " << VetoDigi_Errors << "\n"
	    << "                      Number of VETO ClusterFinder failures = " << VetoCluster_Errors << "\n";
       }
       else
       {
       cout << "                      VETO Digitizer does not exist.\n";
       }
     }
     else
     {
       cout << "VETO detector does not exist.\n";
     }
     
     if ((TheLeafs->CALIFA_exists)||(TheLeafs->STaRTrack_exists))
     {
         cout << "General Digitizer leaf failures = " << OtherGen_Errors << "\n";
     }
     
     cout << "\nRe-check your code to make sure all failures are removed!\n\n";
     
     // =========================================================================================================
     // Now the error report has been written. We also properly defined the Connection boolian and we defined 
     // all the leaf connections and whether a detector exists or not. hence we are done!
     
     // Hence close all blocks:
   }
   }
   }
   }
   
   // Now return the boolian:
   return Connection;

   // Done!   
   
}




