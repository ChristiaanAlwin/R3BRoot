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
  // We begin with shifting the coordinates X, Y, Z to the frame with its origin at the VETO center:
  Double_t Xin = X - Inputs->VETO_geo_Xpos;
  Double_t Yin = Y - Inputs->VETO_geo_Ypos;
  Double_t Zin = Z - Inputs->VETO_geo_Zpos;
  
  // Now we put these new values in a TVector3:
  TVector3 vec;
  vec.SetX(Xin);
  vec.SetY(Yin);
  vec.SetZ(Zin);
  
  // Also put the momentum in a TVector3:
  TVector3 pvec;
  pvec.SetX(px);
  pvec.SetY(py);
  pvec.SetZ(pz);
  
  // Now we will rotate both reverse w.r.t. the VETO geometry source code:
  vec.RotateZ(-1.0*Inputs->VETO_geo_RotZ);
  vec.RotateY(-1.0*Inputs->VETO_geo_RotY);
  vec.RotateX(-1.0*Inputs->VETO_geo_RotX);
  
  pvec.RotateZ(-1.0*Inputs->VETO_geo_RotZ);
  pvec.RotateY(-1.0*Inputs->VETO_geo_RotY);
  pvec.RotateX(-1.0*Inputs->VETO_geo_RotX);
  
  // And now we will obtain the correct X, Y and Z back:
  Xin = vec.X();
  Yin = vec.Y();
  Zin = vec.Z();
  
  Double_t pxin = pvec.X();
  Double_t pyin = pvec.Y();
  Double_t pzin = pvec.Z();
  
  // Then now we can compute theta and phi:
  Double_t theta = GetTheta(pxin,pyin,pzin);
  Double_t phi = GetPhi(pxin,pyin);
  
  // At this moment, the vETO is at the center of our frame and
  // it has standard orientation.

  // Now we will compute two points: one for the last layer of the VETO and one for the second-last
  // layer of the VETO. We transport the point Xin, Yin, Zin along theta and phi until the
  // Z-value is just inside this layer. Then we can test the X-values and Y-values. Remember that
  // we are in a frame where the VETO has standard orientation and has its center at the origin!
  
  Double_t X1 = 0.0;
  Double_t Y1 = 0.0;
  Double_t Z1 = 0.0;
  
  Double_t X2 = 0.0;
  Double_t Y2 = 0.0;
  Double_t Z2 = 0.0;
  
  // define VETO dimensions:
  Double_t VETOhalfThickness = 0.5*Inputs->VETO_geo_TotalBarThickness*((Int_t) Inputs->VETO_geo_NsinglePlanes);
  Double_t VETOhalfheight = 0.5*Inputs->VETO_geo_TotalBarWidth*((Int_t) Inputs->VETO_geo_NbarsPerPlane);
  Double_t VETOhalfwidth = 0.5*Inputs->VETO_geo_TotalBarLength;
  
  // then we know where Z1 and Z2 have to go:
  Z1 = VETOhalfThickness - 0.001;
  Z2 = VETOhalfThickness - Inputs->VETO_geo_TotalBarThickness - 0.001;
  
  // then now we can compute the other coordinates:
  X1 = (Z1-Zin)*TMath::Tan(theta)*TMath::Cos(phi); // NOTE: sign changes...
  Y1 = (Z1-Zin)*TMath::Tan(theta)*TMath::Sin(phi);
  X2 = (Z2-Zin)*TMath::Tan(theta)*TMath::Cos(phi);
  Y2 = (Z2-Zin)*TMath::Tan(theta)*TMath::Sin(phi);
  
  // Define the boolians for the test:
  Bool_t Layer1 = kFALSE;
  Bool_t Layer2 = kFALSE;
  
  // Define the full travel distance:
  Double_t FullTravelDist = 0.00001;
  
  // Then now we must make a distinction between odd and event layers:
  if ((Inputs->VETO_geo_NsinglePlanes)%2==0) // this means even!
  {
    // then the last layer is vertical! hence Y should be between the VETOhalfwidth
    // and X between the VETOhalfheight:
    if ((-1.0*VETOhalfheight<X1)&&(X1<VETOhalfheight)&&(-1.0*VETOhalfwidth<Y1)&&(Y1<VETOhalfwidth)) {Layer1 = kTRUE; FullTravelDist = TMath::Abs((Zin-Z1)/TMath::Cos(theta));}
    
    // but the second-last layer is horizontal:
    if ((-1.0*VETOhalfheight<Y2)&&(Y2<VETOhalfheight)&&(-1.0*VETOhalfwidth<X2)&&(X2<VETOhalfwidth)) {Layer2 = kTRUE; FullTravelDist = TMath::Abs((Zin-Z2)/TMath::Cos(theta));}
  }
  else
  {
    // Then it is the other way around:
    if ((-1.0*VETOhalfheight<X2)&&(X2<VETOhalfheight)&&(-1.0*VETOhalfwidth<Y2)&&(Y2<VETOhalfwidth)) {Layer1 = kTRUE; FullTravelDist = TMath::Abs((Zin-Z1)/TMath::Cos(theta));}
    if ((-1.0*VETOhalfheight<Y1)&&(Y1<VETOhalfheight)&&(-1.0*VETOhalfwidth<X1)&&(X1<VETOhalfwidth)) {Layer2 = kTRUE; FullTravelDist = TMath::Abs((Zin-Z2)/TMath::Cos(theta));}
  }
    
  // Then now create the test:
  Bool_t TheTest = (Layer1||Layer2);
 
  // Now we take modulus of FullTravelDist. Then if TheTest is kFALSE, we make it negative:
  if (TheTest)
  {
    FullTravelDist = TMath::Abs(FullTravelDist);
  }
  else
  {
    FullTravelDist = -1.0*TMath::Abs(FullTravelDist);
  }  
 
  // Then return the FullTravelDist:
  return FullTravelDist;
}

