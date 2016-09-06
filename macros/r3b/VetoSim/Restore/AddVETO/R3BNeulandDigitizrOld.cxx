#include "R3BNeulandDigitizrOld.h"

#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>

#include "TGeoManager.h"
#include "TClonesArray.h"
#include "TMath.h"
#include "TH1F.h"
#include "TVector3.h"
#include "TRandom3.h"

#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "FairLogger.h"


inline bool IsHorizontalPaddle(const Int_t paddle, const Int_t paddle_per_plane)
{
    // TODO: Wait, what?
    return (int)(((paddle - 1) / paddle_per_plane)) / 2. == (int)((int)(((paddle - 1) / paddle_per_plane)) / 2.);
}


R3BNeulandDigitizrOld::R3BNeulandDigitizrOld()
    : FairTask("R3B NeuLAND Digitizer", 0),
      fLandDigi(new TClonesArray("R3BLandDigi")),
      fDigitizingEngine(new Neuland::DigitizingEngine())
{
}


R3BNeulandDigitizrOld::~R3BNeulandDigitizrOld()
{
}


void R3BNeulandDigitizrOld::SetParContainers()
{
    FairRunAna *run = FairRunAna::Instance();
    if (!run) {
        Fatal("SetParContainers", "No analysis run");
        return;
    }

    FairRuntimeDb *rtdb = run->GetRuntimeDb();
    if (!rtdb) {
        Fatal("SetParContainers", "No runtime database");
        return;
    }

    fLandDigiPar = (R3BLandDigiPar *)rtdb->getContainer("R3BLandDigiPar");
    if (!fLandDigiPar) {
        Fatal("SetParContainers", "No R3BLandDigiPar");
        return;
    }

    if (fVerbose && fLandDigiPar) {
        LOG(INFO) << "R3BNeulandDigitizrOld::SetParContainers() " << FairLogger::endl;
        LOG(INFO) << "Container R3BLandDigiPar loaded " << FairLogger::endl;
    }
}


InitStatus R3BNeulandDigitizrOld::Init()
{
fneuland_RandGen = new TRandom3();
    FairRootManager *ioman = FairRootManager::Instance();
    if (!ioman) {
        Fatal("Init", "No FairRootManager");
        return kFATAL;
    }

    fLandPoints = (TClonesArray *) ioman->GetObject("NeulandPoints");
    if (fLandPoints == nullptr) {
        fLandPoints = (TClonesArray *) ioman->GetObject("LandPoint");
    }
    if (fLandPoints == nullptr) {
        LOG(FATAL) << "R3BNeulandDigitizrOld: No NeulandPoints!" << FairLogger::endl;
        return kFATAL;
    }

    ioman->Register("LandDigi", "Digital response in Land", fLandDigi, kTRUE);


    // Parameter retrieval
    // Only after Init one retrieve the Digitization Parameters!
    npaddles = fLandDigiPar->GetMaxPaddle() + 1;
    nplanes = fLandDigiPar->GetMaxPlane();
TFile* inputs = new TFile("../../Data/Inputs/Inputs.root","read");
TH1I* Integers = (TH1I*) inputs->Get("Integers");
TH1D* Doubles = (TH1D*) inputs->Get("Doubles");
fneuland_PaddlesPerSinglePlane = 50;
fneuland_NsinglePlanes = Integers->GetBinContent(2);
fneuland_TotalPaddleLength = 250.0;
fneuland_TotalPaddleWidth = 5.0;
fneuland_TotalPaddleThickness = 5.0;
fneuland_Global_Xpos = Doubles->GetBinContent(111);
fneuland_Global_Ypos = Doubles->GetBinContent(112);
fneuland_Global_Zpos = Doubles->GetBinContent(113);
fneuland_Global_XRotation = (TMath::Pi()/180.0)*Doubles->GetBinContent(114);
fneuland_Global_YRotation = (TMath::Pi()/180.0)*Doubles->GetBinContent(115);
fneuland_Global_ZRotation = (TMath::Pi()/180.0)*Doubles->GetBinContent(116);
fneuland_UseUnformDist = kFALSE;
npaddles = fneuland_PaddlesPerSinglePlane*fneuland_NsinglePlanes + 1;
nplanes = fneuland_NsinglePlanes;
if ((Integers->GetBinContent(312))==2) {fneuland_UseUnformDist = kTRUE;}
// Replace the detector characteristics with the choices from our Inputs.root-file:
Neuland::DigitizingEngine::fTimeRes = 0.001*Doubles->GetBinContent(309); // NOTE: conversion from [ps] to [ns]!!!
Neuland::DigitizingEngine::fThresh = 0.001*Doubles->GetBinContent(301); // NOTE: conversion from [keV] to [MeV]!!!
// fixed!
    LOG(INFO) << "R3BNeulandDigitizrOld: # paddles: " << npaddles - 1 << ", # planes: " << nplanes << FairLogger::endl;
    paddle_per_plane = (npaddles - 1) / nplanes;
    LOG(INFO) << "R3BNeulandDigitizrOld: paddles per plane: " << paddle_per_plane << FairLogger::endl;

    // half of the length of a scintillator
    fPlength = fLandDigiPar->GetPaddleLength(); // [cm]
    LOG(INFO) << "R3BNeulandDigitizrOld: paddle length: " << fPlength << FairLogger::endl;
    fDigitizingEngine->SetPaddleHalfLength(fPlength);

    // Initialise control histograms
    hPMl = new TH1F("PM_left", "Arrival times of left PM", 1000, 0., 1000.);
    hPMr = new TH1F("PM_right", "Arrival times of right PM", 1000, 0., 1000.);
    hMultOne = new TH1F("MultiplicityOne", "Paddle multiplicity: only one PMT per paddle", 3000, 0, 3000);
    hMultTwo = new TH1F("MultiplicityTwo", "Paddle multiplicity: both PMTs of a paddle",   3000, 0, 3000);
    hRLTimeToTrig = new TH1F("hRLTimeToTrig", "R/Ltime-triggerTime", 200, -100, 100);

    return kSUCCESS;
}


