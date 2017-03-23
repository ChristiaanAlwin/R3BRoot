// This hearder file contains the definition of a structure
// containing all the leafs of the Monte Carlo tree.

// Wriiten by C. A. Douma:

// Inclusion of required hearders:
#include "TTree.h"
#include "TLeaf.h"

// Definition of the structure:
struct Leaf_Structure {
  
   // Geant3 or Geant4:
   Bool_t Geant3;
  
   // The Monte Carlo tree:
   TTree* TheTree;
   
   // NeuLAND Digitizer tree:
   TTree* NeuDigiTree;
   
   // VETO Digitizer tree:
   TTree* VetoDigiTree;
   
   // NeuLAND TrackTree:
   TTree* TrackTree;
   
   // Other digitizers:
   TTree* OtherTree;
   
   // STaRTrack reconstructed tracks:
   TTree* StarTree;
  
   // MC leafs:
   TLeaf* MCTrack_fUniqueID;				// UInt_t[n]
   TLeaf* MCTrack_fBits;				// UInt_t[n]
   TLeaf* MCTrack_fPdgCode;				// Int_t[n]
   TLeaf* MCTrack_fMotherId;				// Int_t[n]
   TLeaf* MCTrack_fPx;					// Double32_t[n]
   TLeaf* MCTrack_fPy;					// Double32_t[n]
   TLeaf* MCTrack_fPz;					// Double32_t[n]
   TLeaf* MCTrack_fStartX;				// Double32_t[n]
   TLeaf* MCTrack_fStartY;				// Double32_t[n]
   TLeaf* MCTrack_fStartZ;				// Double32_t[n]
   TLeaf* MCTrack_fStartT;				// Double32_t[n]
   TLeaf* MCTrack_fNPoints;				// Int_t[n]
   TLeaf* MCTrack_fMass;				// Double32_t[n]
   
   // CALIFA leafs:
   TLeaf* CaloCrystalHitSim_fUniqueID;			// UInt_t[n]
   TLeaf* CaloCrystalHitSim_fBits;			// UInt_t[n]
   TLeaf* CaloCrystalHitSim_fLinks;			// set<FairLink>[n]
   TLeaf* CaloCrystalHitSim_fEntryNr_fUniqueID;		// UInt_t[n]
   TLeaf* CaloCrystalHitSim_fEntryNr_fBits;		// UInt_t[n]
   TLeaf* CaloCrystalHitSim_fEntryNr_fFile;		// Int_t[n]
   TLeaf* CaloCrystalHitSim_fEntryNr_fEntry;		// Int_t[n]
   TLeaf* CaloCrystalHitSim_fEntryNr_fType;		// Int_t[n]
   TLeaf* CaloCrystalHitSim_fEntryNr_fIndex;		// Int_t[n]
   TLeaf* CaloCrystalHitSim_fEntryNr_fWeight;		// Float_t[n]
   TLeaf* CaloCrystalHitSim_fDefaultType;		// Int_t[n]
   TLeaf* CaloCrystalHitSim_fEnergy;			// Double32_t[n]
   TLeaf* CaloCrystalHitSim_fNf;			// Double32_t[n]
   TLeaf* CaloCrystalHitSim_fNs;			// Double32_t[n]
   TLeaf* CaloCrystalHitSim_fToT_Energy;		// Double32_t[n]
   TLeaf* CaloCrystalHitSim_fTime;			// ULong64_t[n]
   TLeaf* CaloCrystalHitSim_fCrystalId;			// Int_t[n]
   TLeaf* CaloCrystalHitSim_fCrystalType;		// Int_t[n]
   TLeaf* CaloCrystalHitSim_fCrystalCopy;		// Int_t[n]
   TLeaf* CaloCrystalHitSim_fNSteps;			// Int_t[n]
   TLeaf* CaloCrystalHitSim_fEinc;			// Double32_t[n]
   TLeaf* CaloCrystalHitSim_fTrackID;			// Int_t[n]
   TLeaf* CaloCrystalHitSim_fVolumeID;			// Int_t[n]
   TLeaf* CaloCrystalHitSim_fParentTrackID;		// Int_t[n]
   TLeaf* CaloCrystalHitSim_fTrackPID;			// Int_t[n]
   TLeaf* CaloCrystalHitSim_fTrackUniqueID;		// Int_t[n]
   
   // Crystal Ball Leafs:
   TLeaf* XBCrystalPoint_fUniqueID;                     // UInt_t[n]
   TLeaf* XBCrystalPoint_fBits;                         // UInt_t[n]
   TLeaf* XBCrystalPoint_fLink;                         // FairMultiLinkedData*[n]
   TLeaf* XBCrystalPoint_fTrackID;                      // Int_t[n]
   TLeaf* XBCrystalPoint_fEventId;                      // UInt_t[n]
   TLeaf* XBCrystalPoint_fPx;                           // Double32_t[n]
   TLeaf* XBCrystalPoint_fPy;                           // Double32_t[n]
   TLeaf* XBCrystalPoint_fPz;                           // Double32_t[n]
   TLeaf* XBCrystalPoint_fTime;                         // Double32_t[n]
   TLeaf* XBCrystalPoint_fLength;                       // Double32_t[n]
   TLeaf* XBCrystalPoint_fELoss;                        // Double32_t[n]
   TLeaf* XBCrystalPoint_fDetectorID;                   // Int_t[n]
   TLeaf* XBCrystalPoint_fX;                            // Double32_t[n]
   TLeaf* XBCrystalPoint_fY;                            // Double32_t[n]
   TLeaf* XBCrystalPoint_fZ;                            // Double32_t[n]
   TLeaf* XBCrystalPoint_fX_out;                        // Double32_t[n]
   TLeaf* XBCrystalPoint_fY_out;                        // Double32_t[n]
   TLeaf* XBCrystalPoint_fZ_out;                        // Double32_t[n]
   TLeaf* XBCrystalPoint_fPx_out;                       // Double32_t[n]
   TLeaf* XBCrystalPoint_fPy_out;                       // Double32_t[n]
   TLeaf* XBCrystalPoint_fPz_out;                       // Double32_t[n]
   TLeaf* XBCrystalPoint_fCrystalType;                  // Int_t[n]
   TLeaf* XBCrystalPoint_fCrystalNb;                    // Int_t[n]
   
