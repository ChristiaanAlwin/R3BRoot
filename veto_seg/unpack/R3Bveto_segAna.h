// -----------------------------------------------------------------------------
// -----                               R3Bveto_segAna                          -----
// -----                     Created 22-04-2014 by D.Kresan                -----
// -----------------------------------------------------------------------------

#ifndef R3BVETO_SEGANA_H
#define R3BVETO_SEGANA_H

#include "FairTask.h"

class TClonesArray;
class TH1F;
class TH2F;

class R3Bveto_segAna : public FairTask
{
  public:
    R3Bveto_segAna();
    R3Bveto_segAna(const char* name, Int_t iVerbose);
    virtual ~R3Bveto_segAna();

    virtual InitStatus Init();

    virtual void Exec(Option_t* option);

    virtual void FinishTask();
    
    inline void SetNofBars(Int_t nofBars) { fNofBars = nofBars; }
    
    inline void SetFirstPlaneHorisontal() { fFirstPlaneHorisontal = kTRUE; }
    
    inline void SetMinimalLOSSignals(Int_t n) { fMinimalLOSSignals = n; }
    
    inline void SetTimeOffset(Double_t t) { fTimeOffset = t; }

  private:
    Int_t fNofBars;
    Int_t fMinimalLOSSignals = 4;
    Double_t fTimeOffset;
    Bool_t fFirstPlaneHorisontal;
    
    Int_t fnEvents;

    TClonesArray* fveto_segDigi;
    TClonesArray* fLosHit;

    TH1F* fh_veto_seg_barid;
    TH2F* fh_veto_seg_qdcbarid;
    TH1F* fh_veto_seg_tof;
    TH2F* fh_veto_seg_qdctof;
    TH2F* fh_veto_seg_timebarid;
    TH2F* fh_veto_seg_tofbarid;
    TH2F* fh_veto_seg_betabarid;
    TH2F* fh_veto_seg_yx;
    TH2F* fh_veto_seg_yx1;
    TH2F* fh_veto_seg_yx2;
    TH2F* fh_veto_seg_lbarid;
    TH2F* fh_veto_seg_ltime;
    
    TH1F* fh_veto_seg_beta;
    TH1F* fh_veto_seg_qdc;
    TH1F* fh_veto_seg_qdc_cut;
    
    TH1F* fh_los_time;
    
    TH2F* fh_los_corr;
    
    TH1F* fh_veto_seg_norm_tof_bar;
    TH1F* fh_veto_seg_norm_tof_plane;
    TH1F* fh_veto_seg_norm_tof_total;
        
    void CreateHistos();

    void WriteHistos();

  public:
    ClassDef(R3Bveto_segAna, 0)
};

#endif
