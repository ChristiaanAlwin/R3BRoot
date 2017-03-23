void Tracker_VETO_Condition(Leaf_Structure* const TheLeafs, Inputs_Structure* const Inputs, Tree_Structure* AnaTree, Int_t const Event, Int_t const Counter, TRandom3* generator)
{
  // This function performs the actual VETO condition. There are multiple versions of this function
  // available. This one uses the results from the Neutron tracker. The leafs that should be filled by the VETO
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
  
  // Written by C. A. Douma, for the R3B collaboration
    
  // FIXME: Implement comparison between VETO hits and NeutHits. Implement Prim_E_Detected!
    
  // =============================================================================================
  // NOTE: Nothing needs to be re-declared, deleted, etc. This is done in other functions!
  // =============================================================================================
  // The purpose of this function is to VETO the primary particles based on the R3BNeutronTracker
  // Reconstructed primary interaction points. Note however that if the Tracker does not exist,
  // there is no point in running this function. 
    
  // First we reset everything so that nothing is VETOed:
  Int_t NbarsFired = AnaTree->NbarsFired_NeuLAND;
  for (Int_t k = 0; k<NbarsFired; ++k)
  {
    AnaTree->Neu_Bar_IsVETOed[k] = kFALSE;
  }
  
  if (Inputs->Store_AllBars==1)
  {
     Int_t NbarsNeu = (Inputs->NeuLAND_geo_NsinglePlanes)*50;
     for (Int_t k = 0; k<NbarsNeu; ++k)
     {
        AnaTree->AllBars_IsVETOed[k] = kFALSE;
     }
  }
  
  if (AnaTree->NeuLAND_Nclusters>0)
  {
      for (Int_t k = 0; k<(AnaTree->NeuLAND_Nclusters); ++k)
      {
          AnaTree->NeuLAND_Cluster_IsVETOed[k] = kFALSE;
      }
  }
      
  Int_t Nprims = AnaTree->Nprims_NeuLAND;
  for (Int_t k = 0; k<Nprims; ++k)
  {
      AnaTree->Prim_IsVETOed[k] = kFALSE;
  }
  
  // Then now test if we have to change some stuff. Only if the tracker is there.
  // Otherwise we simply do not do anything!
  if (TheLeafs->Tracker_Exists)
  {
      if (Inputs->Bug_Report==5) {cout << "### BUG REPORT: The tracker exists!\n";}
      
      // Now we should do the actual VETOing. We have the reconstructed
      // Primary interaction points available. From the VETO digitizer
      // we also have the points of the VETO interactions available.
      // Hence we must now somehow compare them and then decide which
      // points to VETO.
      
      // We begin by looping over the VETO hitpoints in the digitizer.
      // Notice that if the tracker exists, the digitizer should also
      // exist. Hence we can safely access it here. Notice that when 
      // we use the tracker, there is no point in using LandPoints
      // for our VETO hit. Hence we simple use the VETO digitizer always here.
      
      // Declare the variables that we need:      
      Int_t VETO_nHits = TheLeafs->veto_segDigi_fQdc->GetNdata();
      Int_t Rec_nHits = AnaTree->N_Reconstructed_PrimPoints;
      
      // Only continue if we have both reconstructed points & VETO hits:
      if ((Rec_nHits>0)&&(VETO_nHits>0))
      {
          
      Double_t c = 29.9792458; // NOTE: Units!!! [cm]\[ns]
      c = Inputs->EffectiveC;
      
      TVector3 NeuPoint;
      TVector3 VetoPoint;
      Double_t* Comparison = new Double_t[Rec_nHits];
      
      Double_t hit_X = 0.0;
      Double_t hit_Y = 0.0;
      Double_t hit_Z = 0.0;
      Double_t hit_T = 0.0;   
      Double_t rec_X = 0.0;
      Double_t rec_Y = 0.0;
      Double_t rec_Z = 0.0;
      Double_t rec_T = 0.0;
      
      Double_t Travel_Dist = 0.0;
      Double_t time = 0.0;
      Double_t beta = 0.0;
      Double_t gamma = 0.0;
      Double_t Ekin = 0.0;
      Double_t theta = 0.0;
      Double_t phi = 0.0;
      Double_t Predicted_TDC = 0.0;
      Double_t min = 1e99;
      Int_t kmin = 0;
      Double_t SpaceTime_Dist = 0.0;
      
      // Then loop over the VETO hits:
      for (Int_t kveto = 0; kveto<VETO_nHits; ++kveto)
      {
          // Reset everything:
          for (Int_t kNeu = 0; kNeu<Rec_nHits; ++kNeu)
          {
              Comparison[kNeu] = 1e98;
          }
          
          // Then get the VETO hit:
          hit_X = TheLeafs->veto_segDigi_fxx->GetValue(kveto); // [cm]
          hit_Y = TheLeafs->veto_segDigi_fyy->GetValue(kveto); // [cm]
          hit_Z = TheLeafs->veto_segDigi_fzz->GetValue(kveto); // [cm]
          hit_T = TheLeafs->veto_segDigi_fTdc->GetValue(kveto); // [ns]
          
          // Now we must somehow compare these hits with the reconstructed
          // points in NeuLAND. We will always VETO exactly 1 point for each
          // fired bar of the VETO, but only if that VETO bar fired before NeuLAND.
          for (Int_t kNeu = 0; kNeu<Rec_nHits; ++kNeu)
          {
            // Get the NeuLAND hit:
            rec_X = AnaTree->X_Reconstructed_PrimPoints[kNeu];
            rec_Y = AnaTree->Y_Reconstructed_PrimPoints[kNeu];
            rec_Z = AnaTree->Z_Reconstructed_PrimPoints[kNeu];
            rec_T = AnaTree->T_Reconstructed_PrimPoints[kNeu];
            
            // Now the trouble is that the points are not very
            // close together, since the particle will travel
            // some more distance untill it makes its primary interaction
            // point in NeuLAND. We have to model this transport.
              
            // Now we will put the NeuLAND reconstructed point into a TVector3 and
            // then align it with the z-axis. We will perform the same transformations
            // on the VETO point. We will rotate around the starting point from the Inputs:
            TVector3 NeuPoint;
            NeuPoint.SetX(rec_X - Inputs->NeuProtonBegin_X);
            NeuPoint.SetY(rec_Y - Inputs->NeuProtonBegin_Y);
            NeuPoint.SetZ(rec_Z - Inputs->NeuProtonBegin_Z);
              
            TVector3 VetoPoint;
            VetoPoint.SetX(hit_X - Inputs->NeuProtonBegin_X);
            VetoPoint.SetY(hit_Y - Inputs->NeuProtonBegin_Y);
            VetoPoint.SetZ(hit_Z - Inputs->NeuProtonBegin_Z);
              
            theta = NeuPoint.Theta();
            phi = NeuPoint.Phi();
              
            NeuPoint.RotateZ(-phi);
            VetoPoint.RotateZ(-phi);
              
            NeuPoint.RotateY(-theta);
            VetoPoint.RotateY(-theta);
              
            // Check that the NeuPoint-vector indeed lies along the z-axis:
            if (((TMath::Abs(NeuPoint.X())>0.001)||(TMath::Abs(NeuPoint.Y())>0.001))&&(Inputs->Bug_Report==5)) {cout << "### BUG REPORT: We performed the wrong rotation!!!!!!!!!!!!!!!!\n";}
              
            // Now we have rotated VETO and NeuLAND hits to where the NeuLAND
            // hit is on the z-axis. Hence the distance to the z-axis of the 
            // veto hit is a good method of comparison. However, we also use
            // the time:
            Predicted_TDC = rec_T*((VetoPoint.Z())/(NeuPoint.Z()));
              
            // Then the real method of comparison becomes:
            SpaceTime_Dist = (VetoPoint.X())*(VetoPoint.X()) + (VetoPoint.Y())*(VetoPoint.Y()) + (c*(Predicted_TDC - hit_T))*(c*(Predicted_TDC - hit_T));
            Comparison[kNeu] = SpaceTime_Dist;
              
            // NOTE: Comparison is all 1e98 in the beginning.
          }
            
          // Now the comparison array is filled. Search for the minimum.
          // That point will be the point that we need! Since min starts at 1e99
          // and Comparison starts at 1e98, there will always be a minimum found!
          min = 1e99;
          kmin = -1;
            
          for (Int_t k = 0; k<Rec_nHits; ++k)
          {
            if (Comparison[k]<min) {min = Comparison[k]; kmin = k;}
          }
            
          // So kmin holds the reconstructed point that we need to VETO.
          // We should however only VETO something for this fired VETO bar,
          // if this bar fired before NeuLAND. Not before entire NeuLAND< just before
          // the primary interaction point you want to VETO.
          if (AnaTree->VETO_TOF_Fired_Res[kveto]<AnaTree->T_Reconstructed_PrimPoints[kmin])
          {
            AnaTree->Reconstructed_PrimPoints_IsVETOed[kmin] = kTRUE;
            
            // Now by comparing TrackID we must decide which primary particle
            // to VETO:
            for (Int_t kprim = 0; kprim<Nprims; ++kprim)
            {
              if (AnaTree->Prim_TrackID[kprim]==AnaTree->TrackID_Reconstructed_PrimPoints[kmin])
              {
                AnaTree->Prim_IsVETOed[kprim] = kTRUE;
              }                        
            }
          }
          
          // Done! Now close the loop over VETO hits:
       }   

     // Every new goes with a delete:
     delete Comparison;

     // Now the VETO condition is done. But we still should compute the reconstructed
     // energy. This is done in processing the tracker, so we do not need to do it here.     
    
  }
  }
}