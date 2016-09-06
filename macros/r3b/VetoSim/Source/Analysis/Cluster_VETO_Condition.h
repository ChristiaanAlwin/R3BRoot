void Cluster_VETO_Condition(Leaf_Structure* const TheLeafs, Inputs_Structure* const Inputs, Tree_Structure* AnaTree, Int_t const Event, Int_t const Counter, TRandom3* generator)
{
  // This function performs the actual VETO condition. There are multiple versions of this function
  // available. This one uses the spatial sructure of the VETO to specifically VETO yes/no
  // certain Clusters of NeuLAND. Based on this, we then determine which primary particles are 
  // VETOed yes/no. The leafs that should be filled by the VETO
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
  
  // Written by C. A. Douma, for the R3B collaboration
    
  // =============================================================================================
  // NOTE: Nothing needs to be re-declared, deleted, etc. This is done in other functions!
  // =============================================================================================
  // Now the idea is that we loop first over all fired VETO bars and then over all clusters.
  // If we find that a certain cluster is within the VETO-ed area, we will VETO it.

  // Declare what we need:
  Double_t NeuhalfThickness = 2.5*((Int_t) Inputs->NeuLAND_geo_NsinglePlanes);
  Int_t VETO_NbarsFired = AnaTree->NbarsFired_VETO;
  Int_t NClusters = AnaTree->NeuLAND_Nclusters;
  Int_t ThisIndex = -1;
  Int_t Sector = -1;
  Double_t Xclust = 0.0;
  Double_t Yclust = 0.0;
  Double_t Zclust = 0.0;
  Double_t Xswitch = 0.0;
  Double_t Yswitch = 0.0;
  Double_t Zswitch = 0.0;
  Double_t Showerwidth_X = Inputs->Shower_Radius/TMath::Cos(TMath::ATan(Inputs->VETO_Xdir));
  Double_t Showerwidth_Y = Inputs->Shower_Radius/TMath::Cos(TMath::ATan(Inputs->VETO_Ydir));
  VETOhit ThisHit;// declaration of the hit-structure.
  
  // check if there is something to loop over:
  if ((VETO_NbarsFired>0)&&(NClusters>0))
  {
    // Loop over the VETO bars:  
    for (Int_t kveto = 0; kveto<VETO_NbarsFired; ++kveto)
    {
      // For each bar we already have the C++ index in the veto_segPoint-leafs that belong to this
      // hit (min. TOF point). Hence with our Tdiff.h we can directly obtain the position of the 
      // hit in the NeuLAND-frame:
      ThisIndex = AnaTree->VETO_TOF_index[kveto];
      
      if (Inputs->UseDigitizer>0)
      {
        ThisHit = GetVETOhit_Digi(Inputs,TheLeafs,ThisIndex);
      }
      else
      {
        ThisHit = GetVETOhit(Inputs,TheLeafs,ThisIndex);
      }
      
      // So now we have the hit, we can loop over the clusters of NeuLAND:
      for (Int_t kclust = 0; kclust<NClusters; ++kclust)
      {
          // Get the x, y, z coordinates of the cluster:
          Xclust = AnaTree->NeuLAND_ClusterX[kclust];
          Yclust = AnaTree->NeuLAND_ClusterY[kclust];
          Zclust = AnaTree->NeuLAND_ClusterZ[kclust];
          
          // Now we must perform the switch (if the particles do not come straight
          // from the target):
          Zswitch = Zclust - ThisHit.Zhit;
          Yswitch = (Inputs->VETO_Ydir)*Zswitch;
          Yclust = Yclust - Yswitch;
          Xswitch = (Inputs->VETO_Xdir)*Zswitch;
          Xclust = Xclust - Xswitch;
          // this corrects the position of the cluster so that we now can pretent as if it
          // is staright behind the VETO. Hence we corected for particles coming in
          // at an angle.
          
          // So now we can make a decision whether to VETO this cluster yes/no:
                  
          if (((ThisHit.Yhit-Showerwidth_Y)<Yclust)&&((ThisHit.Yhit+Showerwidth_Y)>Yclust)&&((ThisHit.Xhit-Showerwidth_X)<Xclust)&&((ThisHit.Xhit+Showerwidth_X)>Xclust))
          {
              // Then do our TOF-check: only VETO if the VETO fired before NeuLAND:
              if (AnaTree->VETO_TOF_Fired_Res[kveto]<AnaTree->Neu_TotalTOF_Res)
              {
                  // Then we can VETO:
                  AnaTree->NeuLAND_Cluster_IsVETOed[kclust] = kTRUE;
              }
          }
          
          // And now we are done with VETOing clusters.
      }
    }
  }
 
  // =============================================================================================
  // Now the next task is to compute the total energy deposition per identified primary particle.
  // Notice that only unVETOed clusters should be taken along.
  
  // Declare the energy array to compute our energies for:
  Int_t Nprims = AnaTree->Nprims_NeuLAND;
  Double_t* Energy = new Double_t[Nprims];
  for (Int_t k = 0; k<Nprims; ++k)
  {
      Energy[k] = 0.0;
  }
  
  // Now loop over clusters and primaries to add up the energy inside the clusters:
  if ((Nprims>0)&&(NClusters>0))
  {
      for (Int_t kprim = 0; kprim<Nprims; ++kprim)
      {
          for (Int_t kclust = 0; kclust<NClusters; ++kclust)
          {
              // compare TrackID to decide which energy to add where:
              if (AnaTree->NeuLAND_Cluster_TrackID[kclust]==AnaTree->Prim_TrackID[kprim])
              {
                  // Then add the energy to the right particle, but only if the cluster is not VETOed:
                  if (!(AnaTree->NeuLAND_Cluster_IsVETOed[kclust]))
                  {
                    Energy[kprim] = Energy[kprim] + AnaTree->NeuLAND_ClusterEnergy[kclust];
                  }
              }
              
              // Notice that in this way clusters with TrackID==-1 are skipped from the addition!
              // These are the clusters that could not be pared up with a primary particle,
              // so this is exactly how we like it!
          }
      }
  }
  
  // So now we have the energies. Now put them into the tree:
  for (Int_t k = 0; k<Nprims; ++k)
  {
      AnaTree->Prim_E_Detected[k] = Energy[k];
  }
  // NOTE: We overwite the tracker result here on purpose, since otherwise this condition fails!
  
  // then delete other arrays:
  delete Energy;

  // =============================================================================================
  // Now after we did this, we can fill in the final leaf we are interested in: Whether or not
  // a primary particle is VETOed. Hopefully the VETO condition will cut out a lot of clusters
  // that belong to this particle. hence the reconstructed (Detected) energy is low. Then we
  // will VETO the particle if the energy is below a certain threshold. Notice however that this
  // is only necessary if the VETO actually fired. if it did not fire (or only fired after NeuLAND),
  // there is no need to try to separate something. We simply should keep everything.
  
  // First see if there is a fired bar in the VETO before NeuLAND:
  Bool_t VETOfired = kFALSE;
  
  if (VETO_NbarsFired>0)
  {
      for (Int_t kveto = 0; kveto<VETO_NbarsFired; ++kveto)
      {
          if (AnaTree->VETO_TOF_Fired_Res[kveto]<AnaTree->Neu_TotalTOF_Res)
          {
              VETOfired = kTRUE;
          }
      }
  }
  
  Double_t Ecut = Inputs->VETO_Ecut;
  
  // loop over the primary particles:
  for (Int_t k = 0; k<Nprims; ++k)
  {
      if (((AnaTree->Prim_E_Detected[k])<Ecut)&&(VETOfired))
      {
          AnaTree->Prim_IsVETOed[k] = kTRUE;
      }
      else
      {
          AnaTree->Prim_IsVETOed[k] = kFALSE;
      }
  }
          
  // And that's it. Now we have filled in all 3 leafs that we had to fill in.        
    
}