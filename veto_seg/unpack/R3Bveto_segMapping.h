// ---------------------------------------------------------------------------------------
// -----                                                                             -----
// -----                           R3Bveto_segMapping                                    -----
// -----                           Version 0.1                                       -----
// -----                      Created  @ 03.2014 by M.I. Cherciu                     -----
// -----                                                                             -----
// ---------------------------------------------------------------------------------------

#ifndef R3BVETO_SEG_MAPPING_H
#define R3BVETO_SEG_MAPPING_H

#include <map>

#include "FairTask.h"

class TClonesArray;

/// Class R3Bveto_segMapping allow to read mapping configuration from file and get identificators for each modules and channels.
class R3Bveto_segMapping : public FairTask
{

  public:
    R3Bveto_segMapping();

    virtual ~R3Bveto_segMapping();

    virtual InitStatus Init();

    virtual void Exec(Option_t* option);

    virtual void FinishEvent();

    void SetFileName(const char* A_FileName)
    {
        fname = A_FileName;
    }
    const char* GetFileName() const
    {
        return fname;
    }
    inline void SetNofBarsPerPlane(Int_t nBars)
    {
        fNofBarsPerPlane = nBars;
    }

  private:
    const char* fname;
    Int_t nMappedElements;
    Int_t fnEvents;
    Int_t fNofBarsPerPlane;
    TClonesArray* fRawData;
    TClonesArray* fveto_segHit;
    Int_t nEntry;
    std::map<Int_t, Int_t> v1map;
    std::map<Int_t, Int_t> v2map;
    std::map<Int_t, Int_t> v3map;
    Bool_t DoMapping();

  public:
    ClassDef(R3Bveto_segMapping, 0);
};

#endif
