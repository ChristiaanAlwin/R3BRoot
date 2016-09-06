#include "R3BNveto_segNeutronReconstructionEvaluation.h"

#include <iostream>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <utility>
#include <functional>

#include "TClonesArray.h"
#include "TH1D.h"

#include "FairRootManager.h"
#include "FairRuntimeDb.h"
#include "FairRtdbRun.h"
#include "FairLogger.h"
#include "FairMCPoint.h"

#include "R3BNveto_segNeutron.h"

const Double_t massNeutron = 939.565; // MeV/c²

Double_t Distance(const R3BNveto_segNeutron& nn, const FairMCPoint& mc)
{
    TVector3 v;
    mc.Position(v);
    v -= nn.GetPosition();
    return std::sqrt(v.Dot(v));
}

Double_t Score(const std::vector<std::pair<R3BNveto_segNeutron, FairMCPoint>>& combination)
{
    return std::accumulate(combination.begin(),
                           combination.end(),
                           0.,
                           [](const Double_t sum, const std::pair<R3BNveto_segNeutron, FairMCPoint>& pair)
                           {
                               return sum + Distance(pair.first, pair.second);
                           });
}

template <typename T, typename U>
std::vector<std::vector<std::pair<T, U>>> GetAllCombinations(std::vector<T> ts /* Copy! */,
                                                             std::vector<U> us /* Copy! */,
                                                             std::function<bool(const U&, const U&)> comparator)
{
    std::vector<std::vector<std::pair<T, U>>> out;

    // Bring both inputs up to the same length
    if (ts.size() < us.size())
    {
        ts.resize(us.size());
    }
    if (us.size() < ts.size())
    {
        us.resize(ts.size());
    }

    std::sort(us.begin(), us.end(), comparator);
    do
    {
        std::vector<std::pair<T, U>> tmp;
        for (size_t i = 0; i < ts.size(); i++)
        {
            tmp.push_back({ ts.at(i), us.at(i) });
        }
        out.push_back(std::move(tmp));
    } while (std::next_permutation(us.begin(), us.end(), comparator));

    return out;
}

R3BNveto_segNeutronReconstructionEvaluation::R3BNveto_segNeutronReconstructionEvaluation()
    : FairTask("R3B Nveto_seg Nveto_segCluster Monitor")
{
    LOG(INFO) << "Using R3B Nveto_seg Neutron Reconstruction" << FairLogger::endl;
}

R3BNveto_segNeutronReconstructionEvaluation::~R3BNveto_segNeutronReconstructionEvaluation() {}

InitStatus R3BNveto_segNeutronReconstructionEvaluation::Init()
{
    FairRootManager* ioman = FairRootManager::Instance();
    if (!ioman)
    {
        LOG(FATAL) << "R3BNveto_segNeutronReconstructionEvaluation::Init: No FairRootManager" << FairLogger::endl;
        return kFATAL;
    }

    // Set Input: TClonesArray of R3BNveto_segNeutrons
    if ((TClonesArray*)ioman->GetObject("Nveto_segNeutrons") == nullptr)
    {
        LOG(FATAL) << "R3BNveto_segNeutronReconstructionEvaluation::Init No Nveto_segNeutrons!" << FairLogger::endl;
        return kFATAL;
    }
    if (!TString(((TClonesArray*)ioman->GetObject("Nveto_segNeutrons"))->GetClass()->GetName())
             .EqualTo("R3BNveto_segNeutron"))
    {
        LOG(FATAL) << "R3BNveto_segNeutronReconstructionEvaluation::Init Branch Nveto_segNeutrons does not contain "
                      "R3BNveto_segNeutrons!" << FairLogger::endl;
        return kFATAL;
    }
    fReconstructedNeutrons = (TClonesArray*)ioman->GetObject("Nveto_segNeutrons");

    // Set Input: TClonesArray of FairMCPoints
    if ((TClonesArray*)ioman->GetObject("Nveto_segPrimaryNeutronInteractionPoints") == nullptr)
    {
        LOG(FATAL) << "R3BNveto_segNeutronReconstructionEvaluation::Init No Nveto_segPrimaryNeutronInteractionPoints!"
                   << FairLogger::endl;
        return kFATAL;
    }
    if (!TString(((TClonesArray*)ioman->GetObject("Nveto_segPrimaryNeutronInteractionPoints"))->GetClass()->GetName())
             .EqualTo("FairMCPoint"))
    {
        LOG(FATAL) << "R3BNveto_segNeutronReconstructionEvaluation::Init Branch Nveto_segPrimaryNeutronInteractionPoints "
                      "does not contain FairMCPoints!" << FairLogger::endl;
        return kFATAL;
    }
    fPrimaryNeutronInteractionPoints = (TClonesArray*)ioman->GetObject("Nveto_segPrimaryNeutronInteractionPoints");

    fhScore = new TH1D("fhScore", "Nveto_seg Neutron Reconstruction Score (lower is better)", 5000, 0, 5000);
    fhCount = new TH1D("fhCount", "Number of reacted primary Neutrons - Number of reconstructed Neutrons", 11, -5, 5);
    fhEdiff = new TH1D("fhEdiff", "Energy of primary Neutron - Energy of reconstructed Neutron", 2001, -1000, 1000);

    return kSUCCESS;
}

