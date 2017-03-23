void TrackEvents(Leaf_Structure* const TheLeafs, Inputs_Structure* const Inputs, Tree_Structure* AnaTree, Int_t const Event, Int_t const Counter, TRandom3* generator)
{
  // This function tracks back all MonteCarlo points in the VETO to identify whether
  // this point came from backscattering of from a primary interaction. 
  // Results are accumulated per scintillator bar of the VETO. So then for every bar we
  // have a boolian that says whether the event contained a primary interaction.
  // a second boolian says whether the bar contained backscattering. A third boolian
  // is used to detect loop interactions, which are situations that appear like
  // a primary interaction, but are actually backscattering.
  // We also pass the TRandom3 generator along as non-=const so that seeds are updated every time.
  
  // The results are put
  // into the analysis tree. Therefore no outputs are required. The inputs are the pointers
  // to the 3 structures that hold all our information and the number of the current event.
  
  // NOTE: AnaTree should NOT be const, otherwise we cannot modify the structure.
  
  // Written by C. A. Douma, for the R3B collaboration
  
  // =============================================================================================
  // NOTE: We are working here with static arrays, not dynamical ones.
  // so no complicated new/delete stuff is required here.
  
  // before we do our calculations, we must however reset the arrays:
  Int_t NbarsVETO = (Inputs->VETO_geo_NsinglePlanes)*(Inputs->VETO_geo_NbarsPerPlane);
  
  for (Int_t k = 0; k<NbarsVETO; ++k)
  {
    AnaTree->PrimInteraction[k] = kFALSE;
    AnaTree->BackInteraction[k] = kFALSE;
    AnaTree->LoopInteraction[k] = kFALSE;
    AnaTree->Energetic_Prim[k] = kFALSE;
    AnaTree->Energetic_Back[k] = kFALSE;
    AnaTree->VETO_TrackTOF[k] = -100.0;
    AnaTree->VETO_TrackTOF_Res[k] = -100.0;
  }
  
  AnaTree->Overal_Prim = kFALSE;
  AnaTree->Overal_Back = kFALSE;
  AnaTree->Overal_EPrim = kFALSE;
  AnaTree->Overal_EBack = kFALSE;
  AnaTree->VETO_TotalTrackTOF = -100.0;
  AnaTree->VETO_TotalTrackTOF_Res = -100.0;
  
  // =============================================================================================  
  // Then we declare all intermediate variables that we might need:
  
  Bool_t ThisPrimInteraction = kFALSE;
  Bool_t ThisBackInteraction = kFALSE;
  Bool_t ThisLoopInteraction = kFALSE;
  Double_t xpoint = 0.0;
  Double_t ypoint = 0.0;
  Double_t zpoint = 0.0;
  Double_t light = 0.0;
  Double_t time = 0.0;
  Int_t ThisBar = 0;
  Int_t TrackID = 0;
  Double_t* Edep_PrimInteraction = new Double_t[NbarsVETO];
  Double_t* TOF_PrimInteraction = new Double_t[NbarsVETO];
  Double_t* Edep_BackInteraction = new Double_t[NbarsVETO];
  Double_t* TOF_BackInteraction = new Double_t[NbarsVETO];
  Double_t* Merged_TOF = new Double_t[NbarsVETO];
  for (Int_t k = 0; k<NbarsVETO; ++k)
  {
      Edep_PrimInteraction[k] = 0.0;
      TOF_PrimInteraction[k] = 1e99;
      Edep_BackInteraction[k] = 0.0;
      TOF_BackInteraction[k] = 1e99;
      Merged_TOF[k] = 1e99;
  }
  
  // Then get the size of the current event:
  Int_t ArraySize = TheLeafs->veto_segPoint_fTrackID->GetNdata();
  
  // Then we can loop, provided that the size is bigger then zero.
  // if this is not the case, then we just leave all the values
  // in the tree equal to kFALSE: that nothing happened.
  // This is of course also correct:
  if (ArraySize>0)
  {
      for (Int_t kveto = 0; kveto<ArraySize; ++kveto)
      {
        
         // First we reset the variables that keep track on this specific
         // interaction point inside the veto:
         ThisPrimInteraction = kFALSE;
         ThisBackInteraction = kFALSE;
         ThisLoopInteraction = kFALSE;
	 
         // Then we extract the information on this specific interaction point:
	 TrackID = TheLeafs->veto_segPoint_fTrackID->GetValue(kveto);
         ThisBar = TheLeafs->veto_segPoint_fSector->GetValue(kveto);
         light = TheLeafs->veto_segPoint_fLightYield->GetValue(kveto);
         time = TheLeafs->veto_segPoint_fTime->GetValue(kveto);
         
         // correct bar number if necessary:
         if ((Version==kFALSE)&&(Inputs->Detectors_VETO_Exists==2)) {ThisBar = ThisBar + 1;}
         
	 if (TrackID<0) {TrackID=0;} // prevent seg-faults: 
	 
         xpoint = TheLeafs->MCTrack_fStartX->GetValue(TrackID);
         ypoint = TheLeafs->MCTrack_fStartY->GetValue(TrackID);
         zpoint = TheLeafs->MCTrack_fStartZ->GetValue(TrackID);
       
         // Then we track this point back untill we leave the veto:
         while (PointInsideVETO(Inputs,xpoint,ypoint,zpoint))
         {
           TrackID = TheLeafs->MCTrack_fMotherId->GetValue(TrackID);
	   if (TrackID<0) {TrackID=0;} // NOTE: do not place the particle gun inside the VETO!
	   xpoint = TheLeafs->MCTrack_fStartX->GetValue(TrackID);
           ypoint = TheLeafs->MCTrack_fStartY->GetValue(TrackID);
           zpoint = TheLeafs->MCTrack_fStartZ->GetValue(TrackID);
         }
         
         // Afterwe come out of the while-loop, xpoint, ypoint and zpoint
         // and also TrackID correspond to the MonteCarlo point
         // outside the VETO. Now we need to establish on
         // which side of the VETO it left:
         if (PointInFrontVETO(Inputs,xpoint,ypoint,zpoint))
         {
             ThisPrimInteraction = kTRUE;
             ThisLoopInteraction = kFALSE;
             ThisBackInteraction = kFALSE;
         }
         else
         {
             ThisPrimInteraction = kFALSE;
             ThisLoopInteraction = kFALSE;
             ThisBackInteraction = kTRUE;
         }
        
         // Now if we have a primary interaction, we have to establish
         // whether it really is a primary interaction, or actually,
         // a loopintercation:
	 
	 if (ThisPrimInteraction)
	 {
	   while (PointInsidePrimVolume(Inputs,xpoint,ypoint,zpoint))
	   {
	     // Now we keep tracking back untill we leave the 
             // primary volume:
	     TrackID = TheLeafs->MCTrack_fMotherId->GetValue(TrackID);
	     xpoint = TheLeafs->MCTrack_fStartX->GetValue(TrackID);
             ypoint = TheLeafs->MCTrack_fStartY->GetValue(TrackID);
             zpoint = TheLeafs->MCTrack_fStartZ->GetValue(TrackID);
             
             // And on each step, we check if we stay in front of the VETO:
             if (!PointInFrontVETO(Inputs,xpoint,ypoint,zpoint)) {ThisLoopInteraction = kTRUE;}
           }
         }

         // So notice that a pure priminteraction has priminteraction true and the other two false.
         // a loopinteraction has both prim and loop true and back false. A pure back has only back
         // equal to true:
         if (ThisBar<NbarsVETO+1)
         {
         if ((ThisPrimInteraction)&&(!ThisLoopInteraction)&&(!ThisBackInteraction)) {AnaTree->PrimInteraction[ThisBar-1] = kTRUE;}
         if ((ThisPrimInteraction)&&(ThisLoopInteraction)&&(!ThisBackInteraction)) {AnaTree->LoopInteraction[ThisBar-1] = kTRUE; AnaTree->BackInteraction[ThisBar-1] = kTRUE;}
         if ((!ThisPrimInteraction)&&(!ThisLoopInteraction)&&(ThisBackInteraction)) {AnaTree->BackInteraction[ThisBar-1] = kTRUE;}
         }
         // NOTE: Then notice that backscattering does ALSO include loop interactions!

         // also notice that if multiple monte carlo points (within a bar) pass, more then 1 boolian can
         // become true!
             
         // Now finally: Only if this is a pure primary interaction, add the light output of that point (and also process TOF):
         // Since we started at 1e99 for TOF, we can safely search for the minimum straight away. If it is not a primary,
         // add the TOF and the light to the backscattering:
         if (ThisBar<NbarsVETO+1)
         {
         if ((ThisPrimInteraction)&&(!ThisLoopInteraction)&&(!ThisBackInteraction))
         {
             Edep_PrimInteraction[ThisBar-1] = Edep_PrimInteraction[ThisBar-1] + light;
             if (time<TOF_PrimInteraction[ThisBar-1]) {TOF_PrimInteraction[ThisBar-1] = time;}
         }
         else
         {
             Edep_BackInteraction[ThisBar-1] = Edep_BackInteraction[ThisBar-1] + light;
             if (time<TOF_BackInteraction[ThisBar-1]) {TOF_BackInteraction[ThisBar-1] = time;}
         }
         }

         // and now we can end our loop over the veto_segPoint:
      }
  }
  
  // =========================================================================================
  // Now we must do some final tasks:
  
  // Change the TOF 1e99 that are left:
  for (Int_t k = 0; k<NbarsVETO; ++k)
  {
      if (TOF_PrimInteraction[k]>1e98) {TOF_PrimInteraction[k] = -100.0;}
      if (TOF_BackInteraction[k]>1e98) {TOF_BackInteraction[k] = -100.0;}
  }
  
  // Now we must add the information of energetic backscattering to the leafs. We also merge the TOF together:
  for (Int_t k = 0; k<NbarsVETO; ++k)
  {
      if ((1e6*Edep_BackInteraction[k])>(Inputs->Veto_Ethreshold)) {AnaTree->Energetic_Back[k] = kTRUE; Merged_TOF[k] = TOF_BackInteraction[k];} // NOTE: Conversion between [GeV] and [keV]!
      if ((1e6*Edep_PrimInteraction[k])>(Inputs->Veto_Ethreshold)) {AnaTree->Energetic_Prim[k] = kTRUE; Merged_TOF[k] = TOF_PrimInteraction[k];} // NOTE: Conversion between [GeV] and [keV]!
      // We do the backscattering first, so that if both are true, Merged_TOF will have the Primary TOF, exactly what we want!
  }
  
  // Then we can now define the overall boolians:
  for (Int_t k = 0; k<NbarsVETO; ++k)
  {
      if (AnaTree->PrimInteraction[k]) {AnaTree->Overal_Prim = kTRUE;}
      if (AnaTree->BackInteraction[k]) {AnaTree->Overal_Back = kTRUE;}
      if (AnaTree->LoopInteraction[k]) {AnaTree->Overal_Back = kTRUE;}
      if (AnaTree->Energetic_Prim[k]) {AnaTree->Overal_EPrim = kTRUE;}
      if (AnaTree->Energetic_Back[k]) {AnaTree->Overal_EBack = kTRUE;}
  }
  
  // NOTE: The information from the Merged_TOF array is difficult to process since one cannot
  // be sure that the amount of fired TOF is exactly the same as it is for the ordinary TOF.
  // But we can put it into a static array. We also search for the minimum:
  Double_t MinTOF = 1e99;
  Double_t MinTOF_Res = 1e99;
  for (Int_t k = 0; k<NbarsVETO; ++k)
  {
      AnaTree->VETO_TrackTOF[k] = Merged_TOF[k];
      AnaTree->VETO_TrackTOF_Res[k] = Merged_TOF[k] + generator->Gaus(0.0,1e-3*(Inputs->VETO_TimeResolution)); // NOTE: conversion from [ps] to [ns]!
      if (AnaTree->VETO_TrackTOF[k]<MinTOF) {MinTOF = AnaTree->VETO_TrackTOF[k];}
      if (AnaTree->VETO_TrackTOF_Res[k]<MinTOF_Res) {MinTOF_Res = AnaTree->VETO_TrackTOF_Res[k];}
  }
  
  if (MinTOF>1e98) {MinTOF = -100.0;}
  if (MinTOF_Res>1e98) {MinTOF_Res = -100.0;}
  AnaTree->VETO_TotalTrackTOF = MinTOF;
  AnaTree->VETO_TotalTrackTOF_Res = MinTOF_Res;
  
  // now redefine the AnaTree-variables onto -100.0:
  for (Int_t k = 0; k<NbarsVETO; ++k)
  {
    if (AnaTree->VETO_TrackTOF[k]>1e98) {AnaTree->VETO_TrackTOF[k] = -100.0;}
    if (AnaTree->VETO_TrackTOF_Res[k]>1e98) {AnaTree->VETO_TrackTOF_Res[k] = -100.0;}
  }
  if (AnaTree->VETO_TotalTrackTOF>1e98) {AnaTree->VETO_TotalTrackTOF = -100.0;}
  if (AnaTree->VETO_TotalTrackTOF_Res>1e98) {AnaTree->VETO_TotalTrackTOF_Res = -100.0;}
  
  // Done! Now delete our intermediate arrays:
  delete Edep_PrimInteraction;
  delete TOF_PrimInteraction;
  delete Edep_BackInteraction;
  delete TOF_BackInteraction;
  delete Merged_TOF;
  
  // ===========================================================================================
  // NOTE: if a VETO bar contains BOTH a primary interaction and 
  // a LoopInteraction or a BackInteraction, then one needs to determine which of these
  // events trigger the energy threshold. Also TOF of these bars is important,
  // since the TOF of the first signal that triggered the energy threshold
  // should be used. This will be done in a different analysis task. This task
  // is done now.
}

