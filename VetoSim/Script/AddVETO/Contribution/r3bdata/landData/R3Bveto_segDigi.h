// -------------------------------------------------------------------------
// -----                      R3Bveto_segDigi header file                  -----
// -----                  Created 26/03/09  by D.Bertini               -----
// -------------------------------------------------------------------------


/**  R3Bveto_segDigi.h
 **/


#ifndef R3BVETO_SEGDIGI_H
#define R3BVETO_SEGDIGI_H


#include "TObject.h"
#include "TVector3.h"



class R3Bveto_segDigi : public TObject
{

 public:

  /** Default constructor **/
  R3Bveto_segDigi();
  R3Bveto_segDigi(Int_t paddle, Double_t tdcL, Double_t tdcR,
   Double_t tdc, Double_t qdcL, Double_t qdcR, Double_t qdc,
   Double_t xx, Double_t yy, Double_t zz);

  R3Bveto_segDigi(Int_t paddle, Double_t tdcR, Double_t tdcL,
	               Double_t qdcR, Double_t qdcL);
   
  /** Copy constructor **/
  R3Bveto_segDigi(const R3Bveto_segDigi&);

  /** Destructor **/
  virtual ~R3Bveto_segDigi();

  R3Bveto_segDigi& operator=(const R3Bveto_segDigi&) { return *this; }

  /** Output to screen **/
  virtual void Print(const Option_t* opt) const;



  void SetTdcR(Double_t time){fTdcR = time;}
  Double_t GetTdcR(){return fTdcR;}

  void SetTdcL(Double_t time){fTdcL = time;}
  Double_t GetTdcL(){return fTdcL;}
  
  void SetTdc(Double_t time){fTdc = time;}
  Double_t GetTdc() const {return fTdc;}

  void SetQdcR(Double_t charge){fQdcR = charge;}
  Double_t GetQdcR(){return fQdcR;}

  void SetQdcL(Double_t charge){fQdcL = charge;}
  Double_t GetQdcL(){return fQdcL;}

  void SetQdc(Double_t charge){fQdc = charge;}
  Double_t GetQdc(){return fQdc;}

  void SetXX(Double_t pos){fxx = pos;}
  Double_t GetXX(){return fxx;}

  void SetYY(Double_t pos){fyy = pos;}
  Double_t GetYY(){return fyy;}

  void SetZZ(Double_t pos){fzz = pos;}
  Double_t GetZZ(){return fzz;}

  void SetPaddleNr(Int_t paddleNb){fPaddleNb = paddleNb;}
  Int_t GetPaddleNr(){return fPaddleNb;}



 protected:

  Int_t fPaddleNb;
  Double32_t fTdcL;
  Double32_t fTdcR;
  Double32_t fTdc;
  Double32_t fQdcL;
  Double32_t fQdcR;
  Double32_t fQdc;
  Double32_t fxx;
  Double32_t fyy;
  Double32_t fzz;

  ClassDef(R3Bveto_segDigi,1)

};

#endif