void R3BNeulandDigitizrOld::Exec(Option_t *)
{
    Reset();

    Double_t xpaddle[npaddles], ypaddle[npaddles], zpaddle[npaddles];

    /* Look at each Land Point, if it deposited energy in the szintillator,
     * store it with reference to the bar */
    const UInt_t nLandPoints = fLandPoints->GetEntries();
    R3BLandPoint *landPoint;
    for (UInt_t l = 0; l < nLandPoints; l++) {
        landPoint = (R3BLandPoint *)fLandPoints->At(l);
// rotate MC points in opposite direction:
TVector3 PosIn;
TVector3 PosOut;
        
PosIn.SetX(landPoint->GetXIn() - fneuland_Global_Xpos);
PosIn.SetY(landPoint->GetYIn() - fneuland_Global_Ypos);
PosIn.SetZ(landPoint->GetZIn() - fneuland_Global_Zpos);
        
PosOut.SetX(landPoint->GetXOut() - fneuland_Global_Xpos);
PosOut.SetY(landPoint->GetYOut() - fneuland_Global_Ypos);
PosOut.SetZ(landPoint->GetZOut() - fneuland_Global_Zpos);
        
PosIn.RotateZ(-1.0*fneuland_Global_ZRotation);
PosIn.RotateY(-1.0*fneuland_Global_YRotation);
PosIn.RotateX(-1.0*fneuland_Global_XRotation);
        
PosOut.RotateZ(-1.0*fneuland_Global_ZRotation);
PosOut.RotateY(-1.0*fneuland_Global_YRotation);
PosOut.RotateX(-1.0*fneuland_Global_XRotation);        const Int_t paddle_id = int(landPoint->GetSector()) - 1; //note that paddle starts at 1

        Double_t light = landPoint->GetLightYield() * 1000.;
        Int_t media = int(landPoint->GetPaddleType());

        if (landPoint->GetEnergyLoss() > 0. && media == 3) {

            // TODO: What is this?
            gGeoManager->FindNode(PosIn.X(), PosIn.Y(), PosIn.Z());
            gGeoManager->CdUp();
            Double_t local_point[] = {0., 0., 0.};
            Double_t global_point[3];
            gGeoManager->LocalToMaster(local_point, global_point);
            xpaddle[paddle_id] = global_point[0];
            ypaddle[paddle_id] = global_point[1];
            zpaddle[paddle_id] = global_point[2];

            Double_t dist;
            if (fLandDigiPar->GetGeometryFileName().Contains("proto")) {
                dist = PosIn.Y(); // only vertical paddles
            } else {
                if (IsHorizontalPaddle(paddle_id, paddle_per_plane)) {
                    dist = PosIn.X();
                } else {
                    dist = PosIn.Y();
                }
            }

            try {
                fDigitizingEngine->DepositLight(paddle_id, landPoint->GetTime(), light, dist);
            } catch (std::exception &e) {
                Fatal("Exec", "%s", e.what());
            }

        } //! eloss
    } //! MC hits


    const Double_t triggerTime = fDigitizingEngine->GetTriggerTime();


    /* Fill histograms */
    Int_t multOne = 0;
    Int_t multTwo = 0;
    for (const auto &kv : fDigitizingEngine->paddles) {
        const auto &paddle = kv.second;

        for (const auto &hit : paddle.leftPMT.GetHits()) {
            hPMl->Fill(hit.time);
        }
        for (const auto &hit : paddle.rightPMT.GetHits()) {
            hPMr->Fill(hit.time);
        }

        // Multiplicity if only PMT has fired
        if (paddle.leftPMT.HasFired() && !paddle.rightPMT.HasFired()) {
            multOne++;
        }
        if (!paddle.leftPMT.HasFired() && paddle.rightPMT.HasFired()) {
            multOne++;
        }

        // Multiplicity if two PMT have fired
        if (paddle.leftPMT.HasFired() && paddle.rightPMT.HasFired()) {
            multTwo++;
            hRLTimeToTrig->Fill(paddle.leftPMT.GetTDC() - triggerTime);
            hRLTimeToTrig->Fill(paddle.rightPMT.GetTDC() - triggerTime);
        }
    }
    hMultOne->Fill(multOne);
    hMultTwo->Fill(multTwo);
    // NOTE: This section is added by C. A. Douma to account for global transformations.
    // It can be used outside of the VetoSim-framework if the proper fneuland-quantities are set.
    
    // globals:
    /*
    fneuland_PaddlesPerSinglePlane;// NOTE: Int_t
    fneuland_NsinglePlanes;        // NOTE: Int_t
    fneuland_TotalPaddleLength;    // NOTE: Double_t cm!
    fneuland_TotalPaddleWidth;     // NOTE: Double_t cm!
    fneuland_TotalPaddleThickness; // NOTE: Double_t cm!
    fneuland_Global_Xpos;          // NOTE: Double_t cm!
    fneuland_Global_Ypos;          // NOTE: Double_t cm!
    fneuland_Global_Zpos;          // NOTE: Double_t cm!
    fneuland_Global_XRotation;     // NOTE: Double_t radians!
    fneuland_Global_YRotation;     // NOTE: Double_t radians!
    fneuland_Global_ZRotation;     // NOTE: Double_t radians!
    fneuland_UseUnformDist;        // NOTE: Bool_t
    fneuland_RandGen;              // NOTE: TRandom3-pointer
    */
    
    // Declare some variables:
    Int_t Paddle_index = 0;
    Double_t zcenter = 0;
    Double_t xcenter = 0.0;
    Double_t ycenter = 0.0;
    TVector3 pos;
    
    // Now loop over all paddles to re-fill the xpaddle, ypaddle and zpaddle arrays:
    for (Int_t kplane = 0; kplane<fneuland_NsinglePlanes; ++kplane)
    {
        for (Int_t kbar = 0; kbar<fneuland_PaddlesPerSinglePlane; ++kbar)
        {
            // define the paddle index. It needs to run from 0 to npaddles:
            // note that for NeuLAND, kplane rins from 0 to 29 and kbar
            // runs from 0 to 49. Hence the proper way to define the index is:
            Paddle_index = kbar + fneuland_PaddlesPerSinglePlane*kplane;
            
            // So now we start in the local coordinate frame, where the first plane
            // is horizontal. the center of the z-coordinate is:
            zcenter = -0.5*fneuland_TotalPaddleThickness*((Int_t) (fneuland_NsinglePlanes - 1));
            zcenter = zcenter + ((Int_t) kplane)*fneuland_TotalPaddleThickness;
            
            // For xcenter and ycenter of the bars we need to distinguish between
            // horizontal and vertical planes. The first plane has kplane = 0 and
            // is horizontal:
            if ((kplane%2)==0)
            {
                // horizontal plane: ycenter should differ and xcenter should
                // be all zeros:
                xcenter = 0.0;
                ycenter = -0.5*fneuland_TotalPaddleWidth*((Int_t) (fneuland_PaddlesPerSinglePlane - 1));
                ycenter = ycenter + ((Int_t) kbar)*fneuland_TotalPaddleWidth;
            }
            else
            {
                // vertical plane: just the opposite:
                ycenter = 0.0;
                xcenter = -0.5*fneuland_TotalPaddleWidth*((Int_t) (fneuland_PaddlesPerSinglePlane - 1));
                xcenter = xcenter + ((Int_t) kbar)*fneuland_TotalPaddleWidth;
            }
            
            // Now we need to add the uniform distribution if this is required:
            if (fneuland_UseUnformDist)
            {
                zcenter = zcenter + fneuland_RandGen->Uniform(fneuland_TotalPaddleThickness) - 0.5*fneuland_TotalPaddleThickness;
                
                if ((kplane%2)==0)
                {
                    xcenter = xcenter + fneuland_RandGen->Uniform(fneuland_TotalPaddleLength) - 0.5*fneuland_TotalPaddleLength;
                    ycenter = ycenter + fneuland_RandGen->Uniform(fneuland_TotalPaddleWidth) - 0.5*fneuland_TotalPaddleWidth;
                }
                else
                {
                    ycenter = ycenter + fneuland_RandGen->Uniform(fneuland_TotalPaddleLength) - 0.5*fneuland_TotalPaddleLength;
                    xcenter = xcenter + fneuland_RandGen->Uniform(fneuland_TotalPaddleWidth) - 0.5*fneuland_TotalPaddleWidth;
                }
            }
            
            // Now we have all the coordinates within the local frame. Now we must transform
            // them to the global frame. But we should only do that AFTER the Tdiff-position
            // is added. This is because otherwise we do not know anymore where to add it!
            
            // now we can re-fill the arrays:
            xpaddle[Paddle_index] = xcenter;
            ypaddle[Paddle_index] = ycenter;
            zpaddle[Paddle_index] = zcenter;
            
            // Done!
        }
    }

    // Now the arrays are re-filled with the exact positions that we want! All that the next
    // piece of code then does is to replace either x or y with the Tdiff-position. For the
    // rest we just copy the array-value into the histograms!


    for (const auto &kv : fDigitizingEngine->paddles) {
        const Int_t paddleNr = kv.first;
        const auto &paddle = kv.second;

        if (paddle.HasFired()) {

            // Get position and other information and fill digis.
            Double_t xx, yy, zz;

            if (fLandDigiPar->GetGeometryFileName().Contains("proto")) {
                // vertical paddles
                xx = xpaddle[paddleNr];
                yy = paddle.GetPosition();
                zz = zpaddle[paddleNr];
            } else {
                if (IsHorizontalPaddle(paddleNr, paddle_per_plane)) {
                    //horizontal paddles
                    xx = paddle.GetPosition();
                    yy = ypaddle[paddleNr];
                    zz = zpaddle[paddleNr];
                } else {
                    // vertical paddles
                    xx = xpaddle[paddleNr];
                    yy = paddle.GetPosition();
                    zz = zpaddle[paddleNr];
                }
            }

            Double_t tdcL = paddle.leftPMT.GetTDC();
// ...
// Now transform to the global LAB-frame:
pos.SetX(xx);
pos.SetY(yy);
pos.SetZ(zz);
pos.RotateX(fneuland_Global_XRotation);
pos.RotateY(fneuland_Global_YRotation);
pos.RotateZ(fneuland_Global_ZRotation);
xx = pos.X() + fneuland_Global_Xpos;
yy = pos.Y() + fneuland_Global_Ypos;
zz = pos.Z() + fneuland_Global_Zpos;            Double_t tdcR = paddle.rightPMT.GetTDC();
            Double_t qdcL = paddle.leftPMT.GetEnergy();
            Double_t qdcR = paddle.rightPMT.GetEnergy();

            Double_t qdc = paddle.GetPaddleEnergy();
            Double_t tdc = paddle.GetPaddleTime();

            new((*fLandDigi)[fLandDigi->GetEntriesFast()]) R3BLandDigi(paddleNr,
                    tdcL, tdcR, tdc,
                    qdcL, qdcR, qdc,
                    xx, yy, zz);
        }
    } // loop over paddles



    if (fVerbose) {
        LOG(INFO) << "R3BNeulandDigitizrOld: produced "
                  << fLandDigi->GetEntries() << " digis" << FairLogger::endl;
    }
}


void R3BNeulandDigitizrOld::Reset()
{
    fLandDigi->Clear();
    fDigitizingEngine->Clear();
}


void R3BNeulandDigitizrOld::Finish()
{
    hPMl->Write();
    hPMr->Write();
    hMultOne->Write();
    hMultTwo->Write();
    hRLTimeToTrig->Write();
}


ClassImp(R3BNeulandDigitizrOld);
