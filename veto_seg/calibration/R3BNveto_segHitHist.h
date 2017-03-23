// -----------------------------------------------------------------------------
// -----                            R3BNveto_segHitHist                      -----
// -----                     Created 22-04-2014 by D.Kresan                -----
// -----------------------------------------------------------------------------

#ifndef R3BNVETO_SEGHITHIST_H
#define R3BNVETO_SEGHITHIST_H

#include "FairTask.h"

class TClonesArray;
class TH1F;
class TH2F;

class R3BNveto_segHitHist : public FairTask
{
  public:
    R3BNveto_segHitHist();
    R3BNveto_segHitHist(const char* name, Int_t iVerbose);
    virtual ~R3BNveto_segHitHist();

    virtual InitStatus Init();

    virtual void Exec(Option_t* option);

    virtual void FinishTask();
    
    inline void SetNofBars(Int_t nofBars) { fNofBars = nofBars; }
    
    inline void SetFirstPlaneHorisontal() { fFirstPlaneHorisontal = kTRUE; }
    
    inline void SetTimeOffset(Double_t t) { fTimeOffset = t; }
    
    inline void SetDistance(Double_t t) { fDistance = t; }

  private:
    Int_t fNofBars;
    Double_t fTimeOffset;
    Bool_t fFirstPlaneHorisontal;
    
    Int_t fnEvents;
    Double_t fDistance;

    TClonesArray* fveto_segDigi;
    TClonesArray* fLosHit;

    TH1F* fh_veto_seg_barid;
    TH2F* fh_veto_seg_qdcbarid;
    TH1F* fh_veto_seg_tof;
    TH2F* fh_veto_seg_qdctof;
    TH2F* fh_veto_seg_qdctof_norm;
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
    
    TH1F* fh_veto_seg_norm_tof;
        
    void CreateHistos();

    void WriteHistos();

  public:
    ClassDef(R3BNveto_segHitHist, 0)
};

#endif
