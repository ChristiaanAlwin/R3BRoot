void Detectors(Leaf_Structure* const TheLeafs, Inputs_Structure* const Inputs, Tree_Structure* AnaTree, Int_t const Event, Int_t const Counter, TRandom3* generator)
{
   // This function processes some information on other detectors into our analysis tree.
    
   // written by C. A. Douma, for the R3B Collaboration.
    
   // ===================================================================================
    
   // put the number of tracks from STaRTrack & the number of hits in CALIFA in our AnalysisTree:
   
   if ((TheLeafs->STaRTrack_exists)&&(TheLeafs->Geant3))
   {
     AnaTree->Ntracks_STaRTrack = TheLeafs->TrackSize->GetValue(0);
   }
   
   if ((TheLeafs->CALIFA_exists)&&(TheLeafs->Geant3))
   {
     AnaTree->Nhits_CALIFA = TheLeafs->CaloHitSim_fEnergy->GetNdata();
   }
   
   // Done!
    
}