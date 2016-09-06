// ------------------------------------------------------------------------
// -----                        R3BNeutHt_seg                            -----
// -----              Created 21.05.2012 by D.Kresan                  -----
// ------------------------------------------------------------------------

#ifndef R3BNEUTHT_SEG_H
#define R3BNEUTHT_SEG_H

#include "TObject.h"
#include "TMath.h"


class R3BNeutHt_seg : public TObject {

 public:
  R3BNeutHt_seg();
  R3BNeutHt_seg(Double_t x, Double_t y, Double_t z, Double_t t);
  virtual ~R3BNeutHt_seg();

 private:
  Double32_t fX;
  Double32_t fY;
  Double32_t fZ;
  Double32_t fT;

 public:
  inline Double_t GetX() const {  return fX;  }
  inline Double_t GetY() const {  return fY;  }
  inline Double_t GetZ() const {  return fZ;  }
  inline Double_t GetT() const {  return fT;  }
  inline Double_t GetD() const {  return TMath::Sqrt(fX*fX+fY*fY+fZ*fZ); }
  Double_t GetP() const;

  ClassDef(R3BNeutHt_seg, 1)
};


#endif
