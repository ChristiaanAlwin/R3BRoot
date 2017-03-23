#ifndef __TObjInt__
#define __TObjInt__
#include "TObject.h"

class TObjInt;

class TObjInt : public TObject
{
private:
  Int_t fint;
  
public:
  TObjInt() {fint = 0;}
  ~TObjInt();
  void Set(Int_t const in) {fint = in;}
  Int_t Get() {return fint;}
  
  ClassDef(TObjInt,1);
};

#endif
  
