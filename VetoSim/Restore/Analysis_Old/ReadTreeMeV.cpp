#include <iostream>

#include "TROOT.h"
#include "TMath.h"
#include "TRandom3.h"

#include "TH1I.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TH3D.h"

#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TLeaf.h"

#include "TString.h"
#include "TObjString.h"

using namespace std;

void ReadTreeMeV()
{
  // This macro reads the cbmsim-tree inside the root-file of the NeuLAND simulation.
  // Inputs are taken from the inputs-file, so the function has no inputs.
  // outputs are not there either.
  
  // Written by C. A. Douma.
  
  // ===========================================================================================
  // We begin with loading the inputs we need from the Inputs.root-file.
  
  // Load the file:
  TFile* file = new TFile("Inputs.root","read");
  
  // Load the data:
  TH1D* Doubles = (TH1D*) file->Get("Doubles");
  TH1I* Integers = (TH1I*) file->Get("Integers");
  
  // Load the strings we need:
  TObjString* Objpath = (TObjString*) file->Get("path");
  TObjString* ObjOutFile = (TObjString*) file->Get("file");
  TObjString* ObjParFile = (TObjString*) file->Get("parfile");
  TObjString* Objversion = (TObjString*) file->Get("version");
  TObjString* ObjLconstructor = (TObjString*) file->Get("Lconstructor");
  TObjString* ObjVconstructor = (TObjString*) file->Get("Vconstructor");
  
  TString path = Objpath->GetString();
  TString OutFile = ObjOutFile->GetString();
  TString ParFile = ObjParFile->GetString();
  TString version = Objversion->GetString();
  TString Lconstructor = ObjLconstructor->GetString();	
  TString Vconstructor = ObjVconstructor->GetString();
  
  // Check for the proper constructors:
  Bool_t constructor_test = kTRUE;
  if ((Lconstructor != "LAND")&&(Lconstructor != "NeuLAND")) {constructor_test = kFALSE;}
  if ((Vconstructor != "veto_seg")&&(Vconstructor != "Nveto_seg")) {constructor_test = kFALSE;}
  if (constructor_test == kFALSE)
  {
    cout << "#######################################################################################################################################################\n";
    cout << "################################# Bad specification of detector-constructors. Correct this in Inputs_Initialize.cpp ###################################\n";
    cout << "#######################################################################################################################################################\n";
  }
  else
  { // Carry out the macro only in this case!
  
  // =============================================================================================
  // Then we connect to the tree inside the root-file with the simulation results.
  
  // We begin with resetting root:
  gROOT->Reset();
  
  // Then we load the file with the simulation results:
  TString simresult = path + OutFile;
  TFile* simfile = new TFile(simresult,"read");
  
  // Then we connect to the tree named cbmsim inside:
  TTree* cbmsim; // create empty pointer
  simfile->GetObject("cbmsim",cbmsim); // connect the pointer to the tree.
  
  // the name of the pointer should be cbmtree, because this is what is used in the branch adresses.
  
  // Now we need a little safety-check whether the connection to the tree has been succesful or not:
  if (cbmsim == 0)
  {
    cout << "Connection to the tree failed! recheck your code!!!!!!!!!!!!!!!!!!!!!\n\n\n\n";
  }
  else
  {
   cout << "Connection to the tree was succesful. We will now connect to the Leafs.\n\n";
    
   // =============================================================================================
   // Now a lot of leafs are C++ arrays instead of simple variables. Hence we first must determine
   // the size of these arrays before we can perform the declaration of leafs and set the branch
   // adresses.
    
   // If the macro ReadLeafSize.cpp has been executed before this macro is used, then these size
   // are stored inside the Integers-array. It is however not very straight-forward to get these
   // numbers out correctly. This is not because they are hard to access; they are in Integers.
   // This is because they absolutely have to be constant if they are used to declare array lengths.
   
   // So first get Integers from the histograms:
   Int_t LandPoint_ArraySize_ii = 0;
   Int_t LandFirstHits_ArraySize_ii = 0;
   Int_t NeuLandPoint_ArraySize_ii = 0;
   Int_t veto_segPoint_ArraySize_ii = 0;
   Int_t veto_segFirstHits_ArraySize_ii = 0;
   Int_t Nveto_segPoint_ArraySize_ii = 0;
   
   if (Lconstructor=="LAND")
   {
     LandPoint_ArraySize_ii = Integers->GetBinContent(8);
     LandFirstHits_ArraySize_ii = Integers->GetBinContent(9);
   }
   if (Lconstructor=="NeuLAND")
   {
     NeuLandPoint_ArraySize_ii = Integers->GetBinContent(8);
   }
   if (Vconstructor=="veto_seg")
   {
     veto_segPoint_ArraySize_ii = Integers->GetBinContent(10);
     veto_segFirstHits_ArraySize_ii = Integers->GetBinContent(11);
   }
   if (Vconstructor=="Nveto_seg")
   {
     Nveto_segPoint_ArraySize_ii = Integers->GetBinContent(10);
   }
   
   Int_t const LandPoint_ArraySize_i = LandPoint_ArraySize_ii;
   Int_t const LandFirstHits_ArraySize_i = LandFirstHits_ArraySize_ii;
   Int_t const NeuLandPoint_ArraySize_i = NeuLandPoint_ArraySize_ii;
   Int_t const veto_segPoint_ArraySize_i = veto_segPoint_ArraySize_ii;
   Int_t const veto_segFirstHits_ArraySize_i = veto_segFirstHits_ArraySize_ii;
   Int_t const Nveto_segPoint_ArraySize_i = Nveto_segPoint_ArraySize_ii;
   Int_t const MCTrack_ArraySize_i = Integers->GetBinContent(7);
   Int_t const cbmroot_ArraySize_i = Integers->GetBinContent(12);
   Int_t const GeoTracks_ArraySize_i = Integers->GetBinContent(13);
   
   // Then duplicate them again as constants. This way they are absolutely constant, causing C++ not
   // to see any errors:
   Int_t const MCTrack_ArraySize = MCTrack_ArraySize_i;
   Int_t const LandPoint_ArraySize = LandPoint_ArraySize_i;
   Int_t const LandFirstHits_ArraySize = LandFirstHits_ArraySize_i;
   Int_t const NeuLandPoint_ArraySize = NeuLandPoint_ArraySize_i;
   Int_t const veto_segPoint_ArraySize = veto_segPoint_ArraySize_i;
   Int_t const veto_segFirstHits_ArraySize = veto_segFirstHits_ArraySize_i;
   Int_t const Nveto_segPoint_ArraySize = Nveto_segPoint_ArraySize_i;
   Int_t const cbmroot_ArraySize = cbmroot_ArraySize_i;
   Int_t const GeoTracks_ArraySize = GeoTracks_ArraySize_i;
  
   // Now these C++ arrays are set to the maximum number of elements that COULD be there. Most events
   // have a lot less elements associated with it, but in order to fit all the elements in there, the
   // C++ arrays have to be declared with the maximum. For more optimal control, it is however 
   // desirable to also have the precise array-length available for each event. Hence load this
   // from the Inputs-file:
   TH1I* hist_MC = (TH1I*) file->Get("MCTrack_Size");
   TH1I* hist_land = 0;
   TH1I* hist_landF = 0;
   TH1I* hist_neuland = 0;
   TH1I* hist_veto_seg = 0;
   TH1I* hist_veto_segF = 0;
   TH1I* hist_Nveto_seg = 0;
   TH1I* hist_header = (TH1I*) file->Get("MCEventHeader_Size");
   TH1I* hist_geo = (TH1I*) file->Get("GeoTracks_Size");
     
   if (Lconstructor=="LAND")
   {
     hist_land = (TH1I*) file->Get("landPoint_Size");
     hist_landF = (TH1I*) file->Get("landFirstHits_Size");
   }
   if (Lconstructor=="NeuLAND")
   {
     hist_neuland = (TH1I*) file->Get("neulandPoint_Size");
   }
   if (Vconstructor=="veto_seg")
   {
     hist_veto_seg = (TH1I*) file->Get("veto_segPoint_Size");
     hist_veto_segF = (TH1I*) file->Get("veto_segFirstHits_Size");
   }
   if (Vconstructor=="Nveto_seg")
   {
     hist_Nveto_seg = (TH1I*) file->Get("Nveto_segPoint_Size");
   }
   
   // Also, the location of the maxima are available from the Integers-histogram:
   Int_t MCTrack_MaxLoc = Integers->GetBinContent(14);
   Int_t LandPoint_MaxLoc = -1;
   Int_t landFirstHist_MaxLoc = -1;
   Int_t NeuLandPoint_MaxLoc = -1;
   Int_t veto_segPoint_MaxLoc = -1;
   Int_t veto_segFirstHist_MaxLoc = -1;
   Int_t Nveto_segPoint_MaxLoc = -1;
   Int_t cbmroot_MaxLoc = Integers->GetBinContent(19);
   Int_t GeoTracks_MaxLoc = Integers->GetBinContent(20);
   
   if (Lconstructor=="LAND")
   {
     LandPoint_MaxLoc = Integers->GetBinContent(15);
     landFirstHist_MaxLoc = Integers->GetBinContent(16);
   }
   if (Lconstructor=="NeuLAND")
   {
     NeuLandPoint_MaxLoc = Integers->GetBinContent(15);
   }
   if (Vconstructor=="veto_seg")
   {
     veto_segPoint_MaxLoc = Integers->GetBinContent(17);
     veto_segFirstHist_MaxLoc = Integers->GetBinContent(18);
   }
   if (Vconstructor=="Nveto_seg")
   {
     Nveto_segPoint_MaxLoc = Integers->GetBinContent(17);
   }

   // ============================================================================================
   // Declaration of leafs is now performed. We will not only declare the leafs but also check whether
   // they really exist, or that we generated an empty pointer. For that matter, we declare some
   // error variables:
   Int_t Errors_MC = 0;
   Int_t Errors_land_point = 0;
   Int_t Errors_neuland_point = 0;
   Int_t Errors_land_Hits = 0;
   Int_t Errors_veto_seg_point = 0;
   Int_t Errors_Nveto_seg_point = 0;
   Int_t Errors_veto_seg_Hits = 0;
   Int_t Errors_header = 0;
   Int_t Errors_Geo = 0;
   
   // ------------------------------------------
   // The following leafs are only concerned with Monte-Carlo properties of the generated Particle
   // tracks. Each element in the C++ array corresponds to a single Geant4-interaction point.
   // A track, however, is defined as the mathematical 3D-path between the point where the particle is 
   // created (possibly by the decay or collision of another particle) and where the particle is
   // annihilated (because it decays or collides, etc). The mother particle is defined as the 
   // particle that is annihilated at the point where the specific particle was generated.
   
   // The quantity MCTrack_ArraySize is the number of interaction points simulated by Geant 4 for the
   // entire event. Hence the interaction points might be ordered in the array in some nasty way.
   // Geant 4 only simulates interaction points. between interaction points nothing happens. Hence
   // The quantity MCTrack_ArraySize holds ALL interaction points (where particles collide, decay,
   // annihilate, deflect, etc) of a single event. This event can consist of multiple tracks.
   // the points are by no means ordered track-by-track in the array!
   
   // Leaves declaration:
   TLeaf* MCTrack_fUniqueID = cbmsim->FindLeaf("MCTrack.fUniqueID"); // UInt_t[n] //
   TLeaf* MCTrack_fBits = cbmsim->FindLeaf("MCTrack.fBits"); // UInt_t[n] //
   TLeaf* MCTrack_fPdgCode  = cbmsim->FindLeaf("MCTrack.fPdgCode"); // Int_t[n] // particle ID of the particle performing this specific interaction. This uses the particle PDG code, ions have 1000......-code, so they are much much higher
										// then elemntary particles. Hence a histogram from TBrowser() does not reveal the structure. for particles (not ions), The PDG code is constructed in the
                                                                                // following way: PDG = 1000*i + 100*j + 10*k + (2*s+1). Here s is the total spin particle. i is the PDG code of the first quark of the particle, j is the
									        // PDG code of the second quark of the particle and k is the PDG code of the third quark of the particle (0 if the particle does not have this quark).
										// u=1, d=2, s=3, c=4, b=5, t=6, b'=7. NB: electrons, gamma's etc have PDG between 10 en 100, so they can be distinguished.
										// mesons have PDG = (100*max(i,j) + 10*min(i,j) + (2s+1))*sign(i-j)*(-1)^max(i,j) this is because they have an antiquark 
   TLeaf* MCTrack_fMotherId = cbmsim->FindLeaf("MCTrack.fMotherId"); // Int_t[n] // particle ID of mother particle of the particle performing this specific interaction. If it equals -1, it is the primary particle! "DIMA".
   TLeaf* MCTrack_fPx = cbmsim->FindLeaf("MCTrack.fPx"); // Double32_t[n] // Particle momentum in x-direction at a specific interaction point.
   TLeaf* MCTrack_fPy = cbmsim->FindLeaf("MCTrack.fPy"); // Double32_t[n] // Particle momentum in y-direction at a specific interaction point.
   TLeaf* MCTrack_fPz = cbmsim->FindLeaf("MCTrack.fPz"); // Double32_t[n] // Particle momentum in z-direction at a specific interaction point.
   TLeaf* MCTrack_fStartX = cbmsim->FindLeaf("MCTrack.fStartX"); // Double32_t[n] // x-coordinate of the start-point of the particle track this specific interaction point belongs to (so not x-coordinate of the interaction point itself!)
   TLeaf* MCTrack_fStartY = cbmsim->FindLeaf("MCTrack.fStartY"); // Double32_t[n] // y-coordinate of the start-point of the particle track this specific interaction point belongs to (so not y-coordinate of the interaction point itself!)
   TLeaf* MCTrack_fStartZ = cbmsim->FindLeaf("MCTrack.fStartZ"); // Double32_t[n] // z-coordinate of the start-point of the particle track this specific interaction point belongs to (so not z-coordinate of the interaction point itself!)
   TLeaf* MCTrack_fStartT = cbmsim->FindLeaf("MCTrack.fStartT"); // Double32_t[n] // time-coordinate of the start-point of the particle track this specific interaction point belongs to (so not time-coordinate of the interaction point itself!)
   TLeaf* MCTrack_fNPoints = cbmsim->FindLeaf("MCTrack.fNPoints"); // Int_t[n] //
   TLeaf* MCTrack_fMass = cbmsim->FindLeaf("MCTrack.fMass"); // Double32_t[n] // mass of the particle performing this specific interaction.
   
   // Checking if the declaration worked:
   if (MCTrack_fUniqueID == 0) {Errors_MC = Errors_MC + 1;}
   if (MCTrack_fBits == 0) {Errors_MC = Errors_MC + 1;}
   if (MCTrack_fPdgCode == 0) {Errors_MC = Errors_MC + 1;}
   if (MCTrack_fMotherId == 0) {Errors_MC = Errors_MC + 1;}
   if (MCTrack_fPx == 0) {Errors_MC = Errors_MC + 1;}
   if (MCTrack_fPy == 0) {Errors_MC = Errors_MC + 1;}
   if (MCTrack_fPz == 0) {Errors_MC = Errors_MC + 1;}
   if (MCTrack_fStartX == 0) {Errors_MC = Errors_MC + 1;}
   if (MCTrack_fStartY == 0) {Errors_MC = Errors_MC + 1;}
   if (MCTrack_fStartZ == 0) {Errors_MC = Errors_MC + 1;}
   if (MCTrack_fStartT == 0) {Errors_MC = Errors_MC + 1;}
   if (MCTrack_fNPoints == 0) {Errors_MC = Errors_MC + 1;}
   if (MCTrack_fMass == 0) {Errors_MC = Errors_MC + 1;}
   
   // Now in order to read out a single data-element from the tree, give the folloing commands (if the pointer is non-empty):
   // Long64_t nbytes = cbmsim->GetEntry(i); //=> to select the proper event: event i.
   // Double_t value = MCTrack_fPx->GetValue(k); //=> to load the k-th value from the leaf into the variable value (corresponding to event i).
   
   // ------------------------------------------------------
   // The following leafs are concerned only for the interaction points inside NeuLAND. The quantity 
   // LandPoint_ArraySize holds the number of interaction points inside NeuLAND. Notice that at a
   // boundary between media, Geant 4 ALWAYS places an interaction point.
   
   // Leaves declaration for the Land-constructor NOTE:
   TLeaf* LandPoint_fUniqueID = cbmsim->FindLeaf("LandPoint.fUniqueID"); // UInt_t[n] //
   TLeaf* LandPoint_fBits = cbmsim->FindLeaf("LandPoint.fBits"); // UInt_t[n] //
   TLeaf* LandPoint_fLinks = cbmsim->FindLeaf("LandPoint.fLinks"); // set<FairLink> //
   TLeaf* LandPoint_fPersistanceCheck = cbmsim->FindLeaf("LandPoint.fPersistanceCheck"); // Bool_t[n] //
   TLeaf* LandPoint_fVerbose = cbmsim->FindLeaf("LandPoint.fVerbose"); // Int_t[n] //
   TLeaf* LandPoint_fDefaultType = cbmsim->FindLeaf("LandPoint.fDefaultType"); // Int_t[n] //
   TLeaf* LandPoint_fTrackID = cbmsim->FindLeaf("LandPoint.fTrackID"); // Int_t[n] // Some number to identify the track. could be just a numerical artifact or a particle ID.
   TLeaf* LandPoint_fEventId = cbmsim->FindLeaf("LandPoint.fEventId"); // UInt_t[n] //
   TLeaf* LandPoint_fPx = cbmsim->FindLeaf("LandPoint.fPx"); // Double32_t[n] // X-momentum [GeV] at entrance of active volume: in this case NeuLAND. "DIMA"
   TLeaf* LandPoint_fPy = cbmsim->FindLeaf("LandPoint.fPy"); // Double32_t[n] // Y-momentum [GeV] at entrance of active volume: in this case NeuLAND. "DIMA"
   TLeaf* LandPoint_fPz = cbmsim->FindLeaf("LandPoint.fPz"); // Double32_t[n] // Z-momentum [GeV] at entrance of active volume: in this case NeuLAND. "DIMA"
   TLeaf* LandPoint_fTime = cbmsim->FindLeaf("LandPoint.fTime"); // Double32_t[n] // Time [ns] since the event started (primary particle generation) (averaged over detector). "DIMA"
   TLeaf* LandPoint_fLength = cbmsim->FindLeaf("LandPoint.fLength"); // Double32_t[n] // Total Track length since creation (averaged over detector) [cm]. "DIMA"
   TLeaf* LandPoint_fELoss = cbmsim->FindLeaf("LandPoint.fELoss"); // Double32_t[n] // Energy deposit in the detector [Gev]. "DIMA"
   TLeaf* LandPoint_fDetectorID = cbmsim->FindLeaf("LandPoint.fDetectorID"); // Int_t[n] // Identification of the detector. Does not say very much, since we know already it is NeuLAND. "DIMA"
   TLeaf* LandPoint_fX = cbmsim->FindLeaf("LandPoint.fX"); // Double32_t[n] // X-coordinate [cm] at entrance of active volume: in this case NeuLAND. "DIMA"
   TLeaf* LandPoint_fY = cbmsim->FindLeaf("LandPoint.fY"); // Double32_t[n] // Y-coordinate [cm] at entrance of active volume: in this case NeuLAND. "DIMA"
   TLeaf* LandPoint_fZ = cbmsim->FindLeaf("LandPoint.fZ"); // Double32_t[n] // Z-coordinate [cm] at entrance of active volume: in this case NeuLAND. "DIMA"
   TLeaf* LandPoint_fX_out = cbmsim->FindLeaf("LandPoint.fX_out"); // Double32_t[n] // X-coordinate [cm] at exit of active volume: in this case NeuLAND. "DIMA"
   TLeaf* LandPoint_fY_out = cbmsim->FindLeaf("LandPoint.fY_out"); // Double32_t[n] // Y-coordinate [cm] at exit of active volume: in this case NeuLAND. "DIMA"
   TLeaf* LandPoint_fZ_out = cbmsim->FindLeaf("LandPoint.fZ_out"); // Double32_t[n] // Z-coordinate [cm] at exit of active volume: in this case NeuLAND. "DIMA"
   TLeaf* LandPoint_fPx_out = cbmsim->FindLeaf("LandPoint.fPx_out"); // Double32_t[n] // X-momentum [GeV] at exit of active volume: in this case NeuLAND. "DIMA"
   TLeaf* LandPoint_fPy_out = cbmsim->FindLeaf("LandPoint.fPy_out"); // Double32_t[n] // Y-momentum [GeV] at exit of active volume: in this case NeuLAND. "DIMA"
   TLeaf* LandPoint_fPz_out = cbmsim->FindLeaf("LandPoint.fPz_out"); // Double32_t[n] // Z-momentum [GeV] at exit of active volume: in this case NeuLAND. "DIMA"
   TLeaf* LandPoint_fSector = cbmsim->FindLeaf("LandPoint.fSector"); // Int_t[n] // The ID of which bar of NeuLAND is hit: 0-3000.
   TLeaf* LandPoint_fPaddleNb = cbmsim->FindLeaf("LandPoint.fPaddleNb"); // Int_t[n] //
   TLeaf* LandPoint_fPaddleTyp = cbmsim->FindLeaf("LandPoint.fPaddleTyp"); // Int_t[n] //
   TLeaf* LandPoint_fLightYield = cbmsim->FindLeaf("LandPoint.fLightYield"); // Double_t[n] // If the energy deposit (dE/dx) is low, then the produced scintillator light is proportional to dE/dx: dN/dx = c*dE/dx where N stands for number of photons produced
                                                                                            // (those photons carry a certain energy distribution). If dE/dx becomes higher however, the number of produced photons saturates. This saturation is descibed by Birk's
											    // law: dL/dx = (S*dE/dx)(1 + B1*dE/dx). Here S and B1 are constants. dL/dx is now STILL energy deposition, but now with the saturation effect taken into account.
											    // Then the produced scintillator light is given by dN/dx = c*dL/dx. This is now valid for all energy ranges. N is the number of produced photons. Hence the quantity
											    // LandPoint_fLightYield->GetValue(k) holds the amount of deposited energy at the k-th interaction point in NeuLAND. Its unit is [GeV] and the saturation already is
											    // taken into account: The number of produced photons at the k-th interaction point is given by N = c*LandPoint_fLightYield->GetValue(k) where c is a constant that
											    // only depends on the scintillator material and the particle. It holds up to all energy ranges, because the saturation effect is taken into account. For fEloss this is
											    // this is not the case. Hence N = c*LandPoint_fLightYield->GetValue(k) gives the number of produced photons at the k-th interaction point. Each photon carries an energy
											    // which is a sample of the same energy distribution, that depends also on LandPoint_fLightYield->GetValue(k) and on the material and particle of course.
											    // hence to call a particle detected if a certain amount of light is PRODUCED, one could simply put an energy threshold on fLightYield per scintillator bar. Then 
											    // the constant c is absorbed in this threshold. In order to put the light propagation in there however, one has to know c and generate a number of photons. One
											    // also has to know the energy distribution of these photons to generate them and then let them propagate...
   
   // Checking if the declaration worked:
   if (LandPoint_fUniqueID == 0) {Errors_land_point = Errors_land_point + 1;}
   if (LandPoint_fBits == 0) {Errors_land_point = Errors_land_point + 1;}
   if ((LandPoint_fLinks == 0)&&(version == "feb14")) {Errors_land_point = Errors_land_point + 1;} // Not compatible with jan15-release
   if ((LandPoint_fPersistanceCheck == 0)&&(version == "feb14")) {Errors_land_point = Errors_land_point + 1;} // Not compatible with jan15-release
   if ((LandPoint_fVerbose == 0)&&(version == "feb14")) {Errors_land_point = Errors_land_point + 1;} // Not compatible with jan15-release
   if ((LandPoint_fDefaultType == 0)&&(version == "feb14")) {Errors_land_point = Errors_land_point + 1;} // Not compatible with jan15-release
   if (LandPoint_fTrackID == 0) {Errors_land_point = Errors_land_point + 1;}
   if (LandPoint_fEventId == 0) {Errors_land_point = Errors_land_point + 1;}
   if (LandPoint_fPx == 0) {Errors_land_point = Errors_land_point + 1;}
   if (LandPoint_fPy == 0) {Errors_land_point = Errors_land_point + 1;}
   if (LandPoint_fPz == 0) {Errors_land_point = Errors_land_point + 1;}
   if (LandPoint_fTime == 0) {Errors_land_point = Errors_land_point + 1;}
   if (LandPoint_fLength == 0) {Errors_land_point = Errors_land_point + 1;}
   if (LandPoint_fELoss == 0) {Errors_land_point = Errors_land_point + 1;}
   if (LandPoint_fDetectorID == 0) {Errors_land_point = Errors_land_point + 1;}
   if (LandPoint_fX == 0) {Errors_land_point = Errors_land_point + 1;}
   if (LandPoint_fY == 0) {Errors_land_point = Errors_land_point + 1;}
   if (LandPoint_fZ == 0) {Errors_land_point = Errors_land_point + 1;}
   if (LandPoint_fX_out == 0) {Errors_land_point = Errors_land_point + 1;}
   if (LandPoint_fY_out == 0) {Errors_land_point = Errors_land_point + 1;}
   if (LandPoint_fZ_out == 0) {Errors_land_point = Errors_land_point + 1;}
   if (LandPoint_fPx_out == 0) {Errors_land_point = Errors_land_point + 1;}
   if (LandPoint_fPy_out == 0) {Errors_land_point = Errors_land_point + 1;}
   if (LandPoint_fPz_out == 0) {Errors_land_point = Errors_land_point + 1;}
   if (LandPoint_fSector == 0) {Errors_land_point = Errors_land_point + 1;}
   if (LandPoint_fPaddleNb == 0) {Errors_land_point = Errors_land_point + 1;}
   if (LandPoint_fPaddleTyp == 0) {Errors_land_point = Errors_land_point + 1;}
   if (LandPoint_fLightYield == 0) {Errors_land_point = Errors_land_point + 1;}
   
   // Leaves declaration for the NeuLand-constructor NOTE:
   // The NeuLAND-class is however obscolete. One should not use it. "DIMA"
   TLeaf* NeuLandPoint_fUniqueID = cbmsim->FindLeaf("NeuLandPoint.fUniqueID"); // UInt_t[n] //
   TLeaf* NeuLandPoint_fBits = cbmsim->FindLeaf("NeuLandPoint.fBits"); // UInt_t[n] //
   TLeaf* NeuLandPoint_fLinks = cbmsim->FindLeaf("NeuLandPoint.fLinks"); // set<FairLink> //
   TLeaf* NeuLandPoint_fPersistanceCheck = cbmsim->FindLeaf("NeuLandPoint.fPersistanceCheck"); // Bool_t[n] //
   TLeaf* NeuLandPoint_fVerbose = cbmsim->FindLeaf("NeuLandPoint.fVerbose"); // Int_t[n] //
   TLeaf* NeuLandPoint_fDefaultType = cbmsim->FindLeaf("NeuLandPoint.fDefaultType"); // Int_t[n] //
   TLeaf* NeuLandPoint_fTrackID = cbmsim->FindLeaf("NeuLandPoint.fTrackID"); // Int_t[n] // Some number to identify the track. could be just a numerical artifact or a particle ID.
   TLeaf* NeuLandPoint_fEventId = cbmsim->FindLeaf("NeuLandPoint.fEventId"); // UInt_t[n] //
   TLeaf* NeuLandPoint_fPx = cbmsim->FindLeaf("NeuLandPoint.fPx"); // Double32_t[n] // X-momentum [GeV] at entrance of active volume: in this case NeuLAND. "DIMA"
   TLeaf* NeuLandPoint_fPy = cbmsim->FindLeaf("NeuLandPoint.fPy"); // Double32_t[n] // Y-momentum [GeV] at entrance of active volume: in this case NeuLAND. "DIMA"
   TLeaf* NeuLandPoint_fPz = cbmsim->FindLeaf("NeuLandPoint.fPz"); // Double32_t[n] // Z-momentum [GeV] at entrance of active volume: in this case NeuLAND. "DIMA"
   TLeaf* NeuLandPoint_fTime = cbmsim->FindLeaf("NeuLandPoint.fTime"); // Double32_t[n] // Time [ns] since the event started (primary particle generation) (averaged over detector). "DIMA"
   TLeaf* NeuLandPoint_fLength = cbmsim->FindLeaf("NeuLandPoint.fLength"); // Double32_t[n] // Total Track length since creation (averaged over detector) [cm]. "DIMA"
   TLeaf* NeuLandPoint_fELoss = cbmsim->FindLeaf("NeuLandPoint.fELoss"); // Double32_t[n] // Energy deposit in the detector [Gev]. "DIMA"
   TLeaf* NeuLandPoint_fDetectorID = cbmsim->FindLeaf("NeuLandPoint.fDetectorID"); // Int_t[n] // Identification of the detector. Does not say very much, since we know already it is NeuLAND. "DIMA"
   TLeaf* NeuLandPoint_fX = cbmsim->FindLeaf("NeuLandPoint.fX"); // Double32_t[n] // X-coordinate [cm] at entrance of active volume: in this case NeuLAND. "DIMA"
   TLeaf* NeuLandPoint_fY = cbmsim->FindLeaf("NeuLandPoint.fY"); // Double32_t[n] // Y-coordinate [cm] at entrance of active volume: in this case NeuLAND. "DIMA"
   TLeaf* NeuLandPoint_fZ = cbmsim->FindLeaf("NeuLandPoint.fZ"); // Double32_t[n] // Z-coordinate [cm] at entrance of active volume: in this case NeuLAND. "DIMA"
   TLeaf* NeuLandPoint_fX_out = cbmsim->FindLeaf("NeuLandPoint.fX_out"); // Double32_t[n] // X-coordinate [cm] at exit of active volume: in this case NeuLAND. "DIMA"
   TLeaf* NeuLandPoint_fY_out = cbmsim->FindLeaf("NeuLandPoint.fY_out"); // Double32_t[n] // Y-coordinate [cm] at exit of active volume: in this case NeuLAND. "DIMA"
   TLeaf* NeuLandPoint_fZ_out = cbmsim->FindLeaf("NeuLandPoint.fZ_out"); // Double32_t[n] // Z-coordinate [cm] at exit of active volume: in this case NeuLAND. "DIMA"
   TLeaf* NeuLandPoint_fPx_out = cbmsim->FindLeaf("NeuLandPoint.fPx_out"); // Double32_t[n] // X-momentum [GeV] at exit of active volume: in this case NeuLAND. "DIMA"
   TLeaf* NeuLandPoint_fPy_out = cbmsim->FindLeaf("NeuLandPoint.fPy_out"); // Double32_t[n] // Y-momentum [GeV] at exit of active volume: in this case NeuLAND. "DIMA"
   TLeaf* NeuLandPoint_fPz_out = cbmsim->FindLeaf("NeuLandPoint.fPz_out"); // Double32_t[n] // Z-momentum [GeV] at exit of active volume: in this case NeuLAND. "DIMA"
   TLeaf* NeuLandPoint_fEventID = cbmsim->FindLeaf("NeuLandPoint.fEventID"); // Int_t[n] // 
   TLeaf* NeuLandPoint_fMot0TrackID = cbmsim->FindLeaf("NeuLandPoint.fMot0TrackID"); // Int_t[n] // 
   TLeaf* NeuLandPoint_fDetID = cbmsim->FindLeaf("NeuLandPoint.fDetID"); // Int_t[n] // 
   TLeaf* NeuLandPoint_fCellID = cbmsim->FindLeaf("NeuLandPoint.fCellID"); // Int_t[n] // 
   TLeaf* NeuLandPoint_fSegID = cbmsim->FindLeaf("NeuLandPoint.fSegID"); // Int_t[n] // 
   TLeaf* NeuLandPoint_fMass = cbmsim->FindLeaf("NeuLandPoint.fMass"); // Double_t[n] // 
   
   // Checking if the declaration worked:
   if (NeuLandPoint_fUniqueID == 0) {Errors_neuland_point = Errors_neuland_point + 1;}
   if (NeuLandPoint_fBits == 0) {Errors_neuland_point = Errors_neuland_point + 1;}
   if ((NeuLandPoint_fLinks == 0)&&(version == "feb14")) {Errors_neuland_point = Errors_neuland_point + 1;} // Not compatible with jan15-release
   if ((NeuLandPoint_fPersistanceCheck == 0)&&(version == "feb14")) {Errors_neuland_point = Errors_neuland_point + 1;} // Not compatible with jan15-release
   if ((NeuLandPoint_fVerbose == 0)&&(version == "feb14")) {Errors_neuland_point = Errors_neuland_point + 1;} // Not compatible with jan15-release
   if ((NeuLandPoint_fDefaultType == 0)&&(version == "feb14")) {Errors_neuland_point = Errors_neuland_point + 1;} // Not compatible with jan15-release
   if (NeuLandPoint_fTrackID == 0) {Errors_neuland_point = Errors_neuland_point + 1;}
   if (NeuLandPoint_fEventId == 0) {Errors_neuland_point = Errors_neuland_point + 1;}
   if (NeuLandPoint_fPx == 0) {Errors_neuland_point = Errors_neuland_point + 1;}
   if (NeuLandPoint_fPy == 0) {Errors_neuland_point = Errors_neuland_point + 1;}
   if (NeuLandPoint_fPz == 0) {Errors_neuland_point = Errors_neuland_point + 1;}
   if (NeuLandPoint_fTime == 0) {Errors_neuland_point = Errors_neuland_point + 1;}
   if (NeuLandPoint_fLength == 0) {Errors_neuland_point = Errors_neuland_point + 1;}
   if (NeuLandPoint_fELoss == 0) {Errors_neuland_point = Errors_neuland_point + 1;}
   if (NeuLandPoint_fDetectorID == 0) {Errors_neuland_point = Errors_neuland_point + 1;}
   if (NeuLandPoint_fX == 0) {Errors_neuland_point = Errors_neuland_point + 1;}
   if (NeuLandPoint_fY == 0) {Errors_neuland_point = Errors_neuland_point + 1;}
   if (NeuLandPoint_fZ == 0) {Errors_neuland_point = Errors_neuland_point + 1;}
   if (NeuLandPoint_fX_out == 0) {Errors_neuland_point = Errors_neuland_point + 1;}
   if (NeuLandPoint_fY_out == 0) {Errors_neuland_point = Errors_neuland_point + 1;}
   if (NeuLandPoint_fZ_out == 0) {Errors_neuland_point = Errors_neuland_point + 1;}
   if (NeuLandPoint_fPx_out == 0) {Errors_neuland_point = Errors_neuland_point + 1;}
   if (NeuLandPoint_fPy_out == 0) {Errors_neuland_point = Errors_neuland_point + 1;}
   if (NeuLandPoint_fPz_out == 0) {Errors_neuland_point = Errors_neuland_point + 1;}
   if (NeuLandPoint_fEventID == 0) {Errors_neuland_point = Errors_neuland_point + 1;}
   if (NeuLandPoint_fMot0TrackID == 0) {Errors_neuland_point = Errors_neuland_point + 1;}
   if (NeuLandPoint_fDetID == 0) {Errors_neuland_point = Errors_neuland_point + 1;}
   if (NeuLandPoint_fCellID == 0) {Errors_neuland_point = Errors_neuland_point + 1;}
   if (NeuLandPoint_fSegID == 0) {Errors_neuland_point = Errors_neuland_point + 1;}
   if (NeuLandPoint_fMass == 0) {Errors_neuland_point = Errors_neuland_point + 1;}
   
   // -----------------------------------------------------
   // The following leafs are also associated with NeuLAND: They give precise information on the coordinates of the first few hits of a particle in NeuLAND. x,y,z,t are all self-expolanatory. "DIMA".
   
   // Declaration of leafs for the Land-constructor NOTE:
   TLeaf* LandFirstHits_fUniqueID = cbmsim->FindLeaf("LandFirstHits.fUniqueID"); // UInt_t[n] //
   TLeaf* LandFirstHits_fBits = cbmsim->FindLeaf("LandFirstHits.fBits"); // UInt_t[n] //
   TLeaf* LandFirstHits_fx0 = cbmsim->FindLeaf("LandFirstHits.fx0"); // Double32_t[n] //
   TLeaf* LandFirstHits_fy0 = cbmsim->FindLeaf("LandFirstHits.fy0"); // Double32_t[n] //
   TLeaf* LandFirstHits_fz0 = cbmsim->FindLeaf("LandFirstHits.fz0"); // Double32_t[n] //
   TLeaf* LandFirstHits_fT0 = cbmsim->FindLeaf("LandFirstHits.fT0"); // Double32_t[n] //
   TLeaf* LandFirstHits_fx1 = cbmsim->FindLeaf("LandFirstHits.fx1"); // Double32_t[n] //
   TLeaf* LandFirstHits_fy1 = cbmsim->FindLeaf("LandFirstHits.fy1"); // Double32_t[n] //
   TLeaf* LandFirstHits_fz1 = cbmsim->FindLeaf("LandFirstHits.fz1"); // Double32_t[n] //
   TLeaf* LandFirstHits_fT1 = cbmsim->FindLeaf("LandFirstHits.fT1"); // Double32_t[n] //
   TLeaf* LandFirstHits_fx2 = cbmsim->FindLeaf("LandFirstHits.fx2"); // Double32_t[n] //
   TLeaf* LandFirstHits_fy2 = cbmsim->FindLeaf("LandFirstHits.fy2"); // Double32_t[n] //
   TLeaf* LandFirstHits_fz2 = cbmsim->FindLeaf("LandFirstHits.fz2"); // Double32_t[n] //
   TLeaf* LandFirstHits_fT2 = cbmsim->FindLeaf("LandFirstHits.fT2"); // Double32_t[n] //
   TLeaf* LandFirstHits_fx3 = cbmsim->FindLeaf("LandFirstHits.fx3"); // Double32_t[n] //
   TLeaf* LandFirstHits_fy3 = cbmsim->FindLeaf("LandFirstHits.fy3"); // Double32_t[n] //
   TLeaf* LandFirstHits_fz3 = cbmsim->FindLeaf("LandFirstHits.fz3"); // Double32_t[n] //
   TLeaf* LandFirstHits_fT3 = cbmsim->FindLeaf("LandFirstHits.fT3"); // Double32_t[n] //
   TLeaf* LandFirstHits_fx4 = cbmsim->FindLeaf("LandFirstHits.fx4"); // Double32_t[n] //
   TLeaf* LandFirstHits_fy4 = cbmsim->FindLeaf("LandFirstHits.fy4"); // Double32_t[n] //
   TLeaf* LandFirstHits_fz4 = cbmsim->FindLeaf("LandFirstHits.fz4"); // Double32_t[n] //
   TLeaf* LandFirstHits_fT4 = cbmsim->FindLeaf("LandFirstHits.fT4"); // Double32_t[n] //
   TLeaf* LandFirstHits_fx5 = cbmsim->FindLeaf("LandFirstHits.fx5"); // Double32_t[n] //
   TLeaf* LandFirstHits_fy5 = cbmsim->FindLeaf("LandFirstHits.fy5"); // Double32_t[n] //
   TLeaf* LandFirstHits_fz5 = cbmsim->FindLeaf("LandFirstHits.fz5"); // Double32_t[n] //
   TLeaf* LandFirstHits_fT5 = cbmsim->FindLeaf("LandFirstHits.fT5"); // Double32_t[n] //
   
   // These hits are the neutron-collisions themselves! the LandPoint however, corresponds to Monte-Carlo interaction points of the charged particles produced in these collisions. If you therefore analyze the LandPoint, you 
   // analyse a different physical process with different probabilities! For analyzing neutron detection, you should therefore use the LandFirstHits instead. Of course in reality it are the charged particles you detect (or
   // rather, their scintillator light), but then you have to take the probability of the neutron collisions, etc also into account. This is done by a tracking algorithm. Hence if you which to use the LandPoint,
   // you cannot do so without the use of a tracking algorithm. This algorithm is available in R3Broot. "DIMA".
   
   // Checking if the declaration worked:
   if (LandFirstHits_fUniqueID == 0) {Errors_land_Hits = Errors_land_Hits + 1;}
   if (LandFirstHits_fBits == 0) {Errors_land_Hits = Errors_land_Hits + 1;}
   if (LandFirstHits_fx0 == 0) {Errors_land_Hits = Errors_land_Hits + 1;}
   if (LandFirstHits_fy0 == 0) {Errors_land_Hits = Errors_land_Hits + 1;}
   if (LandFirstHits_fz0 == 0) {Errors_land_Hits = Errors_land_Hits + 1;}
   if (LandFirstHits_fT0 == 0) {Errors_land_Hits = Errors_land_Hits + 1;}
   if (LandFirstHits_fx1 == 0) {Errors_land_Hits = Errors_land_Hits + 1;}
   if (LandFirstHits_fy1 == 0) {Errors_land_Hits = Errors_land_Hits + 1;}
   if (LandFirstHits_fz1 == 0) {Errors_land_Hits = Errors_land_Hits + 1;}
   if (LandFirstHits_fT1 == 0) {Errors_land_Hits = Errors_land_Hits + 1;}
   if (LandFirstHits_fx2 == 0) {Errors_land_Hits = Errors_land_Hits + 1;}
   if (LandFirstHits_fy2 == 0) {Errors_land_Hits = Errors_land_Hits + 1;}
   if (LandFirstHits_fz2 == 0) {Errors_land_Hits = Errors_land_Hits + 1;}
   if (LandFirstHits_fT2 == 0) {Errors_land_Hits = Errors_land_Hits + 1;}
   if (LandFirstHits_fx3 == 0) {Errors_land_Hits = Errors_land_Hits + 1;}
   if (LandFirstHits_fy3 == 0) {Errors_land_Hits = Errors_land_Hits + 1;}
   if (LandFirstHits_fz3 == 0) {Errors_land_Hits = Errors_land_Hits + 1;}
   if (LandFirstHits_fT3 == 0) {Errors_land_Hits = Errors_land_Hits + 1;}
   if (LandFirstHits_fx4 == 0) {Errors_land_Hits = Errors_land_Hits + 1;}
   if (LandFirstHits_fy4 == 0) {Errors_land_Hits = Errors_land_Hits + 1;}
   if (LandFirstHits_fz4 == 0) {Errors_land_Hits = Errors_land_Hits + 1;}
   if (LandFirstHits_fT4 == 0) {Errors_land_Hits = Errors_land_Hits + 1;}
   if (LandFirstHits_fx5 == 0) {Errors_land_Hits = Errors_land_Hits + 1;}
   if (LandFirstHits_fy5 == 0) {Errors_land_Hits = Errors_land_Hits + 1;}
   if (LandFirstHits_fz5 == 0) {Errors_land_Hits = Errors_land_Hits + 1;}
   if (LandFirstHits_fT5 == 0) {Errors_land_Hits = Errors_land_Hits + 1;}
   
   // ------------------------------------------------------
   // The following leafs are concerned only for the interaction points inside the segmented veto. The quantity 
   // veto_segPoint_ArraySize holds the number of interaction points inside the segmented veto. Notice that at a
   // boundary between media, Geant 4 ALWAYS places an interaction point.
   
   // Leaves declaration for veto_seg-constructor NOTE:   
   TLeaf* veto_segPoint_fUniqueID = cbmsim->FindLeaf("veto_segPoint.fUniqueID"); // UInt_t[n] //
   TLeaf* veto_segPoint_fBits = cbmsim->FindLeaf("veto_segPoint.fBits"); // UInt_t[n] //
   TLeaf* veto_segPoint_fLinks = cbmsim->FindLeaf("veto_segPoint.fLinks"); // set<FairLink> //
   TLeaf* veto_segPoint_fPersistanceCheck = cbmsim->FindLeaf("veto_segPoint.fPersistanceCheck"); // Bool_t[n] //
   TLeaf* veto_segPoint_fVerbose = cbmsim->FindLeaf("veto_segPoint.fVerbose"); // Int_t[n] //
   TLeaf* veto_segPoint_fDefaultType = cbmsim->FindLeaf("veto_segPoint.fDefaultType"); // Int_t[n] //
   TLeaf* veto_segPoint_fTrackID = cbmsim->FindLeaf("veto_segPoint.fTrackID"); // Int_t[n] // Some number to identify the track. could be just a numerical artifact or a particle ID.
   TLeaf* veto_segPoint_fEventId = cbmsim->FindLeaf("veto_segPoint.fEventId"); // UInt_t[n] //
   TLeaf* veto_segPoint_fPx = cbmsim->FindLeaf("veto_segPoint.fPx"); // Double32_t[n] // X-momentum [GeV] at entrance of active volume: in this case the segmented VETO. "DIMA"
   TLeaf* veto_segPoint_fPy = cbmsim->FindLeaf("veto_segPoint.fPy"); // Double32_t[n] // Y-momentum [GeV] at entrance of active volume: in this case the segmented VETO. "DIMA"
   TLeaf* veto_segPoint_fPz = cbmsim->FindLeaf("veto_segPoint.fPz"); // Double32_t[n] // Z-momentum [GeV] at entrance of active volume: in this case the segmented VETO. "DIMA"
   TLeaf* veto_segPoint_fTime = cbmsim->FindLeaf("veto_segPoint.fTime"); // Double32_t[n] // Time [ns] since the event started (primary particle generation) (averaged over detector). "DIMA"
   TLeaf* veto_segPoint_fLength = cbmsim->FindLeaf("veto_segPoint.fLength"); // Double32_t[n] // Total Track length since creation (averaged over detector) [cm]. "DIMA"
   TLeaf* veto_segPoint_fELoss = cbmsim->FindLeaf("veto_segPoint.fELoss"); // Double32_t[n] // Energy deposit in the detector [GeV]. "DIMA"
   TLeaf* veto_segPoint_fDetectorID = cbmsim->FindLeaf("veto_segPoint.fDetectorID"); // Int_t[n] // Identification of the detector. Does not say very much, since we know already it is the segmented VETO. "DIMA"
   TLeaf* veto_segPoint_fX = cbmsim->FindLeaf("veto_segPoint.fX"); // Double32_t[n] // X-coordinate [cm] at entrance of active volume: in this case the segmented VETO. "DIMA"
   TLeaf* veto_segPoint_fY = cbmsim->FindLeaf("veto_segPoint.fY"); // Double32_t[n] // Y-coordinate [cm] at entrance of active volume: in this case the segmented VETO. "DIMA"
   TLeaf* veto_segPoint_fZ = cbmsim->FindLeaf("veto_segPoint.fZ"); // Double32_t[n] // Z-coordinate [cm] at entrance of active volume: in this case the segmented VETO. "DIMA"
   TLeaf* veto_segPoint_fX_out = cbmsim->FindLeaf("veto_segPoint.fX_out"); // Double32_t[n] // X-coordinate [cm] at exit of active volume: in this case the segmented VETO. "DIMA"
   TLeaf* veto_segPoint_fY_out = cbmsim->FindLeaf("veto_segPoint.fY_out"); // Double32_t[n] // Y-coordinate [cm] at exit of active volume: in this case the segmented VETO. "DIMA"
   TLeaf* veto_segPoint_fZ_out = cbmsim->FindLeaf("veto_segPoint.fZ_out"); // Double32_t[n] // Z-coordinate [cm] at exit of active volume: in this case the segmented VETO. "DIMA"
   TLeaf* veto_segPoint_fPx_out = cbmsim->FindLeaf("veto_segPoint.fPx_out"); // Double32_t[n] // X-momentum [GeV] at exit of active volume: in this case the segmented VETO. "DIMA"
   TLeaf* veto_segPoint_fPy_out = cbmsim->FindLeaf("veto_segPoint.fPy_out"); // Double32_t[n] // Y-momentum [GeV] at exit of active volume: in this case the segmented VETO. "DIMA"
   TLeaf* veto_segPoint_fPz_out = cbmsim->FindLeaf("veto_segPoint.fPz_out"); // Double32_t[n] // Z-momentum [GeV] at exit of active volume: in this case the segmented VETO. "DIMA"
   TLeaf* veto_segPoint_fSector = cbmsim->FindLeaf("veto_segPoint.fSector"); // Int_t[n] // // Int_t[n] // The ID of which bar of the segmented VETO is hit.
   TLeaf* veto_segPoint_fPaddleNb = cbmsim->FindLeaf("veto_segPoint.fPaddleNb"); // Int_t[n] //
   TLeaf* veto_segPoint_fPaddleTyp = cbmsim->FindLeaf("veto_segPoint.fPaddleTyp"); // Int_t[n] //
   TLeaf* veto_segPoint_fLightYield = cbmsim->FindLeaf("veto_segPoint.fLightYield"); // Double_t[n] // Same story as with LandPoint_fLightYield.
   
   // Checking if the declaration worked:
   if (veto_segPoint_fUniqueID == 0) {Errors_veto_seg_point = Errors_veto_seg_point + 1;}
   if (veto_segPoint_fBits == 0) {Errors_veto_seg_point = Errors_veto_seg_point + 1;}
   if ((veto_segPoint_fLinks == 0)&&(version == "feb14")) {Errors_veto_seg_point = Errors_veto_seg_point + 1;} // Not compatible with jan15-release
   if ((veto_segPoint_fPersistanceCheck == 0)&&(version == "feb14")) {Errors_veto_seg_point = Errors_veto_seg_point + 1;} // Not compatible with jan15-release
   if ((veto_segPoint_fVerbose == 0)&&(version == "feb14")) {Errors_veto_seg_point = Errors_veto_seg_point + 1;} // Not compatible with jan15-release
   if ((veto_segPoint_fDefaultType == 0)&&(version == "feb14")) {Errors_veto_seg_point = Errors_veto_seg_point + 1;} // Not compatible with jan15-release
   if (veto_segPoint_fTrackID == 0) {Errors_veto_seg_point = Errors_veto_seg_point + 1;}
   if (veto_segPoint_fEventId == 0) {Errors_veto_seg_point = Errors_veto_seg_point + 1;}
   if (veto_segPoint_fPx == 0) {Errors_veto_seg_point = Errors_veto_seg_point + 1;}
   if (veto_segPoint_fPy == 0) {Errors_veto_seg_point = Errors_veto_seg_point + 1;}
   if (veto_segPoint_fPz == 0) {Errors_veto_seg_point = Errors_veto_seg_point + 1;}
   if (veto_segPoint_fTime == 0) {Errors_veto_seg_point = Errors_veto_seg_point + 1;}
   if (veto_segPoint_fLength == 0) {Errors_veto_seg_point = Errors_veto_seg_point + 1;}
   if (veto_segPoint_fELoss == 0) {Errors_veto_seg_point = Errors_veto_seg_point + 1;}
   if (veto_segPoint_fDetectorID == 0) {Errors_veto_seg_point = Errors_veto_seg_point + 1;}
   if (veto_segPoint_fX == 0) {Errors_veto_seg_point = Errors_veto_seg_point + 1;}
   if (veto_segPoint_fY == 0) {Errors_veto_seg_point = Errors_veto_seg_point + 1;}
   if (veto_segPoint_fZ == 0) {Errors_veto_seg_point = Errors_veto_seg_point + 1;}
   if (veto_segPoint_fX_out == 0) {Errors_veto_seg_point = Errors_veto_seg_point + 1;}
   if (veto_segPoint_fY_out == 0) {Errors_veto_seg_point = Errors_veto_seg_point + 1;}
   if (veto_segPoint_fZ_out == 0) {Errors_veto_seg_point = Errors_veto_seg_point + 1;}
   if (veto_segPoint_fPx_out == 0) {Errors_veto_seg_point = Errors_veto_seg_point + 1;}
   if (veto_segPoint_fPy_out == 0) {Errors_veto_seg_point = Errors_veto_seg_point + 1;}
   if (veto_segPoint_fPz_out == 0) {Errors_veto_seg_point = Errors_veto_seg_point + 1;}
   if (veto_segPoint_fSector == 0) {Errors_veto_seg_point = Errors_veto_seg_point + 1;}
   if (veto_segPoint_fPaddleNb == 0) {Errors_veto_seg_point = Errors_veto_seg_point + 1;}
   if (veto_segPoint_fPaddleTyp == 0) {Errors_veto_seg_point = Errors_veto_seg_point + 1;}
   if (veto_segPoint_fLightYield == 0) {Errors_veto_seg_point = Errors_veto_seg_point + 1;}
   
   // Leaves declaration for Nveto_seg-constructor NOTE: 
   TLeaf* Nveto_segPoint_fUniqueID = cbmsim->FindLeaf("Nveto_segPoint.fUniqueID"); // UInt_t[n] //
   TLeaf* Nveto_segPoint_fBits = cbmsim->FindLeaf("Nveto_segPoint.fBits"); // UInt_t[n] //
   TLeaf* Nveto_segPoint_fLinks = cbmsim->FindLeaf("Nveto_segPoint.fLinks"); // set<FairLink> //
   TLeaf* Nveto_segPoint_fPersistanceCheck = cbmsim->FindLeaf("Nveto_segPoint.fPersistanceCheck"); // Bool_t[n] //
   TLeaf* Nveto_segPoint_fVerbose = cbmsim->FindLeaf("Nveto_segPoint.fVerbose"); // Int_t[n] //
   TLeaf* Nveto_segPoint_fDefaultType = cbmsim->FindLeaf("Nveto_segPoint.fDefaultType"); // Int_t[n] //
   TLeaf* Nveto_segPoint_fTrackID = cbmsim->FindLeaf("Nveto_segPoint.fTrackID"); // Int_t[n] // Some number to identify the track. could be just a numerical artifact or a particle ID.
   TLeaf* Nveto_segPoint_fEventId = cbmsim->FindLeaf("Nveto_segPoint.fEventId"); // UInt_t[n] //
   TLeaf* Nveto_segPoint_fPx = cbmsim->FindLeaf("Nveto_segPoint.fPx"); // Double32_t[n] // X-momentum [GeV] at entrance of active volume: in this case the segmented VETO. "DIMA"
   TLeaf* Nveto_segPoint_fPy = cbmsim->FindLeaf("Nveto_segPoint.fPy"); // Double32_t[n] // Y-momentum [GeV] at entrance of active volume: in this case the segmented VETO. "DIMA"
   TLeaf* Nveto_segPoint_fPz = cbmsim->FindLeaf("Nveto_segPoint.fPz"); // Double32_t[n] // Z-momentum [GeV] at entrance of active volume: in this case the segmented VETO. "DIMA"
   TLeaf* Nveto_segPoint_fTime = cbmsim->FindLeaf("Nveto_segPoint.fTime"); // Double32_t[n] // Time [ns] since the event started (primary particle generation) (averaged over detector). "DIMA"
   TLeaf* Nveto_segPoint_fLength = cbmsim->FindLeaf("Nveto_segPoint.fLength"); // Double32_t[n] // Total Track length since creation (averaged over detector) [cm]. "DIMA"
   TLeaf* Nveto_segPoint_fELoss = cbmsim->FindLeaf("Nveto_segPoint.fELoss"); // Double32_t[n] // Energy deposit in the detector [GeV]. "DIMA"
   TLeaf* Nveto_segPoint_fDetectorID = cbmsim->FindLeaf("Nveto_segPoint.fDetectorID"); // Int_t[n] // Identification of the detector. Does not say very much, since we know already it is the segmented VETO. "DIMA"
   TLeaf* Nveto_segPoint_fX = cbmsim->FindLeaf("Nveto_segPoint.fX"); // Double32_t[n] // X-coordinate [cm] at entrance of active volume: in this case the segmented VETO. "DIMA"
   TLeaf* Nveto_segPoint_fY = cbmsim->FindLeaf("Nveto_segPoint.fY"); // Double32_t[n] // Y-coordinate [cm] at entrance of active volume: in this case the segmented VETO. "DIMA"
   TLeaf* Nveto_segPoint_fZ = cbmsim->FindLeaf("Nveto_segPoint.fZ"); // Double32_t[n] // Z-coordinate [cm] at entrance of active volume: in this case the segmented VETO. "DIMA"
   TLeaf* Nveto_segPoint_fX_out = cbmsim->FindLeaf("Nveto_segPoint.fX_out"); // Double32_t[n] // X-coordinate [cm] at exit of active volume: in this case the segmented VETO. "DIMA"
   TLeaf* Nveto_segPoint_fY_out = cbmsim->FindLeaf("Nveto_segPoint.fY_out"); // Double32_t[n] // Y-coordinate [cm] at exit of active volume: in this case the segmented VETO. "DIMA"
   TLeaf* Nveto_segPoint_fZ_out = cbmsim->FindLeaf("Nveto_segPoint.fZ_out"); // Double32_t[n] // Z-coordinate [cm] at exit of active volume: in this case the segmented VETO. "DIMA"
   TLeaf* Nveto_segPoint_fPx_out = cbmsim->FindLeaf("Nveto_segPoint.fPx_out"); // Double32_t[n] // X-momentum [GeV] at exit of active volume: in this case the segmented VETO. "DIMA"
   TLeaf* Nveto_segPoint_fPy_out = cbmsim->FindLeaf("Nveto_segPoint.fPy_out"); // Double32_t[n] // Y-momentum [GeV] at exit of active volume: in this case the segmented VETO. "DIMA"
   TLeaf* Nveto_segPoint_fPz_out = cbmsim->FindLeaf("Nveto_segPoint.fPz_out"); // Double32_t[n] // Z-momentum [GeV] at exit of active volume: in this case the segmented VETO. "DIMA"
   TLeaf* Nveto_segPoint_fEventID = cbmsim->FindLeaf("Nveto_segPoint.fEventID"); // Int_t[n] // 
   TLeaf* Nveto_segPoint_fMot0TrackID = cbmsim->FindLeaf("Nveto_segPoint.fMot0TrackID"); // Int_t[n] // 
   TLeaf* Nveto_segPoint_fDetID = cbmsim->FindLeaf("Nveto_segPoint.fDetID"); // Int_t[n] // 
   TLeaf* Nveto_segPoint_fCellID = cbmsim->FindLeaf("Nveto_segPoint.fCellID"); // Int_t[n] // 
   TLeaf* Nveto_segPoint_fSegID = cbmsim->FindLeaf("Nveto_segPoint.fSegID"); // Int_t[n] // 
   TLeaf* Nveto_segPoint_fMass = cbmsim->FindLeaf("Nveto_segPoint.fMass"); // Double_t[n] // 
   
   // Checking if the declaration worked:
   if (Nveto_segPoint_fUniqueID == 0) {Errors_Nveto_seg_point = Errors_Nveto_seg_point + 1;}
   if (Nveto_segPoint_fBits == 0) {Errors_Nveto_seg_point = Errors_Nveto_seg_point + 1;}
   if ((Nveto_segPoint_fLinks == 0)&&(version == "feb14")) {Errors_Nveto_seg_point = Errors_Nveto_seg_point + 1;} // Not compatible with jan15-release
   if ((Nveto_segPoint_fPersistanceCheck == 0)&&(version == "feb14")) {Errors_Nveto_seg_point = Errors_Nveto_seg_point + 1;} // Not compatible with jan15-release
   if ((Nveto_segPoint_fVerbose == 0)&&(version == "feb14")) {Errors_Nveto_seg_point = Errors_Nveto_seg_point + 1;} // Not compatible with jan15-release
   if ((Nveto_segPoint_fDefaultType == 0)&&(version == "feb14")) {Errors_Nveto_seg_point = Errors_Nveto_seg_point + 1;} // Not compatible with jan15-release
   if (Nveto_segPoint_fTrackID == 0) {Errors_Nveto_seg_point = Errors_Nveto_seg_point + 1;}
   if (Nveto_segPoint_fEventId == 0) {Errors_Nveto_seg_point = Errors_Nveto_seg_point + 1;}
   if (Nveto_segPoint_fPx == 0) {Errors_Nveto_seg_point = Errors_Nveto_seg_point + 1;}
   if (Nveto_segPoint_fPy == 0) {Errors_Nveto_seg_point = Errors_Nveto_seg_point + 1;}
   if (Nveto_segPoint_fPz == 0) {Errors_Nveto_seg_point = Errors_Nveto_seg_point + 1;}
   if (Nveto_segPoint_fTime == 0) {Errors_Nveto_seg_point = Errors_Nveto_seg_point + 1;}
   if (Nveto_segPoint_fLength == 0) {Errors_Nveto_seg_point = Errors_Nveto_seg_point + 1;}
   if (Nveto_segPoint_fELoss == 0) {Errors_Nveto_seg_point = Errors_Nveto_seg_point + 1;}
   if (Nveto_segPoint_fDetectorID == 0) {Errors_Nveto_seg_point = Errors_Nveto_seg_point + 1;}
   if (Nveto_segPoint_fX == 0) {Errors_Nveto_seg_point = Errors_Nveto_seg_point + 1;}
   if (Nveto_segPoint_fY == 0) {Errors_Nveto_seg_point = Errors_Nveto_seg_point + 1;}
   if (Nveto_segPoint_fZ == 0) {Errors_Nveto_seg_point = Errors_Nveto_seg_point + 1;}
   if (Nveto_segPoint_fX_out == 0) {Errors_Nveto_seg_point = Errors_Nveto_seg_point + 1;}
   if (Nveto_segPoint_fY_out == 0) {Errors_Nveto_seg_point = Errors_Nveto_seg_point + 1;}
   if (Nveto_segPoint_fZ_out == 0) {Errors_Nveto_seg_point = Errors_Nveto_seg_point + 1;}
   if (Nveto_segPoint_fPx_out == 0) {Errors_Nveto_seg_point = Errors_Nveto_seg_point + 1;}
   if (Nveto_segPoint_fPy_out == 0) {Errors_Nveto_seg_point = Errors_Nveto_seg_point + 1;}
   if (Nveto_segPoint_fPz_out == 0) {Errors_Nveto_seg_point = Errors_Nveto_seg_point + 1;}
   if (Nveto_segPoint_fEventID == 0) {Errors_Nveto_seg_point = Errors_Nveto_seg_point + 1;}
   if (Nveto_segPoint_fMot0TrackID == 0) {Errors_Nveto_seg_point = Errors_Nveto_seg_point + 1;}
   if (Nveto_segPoint_fDetID == 0) {Errors_Nveto_seg_point = Errors_Nveto_seg_point + 1;}
   if (Nveto_segPoint_fCellID == 0) {Errors_Nveto_seg_point = Errors_Nveto_seg_point + 1;}
   if (Nveto_segPoint_fSegID == 0) {Errors_Nveto_seg_point = Errors_Nveto_seg_point + 1;}
   if (Nveto_segPoint_fMass == 0) {Errors_Nveto_seg_point = Errors_Nveto_seg_point + 1;}
   
   // -----------------------------------------------------
   // The following leafs are also associated with the segmented VETO: They give precise information on the coordinates of the first few hits of a particle in the segmented VETO. x,y,z,t are all self-expolanatory. "DIMA".
   
   // Declaration of leafs for veto_seg-constructor NOTE:
   TLeaf* veto_segFirstHits_fUniqueID = cbmsim->FindLeaf("veto_segFirstHits.fUniqueID"); // UInt_t[n] //
   TLeaf* veto_segFirstHits_fBits = cbmsim->FindLeaf("veto_segFirstHits.fBits"); // UInt_t[n] //
   TLeaf* veto_segFirstHits_fx0 = cbmsim->FindLeaf("veto_segFirstHits.fx0"); // Double32_t[n] //
   TLeaf* veto_segFirstHits_fy0 = cbmsim->FindLeaf("veto_segFirstHits.fy0"); // Double32_t[n] //
   TLeaf* veto_segFirstHits_fz0 = cbmsim->FindLeaf("veto_segFirstHits.fz0"); // Double32_t[n] //
   TLeaf* veto_segFirstHits_fT0 = cbmsim->FindLeaf("veto_segFirstHits.fT0"); // Double32_t[n] //
   TLeaf* veto_segFirstHits_fx1 = cbmsim->FindLeaf("veto_segFirstHits.fx1"); // Double32_t[n] //
   TLeaf* veto_segFirstHits_fy1 = cbmsim->FindLeaf("veto_segFirstHits.fy1"); // Double32_t[n] //
   TLeaf* veto_segFirstHits_fz1 = cbmsim->FindLeaf("veto_segFirstHits.fz1"); // Double32_t[n] //
   TLeaf* veto_segFirstHits_fT1 = cbmsim->FindLeaf("veto_segFirstHits.fT1"); // Double32_t[n] //
   TLeaf* veto_segFirstHits_fx2 = cbmsim->FindLeaf("veto_segFirstHits.fx2"); // Double32_t[n] //
   TLeaf* veto_segFirstHits_fy2 = cbmsim->FindLeaf("veto_segFirstHits.fy2"); // Double32_t[n] //
   TLeaf* veto_segFirstHits_fz2 = cbmsim->FindLeaf("veto_segFirstHits.fz2"); // Double32_t[n] //
   TLeaf* veto_segFirstHits_fT2 = cbmsim->FindLeaf("veto_segFirstHits.fT2"); // Double32_t[n] //
   TLeaf* veto_segFirstHits_fx3 = cbmsim->FindLeaf("veto_segFirstHits.fx3"); // Double32_t[n] //
   TLeaf* veto_segFirstHits_fy3 = cbmsim->FindLeaf("veto_segFirstHits.fy3"); // Double32_t[n] //
   TLeaf* veto_segFirstHits_fz3 = cbmsim->FindLeaf("veto_segFirstHits.fz3"); // Double32_t[n] //
   TLeaf* veto_segFirstHits_fT3 = cbmsim->FindLeaf("veto_segFirstHits.fT3"); // Double32_t[n] //
   TLeaf* veto_segFirstHits_fx4 = cbmsim->FindLeaf("veto_segFirstHits.fx4"); // Double32_t[n] //
   TLeaf* veto_segFirstHits_fy4 = cbmsim->FindLeaf("veto_segFirstHits.fy4"); // Double32_t[n] //
   TLeaf* veto_segFirstHits_fz4 = cbmsim->FindLeaf("veto_segFirstHits.fz4"); // Double32_t[n] //
   TLeaf* veto_segFirstHits_fT4 = cbmsim->FindLeaf("veto_segFirstHits.fT4"); // Double32_t[n] //
   TLeaf* veto_segFirstHits_fx5 = cbmsim->FindLeaf("veto_segFirstHits.fx5"); // Double32_t[n] //
   TLeaf* veto_segFirstHits_fy5 = cbmsim->FindLeaf("veto_segFirstHits.fy5"); // Double32_t[n] //
   TLeaf* veto_segFirstHits_fz5 = cbmsim->FindLeaf("veto_segFirstHits.fz5"); // Double32_t[n] //
   TLeaf* veto_segFirstHits_fT5 = cbmsim->FindLeaf("veto_segFirstHits.fT5"); // Double32_t[n] //
   
   // Checking if the declaration worked:
   if (veto_segFirstHits_fUniqueID == 0) {Errors_veto_seg_Hits = Errors_veto_seg_Hits + 1;}
   if (veto_segFirstHits_fBits == 0) {Errors_veto_seg_Hits = Errors_veto_seg_Hits + 1;}
   if (veto_segFirstHits_fx0 == 0) {Errors_veto_seg_Hits = Errors_veto_seg_Hits + 1;}
   if (veto_segFirstHits_fy0 == 0) {Errors_veto_seg_Hits = Errors_veto_seg_Hits + 1;}
   if (veto_segFirstHits_fz0 == 0) {Errors_veto_seg_Hits = Errors_veto_seg_Hits + 1;}
   if (veto_segFirstHits_fT0 == 0) {Errors_veto_seg_Hits = Errors_veto_seg_Hits + 1;}
   if (veto_segFirstHits_fx1 == 0) {Errors_veto_seg_Hits = Errors_veto_seg_Hits + 1;}
   if (veto_segFirstHits_fy1 == 0) {Errors_veto_seg_Hits = Errors_veto_seg_Hits + 1;}
   if (veto_segFirstHits_fz1 == 0) {Errors_veto_seg_Hits = Errors_veto_seg_Hits + 1;}
   if (veto_segFirstHits_fT1 == 0) {Errors_veto_seg_Hits = Errors_veto_seg_Hits + 1;}
   if (veto_segFirstHits_fx2 == 0) {Errors_veto_seg_Hits = Errors_veto_seg_Hits + 1;}
   if (veto_segFirstHits_fy2 == 0) {Errors_veto_seg_Hits = Errors_veto_seg_Hits + 1;}
   if (veto_segFirstHits_fz2 == 0) {Errors_veto_seg_Hits = Errors_veto_seg_Hits + 1;}
   if (veto_segFirstHits_fT2 == 0) {Errors_veto_seg_Hits = Errors_veto_seg_Hits + 1;}
   if (veto_segFirstHits_fx3 == 0) {Errors_veto_seg_Hits = Errors_veto_seg_Hits + 1;}
   if (veto_segFirstHits_fy3 == 0) {Errors_veto_seg_Hits = Errors_veto_seg_Hits + 1;}
   if (veto_segFirstHits_fz3 == 0) {Errors_veto_seg_Hits = Errors_veto_seg_Hits + 1;}
   if (veto_segFirstHits_fT3 == 0) {Errors_veto_seg_Hits = Errors_veto_seg_Hits + 1;}
   if (veto_segFirstHits_fx4 == 0) {Errors_veto_seg_Hits = Errors_veto_seg_Hits + 1;}
   if (veto_segFirstHits_fy4 == 0) {Errors_veto_seg_Hits = Errors_veto_seg_Hits + 1;}
   if (veto_segFirstHits_fz4 == 0) {Errors_veto_seg_Hits = Errors_veto_seg_Hits + 1;}
   if (veto_segFirstHits_fT4 == 0) {Errors_veto_seg_Hits = Errors_veto_seg_Hits + 1;}
   if (veto_segFirstHits_fx5 == 0) {Errors_veto_seg_Hits = Errors_veto_seg_Hits + 1;}
   if (veto_segFirstHits_fy5 == 0) {Errors_veto_seg_Hits = Errors_veto_seg_Hits + 1;}
   if (veto_segFirstHits_fz5 == 0) {Errors_veto_seg_Hits = Errors_veto_seg_Hits + 1;}
   if (veto_segFirstHits_fT5 == 0) {Errors_veto_seg_Hits = Errors_veto_seg_Hits + 1;}
   
   // ----------------------------------------------------------
   // The following leafs are some general leafs. They are NOT arrays looping
   // over interaction points per event, so they must be general. But it is 
   // unclear what there function is...
   
   // Declaration of leafs:
   TLeaf* MCEventHeader_TNamed_fUniqueID = cbmsim->FindLeaf("MCEventHeader.TNamed.fUniqueID"); // UInt_t //
   TLeaf* MCEventHeader_TNamed_fBits = cbmsim->FindLeaf("MCEventHeader.TNamed.fBits"); // UInt_t //
   TLeaf* MCEventHeader_TNamed_fName = cbmsim->FindLeaf("MCEventHeader.TNamed.fName"); // TString //
   TLeaf* MCEventHeader_TNamed_fTitle = cbmsim->FindLeaf("MCEventHeader.TNamed.fTitle"); // TString //
   TLeaf* MCEventHeader_fRunId = cbmsim->FindLeaf("MCEventHeader.fRunId"); // UInt_t //
   TLeaf* MCEventHeader_fEventId = cbmsim->FindLeaf("MCEventHeader.fEventId"); // UInt_t //
   TLeaf* MCEventHeader_fX = cbmsim->FindLeaf("MCEventHeader.fX"); // Double32_t //
   TLeaf* MCEventHeader_fY = cbmsim->FindLeaf("MCEventHeader.fY"); // Double32_t //
   TLeaf* MCEventHeader_fZ = cbmsim->FindLeaf("MCEventHeader.fZ"); // Double32_t //
   TLeaf* MCEventHeader_fT = cbmsim->FindLeaf("MCEventHeader.fT"); // Double32_t //
   TLeaf* MCEventHeader_fB = cbmsim->FindLeaf("MCEventHeader.fB"); // Double32_t //
   TLeaf* MCEventHeader_fNPrim = cbmsim->FindLeaf("MCEventHeader.fNPrim"); // Int_t //
   TLeaf* MCEventHeader_fIsSet = cbmsim->FindLeaf("MCEventHeader.fIsSet"); // Bool_t //
   TLeaf* MCEventHeader_fRotX = cbmsim->FindLeaf("MCEventHeader.fRotX"); // Double32_t //
   TLeaf* MCEventHeader_fRotY = cbmsim->FindLeaf("MCEventHeader.fRotY"); // Double32_t //
   TLeaf* MCEventHeader_fRotZ = cbmsim->FindLeaf("MCEventHeader.fRotZ"); // Double32_t //
   
   // Checking if the declaration worked:
   if (MCEventHeader_TNamed_fUniqueID == 0) {Errors_header = Errors_header + 1;}
   if (MCEventHeader_TNamed_fBits == 0) {Errors_header = Errors_header + 1;}
   if (MCEventHeader_TNamed_fName == 0) {Errors_header = Errors_header + 1;}
   if (MCEventHeader_TNamed_fTitle == 0) {Errors_header = Errors_header + 1;}
   if (MCEventHeader_fRunId == 0) {Errors_header = Errors_header + 1;}
   if (MCEventHeader_fEventId == 0) {Errors_header = Errors_header + 1;}
   if (MCEventHeader_fX == 0) {Errors_header = Errors_header + 1;}
   if (MCEventHeader_fY == 0) {Errors_header = Errors_header + 1;}
   if (MCEventHeader_fZ == 0) {Errors_header = Errors_header + 1;}
   if (MCEventHeader_fT == 0) {Errors_header = Errors_header + 1;}
   if (MCEventHeader_fB == 0) {Errors_header = Errors_header + 1;}
   if (MCEventHeader_fNPrim == 0) {Errors_header = Errors_header + 1;}
   if (MCEventHeader_fIsSet == 0) {Errors_header = Errors_header + 1;}
   if (MCEventHeader_fRotX == 0) {Errors_header = Errors_header + 1;}
   if (MCEventHeader_fRotY == 0) {Errors_header = Errors_header + 1;}
   if (MCEventHeader_fRotZ == 0) {Errors_header = Errors_header + 1;}

   // ---------------------------------------------------------
   // The following tracks are associated with some general properties of all the specific
   // tracks, like the color whcih they are painted with.The precise purpose is also 
   // not completely clear.
   
   // Declaration of leafs:   
   TLeaf* GeoTracks_fUniqueID = cbmsim->FindLeaf("GeoTracks.fUniqueID"); // UInt_t[n] //
   TLeaf* GeoTracks_fBits = cbmsim->FindLeaf("GeoTracks.fBits"); // UInt_t[n] //
   TLeaf* GeoTracks_fGeoAtt = cbmsim->FindLeaf("GeoTracks.fGeoAtt"); // UInt_t[n] //
   TLeaf* GeoTracks_fLineColor = cbmsim->FindLeaf("GeoTracks.fLineColor"); // Short_t[n] //
   TLeaf* GeoTracks_fLineStyle = cbmsim->FindLeaf("GeoTracks.fLineStyle"); // Short_t[n] //
   TLeaf* GeoTracks_fLineWidth = cbmsim->FindLeaf("GeoTracks.fLineWidth"); // Short_t[n] //
   TLeaf* GeoTracks_fMarkerColor = cbmsim->FindLeaf("GeoTracks.fMarkerColor"); // Short_t[n] //
   TLeaf* GeoTracks_fMarkerStyle = cbmsim->FindLeaf("GeoTracks.fMarkerStyle"); // Short_t[n] //
   TLeaf* GeoTracks_fMarkerSize = cbmsim->FindLeaf("GeoTracks.fMarkerSize"); // Float_t[n] //
   TLeaf* GeoTracks_fPDG = cbmsim->FindLeaf("GeoTracks.fPDG"); // Int_t[n] //
   TLeaf* GeoTracks_fId = cbmsim->FindLeaf("GeoTracks.fId"); // Int_t[n] //
   TLeaf* GeoTracks_fParent = cbmsim->FindLeaf("GeoTracks.fParent"); // TVirtualGeoTrack* //
   TLeaf* GeoTracks_fParticle = cbmsim->FindLeaf("GeoTracks.fParticle"); // TObject* //
   TLeaf* GeoTracks_fTracks = cbmsim->FindLeaf("GeoTracks.fTracks"); // TObjArray* //
   TLeaf* GeoTracks_fPointsSize = cbmsim->FindLeaf("GeoTracks.fPointsSize"); // Int_t[n] //
   TLeaf* GeoTracks_fNpoints = cbmsim->FindLeaf("GeoTracks.fNpoints"); // Int_t[n] //
   TLeaf* GeoTracks_fPoints = cbmsim->FindLeaf("GeoTracks.fPoints"); // Double_t[n] //
   
   // Checking if the declaration worked:
   if (GeoTracks_fUniqueID == 0) {Errors_Geo = Errors_Geo + 1;}
   if (GeoTracks_fBits == 0) {Errors_Geo = Errors_Geo + 1;}
   if (GeoTracks_fGeoAtt == 0) {Errors_Geo = Errors_Geo + 1;}
   if (GeoTracks_fLineColor == 0) {Errors_Geo = Errors_Geo + 1;}
   if (GeoTracks_fLineStyle == 0) {Errors_Geo = Errors_Geo + 1;}
   if (GeoTracks_fLineWidth == 0) {Errors_Geo = Errors_Geo + 1;}
   if (GeoTracks_fMarkerColor == 0) {Errors_Geo = Errors_Geo + 1;}
   if (GeoTracks_fMarkerStyle == 0) {Errors_Geo = Errors_Geo + 1;}
   if (GeoTracks_fMarkerSize == 0) {Errors_Geo = Errors_Geo + 1;}
   if (GeoTracks_fPDG == 0) {Errors_Geo = Errors_Geo + 1;}
   if (GeoTracks_fId == 0) {Errors_Geo = Errors_Geo + 1;}
   if (GeoTracks_fParent == 0) {Errors_Geo = Errors_Geo + 1;}
   if (GeoTracks_fParticle == 0) {Errors_Geo = Errors_Geo + 1;}
   if (GeoTracks_fTracks == 0) {Errors_Geo = Errors_Geo + 1;}
   if (GeoTracks_fPointsSize == 0) {Errors_Geo = Errors_Geo + 1;}
   if (GeoTracks_fNpoints == 0) {Errors_Geo = Errors_Geo + 1;}
   if (GeoTracks_fPoints == 0) {Errors_Geo = Errors_Geo + 1;}
   
   // ---------------------------------------------------------
   
   // Now all the required leafs have been declared.
   
   // ===============================================================================================
   // Now we have declared all the leafs we must process the possible errors. Only if there are no
   // errors we can proceed with the rest of our macro. Otherwise something has to be done
   // to debug the code first:
   Int_t Lconstructor_errors = 0;
   Int_t Lconstructor_errors_hits = 0;
   if (Lconstructor=="Land")
   {
     Lconstructor_errors = Errors_land_point;
     Lconstructor_errors_hits = Errors_land_Hits;
   }
   if (Lconstructor=="NeuLAND")
   {
     Lconstructor_errors = Errors_neuland_point;
     Lconstructor_errors_hits = 0;
   }
   
   Int_t Vconstructor_errors = 0;
   Int_t Vconstructor_errors_hits = 0;
   if (Vconstructor=="veto_seg")
   {
     Vconstructor_errors = Errors_veto_seg_point;
     Vconstructor_errors_hits = Errors_veto_seg_Hits;
   }
   if (Vconstructor=="Nveto_seg")
   {
     Vconstructor_errors = Errors_Nveto_seg_point;
     Vconstructor_errors_hits = 0;
   }
   
   Bool_t Error_tot = (Errors_MC==0)&&(Lconstructor_errors==0)&&(Lconstructor_errors_hits==0)&&(Vconstructor_errors==0)&&(Vconstructor_errors_hits==0)&&(Errors_header==0)&&(Errors_Geo==0);
   
   if (Error_tot == kFALSE)
   {
     cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!FATAL ERROR!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\n";
     
     cout << "The tree could not be read due to some errors in the code.\n"
          << "Please fix these errors first and make sure that every leaf\n"
	  << "of the tree really exists and can be connected to with a\n"
	  << "non-NULL pointer of type TLeaf*.\n\n";
     
     cout << "The amount of detected errors is now given.\n"
          << "MCTrack-leafs = " << Errors_MC << "\n"
	  << "LandPoint-leafs = " << Errors_land_point << "\n"
	  << "landFirstHits-leafs = " << Errors_land_Hits << "\n"
	  << "NeuLandPoint-leafs = " << Errors_neuland_point << "\n"
	  << "veto_segPoint-leafs = " << Errors_veto_seg_point << "\n"
	  << "veto_segFirstHits-leafs = " << Errors_veto_seg_Hits << "\n"
	  << "Nveto_segPoint-leafs = " << Errors_Nveto_seg_point << "\n"
	  << "MCEventHeader-leafs = " << Errors_header << "\n"
	  << "GeoTracks-leafs = " << Errors_Geo << "\n";
     
     cout << "\n\nThe program is now terminated.\n\n\n\n";
   }
   else
   {
   cout << "Connection to the Leafs was ssuccesful. We will now start the pre-processing for the loop.\n\n";
     
   // ===============================================================================================================
   // Now that we know we connected to all leafs succesfully, we can declare the histograms that we want to fill 
   // during the loop over the tree.
  
   // The tree is organized event-by-event. Hence get the total number of events stored in the tree:
   Long64_t nEvents = cbmsim->GetEntries();
   
   // We begin by accessing the information we need for building the histograms:
   Int_t Event_Selection_Condition = Integers->GetBinContent(34);
   Int_t Secondary_Condition = Integers->GetBinContent(32);
   Int_t nbins = Integers->GetBinContent(21);
   Int_t nbinsEs = Integers->GetBinContent(31);
   Int_t nbinstheta = Integers->GetBinContent(28);
   Int_t nbinsphi = Integers->GetBinContent(29);
   Int_t nbinsX = Integers->GetBinContent(30);
   Int_t nbinsTOF = Integers->GetBinContent(33);
   Double_t Emin = Doubles->GetBinContent(11);
   Double_t Emax = Doubles->GetBinContent(12);
   Double_t xcentral = Doubles->GetBinContent(1);
   Double_t ycentral = Doubles->GetBinContent(2);
   Double_t zcentral = Doubles->GetBinContent(3);
   Double_t halfNeuLANDthickness = Doubles->GetBinContent(4);
   Double_t xveto = Doubles->GetBinContent(16);
   Double_t yveto = Doubles->GetBinContent(17);
   Double_t zveto = Doubles->GetBinContent(18);
   Double_t halfVETOwidth = Doubles->GetBinContent(21);
   Double_t halfVETOwidth_x = halfVETOwidth; // For ImPACT.
   Double_t halfVETOwidth_y = halfVETOwidth; // for ImPACT.
   Double_t halfVETOthickness = Doubles->GetBinContent(22);
   Double_t maxtraveldistance = TMath::Sqrt(4.0*halfNeuLANDthickness*halfNeuLANDthickness + 2.0*250.0*250.0);
   Double_t zRange = zveto-50.0;
   Double_t TOFmin = Doubles->GetBinContent(31);
   Double_t TOFmax = Doubles->GetBinContent(32);
   Double_t Tres_Neu = Doubles->GetBinContent(33);
   Double_t Tres_VETO = Doubles->GetBinContent(34);
   Double_t Ethreshold = Doubles->GetBinContent(23);
   Double_t NeuLAND_threshold = Doubles->GetBinContent(35)/1000.0; // Convert threshold back to GeV.
   
   // We perform a liitle reverse-engineering in the case that this code is run after the last VETO-update:
   Double_t vetostep = Doubles->GetBinContent(26);
   Int_t vetocount = Integers->GetBinContent(25);
   Int_t Nvetocounts = Integers->GetBinContent(23);
   if (Nvetocounts==vetocount) {zveto = zveto + vetostep;}
   
   // Compute minimal travel time to the VETO [sec]:
   Double_t MinTravelTime = (zveto - halfVETOthickness)/29979245800.0; // since zveto is in [cm] and we have to go to [sec].
   
   // Declare TH1D efficiency histograms for Energy thresholds:
   TH1D* veto_hist = new TH1D("VETO probability distribution","VETO probability distribution",nbins,Emin,Emax);
   TH1D* Neu_hist = new TH1D("NeuLAND probability distribution","NeuLAND probability distribution",nbins,Emin,Emax);
   TH1D* clean_hist = new TH1D("Complete energy spectrum","Complete energy spectrum",nbins,Emin,Emax);
   TH1D* Real_hist = new TH1D("NeuLAND NOT Veto probability distribution","NeuLAND NOT Veto probability distribution",nbins,Emin,Emax);
   
   // Declare backscattering histograms:
   TH1D* veto_hist_Pass = new TH1D("VETO probability distribution of events that did not touch it","VETO probability distribution of events that did not touch it",nbins,Emin,Emax);
   TH1D* veto_hist_PureBack = new TH1D("VETO probability distribution of pure backscattering events","VETO probability distribution of pure backscattering events",nbins,Emin,Emax);
   TH1D* veto_hist_PurePrim = new TH1D("VETO probability distribution of pure primary events","VETO probability distribution of pure primary events",nbins,Emin,Emax);
   TH1D* veto_hist_PrimBack = new TH1D("VETO probability distribution of events with both primary and backscattering interactions ","VETO probability distributionof events with both primary and backscattering interactions",nbins,Emin,Emax);
   
   // Declare TOF veto histograms:
   TH1D* veto_hist_TOF = new TH1D("VETO probability distribution MC TOF","VETO probability distribution MC TOF",nbins,Emin,Emax);
   TH1D* veto_hist_TOF_Rand = new TH1D("VETO probability distribution TOF 100 ps","VETO probability distribution TOF 100 ps",nbins,Emin,Emax);
   TH1D* veto_hist_TOF_prim = new TH1D("VETO probability distribution MC TOF primary events","VETO probability distribution MC TOF primary events",nbins,Emin,Emax);
   TH1D* veto_hist_TOF_Rand_prim = new TH1D("VETO probability distribution TOF 100 ps primary events","VETO probability distribution TOF 100 ps primary events",nbins,Emin,Emax);
   
   // Declare TH1D efficiency histograms for LandFirstHits:
   TH1D* veto_hist_LFH = new TH1D("VETO probability distribution LFH","VETO probability distribution LFH",nbins,Emin,Emax);
   TH1D* Neu_hist_LFH = new TH1D("NeuLAND probability distribution LFH","NeuLAND probability distribution LFH",nbins,Emin,Emax);
   TH1D* clean_hist_LFH = new TH1D("Complete energy spectrum LFH","Complete energy spectrum LFH",nbins,Emin,Emax);
   TH1D* Real_hist_LFH = new TH1D("NeuLAND NOT Veto probability distribution LFH","NeuLAND NOT Veto probability distribution LFH",nbins,Emin,Emax);
   
   // Declare TH1D phi-histograms for secondary particles:
   TH1D* Phi_detect = new TH1D("LAB-frame Phi-angle of secondary particles from detected event","LAB-frame Phi-angle of secondary particles from detected event",nbinsphi,0,2*TMath::Pi());
   TH1D* Phi_not = new TH1D("LAB-frame Phi-angle of secondary particles from UNdetected event","LAB-frame Phi-angle of secondary particles from UNdetected event",nbinsphi,0,2*TMath::Pi());
   TH1D* Phi_all = new TH1D("LAB-frame Phi-angle of secondary particles from all event","LAB-frame Phi-angle of secondary particles from ALL event",nbinsphi,0,2*TMath::Pi());
   
   // Declare other TH1D histograms:
   TH1D* Secondary_freq = new TH1D("Secondary frequency","Secondary frequency",nEvents,0.0,1.0);
   TH1D* veto_hist_Sec = new TH1D("VETO probability distribution of secondary particles alone","VETO probability distribution of secondary particles alone",nbins,Emin,Emax);
   
   // Declare TH1D histograms for TOF information:
   TH1D* TOF_VETO_MC = new TH1D("MC TOF distribution of VETO detector","MC TOF distribution of VETO detector",nbinsTOF,TOFmin,TOFmax);
   TH1D* TOF_NeuLAND_MC = new TH1D("MC TOF distribution of NeuLAND","MC TOF distribution of NeuLAND",nbinsTOF,TOFmin,TOFmax);
   TH1D* TOF_VETO_Rand = new TH1D("100 ps TOF distribution of VETO detector","100 ps TOF distribution of VETO detector",nbinsTOF,TOFmin,TOFmax);
   TH1D* TOF_NeuLAND_Rand = new TH1D("100 ps TOF distribution of NeuLAND","100 ps TOF distribution of NeuLAND",nbinsTOF,TOFmin,TOFmax);
   
   // Declare TH2D histograms for all secondary particles:
   TH2D* Etheta_detect = new TH2D("Energy VS LAB-theta of secondary particles detected","Energy VS LAB-theta of secondary particles detected",nbinsEs,0.0,Emax,nbinstheta,0.0,TMath::Pi());
   TH2D* Etheta_not = new TH2D("Energy VS LAB-theta of secondary particles UNdetected","Energy VS LAB-theta of secondary particles UNdetected",nbinsEs,0.0,Emax,nbinstheta,0.0,TMath::Pi());
   TH2D* Etheta_all = new TH2D("Energy VS LAB-theta of secondary particles ALL","Energy VS LAB-theta of secondary particles ALL",nbinsEs,0.0,Emax,nbinstheta,0.0,TMath::Pi());
   TH2D* Ez_detect = new TH2D("Energy VS z-coordinate of production point of secondary particles detected","Energy VS z-coordinate of production point of secondary particles detected",nbinsEs,0.0,Emax,nbinsX,zcentral-halfNeuLANDthickness,zcentral+halfNeuLANDthickness);
   TH2D* Ez_not = new TH2D("Energy VS z-coordinate of production point of secondary particles UNdetected","Energy VS z-coordinate of production point of secondary particles UNdetected",nbinsEs,0.0,Emax,nbinsX,zcentral-halfNeuLANDthickness,zcentral+halfNeuLANDthickness);
   TH2D* Ez_all = new TH2D("Energy VS z-coordinate of production point of secondary particles ALL","Energy VS z-coordinate of production point of secondary particles ALL",nbinsEs,0.0,Emax,nbinsX,zcentral-halfNeuLANDthickness,zcentral+halfNeuLANDthickness);
   
   // Declare TH2D histograms for secondary particles that hit the VETO:
   TH2D* EzHIT_proton = new TH2D("Energy VS z-point VS travel distance to VETO for secondary PROTONS Hits","Energy VS z-point VS travel distance to VETO for secondary PROTONS Hits",nbinsEs,0.0,Emax,nbinsX,zRange,zcentral+halfNeuLANDthickness);
   TH2D* EzHIT_neutron = new TH2D("Energy VS z-point VS travel distance to VETO for secondary NEUTRONS Hits","Energy VS z-point VS travel distance to VETO for secondary NEUTRONS Hits",nbinsEs,0.0,Emax,nbinsX,zRange,zcentral+halfNeuLANDthickness);
   TH2D* EzHIT_electron = new TH2D("Energy VS z-point VS travel distance to VETO for secondary ELECTRONS Hits","Energy VS z-point VS travel distance to VETO for secondary ELECTRONS Hits",nbinsEs,0.0,Emax,nbinsX,zRange,zcentral+halfNeuLANDthickness);
   TH2D* EzHIT_gamma = new TH2D("Energy VS z-point VS travel distance to VETO for secondary GAMMAS Hits","Energy VS z-point VS travel distance to VETO for secondary GAMMAS Hits",nbinsEs,0.0,Emax,nbinsX,zRange,zcentral+halfNeuLANDthickness);
   TH2D* EzHIT_pion = new TH2D("Energy VS z-point VS travel distance to VETO for secondary PIONS Hits","Energy VS z-point VS travel distance to VETO for secondary PIONS Hits",nbinsEs,0.0,Emax,nbinsX,zRange,zcentral+halfNeuLANDthickness);
   TH2D* EzHIT_muon = new TH2D("Energy VS z-point VS travel distance to VETO for secondary MUONS Hits","Energy VS z-point VS travel distance to VETO for secondary MUONS Hits",nbinsEs,0.0,Emax,nbinsX,zRange,zcentral+halfNeuLANDthickness);
   TH2D* EzHIT_Cion = new TH2D("Energy VS z-point VS travel distance to VETO for secondary C-IONS Hits","Energy VS z-point VS travel distance to VETO for secondary C-IONS Hits",nbinsEs,0.0,Emax,nbinsX,zRange,zcentral+halfNeuLANDthickness);
   TH2D* EzHIT_other = new TH2D("Energy VS z-point VS travel distance to VETO for secondary REMAINDERS Hits","Energy VS z-point VS travel distance to VETO for secondary REMAINDERS Hits",nbinsEs,0.0,Emax,nbinsX,zRange,zcentral+halfNeuLANDthickness);

    // Declare TH2D histograms for secondary particles that hit the VETO with enough energy deposition:
   TH2D* EzDET_proton = new TH2D("Energy VS z-point VS travel distance to VETO for secondary PROTONS Detected","Energy VS z-point VS travel distance to VETO for secondary PROTONS Detected",nbinsEs,0.0,Emax,nbinsX,zRange,zcentral+halfNeuLANDthickness);
   TH2D* EzDET_neutron = new TH2D("Energy VS z-point VS travel distance to VETO for secondary NEUTRONS Detected","Energy VS z-point VS travel distance to VETO for secondary NEUTRONS Detected",nbinsEs,0.0,Emax,nbinsX,zRange,zcentral+halfNeuLANDthickness);
   TH2D* EzDET_electron = new TH2D("Energy VS z-point VS travel distance to VETO for secondary ELECTRONS Detected","Energy VS z-point VS travel distance to VETO for secondary ELECTRONS Detected",nbinsEs,0.0,Emax,nbinsX,zRange,zcentral+halfNeuLANDthickness);
   TH2D* EzDET_gamma = new TH2D("Energy VS z-point VS travel distance to VETO for secondary GAMMAS Detected","Energy VS z-point VS travel distance to VETO for secondary GAMMAS Detected",nbinsEs,0.0,Emax,nbinsX,zRange,zcentral+halfNeuLANDthickness);
   TH2D* EzDET_pion = new TH2D("Energy VS z-point VS travel distance to VETO for secondary PIONS Detected","Energy VS z-point VS travel distance to VETO for secondary PIONS Detected",nbinsEs,0.0,Emax,nbinsX,zRange,zcentral+halfNeuLANDthickness);
   TH2D* EzDET_muon = new TH2D("Energy VS z-point VS travel distance to VETO for secondary MUONS Detected","Energy VS z-point VS travel distance to VETO for secondary MUONS Detected",nbinsEs,0.0,Emax,nbinsX,zRange,zcentral+halfNeuLANDthickness);
   TH2D* EzDET_Cion = new TH2D("Energy VS z-point VS travel distance to VETO for secondary C-IONS Detected","Energy VS z-point VS travel distance to VETO for secondary C-IONS Detected",nbinsEs,0.0,Emax,nbinsX,zRange,zcentral+halfNeuLANDthickness);
   TH2D* EzDET_other = new TH2D("Energy VS z-point VS travel distance to VETO for secondary REMAINDERS Detected","Energy VS z-point VS travel distance to VETO for secondary REMAINDERS Detected",nbinsEs,0.0,Emax,nbinsX,zRange,zcentral+halfNeuLANDthickness);
   
   // Declare TH3D histograms for secondary protons:
   TH3D* EzdALL_proton = new TH3D("Energy VS z-point VS travel distance for secondary PROTONS","Energy VS z-point VS travel distance for secondary PROTONS",nbinsEs,0.0,Emax,nbinsX,zRange,zcentral+halfNeuLANDthickness,nbinsX,0.0,maxtraveldistance);
   TH3D* EzdAIM_proton = new TH3D("Energy VS z-point VS travel distance to VETO for secondary PROTONS direction","Energy VS z-point VS travel distance to VETO for secondary PROTONS direction",nbinsEs,0.0,Emax,nbinsX,zRange,zcentral+halfNeuLANDthickness,nbinsX,0.0,maxtraveldistance);
   TH3D* EzdREACH_proton = new TH3D("Energy VS z-point VS travel distance to VETO for secondary PROTONS","Energy VS z-point VS travel distance to VETO for secondary PROTONS",nbinsEs,0.0,Emax,nbinsX,zRange,zcentral+halfNeuLANDthickness,nbinsX,0.0,maxtraveldistance);
   
   // Declare TH3D histograms for secondary neutrons:
   TH3D* EzdALL_neutron = new TH3D("Energy VS z-point VS travel distance for secondary NEUTRONS","Energy VS z-point VS travel distance for secondary NEUTRONS",nbinsEs,0.0,Emax,nbinsX,zRange,zcentral+halfNeuLANDthickness,nbinsX,0.0,maxtraveldistance);
   TH3D* EzdAIM_neutron = new TH3D("Energy VS z-point VS travel distance to VETO for secondary NEUTRONS direction","Energy VS z-point VS travel distance to VETO for secondary NEUTRONS direction",nbinsEs,0.0,Emax,nbinsX,zRange,zcentral+halfNeuLANDthickness,nbinsX,0.0,maxtraveldistance);
   TH3D* EzdREACH_neutron = new TH3D("Energy VS z-point VS travel distance to VETO for secondary NEUTRONS","Energy VS z-point VS travel distance to VETO for secondary NEUTRONS",nbinsEs,0.0,Emax,nbinsX,zRange,zcentral+halfNeuLANDthickness,nbinsX,0.0,maxtraveldistance);
   
   // Declare TH3D histograms for secondary electrons:
   TH3D* EzdALL_electron = new TH3D("Energy VS z-point VS travel distance for secondary ELECTRONS","Energy VS z-point VS travel distance for secondary ELECTRONS",nbinsEs,0.0,Emax,nbinsX,zRange,zcentral+halfNeuLANDthickness,nbinsX,0.0,maxtraveldistance);
   TH3D* EzdAIM_electron = new TH3D("Energy VS z-point VS travel distance to VETO for secondary ELECTRONS direction","Energy VS z-point VS travel distance to VETO for secondary ELECTRONS direction",nbinsEs,0.0,Emax,nbinsX,zRange,zcentral+halfNeuLANDthickness,nbinsX,0.0,maxtraveldistance);
   TH3D* EzdREACH_electron = new TH3D("Energy VS z-point VS travel distance to VETO for secondary ELECTRONS","Energy VS z-point VS travel distance to VETO for secondary ELECTRONS",nbinsEs,0.0,Emax,nbinsX,zRange,zcentral+halfNeuLANDthickness,nbinsX,0.0,maxtraveldistance);
  
   // Declare TH3D histograms for secondary gammas:
   TH3D* EzdALL_gamma = new TH3D("Energy VS z-point VS travel distance for secondary GAMMAS","Energy VS z-point VS travel distance for secondary GAMMAS",nbinsEs,0.0,Emax,nbinsX,zRange,zcentral+halfNeuLANDthickness,nbinsX,0.0,maxtraveldistance);
   TH3D* EzdAIM_gamma = new TH3D("Energy VS z-point VS travel distance to VETO for secondary GAMMAS direction","Energy VS z-point VS travel distance to VETO for secondary GAMMAS direction",nbinsEs,0.0,Emax,nbinsX,zRange,zcentral+halfNeuLANDthickness,nbinsX,0.0,maxtraveldistance);
   TH3D* EzdREACH_gamma = new TH3D("Energy VS z-point VS travel distance to VETO for secondary GAMMAS","Energy VS z-point VS travel distance to VETO for secondary GAMMAS",nbinsEs,0.0,Emax,nbinsX,zRange,zcentral+halfNeuLANDthickness,nbinsX,0.0,maxtraveldistance);

   // Declare TH3D histograms for secondary pions (store theta ipv traveldist):
   TH3D* EzdALL_pion = new TH3D("Energy VS z-point VS travel distance for secondary PIONs","Energy VS z-point VS travel distance for secondary PIONS",nbinsEs,0.0,Emax,nbinsX,zRange,zcentral+halfNeuLANDthickness,nbinsX,0.0,maxtraveldistance);
   TH3D* EzdAIM_pion = new TH3D("Energy VS z-point VS travel distance to VETO for secondary PIONS direction","Energy VS z-point VS travel distance to VETO for secondary PIONS direction",nbinsEs,0.0,Emax,nbinsX,zRange,zcentral+halfNeuLANDthickness,nbinsX,0.0,maxtraveldistance);
   TH3D* EzdREACH_pion = new TH3D("Energy VS z-point VS travel distance to VETO for secondary PIONS","Energy VS z-point VS travel distance to VETO for secondary PIONS",nbinsEs,0.0,Emax,nbinsX,zRange,zcentral+halfNeuLANDthickness,nbinsX,0.0,maxtraveldistance);

   // Declare TH3D histograms for secondary muons (store theta ipv traveldist):
   TH3D* EzdALL_muon = new TH3D("Energy VS z-point VS travel distance for secondary MUONS","Energy VS z-point VS travel distance for secondary MUONS",nbinsEs,0.0,Emax,nbinsX,zRange,zcentral+halfNeuLANDthickness,nbinsX,0.0,maxtraveldistance);
   TH3D* EzdAIM_muon = new TH3D("Energy VS z-point VS travel distance to VETO for secondary MUONS direction","Energy VS z-point VS travel distance to VETO for secondary MUONS direction",nbinsEs,0.0,Emax,nbinsX,zRange,zcentral+halfNeuLANDthickness,nbinsX,0.0,maxtraveldistance);
   TH3D* EzdREACH_muon = new TH3D("Energy VS z-point VS travel distance to VETO for secondary MUONS","Energy VS z-point VS travel distance to VETO for secondary MUONS",nbinsEs,0.0,Emax,nbinsX,zRange,zcentral+halfNeuLANDthickness,nbinsX,0.0,maxtraveldistance);
   
   // Declare TH3D histograms for secondary C-ions:
   TH3D* EzdALL_Cion = new TH3D("Energy VS z-point VS travel distance for secondary C-IONS","Energy VS z-point VS travel distance for secondary C-IONS",nbinsEs,0.0,Emax,nbinsX,zRange,zcentral+halfNeuLANDthickness,nbinsX,0.0,maxtraveldistance);
   TH3D* EzdAIM_Cion = new TH3D("Energy VS z-point VS travel distance to VETO for secondary C-IONS direction","Energy VS z-point VS travel distance to VETO for secondary C-IONS direction",nbinsEs,0.0,Emax,nbinsX,zRange,zcentral+halfNeuLANDthickness,nbinsX,0.0,maxtraveldistance);
   TH3D* EzdREACH_Cion = new TH3D("Energy VS z-point VS travel distance to VETO for secondary C-IONS","Energy VS z-point VS travel distance to VETO for secondary C-IONS",nbinsEs,0.0,Emax,nbinsX,zRange,zcentral+halfNeuLANDthickness,nbinsX,0.0,maxtraveldistance);
   
   // Declare TH3D histograms for secondary Other Ions:
   TH3D* EzdALL_other = new TH3D("Energy VS z-point VS travel distance for secondary REMAINDERS","Energy VS z-point VS travel distance for secondary REMAINDERS",nbinsEs,0.0,Emax,nbinsX,zRange,zcentral+halfNeuLANDthickness,nbinsX,0.0,maxtraveldistance);
   TH3D* EzdAIM_other = new TH3D("Energy VS z-point VS travel distance to VETO for secondary REMAINDERS direction","Energy VS z-point VS travel distance to VETO for secondary REMAINDERS direction",nbinsEs,0.0,Emax,nbinsX,zRange,zcentral+halfNeuLANDthickness,nbinsX,0.0,maxtraveldistance);

   // ==================================================================================================
   // We also need to create some arrays to store the energy per scintillator bar in. This is only required for the
   // energy deposition analysis.
   
   // Number of bars in NeuLAND (fixed): NOTE: we add one (also for veto) to fix the problem whether fSector
   // starts at 1 or 0. Since we do everythin consistently, there is no consequence: we still mean the same bar!
   Int_t const Neubars_i = 3000;
   
   // For the VETO we will have to compute the total number of bars. Hence we do:
   Double_t VETO_paddle_dimx = Doubles->GetBinContent(21);         // half of the bar length [cm] 2.5 m.
   Double_t VETO_paddle_dimy = Doubles->GetBinContent(19) + 0.1;   // half of the bar width [cm] 5 cm.
   Double_t VETO_paddle_dimz = Doubles->GetBinContent(20) + 0.1;   // half of the bar depth [cm] 5 cm.
   Double_t VETO_fulldepth = Doubles->GetBinContent(22)*2.0;      // full detector depth.
   
   // Now the length of a bar is the full length of a plane, so the number of bars in one single plane is:
   Double_t nBars1singlePlane = (2.0*VETO_paddle_dimx)/(2.0*VETO_paddle_dimy);
   
   // Then the number of sinle-planes is obviously:
   Double_t nSinlePlanes = VETO_fulldepth/(2.0*VETO_paddle_dimz);
   cout << "Number of single-planes in the VETO = " << nSinlePlanes << "\n";
   
   // Hence the number of bars in the veto will be:
   Int_t const vetobars_i = (Double_t) (nBars1singlePlane*nSinlePlanes);
   
   // Now make the array dimensions constant:
   Int_t const Neubars = Neubars_i;
   Int_t const vetobars = vetobars_i;
   
   // Then decalre the arrays:150
   Double_t NeuArray [Neubars];
   Double_t VetoArray [vetobars];
   
   // Give some output on our calculation:
   cout << "Total number of bars in the VETO = " << vetobars << "\n\n";
   
   // So now we have the arrays and the energy thresholds, meaning that everything is set for our analysis.
   
   // ==============================================================================================  
   // The last thing required to do is to declare the variables we will be using inside the loop:

   // Create the nBytes variable needed for the GetEntry(i)-statement. The GetEntry(i)
   // Loads the number of bytes used to store entry (i) in nBytes AND it loads the relevant
   // information into the declared leafs by using the branch adresses:
   Long64_t nbytes = 0;
   
   // Declare histograms that are used in the loop but NOT kept afterwards:
   TH1D* repeat = new TH1D("repeat","repeat",MCTrack_ArraySize,0.0,1.0);
   TH1D* repeatE = new TH1D("repeatE","repeatE",MCTrack_ArraySize,0.0,1.0);
   TH2D* Edep = new TH2D("Edep","Edep",MCTrack_ArraySize,0.0,1.0,vetobars,0.0,1.0);
   
   // Declare random number generator:
   TRandom3* generator = new TRandom3();
   // The one can get a random number with
   // Double_t number = generator->Gaus(mean,sigma);
   // other distributions are also provided.
   
   // Declaration of doubles:
   Double_t Eparticle = 0.0;
   Double_t fMass = 0.0;
   Double_t light = 0.0;
   Double_t px = 0.0;
   Double_t py = 0.0;
   Double_t pz = 0.0;
   Double_t theta = 0.0;
   Double_t phi = 0.0;
   Double_t Esecondary = 0.0;
   Double_t zpoint = 0.0;
   Double_t xpoint = 0.0;
   Double_t ypoint = 0.0;
   Double_t xtravel = 0.0;
   Double_t ytravel = 0.0;
   Double_t ztravel = 0.0;
   Double_t traveldistance = 0.0;
   Double_t repeat_test = 0;
   Double_t LFH_T = 0.0;
   Double_t LFH_X = 0.0;
   Double_t LFH_Y = 0.0;
   Double_t LFH_Z = 0.0;
   Double_t VFH_T = 0.0;
   Double_t VFH_X = 0.0;
   Double_t VFH_Y = 0.0;
   Double_t VFH_Z = 0.0;
   Double_t E_add = 0.0;
   Double_t Airdistance = 0.0;
   Double_t TOF_veto = 0.0;
   Double_t TOF_Neu = 0.0;
   Double_t TOF_veto_max = 0.0;
   Double_t TOF_Neu_max = 0.0;
   Double_t time = 0.0;
   
   // Declaration of integers:
   Int_t selected_paddle = 0;
   Int_t ArraySize = 0;
   Int_t PrimParticleCount = 0;
   Int_t PrimParticleIndex = 0;
   Int_t nParticles = Integers->GetBinContent(3);
   Int_t MotherID = 0;
   Int_t PDG = 0;
   Int_t TrackID = 0;
   Int_t VETObarNr = 0;
   Int_t TOF_TrackID = 0;
   
   // Declaration of boolians:
   Bool_t veto_detect = kFALSE;
   Bool_t Neu_detect = kFALSE;
   Bool_t Real_detect = kFALSE;
   Bool_t veto_detect_LFH = kFALSE;
   Bool_t Neu_detect_LFH = kFALSE;
   Bool_t Real_detect_LFH = kFALSE;
   Bool_t vetohit = kFALSE;
   Bool_t Secondary_Select = kFALSE;
   Bool_t Event_Select = kFALSE;
   Bool_t TOF_check = kFALSE;
   Bool_t PrimInteraction = kFALSE;
   Bool_t BackInteraction = kFALSE;
   Bool_t LoopInteraction = kFALSE;
   Bool_t ThisPrimInteraction = kFALSE;
   Bool_t ThisBackInteraction = kFALSE;
   Bool_t ThisLoopInteraction = kFALSE;
   Bool_t Possible_Bug = kFALSE;
   
   // ===============================================================================================
   // Now we are ready to loop:
   cout << "We will now loop over the tree. Total number of Events are: " << nEvents << "\n";

   for(Long64_t Entry = 0; Entry<nEvents; ++Entry)
   {
    
     // =============================================================================================
     // Now the first step to do per event is obviously to load the entry we are talking about:
     nbytes = cbmsim->GetEntry(Entry);
     
     // After this command is given, one can simply access each number in the tree with commands like
     // Double_t light = LandPoint_fLightYield->GetValue(3)
     // Which loads the third value from the array corresponding to the leaf 'LandPoint_fLightYield'
     // into the variable light. Due to the GetEntry-command the correct Event is also selected.
     
     // =============================================================================================
     // In the first part of our analysis, we will determine the energy deposition per bar in
     // NeuLAND and in the VETO. We will use fLightYield, so that Birk's law is already taken into
     // account. We then call the primary particle detected if at least one of the bars has an
     // energy deposition above the Energy Threshold.
     
     // Hence we begin with accumulating energy deposition per bar:
     
     // -----------------------------------
     // We do the veto first:
     
     // Clean the veto-array:
     for(Int_t k = 0; k<vetobars; ++k)
     {
       VetoArray[k] = 0.0;
     }
     veto_detect = kFALSE;

     // Get the size of the current event:
     ArraySize = hist_veto_seg->GetBinContent(Entry+1);

     // Re-fill the array:
     for (Int_t k = 0; k<ArraySize; ++k)
     {
       selected_paddle = veto_segPoint_fSector->GetValue(k);
       light = veto_segPoint_fLightYield->GetValue(k);
       VetoArray[selected_paddle - 1] = VetoArray[selected_paddle - 1] + light;
     }
     
     // -----------------------------------
     // Then we do NeuLand:

     // Clean The array:
     for(Int_t k = 0; k<Neubars; ++k)
     {
       NeuArray[k] = 0.0;
     }
     Neu_detect = kFALSE;
     
     // Get the size of the current event:
     ArraySize = hist_land->GetBinContent(Entry+1);
     
     // Re-fill the array:
     for (Int_t k = 0; k<ArraySize; ++k)
     {
       selected_paddle = LandPoint_fSector->GetValue(k);
       light = LandPoint_fLightYield->GetValue(k);
       NeuArray[selected_paddle - 1] = NeuArray[selected_paddle - 1] + light;
     }
     
     // -----------------------------------
     // Now the energy deposited in each bar is known. The next step is to determine the energy of the primary particle.
     // We know that if the MCTrack_fMotherId equals -1, it is a primary particle. Hence the energy is determined by
     // the MCTrack_fPx, MCTrack_fPy and MCTrack_fPz of that particle. Hence all we have to do is find the primary
     // particle among all particle tracks in the MCTrack-leafs.
     
     // We will first define some variables we need:
     ArraySize = hist_MC->GetBinContent(Entry+1); // leaf size of current event.
     PrimParticleCount = 0;
     PrimParticleIndex = 0;
     
     // Then perform the search:
     for(Int_t k = 0; k<ArraySize; ++k)
     {
       MotherID = MCTrack_fMotherId->GetValue(k);
       
       if(MotherID == -1)
       {
	 PrimParticleIndex = k;
	 PrimParticleCount = PrimParticleCount + 1;
       }
     }
     
     // Then we perform a little check. Obviously a test like this is meaningless if there are more then one
     // primary particles. Also: if the number of counted primary particles is bigger then the number of
     // primary particles defined in the generator, there is a serious problem:
     if(PrimParticleCount != nParticles) {cout << "BUG!!! There is a bug in your code. The number of detected primary particles does not match the number of defined primary particles!\n";}
     if(nParticles != 1) {cout << "Our analysis does not work with more then a single primary particle per event. Please adapt this.\n";}
     
     // Hence if we do not see output lines, our code is fine. Now that we know what the primary particle is, we can
     // accumulate the energy:
     px = MCTrack_fPx->GetValue(PrimParticleIndex);
     py = MCTrack_fPy->GetValue(PrimParticleIndex);
     pz = MCTrack_fPz->GetValue(PrimParticleIndex);
     
     fMass = MCTrack_fMass->GetValue(PrimParticleIndex);
     
     Eparticle = TMath::Sqrt(px*px + py*py + pz*pz + fMass*fMass) - fMass;
     
     // Done, now Eparticle holds the energy of the primary particle!
     
     // ---------------------------------
     // Now we have the energy per bar, so now we will decide upon the detection and the filling of the histograms:
     
     // Now make the decision whether a particle is detected or not:
     for(Int_t k = 0; k<vetobars; ++k)
     {
       if(VetoArray[k] > Ethreshold)
       {
	 veto_detect = kTRUE;
       }
     }
     
     for(Int_t k = 0; k<Neubars; ++k)
     {
       if(NeuArray[k] > NeuLAND_threshold)
       {
	 Neu_detect = kTRUE;
       }
     }
     
     // Decide if veto did NOT detect it, but NeuLAND did:
     Real_detect = kFALSE;
     if((Neu_detect==kTRUE)&&(veto_detect==kFALSE)) {Real_detect = kTRUE;}

     // --------------------------------
     // Finally, add the energy to the histograms:
     if(veto_detect) {veto_hist->Fill(Eparticle);}
     if(Neu_detect) {Neu_hist->Fill(Eparticle);}
     if(Real_detect) {Real_hist->Fill(Eparticle);}
     clean_hist->Fill(Eparticle);

     // =========================================================================================================
     // In the second part of our analysis we will determine which particles are detected, based on 
     // the LandFirstHits-leafs. This is easy to do, since we already have the primary particle energy:
     
     // Gather the LandFirstHits information. We only need the first of the LandFirstHist because our
     // analysis is based on the fact that we only have a single primary particle. Hence this is sufficient.
     LFH_T = LandFirstHits_fT0->GetValue(0);
     LFH_X = LandFirstHits_fx0->GetValue(0);
     LFH_Y = LandFirstHits_fy0->GetValue(0);
     LFH_Z = LandFirstHits_fz0->GetValue(0);
     
     VFH_T = veto_segFirstHits_fT0->GetValue(0);
     VFH_X = veto_segFirstHits_fx0->GetValue(0);
     VFH_Y = veto_segFirstHits_fy0->GetValue(0);
     VFH_Z = veto_segFirstHits_fz0->GetValue(0);
     
     // Now determine whether the hit is in NeuLAND yes or no:
     Neu_detect_LFH = kFALSE;
     if ((LFH_X>(xcentral-125.0))&&(LFH_X<(xcentral+125.0))&&(LFH_Y>(ycentral-125.0))&&(LFH_Y<(ycentral+125.0))&&(LFH_Z>(zcentral-halfNeuLANDthickness))&&(LFH_Z<(zcentral+halfNeuLANDthickness))) {Neu_detect_LFH = kTRUE;}
     
     // Now determine whether the hit is in VETO yes or no:
     veto_detect_LFH = kFALSE;
     if ((VFH_X>(xveto-halfVETOwidth_x))&&(VFH_X<(xveto+halfVETOwidth_x))&&(VFH_Y>(yveto-halfVETOwidth_y))&&(VFH_Y<(yveto+halfVETOwidth_y))&&(VFH_Z>(zveto-halfVETOthickness))&&(VFH_Z<(zveto+halfVETOthickness))) {veto_detect_LFH = kTRUE;}
     
     // Now determine the real hit:
     Real_detect_LFH = kFALSE;
     if((Neu_detect_LFH==kTRUE)&&(veto_detect_LFH==kFALSE)) {Real_detect_LFH = kTRUE;}
     
     // Finally, fill the histograms:
     if(veto_detect_LFH) {veto_hist_LFH->Fill(Eparticle);}
     if(Neu_detect_LFH) {Neu_hist_LFH->Fill(Eparticle);}
     if(Real_detect_LFH) {Real_hist_LFH->Fill(Eparticle);}
     clean_hist_LFH->Fill(Eparticle);
     
     // Done.
     
     // =================================================================================================================================================================
     // =================================================================================================================================================================
     // =================================================================================================================================================================
     // From now on we will only analyze events that are meet our analysis condition:
     Event_Select = kFALSE;
     if (Event_Selection_Condition == 0) {Event_Select = kTRUE;}
     if ((Event_Selection_Condition == 1)&&(Neu_detect)) {Event_Select = kTRUE;}
     
     if (Event_Select)
     {
     
     // =========================================================================================================
     // The third part of our analysis is about secondary particles. NOTE that we will deal with
     // ALL secondary particles, not just the backscattered ones! We whish to plot some quantities
     // about these particles, so we have to select them and then fill our histograms.
     
     // Now we also want to plot theta and phi of secondary particles VS counts. Secondary particles
     // have fMotherID: the C++ index in the MCTrack-leafs that holds their parent-track. Hence
     // fMotherID==0 selects the particles produced in the primary interaction: the strictly secondaries.
     // fMotherID==-1 selects the primaries and fMotherID!=-1 selects all particles in the shower except
     // the primaries. We inmplant different possibilities for selection.
     
     // We begin with getting the correct size of the event:
     ArraySize = hist_MC->GetBinContent(Entry+1); // leaf size of current event.
     
     // Then we will loop over the leaf:
     for(Int_t k = 0; k<ArraySize; ++k)
     {
       // Begin with requesting the fMotherID of the secondary particle:
       MotherID = MCTrack_fMotherId->GetValue(k);
       
       // Then test if the particle is indeed a secondary:
       Secondary_Select = kFALSE;
       if ((Secondary_Condition==1)&&(MotherID != -1)) {Secondary_Select = kTRUE;}
       if ((Secondary_Condition==0)&&(MotherID == 0)) {Secondary_Select = kTRUE;}
              
       if (Secondary_Select) 
       {
	 // --------------------------------------------------------
	 // Now we know that we are talking about secondary particles. Request the impulses:
	 px = MCTrack_fPx->GetValue(k);
         py = MCTrack_fPy->GetValue(k);
         pz = MCTrack_fPz->GetValue(k);
	 fMass = MCTrack_fMass->GetValue(k);
	 
	 // also request the coordinates for secondary particle production point:
	 zpoint = MCTrack_fStartZ->GetValue(k);
	 xpoint = MCTrack_fStartX->GetValue(k);
	 ypoint = MCTrack_fStartY->GetValue(k);
	 
	 // Request particle PDG identification cade:
	 PDG = MCTrack_fPdgCode->GetValue(k);
	 
	 // Now compute theta:
	 theta = pz/TMath::Sqrt(px*px + py*py + pz*pz); // this is the cosine!
	 theta = TMath::ACos(theta);
	 
	 // Now compute phi:
	 if (py==0.0)
	 {
	   if (px < 0) {phi = TMath::Pi();}
	   else {phi = 0;}
	 }
	 else if (py < 0)
	 {
	   if (px < 0) {phi = 2*TMath::Pi() - TMath::ASin(TMath::Abs(py)/TMath::Sqrt(px*px + py*py));}
	   else {phi = TMath::Pi() + TMath::ASin(TMath::Abs(py)/TMath::Sqrt(px*px + py*py));}	   
	 }
	 else
	 {
	   if (px < 0) {phi = TMath::Pi() - TMath::ASin(TMath::Abs(py)/TMath::Sqrt(px*px + py*py));}
	   else {phi = TMath::ASin(TMath::Abs(py)/TMath::Sqrt(px*px + py*py));}	   
	 }
	 
	 // Compute if trajectory will hit the veto. We make a distinction whether the trajectory is created
	 // in front of the VETO or behind the VETO:
	 
	 if (zpoint>zveto) // behind the VETO.
	 {
	 ztravel = zpoint - zveto - halfVETOthickness;
	 xtravel = ztravel*TMath::Tan(TMath::Pi() - theta)*TMath::Cos(phi);
	 ytravel = ztravel*TMath::Tan(TMath::Pi() - theta)*TMath::Sin(phi);
	 
	 // Now if xpoint+xtravel and ypoint+ytravel are within +/- halfVETOwidth then x and y will pass through the veto.
	 // This is only true however, if theta>pi/2. Else it does not work. Hence:
	 vetohit = ((xtravel+xpoint)<(xveto+halfVETOwidth_x))&&((xtravel+xpoint)>(xveto-halfVETOwidth_x))&&((ytravel+ypoint)<(yveto+halfVETOwidth_y))&&((ytravel+ypoint)>(yveto-halfVETOwidth_y))&&(theta>(0.5*TMath::Pi()));
	 // if vetohit is kTRUE, then the particle will (just about) move through the veto!
	 
	 // Compute the length that the particle must travel through NeuLAND (only accurate if vetohit==kTRUE):
	 traveldistance = TMath::Abs(zpoint - (zcentral - halfNeuLANDthickness))/TMath::Abs(TMath::Cos(TMath::Pi() - theta));
	 
	 // If the particle is not produced withing NeuLAND: the traveldistance is wrong:
	 if (xpoint>(xcentral+125.0)) {traveldistance = 0.00001;} // Then we end up in the lowest bin: bin rn. 1.
	 if (xpoint<(xcentral-125.0)) {traveldistance = 0.00001;}
	 if (ypoint>(ycentral+125.0)) {traveldistance = 0.00001;}
	 if (xpoint<(ycentral-125.0)) {traveldistance = 0.00001;}
	 if (zpoint<(zcentral-halfNeuLANDthickness)) {traveldistance = 0.00001;}
	 if (zpoint>(zcentral+halfNeuLANDthickness)) {traveldistance = traveldistance*2.0*halfNeuLANDthickness/(zpoint - (zcentral - halfNeuLANDthickness));} // behind NeuLAND.
	 }
	 else // In front of the VETO (works the same way):
	 {
	 ztravel = zveto - halfVETOthickness - zpoint;
	 xtravel = ztravel*TMath::Tan(theta)*TMath::Cos(phi);
	 ytravel = ztravel*TMath::Tan(theta)*TMath::Sin(phi);
	 
	 vetohit = ((xtravel+xpoint)<(xveto+halfVETOwidth_x))&&((xtravel+xpoint)>(xveto-halfVETOwidth_x))&&((ytravel+ypoint)<(yveto+halfVETOwidth_y))&&((ytravel+ypoint)>(yveto-halfVETOwidth_y))&&(theta<(0.5*TMath::Pi()));
	 
	 traveldistance = 0.00001;
	 }
	 
	 // Now compute the energy of the secondary particle: 
	 Esecondary = TMath::Sqrt(px*px + py*py + pz*pz +fMass*fMass) - fMass;
	 
	 // -----------------------------------
	 // Now that we know what we want to know about the secondary particles, fill the histograms. Before we
	 // will fill the histogram, we whish to cut out all the particles that were produced in the VETO itself.
	 // We are not interested in particles that have their production point inside the VETO. NOTE that this
	 // will not be our selection criterium for backscattering. It is just a restriction we impose on the 
	 // plots for all secondary particles!
	 if ((xpoint<(xveto+halfVETOwidth_x))&&(xpoint>(xveto-halfVETOwidth_x))&&(ypoint<(yveto+halfVETOwidth_y))&&(ypoint>(yveto-halfVETOwidth_y))&&(zpoint<(zveto+halfVETOthickness))&&(zpoint>(zveto-halfVETOthickness))) {vetohit = kFALSE;}
	 
	 if (Real_detect)
	 {
	   Phi_detect->Fill(phi);
	   Etheta_detect->Fill(Esecondary,theta);
	   Ez_detect->Fill(Esecondary,zpoint);
	 }
	 else
	 {
	   Phi_not->Fill(phi);
	   Etheta_not->Fill(Esecondary,theta);
	   Ez_not->Fill(Esecondary,zpoint);
	 }
	 
	 Phi_all->Fill(phi);
	 Etheta_all->Fill(Esecondary,theta);
	 Ez_all->Fill(Esecondary,zpoint);
	 
	 // fill in for protons:
	 if (TMath::Abs(PDG)==2212)
	 {
	   EzdALL_proton->Fill(Esecondary,zpoint,traveldistance);
	   if (vetohit) {EzdREACH_proton->Fill(Esecondary,zpoint,traveldistance);}
	   if (vetohit) {EzdAIM_proton->Fill(Esecondary,zpoint,traveldistance);}
	 }
	 
	 // neutrons:
	 else if (TMath::Abs(PDG)==2112)
	 {
	   EzdALL_neutron->Fill(Esecondary,zpoint,traveldistance);
	   if (vetohit) {EzdAIM_neutron->Fill(Esecondary,zpoint,traveldistance);}
	   if (vetohit) {EzdREACH_neutron->Fill(Esecondary,zpoint,traveldistance);}
	 }
	 
	 // fill in for electrons:
	 else if (TMath::Abs(PDG)==11)
	 {
	   EzdALL_electron->Fill(Esecondary,zpoint,traveldistance);
	   if (vetohit) {EzdAIM_electron->Fill(Esecondary,zpoint,traveldistance);}
	   
	   // Now we also have to deal with electrons produced outside NeuLAND.
	   // these electrons travel trough the air, but might not make it. 10 MeV reaches
	   // 4135 cm in Air. This is lineair. Hence we will compute the traveldistance through the air:
	   Airdistance = TMath::Sqrt(xtravel*xtravel+ytravel*ytravel+ztravel*ztravel);
	   
	   // NOTE: this distance is only accurate if vetohit==kTRUE. Hence if traveldistance==0.00001 (not produced in NeuLAND)
	   // and Airdistance is small enough, we should add the electron here. Else we should not.
	   
	   Airdistance = Airdistance/(1000*Esecondary*0.01); // Traveldistance per 10 MeV.
	   
	   // So now we are in busyness:
	   if ((vetohit==kTRUE)&&(traveldistance<0.00002)&&(Airdistance<41.35)) {EzdREACH_electron->Fill(Esecondary,zpoint,traveldistance);}
	   
	   // Now add the electrons produced inside NeuLAND:
	   if ((vetohit==kTRUE)&&(traveldistance>0.00002)) {EzdREACH_electron->Fill(Esecondary,zpoint,traveldistance);}
	   
	   // Notice that if an electron is produced outside NeuLAND we assume it also does not need to travel through NeuLAND
	   // in order to reach the VETO. Of course this is not completely correct: if it is produced close by NeuLAND but outside
	   // NeuLAND it might still be travelling through NeuLAND.
	   
	 }
	 
	 // fill in for Gammas:
	 else if (TMath::Abs(PDG)==22)
	 {
	   EzdALL_gamma->Fill(Esecondary,zpoint,traveldistance);
	   if (vetohit) {EzdAIM_gamma->Fill(Esecondary,zpoint,traveldistance);}
	   if (vetohit) {EzdREACH_gamma->Fill(Esecondary,zpoint,traveldistance);}
	 }
	 
	 // pions: we do not store travel distance throught NeuLAND, but we store theta instead!
	 else if (TMath::Abs(PDG)==211)
	 {
	   EzdALL_pion->Fill(Esecondary,zpoint,traveldistance);
	   if (vetohit) {EzdAIM_pion->Fill(Esecondary,zpoint,traveldistance);}
	   if (vetohit) {EzdREACH_pion->Fill(Esecondary,zpoint,traveldistance);}
	 }
	 
	 // muons: we do not store travel distance throught NeuLAND, but we store theta instead!
	 else if (TMath::Abs(PDG)==13)
	 {
	   EzdALL_muon->Fill(Esecondary,zpoint,traveldistance);
	   if (vetohit) {EzdAIM_muon->Fill(Esecondary,zpoint,traveldistance);}
	   if (vetohit) {EzdREACH_muon->Fill(Esecondary,zpoint,traveldistance);}
	 }
	 
	 // Cions: everything:
	 else if ((TMath::Abs(PDG)>1000060000)&&(TMath::Abs(PDG)<1000060190))
	 {
	   EzdALL_Cion->Fill(Esecondary,zpoint,traveldistance);
	   if (vetohit) {EzdREACH_Cion->Fill(Esecondary,zpoint,traveldistance);}
	   if (vetohit) {EzdAIM_Cion->Fill(Esecondary,zpoint,traveldistance);}
	 }
	 
	 // Remainders: Only ions are counted!
	 else if (TMath::Abs(PDG)>1000000000)
	 {
	   EzdALL_other->Fill(Esecondary,zpoint,traveldistance);
	   if (vetohit) {EzdAIM_other->Fill(Esecondary,zpoint,traveldistance);}
	 }
	 
	 // Checking:
	 //else {cout << ".";}
	 
	 
       }
     }
     
     // =====================================================================================================
     // The fourth part of the analysis: we determine which secondary particles indeed did
     // hit the VETO by using the Monte Carlo points. NOTE that again we are talking about secondary particles,
     // NOT about backscattered particles!
     
     // -------------------------------------------------------------------
     // before we do anything: set the histogram repeat to zero:
     for (Int_t k = 0; k<(MCTrack_ArraySize+2); ++k)
     {
       repeat->SetBinContent(k,0.0);
       repeat->SetBinError(k,0.0);
     }
   
     // Also set the histogram repeatE (energy test repeat) to zero:
     for (Int_t k = 0; k<(MCTrack_ArraySize+2); ++k)
     {
       repeatE->SetBinContent(k,0.0);
       repeatE->SetBinError(k,0.0);
     }
     
     // before we do anything: set the histogram Edep to zero:
     for (Int_t k = 0; k<(MCTrack_ArraySize+2); ++k)
     {
       for (Int_t n = 0; n<(vetobars+2); ++n)
       {
         Edep->SetBinContent(k,n,0.0);
         Edep->SetBinError(k,n,0.0);
       }
     }
   
     // --------------------------------------------------------------------
     // We begin with requesting the event sizes. However, we now need the veto_segPoint-leafs.
     ArraySize = hist_veto_seg->GetBinContent(Entry+1); // leaf size of current event.
     
     // Begin with looping over the veto-leafs:
     for (Int_t kveto = 0; kveto<ArraySize; ++kveto)
     {
       // Request the trackID so that we can retrace this in the MCTrack-leafs:
       TrackID = veto_segPoint_fTrackID->GetValue(kveto);
       
       // Now TrackID is the index in the MCTrack-leafs corresponding to the kveto-interaction point
       // of the veto_segPoint-leafs.
       
       // However the tracks corresponding to these interaction points are the tracks
       // PRODUCED in the VETO. We want the parent tracks of these: the tracks that ended
       // in the VETO. Hence from the TrackID we get the MotherID. The MotherID is the
       // index of the MCTrack-leafs corresponding to the parent of this interaction point.
       // hence we simply should go back in the cascade!
       
       // We trace back untill the production points are outside the VETO.
       // NOTE: integrals include under/overflow?
       
       zpoint = MCTrack_fStartZ->GetValue(TrackID);
       xpoint = MCTrack_fStartX->GetValue(TrackID);
       ypoint = MCTrack_fStartY->GetValue(TrackID);
       
       while ((xpoint>(xveto-halfVETOwidth_x))&&(xpoint<(xveto+halfVETOwidth_x))&&(ypoint>(yveto-halfVETOwidth_y))&&(ypoint<(yveto+halfVETOwidth_y))&&(zpoint>(zveto-halfVETOthickness))&&(zpoint<(zveto+halfVETOthickness))&&(TrackID>-1))
       {
         TrackID = MCTrack_fMotherId->GetValue(TrackID);
	 zpoint = MCTrack_fStartZ->GetValue(TrackID);
         xpoint = MCTrack_fStartX->GetValue(TrackID);
         ypoint = MCTrack_fStartY->GetValue(TrackID);
       }
 
       // Now TrackID holds the index of the particle that is responsible
       // for precisely this hitpoint in the VETO.
       
       // Request impulses:
       px = MCTrack_fPx->GetValue(TrackID);
       py = MCTrack_fPy->GetValue(TrackID);
       pz = MCTrack_fPz->GetValue(TrackID);
       fMass = MCTrack_fMass->GetValue(TrackID);
       
       // Request production point:
       zpoint = MCTrack_fStartZ->GetValue(TrackID);
       xpoint = MCTrack_fStartX->GetValue(TrackID);
       ypoint = MCTrack_fStartY->GetValue(TrackID);

       // Now compute the energy of the secondary particle:
       Esecondary = TMath::Sqrt(px*px + py*py + pz*pz + fMass*fMass) - fMass;
       
       // Request particle ID:
       PDG = MCTrack_fPdgCode->GetValue(TrackID);
       
       // Request the energy deposition:
       light = veto_segPoint_fLightYield->GetValue(kveto);
       VETObarNr = veto_segPoint_fSector->GetValue(kveto);
       
       // Add this to the tracked mother:
       E_add = Edep->GetBinContent(TrackID+1,VETObarNr); // fSector runs from 1 to vetobars. hence it is already a binning index ==> OK.
       E_add = E_add + light;
       Edep->SetBinContent(TrackID+1,VETObarNr,E_add);       
       
       // ------------------------------------------------
       // Now we are ready to fill the histograms:
       
       // We know already that it hits the VETO. Hence all we have to do is to make sure that
       // 1) the particle is not a primary 
       // 2) the particle has not yet been added to the histogram.
       
       // Hence request the repeat:
       repeat_test = repeat->GetBinContent(TrackID+1);
       
       // Also request the MotherID:
       MotherID = MCTrack_fMotherId->GetValue(TrackID);
       
       if (repeat_test<0.5) // testing whether the particle is already added or not
       {
         // Then test if the particle is indeed a secondary:
         Secondary_Select = kFALSE;
         if ((Secondary_Condition==1)&&(MotherID != -1)) {Secondary_Select = kTRUE;}
         if ((Secondary_Condition==0)&&(MotherID == 0)) {Secondary_Select = kTRUE;}
              
         if (Secondary_Select) 
         {
	   // Now we can fill it up in the different histograms. Use the same PDG-conditions as above:
	
	   if (TMath::Abs(PDG)==2212) {EzHIT_proton->Fill(Esecondary,zpoint);}
	   else if (TMath::Abs(PDG)==2112) {EzHIT_neutron->Fill(Esecondary,zpoint);}
	   else if (TMath::Abs(PDG)==11) {EzHIT_electron->Fill(Esecondary,zpoint);}
	   else if (TMath::Abs(PDG)==22) {EzHIT_gamma->Fill(Esecondary,zpoint);}
	   else if (TMath::Abs(PDG)==211) {EzHIT_pion->Fill(Esecondary,zpoint);}
	   else if (TMath::Abs(PDG)==13) {EzHIT_muon->Fill(Esecondary,zpoint);}
	   else if ((TMath::Abs(PDG)>1000060000)&&(TMath::Abs(PDG)<1000060190)) {EzHIT_Cion->Fill(Esecondary,zpoint);}
	   else if (TMath::Abs(PDG)>1000000000) {EzHIT_other->Fill(Esecondary,zpoint);}
	   else {cout << "We have an unidentified particle!!!\n";}
	 
	   // If we succesfully added the particle, we must make sure it is not added again:
	   repeat->SetBinContent(TrackID+1,1.0);
	 
         }
       }
       
       // Now we also add the EzDET-histograms. We use exactly the same conditions,
       // only now we request as additional condition that the Edep-bin (of at least one VETO-paddle) is above the threshold.
       repeat_test = repeatE->GetBinContent(TrackID+1);

       if (repeat_test<0.5) // testing whether the particle is already added or not
       {
         // Then test if the particle is indeed a secondary:
         Secondary_Select = kFALSE;
         if ((Secondary_Condition==1)&&(MotherID != -1)) {Secondary_Select = kTRUE;}
         if ((Secondary_Condition==0)&&(MotherID == 0)) {Secondary_Select = kTRUE;}
         
         // Now get the maximum energy deposit in one bar:  
         E_add = 0.0;
	 for (Int_t nk = 0; nk<(vetobars+2); ++nk)
	 {
	   if (E_add<(Edep->GetBinContent(TrackID+1,nk))) {E_add = Edep->GetBinContent(TrackID+1,nk);}
	 }

	 // Then use this maximum for the test:
	 if (E_add > Ethreshold)
	 {
	   if (Secondary_Select) 
           {
	   
	   // Now we can fill it up in the different histograms. Use the same PDG-conditions as above:
	
	     if (TMath::Abs(PDG)==2212) {EzDET_proton->Fill(Esecondary,zpoint);}
	     else if (TMath::Abs(PDG)==2112) {EzDET_neutron->Fill(Esecondary,zpoint);}
	     else if (TMath::Abs(PDG)==11) {EzDET_electron->Fill(Esecondary,zpoint);}
	     else if (TMath::Abs(PDG)==22) {EzDET_gamma->Fill(Esecondary,zpoint);}
	     else if (TMath::Abs(PDG)==211) {EzDET_pion->Fill(Esecondary,zpoint);}
	     else if (TMath::Abs(PDG)==13) {EzDET_muon->Fill(Esecondary,zpoint);}
	     else if ((TMath::Abs(PDG)>1000060000)&&(TMath::Abs(PDG)<1000060190)) {EzDET_Cion->Fill(Esecondary,zpoint);}
	     else if (TMath::Abs(PDG)>1000000000) {EzDET_other->Fill(Esecondary,zpoint);}
	     else {cout << "We have an unidentified particle!!!\n";}
	 
	     // If we succesfully added the particle, we must make sure it is not added again:
	     repeatE->SetBinContent(TrackID+1,1.0);

	   }
         }
       }
        
     }
     
     // Done, now the histograms EzHIT contains everything that HITS the VETO. Note however that this does not
     // yet state anything about detection! The histograms EzDET contain everything that hit the veto with total
     // energy deposition above the threshold. Hence this does state something about detection.
     
     // Now as a final issue we would like to know whether we should count this event as having deposited secondary
     // particles to the VETO that were detected by the VETO.
     // If the histogram repeatE has a non-zero integral, then it means that there has been at least
     // one secondary particle that hit the veto, is not a primary particle and deposited more then Etheshold. hence:
     repeat_test = repeatE->Integral();
     Secondary_freq->SetBinContent(Entry+1,repeat_test);
     if (repeat_test > 0.01)
     {
       veto_hist_Sec->Fill(Eparticle); 
       // Eparticle already contains the energy of the primary particle.
       
     }
     
     // So now the histogram veto_hist contains all events in which the total energy deposition per bar
     // in the veto exceeded the energy threshold. The histogram veto_hist_Back now contains all events
     // in which there is at least one secondary particle that deposited more then the threshold in the
     // entire veto (NOTE that the bar-structure of the VETO is not taken into account!)
     
     // In the case of a pure plate, the bar-structure is irrelevant. Also, the case where a primary particle
     // interacts in the VETO is a few %. The case when a secondary particle interacts with the VETO is
     // also a few %. Hence the case when both a primary interaction and a secondary one are in the VETO is virtually
     // non-existing. (These cases are INCLUDED in the veto_hist_Sec histogram). Hence one can say very simple
     // that the difference between the veto_hist and veto_hist_Back is purely due to secondary interactions.
     // NOTE that this is NOT the same as backscattering!
     
     // This is because in the air between the source and the VETO, the neutron might scatter a small bit.
     // then this neutron is regarded by the software as a secondary particle, while it actually still
     // is a primary particle.
     
     // =====================================================================================================
     // Now in the fifth part our analysis will detemine whether an event has NOT been VETOed, is a sole
     // primary VETOed event, is a sole backsctattered event or is an event with both a primary
     // interaction and a backscattered one. We will not use the results of the previous parts for this,
     // since they were dealing with secondary PARTICLES, not backscattering. And as explained, the primary
     // neutron might interact with the air and become a secondary particle while it is still the same
     // neutron before hitting the VETO. Hence we will have to do a more thorough sweep here.
     
     // In this case we have to do a check to identify the event as backscattering, primary interaction, or both (or loops around the VETO):
     // We will first assume that the event is neither of them and define boolians accordingly:
     PrimInteraction = kFALSE;
     BackInteraction = kFALSE;
     LoopInteraction = kFALSE;
     // We also define another set of boolians that are reset on every Monte Carlo point. The above set is however 
     // only reset at the beginning of the event.
     
     // Then the next step is to resetthe energy deposition histogram. We need the energy deposition
     // belonging to the tracks as well, since if an event is both primary and backscattering, we
     // need to determine which of them meets the energy threshold (or both). Hence reset the energy histogram:
     
     // For bfinding nasty events:
     Possible_Bug = kFALSE;
     // If a primary interaction happens in the VETO, the TOF of the VETO should be faster then for NeuLAND.
     // It can happen that there is a primary interaction, while the TOF is slower. This is beause then the
     // neutron collides with the air and produces another particle. then this particle is slower then the
     // neutron and this one causes the primary interaction while the neutron itself does not have a
     // primary interaction in the VETO. the rate of these events is estimated at 8/50000. Hence we need
     // this boolian to keep track whether such a thing can happen or not. If it false, this could have happened.
     
     // On very rare occasion (1/50000) there is a neutron that backscatters FOR ITSELF on NeuLAND and then
     // hits the VETO. Since it is the same neutron and not a new particle, it is the same track (with a curve!) 
     // The data available cannot distinguish from that and then we see it as a primary event while it is backscattering.
     // We cannot do something about it.
     
     for (Int_t k = 0; k<(MCTrack_ArraySize+2); ++k)
     {
       for (Int_t n = 0; n<(vetobars+2); ++n)
       {
         Edep->SetBinContent(k,n,0.0);
         Edep->SetBinError(k,n,0.0);
       }
     }
     
     // Now, the first thing we know is that if veto_detect is kFALSE, then the event is NOT VETOed. Hence we can do
     // this check as a first step:
     if (veto_detect)
     {
       // Now if we identify that the event contains a primary interaction, we set the respective boolian to
       // true. We do the same for the backscattering. Then we can easily fill the remaining histograms.
       
       // We will sweep through all Monte-Carlo points inside the VETO and trace these points back until
       // the shower exits the VETO. Then if the event came from the direction of the source, we set this
       // boolian equal to kTRUE. If it came from NeuLAND, we set this Boolian to kTRUE. We make this distinction
       // purely based on the zpoint. Here we go:
       
       // Then first obtain the event size:
       ArraySize = hist_veto_seg->GetBinContent(Entry+1);
       
       // Then loop over this event:
       
       for (Int_t kveto = 0; kveto<ArraySize; ++kveto)
       {
         // Define boolians that are reset on each MC point:
         ThisPrimInteraction = kFALSE;
         ThisBackInteraction = kFALSE;
         ThisLoopInteraction = kFALSE;
	 
	 // perform the tracking back until we leave the VETO, just as in the previous part:
	 TrackID = veto_segPoint_fTrackID->GetValue(kveto);
         zpoint = MCTrack_fStartZ->GetValue(TrackID);
         xpoint = MCTrack_fStartX->GetValue(TrackID);
         ypoint = MCTrack_fStartY->GetValue(TrackID);
       
         while ((xpoint>(xveto-halfVETOwidth_x))&&(xpoint<(xveto+halfVETOwidth_x))&&(ypoint>(yveto-halfVETOwidth_y))&&(ypoint<(yveto+halfVETOwidth_y))&&(zpoint>(zveto-halfVETOthickness))&&(zpoint<(zveto+halfVETOthickness))&&(TrackID>-1))
         {
           TrackID = MCTrack_fMotherId->GetValue(TrackID);
	   zpoint = MCTrack_fStartZ->GetValue(TrackID);
           xpoint = MCTrack_fStartX->GetValue(TrackID);
           ypoint = MCTrack_fStartY->GetValue(TrackID);
         }
         
         // If we come out of VETO and go immediately to the particle gun:
         if (TrackID==0) {Possible_Bug = kTRUE;}
         // because then the TOF of the VETO cannot be slower.
         
         zpoint = MCTrack_fStartZ->GetValue(TrackID);
         xpoint = MCTrack_fStartX->GetValue(TrackID);
         ypoint = MCTrack_fStartY->GetValue(TrackID);
	 
	 // Now at this point, xpoint, ypoint and zpoint are outside the VETO.
	 // Now we will simply put the boolians to true depending on zpoint alone.
	 // An event can contain a lot of different MC points. If they belong to
	 // different showers: one from the primary event and one from the 
	 // backscattering, then due to the for-loop eventually both boolians will
	 // become true.
	 
	 // So now perform the check:
	 if (zpoint<zveto) {ThisPrimInteraction = kTRUE;}
	 if (zpoint>zveto) {ThisBackInteraction = kTRUE;}
	 
	 // Then also store the energy deposition:
	 light = veto_segPoint_fLightYield->GetValue(kveto);
	 selected_paddle = veto_segPoint_fSector->GetValue(kveto);
	 E_add = Edep->GetBinContent(TrackID+1,selected_paddle);
	 E_add = E_add + light;
	 Edep->SetBinContent(TrackID+1,selected_paddle,E_add);
	 
	 // However, if we confirmed a primary interaction, because the shower left the VETO
	 // on the side of the source, we have to make sure that this track does not 
	 // trace back to NeuLAND anyway (this is unlikely, but possible: the loop-interactions). This only
	 // has to be tested for THIS specific MonteCarlo point that we traced, hence
	 // no memeory should be kept and therefore ThisPrimInteraction should be used:
	 
	 if (ThisPrimInteraction)
	 {
	   while (TrackID!=0)
	   {
	     if (zpoint>zveto) {ThisLoopInteraction = kTRUE;}
	     TrackID = MCTrack_fMotherId->GetValue(TrackID);
	     zpoint = MCTrack_fStartZ->GetValue(TrackID);
             xpoint = MCTrack_fStartX->GetValue(TrackID);
             ypoint = MCTrack_fStartY->GetValue(TrackID);
	   }
	 } 
	 
	 // Of course there are also events that scatter of the air between NeuLAND and the VETO, but we will regard
	 // them as backscattering. NOTE that this air-backscattering can occur close to the VETO, causing the TOF-condition
	 // to still regard this event as a primary event and hence VETO it! This is NOT correct because the
	 // primary particle indeed dit NOT interact with the VETO in this case (Or it did interact with the VETO
	 // but did not depose sufficient energy there). Anyway, these events should therefore be regardedas backscattering
	 
	 // So now we have for this specific Monte-Carlo point that ThisPrimInteraction is kTRUE if THIS point leaves the
	 // VETO on the source side. If it then goes back to NeuLAND, also ThisLoopInteraction is kTRUE. If on the other
	 // hand, the point traces back to the side of NeuLAND, then ThisBackInteraction is kTRUE. Hence we can now
	 // define the global interaction boolians:
	 if ((ThisPrimInteraction)&&(!ThisLoopInteraction)&&(!ThisBackInteraction)) {PrimInteraction = kTRUE;}
	 if ((ThisPrimInteraction)&&(ThisLoopInteraction)&&(!ThisBackInteraction)) {BackInteraction = kTRUE; LoopInteraction = kTRUE;}
	 if ((!ThisPrimInteraction)&&(!ThisLoopInteraction)&&(ThisBackInteraction)) {BackInteraction = kTRUE;}
       }
       
       // At this point the Boolians give us the correct information. However, as explained in the case for both backscattering and
       // a primary inetraction, we must still determine which of them is responsible for the energy deposition. Hence test for this
       // situation:
       
       if (PrimInteraction&&BackInteraction)
       {
	 // If this is the case, we will start from scratch again and re-examine all tracks producing
	 // showers in the VETO. These tracks are marked by their trackID, represented by the position (index) in
	 // the histogram Edep. Hence set the global boolians back to kFALSE:
	 PrimInteraction = kFALSE;
	 BackInteraction = kFALSE;
	 LoopInteraction = kFALSE;
	 
	 // Then lop over the histogram to re-examine each track that produced a shower inside the VETO:
	 for (Int_t k = 0; k<(MCTrack_ArraySize+2); ++k)
	 {
	   for (Int_t n = 0; n<(vetobars+2); ++n)
	   {
	     // We again set the local boolians to zero and request the energy deposition
	     // of this particular track:
	     E_add = Edep->GetBinContent(k,n);
	     ThisPrimInteraction = kFALSE;
	     ThisBackInteraction = kFALSE;
	     ThisLoopInteraction = kFALSE;
	     
	     // Now only the energy depositions above the threshold are worthy of a closer inspection of course.
	     // Notice that this test is unnecessary if we have only backscattering or only primary interaction.
	     // We test on veto_detect, hence the energy is already there and in case we only have one of them,
	     // we therefore also konw where the energy comes from. Bot here, we test it:
	     if (E_add>Ethreshold)
	     {
	       // Now get the track ID and therefore the position information belonging to
	       // the track producing this shower inside the VETO:
	       TrackID = k-1;
	       zpoint = MCTrack_fStartZ->GetValue(TrackID);
	       xpoint = MCTrack_fStartX->GetValue(TrackID);
               ypoint = MCTrack_fStartY->GetValue(TrackID);
	   
	       // Then test again for backscattering or primary interaction:
	       if (zpoint<zveto) {ThisPrimInteraction = kTRUE;}
	       if (zpoint>zveto) {ThisBackInteraction = kTRUE;}
	       
	       // Check again for a loop interaction:
	       if (ThisPrimInteraction)
	       {
	         while (TrackID!=0)
	         {
	           if (zpoint>zveto) {ThisLoopInteraction = kTRUE;}
	           TrackID = MCTrack_fMotherId->GetValue(TrackID);
	           zpoint = MCTrack_fStartZ->GetValue(TrackID);
                   xpoint = MCTrack_fStartX->GetValue(TrackID);
                   ypoint = MCTrack_fStartY->GetValue(TrackID);
	         }
	       } 
	       
	       // And then finally set also the global boolians again to the correct value.
	       if ((ThisPrimInteraction)&&(!ThisLoopInteraction)&&(!ThisBackInteraction)) {PrimInteraction = kTRUE;}
	       if ((ThisPrimInteraction)&&(ThisLoopInteraction)&&(!ThisBackInteraction)) {BackInteraction = kTRUE; LoopInteraction = kTRUE;}
	       if ((!ThisPrimInteraction)&&(!ThisLoopInteraction)&&(ThisBackInteraction)) {BackInteraction = kTRUE;}
	       
	     }	     
	   }	   
	 }	 
       }
       
       // Now that we have our boolians, fill the histograms:
       if (PrimInteraction&&(!BackInteraction)) {veto_hist_PurePrim->Fill(Eparticle);}
       if (BackInteraction&&(!PrimInteraction)) {veto_hist_PureBack->Fill(Eparticle);}
       if (PrimInteraction&&BackInteraction) {veto_hist_PrimBack->Fill(Eparticle);}
       // Done!
       
     }
     else
     {
       // This is easy: the event simply isn VETOed:
       veto_hist_Pass->Fill(Eparticle);
     }
     
     // Done with this part!
     
     // =====================================================================================================
     // The sixth part of our analysis is about the TOF analysis. Even though the amount of events in which case
     // the primary neutron gets VETOed due to backscattering is only a few % we would like to improve it.
     // In order to do this, we take the time measurement of NeuLAND and of the VETO and determine which one 
     // was first. In the case that the VETO was first, we know that the signal comes from a primary particle
     // and we can VETO this particle (it still might be a neutron). If NeuLAND is first, we know that the particle
     // passed through the VEYO without being detected and then the backscattered particles fired the VETO.
     
     // For the time signals of NeuLAND and the VETO we will use the time index of the first Monte-Carlo interaction
     // point in either the VETO or in NeuLAND. In order to do this, we will first search for the MAXIMUM of all time indices.
     
     // ----------------------------------------------
     // For finding this MAXIMUM, we will extract the TOF (or time index) of the VETO first:
     ArraySize = hist_veto_seg->GetBinContent(Entry+1); // leaf size of current event.
     TOF_veto_max = 0.0; // initialization of the maximum search.
     
     // Begin with looping over the veto-leafs:
     for (Int_t kveto = 0; kveto<ArraySize; ++kveto)
     {
       // We begin by extracting the Time from the LandPoint-leafs:
       time = veto_segPoint_fTime->GetValue(kveto);
       // This is the time since the beginning of the event, while the MCTrack->fStartT is the time since the beginning of the track!
       // Of course we are interested in the time since the beginning of the event, when the primary particle is fired. Notice that
       // this time is in nanosec.

       // Then if there comes a bigger time, we replace it:
       if (time>TOF_veto_max) {TOF_veto_max = time;}
       
       // Hence in the end, TOF_veto_max holds the maximum time of the interaction points in the Veto.
     }
     
     // After we found the maximum, we can now look for the minimum.
     // We need the maximum first, because we will begin our search with this
     // maximum and only if a point meets the right criteria, we will replace
     // the maximum value by this value. This way, small times which are unphysical
     // can be excluded. Also, we are not interested in the global minimum over the VETO, but only
     // the global minimum of those points belonging to a bar with significant energy
     // deposition.
     
     // Hence we initialize the search:
     TOF_veto = TOF_veto_max;
     
     // Then we loop again:
     for (Int_t kveto = 0; kveto<ArraySize; ++kveto)
     {
       // request the time since beginning of the event:
       time = veto_segPoint_fTime->GetValue(kveto);
       
       // Request also the bar that was hit:
       selected_paddle = veto_segPoint_fSector->GetValue(kveto);
       
       // request the energy deposed in this bar (the arrays of the first part in
       // our analysis are still available:
       E_add = VetoArray[selected_paddle - 1];

       // Now we replace TOF_veto with time if time is smaller and if E_dep is bigger then the threshold:
       if ((time<TOF_veto)&&(E_add>Ethreshold)) {TOF_veto = time;}
       
       // So now we have the minimum that matches the energy threshold.
     }
     
     // -------------------------------
     // Now do the same procedure again for NeuLAND:
     
     ArraySize = hist_land->GetBinContent(Entry+1);
     TOF_Neu_max = 0.0;
     
     for (Int_t kNeu = 0; kNeu<ArraySize; ++kNeu)
     {
       time = LandPoint_fTime->GetValue(kNeu);
       if (time>TOF_Neu_max) {TOF_Neu_max = time;}
     }
     
     TOF_Neu = TOF_Neu_max;
     
     for (Int_t kNeu = 0; kNeu<ArraySize; ++kNeu)
     {
       TrackID = LandPoint_fTrackID->GetValue(kNeu); 
       selected_paddle = LandPoint_fSector->GetValue(kNeu);
       E_add = NeuArray[selected_paddle - 1];
       time = LandPoint_fTime->GetValue(kNeu);
       if ((time<TOF_Neu)&&(E_add>NeuLAND_threshold)) {TOF_Neu = time;}
     }
     
     // Now we have the TOF for both NeuLAND and the VETO. We now convert everything to seconds:
     TOF_Neu = TOF_Neu*1e-9;
     TOF_veto = TOF_veto*1e-9;
     
     // --------------------------------------
     // Now that we have this minimum we can do something with it.
     
     // if veto_detect is kTRUE, we know that the event has been detected in the veto. This also
     // ensures that the event has non-zero veto-points, giving the TOF of the veto a non-zero value.
     
     // Hence we can just do:
     if (veto_detect)
     {
       // Fill the TOF histograms:
       TOF_VETO_MC->Fill(TOF_veto);
       TOF_NeuLAND_MC->Fill(TOF_Neu);
       
       // Apply the VETO condition:
       if (TOF_veto<TOF_Neu) {veto_hist_TOF->Fill(Eparticle);}
       if (PrimInteraction&&(TOF_veto<TOF_Neu)) {veto_hist_TOF_prim->Fill(Eparticle);}
       //if ((repeat_test < 0.01)&&(TOF_veto<TOF_Neu)) {veto_hist_TOF_prim->Fill(Eparticle);}
       
       // Some bug finding if the TOF is slower:
       if (Possible_Bug&&PrimInteraction&&(TOF_veto>TOF_Neu)) {cout << "TOF_Neu = " << TOF_Neu << " TOF_veto = " << TOF_veto << " | Entry = " << Entry << " | ";}
     }
     
     // Now we have to implement the time resolution:
     TOF_veto = TOF_veto + generator->Gaus(0.0,Tres_VETO*1e-12); // Convert from ps to sec.
     TOF_Neu = TOF_Neu + generator->Gaus(0.0,Tres_Neu*1e-12);
     
     // And then we apply the same procedures again:
     if (veto_detect)
     {
       // Fill the TOF histograms:
       TOF_VETO_Rand->Fill(TOF_veto);
       TOF_NeuLAND_Rand->Fill(TOF_Neu);
       
       // Apply the VETO condition:
       if (TOF_veto<TOF_Neu) {veto_hist_TOF_Rand->Fill(Eparticle);}
       if (PrimInteraction&&(TOF_veto<TOF_Neu)) {veto_hist_TOF_Rand_prim->Fill(Eparticle);}
     }
     
     // And now our histograms are availabe for further analysis.
     
     // =============================================================================================
     // The seventh part of our analysis is about neutron scattering:
     
     // =============================================================================================
     }
     
     // Now we are done! Give a sign of life:
     if ((Entry+1)%100 == 0) {cout << "We processed " << Entry+1 << " events.\n";}
    
   }
   
   cout << "Our loop over the tree has now ended succesfully.\n\n\n\n !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! VETO-results are:\n\n"
        << "We took " << (veto_hist_Pass->Integral())+(veto_hist_PurePrim->Integral())+(veto_hist_PureBack->Integral())+(veto_hist_PrimBack->Integral()) << " events along for the VETO-performance.\n"
        << "Naive veto-condition results in VETOing " << (veto_hist_PurePrim->Integral())+(veto_hist_PureBack->Integral())+(veto_hist_PrimBack->Integral()) << " events\n"
	<< "The amount of pure backscattered events is " << veto_hist_PureBack->Integral() << "\n"
	<< "The amount of pure primary events is " << veto_hist_PurePrim->Integral() << "\n"
	<< "The amount of events with both backscattering and primary interaction is " << veto_hist_PrimBack->Integral() << "\n"
	<< "The amount of events with VETO-detected secondary particles is " << veto_hist_Sec->Integral() << "\n\n"

	<< "Precise TOF veto-condition results in VETOing " << veto_hist_TOF->Integral() << " events\n"
	<< "This precise TOF condition VETOed " << veto_hist_TOF_prim->Integral() << " primary events\n"
	<< "Time resolution TOF veto-condition results in VETOing " << veto_hist_TOF_Rand->Integral() << " events.\n"
	<< "This time resolution TOF condition VETOed " << veto_hist_TOF_Rand_prim->Integral() << " primary events\n\n";
	
   cout << "We will now execute the post-processing.\n\n\n\n";
   
   // ===============================================================================================
   // The loop over the tree is now complete. Now we have to post-process the histograms.
   
   // ---------------------------------------------------------
   // This means that the energy histograms should be normalized. We should normalize them with the original energy spectrum: 
   // the clean histogram. However,the purpose is to determine the efficiency p from n detected particles on a total of N. As
   // discussed in the statistical analysis of the LaTeX log, this should be done as p = (n+1)/(N+2) and 
   // sigma_p = sqrt(p(1-p)/(N+3)). Hence this is the calculation we mean with normalization:
      
   Double_t bincontent_hist = 0.0;
   Double_t bincontent_clean = 0.0;
   Double_t bincontent = 0.0;
   Double_t binerror = 0.0;
   
   for (Int_t k=1; k<(nbins+1); ++k)
   {
     // -----------------------
     // Normalize the veto-histograms:
     
     // Secondary particles:
     bincontent_hist = veto_hist_Sec->GetBinContent(k);
     bincontent_clean = clean_hist->GetBinContent(k);
     bincontent = (bincontent_hist + 1.0)/(bincontent_clean + 2.0);
     binerror = TMath::Sqrt(bincontent*(1.0 - bincontent)/(bincontent_clean + 3.0));
     veto_hist_Sec->SetBinContent(k,bincontent);
     veto_hist_Sec->SetBinError(k,binerror);
     
     // Pass:
     bincontent_hist = veto_hist_Pass->GetBinContent(k);
     bincontent_clean = clean_hist->GetBinContent(k);
     bincontent = (bincontent_hist + 1.0)/(bincontent_clean + 2.0);
     binerror = TMath::Sqrt(bincontent*(1.0 - bincontent)/(bincontent_clean + 3.0));
     veto_hist_Pass->SetBinContent(k,bincontent);
     veto_hist_Pass->SetBinError(k,binerror);
     
      // Pure Back:
     bincontent_hist = veto_hist_PureBack->GetBinContent(k);
     bincontent_clean = clean_hist->GetBinContent(k);
     bincontent = (bincontent_hist + 1.0)/(bincontent_clean + 2.0);
     binerror = TMath::Sqrt(bincontent*(1.0 - bincontent)/(bincontent_clean + 3.0));
     veto_hist_PureBack->SetBinContent(k,bincontent);
     veto_hist_PureBack->SetBinError(k,binerror);
     
      // Pure Prim:
     bincontent_hist = veto_hist_PurePrim->GetBinContent(k);
     bincontent_clean = clean_hist->GetBinContent(k);
     bincontent = (bincontent_hist + 1.0)/(bincontent_clean + 2.0);
     binerror = TMath::Sqrt(bincontent*(1.0 - bincontent)/(bincontent_clean + 3.0));
     veto_hist_PurePrim->SetBinContent(k,bincontent);
     veto_hist_PurePrim->SetBinError(k,binerror);
     
      // Prim & Back:
     bincontent_hist = veto_hist_PrimBack->GetBinContent(k);
     bincontent_clean = clean_hist->GetBinContent(k);
     bincontent = (bincontent_hist + 1.0)/(bincontent_clean + 2.0);
     binerror = TMath::Sqrt(bincontent*(1.0 - bincontent)/(bincontent_clean + 3.0));
     veto_hist_PrimBack->SetBinContent(k,bincontent);
     veto_hist_PrimBack->SetBinError(k,binerror);
     
     // ----------------------------
     // Normalize TOF-histograms:
     
     // MC TOF:
     bincontent_hist = veto_hist_TOF->GetBinContent(k);
     bincontent_clean = clean_hist->GetBinContent(k);
     bincontent = (bincontent_hist + 1.0)/(bincontent_clean + 2.0);
     binerror = TMath::Sqrt(bincontent*(1.0 - bincontent)/(bincontent_clean + 3.0));
     veto_hist_TOF->SetBinContent(k,bincontent);
     veto_hist_TOF->SetBinError(k,binerror);
     
     // MC TOF primary event fraction:
     bincontent_hist = veto_hist_TOF_prim->GetBinContent(k);
     bincontent_clean = clean_hist->GetBinContent(k);
     bincontent = (bincontent_hist + 1.0)/(bincontent_clean + 2.0);
     binerror = TMath::Sqrt(bincontent*(1.0 - bincontent)/(bincontent_clean + 3.0));
     veto_hist_TOF_prim->SetBinContent(k,bincontent);
     veto_hist_TOF_prim->SetBinError(k,binerror);
     
     // 100 ps TOF:
     bincontent_hist = veto_hist_TOF_Rand->GetBinContent(k);
     bincontent_clean = clean_hist->GetBinContent(k);
     bincontent = (bincontent_hist + 1.0)/(bincontent_clean + 2.0);
     binerror = TMath::Sqrt(bincontent*(1.0 - bincontent)/(bincontent_clean + 3.0));
     veto_hist_TOF_Rand->SetBinContent(k,bincontent);
     veto_hist_TOF_Rand->SetBinError(k,binerror);
     
     // 100 ps TOF primary event fraction:
     bincontent_hist = veto_hist_TOF_Rand_prim->GetBinContent(k);
     bincontent_clean = clean_hist->GetBinContent(k);
     bincontent = (bincontent_hist + 1.0)/(bincontent_clean + 2.0);
     binerror = TMath::Sqrt(bincontent*(1.0 - bincontent)/(bincontent_clean + 3.0));
     veto_hist_TOF_Rand_prim->SetBinContent(k,bincontent);
     veto_hist_TOF_Rand_prim->SetBinError(k,binerror);
     
     // -----------------------
     // Normalize the energy efficiency histograms:
     
     // VETO:
     bincontent_hist = veto_hist->GetBinContent(k);
     bincontent_clean = clean_hist->GetBinContent(k);
     bincontent = (bincontent_hist + 1.0)/(bincontent_clean + 2.0);
     binerror = TMath::Sqrt(bincontent*(1.0 - bincontent)/(bincontent_clean + 3.0));
     veto_hist->SetBinContent(k,bincontent);
     veto_hist->SetBinError(k,binerror);

     // NeuLAND:
     bincontent_hist = Neu_hist->GetBinContent(k);
     bincontent_clean = clean_hist->GetBinContent(k);
     bincontent = (bincontent_hist + 1.0)/(bincontent_clean + 2.0);
     binerror = TMath::Sqrt(bincontent*(1.0 - bincontent)/(bincontent_clean + 3.0));
     Neu_hist->SetBinContent(k,bincontent);
     Neu_hist->SetBinError(k,binerror);
     
     // TRUE-histogram:
     bincontent_hist = Real_hist->GetBinContent(k);
     bincontent_clean = clean_hist->GetBinContent(k);
     bincontent = (bincontent_hist + 1.0)/(bincontent_clean + 2.0);
     binerror = TMath::Sqrt(bincontent*(1.0 - bincontent)/(bincontent_clean + 3.0));
     Real_hist->SetBinContent(k,bincontent);
     Real_hist->SetBinError(k,binerror);
   }
   
   // Do the same for the LFH-histograms.
   // NOTE: Since a primary neutron interaction can only occur either in the VETO, or in NeuLAND,
   // the content of the VETO-histogram has to be added to the NeuLAND histogram. Hence we do this
   // manually:
   
   for (Int_t k=1; k<(nbins+1); ++k)
   {
     bincontent_hist = Neu_hist_LFH->GetBinContent(k);
     bincontent_clean = veto_hist_LFH->GetBinContent(k);
     
     bincontent_hist = bincontent_hist + bincontent_clean;
     
     Neu_hist_LFH->SetBinContent(k,bincontent_hist);
   }
   
   // Now normalize:
   for (Int_t k=1; k<(nbins+1); ++k)
   {
     // VETO:
     bincontent_hist = veto_hist_LFH->GetBinContent(k);
     bincontent_clean = clean_hist_LFH->GetBinContent(k);
     bincontent = (bincontent_hist + 1.0)/(bincontent_clean + 2.0);
     binerror = TMath::Sqrt(bincontent*(1.0 - bincontent)/(bincontent_clean + 3.0));
     veto_hist_LFH->SetBinContent(k,bincontent);
     veto_hist_LFH->SetBinError(k,binerror);
     
     // NeuLAND:
     bincontent_hist = Neu_hist_LFH->GetBinContent(k);
     bincontent_clean = clean_hist_LFH->GetBinContent(k);
     bincontent = (bincontent_hist + 1.0)/(bincontent_clean + 2.0);
     binerror = TMath::Sqrt(bincontent*(1.0 - bincontent)/(bincontent_clean + 3.0));
     Neu_hist_LFH->SetBinContent(k,bincontent);
     Neu_hist_LFH->SetBinError(k,binerror);
     
     // TRUE histogram:
     bincontent_hist = Real_hist_LFH->GetBinContent(k);
     bincontent_clean = clean_hist_LFH->GetBinContent(k);
     bincontent = (bincontent_hist + 1.0)/(bincontent_clean + 2.0);
     binerror = TMath::Sqrt(bincontent*(1.0 - bincontent)/(bincontent_clean + 3.0));
     Real_hist_LFH->SetBinContent(k,bincontent);
     Real_hist_LFH->SetBinError(k,binerror);
   }
   
   // ========================================================================================================================================
   // Now we also have TH3D-histograms that should be checked. we have Esecondary VS z-point(production) VS traveldistance through NeuLAND.
   // Now we must go bin-by-bin over this and determine if for the energy under consideration, the traveldistance is bigger/smaller then what
   // the particle can travel through NeuLAND. if the particle cannot escape NeuLAND, we will delete the bin. In the end, we will then integrate
   // the traveldistance out.
   
   // ----------------------------------
   // begin with getting the Bethe-Bloch information on protons, pions, muons, C-ions and electrons. For electrons we apply a special version, which is
   // contained in the computation of the TH1D Bethe_Electrons. Hence we can use the same code here! Notice that muons and pions also decay during
   // their trip, but this effect is much smaller then the Bethe-Bloch effect. Hence we ignore it.
   TH1D* Bethe_Proton = (TH1D*) file->Get("Bethe_Protons");
   TH1D* Bethe_Cion = (TH1D*) file->Get("Bethe_Cions");
   TH1D* Bethe_Electron = (TH1D*) file->Get("Bethe_Electrons");
   TH1D* Bethe_Pion = (TH1D*) file->Get("Bethe_Pions");
   TH1D* Bethe_Muon = (TH1D*) file->Get("Bethe_Muons");
   
   // Now we will loop over the TH3D for protons:
   Double_t Bethe_dist = 0.0;
   Double_t real_dist = 0.0;
   
   // NOTE: We must include the underflow and overflow bins in the cut, else we might miss tracks produces just outside NeuLAND.
   for (Int_t kE = 0; kE<(nbinsEs+2); ++kE)
   {
     for (Int_t kz = 0; kz<(nbinsX+2); ++kz)
     {
       for (Int_t kt = 0; kt<(nbinsX+2); ++kt)
       {
	 real_dist = ((((Int_t) kt) - 0.5)/((Int_t) nbinsX))*maxtraveldistance;
	 
	 // protons:
	 Bethe_dist = Bethe_Proton->GetBinContent(kE);
	 if (Bethe_dist<real_dist) // if Bethe_dist < real_dist, the particles travel distances is bigger then their stopping range. Hence
				   // they will not leave NeuLAND, so we cut out these bins.
	 {
	   EzdREACH_proton->SetBinContent(kE,kz,kt,0.0);
           EzdREACH_proton->SetBinError(kE,kz,kt,0.0);
	 }
	 
	 // C-ions:
	 Bethe_dist = Bethe_Cion->GetBinContent(kE);
	 if (Bethe_dist<real_dist)
	 {
	   EzdREACH_Cion->SetBinContent(kE,kz,kt,0.0);
           EzdREACH_Cion->SetBinError(kE,kz,kt,0.0);
	 }
	 
	 // And electrons. NOTE: Here we use the same code, but Bethe_dist is computed in a different way!
         Bethe_dist = Bethe_Electron->GetBinContent(kE);
	 if (Bethe_dist<real_dist)
	 {
	   EzdREACH_electron->SetBinContent(kE,kz,kt,0.0);
	   EzdREACH_electron->SetBinError(kE,kz,kt,0.0);
	 }
	 
	 // Pions:
	 Bethe_dist = Bethe_Pion->GetBinContent(kE);
	 if (Bethe_dist<real_dist)
	 {
	   EzdREACH_pion->SetBinContent(kE,kz,kt,0.0);
           EzdREACH_pion->SetBinError(kE,kz,kt,0.0);
	 }
	 
	 // Muons:
	 Bethe_dist = Bethe_Muon->GetBinContent(kE);
	 if (Bethe_dist<real_dist)
	 {
	   EzdREACH_muon->SetBinContent(kE,kz,kt,0.0);
           EzdREACH_muon->SetBinError(kE,kz,kt,0.0);
	 }
	 
	 // NOTE: Muons and pions also decay during their trip to the VETO. This effect is small, but
	 // causes the number of pions and muons in the REACH-histograms to be somewhat higher then
	 // in the HIT-histograms. Since only the distance through NeuLAND is known for the particles
	 // and not the fukll travel distance to the VETO, incorporating the decay is very difficult.
	 // Therefore we do not do this.
	 
	 // Done!
       } 
     }
   }
   
   // -----------------------------------
   // Now for neutrons and gammas we will model the basic law of radiation instead:
   Double_t z_lifetime_neutrons = Doubles->GetBinContent(27);
   Double_t z_lifetime_gammas = Doubles->GetBinContent(28);
   Double_t p_z_n = 1.0e-2/z_lifetime_neutrons; // convert to [cm^-1]
   Double_t p_z_g = 1.0e-2/z_lifetime_gammas; // convert to [cm^-1]
   bincontent = 0.0;
   binerror = 0.0;
   
   for (Int_t kE = 0; kE<(nbinsEs+2); ++kE)
   {
     for (Int_t kz = 0; kz<(nbinsX+2); ++kz)
     {
       for (Int_t kt = 0; kt<(nbinsX+2); ++kt)
       {
	 real_dist = ((((Int_t) kt) - 0.5)/((Int_t) nbinsX))*maxtraveldistance;

         // Here we do not cut out bins, but attenuate them instead with the basic law of radiation.
	 
	 // ---------------
	 // neutrons:
	 
	 // Get the bins:
	 bincontent = EzdREACH_neutron->GetBinContent(kE,kz,kt);
         binerror = EzdREACH_neutron->GetBinError(kE,kz,kt);
	 
	 // Attenuate:
	 bincontent = bincontent*TMath::Exp(-1.0*p_z_n*real_dist);
	 binerror = binerror*TMath::Exp(-1.0*p_z_n*real_dist);
	 
	 // Re-insert them:
	 EzdREACH_neutron->SetBinContent(kE,kz,kt,bincontent);
	 EzdREACH_neutron->SetBinError(kE,kz,kt,binerror);
	 
         // ---------------
	 // neutrons:
	 
	 // Get the bins:
	 bincontent = EzdREACH_gamma->GetBinContent(kE,kz,kt);
         binerror = EzdREACH_gamma->GetBinError(kE,kz,kt);
	 
	 // Attenuate:
	 bincontent = bincontent*TMath::Exp(-1.0*p_z_g*real_dist);
	 binerror = binerror*TMath::Exp(-1.0*p_z_g*real_dist);
	 
	 // Re-insert them:
	 EzdREACH_gamma->SetBinContent(kE,kz,kt,bincontent);
	 EzdREACH_gamma->SetBinError(kE,kz,kt,binerror);  
	 
	 // --------------
	 // Done!
	 
       }
     }
   }

   // ===========================================================================================================
   // Now we will integrate out the TH3D histograms before saving them. We either integrate out z
   // or we integrate out the traveldistance.
   
   TH2D* EzREACH_proton = (TH2D*) EzdREACH_proton->Project3D("yx");
   TH2D* EzREACH_neutron = (TH2D*) EzdREACH_neutron->Project3D("yx");
   TH2D* EzREACH_electron = (TH2D*) EzdREACH_electron->Project3D("yx");
   TH2D* EzREACH_Cion = (TH2D*) EzdREACH_Cion->Project3D("yx");
   TH2D* EzREACH_gamma = (TH2D*) EzdREACH_gamma->Project3D("yx");
   TH2D* EzREACH_pion = (TH2D*) EzdREACH_pion->Project3D("yx");
   TH2D* EzREACH_muon = (TH2D*) EzdREACH_muon->Project3D("yx");
   //TH2D* EzREACH_other = (TH2D*) EzdREACH_other->Project3D("yx");
   
   TH2D* EdREACH_proton = (TH2D*) EzdREACH_proton->Project3D("zx");
   TH2D* EdREACH_neutron = (TH2D*) EzdREACH_neutron->Project3D("zx");
   TH2D* EdREACH_electron = (TH2D*) EzdREACH_electron->Project3D("zx");
   TH2D* EdREACH_Cion = (TH2D*) EzdREACH_Cion->Project3D("zx");
   TH2D* EdREACH_gamma = (TH2D*) EzdREACH_gamma->Project3D("zx");
   TH2D* EdREACH_pion = (TH2D*) EzdREACH_pion->Project3D("zx");
   TH2D* EdREACH_muon = (TH2D*) EzdREACH_muon->Project3D("zx");
   //TH2D* EdREACH_other = (TH2D*) EzdREACH_other->Project3D("zx");
   
   // Do the same for the geometrical constrain histograms:
   TH2D* EzAIM_proton = (TH2D*) EzdAIM_proton->Project3D("yx");
   TH2D* EzAIM_neutron = (TH2D*) EzdAIM_neutron->Project3D("yx");
   TH2D* EzAIM_electron = (TH2D*) EzdAIM_electron->Project3D("yx");
   TH2D* EzAIM_Cion = (TH2D*) EzdAIM_Cion->Project3D("yx");
   TH2D* EzAIM_gamma = (TH2D*) EzdAIM_gamma->Project3D("yx");
   TH2D* EzAIM_pion = (TH2D*) EzdAIM_pion->Project3D("yx");
   TH2D* EzAIM_muon = (TH2D*) EzdAIM_muon->Project3D("yx");
   TH2D* EzAIM_other = (TH2D*) EzdAIM_other->Project3D("yx");
   
   TH2D* EdAIM_proton = (TH2D*) EzdAIM_proton->Project3D("zx");
   TH2D* EdAIM_neutron = (TH2D*) EzdAIM_neutron->Project3D("zx");
   TH2D* EdAIM_electron = (TH2D*) EzdAIM_electron->Project3D("zx");
   TH2D* EdAIM_Cion = (TH2D*) EzdAIM_Cion->Project3D("zx");
   TH2D* EdAIM_gamma = (TH2D*) EzdAIM_gamma->Project3D("zx");
   TH2D* EdAIM_pion = (TH2D*) EzdAIM_pion->Project3D("zx");
   TH2D* EdAIM_muon = (TH2D*) EzdAIM_muon->Project3D("zx");
   TH2D* EdAIM_other = (TH2D*) EzdAIM_other->Project3D("zx");
   
   // The same as well for the unconstrained histograms:
   TH2D* EzALL_proton = (TH2D*) EzdALL_proton->Project3D("yx");
   TH2D* EzALL_neutron = (TH2D*) EzdALL_neutron->Project3D("yx");
   TH2D* EzALL_electron = (TH2D*) EzdALL_electron->Project3D("yx");
   TH2D* EzALL_Cion = (TH2D*) EzdALL_Cion->Project3D("yx");
   TH2D* EzALL_gamma = (TH2D*) EzdALL_gamma->Project3D("yx");
   TH2D* EzALL_pion = (TH2D*) EzdALL_pion->Project3D("yx");
   TH2D* EzALL_muon = (TH2D*) EzdALL_muon->Project3D("yx");
   TH2D* EzALL_other = (TH2D*) EzdALL_other->Project3D("yx");
   
   TH2D* EdALL_proton = (TH2D*) EzdALL_proton->Project3D("zx");
   TH2D* EdALL_neutron = (TH2D*) EzdALL_neutron->Project3D("zx");
   TH2D* EdALL_electron = (TH2D*) EzdALL_electron->Project3D("zx");
   TH2D* EdALL_Cion = (TH2D*) EzdALL_Cion->Project3D("zx");
   TH2D* EdALL_gamma = (TH2D*) EzdALL_gamma->Project3D("zx");
   TH2D* EdALL_pion = (TH2D*) EzdALL_pion->Project3D("zx");
   TH2D* EdALL_muon = (TH2D*) EzdALL_muon->Project3D("zx");
   TH2D* EdALL_other = (TH2D*) EzdALL_other->Project3D("zx");
   
   // The HIT-histograms and DET-histograms do not need to be projected, they are already TH2D.

   // ===============================================================================================
   // Then finally, the histograms should be stored in the file. For that we need to do some work first
   // in order to construct the correct key:
   
   // First get the file:
   TFile* fileH = new TFile("Histograms.root","Update");
   
   // Then we get the VETO-counter, counting how many displacements we have made:
   Int_t VetoCounter = Integers->GetBinContent(25);
   
   // Now we need to give them some kind of name depending on the counter. Or rather: on the energy threshold used
   // for the detection. We will do it in MeV up to 2 digits:
   Double_t DName1 = Ethreshold*1000.0;
   Double_t DName2 = Ethreshold*100000.0;
   Int_t IName1 = (Double_t) DName1; // Just cuts of at the "."; no roundof, just a cut. 24.93-->24
   Int_t IName2 = (Double_t) DName2;
   IName2 = IName2 - 100*IName1;
   TString StupidStuff = "";
   TString SName1 = StupidStuff.Itoa(IName1,10);
   TString SName2 = StupidStuff.Itoa(IName2,10);
   TString Ename = SName1 + "." + SName2 + "[MeV]";
   // Now Ename should be the correct string:
   
   // We also require a name dependence on the VetoCounter:
   TString Srun = StupidStuff.Itoa(VetoCounter,10);
   
   // ------------------------------------------------------
   // Now define the histogram names:
   
   // LFH-histograms:
   TString SpectrumNameLFH = "Full_Spectrum_LFH";
   SpectrumNameLFH = SpectrumNameLFH + "_" + Srun;
   TString NeuNameLFH = "NeuLAND_Detection_Probability_LFH";
   NeuNameLFH = NeuNameLFH + "_" + Srun;
   TString VetoNameLFH = "Veto_Detection_Probability_LFH";
   VetoNameLFH = VetoNameLFH + "_" + Srun;
   TString TrueNameLFH = "TRUE_Detection_Probability_LFH";
   TrueNameLFH = TrueNameLFH + "_" + Srun;
   
   // Energy histograms:
   TString SpectrumName = "Full_Spectrum_";
   SpectrumName = SpectrumName + Ename + "_" + Srun;
   TString NeuName = "NeuLAND_Detection_Probability_";
   NeuName = NeuName + Ename + "_" + Srun;
   TString VetoName = "Veto_Detection_Probability_";
   VetoName = VetoName + Ename + "_" + Srun;
   TString TrueName = "TRUE_Detection_Probability_";
   TrueName = TrueName + Ename + "_" + Srun;
   
   // VETO histograms:
   TString VetoNameSec = "Veto_Sec_Probability_";
   VetoNameSec = VetoNameSec + Ename + "_" + Srun;
   TString VetoNamePass = "Veto_Pass_Probability_";
   VetoNamePass = VetoNamePass + Ename + "_" + Srun;
   TString VetoNamePureBack = "Veto_Pure_Back_Probability_";
   VetoNamePureBack = VetoNamePureBack + Ename + "_" + Srun;
   TString VetoNamePurePrim = "Veto_Pure_Prim_Probability_";
   VetoNamePurePrim = VetoNamePurePrim + Ename + "_" + Srun;
   TString VetoNamePrimBack = "Veto_Prim&Back_Probability_";
   VetoNamePrimBack = VetoNamePrimBack + Ename + "_" + Srun;
   TString VetoNameTOF = "Veto_Detection_Probability_TOF_MC_";
   VetoNameTOF = VetoNameTOF + Ename + "_" + Srun;
   TString VetoNameTOFR = "Veto_Detection_Probability_TOF100ps_";
   VetoNameTOFR = VetoNameTOFR + Ename + "_" + Srun;
   TString VetoNameTOFprim = "Veto_Detection_Probability_TOF_MC_prim_";
   VetoNameTOFprim = VetoNameTOFprim + Ename + "_" + Srun;
   TString VetoNameTOFRprim = "Veto_Detection_Probability_TOF100ps_prim_";
   VetoNameTOFRprim = VetoNameTOFRprim + Ename + "_" + Srun;
   
   // TOF histograms:
   TString VetoTOFname = "VETO_MC_TOF_Distribution_";
   VetoTOFname = VetoTOFname + Ename + "_" + Srun;
   TString VetoTOFRname = "VETO_100ps_TOF_Distribution_";
   VetoTOFRname = VetoTOFRname + Ename + "_" + Srun;
   TString NeuTOFname = "NeuLAND_MC_TOF_Distribution_";
   NeuTOFname = NeuTOFname + Ename + "_" + Srun;
   TString NeuTOFRname = "NeuLAND_100ps_TOF_Distribution_";
   NeuTOFRname = NeuTOFRname + Ename + "_" + Srun;
      
   // Phi-histograms of secondary particles:
   TString PhidetectName = "LAB_phi_ALLparticles_detected_";
   PhidetectName = PhidetectName + Ename + "_" + Srun;
   TString PhinotName = "LAB_phi_ALLparticles_NOTdetected_";
   PhinotName = PhinotName + Ename + "_" + Srun;
   TString PhiallName = "LAB_phi_ALLparticles_IRRdetected_"; // IRR=irrelevant whether detected or not.
   PhiallName = PhiallName + Ename + "_" + Srun;
   
   // E vs theta of secondary particles:
   TString EthetadetectName = "E_vs_theta_ALLparticles_detected_";
   EthetadetectName = EthetadetectName + Ename + "_" + Srun;
   TString EthetanotName = "E_vs_theta_ALLparticles_NOTdetected_";
   EthetanotName = EthetanotName + Ename + "_" + Srun;
   TString EthetaallName = "E_vs_theta_ALLparticles_IRRdetected_";
   EthetaallName = EthetaallName + Ename + "_" + Srun;
   
   // E vs z of secondary particles:
   TString EzdetectName = "E_vs_z_ALLparticles_detected_";
   EzdetectName = EzdetectName + Ename + "_" + Srun;
   TString EznotName = "E_vs_z_ALLparticles_NOTdetected_";
   EznotName = EznotName + Ename + "_" + Srun;
   TString EzallName = "E_vs_z_ALLparticles_IRRdetected_";
   EzallName = EzallName + Ename + "_" + Srun;
   
   // E vs z/traveldist for secondary PROTONS:
   TString EzALLprotonName = "E_vs_z_Protons_IRRdetected_";
   EzALLprotonName = EzALLprotonName + Ename + "_" + Srun;
   TString EdALLprotonName = "E_vs_TravelDist_Protons_IRRdetected_";
   EdALLprotonName = EdALLprotonName + Ename + "_" + Srun;
   TString EzREACHprotonName = "E_vs_z_Protons_IRRdetected_ReachVETO_";
   EzREACHprotonName = EzREACHprotonName + Ename + "_" + Srun;
   TString EdREACHprotonName = "E_vs_TravelDist_Protons_IRRdetected_ReachVETO_";
   EdREACHprotonName = EdREACHprotonName + Ename + "_" + Srun;
   TString EzAIMprotonName = "E_vs_z_Protons_IRRdetected_AimedVETO_";
   EzAIMprotonName = EzAIMprotonName + Ename + "_" + Srun;
   TString EdAIMprotonName = "E_vs_TravelDist_Protons_IRRdetected_AimedVETO_";
   EdAIMprotonName = EdAIMprotonName + Ename + "_" + Srun;
   TString EzHITprotonName = "E_vs_z_Protons_IRRdetected_HitVETO_";
   EzHITprotonName = EzHITprotonName + Ename + "_" + Srun;
   TString EzDETprotonName = "E_vs_z_Protons_IRRdetected_DetectVETO_";
   EzDETprotonName = EzDETprotonName + Ename + "_" + Srun;
   
   // E vs z/traveldist for secondary NEUTRONS:
   TString EzALLneutronName = "E_vs_z_Neutrons_IRRdetected_";
   EzALLneutronName = EzALLneutronName + Ename + "_" + Srun;
   TString EdALLneutronName = "E_vs_TravelDist_Neutrons_IRRdetected_";
   EdALLneutronName = EdALLneutronName + Ename + "_" + Srun;
   TString EzAIMneutronName = "E_vs_z_Neutrons_IRRdetected_AimedVETO_";
   EzAIMneutronName = EzAIMneutronName + Ename + "_" + Srun;
   TString EdAIMneutronName = "E_vs_TravelDist_Neutrons_IRRdetected_AimedVETO_";
   EdAIMneutronName = EdAIMneutronName + Ename + "_" + Srun;
   TString EzREACHneutronName = "E_vs_z_Neutrons_IRRdetected_ReachVETO_";
   EzREACHneutronName = EzREACHneutronName + Ename + "_" + Srun;
   TString EdREACHneutronName = "E_vs_TravelDist_Neutrons_IRRdetected_ReachVETO_";
   EdREACHneutronName = EdREACHneutronName + Ename + "_" + Srun;
   TString EzHITneutronName = "E_vs_z_Neutrons_IRRdetected_HitVETO_";
   EzHITneutronName = EzHITneutronName + Ename + "_" + Srun;
   TString EzDETneutronName = "E_vs_z_Neutrons_IRRdetected_DetectVETO_";
   EzDETneutronName = EzDETneutronName + Ename + "_" + Srun;
   
   // E vs z/traveldist for secondary ELECTRONS:
   TString EzALLelectronName = "E_vs_z_Electrons_IRRdetected_";
   EzALLelectronName = EzALLelectronName + Ename + "_" + Srun;
   TString EdALLelectronName = "E_vs_TravelDist_Electrons_IRRdetected_";
   EdALLelectronName = EdALLelectronName + Ename + "_" + Srun;
   TString EzREACHelectronName = "E_vs_z_Electrons_IRRdetected_ReachVETO_";
   EzREACHelectronName = EzREACHelectronName + Ename + "_" + Srun;
   TString EdREACHelectronName = "E_vs_TravelDist_Electrons_IRRdetected_ReachVETO_";
   EdREACHelectronName = EdREACHelectronName + Ename + "_" + Srun;
   TString EzAIMelectronName = "E_vs_z_Electrons_IRRdetected_AimedVETO_";
   EzAIMelectronName = EzAIMelectronName + Ename + "_" + Srun;
   TString EdAIMelectronName = "E_vs_TravelDist_Electrons_IRRdetected_AimedVETO_";
   EdAIMelectronName = EdAIMelectronName + Ename + "_" + Srun;
   TString EzHITelectronName = "E_vs_z_Electrons_IRRdetected_HitVETO_";
   EzHITelectronName = EzHITelectronName + Ename + "_" + Srun;
   TString EzDETelectronName = "E_vs_z_Electrons_IRRdetected_DetectVETO_";
   EzDETelectronName = EzDETelectronName + Ename + "_" + Srun;
   
   // E vs z/traveldist for secondary GAMMAS:
   TString EzALLgammaName = "E_vs_z_Gammas_IRRdetected_";
   EzALLgammaName = EzALLgammaName + Ename + "_" + Srun;
   TString EdALLgammaName = "E_vs_TravelDist_Gammas_IRRdetected_";
   EdALLgammaName = EdALLgammaName + Ename + "_" + Srun;
   TString EzAIMgammaName = "E_vs_z_Gammas_IRRdetected_AimedVETO_";
   EzAIMgammaName = EzAIMgammaName + Ename + "_" + Srun;
   TString EdAIMgammaName = "E_vs_TravelDist_Gammas_IRRdetected_AimedVETO_";
   EdAIMgammaName = EdAIMgammaName + Ename + "_" + Srun;
   TString EzREACHgammaName = "E_vs_z_Gammas_IRRdetected_ReachVETO_";
   EzREACHgammaName = EzREACHgammaName + Ename + "_" + Srun;
   TString EdREACHgammaName = "E_vs_TravelDist_Gammas_IRRdetected_ReachVETO_";
   EdREACHgammaName = EdREACHgammaName + Ename + "_" + Srun;
   TString EzHITgammaName = "E_vs_z_Gammas_IRRdetected_HitVETO_";
   EzHITgammaName = EzHITgammaName + Ename + "_" + Srun;
   TString EzDETgammaName = "E_vs_z_Gammas_IRRdetected_DetectVETO_";
   EzDETgammaName = EzDETgammaName + Ename + "_" + Srun;
   
   // E vs z/traveldist for secondary PIONS:
   TString EzALLpionName = "E_vs_z_Pions_IRRdetected_";
   EzALLpionName = EzALLpionName + Ename + "_" + Srun;
   TString EdALLpionName = "E_vs_TravelDist_Pions_IRRdetected_";
   EdALLpionName = EdALLpionName + Ename + "_" + Srun;
   TString EzREACHpionName = "E_vs_z_Pions_IRRdetected_ReachVETO_";
   EzREACHpionName = EzREACHpionName + Ename + "_" + Srun;
   TString EdREACHpionName = "E_vs_TravelDist_Pions_IRRdetected_ReachVETO_";
   EdREACHpionName = EdREACHpionName + Ename + "_" + Srun;
   TString EzAIMpionName = "E_vs_z_Pions_IRRdetected_AimedVETO_";
   EzAIMpionName = EzAIMpionName + Ename + "_" + Srun;
   TString EdAIMpionName = "E_vs_TravelDist_Pions_IRRdetected_AimedVETO_";
   EdAIMpionName = EdAIMpionName + Ename + "_" + Srun;
   TString EzHITpionName = "E_vs_z_Pions_IRRdetected_HitVETO_";
   EzHITpionName = EzHITpionName + Ename + "_" + Srun;
   TString EzDETpionName = "E_vs_z_Pions_IRRdetected_DetectVETO_";
   EzDETpionName = EzDETpionName + Ename + "_" + Srun;
   
   // E vs z/traveldist for secondary MUONS:
   TString EzALLmuonName = "E_vs_z_Muons_IRRdetected_";
   EzALLmuonName = EzALLmuonName + Ename + "_" + Srun;
   TString EdALLmuonName = "E_vs_TravelDist_Muons_IRRdetected_";
   EdALLmuonName = EdALLmuonName + Ename + "_" + Srun;
   TString EzREACHmuonName = "E_vs_z_Muons_IRRdetected_ReachVETO_";
   EzREACHmuonName = EzREACHmuonName + Ename + "_" + Srun;
   TString EdREACHmuonName = "E_vs_TravelDist_Muons_IRRdetected_ReachVETO_";
   EdREACHmuonName = EdREACHmuonName + Ename + "_" + Srun;
   TString EzAIMmuonName = "E_vs_z_Muons_IRRdetected_AimedVETO_";
   EzAIMmuonName = EzAIMmuonName + Ename + "_" + Srun;
   TString EdAIMmuonName = "E_vs_TravelDist_Muons_IRRdetected_AimedVETO_";
   EdAIMmuonName = EdAIMmuonName + Ename + "_" + Srun;
   TString EzHITmuonName = "E_vs_z_Muons_IRRdetected_HitVETO_";
   EzHITmuonName = EzHITmuonName + Ename + "_" + Srun;
   TString EzDETmuonName = "E_vs_z_Muons_IRRdetected_DetectVETO_";
   EzDETmuonName = EzDETmuonName + Ename + "_" + Srun;
   
   // E vs z/traveldist for secondary C-IONS:
   TString EzALLCionName = "E_vs_z_Cions_IRRdetected_";
   EzALLCionName = EzALLCionName + Ename + "_" + Srun;
   TString EdALLCionName = "E_vs_TravelDist_Cions_IRRdetected_";
   EdALLCionName = EdALLCionName + Ename + "_" + Srun;
   TString EzREACHCionName = "E_vs_z_Cions_IRRdetected_ReachVETO_";
   EzREACHCionName = EzREACHCionName + Ename + "_" + Srun;
   TString EdREACHCionName = "E_vs_TravelDist_Cions_IRRdetected_ReachVETO_";
   EdREACHCionName = EdREACHCionName + Ename + "_" + Srun;
   TString EzAIMCionName = "E_vs_z_Cions_IRRdetected_AimedVETO_";
   EzAIMCionName = EzAIMCionName + Ename + "_" + Srun;
   TString EdAIMCionName = "E_vs_TravelDist_Cions_IRRdetected_AimedVETO_";
   EdAIMCionName = EdAIMCionName + Ename + "_" + Srun;
   TString EzHITCionName = "E_vs_z_Cions_IRRdetected_HitVETO_";
   EzHITCionName = EzHITCionName + Ename + "_" + Srun;
   TString EzDETCionName = "E_vs_z_Cions_IRRdetected_DetectVETO_";
   EzDETCionName = EzDETCionName + Ename + "_" + Srun;
   
   // E vs z/traveldist for secondary REMAINDERS:
   TString EzALLotherName = "E_vs_z_Remainders_IRRdetected_";
   EzALLotherName = EzALLotherName + Ename + "_" + Srun;
   TString EdALLotherName = "E_vs_TravelDist_Remainders_IRRdetected_";
   EdALLotherName = EdALLotherName + Ename + "_" + Srun;
   TString EzAIMotherName = "E_vs_z_Remainders_IRRdetected_AimedVETO_";
   EzAIMotherName = EzAIMotherName + Ename + "_" + Srun;
   TString EdAIMotherName = "E_vs_TravelDist_Remainders_IRRdetected_AimedVETO_";
   EdAIMotherName = EdAIMotherName + Ename + "_" + Srun;
   TString EzHITotherName = "E_vs_z_Remainders_IRRdetected_HitVETO_";
   EzHITotherName = EzHITotherName + Ename + "_" + Srun;
   TString EzDETotherName = "E_vs_z_Remainders_IRRdetected_DetectVETO_";
   EzDETotherName = EzDETotherName + Ename + "_" + Srun;
   
   // Do the rest:
   TString Secondary_freq_Name = "Secondary_frequency_";
   Secondary_freq_Name = Secondary_freq_Name + Ename + "_" + Srun;

   // -----------------------------------------------------   
   // Then write the histograms to the file.
   cout << "Post-processing was succesful. We will now write our results to the file.\n\n";

   // LFH histograms:
   clean_hist_LFH->Write(SpectrumNameLFH,2);
   veto_hist_LFH->Write(VetoNameLFH,2);
   Neu_hist_LFH->Write(NeuNameLFH,2);
   Real_hist_LFH->Write(TrueNameLFH,2);
   
   // Energy histograms:
   clean_hist->Write(SpectrumName,2);
   veto_hist->Write(VetoName,2);
   Neu_hist->Write(NeuName,2);
   Real_hist->Write(TrueName,2);
   
   // veto histograms:
   veto_hist_Sec->Write(VetoNameSec,2);
   veto_hist_Pass->Write(VetoNamePass,2);
   veto_hist_PureBack->Write(VetoNamePureBack,2);
   veto_hist_PurePrim->Write(VetoNamePurePrim,2);
   veto_hist_PrimBack->Write(VetoNamePrimBack,2);
   veto_hist_TOF->Write(VetoNameTOF,2);
   veto_hist_TOF_Rand->Write(VetoNameTOFR,2);
   veto_hist_TOF_prim->Write(VetoNameTOFprim,2);
   veto_hist_TOF_Rand_prim->Write(VetoNameTOFRprim,2);
   
   // TOF histograms:
   TOF_VETO_MC->Write(VetoTOFname,2);
   TOF_VETO_Rand->Write(VetoTOFRname,2);
   TOF_NeuLAND_MC->Write(NeuTOFname,2);
   TOF_NeuLAND_Rand->Write(NeuTOFRname,2);
   
   // Phi-histograms of secondary particles:
   Phi_detect->Write(PhidetectName,2);
   Phi_not->Write(PhinotName,2);
   Phi_all->Write(PhiallName,2);
   
   // E vs theta of secondary particles:
   Etheta_detect->Write(EthetadetectName,2);
   Etheta_not->Write(EthetanotName,2);
   Etheta_all->Write(EthetaallName,2);
   
   // E vs z of secondary particles:
   Ez_detect->Write(EzdetectName,2);
   Ez_not->Write(EznotName,2);
   Ez_all->Write(EzallName,2);
   
   // E vs z/traveldist for secondary PROTONS:
   EzALL_proton->Write(EzALLprotonName,2);
   EdALL_proton->Write(EdALLprotonName,2);
   EzREACH_proton->Write(EzREACHprotonName,2);
   EdREACH_proton->Write(EdREACHprotonName,2);
   EzAIM_proton->Write(EzAIMprotonName,2);
   EdAIM_proton->Write(EdAIMprotonName,2);
   EzHIT_proton->Write(EzHITprotonName,2);
   EzDET_proton->Write(EzDETprotonName,2);
   
   // E vs z/traveldist for secondary ELECTRONS:
   EzALL_electron->Write(EzALLelectronName,2);
   EdALL_electron->Write(EdALLelectronName,2);
   EzREACH_electron->Write(EzREACHelectronName,2);
   EdREACH_electron->Write(EdREACHelectronName,2);
   EzAIM_electron->Write(EzAIMelectronName,2);
   EdAIM_electron->Write(EdAIMelectronName,2);
   EzHIT_electron->Write(EzHITelectronName,2);
   EzDET_electron->Write(EzDETelectronName,2);
   
   // E vs z/traveldist for secondary GAMMAS:
   EzALL_gamma->Write(EzALLgammaName,2);
   EdALL_gamma->Write(EdALLgammaName,2);
   EzREACH_gamma->Write(EzREACHgammaName,2);
   EdREACH_gamma->Write(EdREACHgammaName,2);
   EzAIM_gamma->Write(EzAIMgammaName,2);
   EdAIM_gamma->Write(EdAIMgammaName,2);
   EzHIT_gamma->Write(EzHITgammaName,2);
   EzDET_gamma->Write(EzDETgammaName,2);
   
   // E vs z/traveldist for secondary NEUTRONS:
   EzALL_neutron->Write(EzALLneutronName,2);
   EdALL_neutron->Write(EdALLneutronName,2);
   EzREACH_neutron->Write(EzREACHneutronName,2);
   EdREACH_neutron->Write(EdREACHneutronName,2);
   EzAIM_neutron->Write(EzAIMneutronName,2);
   EdAIM_neutron->Write(EdAIMneutronName,2);
   EzHIT_neutron->Write(EzHITneutronName,2);
   EzDET_neutron->Write(EzDETneutronName,2);
   
   // E vs z/traveldist for secondary PIONS:
   EzALL_pion->Write(EzALLpionName,2);
   EdALL_pion->Write(EdALLpionName,2);
   EzREACH_pion->Write(EzREACHpionName,2);
   EdREACH_pion->Write(EdREACHpionName,2);
   EzAIM_pion->Write(EzAIMpionName,2);
   EdAIM_pion->Write(EdAIMpionName,2);
   EzHIT_pion->Write(EzHITpionName,2);
   EzDET_pion->Write(EzDETpionName,2);
   
   // E vs z/traveldist for secondary MUONS:
   EzALL_muon->Write(EzALLmuonName,2);
   EdALL_muon->Write(EdALLmuonName,2);
   EzREACH_muon->Write(EzREACHmuonName,2);
   EdREACH_muon->Write(EdREACHmuonName,2);
   EzAIM_muon->Write(EzAIMmuonName,2);
   EdAIM_muon->Write(EdAIMmuonName,2);
   EzHIT_muon->Write(EzHITmuonName,2);
   EzDET_muon->Write(EzDETmuonName,2);
   
   // E vs z/traveldist for secondary CIONS:
   EzALL_Cion->Write(EzALLCionName,2);
   EdALL_Cion->Write(EdALLCionName,2);
   EzREACH_Cion->Write(EzREACHCionName,2);
   EdREACH_Cion->Write(EdREACHCionName,2);
   EzAIM_Cion->Write(EzAIMCionName,2);
   EdAIM_Cion->Write(EdAIMCionName,2);
   EzHIT_Cion->Write(EzHITCionName,2);
   EzDET_Cion->Write(EzDETCionName,2);
   
   // E vs z/traveldist for secondary REMAINDERS:
   EzALL_other->Write(EzALLotherName,2);
   EdALL_other->Write(EdALLotherName,2);
   EzAIM_other->Write(EzAIMotherName,2);
   EdAIM_other->Write(EdAIMotherName,2);
   EzHIT_other->Write(EzHITotherName,2);
   EzDET_other->Write(EzDETotherName,2);
   
   // Write the rest:
   Secondary_freq->Write(Secondary_freq_Name,2);
   
   // Then close the files:
   //fileH->ls();
   fileH->Close();
   file->Close();
   simfile->Close();
   
   // ===============================================================================================
   // Finally: close all blocks:
   cout << "Analysis was succesful. Program is now terminated.\n\n\n";

}
}
}
}

// Add exactly this text at the end of your master-file. Adapt only the name of your program.
// then add the 'using namespace std;' to all source files and hearder files. Use the MakeFile
// and adapt this to your needs. Then it should run.
#ifndef __CINT__
#include <TApplication.h>
int main(int argc, char** argv)
{
  TApplication* TheApp = new TApplication("ReadTreeMeV",&argc,argv);
  ReadTreeMeV();
  // TheApp->Run(); ==> This macro does not contain graphics.
  return 0;
}
#endif
