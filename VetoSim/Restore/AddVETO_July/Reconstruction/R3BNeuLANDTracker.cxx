// R3B NeuLAND Tracker cxx-file. Written by C. A. Douma.

// inclusion of required C++ headers:

// Inclusion of the required ROOT headers:

// Inclusion of the required FAIR headers:

// Inclusion of the required R3B headers:

// Inclusion of the own header file:
#include "R3BNeuLANDTracker.h"

// Inclusion of the headers containing the member functions:
#include "ReadCalibrationFile.h"
#include "IsElastic.h"
#include "Reset.h"
#include "Filter.h"
#include "ApplyCuts.h"
// #include "Reconstruct.h" ==> Later, he needs the AuxSortClustersBeta-function!
#include "Compare.h"

// Using standard namespaces:
using namespace std;

// declaration of some global variables:
Double_t fTarget_Xpos_global;
Double_t fTarget_Ypos_global;
Double_t fTarget_Zpos_global;
Double_t fBeam_Xpos_global;
Double_t fBeam_Ypos_global;
Double_t fBeam_Zpos_global;
Double_t fBeamBeta_global;

// Auxilliary function for sorting clusters with respect to their beta:
// to be used with std::sort from <algorithm>
// It cannot be defined in a header, since we then encounter a problem
// with the required global variables.
bool AuxSortClustersBeta(R3BNeuLANDCluster* c1, R3BNeuLANDCluster* c2)
{
  // first load the start positions of the clusters:
  TVector3 pos1;
  TVector3 pos2;
  c1->StartPosition(pos1);
  c2->StartPosition(pos2);
  
  // Then Correct for the travel time of the beam:
  Double_t c = 29.9792458;
  Double_t Beam_TravelTime = TMath::Sqrt((fTarget_Xpos_global - fBeam_Xpos_global)*(fTarget_Xpos_global - fBeam_Xpos_global) + (fTarget_Ypos_global - fBeam_Ypos_global)*(fTarget_Ypos_global - fBeam_Ypos_global) + (fTarget_Zpos_global - fBeam_Zpos_global)*(fTarget_Zpos_global - fBeam_Zpos_global));
  Beam_TravelTime = Beam_TravelTime/(fBeamBeta_global*c);
  
  Double_t Travel_Dist1 = TMath::Sqrt((pos1.X() - fTarget_Xpos_global)*(pos1.X() - fTarget_Xpos_global) + (pos1.Y() - fTarget_Ypos_global)*(pos1.Y() - fTarget_Ypos_global) + (pos1.Z() - fTarget_Zpos_global)*(pos1.Z() - fTarget_Zpos_global));
  Double_t time1 = c1->GetStartT() - Beam_TravelTime;
  Double_t beta1 = Travel_Dist1/(time1*c);
  
  Double_t Travel_Dist2 = TMath::Sqrt((pos2.X() - fTarget_Xpos_global)*(pos2.X() - fTarget_Xpos_global) + (pos2.Y() - fTarget_Ypos_global)*(pos2.Y() - fTarget_Ypos_global) + (pos2.Z() - fTarget_Zpos_global)*(pos2.Z() - fTarget_Zpos_global));
  Double_t time2 = c2->GetStartT() - Beam_TravelTime;
  Double_t beta2 = Travel_Dist2/(time2*c);
  
  // Now Execute the beta sorting:
  Double_t val1 = TMath::Abs(beta1-fBeamBeta_global)/c1->GetE();
  Double_t val2 = TMath::Abs(beta2-fBeamBeta_global)/c2->GetE();
  if(val1 < val2) 
  {
    return true;
  } 
  else 
  {
    return false;
  }
}

// Now put in the reconstructor:
#include "Reconstruct.h"

// Default Constructor:
R3BNeuLANDTracker::R3BNeuLANDTracker()
    : FairTask("R3BNeuLANDTracker")
{ 
  fBeamBeta = 0.75;
  fBeamEnergy = 600.0;
  fTarget_Xpos = 0.0;
  fTarget_Ypos = 0.0;
  fTarget_Zpos = 0.0;
  fBeam_Xpos = 0.0;
  fBeam_Ypos = 0.0;
  fBeam_Zpos = 0.0;
  VETO_Exists = kFALSE;
  MC_Exists = kFALSE;
  Nplanes = 60;
  Npaddles = 50;
  fCalibration = kFALSE;
  fKappa = 0.1;
  fNumberOfCuts = 0;
  fNeutronNumber = 0;
  fNClustersSorted = 0;
  fNeutronPredefined = 0;
  fApplyCuts = kTRUE;
  fEliminationLevel = 0;
  fNclustersNotEliminated = 0;
  fEdepTotalNotEliminated = 0.0;
  fArrayVertices = new TClonesArray("R3BNeuLANDVertex");
}

// Default Destructor:
R3BNeuLANDTracker::~R3BNeuLANDTracker()
{
  fArrayDigis->Clear();
  fArrayVetoDigis->Clear();
  fArrayClusters->Clear();
  fArrayNeuPrimPoints->Clear();
  fArrayMCTracks->Clear();
  fArrayVertices->Clear();
  fVectorClusters.clear();
}

