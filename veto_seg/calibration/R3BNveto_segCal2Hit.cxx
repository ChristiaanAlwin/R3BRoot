// -----------------------------------------------------------------------------
// -----                          R3BNveto_segCal2Hit                        -----
// -----                   Created 07-05-2014 by D.Kresan                  -----
// -----------------------------------------------------------------------------

#include "TClonesArray.h"
#include "TMath.h"

#include "FairRootManager.h"
#include "FairLogger.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"

#include "R3BNveto_segCalData.h"
#include "R3Bveto_segDigi.h"
#include "R3BNveto_segCal2Hit.h"
#include "R3BNveto_segHitPar.h"
#include "R3BTCalPar.h"

#include "TH1F.h"
#include "TGraph2D.h"
#include "TCanvas.h"
#include "TH2F.h"

using std::ifstream;

R3BNveto_segCal2Hit::R3BNveto_segCal2Hit()
   : fveto_segPmt (NULL)
   , fveto_segDigi (new TClonesArray ("R3Bveto_segDigi"))
   , fNDigi (0)
   , fFirstPlaneHorisontal (kFALSE) {
}

R3BNveto_segCal2Hit::R3BNveto_segCal2Hit (const char* name, Int_t iVerbose)
   : FairTask (name, iVerbose)
   , fveto_segPmt (NULL)
   , fveto_segDigi (new TClonesArray ("R3Bveto_segDigi"))
   , fNDigi (0)
   , fFirstPlaneHorisontal (kFALSE) {
}

R3BNveto_segCal2Hit::~R3BNveto_segCal2Hit() {
}

InitStatus R3BNveto_segCal2Hit::Init() {
   FairRootManager* fMan = FairRootManager::Instance();
   if (!fMan) {
      FairLogger::GetLogger()->Fatal (MESSAGE_ORIGIN, "FairRootManager not found");
   }

   fveto_segPmt = (TClonesArray*) fMan->GetObject ("Nveto_segCalData");
   if (!fveto_segPmt) {
      FairLogger::GetLogger()->Fatal (MESSAGE_ORIGIN, "Nveto_segCalData not found");
   }

   fMan->Register ("veto_segDigi", "veto_seg", fveto_segDigi, kTRUE);
   
   fNEvent = 0;
    
   SetParameter();
   return kSUCCESS;
}

void R3BNveto_segCal2Hit::SetParContainers() {
   FairRuntimeDb* rtdb = FairRunAna::Instance()->GetRuntimeDb();
   fPar = (R3BNveto_segHitPar*) rtdb->getContainer ("Nveto_segHitPar");
}



void R3BNveto_segCal2Hit::SetParameter(){
   std::map<Int_t, Bool_t> tempMapIsSet;
   std::map<Int_t, Double_t> tempMapVeff;
   std::map<Int_t, Double_t> tempMapTSync;
   std::map<Int_t, Double_t> tempMapEGain;
   
   for (Int_t i = 0; i < fPar->GetNumModulePar(); i++) {
      R3BNveto_segHitModulePar* fModulePar = fPar->GetModuleParAt(i);
      Int_t id = fModulePar->GetModuleId() * 2 + fModulePar->GetSide() - 3;
      tempMapIsSet[id] = kTRUE;
      tempMapVeff[id] = fModulePar->GetEffectiveSpeed();
      tempMapTSync[id] = fModulePar->GetTimeOffset();
      tempMapEGain[id] = fModulePar->GetEnergieGain();
   }
     
   LOG(INFO) << "R3BNveto_segCal2Hit::SetParameter : Number of Parameters: " << fPar->GetNumModulePar() << FairLogger::endl;
   
   fMapIsSet = tempMapIsSet;
   fMapVeff = tempMapVeff;
   fMapTSync = tempMapTSync;
   fMapEGain = tempMapEGain;
}

InitStatus R3BNveto_segCal2Hit::ReInit() {
   SetParContainers();
   SetParameter();
   return kSUCCESS;
}

void R3BNveto_segCal2Hit::Exec (Option_t* option) {
   if(fVerbose != 0 && ++fNEvent % 100000 == 0)
      FairLogger::GetLogger()->Info(MESSAGE_ORIGIN, "R3BNveto_segCal2Hit::Exec : Event: %8d", fNEvent);
     
   Int_t nveto_segPmt = fveto_segPmt->GetEntriesFast();
  
   R3BNveto_segCalData* pmt1;
   R3BNveto_segCalData* pmt2;
   Int_t barId;
   Int_t plane;
   Double_t veff;
   Double_t tdcL, tdcR, tdc;
   Double_t qdcL, qdcR, qdc;
   Double_t x, y, z;
   Int_t id = 0;
     
   if (fFirstPlaneHorisontal) {
      id = 1;
   }
   for (Int_t i1 = 0; i1 < nveto_segPmt; i1++) {
      pmt1 = (R3BNveto_segCalData*) fveto_segPmt->At (i1);
      if (1 != pmt1->GetSide()) {
         continue;
      }
      barId = pmt1->GetBarId();
      if (! fMapIsSet[(barId-1)*2]) {
         continue;
      }

      for (Int_t i2 = 0; i2 < nveto_segPmt; i2++) {
         if (i1 == i2) {
            continue;
         }
         pmt2 = (R3BNveto_segCalData*) fveto_segPmt->At (i2);
         if (2 != pmt2->GetSide()) {
            continue;
         }
         if (pmt2->GetBarId() != barId) {
            continue;
         }

         qdcL = pmt1->GetQdc() * fMapEGain[pmt1->GetBarId() * 2 - 2];
         qdcR = pmt2->GetQdc() * fMapEGain[pmt1->GetBarId() * 2 - 1];
	 		 
         veff = fMapVeff[(barId-1)*2];
         tdcL = pmt1->GetTime() + fMapTSync[pmt1->GetBarId() * 2 - 2];
         tdcR = pmt2->GetTime() + fMapTSync[pmt2->GetBarId() * 2 - 1];
         tdc = (tdcL + tdcR) / 2.;
         qdc = TMath::Sqrt (qdcL * qdcR);
         plane = ((barId - 1) / 50) + 1;
         if (id == plane % 2) {
            x = veff * (tdcR - tdcL);
            y = (barId - 0.5 - (plane - 1) * 50) * 5. - 125.;
         }
         else {
            x = (barId - 0.5 - (plane - 1) * 50) * 5. - 125.;
            y = veff * (tdcR - tdcL);
         }
         z = (plane - 0.5) * 5. + fDistanceToTarget;
	 
	 new ( (*fveto_segDigi) [fNDigi]) R3Bveto_segDigi (barId, tdcL, tdcR, tdc, qdcL, qdcR, qdc, x, y, z);
	 fNDigi += 1;	 
      }
   }
}

void R3BNveto_segCal2Hit::FinishEvent() {
   fveto_segDigi->Clear();
   fNDigi = 0;
}

ClassImp (R3BNveto_segCal2Hit)
