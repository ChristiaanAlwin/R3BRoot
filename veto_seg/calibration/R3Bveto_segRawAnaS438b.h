// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                           R3Bveto_segRawAnaS438b                      -----
// -----                    Created  27-02-2015 by D. Kresan               -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef R3Bveto_segRawAnaS438bS438B_H
#define R3Bveto_segRawAnaS438bS438B_H

#include "FairTask.h"

class TClonesArray;
class TH1F;
class TH2F;
class R3BEventHeader;

class R3Bveto_segRawAnaS438b : public FairTask
{
  public:
    R3Bveto_segRawAnaS438b();
    R3Bveto_segRawAnaS438b(const char* name, Int_t iVerbose);
    virtual ~R3Bveto_segRawAnaS438b();

    virtual InitStatus Init();

    virtual void Exec(Option_t* option);

    virtual void FinishTask();

  private:
    Int_t fnEvents;

    R3BEventHeader *fHeader;
    TClonesArray* fveto_segRawHitMapped;
    TClonesArray* fCaloRawHit;

    TH1F* fh_trigger;
    TH1F* fh_trigger_cut;
    TH2F* fhNhits;
    TH1F* fh_veto_seg_qdc;
    TH2F* fh_veto_seg_qdc_ncalo;
    TH2F* fh_eveto_seg_ecalo;

    void CreateHistos();

    void WriteHistos();

  public:
    ClassDef(R3Bveto_segRawAnaS438b, 0)
};

#endif
