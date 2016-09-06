#include "R3BNveto_segGeoPar.h"

#include <iostream>

#include "TGeoMatrix.h"
#include "TVector3.h"
#include "TGeoBBox.h"

#include "FairParamList.h"

R3BNveto_segGeoPar::R3BNveto_segGeoPar(const char* name, const char* title, const char* context)
    : FairParGenericSet(name, title, context)
    , fNveto_segGeoNode(nullptr)
{
}

R3BNveto_segGeoPar::~R3BNveto_segGeoPar()
{
    // Note: Deleting stuff here or in clear() causes segfaults?
}

void R3BNveto_segGeoPar::clear() {}

void R3BNveto_segGeoPar::putParams(FairParamList* l)
{
    if (!l)
    {
        return;
    }
    l->addObject("Nveto_segGeoNode", fNveto_segGeoNode);
}

Bool_t R3BNveto_segGeoPar::getParams(FairParamList* l)
{
    if (!l)
    {
        return kFALSE;
    }
    if (!l->fillObject("Nveto_segGeoNode", fNveto_segGeoNode))
    {
        return kFALSE;
    }
    BuildPaddleLookup();
    return kTRUE;
}

void R3BNveto_segGeoPar::printParams()
{
    std::cout << "R3BNveto_segGeoPar: Nveto_seg Paddle Positions ..." << std::endl;

    fNveto_segGeoNode->GetMatrix()->Print();

    for (Int_t i = 0; i < fNveto_segGeoNode->GetNdaughters(); i++)
    {
        TGeoNode* node = fNveto_segGeoNode->GetDaughter(i);
        std::cout << node->GetNumber() << std::endl;
        node->GetMatrix()->Print();
    }
}

void R3BNveto_segGeoPar::SetNveto_segGeoNode(const TGeoNode* const p)
{
    fNveto_segGeoNode = (TGeoNode*)p->Clone();
    BuildPaddleLookup();
}

Double_t R3BNveto_segGeoPar::GetPaddleHalfLength() const
{
    // All paddles have to have the same length
    return ((TGeoBBox*)fNveto_segGeoNode->GetDaughter(0)->GetVolume()->GetShape())->GetDX();
}

// Convert positions of e.g. points to the local coordinate of the respective paddle [(-135,135),(-2.5,2.5),(-2.5,2.5)]
TVector3 R3BNveto_segGeoPar::ConvertToLocalCoordinates(const TVector3& position, const Int_t paddleID) const
{
    Double_t pos_in[3] = { position.X(), position.Y(), position.Z() };
    Double_t pos_tmp[3];
    Double_t pos_out[3];

    // First, convert to Nveto_seg-local coordinates (consisting of all paddles)
    fNveto_segGeoNode->GetMatrix()->MasterToLocal(pos_in, pos_tmp);
    // Second, convert to the repective paddle
    fPaddleGeoNodes.at(paddleID)->MasterToLocal(pos_tmp, pos_out);

    return TVector3(pos_out[0], pos_out[1], pos_out[2]);
}

TVector3 R3BNveto_segGeoPar::ConvertToGlobalCoordinates(const TVector3& position, const Int_t paddleID) const
{
    Double_t pos_in[3] = { position.X(), position.Y(), position.Z() };
    Double_t pos_tmp[3];
    Double_t pos_out[3];

    // Note reverse order of Global->Local
    fPaddleGeoNodes.at(paddleID)->LocalToMaster(pos_in, pos_tmp);

    fNveto_segGeoNode->GetMatrix()->LocalToMaster(pos_tmp, pos_out);

    return TVector3(pos_out[0], pos_out[1], pos_out[2]);
}

void R3BNveto_segGeoPar::BuildPaddleLookup()
{
    for (Int_t i = 0; i < fNveto_segGeoNode->GetNdaughters(); i++)
    {
        TGeoNode* node = fNveto_segGeoNode->GetDaughter(i);
        fPaddleGeoNodes[node->GetNumber()] = node;
    }
}

ClassImp(R3BNveto_segGeoPar);
