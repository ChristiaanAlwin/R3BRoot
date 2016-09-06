// -----------------------------------------------------------------------
// -----                    R3BNveto_segCalData                        -----
// -----              Created 22-04-2014 by D.Kresan                 -----
// -----------------------------------------------------------------------

#ifndef R3BNVETO_SEGCALDATA_H
#define R3BNVETO_SEGCALDATA_H

#include "TObject.h"

class R3BNveto_segCalData : public TObject
{
  public:
    R3BNveto_segCalData();
    R3BNveto_segCalData(Int_t barId, Int_t side, Double_t time, Int_t qdc);
    virtual ~R3BNveto_segCalData();

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
    inline const Int_t& GetQdc() const
    {
        return fQdc;
    }

  private:
    Int_t fBarId;
    Int_t fSide;
    Double_t fTime;
    Int_t fQdc;

  public:
    ClassDef(R3BNveto_segCalData, 1)
};

#endif
