#ifndef R3BNVETO_SEGHITPAR_H
#define R3BNVETO_SEGHITPAR_H

#define NMODULEMAX 6000

#include "FairParGenericSet.h" // for FairParGenericSet

#include "TObjArray.h"

#include "R3BNveto_segHitModulePar.h"

using namespace std;

class FairParamList;

class R3BNveto_segHitPar : public FairParGenericSet
{

  public:
    /**
     * Standard constructor. Creates instance of this class.
     * @param name a name of container.
     * @param title a title of container.
     * @param context context/purpose for parameter_seg and conditions.
     * @param own class ownership, if flag is kTRUE FairDB has the par.
     */
    R3BNveto_segHitPar(const char* name = "Nveto_segHitPar",
               const char* title = "Calibration",
               const char* context = "TestDefaultContext",
               Bool_t own = kTRUE);
    /**
     * Destructor. Cleares the memory used by the object.
     */
    virtual ~R3BNveto_segHitPar(void);

    /**
     * Method to reset the values of parameter_seg.
     */
    void clear(void);

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
     * Method to add parameter container for a module.
     * Extends the array.
     * @param tch a parameter container for a detector module.
     */
    void AddModulePar(R3BNveto_segHitModulePar* tch)
    {
        fParams->Add(tch);
    }

    /**
     * Method to retrieve the arrray with module containers.
     * @return an array with parameter containers of type R3BNveto_segHitModulePar.
     */
    TObjArray* GetListOfModulePar()
    {
        return fParams;
    }

    /**
     * Method to get number of modules storred in array.
     * @return size of array.
     */
    Int_t GetNumModulePar()
    {
        return fParams->GetEntriesFast();
    }

    /**
     * Method to get single parameter container for a specific module.
     * @param idx an index of a module.
     * @return parameter container of this module.
     */
    R3BNveto_segHitModulePar* GetModuleParAt(Int_t idx)
    {
        return (R3BNveto_segHitModulePar*)fParams->At(idx);
    }

  private:

    TObjArray* fParams; /**< an array with parameter containers of all modules */

    ClassDef(R3BNveto_segHitPar, 1);
};

#endif /* !R3BNVETO_SEGHITPAR_H*/
