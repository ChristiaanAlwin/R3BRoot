#ifndef R3BGEOVETO_SEG_H
#define R3BGEOVETO_SEG_H

#include "FairGeoSet.h"

class  R3BGeoveto_seg : public FairGeoSet {
protected:
  char modName[20];  // name of module
  char eleName[20];  // substring for elements in module
public:
  R3BGeoveto_seg();
  ~R3BGeoveto_seg() {}
  const char* getModuleName(Int_t);
  const char* getEleName(Int_t);
  inline Int_t getModNumInMod(const TString&);
  ClassDef(R3BGeoveto_seg,0) // Class for STS
};

inline Int_t R3BGeoveto_seg::getModNumInMod(const TString& name) {
  // returns the module index from module name
  return (Int_t)(name[3]-'0')-1;
}

#endif  /* !R3BGEOVETO_SEG_H */
