void R3BNeuLANDTracker::Filter()
{
  // This function eliminates the NeuLAND clusters that are created by
  // charged particles.
    
  // In order to do that, we begin by loading the number VETO digis:
  Int_t NvetoDigis = fArrayVetoDigis->GetEntries();
  
  // Also load the number of clusters:
  Int_t NClusters = fArrayClusters->GetEntries();
  
  // Declare the rest of the variables we might need:
  R3BNveto_segDigi* VetoDigi;
  R3BNeuLANDClustr* cluster;
  TVector3 vetohit;
  TVector3 hit1;
  TVector3 hit2;
  TVector3 pos;
  Double_t veto_t;
  Double_t veto_x;
  Double_t veto_y;
  Double_t veto_z;
  Double_t hit1_t;
  Double_t hit1_x;
  Double_t hit1_y;
  Double_t hit1_z;
  Double_t hit2_t;
  Double_t hit2_x;
  Double_t hit2_y;
  Double_t hit2_z;
  Double_t theta;
  Double_t phi;
  Double_t DistLine;
  Double_t SpaceTimeDist;
  Double_t MinDist;
  Double_t v_Estimate;
  Double_t t_Estimate;
  Int_t Topology;
  Int_t MinDistIndex;
  
  // Some definitions:
  Double_t ceff = 14.0; // Rougly the effective speed of light inside NeuLAND scintillators.
  
  if (NvetoDigis>0)
  {
      // Then we actually have some hits: loop over the VETO hits:
      for (Int_t kveto = 0; kveto<NvetoDigis; ++kveto)
      {
          // load the VETO hit:
          VetoDigi = (R3BNveto_segDigi*) fArrayVetoDigis->At(kveto);
          
          // Reset the search for the minimum:
          MinDist = 1e99;
          MinDistIndex = -1;
          
          // Then we loop over the clusters:
          for (Int_t kc = 0; kc<NClusters; ++kc)
          {
              // Load the cluster:
              cluster = (R3BNeuLANDClustr*) fArrayClusters->At(kc);
              
              // Now we whish to eliminate the clusters that are in a straight line
              // behind the VETO hit. However, the topology of the cluster
              // has to be taken into account.
              
              // As a first step, only continue for clusters with a size of at least 2:
              if (cluster->GetSize()>1)
              {     
                  // Load the hits:
                  pos = VetoDigi->GetPosition();
                  veto_t = VetoDigi->GetT();
                  veto_x = pos.X();
                  veto_y = pos.Y();
                  veto_z = pos.Z();
              
                  hit1_t = cluster->GetStartT();
                  hit1_x = cluster->GetStartX();
                  hit1_y = cluster->GetStartY();
                  hit1_z = cluster->GetStartZ();
                  
                  hit2_t = cluster->GetStopT();
                  hit2_x = cluster->GetStopX();
                  hit2_y = cluster->GetStopY();
                  hit2_z = cluster->GetStopZ();
                  
                  Topology = cluster->GetTopology();
                  
                  if ((Topology==2)||(Topology==3)||(Topology==4))
                  {
                      hit2_t = cluster->GetEdgeT();
                      hit2_x = cluster->GetEdgeX();
                      hit2_y = cluster->GetEdgeY();
                      hit2_z = cluster->GetEdgeZ();
                  }
                  
                  // Only continue for clusters that indeed fired after the VETO:
                  if (hit1_t>veto_t)
                  {
                  
                      // Now shift to a frame where the veto hit is at the origin:
                      hit1.SetX(hit1_x - veto_x);
                      hit1.SetY(hit1_y - veto_y);
                      hit1.SetZ(hit1_z - veto_z);
                  
                      hit2.SetX(hit2_x - veto_x);
                      hit2.SetY(hit2_y - veto_y);
                      hit2.SetZ(hit2_z - veto_z);
                  
                      // Now rotate untill hit1 is at the z-axis:
                      theta = hit1.Theta();
                      phi = hit1.Phi();
                  
                      hit1.RotateZ(-1.0*phi);
                      hit1.RotateY(-1.0*theta);
                  
                      hit2.RotateZ(-1.0*phi);
                      hit2.RotateY(-1.0*theta);
                  
                      // Then the distance to the line is:
                      DistLine = TMath::Sqrt(hit2.X()*hit2.X() + hit2.Y()*hit2.Y());
                  
                      // Now we must extrapolate the time of hit2 from vetohit and hit1:
                      v_Estimate = hit1.Z()/(hit1_t - veto_t);
                      t_Estimate = veto_t + (hit2.Z()/v_Estimate);
                  
                      // The spacetime distance equals:
                      SpaceTimeDist = TMath::Sqrt((hit2_t-t_Estimate)*(hit2_t-t_Estimate)*ceff*ceff + DistLine*DistLine);
                      
                      // Now search for the minimum:
                      if (SpaceTimeDist<MinDist)
                      {
                          MinDist = SpaceTimeDist;
                          MinDistIndex = kc;
                      }
                      
                      // That's it. If we now close the cluster-loop, MinDistIndex will
                      // hold the index of the cluster that fits best with the VETO hit.
                      // But if this index still equals -1, we know that there were no suitable
                      // clusters, for example due to the TOF requirement that the VETO should fire first.
                  }
              }
          }
          
          // So now we can eliminate a cluster for this VETO hit:
          if (MinDistIndex>-1)
          {
              cluster = (R3BNeuLANDClustr*) fArrayClusters->At(MinDistIndex);
              cluster->Eliminate();
          }
          
          // Now we close the VETO hits loop so that each VETO has an eliminated cluster:
      }
  }
  
  // NOTE: This code is not completely perfect, since in the case of multiple VETO hits, it will
  // simply search for the best cluster for VETO hit 1 and then eliminate it. However, this cluster
  // might fit better for VETO hit 2 then for hit 1. In which case we should have found another cluster
  // for hit 1. If the same cluster corresponds both to hit 1 & 2, only 1 cluster is eliminated in this case.
  // maybe even better in the case when 2 particles fire the same part of the detector...
  
  // ==================================================================================================
  // Now this only has been the first step in the elimination process. In the second stage, we will
  // start from the first eliminated clusters, directly by the VETO hit and see which other clusters
  // can be eliminated too. For each step in the elimination level, we will see which UnEliminated cluster
  // has its start point closest to the endpoint of the eliminated cluster. We will then also VETO this
  // cluster. And so on. This means that the number of eliminated clusters doubles on each time this
  // process is repeated. This is determined by the eliminationlevel. We will however eliminate in two steps
  // to avoid eliminating too many clusters.
  
  // Declare what we need:
  R3BNeuLANDClustr* ELIMcluster;
  Double_t Min = 0.0;
  Double_t kMin = -1;
  Bool_t Test;
  
  // Check if we should continue:
  if (fEliminationLevel>0)
  {
      // We begin with a for-loop of the elimination level:
      for (Int_t kel = 0; kel<fEliminationLevel; ++kel)
      {
          // Then we begin with the first loop over the clusters:
          for (Int_t k1 = 0; k1<NClusters; ++k1)
          {
              // retrieve the first cluster:
              ELIMcluster = (R3BNeuLANDClustr*) fArrayClusters->At(k1);
              
              // Check if this one is eliminated:
              if (ELIMcluster->IsEliminated())
              {
                  // Reset the search for the minimum:
                  Min = 1e99;
                  kMin = -1;
                  
                  // Then, loop again over all the other clusters:
                  for (Int_t k2 = 0; k2<NClusters; ++k2)
                  {
                      // Make sure we do not get the same cluster:
                      if (k2!=k1)
                      {
                          // Retrieve the second cluster:
                          cluster = (R3BNeuLANDClustr*) fArrayClusters->At(k2);
                          
                          // Compute the distance between the starting point and the stopping point:
                          SpaceTimeDist = 0.0;
                          SpaceTimeDist = SpaceTimeDist + (ELIMcluster->GetStopX() - cluster->GetStartX())*(ELIMcluster->GetStopX() - cluster->GetStartX()); // NOTE: Difference between Stop point of eliminated cluster & startpoint of other cluster!
                          SpaceTimeDist = SpaceTimeDist + (ELIMcluster->GetStopY() - cluster->GetStartY())*(ELIMcluster->GetStopY() - cluster->GetStartY()); // NOTE: Difference between Stop point of eliminated cluster & startpoint of other cluster!
                          SpaceTimeDist = SpaceTimeDist + (ELIMcluster->GetStopZ() - cluster->GetStartZ())*(ELIMcluster->GetStopZ() - cluster->GetStartZ()); // NOTE: Difference between Stop point of eliminated cluster & startpoint of other cluster!
                          SpaceTimeDist = SpaceTimeDist + ceff*ceff*(ELIMcluster->GetStopT() - cluster->GetStartT())*(ELIMcluster->GetStopT() - cluster->GetStartT()); // NOTE: Difference between Stop point of eliminated cluster & startpoint of other cluster!
                          SpaceTimeDist = TMath::Sqrt(SpaceTimeDist);
                          
                          // Search for the minimum, but only for uneliminated clusters:
                          if ((SpaceTimeDist<Min)&&(!(cluster->IsEliminated())))
                          {
                              Min = SpaceTimeDist;
                              kMin = k2;
                          }
                          
                          // End the second cluster loop k2:
                      }
                  }
                  
                  // Now that we found the closest cluster to the eliminated cluster.
                  // marki this cluster for elimination. However, it might happen for higher elimination
                  // levels that a minimum could no lo longer be found among uneliminated clusters
                  // because there are simply not enough clusters. Hence check for this:
                  if (kMin>-1)
                  {
                    cluster = (R3BNeuLANDClustr*) fArrayClusters->At(kMin);
                    cluster->MarkEl();
                  }
                  
                  // Now end the cluster loops:
              }
          }
      
          // Now eliminate the clusters that are marked for elimination:
          for (Int_t k1 = 0; k1<NClusters; ++k1)
          {
            // retrieve the cluster:
            ELIMcluster = (R3BNeuLANDClustr*) fArrayClusters->At(k1);
          
            // Eliminate it, but lift its marker:
            if (ELIMcluster->IsElMarked()==kTRUE) {ELIMcluster->Eliminate(); ELIMcluster->UnMarkEl();}
          }
      
          // By doing this, one avoids that an cluster early in the indices eliminates a cluster
          // late in the indices, which then in turn also eliminates a cluster within the same loop.
          // we do not want that!
          
          // End now the loop over the elimination level:
      }      
  }
  
  // count number of eliminated clusters:
  Int_t ElimCounter = 0;
  
  for (Int_t k = 0; k<NClusters; ++k)
  {
      // retrieve the first cluster:
      cluster = (R3BNeuLANDClustr*) fArrayClusters->At(k);
      
      if (cluster->IsEliminated()) {ElimCounter = ElimCounter + 1;}
  }
  
  h_elimclusters->Fill(ElimCounter);
                  
  // The filtering from the VETO is now done.
}