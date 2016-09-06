// -----------------------------------------------------------------------------
// -----                   R3BNeuLandClusterFinder                         -----
// -----     Created 2fneuland_ClusterTimeRadius2-2012 by D.Kresan         -----
// -----         The original algorithm is written by M.Heil               -----
// -----               Merging is added by C. A. Douma                     -----
// -----------------------------------------------------------------------------

#ifndef R3BNEULANDCLUSTERFINDER_H
#define R3BNEULANDCLUSTERFINDER_H

#include <vector>

#include "FairTask.h"

class TClonesArray;
class TH1F;
class TH2F;


class R3BNeuLandClusterFinder : public FairTask {

 public:
  // Default constructor
  R3BNeuLandClusterFinder();

  // Standard constructor
  R3BNeuLandClusterFinder(Int_t verbose);

  // Destructor
  virtual ~R3BNeuLandClusterFinder();

  // Task initialization
  virtual InitStatus Init();

  // Process an event
  virtual void Exec(Option_t *option);

  // Finish of task
  virtual void Finish();

  // Reset the containers
  void Reset();
  void SetNeuLAND_NsinglePlanes(Int_t const n) {fneuland_Nplanes = n;}
  void SetSpaceRadius(Double_t const Space) {fneuland_ClusterSpaceRadius = Space;}
  void SetTimeRadius(Double_t const Time) {fneuland_ClusterTimeRadius = Time;}
  void UseSphericalForm() {fneuland_UseSphericalRadius = kTRUE;}
  void UseCubicForm() {fneuland_UseSphericalRadius = kFALSE;}
  void UseDebugging() {fneuland_DeBug = kTRUE;}
  void NoDebugging() {fneuland_DeBug = kFALSE;}

 private:
  TClonesArray              *fArrayDigi;    // Array of digis - input
  TClonesArray              *fArrayCluster; // Array of clusters - output
  std::vector<R3BLandDigi*>  fVectorDigi;   // Vector of digis (for sorting in time)

  // Control histograms
  TH1F *fhClusterSize;
  TH1F *fhClusterEnergy;
  TH1F *fhClusters;
  TH1F *fhHits;
  TH2F *fhClusterNo_vs_Size;
  Double_t fneuland_ClusterSpaceRadius;
  Double_t fneuland_ClusterTimeRadius;
  Int_t fneuland_Nplanes;
  Bool_t fneuland_UseSphericalRadius;
  Bool_t fneuland_DeBug;

  // Initialization of control histograms
  void CreateHistograms();

  ClassDef(R3BNeuLandClusterFinder, 1);
};


#endif

