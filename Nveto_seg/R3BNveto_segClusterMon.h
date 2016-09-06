#ifndef R3BNVETO_SEGCLUSTERMON_H
#define R3BNVETO_SEGCLUSTERMON_H 1

/** Nveto_seg Cluster Monitor
 * @author Jan Mayer
 * @since  10.06.2015
 *
 *  Input:  Clusters. Should work both on Clusters from Monte Carlo simulations
 *          as well as experimental data on digi level.
 *  Output: Soon: Vairous diagrams.
 *          Currently: A 3D scatterplot for each event.
 */

#include "FairTask.h"

class TClonesArray;
class TH1D;
class TH2D;
class TH3D;

class R3BNveto_segClusterMon : public FairTask
{
  public:
    R3BNveto_segClusterMon(const Option_t* option = "");
    ~R3BNveto_segClusterMon();

    InitStatus Init();
    void Exec(Option_t* option);
    void Finish();

  private:
    TClonesArray* fNveto_segClusters;

    Bool_t fIs3DTrackEnabled;
    TH3D* fh3;

    TH1D* fhClusters;
    TH1D* fhClusterSize;
    TH1D* fhClusterEnergy;
    TH2D* fhClusterNumberVSEnergy;

    ClassDef(R3BNveto_segClusterMon, 0);
};

#endif // R3BNVETO_SEGCLUSTERMON_H
