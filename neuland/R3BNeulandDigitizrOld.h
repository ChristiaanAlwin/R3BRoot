#ifndef _R3B_NEULAND_DIGITIZER_H_
#define _R3B_NEULAND_DIGITIZER_H_ 1

#include "FairTask.h"

#include "R3BLandPoint.h"
#include "R3BLandDigiPar.h"
#include "R3BLandDigi.h"

#include "DigitizingEngine.h"

class TClonesArray;
class TH1F;


class R3BNeulandDigitizrOld : public FairTask {

public:
    R3BNeulandDigitizrOld();
    ~R3BNeulandDigitizrOld();

    virtual InitStatus Init();
    virtual void Exec(Option_t *);
    virtual void Finish();
    virtual void Reset();

protected:
    TClonesArray *fLandPoints;
    TClonesArray *fLandDigi;

    R3BLandDigiPar *fLandDigiPar;
    Double_t fPlength;

    Neuland::DigitizingEngine *fDigitizingEngine;

    TH1F *hPMl;
    TH1F *hPMr;
    TH1F *hMultOne;
    TH1F *hMultTwo;
    TH1F *hRLTimeToTrig;

    Int_t npaddles;
    Int_t nplanes;
    Int_t paddle_per_plane;
Int_t fneuland_PaddlesPerSinglePlane;   // NOTE: Int_t
Int_t fneuland_NsinglePlanes;           // NOTE: Int_t
Double_t fneuland_TotalPaddleLength;    // NOTE: Double_t cm!
Double_t fneuland_TotalPaddleWidth;     // NOTE: Double_t cm!
Double_t fneuland_TotalPaddleThickness; // NOTE: Double_t cm!
Double_t fneuland_Global_Xpos;          // NOTE: Double_t cm!
Double_t fneuland_Global_Ypos;          // NOTE: Double_t cm!
Double_t fneuland_Global_Zpos;          // NOTE: Double_t cm!
Double_t fneuland_Global_XRotation;     // NOTE: Double_t radians!
Double_t fneuland_Global_YRotation;     // NOTE: Double_t radians!
Double_t fneuland_Global_ZRotation;     // NOTE: Double_t radians!
Double_t fneuland_UseUnformDist;        // NOTE: Bool_t
TRandom3* fneuland_RandGen;             // NOTE: TRandom3-pointer

private:
    virtual void SetParContainers();

    ClassDef(R3BNeulandDigitizrOld, 1)
};

#endif //_R3B_NEULAND_DIGITIZER_H_
