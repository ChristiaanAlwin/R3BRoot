// R3B NeuLAND Cluster cxx-file. Written by C. A. Douma.

// inclusion of required C++ headers:

// Inclusion of the required ROOT headers:

// Inclusion of the required FAIR headers:

// Inclusion of the required R3B headers:

// Inclusion of the own header file:
#include "R3BNeuLANDVertex.h"

// Using standard namespaces:
using namespace std;

// Define the class:
ClassImp(R3BNeuLANDVertex)

// Default Constructor:
R3BNeuLANDVertex::R3BNeuLANDVertex()
{
  fReconstructed_Time = 0.0;
  fReconstructed_X = 0.0;
  fReconstructed_Y = 0.0;
  fReconstructed_Z = 0.0;
  fReconstructed_E = 0.0;
  fHasMCpoint = kFALSE;
  fHasBeenCompared = kFALSE;
  fEventSucces = kFALSE;
  fMC_Time = 0.0;
  fMC_X = 0.0;
  fMC_Y = 0.0;
  fMC_Z = 0.0;
  fMC_E = 0.0;
  fDeltaR = -10.0;
  fDeltaT = -10.0;
  fMC_TrackID = 0;
  fMC_PDG = 0;
}

// Constructor from a cluster:
R3BNeuLANDVertex::R3BNeuLANDVertex(R3BNeuLANDCluster* cluster, Double_t const Ekin)
{
  // create the vertex:
  fReconstructed_Time = cluster->GetStartT();
  fReconstructed_X = cluster->GetStartX();
  fReconstructed_Y = cluster->GetStartY();
  fReconstructed_Z = cluster->GetStartZ();
  
  // Set the rest to zero; that part comes later:
  fHasMCpoint = kFALSE;
  fHasBeenCompared = kFALSE;
  fEventSucces = kFALSE;
  fMC_Time = 0.0;
  fMC_X = 0.0;
  fMC_Y = 0.0;
  fMC_Z = 0.0;
  fMC_E = 0.0;
  fDeltaR = -10.0;
  fDeltaT = -10.0;
  fMC_TrackID = 0;
  fMC_PDG = 0;
  
  // Set the reconstructed energy: 
  fReconstructed_E = Ekin;
  
  // Done!
}

// Destructor:
R3BNeuLANDVertex::~R3BNeuLANDVertex()
{
  fReconstructed_Time = 0.0;
  fReconstructed_X = 0.0;
  fReconstructed_Y = 0.0;
  fReconstructed_Z = 0.0;
  fReconstructed_E = 0.0;
  fHasMCpoint = kFALSE;
  fHasBeenCompared = kFALSE;
  fEventSucces = kFALSE;
  fMC_Time = 0.0;
  fMC_X = 0.0;
  fMC_Y = 0.0;
  fMC_Z = 0.0;
  fMC_E = 0.0;
  fDeltaR = 0.0;
  fDeltaT = 0.0;
  fMC_TrackID = 0;
  fMC_PDG = 0;
}

// Define the Get-functions:
Double_t R3BNeuLANDVertex::GetRecTime() {return fReconstructed_Time;}
Double_t R3BNeuLANDVertex::GetRecX() {return fReconstructed_X;}
Double_t R3BNeuLANDVertex::GetRecY() {return fReconstructed_Y;}
Double_t R3BNeuLANDVertex::GetRecZ() {return fReconstructed_Z;}
Double_t R3BNeuLANDVertex::GetRecE() {return fReconstructed_E;}
Bool_t R3BNeuLANDVertex::HasMCData() {return fHasMCpoint;}
Bool_t R3BNeuLANDVertex::HasBeenCompared() {return fHasBeenCompared;}
Bool_t R3BNeuLANDVertex::EventSucces() {return fEventSucces;}
Double_t R3BNeuLANDVertex::GetMCTime() {return fMC_Time;}
Double_t R3BNeuLANDVertex::GetMCX() {return fMC_X;}
Double_t R3BNeuLANDVertex::GetMCY() {return fMC_Y;}
Double_t R3BNeuLANDVertex::GetMCZ() {return fMC_Z;}
Double_t R3BNeuLANDVertex::GetMCE() {return fMC_E;}
Double_t R3BNeuLANDVertex::GetDeltaR() {return fDeltaR;}
Double_t R3BNeuLANDVertex::GetDeltaT() {return fDeltaT;}
Int_t R3BNeuLANDVertex::GetMCTrackID() {return fMC_TrackID;}
Int_t R3BNeuLANDVertex::GetMCPDG() {return fMC_PDG;}

// Define the Set-functions:
void R3BNeuLANDVertex::SetRecPoint(Double_t const t, Double_t const x, Double_t const y, Double_t const z) {fReconstructed_Time = t; fReconstructed_X = x; fReconstructed_Y = y; fReconstructed_Z = z;}
void R3BNeuLANDVertex::SetRecE(Double_t const E) {fReconstructed_E = E;}
void R3BNeuLANDVertex::SetMCE(Double_t const E) {fMC_E = E;}
void R3BNeuLANDVertex::PointHasMC() {fHasMCpoint = kTRUE;}
void R3BNeuLANDVertex::PointHasNoMC() {fHasMCpoint = kFALSE;}
void R3BNeuLANDVertex::PointIsCompared() {fHasBeenCompared = kTRUE;}
void R3BNeuLANDVertex::PointIsNotCompared() {fHasBeenCompared = kFALSE;}
void R3BNeuLANDVertex::EventSucceeded() {fEventSucces = kTRUE;}
void R3BNeuLANDVertex::EventFailed() {fEventSucces = kFALSE;}
void R3BNeuLANDVertex::SetTrackID(Int_t const track) {fMC_TrackID = track;}
void R3BNeuLANDVertex::SetPDG(Int_t const pdg) {fMC_PDG = pdg;}

void R3BNeuLANDVertex::SetMCPoint(Double_t const t, Double_t const x, Double_t const y, Double_t const z) 
{
    fMC_Time = t; 
    fMC_X = x; 
    fMC_Y = y; 
    fMC_Z = z;
    fDeltaR = TMath::Sqrt((fMC_X-fReconstructed_X)*(fMC_X-fReconstructed_X) + (fMC_Y-fReconstructed_Y)*(fMC_Y-fReconstructed_Y) + (fMC_Z-fReconstructed_Z)*(fMC_Z-fReconstructed_Z));
    fDeltaT = TMath::Abs(fMC_Time - fReconstructed_Time);
}
