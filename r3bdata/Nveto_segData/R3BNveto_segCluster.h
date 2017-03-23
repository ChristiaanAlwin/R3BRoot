#ifndef R3BNVETO_SEGCLUSTER_H
#define R3BNVETO_SEGCLUSTER_H

#include <vector>
#include <iostream>

#include "TObject.h"

#include "R3BNveto_segDigi.h"

class R3BNveto_segCluster : public TObject
{
  public:
    R3BNveto_segCluster() {}

    R3BNveto_segCluster(const R3BNveto_segDigi& digi)
        : fDigis({ digi })
    {
    }

    R3BNveto_segCluster(std::vector<R3BNveto_segDigi>::iterator begin, std::vector<R3BNveto_segDigi>::iterator end)
        : fDigis(begin, end)
    {
    }

    R3BNveto_segCluster(const std::vector<R3BNveto_segDigi> digis)
        : fDigis(digis)
    {
    }

    // Rule of zero: Non-ownership-managing classes should not have custom
    // destructors, copy/move constructors or copy/move assignment operators.

    // TODO: #ROOT6: Declare functions overrriding virtual functions override
    void Print(const Option_t*) const; // override

    void AddDigi(const R3BNveto_segDigi& digi) { fDigis.push_back(digi); }
    std::vector<R3BNveto_segDigi> GetDigis() const { return fDigis; }

    R3BNveto_segDigi GetFirstDigi() const;
    R3BNveto_segDigi GetLastDigi() const;
    Double_t GetT() const;
    Double_t GetE() const;
    UInt_t GetSize() const { return fDigis.size(); }

  protected:
    std::vector<R3BNveto_segDigi> fDigis;

  public:
    ClassDef(R3BNveto_segCluster, 1)
};

// Support easy printing with energy.g. R3BNveto_segCluster mycluster; cout << mycluster;
std::ostream& operator<<(std::ostream&, const R3BNveto_segCluster&);

#endif // R3BNVETO_SEGCLUSTER_H
