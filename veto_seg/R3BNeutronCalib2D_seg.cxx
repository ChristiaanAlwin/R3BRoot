// -----------------------------------------------------------------------------
// -----                       R3BNeutronCalib2D_seg                          -----
// -----                 Created 27-02-2012 by D.Kresan                    -----
// -----------------------------------------------------------------------------

#include <iostream>

#include "TClonesArray.h"
#include "TH1F.h"
#include "TH2F.h"

#include "FairRootManager.h"

#include "R3Bveto_segDigi.h"
#include "R3BNeutronCalib2D_seg.h"

using std::cout;
using std::endl;

ClassImp(R3BNeutronCalib2D_seg)



// -----------------------------------------------------------------------------
R3BNeutronCalib2D_seg::R3BNeutronCalib2D_seg() :
FairTask("Calibration of NVETO_SEG cuts"),
  fEventNo(0)
{ 
}
// -----------------------------------------------------------------------------



// -----------------------------------------------------------------------------
R3BNeutronCalib2D_seg::~R3BNeutronCalib2D_seg()
{
}
// -----------------------------------------------------------------------------



// -----------------------------------------------------------------------------
InitStatus R3BNeutronCalib2D_seg::Init()
{
  // Get input array 
  FairRootManager* ioman = FairRootManager::Instance();
  if ( ! ioman ) Fatal("Init", "No FairRootManager");
  fArrayDigi = (TClonesArray*) ioman->GetObject("veto_segDigi");
  fArrayCluster = (TClonesArray*) ioman->GetObject("NVeto_segCluster");
     
  CreateHistograms();

  return kSUCCESS;
}
// -----------------------------------------------------------------------------



// -----------------------------------------------------------------------------
void R3BNeutronCalib2D_seg::Exec(Option_t* opt)
{
//  if(0 == (fEventNo%10)) {
//    cout << "-I- R3BNeutronCalib2D_seg : Event #: " << fEventNo << "." << endl;
//  }
  fEventNo += 1;

  // Loop over digis
  Float_t eTot = 0;   
  R3Bveto_segDigi *digi;
  for(Int_t id = 0; id < fArrayDigi->GetEntries(); id++) {
    digi = (R3Bveto_segDigi*) fArrayDigi->At(id);
    eTot += digi->GetQdc();
  }

  fh_etot->Fill(eTot);
  fh_ncl_etot->Fill(eTot, fArrayCluster->GetEntries());
}
// -----------------------------------------------------------------------------



/*
// -----------------------------------------------------------------------------
void R3BNeutronCalib2D_seg::CreateHistograms()
{
  if(fBeamEnergy < 300) {
    fh_etot = new TH1F("h_etot", "Total light", 1500, 0., 1500.);
    fh_ncl_etot = new TH2F("h_ncl_etot", "Number of clusters vs. total light", 1500, 0., 1500., 150, -0.5, 149.5);
  } else {
    fh_etot = new TH1F("h_etot", "Total light", 150, 0., 1500.);
    fh_ncl_etot = new TH2F("h_ncl_etot", "Number of clusters vs. total light", 150, 0., 1500., 150, -0.5, 149.5);
  }
}
*/
// -----------------------------------------------------------------------------

void R3BNeutronCalib2D_seg::CreateHistograms()
{
Int_t NbinsE = (Int_t) fEhistBoundary_Energy;
Int_t NbinsC = (Int_t) fEhistBoundary_Clusters;
if (NbinsE>10000) {NbinsE = 10000;}
if (NbinsC>10000) {NbinsC = 10000;}
fh_etot = new TH1F("h_etot","Total light",NbinsE,0.0,fEhistBoundary_Energy);
fh_ncl_etot = new TH2F("h_ncl_etot","Number of clusters vs. total light",NbinsE,0.0,fEhistBoundary_Energy,NbinsC,-0.5,fEhistBoundary_Clusters-0.5);
}


// -----------------------------------------------------------------------------
void R3BNeutronCalib2D_seg::Finish()
{
  fh_etot->Write();
  fh_ncl_etot->Write();
}
// -----------------------------------------------------------------------------

