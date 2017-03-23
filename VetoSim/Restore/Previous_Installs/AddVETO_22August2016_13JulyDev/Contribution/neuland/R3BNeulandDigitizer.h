// -----                     Updated by C. A. Douma                        -----

#ifndef _R3B_NEULAND_DIGITIZER_H_
#define _R3B_NEULAND_DIGITIZER_H_ 1

#include "FairTask.h"

#include "TMath.h"

#include "R3BLandPoint.h"
#include "R3BLandDigiPar.h"
#include "R3BLandDigi.h"

#include "DigitizingEngine.h"

class TClonesArray;
class TH1F;


class R3BNeulandDigitizer : public FairTask {

public:
    R3BNeulandDigitizer();
    ~R3BNeulandDigitizer();

    virtual InitStatus Init();
    virtual void Exec(Option_t *);
    virtual void Finish();
    virtual void Reset();
    
    void SetNeuLAND_EnergyThreshold(Double_t const Ethres) {fneuland_Ethreshold = 0.001*Ethres;} // submit value in keV!
    void SetNeuLAND_TimeResolution(Double_t const tres) {fneuland_TimeResolution = 0.001*tres;} // submit value in keV!
    void SetNeuLAND_NsinglePlanes(Int_t const n) {fneuland_NsinglePlanes = n;}
    void SetNeuLAND_GlobalPosition(Double_t const x, Double_t const y, Double_t const z) {fneuland_Global_Xpos = x; fneuland_Global_Ypos = y; fneuland_Global_Zpos = z;} // submit value in cm!
    void SetNeuLAND_GlobalRotation(Double_t const x, Double_t const y, Double_t const z) // submit value in degrees!
    {
        fneuland_Global_XRotation = TMath::Pi()*x/180.0; 
        fneuland_Global_YRotation = TMath::Pi()*y/180.0; 
        fneuland_Global_ZRotation = TMath::Pi()*z/180.0;
    }
    void ActivateUniformSmearing() {fneuland_UseUnformDist = kTRUE;}

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
    Double_t fneuland_TimeResolution;       // NOTE: Double_t ns!
    Double_t fneuland_Ethreshold;           // NOTE: Double_t MeV!
    
    TRandom3* fneuland_RandGen;             // NOTE: TRandom3-pointer

private:
    virtual void SetParContainers();

    ClassDef(R3BNeulandDigitizer, 1)
};

#endif //_R3B_NEULAND_DIGITIZER_H_
