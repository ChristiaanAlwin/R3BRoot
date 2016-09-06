void QTDC(Leaf_Structure* const TheLeafs, Inputs_Structure* const Inputs, Tree_Structure* AnaTree, Int_t const Event, Int_t const Counter, TRandom3* generator)
{
  // This function Gets the total energy deposition and the TOF per bar. This is performed
  // for both the VETO and for NeuLAND. For NeuLAND and the VETO, the data is saved
  // as a dynamical array: only bars that are fired are stored. It also tracks back each
  // bar to the primary that fired it. The Monte Carlo point that hit the bar with smallest TOF
  // is used to do this tracking. Only Fired bars are taken along. For the VETO, the index of
  // the specific Monte Carlo point is stored as well. For NeuLAND, a leaf that states which bar is
  // VETOed, is created too, but not filled.
  // We also pass the TRandom3 generator along as non-=const so that seeds are updated every time.
  
  // The results are put the results
  // into the analysis tree. Therefore no outputs are required. The inputs are the pointers
  // to the 3 structures that hold all our information and the number of the current event.
  
  // NOTE: AnaTree should NOT be const, otherwise we cannot modify the structure.
  
  // Template is Written by C. A. Douma, for the R3B collaboration
    
  // =============================================================================================
  // Before starting your calculations, the values of the variables holding
  // information that will be put into the tree should be reset. This should be
  // done for single valued leafs and for static arrays:
  AnaTree->Neu_TotalTOF = 0.0;
  AnaTree->VETO_TotalTOF = 0.0;
  AnaTree->Neu_TotalTOF_Res = 0.0;
  AnaTree->VETO_TotalTOF_Res = 0.0;
  
  // =============================================================================================
  // We begin with obtaining the total amount of bars for NeuLAND and for the VETO:
  Int_t NbarsVETO = (Inputs->VETO_geo_NsinglePlanes)*(Inputs->VETO_geo_NbarsPerPlane);  
  Int_t NbarsNeu = (Inputs->NeuLAND_geo_NsinglePlanes)*50;
  
  // Then we declare some arrays to store our information in:
  Double_t* EdepNeu = new Double_t[NbarsNeu];
  Double_t* TOFNeu = new Double_t[NbarsNeu];
  Double_t* TOFNeuRes = new Double_t[NbarsNeu];
  Int_t* NeuIndex = new Int_t[NbarsNeu]; // index of the minimun, hence the TOF
  Int_t* NeuPrimID = new Int_t[NbarsNeu]; // tracked back primary of NeuIndex.
  
  Double_t* EdepVETO = new Double_t[NbarsVETO];
  Double_t* TOFVETO = new Double_t[NbarsVETO];
  Double_t* TOFVETORes = new Double_t[NbarsVETO];
  Int_t* VETOIndex = new Int_t[NbarsVETO];
  Int_t* VETOPrimID = new Int_t[NbarsVETO];
  
  // Then fill the arrays with zeros:
  for (Int_t k = 0; k<NbarsNeu; ++k)
  {
      EdepNeu[k] = 0.0;
      TOFNeu[k] = 0.0;
      TOFNeuRes[k] = 0.0;
      NeuIndex[k] = -1; // these indices give seg-fault. Hence if our code does not work, we see it immediately!
      NeuPrimID[k] = -1;
  }
  
  for (Int_t k = 0; k<NbarsVETO; ++k)
  {
      EdepVETO[k] = 0.0;
      TOFVETO[k] = 0.0;
      TOFNeuRes[k] = 0.0;
      VETOIndex[k] = -1;
      VETOPrimID[k] = -1;
  }
  
  // Now declare all variables that we need:
  Double_t light = 0.0;
  Double_t TotalLight = 0.0;
  Double_t TOF = 0.0;
  Double_t TotalTOF = 0.0;
  Int_t Sector = 0;
  Int_t ThisNeuIndex = -1;
  Int_t ThisVETOIndex = -1;
  
  // We begin with looping over the landPoints. We fill the Edep array with
  // fLightYield and we fill the TOF with the MAXIMUM of the TOF of all points:
  Int_t NPointsNeu = TheLeafs->LandPoint_fLightYield->GetNdata();
  
  if (NPointsNeu>0)
  {
      for (Int_t kNeu = 0; kNeu<NPointsNeu; ++kNeu)
      {
          TOF = TheLeafs->LandPoint_fTime->GetValue(kNeu);
          light = TheLeafs->LandPoint_fLightYield->GetValue(kNeu);
          Sector = TheLeafs->LandPoint_fSector->GetValue(kNeu);
          
          if (Sector<NbarsNeu+1) { // NOTE: Ignore NEBULA!
          // Add all lightYields:
          TotalLight = EdepNeu[Sector-1];
          TotalLight = TotalLight + light;
          EdepNeu[Sector-1] = TotalLight;
          
          // search for the maximum:
          TotalTOF = TOFNeu[Sector-1];
          if (TotalTOF<TOF) {TotalTOF = TOF;}
          TOFNeu[Sector-1] = TotalTOF;}
      }
      
      // Now search for the minimal TOF:
      for (Int_t kNeu = 0; kNeu<NPointsNeu; ++kNeu)
      {
          TOF = TheLeafs->LandPoint_fTime->GetValue(kNeu);
          Sector = TheLeafs->LandPoint_fSector->GetValue(kNeu);
          
          if (Sector<NbarsNeu+1) {
          // search for the minimum:
          TotalTOF = TOFNeu[Sector-1];
	  ThisNeuIndex = NeuIndex[Sector-1];
          if (TotalTOF>=TOF) {TotalTOF = TOF; ThisNeuIndex = kNeu;} // also save the index...
          TOFNeu[Sector-1] = TotalTOF;
	  NeuIndex[Sector-1] = ThisNeuIndex;}
      }
  }
  
  // Now do the same with looping over the VETO points:
  Int_t NPointsVETO = TheLeafs->veto_segPoint_fLightYield->GetNdata();
  
  if (NPointsVETO>0)
  {
      for (Int_t kveto = 0; kveto<NPointsVETO; ++kveto)
      {
          TOF = TheLeafs->veto_segPoint_fTime->GetValue(kveto);
          light = TheLeafs->veto_segPoint_fLightYield->GetValue(kveto);
          Sector = TheLeafs->veto_segPoint_fSector->GetValue(kveto);
          
          if (Sector<NbarsVETO+1) {
          // Add all lightYields:
          TotalLight = EdepVETO[Sector-1];
          TotalLight = TotalLight + light;
          EdepVETO[Sector-1] = TotalLight;
          
          // search for the maximum:
          TotalTOF = TOFVETO[Sector-1];
          if (TotalTOF<TOF) {TotalTOF = TOF;}
          TOFVETO[Sector-1] = TotalTOF;}
      }
      
      // Now search for the minimal TOF:
      for (Int_t kveto = 0; kveto<NPointsVETO; ++kveto)
      {
          TOF = TheLeafs->veto_segPoint_fTime->GetValue(kveto);
          Sector = TheLeafs->veto_segPoint_fSector->GetValue(kveto);
          
          if (Sector<NbarsVETO+1) {
          // search for the minimum:
          TotalTOF = TOFVETO[Sector-1];
	  ThisVETOIndex = VETOIndex[Sector-1];
          if (TotalTOF>=TOF) {TotalTOF = TOF; ThisVETOIndex = kveto;}
          TOFVETO[Sector-1] = TotalTOF;
	  VETOIndex[Sector-1] = ThisVETOIndex;}
      }
  }
   
  // Now that we have all TOF and Edep, we must add the time resolution. Notice that all 
  // TOF we obtained are in [ns] while the time resolutions are in [ps].
  for (Int_t k = 0; k<NbarsNeu; ++k)
  {
      TOFNeuRes[k] = TOFNeu[k] + generator->Gaus(0.0,((Inputs->Neu_TimeResolution)*1e-3));
  }
  
  for (Int_t k = 0; k<NbarsVETO; ++k)
  {
      TOFVETORes[k] = TOFVETO[k] + generator->Gaus(0.0,((Inputs->VETO_TimeResolution)*1e-3));
  }
  
  // Now we have all our indices of the min. TOF point. Now we must track them back.
  // Declare what we need:
  Int_t index = -1;
  Int_t TrackID = -1;
  Double_t xpoint = 0.0;
  Double_t ypoint = 0.0;
  Double_t zpoint = 0.0;
  
  // Now loop over the bars for this tracking:
  for (Int_t bar = 0; bar<NbarsNeu; ++bar)
  {
    // get our index of the current bar:
    index = NeuIndex[bar];
    
    // Now only if it is >=0, there is something to track it back...
    if (index>=0)
    {
      // Get the track information that belongs to this index:
      TrackID = TheLeafs->LandPoint_fTrackID->GetValue(index);
      if (TrackID<0) {TrackID=0;} // prevent seg-faults.
      xpoint = TheLeafs->MCTrack_fStartX->GetValue(TrackID);
      ypoint = TheLeafs->MCTrack_fStartY->GetValue(TrackID);
      zpoint = TheLeafs->MCTrack_fStartZ->GetValue(TrackID);
      
      // Now in order to get a primary particle, we must track it
      // back untill it leaves the primary volume:
      while (PointInsidePrimVolume(Inputs,xpoint,ypoint,zpoint))
      {
	TrackID = TheLeafs->MCTrack_fMotherId->GetValue(TrackID);
        xpoint = TheLeafs->MCTrack_fStartX->GetValue(TrackID);
        ypoint = TheLeafs->MCTrack_fStartY->GetValue(TrackID);
        zpoint = TheLeafs->MCTrack_fStartZ->GetValue(TrackID);
      }
      
      // So now we have the TrackID and position of the particle
      // just before entering the PrimVolume. Hence now it is a
      // primary particle. Hence we now can store the TrackID:
      NeuPrimID[bar] = TrackID;
      
      // Done!
    }
    else
    {
      // In this case there is nothing to track back. Hence
      // we simply store again a -1:
      NeuPrimID[bar] = -1;
    }
  }
  
  // So that's it. Now do exactly the same for the VETO:
  for (Int_t bar = 0; bar<NbarsVETO; ++bar)
  {
    // get our index of the current bar:
    index = VETOIndex[bar];
    
    // Now only if it is >=0, there is something to track it back...
    if (index>=0)
    {
      // Get the track information that belongs to this index:
      TrackID = TheLeafs->veto_segPoint_fTrackID->GetValue(index);
      if (TrackID<0) {TrackID=0;} // prevent seg-faults.
      xpoint = TheLeafs->MCTrack_fStartX->GetValue(TrackID);
      ypoint = TheLeafs->MCTrack_fStartY->GetValue(TrackID);
      zpoint = TheLeafs->MCTrack_fStartZ->GetValue(TrackID);
      
      // Now in order to get a primary particle, we must track it
      // back untill it leaves the primary volume:
      while (PointInsidePrimVolume(Inputs,xpoint,ypoint,zpoint))
      {
	TrackID = TheLeafs->MCTrack_fMotherId->GetValue(TrackID);
        xpoint = TheLeafs->MCTrack_fStartX->GetValue(TrackID);
        ypoint = TheLeafs->MCTrack_fStartY->GetValue(TrackID);
        zpoint = TheLeafs->MCTrack_fStartZ->GetValue(TrackID);
      }
      
      // So now we have the TrackID and position of the particle
      // just before entering the PrimVolume. Hence now it is a
      // primary particle. Hence we now can store the TrackID:
      VETOPrimID[bar] = TrackID;
      
      // Done!
    }
    else
    {
      // In this case there is nothing to track back. Hence
      // we simply store again a -1:
      VETOPrimID[bar] = -1;
    }
  }
  
  // =================================================================================================
  // Now that we have all of our information, we can compare to the energy thresholds and put
  // our information in std::vectors 
  std::vector<Int_t> vec_NeuBarNr;
  std::vector<Double_t> vec_NeuEdep;
  std::vector<Double_t> vec_NeuTOF;
  std::vector<Double_t> vec_NeuTOFRes;
  std::vector<Int_t> vec_NeuPrimID;
  std::vector<Int_t> vec_VETOBarNr;
  std::vector<Double_t> vec_VETOEdep;
  std::vector<Double_t> vec_VETOTOF;
  std::vector<Double_t> vec_VETOTOFRes;
  std::vector<Int_t> vec_VETOIndex;
  std::vector<Int_t> vec_VETOPrimID;
  vec_NeuBarNr.clear();
  vec_NeuEdep.clear();
  vec_NeuTOF.clear();
  vec_NeuTOFRes.clear();
  vec_NeuPrimID.clear();
  vec_VETOBarNr.clear();
  vec_VETOEdep.clear();
  vec_VETOTOF.clear();
  vec_VETOTOFRes.clear();
  vec_VETOIndex.clear();
  vec_VETOPrimID.clear();
  
  // When comparing to energy threshold, notice that the array
  // is in [GeV] and the threshold is in [keV]!
  for (Int_t k = 0; k<NbarsNeu; ++k)
  {
      if ((1e6*EdepNeu[k])>(Inputs->Neu_Ethreshold))
      {
          vec_NeuBarNr.push_back(k+1);
          vec_NeuEdep.push_back(1e3*EdepNeu[k]); // NOTE: conversion to [MeV]!
          vec_NeuTOF.push_back(TOFNeu[k]);
          vec_NeuTOFRes.push_back(TOFNeuRes[k]);
	  vec_NeuPrimID.push_back(NeuPrimID[k]);
      }
  }
  
  for (Int_t k = 0; k<NbarsVETO; ++k)
  {
      if ((1e6*EdepVETO[k])>(Inputs->Veto_Ethreshold))
      {
          vec_VETOBarNr.push_back(k+1);
          vec_VETOEdep.push_back(1e3*EdepVETO[k]); // NOTE: conversion to [MeV]!
          vec_VETOTOF.push_back(TOFVETO[k]);
          vec_VETOTOFRes.push_back(TOFVETORes[k]);
	  vec_VETOPrimID.push_back(VETOPrimID[k]);
          vec_VETOIndex.push_back(VETOIndex[k]);
      }
  }
  
  // Now that everything has been put into std::vectors, we can search for the minimum over
  // all of the bars:
  Int_t Size = vec_NeuBarNr.size();
  Double_t NeuMinTOF = -100.0; // This value will be it when there are no bars that fired!
  Double_t NeuMinTOFRes = -100.0;
  if (Size>0)
  {
    NeuMinTOF = vec_NeuTOF.at(0);
    NeuMinTOFRes = vec_NeuTOFRes.at(0);
    for (Int_t k = 0; k<Size; ++k)
    {
        if ((vec_NeuTOF.at(k))<NeuMinTOF) {NeuMinTOF = vec_NeuTOF.at(k);}
        if ((vec_NeuTOFRes.at(k))<NeuMinTOFRes) {NeuMinTOFRes = vec_NeuTOFRes.at(k);}
    }
  }  
  
  Size = vec_VETOBarNr.size();
  Double_t VETOMinTOF = -100.0;
  Double_t VETOMinTOFRes = -100.0;
  if (Size>0)
  {
    VETOMinTOF = vec_VETOTOF.at(0);
    VETOMinTOFRes = vec_VETOTOFRes.at(0);
    for (Int_t k = 0; k<Size; ++k)
    {
        if ((vec_VETOTOF.at(k))<VETOMinTOF) {VETOMinTOF = vec_VETOTOF.at(k);}
        if ((vec_VETOTOFRes.at(k))<VETOMinTOFRes) {VETOMinTOFRes = vec_VETOTOFRes.at(k);}
    }
  }
  
  // And thatś it. We can now put the content of the std::vectors into the leafs!
  
  // NOTE: Of course: after completing our calculations, these 
  // arrays should be deleted:
  delete EdepNeu;
  delete TOFNeu;
  delete TOFNeuRes;
  delete NeuIndex;
  delete NeuPrimID;
  
  delete EdepVETO;
  delete TOFVETO;
  delete TOFVETORes;
  delete VETOIndex;
  delete VETOPrimID;
  
  // =============================================================================================
  // In principle all of our calculations are done now. What we will do now, is compare the 
  // primary particles that fired a bar with the identified primary particles. We give a bug
  // report if an unidenified particle fired a bar. We can now also state which primary particles
  // are detected and which are not.
  
  // Declare what we need:
  Int_t Nprims = AnaTree->Nprims_NeuLAND;
  Int_t N_NeuBarsFired = vec_NeuBarNr.size();
  Bool_t registered = kFALSE;
  
  // Now we will loop over all primary particles that fired a NeuLAND bar:
  for (Int_t k = 0; k<N_NeuBarsFired; ++k)
  {
      // Check that these primaries were registered in Identify_PrimParticles.h:
      registered = kFALSE;
      
      // Loop over the identified primaries:
      for (Int_t n = 0; n<Nprims; ++n)
      {
          // Then compare there particles based on TrackID:
          if (vec_NeuPrimID.at(k)==AnaTree->Prim_TrackID[n]) 
          {
              // If the particle is identified: set the registration to kTRUE:
              registered = kTRUE;
              
              // The particle has fired at least one NeuLAND bar and was identified.
              // Hence we now set the IsDetected-leafto kTRUE. We could not do that
              // during the identification of primary particles. Hence do it now:
              AnaTree->Prim_Detected[n] = kTRUE;
          }
      }
      
      // Now if it turns out that a particle fired a NeuLAND bar but was NOT
      // identified asprimary, then obviously there is a serious bug in the code.
      // Give output:
      if ((!registered)&&(Inputs->Bug_Report==3))
      {
          cout << "### BUG REPORT: Unidentified PRIMARY particle! | TrackID = " << vec_NeuPrimID.at(k) << " | Event = " << Event << " | PDG = " << TheLeafs->MCTrack_fPdgCode->GetValue(TrackID) << "\n";
      }
  }
  
  // And thatś it. Now we write the information on QDCand TDC to the leafs:
  
  // =============================================================================================
  // Now that the calculations are done, the results have to be put into the Analysis Tree.
  // since some of your leafs might be dynamical arrays, one first has to
  // re-allocate the arrays. Then, a SetBranchAddress should be performed, 
  // because the delete-operations destroys the link to the Analysis Tree. Only after all this,
  // the results of your calculations can be put into the Dynamical arrays. Then this
  // will be filled into the tree automatically. The delete commands that avoid memory leaks
  // should be put into the DeleteEverything function.
  
  // So first define the length of our arrays:
  Int_t ArrayLength_Neu = vec_NeuBarNr.size();
  Int_t ArrayLength_VETO = vec_VETOBarNr.size();
  
  // Then put this into the leaf that holds the size of these dynamical arrays:
  AnaTree->NbarsFired_NeuLAND = ArrayLength_Neu;
  AnaTree->NbarsFired_VETO = ArrayLength_VETO;
  
  // Then re-allocate the arrays:
  AnaTree->Neu_BarNumber_Fired = new Int_t[ArrayLength_Neu];
  AnaTree->Neu_Edep_Fired = new Double_t[ArrayLength_Neu];
  AnaTree->Neu_TOF_Fired = new Double_t[ArrayLength_Neu];
  AnaTree->Neu_TOF_Fired_Res = new Double_t[ArrayLength_Neu];
  AnaTree->Neu_Primary_TrackID = new Int_t[ArrayLength_Neu];
  AnaTree->Neu_Bar_IsVETOed = new Bool_t[ArrayLength_Neu];
  
  AnaTree->VETO_BarNumber_Fired = new Int_t[ArrayLength_VETO];
  AnaTree->VETO_Edep_Fired = new Double_t[ArrayLength_VETO];
  AnaTree->VETO_TOF_Fired = new Double_t[ArrayLength_VETO];
  AnaTree->VETO_TOF_Fired_Res = new Double_t[ArrayLength_VETO];
  AnaTree->VETO_Primary_TrackID = new Int_t[ArrayLength_VETO];
  AnaTree->VETO_TOF_index = new Int_t[ArrayLength_VETO];
  
  // Then re-set the Branch Addresses:
  AnaTree->TheTree->SetBranchAddress("Neu_BarNumber_Fired",(void*) AnaTree->Neu_BarNumber_Fired);
  AnaTree->TheTree->SetBranchAddress("Neu_Edep_Fired",(void*) AnaTree->Neu_Edep_Fired);
  AnaTree->TheTree->SetBranchAddress("Neu_TOF_Fired",(void*) AnaTree->Neu_TOF_Fired);
  AnaTree->TheTree->SetBranchAddress("Neu_TOF_Fired_Res",(void*) AnaTree->Neu_TOF_Fired_Res);
  AnaTree->TheTree->SetBranchAddress("Neu_Primary_TrackID",(void*) AnaTree->Neu_Primary_TrackID);
  AnaTree->TheTree->SetBranchAddress("Neu_Bar_IsVETOed",(void*) AnaTree->Neu_Bar_IsVETOed);
  
  AnaTree->TheTree->SetBranchAddress("VETO_BarNumber_Fired",(void*) AnaTree->VETO_BarNumber_Fired);
  AnaTree->TheTree->SetBranchAddress("VETO_Edep_Fired",(void*) AnaTree->VETO_Edep_Fired);
  AnaTree->TheTree->SetBranchAddress("VETO_TOF_Fired",(void*) AnaTree->VETO_TOF_Fired);
  AnaTree->TheTree->SetBranchAddress("VETO_TOF_Fired_Res",(void*) AnaTree->VETO_TOF_Fired_Res);
  AnaTree->TheTree->SetBranchAddress("VETO_Primary_TrackID",(void*) AnaTree->VETO_Primary_TrackID);
  AnaTree->TheTree->SetBranchAddress("VETO_TOF_index",(void*) AnaTree->VETO_TOF_index);
  
  // Now put the results of your calculations into the arrays you just
  // declared with new:
  if (ArrayLength_Neu>0)
  {
        for (Int_t k = 0; k<ArrayLength_Neu; ++k)
        {
            AnaTree->Neu_BarNumber_Fired[k] = vec_NeuBarNr.at(k);
            AnaTree->Neu_Edep_Fired[k] = vec_NeuEdep.at(k);
            AnaTree->Neu_TOF_Fired[k] = vec_NeuTOF.at(k);
            AnaTree->Neu_TOF_Fired_Res[k] = vec_NeuTOFRes.at(k);
	    AnaTree->Neu_Primary_TrackID[k] = vec_NeuPrimID.at(k);
            AnaTree->Neu_Bar_IsVETOed[k] = kFALSE; // NOTE: This will be filled in later on!
        }
  }
  
  if (ArrayLength_VETO>0)
  {
        for (Int_t k = 0; k<ArrayLength_VETO; ++k)
        {
            AnaTree->VETO_BarNumber_Fired[k] = vec_VETOBarNr.at(k);
            AnaTree->VETO_Edep_Fired[k] = vec_VETOEdep.at(k);
            AnaTree->VETO_TOF_Fired[k] = vec_VETOTOF.at(k);
            AnaTree->VETO_TOF_Fired_Res[k] = vec_VETOTOFRes.at(k);
	    AnaTree->VETO_Primary_TrackID[k] = vec_VETOPrimID.at(k);
            AnaTree->VETO_TOF_index[k] = vec_VETOIndex.at(k);
        }
  }
  
  // Fill the single-valued leafs:
  AnaTree->Neu_TotalTOF = NeuMinTOF;
  AnaTree->Neu_TotalTOF_Res = NeuMinTOFRes;
  AnaTree->VETO_TotalTOF = VETOMinTOF;
  AnaTree->VETO_TotalTOF_Res = VETOMinTOFRes;
  
  // NOTE: The delete commands should be put in the DeleteEverything-function.
  
  // =============================================================================================
  // And now we are done. All variables that we declared here without new are lost
  // after this function is complete, and that is how we like it (memory leaks)! The results are
  // NOT lost, since we put them into the arrays that are part of the AnaTree-structure.
  // here we only work with a pointer to this structure, so the results are put
  // in our structure outside this function. Then outside this function, the
  // command tree->Fill() is given and results are store. You are done now.
}

