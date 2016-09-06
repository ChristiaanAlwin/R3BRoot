// -----------------------------------------------------------------------------
// -----                    R3BNVeto_segClusterFinder                        -----
// -----                Created 21-02-2012 by D.Kresan                     -----
// -----------------------------------------------------------------------------

#ifndef R3BNVETO_SEGCLUSTERFINDER_H
#define R3BNVETO_SEGCLUSTERFINDER_H

#include <vector>

#include "FairTask.h"

class TClonesArray;
class TH1F;
class TH2F;


class R3BNVeto_segClusterFinder : public FairTask {

 public:
  // Default constructor
  R3BNVeto_segClusterFinder();

  // Standard constructor
  R3BNVeto_segClusterFinder(Int_t verbose);

  // Destructor
  virtual ~R3BNVeto_segClusterFinder();

  // Task initialization
  virtual InitStatus Init();

  // Process an event
  virtual void Exec(Option_t *option);

  // Finish of task
  virtual void Finish();

  // Reset the containers
  void Reset();
  void SetSpaceRadius(Double_t const Space) {fNveto_seg_ClusterSpaceRadius = Space;}
  void SetTimeRadius(Double_t const Time) {fNveto_seg_ClusterTimeRadius = Time;}
  void UseSphericalForm() {fNveto_seg_UseSphericalRadius = kTRUE;}
  void UseCubicForm() {fNveto_seg_UseSphericalRadius = kFALSE;}
  void UseDebugging() {fNveto_seg_DeBug = kTRUE;}
  void NoDebugging() {fNveto_seg_DeBug = kFALSE;}

 private:
  TClonesArray              *fArrayDigi;    // Array of digis - input
  TClonesArray              *fArrayCluster; // Array of clusters - output
  std::vector<R3Bveto_segDigi*>  fVectorDigi;   // Vector of digis (for sorting in time)

  // Control histograms
  TH1F *fhClusterSize;
  TH1F *fhClusterEnergy;
  TH1F *fhClusters;
  TH1F *fhHits;
  TH2F *fhClusterNo_vs_Size;
  Double_t fNveto_seg_ClusterSpaceRadius;
  Double_t fNveto_seg_ClusterTimeRadius;
  Int_t fNveto_seg_Nplanes;
  Bool_t fNveto_seg_UseSphericalRadius;
  Bool_t fNveto_seg_DeBug;

  // Initialization of control histograms
  void CreateHistograms();

  ClassDef(R3BNVeto_segClusterFinder, 1);
};


#endif

