#include "TVector3.h"

void ChristiaanTracker(Leaf_Structure* const TheLeafs, Inputs_Structure* const Inputs, Tree_Structure* AnaTree, Int_t const Event, Int_t const Counter, TRandom3* generator)
{
  // This function performs a tracking algorithm for NeuLAND. // The tracker works in the following way:
  // We first loop over all bars. If a horizontal and a vertical bar fired together, we assign a hitpoint to the crossroad.
  // we do this for all possible combinations of bars. Then we will loop over the hitpoints. If two hitpoints are less
  // far away then a certain radius, we will assign them to be part of the same cluster. The idea is that if we do this
  // correctly, we will get 1 cluster per incoming particle. Of course the cluster will have a complicated shape. Then all we
  // have to do is to find the 'first hit' in the cluster. we do this by TOF. This we then call the staring point of the cluster.
  // hence we can assign a track staring at this staring point and assign a direction based on the next hitpoint. This will
  // give us then a track.
  
  // Written by C. A. Douma.
  
  // ==========================================================================================================================
  // We begin with obtaining the number of bars per plane and the number of planes in NeuLAND:
  Int_t NbarsPerPlane = 50;
  Int_t NSinglePlanes = Inputs->NeuLAND_geo_NsinglePlanes;
  
  // Define std::vectors to store the hits that we detect:
  std::vector<Int_t> Hit_IDNumber; // [int]
  std::vector<Double_t> Hit_Xlocation; // [cm]
  std::vector<Double_t> Hit_Ylocation; // [cm]
  std::vector<Double_t> Hit_Zlocation; // [cm]
  std::vector<Double_t> Hit_TOF; // [ns]
  std::vector<Int_t> Hit_BarNr_first // [int]
  std::vector<Int_t> Hit_BarNr_second // [int]
  std::vector<Int_t> Hit_ClusterID // [int]
  
  Hit_IDNumber.clear();
  Hit_Xlocation.clear();
  Hit_Ylocation.clear();
  Hit_Zlocation.clear();
  Hit_TOF.clear();
  Hit_BarNr_first.clear();
  Hit_BarNr_second.clear();
  Hit_ClusterID.clear();  
  
  // Then execute the loop to identify the hits:
  TVector3 hitXYZ;
  Int_t Sector = 0;
  Int_t NextSector = 0;
  Int_t NBarsFired = AnaTree->NbarsFired_NeuLAND;
  Int_t FiredNr = 0;
  Bool_t ThisFired = kFALSE;
  Bool_t NextFired = kFALSE;
  Double_t ThisTOF = 0.0;
  Double_t NextTOF = 0.0;
  Double_t ThisEdep = 0.0;
  Double_t NextEdep = 0.0;
  Double_t Xhit = 0.0;
  Double_t Yhit = 0.0;
  Double_t Zhit = 0.0;
  Double_t TOFhit = 0.0;
  
  Int_t HitCounter = 1;
  
  // Loop over the planes:
  for (Int_t Plane = 1; Plane<(NSinglePlanes+0); ++Plane) // we skip the last plane, since it is covered by nextbar!
  {
    // Loop over the bars in this plane:
    for (Int_t Bar = 1; Bar<(NbarsPerPlane+1); ++Bar)
    {
      // loop over the bars in the next plane:
      for (Int_t NextBar = 1; NextBar<(NbarsPerPlane+1) ++NextBar)
      {
	// Now firts we need to decide if Bar and NextBar fired:
	Sector = (Plane-1)*50 + Bar;
	NextSector = (Plane-0)*50 + NextBar;
	
	// reset:
	ThisFired = kFALSE;
	NextFired = kFALSE;
	
	// loop over all fired bars:
	for (Int_t k = 0; k<NBarsFired; ++k)
	{
	  FiredNr = AnaTree->Neu_BarNumber_Fired[k];
	  if (FiredNr==Sector) {ThisFired = kTRUE; ThisTOF = AnaTree->Neu_TOF_Fired_Res[k]; ThisEdep = AnaTree->Neu_Edep_Fired[k];}
	  if (FiredNr==NextSector) {NextFired = kTRUE; NextTOF = AnaTree->Neu_TOF_Fired_Res[k]; NextEdep = AnaTree->Neu_Edep_Fired[k];}
	}
	  
	// Now we know whether the bars fired yes/no. Hwence only continue if both bars fired:
	if (ThisFired&&NextFired)
	{
	  // Then we should now add a hit. First compute the spatial coordinates of the hit:
	  Zhit = -2.5*NSinglePlanes + 5.0*Plane;
	  
	  // Decide which planes are horizontal and which are vertical:
	  if (Plane%2==0)
	  {
	    // Then Plane is even. Hence the ThisPlane will be vertical and the NextPlane will be horizontal.
	    // hence we get the x from the ThisPlane and the y from the NextPlane.
	    Xhit = -125.0 + 5.0*(((Int_t) Bar) - 0.5);
	    Yhit = -125.0 + 5.0*(((Int_t) NextBar) - 0.5);
	  }
	  else
	  {
	    // then it is the other way around:
	    Yhit = -125.0 + 5.0*(((Int_t) Bar) - 0.5);
	    Xhit = -125.0 + 5.0*(((Int_t) NextBar) - 0.5);
	  }
	  
	  // Now we have the location of the hit. But x, y, z are in NeuLAND standard frame. Now we have to transform it to the LAB-frame:
	  hitXYZ.SetX(Xhit);
	  hitXYZ.SetY(Yhit);
	  hitXYZ.SetZ(Zhit);
	  
	  hitXYZ.RotateX(Inputs->NeuLAND_geo_RotX);
	  hitXYZ.RotateY(Inputs->NeuLAND_geo_RotY);
	  hitXYZ.RotateZ(Inputs->NeuLAND_geo_RotZ);
	  
	  Xhit = hitXYZ.X() + Inputs->NeuLAND_geo_Xpos;
	  Yhit = hitXYZ.Y() + Inputs->NeuLAND_geo_Ypos;
	  Zhit = hitXYZ.Z() + Inputs->NeuLAND_geo_Zpos;
	  
	  // Now we should also obtain the TOF from the hit:
	  if (ThisTOF<NextTOF) {TOFhit = ThisTOF;} else {TOFhit = NextTOF;}
	  
	  // And now we have everything. Now add the hit to the std::vectors:
	  Hit_IDNumber.push_back(HitCounter);
	  Hit_Xlocation.push_back(Xhit);
	  Hit_Ylocation.push_back(Yhit);
	  Hit_Zlocation.push_back(Zhit);
	  Hit_TOF.push_back(TOFhit);
	  Hit_BarNr_first.push_back(Bar);
	  Hit_BarNr_second.push_back(NextBar);
	  Hit_ClusterID.push_back(0); // will be determined later!
	  
	  // just increase the counter:
	  HitCounter = HitCounter + 1;
	  
	  // Done! Just close all blocks now:
	}
      }
    }
  }
  
  // =================================================================================================================
  // So now we have all of our hits. Now we have to loop over our hits and assign a cluster_ID to it so that we
  // know to which cluster they belong:
  
  // Declare what we need:
  Int_t Nhits = Hit_IDNumber.size();
  Double_t Sphere_Radius = 10.0; // [cm] ==> All hits inside belong to the same cluster!
  Double_t Hit_Distance = 0.0;
  
  // Storing the current hit:
  Int_t ThisHit_IDNumber; // [int]
  Double_t ThisHit_Xlocation; // [cm]
  Double_t ThisHit_Ylocation; // [cm]
  Double_t ThisHit_Zlocation; // [cm]
  Double_t ThisHit_TOF; // [ns]
  Int_t ThisHit_BarNr_first // [int]
  Int_t ThisHit_BarNr_second // [int]
  Int_t ThisHit_ClusterID // [int]

  // Define vectors to store the hits within a single sphere:
  std::vector<Int_t> HitsInSphere_IDNumber; // [int]
  std::vector<Double_t> HitsInSphere_Xlocation; // [cm]
  std::vector<Double_t> HitsInSphere_Ylocation; // [cm]
  std::vector<Double_t> HitsInSphere_Zlocation; // [cm]
  std::vector<Double_t> HitsInSphere_TOF; // [ns]
  std::vector<Int_t> HitsInSphere_BarNr_first // [int]
  std::vector<Int_t> HitsInSphere_BarNr_second // [int]
  std::vector<Int_t> HitsInSphere_ClusterID // [int]
  
  HitsInSphere_IDNumber.clear();
  HitsInSphere_Xlocation.clear();
  HitsInSphere_Ylocation.clear();
  HitsInSphere_Zlocation.clear();
  HitsInSphere_TOF.clear();
  HitsInSphere_BarNr_first.clear();
  HitsInSphere_BarNr_second.clear();
  HitsInSphere_ClusterID.clear(); 
  
  // Now loop over all hits:
  for (Int_t ThisHit = 0; ThisHit<Nhits ++ThisHit)
  {
    // Extract the hit under investigation:
    ThisHit_ID = Hit_IDNumber.at(ThisHit);
    ThisHit_Xlocation = Hit_Xlocation.at(ThisHit);
    ThisHit_Ylocation = Hit_Ylocation.at(ThisHit);
    ThisHit_Zlocation = Hit_Zlocation.at(ThisHit);
    ThisHit_TOF = Hit_TOF.at(ThisHit);
    ThisHit_BarNr_first = Hit_BarNr_first.at(ThisHit);
    ThisHit_BarNr_second = Hit_BarNr_second.at(ThisHit);
    ThisHit_ClusterID = Hit_ClusterID.at(ThisHit);
    
    // Then loop over all hits in total:
    for (Int_t CheckHit = 0; CheckHit<Nhits; ++CheckHit)
    {
      // But skip the one under investigation:
      if (!(CheckHit==ThisHit))
      {
	// then compute the hit distance:
	Hit_Distance = TMath::Sqrt((ThisHit_Xlocation - Hit_Xlocation.at(CheckHit))*(ThisHit_Xlocation - Hit_Xlocation.at(CheckHit)) + (ThisHit_Ylocation - Hit_Ylocation.at(CheckHit))*(ThisHit_Ylocation - Hit_Ylocation.at(CheckHit)) + (ThisHit_Zlocation - Hit_Zlocation.at(CheckHit))*(ThisHit_Zlocation - Hit_Zlocation.at(CheckHit)));
	
	// Then add it to the new vectors if this is below the radius:
	if (Hit_Distance<Sphere_Radius)
	{
	  HitsInSphere_IDNumber.push_back(Hit_IDNumber.at(CheckHit));
	  HitsInSphere_Xlocation.push_back(Hit_Xlocation.at(CheckHit));
	  HitsInSphere_Ylocation.push_back(Hit_Ylocation.at(CheckHit));
	  HitsInSphere_Zlocation.push_back(Hit_Zlocation.at(CheckHit));
	  HitsInSphere_TOF.push_back(Hit_TOF.at(CheckHit));
	  HitsInSphere_BarNr_first.push_back(Hit_BarNr_first.at(CheckHit));
	  HitsInSphere_BarNr_second.push_back(Hit_BarNr_second.at(CheckHit));
	  HitsInSphere_ClusterID.push_back(Hit_ClusterID.at(CheckHit));
	}
      }
    }
    
    // So now we have all the hits inside the sphere. What we now should do is to
    // check their clusterID. If all hits inside this cluster are 0, then the ThisHit
    // belongs to a new cluster and we can assign a ClusterID accordingly. If the clusterID
    // are either zero or 1 other value, then we can assign the same ClusterID to ThisHit
    // as that other value and therebby adding it to the existing cluster. But if there are
    // multiple distinct values different from zero inside this sphere, then we should
    // merge all hits with all those different values to a single value and therefore
    // creating a single cluster out of multiple clusters.
    
    
    
    
    
  }
  
  
  
  
  
  
  
  
  
}