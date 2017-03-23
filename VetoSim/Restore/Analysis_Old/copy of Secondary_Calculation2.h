// This header contains several small functions that are used to compute some
// quantities for the secondary particle analysis.

// Written by C. A. Douma, for the R3B collaboration.

// inclusion of required headers:
#include "TMath.h"
#include "TVector3.h"
#include "TRandom3.h"

Double_t GetTheta(Double_t const px, Double_t const py, Double_t const pz)
{
  // Computes theta-ange of emittance:
  Double_t theta = pz/TMath::Sqrt(px*px + py*py + pz*pz); // this is the cosine!
  theta = TMath::ACos(theta);
  return theta;
}

Double_t GetPhi(Double_t const px, Double_t const py)
{
  // Computes phi-ange of emittance:
  Double_t phi = 0.0;
  
  if (py==0.0)
  {
    if (px < 0) {phi = TMath::Pi();}
    else {phi = 0;}
  }
  else if (py < 0)
  {
    if (px < 0) {phi = 2*TMath::Pi() - TMath::ASin(TMath::Abs(py)/TMath::Sqrt(px*px + py*py));}
    else {phi = TMath::Pi() + TMath::ASin(TMath::Abs(py)/TMath::Sqrt(px*px + py*py));}	   
  }
  else
  {
    if (px < 0) {phi = TMath::Pi() - TMath::ASin(TMath::Abs(py)/TMath::Sqrt(px*px + py*py));}
    else {phi = TMath::ASin(TMath::Abs(py)/TMath::Sqrt(px*px + py*py));}	   
  }
	    
  return phi;
}
  