Double_t GetTravelDistance(Inputs_Structure* const Inputs, Double_t const X, Double_t const Y, Double_t const Z, Double_t const px, Double_t const py, Double_t const pz)
{
  // This function calculates the distance a track starting at X, Y, Z with momentum px, py, pz [GeV]
  // has to travel through NeuLAND. Therefore we require this track information as inputs,
  // along with the Inputs structure that tells us where the VETO is. We return a Double_t
  // holding this distance [cm].
  
  // ============================================================================================
  // Before we do anything, declare the travel distance and check if the production point is
  // inside NNeuLAND:
  Double_t TravelDist = 0.00001;
  
  if (PointInsideNeuLAND(Inputs,X,Y,Z))
  {
  // Then we can continue, else we do not do anything!
  
  // ============================================================================================
  // We do the same as with the track through the VETO. First we transform everything to a center
  // in whitch NeuLAND is at the origin and has standard orientation:
  Double_t Xin = X - Inputs->NeuLAND_geo_Xpos;
  Double_t Yin = Y - Inputs->NeuLAND_geo_Ypos;
  Double_t Zin = Z - Inputs->NeuLAND_geo_Zpos;
  
  TVector3 vec;
  vec.SetX(Xin);
  vec.SetY(Yin);
  vec.SetZ(Zin);
  
  TVector3 pvec;
  pvec.SetX(px);
  pvec.SetY(py);
  pvec.SetZ(pz);
  
  // Now we will rotate it reverse w.r.t. the NeuLAND geometry source code:
  vec.RotateZ(-1.0*Inputs->NeuLAND_geo_RotZ);
  vec.RotateY(-1.0*Inputs->NeuLAND_geo_RotY);
  vec.RotateX(-1.0*Inputs->NeuLAND_geo_RotX);
  
  pvec.RotateZ(-1.0*Inputs->NeuLAND_geo_RotZ);
  pvec.RotateY(-1.0*Inputs->NeuLAND_geo_RotY);
  pvec.RotateX(-1.0*Inputs->NeuLAND_geo_RotX);
  
  Xin = vec.X();
  Yin = vec.Y();
  Zin = vec.Z();
  
  Double_t pxin = pvec.X();
  Double_t pyin = pvec.Y();
  Double_t pzin = pvec.Z();
  
  // Declare traveldistance:
  TravelDist = 0.000001;
  
  // We are not going to do thius with theta and phi, but we do this in carthesian coordinates.
  // This is because NeuLAND is intrinsically square. First we need to distinuish on which side of
  // NeuLAND the particle is leaving:
  Double_t theta = GetTheta(pxin,pyin,pzin);
  
  // We also need the dimensions of NeuLAND:
  Double_t Neu_HalfWidth = 125.0;
  Double_t Neu_HalfThick = 2.5*((Int_t) Inputs->NeuLAND_geo_NsinglePlanes);
  
  if (theta<(0.5*TMath::Pi()))
  {
    // it will leave NeuLAND at the back. So normalize the momentum now in
    // such a way that the z-coordinate of px exactly hits the back of neuland:
    pxin = pxin*(Neu_HalfThick-Zin)/pzin;
    pyin = pyin*(Neu_HalfThick-Zin)/pzin;
    pzin = Neu_HalfThick-Zin;
    
    // Now we should determine if it will leave NeuLAND through the back or through
    // the side:
    if ((-1.0*Neu_HalfWidth<Xin+pxin)&&(Xin+pxin<Neu_HalfWidth)&&(-1.0*Neu_HalfWidth<Yin+pyin)&&(Yin+pyin<Neu_HalfWidth))
    {
      // Then we leave through the back:
      TravelDist = TMath::Abs(pzin/TMath::Cos(theta));
    }
    else
    {
      // Then we leave through the side. We have to know which side.
      // so compute phi:
      Double_t phi = GetPhi(pxin,pyin);
      
      // Compute boundaries:
      Double_t phi1 = (-1.0*Neu_HalfWidth-Xin,-1.0*Neu_HalfWidth-Yin);
      Double_t phi2 = (-1.0*Neu_HalfWidth-Xin,1.0*Neu_HalfWidth-Yin);
      Double_t phi3 = (1.0*Neu_HalfWidth-Xin,-1.0*Neu_HalfWidth-Yin);
      Double_t phi4 = (1.0*Neu_HalfWidth-Xin,1.0*Neu_HalfWidth-Yin);
      
      // Then the Ylow-surface means that phi is between phi1 and phi3.
      // the Yup surafce means between phi2 and phi4. The Xtof is between phi1 and phi2.
      // the Xother is between phi3 and phi4. notice that we always have that
      // 0deg<phi4<phi2<phi1<phi3<360deg by definition of these angles. Hence:
      if ((phi4<phi)&&(phi<phi2))
      {
	// This is the Yup: so re-normalize momentum to be on this surface:
	pxin = pxin*(Neu_HalfWidth-Yin)/pyin;
        pzin = pzin*(Neu_HalfWidth-Yin)/pyin;
        pyin = Neu_HalfWidth-Yin;
	
	// Then the travel distance equals:
	TravelDist = TMath::Abs(pzin/TMath::Cos(theta));
      }
      else if ((phi2<phi)&&(phi<phi1))
      {
	// This is the Xtof: so re-normalize momentum to be on this surface:
	pyin = pyin*(-1.0*Neu_HalfWidth-Xin)/pxin;
        pzin = pzin*(-1.0*Neu_HalfWidth-Xin)/pxin;
        pxin = -1.0*Neu_HalfWidth-Xin;
	
	// Then the travel distance equals:
	TravelDist = TMath::Abs(pzin/TMath::Cos(theta));
      }
      else if ((phi1<phi)&&(phi<phi3))
      {
	// This is the Ylow: so re-normalize momentum to be on this surface:
	pxin = pxin*(-1.0*Neu_HalfWidth-Yin)/pyin;
        pzin = pzin*(-1.0*Neu_HalfWidth-Yin)/pyin;
        pyin = -1.0*Neu_HalfWidth-Yin;
	
	// Then the travel distance equals:
	TravelDist = TMath::Abs(pzin/TMath::Cos(theta));
      }
      else if (((phi3<phi)&&(phi<(phi4+2.0*TMath::Pi())))&&(((phi3-2.0*TMath::Pi())<phi)&&(phi<phi4)))
      {
	// This is the Xother: so re-normalize momentum to be on this surface:
	pyin = pyin*(Neu_HalfWidth-Xin)/pxin;
        pzin = pzin*(Neu_HalfWidth-Xin)/pxin;
        pxin = Neu_HalfWidth-Xin;
	
	// Then the travel distance equals:
	TravelDist = TMath::Abs(pzin/TMath::Cos(theta));
      }
      else
      {
	if (Inputs->Bug_Report==1) {cout << "### BUG REPORT: We have a track that does not leave NeuLAND somewhere!\n";}
      }
    }
  }
  else
  {
    // it will leave NeuLAND at the front (particle gun side). So normalize the momentum now in
    // such a way that the z-coordinate of p exactly hits the front of neuland:
    pxin = pxin*(-1.0*Neu_HalfThick-Zin)/pzin;
    pyin = pyin*(-1.0*Neu_HalfThick-Zin)/pzin;
    pzin = -1.0*Neu_HalfThick-Zin;
    
    // Now we should determine if it will leave NeuLAND through the back or through
    // the side:
    if ((-1.0*Neu_HalfWidth<Xin+pxin)&&(Xin+pxin<Neu_HalfWidth)&&(-1.0*Neu_HalfWidth<Yin+pyin)&&(Yin+pyin<Neu_HalfWidth))
    {
      // Then we leave through the back:
      TravelDist = TMath::Abs(pzin/TMath::Cos(theta));
    }
    else
    {
      // Then we leave through the side. We have to know which side.
      // so compute phi:
      Double_t phi = GetPhi(pxin,pyin);
      
      // NOTE: We can now do exactly the same as with the back end of NeuLAND, since it is
      // again the same side.
      
      // Compute boundaries:
      Double_t phi1 = (-1.0*Neu_HalfWidth-Xin,-1.0*Neu_HalfWidth-Yin);
      Double_t phi2 = (-1.0*Neu_HalfWidth-Xin,1.0*Neu_HalfWidth-Yin);
      Double_t phi3 = (1.0*Neu_HalfWidth-Xin,-1.0*Neu_HalfWidth-Yin);
      Double_t phi4 = (1.0*Neu_HalfWidth-Xin,1.0*Neu_HalfWidth-Yin);
      
      // Then the Ylow-surface means that phi is between phi1 and phi3.
      // the Yup surafce means between phi2 and phi4. The Xtof is between phi1 and phi2.
      // the Xother is between phi3 and phi4. notice that we always have that
      // 0deg<phi4<phi2<phi1<phi3<360deg by definition of these angles. Hence:
      if ((phi4<phi)&&(phi<phi2))
      {
	// This is the Yup: so re-normalize momentum to be on this surface:
	pxin = pxin*(Neu_HalfWidth-Yin)/pyin;
        pzin = pzin*(Neu_HalfWidth-Yin)/pyin;
        pyin = Neu_HalfWidth-Yin;
	
	// Then the travel distance equals:
	TravelDist = TMath::Abs(pzin/TMath::Cos(theta));
      }
      else if ((phi2<phi)&&(phi<phi1))
      {
	// This is the Xtof: so re-normalize momentum to be on this surface:
	pyin = pyin*(-1.0*Neu_HalfWidth-Xin)/pxin;
        pzin = pzin*(-1.0*Neu_HalfWidth-Xin)/pxin;
        pxin = -1.0*Neu_HalfWidth-Xin;
	
	// Then the travel distance equals:
	TravelDist = TMath::Abs(pzin/TMath::Cos(theta));
      }
      else if ((phi1<phi)&&(phi<phi3))
      {
	// This is the Ylow: so re-normalize momentum to be on this surface:
	pxin = pxin*(-1.0*Neu_HalfWidth-Yin)/pyin;
        pzin = pzin*(-1.0*Neu_HalfWidth-Yin)/pyin;
        pyin = -1.0*Neu_HalfWidth-Yin;
	
	// Then the travel distance equals:
	TravelDist = TMath::Abs(pzin/TMath::Cos(theta));
      }
      else if (((phi3<phi)&&(phi<(phi4+2.0*TMath::Pi())))&&(((phi3-2.0*TMath::Pi())<phi)&&(phi<phi4)))
      {
	// This is the Xother: so re-normalize momentum to be on this surface:
	pyin = pyin*(Neu_HalfWidth-Xin)/pxin;
        pzin = pzin*(Neu_HalfWidth-Xin)/pxin;
        pxin = Neu_HalfWidth-Xin;
	
	// Then the travel distance equals:
	TravelDist = TMath::Abs(pzin/TMath::Cos(theta));
      }
      else
      {
	if (Inputs->Bug_Report==1) {cout << "### BUG REPORT: We have a track that does not leave NeuLAND somewhere!\n";}
      }
    }
  }
  }
  
  // and then, finally finally finally, we have our travel distance:
  return TravelDist;
}

