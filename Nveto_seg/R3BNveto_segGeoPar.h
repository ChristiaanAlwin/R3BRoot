#ifndef R3BNVETO_SEGGEOPAR_H
#define R3BNVETO_SEGGEOPAR_H

#include <map>
#include "TGeoNode.h"
#include "FairParGenericSet.h"
class FairParamList;
class TVector3;

/**
 * Nveto_seg geometry parameter storage
 * @author Jan Mayer
 *
 * Stores the full Nveto_seg geo node used in the simulation for later reference, especially for coordinate
 * transformation from and to local and global coordinates.
 */

class R3BNveto_segGeoPar : public FairParGenericSet
{
  public:
    // needs to be public?
    TGeoNode* fNveto_segGeoNode;

    R3BNveto_segGeoPar(const char* name = "R3BNveto_segGeoPar",
                     const char* title = "Nveto_seg Geometry Parameters",
                     const char* context = "TestDefaultContext");
    ~R3BNveto_segGeoPar();

    // TODO: #ROOT6: Declare functions overrriding virtual functions overrride
    void clear();                     // override
    void putParams(FairParamList*);   // override
    Bool_t getParams(FairParamList*); // override
    void printParams();               // override

    TGeoNode* GetNveto_segGeoNode() { return fNveto_segGeoNode; } // FIXME: const?
    void SetNveto_segGeoNode(const TGeoNode* const p);

    Double_t GetPaddleHalfLength() const;
    TVector3 ConvertToLocalCoordinates(const TVector3& position, const Int_t paddleID) const;
    TVector3 ConvertToGlobalCoordinates(const TVector3& position, const Int_t paddleID) const;

  private:
    std::map<Int_t, TGeoNode*> fPaddleGeoNodes;
    void BuildPaddleLookup();

    R3BNveto_segGeoPar(const R3BNveto_segGeoPar&);
    R3BNveto_segGeoPar& operator=(const R3BNveto_segGeoPar&);

    ClassDef(R3BNveto_segGeoPar, 1)
};

#endif // R3BNVETO_SEGGEOPAR_H
