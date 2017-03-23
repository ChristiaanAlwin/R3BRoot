void ProcessTracker(Leaf_Structure* const TheLeafs, Inputs_Structure* const Inputs, Tree_Structure* AnaTree, Int_t const Event, Int_t const Counter, TRandom3* generator)
{
  // This function retrieves the Reconstructed Primary Interaction Points from the R3BNeutronTracker
  // and puts them into our own analysis tree. then, each reconstructed point is assigned a TrackID
  // of one of the primary particles identified in the beginning of our program. The primary particle 
  // that has its primary MC interaction point clostest to this reconstructed point is selected for this.
  // Closest means that x^2 + y^2 + z^2 + (ct)^2 is minimal. for each reconstructed point a single primary interaction point is found by minimizing
  // this distance. If a primary interaction point is assigned to multiple reconstructed points, the smallest distance is kept and the other reconstructed
  // points are re-assigned. If no primary interaction point can be found for a reconstructed point, then this value becomes -1.
    
  // Thatś all this function does.
    
  // Written by C. A. Douma for the R3B-Collaboration.
    
  // ===============================================================================================
  // Reset our single-value leafs:
  if (Inputs->Bug_Report==7) {cout << "### BUG REPORT: -------------- Begin of event number " << Event << " --------------------\n";}   
  AnaTree->N_Reconstructed_PrimPoints = 0;
  
  // reset some other leafs:
  Int_t Nprims = AnaTree->Nprims_NeuLAND;
  
  for (Int_t k = 0; k<Nprims; ++k)
  {
    AnaTree->Prim_Resolved[k] = kFALSE;
  }
    
  // ===============================================================================================
  // Do our computations. Actually this will be quite hard to do, since our most important job is
  // to just retrieve the results from the tracker. Then we better put them directly in our tree.
  // Then the only thing left is to make the comparison, which we then do in the end.    
  // =============================================================================================
  // Now we put our results in the tree. Notice that most of our leafs are dynamical arrays, so
  // we should fill them with caution!
    
  // First define the length of our arrays:
  Int_t ArrayLength = TheLeafs->LandNeutHits_fX->GetNdata();
  
  // Then put this into the leaf that holds the size of these dynamical arrays:
  AnaTree->N_Reconstructed_PrimPoints = ArrayLength;
  if (Inputs->Bug_Report==7) {cout << "### BUG REPORT: Number of reconstructed tracker = " << ArrayLength << "\n";}   
  
  // Then re-allocate the arrays:
  AnaTree->T_Reconstructed_PrimPoints = new Double_t[ArrayLength];
  AnaTree->X_Reconstructed_PrimPoints = new Double_t[ArrayLength];
  AnaTree->Y_Reconstructed_PrimPoints = new Double_t[ArrayLength];
  AnaTree->Z_Reconstructed_PrimPoints = new Double_t[ArrayLength];
  AnaTree->Reconstructed_PrimPoints_IsVETOed = new Bool_t[ArrayLength];
  AnaTree->TrackID_Reconstructed_PrimPoints = new Int_t[ArrayLength];
  
  // Then re-set the Branch Addresses:
  AnaTree->TheTree->SetBranchAddress("T_Reconstructed_PrimPoints",(void*) AnaTree->T_Reconstructed_PrimPoints);
  AnaTree->TheTree->SetBranchAddress("X_Reconstructed_PrimPoints",(void*) AnaTree->X_Reconstructed_PrimPoints);
  AnaTree->TheTree->SetBranchAddress("Y_Reconstructed_PrimPoints",(void*) AnaTree->Y_Reconstructed_PrimPoints);
  AnaTree->TheTree->SetBranchAddress("Z_Reconstructed_PrimPoints",(void*) AnaTree->Z_Reconstructed_PrimPoints);
  AnaTree->TheTree->SetBranchAddress("Reconstructed_PrimPoints_IsVETOed",(void*) AnaTree->Reconstructed_PrimPoints_IsVETOed);
  AnaTree->TheTree->SetBranchAddress("TrackID_Reconstructed_PrimPoints",(void*) AnaTree->TrackID_Reconstructed_PrimPoints);
  
  // NOTE: The delete commands should be put in the DeleteEverything-function.
  
  // Now put the results of the Neutron Tracker into the tree:
  if (ArrayLength>0)
  {
    for (Int_t k = 0; k<ArrayLength; ++k)
    {
      AnaTree->T_Reconstructed_PrimPoints[k] = TheLeafs->LandNeutHits_fT->GetValue(k);
      AnaTree->X_Reconstructed_PrimPoints[k] = TheLeafs->LandNeutHits_fX->GetValue(k);
      AnaTree->Y_Reconstructed_PrimPoints[k] = TheLeafs->LandNeutHits_fY->GetValue(k);
      AnaTree->Z_Reconstructed_PrimPoints[k] = TheLeafs->LandNeutHits_fZ->GetValue(k);
      AnaTree->Reconstructed_PrimPoints_IsVETOed[k] = kFALSE; // Will be filled in during the VETO condition, if relevant!
      AnaTree->TrackID_Reconstructed_PrimPoints[k] = -1; // Fill it in below!
    }
  }
  
  // =============================================================================================
  // Now the only thing left to do is to make a comparison between the MC primary interaction
  // points and the reconstructed primary interaction points.
  
  // Define the speed of light in cm/ns:
  Double_t c = 29.9792458; // NOTE: Units!!!
  c = Inputs->EffectiveC;
  
  // Define the number of detected particles:
  Int_t Nprims_Det = 0;
  for (Int_t k = 0; k<Nprims; ++k)
  {
      if ((Inputs->Only_Detected!=1)||(AnaTree->Prim_Detected[k]==kTRUE)) {Nprims_Det = Nprims_Det + 1;}
  }
  
  // First test if we actually have primary interactions and reconstructed points:
  if ((Nprims_Det>0)&&(ArrayLength>0)) // If ArrayLength>0 but Nprims=0 then everything stays -1!
  {
  
  // Define an array with spacetime intervals:
  Double_t** dist = new Double_t*[ArrayLength]; // first index = reconstructed.
  for (Int_t k = 0; k<ArrayLength; ++k)
  {
      dist[k] = new Double_t[Nprims]; // Second index = MC.
  }
  
  // Now we should fill this array:
  Double_t Dx = 0.0;
  Double_t Dy = 0.0;
  Double_t Dz = 0.0;
  Double_t Dt = 0.0; // without c!!!
  
  for (Int_t krec = 0; krec<ArrayLength; ++krec)
  {
      for (Int_t kmc = 0; kmc<Nprims; ++kmc) // Need the full array since MC and REC are 2 different things!
      {
          if ((Inputs->Only_Detected!=1)||(AnaTree->Prim_Detected[kmc]==kTRUE))
          {
            Dx = AnaTree->X_Reconstructed_PrimPoints[krec] - AnaTree->Prim_MCinteraction_X[kmc];
            Dy = AnaTree->Y_Reconstructed_PrimPoints[krec] - AnaTree->Prim_MCinteraction_Y[kmc];
            Dz = AnaTree->Z_Reconstructed_PrimPoints[krec] - AnaTree->Prim_MCinteraction_Z[kmc];
            Dt = AnaTree->T_Reconstructed_PrimPoints[krec] - AnaTree->Prim_MCinteraction_T[kmc];
          
            dist[krec][kmc] = TMath::Sqrt(Dx*Dx + Dy*Dy + Dz*Dz + (c*Dt)*(c*Dt));
          }
          else
          {
            dist[krec][kmc] = 1e99;
          }
      }
  }
  
  // Now we should find minimal distances.
  Double_t mindist = 1e98;
  Double_t thisdist = 0.0;
  Int_t minkrec = -1;
  Int_t minkmc = -1;
  
  Int_t Nsearch = ArrayLength;
  if (Nprims_Det<ArrayLength) {Nsearch = Nprims_Det;}
  
  for (Int_t k = 0; k<Nsearch; ++k)
  {
      // This for-loop is just because we have to do this trick for a fixed amount
      // of reconstructed points.
      
      // Now find the minimum over the entire array:
      mindist = 1e98;
      minkrec = -1;
      minkmc = -1;
      
      for (Int_t krec = 0; krec<ArrayLength; ++krec)
      {
        for (Int_t kmc = 0; kmc<Nprims; ++kmc) // Need the full array since MC and REC are 2 different things!
        {
          thisdist = dist[krec][kmc];
          if (thisdist<mindist) {mindist = thisdist; minkrec = krec; minkmc = kmc;}
        }
      }
      
      // Now we know that reconstructed point minkrec and prim particle minkmc should be linked:
      AnaTree->TrackID_Reconstructed_PrimPoints[minkrec] = AnaTree->Prim_TrackID[minkmc];
      if (Inputs->Bug_Report==7) {cout << "### BUG REPORT: We linked rec. point " << minkrec << " to prim. particle " << minkmc << "\n";}
      
      // Then also decide if the MC-point is resolved:
      if (mindist<(Inputs->ResolveBoundary)) 
      {
          AnaTree->Prim_Resolved[minkmc] = kTRUE;
          
          // now due to all the digitization, etc. It can occur (rarely) that
          // the primary interaction point is linked to a primary particle that is
          // not detected: e.g. that did not fire at least one bar first based
          // on TOF, etc. This is an effect of o.a. time resolution, etc. Hence
          // we should manually define that a resolved particle is always detected:
          AnaTree->Prim_Detected[minkmc] = kTRUE;
          
          // Bug reports:
          if (Inputs->Bug_Report==7) {cout << "### BUG REPORT: We resolved a primary particle!\n";}
      }
      
      // This now means that the primary particle minkmc is taken. Hence exclude this from
      // the next search:
      for (Int_t krec = 0; krec<ArrayLength; ++krec)
      {
          dist[krec][minkmc] = 1e99;
      }
      
      // Also, the point minkrec is allocated. Hence also remove this from the search:
      for (Int_t kmc = 0; kmc<Nprims; ++kmc)
      {
          dist[minkrec][kmc] = 1e99;
      }
  
      // So now distances to both points that have been dealt with are eliminated.
      // So then reset everything and re-start the search. Theoretically one should
      // need to do this exactly ArrayLength times to ensure that all reconstructed
      // points have sometthing. Unless Nprims is smaller, then one needs to do it
      // Nprims times.
  }
  
  // Done! Now we need to reconstruct the energy with the tracker:
  Double_t rec_X = 0.0;
  Double_t rec_Y = 0.0;
  Double_t rec_Z = 0.0;
  Double_t rec_T = 0.0;
  Double_t Travel_Dist = 0.0;
  Double_t beta2 = 0.0;
  Double_t gamma = 0.0;
  Double_t Ekin = 0.0;
  Double_t time = 0.0;
  Double_t nmass = 939.565379; // Neutron mass [MeV].
  Double_t cvac = 29.9792458; // NOTE: Units!!! [cm]\[ns]
      
  for (Int_t kNeu = 0; kNeu<ArrayLength; ++kNeu)
  {
      // Get the NeuLAND hit:
      rec_X = AnaTree->X_Reconstructed_PrimPoints[kNeu];
      rec_Y = AnaTree->Y_Reconstructed_PrimPoints[kNeu];
      rec_Z = AnaTree->Z_Reconstructed_PrimPoints[kNeu];
      rec_T = AnaTree->T_Reconstructed_PrimPoints[kNeu];

      // We defined a particle leaving point in the Inputs.
      // Hence first compute the beta belonging to this
      // reconstructed point. Then we also can define the
      // particle energy, assuming neutron mass:
      Travel_Dist = TMath::Sqrt((Inputs->NeuBegin_X - rec_X)*(Inputs->NeuBegin_X - rec_X) + (Inputs->NeuBegin_Y - rec_Y)*(Inputs->NeuBegin_Y - rec_Y) + (Inputs->NeuBegin_Z - rec_Z)*(Inputs->NeuBegin_Z - rec_Z));
      
      // If the beam is not from the target, we have to add that distance:
      Travel_Dist = Travel_Dist + TMath::Sqrt((Inputs->NeuBegin_X - Inputs->Beam_Xpos)*(Inputs->NeuBegin_X - Inputs->Beam_Xpos) + (Inputs->NeuBegin_Y - Inputs->Beam_Ypos)*(Inputs->NeuBegin_Y - Inputs->Beam_Ypos) + (Inputs->NeuBegin_Z - Inputs->Beam_Zpos)*(Inputs->NeuBegin_Z - Inputs->Beam_Zpos));
            
      time = rec_T;
      beta2 = (Travel_Dist/time)/cvac; // NOTE: dimensionless!
      gamma = 1.0/TMath::Sqrt(1.0 - beta2*beta2);
      Ekin = (gamma - 1.0)*nmass; // [MeV]
     
      // Then loop over the primary MC points to find the 
      // TrackID and assign the energy:
      for (Int_t kprim = 0; kprim<Nprims; ++kprim)
      {
         if (AnaTree->Prim_TrackID[kprim]==AnaTree->TrackID_Reconstructed_PrimPoints[kNeu])
         {
             AnaTree->Prim_E_Detected[kprim] = Ekin;
         }                        
      }
  }
  
  // Everything is now OK. Close all blocks: 
  }
  
  if (Inputs->Bug_Report==7) {cout << "### BUG REPORT: -------------- End of event number " << Event << " --------------------\n\n\n";}   
}

