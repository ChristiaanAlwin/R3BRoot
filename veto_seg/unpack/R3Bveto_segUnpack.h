// ---------------------------------------------------------------------------------------
// -----                                                                             -----
// -----                           R3Bveto_segUnpack                                     -----
// -----                           Version 0.1                                       -----
// -----         Adapted by M.I. Cherciu @ 01.2014 after Y.Gonzalez code             -----
// -----                                                                             -----
// -----        * modification @ 03.2014: added TClonesArray for R3Bveto_segHit          -----
// ---------------------------------------------------------------------------------------

#ifndef R3BVETO_SEGUNPACK_H
#define R3BVETO_SEGUNPACK_H

#include "FairUnpack.h"

class TClonesArray;
class TH1F;

class R3Bveto_segUnpack : public FairUnpack
{
  public:
    // Constructor
    R3Bveto_segUnpack(Short_t type = 94, Short_t subType = 9400, Short_t procId = 10, Short_t subCrate = 1, Short_t control = 3);

    // Destructor
    virtual ~R3Bveto_segUnpack();

    // Fair specific
    virtual Bool_t Init();
    virtual Bool_t DoUnpack(Int_t* data, Int_t size);
    virtual void Reset();

  protected:
    virtual void Register();

  private:
    TClonesArray* fRawData;
    Int_t fNHits;

  public:
    // Class definition
    ClassDef(R3Bveto_segUnpack, 0)
};

#endif
