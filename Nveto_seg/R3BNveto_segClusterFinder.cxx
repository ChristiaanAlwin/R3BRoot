#include <iostream>
#include <algorithm>

#include "TClonesArray.h"
#include "TMath.h"
#include "TH1F.h"
#include "TH2F.h"

#include "FairRootManager.h"
#include "FairLogger.h"

#include "R3BNveto_segDigi.h"
#include "R3BNveto_segCluster.h"
#include "R3BNveto_segClusterFinder.h"

#include "NvetoClustringEngine.h"

/* Clustering function for R3BNveto_segDigis.
 * Returns true if the Digis belong together in the same cluster and false otherwise. */
static const auto clusteringCondition = [](const R3BNveto_segDigi& a, const R3BNveto_segDigi& b)
{
    return TMath::Abs(a.GetPosition().X() - b.GetPosition().X()) < 7.5 &&
           TMath::Abs(a.GetPosition().Y() - b.GetPosition().Y()) < 7.5 &&
           TMath::Abs(a.GetPosition().Z() - b.GetPosition().Z()) < 7.5 && TMath::Abs(a.GetT() - b.GetT()) < 1.0;
};

R3BNveto_segClusterFinder::R3BNveto_segClusterFinder()
    : FairTask("R3BNveto_segClusterFinder")
    , fNveto_segClusters(new TClonesArray("R3BNveto_segCluster"))
// TODO: #ROOT6
//, fNvetoClustringEngine(new Nveto_seg::NvetoClustringEngine<R3BNveto_segDigi>(clusteringCondition))
{
}

R3BNveto_segClusterFinder::~R3BNveto_segClusterFinder()
{
    if (fNveto_segClusters)
    {
        delete fNveto_segClusters;
    }
    // TODO: #ROOT6
    //    if (fNvetoClustringEngine)
    //   {
    //      delete fNvetoClustringEngine:
    //    }
}

InitStatus R3BNveto_segClusterFinder::Init()
{
    FairRootManager* ioman = FairRootManager::Instance();
    if (!ioman)
    {
        LOG(FATAL) << "R3BNveto_segClusterFinder::Init: No FairRootManager" << FairLogger::endl;
        return kFATAL;
    }

    // Set Input: TClonesArray of R3BNveto_segDigis
    if ((TClonesArray*)ioman->GetObject("Nveto_segDigis") == nullptr)
    {
if ((TClonesArray*)ioman->GetObject("veto_segDigi") == nullptr)
{
        LOG(FATAL) << "R3BNveto_segClusterFinder::Init No Nveto_segDigis!" << FairLogger::endl;
        return kFATAL;
}
    }
    if (!TString(((TClonesArray*)ioman->GetObject("Nveto_segDigis"))->GetClass()->GetName()).EqualTo("R3BNveto_segDigi"))
    {
if (!TString(((TClonesArray*)ioman->GetObject("veto_segDigi"))->GetClass()->GetName()).EqualTo("R3Bveto_segDigi"))
{
        LOG(FATAL) << "R3BNveto_segClusterFinder::Init Branch Nveto_segDigis does not contain R3BNveto_segDigis!"
                   << FairLogger::endl;
        return kFATAL;
}
    }
    fNveto_segDigis = (TClonesArray*)ioman->GetObject("Nveto_segDigis");
if (fNveto_segDigis == nullptr) {fNveto_segDigis = (TClonesArray*)ioman->GetObject("veto_segDigi");}

    // Set Output: TClonesArray of R3BNveto_segDigis
    ioman->Register("Nveto_segClusters", "Clusters in Nveto_seg", fNveto_segClusters, kTRUE);

    return kSUCCESS;
}

void R3BNveto_segClusterFinder::Exec(Option_t*)
{
    Reset();

    // Convert the TClonesArray to a std::vector
    // TODO: #ROOT6 Convert this into a (FairRoot) template function
    const Int_t nDigis = fNveto_segDigis->GetEntries();
    // Note: Reseving space can improve performance
    fVectorDigis.reserve(nDigis);
    R3BNveto_segDigi* digi;
    for (Int_t i = 0; i < nDigis; i++)
    {
        digi = (R3BNveto_segDigi*)fNveto_segDigis->At(i);
        // Note: Derefencing, the vector does contain full objects, not just pointers
        fVectorDigis.push_back(*digi);
    }

    // TODO: #ROOT6 Promote this to a member function so the object does not have to be recreated for every event
    auto fNvetoClustringEngine = Nveto_seg::NvetoClustringEngine<R3BNveto_segDigi>(clusteringCondition);
    auto clusters = fNvetoClustringEngine.Clusterize(fVectorDigis);

    for (auto& cluster : clusters)
    {
        new ((*fNveto_segClusters)[fNveto_segClusters->GetEntries()]) R3BNveto_segCluster(std::move(cluster));
    }
}

void R3BNveto_segClusterFinder::Finish() {}

void R3BNveto_segClusterFinder::Reset()
{
    fVectorDigis.clear();
    fNveto_segClusters->Clear();
}

ClassImp(R3BNveto_segClusterFinder);
