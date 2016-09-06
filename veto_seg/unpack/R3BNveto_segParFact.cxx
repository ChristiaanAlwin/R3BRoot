/////////////////////////////////////////////////////////////
//
//  R3BNveto_segParFact
//
//  Factory for the parameter containers
//
/////////////////////////////////////////////////////////////
#include "R3BNveto_segParFact.h"

#include "R3BNveto_segHitPar.h"    // for R3Bveto_segGeometryPar
#include "FairParSet.h"    // for FairParSet
#include "FairRuntimeDb.h" // for FairRuntimeDb
#include "FairLogger.h"

#include "Riosfwd.h" // for ostream
#include "TList.h"   // for TList
#include "TString.h" // for TString

//#include <string.h> // for strcmp, NULL

using namespace std;

static R3BNveto_segParFact gR3BNveto_segParFact;

R3BNveto_segParFact::R3BNveto_segParFact()
{
    // Constructor (called when the library is loaded)
    fName = "R3BNveto_segParFact";
    fTitle = "Tutorial factory for parameter containers"; //TODO
    setAllContainers();
    FairRuntimeDb::instance()->addContFactory(this);
}

void R3BNveto_segParFact::setAllContainers()
{
    /** Creates the Container objects with all accepted contexts and adds them to
     *  the list of containers.*/

    FairContainer* p1 = new FairContainer("Nveto_segHitPar", "Nveto_seg Modul Parameters", "TestDefaultContext"); //TODO
    p1->addContext("TestNonDefaultContext");
    containers->Add(p1);
}

FairParSet* R3BNveto_segParFact::createContainer(FairContainer* c)
{
    /** Calls the constructor of the corresponding parameter container.
     * For an actual context, which is not an empty string and not the default context
     * of this container, the name is concatinated with the context. */

    const char* name = c->GetName();
    LOG(INFO) << "R3BNveto_segParFact::createContainer : " << name << FairLogger::endl;
    FairParSet* p = NULL;

    if (strcmp(name, "Nveto_segHitPar") == 0) //TODO
    {
        p = new R3BNveto_segHitPar(c->getConcatName().Data(), c->GetTitle(), c->getContext());
    }
    return p;
}

ClassImp(R3BNveto_segParFact);