Double_t TrackAimedAtVETO(Inputs_Structure* const Inputs, Double_t const X, Double_t const Y, Double_t const Z, Double_t const px, Double_t const py, Double_t const pz)
{
  // This function calculates whether a track starting at X, Y, Z [cm] with momentum px, py, pz [GeV]
  // is aimed at the VETO yes/no. Therefore we require this track information as inputs,
  // along with the Inputs structure that tells us where the VETO is. We return a 
  // Double_t that holds the FullTravelDistance to the VETO. If this number is positive, it means that
  // the track is indeed aimed at the VETO. Negative means that it is not.
 
  // ============================================================================================
  // We begin with defining 'inner' values of X, Y, Z, px, py, pz.
  
  // We shift the starting point untill the center of th VETO is at the origin:
  Double_t Xin = X - Inputs->VETO_geo_Xpos;
  Double_t Yin = Y - Inputs->VETO_geo_Ypos;
  Double_t Zin = Z - Inputs->VETO_geo_Zpos;
  
  // No need to shift the impulses:
  Double_t pxin = px;
  Double_t pyin = py;
  Double_t pzin = pz;
  
  // Then we must now rotate the VETO back to standard orientation:
  TVector3 vec;
  vec.SetX(Xin);
  vec.SetY(Yin);
  vec.SetZ(Zin);
  
  TVector3 pvec;
  pvec.SetX(pxin);
  pvec.SetY(pyin);
  pvec.SetZ(pzin);

  vec.RotateZ(-1.0*Inputs->VETO_geo_RotZ);
  vec.RotateY(-1.0*Inputs->VETO_geo_RotY);
  vec.RotateX(-1.0*Inputs->VETO_geo_RotX);
  
  pvec.RotateZ(-1.0*Inputs->VETO_geo_RotZ);
  pvec.RotateY(-1.0*Inputs->VETO_geo_RotY);
  pvec.RotateX(-1.0*Inputs->VETO_geo_RotX);
  
  Xin = vec.X();
  Yin = vec.Y();
  Zin = vec.Z();
  
  pxin = pvec.X();
  pyin = pvec.Y();
  pzin = pvec.Z();
  
  // Then we compute theta and phi:
  Double_t theta = GetTheta(pxin,pyin,pzin);
  Double_t phi = GetPhi(pxin,pyin);
  
  // Get some dimensions of the VETO:
  Double_t VETOhalfThickness = 0.5*Inputs->VETO_geo_TotalBarThickness*((Int_t) Inputs->VETO_geo_NsinglePlanes);
  Double_t VETOhalfheight = 0.5*Inputs->VETO_geo_TotalBarWidth*((Int_t) Inputs->VETO_geo_NbarsPerPlane);
  Double_t VETOhalfwidth = 0.5*Inputs->VETO_geo_TotalBarLength;
 
  // Declare some variables that we need:
  Double_t FullTravelDistance = 0.0;
  Double_t Ztravel = 0.0;
  Double_t Xtravel = 0.0;
  Double_t Ytravel = 0.0;
  Bool_t vetohit = kFALSE;
  Bool_t vetohit1 = kFALSE;
  Bool_t vetohit2 = kFALSE;
  
  // Now distinguish between in front of the VETO and behind the VETO:
  if (Zin>0.0) // behind the VETO (in the frame with VETO at the center):
  {
    // Now transport Xin, Yin, Zin along theta and phi:	
    Ztravel = Zin - VETOhalfThickness;
    Xtravel = Ztravel*TMath::Tan(TMath::Pi() - theta)*TMath::Cos(phi);
    Ytravel = Ztravel*TMath::Tan(TMath::Pi() - theta)*TMath::Sin(phi);

    // Now distinguish if we have a vertical or horizontal layer in the end:
    if ((Inputs->VETO_geo_NsinglePlanes)%2==0)
    {
      // Final layer is vertical layer:
      vetohit1 = ((Xtravel+Xin)<(VETOhalfheight))&&((Xtravel+Xin)>(-1.0*VETOhalfheight))&&((Ytravel+Yin)<(VETOhalfwidth))&&((Ytravel+Yin)>(-1.0*VETOhalfwidth))&&(theta>(0.5*TMath::Pi()));
    }
    else
    {
      // Final layer is Horizontal layer:
      vetohit1 = ((Xtravel+Xin)<(VETOhalfwidth))&&((Xtravel+Xin)>(-1.0*VETOhalfwidth))&&((Ytravel+Yin)<(VETOhalfheight))&&((Ytravel+Yin)>(-1.0*VETOhalfheight))&&(theta>(0.5*TMath::Pi()));
    }
    
    if (vetohit1) {FullTravelDistance = TMath::Sqrt(Xtravel*Xtravel + Ytravel*Ytravel + Ztravel*Ztravel);}
    
    // Now we must do the same for the second-to-last layer:
    Ztravel = Zin - (VETOhalfThickness - Inputs->VETO_geo_TotalBarThickness);
    Xtravel = Ztravel*TMath::Tan(TMath::Pi() - theta)*TMath::Cos(phi);
    Ytravel = Ztravel*TMath::Tan(TMath::Pi() - theta)*TMath::Sin(phi);

    // Now distinguish if we have a vertical or horizontal layer in the end:
    if ((Inputs->VETO_geo_NsinglePlanes)%2==0)
    {
      // Second to Final layer is Horizontal layer:
      vetohit2 = ((Xtravel+Xin)<(VETOhalfwidth))&&((Xtravel+Xin)>(-1.0*VETOhalfwidth))&&((Ytravel+Yin)<(VETOhalfheight))&&((Ytravel+Yin)>(-1.0*VETOhalfheight))&&(theta>(0.5*TMath::Pi()));
    }
    else
    {
      // second to Final layer is vertical layer:
      vetohit2 = ((Xtravel+Xin)<(VETOhalfheight))&&((Xtravel+Xin)>(-1.0*VETOhalfheight))&&((Ytravel+Yin)<(VETOhalfwidth))&&((Ytravel+Yin)>(-1.0*VETOhalfwidth))&&(theta>(0.5*TMath::Pi()));
    }
    
    if (vetohit2) {FullTravelDistance = TMath::Sqrt(Xtravel*Xtravel + Ytravel*Ytravel + Ztravel*Ztravel);}
    
    // Combine:
    vetohit = (vetohit1||vetohit2);
    
  }
  else
  {
    // Now transport Xin, Yin, Zin along theta and phi:	
    Ztravel = -1.0*VETOhalfThickness - Zin;
    Xtravel = Ztravel*TMath::Tan(theta)*TMath::Cos(phi);
    Ytravel = Ztravel*TMath::Tan(theta)*TMath::Sin(phi);
	 
    // Now distinguish if we have a verticla or horizontal layer in the end:
    if ((Inputs->VETO_geo_NsinglePlanes)%2==0)
    {
      // Final layer is vertical layer:
      vetohit1 = ((Xtravel+Xin)<(VETOhalfheight))&&((Xtravel+Xin)>(-1.0*VETOhalfheight))&&((Ytravel+Yin)<(VETOhalfwidth))&&((Ytravel+Yin)>(-1.0*VETOhalfwidth))&&(theta>(0.5*TMath::Pi()));
    }
    else
    {
      // Final layer is Horizontal layer:
      vetohit1 = ((Xtravel+Xin)<(VETOhalfwidth))&&((Xtravel+Xin)>(-1.0*VETOhalfwidth))&&((Ytravel+Yin)<(VETOhalfheight))&&((Ytravel+Yin)>(-1.0*VETOhalfheight))&&(theta>(0.5*TMath::Pi()));
    }
    
    if (vetohit1) {FullTravelDistance = TMath::Sqrt(Xtravel*Xtravel + Ytravel*Ytravel + Ztravel*Ztravel);}
    
    // Now do the same for the second-to-last layer:
    Ztravel = -1.0*(VETOhalfThickness - Inputs->VETO_geo_TotalBarThickness) - Zin;
    Xtravel = Ztravel*TMath::Tan(theta)*TMath::Cos(phi);
    Ytravel = Ztravel*TMath::Tan(theta)*TMath::Sin(phi);
	 
    // Now distinguish if we have a vertical or horizontal layer in the end:
    if ((Inputs->VETO_geo_NsinglePlanes)%2==0)
    {
      // Second to Final layer is Horizontal layer:
      vetohit2 = ((Xtravel+Xin)<(VETOhalfwidth))&&((Xtravel+Xin)>(-1.0*VETOhalfwidth))&&((Ytravel+Yin)<(VETOhalfheight))&&((Ytravel+Yin)>(-1.0*VETOhalfheight))&&(theta>(0.5*TMath::Pi()));
    }
    else
    {
      // Second to Final layer is vertical layer:
      vetohit2 = ((Xtravel+Xin)<(VETOhalfheight))&&((Xtravel+Xin)>(-1.0*VETOhalfheight))&&((Ytravel+Yin)<(VETOhalfwidth))&&((Ytravel+Yin)>(-1.0*VETOhalfwidth))&&(theta>(0.5*TMath::Pi()));
    }
    
    if (vetohit2) {FullTravelDistance = TMath::Sqrt(Xtravel*Xtravel + Ytravel*Ytravel + Ztravel*Ztravel);}
    
    // Combine:
    vetohit = (vetohit1||vetohit2);
  }
  
  // Now before doing anything else, we decide that we do NOT want particles produced inside the VETO to be 'aimed' at the VETO:
  if (PointInsideVETO(Inputs,X,Y,Z)) {vetohit = kFALSE;} // NOTE: use old X, Y, Z here, because we perform the rotation there!
  
  // Now vetohit should say if we indeed did hit the VETO yes/no.
  // the traveled distance to teh VETO is covered by FullTravelDistance, which is positive for sure. Hence:
  if (!vetohit) {FullTravelDistance = -1.0*FullTravelDistance;}
  
  // now return the answer:
  return FullTravelDistance;  
}

