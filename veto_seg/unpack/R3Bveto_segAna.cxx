// -----------------------------------------------------------------------------
// -----                               R3Bveto_segAna                          -----
// -----                     Created 22-04-2014 by D.Kresan                -----
// -----------------------------------------------------------------------------

#include "TClonesArray.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TMath.h"

#include "FairLogger.h"
#include "FairRootManager.h"

#include "R3Bveto_segAna.h"
#include "R3Bveto_segDigi.h"
#include "R3BLosHit.h"

const Double_t C_LIGHT = 29.9792458; // Speed of light [cm/ns]

R3Bveto_segAna::R3Bveto_segAna()
    : fNofBars(100)
    , fFirstPlaneHorisontal(kFALSE)
    , fnEvents(0)
    , fveto_segDigi(NULL)
    , fLosHit(NULL)
{
}

R3Bveto_segAna::R3Bveto_segAna(const char* name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , fNofBars(100)
    , fFirstPlaneHorisontal(kFALSE)
    , fnEvents(0)
    , fveto_segDigi(NULL)
    , fLosHit(NULL)
{
}

R3Bveto_segAna::~R3Bveto_segAna()
{
}

InitStatus R3Bveto_segAna::Init()
{
    FairRootManager* fMan = FairRootManager::Instance();
    if (!fMan)
    {
        FairLogger::GetLogger()->Fatal(MESSAGE_ORIGIN, "FairRootManager not found");
    }
    fveto_segDigi = (TClonesArray*)fMan->GetObject("veto_segDigi");
    fLosHit = (TClonesArray*)fMan->GetObject("LosHit");
    CreateHistos();

    return kSUCCESS;
}

Int_t getStart()
{
    return -350;
}

Double_t getStep()
{
    return 0.5;
}

void R3Bveto_segAna::Exec(Option_t* option)
{
    Bool_t startSeen = kFALSE;
    Double_t startTime = 0.;
    if (fLosHit)
    {
        Int_t nLosHit = fLosHit->GetEntriesFast();
        Double_t stime[4];
        R3BLosHit* losHit;
        Int_t ch;
        Bool_t seen[] = { kFALSE, kFALSE, kFALSE, kFALSE };
        for (Int_t i = 0; i < nLosHit; i++)
        {
            losHit = (R3BLosHit*)fLosHit->At(i);
            ch = losHit->GetChannel();
            if (ch >= 4)
            {
                continue;
            }
            if (seen[ch])
            {
//                LOG(ERROR) << "R3Bveto_segAna::Exec : LOS channel " << ch << " seen > once" <<
//                FairLogger::endl;
//                LOG(INFO) << "ch=" << ch << "  " << losHit->GetTime() << " vs " << stime[ch] <<
//                FairLogger::endl;
//                stime[ch] = TMath::Min(stime[ch], losHit->GetTime());
            }
            else
            {
                seen[ch] = kTRUE;
                stime[ch] = losHit->GetTime();
            }
        }

        Int_t n = 0;
        for (Int_t i = 0; i < 4; i++)
        {
            if (seen[i])
            {
                n++;
                startTime += stime[i];
            }
        }
        if (n >= fMinimalLOSSignals)
        {
            startTime = startTime / n;
            startSeen = kTRUE;
            fh_los_time->Fill(startTime);
        }
    }

    Int_t count = 0;
    Double_t tmean = 0.;

    if (fveto_segDigi)
    {
        Int_t nveto_segDigi = fveto_segDigi->GetEntriesFast();
        R3Bveto_segDigi* digi;
        Int_t barId;
        Double_t time, time2;
        Double_t qdc, qdcL, qdcR;
        Double_t x, y, z;
        Double_t l;
        Double_t beta;
        Double_t normt;
        for (Int_t i = 0; i < nveto_segDigi; i++)
        {
            digi = (R3Bveto_segDigi*)fveto_segDigi->At(i);

            barId = digi->GetPaddleNr();
            time = digi->GetTdc();
            qdc = digi->GetQdc();
            qdcL = digi->GetQdcL();
            qdcR = digi->GetQdcR();
            x = digi->GetXX();
            y = digi->GetYY();
            z = digi->GetZZ();
            l = TMath::Sqrt((x) * (x) + y * y + z * z);

            tmean += time;
            count += 1;

            fh_veto_seg_qdcbarid->Fill(barId, qdc);
            fh_veto_seg_yx->Fill(x, y);
            if (barId < 51)
            {
                fh_veto_seg_yx1->Fill(x, y);
            }
            else if (barId < 101)
            {
                fh_veto_seg_yx2->Fill(x, y);
            }

            if (startSeen)
            {
                fh_los_corr->Fill(time, startTime);

                fh_veto_seg_barid->Fill(barId);
                fh_veto_seg_timebarid->Fill(barId, time);
                time2 = time - startTime - fTimeOffset;
                beta = l / time2;
                normt = time2 - (l - 1300.) / beta;
                fh_veto_seg_qdctof->Fill(normt, qdc);
                fh_veto_seg_tofbarid->Fill(barId, time2);
                fh_veto_seg_lbarid->Fill(barId, l);
                fh_veto_seg_ltime->Fill(time2, l);
                fh_veto_seg_betabarid->Fill(barId, beta);
                fh_veto_seg_tof->Fill(time2);
                if (beta > 29.55 && beta < 29.95 && qdcL > 150. && qdcR > 150.)
                {
                    fh_veto_seg_norm_tof_total->Fill(normt);
                    if (barId < 51)
                    {
                        fh_veto_seg_norm_tof_plane->Fill(normt);
                        if (barId == 25)
                        {
                            fh_veto_seg_norm_tof_bar->Fill(normt);
                        }
                    }
                }
                fh_veto_seg_beta->Fill(beta);
                fh_veto_seg_qdc->Fill(qdc);
                if (beta > 16 && beta < 26)
                {
                    fh_veto_seg_qdc_cut->Fill(qdc);
                }
            }
        }
    }

    fnEvents += 1;
    if (0 == (fnEvents % 1000))
    {
        LOG(INFO) << "R3Bveto_segAna : " << fnEvents << " events collected,  start time=" << startTime;
        if (count)
        {
            LOG(INFO) << ",  mean time=" << (tmean / (Double_t)count);
        }
        LOG(INFO) << FairLogger::endl;
    }
}

void R3Bveto_segAna::FinishTask()
{
    WriteHistos();
}

void R3Bveto_segAna::CreateHistos()
{
    fh_veto_seg_barid = new TH1F("h_veto_seg_barid", "Bar ID", fNofBars, (Double_t)fNofBars + 0.5, 100.5);
    fh_veto_seg_qdcbarid =
        new TH2F("h_veto_seg_qdcbarid", "QDC vs Bar ID", fNofBars, 0.5, (Double_t)fNofBars + 0.5, 2000, 0., 2000.);
    fh_veto_seg_timebarid =
        new TH2F("h_veto_seg_timebarid", "Time vs Bar ID", fNofBars, 0.5, (Double_t)fNofBars + 0.5, 4000, 0., 2000.);
    fh_veto_seg_tofbarid =
        new TH2F("h_veto_seg_tofbarid", "ToF vs Bar ID", fNofBars, 0.5, (Double_t)fNofBars + 0.5, 1000, 0., 100.);
    fh_veto_seg_tof = new TH1F("h_veto_seg_tof", "ToF", 1000, 0., 100.);
    fh_veto_seg_qdctof = new TH2F("h_veto_seg_qdctof", "QDC vs ToF", 2000, -50., 150., 2000, 0., 2000.);
    fh_veto_seg_betabarid =
        new TH2F("h_veto_seg_betabarid", "Velocity vs Bar ID", fNofBars, 0.5, (Double_t)fNofBars + 0.5, 500, 0., 50.);
    fh_veto_seg_lbarid =
        new TH2F("h_veto_seg_lbarid", "Length vs Bar ID", fNofBars, 0.5, (Double_t)fNofBars + 0.5, 1000, 700., 900.);
    fh_veto_seg_ltime = new TH2F("h_veto_seg_ltime", "Length vs Bar ID", 4000, 0., 2000., 1000, 700., 900.);
    fh_veto_seg_yx = new TH2F("h_veto_seg_yx", "Y vs X", 340, -170., 170., 340, -170., 170.);
    if (fFirstPlaneHorisontal)
    {
        fh_veto_seg_yx1 = new TH2F("h_veto_seg_yx1", "Y vs X", 340, -170., 170., 68, -170., 170.);
        fh_veto_seg_yx2 = new TH2F("h_veto_seg_yx2", "Y vs X", 68, -170., 170., 340, -170., 170.);
    }
    else
    {
        fh_veto_seg_yx1 = new TH2F("h_veto_seg_yx1", "Y vs X", 68, -170., 170., 340, -170., 170.);
        fh_veto_seg_yx2 = new TH2F("h_veto_seg_yx2", "Y vs X", 340, -170., 170., 68, -170., 170.);
    }
    fh_veto_seg_beta = new TH1F("h_veto_seg_beta", "Velocity", 4500, -10., 35.);
    fh_veto_seg_qdc = new TH1F("h_veto_seg_qdc", "QDC", 2000, 0., 2000.);
    fh_veto_seg_qdc_cut = new TH1F("h_veto_seg_qdc_cut", "QDC cut", 2000, 0., 2000.);
    fh_los_time = new TH1F("h_los_time", "LOS Time", 2000, -1000., -800.);
    fh_los_corr = new TH2F("h_los_corr", "LOS vs Nveto_seg", 2000, 700., 900., 2000, -1000., -800.);

    fh_veto_seg_norm_tof_bar = new TH1F("h_veto_seg_norm_tof_bar", "norm. ToF (1 bar)", 50, 40., 50.);
    fh_veto_seg_norm_tof_plane = new TH1F("h_veto_seg_norm_tof_plane", "norm. ToF (1 plane)", 100, 40., 50.);
    fh_veto_seg_norm_tof_total = new TH1F("h_veto_seg_norm_tof_total", "norm. ToF (total)", 1000, 40., 50.);
}

void R3Bveto_segAna::WriteHistos()
{
    fh_veto_seg_barid->Write();
    fh_veto_seg_qdcbarid->Write();
    fh_veto_seg_tof->Write();
    fh_veto_seg_qdctof->Write();
    fh_veto_seg_timebarid->Write();
    fh_veto_seg_tofbarid->Write();
    fh_veto_seg_yx->Write();
    fh_veto_seg_yx1->Write();
    fh_veto_seg_yx2->Write();
    fh_veto_seg_lbarid->Write();
    fh_veto_seg_ltime->Write();
    fh_veto_seg_beta->Write();
    fh_veto_seg_qdc->Write();
    fh_veto_seg_qdc_cut->Write();
    fh_veto_seg_betabarid->Write();
    fh_los_time->Write();
    fh_los_corr->Write();
    fh_veto_seg_norm_tof_bar->Write();
    fh_veto_seg_norm_tof_plane->Write();
    fh_veto_seg_norm_tof_total->Write();
}

ClassImp(R3Bveto_segAna)