// ####################################################################################################
// Put in the empty function:

void Empty_QTDC(Leaf_Structure* const TheLeafs, Inputs_Structure* const Inputs, Tree_Structure* AnaTree, Int_t const Event, Int_t const Counter, TRandom3* generator)
{
  Int_t ArrayLength_Neu = 0;
  Int_t ArrayLength_VETO = 0;
  
  AnaTree->NbarsFired_NeuLAND = ArrayLength_Neu;
  AnaTree->NbarsFired_VETO = ArrayLength_VETO;
  
  AnaTree->Neu_BarNumber_Fired = new Int_t[ArrayLength_Neu];
  AnaTree->Neu_Edep_Fired = new Double_t[ArrayLength_Neu];
  AnaTree->Neu_TOF_Fired = new Double_t[ArrayLength_Neu];
  AnaTree->Neu_TOF_Fired_Res = new Double_t[ArrayLength_Neu];
  AnaTree->Neu_Primary_TrackID = new Int_t[ArrayLength_Neu];
  AnaTree->Neu_Bar_IsVETOed = new Bool_t[ArrayLength_Neu];
  
  AnaTree->VETO_BarNumber_Fired = new Int_t[ArrayLength_VETO];
  AnaTree->VETO_Edep_Fired = new Double_t[ArrayLength_VETO];
  AnaTree->VETO_TOF_Fired = new Double_t[ArrayLength_VETO];
  AnaTree->VETO_TOF_Fired_Res = new Double_t[ArrayLength_VETO];
  AnaTree->VETO_Primary_TrackID = new Int_t[ArrayLength_VETO];
  AnaTree->VETO_TOF_index = new Int_t[ArrayLength_VETO];
  
  AnaTree->TheTree->SetBranchAddress("Neu_BarNumber_Fired",(void*) AnaTree->Neu_BarNumber_Fired);
  AnaTree->TheTree->SetBranchAddress("Neu_Edep_Fired",(void*) AnaTree->Neu_Edep_Fired);
  AnaTree->TheTree->SetBranchAddress("Neu_TOF_Fired",(void*) AnaTree->Neu_TOF_Fired);
  AnaTree->TheTree->SetBranchAddress("Neu_TOF_Fired_Res",(void*) AnaTree->Neu_TOF_Fired_Res);
  AnaTree->TheTree->SetBranchAddress("Neu_Primary_TrackID",(void*) AnaTree->Neu_Primary_TrackID);
  AnaTree->TheTree->SetBranchAddress("Neu_Bar_IsVETOed",(void*) AnaTree->Neu_Bar_IsVETOed);
 
  AnaTree->TheTree->SetBranchAddress("VETO_BarNumber_Fired",(void*) AnaTree->VETO_BarNumber_Fired);
  AnaTree->TheTree->SetBranchAddress("VETO_Edep_Fired",(void*) AnaTree->VETO_Edep_Fired);
  AnaTree->TheTree->SetBranchAddress("VETO_TOF_Fired",(void*) AnaTree->VETO_TOF_Fired);
  AnaTree->TheTree->SetBranchAddress("VETO_TOF_Fired_Res",(void*) AnaTree->VETO_TOF_Fired_Res);
  AnaTree->TheTree->SetBranchAddress("VETO_Primary_TrackID",(void*) AnaTree->VETO_Primary_TrackID);
  AnaTree->TheTree->SetBranchAddress("VETO_TOF_index",(void*) AnaTree->VETO_TOF_index);
  
  AnaTree->Neu_TotalTOF = -100.0;
  AnaTree->Neu_TotalTOF_Res = -100.0;
  AnaTree->VETO_TotalTOF = -100.0;
  AnaTree->VETO_TotalTOF_Res = -100.0;
}