#include <iostream>
#include "R3Bveto_segDigiPar.h"
#include "TMath.h"

R3Bveto_segDigiPar::R3Bveto_segDigiPar(const char* name, const char* title, const char* context)
  : FairParGenericSet(name,title,context)
{
  clear();
}

void R3Bveto_segDigiPar::putParams(FairParamList* list)
{
  std::cout<<"-I- R3Bveto_segDigiPar::putParams() called"<<std::endl;
  if(!list) return;
  list->add("max_paddle", (Int_t)nMaxPaddle);
  list->add("max_plane", (Int_t)nMaxPlane);
  list->add("paddle_length", (Double_t)paddleLength);
  list->add("paddle_height", (Double_t)paddleHeight);
  list->add("paddle_depth", (Double_t)paddleDepth);
  list->add("paddle_spacing", (Double_t)paddleSpacing);
  list->add("paddle_wrapping", (Double_t)paddleWrapping);
}

Bool_t R3Bveto_segDigiPar::getParams(FairParamList* list)
{
  std::cout<<"-I- R3Bveto_segDigiPar::getParams() called"<<std::endl;
  if (!list) return kFALSE;
  std::cout<<"-I- R3Bveto_segDigiPar::getParams() 1 ";
  
  if (!list->fill("max_paddle", &nMaxPaddle)) return kFALSE;
  if (!list->fill("max_plane", &nMaxPlane)) return kFALSE;
  if (!list->fill("paddle_length", &paddleLength)) return kFALSE;
  if (!list->fill("paddle_heigth", &paddleHeight)) return kFALSE;
  if (!list->fill("paddle_depth", &paddleDepth)) return kFALSE;
  if (!list->fill("paddle_spacing", &paddleSpacing)) return kFALSE;
  if (!list->fill("paddle_wrapping", &paddleWrapping)) return kFALSE;
  return kTRUE;
}


void R3Bveto_segDigiPar::Print(Option_t* option) const
{
    std::cout<<"-I- veto_seg Digi Parameters:"<<std::endl;
    std::cout<<"   Max Paddle   = "<<nMaxPaddle<<std::endl;
    std::cout<<"   Max Plane   = "<<nMaxPlane<<std::endl;
    std::cout<<"   Paddle Length   = "<<paddleLength<<std::endl;
    std::cout<<"   Paddle Heigth   = "<<paddleHeight<<std::endl;
    std::cout<<"   Paddle Depth   = "<<paddleDepth<<std::endl;
    std::cout<<"   Paddle Spacing = " << paddleSpacing << std::endl;
    std::cout<<"   Paddle Wrapping = " << paddleWrapping << std::endl;
}

ClassImp(R3Bveto_segDigiPar);