// ###############################################################################################################
// Add the empty function:

void Empty_TrackEvents(Leaf_Structure* const TheLeafs, Inputs_Structure* const Inputs, Tree_Structure* AnaTree, Int_t const Event, Int_t const Counter, TRandom3* generator)
{
  Int_t NbarsVETO = (Inputs->VETO_geo_NsinglePlanes)*(Inputs->VETO_geo_NbarsPerPlane);
  
  for (Int_t k = 0; k<NbarsVETO; ++k)
  {
    AnaTree->PrimInteraction[k] = kFALSE;
    AnaTree->BackInteraction[k] = kFALSE;
    AnaTree->LoopInteraction[k] = kFALSE;
    AnaTree->Energetic_Prim[k] = kFALSE;
    AnaTree->Energetic_Back[k] = kFALSE;
    AnaTree->VETO_TrackTOF[k] = -100.0;
    AnaTree->VETO_TrackTOF_Res[k] = -100.0;
  }
  
  AnaTree->Overal_Prim = kFALSE;
  AnaTree->Overal_Back = kFALSE;
  AnaTree->Overal_EPrim = kFALSE;
  AnaTree->Overal_EBack = kFALSE;
  AnaTree->VETO_TotalTrackTOF = -100.0;
  AnaTree->VETO_TotalTrackTOF_Res = -100.0;
}
