
#ifndef R3BNVETO_SEGMAPPEDITEM_H
#define R3BNVETO_SEGMAPPEDITEM_H

#include "R3BNveto_segUnpackData.h"

class R3BNveto_segMappedData : public R3BNveto_segUnpackData
{
  public:
    // Default Constructor
    R3BNveto_segMappedData();

    /** Standard Constructor
     **/
    R3BNveto_segMappedData(Int_t sam,
                         Int_t gtb,
                         Int_t tacAddr,
                         Int_t cal,
                         Int_t clock,
                         Int_t tacData,
                         Int_t qdcData,
                         Int_t plane,
                         Int_t paddle,
                         Int_t side,
                         Bool_t is17);

    // Destructor
    virtual ~R3BNveto_segMappedData()
    {
    }

    // Getters
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
    inline const Bool_t& Is17() const
    {
        return fIs17;
    }

  private:
    Int_t fPlane;  //... plane ID
    Int_t fPaddle; //... paddle ID
    Int_t fSide;   //... PMT
    Bool_t fIs17;  //... true if CH 17

  public:
    ClassDef(R3BNveto_segMappedData, 2)
};

#endif
