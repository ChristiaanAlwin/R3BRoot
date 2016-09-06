#include "R3BNveto_segCluster.h"

#include <algorithm>
#include <stdexcept>
#include <numeric>

using namespace std;

R3BNveto_segDigi R3BNveto_segCluster::GetFirstDigi() const
{
    auto min = std::min_element(fDigis.begin(),
                                fDigis.end(),
                                [](const R3BNveto_segDigi& a, const R3BNveto_segDigi& b)
                                {
                                    return a.GetT() < b.GetT();
                                });
    if (min == fDigis.end())
    {
        throw std::logic_error("R3BNveto_segCluster::GetFirstDigi(): Cluster has no Digis!");
    }
    return *min;
}

R3BNveto_segDigi R3BNveto_segCluster::GetLastDigi() const
{
    auto max = std::max_element(fDigis.begin(),
                                fDigis.end(),
                                [](const R3BNveto_segDigi& a, const R3BNveto_segDigi& b)
                                {
                                    return a.GetT() < b.GetT();
                                });
    if (max == fDigis.end())
    {
        throw std::logic_error("R3BNveto_segCluster::GetLastDigi(): Cluster has no Digis!");
    }
    return *max;
}

Double_t R3BNveto_segCluster::GetE() const
{
    return std::accumulate(fDigis.begin(),
                           fDigis.end(),
                           0.,
                           [](const Double_t sum, const R3BNveto_segDigi& digi)
                           {
                               return sum + digi.GetE();
                           });
}

Double_t R3BNveto_segCluster::GetT() const
{
    return GetFirstDigi().GetT();
}

ostream& operator<<(ostream& os, const R3BNveto_segCluster& cluster)
{
    os << "R3BNveto_segCluster: Nveto_seg Cluster with size " << cluster.GetSize() << std::endl;
    if (cluster.GetSize() > 0)
    {
        os << "    Sum Energy: " << cluster.GetE() << std::endl
           << "    FirstDigi: " << cluster.GetFirstDigi() << "    LastDigi: " << cluster.GetLastDigi();
    }
    return os;
}

void R3BNveto_segCluster::Print(const Option_t*) const { std::cout << *this; }

ClassImp(R3BNveto_segCluster)
