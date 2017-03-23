// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                            R3Bveto_segRawAna                          -----
// -----                    Created @ 01.2014 by Madalin Cherciu           -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#include <iostream>
using namespace std;

#include "TClonesArray.h"
#include "TH1F.h"
#include "TH2F.h"

#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRunOnline.h"

#include "R3BEventHeader.h"
#include "R3BLosMappedData.h"
#include "R3BNveto_segMappedData.h"
#include "R3BNveto_segMappedHist.h"
#include "R3BPaddle_segTamexMappedData.h"

R3BNveto_segMappedHist::R3BNveto_segMappedHist()
    : fnEvents(0)
    , fNItemsTotal(0)
    , fHeader(NULL)
    , fveto_segMappedData(NULL)
    , fNveto_segTamexHitMapped(NULL)
    , fLosMappedData(NULL)
{
}

R3BNveto_segMappedHist::R3BNveto_segMappedHist(const char* name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , fnEvents(0)
    , fNItemsTotal(0)
    , fHeader(NULL)
    , fveto_segMappedData(NULL)
    , fNveto_segTamexHitMapped(NULL)
    , fLosMappedData(NULL)
{
}

R3BNveto_segMappedHist::~R3BNveto_segMappedHist()
{
}

InitStatus R3BNveto_segMappedHist::Init()
{
    FairRootManager* fMan = FairRootManager::Instance();
    fHeader = (R3BEventHeader*)fMan->GetObject("R3BEventHeader");
    fveto_segMappedData = (TClonesArray*)fMan->GetObject("Nveto_segMappedData");
    fLosMappedData = (TClonesArray*)fMan->GetObject("LosMapped");
    fNveto_segTamexHitMapped = (TClonesArray*)fMan->GetObject("Nveto_segTamexMappedItem");
    CreateHistos();

    return kSUCCESS;
}

void R3BNveto_segMappedHist::Exec(Option_t* option)
{
    if (fHeader)
    {
        fh_trigger->Fill(fHeader->GetTrigger());
    }

    if (fveto_segMappedData)
    {
        Int_t nveto_segMapped = fveto_segMappedData->GetEntries();
        fNItemsTotal += nveto_segMapped;
        R3BNveto_segMappedData* hitmapped;
        for (Int_t i = 0; i < nveto_segMapped; i++)
        {
            hitmapped = (R3BNveto_segMappedData*)fveto_segMappedData->At(i);
            fh_veto_seg_mapped_barid->Fill((hitmapped->GetPlane() - 1) * 50 + hitmapped->GetPaddle());
            fh_veto_seg_mapped_side->Fill(hitmapped->GetSide());
            fh_veto_seg_mapped_clock->Fill(hitmapped->GetClock());
            fh_veto_seg_mapped_tac->Fill(hitmapped->GetTacData());
            fh_veto_seg_mapped_qdc->Fill(hitmapped->GetQdcData());
        }
    }

    if (fNveto_segTamexHitMapped)
    {
        Int_t nNveto_segTamexHitsMapped = fNveto_segTamexHitMapped->GetEntries();
        //        fNItemsTotal += nveto_segRawHitsMapped;
        R3BPaddle_segTamexMappedData* hitmapped;
        for (Int_t i = 0; i < nNveto_segTamexHitsMapped; i++)
        {
            hitmapped = (R3BPaddle_segTamexMappedData*)fNveto_segTamexHitMapped->At(i);
            fh_Nveto_seg_mapped_is17->Fill(hitmapped->Is17());
            if (!hitmapped->Is17())
            {
                fh_Nveto_seg_mapped_planeid->Fill(hitmapped->GetPlaneId());
                fh_Nveto_seg_mapped_barid->Fill(hitmapped->GetBarId());
                fh_Nveto_seg_mapped_side->Fill(hitmapped->GetSide());
                fh_Nveto_seg_mapped_cle->Fill(hitmapped->GetCoarseTimeLE());
                fh_Nveto_seg_mapped_cte->Fill(hitmapped->GetCoarseTimeTE());
                fh_Nveto_seg_mapped_fle->Fill(hitmapped->GetFineTimeLE());
                fh_Nveto_seg_mapped_fte->Fill(hitmapped->GetFineTimeTE());
                //        cout<<"in ana: "<<hitmapped->GetPlaneId()<<"  "<<hitmapped->GetBarId()<<endl;
            }
        }
    }

    if (fLosMappedData)
    {
        Int_t nLosMapped = fLosMappedData->GetEntries();
        R3BLosMappedData* loshit;
        for (Int_t i = 0; i < nLosMapped; i++)
        {
            loshit = (R3BLosMappedData*)fLosMappedData->At(i);
            fh_los_det->Fill(loshit->GetDetector());
            fh_los_ch->Fill(loshit->GetChannel());
            fh_los_tcoarse->Fill(loshit->GetTimeCoarse());
            fh_los_tfine->Fill(loshit->GetTimeFine());
        }
    }

    fnEvents += 1;
    //if (0 == (fnEvents % 1000))
    //{
    //    LOG(INFO) << "R3BNveto_segMappedHist : " << fnEvents << " events collected" << FairLogger::endl;
    //}
}

void R3BNveto_segMappedHist::FinishTask()
{
    WriteHistos();
}

void R3BNveto_segMappedHist::CreateHistos()
{
    fh_trigger = new TH1F("h_trigger", "Trigger", 10, -0.5, 9.5);

    fh_veto_seg_mapped_barid = new TH1F("h_veto_seg_mapped_barid", "Bar ID", 500, -0.5, 499.5);
    fh_veto_seg_mapped_side = new TH1F("h_veto_seg_mapped_side", "Side", 5, -0.5, 4.5);
    fh_veto_seg_mapped_clock = new TH1F("h_veto_seg_mapped_clock", "Clock count", 70, -0.5, 69.5);
    fh_veto_seg_mapped_tac = new TH1F("h_veto_seg_mapped_tac", "TAC data", 500, 0., 5000.);
    fh_veto_seg_mapped_qdc = new TH1F("h_veto_seg_mapped_qdc", "QDC data", 100, 0., 1000.);

    fh_Nveto_seg_mapped_is17 = new TH1F("h_Nveto_seg_mapped_is17", "Is 17", 4, -0.5, 3.5);
    fh_Nveto_seg_mapped_planeid = new TH1F("h_Nveto_seg_mapped_planeid", "Plane ID", 30, -0.5, 29.5);
    fh_Nveto_seg_mapped_barid = new TH1F("h_Nveto_seg_mapped_barid", "Bar ID", 50, -0.5, 49.5);
    fh_Nveto_seg_mapped_side = new TH1F("h_Nveto_seg_mapped_side", "Side", 5, -0.5, 4.5);
    fh_Nveto_seg_mapped_cle = new TH1F("h_Nveto_seg_mapped_cle", "Coarse time LE", 8200, 0., 8200.);
    fh_Nveto_seg_mapped_cte = new TH1F("h_Nveto_seg_mapped_cte", "Coarse time TE", 8200, 0., 8200.);
    fh_Nveto_seg_mapped_fle = new TH1F("h_Nveto_seg_mapped_fle", "Fine time LE", 600, 0., 600.);
    fh_Nveto_seg_mapped_fte = new TH1F("h_Nveto_seg_mapped_fte", "Fine time TE", 600, 0., 600.);

    fh_los_det = new TH1F("h_los_det", "Detector", 20, -0.5, 19.5);
    fh_los_ch = new TH1F("h_los_ch", "Channel", 20, -0.5, 19.5);
    fh_los_tcoarse = new TH1F("h_los_tcoarse", "Time coarse", 1000, 0., 10000.);
    fh_los_tfine = new TH1F("h_los_tfine", "Time fine", 200, 0., 2000.);

    FairRunOnline* run = FairRunOnline::Instance();

    run->AddObject(fh_trigger);

    run->AddObject(fh_veto_seg_mapped_barid);
    run->AddObject(fh_veto_seg_mapped_side);
    run->AddObject(fh_veto_seg_mapped_clock);
    run->AddObject(fh_veto_seg_mapped_tac);
    run->AddObject(fh_veto_seg_mapped_qdc);

    run->AddObject(fh_Nveto_seg_mapped_is17);
    run->AddObject(fh_Nveto_seg_mapped_planeid);
    run->AddObject(fh_Nveto_seg_mapped_barid);
    run->AddObject(fh_Nveto_seg_mapped_side);
    run->AddObject(fh_Nveto_seg_mapped_cle);
    run->AddObject(fh_Nveto_seg_mapped_cte);
    run->AddObject(fh_Nveto_seg_mapped_fle);
    run->AddObject(fh_Nveto_seg_mapped_fte);

    run->AddObject(fh_los_det);
    run->AddObject(fh_los_ch);
    run->AddObject(fh_los_tcoarse);
    run->AddObject(fh_los_tfine);

    run->RegisterHttpCommand("/Reset_h_veto_seg_mapped_barid", "/h_veto_seg_mapped_barid/->Reset()");
}

void R3BNveto_segMappedHist::WriteHistos()
{
    fh_Nveto_seg_mapped_is17->Write();
    fh_Nveto_seg_mapped_planeid->Write();
    fh_Nveto_seg_mapped_barid->Write();
    fh_Nveto_seg_mapped_side->Write();
    fh_Nveto_seg_mapped_cle->Write();
    fh_Nveto_seg_mapped_cte->Write();
    fh_Nveto_seg_mapped_fle->Write();
    fh_Nveto_seg_mapped_fte->Write();
}

ClassImp(R3BNveto_segMappedHist)
