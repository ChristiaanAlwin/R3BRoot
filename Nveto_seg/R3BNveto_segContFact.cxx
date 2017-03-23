#include "R3BNveto_segContFact.h"
#include "R3BNveto_segGeoPar.h"
#include "R3BNveto_segNeutron2DPar.h"
#include "FairRuntimeDb.h"

static R3BNveto_segContFact gR3BNveto_segContFact;

R3BNveto_segContFact::R3BNveto_segContFact()
{
    fName = "R3BNveto_segContFact";
    fTitle = "Factory for Nveto_seg parameter containers";
    setAllContainers();
    FairRuntimeDb::instance()->addContFactory(this);
}

void R3BNveto_segContFact::setAllContainers()
{
    FairContainer* p1 = new FairContainer("R3BNveto_segGeoPar", "Nveto_seg Geometry Parameters", "TestDefaultContext");
    p1->addContext("TestNonDefaultContext");
    containers->Add(p1);

    FairContainer* p2 =
        new FairContainer("R3BNveto_segNeutron2DPar", "Nveto_seg Neutron 2D Parameters", "TestDefaultContext");
    p2->addContext("TestNonDefaultContext");
    containers->Add(p2);
}

FairParSet* R3BNveto_segContFact::createContainer(FairContainer* c)
{
    const char* name = c->GetName();

    if (strcmp(name, "R3BNveto_segGeoPar") == 0)
    {
        return new R3BNveto_segGeoPar(c->getConcatName().Data(), c->GetTitle(), c->getContext());
    }

    if (strcmp(name, "R3BNveto_segNeutron2DPar") == 0)
    {
        return new R3BNveto_segNeutron2DPar(c->getConcatName().Data(), c->GetTitle(), c->getContext());
    }

    return nullptr;
}

ClassImp(R3BNveto_segContFact)
