//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Aug  5 11:44:28 2014 by ROOT version 5.34/13
// from TTree cbmsim//cbmroot
// found on file: r3bsim.root
//////////////////////////////////////////////////////////

#ifndef cbmsim_h
#define cbmsim_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
//miki++
#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "TRandom.h"
#include "TTree.h"
//miki--

// Header file for the classes stored in the TTree if any.
#include <TClonesArray.h>
#include <TObject.h>
#include "/u/kmiki/r3broot/r3bdata/R3BMCTrack.h"
#include "/u/kmiki/r3broot/base/event/FairMultiLinkedData.h"
#include "/u/kmiki/r3broot/base/event/FairMCPoint.h"
#include "/u/kmiki/r3broot/r3bdata/tofData/R3BTofPoint.h"
#include "/u/kmiki/r3broot/r3bdata/gfiData/R3BGfiPoint.h"
#include "/u/kmiki/r3broot/r3bdata/landData/R3BLandPoint.h"
#include "/u/kmiki/r3broot/r3bdata/landData/R3BLandFirstHits.h"
#include "/u/kmiki/r3broot/base/event/FairMCEventHeader.h"
#include <TNamed.h>
#include <TGeoAtt.h>
#include <TAttLine.h>
#include <TAttMarker.h>
#include <TVirtualGeoTrack.h>
#include <TGeoTrack.h>

// Fixed size dimensions of array or collections stored in the TTree if any.
const Int_t kMaxcbmroot_Stack_MCTrack = 4000;
const Int_t kMaxcbmroot_Tof_TOFPoint = 200;
const Int_t kMaxcbmroot_Gfi_GFIPoint = 100;
const Int_t kMaxcbmroot_Land_LandPoint = 500;
const Int_t kMaxcbmroot_Land_LandFirstHits = 1;
const Int_t kMaxcbmroot_Event_MCEventHeader = 1;
const Int_t kMaxcbmroot_MCGeoTrack_GeoTracks = 4000;

