// -----------------------------------------------------------------------
// -----                         R3BNveto_segPmt                       -----
// -----                Created 27-01-2016 by M.Heil                 -----
// -----------------------------------------------------------------------

#ifndef R3BNVETO_SEGPMT
#define R3BNVETO_SEGPMT

#include "TObject.h"

class R3BNveto_segPmt : public TObject
{
  public:
    R3BNveto_segPmt();
    R3BNveto_segPmt(Int_t planeId, Int_t barId, Int_t side, Double_t time, Double_t charge);
    virtual ~R3BNveto_segPmt();

    inline const Int_t& GetPlaneId() const
    {
        return fPlaneId;
    }
    inline const Int_t& GetBarId() const
    {
        return fBarId;
    }
    inline const Int_t& GetSide() const
    {
        return fSide;
    }
    inline const Double_t& GetTime() const
    {
        return fTime;
    }
    inline const Double_t& GetCharge() const
    {
        return fCharge;
    }

  private:
    Int_t fPlaneId;
    Int_t fBarId;
    Int_t fSide;
    Double_t fTime;
    Double_t fCharge;

  public:
    ClassDef(R3BNveto_segPmt, 1)
};

#endif
