
#include <stdio.h>

#include "TClonesArray.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TMath.h"

#include "FairLogger.h"
#include "FairRootManager.h"

#include "R3BMCTrack.h"
#include "R3Bveto_segPoint.h"
#include "R3Bveto_segDigi.h"
#include "R3Bveto_segDigitizerQA.h"


R3Bveto_segDigitizerQA::R3Bveto_segDigitizerQA()
: FairTask("R3Bveto_segDigitizerQA"),
fEvent(0), fTracks(NULL), fPoints(NULL), fDigis(NULL)
{
}


R3Bveto_segDigitizerQA::R3Bveto_segDigitizerQA(const R3Bveto_segDigitizerQA& right)
: FairTask("R3Bveto_segDigitizerQA"),
fEvent(right.fEvent), fTracks(NULL), fPoints(NULL), fDigis(NULL)
{
}


R3Bveto_segDigitizerQA::~R3Bveto_segDigitizerQA()
{
}


InitStatus R3Bveto_segDigitizerQA::Init()
{
  FairRootManager *rm = FairRootManager::Instance();
  if(NULL == rm) {
    LOG(ERROR) << "R3Bveto_segDigitizerQA: no pointer to FairRootManager..." << FairLogger::endl;
    return kERROR;
  }
  
  fTracks = (TClonesArray*) rm->GetObject("MCTrack");
  if(NULL == fTracks) {
    LOG(ERROR) << "R3Bveto_segDigitizerQA: no MCTrack array..." << FairLogger::endl;
    return kERROR;
  }

  fPoints = (TClonesArray*) rm->GetObject("veto_segPoint");
  if(NULL == fPoints) {
    LOG(ERROR) << "R3Bveto_segDigitizerQA: no veto_segPoint array..." << FairLogger::endl;
    return kERROR;
  }

  fDigis = (TClonesArray*) rm->GetObject("veto_segDigi");
  if(NULL == fDigis) {
    LOG(ERROR) << "R3Bveto_segDigitizerQA: no veto_segDigi array..." << FairLogger::endl;
    return kERROR;
  }
  
  CreateHisto();
  
  return kSUCCESS;
}


void R3Bveto_segDigitizerQA::Exec(Option_t *option)
{
  if(NULL == fTracks || NULL == fPoints || NULL == fDigis) {
    return;
  }
  
  Double_t totEnergy = 0.;
  Double_t totEnergyLee = 0.;
  Double_t totEnergyReco = 0.;

  Int_t nTracks = fTracks->GetEntriesFast();
  
  Int_t nPoints = fPoints->GetEntriesFast();
  R3Bveto_segPoint *point;
  Double_t eloss;
  Int_t media;
  Double_t light;
  Double_t time;
  Int_t pdg;
  for (Int_t i = 0; i < nPoints; i++) {
    point = (R3Bveto_segPoint*) fPoints->At(i);
    eloss = point->GetEnergyLoss()*1000.;
    media = Int_t(point->GetPaddleType());
    light = point->GetLightYield()*1000.;
    if(eloss > 0. && 3 == media) {
      totEnergy += eloss;
      totEnergyLee += light;
      time = point->GetTime();
      pdg = ((R3BMCTrack*)fTracks->At(point->GetTrackID()))->GetPdgCode();
      fhElossPdg->Fill(pdg, eloss);
    }
  }
  
  Int_t nDigis = fDigis->GetEntriesFast();
  R3Bveto_segDigi *digi;
  Double_t qdc;
  Double_t tdc;
  Double_t x, y, z;
  Double_t beta;
  Double_t tdc_first = 1e10;
  Int_t first = -1;
  Double_t beta_first = 1e10;
  Int_t bfirst = -1;
  Double_t qdc_first;
  for (Int_t i = 0; i < nDigis; i++) {
    digi = (R3Bveto_segDigi*) fDigis->At(i);
    qdc = digi->GetQdc();
    fhPaddleE->Fill(qdc);
    fhElossTime->Fill(tdc, qdc);
    if(qdc > 0.) {
      totEnergyReco += qdc;
      tdc = digi->GetTdc();
      x = digi->GetXX();
      y = digi->GetYY();
      z = digi->GetZZ();
      fhQdcXL->Fill(y, digi->GetQdcL());
      fhQdcXR->Fill(y, digi->GetQdcR());
      fhThresh->Fill(digi->GetPaddleNr(), qdc);
      fhQdcRatio->Fill(digi->GetQdcL()/digi->GetQdcR());
      beta = TMath::Sqrt(x*x+y*y+z*z)/tdc/(29.9792458);
      if(tdc < tdc_first) {
        tdc_first = tdc;
        first = digi->GetPaddleNr();
        qdc_first = qdc;
      }
      if(beta < beta_first) {
        beta_first = beta;
        bfirst = digi->GetPaddleNr();
      }
    }
  }
  if(first > 0) {
    fhFirstTvsBar->Fill((Double_t)first);
    fhFirstE->Fill(qdc_first);
  }
  if(bfirst > 0) {
    fhFirstBvsBar->Fill(bfirst);
  }
  
  if(totEnergy > 0.) {
    fhTotEnergy->Fill(totEnergy);
    fhTotLight->Fill(totEnergyLee);
    fhTotEnergyRel->Fill(totEnergy/fBeamEnergy);
    fhTotLightRel->Fill(totEnergyLee/fBeamEnergy);
    fhElossLight->Fill(totEnergy, totEnergyLee);
    fhTotEnergyReco->Fill(totEnergyReco);
  }

  if(0 == (fEvent%100)) {
    char str1[100];
    char str2[100];
    char str3[100];
    char str4[100];
    sprintf(str1, "%8d", fEvent);
    sprintf(str2, "%4d", nTracks);
    sprintf(str3, "%4d", nPoints);
    sprintf(str4, "%4d", nDigis);
    LOG(INFO) << "R3Bveto_segDigitizerQA: event # " << str1
    << ", MCTrack: " << str2
    << ", veto_segPoint: " << str3
    << ", veto_segDigi: " << str4 << FairLogger::endl;
  }
}