class cbmsim {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           MCTrack_;
   UInt_t          MCTrack_fUniqueID[kMaxcbmroot_Stack_MCTrack];   //[cbmroot.Stack.MCTrack_]
   UInt_t          MCTrack_fBits[kMaxcbmroot_Stack_MCTrack];   //[cbmroot.Stack.MCTrack_]
   Int_t           MCTrack_fPdgCode[kMaxcbmroot_Stack_MCTrack];   //[cbmroot.Stack.MCTrack_]
   Int_t           MCTrack_fMotherId[kMaxcbmroot_Stack_MCTrack];   //[cbmroot.Stack.MCTrack_]
   Double32_t      MCTrack_fPx[kMaxcbmroot_Stack_MCTrack];   //[cbmroot.Stack.MCTrack_]
   Double32_t      MCTrack_fPy[kMaxcbmroot_Stack_MCTrack];   //[cbmroot.Stack.MCTrack_]
   Double32_t      MCTrack_fPz[kMaxcbmroot_Stack_MCTrack];   //[cbmroot.Stack.MCTrack_]
   Double32_t      MCTrack_fStartX[kMaxcbmroot_Stack_MCTrack];   //[cbmroot.Stack.MCTrack_]
   Double32_t      MCTrack_fStartY[kMaxcbmroot_Stack_MCTrack];   //[cbmroot.Stack.MCTrack_]
   Double32_t      MCTrack_fStartZ[kMaxcbmroot_Stack_MCTrack];   //[cbmroot.Stack.MCTrack_]
   Double32_t      MCTrack_fStartT[kMaxcbmroot_Stack_MCTrack];   //[cbmroot.Stack.MCTrack_]
   Int_t           MCTrack_fNPoints[kMaxcbmroot_Stack_MCTrack];   //[cbmroot.Stack.MCTrack_]
   Double32_t      MCTrack_fMass[kMaxcbmroot_Stack_MCTrack];   //[cbmroot.Stack.MCTrack_]
   Int_t           TOFPoint_;
   UInt_t          TOFPoint_fUniqueID[kMaxcbmroot_Tof_TOFPoint];   //[cbmroot.Tof.TOFPoint_]
   UInt_t          TOFPoint_fBits[kMaxcbmroot_Tof_TOFPoint];   //[cbmroot.Tof.TOFPoint_]
 //set<FairLink>   TOFPoint_fLinks[kMaxcbmroot_Tof_TOFPoint];
   Bool_t          TOFPoint_fPersistanceCheck[kMaxcbmroot_Tof_TOFPoint];   //[cbmroot.Tof.TOFPoint_]
   Int_t           TOFPoint_fVerbose[kMaxcbmroot_Tof_TOFPoint];   //[cbmroot.Tof.TOFPoint_]
   Int_t           TOFPoint_fDefaultType[kMaxcbmroot_Tof_TOFPoint];   //[cbmroot.Tof.TOFPoint_]
   Int_t           TOFPoint_fTrackID[kMaxcbmroot_Tof_TOFPoint];   //[cbmroot.Tof.TOFPoint_]
   UInt_t          TOFPoint_fEventId[kMaxcbmroot_Tof_TOFPoint];   //[cbmroot.Tof.TOFPoint_]
   Double32_t      TOFPoint_fPx[kMaxcbmroot_Tof_TOFPoint];   //[cbmroot.Tof.TOFPoint_]
   Double32_t      TOFPoint_fPy[kMaxcbmroot_Tof_TOFPoint];   //[cbmroot.Tof.TOFPoint_]
   Double32_t      TOFPoint_fPz[kMaxcbmroot_Tof_TOFPoint];   //[cbmroot.Tof.TOFPoint_]
   Double32_t      TOFPoint_fTime[kMaxcbmroot_Tof_TOFPoint];   //[cbmroot.Tof.TOFPoint_]
   Double32_t      TOFPoint_fLength[kMaxcbmroot_Tof_TOFPoint];   //[cbmroot.Tof.TOFPoint_]
   Double32_t      TOFPoint_fELoss[kMaxcbmroot_Tof_TOFPoint];   //[cbmroot.Tof.TOFPoint_]
   Int_t           TOFPoint_fDetectorID[kMaxcbmroot_Tof_TOFPoint];   //[cbmroot.Tof.TOFPoint_]
   Double32_t      TOFPoint_fX[kMaxcbmroot_Tof_TOFPoint];   //[cbmroot.Tof.TOFPoint_]
   Double32_t      TOFPoint_fY[kMaxcbmroot_Tof_TOFPoint];   //[cbmroot.Tof.TOFPoint_]
   Double32_t      TOFPoint_fZ[kMaxcbmroot_Tof_TOFPoint];   //[cbmroot.Tof.TOFPoint_]
   Double32_t      TOFPoint_fX_out[kMaxcbmroot_Tof_TOFPoint];   //[cbmroot.Tof.TOFPoint_]
   Double32_t      TOFPoint_fY_out[kMaxcbmroot_Tof_TOFPoint];   //[cbmroot.Tof.TOFPoint_]
   Double32_t      TOFPoint_fZ_out[kMaxcbmroot_Tof_TOFPoint];   //[cbmroot.Tof.TOFPoint_]
   Double32_t      TOFPoint_fPx_out[kMaxcbmroot_Tof_TOFPoint];   //[cbmroot.Tof.TOFPoint_]
   Double32_t      TOFPoint_fPy_out[kMaxcbmroot_Tof_TOFPoint];   //[cbmroot.Tof.TOFPoint_]
   Double32_t      TOFPoint_fPz_out[kMaxcbmroot_Tof_TOFPoint];   //[cbmroot.Tof.TOFPoint_]
   Int_t           GFIPoint_;
   UInt_t          GFIPoint_fUniqueID[kMaxcbmroot_Gfi_GFIPoint];   //[cbmroot.Gfi.GFIPoint_]
   UInt_t          GFIPoint_fBits[kMaxcbmroot_Gfi_GFIPoint];   //[cbmroot.Gfi.GFIPoint_]
 //set<FairLink>   GFIPoint_fLinks[kMaxcbmroot_Gfi_GFIPoint];
   Bool_t          GFIPoint_fPersistanceCheck[kMaxcbmroot_Gfi_GFIPoint];   //[cbmroot.Gfi.GFIPoint_]
   Int_t           GFIPoint_fVerbose[kMaxcbmroot_Gfi_GFIPoint];   //[cbmroot.Gfi.GFIPoint_]
   Int_t           GFIPoint_fDefaultType[kMaxcbmroot_Gfi_GFIPoint];   //[cbmroot.Gfi.GFIPoint_]
   Int_t           GFIPoint_fTrackID[kMaxcbmroot_Gfi_GFIPoint];   //[cbmroot.Gfi.GFIPoint_]
   UInt_t          GFIPoint_fEventId[kMaxcbmroot_Gfi_GFIPoint];   //[cbmroot.Gfi.GFIPoint_]
   Double32_t      GFIPoint_fPx[kMaxcbmroot_Gfi_GFIPoint];   //[cbmroot.Gfi.GFIPoint_]
   Double32_t      GFIPoint_fPy[kMaxcbmroot_Gfi_GFIPoint];   //[cbmroot.Gfi.GFIPoint_]
   Double32_t      GFIPoint_fPz[kMaxcbmroot_Gfi_GFIPoint];   //[cbmroot.Gfi.GFIPoint_]
   Double32_t      GFIPoint_fTime[kMaxcbmroot_Gfi_GFIPoint];   //[cbmroot.Gfi.GFIPoint_]
   Double32_t      GFIPoint_fLength[kMaxcbmroot_Gfi_GFIPoint];   //[cbmroot.Gfi.GFIPoint_]
   Double32_t      GFIPoint_fELoss[kMaxcbmroot_Gfi_GFIPoint];   //[cbmroot.Gfi.GFIPoint_]
   Int_t           GFIPoint_fDetectorID[kMaxcbmroot_Gfi_GFIPoint];   //[cbmroot.Gfi.GFIPoint_]
   Double32_t      GFIPoint_fX[kMaxcbmroot_Gfi_GFIPoint];   //[cbmroot.Gfi.GFIPoint_]
   Double32_t      GFIPoint_fY[kMaxcbmroot_Gfi_GFIPoint];   //[cbmroot.Gfi.GFIPoint_]
   Double32_t      GFIPoint_fZ[kMaxcbmroot_Gfi_GFIPoint];   //[cbmroot.Gfi.GFIPoint_]
   Double32_t      GFIPoint_fX_out[kMaxcbmroot_Gfi_GFIPoint];   //[cbmroot.Gfi.GFIPoint_]
   Double32_t      GFIPoint_fY_out[kMaxcbmroot_Gfi_GFIPoint];   //[cbmroot.Gfi.GFIPoint_]
   Double32_t      GFIPoint_fZ_out[kMaxcbmroot_Gfi_GFIPoint];   //[cbmroot.Gfi.GFIPoint_]
   Double32_t      GFIPoint_fPx_out[kMaxcbmroot_Gfi_GFIPoint];   //[cbmroot.Gfi.GFIPoint_]
   Double32_t      GFIPoint_fPy_out[kMaxcbmroot_Gfi_GFIPoint];   //[cbmroot.Gfi.GFIPoint_]
   Double32_t      GFIPoint_fPz_out[kMaxcbmroot_Gfi_GFIPoint];   //[cbmroot.Gfi.GFIPoint_]
   Int_t           GFIPoint_fModule[kMaxcbmroot_Gfi_GFIPoint];   //[cbmroot.Gfi.GFIPoint_]
   Int_t           LandPoint_;
   UInt_t          LandPoint_fUniqueID[kMaxcbmroot_Land_LandPoint];   //[cbmroot.Land.LandPoint_]
   UInt_t          LandPoint_fBits[kMaxcbmroot_Land_LandPoint];   //[cbmroot.Land.LandPoint_]
 //set<FairLink>   LandPoint_fLinks[kMaxcbmroot_Land_LandPoint];
   Bool_t          LandPoint_fPersistanceCheck[kMaxcbmroot_Land_LandPoint];   //[cbmroot.Land.LandPoint_]
   Int_t           LandPoint_fVerbose[kMaxcbmroot_Land_LandPoint];   //[cbmroot.Land.LandPoint_]
   Int_t           LandPoint_fDefaultType[kMaxcbmroot_Land_LandPoint];   //[cbmroot.Land.LandPoint_]
   Int_t           LandPoint_fTrackID[kMaxcbmroot_Land_LandPoint];   //[cbmroot.Land.LandPoint_]
   UInt_t          LandPoint_fEventId[kMaxcbmroot_Land_LandPoint];   //[cbmroot.Land.LandPoint_]
   Double32_t      LandPoint_fPx[kMaxcbmroot_Land_LandPoint];   //[cbmroot.Land.LandPoint_]
   Double32_t      LandPoint_fPy[kMaxcbmroot_Land_LandPoint];   //[cbmroot.Land.LandPoint_]
   Double32_t      LandPoint_fPz[kMaxcbmroot_Land_LandPoint];   //[cbmroot.Land.LandPoint_]
   Double32_t      LandPoint_fTime[kMaxcbmroot_Land_LandPoint];   //[cbmroot.Land.LandPoint_]
   Double32_t      LandPoint_fLength[kMaxcbmroot_Land_LandPoint];   //[cbmroot.Land.LandPoint_]
   Double32_t      LandPoint_fELoss[kMaxcbmroot_Land_LandPoint];   //[cbmroot.Land.LandPoint_]
   Int_t           LandPoint_fDetectorID[kMaxcbmroot_Land_LandPoint];   //[cbmroot.Land.LandPoint_]
   Double32_t      LandPoint_fX[kMaxcbmroot_Land_LandPoint];   //[cbmroot.Land.LandPoint_]
   Double32_t      LandPoint_fY[kMaxcbmroot_Land_LandPoint];   //[cbmroot.Land.LandPoint_]
   Double32_t      LandPoint_fZ[kMaxcbmroot_Land_LandPoint];   //[cbmroot.Land.LandPoint_]
   Double32_t      LandPoint_fX_out[kMaxcbmroot_Land_LandPoint];   //[cbmroot.Land.LandPoint_]
   Double32_t      LandPoint_fY_out[kMaxcbmroot_Land_LandPoint];   //[cbmroot.Land.LandPoint_]
   Double32_t      LandPoint_fZ_out[kMaxcbmroot_Land_LandPoint];   //[cbmroot.Land.LandPoint_]
   Double32_t      LandPoint_fPx_out[kMaxcbmroot_Land_LandPoint];   //[cbmroot.Land.LandPoint_]
   Double32_t      LandPoint_fPy_out[kMaxcbmroot_Land_LandPoint];   //[cbmroot.Land.LandPoint_]
   Double32_t      LandPoint_fPz_out[kMaxcbmroot_Land_LandPoint];   //[cbmroot.Land.LandPoint_]
   Int_t           LandPoint_fSector[kMaxcbmroot_Land_LandPoint];   //[cbmroot.Land.LandPoint_]
   Int_t           LandPoint_fPaddleNb[kMaxcbmroot_Land_LandPoint];   //[cbmroot.Land.LandPoint_]
   Int_t           LandPoint_fPaddleTyp[kMaxcbmroot_Land_LandPoint];   //[cbmroot.Land.LandPoint_]
   Double_t        LandPoint_fLightYield[kMaxcbmroot_Land_LandPoint];   //[cbmroot.Land.LandPoint_]
   Int_t           LandFirstHits_;
   UInt_t          LandFirstHits_fUniqueID[kMaxcbmroot_Land_LandFirstHits];   //[cbmroot.Land.LandFirstHits_]
   UInt_t          LandFirstHits_fBits[kMaxcbmroot_Land_LandFirstHits];   //[cbmroot.Land.LandFirstHits_]
   Double32_t      LandFirstHits_fx0[kMaxcbmroot_Land_LandFirstHits];   //[cbmroot.Land.LandFirstHits_]
   Double32_t      LandFirstHits_fy0[kMaxcbmroot_Land_LandFirstHits];   //[cbmroot.Land.LandFirstHits_]
   Double32_t      LandFirstHits_fz0[kMaxcbmroot_Land_LandFirstHits];   //[cbmroot.Land.LandFirstHits_]
   Double32_t      LandFirstHits_fT0[kMaxcbmroot_Land_LandFirstHits];   //[cbmroot.Land.LandFirstHits_]
   Double32_t      LandFirstHits_fx1[kMaxcbmroot_Land_LandFirstHits];   //[cbmroot.Land.LandFirstHits_]
   Double32_t      LandFirstHits_fy1[kMaxcbmroot_Land_LandFirstHits];   //[cbmroot.Land.LandFirstHits_]
   Double32_t      LandFirstHits_fz1[kMaxcbmroot_Land_LandFirstHits];   //[cbmroot.Land.LandFirstHits_]
   Double32_t      LandFirstHits_fT1[kMaxcbmroot_Land_LandFirstHits];   //[cbmroot.Land.LandFirstHits_]
   Double32_t      LandFirstHits_fx2[kMaxcbmroot_Land_LandFirstHits];   //[cbmroot.Land.LandFirstHits_]
   Double32_t      LandFirstHits_fy2[kMaxcbmroot_Land_LandFirstHits];   //[cbmroot.Land.LandFirstHits_]
   Double32_t      LandFirstHits_fz2[kMaxcbmroot_Land_LandFirstHits];   //[cbmroot.Land.LandFirstHits_]
   Double32_t      LandFirstHits_fT2[kMaxcbmroot_Land_LandFirstHits];   //[cbmroot.Land.LandFirstHits_]
   Double32_t      LandFirstHits_fx3[kMaxcbmroot_Land_LandFirstHits];   //[cbmroot.Land.LandFirstHits_]
   Double32_t      LandFirstHits_fy3[kMaxcbmroot_Land_LandFirstHits];   //[cbmroot.Land.LandFirstHits_]
   Double32_t      LandFirstHits_fz3[kMaxcbmroot_Land_LandFirstHits];   //[cbmroot.Land.LandFirstHits_]
   Double32_t      LandFirstHits_fT3[kMaxcbmroot_Land_LandFirstHits];   //[cbmroot.Land.LandFirstHits_]
   Double32_t      LandFirstHits_fx4[kMaxcbmroot_Land_LandFirstHits];   //[cbmroot.Land.LandFirstHits_]
   Double32_t      LandFirstHits_fy4[kMaxcbmroot_Land_LandFirstHits];   //[cbmroot.Land.LandFirstHits_]
   Double32_t      LandFirstHits_fz4[kMaxcbmroot_Land_LandFirstHits];   //[cbmroot.Land.LandFirstHits_]
   Double32_t      LandFirstHits_fT4[kMaxcbmroot_Land_LandFirstHits];   //[cbmroot.Land.LandFirstHits_]
   Double32_t      LandFirstHits_fx5[kMaxcbmroot_Land_LandFirstHits];   //[cbmroot.Land.LandFirstHits_]
   Double32_t      LandFirstHits_fy5[kMaxcbmroot_Land_LandFirstHits];   //[cbmroot.Land.LandFirstHits_]
   Double32_t      LandFirstHits_fz5[kMaxcbmroot_Land_LandFirstHits];   //[cbmroot.Land.LandFirstHits_]
   Double32_t      LandFirstHits_fT5[kMaxcbmroot_Land_LandFirstHits];   //[cbmroot.Land.LandFirstHits_]
   FairMCEventHeader *MCEventHeader_;
   UInt_t          MCEventHeader_TNamed_fUniqueID;
   UInt_t          MCEventHeader_TNamed_fBits;
   TString         MCEventHeader_TNamed_fName;
   TString         MCEventHeader_TNamed_fTitle;
   UInt_t          MCEventHeader_fRunId;
   UInt_t          MCEventHeader_fEventId;
   Double32_t      MCEventHeader_fX;
   Double32_t      MCEventHeader_fY;
   Double32_t      MCEventHeader_fZ;
   Double32_t      MCEventHeader_fT;
   Double32_t      MCEventHeader_fB;
   Int_t           MCEventHeader_fNPrim;
   Bool_t          MCEventHeader_fIsSet;
   Double32_t      MCEventHeader_fRotX;
   Double32_t      MCEventHeader_fRotY;
   Double32_t      MCEventHeader_fRotZ;
   Int_t           GeoTracks_;
   UInt_t          GeoTracks_fUniqueID[kMaxcbmroot_MCGeoTrack_GeoTracks];   //[cbmroot.MCGeoTrack.GeoTracks_]
   UInt_t          GeoTracks_fBits[kMaxcbmroot_MCGeoTrack_GeoTracks];   //[cbmroot.MCGeoTrack.GeoTracks_]
   UInt_t          GeoTracks_fGeoAtt[kMaxcbmroot_MCGeoTrack_GeoTracks];   //[cbmroot.MCGeoTrack.GeoTracks_]
   Short_t         GeoTracks_fLineColor[kMaxcbmroot_MCGeoTrack_GeoTracks];   //[cbmroot.MCGeoTrack.GeoTracks_]
   Short_t         GeoTracks_fLineStyle[kMaxcbmroot_MCGeoTrack_GeoTracks];   //[cbmroot.MCGeoTrack.GeoTracks_]
   Short_t         GeoTracks_fLineWidth[kMaxcbmroot_MCGeoTrack_GeoTracks];   //[cbmroot.MCGeoTrack.GeoTracks_]
   Short_t         GeoTracks_fMarkerColor[kMaxcbmroot_MCGeoTrack_GeoTracks];   //[cbmroot.MCGeoTrack.GeoTracks_]
   Short_t         GeoTracks_fMarkerStyle[kMaxcbmroot_MCGeoTrack_GeoTracks];   //[cbmroot.MCGeoTrack.GeoTracks_]
   Float_t         GeoTracks_fMarkerSize[kMaxcbmroot_MCGeoTrack_GeoTracks];   //[cbmroot.MCGeoTrack.GeoTracks_]
   Int_t           GeoTracks_fPDG[kMaxcbmroot_MCGeoTrack_GeoTracks];   //[cbmroot.MCGeoTrack.GeoTracks_]
   Int_t           GeoTracks_fId[kMaxcbmroot_MCGeoTrack_GeoTracks];   //[cbmroot.MCGeoTrack.GeoTracks_]
   Int_t           GeoTracks_fPointsSize[kMaxcbmroot_MCGeoTrack_GeoTracks];   //[cbmroot.MCGeoTrack.GeoTracks_]
   Int_t           GeoTracks_fNpoints[kMaxcbmroot_MCGeoTrack_GeoTracks];   //[cbmroot.MCGeoTrack.GeoTracks_]
   Double_t       *GeoTracks_fPoints[kMaxcbmroot_MCGeoTrack_GeoTracks];   //[cbmroot_MCGeoTrack_GeoTracks_fNpoints]