   // GFI Leafs:
   TLeaf* GFIPoint_fUniqueID;                           // UInt_t[n]
   TLeaf* GFIPoint_fBits;                               // UInt_t[n]
   TLeaf* GFIPoint_fLink;                               // FairMultiLinkedData*[n]
   TLeaf* GFIPoint_fTrackID;                            // Int_t[n]
   TLeaf* GFIPoint_fEventId;                            // UInt_t[n]
   TLeaf* GFIPoint_fPx;                                 // Double32_t[n]
   TLeaf* GFIPoint_fPy;                                 // Double32_t[n]
   TLeaf* GFIPoint_fPz;                                 // Double32_t[n]
   TLeaf* GFIPoint_fTime;                               // Double32_t[n]
   TLeaf* GFIPoint_fLength;                             // Double32_t[n]
   TLeaf* GFIPoint_fELoss;                              // Double32_t[n]
   TLeaf* GFIPoint_fDetectorID;                         // Int_t[n]
   TLeaf* GFIPoint_fX;                                  // Double32_t[n]
   TLeaf* GFIPoint_fY;                                  // Double32_t[n]
   TLeaf* GFIPoint_fZ;                                  // Double32_t[n]
   TLeaf* GFIPoint_fX_out;                              // Double32_t[n]
   TLeaf* GFIPoint_fY_out;                              // Double32_t[n]
   TLeaf* GFIPoint_fZ_out;                              // Double32_t[n]
   TLeaf* GFIPoint_fPx_out;                             // Double32_t[n]
   TLeaf* GFIPoint_fPy_out;                             // Double32_t[n]
   TLeaf* GFIPoint_fPz_out;                             // Double32_t[n]
   TLeaf* GFIPoint_fModule;                             // Int_t[n]
   
   // STaRTrack leafs:
   TLeaf* STaRTraPoint_fUniqueID;			// UInt_t[n]
   TLeaf* STaRTraPoint_fBits;				// UInt_t[n]
   TLeaf* STaRTraPoint_fLink;				// FairMultiLinkedData*[n]
   TLeaf* STaRTraPoint_fTrackID;			// Int_t[n]
   TLeaf* STaRTraPoint_fEventId;			// UInt_t[n]
   TLeaf* STaRTraPoint_fPx;				// Double32_t[n]
   TLeaf* STaRTraPoint_fPy;				// Double32_t[n]
   TLeaf* STaRTraPoint_fPz;				// Double32_t[n]
   TLeaf* STaRTraPoint_fTime;				// Double32_t[n]
   TLeaf* STaRTraPoint_fLength;				// Double32_t[n]
   TLeaf* STaRTraPoint_fELoss;				// Double32_t[n]
   TLeaf* STaRTraPoint_fDetectorID;			// Int_t[n]
   TLeaf* STaRTraPoint_fX;				// Double32_t[n]
   TLeaf* STaRTraPoint_fY;				// Double32_t[n]
   TLeaf* STaRTraPoint_fZ;				// Double32_t[n]
   TLeaf* STaRTraPoint_fX_out;				// Double32_t[n]
   TLeaf* STaRTraPoint_fY_out;				// Double32_t[n]
   TLeaf* STaRTraPoint_fZ_out;				// Double32_t[n]
   TLeaf* STaRTraPoint_fPx_out;				// Double32_t[n]
   TLeaf* STaRTraPoint_fPy_out;				// Double32_t[n]
   TLeaf* STaRTraPoint_fPz_out;				// Double32_t[n]
   TLeaf* STaRTraPoint_fDetCopyID;			// Int_t[n]
   