Double_t GetMaxTravel(Inputs_Structure* const Inputs, Double_t const Energy, Int_t const PDG)
{
  // This function returns the maximum distance a particle with energy <<Energy>> can
  // travel througfh NeuLAND. The information is extracted from histograms
  // stored in Bethe.root. Obviously we also need the PDG identification for this. This distance
  // is then returned as output [cm].
  
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
  TRandom3* generator = new TRandom3();
  if (PDG==2112) {TravelDist = generator->Exp(Inputs->Z_mean_neutron);}
  if (PDG==22) {TravelDist = generator->Exp(Inputs->Z_mean_gamma);}
  delete generator; // NOTE: always delete every new!
  // So the travel distance through neuland is always the same, but sometimes
  // you are lucky that the random number is bigger and sometimes you are not:
  // this will attentuiate the bins in the same way!
  
  return TravelDist;
  // NOTE: if a particle is not in the list, its maximum travel distance will become zero!
}

Double_t GetMaxAirTravelDist(Inputs_Structure* Inputs, Double_t const Energy)
{
  // This function returns the maximum distance an ELECTRON with energy <<Energy>> can
  // travel through the air. The information is extracted from histograms
  // stored in Bethe.root This distance is then returned as output [cm].
  
  // ============================================================================================
  // We just do the same trick as for NeuLAND:
  Double_t Energy_conv = (Inputs->Nbins_Esec)*Energy;
  Int_t Energy_int = (Double_t) Energy_conv; 
  Energy_int = Energy_int+1;
  
  return Inputs->Electrons_TravelDist_Air->GetBinContent(Energy_int);
}