   // List of branches
   TBranch        *b_cbmroot_Stack_MCTrack_;   //!
   TBranch        *b_MCTrack_fUniqueID;   //!
   TBranch        *b_MCTrack_fBits;   //!
   TBranch        *b_MCTrack_fPdgCode;   //!
   TBranch        *b_MCTrack_fMotherId;   //!
   TBranch        *b_MCTrack_fPx;   //!
   TBranch        *b_MCTrack_fPy;   //!
   TBranch        *b_MCTrack_fPz;   //!
   TBranch        *b_MCTrack_fStartX;   //!
   TBranch        *b_MCTrack_fStartY;   //!
   TBranch        *b_MCTrack_fStartZ;   //!
   TBranch        *b_MCTrack_fStartT;   //!
   TBranch        *b_MCTrack_fNPoints;   //!
   TBranch        *b_MCTrack_fMass;   //!
   TBranch        *b_cbmroot_Tof_TOFPoint_;   //!
   TBranch        *b_TOFPoint_fUniqueID;   //!
   TBranch        *b_TOFPoint_fBits;   //!
   TBranch        *b_TOFPoint_fPersistanceCheck;   //!
   TBranch        *b_TOFPoint_fVerbose;   //!
   TBranch        *b_TOFPoint_fDefaultType;   //!
   TBranch        *b_TOFPoint_fTrackID;   //!
   TBranch        *b_TOFPoint_fEventId;   //!
   TBranch        *b_TOFPoint_fPx;   //!
   TBranch        *b_TOFPoint_fPy;   //!
   TBranch        *b_TOFPoint_fPz;   //!
   TBranch        *b_TOFPoint_fTime;   //!
   TBranch        *b_TOFPoint_fLength;   //!
   TBranch        *b_TOFPoint_fELoss;   //!
   TBranch        *b_TOFPoint_fDetectorID;   //!
   TBranch        *b_TOFPoint_fX;   //!
   TBranch        *b_TOFPoint_fY;   //!
   TBranch        *b_TOFPoint_fZ;   //!
   TBranch        *b_TOFPoint_fX_out;   //!
   TBranch        *b_TOFPoint_fY_out;   //!
   TBranch        *b_TOFPoint_fZ_out;   //!
   TBranch        *b_TOFPoint_fPx_out;   //!
   TBranch        *b_TOFPoint_fPy_out;   //!
   TBranch        *b_TOFPoint_fPz_out;   //!
   TBranch        *b_cbmroot_Gfi_GFIPoint_;   //!
   TBranch        *b_GFIPoint_fUniqueID;   //!
   TBranch        *b_GFIPoint_fBits;   //!
   TBranch        *b_GFIPoint_fPersistanceCheck;   //!
   TBranch        *b_GFIPoint_fVerbose;   //!
   TBranch        *b_GFIPoint_fDefaultType;   //!
   TBranch        *b_GFIPoint_fTrackID;   //!
   TBranch        *b_GFIPoint_fEventId;   //!
   TBranch        *b_GFIPoint_fPx;   //!
   TBranch        *b_GFIPoint_fPy;   //!
   TBranch        *b_GFIPoint_fPz;   //!
   TBranch        *b_GFIPoint_fTime;   //!
   TBranch        *b_GFIPoint_fLength;   //!
   TBranch        *b_GFIPoint_fELoss;   //!
   TBranch        *b_GFIPoint_fDetectorID;   //!
   TBranch        *b_GFIPoint_fX;   //!
   TBranch        *b_GFIPoint_fY;   //!
   TBranch        *b_GFIPoint_fZ;   //!
   TBranch        *b_GFIPoint_fX_out;   //!
   TBranch        *b_GFIPoint_fY_out;   //!
   TBranch        *b_GFIPoint_fZ_out;   //!
   TBranch        *b_GFIPoint_fPx_out;   //!
   TBranch        *b_GFIPoint_fPy_out;   //!
   TBranch        *b_GFIPoint_fPz_out;   //!
   TBranch        *b_GFIPoint_fModule;   //!
   TBranch        *b_cbmroot_Land_LandPoint_;   //!
   TBranch        *b_LandPoint_fUniqueID;   //!
   TBranch        *b_LandPoint_fBits;   //!
   TBranch        *b_LandPoint_fPersistanceCheck;   //!
   TBranch        *b_LandPoint_fVerbose;   //!
   TBranch        *b_LandPoint_fDefaultType;   //!
   TBranch        *b_LandPoint_fTrackID;   //!
   TBranch        *b_LandPoint_fEventId;   //!
   TBranch        *b_LandPoint_fPx;   //!
   TBranch        *b_LandPoint_fPy;   //!
   TBranch        *b_LandPoint_fPz;   //!
   TBranch        *b_LandPoint_fTime;   //!
   TBranch        *b_LandPoint_fLength;   //!
   TBranch        *b_LandPoint_fELoss;   //!
   TBranch        *b_LandPoint_fDetectorID;   //!
   TBranch        *b_LandPoint_fX;   //!
   TBranch        *b_LandPoint_fY;   //!
   TBranch        *b_LandPoint_fZ;   //!
   TBranch        *b_LandPoint_fX_out;   //!
   TBranch        *b_LandPoint_fY_out;   //!
   TBranch        *b_LandPoint_fZ_out;   //!
   TBranch        *b_LandPoint_fPx_out;   //!
   TBranch        *b_LandPoint_fPy_out;   //!
   TBranch        *b_LandPoint_fPz_out;   //!
   TBranch        *b_LandPoint_fSector;   //!
   TBranch        *b_LandPoint_fPaddleNb;   //!
   TBranch        *b_LandPoint_fPaddleTyp;   //!
   TBranch        *b_LandPoint_fLightYield;   //!
   TBranch        *b_cbmroot_Land_LandFirstHits_;   //!
   TBranch        *b_LandFirstHits_fUniqueID;   //!
   TBranch        *b_LandFirstHits_fBits;   //!
   TBranch        *b_LandFirstHits_fx0;   //!
   TBranch        *b_LandFirstHits_fy0;   //!
   TBranch        *b_LandFirstHits_fz0;   //!
   TBranch        *b_LandFirstHits_fT0;   //!
   TBranch        *b_LandFirstHits_fx1;   //!
   TBranch        *b_LandFirstHits_fy1;   //!
   TBranch        *b_LandFirstHits_fz1;   //!
   TBranch        *b_LandFirstHits_fT1;   //!
   TBranch        *b_LandFirstHits_fx2;   //!
   TBranch        *b_LandFirstHits_fy2;   //!
   TBranch        *b_LandFirstHits_fz2;   //!
   TBranch        *b_LandFirstHits_fT2;   //!
   TBranch        *b_LandFirstHits_fx3;   //!
   TBranch        *b_LandFirstHits_fy3;   //!
   TBranch        *b_LandFirstHits_fz3;   //!
   TBranch        *b_LandFirstHits_fT3;   //!
   TBranch        *b_LandFirstHits_fx4;   //!
   TBranch        *b_LandFirstHits_fy4;   //!
   TBranch        *b_LandFirstHits_fz4;   //!
   TBranch        *b_LandFirstHits_fT4;   //!
   TBranch        *b_LandFirstHits_fx5;   //!
   TBranch        *b_LandFirstHits_fy5;   //!
   TBranch        *b_LandFirstHits_fz5;   //!
   TBranch        *b_LandFirstHits_fT5;   //!
   TBranch        *b_cbmroot_Event_MCEventHeader_;   //!
   TBranch        *b_MCEventHeader_cbmroot_Event_MCEventHeader_TNamed_fUniqueID;   //!
   TBranch        *b_MCEventHeader_cbmroot_Event_MCEventHeader_TNamed_fBits;   //!
   TBranch        *b_MCEventHeader_cbmroot_Event_MCEventHeader_TNamed_fName;   //!
   TBranch        *b_MCEventHeader_cbmroot_Event_MCEventHeader_TNamed_fTitle;   //!
   TBranch        *b_MCEventHeader_cbmroot_Event_MCEventHeader_fRunId;   //!
   TBranch        *b_MCEventHeader_cbmroot_Event_MCEventHeader_fEventId;   //!
   TBranch        *b_MCEventHeader_cbmroot_Event_MCEventHeader_fX;   //!
   TBranch        *b_MCEventHeader_cbmroot_Event_MCEventHeader_fY;   //!
   TBranch        *b_MCEventHeader_cbmroot_Event_MCEventHeader_fZ;   //!
   TBranch        *b_MCEventHeader_cbmroot_Event_MCEventHeader_fT;   //!
   TBranch        *b_MCEventHeader_cbmroot_Event_MCEventHeader_fB;   //!
   TBranch        *b_MCEventHeader_cbmroot_Event_MCEventHeader_fNPrim;   //!
   TBranch        *b_MCEventHeader_cbmroot_Event_MCEventHeader_fIsSet;   //!
   TBranch        *b_MCEventHeader_cbmroot_Event_MCEventHeader_fRotX;   //!
   TBranch        *b_MCEventHeader_cbmroot_Event_MCEventHeader_fRotY;   //!
   TBranch        *b_MCEventHeader_cbmroot_Event_MCEventHeader_fRotZ;   //!
   TBranch        *b_cbmroot_MCGeoTrack_GeoTracks_;   //!
   TBranch        *b_GeoTracks_fUniqueID;   //!
   TBranch        *b_GeoTracks_fBits;   //!
   TBranch        *b_GeoTracks_fGeoAtt;   //!
   TBranch        *b_GeoTracks_fLineColor;   //!
   TBranch        *b_GeoTracks_fLineStyle;   //!
   TBranch        *b_GeoTracks_fLineWidth;   //!
   TBranch        *b_GeoTracks_fMarkerColor;   //!
   TBranch        *b_GeoTracks_fMarkerStyle;   //!
   TBranch        *b_GeoTracks_fMarkerSize;   //!
   TBranch        *b_GeoTracks_fPDG;   //!
   TBranch        *b_GeoTracks_fId;   //!
   TBranch        *b_GeoTracks_fPointsSize;   //!
   TBranch        *b_GeoTracks_fNpoints;   //!
   TBranch        *b_GeoTracks_fPoints;   //!

