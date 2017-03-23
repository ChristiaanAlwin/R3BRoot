void QTDC(Leaf_Structure* const TheLeafs, Inputs_Structure* const Inputs, Tree_Structure* AnaTree, Int_t const Event, Int_t const Counter, TRandom3* generator)
{
  // This function Gets the total energy deposition and the TOF per bar. This is performed
  // for both the VETO and for NeuLAND. For the VETO a static array is chosen to save the
  // data due to simplicity and a limited number of bars. For NeuLAND, the data is saved
  // as a dynamical array: only bars that are fired are stored.
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
  
  Double_t* EdepVETO = new Double_t[NbarsVETO];
  Double_t* TOFVETO = new Double_t[NbarsVETO];
  Double_t* TOFVETORes = new Double_t[NbarsVETO];
  
  // Then fill the arrays with zeros:
  for (Int_t k = 0; k<NbarsNeu; ++k)
  {
      EdepNeu[k] = 0.0;
      TOFNeu[k] = 0.0;
      TOFNeuRes[k] = 0.0;
  }
  
  for (Int_t k = 0; k<NbarsVETO; ++k)
  {
      EdepVETO[k] = 0.0;
      TOFVETO[k] = 0.0;
      TOFNeuRes[k] = 0.0;
  }
  
  // Now declare arr variables that we need:
  Double_t light = 0.0;
  Double_t TotalLight = 0.0;
  Double_t TOF = 0.0;
  Double_t TotalTOF = 0.0;
  Int_t Sector = 0;
  
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
          
          // Add all lightYields:
          TotalLight = EdepNeu[Sector-1];
          TotalLight = TotalLight + light;
          EdepNeu[Sector-1] = TotalLight;
          
          // search for the maximum:
          TotalTOF = TOFNeu[Sector-1];
          if (TotalTOF<TOF) {TotalTOF = TOF;}
          TOFNeu[Sector-1] = TotalTOF;
      }
      
      // Now search for the minimal TOF:
      for (Int_t kNeu = 0; kNeu<NPointsNeu; ++kNeu)
      {
          TOF = TheLeafs->LandPoint_fTime->GetValue(kNeu);
          Sector = TheLeafs->LandPoint_fSector->GetValue(kNeu);
          
          // search for the minimum:
          TotalTOF = TOFNeu[Sector-1];
          if (TotalTOF>TOF) {TotalTOF = TOF;}
          TOFNeu[Sector-1] = TotalTOF;
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
          
          // Add all lightYields:
          TotalLight = EdepVETO[Sector-1];
          TotalLight = TotalLight + light;
          EdepVETO[Sector-1] = TotalLight;
          
          // search for the maximum:
          TotalTOF = TOFVETO[Sector-1];
          if (TotalTOF<TOF) {TotalTOF = TOF;}
          TOFVETO[Sector-1] = TotalTOF;
      }
      
      // Now search for the minimal TOF:
      for (Int_t kveto = 0; kveto<NPointsVETO; ++kveto)
      {
          TOF = TheLeafs->veto_segPoint_fTime->GetValue(kveto);
          Sector = TheLeafs->veto_segPoint_fSector->GetValue(kveto);
          
          // search for the minimum:
          TotalTOF = TOFVETO[Sector-1];
          if (TotalTOF>TOF) {TotalTOF = TOF;}
          TOFVETO[Sector-1] = TotalTOF;
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
  
  // Now that we have all of our information, we can compare to the energy thresholds and put
  // our information in std::vectors 
  std::vector<Int_t> vec_NeuBarNr;
  std::vector<Double_t> vec_NeuEdep;
  std::vector<Double_t> vec_NeuTOF;
  std::vector<Double_t> vec_NeuTOFRes;
  std::vector<Int_t> vec_VETOBarNr;
  std::vector<Double_t> vec_VETOEdep;
  std::vector<Double_t> vec_VETOTOF;
  std::vector<Double_t> vec_VETOTOFRes;
  vec_NeuBarNr.clear();
  vec_NeuEdep.clear();
  vec_NeuTOF.clear();
  vec_NeuTOFRes.clear();
  vec_VETOBarNr.clear();
  vec_VETOEdep.clear();
  vec_VETOTOF.clear();
  vec_VETOTOFRes.clear();
  
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
  delete EdepVETO;
  delete TOFVETO;
  delete TOFVETORes;
  
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
  
  AnaTree->VETO_BarNumber_Fired = new Int_t[ArrayLength_VETO];
  AnaTree->VETO_Edep_Fired = new Double_t[ArrayLength_VETO];
  AnaTree->VETO_TOF_Fired = new Double_t[ArrayLength_VETO];
  AnaTree->VETO_TOF_Fired_Res = new Double_t[ArrayLength_VETO];
  
  // Then re-set the Branch Addresses:
  AnaTree->TheTree->SetBranchAddress("Neu_BarNumber_Fired",(void*) AnaTree->Neu_BarNumber_Fired);
  AnaTree->TheTree->SetBranchAddress("Neu_Edep_Fired",(void*) AnaTree->Neu_Edep_Fired);
  AnaTree->TheTree->SetBranchAddress("Neu_TOF_Fired",(void*) AnaTree->Neu_TOF_Fired);
  AnaTree->TheTree->SetBranchAddress("Neu_TOF_Fired_Res",(void*) AnaTree->Neu_TOF_Fired_Res);
  
  AnaTree->TheTree->SetBranchAddress("VETO_BarNumber_Fired",(void*) AnaTree->VETO_BarNumber_Fired);
  AnaTree->TheTree->SetBranchAddress("VETO_Edep_Fired",(void*) AnaTree->VETO_Edep_Fired);
  AnaTree->TheTree->SetBranchAddress("VETO_TOF_Fired",(void*) AnaTree->VETO_TOF_Fired);
  AnaTree->TheTree->SetBranchAddress("VETO_TOF_Fired_Res",(void*) AnaTree->VETO_TOF_Fired_Res);
  
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
  
  AnaTree->VETO_BarNumber_Fired = new Int_t[ArrayLength_VETO];
  AnaTree->VETO_Edep_Fired = new Double_t[ArrayLength_VETO];
  AnaTree->VETO_TOF_Fired = new Double_t[ArrayLength_VETO];
  AnaTree->VETO_TOF_Fired_Res = new Double_t[ArrayLength_VETO];
  
  AnaTree->TheTree->SetBranchAddress("Neu_BarNumber_Fired",(void*) AnaTree->Neu_BarNumber_Fired);
  AnaTree->TheTree->SetBranchAddress("Neu_Edep_Fired",(void*) AnaTree->Neu_Edep_Fired);
  AnaTree->TheTree->SetBranchAddress("Neu_TOF_Fired",(void*) AnaTree->Neu_TOF_Fired);
  AnaTree->TheTree->SetBranchAddress("Neu_TOF_Fired_Res",(void*) AnaTree->Neu_TOF_Fired_Res);
  
  AnaTree->TheTree->SetBranchAddress("VETO_BarNumber_Fired",(void*) AnaTree->VETO_BarNumber_Fired);
  AnaTree->TheTree->SetBranchAddress("VETO_Edep_Fired",(void*) AnaTree->VETO_Edep_Fired);
  AnaTree->TheTree->SetBranchAddress("VETO_TOF_Fired",(void*) AnaTree->VETO_TOF_Fired);
  AnaTree->TheTree->SetBranchAddress("VETO_TOF_Fired_Res",(void*) AnaTree->VETO_TOF_Fired_Res);
  
  AnaTree->Neu_TotalTOF = -100.0;
  AnaTree->Neu_TotalTOF_Res = -100.0;
  AnaTree->VETO_TotalTOF = -100.0;
  AnaTree->VETO_TotalTOF_Res = -100.0;
}