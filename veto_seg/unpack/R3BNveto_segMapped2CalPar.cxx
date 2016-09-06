// ----------------------------------------------------------------
// -----                 R3BNveto_segMapped2CalPar              -----
// -----             Created 04-04-2014 by D.Kresan           -----
// ----------------------------------------------------------------

#include "R3BNveto_segMapped2CalPar.h"
#include "R3BNveto_segMappedData.h"
#include "R3BEventHeader.h"
#include "R3BTCalPar.h"
#include "R3BTCalEngine.h"

#include "FairRootManager.h"
#include "FairRuntimeDb.h"
#include "FairRunIdGenerator.h"
#include "FairRtdbRun.h"
#include "FairLogger.h"

#include "TClonesArray.h"
#include "TH1F.h"
#include "TF1.h"

#include <iostream>
#include <stdlib.h>

using namespace std;

R3BNveto_segMapped2CalPar::R3BNveto_segMapped2CalPar()
    : fUpdateRate(1000000)
    , fMinStats(100000)
    , fTrigger(-1)
    , fNofPMTs(0)
    , fNEvents(0)
    , fCal_Par(NULL)
{
}

R3BNveto_segMapped2CalPar::R3BNveto_segMapped2CalPar(const char* name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , fUpdateRate(1000000)
    , fMinStats(100000)
    , fTrigger(-1)
    , fNofPMTs(0)
    , fNEvents(0)
    , fCal_Par(NULL)
{
}

R3BNveto_segMapped2CalPar::~R3BNveto_segMapped2CalPar()
{
    if (fCal_Par)
    {
        delete fCal_Par;
    }
    if (fEngine)
    {
        delete fEngine;
    }
}

InitStatus R3BNveto_segMapped2CalPar::Init()
{
    FairRootManager* rm = FairRootManager::Instance();
    if (!rm)
    {
        return kFATAL;
    }
    header = (R3BEventHeader*)rm->GetObject("R3BEventHeader");
    if (!header)
    {
        return kFATAL;
    }
    fHits = (TClonesArray*)rm->GetObject("Nveto_segMappedData");
    if (!fHits)
    {
        return kFATAL;
    }

    fCal_Par = (R3BTCalPar*)FairRuntimeDb::instance()->getContainer("veto_segTCalPar");
    fCal_Par->setChanged();

    fEngine = new R3BTCalEngine(fCal_Par, fMinStats);

    return kSUCCESS;
}

void R3BNveto_segMapped2CalPar::Exec(Option_t* option)
{
    if (fTrigger >= 0)
    {
        if (header->GetTrigger() != fTrigger)
        {
            return;
        }
    }

    Int_t nHits = fHits->GetEntries();
    if (nHits > (fNofPMTs / 2))
    {
        return;
    }

    R3BNveto_segMappedData* hit;
    Int_t iPlane;
    Int_t iPaddle;
    Int_t iSide;

    // Loop over mapped hits
    for (Int_t i = 0; i < nHits; i++)
    {
        hit = (R3BNveto_segMappedData*)fHits->At(i);
        if (!hit)
        {
            continue;
        }

        // Check bar ID
        iPlane = hit->GetPlane();
        iPaddle = hit->GetPaddle();
        iSide = hit->GetSide();

        // Fill TAC histogram
        fEngine->Fill(iPlane, iPaddle, iSide, hit->GetTacData());
        if(0 == hit->GetSam())
        {
            fEngine->Fill(iPlane, iPaddle, iSide + 2, hit->GetCal());
        }
    }

    // Increment events
    fNEvents += 1;
}

void R3BNveto_segMapped2CalPar::FinishEvent()
{
}

void R3BNveto_segMapped2CalPar::FinishTask()
{
    fEngine->CalculateParamTacquila();
}

ClassImp(R3BNveto_segMapped2CalPar)
