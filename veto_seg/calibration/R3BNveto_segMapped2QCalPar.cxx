#include "R3BNveto_segMapped2PedPar.h"
#include "FairLogger.h"
#include "TClonesArray.h"
#include "TH1F.h"
#include "R3BNveto_segMappedData.h"
#include "R3BTCalEngine.h"
#include "R3BEventHeader.h"
#include "R3BNveto_segQCalPar.h"
#include "R3BNveto_segMapped2QCalPar.h"
#include "FairRuntimeDb.h"

#define nPMTs 2*fPaddles*fPlanes

R3BNveto_segMapped2QCalPar::R3BNveto_segMapped2QCalPar(const char* name, Int_t iVerbose)
:FairTask(name, iVerbose)
   , fPar(NULL)
   , fHits(NULL) 
{

}

R3BNveto_segMapped2QCalPar::R3BNveto_segMapped2QCalPar()
:FairTask("R3BNveto_segMapped2QCalPar")
   , fPar(NULL)
   , fHits(NULL) 
{

}

R3BNveto_segMapped2QCalPar::~R3BNveto_segMapped2QCalPar()
{
  for(Int_t i = fData.size()-1; i > 0 ; i++){
    fData.at(i)->Delete();
  }
  
  if (fPar)
    delete fPar;
}

InitStatus R3BNveto_segMapped2QCalPar::Init()
{
  FairRootManager* fMan = FairRootManager::Instance();
  if (! fMan) {
    FairLogger::GetLogger()->Fatal(MESSAGE_ORIGIN, " FairRootManager not found.");
    return kFATAL;
  }
  fHits = (TClonesArray*) fMan->GetObject("Nveto_segMappedData");
  if (!fHits) {
    FairLogger::GetLogger()->Fatal(MESSAGE_ORIGIN, " Branch: Nveto_segCalData not found in Tree.");
    return kFATAL;
  } 
  header = (R3BEventHeader*)fMan->GetObject("R3BEventHeader");
  if (!header)
  {
    FairLogger::GetLogger()->Fatal(MESSAGE_ORIGIN, " Branch: R3BEventHeader not found in Tree.");
    return kFATAL;
  }
  
  TH1F* histo;

  for(Int_t i = 0; i < nPMTs; i++){
    std::string s = "h_pedpar_" + std::to_string(i+1);
    histo = new TH1F(s.c_str(), "PedPar", 2001, -0.5, 2000.5);
    fData.push_back(histo);
  }
  
  fPar = (R3BNveto_segQCalPar*) FairRuntimeDb::instance()->getContainer("Nveto_segQCalPar");
  
  return kSUCCESS;
}

void R3BNveto_segMapped2QCalPar::Exec(Option_t* option)
{  
  R3BNveto_segMappedData* hit;
  Int_t nHits = fHits->GetEntries();
  Int_t iBar;
  Int_t iSide;
  
  if (header->GetTrigger() != 2)
    return;
  
  if (nHits < nPMTs)
    return;
  
  for(Int_t i = 0; i < nHits; i++){
    hit = (R3BNveto_segMappedData*)fHits->At(i);
    if (!hit)
      continue;
    
    iBar = (hit->GetPlane() - 1) * fPaddles + hit->GetPaddle() - 1;
    iSide = hit->GetSide();

    if(iBar > 0)
      fData.at(iBar * 2 + iSide - 1)->Fill(hit->GetQdcData());
  }
}

void R3BNveto_segMapped2QCalPar::FinishTask()
{
  TH1F* pars = new TH1F("QCalPar", "Pedestal Offset", nPMTs, 0.5, nPMTs + 0.5);
  
  fPar->SetSize(nPMTs);
  for(Int_t i = 0; i < nPMTs; i++){
    pars->SetBinContent(i + 1, fData.at(i)->GetMaximumBin());
    fPar->SetParAt(i,fData.at(i)->GetMaximumBin());
  }  
  pars->Write();
  fPar->setChanged();
}

ClassImp(R3BNveto_segMapped2QCalPar)
