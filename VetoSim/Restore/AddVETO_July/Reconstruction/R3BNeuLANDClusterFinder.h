// R3B NeuLAND ClusterFinder header-file. Written by C. A. Douma.

// Definition of the header:
#ifndef R3BNEULANDCLUSTERFINDER_H
#define R3BNEULANDCLUSTERFINDER_H

// inclusion of required C++ headers:
#include <vector>

// Inclusion of the required ROOT headers:
#include "TClonesArray.h"
#include "TH1I.h"
#include "TH1D.h"
#include "TH2D.h"

// Inclusion of the required FAIR headers:
#include "FairTask.h"

// Inclusion of the required R3B headers:
#include "R3BLandDigi.h"
#include "R3BNeuLANDCluster.h"

// The class definition:
class R3BNeuLANDClusterFinder : public FairTask {

 public:
     
  // Default constructor
  R3BNeuLANDClusterFinder();
  
  // Destructor
  virtual ~R3BNeuLANDClusterFinder();

  // FairTask Initialization function:
  virtual InitStatus Init();

  // FairTask Execute function:
  virtual void Exec(Option_t *option);

  // FairTask finishing function (after entire task):
  virtual void Finish();
  
  // Creation of the control histograms:
  void CreateHistograms();

  // Set-functions to define the parameters by the user:
  void SetSpaceRadius(Double_t const Space) {fSpaceRadius = Space;}
  void SetTimeRadius(Double_t const Time) {fTimeRadius = Time;}
  void SetTopologyBoundary(Double_t const top) {fTopBoundary = top;}
  void SetHistograms(Double_t const Emax, Int_t const Clmax) {fEmax_hist = Emax, fNClmax_hist = Clmax;}

 private:
     
  // Input/output arrays:
  TClonesArray* fArrayDigi;    
  TClonesArray* fArrayCluster;
  std::vector<R3BLandDigi*> fVectorDigi;

  // Control histograms
  TH1I* h_NClusters;
  TH1D* h_Energy;
  TH1I* h_Size;
  TH1I* h_Topology;
  TH2D* h2_Calibration;
  
  // Parameters:
  Double_t fSpaceRadius; // [cm]
  Double_t fTimeRadius; // [ns]
  Double_t fTopBoundary; // [cm]
  Double_t fEmax_hist;
  Int_t fNplanes;
  Int_t fNClmax_hist;
  

  ClassDef(R3BNeuLANDClusterFinder, 1);
};


#endif

