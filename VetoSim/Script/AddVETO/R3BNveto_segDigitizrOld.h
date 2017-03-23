#ifndef _R3B_NVETO_SEG_DIGITIZER_H_
#define _R3B_NVETO_SEG_DIGITIZER_H_ 1

#include "FairTask.h"

#include "R3Bveto_segPoint.h"
#include "R3Bveto_segDigiPar.h"
#include "R3Bveto_segDigi.h"

#include "Digitizin_segEngine.h"

class TClonesArray;
class TH1F;


class R3BNveto_segDigitizrOld : public FairTask {

public:
    R3BNveto_segDigitizrOld();
    ~R3BNveto_segDigitizrOld();

    virtual InitStatus Init();
    virtual void Exec(Option_t *);
    virtual void Finish();
    virtual void Reset();

protected:
    TClonesArray *fveto_segPoints;
    TClonesArray *fveto_segDigi;

    R3Bveto_segDigiPar *fveto_segDigiPar;
    Double_t fPlength;

    Nveto_seg::Digitizin_segEngine *fDigitizin_segEngine;

    TH1F *hPMl;
    TH1F *hPMr;
    TH1F *hMultOne;
    TH1F *hMultTwo;
    TH1F *hRLTimeToTrig;

    Int_t npaddles;
    Int_t nplanes;
    Int_t paddle_per_plane;
Int_t fNveto_seg_PaddlesPerSinglePlane;   // NOTE: Int_t
Int_t fNveto_seg_NsinglePlanes;           // NOTE: Int_t
Double_t fNveto_seg_TotalPaddleLength;    // NOTE: Double_t cm!
Double_t fNveto_seg_TotalPaddleWidth;     // NOTE: Double_t cm!
Double_t fNveto_seg_TotalPaddleThickness; // NOTE: Double_t cm!
Double_t fNveto_seg_Global_Xpos;          // NOTE: Double_t cm!
Double_t fNveto_seg_Global_Ypos;          // NOTE: Double_t cm!
Double_t fNveto_seg_Global_Zpos;          // NOTE: Double_t cm!
Double_t fNveto_seg_Global_XRotation;     // NOTE: Double_t radians!
Double_t fNveto_seg_Global_YRotation;     // NOTE: Double_t radians!
Double_t fNveto_seg_Global_ZRotation;     // NOTE: Double_t radians!
Double_t fNveto_seg_UseUnformDist;        // NOTE: Bool_t
Double_t fTcut_up;
Double_t fTcut_low;
TRandom3* fNveto_seg_RandGen;             // NOTE: TRandom3-pointer

private:
    virtual void SetParContainers();

    ClassDef(R3BNveto_segDigitizrOld, 1)
};

#endif //_R3B_NVETO_SEG_DIGITIZER_H_
