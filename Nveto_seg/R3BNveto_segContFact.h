#ifndef R3BNVETO_SEGCONTFACT_H
#define R3BNVETO_SEGCONTFACT_H

#include "FairContFact.h"

class FairContainer;

/**
 * Nveto_seg parameter container factory
 * @author Jan Mayer
 *
 * Lots of super brittle boilerplate code that should really not be necessary
 * TODO: Make FairRoot better so all of this can be deletet.
 */

class R3BNveto_segContFact : public FairContFact
{
  private:
    void setAllContainers();

  public:
    R3BNveto_segContFact();
    FairParSet* createContainer(FairContainer*);
    ClassDef(R3BNveto_segContFact, 0)
};

#endif // R3BNVETO_SEGCONTFACT_H
