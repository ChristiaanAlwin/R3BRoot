void Identify_PrimParticles(Leaf_Structure* TheLeafs, Inputs_Structure* Inputs, Tree_Structure* AnaTree, Int_t const Event, Int_t const Counter, TRandom3* generator)
{
  // This function identifies the primary particles within a single event and puts the results
  // into the analysis tree. Therefore no outputs are required. The inputs are the pointers
  // to the 3 structures that hold all our information and the number of the current event.
  // We also pass the TRandom3 generator along as non-=const so that seeds are updated every time.
  
  // NOTE: AnaTree should NOT be const, otherwise we cannot modify the structure.
  
  // Written by C. A. Douma, for the R3B collaboration
  
  // =============================================================================================
  // First we do our calculations. In order to identify the primary particles,
  // we will loop over all Monte Carlo points. We will then track each point back 1 step.
  // Then if the point (daughter) goes from inside the primary volume to outside (parent), 
  // we know that the parent is a primary particle. If the daughter is outside this volume,
  // we test if the track goes through the volume. In that case the parent is also a primary particle.
  // else it is not a primary particle.
    
  // FIXME 2 problems with this code:
  // 1. Sometimes 0 primary particles are registered when 1 tries to duplicate results from previous simulations
  //    This is because for these events, the primary track does not change track number, while it DOES enter
  //    the volume that identifies primary particles! Hence this code thinks the track never entered.
  // 2. Sometimes too many primary particles are registered. This is because a particle can loop back
  //    through the air and re-enter the primary volume. Then this code thinks it is a new particle, while
  //    it actually is not! This is not a problem, just an artifact of this code.
    
  // --------------------
  // We loop over the Monte Carlo points. First declare the variables to store
  // our information:
    
  // The size of this event:  
  Int_t MC_ArraySize = TheLeafs->MCTrack_fMass->GetNdata();
  
  // The std vectors that we use to store the primary particles in:
  std::vector<Int_t> Prim_TrackID_vec;
  std::vector<Int_t> Prim_PDG_vec;
  std::vector<Double_t> Prim_E_vec;
  std::vector<Double_t> Prim_MCvec_T;
  std::vector<Double_t> Prim_MCvec_X;
  std::vector<Double_t> Prim_MCvec_Y;
  std::vector<Double_t> Prim_MCvec_Z;
  std::vector<Double_t> Prim_MCpos_T;
  std::vector<Double_t> Prim_MCpos_X;
  std::vector<Double_t> Prim_MCpos_Y;
  std::vector<Double_t> Prim_MCpos_Z;
  std::vector<Bool_t> From_Target;
  Prim_TrackID_vec.clear();
  Prim_PDG_vec.clear();
  Prim_E_vec.clear();
  Prim_MCvec_T.clear();
  Prim_MCvec_X.clear();
  Prim_MCvec_Y.clear();
  Prim_MCvec_Z.clear();
  Prim_MCpos_T.clear();
  Prim_MCpos_X.clear();
  Prim_MCpos_Y.clear();
  Prim_MCpos_Z.clear();
  From_Target.clear();
    
  // The information we need about the Monte Carlo points:
  Double_t Tdaughter = 0.0;
  Double_t Xdaughter = 0.0;
  Double_t Ydaughter = 0.0;
  Double_t Zdaughter = 0.0;
  Double_t pXdaughter = 0.0;
  Double_t pYdaughter = 0.0;
  Double_t pZdaughter = 0.0;
  Double_t Massdaughter = 0.0;
  Double_t Edaughter = 0.0;
  Int_t TrackID_daughter = 0;
  Int_t PDGdaughter = 0;
  
  Double_t Tparent = 0.0;
  Double_t Xparent = 0.0;
  Double_t Yparent = 0.0;
  Double_t Zparent = 0.0;
  Double_t pXparent = 0.0;
  Double_t pYparent = 0.0;
  Double_t pZparent = 0.0;
  Double_t Massparent = 0.0;
  Double_t Eparent = 0.0;
  Int_t TrackID_parent = 0;
  Int_t PDGparent = 0;
  
  // The boolians we need to help us decide:
  Bool_t DaughterInside = kFALSE;
  Bool_t ParentInside = kFALSE;
  Bool_t TrackInside = kFALSE;
  Bool_t RegisteredBefore = kFALSE;
  Bool_t Particle_IsPrimary = kFALSE;
  
  // other variables we might need:
  Int_t vecsize = 0;
  Double_t target_radius = 0.0;
  
  for (Int_t k = 0; k<MC_ArraySize; ++k)
  {
      // Get all information about the daughter:
      TrackID_daughter = k;
      PDGdaughter = TheLeafs->MCTrack_fPdgCode->GetValue(k);
      Tdaughter = TheLeafs->MCTrack_fStartT->GetValue(k);
      Xdaughter = TheLeafs->MCTrack_fStartX->GetValue(k);
      Ydaughter = TheLeafs->MCTrack_fStartY->GetValue(k);
      Zdaughter = TheLeafs->MCTrack_fStartZ->GetValue(k);
      pXdaughter = TheLeafs->MCTrack_fPx->GetValue(k);
      pYdaughter = TheLeafs->MCTrack_fPy->GetValue(k);
      pZdaughter = TheLeafs->MCTrack_fPz->GetValue(k);
      Massdaughter = TheLeafs->MCTrack_fMass->GetValue(k);
      Edaughter = TMath::Sqrt(pXdaughter*pXdaughter + pYdaughter*pYdaughter + pZdaughter*pZdaughter + Massdaughter*Massdaughter) - Massdaughter;
      
      // then track back:
      TrackID_parent = TheLeafs->MCTrack_fMotherId->GetValue(k);
      
      // Now check if the parent particle actually exists:
      if (TrackID_parent>(-1))
      {
          // it exists, so get its information:
          PDGparent = TheLeafs->MCTrack_fPdgCode->GetValue(TrackID_parent);
          Tparent = TheLeafs->MCTrack_fStartT->GetValue(TrackID_parent);
          Xparent = TheLeafs->MCTrack_fStartX->GetValue(TrackID_parent);
          Yparent = TheLeafs->MCTrack_fStartY->GetValue(TrackID_parent);
          Zparent = TheLeafs->MCTrack_fStartZ->GetValue(TrackID_parent);
          pXparent = TheLeafs->MCTrack_fPx->GetValue(TrackID_parent);
          pYparent = TheLeafs->MCTrack_fPy->GetValue(TrackID_parent);
          pZparent = TheLeafs->MCTrack_fPz->GetValue(TrackID_parent);
          Massparent = TheLeafs->MCTrack_fMass->GetValue(TrackID_parent);
          Eparent = TMath::Sqrt(pXparent*pXparent + pYparent*pYparent + pZparent*pZparent + Massparent*Massparent) - Massparent;
      }
      else
      {
          // it does not exist, so equate the parent to the daughter
          TrackID_parent = TrackID_daughter;
          PDGparent = PDGdaughter;
          Tparent = Tdaughter;
          Xparent = Xdaughter;
          Yparent = Ydaughter;
          Zparent = Zdaughter;
          pXparent = pXdaughter;
          pYparent = pYdaughter;
          pZparent = pZdaughter;
          Massparent = Massdaughter;
          Eparent = Edaughter;
      }
      
      // So now we have all of our information, we can test for primary particles.
      // first get the appropriate values of our boolians:
      DaughterInside = PointInsidePrimVolume(Inputs,Xdaughter,Ydaughter,Zdaughter);
      ParentInside = PointInsidePrimVolume(Inputs,Xparent,Yparent,Zparent);
      TrackInside = kFALSE;
      
      if ((!DaughterInside)&&(!ParentInside))
      {
          TrackInside = TrackThroughPrimVolume(Inputs,Xparent,Yparent,Zparent,Xdaughter,Ydaughter,Zdaughter);
      }
      
      // And now we make the decission whether the particle is primary:
      Particle_IsPrimary = kFALSE;
      if ((DaughterInside)&&(!ParentInside)) {Particle_IsPrimary = kTRUE;}
      if (TrackInside) {Particle_IsPrimary = kTRUE;}
      
      // Now this is all very good, but we might add the same
      // primary particle way too often. Thatś what we will check for now:
      RegisteredBefore = kFALSE;
      
      vecsize = Prim_TrackID_vec.size();
      
      if (vecsize>0) // otherwise there are no particles yet, so also none registered before!
      {
            for (Int_t n = 0; n<vecsize; ++n)
            {
                if (TrackID_parent==(Prim_TrackID_vec.at(n)))
                {
                    RegisteredBefore = kTRUE;
                    
                    // But now if this is the case, we might need to replace the primary interaction point:
                    if (Tdaughter<Prim_MCvec_T.at(n))
                    {
                        Prim_MCvec_T.at(n) = Tdaughter;
                        Prim_MCvec_X.at(n) = Xdaughter;
                        Prim_MCvec_Y.at(n) = Ydaughter;
                        Prim_MCvec_Z.at(n) = Zdaughter;
                    }
                    
                    // same for the production point:
                    if (Tparent<Prim_MCpos_T.at(n))
                    {
                        Prim_MCpos_T.at(n) = Tparent;
                        Prim_MCpos_X.at(n) = Xparent;
                        Prim_MCpos_Y.at(n) = Yparent;
                        Prim_MCpos_Z.at(n) = Zparent;
                    }
                }
            }
      }
      
      // And now we fill our std::vector for the leafs:
      if ((Particle_IsPrimary)&&(!RegisteredBefore))
      {
        // NOTE: We do not cut on PrimTcut_low since a particle with the first MC point below the cut can still be detected!
        //if (((Inputs->Monte_Carlo_Engine=="TGeant4")&&(Tdaughter<Inputs->PrimTcut_up*1e-9))||(((Inputs->Monte_Carlo_Engine=="TGeant3"))&&(Tdaughter<Inputs->PrimTcut_up)))
        //{
          Prim_TrackID_vec.push_back(TrackID_parent);
          Prim_PDG_vec.push_back(PDGparent);
          Prim_E_vec.push_back(Eparent);
          
          // We also keep track of the first interaction point of this primary particle:
          Prim_MCvec_T.push_back(Tdaughter);
          Prim_MCvec_X.push_back(Xdaughter);
          Prim_MCvec_Y.push_back(Ydaughter);
          Prim_MCvec_Z.push_back(Zdaughter);
          
          // same for the production point:
          Prim_MCpos_T.push_back(Tparent);
          Prim_MCpos_X.push_back(Xparent);
          Prim_MCpos_Y.push_back(Yparent);
          Prim_MCpos_Z.push_back(Zparent);
          
          // check if the particle is from the target:
          target_radius = 0.0;
          target_radius = target_radius + (Xparent-Inputs->NeuBegin_X)*(Xparent-Inputs->NeuBegin_X);
          target_radius = target_radius + (Yparent-Inputs->NeuBegin_Y)*(Yparent-Inputs->NeuBegin_Y);
          target_radius = target_radius + (Zparent-Inputs->NeuBegin_Z)*(Zparent-Inputs->NeuBegin_Z);
          target_radius = TMath::Sqrt(target_radius);
          
          if (target_radius<(Inputs->AllowedTargetRadius))
          {
              From_Target.push_back(kTRUE);
          }
          else
          {
              From_Target.push_back(kFALSE);
          }
          
        //}
      }

  }
  
  // After this, we will also check for the LandPoints. It might be the case that a particle comes from
  // outside primary volume and performs interactions inside NeuLAND without changing TrackID ever.
  // in this case there are interactions inside NeuLAND, but no primary particle is registered, 
  // because no Monte Carlo Points are available inside the primary volume. So, we will do:
  
  Int_t Land_ArraySize = TheLeafs->LandPoint_fTrackID->GetNdata();
  
  if (Land_ArraySize>0)
  {
    for (Int_t k = 0; k<Land_ArraySize; ++k)
    {
          // we will simply check for each landpoint which monte carlo point
          // it belongs to. If this point is outside the primary volume and it has not been 
          // registered before, we will add it. Note that if the MC_point the landpoint
          // belongs to is inside the primary volume, the previous code will have already found it for sure!
          
          // Get the Monte Carlo point:     
          TrackID_parent = TheLeafs->LandPoint_fTrackID->GetValue(k);
          PDGparent = TheLeafs->MCTrack_fPdgCode->GetValue(TrackID_parent);
          Tparent = TheLeafs->MCTrack_fStartT->GetValue(TrackID_parent);
          Xparent = TheLeafs->MCTrack_fStartX->GetValue(TrackID_parent);
          Yparent = TheLeafs->MCTrack_fStartY->GetValue(TrackID_parent);
          Zparent = TheLeafs->MCTrack_fStartZ->GetValue(TrackID_parent);
          pXparent = TheLeafs->MCTrack_fPx->GetValue(TrackID_parent);
          pYparent = TheLeafs->MCTrack_fPy->GetValue(TrackID_parent);
          pZparent = TheLeafs->MCTrack_fPz->GetValue(TrackID_parent);
          Massparent = TheLeafs->MCTrack_fMass->GetValue(TrackID_parent);
          Eparent = TMath::Sqrt(pXparent*pXparent + pYparent*pYparent + pZparent*pZparent + Massparent*Massparent) - Massparent;
          
          // Get the primary interaction point:
          Tdaughter = TheLeafs->LandPoint_fTime->GetValue(k);
          Xdaughter = TheLeafs->LandPoint_fX->GetValue(k);
          Ydaughter = TheLeafs->LandPoint_fY->GetValue(k);
          Zdaughter = TheLeafs->LandPoint_fZ->GetValue(k);
	  
	  // Check if it is inside the primary volume:
	  ParentInside = PointInsidePrimVolume(Inputs,Xparent,Yparent,Zparent);
	  
	  // If this is not the case, then the particle is a primary:
	  Particle_IsPrimary = !ParentInside;
	  
	  // Check if the particle is already registered:
	  RegisteredBefore = kFALSE;
      
          vecsize = Prim_TrackID_vec.size();
      
          if (vecsize>0) // otherwise there are no particles yet, so also none registered before!
          {
            for (Int_t n = 0; n<vecsize; ++n)
            {
                if (TrackID_parent==(Prim_TrackID_vec.at(n)))
                {
                    RegisteredBefore = kTRUE;
                    
                    // But now if this is the case, we might need to replace the primary interaction point:
                    if (Tdaughter<Prim_MCvec_T.at(n))
                    {
                        Prim_MCvec_T.at(n) = Tdaughter;
                        Prim_MCvec_X.at(n) = Xdaughter;
                        Prim_MCvec_Y.at(n) = Ydaughter;
                        Prim_MCvec_Z.at(n) = Zdaughter;
                    }
                    
                    // same for the production point:
                    if (Tparent<Prim_MCpos_T.at(n))
                    {
                        Prim_MCpos_T.at(n) = Tparent;
                        Prim_MCpos_X.at(n) = Xparent;
                        Prim_MCpos_Y.at(n) = Yparent;
                        Prim_MCpos_Z.at(n) = Zparent;
                    }
                }
            }
          }
      
          // And now we fill our std::vector if the particle is a primary and is not registerd before:
          if ((Particle_IsPrimary)&&(!RegisteredBefore))
          {
           // NOTE: We do not cut on PrimTcut_low since a particle with the first MC point below the cut can still be detected!
           //if (((Inputs->Monte_Carlo_Engine=="TGeant4")&&(Tdaughter<Inputs->PrimTcut_up*1e-9))||(((Inputs->Monte_Carlo_Engine=="TGeant3"))&&(Tdaughter<Inputs->PrimTcut_up)))
           //{
             Prim_TrackID_vec.push_back(TrackID_parent);
             Prim_PDG_vec.push_back(PDGparent);
             Prim_E_vec.push_back(Eparent);
             
             // We also keep track of the first interaction point of this primary particle:
             Prim_MCvec_T.push_back(Tdaughter);
             Prim_MCvec_X.push_back(Xdaughter);
             Prim_MCvec_Y.push_back(Ydaughter);
             Prim_MCvec_Z.push_back(Zdaughter);
             
             // same for the production point:
             Prim_MCpos_T.push_back(Tparent);
             Prim_MCpos_X.push_back(Xparent);
             Prim_MCpos_Y.push_back(Yparent);
             Prim_MCpos_Z.push_back(Zparent);
             
             // check if the particle is from the target:
             target_radius = 0.0;
             target_radius = target_radius + (Xparent-Inputs->NeuBegin_X)*(Xparent-Inputs->NeuBegin_X);
             target_radius = target_radius + (Yparent-Inputs->NeuBegin_Y)*(Yparent-Inputs->NeuBegin_Y);
             target_radius = target_radius + (Zparent-Inputs->NeuBegin_Z)*(Zparent-Inputs->NeuBegin_Z);
             target_radius = TMath::Sqrt(target_radius);
          
             if (target_radius<(Inputs->AllowedTargetRadius))
             {
                 From_Target.push_back(kTRUE);
             }
             else
             {
                 From_Target.push_back(kFALSE);
             }
             
           //}
          }
	    
	  // Done!  
    }
  }
  
  // And now our calculations are done.
  
  // NOTE: we take the firts change in trackID of the primary particle as its primary interaction
  // point. If for some reason this first change of trackID cannot be found, then the primary interaction point will
  // be the production point of that primary particle. Then when we search LandPoints, we use 
  // the landpoint X, Y, Z, T itself as primary interaction point. Notice also that even if a
  // primary particle is already registered, we still check if the new primary interaction point
  // candidate has a lower TOF than the current. If so, it is replaced. Therefore for both
  // neutrons and protons, the primary interaction point becomes the first point where something
  // happened. Precisely what we want.
  
  // =============================================================================================
  // Now that our calculations are done, we have to put our results into the Analysis Tree.
  // since some of the leafs that we need for this are dynamical arrays, we first have to 
  // re-allocate the arrays. We also have to do SetBranchAddress again, 
  // because a delete also destroys the link to the Analysis Tree. Only after all this,
  // the results of our calculations can be put into the Dynamical arrays. Then this
  // will be filled into the tree automatically. The delete commands that should
  // accompany all the new-commands are located in the DeleteEverything function.
  
  // So first define the length of our arrays:
  Int_t ArrayLength = Prim_PDG_vec.size();
  Bool_t PrimExists = kTRUE;
  if (ArrayLength==0)
  {
      if (Inputs->Bug_Report==1) {cout << "### BUG REPORT: 0 Primary Particles Detected! | Event = " << Event << " | MC_ArraySize = " << MC_ArraySize << " | LandPointSize = " << TheLeafs->LandPoint_fX->GetNdata() << "\n";}
      PrimExists = kFALSE; // Tests if there are actually primary particles detected!
  }
  
  if (Inputs->Bug_Report==1) {if (ArrayLength>1) {cout << "### BUG REPORT: Multiple Primary Particles Detected! | Event = " << Event << "\n";}}
  
  // Then put this into the leaf that holds the size of these dynamical arrays:
  AnaTree->Nprims_NeuLAND = ArrayLength;
  
  // Then re-allocate the arrays:
  AnaTree->Prim_Detected = new Bool_t[ArrayLength];
  AnaTree->Prim_Resolved = new Bool_t[ArrayLength];
  AnaTree->Prim_TrackID = new Int_t[ArrayLength];
  AnaTree->Prim_PDG = new Int_t[ArrayLength];
  AnaTree->Prim_E = new Double_t[ArrayLength];
  AnaTree->Prim_IsVETOed = new Bool_t[ArrayLength];
  AnaTree->Prim_IsFromTarget = new Bool_t[ArrayLength];
  AnaTree->Prim_E_Detected = new Double_t[ArrayLength];
  AnaTree->Prim_MCinteraction_T = new Double_t[ArrayLength];
  AnaTree->Prim_MCinteraction_X = new Double_t[ArrayLength];
  AnaTree->Prim_MCinteraction_Y = new Double_t[ArrayLength];
  AnaTree->Prim_MCinteraction_Z = new Double_t[ArrayLength];
  AnaTree->Prim_MCproduction_T = new Double_t[ArrayLength];
  AnaTree->Prim_MCproduction_X = new Double_t[ArrayLength];
  AnaTree->Prim_MCproduction_Y = new Double_t[ArrayLength];
  AnaTree->Prim_MCproduction_Z = new Double_t[ArrayLength];
  
  // Then re-set the Branch Addresses:
  AnaTree->TheTree->SetBranchAddress("Prim_Detected",(void*) AnaTree->Prim_Detected);
  AnaTree->TheTree->SetBranchAddress("Prim_Resolved",(void*) AnaTree->Prim_Resolved);
  AnaTree->TheTree->SetBranchAddress("Prim_TrackID",(void*) AnaTree->Prim_TrackID);
  AnaTree->TheTree->SetBranchAddress("Prim_PDG",(void*) AnaTree->Prim_PDG);
  AnaTree->TheTree->SetBranchAddress("Prim_E",(void*) AnaTree->Prim_E);
  AnaTree->TheTree->SetBranchAddress("Prim_IsVETOed",(void*) AnaTree->Prim_IsVETOed);
  AnaTree->TheTree->SetBranchAddress("Prim_IsFromTarget",(void*) AnaTree->Prim_IsFromTarget);
  AnaTree->TheTree->SetBranchAddress("Prim_E_Detected",(void*) AnaTree->Prim_E_Detected);
  AnaTree->TheTree->SetBranchAddress("Prim_MCinteraction_T",(void*) AnaTree->Prim_MCinteraction_T);
  AnaTree->TheTree->SetBranchAddress("Prim_MCinteraction_X",(void*) AnaTree->Prim_MCinteraction_X);
  AnaTree->TheTree->SetBranchAddress("Prim_MCinteraction_Y",(void*) AnaTree->Prim_MCinteraction_Y);
  AnaTree->TheTree->SetBranchAddress("Prim_MCinteraction_Z",(void*) AnaTree->Prim_MCinteraction_Z);
  AnaTree->TheTree->SetBranchAddress("Prim_MCproduction_T",(void*) AnaTree->Prim_MCproduction_T);
  AnaTree->TheTree->SetBranchAddress("Prim_MCproduction_X",(void*) AnaTree->Prim_MCproduction_X);
  AnaTree->TheTree->SetBranchAddress("Prim_MCproduction_Y",(void*) AnaTree->Prim_MCproduction_Y);
  AnaTree->TheTree->SetBranchAddress("Prim_MCproduction_Z",(void*) AnaTree->Prim_MCproduction_Z);
  
  // Now put the results of your calculations into the arrays you
  // declared with new:
  if (PrimExists)
  {
    for (Int_t k = 0; k<ArrayLength; ++k)
    {
        AnaTree->Prim_Detected[k] = kFALSE; // NOTE: This is corrected in QTDC.h ==> There it is determined which particles are detected by NeuLAND yes/no.
        AnaTree->Prim_Resolved[k] = kFALSE; // NOTE: This is only filled in if the Tracker is enabled.
        AnaTree->Prim_TrackID[k] = Prim_TrackID_vec.at(k);
        AnaTree->Prim_PDG[k] = Prim_PDG_vec.at(k);
        AnaTree->Prim_E[k] = Prim_E_vec.at(k)*1e3; // NOTE: conversion to MeV;
        AnaTree->Prim_E_Detected[k] = 0.0;  // NOTE: Will be filled in on the VETO conditions. ==> Because E_Detected also depends upon the (segmented) VETO condition.
        AnaTree->Prim_IsVETOed[k] = kFALSE; // NOTE: Will be filled in on the VETO conditions.
        AnaTree->Prim_IsFromTarget[k] = From_Target.at(k);
        AnaTree->Prim_MCinteraction_T[k] = Prim_MCvec_T.at(k); // NOTE: Conversion to nanoseconds!!! Geant3 provides [ns], but Geant4 provides [s]!
        if (Inputs->Monte_Carlo_Engine=="TGeant4") {AnaTree->Prim_MCinteraction_T[k] = 1e9*Prim_MCvec_T.at(k);}
        AnaTree->Prim_MCinteraction_X[k] = Prim_MCvec_X.at(k);
        AnaTree->Prim_MCinteraction_Y[k] = Prim_MCvec_Y.at(k);
        AnaTree->Prim_MCinteraction_Z[k] = Prim_MCvec_Z.at(k);
        AnaTree->Prim_MCproduction_T[k] = Prim_MCpos_T.at(k); // NOTE: Conversion to nanoseconds!!! Geant3 provides [ns], but Geant4 provides [s]!
        if (Inputs->Monte_Carlo_Engine=="TGeant4") {AnaTree->Prim_MCproduction_T[k] = 1e9*Prim_MCpos_T.at(k);}
        AnaTree->Prim_MCproduction_X[k] = Prim_MCpos_X.at(k);
        AnaTree->Prim_MCproduction_Y[k] = Prim_MCpos_Y.at(k);
        AnaTree->Prim_MCproduction_Z[k] = Prim_MCpos_Z.at(k);
    }
  }
  // if there are no primary particles, no values are put in the tree either,
  // so we do not need to give a value then.
  
  // Finally, also add the number of primary particles shot by the particle gun:
  AnaTree->Nprims_ParticleGun = Inputs->Beam_Nparticles;
  if (Inputs->Beam2_Exe==1) {AnaTree->Nprims_ParticleGun = AnaTree->Nprims_ParticleGun + Inputs->Beam2_Nparticles;}
  
  // =============================================================================================
  // And now we are done. All variables that we declared here without new are lost
  // after this function is complete, and that is how we like it! The results are
  // NOT lost, since we put them into the arrays that are part of the AnaTree-structure.
  // here we only work with a pointer to this structure, so the results are put
  // in our structure outside this function. Then outside this function, the
  // command tree->Fill() is given, so we are done now! Notice that the DeleteEverything
  // function is called AFTER the tree->Fill(), so we do not have memory leaks.
}

