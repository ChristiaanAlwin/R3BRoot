#ifndef R3BNVETO_SEGCALTEST_H
#define R3BNVETO_SEGCALTEST_H

#include "FairTask.h"

class TClonesArray;
class TH1F;
class TH2F;

class R3BNveto_segCalTest : public FairTask
{
  public:
    R3BNveto_segCalTest();
    R3BNveto_segCalTest(const char* name, Int_t iVerbose);
    virtual ~R3BNveto_segCalTest();

    virtual InitStatus Init();

    virtual void Exec(Option_t* option);

    virtual void FinishTask();

  private:
    Int_t fnEvents;

    TClonesArray* fveto_segPmt;
    TClonesArray* fNveto_segPmt;
    TClonesArray* fLosHit;

    TH2F* fh_los_corr;
    TH1F* fh_tof;
    TH2F* fh_qdctof;
    TH1F* fh_tdiff;
    

    TH2F* fh_qdctof_2;

    void CreateHistos();

    void WriteHistos();

  public:
    ClassDef(R3BNveto_segCalTest, 1)
};

#endif
