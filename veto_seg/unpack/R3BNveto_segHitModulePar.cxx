#include "R3BNveto_segHitModulePar.h"

#include "FairParamList.h" // for FairParamList
#include "FairLogger.h"

using namespace std;

R3BNveto_segHitModulePar::R3BNveto_segHitModulePar (const char* name, const char* title, const char* context, Bool_t own)
   : FairParGenericSet (name, title, context, own)
   , fModuleId (0)
   , fSide (0) {
   // Reset all parameter_seg
   clear();
}

R3BNveto_segHitModulePar::~R3BNveto_segHitModulePar() {
}

void R3BNveto_segHitModulePar::putParams (FairParamList* list) {
   LOG (INFO) << "R3BNveto_segHitModulePar::putParams() called" << FairLogger::endl;
   if (!list) {
      return;
   }
   list->add ("module_id", fModuleId);
   list->add ("side", fSide);
   list->add ("timeoffset", fTimeOffset);
   list->add ("timeoffset_error", fTimeOffsetError);
   list->add ("energiegain", fEnergieGain);
   list->add ("energiegain_error", fEnergieGainError);
   list->add ("effectivespeed", fEffectiveSpeed);
   list->add ("effectivespeed_error", fEffectiveSpeedError);
}

Bool_t R3BNveto_segHitModulePar::getParams (FairParamList* list) {
   if (!list) {
      return kFALSE;
   }
   if (!list->fill ("module_id", &fModuleId)) {
      return kFALSE;
   }
   if (!list->fill ("side", &fSide)) {
      return kFALSE;
   }
   if (!list->fill ("timeoffset", &fTimeOffset)) {
      return kFALSE;
   }
   if (!list->fill ("timeoffset_error", &fTimeOffsetError)) {
      return kFALSE;
   }
   if (!list->fill ("energiegain", &fEnergieGain)) {
      return kFALSE;
   }
   if (!list->fill ("energiegain_error", &fEnergieGainError)) {
      return kFALSE;
   }
   if (!list->fill ("effectivespeed", &fEffectiveSpeed)) {
      return kFALSE;
   }
   if (!list->fill ("effectivespeed_error", &fEffectiveSpeedError)) {
      return kFALSE;
   }

   return kTRUE;
}

void R3BNveto_segHitModulePar::clear() {
}

void R3BNveto_segHitModulePar::printParams() {
   LOG (INFO) << "   R3BNveto_segHitModulePar: Calibration Parameters: " << FairLogger::endl;
   LOG (INFO) << "   fBarId: " << fModuleId << FairLogger::endl;
   LOG (INFO) << "   fSide: " << fSide << FairLogger::endl;
   FairLogger::GetLogger()->Info (MESSAGE_ORIGIN, "   fTimeOffset: %4.2f  ±   %4.2f", fTimeOffset, fTimeOffsetError);
   FairLogger::GetLogger()->Info (MESSAGE_ORIGIN, "   fEffectiveSpeed: %4.2f  ±   %4.2f", fEffectiveSpeed, fEffectiveSpeedError);
   FairLogger::GetLogger()->Info (MESSAGE_ORIGIN, "   fEnergieGain: %4.2f  ±   %4.2f", fEnergieGain, fEnergieGainError);
}

ClassImp (R3BNveto_segHitModulePar);
