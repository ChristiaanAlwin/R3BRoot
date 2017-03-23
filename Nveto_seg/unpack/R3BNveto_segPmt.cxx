// -----------------------------------------------------------------------
// -----                          R3BNveto_segPmt                         -----
// -----                Created 27-01-2016 by M.Heil               -----
// -----------------------------------------------------------------------

#include "R3BNveto_segPmt.h"

R3BNveto_segPmt::R3BNveto_segPmt()
    : fPlaneId(0)
    , fBarId(0)
    , fSide(0)
    , fTime(0.)
    , fCharge(0)
{
}

R3BNveto_segPmt::R3BNveto_segPmt(Int_t planeId, Int_t barId, Int_t side, Double_t time, Double_t charge)
: fPlaneId(planeId)
, fBarId(barId)
, fSide(side)
, fTime(time)
, fCharge(charge)
{
}

R3BNveto_segPmt::~R3BNveto_segPmt()
{
}

ClassImp(R3BNveto_segPmt)
