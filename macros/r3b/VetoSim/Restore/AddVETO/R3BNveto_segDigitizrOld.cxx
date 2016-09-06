#include "R3BNveto_segDigitizrOld.h"

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


R3BNveto_segDigitizrOld::R3BNveto_segDigitizrOld()
    : FairTask("R3B Nveto_seg Digitizer", 0),
      fveto_segDigi(new TClonesArray("R3Bveto_segDigi")),
      fDigitizin_segEngine(new Nveto_seg::Digitizin_segEngine())
{
}


R3BNveto_segDigitizrOld::~R3BNveto_segDigitizrOld()
{
}


void R3BNveto_segDigitizrOld::SetParContainers()
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

    fveto_segDigiPar = (R3Bveto_segDigiPar *)rtdb->getContainer("R3Bveto_segDigiPar");
    if (!fveto_segDigiPar) {
        Fatal("SetParContainers", "No R3Bveto_segDigiPar");
        return;
    }

    if (fVerbose && fveto_segDigiPar) {
        LOG(INFO) << "R3BNveto_segDigitizrOld::SetParContainers() " << FairLogger::endl;
        LOG(INFO) << "Container R3Bveto_segDigiPar loaded " << FairLogger::endl;
    }
}


InitStatus R3BNveto_segDigitizrOld::Init()
{
fNveto_seg_RandGen = new TRandom3();
    FairRootManager *ioman = FairRootManager::Instance();
    if (!ioman) {
        Fatal("Init", "No FairRootManager");
        return kFATAL;
    }

    fveto_segPoints = (TClonesArray *) ioman->GetObject("Nveto_segPoints");
    if (fveto_segPoints == nullptr) {
        fveto_segPoints = (TClonesArray *) ioman->GetObject("veto_segPoint");
    }
    if (fveto_segPoints == nullptr) {
        LOG(FATAL) << "R3BNveto_segDigitizrOld: No Nveto_segPoints!" << FairLogger::endl;
        return kFATAL;
    }

    ioman->Register("veto_segDigi", "Digital response in veto_seg", fveto_segDigi, kTRUE);


    // Parameter retrieval
    // Only after Init one retrieve the Digitization Parameters!
    npaddles = fveto_segDigiPar->GetMaxPaddle() + 1;
    nplanes = fveto_segDigiPar->GetMaxPlane();
TFile* inputs = new TFile("../../Data/Inputs/Inputs.root","read");
TH1I* Integers = (TH1I*) inputs->Get("Integers");
TH1D* Doubles = (TH1D*) inputs->Get("Doubles");
fNveto_seg_PaddlesPerSinglePlane = Integers->GetBinContent(4);
fNveto_seg_NsinglePlanes = Integers->GetBinContent(3);
fNveto_seg_TotalPaddleLength = Doubles->GetBinContent(127);
fNveto_seg_TotalPaddleWidth = Doubles->GetBinContent(128);
fNveto_seg_TotalPaddleThickness = Doubles->GetBinContent(129);
fNveto_seg_Global_Xpos = Doubles->GetBinContent(121);
fNveto_seg_Global_Ypos = Doubles->GetBinContent(122);
fNveto_seg_Global_Zpos = Doubles->GetBinContent(123);
fNveto_seg_Global_XRotation = (TMath::Pi()/180.0)*Doubles->GetBinContent(124);
fNveto_seg_Global_YRotation = (TMath::Pi()/180.0)*Doubles->GetBinContent(125);
fNveto_seg_Global_ZRotation = (TMath::Pi()/180.0)*Doubles->GetBinContent(126);
fNveto_seg_UseUnformDist = kFALSE;
npaddles = fNveto_seg_PaddlesPerSinglePlane*fNveto_seg_NsinglePlanes + 1;
nplanes = fNveto_seg_NsinglePlanes;
if ((Integers->GetBinContent(312))==2) {fNveto_seg_UseUnformDist = kTRUE;}
// Replace the detector characteristics with the choices from our Inputs.root-file:
Nveto_seg::Digitizin_segEngine::fTimeRes = 0.001*Doubles->GetBinContent(310); // NOTE: conversion from [ps] to [ns]!!!
Nveto_seg::Digitizin_segEngine::fThresh = 0.001*Doubles->GetBinContent(302); // NOTE: conversion from [keV] to [MeV]!!!
Nveto_seg::Digitizin_segEngine::fAttenuation = Doubles->GetBinContent(335);
Nveto_seg::Digitizin_segEngine::fcMedium = Doubles->GetBinContent(334);
// fixed!
    LOG(INFO) << "R3BNveto_segDigitizrOld: # paddles: " << npaddles - 1 << ", # planes: " << nplanes << FairLogger::endl;
    paddle_per_plane = (npaddles - 1) / nplanes;
    LOG(INFO) << "R3BNveto_segDigitizrOld: paddles per plane: " << paddle_per_plane << FairLogger::endl;

    // half of the length of a scintillator
    fPlength = fveto_segDigiPar->GetPaddleLength(); // [cm]
    LOG(INFO) << "R3BNveto_segDigitizrOld: paddle length: " << fPlength << FairLogger::endl;
    fDigitizin_segEngine->SetPaddleHalfLength(fPlength);

    // Initialise control histograms
    hPMl = new TH1F("PM_left", "Arrival times of left PM", 1000, 0., 1000.);
    hPMr = new TH1F("PM_right", "Arrival times of right PM", 1000, 0., 1000.);
    hMultOne = new TH1F("MultiplicityOne", "Paddle multiplicity: only one PMT per paddle", 3000, 0, 3000);
    hMultTwo = new TH1F("MultiplicityTwo", "Paddle multiplicity: both PMTs of a paddle",   3000, 0, 3000);
    hRLTimeToTrig = new TH1F("hRLTimeToTrig", "R/Ltime-triggerTime", 200, -100, 100);

    return kSUCCESS;
}


