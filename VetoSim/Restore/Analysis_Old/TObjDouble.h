#ifndef __TObjDouble__
#define __TObjDouble__
#include "TObject.h"

class TObjDouble;

class TObjDouble : public TObject
{
private:
  Double_t fdouble;
  
public:
  TObjDouble() {fdouble = 0.0;}
  ~TObjDouble();
  void Set(Double_t const in) {fdouble = in;}
  Double_t Get() {return fdouble;}
  
  ClassDef(TObjDouble,1);
};

#endif
  