Double_t GetTravelDistance(Inputs_Structure* const Inputs, Double_t const X, Double_t const Y, Double_t const Z, Double_t const px, Double_t const py, Double_t const pz)
{
  // This function calculates the distance a track starting at X, Y, Z with momentum px, py, pz [GeV]
  // has to travel through NeuLAND. Therefore we require this track information as inputs,
  // along with the Inputs structure that tells us where the VETO is. We return a Double_t
  // holding this distance [cm].
  
  // ============================================================================================
  // Now before we do anything: define the travel distance:
  Double_t TravelDist = 0.0001;
  
  // Now if the production point is outside NeuLAND, then this is the value that
  // should be returned. So before doing any heavy computations, check for this:
  if (PointInsideNeuLAND(Inputs,X,Y,Z))
  {
  // Only now do something:
    
  // we begin with defining inner values and shifting and rotating to a frame with NeuLAND
  // in the center:
  Double_t Xin = X - Inputs->NeuLAND_geo_Xpos;
  Double_t Yin = Y - Inputs->NeuLAND_geo_Ypos;
  Double_t Zin = Z - Inputs->NeuLAND_geo_Zpos;
  
  // No need to shift the impulses:
  Double_t pxin = px;
  Double_t pyin = py;
  Double_t pzin = pz;
  
  // Then we must now rotate NeuLAND back to standard orientation:
  TVector3 vec;
  vec.SetX(Xin);
  vec.SetY(Yin);
  vec.SetZ(Zin);
  
  TVector3 pvec;
  pvec.SetX(pxin);
  pvec.SetY(pyin);
  pvec.SetZ(pzin);

  vec.RotateZ(-1.0*Inputs->NeuLAND_geo_RotZ);
  vec.RotateY(-1.0*Inputs->NeuLAND_geo_RotY);
  vec.RotateX(-1.0*Inputs->NeuLAND_geo_RotX);
  
  pvec.RotateZ(-1.0*Inputs->NeuLAND_geo_RotZ);
  pvec.RotateY(-1.0*Inputs->NeuLAND_geo_RotY);
  pvec.RotateX(-1.0*Inputs->NeuLAND_geo_RotX);
  
  Xin = vec.X();
  Yin = vec.Y();
  Zin = vec.Z();
  
  pxin = pvec.X();
  pyin = pvec.Y();
  pzin = pvec.Z();
  
  // Then we compute theta and phi:
  Double_t theta = GetTheta(pxin,pyin,pzin);
  Double_t phi = GetPhi(pxin,pyin);
  
  // Get some dimensions of NeuLAND:
  Double_t NeuLANDhalfThickness = 2.5*((Int_t) Inputs->NeuLAND_geo_NsinglePlanes);
  Double_t NeuLANDhalfheight = 125.0;
  Double_t NeuLANDhalfwidth = 125.0;
  
  // Now if we know that the track leaves NeuLAND at the front (backscattering, then computing the
  // traveldistance through NeuLAND is not that hard:
  TravelDist = TMath::Abs(Zin - (-1.0*NeuLANDhalfThickness))/TMath::Abs(TMath::Cos(TMath::Pi() - theta)); // FIXME: What about the other sides of NeuLAND???
  
  }
  
  // Now return the travel distance:
  return TravelDist;
}

