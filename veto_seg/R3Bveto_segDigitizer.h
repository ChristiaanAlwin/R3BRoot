#ifndef R3BVETO_SEGDIGITIZER_H
#define R3BVETO_SEGDIGITIZER_H 1


#include "FairTask.h"
#include "R3Bveto_segDigiPar.h"
#include "R3Bveto_segDigi.h"

class TClonesArray;
class TObjectArray;
class TH1F;
class TH2F;
class TF1;
class TRandom3;


struct PM_RES
{
  Double_t Ltime,Rtime,LlightCFD,RlightCFD,LlightQDC,RlightQDC,Lenergy,Renergy;
};


class R3Bveto_segDigitizer : public FairTask {

public:
  /** Default constructor **/
  R3Bveto_segDigitizer();

  /** Constructor **/
  R3Bveto_segDigitizer(Int_t verbose);

  /** Destructor **/
  ~R3Bveto_segDigitizer();

  /** Virtual method Init **/
  virtual InitStatus Init();

  /** Virtual method Exec **/
  virtual void Exec(Option_t* opt);

  virtual void Finish();

  virtual void Reset();

  // Allow change of the PMT Saturation at runtime. R3Bveto_segDigitizer is initialized with the default value
  inline void SetSaturationCoefficient(const Double_t& saturationCoefficient) {  fSaturationCoefficient = saturationCoefficient;  }
  inline Double_t GetSaturationCoefficient() const  {  return fSaturationCoefficient;  }

  // Allow change of length of time gate for QDC ("Integration time") at runtime
  inline void SetTOFRange(const Double_t& TOFRange) {  fTOFRange = TOFRange;  }
  inline Double_t GetTOFRange() const  {  return fTOFRange;  }
  Double_t BuildTOFRangeFromBeamEnergy(const Double_t &e);
  // Compatibility to not break existing code
  inline void UseBeam(const Double_t& beamEnergy) { fBeamEnergy = beamEnergy; }


  inline void UseThresholds(const char *fileName, const Int_t nChannels)
  { fThreshFileName = TString(fileName);  fNChannels = nChannels; }

  R3Bveto_segDigi* AddHit(Int_t paddleNr, Double_t tdcL, Double_t tdcR, Double_t tdc,
                      Double_t qdcL, Double_t qdcR, Double_t qdc,
                      Double_t xx,Double_t yy, Double_t zz );


protected:

  TF1 *f1;
  TRandom3 *fRnd;

  TString fThreshFileName;
  Int_t fNChannels;

  TClonesArray *fveto_segPoints;
  TClonesArray *fveto_segMCTrack;
  TClonesArray *fveto_segDigi;

  // Parameter class
  R3Bveto_segDigiPar *fveto_segDigiPar;

  // Control Hitograms
  TH1F *hPMl;
  TH1F *hPMr;
  TH1F *hMult1;
  TH1F *hMult2;
  TH1F *hRLTimeToTrig;

  Double_t threshL[5000];
  Double_t threshR[5000];

  Int_t eventNo;
  Int_t npaddles;
  Int_t nplanes;
  Int_t paddle_per_plane;
  Double_t plength; // half length of paddle
  Double_t att; // light attenuation factor [1/cm]
  Double_t c;
  Double_t cMedia; // speed of light in material in cm/ns
  Double_t fBeamEnergy;
  PM_RES **PM_res;

  static const Double_t DEFAULT_SATURATION_COEFFICIENT;
  Double_t fSaturationCoefficient;
  
  Double_t fTOFRange;

private:

  virtual void SetParContainers();


  ClassDef(R3Bveto_segDigitizer,1)
};

#endif
