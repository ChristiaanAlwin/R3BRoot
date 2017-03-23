void No_VETO_Condition(Leaf_Structure* const TheLeafs, Inputs_Structure* const Inputs, Tree_Structure* AnaTree, Int_t const Event, Int_t const Counter, TRandom3* generator)
{
  // This function performs the actual VETO condition. There are multiple versions of this function
  // available. This one simply does not VETO anything. The leafs that should be filled by the VETO
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
  // The purpose of this function is not to VETO anything! Hence we must first set all
  // leafs to kFALSE and then compute the detected energy. The first thing is easy:
    
  Int_t Nprims = AnaTree->Nprims_NeuLAND;
  for (Int_t k = 0; k<Nprims; ++k)
  {
      AnaTree->Prim_IsVETOed[k] = kFALSE;
  }
  
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
  
  if (AnaTree->N_Reconstructed_PrimPoints>0)
  {
      for (Int_t k = 0; k<(AnaTree->N_Reconstructed_PrimPoints); ++k)
      {
          AnaTree->Reconstructed_PrimPoints_IsVETOed[k] = kFALSE;
      }
  }
  
  if (AnaTree->NeuLAND_Nclusters>0)
  {
      for (Int_t k = 0; k<(AnaTree->NeuLAND_Nclusters); ++k)
      {
          AnaTree->NeuLAND_Cluster_IsVETOed[k] = kFALSE;
      }
  }
  
  // =============================================================================================
  // Now the next task is to compute the total energy deposition per identified primary particle.
  // Notice that only unVETOed bars should be taken along. But if the tracker is there,
  // the tracker gives a better result:
  
  if (!((TheLeafs->Tracker_Exists)&&(Inputs->AnaTracker_Exe==1))) {GetTotalEdep(TheLeafs,Inputs,AnaTree,Event,Counter,generator);}
    
  // Now this is all that we wanted to do.  
    
}