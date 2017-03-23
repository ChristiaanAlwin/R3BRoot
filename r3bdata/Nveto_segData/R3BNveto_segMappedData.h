
#ifndef R3BNVETO_SEGMAPPEDITEM_H
#define R3BNVETO_SEGMAPPEDITEM_H

#include "TObject.h"

class R3BNveto_segMappedData : public TObject
{
  public:
    // Default Constructor
    R3BNveto_segMappedData();

    /** Standard Constructor
     **/
    R3BNveto_segMappedData(Int_t clock,
                         Int_t tacData,
                         Int_t stopT,
                         Int_t qdcData,
                         Int_t plane,
                         Int_t paddle,
                         Int_t side);

    // Destructor
    virtual ~R3BNveto_segMappedData()
    {
    }

    // Getters
    inline const UShort_t& GetClock() const
    {
        return fClock;
    }
    inline const UShort_t& GetTacData() const
    {
        return fTacData;
    }
    inline const UShort_t& GetStopT() const
    {
        return fStopT;
    }
    inline const UShort_t& GetQdcData() const
    {
        return fQdcData;
    }
    inline const Int_t& GetPlane() const
    {
        return fPlane;
    }
    inline const Int_t& GetPaddle() const
    {
        return fPaddle;
    }
    inline const Int_t& GetSide() const
    {
        return fSide;
    }

  private:
    UShort_t fClock;   //... Clock data
    UShort_t fTacData; //... TAC data
    UShort_t fStopT;   //... Stop time
    UShort_t fQdcData; //... QDC data
    Int_t fPlane;  //... plane ID
    Int_t fPaddle; //... paddle ID
    Int_t fSide;   //... PMT

  public:
    ClassDef(R3BNveto_segMappedData, 2)
};

#endif
