#ifndef R3BNVETO_SEGMAPPED2QCALPAR_H
#define R3BNVETO_SEGMAPPED2QCALPAR_H

#include "FairTask.h"

class TClonesArray;
class TH1F;
class R3BEventHeader;
class R3BNveto_segQCalPar;

class R3BNveto_segMapped2QCalPar : public FairTask
{

  public:
    R3BNveto_segMapped2QCalPar();
    R3BNveto_segMapped2QCalPar(const char* name, Int_t iVerbose = 1);
    virtual ~R3BNveto_segMapped2QCalPar();

    virtual InitStatus Init();

    virtual void Exec(Option_t* option);

    virtual void FinishTask();
    
    void SetPlanes(Int_t planes){
	fPlanes = planes;
    }

  private:
    Int_t fPlanes = 60;
    Int_t fPaddles = 50;
    
    TClonesArray* fHits;
    R3BNveto_segQCalPar* fPar;

    Int_t fEventNumber = 0;
    
    R3BEventHeader* header;
    
    std::vector<TH1F*> fData;

  public:
    ClassDef(R3BNveto_segMapped2QCalPar, 0)
};

#endif