void R3BNveto_segDigitizrOld::Exec(Option_t *)
{
    Reset();

    Double_t xpaddle[npaddles], ypaddle[npaddles], zpaddle[npaddles];

    /* Look at each veto_seg Point, if it deposited energy in the szintillator,
     * store it with reference to the bar */
    const UInt_t nveto_segPoints = fveto_segPoints->GetEntries();
    R3Bveto_segPoint *veto_segPoint;
    for (UInt_t l = 0; l < nveto_segPoints; l++) {
        veto_segPoint = (R3Bveto_segPoint *)fveto_segPoints->At(l);
// rotate MC points in opposite direction:
TVector3 PosIn;
TVector3 PosOut;
        
PosIn.SetX(veto_segPoint->GetXIn() - fNveto_seg_Global_Xpos);
PosIn.SetY(veto_segPoint->GetYIn() - fNveto_seg_Global_Ypos);
PosIn.SetZ(veto_segPoint->GetZIn() - fNveto_seg_Global_Zpos);
        
PosOut.SetX(veto_segPoint->GetXOut() - fNveto_seg_Global_Xpos);
PosOut.SetY(veto_segPoint->GetYOut() - fNveto_seg_Global_Ypos);
PosOut.SetZ(veto_segPoint->GetZOut() - fNveto_seg_Global_Zpos);
        
PosIn.RotateZ(-1.0*fNveto_seg_Global_ZRotation);
PosIn.RotateY(-1.0*fNveto_seg_Global_YRotation);
PosIn.RotateX(-1.0*fNveto_seg_Global_XRotation);
        
PosOut.RotateZ(-1.0*fNveto_seg_Global_ZRotation);
PosOut.RotateY(-1.0*fNveto_seg_Global_YRotation);
PosOut.RotateX(-1.0*fNveto_seg_Global_XRotation);        const Int_t paddle_id = int(veto_segPoint->GetSector()) - 1; //note that paddle starts at 1

        Double_t light = veto_segPoint->GetLightYield() * 1000.;
        Int_t media = int(veto_segPoint->GetPaddleType());

        if (veto_segPoint->GetEnergyLoss() > 0. && media == 3) {

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
            if (fveto_segDigiPar->GetGeometryFileName().Contains("proto")) {
                dist = PosIn.Y(); // only vertical paddles
            } else {
                if (IsHorizontalPaddle(paddle_id, paddle_per_plane)) {
                    dist = PosIn.X();
                } else {
                    dist = PosIn.Y();
                }
            }

            try {
                fDigitizin_segEngine->DepositLight(paddle_id, veto_segPoint->GetTime(), light, dist);
            } catch (std::exception &e) {
                Fatal("Exec", "%s", e.what());
            }

        } //! eloss
    } //! MC hits


    const Double_t triggerTime = fDigitizin_segEngine->GetTriggerTime();


    /* Fill histograms */
    Int_t multOne = 0;
    Int_t multTwo = 0;
    for (const auto &kv : fDigitizin_segEngine->paddles) {
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
    // It can be used outside of the VetoSim-framework if the proper fNveto_seg-quantities are set.
    
    // globals:
    /*
    fNveto_seg_PaddlesPerSinglePlane;// NOTE: Int_t
    fNveto_seg_NsinglePlanes;        // NOTE: Int_t
    fNveto_seg_TotalPaddleLength;    // NOTE: Double_t cm!
    fNveto_seg_TotalPaddleWidth;     // NOTE: Double_t cm!
    fNveto_seg_TotalPaddleThickness; // NOTE: Double_t cm!
    fNveto_seg_Global_Xpos;          // NOTE: Double_t cm!
    fNveto_seg_Global_Ypos;          // NOTE: Double_t cm!
    fNveto_seg_Global_Zpos;          // NOTE: Double_t cm!
    fNveto_seg_Global_XRotation;     // NOTE: Double_t radians!
    fNveto_seg_Global_YRotation;     // NOTE: Double_t radians!
    fNveto_seg_Global_ZRotation;     // NOTE: Double_t radians!
    fNveto_seg_UseUnformDist;        // NOTE: Bool_t
    fNveto_seg_RandGen;              // NOTE: TRandom3-pointer
    */
    
    // Declare some variables:
    Int_t Paddle_index = 0;
    Double_t zcenter = 0;
    Double_t xcenter = 0.0;
    Double_t ycenter = 0.0;
    TVector3 pos;
    
    // Now loop over all paddles to re-fill the xpaddle, ypaddle and zpaddle arrays:
    for (Int_t kplane = 0; kplane<fNveto_seg_NsinglePlanes; ++kplane)
    {
        for (Int_t kbar = 0; kbar<fNveto_seg_PaddlesPerSinglePlane; ++kbar)
        {
            // define the paddle index. It needs to run from 0 to npaddles:
            // note that for Nveto_seg, kplane rins from 0 to 29 and kbar
            // runs from 0 to 49. Hence the proper way to define the index is:
            Paddle_index = kbar + fNveto_seg_PaddlesPerSinglePlane*kplane;
            
            // So now we start in the local coordinate frame, where the first plane
            // is horizontal. the center of the z-coordinate is:
            zcenter = -0.5*fNveto_seg_TotalPaddleThickness*((Int_t) (fNveto_seg_NsinglePlanes - 1));
            zcenter = zcenter + ((Int_t) kplane)*fNveto_seg_TotalPaddleThickness;
            
            // For xcenter and ycenter of the bars we need to distinguish between
            // horizontal and vertical planes. The first plane has kplane = 0 and
            // is horizontal:
            if ((kplane%2)==0)
            {
                // horizontal plane: ycenter should differ and xcenter should
                // be all zeros:
                xcenter = 0.0;
                ycenter = -0.5*fNveto_seg_TotalPaddleWidth*((Int_t) (fNveto_seg_PaddlesPerSinglePlane - 1));
                ycenter = ycenter + ((Int_t) kbar)*fNveto_seg_TotalPaddleWidth;
            }
            else
            {
                // vertical plane: just the opposite:
                ycenter = 0.0;
                xcenter = -0.5*fNveto_seg_TotalPaddleWidth*((Int_t) (fNveto_seg_PaddlesPerSinglePlane - 1));
                xcenter = xcenter + ((Int_t) kbar)*fNveto_seg_TotalPaddleWidth;
            }
            
            // Now we need to add the uniform distribution if this is required:
            if (fNveto_seg_UseUnformDist)
            {
                zcenter = zcenter + fNveto_seg_RandGen->Uniform(fNveto_seg_TotalPaddleThickness) - 0.5*fNveto_seg_TotalPaddleThickness;
                
                if ((kplane%2)==0)
                {
                    xcenter = xcenter + fNveto_seg_RandGen->Uniform(fNveto_seg_TotalPaddleLength) - 0.5*fNveto_seg_TotalPaddleLength;
                    ycenter = ycenter + fNveto_seg_RandGen->Uniform(fNveto_seg_TotalPaddleWidth) - 0.5*fNveto_seg_TotalPaddleWidth;
                }
                else
                {
                    ycenter = ycenter + fNveto_seg_RandGen->Uniform(fNveto_seg_TotalPaddleLength) - 0.5*fNveto_seg_TotalPaddleLength;
                    xcenter = xcenter + fNveto_seg_RandGen->Uniform(fNveto_seg_TotalPaddleWidth) - 0.5*fNveto_seg_TotalPaddleWidth;
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


    for (const auto &kv : fDigitizin_segEngine->paddles) {
        const Int_t paddleNr = kv.first;
        const auto &paddle = kv.second;

        if (paddle.HasFired()) {

            // Get position and other information and fill digis.
            Double_t xx, yy, zz;

            if (fveto_segDigiPar->GetGeometryFileName().Contains("proto")) {
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
pos.RotateX(fNveto_seg_Global_XRotation);
pos.RotateY(fNveto_seg_Global_YRotation);
pos.RotateZ(fNveto_seg_Global_ZRotation);
xx = pos.X() + fNveto_seg_Global_Xpos;
yy = pos.Y() + fNveto_seg_Global_Ypos;
zz = pos.Z() + fNveto_seg_Global_Zpos;            Double_t tdcR = paddle.rightPMT.GetTDC();
            Double_t qdcL = paddle.leftPMT.GetEnergy();
            Double_t qdcR = paddle.rightPMT.GetEnergy();

            Double_t qdc = paddle.GetPaddleEnergy();
            Double_t tdc = paddle.GetPaddleTime();

            new((*fveto_segDigi)[fveto_segDigi->GetEntriesFast()]) R3Bveto_segDigi(paddleNr,
                    tdcL, tdcR, tdc,
                    qdcL, qdcR, qdc,
                    xx, yy, zz);
        }
    } // loop over paddles



    if (fVerbose) {
        LOG(INFO) << "R3BNveto_segDigitizrOld: produced "
                  << fveto_segDigi->GetEntries() << " digis" << FairLogger::endl;
    }
}


void R3BNveto_segDigitizrOld::Reset()
{
    fveto_segDigi->Clear();
    fDigitizin_segEngine->Clear();
}


void R3BNveto_segDigitizrOld::Finish()
{
    hPMl->Write();
    hPMr->Write();
    hMultOne->Write();
    hMultTwo->Write();
    hRLTimeToTrig->Write();
}


ClassImp(R3BNveto_segDigitizrOld);
