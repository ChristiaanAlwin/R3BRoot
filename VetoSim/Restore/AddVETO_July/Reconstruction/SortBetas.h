// Auxilliary function for sorting clusters with respect to their beta:
// to be used with std::sort from <algorithm>
bool AuxSortClustersBeta(R3BNeuLANDCluster* c1, R3BNeuLANDCluster* c2)
{
  // first load the start positions of the clusters:
  TVector3 pos1;
  TVector3 pos2;
  c1->StartPosition(pos1);
  c2->StartPosition(pos2);
  
  // Then Correct for the travel time of the beam:
  Double_t c = 29.9792458;
  Double_t Beam_TravelTime = TMath::Sqrt((fTarget_Xpos_global - fBeam_Xpos_global)*(fTarget_Xpos_global - fBeam_Xpos_global) + (fTarget_Ypos_global - fBeam_Ypos_global)*(fTarget_Ypos_global - fBeam_Ypos_global) + (fTarget_Zpos_global - fBeam_Zpos_global)*(fTarget_Zpos_global - fBeam_Zpos_global));
  Beam_TravelTime = Beam_TravelTime/(fBeamBeta_global*c);
  
  Double_t Travel_Dist1 = TMath::Sqrt((pos1.X() - fTarget_Xpos_global)*(pos1.X() - fTarget_Xpos_global) + (pos1.Y() - fTarget_Ypos_global)*(pos1.Y() - fTarget_Ypos_global) + (pos1.Z() - fTarget_Zpos_global)*(pos1.Z() - fTarget_Zpos_global));
  Double_t time1 = c1->GetStartT() - Beam_TravelTime;
  Double_t beta1 = Travel_Dist1/(time1*c);
  
  Double_t Travel_Dist2 = TMath::Sqrt((pos2.X() - fTarget_Xpos_global)*(pos2.X() - fTarget_Xpos_global) + (pos2.Y() - fTarget_Ypos_global)*(pos2.Y() - fTarget_Ypos_global) + (pos2.Z() - fTarget_Zpos_global)*(pos2.Z() - fTarget_Zpos_global));
  Double_t time2 = c2->GetStartT() - Beam_TravelTime;
  Double_t beta2 = Travel_Dist2/(time2*c);
  
  // Now Execute the beta sorting:
  Double_t val1 = TMath::Abs(beta1-fBeamBeta_global)/c1->GetE();
  Double_t val2 = TMath::Abs(beta2-fBeamBeta_global)/c2->GetE();
  if(val1 < val2) 
  {
    return true;
  } 
  else 
  {
    return false;
  }
}