   cbmsim(TTree *tree=0);
   virtual ~cbmsim();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);

   virtual void     InitTree(); //miki
   virtual Int_t    IsInNeuLAND(Double32_t x, Double32_t y, Double32_t z); //miki
   virtual Int_t    IsInNeuLANDPlane(Int_t iPlane, Double32_t x, Double32_t y, Double32_t z); //miki


  //miki++
  // #######################   GFI   #######################
  static const Int_t GFIMaxHits=100;
  Int_t GFIn            ; //Number of Hits
  Int_t GFIp[GFIMaxHits]; //Particle ID

  // ####################### NeuLAND #######################
  static const Int_t nBar=50;
  //  //S438 apr14, 2014
  //  static const Int_t nPlane=2;
  //  static const Int_t nBarAll=100;
  //  static const Double32_t xcNeuLAND =   43.0; 
  //  static const Double32_t ycNeuLAND =    0.0; 
  //  static const Double32_t zcNeuLAND =  917.6; 
  //  static const Double32_t dxNeuLAND =  125.0; 
  //  static const Double32_t dyNeuLAND =  125.0; 
  //  static const Double32_t dzNeuLAND =   10.0; 
  //S438 sep14, 2014
  static const Int_t nPlane=8;
  static const Int_t nBarAll=400;
  static const Double32_t xcNeuLAND =    0.0; 
  static const Double32_t ycNeuLAND =    0.0; 
  static const Double32_t zcNeuLAND = 1320.0; 
  static const Double32_t dxNeuLAND =  125.0; 
  static const Double32_t dyNeuLAND =  125.0; 
  static const Double32_t dzNeuLAND =   20.0; 

  static const Double32_t dxNeuLANDPlane =  dxNeuLAND; 
  static const Double32_t dyNeuLANDPlane =  dyNeuLAND; 
  static const Double32_t dzNeuLANDPlane =  2.5; 

  static const Double_t dEthr=0.001; // [GeV]
  Double_t de[nBarAll+1];       // total energy loss in one bar in one event
  //  Double_t ELossMax[nBarAll+1]; // Maximum one-particle energy loss in one bar in one event
  Double_t TOFmin[nBarAll+1]; // minimum TOF in one bar in one event
  Double_t Xmin[nBarAll+1];   // X for minimum TOF hit
  Double_t Ymin[nBarAll+1];   // Y for minimum TOF hit
  Double_t Zmin[nBarAll+1];   // Z for minimum TOF hit
  Double_t XReact[nBarAll+1];   // Reaction point X for minimum TOF hit
  Double_t YReact[nBarAll+1];   // Reaction point Y for minimum TOF hit
  Double_t ZReact[nBarAll+1];   // Reaction point Z for minimum TOF hit
  Int_t    PIDTmin[nBarAll+1]; // Particle ID for the particle with shortest TOF
  //  Int_t    PIDEmax[nBarAll+1]; // Particle ID for the particle with largest energy loss
  TFile    *fLand;  // output rootfile pointer
  TTree    *tLand;
  Int_t    Event;
  Int_t    Nnpn[nPlane+1]        ; //number of hit bars in one plane
  Int_t    Nnpi[nPlane+1][nBar+1]; //hit bar#
  Double_t Nnpt[nPlane+1][nBar+1]; //TOF
  Double_t Nnpe[nPlane+1][nBar+1]; //dE
  Double_t Nnpx[nPlane+1][nBar+1]; //x
  Double_t Nnpy[nPlane+1][nBar+1]; //y
  Double_t Nnpz[nPlane+1][nBar+1]; //z
  Double_t NnpxReact[nPlane+1][nBar+1]; //x reaction point
  Double_t NnpyReact[nPlane+1][nBar+1]; //y reaction point
  Double_t NnpzReact[nPlane+1][nBar+1]; //z reaction point
  Int_t Nnpp[nPlane+1][nBar+1]; //Particle ID
  //miki--
};

