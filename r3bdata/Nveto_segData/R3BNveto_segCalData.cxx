// -----------------------------------------------------------------------
// -----                      R3BNveto_segCalData                      -----
// -----              Created 22-04-2014 by D.Kresan                 -----
// -----------------------------------------------------------------------

#include "R3BNveto_segCalData.h"

R3BNveto_segCalData::R3BNveto_segCalData()
    : fBarId(0)
    , fSide(0)
    , fTime(0.)
    , fQdc(0)
{
}

R3BNveto_segCalData::R3BNveto_segCalData(Int_t barId, Int_t side, Double_t time, Int_t qdc)
: fBarId(barId)
, fSide(side)
, fTime(time)
, fQdc(qdc)
{
}

R3BNveto_segCalData::~R3BNveto_segCalData()
{
}

ClassImp(R3BNveto_segCalData)
