#ifndef R3BNVETO_SEGNEUTRONRECONSTRUCTION_H
#define R3BNVETO_SEGNEUTRONRECONSTRUCTION_H

/** Nveto_seg Neutron Reconstruction
 * @author Jan Mayer
 *
 * Work in progress
 */

#include "FairTask.h"

class R3BNveto_segNeutron2DPar;
class TClonesArray;

class R3BNveto_segNeutronReconstruction : public FairTask
{
  public:
    R3BNveto_segNeutronReconstruction();
    ~R3BNveto_segNeutronReconstruction();

  private:
    // Rule of three/five: If a class requires a user-defined destructor, a user-defined copy
    // constructor, or a user-defined copy assignment operator, it almost certainly requires all three
    // Here no copy and no move is allowed
    R3BNveto_segNeutronReconstruction(const R3BNveto_segNeutronReconstruction&);            // copy constructor
    R3BNveto_segNeutronReconstruction(R3BNveto_segNeutronReconstruction&&);                 // move constructor
    R3BNveto_segNeutronReconstruction& operator=(const R3BNveto_segNeutronReconstruction&); // copy assignment
    R3BNveto_segNeutronReconstruction& operator=(R3BNveto_segNeutronReconstruction&&);      // move assignment

  protected:
    // TODO: #ROOT6: Declare functions overrriding virtual functions overrride
    InitStatus Init();       // override
    void Finish();           // override
    void SetParContainers(); // override
  public:
    void Exec(Option_t*); // override

  private:
    void Reset();

    // TODO: #ROOT6 Replace raw pointers with std::unique_ptr?
    TClonesArray* fClusters;
    TClonesArray* fNeutrons;

    R3BNveto_segNeutron2DPar* fNveto_segNeutron2DPar;

  public:
    ClassDef(R3BNveto_segNeutronReconstruction, 0);
};

#endif // R3BNVETO_SEGNEUTRONRECONSTRUCTION_H
