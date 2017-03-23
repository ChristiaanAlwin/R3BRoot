#include "R3BNveto_segHitPar.h"

#include "FairParamList.h" // for FairParamList
#include "FairLogger.h"

R3BNveto_segHitPar::R3BNveto_segHitPar(const char* name, const char* title, const char* context, Bool_t own)
    : FairParGenericSet(name, title, context, own)
    , fParams(new TObjArray(NMODULEMAX))
{
}

R3BNveto_segHitPar::~R3BNveto_segHitPar()
{
    if (fParams)
    {
        delete fParams;
        fParams = NULL;
    }
}

void R3BNveto_segHitPar::putParams(FairParamList* list)
{
    LOG(INFO) << "R3BNveto_segHitPar::putParams() called" << FairLogger::endl;
    if (!list)
    {
        return;
    }
    list->addObject(GetName(), fParams);
}

Bool_t R3BNveto_segHitPar::getParams(FairParamList* list)
{
    LOG(INFO) << "R3BNveto_segHitPar::getParams() called" << FairLogger::endl;
    if (!list)
    {
        return kFALSE;
    }
    if (!list->fillObject(GetName(), fParams))
    {
        return kFALSE;
    }
    return kTRUE;
}

void R3BNveto_segHitPar::clear()
{
}

void R3BNveto_segHitPar::printParams()
{

    LOG(INFO) << " -----------  " << GetName() << " Nveto_seg Parameters -------------  " << FairLogger::endl;

    LOG(INFO) << " Number of Parameters " << fParams->GetEntries() << FairLogger::endl;
    for (Int_t i = 0; i < fParams->GetEntries(); i++)
    {
        R3BNveto_segHitModulePar* t_par = (R3BNveto_segHitModulePar*)fParams->At(i);
        LOG(INFO) << "----------------------------------------------------------------------" << FairLogger::endl;
        if (t_par)
        {
            t_par->printParams();
        }
    }
}

ClassImp(R3BNveto_segHitPar);