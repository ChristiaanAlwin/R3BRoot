void DeleteEverything(Leaf_Structure* const TheLeafs, Inputs_Structure* const Inputs, Tree_Structure* AnaTree, Int_t const Event, Int_t const Counter)
{
  // This function performs all the delete operators for all the dynamical arrays. For the rest it just has 
  // the same form as an analysis task.
  
  // So here we go:
  if ((AnaTree->Prim_Detected)!=0) {delete[] (AnaTree->Prim_Detected);}
  if ((AnaTree->Prim_Resolved)!=0) {delete[] (AnaTree->Prim_Resolved);}
  if ((AnaTree->Prim_TrackID)!=0) {delete[] (AnaTree->Prim_TrackID);}
  if ((AnaTree->Prim_PDG)!=0) {delete[] (AnaTree->Prim_PDG);}
  if ((AnaTree->Prim_E)!=0) {delete[] (AnaTree->Prim_E);}
  if ((AnaTree->Prim_IsVETOed)!=0) {delete[] (AnaTree->Prim_IsVETOed);}
  if ((AnaTree->Prim_IsFromTarget)!=0) {delete[] (AnaTree->Prim_IsFromTarget);}
  if ((AnaTree->Prim_E_Detected)!=0) {delete[] (AnaTree->Prim_E_Detected);}
  if ((AnaTree->Prim_MCinteraction_T)!=0) {delete[] (AnaTree->Prim_MCinteraction_T);}
  if ((AnaTree->Prim_MCinteraction_X)!=0) {delete[] (AnaTree->Prim_MCinteraction_X);}
  if ((AnaTree->Prim_MCinteraction_Y)!=0) {delete[] (AnaTree->Prim_MCinteraction_Y);}
  if ((AnaTree->Prim_MCinteraction_Z)!=0) {delete[] (AnaTree->Prim_MCinteraction_Z);}
  
  if ((AnaTree->Neu_BarNumber_Fired)!=0) {delete[] (AnaTree->Neu_BarNumber_Fired);}
  if ((AnaTree->Neu_Edep_Fired)!=0) {delete[] (AnaTree->Neu_Edep_Fired);}
  if ((AnaTree->Neu_TOF_Fired)!=0) {delete[] (AnaTree->Neu_TOF_Fired);}
  if ((AnaTree->Neu_TOF_Fired_Res)!=0) {delete[] (AnaTree->Neu_TOF_Fired_Res);}
  if ((AnaTree->Neu_Primary_TrackID)!=0) {delete[] (AnaTree->Neu_Primary_TrackID);}
  if ((AnaTree->Neu_Bar_IsVETOed)!=0) {delete[] (AnaTree->Neu_Bar_IsVETOed);}
  
  if ((AnaTree->VETO_BarNumber_Fired)!=0) {delete[] (AnaTree->VETO_BarNumber_Fired);}
  if ((AnaTree->VETO_Edep_Fired)!=0) {delete[] (AnaTree->VETO_Edep_Fired);}
  if ((AnaTree->VETO_TOF_Fired)!=0) {delete[] (AnaTree->VETO_TOF_Fired);}
  if ((AnaTree->VETO_TOF_Fired_Res)!=0) {delete[] (AnaTree->VETO_TOF_Fired_Res);}
  if ((AnaTree->VETO_Primary_TrackID)!=0) {delete[] (AnaTree->VETO_Primary_TrackID);}
  if ((AnaTree->VETO_TOF_index)!=0) {delete[] (AnaTree->VETO_TOF_index);}
  
  if ((AnaTree->NeuLAND_ClusterX)!=0) {delete[] (AnaTree->NeuLAND_ClusterX);}
  if ((AnaTree->NeuLAND_ClusterY)!=0) {delete[] (AnaTree->NeuLAND_ClusterY);}
  if ((AnaTree->NeuLAND_ClusterZ)!=0) {delete[] (AnaTree->NeuLAND_ClusterZ);}
  if ((AnaTree->NeuLAND_ClusterTime)!=0) {delete[] (AnaTree->NeuLAND_ClusterTime);}
  if ((AnaTree->NeuLAND_ClusterEnergy)!=0) {delete[] (AnaTree->NeuLAND_ClusterEnergy);}
  if ((AnaTree->NeuLAND_ClusterSize)!=0) {delete[] (AnaTree->NeuLAND_ClusterSize);}
  if ((AnaTree->NeuLAND_Cluster_TrackID)!=0) {delete[] (AnaTree->NeuLAND_Cluster_TrackID);}
  if ((AnaTree->NeuLAND_Cluster_IsVETOed)!=0) {delete[] (AnaTree->NeuLAND_Cluster_IsVETOed);}
  
  if ((AnaTree->T_Reconstructed_PrimPoints)!=0) {delete[] (AnaTree->T_Reconstructed_PrimPoints);}
  if ((AnaTree->X_Reconstructed_PrimPoints)!=0) {delete[] (AnaTree->X_Reconstructed_PrimPoints);}
  if ((AnaTree->Y_Reconstructed_PrimPoints)!=0) {delete[] (AnaTree->Y_Reconstructed_PrimPoints);}
  if ((AnaTree->Z_Reconstructed_PrimPoints)!=0) {delete[] (AnaTree->Z_Reconstructed_PrimPoints);}
  if ((AnaTree->TrackID_Reconstructed_PrimPoints)!=0) {delete[] (AnaTree->TrackID_Reconstructed_PrimPoints);}
  
  // NOTE: New delete commands should be put here!
}