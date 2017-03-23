#include "TClonesArray.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TMath.h"

#include "FairRootManager.h"
#include "FairLogger.h"

#include "R3BNveto_segCalData.h"
#include "R3BNveto_segPmt.h"
#include "R3BLosHit.h"
#include "R3BNveto_segCalTest.h"

Double_t walk(Double_t x)
{
    Double_t y=0;
    
    Double_t par1=1500.; // +-0.2238
    Double_t par2=0.00075;//+-2.355e-05
    y=par1*TMath::Power(x,par2)-(par1*TMath::Power(400.,par2)); // Michael's
    
    //y=2.29083*log(x)-0.0870157*log(x)*log(x)-4.57824;  // mine
    
    return y;
    //return 0.;
}

R3BNveto_segCalTest::R3BNveto_segCalTest()
    : fnEvents(0)
    , fveto_segPmt(NULL)
    , fNveto_segPmt(NULL)
    , fLosHit(NULL)    
{
}

R3BNveto_segCalTest::R3BNveto_segCalTest(const char* name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , fnEvents(0)
    , fveto_segPmt(NULL)
    , fNveto_segPmt(NULL)
    , fLosHit(NULL)
{
}

R3BNveto_segCalTest::~R3BNveto_segCalTest()
{
}

InitStatus R3BNveto_segCalTest::Init()
{
    FairRootManager* fMan = FairRootManager::Instance();
    if (!fMan)
    {
        FairLogger::GetLogger()->Fatal(MESSAGE_ORIGIN, "FairRootManager not found");
    }
    fveto_segPmt = (TClonesArray*)fMan->GetObject("Nveto_segCalData");
    fNveto_segPmt = (TClonesArray*)fMan->GetObject("Nveto_segPmt");
    fLosHit = (TClonesArray*)fMan->GetObject("LosHit");
    CreateHistos();

    return kSUCCESS;
}

void R3BNveto_segCalTest::Exec(Option_t* option)
{
    Bool_t startSeen = kTRUE;
    Double_t startTime = 0.;
    if (fLosHit)
    {
        Int_t nLosHit = fLosHit->GetEntriesFast();
        Double_t stime[4];
        R3BLosHit* losHit;
        Int_t ch;
        Bool_t seen[] = { kFALSE, kFALSE, kFALSE, kFALSE};
        for (Int_t i = 0; i < nLosHit; i++)
        {
            losHit = (R3BLosHit*)fLosHit->At(i);
            ch = losHit->GetChannel();
            if (ch > 3)
            {
                continue;
            }
            else
            {
                seen[ch] = kTRUE;
                stime[ch] = losHit->GetTime();
            }
        }

        for (Int_t i = 0; i < 4; i++)
        {
            if(1 == i)
            {
                continue;
            }
            if (!seen[i])
            {
                startSeen = kFALSE;
                break;
            }
        }
        if (startSeen)
        {
            startTime = (stime[0] + stime[2] + stime[3]) / 3.;
        }
    }

    if (fveto_segPmt)
    {
        Int_t nveto_segPmt = fveto_segPmt->GetEntriesFast();
        R3BNveto_segCalData* pmt1;
        R3BNveto_segCalData* pmt2;
        Int_t barId;
        Double_t time1, time2;
        for (Int_t i1 = 0; i1 < nveto_segPmt; i1++)
        {
            pmt1 = (R3BNveto_segCalData*)fveto_segPmt->At(i1);
            barId = pmt1->GetBarId();
            time1 = pmt1->GetTime();
            time1 += walk(pmt1->GetQdc());

            for (Int_t i2 = 0; i2 < nveto_segPmt; i2++)
            {
                if (i2 == i1)
                {
                    continue;
                }
                pmt2 = (R3BNveto_segCalData*)fveto_segPmt->At(i2);
                if (barId != pmt2->GetBarId())
                {
                    continue;
                }
                time2 = pmt2->GetTime();
                time2 += walk(pmt2->GetQdc());

                if (startSeen && barId == 125)
                {
                    fh_los_corr->Fill((time1 + time2) / 2., startTime);
                    fh_tof->Fill((time1 + time2) / 2. - startTime);
                    fh_qdctof->Fill((time1 + time2) / 2. - startTime - 1563. + 30., TMath::Sqrt(pmt1->GetQdc()*pmt2->GetQdc()));
                }
                if (startSeen && barId == 225)
                {
                    fh_qdctof->Fill((time1 + time2) / 2. - startTime - 1571. + 30., TMath::Sqrt(pmt1->GetQdc()*pmt2->GetQdc()));
                }
            }
        }
    }
    if (fNveto_segPmt)
    {
        Int_t nNveto_segPmt = fNveto_segPmt->GetEntriesFast();
        R3BNveto_segPmt* pmt1;
        R3BNveto_segPmt* pmt2;
        Int_t planeId;
        Int_t barId;
        Double_t time1, time2;
        for (Int_t i1 = 0; i1 < nNveto_segPmt; i1++)
        {
            pmt1 = (R3BNveto_segPmt*)fNveto_segPmt->At(i1);
            planeId = pmt1->GetPlaneId();            
            barId = pmt1->GetBarId();
            time1 = pmt1->GetTime();
            time1 += walk(pmt1->GetCharge());

            for (Int_t i2 = i1+1; i2 < nNveto_segPmt; i2++)
            {
                if (i2 == i1)
                {
                    continue;
                }
                pmt2 = (R3BNveto_segPmt*)fNveto_segPmt->At(i2);
                if (barId != pmt2->GetBarId())
                {
                    continue;
                }
                time2 = pmt2->GetTime();
                time2 += walk(pmt2->GetCharge());

                fh_tdiff->Fill(time1 - time2);
/*                
                if (startSeen && barId == 125)
                {
                    fh_los_corr->Fill((time1 + time2) / 2., startTime);
                    fh_tof->Fill((time1 + time2) / 2. - startTime);
                    fh_qdctof->Fill((time1 + time2) / 2. - startTime - 1563. + 30., TMath::Sqrt(pmt1->GetQdc()*pmt2->GetQdc()));
                }
                if (startSeen && barId == 225)
                {
                    fh_qdctof->Fill((time1 + time2) / 2. - startTime - 1571. + 30., TMath::Sqrt(pmt1->GetQdc()*pmt2->GetQdc()));
                }
*/                
            }
        }
    }

    fnEvents += 1;
}

void R3BNveto_segCalTest::FinishTask()
{
    WriteHistos();
}

void R3BNveto_segCalTest::CreateHistos()
{
    fh_los_corr = new TH2F("h_los_corr", "LOS vs Nveto_seg", 1000, 100, 400., 1000, -1360., -1310.);
    fh_tof = new TH1F("h_tof", "ToF", 1000, 1500., 1700.);
    fh_qdctof = new TH2F("h_qdctof", "QDC vs ToF", 1000, 0., 100., 200, 0., 2000.);
    fh_qdctof_2 = new TH2F("h_qdctof_2", "QDC vs ToF", 1000, 0., 100., 200, 0., 2000.);
    fh_tdiff = new TH1F("h_tdiff", "Tdiff", 10000, -50, 50.);
}

void R3BNveto_segCalTest::WriteHistos()
{
    fh_los_corr->Write();
    fh_tof->Write();
    fh_qdctof->Write();
    fh_qdctof_2->Write();
    fh_tdiff->Write();
}

ClassImp(R3BNveto_segCalTest)
