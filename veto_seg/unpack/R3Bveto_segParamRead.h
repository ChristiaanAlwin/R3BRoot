#ifndef R3BVETO_SEGPARAMREAD_H
#define R3BVETO_SEGPARAMREAD_H

#include "FairTask.h" // for FairTask, InitStatus

#include "Rtypes.h" // for ClassDef

#include <stddef.h> // for NULL

class R3BTCalPar;

class R3Bveto_segParamRead : public FairTask
{
  public:
    /** Default constructor **/
    R3Bveto_segParamRead()
        : FairTask()
        , fCalPar(NULL){};

    /** Destructor **/
    virtual ~R3Bveto_segParamRead(){};

    /** Virtual method Init **/
    virtual void SetParContainers();
    virtual InitStatus Init();
    virtual InitStatus ReInit();

    /** Virtual method Exec **/
    virtual void Exec(Option_t* opt);
    /** Virtual method Finish **/
    virtual void Finish();

    R3BTCalPar* fCalPar;

    R3Bveto_segParamRead(const R3Bveto_segParamRead&);
    R3Bveto_segParamRead& operator=(const R3Bveto_segParamRead&);

    ClassDef(R3Bveto_segParamRead, 0);
};

#endif
