void ProcessClusters(Leaf_Structure* const TheLeafs, Inputs_Structure* const Inputs, Tree_Structure* AnaTree, Int_t const Event, Int_t const Counter, TRandom3* generator)
{
  // This function takes the NeuLAND clusters of the Clusterfinder and puts them into our
  // AnalysisTree. It will only store (t,x,y,z) of the first hit (on TOF basis) in
  // each cluster. It will also store the total energy in the cluster and the trackID
  // of the primary particle responsible for firing the first hit in the cluster. It will do this
  // by fitting x,y,z into a NeuLAND bar and then use the trackID of that bar.
  
  // Output is put into the analysis tree. Therefore no outputs are required. The inputs are the pointers
  // to the 3 structures that hold all our information and the number of the current event.
  // We also pass the TRandom3 generator along as non-=const so that seeds are updated every time. Do not delete it!
  
  // NOTE: AnaTree should NOT be const, otherwise we cannot modify the structure.
  
  // Template is Written by C. A. Douma, for the R3B collaboration
    
  // =============================================================================================
  // Before starting your calculations, the values of the variables holding
  // information that will be put into the tree should be reset. This should be
  // done for single valued leafs and for static arrays:
  
  AnaTree->NeuLAND_Nclusters = 0;
  
  // =============================================================================================
  // NOTE: Do your calculations here. 
  
  // There is not much to do here. This is because the clusterfinder does the work for us. 
  // We can just duplicate the results and put them in our tree. This is all true except for
  // the TrackID and IsVETOed leafs. The TrackID we can only fill however after we retrieved the results.
  // The IsVETOed-leaf is put to kFALSE here by default. It is then filled in another
  // function.
  
  // =============================================================================================
  // Now that the calculations are done, the results have to be put into the Analysis Tree.
  // since some of your leafs might be dynamical arrays, one first has to
  // re-allocate the arrays. Then, a SetBranchAddress should be performed, 
  // because the delete-operations destroys the link to the Analysis Tree. Only after all this,
  // the results of your calculations can be put into the Dynamical arrays. Then this
  // will be filled into the tree automatically. The delete commands that avoid memory leaks
  // should be put into the DeleteEverything function.
  
  // So first define the length of our arrays. NOTE: This length should
  // only be non-zero if the digitizer (and clusterfinder) exists!
  // Then the rest of the function can just be skipped if ArrayLength==0.
  Int_t ArrayLength = 0;
  if ((TheLeafs->Geant3)&&((Inputs->Detectors_NeuLAND_Exists==1)||(Inputs->Detectors_NeuLAND_Exists==2))&&(Inputs->Detectors_NEBULA_Exists!=1)&&(Inputs->Detectors_NebVETO_Exists!=1)) {ArrayLength = TheLeafs->NeuLandCluster_fSize->GetNdata();}
  
  // Then put this into the leaf that holds the size of these dynamical arrays:
  AnaTree->NeuLAND_Nclusters = ArrayLength;
  
  // Then re-allocate the arrays:
  AnaTree->NeuLAND_ClusterX = new Double_t[ArrayLength];
  AnaTree->NeuLAND_ClusterY = new Double_t[ArrayLength];
  AnaTree->NeuLAND_ClusterZ = new Double_t[ArrayLength];
  AnaTree->NeuLAND_ClusterTime = new Double_t[ArrayLength];
  AnaTree->NeuLAND_CStopX = new Double_t[ArrayLength];
  AnaTree->NeuLAND_CStopY = new Double_t[ArrayLength];
  AnaTree->NeuLAND_CStopZ = new Double_t[ArrayLength];
  AnaTree->NeuLAND_CStopTime = new Double_t[ArrayLength];
  AnaTree->NeuLAND_ClusterEnergy = new Double_t[ArrayLength];
  AnaTree->NeuLAND_ClusterSize = new Int_t[ArrayLength];
  AnaTree->NeuLAND_Cluster_TrackID = new Int_t[ArrayLength];
  AnaTree->NeuLAND_Cluster_IsVETOed = new Bool_t[ArrayLength];
  
  // Then re-set the Branch Addresses:
  AnaTree->TheTree->SetBranchAddress("NeuLAND_ClusterX",(void*) AnaTree->NeuLAND_ClusterX);
  AnaTree->TheTree->SetBranchAddress("NeuLAND_ClusterY",(void*) AnaTree->NeuLAND_ClusterY);
  AnaTree->TheTree->SetBranchAddress("NeuLAND_ClusterZ",(void*) AnaTree->NeuLAND_ClusterZ);
  AnaTree->TheTree->SetBranchAddress("NeuLAND_ClusterTime",(void*) AnaTree->NeuLAND_ClusterTime);
  AnaTree->TheTree->SetBranchAddress("NeuLAND_CStopX",(void*) AnaTree->NeuLAND_CStopX);
  AnaTree->TheTree->SetBranchAddress("NeuLAND_CStopY",(void*) AnaTree->NeuLAND_CStopY);
  AnaTree->TheTree->SetBranchAddress("NeuLAND_CStopZ",(void*) AnaTree->NeuLAND_CStopZ);
  AnaTree->TheTree->SetBranchAddress("NeuLAND_CStopTime",(void*) AnaTree->NeuLAND_CStopTime);
  AnaTree->TheTree->SetBranchAddress("NeuLAND_ClusterEnergy",(void*) AnaTree->NeuLAND_ClusterEnergy);
  AnaTree->TheTree->SetBranchAddress("NeuLAND_ClusterSize",(void*) AnaTree->NeuLAND_ClusterSize);
  AnaTree->TheTree->SetBranchAddress("NeuLAND_Cluster_TrackID",(void*) AnaTree->NeuLAND_Cluster_TrackID);
  AnaTree->TheTree->SetBranchAddress("NeuLAND_Cluster_IsVETOed",(void*) AnaTree->NeuLAND_Cluster_IsVETOed);
  
  // Now put the results of your calculations into the arrays you just declared with new:
  
  if (ArrayLength>0)
  {
    for (Int_t k = 0; k<ArrayLength; ++k)
    {
      AnaTree->NeuLAND_ClusterX[k] = TheLeafs->NeuLandCluster_fStartX->GetValue(k);
      AnaTree->NeuLAND_ClusterY[k] = TheLeafs->NeuLandCluster_fStartY->GetValue(k);
      AnaTree->NeuLAND_ClusterZ[k] = TheLeafs->NeuLandCluster_fStartZ->GetValue(k);
      AnaTree->NeuLAND_ClusterTime[k] = TheLeafs->NeuLandCluster_fStartT->GetValue(k);
      AnaTree->NeuLAND_CStopX[k] = TheLeafs->NeuLandCluster_fStopX->GetValue(k);
      AnaTree->NeuLAND_CStopY[k] = TheLeafs->NeuLandCluster_fStopY->GetValue(k);
      AnaTree->NeuLAND_CStopZ[k] = TheLeafs->NeuLandCluster_fStopZ->GetValue(k);
      AnaTree->NeuLAND_CStopTime[k] = TheLeafs->NeuLandCluster_fStopT->GetValue(k);
      AnaTree->NeuLAND_ClusterEnergy[k] = TheLeafs->NeuLandCluster_fE->GetValue(k);
      AnaTree->NeuLAND_ClusterSize[k] = TheLeafs->NeuLandCluster_fSize->GetValue(k);
      AnaTree->NeuLAND_Cluster_TrackID[k] = -1; // we will fix this in a minute.
      AnaTree->NeuLAND_Cluster_IsVETOed[k] = kFALSE; // NOTE: We will do this in another function.
    }
  }
  
    
  // NOTE: The delete commands should be put in the DeleteEverything-function.
  
  // =============================================================================================
  // In principle we are done now. However, we do need to solve for the TrackID. we will
  // do this by deciding in which bar the starting position of the cluster is and take
  // the TrackID of that bar. This means that we need a for-loop over the clusters.
  // Hence first declare what we need:
  TVector3 ClusterStart;
  Double_t clus_x = 0.0;
  Double_t clus_y = 0.0;
  Double_t clus_z = 0.0;
  Int_t NeuPlanes = Inputs->NeuLAND_geo_NsinglePlanes;
  Double_t zlow = 0.0;
  Double_t xlow = 0.0;
  Double_t ylow = 0.0;
  Int_t PlaneNr = 0;
  Int_t BarNr = 0;
  Int_t TotalBarNr = 0;
  Int_t BarIndex = -1;
  Int_t CheckCounter = 0;

  // Then do the for-loop:
  if (ArrayLength>0)
  {
    for (Int_t k = 0; k<ArrayLength; ++k)
    {
      // This procedure means that we first need to transform the coordinates to the frame where NeuLAND
      // has standard orientation and is located at the origin:
      ClusterStart.SetX(AnaTree->NeuLAND_ClusterX[k] - (Inputs->NeuLAND_geo_Xpos));
      ClusterStart.SetY(AnaTree->NeuLAND_ClusterY[k] - (Inputs->NeuLAND_geo_Ypos));
      ClusterStart.SetZ(AnaTree->NeuLAND_ClusterZ[k] - (Inputs->NeuLAND_geo_Zpos));
      
      ClusterStart.RotateZ(-1.0*Inputs->NeuLAND_geo_RotZ);
      ClusterStart.RotateY(-1.0*Inputs->NeuLAND_geo_RotY);
      ClusterStart.RotateX(-1.0*Inputs->NeuLAND_geo_RotX);
      
      clus_x = ClusterStart.X();
      clus_y = ClusterStart.Y();
      clus_z = ClusterStart.Z();
      
      // Now we need to decide in which bar we are. Let us first decide
      // upon the plane.
      PlaneNr = -1;
      BarNr = -1;
      
      zlow = -0.5*5.0*((Int_t) NeuPlanes);
      for (Int_t nplane = 1; nplane<(NeuPlanes+1); ++nplane)
      {
          if ((clus_z>zlow)&&(clus_z<(zlow+5.0))) {PlaneNr = nplane;}
          zlow = zlow + 5.0;
      }
      
      // Then we need to decide upon a horizontal or vertical plane:
      if ((PlaneNr%2)==0)
      {
          // Then we have a vertical plane (We start at 1 = horizontal).
          // So decide upon the bar nr. NOTE: vertical plane means one varies x:
          xlow = -125.0;
          for (Int_t nbar = 1; nbar<51; ++nbar)
          {
            if ((clus_x>xlow)&&(clus_x<(xlow+5.0))) {BarNr = nbar;}
            xlow = xlow + 5.0;
          }
      }
      else
      {
          // same, but now horizontal planes:
          ylow = -125.0;
          for (Int_t nbar = 1; nbar<51; ++nbar)
          {
            if ((clus_y>ylow)&&(clus_y<(ylow+5.0))) {BarNr = nbar;}
            ylow = ylow + 5.0;
          }
      }
      
      // Check for errors:
      if ((BarNr<1)||(PlaneNr<1)||(BarNr>50)||(PlaneNr>(Inputs->NeuLAND_geo_NsinglePlanes)))
      {
          if (Inputs->Bug_Report==4) {cout << "### BUG REPORT: Found a cluster outside NeuLAND! ClusterIndex = " << k << " | Event = " << Event << "\n";}
      }
      
      // Then make it fit inside NeuLAND to prevent errors:
      if (BarNr<1) {BarNr = 1;}
      if (BarNr>50) {BarNr = 50;}
      if (PlaneNr<1) {PlaneNr = 1;}
      if (PlaneNr>(Inputs->NeuLAND_geo_NsinglePlanes)) {PlaneNr = Inputs->NeuLAND_geo_NsinglePlanes;}      
    
      // Then one can now construct the total bar number:
      TotalBarNr = BarNr + 50*(PlaneNr - 1); // NOTE: Both start at 1!
      
      // Now that we have the bar number, we can get the TrackID.      
      // We first need to match the C++ index of this one (k) and of the fired bars:
      
      CheckCounter = 0;
      if (AnaTree->NbarsFired_NeuLAND>0)
      {
        for (Int_t n = 0; n<(AnaTree->NbarsFired_NeuLAND); ++n)
        {
          if (AnaTree->Neu_BarNumber_Fired[n] == TotalBarNr) {AnaTree->NeuLAND_Cluster_TrackID[k] = AnaTree->Neu_Primary_TrackID[n]; CheckCounter = CheckCounter + 1;}
        }
      }
      else {if (Inputs->Bug_Report==4) {cout << "### BUG REPORT: This Cluster doesn't have fired bars! ClusterIndex = " << k << " | Event = " << Event << "\n";}}
      
      // Check again for errors:
      if ((CheckCounter==0)&&(Inputs->Bug_Report==4)) {cout << "### BUG REPORT: This Cluster doesn't have a first bar! ClusterIndex = " << k << " | Event = " << Event << "\n";}
      if ((CheckCounter>1)&&(Inputs->Bug_Report==4)) {cout << "### BUG REPORT: This Cluster has multiple first bars! ClusterIndex = " << k << " | Event = " << Event << "\n";}
      
      // Done! Now the clusters have a matched TrackID.
    }
  }
      
  // =============================================================================================
  // And now we are done. All variables that we declared here without new are lost
  // after this function is complete, and that is how we like it (memory leaks)! The results are
  // NOT lost, since we put them into the arrays that are part of the AnaTree-structure.
  // here we only work with a pointer to this structure, so the results are put
  // in our structure outside this function. Then outside this function, the
  // command tree->Fill() is given and results are stored. You are done now.
}

