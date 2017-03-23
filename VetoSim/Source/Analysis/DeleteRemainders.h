void DeleteRemainders(Leaf_Structure* const TheLeafs, Inputs_Structure* const Inputs, Tree_Structure* const AnaTree)
{
  // This function performs all the delete operators for all the static arrays and histograms.
  // it also closes the file of the AnalysisTree.
  
  // We will begin with deleting our histograms:
  if (Inputs->Secondaries_Exe==1)
  {
    for (Int_t k1 = 0; k1<10; ++k1)
    {
      for (Int_t k2 = 0; k2<5; ++k2)
      {
        AnaTree->Harray[k1][k2]->Delete();
      }
    }

    Inputs->Protons_TravelDist->Delete();
    Inputs->Electrons_TravelDist->Delete();
    Inputs->Pions_TravelDist->Delete();
    Inputs->Muons_TravelDist->Delete();
    Inputs->Alphas_TravelDist->Delete();
  }

  // Then we can delete our static arrays:
  delete[] AnaTree->PrimInteraction;
  delete[] AnaTree->BackInteraction;
  delete[] AnaTree->LoopInteraction;
  delete[] AnaTree->Energetic_Prim;
  delete[] AnaTree->Energetic_Back;
  delete[] AnaTree->VETO_TrackTOF;
  delete[] AnaTree->VETO_TrackTOF_Res;
  
  delete[] AnaTree->Secondary_Nprotons;
  delete[] AnaTree->Secondary_Nneutrons;
  delete[] AnaTree->Secondary_Nelectrons;
  delete[] AnaTree->Secondary_Ngammas;
  delete[] AnaTree->Secondary_Npions;
  delete[] AnaTree->Secondary_Nmuons;
  delete[] AnaTree->Secondary_Nalphas;
  delete[] AnaTree->Secondary_NCions;
  delete[] AnaTree->Secondary_Nfragments;
  delete[] AnaTree->Secondary_NallParticles;
  
  delete[] AnaTree->AllBars_IsVETOed;

  // NOTE: New delete commands should be put here!
  
  // Then finally, we close our output file:
  AnaTree->TheFile->Close();

  // We do not need to delete our structures themselves,
  // since they are not created with new or with a constructor.
  
  // Done!
  
}