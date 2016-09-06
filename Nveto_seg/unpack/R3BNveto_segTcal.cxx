
// ------------------------------------------------------------
// -----                  R3BNVeto_segTcal                  -----
// -----            Created 27-01-2015 by M.Heil          -----
// ------------------------------------------------------------


#include "R3BNveto_segTcal.h"

#include "R3BTCalEngine.h"
#include "R3BPaddle_segTamexMappedData.h"
#include "R3BNveto_segPmt.h"
#include "R3BTCalPar.h"
#include "R3BEventHeader.h"

#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "FairRootManager.h"
#include "FairLogger.h"

#include "TClonesArray.h"

R3BNveto_segTcal::R3BNveto_segTcal()
    : FairTask("veto_segTcal", 1)
    , fNEvents(0)
    , fMappedHit(NULL)
    , fPmt(new TClonesArray("R3BNveto_segPmt"))
    , fNPmt(0)
    , fTcalPar(NULL)
    , fTrigger(-1)
//    , fMap17Seen()
//    , fMapStopTime()
//    , fMapStopClock()
    , fClockFreq(1. / VFTX_CLOCK_MHZ * 1000.)
{
}

R3BNveto_segTcal::R3BNveto_segTcal(const char* name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , fNEvents(0)
    , fMappedHit(NULL)
    , fPmt(new TClonesArray("R3BNveto_segPmt"))
    , fNPmt(0)
    , fTcalPar(NULL)
    , fTrigger(-1)
//    , fMap17Seen()
//    , fMapStopTime()
//    , fMapStopClock()
    , fClockFreq(1. / VFTX_CLOCK_MHZ * 1000.)
{
}

R3BNveto_segTcal::~R3BNveto_segTcal()
{
    if (fPmt)
    {
        delete fPmt;
        fPmt = NULL;
        fNPmt = 0;
    }
}

InitStatus R3BNveto_segTcal::Init()
{
    LOG(INFO) << "R3BNveto_segTcal::Init : read " << fTcalPar->GetNumModulePar() << " calibrated modules"
              << FairLogger::endl;
    // fTcalPar->printParams();

    FairRootManager* mgr = FairRootManager::Instance();
    if (NULL == mgr)
    {
        FairLogger::GetLogger()->Fatal(MESSAGE_ORIGIN, "FairRootManager not found");
    }
/*
    header = (R3BEventHeader*)mgr->GetObject("R3BEventHeader");
    if (NULL == header)
    {
        FairLogger::GetLogger()->Fatal(MESSAGE_ORIGIN, "Branch R3BEventHeader not found");
    }
*/
    fMappedHit = (TClonesArray*)mgr->GetObject("Nveto_segTamexMappedData");
    if (NULL == fMappedHit)
    {
        FairLogger::GetLogger()->Fatal(MESSAGE_ORIGIN, "Branch R3BNveto_segTamexMappedData not found");
    }

    mgr->Register("Nveto_segPmt", "veto_seg", fPmt, kTRUE);

    return kSUCCESS;
}

void R3BNveto_segTcal::SetParContainers()
{
    FairRunAna* ana = FairRunAna::Instance();
    FairRuntimeDb* rtdb = ana->GetRuntimeDb();
    fTcalPar = (R3BTCalPar*)(rtdb->getContainer("veto_segTCalPar"));
}

InitStatus R3BNveto_segTcal::ReInit()
{
    SetParContainers();
    return kSUCCESS;
}

void R3BNveto_segTcal::Exec(Option_t*)
{
	/*
    if (fTrigger >= 0)
    {
        if (header->GetTrigger() != fTrigger)
        {
            return;
        }
    }
*/
    Int_t nHits = fMappedHit->GetEntriesFast();
/*
    if (nHits > (fNofPMTs / 2))
    {
        return;
    }
*/
    R3BPaddle_segTamexMappedData* hit;
    Int_t iPlane;
    Int_t iBar;
    Int_t iSide;
    Int_t channel;
    Int_t tdc;
    R3BTCalModulePar* par;
    Double_t timeLE;
    Double_t timeTE;

    for (Int_t ihit = 0; ihit < nHits; ihit++)
    {
       hit = (R3BPaddle_segTamexMappedData*)fMappedHit->At(ihit);
       if (NULL == hit)
       {
          continue;
       }
       iPlane = hit->GetPlaneId();
       iBar = hit->GetBarId();
       iSide = hit->GetSide();
       if (hit->Is17())
       {
           // 17-th channel
           continue;
       }

       // Convert TDC to [ns]
       if (! (par = fTcalPar->GetModuleParAt(iPlane, iBar, iSide)))
       {
           LOG(DEBUG) << "R3BNveto_segTcal::Exec : Tcal par not found, barId: " << iBar << ", side: " << iSide
                      << FairLogger::endl;
           continue;
       }
       

       tdc = hit->GetFineTimeLE();
       timeLE = par->GetTimeVFTX(tdc);

       if (! (par = fTcalPar->GetModuleParAt(iPlane, iBar, iSide + 2)))
       {
           LOG(DEBUG) << "R3BNveto_segTcal::Exec : Tcal par not found, barId: " << iBar << ", side: " << iSide
                      << FairLogger::endl;
           continue;
       }
      
       tdc = hit->GetFineTimeTE();
       timeTE = par->GetTimeVFTX(tdc);
       

/*            
       if (timeLE < -1000.)
       {
           continue;
       }
*/
       if (timeLE < 0. || timeLE > fClockFreq || timeTE < 0. || timeTE > fClockFreq)
       {
           LOG(ERROR) << "R3BNveto_segTcal::Exec : error in time calibration: ch= " << channel << ", tdc= " << tdc
                      << ", time leading edge = " << timeLE << ", time trailing edge = " << timeTE << FairLogger::endl;
           continue;
       }

       timeLE = fClockFreq-timeLE + hit->GetCoarseTimeLE() * fClockFreq;
       timeTE = fClockFreq-timeTE + hit->GetCoarseTimeTE() * fClockFreq;
       
       
       new ((*fPmt)[fNPmt]) R3BNveto_segPmt(iPlane, iBar, iSide, timeLE, timeTE-timeLE);
       fNPmt += 1;
       

    }
}

void R3BNveto_segTcal::FinishEvent()
{
    if (fVerbose && 0 == (fNEvents % 1))
    {
        LOG(INFO) << "R3BNveto_segTcal::Exec : event=" << fNEvents << " nPMTs=" << fNPmt << FairLogger::endl;
    }

    if (fPmt)
    {
        fPmt->Clear();
        fNPmt = 0;
    }

    fNEvents += 1;
}

void R3BNveto_segTcal::FinishTask()
{
}

ClassImp(R3BNveto_segTcal)
