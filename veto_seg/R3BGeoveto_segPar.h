#ifndef R3BGEOVETO_SEGPAR_H
#define R3BGEOVETO_SEGPAR_H
 
#include "FairParGenericSet.h"

#include "TH1F.h"

class R3BGeoveto_segPar : public FairParGenericSet {
public:
  TObjArray            *fGeoSensNodes; // List of FairGeoNodes for sensitive volumes
  TObjArray            *fGeoPassNodes; // List of FairGeoNodes for sensitive volumes

  R3BGeoveto_segPar(const char* name="R3BGeoveto_segPar",
             const char* title="veto_seg Geometry Parameters",
             const char* context="TestDefaultContext");
  ~R3BGeoveto_segPar(void);
  void clear(void);
  void putParams(FairParamList*);
  Bool_t getParams(FairParamList*);
  TObjArray             *GetGeoSensitiveNodes(){return fGeoSensNodes;}
  TObjArray             *GetGeoPassiveNodes(){return fGeoPassNodes;}

  ClassDef(R3BGeoveto_segPar,1)
};

#endif /* !R3BGEOVETO_SEGPAR_H */
