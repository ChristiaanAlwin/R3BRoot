// R3B NeuLAND Cluster header-file. Written by C. A. Douma.

// Definition of the header:
#ifndef R3BNEULANDCLUSTR
#define R3BNEULANDCLUSTR

// inclusion of required C++ headers:
#include <iostream>

// Inclusion of the required ROOT headers:
#include "TObject.h"
#include "TVector3.h"
#include "TMath.h"
#include "TClonesArray.h"

// Inclusion of the required FAIR headers:

// Inclusion of the required R3B headers:
#include "R3BNeulandDigi.h"

// The class definition:
class R3BNeuLANDClustr : public TObject
{
 public:

  // Default Constructor:
  R3BNeuLANDClustr();
  
  // Constructor from 1 digi:
  R3BNeuLANDClustr(R3BNeulandDigi* digi);

  // Constructor from 2 digis:
  R3BNeuLANDClustr(R3BNeulandDigi* digi1, R3BNeulandDigi* digi2);
  
  // Destructor:
  virtual ~R3BNeuLANDClustr();

  // Declare the Get-functions:
  Double_t GetStartT();
  Double_t GetStartX();
  Double_t GetStartY();
  Double_t GetStartZ(); 
  Double_t GetStopT(); 
  Double_t GetStopX(); 
  Double_t GetStopY(); 
  Double_t GetStopZ(); 
  Double_t GetEdgeT(); 
  Double_t GetEdgeX(); 
  Double_t GetEdgeY(); 
  Double_t GetEdgeZ(); 
  Double_t GetE();
  Int_t GetSize();
  Int_t GetTopology();
  Int_t GetNbars();
  Int_t GetBar(Int_t const index);
  Bool_t IsMarked();
  Bool_t IsEliminated();
  Bool_t IsElMarked();
  void StartPosition(TVector3 &pos);
  void StopPosition(TVector3 &pos);
  void EdgePosition(TVector3 &pos);

  // Declare the Set-functions:
  void Mark();
  void UnMark();
  void Eliminate();
  void Allow();
  void MarkEl();
  void UnMarkEl();
  
  // Declare other functions:
  void Print();
  void AddDigi(R3BNeulandDigi* digi);
  void Merge(R3BNeuLANDClustr* Second, TClonesArray* fDigis);
  void Replace(R3BNeuLANDClustr* Second);
  Bool_t TopologyIsLine(TClonesArray* fDigis, Double_t const TopBoundary);
  Bool_t TopologyIsKink(TClonesArray* fDigis, Double_t const TopBoundary);
  Bool_t TopologyIsPitchFork(TClonesArray* fDigis, Double_t const SpaceRadius, Double_t const TimeRadius);
  Bool_t TopologyIsCross(TClonesArray* fDigis, Double_t const SpaceRadius, Double_t const TimeRadius);
  void IdentifyTopology(TClonesArray* fDigis, Double_t const SpaceRadius, Double_t const TimeRadius, Double_t const TopBoundary);
  
  // NOTE: After the clusterfinding has been done, the user should simply call the
  // IdentifyTopology function once. fTopology will then be set to one of the following values:
  // 0 ==> This function has not been called yet.
  // 1 ==> The cluster has the form of a straight line.
  // 2 ==> The cluster has the form of a line with a single kink. fEdge holds the kink-coordinates after this function is called.
  // 3 ==> The cluster has the form of a picthfork (one line going to 2). fEdge has the junction coordinates.
  // 4 ==> The cluster has the form of a cross (two intersecting lines). fEdge holds the intersection.
  // 5 ==> The cluster has none of the above shapes.
  // 6 ==> Cluster consists only of a single digi.
  // 7 ==> Clutsre consists of zero digis.
  // The SpaceRadius and TimeRadius should be the Radii of the clusterfinder and TopBoundary is the
  // averagde distance [cm] that a signle digi is allowed to have from the line(s) to still let the test succeed.

 private:
     
  // Declare the content of this class:
  Double_t fStartX;
  Double_t fStartY;
  Double_t fStartZ;
  Double_t fStartT;
  Double_t fStopX;
  Double_t fStopY;
  Double_t fStopZ;
  Double_t fStopT;
  Double_t fEdgeT;
  Double_t fEdgeX;
  Double_t fEdgeY;
  Double_t fEdgeZ;
  Double_t fE;
  Int_t fSize;
  Int_t fTopology;
  Bool_t fMarked;
  Bool_t fEliminated;
  Bool_t fElMarker;
  std::vector<Int_t> fDigiBars;

  // End the class definition:
 public:
  ClassDef(R3BNeuLANDClustr, 1)
};


#endif