   // DCH leafs:
   TLeaf* DCHPoint_fUniqueID;				// UInt_t[n]
   TLeaf* DCHPoint_fBits;				// UInt_t[n]
   TLeaf* DCHPoint_fLink;				// FairMultiLinkedData*[n]
   TLeaf* DCHPoint_fTrackID;				// Int_t[n]
   TLeaf* DCHPoint_fEventId;				// UInt_t[n]
   TLeaf* DCHPoint_fPx;					// Double32_t[n]
   TLeaf* DCHPoint_fPy;					// Double32_t[n]
   TLeaf* DCHPoint_fPz;					// Double32_t[n]
   TLeaf* DCHPoint_fTime;				// Double32_t[n]
   TLeaf* DCHPoint_fLength;				// Double32_t[n]
   TLeaf* DCHPoint_fELoss;				// Double32_t[n]
   TLeaf* DCHPoint_fDetectorID;				// Int_t[n]
   TLeaf* DCHPoint_fX;					// Double32_t[n]
   TLeaf* DCHPoint_fY;					// Double32_t[n]
   TLeaf* DCHPoint_fZ;					// Double32_t[n]
   TLeaf* DCHPoint_fX_out;				// Double32_t[n]
   TLeaf* DCHPoint_fY_out;				// Double32_t[n]
   TLeaf* DCHPoint_fZ_out;				// Double32_t[n]
   TLeaf* DCHPoint_fPx_out;				// Double32_t[n]
   TLeaf* DCHPoint_fPy_out;				// Double32_t[n]	
   TLeaf* DCHPoint_fPz_out;				// Double32_t[n]
   TLeaf* DCHPoint_fLocalX_in;				// Double32_t[n]
   TLeaf* DCHPoint_fLocalY_in;				// Double32_t[n]
   TLeaf* DCHPoint_fLocalZ_in;				// Double32_t[n]
   TLeaf* DCHPoint_fLocalPx_in;				// Double32_t[n]
   TLeaf* DCHPoint_fLocalPy_in;				// Double32_t[n]
   TLeaf* DCHPoint_fLocalPz_in;				// Double32_t[n]
   TLeaf* DCHPoint_fLocalX_out;				// Double32_t[n]
   TLeaf* DCHPoint_fLocalY_out;				// Double32_t[n]
   TLeaf* DCHPoint_fLocalZ_out;				// Double32_t[n]
   TLeaf* DCHPoint_fLocalPx_out;			// Double32_t[n]
   TLeaf* DCHPoint_fLocalPy_out;			// Double32_t[n]
   TLeaf* DCHPoint_fLocalPz_out;			// Double32_t[n]
   TLeaf* DCHPoint_fModule;				// Int_t[n]
   TLeaf* DCHPoint_fLayer;				// Int_t[n]
   TLeaf* DCHPoint_fCell;				// Int_t[n]
   
   // TOF wall leafs:
   TLeaf* TOFPoint_fUniqueID;				// UInt_t[n]
   TLeaf* TOFPoint_fBits;				// UInt_t[n]
   TLeaf* TOFPoint_fLink;				// FairMultiLinkedData*[n]
   TLeaf* TOFPoint_fTrackID;				// Int_t[n]
   TLeaf* TOFPoint_fEventId;				// UInt_t[n]
   TLeaf* TOFPoint_fPx;					// Double32_t[n]
   TLeaf* TOFPoint_fPy;					// Double32_t[n]
   TLeaf* TOFPoint_fPz;					// Double32_t[n]
   TLeaf* TOFPoint_fTime;				// Double32_t[n]
   TLeaf* TOFPoint_fLength;				// Double32_t[n]
   TLeaf* TOFPoint_fELoss;				// Double32_t[n]
   TLeaf* TOFPoint_fDetectorID;				// Int_t[n]
   TLeaf* TOFPoint_fX;					// Double32_t[n]
   TLeaf* TOFPoint_fY;					// Double32_t[n]
   TLeaf* TOFPoint_fZ;					// Double32_t[n]
   TLeaf* TOFPoint_fX_out;				// Double32_t[n]
   TLeaf* TOFPoint_fY_out;				// Double32_t[n]
   TLeaf* TOFPoint_fZ_out;				// Double32_t[n]
   TLeaf* TOFPoint_fPx_out;				// Double32_t[n]
   TLeaf* TOFPoint_fPy_out;				// Double32_t[n]
   TLeaf* TOFPoint_fPz_out;				// Double32_t[n]
   
   // mTOF leafs:
   TLeaf* mTOFPoint_fUniqueID;				// UInt_t[n]
   TLeaf* mTOFPoint_fBits;				// UInt_t[n]
   TLeaf* mTOFPoint_fLink;				// FairMultiLinkedData*[n]
   TLeaf* mTOFPoint_fTrackID;				// Int_t[n]
   TLeaf* mTOFPoint_fEventId;				// UInt_t[n]
   TLeaf* mTOFPoint_fPx;				// Double32_t[n]
   TLeaf* mTOFPoint_fPy;				// Double32_t[n]
   TLeaf* mTOFPoint_fPz;				// Double32_t[n]
   TLeaf* mTOFPoint_fTime;				// Double32_t[n]
   TLeaf* mTOFPoint_fLength;				// Double32_t[n]
   TLeaf* mTOFPoint_fELoss;				// Double32_t[n]
   TLeaf* mTOFPoint_fDetectorID;			// Int_t[n]
   TLeaf* mTOFPoint_fX;					// Double32_t[n]
   TLeaf* mTOFPoint_fY;					// Double32_t[n]
   TLeaf* mTOFPoint_fZ;					// Double32_t[n]
   TLeaf* mTOFPoint_fX_out;				// Double32_t[n]
   TLeaf* mTOFPoint_fY_out;				// Double32_t[n]
   TLeaf* mTOFPoint_fZ_out;				// Double32_t[n]
   TLeaf* mTOFPoint_fPx_out;				// Double32_t[n]
   TLeaf* mTOFPoint_fPy_out;				// Double32_t[n]
   TLeaf* mTOFPoint_fPz_out;				// Double32_t[n]
   
