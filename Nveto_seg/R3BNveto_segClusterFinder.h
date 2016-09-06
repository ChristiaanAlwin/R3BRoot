#ifndef R3BNVETO_SEGCLUSTERFINDER_H
#define R3BNVETO_SEGCLUSTERFINDER_H

#include <vector>
#include "FairTask.h"
#include "R3BNveto_segDigi.h"

class TClonesArray;

/**
 * Nveto_seg cluster finder task
 * @author Jan Mayer
 *
 * For each event, get the R3BNveto_segDigis and group them into R3BNveto_segClusters using the Nveto_seg Clustering Engine.
 *   Input:  Branch Nveto_segDigis    = TClonesArray("R3BNveto_segDigi")
 *   Output: Branch Nveto_segClusters = TClonesArray("R3BNveto_segCluster")
 *
 * The idea is that once ROOT6 is used by default and ROOT5 is abandned, the Clutering Engine Component can be passed
 * to this task, e.g. R3BNveto_segClusterFinder(myNvetoClustringEngine), where all necessary configuration can be done on the
 * engine, such that the task can keep a small footprint. Currently hardcoded.
 */

class R3BNveto_segClusterFinder : public FairTask
{
  public:
    R3BNveto_segClusterFinder();
    ~R3BNveto_segClusterFinder();

  private:
    // Rule of three/five: If a class requires a user-defined destructor, a user-defined copy
    // constructor, or a user-defined copy assignment operator, it almost certainly requires all three
    // Here no copy and no move is allowed
    R3BNveto_segClusterFinder(const R3BNveto_segClusterFinder&);            // copy constructor
    R3BNveto_segClusterFinder(R3BNveto_segClusterFinder&&);                 // move constructor
    R3BNveto_segClusterFinder& operator=(const R3BNveto_segClusterFinder&); // copy assignment
    R3BNveto_segClusterFinder& operator=(R3BNveto_segClusterFinder&&);      // move assignment

  protected:
    // TODO: #ROOT6: Declare functions overrriding virtual functions overrride
    InitStatus Init(); // override
    void Finish();     // override
                       // void SetParContainers(); // override
  public:
    void Exec(Option_t*); // override

  private:
    void Reset();

  private:
    // TODO: #ROOT6 Replace raw pointers with std::unique_ptr?
    TClonesArray* fNveto_segDigis;
    TClonesArray* fNveto_segClusters;
    std::vector<R3BNveto_segDigi> fVectorDigis;

    // Note that ROOT5 fails hard when dealing with templates, so sadly this currently cannot be a class member
    // TODO: #ROOT6: Promote this to class member
    // Nveto_seg::NvetoClustringEngine<R3BNveto_segDigi>* fNvetoClustringEngine;

  public:
    ClassDef(R3BNveto_segClusterFinder, 1);
};

#endif // R3BNVETO_SEGCLUSTERFINDER_H
