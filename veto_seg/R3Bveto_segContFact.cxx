//*-- AUTHOR : Denis Bertini
//*-- Created : 20/03/2009

/////////////////////////////////////////////////////////////
//
//  R3Bveto_segContFact
//
//  Factory for the parameter containers in libR3Bveto_seg
//
/////////////////////////////////////////////////////////////


#include "R3Bveto_segContFact.h"

//#include "R3Bveto_segParRootFileIo.h"
//#include "R3Bveto_segParAsciiFileIo.h"
#include "R3BGeoveto_segPar.h"
#include "R3Bveto_segDigiPar.h"

#include "FairRuntimeDb.h"
#include "FairParRootFileIo.h"
#include "FairParAsciiFileIo.h"
#include "FairLogger.h"

#include "TClass.h"

#include <iostream>
#include <iomanip>

using std::cout;
using std::endl;

ClassImp(R3Bveto_segContFact)

static R3Bveto_segContFact gR3Bveto_segContFact;

R3Bveto_segContFact::R3Bveto_segContFact() {
  // Constructor (called when the library is loaded)
  fName="R3Bveto_segContFact";
  fTitle="Factory for parameter containers in libR3Bveto_seg";
  setAllContainers();
  FairRuntimeDb::instance()->addContFactory(this);
}

void R3Bveto_segContFact::setAllContainers() {
  /** Creates the Container objects with all accepted contexts and adds them to
   *  the list of containers for the STS library.*/

    FairContainer* p1= new FairContainer("R3Bveto_segDigiPar",
                                          "veto_seg Digitisation Parameters",
                                          "TestDefaultContext");
    p1->addContext("TestNonDefaultContext");


    containers->Add(p1);
}

FairParSet* R3Bveto_segContFact::createContainer(FairContainer* c) {
  /** veto_segls the constructor of the corresponding parameter container.
   * For an actual context, which is not an empty string and not the default context
   * of this container, the name is concatinated with the context. */

  const char* name=c->GetName();
  LOG(INFO) << "R3Bveto_segContFact: Create container name: " << name << FairLogger::endl;
  FairParSet* p=0;
  if (strcmp(name,"R3Bveto_segDigiPar")==0) {
    p=new R3Bveto_segDigiPar(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  }
  return p;

}

void  R3Bveto_segContFact::activateParIo(FairParIo* io) {
  // activates the input/output class for the parameter_seg
  // needed by the Sts
  /*
  if (strcmp(io->IsA()->GetName(),"FairParRootFileIo")==0) {
    R3Bveto_segParRootFileIo* p=new R3Bveto_segParRootFileIo(((FairParRootFileIo*)io)->getParRootFile());
    io->setDetParIo(p);
  }
  if (strcmp(io->IsA()->GetName(),"FairParAsciiFileIo")==0) {
    R3Bveto_segParAsciiFileIo* p=new R3Bveto_segParAsciiFileIo(((FairParAsciiFileIo*)io)->getFile());
    io->setDetParIo(p);
    }
  */
}

