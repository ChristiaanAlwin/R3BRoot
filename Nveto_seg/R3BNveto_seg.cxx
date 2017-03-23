#include "R3BNveto_seg.h"

#include "R3BNveto_segPoint.h"
#include "R3BNveto_segGeoPar.h"
#include "R3BMCStack.h"

#include "FairRun.h"
#include "FairRuntimeDb.h"
#include "FairMCPoint.h"

#include "TClonesArray.h"
#include "TObjArray.h"
#include "TParticle.h"
#include "TVirtualMC.h"

#include "TGeoManager.h"
#include "TGeoBBox.h"

// Initialise variables from Birk' s Law
static constexpr Double_t BirkdP = 1.032;
static constexpr Double_t BirkC1 = 0.013 / BirkdP;
static constexpr Double_t BirkC2 = 9.6e-6 / (BirkdP * BirkdP);

inline Double_t GetLightYield(const Int_t charge, const Double_t length, const Double_t edep)
{
    // Apply Birk's law ( Adapted from G3BIRK/Geant3)
    if (charge != 0 && length > 0)
    {
        Double_t birkC1Mod = BirkC1;

        // Apply correction for higher charge states
        if (TMath::Abs(charge) >= 2)
        {
            birkC1Mod *= 7.2 / 12.6;
        }

        Double_t dedxcm = 1000. * edep / length;
        Double_t lightYield = edep / (1. + birkC1Mod * dedxcm + BirkC2 * dedxcm * dedxcm);
        return lightYield;
    }
    return 0.;
}

R3BNveto_seg::R3BNveto_seg()
    : R3BDetector("R3BNveto_seg", kTRUE, kNVETO_SEG)
    , fNveto_segPoints(new TClonesArray("R3BNveto_segPoint"))
    , fNveto_segPrimaryNeutronInteractionPoints(new TClonesArray("FairMCPoint"))
{
}

R3BNveto_seg::R3BNveto_seg(const char* name, Bool_t active)
    : R3BDetector(name, active, kNVETO_SEG)
    , fNveto_segPoints(new TClonesArray("R3BNveto_segPoint"))
    , fNveto_segPrimaryNeutronInteractionPoints(new TClonesArray("FairMCPoint"))
{
}

void R3BNveto_seg::Initialize()
{
    LOG(INFO) << "R3BNveto_seg initialization ..." << FairLogger::endl;

    FairDetector::Initialize();

    WriteParameterFile();
    ResetValues();
}

Bool_t R3BNveto_seg::ProcessHits(FairVolume*)
{
    // New hit in detector
    if (gMC->IsTrackEntering())
    {
        if (!fLastHitDone)
        {
            LOG(WARNING) << "R3BNveto_seg: Incomplete hit discarded" << FairLogger::endl;
            ResetValues();
        }

        fLastHitDone = kFALSE;
        fELoss = 0.;
        fLightYield = 0.;
        fTime = gMC->TrackTime() * 1.0e09;
        fLength = gMC->TrackLength();
        gMC->TrackPosition(fPosIn);
        gMC->TrackMomentum(fMomIn);
        gMC->CurrentVolOffID(1, fPaddleID);
    }

    // Sum energy loss for all steps in the active volume
    fELoss += gMC->Edep();
    fLightYield += GetLightYield(gMC->TrackCharge(), gMC->TrackStep(), gMC->Edep());

    // Set additional parameter_seg at exit of active volume. Create R3BNveto_segPoint.
    if (gMC->IsTrackExiting() || gMC->IsTrackStop() || gMC->IsTrackDisappeared())
    {

        // Do not save a hit if no energy deposited
        if (fELoss == 0.)
        {
            ResetValues();
            return kFALSE;
        }

        fTrackID = gMC->GetStack()->GetCurrentTrackNumber();
        gMC->TrackPosition(fPosOut);
        gMC->TrackMomentum(fMomOut);

        // Add Point
        LOG(DEBUG) << "R3BNveto_seg: Adding Point at (" << fPosIn.X() << ", " << fPosIn.Y() << ", " << fPosIn.Z()
                   << ") cm,  paddle " << fPaddleID << ", track " << fTrackID << ", energy loss " << fELoss << " GeV "
                   << gMC->GetStack()->GetCurrentParentTrackNumber() << FairLogger::endl;

        Int_t size = fNveto_segPoints->GetEntriesFast();
        new ((*fNveto_segPoints)[size]) R3BNveto_segPoint(fTrackID,
                                                      fPaddleID,
                                                      fPosIn.Vect(),
                                                      fMomIn.Vect(),
                                                      fTime,
                                                      fLength,
                                                      fELoss,
                                                      gMC->CurrentEvent(),
                                                      fLightYield);

        // Increment number of veto_segPoints for this track
        R3BStack* stack = (R3BStack*)gMC->GetStack();
        stack->AddPoint(kNVETO_SEG);
        ResetValues();
    }

    return kTRUE;
}

