#include "R3BNveto_segClusterMon.h"

#include <iostream>
#include <algorithm>
#include <numeric>

#include "TClonesArray.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TH3D.h"

#include "FairRootManager.h"
#include "FairLogger.h"

#include "R3BNveto_segCluster.h"

R3BNveto_segClusterMon::R3BNveto_segClusterMon(const Option_t* option)
    : FairTask("R3B Nveto_seg Nveto_segCluster Monitor")
{
    LOG(INFO) << "Using R3B Nveto_seg Nveto_segCluster Monitor" << FairLogger::endl;

    TString opt = option;
    opt.ToUpper();

    if (opt.Contains("3DTRACK"))
    {
        fIs3DTrackEnabled = true;
        LOG(INFO) << "... with 3D track visualization" << FairLogger::endl;
    }
    else
    {
        fIs3DTrackEnabled = false;
    }
}

R3BNveto_segClusterMon::~R3BNveto_segClusterMon() {}

InitStatus R3BNveto_segClusterMon::Init()
{
    FairRootManager* ioman = FairRootManager::Instance();
    if (!ioman)
    {
        LOG(FATAL) << "R3BNveto_segClusterMon::Init: No FairRootManager" << FairLogger::endl;
        return kFATAL;
    }

    // Set Input: TClonesArray of R3BNveto_segClusters
    if ((TClonesArray*)ioman->GetObject("Nveto_segClusters") == nullptr)
    {
        LOG(FATAL) << "R3BNveto_segClusterMon::Init No Nveto_segClusters!" << FairLogger::endl;
        return kFATAL;
    }
    if (!TString(((TClonesArray*)ioman->GetObject("Nveto_segClusters"))->GetClass()->GetName())
             .EqualTo("R3BNveto_segCluster"))
    {
        LOG(FATAL) << "R3BNveto_segClusterMon::Init Branch Nveto_segClusters does not contain R3BNveto_segClusters!"
                   << FairLogger::endl;
        return kFATAL;
    }
    fNveto_segClusters = (TClonesArray*)ioman->GetObject("Nveto_segClusters");

    if (fIs3DTrackEnabled)
    {
        // XYZ -> ZXY (side view)
        fh3 = new TH3D("hClusters", "hClusters", 60, 1400, 1700, 50, -125, 125, 50, -125, 125);
        fh3->SetTitle("Nveto_seg Clusters");
        fh3->GetXaxis()->SetTitle("Z");
        fh3->GetYaxis()->SetTitle("X");
        fh3->GetZaxis()->SetTitle("Y");

        ioman->Register("Nveto_segClusterMon", "Cluster TH3Ds in Nveto_seg", fh3, kTRUE);
    }

    fhClusters = new TH1D("Nveto_seg/Clusters", "Number of clusters in one event", 100, 0, 100);
    fhClusterSize = new TH1D("Nveto_seg/ClusterSize", "Number of Digis for each Cluster", 100, 0, 100);
    fhClusterEnergy = new TH1D("Nveto_seg/ClusterEnergy", "Energy for each Cluster", 5000, 0., 500.);
    fhClusterNumberVSEnergy =
        new TH2D("Nveto_seg/ClusterNumberEtot", "Number of Clusters vs. Total Energy", 100, 0, 1000, 50, 0, 50);
    fhClusterNumberVSEnergy->GetXaxis()->SetTitle("Total energy [MeV]");
    fhClusterNumberVSEnergy->GetYaxis()->SetTitle("Number of Clusters");

    return kSUCCESS;
}

void R3BNveto_segClusterMon::Exec(Option_t*)
{
    const UInt_t nClusters = fNveto_segClusters->GetEntries();

    if (fIs3DTrackEnabled)
    {
        fh3->Reset("ICES");
        for (UInt_t i = 0; i < nClusters; i++)
        {
            const R3BNveto_segCluster* cluster = (R3BNveto_segCluster*)fNveto_segClusters->At(i);
            const auto start = cluster->GetFirstDigi().GetPosition();
            // XYZ -> ZXY (side view)
            fh3->Fill(start.Z(), start.X(), start.Y(), cluster->GetE());
        }
    }

    fhClusters->Fill(nClusters);

    Double_t etot = 0.;
    for (UInt_t i = 0; i < nClusters; i++)
    {
        const R3BNveto_segCluster* cluster = (R3BNveto_segCluster*)fNveto_segClusters->At(i);
        fhClusterSize->Fill(cluster->GetSize());
        fhClusterEnergy->Fill(cluster->GetE());
        etot += cluster->GetE();
    }
    fhClusterNumberVSEnergy->Fill(etot, nClusters);
}

void R3BNveto_segClusterMon::Finish()
{
    fhClusters->Write();
    fhClusterSize->Write();
    fhClusterEnergy->Write();
    fhClusterNumberVSEnergy->Write();
}

ClassImp(R3BNveto_segClusterMon)