void R3Bveto_segDigitizerQA::FinishEvent()
{
  fEvent += 1;
}


void R3Bveto_segDigitizerQA::Finish()
{
  fhTotEnergy->Write();
  fhTotLight->Write();
  fhTotEnergyRel->Write();
  fhTotLightRel->Write();
  fhElossLight->Write();
  fhTotEnergyReco->Write();
  fhPaddleE->Write();
  fhFirstTvsBar->Write();
  fhFirstBvsBar->Write();
  fhFirstE->Write();
  fhElossTime->Write();
  fhElossPdg->Write();
  fhQdcXL->Write();
  fhQdcXR->Write();
  fhThresh->Write();
  fhQdcRatio->Write();
}


void R3Bveto_segDigitizerQA::CreateHisto()
{
  fhTotEnergy = new TH1F("Total_Energy", "Total energy detected", 5000, 0., 5000.);
  fhTotLight = new TH1F("Total_Light", "Total light detected (energy equivalent)", 5000, 0., 5000.);
  fhTotEnergyRel = new TH1F("Total_Energy_rel", "Relative Total energy detected", 55, -0.02, 1.08);
  fhTotLightRel = new TH1F("Total_Light_rel", "Relative Total light detected (energy equivalent)", 55, -0.02, 1.08);
  fhElossLight = new TH2F("ElossLight", "Light quenching for protons", 5000, 0., 5000., 5000, 0., 5000.);
  fhTotEnergyReco = new TH1F("Total_Energy_Reco", "Total energy reconstructed", 5000, 0., 5000.);
//  fhPaddleE = new TH1F("PaddleEnergy", "", 506, 0., 101.2);
  fhPaddleE = new TH1F("PaddleEnergy", "", 1012, 0., 202.4);
  fhFirstTvsBar = new TH1F("FirstTvsBar", "", 155, 0., 155.);
  fhFirstBvsBar = new TH1F("FirstBvsBar", "", 155, 0., 155.);
  fhFirstE = new TH1F("FirstE", "", 200, 0., 200.);
  fhElossTime = new TH2F("ElossTime", "", 400, 0., 200., 2000, 0., 200.);
  fhElossPdg = new TH2F("ElossPdg", "", 30000, 0., 30000., 2000, 0., 200.);
  fhQdcXL = new TH2F("QdcXL", "", 300, -150., 150., 100, 0., 100.);
  fhQdcXR = new TH2F("QdcXR", "", 300, -150., 150., 100, 0., 100.);
  fhThresh = new TH2F("Thresh", "", 150, 0., 150., 500, 0., 100.);
  fhQdcRatio = new TH1F("QdcRatio", "", 100, -5., 5.);
}


ClassImp(R3Bveto_segDigitizerQA)