   // Land-class: LandPoint leafs:
   TLeaf* LandPoint_fUniqueID;				// UInt_t[n]
   TLeaf* LandPoint_fBits;				// UInt_t[n]
   TLeaf* LandPoint_fLink;				// FairMultiLinkedData*[n]
   TLeaf* LandPoint_fTrackID;				// Int_t[n]
   TLeaf* LandPoint_fEventId;				// UInt_t[n]
   TLeaf* LandPoint_fPx;				// Double32_t[n]
   TLeaf* LandPoint_fPy;				// Double32_t[n]
   TLeaf* LandPoint_fPz;				// Double32_t[n]
   TLeaf* LandPoint_fTime;				// Double32_t[n]
   TLeaf* LandPoint_fLength;				// Double32_t[n]
   TLeaf* LandPoint_fELoss;				// Double32_t[n]
   TLeaf* LandPoint_fDetectorID;			// Int_t[n]
   TLeaf* LandPoint_fX;					// Double32_t[n]
   TLeaf* LandPoint_fY;					// Double32_t[n]
   TLeaf* LandPoint_fZ;					// Double32_t[n]
   TLeaf* LandPoint_fX_out;				// Double32_t[n]
   TLeaf* LandPoint_fY_out;				// Double32_t[n]
   TLeaf* LandPoint_fZ_out;				// Double32_t[n]
   TLeaf* LandPoint_fPx_out;				// Double32_t[n]
   TLeaf* LandPoint_fPy_out;				// Double32_t[n]
   TLeaf* LandPoint_fPz_out;				// Double32_t[n]
   TLeaf* LandPoint_fSector;				// Int_t[n]
   TLeaf* LandPoint_fPaddleNb;				// Int_t[n]
   TLeaf* LandPoint_fPaddleTyp;				// Int_t[n]
   TLeaf* LandPoint_fLightYield;			// Double32_t[n]
   
   // Land-class: LandFirstHits-leafs:
   TLeaf* LandFirstHits_fUniqueID;			// UInt_t
   TLeaf* LandFirstHits_fBits;				// UInt_t
   TLeaf* LandFirstHits_fx0;				// Double32_t
   TLeaf* LandFirstHits_fy0;				// Double32_t
   TLeaf* LandFirstHits_fz0;				// Double32_t
   TLeaf* LandFirstHits_fT0;				// Double32_t
   TLeaf* LandFirstHits_fx1;				// Double32_t
   TLeaf* LandFirstHits_fy1;				// Double32_t	
   TLeaf* LandFirstHits_fz1;				// Double32_t
   TLeaf* LandFirstHits_fT1;				// Double32_t
   TLeaf* LandFirstHits_fx2;				// Double32_t
   TLeaf* LandFirstHits_fy2;				// Double32_t
   TLeaf* LandFirstHits_fz2;				// Double32_t
   TLeaf* LandFirstHits_fT2;				// Double32_t
   TLeaf* LandFirstHits_fx3;				// Double32_t
   TLeaf* LandFirstHits_fy3;				// Double32_t
   TLeaf* LandFirstHits_fz3;				// Double32_t
   TLeaf* LandFirstHits_fT3;				// Double32_t
   TLeaf* LandFirstHits_fx4;				// Double32_t
   TLeaf* LandFirstHits_fy4;				// Double32_t
   TLeaf* LandFirstHits_fz4;				// Double32_t
   TLeaf* LandFirstHits_fT4;				// Double32_t
   TLeaf* LandFirstHits_fx5;				// Double32_t
   TLeaf* LandFirstHits_fy5;				// Double32_t
   TLeaf* LandFirstHits_fz5;				// Double32_t
   TLeaf* LandFirstHits_fT5;				// Double32_t
   
   // Neuland-class: NeulandPrimaryNeutronInteractionPoints:
   TLeaf* NeuPrimIntPoint_fUniqueID;			// UInt_t[n]
   TLeaf* NeuPrimIntPoint_fBits;			// UInt_t[n]
   TLeaf* NeuPrimIntPoint_fLink;			// FairMultiLinkedData*[n]
   TLeaf* NeuPrimIntPoint_fTrackID;			// Int_t[n]
   TLeaf* NeuPrimIntPoint_fEventId;			// UInt_t[n]
   TLeaf* NeuPrimIntPoint_fPx;				// Double32_t[n]
   TLeaf* NeuPrimIntPoint_fPy;				// Double32_t[n]
   TLeaf* NeuPrimIntPoint_fPz;				// Double32_t[n]
   TLeaf* NeuPrimIntPoint_fTime;			// Double32_t[n]
   TLeaf* NeuPrimIntPoint_fLength;			// Double32_t[n]
   TLeaf* NeuPrimIntPoint_fELoss;			// Double32_t[n]
   TLeaf* NeuPrimIntPoint_fDetectorID;			// Int_t[n]
   TLeaf* NeuPrimIntPoint_fX;				// Double32_t[n]
   TLeaf* NeuPrimIntPoint_fY;				// Double32_t[n]
   TLeaf* NeuPrimIntPoint_fZ;				// Double32_t[n]
   
