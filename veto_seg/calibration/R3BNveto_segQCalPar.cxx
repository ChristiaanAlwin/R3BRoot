#include "R3BNveto_segQCalPar.h"

#include "FairParamList.h" // for FairParamList
#include "FairLogger.h"

R3BNveto_segQCalPar::R3BNveto_segQCalPar(const char* name, const char* title, const char* context, Bool_t own)
    : FairParGenericSet(name, title, context, own)
{
}

R3BNveto_segQCalPar::~R3BNveto_segQCalPar()
{
}

void R3BNveto_segQCalPar::putParams(FairParamList* list)
{
    LOG(INFO) << "R3BNveto_segQCalPar::putParams() called" << FairLogger::endl;
    if (!list)
    {
        return;
    }
    list->add("QCalParams", fParams);
}

Bool_t R3BNveto_segQCalPar::getParams(FairParamList* list)
{
    LOG(INFO) << "R3BNveto_segQCalPar::getParams() called" << FairLogger::endl;
    if (!list)
    {
        return kFALSE;
    }
    if (!list->fill("QCalParams", &fParams))
    {
        return kFALSE;
    }
    return kTRUE;
}

void R3BNveto_segQCalPar::SetSize(Int_t size)
{
  fParams.Set(size);
  fParams.Reset();
}

void R3BNveto_segQCalPar::printParams()
{

    LOG(INFO) << " -----------  " << GetName() << " Nveto_seg Parameters -------------  " << FairLogger::endl;
    LOG(INFO) << " Number of Parameters " << fParams.GetSize() << FairLogger::endl;
    for (Int_t i = 0; i < fParams.GetSize(); i++)
      LOG(INFO) << "PMT Nr. " << i << " : pedestal level = " << fParams.At(i) << FairLogger::endl;
    LOG(INFO) << "----------------------------------------------------------------------" << FairLogger::endl;
}

ClassImp(R3BNveto_segQCalPar);