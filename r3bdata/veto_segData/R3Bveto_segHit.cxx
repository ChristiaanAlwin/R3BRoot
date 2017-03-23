// -------------------------------------------------------------------------
// -----                      R3Bveto_segPoint source file                  -----
// -------------------------------------------------------------------------

#include "R3Bveto_segHit.h"

#include <iostream>

using std::cout;
using std::endl;
using std::flush;



// -----   Default constructor   -------------------------------------------
R3Bveto_segHit::R3Bveto_segHit()
  : FairHit(),
    fEnergy(0.), fTof(0.), fSaturnPaddleNb(-1)
{
}



R3Bveto_segHit::R3Bveto_segHit(const R3Bveto_segHit& right)
  : FairHit(right),
    fEnergy(right.fEnergy), fTof(right.fTof),
    fSaturnPaddleNb(right.fSaturnPaddleNb)
{
}



// -----   Destructor   ----------------------------------------------------
R3Bveto_segHit::~R3Bveto_segHit()
{
}



// -----   Public method Print   -------------------------------------------
void R3Bveto_segHit::Print(const Option_t* opt) const
{
  cout << "-I- R3Bveto_segHit: VETO_SEG Hit in detector " << fDetectorID << endl;
  cout << "    Tof: " << fTof << " cm,  Energy loss "
       << fEnergy*1.0e06 << " keV" << endl;
}
// -------------------------------------------------------------------------



ClassImp(R3Bveto_segHit)