   // veto_seg-class: veto_segPoint-leafs:
   TLeaf* veto_segPoint_fUniqueID;			// UInt_t[n]
   TLeaf* veto_segPoint_fBits;				// UInt_t[n]
   TLeaf* veto_segPoint_fLink;				// FairMultiLinkedData*[n]
   TLeaf* veto_segPoint_fTrackID;			// Int_t[n]
   TLeaf* veto_segPoint_fEventId;			// UInt_t[n]
   TLeaf* veto_segPoint_fPx;				// Double32_t[n]
   TLeaf* veto_segPoint_fPy;				// Double32_t[n]
   TLeaf* veto_segPoint_fPz;				// Double32_t[n]
   TLeaf* veto_segPoint_fTime;				// Double32_t[n]
   TLeaf* veto_segPoint_fLength;			// Double32_t[n]
   TLeaf* veto_segPoint_fELoss;				// Double32_t[n]
   TLeaf* veto_segPoint_fDetectorID;			// Int_t[n]
   TLeaf* veto_segPoint_fX;				// Double32_t[n]
   TLeaf* veto_segPoint_fY;				// Double32_t[n]
   TLeaf* veto_segPoint_fZ;				// Double32_t[n]
   TLeaf* veto_segPoint_fX_out;				// Double32_t[n]
   TLeaf* veto_segPoint_fY_out;				// Double32_t[n]
   TLeaf* veto_segPoint_fZ_out;				// Double32_t[n]
   TLeaf* veto_segPoint_fPx_out;			// Double32_t[n]
   TLeaf* veto_segPoint_fPy_out;			// Double32_t[n]
   TLeaf* veto_segPoint_fPz_out;			// Double32_t[n]
   TLeaf* veto_segPoint_fSector;			// Int_t[n]
   TLeaf* veto_segPoint_fPaddleNb;			// Int_t[n]
   TLeaf* veto_segPoint_fPaddleTyp;			// Int_t[n]
   TLeaf* veto_segPoint_fLightYield;			// Double32_t[n]
   
   // veto_seg-class: veto_segFirstHits-leafs:
   TLeaf* veto_segFirstHits_fUniqueID;			// UInt_t
   TLeaf* veto_segFirstHits_fBits;			// UInt_t
   TLeaf* veto_segFirstHits_fx0;			// Double32_t
   TLeaf* veto_segFirstHits_fy0;			// Double32_t
   TLeaf* veto_segFirstHits_fz0;			// Double32_t
   TLeaf* veto_segFirstHits_fT0;			// Double32_t
   TLeaf* veto_segFirstHits_fx1;			// Double32_t
   TLeaf* veto_segFirstHits_fy1;			// Double32_t
   TLeaf* veto_segFirstHits_fz1;			// Double32_t
   TLeaf* veto_segFirstHits_fT1;			// Double32_t
   TLeaf* veto_segFirstHits_fx2;			// Double32_t
   TLeaf* veto_segFirstHits_fy2;			// Double32_t
   TLeaf* veto_segFirstHits_fz2;			// Double32_t
   TLeaf* veto_segFirstHits_fT2;			// Double32_t
   TLeaf* veto_segFirstHits_fx3;			// Double32_t
   TLeaf* veto_segFirstHits_fy3;			// Double32_t
   TLeaf* veto_segFirstHits_fz3;			// Double32_t
   TLeaf* veto_segFirstHits_fT3;			// Double32_t
   TLeaf* veto_segFirstHits_fx4;			// Double32_t
   TLeaf* veto_segFirstHits_fy4;			// Double32_t
   TLeaf* veto_segFirstHits_fz4;			// Double32_t
   TLeaf* veto_segFirstHits_fT4;			// Double32_t
   TLeaf* veto_segFirstHits_fx5;			// Double32_t
   TLeaf* veto_segFirstHits_fy5;			// Double32_t
   TLeaf* veto_segFirstHits_fz5;			// Double32_t
   TLeaf* veto_segFirstHits_fT5;			// Double32_t
   
   // Neuland-class: Nveto_segPrimaryNeutronInteractionPoints:
   TLeaf* VetoPrimIntPoint_fUniqueID;			// UInt_t[n]
   TLeaf* VetoPrimIntPoint_fBits;			// UInt_t[n]
   TLeaf* VetoPrimIntPoint_fLink;			// FairMultiLinkedData*[n]
   TLeaf* VetoPrimIntPoint_fTrackID;			// Int_t[n]
   TLeaf* VetoPrimIntPoint_fEventId;			// UInt_t[n]
   TLeaf* VetoPrimIntPoint_fPx;				// Double32_t[n]
   TLeaf* VetoPrimIntPoint_fPy;				// Double32_t[n]
   TLeaf* VetoPrimIntPoint_fPz;				// Double32_t[n]
   TLeaf* VetoPrimIntPoint_fTime;			// Double32_t[n]
   TLeaf* VetoPrimIntPoint_fLength;			// Double32_t[n]
   TLeaf* VetoPrimIntPoint_fELoss;			// Double32_t[n]
   TLeaf* VetoPrimIntPoint_fDetectorID;			// Int_t[n]
   TLeaf* VetoPrimIntPoint_fX;				// Double32_t[n]
   TLeaf* VetoPrimIntPoint_fY;				// Double32_t[n]
   TLeaf* VetoPrimIntPoint_fZ;				// Double32_t[n]
   
   // MFI leafs:
   TLeaf* MFIPoint_fUniqueID;				// UInt_t[n]
   TLeaf* MFIPoint_fBits;				// UInt_t[n]
   TLeaf* MFIPoint_fLink;				// FairMultiLinkedData*[n]
   TLeaf* MFIPoint_fTrackID;				// Int_t[n]
   TLeaf* MFIPoint_fEventId;				// UInt_t[n]
   TLeaf* MFIPoint_fPx;					// Double32_t[n]
   TLeaf* MFIPoint_fPy;					// Double32_t[n]
   TLeaf* MFIPoint_fPz;					// Double32_t[n]
   TLeaf* MFIPoint_fTime;				// Double32_t[n]
   TLeaf* MFIPoint_fLength;				// Double32_t[n]
   TLeaf* MFIPoint_fELoss;				// Double32_t[n]
   TLeaf* MFIPoint_fDetectorID;				// Int_t[n]
   TLeaf* MFIPoint_fX;					// Double32_t[n]
   TLeaf* MFIPoint_fY;					// Double32_t[n]
   TLeaf* MFIPoint_fZ;					// Double32_t[n]
   TLeaf* MFIPoint_fX_out;				// Double32_t[n]
   TLeaf* MFIPoint_fY_out;				// Double32_t[n]
   TLeaf* MFIPoint_fZ_out;				// Double32_t[n]
   TLeaf* MFIPoint_fPx_out;				// Double32_t[n]
   TLeaf* MFIPoint_fPy_out;				// Double32_t[n]
   TLeaf* MFIPoint_fPz_out;				// Double32_t[n]
   TLeaf* MFIPoint_fModule;				// Int_t[n]
   
