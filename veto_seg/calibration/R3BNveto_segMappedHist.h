// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                            R3Bveto_segRawAna                          -----
// -----                      Created  @ 01.2014 by M.I. Cherciu           -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef R3BNVETO_SEGMAPPEDHIST_H
#define R3BNVETO_SEGMAPPEDHIST_H

#include "FairTask.h"

class R3BEventHeader;
class TClonesArray;
class TH1F;
class TH2F;

class R3BNveto_segMappedHist : public FairTask
{
  public:
    R3BNveto_segMappedHist();
    R3BNveto_segMappedHist(const char* name, Int_t iVerbose);
    virtual ~R3BNveto_segMappedHist();

    virtual InitStatus Init();

    virtual void Exec(Option_t* option);

    virtual void FinishTask();

    inline Int_t GetNItemsTotal()
    {
        return fNItemsTotal;
    }

  private:
    Int_t fnEvents;
    Int_t fNItemsTotal;

    R3BEventHeader* fHeader;
    TClonesArray* fveto_segMappedData;
    TClonesArray* fNveto_segTamexHitMapped;
    TClonesArray* fLosMappedData;

    TH1F* fh_trigger;

    TH1F* fh_veto_seg_mapped_barid;
    TH1F* fh_veto_seg_mapped_side;
    TH1F* fh_veto_seg_mapped_clock;
    TH1F* fh_veto_seg_mapped_tac;
    TH1F* fh_veto_seg_mapped_qdc;

    TH1F* fh_Nveto_seg_mapped_is17;
    TH1F* fh_Nveto_seg_mapped_planeid;
    TH1F* fh_Nveto_seg_mapped_barid;
    TH1F* fh_Nveto_seg_mapped_side;
    TH1F* fh_Nveto_seg_mapped_cle;
    TH1F* fh_Nveto_seg_mapped_cte;
    TH1F* fh_Nveto_seg_mapped_fle;
    TH1F* fh_Nveto_seg_mapped_fte;

    TH1F* fh_los_det;
    TH1F* fh_los_ch;
    TH1F* fh_los_tcoarse;
    TH1F* fh_los_tfine;

    void CreateHistos();

    void WriteHistos();

  public:
    ClassDef(R3BNveto_segMappedHist, 0)
};

#endif
