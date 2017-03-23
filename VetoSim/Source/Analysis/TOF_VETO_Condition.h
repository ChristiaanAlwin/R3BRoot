void TOF_VETO_Condition(Leaf_Structure* const TheLeafs, Inputs_Structure* const Inputs, Tree_Structure* AnaTree, Int_t const Event, Int_t const Counter, TRandom3* generator)
{
  // This function performs the actual VETO condition. There are multiple versions of this function
  // available. This one simply does VETO anything when the VETO fired BEFORE NeuLAND fired. The leafs that should be filled by the VETO
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
  // The purpose of this function is to VETO anything when the VETO fired BEFORE NeuLAND! Hence we must first set all
  // Boolian leafs appropriately and then compute the detected energy. Notice that for conditions
  // that either VETO an entire Event or nothing, there is no difference between VETOing
  // particles and bars. We simply set everything to kTRUE or to kFALSE.
    
  Int_t Nprims = AnaTree->Nprims_NeuLAND;
  for (Int_t k = 0; k<Nprims; ++k)
  {
      // Apply the TOF VETO condition:
      if ((AnaTree->NbarsFired_VETO>0)&&(AnaTree->VETO_TotalTOF_Res<AnaTree->Neu_TotalTOF_Res))
      {
            AnaTree->Prim_IsVETOed[k] = kTRUE;
      }
      else
      {
            AnaTree->Prim_IsVETOed[k] = kFALSE;
      }
  }
  
  Int_t NbarsFired = AnaTree->NbarsFired_NeuLAND;
  for (Int_t k = 0; k<NbarsFired; ++k)
  {
      // Apply the TOF VETO condition:
      if ((AnaTree->NbarsFired_VETO>0)&&(AnaTree->VETO_TotalTOF_Res<AnaTree->Neu_TotalTOF_Res))
      {
          AnaTree->Neu_Bar_IsVETOed[k] = kTRUE;
      }
      else
      {
          AnaTree->Neu_Bar_IsVETOed[k] = kFALSE;
      }
  }
  
  if (Inputs->Store_AllBars==1)
  {
    Int_t NbarsNeu = (Inputs->NeuLAND_geo_NsinglePlanes)*50;
    for (Int_t k = 0; k<NbarsNeu; ++k)
    {
        // Apply the TOF VETO condition:
        if ((AnaTree->NbarsFired_VETO>0)&&(AnaTree->VETO_TotalTOF_Res<AnaTree->Neu_TotalTOF_Res))
        {
            AnaTree->AllBars_IsVETOed[k] = kTRUE;
        }
        else
        {
            AnaTree->AllBars_IsVETOed[k] = kFALSE;
        }
    }
  }
  
  if (AnaTree->N_Reconstructed_PrimPoints>0)
  {
      for (Int_t k = 0; k<(AnaTree->N_Reconstructed_PrimPoints); ++k)
      {
          // Apply the TOF VETO condition:
          if ((AnaTree->NbarsFired_VETO>0)&&(AnaTree->VETO_TotalTOF_Res<AnaTree->Neu_TotalTOF_Res))
          {
              AnaTree->Reconstructed_PrimPoints_IsVETOed[k] = kTRUE;
          }
          else
          {
              AnaTree->Reconstructed_PrimPoints_IsVETOed[k] = kFALSE;
          }
      }
  }
  
  if (AnaTree->NeuLAND_Nclusters>0)
  {
      for (Int_t k = 0; k<(AnaTree->NeuLAND_Nclusters); ++k)
      {
          // Apply the TOF VETO condition:
          if ((AnaTree->NbarsFired_VETO>0)&&(AnaTree->VETO_TotalTOF_Res<AnaTree->Neu_TotalTOF_Res))
          {
              AnaTree->NeuLAND_Cluster_IsVETOed[k] = kTRUE;
          }
          else
          {
              AnaTree->NeuLAND_Cluster_IsVETOed[k] = kFALSE;
          }
      }
  }
  
  // =============================================================================================
  // Now the next task is to compute the total energy deposition per identified primary particle.
  // Notice that only unVETOed bars should be taken along. Only makes sense without a tracker:
  
  if (!((TheLeafs->Tracker_Exists)&&(Inputs->AnaTracker_Exe==1))) {GetTotalEdep(TheLeafs,Inputs,AnaTree,Event,Counter,generator);}
    
  // Now this is all that we wanted to do.  
    
}