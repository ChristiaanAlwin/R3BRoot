void R3BNeuLANDTracker::Reset()
{
  // Resets the event:
  fArrayVertices->Clear();
  fVectorClusters.clear();
  fNeutronNumber = 0;
  fNClustersSorted = 0;
  fNVertices = 0;
  fNclustersNotEliminated = 0;
  fEdepTotalNotEliminated = 0.0;
}