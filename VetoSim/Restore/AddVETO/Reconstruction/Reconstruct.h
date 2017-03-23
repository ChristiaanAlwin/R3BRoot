void R3BNeuLANDTracker::Reconstruct()
{
  // This function actually performs the reconstrcution of the NeuLAND Vertices.
  // It is basically the AdvanchedMethod()-function of the old tracker which has
  // undergone some cosmetic changes (and a few upgrades).

  // The general procedure is that we loop over all clusters. As a first change, we
  // will now only take clusters that haven't been eliminated. From the clusters
  // that have not been eliminated, we will test them pairswise whether they can come
  // from elastic scattering yes/no. Clusters that can come from elastic scattering are marked.
    
  // Then the UNmarked clusters are sorted by their relativistic beta. Then the 
  // sorted & UNmarked clusters are divided into groups. The first staring point 
  // of each group becomes a vertex. This is how we do the reconstruction.
    
  // ===========================================================================================
  // The first step: marking the clusters:
  Double_t c = 29.9792458;
  Double_t Mneutron = 939.565379; // Neutron mass [MeV].
  Int_t NClusters = fArrayClusters->GetEntries();
  R3BNeuLANDClustr* cluster1;
  R3BNeuLANDClustr* cluster2;
  Bool_t WeMarked = kFALSE;
  Bool_t Elastic = kFALSE;
  
  // Set the counter for the vertices to zero:
  fNVertices = 0;
  
  // loop over pairs of clusters:
  for (Int_t k1 = 0; k1<NClusters; ++k1)
  {
      // load the first cluster:
      cluster1 = (R3BNeuLANDClustr*) fArrayClusters->At(k1);
      
      // Only do something if this cluster has at least size 2
      // And if it is not eliminated:
      if ((cluster1->GetSize()>1)&&(!(cluster1->IsEliminated())))
      {
          // Now we only want to mark only once per k1-turn. Reset:
          WeMarked = kFALSE;
          
          // Loop over all clusters we haven't done yet:
          for (Int_t k2 = k1+1; k2<NClusters; ++k2)
          {
              // But we should only do something if we haven't marked 
              // something yet inside this loop:
              if (!WeMarked)
              {
                    // load the second cluster:
                    cluster2 = (R3BNeuLANDClustr*) fArrayClusters->At(k2);
              
                    // Test for elastic scattering:
                    Elastic = IsElastic(cluster1,cluster2);
              
                    // If this worked, then mark if the second cluster was
                    // not eliminated:
                    if ((Elastic)&&(!(cluster2->IsEliminated())))
                    {
                        WeMarked = kTRUE;
                        cluster2->Mark();
                    }
                    
                    // Done, so close all loops:
              }
          }
      }
  }
  
  // So now we marked the clusters. The next step is to sort them according to
  // their relativistic beta. For that, we put all UNmarked & not eliminated
  // clusters inside an std::vector and then sort them.
  
  // NOTE: If I read the code correctly, then all marked clusters should NOT
  // be in the vector, only unmarked clusters!
  
  for (Int_t k = 0; k<NClusters; ++k)
  {
      // load the cluster:
      cluster1 = (R3BNeuLANDClustr*) fArrayClusters->At(k);
    
      // Check if teh cluster is eliminated:
      if (cluster1->IsEliminated()==kFALSE)
      {
          // Now check if the cluster is marked.
          // NOTE: unmarked clusters should be put in the vector!
          if (cluster1->IsMarked()==kFALSE)
          {
              fVectorClusters.push_back(cluster1);
          }
      }
  }
  
  // so now we have our vector with clusters. Sort them:
  fNClustersSorted = fVectorClusters.size();
  
  if (fNClustersSorted>1) 
  {
    std::sort(fVectorClusters.begin(), fVectorClusters.end(), AuxSortClustersBeta);
    // NOTE: AuxSortClustersBeta is the auxillary sorting function that we wrote just for that!
  }
  
  // So now we have our vector with sorted clusters. If a cluster from this vector passes our tests,
  // it can become a neutron vertex. But no more vertices are created then there are neutrons.
  
  // Check that we have clusters to work with:
  if (fNClustersSorted>0)
  {
    // Define an array to keep track of the clusters that we already used:
    Bool_t* Used = new Bool_t[fNClustersSorted];
    
    // And a variable to keep track whether we reconstructed or not:
    Bool_t FoundCluster = kFALSE;
    
    // We also need some other variables:
    Double_t time = 0.0;
    Double_t xx = 0.0;
    Double_t yy = 0.0;
    Double_t zz = 0.0;
    Double_t Travel_Dist = 0.0;
    Double_t BeamTime = 0.0;
    Double_t beta = 0.0;
    Double_t gamma = 0.0;
    Double_t Ekin = 0.0;
    
    // Set is to false:
    for (Int_t k = 0; k<fNClustersSorted; ++k)
    {
        Used[k] = kFALSE;
    }
    
    // Now redo our procedure as often as we need for the number of neutrons:
    for (Int_t kneutron = 0; kneutron<fNeutronNumber; ++kneutron)
    {
        // Reset:
        FoundCluster = kFALSE;
        
        // loop over all clusters in the vector:
        for (Int_t kcluster = 0; kcluster<fNClustersSorted; ++kcluster)
        {
            // Load the cluster:
            cluster1 = (R3BNeuLANDClustr*) fVectorClusters.at(kcluster);
            
            // As a first test: check if the cluster
            // is used already for the previous vertex, or if we already
            // found a cluster for this vertex:
            if ((Used[kcluster]==kFALSE)&&(FoundCluster==kFALSE))
            {
                // Then it is not used. As the next step, check if there 
                // is enough energy in the cluster:
                if (cluster1->GetE()>2.5)
                {
                    // Now there is enough energy in the cluster.
                    // Now also check if the beta of the cluster is OK:
                    xx = cluster1->GetStartX();
                    yy = cluster1->GetStartY();
                    zz = cluster1->GetStartZ();
                    
                    Travel_Dist = TMath::Sqrt((xx - fTarget_Xpos)*(xx - fTarget_Xpos) + (yy - fTarget_Ypos)*(yy - fTarget_Ypos) + (zz - fTarget_Zpos)*(zz - fTarget_Zpos));
                    BeamTime = TMath::Sqrt((fTarget_Xpos - fBeam_Xpos)*(fTarget_Xpos - fBeam_Xpos) + (fTarget_Ypos - fBeam_Ypos)*(fTarget_Ypos - fBeam_Ypos) + (fTarget_Zpos - fBeam_Zpos)*(fTarget_Zpos - fBeam_Zpos));
                    BeamTime= BeamTime/(fBeamBeta*c);
                    time = cluster1->GetStartT() - BeamTime;
                    beta = Travel_Dist/(time*c);
                    
                    // We now know the beta, check for it:
                    if (TMath::Abs(beta-fBeamBeta)<(0.05*600./fBeamEnergy))
                    {
                        // Then we also passed the beta-test. Hence we can now use this
                        // cluster to create a vertex. First reconstruct the energy:
                        if (beta<0.0) {beta = 0.0; cout << "### ERROR: beta smaller then zero!\n";}
                        if (beta>=1.0) {beta = 0.999; cout << "### ERROR: superliminal vertex found!\n";}
                        gamma = 1.0/TMath::Sqrt(1.0 - beta*beta);
                        Ekin = (gamma - 1.0)*Mneutron;
                   
                        // Now create the vertex:
                        new ((*fArrayVertices)[fNVertices]) R3BNeuLANDVertex(cluster1,Ekin);
                        
                        // Update the counters:
                        fNVertices = fNVertices + 1;
                        Used[kcluster] = kTRUE;
                        FoundCluster = kTRUE;
                        
                        // That's it. We now succesfully creatd our vertices!
                        // close the loops:
                    }
                }
            }
        }
    }
    
    if (fNVertices>fNeutronNumber) {cout << "### ERROR: Too many verices in event " << fEventCounter << "\n";}
    
    // One more thing: every new goes with a delete:
    delete Used;
  }
                        
  // Finished the reconstruction!    
}