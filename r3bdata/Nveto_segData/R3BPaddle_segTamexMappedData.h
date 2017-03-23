// ----------------------------------------------------------------
// -----              R3BPaddle_segTamexMappedData               -----
// -----             Created 27-01-2015 by M.Heil             -----
// ----------------------------------------------------------------

#ifndef R3BPADDLE_SEGTAMEXMAPPEDITEM_H
#define R3BPADDLE_SEGTAMEXMAPPEDITEM_H

#include "TObject.h"

class R3BPaddle_segTamexMappedData : public TObject
{
  public:
    // Default Constructor
    R3BPaddle_segTamexMappedData();

    /** Standard Constructor
     **/
    R3BPaddle_segTamexMappedData( Int_t planeId,
                              Int_t barId,
                              Int_t side,
                              Int_t coarseTimeLE,
                              Int_t fineTimeLE,
                              Int_t coarseTimeTE,
                              Int_t fineTimeTE,
                              Bool_t is17);
    R3BPaddle_segTamexMappedData(const R3BPaddle_segTamexMappedData&);
   
    // Destructor
    virtual ~R3BPaddle_segTamexMappedData()
    {
    }

    // Getters
    inline const Int_t& GetPlaneId() const
    {
        return fPlane;
    }
    inline const Int_t& GetBarId() const
    {
        return fBar;
    }
    inline const Int_t& GetSide() const
    {
        return fSide;
    }
    inline const Int_t& GetCoarseTimeLE() const
    {
        return fCoarseTimeLE;
    }
    inline const Int_t& GetFineTimeLE() const
    {
        return fFineTimeLE;
    }
    inline const Int_t& GetCoarseTimeTE() const
    {
        return fCoarseTimeTE;
    }   
    inline const Int_t& GetFineTimeTE() const
    {
        return fFineTimeTE;
    }

	// e = edge: 0=leading 1=trailing
    inline const Int_t& GetCoarseTime(int e) const
    {
        return e ? fCoarseTimeTE : fCoarseTimeLE;
    }
    inline const Int_t& GetFineTime(int e) const
    {
        return e ? fFineTimeTE : fFineTimeLE;
    }

    inline const Bool_t& Is17() const
    {
        return fIs17;
    }

  protected:
    Int_t fPlane; //... number of plane 1..n
    Int_t fBar;   //... number of bar   1..n
    Int_t fSide;    //... PMT             1 or 2
    Int_t fCoarseTimeLE;  //... coarse time of leading edge
    Int_t fFineTimeLE;  //... fine time of leading edge
    Int_t fCoarseTimeTE;  //... coarse time of trailing edge
    Int_t fFineTimeTE;  //... fine time of trailing edge    
    
    Bool_t fIs17; //... true if CH 17

  public:
    ClassDef(R3BPaddle_segTamexMappedData, 1)
};

//typedef R3BPaddle_segTamexMappedData R3BNveto_segTamexMappedItem;

#endif