#endif

#ifdef cbmsim_cxx
cbmsim::cbmsim(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("r3bsim.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("r3bsim.root");
      }
      f->GetObject("cbmsim",tree);

   }
   Init(tree);
}

cbmsim::~cbmsim()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t cbmsim::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t cbmsim::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void cbmsim::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   MCEventHeader_ = 0;
   // Set array pointer
   for(int i=0; i<kMaxcbmroot_MCGeoTrack_GeoTracks; ++i) GeoTracks_fPoints[i] = 0;

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("MCTrack", &MCTrack_, &b_cbmroot_Stack_MCTrack_);
   fChain->SetBranchAddress("MCTrack.fUniqueID", MCTrack_fUniqueID, &b_MCTrack_fUniqueID);
   fChain->SetBranchAddress("MCTrack.fBits", MCTrack_fBits, &b_MCTrack_fBits);
   fChain->SetBranchAddress("MCTrack.fPdgCode", MCTrack_fPdgCode, &b_MCTrack_fPdgCode);
   fChain->SetBranchAddress("MCTrack.fMotherId", MCTrack_fMotherId, &b_MCTrack_fMotherId);
   fChain->SetBranchAddress("MCTrack.fPx", MCTrack_fPx, &b_MCTrack_fPx);
   fChain->SetBranchAddress("MCTrack.fPy", MCTrack_fPy, &b_MCTrack_fPy);
   fChain->SetBranchAddress("MCTrack.fPz", MCTrack_fPz, &b_MCTrack_fPz);
   fChain->SetBranchAddress("MCTrack.fStartX", MCTrack_fStartX, &b_MCTrack_fStartX);
   fChain->SetBranchAddress("MCTrack.fStartY", MCTrack_fStartY, &b_MCTrack_fStartY);
   fChain->SetBranchAddress("MCTrack.fStartZ", MCTrack_fStartZ, &b_MCTrack_fStartZ);
   fChain->SetBranchAddress("MCTrack.fStartT", MCTrack_fStartT, &b_MCTrack_fStartT);
   fChain->SetBranchAddress("MCTrack.fNPoints", MCTrack_fNPoints, &b_MCTrack_fNPoints);
   fChain->SetBranchAddress("MCTrack.fMass", MCTrack_fMass, &b_MCTrack_fMass);
   fChain->SetBranchAddress("TOFPoint", &TOFPoint_, &b_cbmroot_Tof_TOFPoint_);
   fChain->SetBranchAddress("TOFPoint.fUniqueID", TOFPoint_fUniqueID, &b_TOFPoint_fUniqueID);
   fChain->SetBranchAddress("TOFPoint.fBits", TOFPoint_fBits, &b_TOFPoint_fBits);
   fChain->SetBranchAddress("TOFPoint.fPersistanceCheck", TOFPoint_fPersistanceCheck, &b_TOFPoint_fPersistanceCheck);
   fChain->SetBranchAddress("TOFPoint.fVerbose", TOFPoint_fVerbose, &b_TOFPoint_fVerbose);
   fChain->SetBranchAddress("TOFPoint.fDefaultType", TOFPoint_fDefaultType, &b_TOFPoint_fDefaultType);
   fChain->SetBranchAddress("TOFPoint.fTrackID", TOFPoint_fTrackID, &b_TOFPoint_fTrackID);
   fChain->SetBranchAddress("TOFPoint.fEventId", TOFPoint_fEventId, &b_TOFPoint_fEventId);
   fChain->SetBranchAddress("TOFPoint.fPx", TOFPoint_fPx, &b_TOFPoint_fPx);
   fChain->SetBranchAddress("TOFPoint.fPy", TOFPoint_fPy, &b_TOFPoint_fPy);
   fChain->SetBranchAddress("TOFPoint.fPz", TOFPoint_fPz, &b_TOFPoint_fPz);
   fChain->SetBranchAddress("TOFPoint.fTime", TOFPoint_fTime, &b_TOFPoint_fTime);
   fChain->SetBranchAddress("TOFPoint.fLength", TOFPoint_fLength, &b_TOFPoint_fLength);
   fChain->SetBranchAddress("TOFPoint.fELoss", TOFPoint_fELoss, &b_TOFPoint_fELoss);
   fChain->SetBranchAddress("TOFPoint.fDetectorID", TOFPoint_fDetectorID, &b_TOFPoint_fDetectorID);
   fChain->SetBranchAddress("TOFPoint.fX", TOFPoint_fX, &b_TOFPoint_fX);
   fChain->SetBranchAddress("TOFPoint.fY", TOFPoint_fY, &b_TOFPoint_fY);
   fChain->SetBranchAddress("TOFPoint.fZ", TOFPoint_fZ, &b_TOFPoint_fZ);
   fChain->SetBranchAddress("TOFPoint.fX_out", TOFPoint_fX_out, &b_TOFPoint_fX_out);
   fChain->SetBranchAddress("TOFPoint.fY_out", TOFPoint_fY_out, &b_TOFPoint_fY_out);
   fChain->SetBranchAddress("TOFPoint.fZ_out", TOFPoint_fZ_out, &b_TOFPoint_fZ_out);
   fChain->SetBranchAddress("TOFPoint.fPx_out", TOFPoint_fPx_out, &b_TOFPoint_fPx_out);
   fChain->SetBranchAddress("TOFPoint.fPy_out", TOFPoint_fPy_out, &b_TOFPoint_fPy_out);
   fChain->SetBranchAddress("TOFPoint.fPz_out", TOFPoint_fPz_out, &b_TOFPoint_fPz_out);
   fChain->SetBranchAddress("GFIPoint", &GFIPoint_, &b_cbmroot_Gfi_GFIPoint_);
   fChain->SetBranchAddress("GFIPoint.fUniqueID", GFIPoint_fUniqueID, &b_GFIPoint_fUniqueID);
   fChain->SetBranchAddress("GFIPoint.fBits", GFIPoint_fBits, &b_GFIPoint_fBits);
   fChain->SetBranchAddress("GFIPoint.fPersistanceCheck", GFIPoint_fPersistanceCheck, &b_GFIPoint_fPersistanceCheck);
   fChain->SetBranchAddress("GFIPoint.fVerbose", GFIPoint_fVerbose, &b_GFIPoint_fVerbose);
   fChain->SetBranchAddress("GFIPoint.fDefaultType", GFIPoint_fDefaultType, &b_GFIPoint_fDefaultType);
   fChain->SetBranchAddress("GFIPoint.fTrackID", GFIPoint_fTrackID, &b_GFIPoint_fTrackID);
   fChain->SetBranchAddress("GFIPoint.fEventId", GFIPoint_fEventId, &b_GFIPoint_fEventId);
   fChain->SetBranchAddress("GFIPoint.fPx", GFIPoint_fPx, &b_GFIPoint_fPx);
   fChain->SetBranchAddress("GFIPoint.fPy", GFIPoint_fPy, &b_GFIPoint_fPy);
   fChain->SetBranchAddress("GFIPoint.fPz", GFIPoint_fPz, &b_GFIPoint_fPz);
   fChain->SetBranchAddress("GFIPoint.fTime", GFIPoint_fTime, &b_GFIPoint_fTime);
   fChain->SetBranchAddress("GFIPoint.fLength", GFIPoint_fLength, &b_GFIPoint_fLength);
   fChain->SetBranchAddress("GFIPoint.fELoss", GFIPoint_fELoss, &b_GFIPoint_fELoss);
   fChain->SetBranchAddress("GFIPoint.fDetectorID", GFIPoint_fDetectorID, &b_GFIPoint_fDetectorID);
   fChain->SetBranchAddress("GFIPoint.fX", GFIPoint_fX, &b_GFIPoint_fX);
   fChain->SetBranchAddress("GFIPoint.fY", GFIPoint_fY, &b_GFIPoint_fY);
   fChain->SetBranchAddress("GFIPoint.fZ", GFIPoint_fZ, &b_GFIPoint_fZ);
   fChain->SetBranchAddress("GFIPoint.fX_out", GFIPoint_fX_out, &b_GFIPoint_fX_out);
   fChain->SetBranchAddress("GFIPoint.fY_out", GFIPoint_fY_out, &b_GFIPoint_fY_out);
   fChain->SetBranchAddress("GFIPoint.fZ_out", GFIPoint_fZ_out, &b_GFIPoint_fZ_out);
   fChain->SetBranchAddress("GFIPoint.fPx_out", GFIPoint_fPx_out, &b_GFIPoint_fPx_out);
   fChain->SetBranchAddress("GFIPoint.fPy_out", GFIPoint_fPy_out, &b_GFIPoint_fPy_out);
   fChain->SetBranchAddress("GFIPoint.fPz_out", GFIPoint_fPz_out, &b_GFIPoint_fPz_out);
   fChain->SetBranchAddress("GFIPoint.fModule", GFIPoint_fModule, &b_GFIPoint_fModule);
   fChain->SetBranchAddress("LandPoint", &LandPoint_, &b_cbmroot_Land_LandPoint_);
   fChain->SetBranchAddress("LandPoint.fUniqueID", LandPoint_fUniqueID, &b_LandPoint_fUniqueID);
   fChain->SetBranchAddress("LandPoint.fBits", LandPoint_fBits, &b_LandPoint_fBits);
   fChain->SetBranchAddress("LandPoint.fPersistanceCheck", LandPoint_fPersistanceCheck, &b_LandPoint_fPersistanceCheck);
   fChain->SetBranchAddress("LandPoint.fVerbose", LandPoint_fVerbose, &b_LandPoint_fVerbose);
   fChain->SetBranchAddress("LandPoint.fDefaultType", LandPoint_fDefaultType, &b_LandPoint_fDefaultType);
   fChain->SetBranchAddress("LandPoint.fTrackID", LandPoint_fTrackID, &b_LandPoint_fTrackID);
   fChain->SetBranchAddress("LandPoint.fEventId", LandPoint_fEventId, &b_LandPoint_fEventId);
   fChain->SetBranchAddress("LandPoint.fPx", LandPoint_fPx, &b_LandPoint_fPx);
   fChain->SetBranchAddress("LandPoint.fPy", LandPoint_fPy, &b_LandPoint_fPy);
   fChain->SetBranchAddress("LandPoint.fPz", LandPoint_fPz, &b_LandPoint_fPz);
   fChain->SetBranchAddress("LandPoint.fTime", LandPoint_fTime, &b_LandPoint_fTime);
   fChain->SetBranchAddress("LandPoint.fLength", LandPoint_fLength, &b_LandPoint_fLength);
   fChain->SetBranchAddress("LandPoint.fELoss", LandPoint_fELoss, &b_LandPoint_fELoss);
   fChain->SetBranchAddress("LandPoint.fDetectorID", LandPoint_fDetectorID, &b_LandPoint_fDetectorID);
   fChain->SetBranchAddress("LandPoint.fX", LandPoint_fX, &b_LandPoint_fX);
   fChain->SetBranchAddress("LandPoint.fY", LandPoint_fY, &b_LandPoint_fY);
   fChain->SetBranchAddress("LandPoint.fZ", LandPoint_fZ, &b_LandPoint_fZ);
   fChain->SetBranchAddress("LandPoint.fX_out", LandPoint_fX_out, &b_LandPoint_fX_out);
   fChain->SetBranchAddress("LandPoint.fY_out", LandPoint_fY_out, &b_LandPoint_fY_out);
   fChain->SetBranchAddress("LandPoint.fZ_out", LandPoint_fZ_out, &b_LandPoint_fZ_out);
   fChain->SetBranchAddress("LandPoint.fPx_out", LandPoint_fPx_out, &b_LandPoint_fPx_out);
   fChain->SetBranchAddress("LandPoint.fPy_out", LandPoint_fPy_out, &b_LandPoint_fPy_out);
   fChain->SetBranchAddress("LandPoint.fPz_out", LandPoint_fPz_out, &b_LandPoint_fPz_out);
   fChain->SetBranchAddress("LandPoint.fSector", LandPoint_fSector, &b_LandPoint_fSector);
   fChain->SetBranchAddress("LandPoint.fPaddleNb", LandPoint_fPaddleNb, &b_LandPoint_fPaddleNb);
   fChain->SetBranchAddress("LandPoint.fPaddleTyp", LandPoint_fPaddleTyp, &b_LandPoint_fPaddleTyp);
   fChain->SetBranchAddress("LandPoint.fLightYield", LandPoint_fLightYield, &b_LandPoint_fLightYield);
   fChain->SetBranchAddress("LandFirstHits", &LandFirstHits_, &b_cbmroot_Land_LandFirstHits_);
   fChain->SetBranchAddress("LandFirstHits.fUniqueID", LandFirstHits_fUniqueID, &b_LandFirstHits_fUniqueID);
   fChain->SetBranchAddress("LandFirstHits.fBits", LandFirstHits_fBits, &b_LandFirstHits_fBits);
   fChain->SetBranchAddress("LandFirstHits.fx0", LandFirstHits_fx0, &b_LandFirstHits_fx0);
   fChain->SetBranchAddress("LandFirstHits.fy0", LandFirstHits_fy0, &b_LandFirstHits_fy0);
   fChain->SetBranchAddress("LandFirstHits.fz0", LandFirstHits_fz0, &b_LandFirstHits_fz0);
   fChain->SetBranchAddress("LandFirstHits.fT0", LandFirstHits_fT0, &b_LandFirstHits_fT0);
   fChain->SetBranchAddress("LandFirstHits.fx1", LandFirstHits_fx1, &b_LandFirstHits_fx1);
   fChain->SetBranchAddress("LandFirstHits.fy1", LandFirstHits_fy1, &b_LandFirstHits_fy1);
   fChain->SetBranchAddress("LandFirstHits.fz1", LandFirstHits_fz1, &b_LandFirstHits_fz1);
   fChain->SetBranchAddress("LandFirstHits.fT1", LandFirstHits_fT1, &b_LandFirstHits_fT1);
   fChain->SetBranchAddress("LandFirstHits.fx2", LandFirstHits_fx2, &b_LandFirstHits_fx2);
   fChain->SetBranchAddress("LandFirstHits.fy2", LandFirstHits_fy2, &b_LandFirstHits_fy2);
   fChain->SetBranchAddress("LandFirstHits.fz2", LandFirstHits_fz2, &b_LandFirstHits_fz2);
   fChain->SetBranchAddress("LandFirstHits.fT2", LandFirstHits_fT2, &b_LandFirstHits_fT2);
   fChain->SetBranchAddress("LandFirstHits.fx3", LandFirstHits_fx3, &b_LandFirstHits_fx3);
   fChain->SetBranchAddress("LandFirstHits.fy3", LandFirstHits_fy3, &b_LandFirstHits_fy3);
   fChain->SetBranchAddress("LandFirstHits.fz3", LandFirstHits_fz3, &b_LandFirstHits_fz3);
   fChain->SetBranchAddress("LandFirstHits.fT3", LandFirstHits_fT3, &b_LandFirstHits_fT3);
   fChain->SetBranchAddress("LandFirstHits.fx4", LandFirstHits_fx4, &b_LandFirstHits_fx4);
   fChain->SetBranchAddress("LandFirstHits.fy4", LandFirstHits_fy4, &b_LandFirstHits_fy4);
   fChain->SetBranchAddress("LandFirstHits.fz4", LandFirstHits_fz4, &b_LandFirstHits_fz4);
   fChain->SetBranchAddress("LandFirstHits.fT4", LandFirstHits_fT4, &b_LandFirstHits_fT4);
   fChain->SetBranchAddress("LandFirstHits.fx5", LandFirstHits_fx5, &b_LandFirstHits_fx5);
   fChain->SetBranchAddress("LandFirstHits.fy5", LandFirstHits_fy5, &b_LandFirstHits_fy5);
   fChain->SetBranchAddress("LandFirstHits.fz5", LandFirstHits_fz5, &b_LandFirstHits_fz5);
   fChain->SetBranchAddress("LandFirstHits.fT5", LandFirstHits_fT5, &b_LandFirstHits_fT5);
   fChain->SetBranchAddress("MCEventHeader.", &MCEventHeader_, &b_cbmroot_Event_MCEventHeader_);
   fChain->SetBranchAddress("MCEventHeader.TNamed.fUniqueID", &MCEventHeader_TNamed_fUniqueID, &b_MCEventHeader_cbmroot_Event_MCEventHeader_TNamed_fUniqueID);
   fChain->SetBranchAddress("MCEventHeader.TNamed.fBits", &MCEventHeader_TNamed_fBits, &b_MCEventHeader_cbmroot_Event_MCEventHeader_TNamed_fBits);
   fChain->SetBranchAddress("MCEventHeader.TNamed.fName", &MCEventHeader_TNamed_fName, &b_MCEventHeader_cbmroot_Event_MCEventHeader_TNamed_fName);
   fChain->SetBranchAddress("MCEventHeader.TNamed.fTitle", &MCEventHeader_TNamed_fTitle, &b_MCEventHeader_cbmroot_Event_MCEventHeader_TNamed_fTitle);
   fChain->SetBranchAddress("MCEventHeader.fRunId", &MCEventHeader_fRunId, &b_MCEventHeader_cbmroot_Event_MCEventHeader_fRunId);
   fChain->SetBranchAddress("MCEventHeader.fEventId", &MCEventHeader_fEventId, &b_MCEventHeader_cbmroot_Event_MCEventHeader_fEventId);
   fChain->SetBranchAddress("MCEventHeader.fX", &MCEventHeader_fX, &b_MCEventHeader_cbmroot_Event_MCEventHeader_fX);
   fChain->SetBranchAddress("MCEventHeader.fY", &MCEventHeader_fY, &b_MCEventHeader_cbmroot_Event_MCEventHeader_fY);
   fChain->SetBranchAddress("MCEventHeader.fZ", &MCEventHeader_fZ, &b_MCEventHeader_cbmroot_Event_MCEventHeader_fZ);
   fChain->SetBranchAddress("MCEventHeader.fT", &MCEventHeader_fT, &b_MCEventHeader_cbmroot_Event_MCEventHeader_fT);
   fChain->SetBranchAddress("MCEventHeader.fB", &MCEventHeader_fB, &b_MCEventHeader_cbmroot_Event_MCEventHeader_fB);
   fChain->SetBranchAddress("MCEventHeader.fNPrim", &MCEventHeader_fNPrim, &b_MCEventHeader_cbmroot_Event_MCEventHeader_fNPrim);
   fChain->SetBranchAddress("MCEventHeader.fIsSet", &MCEventHeader_fIsSet, &b_MCEventHeader_cbmroot_Event_MCEventHeader_fIsSet);
   fChain->SetBranchAddress("MCEventHeader.fRotX", &MCEventHeader_fRotX, &b_MCEventHeader_cbmroot_Event_MCEventHeader_fRotX);
   fChain->SetBranchAddress("MCEventHeader.fRotY", &MCEventHeader_fRotY, &b_MCEventHeader_cbmroot_Event_MCEventHeader_fRotY);
   fChain->SetBranchAddress("MCEventHeader.fRotZ", &MCEventHeader_fRotZ, &b_MCEventHeader_cbmroot_Event_MCEventHeader_fRotZ);
   fChain->SetBranchAddress("GeoTracks", &GeoTracks_, &b_cbmroot_MCGeoTrack_GeoTracks_);
   fChain->SetBranchAddress("GeoTracks.fUniqueID", GeoTracks_fUniqueID, &b_GeoTracks_fUniqueID);
   fChain->SetBranchAddress("GeoTracks.fBits", GeoTracks_fBits, &b_GeoTracks_fBits);
   fChain->SetBranchAddress("GeoTracks.fGeoAtt", GeoTracks_fGeoAtt, &b_GeoTracks_fGeoAtt);
   fChain->SetBranchAddress("GeoTracks.fLineColor", GeoTracks_fLineColor, &b_GeoTracks_fLineColor);
   fChain->SetBranchAddress("GeoTracks.fLineStyle", GeoTracks_fLineStyle, &b_GeoTracks_fLineStyle);
   fChain->SetBranchAddress("GeoTracks.fLineWidth", GeoTracks_fLineWidth, &b_GeoTracks_fLineWidth);
   fChain->SetBranchAddress("GeoTracks.fMarkerColor", GeoTracks_fMarkerColor, &b_GeoTracks_fMarkerColor);
   fChain->SetBranchAddress("GeoTracks.fMarkerStyle", GeoTracks_fMarkerStyle, &b_GeoTracks_fMarkerStyle);
   fChain->SetBranchAddress("GeoTracks.fMarkerSize", GeoTracks_fMarkerSize, &b_GeoTracks_fMarkerSize);
   fChain->SetBranchAddress("GeoTracks.fPDG", GeoTracks_fPDG, &b_GeoTracks_fPDG);
   fChain->SetBranchAddress("GeoTracks.fId", GeoTracks_fId, &b_GeoTracks_fId);
   fChain->SetBranchAddress("GeoTracks.fPointsSize", GeoTracks_fPointsSize, &b_GeoTracks_fPointsSize);
   fChain->SetBranchAddress("GeoTracks.fNpoints", GeoTracks_fNpoints, &b_GeoTracks_fNpoints);
   fChain->SetBranchAddress("GeoTracks.fPoints", GeoTracks_fPoints, &b_GeoTracks_fPoints);
   Notify();
}

Bool_t cbmsim::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void cbmsim::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t cbmsim::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef cbmsim_cxx