Double_t GetMaxTravel(Inputs_Structure* const Inputs, Double_t const Energy, Int_t const PDG, TRandom3* generator)
{
  // This function returns the maximum distance a particle with energy <<Energy>> can
  // travel througfh NeuLAND. The information is extracted from histograms
  // stored in Bethe.root. Obviously we also need the PDG identification for this. This distance
  // is then returned as output [cm]. We also need the generator as a non-const input because
  // we require that the seed is re-generated every time. Hence we cannot re-declare the generator
  // every time this function is called.
  
  // ============================================================================================
  // The bethe-histograms have Energy [GeV] on the x-axis and distance [cm] on the functional
  // axis. So all we need to do is to figure out in which bin the energy is located. This is easy:
  Double_t Energy_conv = (Inputs->Nbins_Esec)*Energy;
  Int_t Energy_int = (Double_t) Energy_conv; // This cuts anything below decimal point off.
					     // however, if we are in the first bin, then this
					     // number is zero, not 1. Hence:
  Energy_int = Energy_int+1;
  
  // Then get the travel distance:
  Double_t TravelDist = 0.0;
  
  if (PDG==2212) {TravelDist = Inputs->Protons_TravelDist->GetBinContent(Energy_int);} // NOTE: anti-particles will remain with travel distance zero!
  if (PDG==11) {TravelDist = Inputs->Electrons_TravelDist->GetBinContent(Energy_int);} // NOTE: anti-particles will remain with travel distance zero!
  if (PDG==211) {TravelDist = Inputs->Pions_TravelDist->GetBinContent(Energy_int);} // NOTE: anti-particles will remain with travel distance zero!
  if (PDG==13) {TravelDist = Inputs->Muons_TravelDist->GetBinContent(Energy_int);} // NOTE: anti-particles will remain with travel distance zero!
  
  // For Alphas and C-ions it is a bit more complex:
  if ((TMath::Abs(PDG)>1000060000)&&(TMath::Abs(PDG)<1000060190)) // NOTE: anti-Cions will also get a distance, but this doesn't matter...
  {
    Energy_conv = (Inputs->Nbins_Esec)*Energy/12.0;
    Energy_int = (Double_t) Energy_conv;
    Energy_int = Energy_int+1;
    TravelDist = Inputs->Cions_TravelDist->GetBinContent(Energy_int);
  }
  if (PDG==1000020040) // NOTE: anti-particles will remain with travel distance zero!
  {
    Energy_conv = (Inputs->Nbins_Esec)*Energy/4.0;
    Energy_int = (Double_t) Energy_conv;
    Energy_int = Energy_int+1;
    TravelDist = Inputs->Cions_TravelDist->GetBinContent(Energy_int);
  }
  
  // then now we have to come up with something for neutrons and gammas:
  if (PDG==2112) {TravelDist = generator->Exp(Inputs->Z_mean_neutron);}
  if (PDG==22) {TravelDist = generator->Exp(Inputs->Z_mean_gamma);}
  // So the travel distance through neuland is always the same, but sometimes
  // you are lucky that the random number is bigger and sometimes you are not:
  // this will attentuiate the bins in the same way!
  
  return TravelDist;
  // NOTE: if a particle is not in the list, its maximum travel distance will become zero!
  // NOTE: hence all the remaining fragments are never emitted!
}

Double_t GetMaxAirTravelDist(Inputs_Structure* Inputs, Double_t const Energy, Int_t const PDG)
{
  // This function returns the maximum distance an ELECTRON or ALPHA with energy <<Energy>> can
  // travel through the air. The information is NOT extracted from histograms
  // stored in Bethe.root but calculated instead. This distance is then returned as output [cm].
  
  // ============================================================================================
    // We know that electrons from 10 MeV can travel 4135 cm through air and that 
  // (especially for low energies) this is about linear with energy. It is computationally
  // too intensive to compute everything from the Bethe-Bloch formula, but with this
  // rule we can manage OK.
  
  // For alphas: 2.7 cm for 1 MeV/u:
  Double_t TravelDist = 0.0;
  if (TMath::Abs(PDG)==11) {TravelDist = 4135.0*(Energy/0.01);}
  if (TMath::Abs(PDG)==1000020040) {TravelDist = 2.7*(Energy/0.04);}
  
  return TravelDist;
}