// R3B NeuLAND Tracker header-file. Written by C. A. Douma.

// Definition of the header:
#ifndef R3BNEULANDTRACKER_H
#define R3BNEULANDTRACKER_H

// inclusion of the required C++ headers:
#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>

//inclusion of the required ROOT-headers:
#include "TH1I.h"
#include "TH1D.h"
#include "TMath.h"
#include "TVector3.h"
#include "TClonesArray.h"

// inclusion of the required FAIR headers:
#include "FairTask.h"
#include "FairRootManager.h"
#include "FairMCPoint.h" // for Neuland primpoints.

// Inclusion of the required R3B headers:
#include "R3BLandDigi.h"
#include "R3BNeuLANDCluster.h"
#include "R3Bveto_segDigi.h"
#include "R3BNeuLANDVertex.h"
#include "R3BMCTrack.h"

// The class definition:
class R3BNeuLANDTracker : public FairTask
{
 public:
  // Default constructor:
  R3BNeuLANDTracker();
  
  // Default Destructor:
  ~R3BNeuLANDTracker();

  // FairTask Initialization function:
  virtual InitStatus Init();
    
  // FairTask Execute function:
  virtual void Exec(Option_t* opt);

  // FairTask finishing function (after entire task):
  virtual void Finish();
  
  // Function to create histograms:
  void CreateHistograms();
  
  // Function to read the calibration file:
  void ReadCalibrationFile(TString const FileName);

  // Inline functions to set all the parameters for the reconstruction:
  void SetBeamBeta(Double_t const beta) {fBeamBeta = beta;}
  void SetBeamEnergy(Double_t const E) {fBeamEnergy = E;}
  void SetBeamStartingPosition(Double_t const x, Double_t const y, Double_t const z) {fBeam_Xpos = x; fBeam_Ypos = y; fBeam_Zpos = z;}
  void SetTargetPosition(Double_t const x, Double_t const y, Double_t const z) {fTarget_Xpos = x; fTarget_Ypos = y; fTarget_Zpos = z;}
  void SetNSinglePlanes(Int_t const n) {Nplanes = n;}
  void SetNeutronNumber(Int_t const n) {fCalibration = kTRUE; fApplyCuts = kFALSE; fNeutronPredefined = n;}
  void SetEliminationLevel(Int_t const n) {fEliminationLevel = n;}
  
  // Task functions:
  Bool_t IsElastic(R3BNeuLANDCluster* c1, R3BNeuLANDCluster* c2);
  void ApplyCuts();
  void Reset();
  void Filter();
  void Reconstruct();
  void Compare();

 private:
     
  // Event Counter:
  Int_t fEventCounter;
  
  // input/output TClonesArrays:
  TClonesArray* fArrayDigis;
  TClonesArray* fArrayVetoDigis;
  TClonesArray* fArrayClusters;
  TClonesArray* fArrayNeuPrimPoints;
  TClonesArray* fArrayMCTracks;
  TClonesArray* fArrayVertices;
  std::vector<R3BNeuLANDCluster*> fVectorClusters;
  
  // Parameters for the reconstruction:
  Double_t fBeamBeta;
  Double_t fBeamEnergy;
  Double_t fTarget_Xpos;
  Double_t fTarget_Ypos;
  Double_t fTarget_Zpos;
  Double_t fBeam_Xpos;
  Double_t fBeam_Ypos;
  Double_t fBeam_Zpos;
  Double_t fKappa;
  Double_t* fCuts;
  Bool_t VETO_Exists;
  Bool_t MC_Exists;
  Bool_t fCalibration;
  Bool_t fApplyCuts;
  Int_t Nplanes;
  Int_t Npaddles;
  Int_t fNumberOfCuts;
  Int_t fNeutronPredefined;
  Int_t fEliminationLevel;
  
  // Variables used during reconstruction:
  Int_t fNeutronNumber;
  Int_t fNClustersSorted;
  Int_t fNclustersNotEliminated;
  Int_t fNVertices;
  Double_t fEdepTotalNotEliminated;
  
  // Control histograms:
  TH1I* h_ncl;
  TH1I* h_ncl_elim;
  TH1I* h_elimclusters;
  TH1D* h_edeptot_elim;
  TH1I* h_Nvertices;
  
 public: 
  ClassDef(R3BNeuLANDTracker,1)  
};


#endif