// ##################################################################################################################
// Add the empty function:

void Empty_Identify_PrimParticles(Leaf_Structure* TheLeafs, Inputs_Structure* Inputs, Tree_Structure* AnaTree, Int_t const Event, Int_t const Counter, TRandom3* generator)
{
  Int_t ArrayLength = 0;
  AnaTree->Nprims_NeuLAND = ArrayLength;
  
  AnaTree->Prim_Detected = new Bool_t[ArrayLength];
  AnaTree->Prim_Resolved = new Bool_t[ArrayLength];
  AnaTree->Prim_TrackID = new Int_t[ArrayLength];
  AnaTree->Prim_PDG = new Int_t[ArrayLength];
  AnaTree->Prim_E = new Double_t[ArrayLength];
  AnaTree->Prim_IsVETOed = new Bool_t[ArrayLength];
  AnaTree->Prim_IsFromTarget = new Bool_t[ArrayLength];
  AnaTree->Prim_E_Detected = new Double_t[ArrayLength];
  AnaTree->Prim_MCinteraction_T = new Double_t[ArrayLength];
  AnaTree->Prim_MCinteraction_X = new Double_t[ArrayLength];
  AnaTree->Prim_MCinteraction_Y = new Double_t[ArrayLength];
  AnaTree->Prim_MCinteraction_Z = new Double_t[ArrayLength];
  AnaTree->Prim_MCproduction_T = new Double_t[ArrayLength];
  AnaTree->Prim_MCproduction_X = new Double_t[ArrayLength];
  AnaTree->Prim_MCproduction_Y = new Double_t[ArrayLength];
  AnaTree->Prim_MCproduction_Z = new Double_t[ArrayLength];
  
  AnaTree->TheTree->SetBranchAddress("Prim_Detected",(void*) AnaTree->Prim_Detected);
  AnaTree->TheTree->SetBranchAddress("Prim_Resolved",(void*) AnaTree->Prim_Resolved);
  AnaTree->TheTree->SetBranchAddress("Prim_TrackID",(void*) AnaTree->Prim_TrackID);
  AnaTree->TheTree->SetBranchAddress("Prim_PDG",(void*) AnaTree->Prim_PDG);
  AnaTree->TheTree->SetBranchAddress("Prim_E",(void*) AnaTree->Prim_E);
  AnaTree->TheTree->SetBranchAddress("Prim_IsVETOed",(void*) AnaTree->Prim_IsVETOed);
  AnaTree->TheTree->SetBranchAddress("Prim_IsFromTarget",(void*) AnaTree->Prim_IsFromTarget);
  AnaTree->TheTree->SetBranchAddress("Prim_E_Detected",(void*) AnaTree->Prim_E_Detected);
  AnaTree->TheTree->SetBranchAddress("Prim_MCinteraction_T",(void*) AnaTree->Prim_MCinteraction_T);
  AnaTree->TheTree->SetBranchAddress("Prim_MCinteraction_X",(void*) AnaTree->Prim_MCinteraction_X);
  AnaTree->TheTree->SetBranchAddress("Prim_MCinteraction_Y",(void*) AnaTree->Prim_MCinteraction_Y);
  AnaTree->TheTree->SetBranchAddress("Prim_MCinteraction_Z",(void*) AnaTree->Prim_MCinteraction_Z);
  AnaTree->TheTree->SetBranchAddress("Prim_MCproduction_T",(void*) AnaTree->Prim_MCproduction_T);
  AnaTree->TheTree->SetBranchAddress("Prim_MCproduction_X",(void*) AnaTree->Prim_MCproduction_X);
  AnaTree->TheTree->SetBranchAddress("Prim_MCproduction_Y",(void*) AnaTree->Prim_MCproduction_Y);
  AnaTree->TheTree->SetBranchAddress("Prim_MCproduction_Z",(void*) AnaTree->Prim_MCproduction_Z);
  
  AnaTree->Nprims_ParticleGun = Inputs->Beam_Nparticles = 0;
}
