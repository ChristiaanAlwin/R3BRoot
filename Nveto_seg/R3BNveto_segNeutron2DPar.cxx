#include "R3BNveto_segNeutron2DPar.h"

#include <iostream>
#include <stdexcept>
#include "FairParamList.h"
#include "TObjString.h"

R3BNveto_segNeutron2DPar::R3BNveto_segNeutron2DPar(const char* name, const char* title, const char* context)
    : FairParGenericSet(name, title, context)
    , fNeutronCuts(nullptr)
{
}

R3BNveto_segNeutron2DPar::~R3BNveto_segNeutron2DPar()
{
    // Note: Deleting stuff here or in clear() causes segfaults?
}

void R3BNveto_segNeutron2DPar::clear() {}

void R3BNveto_segNeutron2DPar::putParams(FairParamList* l)
{
    if (!l)
    {
        return;
    }
    l->addObject("Nveto_segNeutronCuts", fNeutronCuts);
}

Bool_t R3BNveto_segNeutron2DPar::getParams(FairParamList* l)
{
    if (!l)
    {
        return kFALSE;
    }
    if (!l->fillObject("Nveto_segNeutronCuts", fNeutronCuts))
    {
        return kFALSE;
    }
    return kTRUE;
}

void R3BNveto_segNeutron2DPar::printParams()
{
    std::cout << "R3BNveto_segNeutron2DPar: Nveto_seg Neutron Cuts ..." << std::endl;
    for (const auto& nc : GetNeutronCuts())
    {
        std::cout << nc.first << std::endl;
        nc.second->Print();
    }
}

void R3BNveto_segNeutron2DPar::SetNeutronCuts(const std::map<UInt_t, TCutG*>& cuts)
{
    fNeutronCuts = new TMap();
    fNeutronCuts->SetOwner(kTRUE);
    for (const auto& nc : cuts)
    {
        TObjString* key = new TObjString(TString::Itoa(nc.first, 10));
        fNeutronCuts->Add(key, nc.second->Clone());
    }
}

std::map<UInt_t, TCutG*> R3BNveto_segNeutron2DPar::GetNeutronCuts() const
{
    if (fNeutronCuts == nullptr)
    {
        throw std::runtime_error("R3BNveto_segNeutron2DPar: NeutronCuts not set!");
    }

    std::map<UInt_t, TCutG*> map;

    TObjString* key;
    TIterator* nextobj = fNeutronCuts->MakeIterator();
    while ((key = (TObjString*)nextobj->Next()))
    {
        UInt_t nNeutrons = key->GetString().Atoi();
        map[nNeutrons] = (TCutG*)fNeutronCuts->GetValue(key)->Clone();
    }

    return map;
}

UInt_t R3BNveto_segNeutron2DPar::GetNeutronMultiplicity(const Double_t energy, const Double_t nClusters) const
{
    // Note: it might be better to implement std::map as a member and sync between the tmap for this type of usage.
    if (fNeutronCuts == nullptr)
    {
        throw std::runtime_error("R3BNveto_segNeutron2DPar: NeutronCuts not set!");
    }

    TObjString* key;
    TIterator* nextobj = fNeutronCuts->MakeIterator();
    while ((key = (TObjString*)nextobj->Next()))
    {
        // UInt_t nNeutrons = key->GetString().Atoi();
        if (((TCutG*)fNeutronCuts->GetValue(key))->IsInside(energy, nClusters))
        {
            return (UInt_t)key->GetString().Atoi();
        }
    }
    // The list of cuts does contain a cut for multiplicity 0, so if no match is found, the neutron multiplicity must be
    // higher than the highest saved cut.
    return GetNeutronCuts().rbegin()->first + 1;
}

ClassImp(R3BNveto_segNeutron2DPar);
