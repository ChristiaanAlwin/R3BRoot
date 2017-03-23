void GetTotalEdep(Leaf_Structure* const TheLeafs, Inputs_Structure* const Inputs, Tree_Structure* AnaTree, Int_t const Event, Int_t const Counter, TRandom3* generator)
{
  // This function simply computes the total energy as measured by NeuLAND.
  // It adds the Edep per bar for all primary particle and then stores this in the appropriate leaf.
  // This is already defines in previous tasks. The comments are pretty small, since this is a sub-task,
  // not a task.
    
  // ==================================================================================================
  // So here we go:
  
  // Declare what we need:
  Int_t Nprims = AnaTree->Nprims_NeuLAND;
  Int_t NbarsFired = AnaTree->NbarsFired_NeuLAND;
  Double_t Total_Edep = 0.0;
  Int_t ThisTrackID = 0;
  Int_t BarTrackID = 0;
  
  for (Int_t n = 0; n<Nprims; ++n)
  {
      Total_Edep = 0.0; // reset the total energy deposition
      ThisTrackID = AnaTree->Prim_TrackID[n]; // Only do this particle:
      
      // Now loop over the fired bars in NeuLAND:
      for (Int_t k = 0; k<NbarsFired; ++k)
      {
          // First obtain by which particle this baris fired:
          BarTrackID = AnaTree->Neu_Primary_TrackID[k];
          
          // Of course only add the energy deposition if this is the ThisTrackID that we are investigating:
          if (BarTrackID==ThisTrackID)
          {
              // Then we only continue as well if this bas is not VETOed:
              if (!AnaTree->Neu_Bar_IsVETOed[k])
              {
                  // Now in this case: we have the right particle, we already know that we only 
                  // talk about fired bars and we know that the bar is not VETOed. So now add 
                  // the energy deposition:
                  Total_Edep = Total_Edep + AnaTree->Neu_Edep_Fired[k];
              }
          }
      }
      
      // So now for the ThirTrackID particle, we know the total energy deposition.
      // Hence add this to the leaf:
      AnaTree->Prim_E_Detected[n] = Total_Edep;
      
      // Done!      
  }
    
  // Now this is all that we wanted to do.  
    
}