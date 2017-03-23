struct VETOhit
{
    Double_t Xhit;
    Double_t Yhit;
    Double_t Zhit;
    Double_t Xres;
    Double_t Yres;
    Double_t Zres;
    Double_t Transversal_Resolution;
};

VETOhit GetVETOhit(Inputs_Structure* const Inputs, Leaf_Structure* const TheLeafs, Int_t const veto_segPoint_Cindex)
{
  // This function computes the location of a hit in the VETO. It requires the Inputs and the Leafs from
  // the Monte Carlo results to accomplish this task. Then as a third input, it needs to know about which
  // Track we are talking. veto_segPoint_Cindex is the C++ index of the track in the veto_segPoint leafs
  // that we want to know the Hit information for. The hit information is returned in the form of a structure
  // as defined above. All units are in [cm]. Transversal_Resolution is the Pythagoras of Xres and Yres.
  // NOTE: The information is in a frame in which NeuLAND is at the origin and has standard orientation!
  // The idea is that we select a track on C++ index. Then along a bar, the position of the hit can be
  // determined precisely, so we keep the Geant4 coordinate. But in the other directions we give the 
  // middle of the bar and we give a non-zero resolution of half the bar dimensions. The Track that we will
  // select should be the one that has minimal TOF, so the minimum that we use for our TOF computation.
  // this is when TDC starts counting and hence this is also the position that we will obtain in reality from Tdiff.
  // Then in the NeuLAND frame, this hit information can then be used to build a VETO condition.
    
  // Written by C. A. Douma, for R3B collaboration.
    
  // ======================================================================================================
  // The first things that we need is the spatial point of the hit in the lab frame.
  // NOTE: We will use the LandPoint_fX Leafs for that. We cannot use MCTrack_fStartX since those
  // points are the starts of the tracks, not the interaction point. So we will obtain the
  // global coordinates with this and then transform them back to a standard orientation
  // of the VETO, just like with the PointInsideVETO-function.
    
  // Obtain global points and shift them to a VETO-centered frame:    
  Double_t Xin = TheLeafs->veto_segPoint_fX->GetValue(veto_segPoint_Cindex) - Inputs->VETO_geo_Xpos;
  Double_t Yin = TheLeafs->veto_segPoint_fY->GetValue(veto_segPoint_Cindex) - Inputs->VETO_geo_Ypos;
  Double_t Zin = TheLeafs->veto_segPoint_fZ->GetValue(veto_segPoint_Cindex) - Inputs->VETO_geo_Zpos;
  
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
  
  // Now we obtain some geometrical properties on the VETO that we might need:
  Double_t VETOhalfThickness = 0.5*Inputs->VETO_geo_TotalBarThickness*((Int_t) Inputs->VETO_geo_NsinglePlanes);
  Double_t VETOhalfheight = 0.5*Inputs->VETO_geo_TotalBarWidth*((Int_t) Inputs->VETO_geo_NbarsPerPlane);
  Double_t VETOhalfwidth = 0.5*Inputs->VETO_geo_TotalBarLength;
  
  // We already know in which bar the hit took place:
  Int_t BarNr = TheLeafs->veto_segPoint_fSector->GetValue(veto_segPoint_Cindex);
  
  // So then we also can extract in which plane the hit took place. We know the number of planes and 
  // the number of bars per plane. Both are stored in Inputs. Notice that fSector starts at 1:
  Int_t PlaneOfHit = 1;
  
  while (BarNr > Inputs->VETO_geo_NbarsPerPlane)
  {
      BarNr = BarNr - Inputs->VETO_geo_NbarsPerPlane;
      PlaneOfHit = PlaneOfHit + 1;
      
      // If BarNr is 1-50 and VETO_geo_NbarsPerPlane is also 50, then it is not bigger and the plane
      // of the hit will stay 1. If it is bigger, we subtract 50 (or whatever it may be) and check
      // again. Obviously this while-loop will always end. And in the end we will know 
      // in which plane the hit took place.
  }
  
  // So now comes the final part: if we are in a horizontal plane (planes 1, 3, etc.)
  // then Xin will be our Tdiff. This will give us our position along the bar. Y and Z are then
  // determined by spatial resolutions. In a vertical plane, it is the other way around.
  // Notice that due to the VETO-centered frame when the VETO is in standard orientation,
  // Xi will be Tdiff exactly!
  
  // Hence we proceed. Declare the hit and the resolution:
  Double_t Xhit = 0.0;
  Double_t Yhit = 0.0;
  Double_t Zhit = 0.0;
  Double_t Xres = 0.0;
  Double_t Yres = 0.0;
  Double_t Zres = 0.0;
  
  // Now compute it:
  if (PlaneOfHit%2==0)
  {
      // then we have an even plane, hence vertical. So it is Y that is precise:
      Yhit = Yin;
      Yres = 0.0;
      
      Zhit = -1.0*VETOhalfThickness + (((Int_t) PlaneOfHit) - 0.5)*(Inputs->VETO_geo_TotalBarThickness); // PlaneOfHit starts at 1.
      Zres = 0.5*Inputs->VETO_geo_TotalBarThickness;
      
      // labeling of the bars starts at the bottom and at the right (at Xtof, x<0):
      Xhit = -1.0*VETOhalfheight + (((Int_t) BarNr) - 0.5)*(Inputs->VETO_geo_TotalBarWidth); // same as for z. NOTE: BarNr is now reduced to the current plane!
      Xres = 0.5*Inputs->VETO_geo_TotalBarWidth;
  }
  else
  {
      // We are at a horizontal plane: then we have an odd plane. So it is X that is precise:
      Xhit = Xin;
      Xres = 0.0;
      
      Zhit = -1.0*VETOhalfThickness + (((Int_t) PlaneOfHit) - 0.5)*(Inputs->VETO_geo_TotalBarThickness); // PlaneOfHit starts at 1.
      Zres = 0.5*Inputs->VETO_geo_TotalBarThickness;
      
      // labeling of the bars starts at the bottom and at the left (at Xother, x>0):
      Yhit = -1.0*VETOhalfheight + (((Int_t) BarNr) - 0.5)*(Inputs->VETO_geo_TotalBarWidth); // same as for z. NOTE: BarNr is now reduced to the current plane!
      Yres = 0.5*Inputs->VETO_geo_TotalBarWidth;
  }
  
  // So now we have our hitpoint. We also know the resolutions. Now the real interest to us is 
  // not necessarily how this hitpoint and its resolutions look in the VETO-frame with
  // standard orientation for the VETO, but how it looks in the frame of standard
  // orientation for NeuLAND.
  
  // So we will now put our hit and our resolutions in a TVector3 and transform to the frame
  // where NeuLAND has standard orientation:
  
  TVector3 Hit;
  TVector3 Res;
  
  Hit.SetX(Xhit);
  Hit.SetY(Yhit);
  Hit.SetZ(Zhit);
  
  Res.SetX(Xres);
  Res.SetY(Yres);
  Res.SetZ(Zres);
  
  //cout << "Xhit = " << Hit.X() << " Yhit = " << Hit.Y() << " Zhit = " << Hit.Z() << " Zres = " << Res.Z() << " | ";
  
  Hit.RotateX(1.0*Inputs->VETO_geo_RotX);
  Hit.RotateY(1.0*Inputs->VETO_geo_RotY);
  Hit.RotateZ(1.0*Inputs->VETO_geo_RotZ);
  
  Res.RotateX(1.0*Inputs->VETO_geo_RotX);
  Res.RotateY(1.0*Inputs->VETO_geo_RotY);
  Res.RotateZ(1.0*Inputs->VETO_geo_RotZ);
  
  Hit.SetX(Hit.X() + Inputs->VETO_geo_Xpos);
  Hit.SetY(Hit.Y() + Inputs->VETO_geo_Ypos);
  Hit.SetZ(Hit.Z() + Inputs->VETO_geo_Zpos);
 
  // Do not shift the resolutions!
  
  // So now we are in the LAB-frame. Now we will go to the frame of NeuLAND:
  
  Hit.SetX(Hit.X() - Inputs->NeuLAND_geo_Xpos);
  Hit.SetY(Hit.Y() - Inputs->NeuLAND_geo_Ypos);
  Hit.SetZ(Hit.Z() - Inputs->NeuLAND_geo_Zpos);
  
  // Do not shift the resolutions!
  
  Hit.RotateZ(-1.0*Inputs->NeuLAND_geo_RotZ);
  Hit.RotateY(-1.0*Inputs->NeuLAND_geo_RotY);
  Hit.RotateX(-1.0*Inputs->NeuLAND_geo_RotX);
  
  Res.RotateZ(-1.0*Inputs->NeuLAND_geo_RotZ);
  Res.RotateY(-1.0*Inputs->NeuLAND_geo_RotY);
  Res.RotateX(-1.0*Inputs->NeuLAND_geo_RotX);
  
  // And now we have our hitpoint in coordinates with respect to NeuLAND
  // standard orientation. Now we will put these two TVector3 quantities
  // in a structure together so that we can return it:

  VETOhit TheHit;
  TheHit.Xhit = Hit.X();
  TheHit.Yhit = Hit.Y();
  TheHit.Zhit = Hit.Z();
  TheHit.Xres = Res.X();
  TheHit.Yres = Res.Y();
  TheHit.Zres = Res.Z();
  TheHit.Transversal_Resolution = TMath::Sqrt((Res.X())*(Res.X()) + (Res.Y())*(Res.Y()));
  
  // And then we are done:
  return TheHit;
  
}

