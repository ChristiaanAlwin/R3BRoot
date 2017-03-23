#ifndef R3BNVETO_SEGNEUTRON2DPAR_H
#define R3BNVETO_SEGNEUTRON2DPAR_H

#include <map>
#include "FairParGenericSet.h"
#include "TCutG.h"
class FairParamList;
class TMap;

/**
 * Nveto_seg number of clusters / energy - neutron multiplicty parameter storage
 * @author Jan Mayer
 *
 * Stores the cuts for the 2D Calibr method, can be asked about the neutron muliplicity
 */

class R3BNveto_segNeutron2DPar : public FairParGenericSet
{
  public:
    // needs to be public?
    TMap* fNeutronCuts;

    R3BNveto_segNeutron2DPar(const char* name = "R3BNveto_segNeutron2DPar",
                           const char* title = "Nveto_seg Neutron 2D Parameters",
                           const char* context = "TestDefaultContext");
    ~R3BNveto_segNeutron2DPar();

    // TODO: #ROOT6: Declare functions overrriding virtual functions overrride
    void clear();                     // override
    void putParams(FairParamList*);   // override
    Bool_t getParams(FairParamList*); // override
    void printParams();               // override

    std::map<UInt_t, TCutG*> GetNeutronCuts() const;
    void SetNeutronCuts(const std::map<UInt_t, TCutG*>& cuts);
    UInt_t GetNeutronMultiplicity(const Double_t energy, const Double_t nClusters) const;

  private:
    R3BNveto_segNeutron2DPar(const R3BNveto_segNeutron2DPar&);
    R3BNveto_segNeutron2DPar& operator=(const R3BNveto_segNeutron2DPar&);

    ClassDef(R3BNveto_segNeutron2DPar, 1)
};

#endif // R3BNVETO_SEGNEUTRON2DPAR_H
