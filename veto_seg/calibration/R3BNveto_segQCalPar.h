#ifndef R3BNVETO_SEGQCALPAR_H
#define R3BNVETO_SEGQCALPAR_H

#define NMODULEMAX 6000

#include "FairParGenericSet.h" // for FairParGenericSet

#include "TObjArray.h"

#include "TArrayI.h"

using namespace std;

class FairParamList;

class R3BNveto_segQCalPar : public FairParGenericSet
{

  public:
    /**
     * Standard constructor. Creates instance of this class.
     * @param name a name of container.
     * @param title a title of container.
     * @param context context/purpose for parameter_seg and conditions.
     * @param own class ownership, if flag is kTRUE FairDB has the par.
     */
    R3BNveto_segQCalPar(const char* name = "Nveto_segQCalPar",
               const char* title = "Calibration",
               const char* context = "TestDefaultContext",
               Bool_t own = kTRUE);
    /**
     * Destructor. Cleares the memory used by the object.
     */
    virtual ~R3BNveto_segQCalPar(void);

    /**
     * Method to set the size of the array of parameter_seg.
     */
    void SetSize(Int_t size);

    /**
     * Method to store parameter_seg using FairRuntimeDB.
     * @param list a list of parameter_seg.
     */
    void putParams(FairParamList* list);

    /**
     * Method to retrieve parameter_seg using FairRuntimeDB.
     * @param list a list of parameter_seg.
     * @return kTRUE if successful, else kFALSE.
     */
    Bool_t getParams(FairParamList* list);

    /**
     * Method to print value of parameter_seg to the standard output.
     * Calls printParams() for every module container.
     */
    void printParams();

    /**
     * Method to set parameter for a module.
     * @param id id of the module.
     * @param offset pedestal offset of the module.
     */
    void SetParAt(Int_t  id, Int_t offset)
    {
        fParams[id] = offset;
    }

    /**
     * Method to retrieve the pedestal offset of a module.
     * @param id id of the module.
     * @return pedestal offset of the module.
     */
    Int_t GetParAt(Int_t id)
    {
        return fParams[id];
    }

    /**
     * Method to get number of modules storred in array.
     * @return size of array.
     */
    Int_t GetNumPar()
    {
        return fParams.GetSize();
    }

  private:

    TArrayI fParams; /**< an array with parameter_seg of all modules */

    ClassDef(R3BNveto_segQCalPar, 1);
};

#endif /* !R3BNVETO_SEGQCALPAR_H*/
