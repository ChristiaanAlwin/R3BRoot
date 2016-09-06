#ifndef R3BNVETO_SEGNEUTRONRECONSTRUCTIONEVALUATION_H
#define R3BNVETO_SEGNEUTRONRECONSTRUCTIONEVALUATION_H

/** Nveto_seg Neutron Reconstruction Evaluation
 * @author Jan Mayer
 *
 * Work in progress
 */

#include "FairTask.h"

class TClonesArray;
class TH1D;

class R3BNveto_segNeutronReconstructionEvaluation : public FairTask
{
  public:
    R3BNveto_segNeutronReconstructionEvaluation();
    ~R3BNveto_segNeutronReconstructionEvaluation();

  private:
    // Rule of three/five: If a class requires a user-defined destructor, a user-defined copy
    // constructor, or a user-defined copy assignment operator, it almost certainly requires all three
    // Here no copy and no move is allowed
    R3BNveto_segNeutronReconstructionEvaluation(const R3BNveto_segNeutronReconstructionEvaluation&); // copy constructor
    R3BNveto_segNeutronReconstructionEvaluation(R3BNveto_segNeutronReconstructionEvaluation&&);      // move constructor
    R3BNveto_segNeutronReconstructionEvaluation& operator=(
        const R3BNveto_segNeutronReconstructionEvaluation&); // copy assignment
    R3BNveto_segNeutronReconstructionEvaluation& operator=(
        R3BNveto_segNeutronReconstructionEvaluation&&); // move assignment

  protected:
    // TODO: #ROOT6: Declare functions overrriding virtual functions overrride
    InitStatus Init();       // override
    void Finish();           // override
  public:
    void Exec(Option_t*); // override

  private:
    TH1D* fhCount;
    TH1D* fhScore;
    TH1D* fhEdiff;

    // TODO: #ROOT6 Replace raw pointers with std::unique_ptr?
    TClonesArray* fPrimaryNeutronInteractionPoints;
    TClonesArray* fReconstructedNeutrons;

  public:
    ClassDef(R3BNveto_segNeutronReconstructionEvaluation, 0);
};

#endif // R3BNVETO_SEGNEUTRONRECONSTRUCTIONEVALUATION_H