void R3BNveto_segNeutronReconstructionEvaluation::Exec(Option_t*)
{
    const UInt_t nReconstructedNeutrons = fReconstructedNeutrons->GetEntries();
    std::vector<R3BNveto_segNeutron> neutrons;
    neutrons.reserve(nReconstructedNeutrons);
    for (UInt_t i = 0; i < nReconstructedNeutrons; i++)
    {
        neutrons.push_back(*((R3BNveto_segNeutron*)fReconstructedNeutrons->At(i)));
    }

    const UInt_t nPNIPS = fPrimaryNeutronInteractionPoints->GetEntries();
    std::vector<FairMCPoint> pnips;
    pnips.reserve(nReconstructedNeutrons);
    for (UInt_t i = 0; i < nPNIPS; i++)
    {
        pnips.push_back(*((FairMCPoint*)fPrimaryNeutronInteractionPoints->At(i)));
    }

    fhCount->Fill(nPNIPS - nReconstructedNeutrons);

    // Find the lowes score by brute force creating all combinations of MC-known and reconstructed neutron hits.
    // Score is the sum of distances. Note that detecting a wrong amount of neutrons will create a huge score
    // WiP. Slow because of all the copying
    const auto allCombinations =
        GetAllCombinations<R3BNveto_segNeutron, FairMCPoint>(neutrons,
                                                           pnips,
                                                           [](const FairMCPoint& a, const FairMCPoint& b)
                                                           {
                                                               return a.GetTime() < b.GetTime();
                                                           });

    const auto bestCombination = *std::min_element(allCombinations.begin(),
                                                   allCombinations.end(),
                                                   [](const std::vector<std::pair<R3BNveto_segNeutron, FairMCPoint>>& a,
                                                      const std::vector<std::pair<R3BNveto_segNeutron, FairMCPoint>>& b)
                                                   {
                                                       return Score(a) < Score(b);
                                                   });

    const Double_t minScore = Score(bestCombination);
    fhScore->Fill(minScore);

    for (const auto& pair : bestCombination)
    {
        if (pair.second.GetEnergyLoss() > 0 || pair.first.GetE() > 0)
        {
            // TODO: Only for massNeutron
            if (pair.second.GetEnergyLoss() > 0)
            {
                fhEdiff->Fill(pair.second.GetEnergyLoss() * 1000 - massNeutron - pair.first.GetE());
            }
            else
            {
                fhEdiff->Fill(-pair.first.GetE());
            }
        }
    }
}

void R3BNveto_segNeutronReconstructionEvaluation::Finish()
{
    fhCount->Write();
    fhScore->Write();
    fhEdiff->Write();
}

ClassImp(R3BNveto_segNeutronReconstructionEvaluation)