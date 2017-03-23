// -------------------------------------------------------------------------
// -----                      R3Bveto_segHit header file                  -----
// -----                  Created 26/03/09  by D.Bertini               -----
// -------------------------------------------------------------------------


/**  R3Bveto_segHit.h
 **/


#ifndef R3BVETO_SEGHIT_H
#define R3BVETO_SEGHIT_H


#include "TObject.h"
#include "TVector3.h"

#include "FairHit.h"


class R3Bveto_segHit : public FairHit
{

 public:

  /** Default constructor **/
  R3Bveto_segHit();

  /** Copy constructor **/
  R3Bveto_segHit(const R3Bveto_segHit&);

  /** Destructor **/
  virtual ~R3Bveto_segHit();

  R3Bveto_segHit& operator=(const R3Bveto_segHit&) { return *this; }

  /** Output to screen **/
  virtual void Print(const Option_t* opt) const;


  void SetEnergy(Double_t en){fEnergy = en;}
  Double_t GetEnergy(){return fEnergy;}

  void SetToF(Double_t time){fTof = time;}
  Double_t GetToF(){return fTof;}

  void SetDetID(Int_t id){ fDetectorID = id; }
  Int_t GetDetID(){ return fDetectorID; }

  void SetSaturnPaddle(Int_t paddleNb){fSaturnPaddleNb = paddleNb;}
  Int_t GetSaturnPaddle(){return fSaturnPaddleNb;}



 protected:

  Double32_t fEnergy;
  Double32_t fTof;
  Int_t fSaturnPaddleNb;

  ClassDef(R3Bveto_segHit,1)

};

#endif
