void R3BNeuLANDTracker::ApplyCuts()
{
  // This function Applies the cuts of teh Calibration file
  // to determine the number of neutrons.
    
  // We begin by looping over the number of clusters. We count the
  // total energy in the clusters and the total number of clusters.
  // However, we should only take along clustersv that are not
  // eliminated:

  Int_t NClusters = fArrayClusters->GetEntries();
  fNclustersNotEliminated = 0;
  fEdepTotalNotEliminated = 0.0;
  R3BNeuLANDCluster* cluster;
  
  for (Int_t k = 0; k<NClusters; ++k)
  {
      cluster = (R3BNeuLANDCluster*) fArrayClusters->At(k);
      
      if (!(cluster->IsEliminated()))
      {
          // Then count it:
          fNclustersNotEliminated = fNclustersNotEliminated + 1;
          fEdepTotalNotEliminated = fEdepTotalNotEliminated + cluster->GetE();
      }
  }
  
  // fill control histograms:
  h_ncl_elim->Fill(fNclustersNotEliminated);
  h_ncl->Fill(NClusters);
  h_edeptot_elim->Fill(fEdepTotalNotEliminated);
  
  // Now we have the total energy deposition. Now we can apply the cuts:
  Double_t Ylow = 0.0;
  Double_t Yhigh = 0.0;
  Double_t Yreal = (Int_t) fNclustersNotEliminated;
  
  for (Int_t k = 0; k<(fNumberOfCuts-1); ++k)
  {
      // Calculate the boundaries: lines are defined by y = kappa*(Ecut - x).
      Ylow = fKappa*(fCuts[k] - fEdepTotalNotEliminated);
      Yhigh = fKappa*(fCuts[k+1] - fEdepTotalNotEliminated);
      
      // Get the number of neutrons:
      if ((Yreal>Ylow)&&(Ylow<Yhigh)) {fNeutronNumber = k+1;}
  }
  
  // Note that we should actually have registered something.
  // We say that NeuLAND did not fire if the total energy deposition
  // is below 1 MeV:
  if (fEdepTotalNotEliminated<1.0) {fNeutronNumber = 0;}
    
  // Done!
}