// NOTE: FairTask initialization function:
InitStatus R3BNeuLANDTracker::Init()
{
  // Each FairTask begins with defining the input/output manager:
  FairRootManager* ioman = FairRootManager::Instance();
  
  // Check that it worked:
  if (NULL == ioman) 
  {
    cout << "### FATAL: No FairRootManager created!\n";
    return kFATAL;
  }

  // Connect to the LandDigis:
  fArrayDigis = (TClonesArray*) ioman->GetObject("LandDigi");
  
  // Again: Check that it worked:
  if(NULL == fArrayDigis) 
  {
    cout << "### FATAL: No Digitizer output detected!\n";
    return kFATAL;
  }
  
  // connect to the NeuLAND clusters:
  fArrayClusters = (TClonesArray*) ioman->GetObject("NeuLANDCluster");
  
  // Again: Check that it worked:
  if(NULL == fArrayClusters) 
  {
    cout << "### FATAL: No NeuLAND Cluster output detected!\n";
    return kFATAL;
  }
  
  // Connect to the VETO digitizer output:
  fArrayVetoDigis = (TClonesArray*) ioman->GetObject("veto_segDigi");
  
  // Connect to the NeuLAND Primary Interaction points from the data:
  fArrayNeuPrimPoints = (TClonesArray*) ioman->GetObject("NeulandPrimaryNeutronInteractionPoints");
  fArrayMCTracks = (TClonesArray*) ioman->GetObject("MCTrack");
  
  // We do not check those three. If they failed, we can still make a reconstruction,
  // although without the VETO we cannot filter out the charged particles and without
  // the Monte Carlo Data we can still make a reconstrcution. Just set the boolians:
  if (NULL == fArrayVetoDigis) {VETO_Exists = kFALSE;} else {VETO_Exists = kTRUE;}
  if ((NULL == fArrayNeuPrimPoints)||(NULL == fArrayMCTracks)) {MC_Exists = kFALSE;} else {MC_Exists = kTRUE;}
  
  // Check if we indeed have a calibration file:
  if (!fCalibration)
  {
      cout << "### FATAL: You did not provide a calibration file!\n"
           << "           You can do this by calling\n"
           << "           tracker->ReadCalibrationFile('/path/to/file.txt');\n"
           << "           before you add tracker as a task in your macro.\n\n\n";
           return kFATAL;
  }

  // Register the output array:
  ioman->Register("NeuLANDVertex","Vertices in NeuLAND",fArrayVertices,kTRUE);

  // Pass parameter values to global variables:
  fTarget_Xpos_global = fTarget_Xpos;
  fTarget_Ypos_global = fTarget_Ypos;
  fTarget_Zpos_global = fTarget_Zpos;
  fBeam_Xpos_global = fBeam_Xpos;
  fBeam_Ypos_global = fBeam_Ypos;
  fBeam_Zpos_global = fBeam_Zpos;
  fBeamBeta_global = fBeamBeta;
  
  // Set the event Counter to zero:
  fEventCounter = 0;
  
  // Create our histograms:
  CreateHistograms();
  
  // Done.
  cout << "\n\n### NOTE: fEliminationLevel = " << fEliminationLevel << "\n\n";
  return kSUCCESS;
}

// NOTE: FairTask Execute function:
void R3BNeuLANDTracker::Exec(Option_t* opt)
{
  // At first we begin with resetting everything:
  Reset();
  
  // Then we remove the clusters from the charged particles:
  if ((VETO_Exists)&&(fEliminationLevel>=0)) {Filter();}
  
  // Then we apply the cuts of the calibrationfile:
  if (fApplyCuts) {ApplyCuts();} else {fNeutronNumber = fNeutronPredefined;}
  
  // Then we perform the reconstruction:
  if (fNeutronNumber>0) {Reconstruct();}
  h_Nvertices->Fill(fNVertices);
  
  // Then we compare the the Monte Carlo Primary Interaction points:
  if (MC_Exists) {Compare();}
  
  // Finally, update the event counter:
  fEventCounter = fEventCounter + 1;
  
  // Give a sign of life:
  if ((fEventCounter%100)==0) {cout << "R3BNeuLANDTracker: We reconstructed " << fEventCounter << " events with option" << opt << "\n";}

  // Done!
}

// NOTE: FairTask Finish function:
void R3BNeuLANDTracker::Finish()
{
    h_ncl->Write("NClusters",2);
    h_ncl_elim->Write("NClusters_AfterElimination",2);
    h_elimclusters->Write("NElimClusters",2);
    h_edeptot_elim->Write("EdepTot_elim",2);
    h_Nvertices->Write("Nvertices",2);
}

// Function to create our histograms:
void R3BNeuLANDTracker::CreateHistograms()
{
    h_ncl = new TH1I("NClusters","Total number of clusters per event",100,0,99);
    h_ncl_elim = new TH1I("NClusters_AfterElimination","UnEliminated number of clusters per event",100,0,99);
    h_elimclusters = new TH1I("NElimClusters","Eliminated number of clusters per event",100,0,99);
    h_edeptot_elim = new TH1D("EdepTot_elim","Total Energy deposition per event after elimination",1500,0.0,1500.0);
    h_Nvertices = new TH1I("Nvertices","Number of vertices per event",10,0,9);
}
