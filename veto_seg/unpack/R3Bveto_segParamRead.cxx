#include "R3Bveto_segParamRead.h"

#include "R3BTCalPar.h"    // for FairDbTutPar
#include "FairRunAna.h"    // for FairRunAna
#include "FairRuntimeDb.h" // for FairRuntimeDb

#include "Riosfwd.h" // for ostream

#include <iostream> // for operator<<, basic_ostream, etc

using std::cout;
using std::endl;

ClassImp(R3Bveto_segParamRead);

void R3Bveto_segParamRead::SetParContainers()
{
    FairRunAna* ana = FairRunAna::Instance();
    FairRuntimeDb* rtdb = ana->GetRuntimeDb();
    fCalPar = (R3BTCalPar*)(rtdb->getContainer("veto_segTCalPar"));
}

InitStatus R3Bveto_segParamRead::Init()
{
    if (!fCalPar)
    {
        std::cout << "-E- R3Bveto_segParamRead: fCalPar Container does not exist!" << std::endl;
        return kERROR;
    }

    if (fCalPar)
        fCalPar->printParams();

    return kSUCCESS;
}

InitStatus R3Bveto_segParamRead::ReInit()
{
    SetParContainers();
    return kSUCCESS;
}

void R3Bveto_segParamRead::Exec(Option_t* opt)
{
    return;
}

void R3Bveto_segParamRead::Finish()
{
    cout << " -I- R3Bveto_segParamRead::Finish() ***** " << endl;

    fCalPar->printParams();

    return;
}
