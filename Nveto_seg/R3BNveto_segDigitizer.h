#ifndef R3BNVETO_SEGDIGITIZER_H
#define R3BNVETO_SEGDIGITIZER_H

#include <map>

#include "FairTask.h"

#include "R3BNveto_segPoint.h"
#include "R3BNveto_segDigi.h"
#include "R3BNveto_segGeoPar.h"

#include "Digitizin_segEngine.h"

class TClonesArray;
class TGeoNode;
class TH1F;
class TH2F;

/**
 * Nveto_seg digitizing finder task
 * @author Jan Mayer
 *
 * For each event, get the R3BNveto_segPoints and apply the detector response to create R3BNveto_segDigis.
 *   Input:  Branch Nveto_segPoints = TClonesArray("R3BNveto_segPoint")
 *           Stored Nveto_seg Geometry Parameter Nveto_segGeoPar
 *   Output: Branch Nveto_segDigis  = TClonesArray("R3BNveto_segDigi")
 *   Additional output: Some control histograms
 *
 * The idea is that once ROOT6 is used by default and ROOT5 is abandned, the Digitizin_seg Engine Component can be passed
 * to this task, e.g. R3BNveto_segDigitizer(myDigitizin_segEngine), where all necessary configuration can be done on the
 * engine, such that the task can keep a small footprint. Currently hardcoded.
 */

class R3BNveto_segDigitizer : public FairTask
{
  public:
    R3BNveto_segDigitizer();
    ~R3BNveto_segDigitizer();

  private:
    // Rule of three/five: If a class requires a user-defined destructor, a user-defined copy
    // constructor, or a user-defined copy assignment operator, it almost certainly requires all three
    // Here no copy and no move is allowed
    R3BNveto_segDigitizer(const R3BNveto_segDigitizer&);            // copy constructor
    R3BNveto_segDigitizer(R3BNveto_segDigitizer&&);                 // move constructor
    R3BNveto_segDigitizer& operator=(const R3BNveto_segDigitizer&); // copy assignment
    R3BNveto_segDigitizer& operator=(R3BNveto_segDigitizer&&);      // move assignment

  protected:
    // TODO: #ROOT6: Declare functions overrriding virtual functions overrride
    InitStatus Init();       // override
    void Finish();           // override
    void SetParContainers(); // override
  public:
    void Exec(Option_t*); // override

  private:
    void Reset();

    // TODO: #ROOT6 Replace raw pointers with std::unique_ptr?
    TClonesArray* fNveto_segPoints;
    TClonesArray* fNveto_segDigis;

    R3BNveto_segGeoPar* fNveto_segGeoPar;
    Nveto_seg::Digitizin_segEngine* fDigitizin_segEngine;

    TH1F* hPMl;
    TH1F* hPMr;
    TH1F* hMultOne;
    TH1F* hMultTwo;
    TH1F* hRLTimeToTrig;
    TH2F* hElossVSQDC;

  public:
    ClassDef(R3BNveto_segDigitizer, 1)
};

#endif // R3BNVETO_SEGDIGITIZER_H