   // PSP leafs:
   TLeaf* PSPPoint_fUniqueID;				// UInt_t[n]
   TLeaf* PSPPoint_fBits;				// UInt_t[n]
   TLeaf* PSPPoint_fLink;				// FairMultiLinkedData*[n]
   TLeaf* PSPPoint_fTrackID;				// Int_t[n]
   TLeaf* PSPPoint_fEventId;				// UInt_t[n]
   TLeaf* PSPPoint_fPx;					// Double32_t[n]
   TLeaf* PSPPoint_fPy;					// Double32_t[n]
   TLeaf* PSPPoint_fPz;					// Double32_t[n]
   TLeaf* PSPPoint_fTime;				// Double32_t[n]
   TLeaf* PSPPoint_fLength;				// Double32_t[n]
   TLeaf* PSPPoint_fELoss;				// Double32_t[n]
   TLeaf* PSPPoint_fDetectorID;				// Int_t[n]
   TLeaf* PSPPoint_fX;					// Double32_t[n]
   TLeaf* PSPPoint_fY;					// Double32_t[n]
   TLeaf* PSPPoint_fZ;					// Double32_t[n]
   TLeaf* PSPPoint_fX_out;				// Double32_t[n]
   TLeaf* PSPPoint_fY_out;				// Double32_t[n]
   TLeaf* PSPPoint_fZ_out;				// Double32_t[n]
   TLeaf* PSPPoint_fPx_out;				// Double32_t[n]
   TLeaf* PSPPoint_fPy_out;				// Double32_t[n]
   TLeaf* PSPPoint_fPz_out;				// Double32_t[n]
   TLeaf* PSPPoint_fModule;				// Int_t[n]
   
   // MC Eventheader leafs:
   TLeaf* MCEventHeader_TNamed_fUniqueID;		// UInt_t
   TLeaf* MCEventHeader_TNamed_fBits;			// UInt_t
   TLeaf* MCEventHeader_TNamed_fName;			// TString
   TLeaf* MCEventHeader_TNamed_fTitle;			// TString
   TLeaf* MCEventHeader_fRunId;				// UInt_t
   TLeaf* MCEventHeader_fEventId;			// UInt_t
   TLeaf* MCEventHeader_fX;				// Double32_t
   TLeaf* MCEventHeader_fY;				// Double32_t
   TLeaf* MCEventHeader_fZ;				// Double32_t
   TLeaf* MCEventHeader_fT;				// Double32_t
   TLeaf* MCEventHeader_fB;				// Double32_t
   TLeaf* MCEventHeader_fNPrim;				// Int_t
   TLeaf* MCEventHeader_fIsSet;				// Bool_t
   TLeaf* MCEventHeader_fRotX;				// Double32_t
   TLeaf* MCEventHeader_fRotY;				// Double32_t
   TLeaf* MCEventHeader_fRotZ;				// Double32_t
   
   // GeoTracks leafs:
   TLeaf* GeoTracks_fUniqueID;				// UInt_t[n]
   TLeaf* GeoTracks_fBits;				// UInt_t[n]
   TLeaf* GeoTracks_fGeoAtt;				// UInt_t[n]
   TLeaf* GeoTracks_fLineColor;				// Short_t[n]
   TLeaf* GeoTracks_fLineStyle;				// Short_t[n]
   TLeaf* GeoTracks_fLineWidth;				// Short_t[n]
   TLeaf* GeoTracks_fMarkerColor;			// Short_t[n]
   TLeaf* GeoTracks_fMarkerStyle;			// Short_t[n]
   TLeaf* GeoTracks_fMarkerSize;			// Float_t[n]
   TLeaf* GeoTracks_fPDG;				// Int_t[n]
   TLeaf* GeoTracks_fId;				// Int_t[n]
   TLeaf* GeoTracks_fParent;				// TVirtualGeoTrack*[n]
   TLeaf* GeoTracks_fParticle;				// TObject*[n]
   TLeaf* GeoTracks_fTracks;				// TObjArray*[n]
   TLeaf* GeoTracks_fPointsSize;			// Int_t[n]
   TLeaf* GeoTracks_fNpoints;				// Int_t[n]
   TLeaf* GeoTracks_fPoints;				// Double_t[n]
   
   // NeuLAND Digitizer leafs:
   TLeaf* LandDigi_fUniqueID;				// UInt_t[n]
   TLeaf* LandDigi_fBits;				// UInt_t[n]
   TLeaf* LandDigi_fPaddleNb;				// Int_t[n]
   TLeaf* LandDigi_fTdcL;				// Double32_t[n]
   TLeaf* LandDigi_fTdcR;				// Double32_t[n]
   TLeaf* LandDigi_fTdc;				// Double32_t[n]
   TLeaf* LandDigi_fQdcL;				// Double32_t[n]
   TLeaf* LandDigi_fQdcR;				// Double32_t[n]
   TLeaf* LandDigi_fQdc;				// Double32_t[n]
   TLeaf* LandDigi_fxx;					// Double32_t[n]
   TLeaf* LandDigi_fyy;					// Double32_t[n]
   TLeaf* LandDigi_fzz;					// Double32_t[n]
   
