// -----------------------------------------------------------------------------
// -----                       R3BNeutronCalib2D_seg                          -----
// -----                 Created 27-02-2012 by D.Kresan                    -----
// -----------------------------------------------------------------------------

#ifndef R3BNEUTRONCALIB2D_SEG_H
#define R3BNEUTRONCALIB2D_SEG_H

#include "FairTask.h"

class TClonesArray;
class TH1F;
class TH2F;


class R3BNeutronCalib2D_seg : public FairTask
{
 public:
  /** Default constructor **/  
  R3BNeutronCalib2D_seg();
  
  /** Destructor **/
  ~R3BNeutronCalib2D_seg();

  /** Virtual method Init **/
  virtual InitStatus Init();

  /** Virtual method Exec **/
  virtual void Exec(Option_t* opt);

  virtual void Finish();

  inline void UseBeam(Int_t beamEnergy) { fBeamEnergy = beamEnergy; }

inline void SetEnergyBoundary(Double_t Ebound) { fEhistBoundary_Energy = Ebound; }
// blank
inline void SetClusterBoundary(Double_t Cbound) { fEhistBoundary_Clusters = Cbound; }
// blank
 private:
  Int_t         fEventNo;
  Int_t         fBeamEnergy;
Double_t fEhistBoundary_Clusters = 1500.0;
Double_t fEhistBoundary_Energy = 150.0;
  TClonesArray *fArrayDigi;
  TClonesArray *fArrayCluster;
  TH1F         *fh_etot;
  TH2F         *fh_ncl_etot;

  void CreateHistograms();

 public: 
  ClassDef(R3BNeutronCalib2D_seg,1)  
};


#endif
