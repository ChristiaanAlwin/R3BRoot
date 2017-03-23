void Trigger(Leaf_Structure* const TheLeafs, Inputs_Structure* const Inputs, Tree_Structure* AnaTree, Int_t const Event, Int_t const Counter, TRandom3* generator)
{
  // This function calculates whether the trigger of the full setup fired. The trigger is what Igor told me
  // for the reaction F29(beam) + H(target) --> H(recoil) + F28 + n
  // We trigger on the condition that there is a beam particle, the TOF wall fired and NeuLAND
  // has at least 2 PMTs that fired. The trigger output is a simple boolian in the AnalysisTree.
  // In addition, also the average hit position of all TOF MC points is calculated (horizonatl, along TOF plane).
  // This position is also stored.
  
  // Results are put into the analysis tree. Therefore no outputs are required. The inputs are the pointers
  // to the 3 structures that hold all our information and the number of the current event.
  // We also pass the TRandom3 generator along as non-=const so that seeds are updated every time. Do not delete it!
  
  // NOTE: AnaTree should NOT be const, otherwise we cannot modify the structure.
  
  // Written by C. A. Douma, for the R3B collaboration
    
  // =============================================================================================
  // Before starting your calculations, the values of the variables holding
  // information that will be put into the tree should be reset. This should be
  // done for single valued leafs and for static arrays:
  
  AnaTree->Full_Trigger = kFALSE;
  AnaTree->TOF_HorizontalHit = -1000.0; // to easily identify!
    
  // =============================================================================================
  // NOTE: Do your calculations here. 
  
  // Define enery thresholds:
  Double_t Ethres_PSP = 1.0; // [MeV]
  Double_t Ethres_TOF = 1.0; // [MeV]
  Int_t Mult_L = 1;
  Int_t Mult_R = 1;
  
  // At first we need to check if the detectors actually exist:
  if ((TheLeafs->TOF_exists==kTRUE)&&(TheLeafs->PSP_exists==kTRUE)&&(TheLeafs->NeuLAND_exists==kTRUE))
  {
      // Now we know that both detectors are actually there, so we can proceed:
      
      // Begin by looping over PSP and collect the total energy deposition per detector:
      Double_t E_psp1 = 0.0;
      Double_t E_psp2 = 0.0;
      Double_t E_psp3 = 0.0;
      
      Int_t PSP_Size = TheLeafs->PSPPoint_fELoss->GetNdata();
      Int_t PSP_Module = 0;
      
      if (PSP_Size>0)
      {
        for (Int_t k = 0; k<PSP_Size; ++k)
        {
          PSP_Module = TheLeafs->PSPPoint_fModule->GetValue(k);
          
          if (PSP_Module==1) {E_psp1 = E_psp1 + TheLeafs->PSPPoint_fELoss->GetValue(k);}
          if (PSP_Module==2) {E_psp2 = E_psp2 + TheLeafs->PSPPoint_fELoss->GetValue(k);}
          if (PSP_Module==3) {E_psp3 = E_psp3 + TheLeafs->PSPPoint_fELoss->GetValue(k);}
        }
      }
      
      // convert to MeV:
      E_psp1 = E_psp1*1e3;
      E_psp2 = E_psp2*1e3;
      E_psp3 = E_psp3*1e3;
      
      // Collect total energy deposition in TOF. Also collect the TOF hitting position:
      TVector3 pos;
      TVector3 vec;
      Double_t E_TOF = 0.0;
      Double_t TOF_pos = 0.0;
      
      Int_t TOF_Size = TheLeafs->TOFPoint_fELoss->GetNdata();
      
      if (TOF_Size>0)
      {
        for (Int_t k = 0; k<TOF_Size; ++k)
        {
          // Collect total energy:
          E_TOF = E_TOF + TheLeafs->TOFPoint_fELoss->GetValue(k);
          
          // Load the hit position into a pos:
          pos.SetX(TheLeafs->TOFPoint_fX->GetValue(k));
          pos.SetY(TheLeafs->TOFPoint_fY->GetValue(k));
          pos.SetZ(TheLeafs->TOFPoint_fZ->GetValue(k));
          
          // Load the track direction into a vec:
          vec.SetX(TheLeafs->TOFPoint_fPx->GetValue(k));
          vec.SetY(TheLeafs->TOFPoint_fPy->GetValue(k));
          vec.SetZ(TheLeafs->TOFPoint_fPz->GetValue(k));
          
          // Take the Theta-Angle of this track in case there
          // is only a single TOF-point and add it to the average:
          if (TOF_Size==1)
          {
              AnaTree->n_BendAngle = AnaTree->n_BendAngle + 1;
              AnaTree->BendAngle = AnaTree->BendAngle + vec.Theta();
          }
          
          // translate back:
          pos.SetZ(pos.Z() - Inputs->TOF_geo_Zpos);
          pos.SetY(pos.Y() - Inputs->TOF_geo_Ypos);
          pos.SetX(pos.X() - Inputs->TOF_geo_Xpos);
          
          // Rotate back:
          pos.RotateZ(-1.0*Inputs->TOF_geo_RotZ);
          pos.RotateY(-1.0*Inputs->TOF_geo_RotY);
          pos.RotateX(-1.0*Inputs->TOF_geo_RotX);
      
          // Now we can extract the hit position and compute our average:
          TOF_pos = TOF_pos + pos.X();
        }
      
        // Finish computation:
        TOF_pos = TOF_pos/((Int_t) TOF_Size);
        
        if (Inputs->Bug_Report==8) {cout << "### BUG REPORT: Average TOF hit position [cm] = " << TOF_pos << "\n";}
        
        // convert to MeV:
        E_TOF = E_TOF*1e3;
      }
      
      // Collect the number of PMTs in NeuLAND that has fired:
      Int_t n_PMT_L = TheLeafs->LandDigi_fQdcL->GetNdata();
      Int_t n_PMT_R = TheLeafs->LandDigi_fQdcR->GetNdata();
      
      // Now compose the trigger:
      Bool_t Trigger = kFALSE;
      
      if ((n_PMT_L>=Mult_L)&&(n_PMT_R>=Mult_R)) // NeuLAND at least 2 PMTs have fired (possibly in different bars)
      {
          if (E_TOF>Ethres_TOF) // TOF has fired. No position cut.
          {
              if ((E_psp1>Ethres_PSP)&&(E_psp2>Ethres_PSP)) // Both PSP1 and PSP2 have fired. to exclude random coincidences.
              {
                  Trigger = kTRUE;
              }
          }
      }
      
      // re-tune the TOF position:
      if (TOF_Size==0) {TOF_pos = -1000.0;}
      
      // That's it. Now write the values to the tree:
      AnaTree->Full_Trigger = Trigger;
      AnaTree->TOF_HorizontalHit = TOF_pos;
      
      // Done! Close the loops:
  }     
    
  // =============================================================================================
  // Now that the calculations are done, the results have to be put into the Analysis Tree.
  // This is easy, since we do not write arrays to the tree, only single numbers. Hence no
  // re-allocation, etc. is required here.
  
  // =============================================================================================
  // No deletes are required either.
}

void NoTrigger(Leaf_Structure* const TheLeafs, Inputs_Structure* const Inputs, Tree_Structure* AnaTree, Int_t const Event, Int_t const Counter, TRandom3* generator)
{
    AnaTree->Full_Trigger = kFALSE;
    AnaTree->TOF_HorizontalHit = -1000.0; // to easily identify!
}