   // NeuLand ClusterFinder leafs:
   TLeaf* NeuLandCluster_fUniqueID;			// UInt_t[n]
   TLeaf* NeuLandCluster_fBits;				// UInt_t[n]
   TLeaf* NeuLandCluster_fStartX;			// Double32_t[n]
   TLeaf* NeuLandCluster_fStartY;			// Double32_t[n]
   TLeaf* NeuLandCluster_fStartZ;			// Double32_t[n]
   TLeaf* NeuLandCluster_fStartT;			// Double32_t[n]
   TLeaf* NeuLandCluster_fStopX;			// Double32_t[n]
   TLeaf* NeuLandCluster_fStopY;			// Double32_t[n]
   TLeaf* NeuLandCluster_fStopZ;			// Double32_t[n]
   TLeaf* NeuLandCluster_fStopT;			// Double32_t[n]
   TLeaf* NeuLandCluster_fEdgeX;			// Double32_t[n]
   TLeaf* NeuLandCluster_fEdgeY;			// Double32_t[n]
   TLeaf* NeuLandCluster_fEdgeZ;			// Double32_t[n]
   TLeaf* NeuLandCluster_fEdgeT;			// Double32_t[n]
   TLeaf* NeuLandCluster_fE;				// Double32_t[n]
   TLeaf* NeuLandCluster_fSize;				// Double32_t[n]	
   TLeaf* NeuLandCluster_fTopology;			// Int_t[n]
   TLeaf* NeuLandCluster_fMarked;			// Bool_t[n]
   TLeaf* NeuLandCluster_fEliminated;			// Bool_t[n]
   TLeaf* NeuLandCluster_fDigibars;			// std::vector<Int_t>[n]
   
   // VETO Digitizer leafs:
   TLeaf* veto_segDigi_fUniqueID;			// UInt_t[n]
   TLeaf* veto_segDigi_fBits;				// UInt_t[n]
   TLeaf* veto_segDigi_fPaddleNb;			// Int_t[n]
   TLeaf* veto_segDigi_fTdcL;				// Double32_t[n]
   TLeaf* veto_segDigi_fTdcR;				// Double32_t[n]
   TLeaf* veto_segDigi_fTdc;				// Double32_t[n]
   TLeaf* veto_segDigi_fQdcL;				// Double32_t[n]
   TLeaf* veto_segDigi_fQdcR;				// Double32_t[n]
   TLeaf* veto_segDigi_fQdc;				// Double32_t[n]
   TLeaf* veto_segDigi_fxx;				// Double32_t[n]
   TLeaf* veto_segDigi_fyy;				// Double32_t[n]
   TLeaf* veto_segDigi_fzz;				// Double32_t[n]
   
   // VETO ClusterFinder leafs:
   TLeaf* Nveto_segCluster_fUniqueID;			// UInt_t[n]
   TLeaf* Nveto_segCluster_fBits;			// UInt_t[n]
   TLeaf* Nveto_segCluster_fStartX;			// Double32_t[n]
   TLeaf* Nveto_segCluster_fStartY;			// Double32_t[n]
   TLeaf* Nveto_segCluster_fStartZ;			// Double32_t[n]
   TLeaf* Nveto_segCluster_fStartT;			// Double32_t[n]
   TLeaf* Nveto_segCluster_fStopX;			// Double32_t[n]
   TLeaf* Nveto_segCluster_fStopY;			// Double32_t[n]
   TLeaf* Nveto_segCluster_fStopZ;			// Double32_t[n]
   TLeaf* Nveto_segCluster_fStopT;			// Double32_t[n]
   TLeaf* Nveto_segCluster_fE;				// Double32_t[n]
   TLeaf* Nveto_segCluster_fSize;			// Double32_t[n]	
   TLeaf* Nveto_segCluster_fMarked;			// Bool_t[n]
   
   // Neutron Tracker Leafs:
   TLeaf* LandNeutHits_fUniqueID;
   TLeaf* LandNeutHits_fBits;
   TLeaf* LandNeutHits_fX;
   TLeaf* LandNeutHits_fY;
   TLeaf* LandNeutHits_fZ;
   TLeaf* LandNeutHits_fT;
   TLeaf* LandNeutHits_fE;
   TLeaf* LandNeutHits_fMCX;
   TLeaf* LandNeutHits_fMCY;
   TLeaf* LandNeutHits_fMCZ;
   TLeaf* LandNeutHits_fMCT;
   TLeaf* LandNeutHits_fMCE;
   TLeaf* LandNeutHits_fDeltaT;			// Double32_t[n]
   TLeaf* LandNeutHits_fDeltaR;			// Double32_t[n]
   TLeaf* LandNeutHits_fHasMC;
   TLeaf* LandNeutHits_fIsCompared;
   TLeaf* LandNeutHits_fSucces;
   TLeaf* LandNeutHits_fTrackID;
   TLeaf* LandNeutHits_fPDG;
   
