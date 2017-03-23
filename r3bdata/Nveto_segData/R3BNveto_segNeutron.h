#ifndef R3BNVETO_SEGNEUTRON_H
#define R3BNVETO_SEGNEUTRON_H

#include <iostream>

#include "TObject.h"
#include "TVector3.h"

class R3BNveto_segNeutron : public TObject
{
  public:
    R3BNveto_segNeutron()
        : fT(0)
        , fE(0)
        , fPosition(TVector3(0, 0, 0))
    {
    }

    R3BNveto_segNeutron(const Double_t time, const Double_t energy, const TVector3& pos)
        : fT(time)
        , fE(energy)
        , fPosition(pos)
    {
    }

    // Rule of zero: Non-ownership-managing classes should not have custom
    // destructors, copy/move constructors or copy/move assignment operators.

    // TODO: #ROOT6: Declare functions overrriding virtual functions overrride
    void Print(const Option_t*) const; // override

    void SetT(const Double_t time) { fT = time; }
    Double_t GetT() const { return fT; }

    void SetE(const Double_t energy) { fE = energy; }
    Double_t GetE() const { return fE; }

    void SetPosition(const TVector3& position) { fPosition = position; }
    TVector3 GetPosition() const { return fPosition; }

  protected:
    Double_t fT;
    Double_t fE;
    TVector3 fPosition;

  public:
    ClassDef(R3BNveto_segNeutron, 1)
};

// Support easy printing with R3BNveto_segNeutron myneutron; cout << myneutron;
std::ostream& operator<<(std::ostream&, const R3BNveto_segNeutron&);

#endif // R3BNVETO_SEGNEUTRON_H
