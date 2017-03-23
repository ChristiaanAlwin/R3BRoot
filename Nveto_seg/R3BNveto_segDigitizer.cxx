#include "R3BNveto_segDigitizer.h"

#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>

#include "TGeoManager.h"
#include "TClonesArray.h"
#include "TMath.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TString.h"
#include "TGeoNode.h"

#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "FairLogger.h"

R3BNveto_segDigitizer::R3BNveto_segDigitizer()
    : FairTask("R3B Nveto_seg Digitizer", 0)
    , fNveto_segDigis(new TClonesArray("R3BNveto_segDigi"))
    , fDigitizin_segEngine(new Nveto_seg::Digitizin_segEngine())
{
}

R3BNveto_segDigitizer::~R3BNveto_segDigitizer()
{
    if (fNveto_segDigis)
    {
        delete fNveto_segDigis;
    }
    if (fDigitizin_segEngine)
    {
        delete fDigitizin_segEngine;
    }
}

void R3BNveto_segDigitizer::SetParContainers()
{
    FairRunAna* run = FairRunAna::Instance();
    if (!run)
    {
        LOG(FATAL) << "R3BNveto_segDigitizer::SetParContainers: No analysis run" << FairLogger::endl;
        return;
    }

    FairRuntimeDb* rtdb = run->GetRuntimeDb();
    if (!rtdb)
    {
        LOG(FATAL) << "R3BNveto_segDigitizer::SetParContainers: No runtime database" << FairLogger::endl;
        return;
    }

    fNveto_segGeoPar = (R3BNveto_segGeoPar*)rtdb->getContainer("R3BNveto_segGeoPar");
    if (!fNveto_segGeoPar)
    {
        LOG(FATAL) << "R3BNveto_segDigitizer::SetParContainers: No R3BNveto_segGeoPar" << FairLogger::endl;
        return;
    }
}

InitStatus R3BNveto_segDigitizer::Init()
{
    FairRootManager* ioman = FairRootManager::Instance();
    if (!ioman)
    {
        LOG(FATAL) << "R3BNveto_segDigitizer::Init: No FairRootManager" << FairLogger::endl;
        return kFATAL;
    }

    // Set Input: TClonesArray of R3BNveto_segPoints
    if ((TClonesArray*)ioman->GetObject("Nveto_segPoints") == nullptr)
    {
if ((TClonesArray*)ioman->GetObject("veto_segPoints") == nullptr)
{
        LOG(FATAL) << "R3BNveto_segDigitizer::Init No Nveto_segPoints!" << FairLogger::endl;
        return kFATAL;
}
    }
    if (!TString(((TClonesArray*)ioman->GetObject("Nveto_segPoints"))->GetClass()->GetName()).EqualTo("R3BNveto_segPoint"))
    {
if (!TString(((TClonesArray*)ioman->GetObject("veto_segPoints"))->GetClass()->GetName()).EqualTo("R3Bveto_segPoint"))
{
        LOG(FATAL) << "R3BNveto_segDigitizer::Init Branch Nveto_segPoints does not contain R3BNveto_segPoints!"
                   << FairLogger::endl;
        return kFATAL;
}
    }
    fNveto_segPoints = (TClonesArray*)ioman->GetObject("Nveto_segPoints");
if (fNveto_segPoints == nullptr) {fNveto_segPoints = (TClonesArray*)ioman->GetObject("veto_segPoints");}
TFile* inputs = new TFile("../../Data/Inputs/Inputs.root","read");
TH1I* Integers = (TH1I*) inputs->Get("Integers");
TH1D* Doubles = (TH1D*) inputs->Get("Doubles");

    // Set Output: TClonesArray of R3BNveto_segDigis
    ioman->Register("Nveto_segDigis", "Digital response in Nveto_seg", fNveto_segDigis, kTRUE);

    // Get Paddle Size
Nveto_seg::Digitizin_segEngine::fTimeRes = 0.001*Doubles->GetBinContent(310); // NOTE: conversion from [ps] to [ns]!!!
Nveto_seg::Digitizin_segEngine::fThresh = 0.001*Doubles->GetBinContent(302); // NOTE: conversion from [keV] to [MeV]!!!
Nveto_seg::Digitizin_segEngine::fAttenuation = Doubles->GetBinContent(335);
Nveto_seg::Digitizin_segEngine::fcMedium = Doubles->GetBinContent(334);
// fixed!
    LOG(DEBUG) << "R3BNveto_segDigitizer: Paddle Half Length is: " << fNveto_segGeoPar->GetPaddleHalfLength()
               << FairLogger::endl;
    fDigitizin_segEngine->SetPaddleHalfLength(fNveto_segGeoPar->GetPaddleHalfLength());

    // Initialise control histograms
    hPMl = new TH1F("PM_left", "Arrival times of left PM", 1000, 0., 1000.);
    hPMr = new TH1F("PM_right", "Arrival times of right PM", 1000, 0., 1000.);
    hMultOne = new TH1F("MultiplicityOne", "Paddle multiplicity: only one PMT per paddle", 3000, 0, 3000);
    hMultTwo = new TH1F("MultiplicityTwo", "Paddle multiplicity: both PMTs of a paddle", 3000, 0, 3000);
    hRLTimeToTrig = new TH1F("hRLTimeToTrig", "R/Ltime-triggerTime", 200, -100, 100);
// Replace the detector characteristics with the choices from our Inputs.root-file:

    hElossVSQDC = new TH2F("hElossVSQDC", "Energy loss in a paddle vs paddle qdc value", 1000, 0, 1000, 1000, 0, 100);
    hElossVSQDC->GetXaxis()->SetTitle("Deposited Energy [MeV]");
    hElossVSQDC->GetYaxis()->SetTitle("Paddle QDC [a.u.]");

    return kSUCCESS;
}