void Empty_ProcessClusters(Leaf_Structure* const TheLeafs, Inputs_Structure* const Inputs, Tree_Structure* AnaTree, Int_t const Event, Int_t const Counter, TRandom3* generator)
{
  Int_t ArrayLength = 0;
  AnaTree->NeuLAND_Nclusters = ArrayLength;
  
  // Then re-allocate the arrays:
  AnaTree->NeuLAND_ClusterX = new Double_t[ArrayLength];
  AnaTree->NeuLAND_ClusterY = new Double_t[ArrayLength];
  AnaTree->NeuLAND_ClusterZ = new Double_t[ArrayLength];
  AnaTree->NeuLAND_ClusterTime = new Double_t[ArrayLength];
  AnaTree->NeuLAND_CStopX = new Double_t[ArrayLength];
  AnaTree->NeuLAND_CStopY = new Double_t[ArrayLength];
  AnaTree->NeuLAND_CStopZ = new Double_t[ArrayLength];
  AnaTree->NeuLAND_CStopTime = new Double_t[ArrayLength];
  AnaTree->NeuLAND_ClusterEnergy = new Double_t[ArrayLength];
  AnaTree->NeuLAND_ClusterSize = new Int_t[ArrayLength];
  AnaTree->NeuLAND_Cluster_TrackID = new Int_t[ArrayLength];
  AnaTree->NeuLAND_Cluster_IsVETOed = new Bool_t[ArrayLength];
  
  // Then re-set the Branch Addresses:
  AnaTree->TheTree->SetBranchAddress("NeuLAND_ClusterX",(void*) AnaTree->NeuLAND_ClusterX);
  AnaTree->TheTree->SetBranchAddress("NeuLAND_ClusterY",(void*) AnaTree->NeuLAND_ClusterY);
  AnaTree->TheTree->SetBranchAddress("NeuLAND_ClusterZ",(void*) AnaTree->NeuLAND_ClusterZ);
  AnaTree->TheTree->SetBranchAddress("NeuLAND_ClusterTime",(void*) AnaTree->NeuLAND_ClusterTime);
  AnaTree->TheTree->SetBranchAddress("NeuLAND_CStopX",(void*) AnaTree->NeuLAND_CStopX);
  AnaTree->TheTree->SetBranchAddress("NeuLAND_CStopY",(void*) AnaTree->NeuLAND_CStopY);
  AnaTree->TheTree->SetBranchAddress("NeuLAND_CStopZ",(void*) AnaTree->NeuLAND_CStopZ);
  AnaTree->TheTree->SetBranchAddress("NeuLAND_CStopTime",(void*) AnaTree->NeuLAND_CStopTime);
  AnaTree->TheTree->SetBranchAddress("NeuLAND_ClusterEnergy",(void*) AnaTree->NeuLAND_ClusterEnergy);
  AnaTree->TheTree->SetBranchAddress("NeuLAND_ClusterSize",(void*) AnaTree->NeuLAND_ClusterSize);
  AnaTree->TheTree->SetBranchAddress("NeuLAND_Cluster_TrackID",(void*) AnaTree->NeuLAND_Cluster_TrackID);
  AnaTree->TheTree->SetBranchAddress("NeuLAND_Cluster_IsVETOed",(void*) AnaTree->NeuLAND_Cluster_IsVETOed);
}