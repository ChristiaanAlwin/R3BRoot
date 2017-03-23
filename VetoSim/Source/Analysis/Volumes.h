// This header contains several small functions that are used to compute whether a point 
// or a track is inside a specific volume or not.

// Written by C. A. Douma, for the R3B collaboration.

// inclusion of required headers:
#include "TVector3.h"
#include "TMath.h"

Double_t GetTheta(Double_t const px, Double_t const py, Double_t const pz)
{
  // Computes theta-ange of emittance:
  Double_t theta = pz/TMath::Sqrt(px*px + py*py + pz*pz); // this is the cosine!
  theta = TMath::ACos(theta);
  return theta;
}

Double_t GetPhi(Double_t const px, Double_t const py)
{
  // Computes phi-angle of emittance:
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

Bool_t PointInsidePrimVolume(Inputs_Structure* const Inputs, Double_t const X, Double_t const Y, Double_t const Z)
{
  // This function tests if a point with coordinates X, Y, Z is inside the volume
  // that is used to identify primary particles. The volume has a block-shape and
  // is defined in the Inputs with the analysis parameters. Therefore the Inputs
  // structure is also required as an input. The output is a boolian that is kTRUE
  // if the point is inside the surface and kFALSE if not.
  
  // ===============================================================================
  // First define the boolians we need:
  Bool_t Xinside = kFALSE;
  Bool_t Yinside = kFALSE;
  Bool_t Zinside = kFALSE;
  
  // Then test coordinate-by-coordinate if the point is inside the volume:
  if (((Inputs->PrimIDplane_Xtof)<X)&&((Inputs->PrimIDplane_Xother)>X)) {Xinside = kTRUE;} // the negative X-axis points to the TOF
  if (((Inputs->PrimIDplane_Ylow)<Y)&&((Inputs->PrimIDplane_Yup)>Y)) {Yinside = kTRUE;} // the positive Y-axis points upwards
  if (((Inputs->PrimIDplane_Zfront)<Z)&&((Inputs->PrimIDplane_Zback)>Z)) {Zinside = kTRUE;} // the positive Z-axis points along the beam.
  
  // Now merge them together:
  Bool_t TheTest = Xinside&&Yinside&&Zinside;
  
  // Now return this boolian and we are done:
  return TheTest;
}

Bool_t FaceTest(Double_t const Zpos, Double_t const Xlow, Double_t const Xup, Double_t const Ylow, Double_t const Yup, TVector3 const TrackStart, TVector3 const TrackTravel)
{
  // This function tests if a track starting at TrackStart and travelling over a vector TrackTravel will hit a square
  // surface normal to the z-axis and ranging from Xlow to Xup and Ylow to Yup. The track must go into the forward direction
  // and be generated in front of the surface.
  
  // ========================================================================================================================
  // First define the boolian for the test:
  Bool_t TheTest = kFALSE;
  
  // Then extract the components of the vectors:
  Double_t Xstart = TrackStart.X();
  Double_t Ystart = TrackStart.Y();
  Double_t Zstart = TrackStart.Z();
  
  Double_t Xrealtravel = TrackTravel.X();
  Double_t Yrealtravel = TrackTravel.Y();
  Double_t Zrealtravel = TrackTravel.Z();
  
  // Then test if the track is generated in front of the surface:
  if (Zstart<Zpos)
  {
    // then now we can proceed. Get Theta and phi:
    Double_t theta = GetTheta(Xrealtravel,Yrealtravel,Zrealtravel);
    Double_t phi = GetPhi(Xrealtravel,Yrealtravel);
    
    // Then just as with the position of Z, there is no point to keep going
    // if theta is not between 0 and pi/2:
    if (theta<0.5*TMath::Pi())
    {
      // Then now we can compute how far we should travel to the surface:
      Double_t Ztravel = Zpos - Zstart; // hence positive for sure:
      Double_t Xtravel = Ztravel*TMath::Tan(theta)*TMath::Cos(phi);
      Double_t Ytravel = Ztravel*TMath::Tan(theta)*TMath::Sin(phi);
      
      // Then now we can test if it goes through the surface:
      Bool_t Aimed = (((Xtravel+Xstart)<Xup)&&((Xtravel+Xstart)>Xlow)&&((Ytravel+Ystart)<Yup)&&((Ytravel+Ystart)>Ylow));
      
      // But it only reaches the surface if:
      Bool_t reach = (Ztravel<Zrealtravel);
      
      // So then the test will become:
      TheTest = Aimed&&reach;
    }
    // Else we do nothing again.
    
  }
  // If this is not true, then the track will not pass the face. Hence the boolian is defined
  // false and remains false. Hence simply return it:
  return TheTest;
}

Bool_t TrackThroughPrimVolume(Inputs_Structure* const Inputs, Double_t const Xparent, Double_t const Yparent, Double_t const Zparent, Double_t const Xdaughter, Double_t const Ydaughter, Double_t const Zdaughter)
{
  // This function tests if a track with parent coordinates X, Y, Z and a daughter
  // with coordinates X, Y, Z goes through the volume    
  // that is used to identify primary particles. The volume has a block-shape and
  // is defined in the Inputs with the analysis parameters. Therefore the Inputs
  // structure is also required as an input. The output is a boolian that is kTRUE
  // if the track goes through the surface and kFALSE if not.
  
  // ===============================================================================
  // We begin by defining the boolian for the test:
  Bool_t TheTest = kFALSE;
  
  // Now we will test the following: if a track is generated in front of 
  // the primary volume, is it aimed onto the front face of the volume
  // yes/no. If so, is the track also long enough to actually cover this distance?
  
  // So compute some quantities:
  Double_t Xrealtravel = Xdaughter - Xparent;
  Double_t Yrealtravel = Ydaughter - Yparent;
  Double_t Zrealtravel = Zdaughter - Zparent;
  
  Double_t theta = GetTheta(Xrealtravel,Yrealtravel,Zrealtravel);
  Double_t phi = GetPhi(Xrealtravel,Yrealtravel);
  
  // We can then make this test for all 6 faces. Declare what we need:
  Bool_t Zfront = kFALSE;
  Bool_t Zback = kFALSE;
  Bool_t Yup = kFALSE;
  Bool_t Ylow = kFALSE;
  Bool_t Xother = kFALSE;
  Bool_t Xtof = kFALSE;
  Double_t Xtravel = 0.0;
  Double_t Ytravel = 0.0;
  Double_t Ztravel = 0.0;
  Bool_t Aimed = kFALSE;
  Bool_t reach = kFALSE;
  
  // The first one is easy: Zfront.
  if ((Zparent<Inputs->PrimIDplane_Zfront)&&(theta<0.5*TMath::Pi()))
  {
    // compute the travel distances:
    Ztravel = Inputs->PrimIDplane_Zfront - Zparent;
    Xtravel = Ztravel*TMath::Tan(theta)*TMath::Cos(phi);
    Ytravel = Ztravel*TMath::Tan(theta)*TMath::Sin(phi);
    
    // Make the test:
    Aimed = (((Xtravel+Xparent)<Inputs->PrimIDplane_Xother)&&((Xtravel+Xparent)>Inputs->PrimIDplane_Xtof)&&((Ytravel+Yparent)<Inputs->PrimIDplane_Yup)&&((Ytravel+Yparent)>Inputs->PrimIDplane_Ylow));
    reach = (Ztravel<TMath::Abs(Zrealtravel));
    Zfront = Aimed&&reach;
  }
  
  // Zback is also doable:
  if ((Zparent>Inputs->PrimIDplane_Zback)&&(theta>0.5*TMath::Pi()))
  {
    // compute the travel distances:
    Ztravel = Zparent - Inputs->PrimIDplane_Zfront;
    Xtravel = Ztravel*TMath::Tan(TMath::Pi() - theta)*TMath::Cos(phi);
    Ytravel = Ztravel*TMath::Tan(TMath::Pi() - theta)*TMath::Sin(phi);
    
    // Make the test:
    Aimed = (((Xtravel+Xparent)<Inputs->PrimIDplane_Xother)&&((Xtravel+Xparent)>Inputs->PrimIDplane_Xtof)&&((Ytravel+Yparent)<Inputs->PrimIDplane_Yup)&&((Ytravel+Yparent)>Inputs->PrimIDplane_Ylow));
    reach = (Ztravel<TMath::Abs(Zrealtravel));
    Zback = Aimed&&reach;
  }
  
  // Then now we will do Yup:
  if ((Yparent>Inputs->PrimIDplane_Yup)&&(phi>TMath::Pi()))
  {
    // compute the travel distances:
    Ytravel = Yparent - Inputs->PrimIDplane_Yup;
    Ztravel = Zrealtravel*(Ytravel/TMath::Abs(Yrealtravel));
    Xtravel = Xrealtravel*(Ytravel/TMath::Abs(Yrealtravel));
    
    // Make the test:
    Aimed = (((Xtravel+Xparent)<Inputs->PrimIDplane_Xother)&&((Xtravel+Xparent)>Inputs->PrimIDplane_Xtof)&&((Ztravel+Zparent)<Inputs->PrimIDplane_Zback)&&((Ztravel+Zparent)>Inputs->PrimIDplane_Zfront));
    reach = (Ytravel<TMath::Abs(Yrealtravel));
    Yup = Aimed&&reach;
  }
  
  // Then now we will do Ylow:
  if ((Yparent<Inputs->PrimIDplane_Ylow)&&(phi<TMath::Pi()))
  {
    // compute the travel distances:
    Ytravel = Inputs->PrimIDplane_Ylow - Yparent;
    Ztravel = Zrealtravel*(Ytravel/TMath::Abs(Yrealtravel));
    Xtravel = Xrealtravel*(Ytravel/TMath::Abs(Yrealtravel));
    
    // Make the test:
    Aimed = (((Xtravel+Xparent)<Inputs->PrimIDplane_Xother)&&((Xtravel+Xparent)>Inputs->PrimIDplane_Xtof)&&((Ztravel+Zparent)<Inputs->PrimIDplane_Zback)&&((Ztravel+Zparent)>Inputs->PrimIDplane_Zfront));
    reach = (Ytravel<TMath::Abs(Yrealtravel));
    Yup = Aimed&&reach;
  }
  
  // Then now we will do Xother:
  if ((Xparent>Inputs->PrimIDplane_Xother)&&(phi>0.5*TMath::Pi())&&(phi<1.5*TMath::Pi()))
  {
    // compute the travel distances:
    Xtravel = Xparent - Inputs->PrimIDplane_Xother;
    Ztravel = Zrealtravel*(Xtravel/TMath::Abs(Xrealtravel));
    Ytravel = Yrealtravel*(Xtravel/TMath::Abs(Xrealtravel));
    
    // Make the test:
    Aimed = (((Ytravel+Yparent)<Inputs->PrimIDplane_Yup)&&((Ytravel+Yparent)>Inputs->PrimIDplane_Ylow)&&((Ztravel+Zparent)<Inputs->PrimIDplane_Zback)&&((Ztravel+Zparent)>Inputs->PrimIDplane_Zfront));
    reach = (Xtravel<TMath::Abs(Xrealtravel));
    Xother = Aimed&&reach;
  }
  
  // Then now we will do Xtof:
  if ((Xparent<Inputs->PrimIDplane_Xtof)&&((phi<0.5*TMath::Pi())||(phi>1.5*TMath::Pi())))
  {
    // compute the travel distances:
    Xtravel = Inputs->PrimIDplane_Xother - Xparent;
    Ztravel = Zrealtravel*(Xtravel/TMath::Abs(Xrealtravel));
    Ytravel = Yrealtravel*(Xtravel/TMath::Abs(Xrealtravel));
    
    // Make the test:
    Aimed = (((Ytravel+Yparent)<Inputs->PrimIDplane_Yup)&&((Ytravel+Yparent)>Inputs->PrimIDplane_Ylow)&&((Ztravel+Zparent)<Inputs->PrimIDplane_Zback)&&((Ztravel+Zparent)>Inputs->PrimIDplane_Zfront));
    reach = (Xtravel<TMath::Abs(Xrealtravel));
    Xtof = Aimed&&reach;
  }
  
  // And then finally, we can combine:
  TheTest = (Zfront||Zback||Xtof||Xother||Ylow||Yup);
  
  // Done! Return our test:
  return TheTest;
  
}

Bool_t PointInsideNeuLAND(Inputs_Structure* const Inputs, Double_t const X, Double_t const Y, Double_t const Z)
{
  // This function tests if a point with coordinates X, Y, Z is inside NeuLAND.
  // Hence the Inputs structure is also required as an input. The output is a boolian
  // that is kTRUE if the point is inside the surface and kFALSE if not.
  
  // ===============================================================================
  // This is a little trickier then the volume for primary particles, since
  // NeuLAND can be rotated. In order to solve this, we will first 
  // rotate the points X, Y, Z in precisely the other direction and then
  // pretend like NeuLAND was not rotated.
  
  // before we do this, we will first however shift X, Y, and Z so that the
  // center of NeuLAND is located at the origin:
  Double_t Xin = X - Inputs->NeuLAND_geo_Xpos;
  Double_t Yin = Y - Inputs->NeuLAND_geo_Ypos;
  Double_t Zin = Z - Inputs->NeuLAND_geo_Zpos;
  
  // Now we put these new values in a TVector3:
  TVector3 vec;
  vec.SetX(Xin);
  vec.SetY(Yin);
  vec.SetZ(Zin);
  
  // Now we will rotate it reverse w.r.t. the NeuLAND geometry source code:
  vec.RotateZ(-1.0*Inputs->NeuLAND_geo_RotZ);
  vec.RotateY(-1.0*Inputs->NeuLAND_geo_RotY);
  vec.RotateX(-1.0*Inputs->NeuLAND_geo_RotX);
  
  // And now we will obtain the correct X, Y and Z back:
  Xin = vec.X();
  Yin = vec.Y();
  Zin = vec.Z();
 
  // Then we define the boolian for the test:
  Bool_t TheTest = kFALSE;
  
  // And we get the half thickness of the entire NeuLAND:
  Double_t NeuhalfThickness = 2.5*((Int_t) Inputs->NeuLAND_geo_NsinglePlanes);
  
  // get half height of a horizontal plane:
  Double_t Neuhalfheight = 125.0;
  
  // and get half the width:
  Double_t Neuhalfwidth = 125.0;
  
  // So now we can perform the test:
  Bool_t Xinside = kFALSE;
  Bool_t Yinside = kFALSE;
  Bool_t Zinside = kFALSE;
    
  if (((-1.0*Neuhalfwidth)<Xin)&&(Neuhalfwidth>Xin)) {Xinside = kTRUE;}
  if (((-1.0*Neuhalfheight)<Yin)&&(Neuhalfheight>Yin)) {Yinside = kTRUE;}
  if (((-1.0*NeuhalfThickness)<Zin)&&(NeuhalfThickness>Zin)) {Zinside = kTRUE;}
    
  TheTest = Xinside&&Yinside&&Zinside;
    
  // Done, now TheTest holds the correct value:
  return TheTest;
    
}

Bool_t PointInsideVETO(Inputs_Structure* const Inputs, Double_t const X, Double_t const Y, Double_t const Z)
{
  // This function tests if a point with coordinates X, Y, Z is inside the VETO.
  // Hence the Inputs structure is also required as an input. The output is a boolian
  // that is kTRUE if the point is inside the surface and kFALSE if not.
  
  // ===============================================================================
  // This is even trickier then for NeuLAND, since not only
  // the VETO can be rotated, but we also have to take into account
  // that the VETO might not be square. Then we have to know in which
  // plane we are. In order to solve this, we will first 
  // rotate the points X, Y, Z in precisely the other direction and then
  // pretend like the VETO was not rotated.
  
  // before we do this, we will first however shift X, Y, and Z so that the
  // center of the VETO is located at the origin:
  Double_t Xin = X - Inputs->VETO_geo_Xpos;
  Double_t Yin = Y - Inputs->VETO_geo_Ypos;
  Double_t Zin = Z - Inputs->VETO_geo_Zpos;
  
  // Now we put these new values in a TVector3:
  TVector3 vec;
  vec.SetX(Xin);
  vec.SetY(Yin);
  vec.SetZ(Zin);
  
  // Now we will rotate it reverse w.r.t. the VETO geometry source code:
  vec.RotateZ(-1.0*Inputs->VETO_geo_RotZ);
  vec.RotateY(-1.0*Inputs->VETO_geo_RotY);
  vec.RotateX(-1.0*Inputs->VETO_geo_RotX);
  
  // And now we will obtain the correct X, Y and Z back:
  Xin = vec.X();
  Yin = vec.Y();
  Zin = vec.Z();
  
  // now the first question is to determine in which plane the
  // point is located. And more specifically: is it in an odd
  // plane or an even plane. This is because odd planes are horizontal:
  // the bar length is in the X-direction and the bar width in the Y.
  // for even planes this is reversed.
  
  // Before we bother with this however, we define the boolian for the test:
  Bool_t TheTest = kFALSE;
  
  // And we get the half thickness of the entire VETO:
  Double_t VETOhalfThickness = 0.5*Inputs->VETO_geo_TotalBarThickness*((Int_t) Inputs->VETO_geo_NsinglePlanes);
  
  // get half height of a horizontal plane:
  Double_t VETOhalfheight = 0.5*Inputs->VETO_geo_TotalBarWidth*((Int_t) Inputs->VETO_geo_NbarsPerPlane);
  
  // and get half the width:
  Double_t VETOhalfwidth = 0.5*Inputs->VETO_geo_TotalBarLength;
  
  // then for a horizontal plane, X should be between +/- VETOhalfwidth and Y should be
  // between +/- VETOhalfheight. For a vertical plane it is the other way around.
  
  // Now before bothering with planes, test if Z falls in the range:
  if (((-1.0*VETOhalfThickness)<Zin)&&(VETOhalfThickness>Zin))
  {
    // now that we know this, we can bother with the horizontal/vertical question:
    
    // So we define a boolian that is kTRUE if the point is in a
    // horizontal plane and kFALSE if it is in a vertical plane:
    Bool_t plane = kTRUE;
    
    // Get the difference between the front side of the VETO and the point in question:
    Double_t Zdiff = Zin + VETOhalfThickness; // NOTE: this should be positive:
    if (Zdiff<0) {cout << "### BUG REPORT: Zdiff is negative!\n";}
    
    // Now see how many layers are between the front and the point:
    Double_t nd = Zdiff/(Inputs->VETO_geo_TotalBarThickness);
    Int_t n = (Double_t) nd;
    
    // This cuts off anything above the integer. Hence if n is even (or zero),
    // we are in a horizontal plane, else not:
    if (n%2 == 0)
    {
      plane = kTRUE;
    }
    else
    {
      plane = kFALSE;
    }
    
    // Now that we know what kind of a plane we are talking about, we can do the rest of our test:
    Bool_t Xinside = kFALSE;
    Bool_t Yinside = kFALSE;
    
    // horizontal planes:
    if (plane)
    {
      if (((-1.0*VETOhalfwidth)<Xin)&&(VETOhalfwidth>Xin)) {Xinside = kTRUE;}
      if (((-1.0*VETOhalfheight)<Yin)&&(VETOhalfheight>Yin)) {Yinside = kTRUE;}
    }
    else // vertical planes:
    {
      if (((-1.0*VETOhalfwidth)<Yin)&&(VETOhalfwidth>Yin)) {Yinside = kTRUE;}
      if (((-1.0*VETOhalfheight)<Xin)&&(VETOhalfheight>Xin)) {Xinside = kTRUE;}
    }
    
    // now inside this if-statement we already know that Z is inside the VETO,
    // so we can just define:
    TheTest = Xinside&&Yinside;
    
    // Done, now TheTest holds the correct value.
    
  }
  else
  {
    // Then the point is not in the VETO anyway, so:
    TheTest = kFALSE;
  }
  
  // And now we are done. So return the test:
  return TheTest;  
}

Bool_t PointInFrontVETO(Inputs_Structure* const Inputs, Double_t const X, Double_t const Y, Double_t const Z)
{
  // This function tests if a point with coordinates X, Y, Z is in front of the VETO.
  // Hence the Inputs structure is also required as an input. The output is a boolian
  // that is kTRUE if the point is in front of the surface and kFALSE if not.
  
  // ===============================================================================
  // We follow the same approach as to check for a point if it is inside of
  // the VETO. But now we do not have to trouble ourselves with layers...
    
  // So here we go!
  
  // before we do this, we will first however shift X, Y, and Z so that the
  // center of the VETO is located at the origin:
  Double_t Xin = X - Inputs->VETO_geo_Xpos;
  Double_t Yin = Y - Inputs->VETO_geo_Ypos;
  Double_t Zin = Z - Inputs->VETO_geo_Zpos;
  
  // Now we put these new values in a TVector3:
  TVector3 vec;
  vec.SetX(Xin);
  vec.SetY(Yin);
  vec.SetZ(Zin);
  
  // Now we will rotate it reverse w.r.t. the VETO geometry source code:
  vec.RotateZ(-1.0*Inputs->VETO_geo_RotZ);
  vec.RotateY(-1.0*Inputs->VETO_geo_RotY);
  vec.RotateX(-1.0*Inputs->VETO_geo_RotX);
  
  // And now we will obtain the correct X, Y and Z back:
  Xin = vec.X();
  Yin = vec.Y();
  Zin = vec.Z();
  
  // now define a boolian for the test:
  Bool_t TheTest = kFALSE;
  
  // And we get the half thickness of the entire VETO:
  Double_t VETOhalfThickness = 0.5*Inputs->VETO_geo_TotalBarThickness*((Int_t) Inputs->VETO_geo_NsinglePlanes);
  
  // So now it is easy. If Zin < -VETOhalfThickness, then we are in front
  // of the VETO and otherwise we are not:
  if (Zin<(-1.0*VETOhalfThickness)) {TheTest = kTRUE;}
  
  // now return the anser:
  return TheTest;
}