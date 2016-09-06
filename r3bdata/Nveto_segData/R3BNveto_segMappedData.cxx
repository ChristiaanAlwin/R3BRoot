
#include "R3BNveto_segMappedData.h"

R3BNveto_segMappedData::R3BNveto_segMappedData()
    : fPlane(0)
    , fPaddle(0)
    , fSide(0)
    , fIs17(kFALSE)
{
}

R3BNveto_segMappedData::R3BNveto_segMappedData(Int_t sam,
                                           Int_t gtb,
                                           Int_t tacAddr,
                                           Int_t cal,
                                           Int_t clock,
                                           Int_t tacData,
                                           Int_t qdcData,
                                           Int_t plane,
                                           Int_t paddle,
                                           Int_t side,
                                           Bool_t is17)
    : R3BNveto_segUnpackData(sam, gtb, tacAddr, 0, cal, clock, tacData, qdcData)
    , fPlane(plane)
    , fPaddle(paddle)
    , fSide(side)
    , fIs17(is17)
{
}

ClassImp(R3BNveto_segMappedData)