   // Other digitizer leafs:
   TLeaf* EventHeader_TNamed_fUniqueID;
   TLeaf* EventHeader_TNamed_fBits;
   TLeaf* EventHeader_TNamed_fName;
   TLeaf* EventHeader_TNamed_fTitle;
   TLeaf* EventHeader_fRunId;
   TLeaf* EventHeader_fEventTime;
   TLeaf* EventHeader_fInputFileId;
   TLeaf* EventHeader_fMCEntryNo;
   
   // CALIFA Digitizer leafs:
   TLeaf* CaloHitSim_fUniqueID;
   TLeaf* CaloHitSim_fBits;
   TLeaf* CaloHitSim_fLinks;
   TLeaf* CaloHitSim_fEntryNr_fUniqueID;
   TLeaf* CaloHitSim_fEntryNr_fBits;
   TLeaf* CaloHitSim_fEntryNr_fFile;
   TLeaf* CaloHitSim_fEntryNr_fType;
   TLeaf* CaloHitSim_fEntryNr_fIndex;
   TLeaf* CaloHitSim_fEntryNr_fWeight;
   TLeaf* CaloHitSim_fEntryNr_fEntry;
   TLeaf* CaloHitSim_fDefaultType;
   TLeaf* CaloHitSim_fNbOfCrystalHits;
   TLeaf* CaloHitSim_fEnergy;
   TLeaf* CaloHitSim_fNf;
   TLeaf* CaloHitSim_fNs;
   TLeaf* CaloHitSim_fTheta;
   TLeaf* CaloHitSim_fPhi;
   TLeaf* CaloHitSim_fTime;
   TLeaf* CaloHitSim_fEinc;
   
   // STaRTrack hit leafs:
   TLeaf* STaRTrackerHit_fUniqueID;
   TLeaf* STaRTrackerHit_fBits;
   TLeaf* STaRTrackerHit_fLinks;
   TLeaf* STaRTrackerHit_fEntryNr_fUniqueID;
   TLeaf* STaRTrackerHit_fEntryNr_fBits;
   TLeaf* STaRTrackerHit_fEntryNr_fFile;
   TLeaf* STaRTrackerHit_fEntryNr_fEntry;
   TLeaf* STaRTrackerHit_fEntryNr_fType;
   TLeaf* STaRTrackerHit_fEntryNr_fIndex;
   TLeaf* STaRTrackerHit_fEntryNr_fWeight;
   TLeaf* STaRTrackerHit_fDefaultType;
   TLeaf* STaRTrackerHit_fEnergy;
   TLeaf* STaRTrackerHit_fDetector;
   TLeaf* STaRTrackerHit_fXHit;
   TLeaf* STaRTrackerHit_fYHit;
   TLeaf* STaRTrackerHit_fZHit;
   TLeaf* STaRTrackerHit_fPx;
   TLeaf* STaRTrackerHit_fPy;
   TLeaf* STaRTrackerHit_fPz;
   TLeaf* STaRTrackerHit_fThetaZero;
   TLeaf* STaRTrackerHit_fPhiZero;
   
   // STaRTrack Digitizer leafs:
   TLeaf* STaRTrackerDigitHit_fUniqueID;
   TLeaf* STaRTrackerDigitHit_fBits;
   TLeaf* STaRTrackerDigitHit_fLinks;
   TLeaf* STaRTrackerDigitHit_fEntryNr_fUniqueID;
   TLeaf* STaRTrackerDigitHit_fEntryNr_fBits;
   TLeaf* STaRTrackerDigitHit_fEntryNr_fFile;
   TLeaf* STaRTrackerDigitHit_fEntryNr_fEntry;
   TLeaf* STaRTrackerDigitHit_fEntryNr_fType;
   TLeaf* STaRTrackerDigitHit_fEntryNr_fIndex;
   TLeaf* STaRTrackerDigitHit_fEntryNr_fWeight;
   TLeaf* STaRTrackerDigitHit_fDefaultType;
   TLeaf* STaRTrackerDigitHit_fEnergy;
   TLeaf* STaRTrackerDigitHit_fTime;
   TLeaf* STaRTrackerDigitHit_fDetector;
   TLeaf* STaRTrackerDigitHit_fChip;
   TLeaf* STaRTrackerDigitHit_fSide;
   TLeaf* STaRTrackerDigitHit_fStrip;
   
   // STaRTrack Track-leafs:
   TLeaf* TrackSize;
   TLeaf* Xtrack;
   TLeaf* Ytrack;
   TLeaf* Ztrack;
   TLeaf* ThetaTrack;
   TLeaf* PhiTrack;
   TLeaf* ElossTrack;
   TLeaf* NumberOfSolvedTracks;
   TLeaf* NumberOfGoodTracks;
   TLeaf* MiddleMult;
   TLeaf* theta;
   TLeaf* phi;
   TLeaf* Ep;
   
   // Whether detectors exists yes/no:
   Bool_t TARGET_exists;
   Bool_t STaRTrack_exists;
   Bool_t GLAD_exists;
   Bool_t TOF_exists;
   Bool_t mTOF_exists;
   Bool_t PSP_exists;
   Bool_t MFI_exists;
   Bool_t DCH_exists;
   Bool_t GFI_exists;
   Bool_t ALADIN_exists;
   Bool_t NeuLAND_exists;
   Bool_t VETO_exists;
   Bool_t VACVESSEL_exists;
   Bool_t CALIFA_exists;
   Bool_t CrystalBall_exists;
   Bool_t VacChamber_exists;
   
   // Whether the Tracker exists yes/no:
   Bool_t Tracker_Exists;
};




