void QTDC_Digi(Leaf_Structure* const TheLeafs, Inputs_Structure* const Inputs, Tree_Structure* AnaTree, Int_t const Event, Int_t const Counter, TRandom3* generator)
{
  // This function is to be performed INSTEAD of the ordinary QDTC()-function. From all analysis tasks, The QDTC-task
  // is the only one that requires Digitizer output to work properly. 
  // 1) Identy_PrimParticles() ==> Uses Monte Carlo points to identify the incoming particles + some properties. Obviously this can never be done without Monte Carlo
  //                               points. Hence this task does not need to be overrules/replaced by a digitizer.
  // 2) TrackEvents() ==> Uses Monte Carlo points to track for each event whether it is backscattering or not Obviously this can only be done
  //                      with Monte carlo points. It also uses energy thresholds etc. to compute TrackTOF etc. But without detailed knowledge on Monte Carlo points
  //                      it is also impossible to compute a TrackTOF, since this requires knowledge on the precise pulse shape, something the digitizer does not provide.
  // 3) QTDC() ==> Obviously here a digitizer is useful.
  // 4) Secondary_Particles() ==> Uses Monte Carlo points to establish a picture and a model about the amount of secondary particles and backscattering. This kind of information
  //                              Can again never be obtained form a digitizer, since TDC and QDC cannot count particles, just measure E and t.

  // However, also for the QDTC-function, not all tasks can be performed with the help of the
  // digitizer. Sure, TOF (only with resolution!) and Edep can be obtained from the Digitizer.
  // but the tracking from a bar back to which primary actually fired it is not possible only with digitizer
  // output. It is also very hard to use the ordinary QDTC-function for that because QDTC() and Digitizer
  // may not determine that the same bars actually fired. Also the VETO TOF index cannot be
  // determined from the digitizer. This is however not a big problem, since this leaf is used only
  // to determine the location of a hit. But this can be done with the Digitizer as well. Just in a different way.
  
  // Now the digitizer already did all the work, so we can just take the results and put them
  // into the analysis tree. Therefore no outputs are required. The inputs are the pointers
  // to the 3 structures that hold all our information and the number of the current event.
  // We also pass the TRandom3 generator along as non-=const so that seeds are updated every time. Do not delete it!
  
  // NOTE: AnaTree should NOT be const, otherwise we cannot modify the structure.
  
  // Template is Written by C. A. Douma, for the R3B collaboration
    
  // =============================================================================================
  // Reset all leafs with single-values or static arrays:
  AnaTree->Neu_TotalTOF = -100.0;
  AnaTree->Neu_TotalTOF_Res = -100.0;
  AnaTree->VETO_TotalTOF = -100.0;
  AnaTree->VETO_TotalTOF_Res = -100.0;
  
  // =============================================================================================
  // NOTE: No calculations are performed here. They are simply taken from the digitizer.
  // =============================================================================================
  // Now that the calculations are done, the results have to be put into the Analysis Tree.
  // since some of your leafs might be dynamical arrays, one first has to
  // re-allocate the arrays. Then, a SetBranchAddress should be performed, 
  // because the delete-operations destroys the link to the Analysis Tree. Only after all this,
  // the results of your calculations can be put into the Dynamical arrays. Then this
  // will be filled into the tree automatically. The delete commands that avoid memory leaks
  // should be put into the DeleteEverything function.
  
  // So first define the length of our arrays:
  Int_t ArrayLength_Neu = 0;
  Int_t ArrayLength_VETO = 0;
  
  // It should be zero first and only if there actually is a digitizer, we can adjust:
  if ((TheLeafs->Geant3)&&((Inputs->Detectors_NeuLAND_Exists==1)||(Inputs->Detectors_NeuLAND_Exists==2))&&(Inputs->Detectors_NEBULA_Exists!=1)&&(Inputs->Detectors_NebVETO_Exists!=1)) {ArrayLength_Neu = TheLeafs->LandDigi_fQdc->GetNdata();}
  if ((TheLeafs->Geant3)&&((Inputs->Detectors_VETO_Exists==1)||(Inputs->Detectors_VETO_Exists==2))) {ArrayLength_VETO = TheLeafs->veto_segDigi_fQdc->GetNdata();}
  
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
  
  // We will do the NeuLAND leafs first:
  if (ArrayLength_Neu>0)
  {
      for (Int_t k = 0; k<ArrayLength_Neu; ++k)
      {
          AnaTree->Neu_BarNumber_Fired[k] = 1 + TheLeafs->LandDigi_fPaddleNb->GetValue(k); // NOTE: The LandDigi_fPaddleNb starts at 0, not 1!
          AnaTree->Neu_Edep_Fired[k] = 1.0*TheLeafs->LandDigi_fQdc->GetValue(k); // NOTE: Calibration to [MeV] from Digitizer!
          AnaTree->Neu_TOF_Fired[k] = -100.0; // The digitizer does NOT provide a precise TOF like Monte Carlo points. There's no way to get out this info without Monte Carlo points.
          AnaTree->Neu_TOF_Fired_Res[k] = 1.0*TheLeafs->LandDigi_fTdc->GetValue(k); // NOTE: calibration to [ns] from Digitizer!
          AnaTree->Neu_Bar_IsVETOed[k] = kFALSE; // This is only filled in after the VETO condition is performed, not here!
          AnaTree->Neu_Primary_TrackID[k] = -1; // The Digitizer does not provide data for this. This should actually be obtained from a tracking algorithm!
          // Done!
      }
  }
  
  // Then also do the VETO leafs:
  if (ArrayLength_VETO>0)
  {
      for (Int_t k = 0; k<ArrayLength_VETO; ++k)
      {
          AnaTree->VETO_BarNumber_Fired[k] = 1 + TheLeafs->veto_segDigi_fPaddleNb->GetValue(k); // NOTE: The veto_segDigi_fPaddleNb starts at 0, not 1!
          AnaTree->VETO_Edep_Fired[k] = 1.0*TheLeafs->veto_segDigi_fQdc->GetValue(k); // NOTE: Calibration to [MeV] from Digitizer!
          AnaTree->VETO_TOF_Fired[k] = -100.0; // The digitizer does NOT provide a precise TOF like Monte Carlo points. There's no way to get out this info without Monte Carlo points.
          AnaTree->VETO_TOF_Fired_Res[k] = 1.0*TheLeafs->veto_segDigi_fTdc->GetValue(k); // NOTE: calibration to [ns] from Digitizer!
          AnaTree->VETO_Primary_TrackID[k] = -1; // The Digitizer does not provide data for this. This should actually be obtained from a tracking algorithm!
          AnaTree->VETO_TOF_index[k] = k; // This is only used by the VETO condition to obtain the hit position, but the Digitizer already possesses the information on the hit. Hence we do not need this.
                                          // what we do however need is a way to obtain this information from the digitizer. That is why we need to store this k, so we can acces the same event later.
          // Done!
          
          //cout << "x = " << TheLeafs->veto_segDigi_fxx->GetValue(k) << " | y = " << TheLeafs->veto_segDigi_fyy->GetValue(k) << "\n";
      }
  }
  
  // ================================================================================================
  // In principle we are now done. We only need to obtain minima of all the TOF values
  // for the overall TOF leafs and we need to do some tracking.
  
  // First search for the minimum:
  if (ArrayLength_Neu>0)
  {
      AnaTree->Neu_TotalTOF_Res = 1e99;
      for (Int_t k = 0; k<ArrayLength_Neu; ++k)
      {
          if (AnaTree->Neu_TOF_Fired_Res[k]<AnaTree->Neu_TotalTOF_Res) {AnaTree->Neu_TotalTOF_Res = AnaTree->Neu_TOF_Fired_Res[k];}
      }
  }
  
  // Also for the VETO:
  if (ArrayLength_VETO>0)
  {
      AnaTree->VETO_TotalTOF_Res = 1e99;
      for (Int_t k = 0; k<ArrayLength_VETO; ++k)
      {
          if (AnaTree->VETO_TOF_Fired_Res[k]<AnaTree->VETO_TotalTOF_Res) {AnaTree->VETO_TotalTOF_Res = AnaTree->VETO_TOF_Fired_Res[k];}
      }
  }
  
  // =================================================================================================
  // Then we should do some tracking: We will do this after we obtained the Digitizer output. We will loop over all LandPoints and see
  // for each bar in the digitizer if there are LandPoints in that bar. Then we take the min. TOF from those points and track it back.
  // NOTE: this is of course a calculation. Also note that we only need it as long as we do not have a good tracking algorithm!
  
  // We will first declare what we need:
  Int_t NbarsVETO = (Inputs->VETO_geo_NsinglePlanes)*(Inputs->VETO_geo_NbarsPerPlane);  
  Int_t NbarsNeu = (Inputs->NeuLAND_geo_NsinglePlanes)*50;
  
  Double_t* TOF_Neu = new Double_t[NbarsNeu];
  Int_t* TrackID_Neu = new Int_t[NbarsNeu];
  Double_t* TOF_VETO = new Double_t[NbarsVETO];
  Int_t* TrackID_VETO = new Int_t[NbarsVETO];
  
  Int_t ThisSector = 0;
  Int_t ThisTrackID = 0;
  Double_t ThisTOF = -100.0;
  Double_t xpoint = 0.0;
  Double_t ypoint = 0.0;
  Double_t zpoint = 0.0;  
  
  // Initialize our arrays:
  for (Int_t k = 0; k<NbarsNeu; ++k)
  {
    TOF_Neu[k] = 1e99;
    TrackID_Neu[k] = -1;
  }
  for (Int_t k = 0; k<NbarsVETO; ++k)
  {
    TOF_VETO[k] = 1e99;
    TrackID_VETO[k] = -1;
  }
  
  // Get the size of the LandPoints:
  Int_t LandPointSize = TheLeafs->LandPoint_fLightYield->GetNdata();
  
  // Now loop over the LandPoints:
  if ((LandPointSize>0)&&(ArrayLength_Neu>0))
  {
    for (Int_t kNeu = 0; kNeu<LandPointSize; ++kNeu)
    {
      ThisTOF = TheLeafs->LandPoint_fTime->GetValue(kNeu);
      ThisSector = TheLeafs->LandPoint_fSector->GetValue(kNeu);
      ThisTrackID = TheLeafs->LandPoint_fTrackID->GetValue(kNeu);
      
      // NEBULA-check:
      if (ThisSector<(NbarsNeu+1))
      {
	// Now compare for TOF:
	if (ThisTOF<TOF_Neu[ThisSector-1])
	{
	  TOF_Neu[ThisSector-1] = ThisTOF;
	  TrackID_Neu[ThisSector-1] = ThisTrackID;
	}
      }
    }

    // Now track back the TrackID:
    for (Int_t kBar = 0; kBar<NbarsNeu; ++kBar)
    {
      // request trackID:
      ThisTrackID = TrackID_Neu[kBar];
      
      if (!(ThisTrackID<0))
      {
	
	xpoint = TheLeafs->MCTrack_fStartX->GetValue(ThisTrackID);
	ypoint = TheLeafs->MCTrack_fStartY->GetValue(ThisTrackID);
	zpoint = TheLeafs->MCTrack_fStartZ->GetValue(ThisTrackID);
	
	// Now track it back:
	while (PointInsidePrimVolume(Inputs,xpoint,ypoint,zpoint))
	{
	  ThisTrackID = TheLeafs->MCTrack_fMotherId->GetValue(ThisTrackID);
	  xpoint = TheLeafs->MCTrack_fStartX->GetValue(ThisTrackID);
	  ypoint = TheLeafs->MCTrack_fStartY->GetValue(ThisTrackID);
	  zpoint = TheLeafs->MCTrack_fStartZ->GetValue(ThisTrackID);
	}
	
	// Now store it back into the array:
	TrackID_Neu[kBar] = ThisTrackID;
      }
    }

    // after tracking it back, we should store the TrackID inside the AnaTree:
    for (Int_t k = 0; k<ArrayLength_Neu; ++k)
    {
      ThisSector = AnaTree->Neu_BarNumber_Fired[k];
      AnaTree->Neu_Primary_TrackID[k] = TrackID_Neu[ThisSector-1];
      if ((Inputs->Bug_Report==4)&&(TrackID_Neu[ThisSector-1]<0)) {cout << "### BUG REPORT: A NeuLAND bar could not be tracked back from the digitizer! Event = " << Event << " | BarNr = " << ThisSector << "\n";}
    }
      
    // That's it! now all the NeuLAND TrackID information should be there!  
  }
  
  // Now we will do exactly the same for the VETO:
  Int_t VetoPointSize = TheLeafs->veto_segPoint_fLightYield->GetNdata();
  
  // Now loop over the LandPoints:
  if ((VetoPointSize>0)&&(ArrayLength_VETO>0))
  {
    for (Int_t kveto = 0; kveto<VetoPointSize; ++kveto)
    {
      ThisTOF = TheLeafs->veto_segPoint_fTime->GetValue(kveto);
      ThisSector = TheLeafs->veto_segPoint_fSector->GetValue(kveto);
      ThisTrackID = TheLeafs->veto_segPoint_fTrackID->GetValue(kveto);
      
      // NEBULA-check:
      if (ThisSector<(NbarsVETO+1))
      {
	// Now compare for TOF:
	if (ThisTOF<TOF_VETO[ThisSector-1])
	{
	  TOF_VETO[ThisSector-1] = ThisTOF;
	  TrackID_VETO[ThisSector-1] = ThisTrackID;
	}
      }
    }

    // Now track back the TrackID:
    for (Int_t kBar = 0; kBar<NbarsVETO; ++kBar)
    {
      // request trackID:
      ThisTrackID = TrackID_VETO[kBar];
      
      if (!(ThisTrackID<0))
      {
	
	xpoint = TheLeafs->MCTrack_fStartX->GetValue(ThisTrackID);
	ypoint = TheLeafs->MCTrack_fStartY->GetValue(ThisTrackID);
	zpoint = TheLeafs->MCTrack_fStartZ->GetValue(ThisTrackID);
	
	// Now track it back:
	while (PointInsidePrimVolume(Inputs,xpoint,ypoint,zpoint))
	{
	  ThisTrackID = TheLeafs->MCTrack_fMotherId->GetValue(ThisTrackID);
	  xpoint = TheLeafs->MCTrack_fStartX->GetValue(ThisTrackID);
	  ypoint = TheLeafs->MCTrack_fStartY->GetValue(ThisTrackID);
	  zpoint = TheLeafs->MCTrack_fStartZ->GetValue(ThisTrackID);
	}
	
	// Now store it back into the array:
	TrackID_VETO[kBar] = ThisTrackID;
      }
    }

    // after tracking it back, we should store the TrackID inside the AnaTree:
    for (Int_t k = 0; k<ArrayLength_VETO; ++k)
    {
      ThisSector = AnaTree->VETO_BarNumber_Fired[k];
      AnaTree->VETO_Primary_TrackID[k] = TrackID_VETO[ThisSector-1];
      if ((Inputs->Bug_Report==3)&&(TrackID_VETO[ThisSector-1]<0)) {cout << "### BUG REPORT: A VETO bar could not be tracked back from the digitizer! Event = " << Event << " | BarNr = " << ThisSector << "\n";}
    }
      
    // That's it! now all the NeuLAND TrackID information should be there!  
  }
  
  // delete our auxillary arrays:
  delete TOF_Neu;
  delete TrackID_Neu;
  delete TOF_VETO;
  delete TrackID_VETO;
  
  // Now we should define which primary particles are detected and which are not.
  
  // Declare what we need:
  Int_t Nprims = AnaTree->Nprims_NeuLAND;
  Int_t N_NeuBarsFired = ArrayLength_Neu;
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
          if (AnaTree->Neu_Primary_TrackID[k]==AnaTree->Prim_TrackID[n]) 
          {
              // If the particle is identified: set the registration to kTRUE:
              registered = kTRUE;
              
              // The particle has fired at least one NeuLAND bar and was identified.
              // Hence we now set the IsDetected-leaf to kTRUE. We could not do that
              // during the identification of primary particles. Hence do it now:
              AnaTree->Prim_Detected[n] = kTRUE;
          }
      }
      
      // Now if it turns out that a particle fired a NeuLAND bar but was NOT
      // identified as primary, then obviously there is a serious bug in the code.
      // Give output:
      if ((!registered)&&(Inputs->Bug_Report==3))
      {
          cout << "### BUG REPORT: Unidentified PRIMARY particle! | TrackID = " << AnaTree->Neu_Primary_TrackID[k] << " | Event = " << Event << " | PDG = " << TheLeafs->MCTrack_fPdgCode->GetValue(AnaTree->Neu_Primary_TrackID[k]) << "\n";
      }
  }
  
  // NOTE: The delete commands for the leafs should be put in the DeleteEverything-function.
  
  // =============================================================================================
  // And now we are done. All variables that we declared here without new are lost
  // after this function is complete, and that is how we like it (memory leaks)! The results are
  // NOT lost, since we put them into the arrays that are part of the AnaTree-structure.
  // here we only work with a pointer to this structure, so the results are put
  // in our structure outside this function. Then outside this function, the
  // command tree->Fill() is given and results are store. You are done now.
}