VETOhit GetVETOhit_Digi(Inputs_Structure* const Inputs, Leaf_Structure* const TheLeafs, Int_t const Cindex)
{
    // Now we do the same stuff as before, but now with the digitizer. Notice that the positions of the hits
    // are already available in the LAB-frame! Hence this will now get really easy. We just need to know which
    // vaule to take (that is where the Cindex comes in).
    
  // First declare the TVector3 objects we need:
  TVector3 Hit;
  TVector3 Res;
  
  // Fill it with information from the digitizer (if the VETO digitizer exists):
  if ((TheLeafs->Geant3)&&((Inputs->Detectors_VETO_Exists==1)||(Inputs->Detectors_VETO_Exists==2)))
  {
    Hit.SetX(TheLeafs->veto_segDigi_fxx->GetValue(Cindex));
    Hit.SetY(TheLeafs->veto_segDigi_fyy->GetValue(Cindex));
    Hit.SetZ(TheLeafs->veto_segDigi_fzz->GetValue(Cindex));
  }
  else
  {
    Hit.SetX(0.0);
    Hit.SetY(0.0);
    Hit.SetZ(0.0);
  }    
    
  // For the resolution it is a bit more difficult, since we have to determine what kind of plane
  // we are in:  
  Int_t TheBar = 1 + TheLeafs->veto_segDigi_fPaddleNb->GetValue(Cindex);
  Int_t ThePlane = 1;
  while (TheBar>Inputs->VETO_geo_NbarsPerPlane)
  {
    TheBar = TheBar - Inputs->VETO_geo_NbarsPerPlane;
    ThePlane = ThePlane + 1;
  }
  
  // Now we can go:
  Double_t xres = 0.0;
  Double_t yres = 0.0;
  Double_t zres = 0.0;
  
  if (ThePlane%2==0)
  {
    // even, hence vertical plane:
    zres = 0.5*Inputs->VETO_geo_TotalBarThickness;
    xres = 0.5*Inputs->VETO_geo_TotalBarWidth;
    yres = (1e-12*Inputs->VETO_TimeResolution)*3e10; // NOTE: resolution is now in [cm].
  }
  else
  {
    // even, hence vertical plane:
    zres = 0.5*Inputs->VETO_geo_TotalBarThickness;
    yres = 0.5*Inputs->VETO_geo_TotalBarWidth;
    xres = (1e-12*Inputs->VETO_TimeResolution)*3e10; // NOTE: resolution is now in [cm].
  }
  
  // Put it into the vector:
  Res.SetX(xres);
  Res.SetY(yres);
  Res.SetZ(zres);
  
  // NOTE: Hit is in the LAB-frame, but Res is in the VETO-frame. Hence we first transform Res to
  // the LAB-frame. We do not need to shift resolutions, but we should rotate it!
  Res.RotateX(1.0*Inputs->VETO_geo_RotX);
  Res.RotateY(1.0*Inputs->VETO_geo_RotY);
  Res.RotateZ(1.0*Inputs->VETO_geo_RotZ);
  
  // NOTE: We are in the LAB-frame with both now. Now transform to the NeuLAND frame:
  Hit.SetX(Hit.X() - Inputs->NeuLAND_geo_Xpos);
  Hit.SetY(Hit.Y() - Inputs->NeuLAND_geo_Ypos);
  Hit.SetZ(Hit.Z() - Inputs->NeuLAND_geo_Zpos);
  
  // Do not shift the resolutions!
  
  Hit.RotateZ(-1.0*Inputs->NeuLAND_geo_RotZ);
  Hit.RotateY(-1.0*Inputs->NeuLAND_geo_RotY);
  Hit.RotateX(-1.0*Inputs->NeuLAND_geo_RotX);
  
  Res.RotateZ(-1.0*Inputs->NeuLAND_geo_RotZ);
  Res.RotateY(-1.0*Inputs->NeuLAND_geo_RotY);
  Res.RotateX(-1.0*Inputs->NeuLAND_geo_RotX);
  
  // And now we have our hitpoint in coordinates with respect to NeuLAND
  // standard orientation. Now we will put these two TVector3 quantities
  // in a structure together so that we can return it:

  VETOhit TheHit;
  TheHit.Xhit = Hit.X();
  TheHit.Yhit = Hit.Y();
  TheHit.Zhit = Hit.Z();
  TheHit.Xres = Res.X();
  TheHit.Yres = Res.Y();
  TheHit.Zres = Res.Z();
  TheHit.Transversal_Resolution = TMath::Sqrt((Res.X())*(Res.X()) + (Res.Y())*(Res.Y()));
  
  // And then we are done:
  return TheHit;
}