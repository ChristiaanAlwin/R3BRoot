// -------------------------------------------------------------------------
// -----                      R3BPaddleCrystalHt_seg source file                  -----
// -------------------------------------------------------------------------

#include "R3BPaddleCrystalHt_seg.h"

#include <iostream>

using std::cout;
using std::endl;
using std::flush;



// -----   Default constructor   -------------------------------------------
R3BPaddleCrystalHt_seg::R3BPaddleCrystalHt_seg()
  : FairHit(),
    type(0), copy(0),
    fEnergy(0.), nbOfSteps(0),
    fTimeFirstStep(0.), fTimeLastStep(0.),
    nbOfPrimaries(0), fEnergyPrimary(0.),
    fThetaPrimary(0.), fPhiPrimary(0.),
    eventID(0), runID(0)
{
}



R3BPaddleCrystalHt_seg::R3BPaddleCrystalHt_seg(const R3BPaddleCrystalHt_seg& right)
  : FairHit(right),
    type(right.type), copy(right.copy),
    fEnergy(right.fEnergy), nbOfSteps(right.nbOfSteps),
    fTimeFirstStep(right.fTimeFirstStep), fTimeLastStep(right.fTimeLastStep),
    nbOfPrimaries(right.nbOfPrimaries), fEnergyPrimary(right.fEnergyPrimary),
    fThetaPrimary(right.fThetaPrimary), fPhiPrimary(right.fPhiPrimary),
    eventID(right.eventID), runID(right.runID)
{
}



// -----   Destructor   ----------------------------------------------------
R3BPaddleCrystalHt_seg::~R3BPaddleCrystalHt_seg()
{
}
// -------------------------------------------------------------------------



// -----   Public method Print   -------------------------------------------
void R3BPaddleCrystalHt_seg::Print(const Option_t* opt) const
{
  cout <<"==============================================" << endl;
  cout << "-I- R3BPaddleCrystalHt_seg:  Hit in detector: "
       <<  fDetectorID << endl;
  cout << "    Position (" << fX << ", " << fY << ", " << fZ
       << ") cm" << endl;
  
  cout << " type: " << type
       << ", copy: " << copy
       << endl;
  cout << " energy: " << fEnergy * 1000.  << " MeV"
       << ", nbOfSteps: " << nbOfSteps
       << endl;
  cout << " nbOfPrimaries: " << nbOfPrimaries
       << ", energyPrimary: " << fEnergyPrimary * 1000.  << " MeV"
       << ", thetaPrimary: " << fThetaPrimary   << " deg"
       << ", phiPrimary: " << fPhiPrimary   << " deg" << endl;
  cout << "----------------------------------------------" <<  endl;
}
// -------------------------------------------------------------------------



ClassImp(R3BPaddleCrystalHt_seg)
