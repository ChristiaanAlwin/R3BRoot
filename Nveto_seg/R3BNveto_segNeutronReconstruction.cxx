#include "R3BNveto_segNeutronReconstruction.h"

#include <iostream>
#include <algorithm>
#include <numeric>
#include <cmath>

#include "TClonesArray.h"

#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "FairRtdbRun.h"
#include "FairLogger.h"

#include "R3BNveto_segNeutron2DPar.h"
#include "R3BNveto_segCluster.h"
#include "R3BNveto_segNeutron.h"

R3BNveto_segNeutronReconstruction::R3BNveto_segNeutronReconstruction()
    : FairTask("R3B Nveto_seg Nveto_segCluster Monitor")
    , fNeutrons(new TClonesArray("R3BNveto_segNeutron"))
{
    LOG(INFO) << "Using R3B Nveto_seg Neutron Reconstruction" << FairLogger::endl;
}

R3BNveto_segNeutronReconstruction::~R3BNveto_segNeutronReconstruction() {}

void R3BNveto_segNeutronReconstruction::SetParContainers()
{
    FairRunAna* run = FairRunAna::Instance();
    if (!run)
    {
        LOG(FATAL) << "R3BNveto_segNeutronReconstruction::SetParContainers: No analysis run" << FairLogger::endl;
        return;
    }

    FairRuntimeDb* rtdb = run->GetRuntimeDb();
    if (!rtdb)
    {
        LOG(FATAL) << "R3BNveto_segNeutronReconstruction::SetParContainers: No runtime database" << FairLogger::endl;
        return;
    }

    fNveto_segNeutron2DPar = (R3BNveto_segNeutron2DPar*)rtdb->getContainer("R3BNveto_segNeutron2DPar");
    if (fNveto_segNeutron2DPar == nullptr)
    {
        LOG(FATAL) << "R3BNveto_segNeutronReconstruction::SetParContainers: No R3BNveto_segNeutron2DPar"
                   << FairLogger::endl;
        return;
    }
}

InitStatus R3BNveto_segNeutronReconstruction::Init()
{
    FairRootManager* ioman = FairRootManager::Instance();
    if (!ioman)
    {
        LOG(FATAL) << "R3BNveto_segNeutronReconstruction::Init: No FairRootManager" << FairLogger::endl;
        return kFATAL;
    }

    // Set Input: TClonesArray of R3BNveto_segClusters
    if ((TClonesArray*)ioman->GetObject("Nveto_segClusters") == nullptr)
    {
        LOG(FATAL) << "R3BNveto_segNeutronReconstruction::Init No Nveto_segClusters!" << FairLogger::endl;
        return kFATAL;
    }
    if (!TString(((TClonesArray*)ioman->GetObject("Nveto_segClusters"))->GetClass()->GetName())
             .EqualTo("R3BNveto_segCluster"))
    {
        LOG(FATAL)
            << "R3BNveto_segNeutronReconstruction::Init Branch Nveto_segClusters does not contain R3BNveto_segClusters!"
            << FairLogger::endl;
        return kFATAL;
    }
    fClusters = (TClonesArray*)ioman->GetObject("Nveto_segClusters");

    // Set Output: TClonesArray of R3BNveto_segNeutrons
    ioman->Register("Nveto_segNeutrons", "Neutrons detected in Nveto_seg", fNeutrons, kTRUE);

    // FIXME: FairRuntimeDB needs to be forced to load the Data from the second file with Run Id 1
    FairRuntimeDb::instance()->initContainers(FairRuntimeDb::instance()->getCurrentRun()->getRunId(), 1);
    fNveto_segNeutron2DPar->printParams();

    return kSUCCESS;
}

void R3BNveto_segNeutronReconstruction::Exec(Option_t*)
{
    Reset();

    const UInt_t nClusters = fClusters->GetEntries();

    std::vector<R3BNveto_segCluster*> clusters;
    clusters.reserve(nClusters);
    for (UInt_t i = 0; i < nClusters; i++)
    {
        clusters.push_back((R3BNveto_segCluster*)fClusters->At(i));
    }

    const Double_t Etot = std::accumulate(clusters.begin(),
                                          clusters.end(),
                                          Double_t(0.),
                                          [](const Double_t a, const R3BNveto_segCluster* b)
                                          {
                                              return a + b->GetE();
                                          });

    // Get Number of neutrons using the previously calibrated cuts stored in the parameter file
    const UInt_t nNeutrons = fNveto_segNeutron2DPar->GetNeutronMultiplicity(Etot, nClusters);

    // WIP: Sort the digis by likeliness of being a first neutron interaction descending
    std::sort(clusters.begin(),
              clusters.end(),
              [](const R3BNveto_segCluster* a, const R3BNveto_segCluster* b)
              {
                  return a->GetT() < b->GetT();
              });

    for (UInt_t n = 0; n < nNeutrons && n < clusters.size(); n++)
    {
        const R3BNveto_segDigi digi = clusters.at(n)->GetFirstDigi();

        // WIP: ToF Calculation -> Should respect other origin than 0,0,0,0.
        const Double_t s2 = std::pow(digi.GetPosition().X(), 2) + std::pow(digi.GetPosition().Y(), 2) +
                            std::pow(digi.GetPosition().Z(), 2); // cm²
        const Double_t v2 = s2 / std::pow(digi.GetT(), 2);       // ns²

        const Double_t c2 = 898.8;            // cm²/ns²
        const Double_t massNeutron = 939.565; // MeV/c²
        const Double_t ETimeOfFlight = massNeutron * ((1. / std::sqrt(1 - (v2 / c2))) - 1);

        new ((*fNeutrons)[fNeutrons->GetEntries()]) R3BNveto_segNeutron(digi.GetT(), ETimeOfFlight, digi.GetPosition());
    }
}

void R3BNveto_segNeutronReconstruction::Reset() { fNeutrons->Clear(); }

void R3BNveto_segNeutronReconstruction::Finish() {}

ClassImp(R3BNveto_segNeutronReconstruction)