// ####################################################################################################
// Put in the empty function:

void Empty_ProcessTracker(Leaf_Structure* const TheLeafs, Inputs_Structure* const Inputs, Tree_Structure* AnaTree, Int_t const Event, Int_t const Counter, TRandom3* generator)
{
  // Define empty arrays:
  Int_t ArrayLength = 0;
  AnaTree->N_Reconstructed_PrimPoints = ArrayLength;

  AnaTree->T_Reconstructed_PrimPoints = new Double_t[ArrayLength];
  AnaTree->X_Reconstructed_PrimPoints = new Double_t[ArrayLength];
  AnaTree->Y_Reconstructed_PrimPoints = new Double_t[ArrayLength];
  AnaTree->Z_Reconstructed_PrimPoints = new Double_t[ArrayLength];
  AnaTree->TrackID_Reconstructed_PrimPoints = new Int_t[ArrayLength];

  AnaTree->TheTree->SetBranchAddress("T_Reconstructed_PrimPoints",(void*) AnaTree->T_Reconstructed_PrimPoints);
  AnaTree->TheTree->SetBranchAddress("X_Reconstructed_PrimPoints",(void*) AnaTree->X_Reconstructed_PrimPoints);
  AnaTree->TheTree->SetBranchAddress("Y_Reconstructed_PrimPoints",(void*) AnaTree->Y_Reconstructed_PrimPoints);
  AnaTree->TheTree->SetBranchAddress("Z_Reconstructed_PrimPoints",(void*) AnaTree->Z_Reconstructed_PrimPoints);
  AnaTree->TheTree->SetBranchAddress("TrackID_Reconstructed_PrimPoints",(void*) AnaTree->TrackID_Reconstructed_PrimPoints);
}