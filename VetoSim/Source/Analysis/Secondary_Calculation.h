// This header contains several small functions that are used to compute some
// quantities for the secondary particle analysis.

// Written by C. A. Douma, for the R3B collaboration.

// inclusion of required headers:
#include "TMath.h"
#include "TVector3.h"
#include "TRandom3.h"

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
  Double_t FullTravelDistance = 1e99; // if not aimed at the VETO...
  Double_t Ztravel = 0.0;
  Double_t Xtravel = 0.0;
  Double_t Ytravel = 0.0;
  Bool_t vetohit = kFALSE;
  Bool_t vetohit1 = kFALSE;
  Bool_t vetohit2 = kFALSE;
  
  // Now distinguish between in front of the VETO and behind the VETO:
  if (Zin>VETOhalfThickness) // behind the VETO (in the frame with VETO at the center):
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
    
    // Now we must do the same for the second-to-last layer:
    Ztravel = Zin + (VETOhalfThickness - Inputs->VETO_geo_TotalBarThickness) - VETOhalfThickness;
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
    
    // Then compute the travel distance:
    if (vetohit2) {FullTravelDistance = TMath::Sqrt(Xtravel*Xtravel + Ytravel*Ytravel + Ztravel*Ztravel);}
    if (vetohit1) {FullTravelDistance = TMath::Sqrt(Xtravel*Xtravel + Ytravel*Ytravel + Ztravel*Ztravel);} // we do this one last, since it is a shorter route.
    
    // Combine the boolians:
    vetohit = (vetohit1||vetohit2);
    
  }
  else if (Zin<-1.0*VETOhalfThickness)
  {
    // Now transport Xin, Yin, Zin along theta and phi:	
    Ztravel = TMath::Abs(Zin - (-1.0*VETOhalfThickness));
    Xtravel = Ztravel*TMath::Tan(theta)*TMath::Cos(phi);
    Ytravel = Ztravel*TMath::Tan(theta)*TMath::Sin(phi);
	 
    // We always have a horizontal layer in the begin:
    vetohit1 = ((Xtravel+Xin)<(VETOhalfwidth))&&((Xtravel+Xin)>(-1.0*VETOhalfwidth))&&((Ytravel+Yin)<(VETOhalfheight))&&((Ytravel+Yin)>(-1.0*VETOhalfheight))&&(theta<(0.5*TMath::Pi()));
    
    // Now do the same for the second-to-last layer:
    Ztravel = TMath::Abs(Zin - (-1.0*VETOhalfThickness + (Inputs->VETO_geo_TotalBarThickness)));
    Xtravel = Ztravel*TMath::Tan(theta)*TMath::Cos(phi);
    Ytravel = Ztravel*TMath::Tan(theta)*TMath::Sin(phi);
	 
    // Second layer is always verical:
    vetohit2 = ((Xtravel+Xin)<(VETOhalfheight))&&((Xtravel+Xin)>(-1.0*VETOhalfheight))&&((Ytravel+Yin)<(VETOhalfwidth))&&((Ytravel+Yin)>(-1.0*VETOhalfwidth))&&(theta<(0.5*TMath::Pi()));
    
    // Then compute the travel distance:
    if (vetohit2) {FullTravelDistance = TMath::Sqrt(Xtravel*Xtravel + Ytravel*Ytravel + Ztravel*Ztravel);}
    if (vetohit1) {FullTravelDistance = TMath::Sqrt(Xtravel*Xtravel + Ytravel*Ytravel + Ztravel*Ztravel);} // we do this one last, since it is a shorter route.
    
    // Combine the boolians:
    vetohit = (vetohit1||vetohit2);
  }
  else {} // Then it is inside the VETO: we do not do anything. Hence vetohit=kFALSE and TravelDist=1e99
  
  // Now vetohit should say if we indeed did hit the VETO yes/no.
  // the traveled distance to the VETO is covered by FullTravelDistance, which is positive for sure. Hence:
  if (!vetohit) {FullTravelDistance = -1.0*FullTravelDistance;}
  // Now the sign of the traveldistance carries whether we hit the VETO yes/no. If we do not hit, probably |TravelDist|=1e99

  // Hence we can return the answer. 
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
  
  // compute theta and phi:
  Double_t theta = GetTheta(pxin,pyin,pzin);
  Double_t phi = GetPhi(pxin,pyin);
  
  // Get some dimensions of NeuLAND:
  Double_t NeuLANDhalfThickness = 2.5*((Int_t) Inputs->NeuLAND_geo_NsinglePlanes);
  Double_t NeuLANDhalfheight = 125.0;
  Double_t NeuLANDhalfwidth = 125.0;
  
  // Declare the variables we need later on:
  Double_t Xtravel = 0.0;
  Double_t Ytravel = 0.0;
  Double_t Ztravel = 0.0;
  Bool_t Frontend = kFALSE;
  Bool_t Backend = kFALSE;
  Bool_t Xtof = kFALSE;
  Bool_t Xother = kFALSE;
  Bool_t Ylow = kFALSE;
  Bool_t Yup = kFALSE;
  
  // Now the difficult part is that we have to test on each side of NeuLAND
  // whether the particle leaves through this side yes/no. Hence test this:
  
  // Test Zfront (particle gun):
  Ztravel = TMath::Abs(Zin - (-1.0*NeuLANDhalfThickness));
  Xtravel = Ztravel*TMath::Tan(TMath::Pi() - theta)*TMath::Cos(phi);
  Ytravel = Ztravel*TMath::Tan(TMath::Pi() - theta)*TMath::Sin(phi);
  Frontend = (((Xtravel+Xin)<(NeuLANDhalfwidth))&&((Xtravel+Xin)>(-1.0*NeuLANDhalfwidth))&&((Ytravel+Yin)<(NeuLANDhalfheight))&&((Ytravel+Yin)>(-1.0*NeuLANDhalfheight))&&(theta>0.5*TMath::Pi()));
  if (Frontend) {TravelDist = TMath::Sqrt(Ytravel*Ytravel+Xtravel*Xtravel+Ztravel*Ztravel);}
  
  // Test Zback:
  Ztravel = TMath::Abs(NeuLANDhalfThickness - Zin);
  Xtravel = Ztravel*TMath::Tan(theta)*TMath::Cos(phi);
  Ytravel = Ztravel*TMath::Tan(theta)*TMath::Sin(phi);
  Backend = (((Xtravel+Xin)<(NeuLANDhalfwidth))&&((Xtravel+Xin)>(-1.0*NeuLANDhalfwidth))&&((Ytravel+Yin)<(NeuLANDhalfheight))&&((Ytravel+Yin)>(-1.0*NeuLANDhalfheight))&&(theta<0.5*TMath::Pi()));
  if (Backend) {TravelDist = TMath::Sqrt(Ytravel*Ytravel+Xtravel*Xtravel+Ztravel*Ztravel);}
  
  // Now if none of these cases is true, it leaves through one of the sides:
  if ((Frontend)&&(Backend)&&(Inputs->Bug_Report==2)) {cout << "### BUG REPORT: There is track that leaves NeuLAND at the front AND the back!\n";}
  if ((!Frontend)&&(!Backend))
  {
    // Then it leaves neuland through one of the sides. Determine which one with the 4 phi-angles:
    Double_t phi1 = GetPhi(NeuLANDhalfwidth-Xin,NeuLANDhalfheight-Yin); // corner between Xother and Yup.
    Double_t phi2 = GetPhi(-1.0*NeuLANDhalfwidth-Xin,NeuLANDhalfheight-Yin); // corner between Yup and Xtof
    Double_t phi3 = GetPhi(-1.0*NeuLANDhalfwidth-Xin,-1.0*NeuLANDhalfheight-Yin); // corner between Xtof and Ylow
    Double_t phi4 = GetPhi(NeuLANDhalfwidth-Xin,-1.0*NeuLANDhalfheight-Yin); // corner between Ylow and Xother.
    
    // Notice that 0<phi1<phi2<phi3<phi4<360; Hence:
    if ((phi1<phi)&&(phi<phi2))
    {
      // then it leaves at Yup.
      // Normalize the impulse in such a way that it ends at Yup (hence pyin>0):
      pxin = (pxin/pyin)*TMath::Abs(NeuLANDhalfheight-Yin);
      pzin = (pzin/pyin)*TMath::Abs(NeuLANDhalfheight-Yin);
      pyin = TMath::Abs(NeuLANDhalfheight-Yin);
      
      // then we now know the travel distance:
      TravelDist = TMath::Sqrt(pxin*pxin+pyin*pyin+pzin*pzin);
    }
    else if ((phi2<phi)&&(phi<phi3))
    {
      // then it leaves at Xtof.
      // Normalize the impulse in such a way that it ends at Xtof (hence pxin<0):
      pyin = -1.0*(pyin/pxin)*TMath::Abs(-1.0*NeuLANDhalfwidth-Xin);
      pzin = -1.0*(pzin/pxin)*TMath::Abs(-1.0*NeuLANDhalfwidth-Xin);
      pxin = -1.0*TMath::Abs(-1.0*NeuLANDhalfwidth-Xin);
      
      // then we now know the travel distance:
      TravelDist = TMath::Sqrt(pxin*pxin+pyin*pyin+pzin*pzin);
    }
    else if ((phi3<phi)&&(phi<phi4))
    {
      // then it leaves at Ylow.
      // Normalize the impulse in such a way that it ends at Yup (hence pyin<0):
      pxin = -1.0*(pxin/pyin)*TMath::Abs(-1.0*NeuLANDhalfheight-Yin);
      pzin = -1.0*(pzin/pyin)*TMath::Abs(-1.0*NeuLANDhalfheight-Yin);
      pyin = -1.0*TMath::Abs(-1.0*NeuLANDhalfheight-Yin);
      
      // then we now know the travel distance:
      TravelDist = TMath::Sqrt(pxin*pxin+pyin*pyin+pzin*pzin);
    }
    else if (((phi4<phi)&&(phi<(2.0*TMath::Pi())))||((0.0<phi)&&(phi<phi1)))
    {
      // then it leaves at Xother.
      // Normalize the impulse in such a way that it ends at Xtof (hence pxin>0):
      pyin = (pyin/pxin)*TMath::Abs(NeuLANDhalfwidth-Xin);
      pzin = (pzin/pxin)*TMath::Abs(NeuLANDhalfwidth-Xin);
      pxin = TMath::Abs(NeuLANDhalfwidth-Xin);
      
      // then we now know the travel distance:
      TravelDist = TMath::Sqrt(pxin*pxin+pyin*pyin+pzin*pzin);
    }
    else
    {
      if (Inputs->Bug_Report==2) {cout << "### BUG REPORT: There is a track that does not leave NeuLAND properly!\n";}
    }
  }
  
  // Now that we also checked the sides, we are done!
  
  }
  
  // But we are not done yet! any particle that comes out of NeuLAND should always penetrate 
  // at least one layer of reflective foil and tape around the VETO before it can enter the
  // active area of the VETO. Hence we will add the thickness of the foil and of the tape
  // (both one layer) to the travel distance through neuland. The tape is made of CH2, hence
  // its density will be rougly the same as that of BC408, so we can just add the thickness.
  // but the aluminium foil is much heavier. Hence we will weight the thickness by its density,
  // relative to the BC408 density (1032.0 kg/m^3). Aluminium is 2707 kg/m^3. Hence:
  TravelDist = TravelDist + Inputs->VETO_geo_TapeThickness + Inputs->VETO_geo_FoilThickness*(2707.0/1032.0);
  
  // Now we can finally return this travel distance.
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
  // axis. Note: bin 1 holds E=0, bin 2 holds one step higher, etc. In order to now obtain
  // the true travel distance we will do a lineair interpolation between 2 energy points.
  
  // However, for teh neutral particles we need to apply basic law of radiation first.
  // Hence declare the travel distance:
  Double_t TravelDist = 0.0;
  
  // then figure out the neutrons and gammas:
  if (PDG==2112) {TravelDist = generator->Exp(Inputs->Z_mean_neutron);}
  else if (PDG==22) {TravelDist = generator->Exp(Inputs->Z_mean_gamma);}
  else
  {
    // then we have a charged particle. Figure out between which bins the
    // energy is located:
    Double_t Econv = ((Int_t) Inputs->Nbins_Esec)*(Energy/(Inputs->Beam_Emax));
    Int_t Ebinlow = 1 + (Int_t) Econv; // if Energy=0, then Econv = 0 but this corresponds to bin 1 in Bethe.h!
    Int_t Ebinup = 2 + (Int_t) Econv;
    
    // Then get the travel distances:
    Double_t Distlow = 0.0;
    Double_t Distup = 0.0;
    if (PDG==2212) {Distlow = Inputs->Protons_TravelDist->GetBinContent(Ebinlow); Distup = Inputs->Protons_TravelDist->GetBinContent(Ebinup);}
    if (PDG==11) {Distlow = Inputs->Electrons_TravelDist->GetBinContent(Ebinlow); Distup = Inputs->Electrons_TravelDist->GetBinContent(Ebinup);}
    if (TMath::Abs(PDG)==211) {Distlow = Inputs->Pions_TravelDist->GetBinContent(Ebinlow); Distup = Inputs->Pions_TravelDist->GetBinContent(Ebinup);}
    if (PDG==13) {Distlow = Inputs->Muons_TravelDist->GetBinContent(Ebinlow); Distup = Inputs->Muons_TravelDist->GetBinContent(Ebinup);}
    if (PDG==1000020040) {Distlow = Inputs->Alphas_TravelDist->GetBinContent(Ebinlow); Distup = Inputs->Alphas_TravelDist->GetBinContent(Ebinup);}
    if ((PDG>1000060000)&&(PDG<1000060190)) {Distlow = Inputs->Cions_TravelDist->GetBinContent(Ebinlow); Distup = Inputs->Cions_TravelDist->GetBinContent(Ebinup);}
    
    // Now perform a lineair interpolation:
    Double_t Elow = (((Int_t) Ebinlow) - 1.0)*((Inputs->Beam_Emax)/((Int_t) Inputs->Nbins_Esec)); // Notice: Emin = 0.0;
    Double_t Eup = (((Int_t) Ebinup) - 1.0)*((Inputs->Beam_Emax)/((Int_t) Inputs->Nbins_Esec)); // Notice: Emin = 0.0;
    
    TravelDist = Distlow + ((Distup-Distlow)/(Eup-Elow))*(Energy-Elow);
    
    // Done!
  }
  
  //TravelDist = TravelDist - 0.1; // Gab size.
  
  // Now we can return the travel distance. NOTE: any particle that is not in this list still
  // has a maximum travel distance of zero and therefore never makes is to the VETO. Also notice that
  // since the TMath::Abs is missing with most particles, the anti-particles will therefore never
  // escape neuland. Notice that for pions we do include the anti-particles.
  
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
  // rule we can manage OK. Muons: 400 MeV travels 1590 cm. Alphas: 2.7 cm for 1 MeV/u.
  
  Double_t TravelDist = 0.0;
  if (TMath::Abs(PDG)==11) {TravelDist = 4135.0*(Energy/0.01);} // estimate on electron travel distance in the air.
  if (TMath::Abs(PDG)==1000020040) {TravelDist = 2.7*(Energy/0.04);} // estimate on alpha travel distance in the air.
  if (TMath::Abs(PDG)==13) {TravelDist = 1590.0*(Energy/0.4);} // estimate on muon travel distance in the air.
  return TravelDist;
}