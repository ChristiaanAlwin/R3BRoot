#include "R3BNveto_segNeutron.h"

using namespace std;

ostream& operator<<(ostream& os, const R3BNveto_segNeutron& digi)
{
    os << "R3BNveto_segNeutron: XYZTE    " << digi.GetPosition().X() << "    " << digi.GetPosition().Y() << "    "
       << digi.GetPosition().Z() << "    " << digi.GetT() << "    " << digi.GetE() << std::endl;
    return os;
}

void R3BNveto_segNeutron::Print(const Option_t*) const { std::cout << *this; }

ClassImp(R3BNveto_segNeutron)