void R3BNveto_segDigitizer::Exec(Option_t*)
{
    Reset();

    std::map<UInt_t, Double_t> paddleEnergyDeposit;

    // Look at each veto_seg Point, if it deposited energy in the szintillator, store it with reference to the bar
    const UInt_t nPoints = fNveto_segPoints->GetEntries();
    R3BNveto_segPoint* point;
    for (UInt_t l = 0; l < nPoints; l++)
    {
        point = (R3BNveto_segPoint*)fNveto_segPoints->At(l);

        if (point->GetEnergyLoss() > 0.)
        {
            const Int_t paddleID = point->GetPaddle();

            // Convert position of point to paddle-coordinates, including any rotation or translation
            const TVector3 position = point->GetPosition();
            const TVector3 converted_position = fNveto_segGeoPar->ConvertToLocalCoordinates(position, paddleID);
            LOG(DEBUG) << "Nveto_segDigitizer: Point in paddle " << paddleID
                       << " with global position XYZ: " << position.X() << " " << position.Y() << " " << position.Z()
                       << FairLogger::endl;
            LOG(DEBUG) << "Nveto_segDigitizer: Converted to local position XYZ: " << converted_position.X() << " "
                       << converted_position.Y() << " " << converted_position.Z() << FairLogger::endl;

            // Within the paddle frame, the relevant distance of the light from the pmt is always given by the
            // X-Coordinate
            const Double_t dist = converted_position.X();
            try
            {
                fDigitizin_segEngine->DepositLight(paddleID, point->GetTime(), point->GetLightYield() * 1000., dist);
                paddleEnergyDeposit[paddleID] += point->GetEnergyLoss() * 1000;
            }
            catch (std::exception& e)
            {
                LOG(FATAL) << "Nveto_segDigitizer: " << e.what() << FairLogger::endl;
            }

        } // eloss
    }     // MC hits

    // Fill histograms
    Int_t multOne = 0;
    Int_t multTwo = 0;
    const Double_t triggerTime = fDigitizin_segEngine->GetTriggerTime();
    for (const auto& kv : fDigitizin_segEngine->paddles)
    {
        const auto& paddle = kv.second;

        for (const auto& hit : paddle.leftPMT.GetHits())
        {
            hPMl->Fill(hit.time);
        }
        for (const auto& hit : paddle.rightPMT.GetHits())
        {
            hPMr->Fill(hit.time);
        }

        // Multiplicity if only PMT has fired
        if (paddle.leftPMT.HasFired() && !paddle.rightPMT.HasFired())
        {
            multOne++;
        }
        if (!paddle.leftPMT.HasFired() && paddle.rightPMT.HasFired())
        {
            multOne++;
        }

        // Multiplicity if two PMT have fired
        if (paddle.leftPMT.HasFired() && paddle.rightPMT.HasFired())
        {
            multTwo++;
            hRLTimeToTrig->Fill(paddle.leftPMT.GetTDC() - triggerTime);
            hRLTimeToTrig->Fill(paddle.rightPMT.GetTDC() - triggerTime);
        }
    } // loop over paddles
    hMultOne->Fill(multOne);
    hMultTwo->Fill(multTwo);

    // Create Digis
    for (const auto& kv : fDigitizin_segEngine->paddles)
    {
        const Int_t paddleID = kv.first;
        const auto& paddle = kv.second;

        if (paddle.HasFired())
        {
            const TVector3 digiPositionLocal = TVector3(paddle.GetPosition(), 0., 0.);
            const TVector3 digiPositionGlobal = fNveto_segGeoPar->ConvertToGlobalCoordinates(digiPositionLocal, paddleID);

            new ((*fNveto_segDigis)[fNveto_segDigis->GetEntries()]) R3BNveto_segDigi(paddleID,
                                                                               paddle.leftPMT.GetTDC(),
                                                                               paddle.rightPMT.GetTDC(),
                                                                               paddle.GetPaddleTime(),
                                                                               paddle.leftPMT.GetEnergy(),
                                                                               paddle.rightPMT.GetEnergy(),
                                                                               paddle.GetPaddleEnergy(),
                                                                               digiPositionGlobal);

            hElossVSQDC->Fill(paddleEnergyDeposit[paddleID], paddle.GetPaddleEnergy());
        }
    } // loop over paddles

    if (fVerbose)
    {
        LOG(INFO) << "R3BNveto_segDigitizer: produced " << fNveto_segDigis->GetEntries() << " digis" << FairLogger::endl;
    }
}

void R3BNveto_segDigitizer::Reset()
{
    fNveto_segDigis->Clear();
    fDigitizin_segEngine->Clear();
}

void R3BNveto_segDigitizer::Finish()
{
    hPMl->Write();
    hPMr->Write();
    hMultOne->Write();
    hMultTwo->Write();
    hRLTimeToTrig->Write();
    hElossVSQDC->Write();
}

ClassImp(R3BNveto_segDigitizer);
