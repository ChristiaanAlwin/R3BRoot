// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                           R3Bveto_segRawAnaS438bS438b                      -----
// -----                    Created  27-02-2015 by D. Kresan               -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#include <iostream>
using namespace std;

#include "TClonesArray.h"
#include "TH1F.h"
#include "TH2F.h"

#include "FairRootManager.h"
#include "FairRunOnline.h"
#include "FairLogger.h"

#include "R3BEventHeader.h"
#include "R3BNveto_segMappedData.h"
#include "R3BCaloRawHit.h"
#include "R3Bveto_segRawAnaS438b.h"

R3Bveto_segRawAnaS438b::R3Bveto_segRawAnaS438b()
    : fnEvents(0)
    , fveto_segRawHitMapped(NULL)
    , fCaloRawHit(NULL)
{
}

R3Bveto_segRawAnaS438b::R3Bveto_segRawAnaS438b(const char* name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , fnEvents(0)
    , fveto_segRawHitMapped(NULL)
    , fCaloRawHit(NULL)
{
}

R3Bveto_segRawAnaS438b::~R3Bveto_segRawAnaS438b()
{
}

InitStatus R3Bveto_segRawAnaS438b::Init()
{
    FairRootManager* fMan = FairRootManager::Instance();
    fHeader = (R3BEventHeader*)fMan->GetObject("R3BEventHeader");
    fveto_segRawHitMapped = (TClonesArray*)fMan->GetObject("veto_segRawHitMapped");
    fCaloRawHit = (TClonesArray*)fMan->GetObject("CaloRawHit");
    CreateHistos();

    return kSUCCESS;
}

void R3Bveto_segRawAnaS438b::Exec(Option_t* option)
{
    Int_t nCalo = 0;
    Int_t nCalo1 = 0;
    Double_t eCalo = 0.;
    R3BCaloRawHit *caloHit;
    if (fCaloRawHit)
    {
        nCalo = fCaloRawHit->GetEntries();
        for(Int_t i = 0; i < nCalo; i++)
        {
            caloHit = (R3BCaloRawHit*) fCaloRawHit->At(i);
            eCalo += caloHit->GetEnergy();
            if(caloHit->GetEnergy() > 90.)
            {
                nCalo1 += 1;
            }
        }
    }
    
    Int_t nveto_seg = 0;
    Int_t nveto_seg1 = 0;
    Double_t eveto_seg = 0.;
    Bool_t filled = kFALSE;
    if (fveto_segRawHitMapped)
    {
        Int_t nveto_segRawHitsMapped = fveto_segRawHitMapped->GetEntries();
        R3BNveto_segMappedData* hitmapped;
        for (Int_t i = 0; i < nveto_segRawHitsMapped; i++)
        {
            hitmapped = (R3BNveto_segMappedData*)fveto_segRawHitMapped->At(i);
            if(! hitmapped->Is17())
            {
                if(nCalo1 > 0 && eCalo > 60)
                {
                    fh_veto_seg_qdc->Fill(hitmapped->GetQdcData());
                    if(! filled)
                    {
                        fh_trigger_cut->Fill(fHeader->GetTrigger());
                        filled = kTRUE;
                    }
                }
                fh_veto_seg_qdc_ncalo->Fill(nCalo1, hitmapped->GetQdcData());
                fh_eveto_seg_ecalo->Fill(eCalo, hitmapped->GetQdcData());
                nveto_seg += 1;
                eveto_seg += hitmapped->GetQdcData();
                if(hitmapped->GetQdcData() > 200.)
                {
                    nveto_seg1 += 1;
                }
            }
        }
    }
    
    fhNhits->Fill(nCalo1, nveto_seg1);
    //fh_eveto_seg_ecalo->Fill(eCalo, eveto_seg);

    fnEvents += 1;
    if(0 == (fnEvents%1000))
    {
        LOG(INFO) << "R3Bveto_segRawAnaS438b : " << fnEvents << " events collected" << FairLogger::endl;
    }
}

void R3Bveto_segRawAnaS438b::FinishTask()
{
}

void R3Bveto_segRawAnaS438b::CreateHistos()
{
    fh_trigger_cut = new TH1F("h_trigger_cut", "Trigger", 10, -0.5, 9.5);
    fhNhits = new TH2F("hNhits2", "Number of hits : CALO vs VETO_SEG", 100, -0.5, 99.5, 100, -0.5, 99.5);
    fh_veto_seg_qdc = new TH1F("h_veto_seg_qdc_cut", "QDC data", 100, 0., 1000.);
    fh_veto_seg_qdc_ncalo = new TH2F("h_veto_seg_qdc_ncalo", "QDC data", 50, -0.5, 49.5, 100, 0., 1000.);
    fh_eveto_seg_ecalo = new TH2F("h_qdc_ecalo", "QDC data", 100, 0., 1000., 100, 0., 1000.);

    FairRunOnline* run = FairRunOnline::Instance();
    
    run->AddObject(fh_trigger_cut);
    run->AddObject(fhNhits);
    run->AddObject(fh_veto_seg_qdc);
    run->AddObject(fh_veto_seg_qdc_ncalo);
    run->AddObject(fh_eveto_seg_ecalo);
}

void R3Bveto_segRawAnaS438b::WriteHistos()
{
}

ClassImp(R3Bveto_segRawAnaS438b)