void R3BNveto_seg::PostTrack()
{
    // Do NOT use FinishPrimary() -> different track numbering!
    // Look for primary neutron
    if (gMC->GetStack()->GetCurrentParentTrackNumber() == -1 &&
        gMC->GetStack()->GetCurrentTrack()->GetPdgCode() == 2112)
    {

        // Check if the primary neutron track ended in a volume of Nveto_seg
        auto volName = std::string(gMC->CurrentVolName());
        if (volName == "vollBC408_seg" || volName == "vollAlWrapping_seg" || volName == "vollTapeWrapping_seg")
        {
            TLorentzVector pos;
            gMC->TrackPosition(pos);
            TLorentzVector mom;
            gMC->TrackMomentum(mom);
            Int_t paddleID;
            gMC->CurrentVolOffID(1, paddleID);

            LOG(DEBUG) << "R3BNveto_seg::PostTrack()"
                       << " TrackNumber: " << ((R3BStack*)gMC->GetStack())->GetCurrentTrackNumber() << " XYZTE "
                       << pos.X() << " " << pos.Y() << " " << pos.Z() << " " << pos.T() << " " << mom.E()
                       << " VolName: " << gMC->CurrentVolName() << " PaddleID: " << paddleID << FairLogger::endl;

            Int_t size = fNveto_segPrimaryNeutronInteractionPoints->GetEntriesFast();
            new ((*fNveto_segPrimaryNeutronInteractionPoints)[size]) FairMCPoint(gMC->GetStack()->GetCurrentTrackNumber(),
                                                                               paddleID,
                                                                               pos.Vect(),
                                                                               mom.Vect(),
                                                                               pos.T() * 1.0e09,
                                                                               gMC->TrackLength(),
                                                                               mom.E(),
                                                                               gMC->CurrentEvent());
        }
    }
}

Bool_t R3BNveto_seg::CheckIfSensitive(std::string name) { return name == "vollBC408_seg"; }

void R3BNveto_seg::EndOfEvent()
{
    if (fVerboseLevel)
    {
        Print();
    }
    Reset();
}

TClonesArray* R3BNveto_seg::GetCollection(Int_t iColl) const
{
    if (iColl == 0)
    {
        // return fNveto_segPoints.get();
        return fNveto_segPoints;
    }
    return nullptr;
}

void R3BNveto_seg::Register()
{
    // FairRootManager::Instance()->Register("Nveto_segPoints", GetName(), fNveto_segPoints.get(), kTRUE);
    FairRootManager::Instance()->Register("Nveto_segPoints", GetName(), fNveto_segPoints, kTRUE);
    // FairRootManager::Instance()->Register("Nveto_segPrimaryNeutronInteractionPoints", GetName(),
    // fNveto_segPrimaryNeutronInteractionPoints.get(), kTRUE);
    FairRootManager::Instance()->Register(
        "Nveto_segPrimaryNeutronInteractionPoints", GetName(), fNveto_segPrimaryNeutronInteractionPoints, kTRUE);
}

void R3BNveto_seg::Print(Option_t*) const
{
    LOG(INFO) << "R3BNveto_seg: " << fNveto_segPoints->GetEntries() << " Nveto_seg Points registered in this event"
              << FairLogger::endl;
    LOG(INFO) << "R3BNveto_seg: " << fNveto_segPrimaryNeutronInteractionPoints->GetEntries()
              << " Nveto_seg Primary Neutron Interaction Points registered in this event" << FairLogger::endl;
}

void R3BNveto_seg::Reset()
{
    fNveto_segPoints->Clear();
    fNveto_segPrimaryNeutronInteractionPoints->Clear();
    ResetValues();
}

void R3BNveto_seg::ConstructGeometry()
{
    TString fileName = GetGeometryFileName();
    if (fileName.EndsWith(".root"))
    {
        LOG(INFO) << "Constructing (Neu)VETO_SEG geometry from ROOT file " << fileName.Data() << FairLogger::endl;
        ConstructRootGeometry();
    }
    else
    {
        LOG(FATAL) << "Geometry file name is not set" << FairLogger::endl;
    }
}

void R3BNveto_seg::ResetValues()
{
    fLastHitDone = kTRUE;
    fTrackID = 0;
    fPaddleID = -1;
    fPosIn.Clear();
    fPosOut.Clear();
    fMomIn.Clear();
    fMomOut.Clear();
    fTime = fLength = fELoss = fLightYield = 0;
}

void R3BNveto_seg::WriteParameterFile()
{

    FairRuntimeDb* rtdb = FairRun::Instance()->GetRuntimeDb();
    R3BNveto_segGeoPar* par = (R3BNveto_segGeoPar*)rtdb->getContainer("R3BNveto_segGeoPar");

    // Really bad way to find the Nveto_seg *node* (not the volume!)
    TGeoNode* geoNodeNveto_seg = nullptr;
    for (Int_t i = 0; i < gGeoManager->GetTopNode()->GetNdaughters(); i++)
    {
        if (TString(gGeoManager->GetTopNode()->GetDaughter(i)->GetVolume()->GetName()) == "volNveto_seg")
        {
            geoNodeNveto_seg = gGeoManager->GetTopNode()->GetDaughter(i);
            break;
        }
    }

    if (!geoNodeNveto_seg)
    {
        LOG(FATAL) << "volNveto_seg not found" << FairLogger::endl;
    }

    par->SetNveto_segGeoNode(geoNodeNveto_seg);
    par->setChanged();
}

ClassImp(R3BNveto_seg);
