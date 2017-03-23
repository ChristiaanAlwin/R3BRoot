// R3B NeuLAND Vertex header-file. Written by C. A. Douma.

// Definition of the header:
#ifndef R3BNEULANDVERTEX
#define R3BNEULANDVERTEX

// inclusion of required C++ headers:
#include <iostream>

// Inclusion of the required ROOT headers:
#include "TObject.h"
#include "TMath.h"

// Inclusion of the required FAIR headers:

// Inclusion of the required R3B headers:
#include "R3BNeuLANDClustr.h"

// The class definition:
class R3BNeuLANDVertex : public TObject
{
 public:

  // Default Constructor:
  R3BNeuLANDVertex();
  
  // Constructor from a cluster:
  // NOTE: This will load only the reconstructed data. Reconstructed energy will be computed by using the global variables in the Tracker.cxx-file!
  R3BNeuLANDVertex(R3BNeuLANDClustr* cluster, Double_t const Ekin);
  
  // Destructor:
  virtual ~R3BNeuLANDVertex();

  // Declare the Get-functions:
  Double_t GetRecTime();
  Double_t GetRecX();
  Double_t GetRecY();
  Double_t GetRecZ();
  Double_t GetRecE();
  Bool_t HasMCData();
  Bool_t HasBeenCompared();
  Bool_t EventSucces();
  Double_t GetMCTime();
  Double_t GetMCX();
  Double_t GetMCY();
  Double_t GetMCZ();
  Double_t GetMCE();
  Double_t GetDeltaR();
  Double_t GetDeltaT();
  Int_t GetMCTrackID();
  Int_t GetMCPDG();

  // Declare the Set-functions:
  void SetRecPoint(Double_t const t, Double_t const x, Double_t const y, Double_t const z);
  void SetMCPoint(Double_t const t, Double_t const x, Double_t const y, Double_t const z);
  void SetRecE(Double_t const E);
  void SetMCE(Double_t const E);
  void PointHasMC();
  void PointHasNoMC();
  void PointIsCompared();
  void PointIsNotCompared();
  void EventSucceeded();
  void EventFailed();
  void SetTrackID(Int_t const track);
  void SetPDG(Int_t const pdg);
  
  // Declare other functions:

 private:
     
  // Declare the content of this class:
  Double_t fReconstructed_Time;
  Double_t fReconstructed_X;
  Double_t fReconstructed_Y;
  Double_t fReconstructed_Z;
  Double_t fReconstructed_E;
  Bool_t fHasMCpoint;
  Bool_t fHasBeenCompared;
  Bool_t fEventSucces;
  Double_t fMC_Time;
  Double_t fMC_X;
  Double_t fMC_Y;
  Double_t fMC_Z;
  Double_t fMC_E;
  Double_t fDeltaR;
  Double_t fDeltaT;
  Int_t fMC_TrackID;
  Int_t fMC_PDG;
  
  // End the class definition:
 public:
  ClassDef(R3BNeuLANDVertex, 1)
};


#endif


