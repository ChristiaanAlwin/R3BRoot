#ifndef R3BVETO_SEGCONTFACT_H
#define R3BVETO_SEGCONTFACT_H

#include "FairContFact.h"

class FairContainer;

class R3Bveto_segContFact : public FairContFact {
private:
  void setAllContainers();
public:
  R3Bveto_segContFact();
  ~R3Bveto_segContFact() {}
  FairParSet* createContainer(FairContainer*);
  void  activateParIo(FairParIo* io);
  ClassDef( R3Bveto_segContFact,0) // Factory for all VETO_SEG parameter containers
};

#endif  /* !R3BVETO_SEGCONTFACT_H */
