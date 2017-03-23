
#include "R3BNveto_segMappedData.h"

R3BNveto_segMappedData::R3BNveto_segMappedData()
    : fClock(0)
    , fTacData(0)
    , fStopT(0)
    , fQdcData(0)
    , fPlane(0)
    , fPaddle(0)
    , fSide(0)
{
}

R3BNveto_segMappedData::R3BNveto_segMappedData(Int_t clock,
                                           Int_t tacData,
                                           Int_t stopT,
                                           Int_t qdcData,
                                           Int_t plane,
                                           Int_t paddle,
                                           Int_t side)
    : fClock(clock)
    , fTacData(tacData)
    , fStopT(stopT)
    , fQdcData(qdcData)
    , fPlane(plane)
    , fPaddle(paddle)
    , fSide(side)
{
}

ClassImp(R3BNveto_segMappedData)
