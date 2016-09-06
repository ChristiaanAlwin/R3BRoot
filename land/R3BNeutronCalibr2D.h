// -----------------------------------------------------------------------------
// -----                       R3BNeutronCalibr2D                          -----
// -----                 Created 27-02-2012 by D.Kresan                    -----
// -----------------------------------------------------------------------------

#ifndef R3BNEUTRONCALIBR2D_H
#define R3BNEUTRONCALIBR2D_H

#include "FairTask.h"

class TClonesArray;
class TH1F;
class TH2F;


class R3BNeutronCalibr2D : public FairTask
{
 public:
  /** Default constructor **/  
  R3BNeutronCalibr2D();
  
  /** Destructor **/
  ~R3BNeutronCalibr2D();

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
  ClassDef(R3BNeutronCalibr2D,1)  
};


#endif
