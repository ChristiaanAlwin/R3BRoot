void Segmented_VETO_Condition(Leaf_Structure* const TheLeafs, Inputs_Structure* const Inputs, Tree_Structure* AnaTree, Int_t const Event, Int_t const Counter, TRandom3* generator)
{
  // This function performs the actual VETO condition. There are multiple versions of this function
  // available. This one uses the spatial sructure of the VETO to specifically VETO yes/no
  // certain bars of NeuLAND. Based on this, we then determine which primary particles are 
  // VETOed yes/no. The leafs that should be filled by the VETO
  // condition are:
  // AnaTree->Prim_E_Detected
  // AnaTree->Prim_IsVETOed
  // AnaTree->Neu_Bar_IsVETOed
  // However, these leafs are already created in previous analysis tasks. They are also re-declared, connected,
  // deleted, etc there. (The first two in Identify_PrimParticles.h and the third in QTDC.h
  // Hence we do not need to worry about anything here, we just need to fill them.
  
  // This filling is then put into the analysis tree. Therefore no outputs are required. The inputs are the pointers
  // to the 3 structures that hold all our information and the number of the current event.
  // We also pass the TRandom3 generator along as non-=const so that seeds are updated every time. Do not delete it!
  
  // NOTE: AnaTree should NOT be const, otherwise we cannot modify the structure.
  
  // Template is Written by C. A. Douma, for the R3B collaboration
    
  // =============================================================================================
  // NOTE: Nothing needs to be re-declared, deleted, etc. This is done in other functions!
  // =============================================================================================
  // Now here we go. We begin with declaring a full array that holds all barsof NeuLAND:
  // whether they are VETOed or not:
  Int_t NbarsNeu = (Inputs->NeuLAND_geo_NsinglePlanes)*50;
  Bool_t* VETObars = new Bool_t[NbarsNeu];
  for (Int_t k = 0; k<NbarsNeu; ++k)
  {
      VETObars[k] = kFALSE;
  }
  
  // Now in order to perform the VETO condition, we begin with looping over the bars of the VETO.
  // Notice that we only take fired bars of the VETO:
  Double_t NeuhalfThickness = 2.5*((Int_t) Inputs->NeuLAND_geo_NsinglePlanes);
  Int_t VETO_NbarsFired = AnaTree->NbarsFired_VETO;
  Int_t ThisIndex = -1;
  Int_t Sector = -1;
  Double_t Xbar = 0.0;
  Double_t Ybar = 0.0;
  Double_t Zbar = 0.0;
  Double_t Xswitch = 0.0;
  Double_t Yswitch = 0.0;
  Double_t Zswitch = 0.0;
  Double_t Showerwidth_X = Inputs->Shower_Radius/TMath::Cos(TMath::ATan(Inputs->VETO_Xdir));
  Double_t Showerwidth_Y = Inputs->Shower_Radius/TMath::Cos(TMath::ATan(Inputs->VETO_Ydir));
  VETOhit ThisHit;// declaration of the hit-structure.
  
  if (VETO_NbarsFired>0)
  {
  for (Int_t kveto = 0; kveto<VETO_NbarsFired; ++kveto)
  {
      // For each bar we already have the C++ index in the veto_segPoint-leafs that belong to this
      // hit (min. TOF point). Hence with our Tdiff.h we can directly obtain the position of the 
      // hit in the NeuLAND-frame:
      ThisIndex = AnaTree->VETO_TOF_index[kveto];
      
      if (Inputs->UseDigitizer>0)
      {
        ThisHit = GetVETOhit_Digi(Inputs,TheLeafs,ThisIndex);
      }
      else
      {
        ThisHit = GetVETOhit(Inputs,TheLeafs,ThisIndex);
      }
      
      // So now we have the hit, we can loop over the bars of NeuLAND. we will in this
      // case first loop over the number of single planes and then over the bars in them:
      for (Int_t ThePlane = 0; ThePlane<(Inputs->NeuLAND_geo_NsinglePlanes); ++ThePlane)
      {
          for (Int_t TheBar = 0; TheBar<50; ++TheBar)
          {
              // First define the overall index:
              Sector = TheBar + 50*ThePlane; // NOTE: we ignore NEBULA!
              
              // Now we must know whether we are in a horizontal plane yes/no:
              if (ThePlane%2==0)
              {
                  // Then we are in a horizontal plane. Then the y-coordinate of the middle of the bar
                  // is given by:
                  Ybar = -125.0 + (((Int_t) TheBar) + 0.5)*5.0;
                  Zbar = -1.0*NeuhalfThickness + (((Int_t) ThePlane) + 0.5)*5.0;
                  
                  // Then VETO if this Ybar is between Yhit+/-Showerwidth. However, we will first
                  // switch Ybar over the appropriate value depending on the VETO directional vector.
                  
                  Zswitch = Zbar - ThisHit.Zhit;
                  Yswitch = (Inputs->VETO_Ydir)*Zswitch;
                  Ybar = Ybar - Yswitch;
                  
                  if (((ThisHit.Yhit-Showerwidth_Y)<Ybar)&&((ThisHit.Yhit+Showerwidth_Y)>Ybar))
                  {
                      // But we now only really VETO this bar if the TOF of the VETO-bar
                      // is smaller then the overall TOF of NeuLAND:
                      if (AnaTree->VETO_TOF_Fired_Res[kveto]<AnaTree->Neu_TotalTOF_Res)
                      {
                           VETObars[Sector] = kTRUE;
                      }
                  }
              }
              else
              {
                  // Then we are in a vertial plane. Then the x-coordinate of the middle of the bar
                  // is given by:
                  Xbar = -125.0 + (((Int_t) TheBar) + 0.5)*5.0;
                  Zbar = -1.0*NeuhalfThickness + (((Int_t) ThePlane) + 0.5)*5.0;
                  
                  // Then VETO if this Xbar is between Xhit+/-Showerwidth. However, we will first
                  // switch Xbar over the appropriate value depending on the VETO directional vector.
                  Zswitch = Zbar - ThisHit.Zhit;
                  Xswitch = (Inputs->VETO_Xdir)*Zswitch;
                  Xbar = Xbar - Xswitch;

                  if (((ThisHit.Xhit-Showerwidth_X)<Xbar)&&((ThisHit.Xhit+Showerwidth_X)>Xbar))
                  {
                      // But we now only really VETO this bar if the TOF of the VETO-bar
                      // is smaller then the overall TOF of NeuLAND:
                      if (AnaTree->VETO_TOF_Fired_Res[kveto]<AnaTree->Neu_TotalTOF_Res)
                      {
                          VETObars[Sector] = kTRUE;
                      }
                  }
              }
              
              // NOTE: The VETO hit is already in the frame of NeuLAND, so we do not need to
              // worry about difficult transformations etc. Then the idea is that we put a cyllinder
              // around the hitpoint in the VETO. the axis is parallel to the z-axis in the frame where
              // NeuLAND has standard orientation. The radius is an analysis parameter: the Showerwidth.
              // Then if a bar crosses his cyllinder, we VETO it. This we do for all hits in the VETO.
              
              // That's all.
          }
      }
  }
  }
  
  // So now essentially we have all bars VETO yes/no. Now we must put them in the leafs. But only
  // fired bars are in the leafs. hence we must sort this out.
  
  // But first store everything if this is required:
  if (Inputs->Store_AllBars==1)
  {
      for (Int_t k = 0; k<NbarsNeu; ++k)
      {
          AnaTree->AllBars_IsVETOed[k] = VETObars[k];
      }
  }
  
  // Declare what we need:
  Int_t Neu_NbarsFired = AnaTree->NbarsFired_NeuLAND;
  Int_t BarNr = 0;
  
  // Loop over all fired bars:
  for (Int_t k = 0; k<Neu_NbarsFired; ++k)
  {
      // Get the real bar-number:
      BarNr = AnaTree->Neu_BarNumber_Fired[k]; // NOTE: not a C-index, it starts at 1!
      
      // Then put the VETO condition into the leafs:
      AnaTree->Neu_Bar_IsVETOed[k] = VETObars[BarNr-1];
  }
  
  // So now the bars that we VETOed are in the right leaf.
  // Now that we are done: delete the array:
  delete VETObars;
  
  // =============================================================================================
  // Now the next task is to compute the total energy deposition per identified primary particle.
  // Notice that only unVETOed bars should be taken along.
  
  // Use our special function for that:
  GetTotalEdep(TheLeafs,Inputs,AnaTree,Event,Counter,generator);
  // Notice that here we overwrite the tracker reconstructed energy on purpose, since this VETO
  // condition does not work without this energy!

  // =============================================================================================
  // Now after we did this, we can fill in the final leaf we are interested in: Whether or not
  // a primary particle is VETOed. Hopefully the VETO condition will cut out a lot of bars
  // that belong to this particle. hence the reconstructed (Detected) energy is low. Then we
  // will VETO the particle if the energy is below a certain threshold. Notice however that this
  // is only necessary if the VETO actually fired. if it did not fire (or only fired after NeuLAND),
  // there is no need to try to separate something. We simply should keep everything.
  
  // First see if there is a fired bar in the VETO before NeuLAND:
  Bool_t VETOfired = kFALSE;
  
  if (VETO_NbarsFired>0)
  {
      for (Int_t kveto = 0; kveto<VETO_NbarsFired; ++kveto)
      {
          if (AnaTree->VETO_TOF_Fired_Res[kveto]<AnaTree->Neu_TotalTOF_Res)
          {
              VETOfired = kTRUE;
          }
      }
  }
  
  Double_t Ecut = Inputs->VETO_Ecut;
  
  // loop over the primary particles:
  Int_t Nprims = AnaTree->Nprims_NeuLAND;
  
  for (Int_t k = 0; k<Nprims; ++k)
  {
      if (((AnaTree->Prim_E_Detected[k])<Ecut)&&(VETOfired))
      {
          AnaTree->Prim_IsVETOed[k] = kTRUE;
      }
      else
      {
          AnaTree->Prim_IsVETOed[k] = kFALSE;
      }
  }
          
  // And that's it. Now we have filled in all 3 leafs that we had to fill in.        
    
}