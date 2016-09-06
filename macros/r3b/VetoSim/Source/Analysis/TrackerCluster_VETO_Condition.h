Int_t LineThroughVETObar(Inputs_Structure* const Inputs, Double_t const ox1, Double_t const oy1, Double_t const oz1, Double_t const ox2, Double_t const oy2, Double_t const oz2, Int_t const VETOlayer)
{
    // Computes through which bar of the VETO this line goes. 
    // NOTE: the bar number will be on the specified VETO layer (starts at 1).
    
    // written by C. A. Douma, for the R3B collaboration.
    
    // =====================================================================
    // Set internal variables:
    Double_t x1 = ox1;
    Double_t y1 = oy1;
    Double_t z1 = oz1;
    Double_t x2 = ox2;
    Double_t y2 = oy2;
    Double_t z2 = oz2;

    // ======================================================================
    // Now we must transform all of these points to standard coordinates.
    // we will go to a system where x=y=z=0 are in the VETO center.
    
    TVector3 point1;
    TVector3 point2;
    
    point1.SetX(x1 - Inputs->VETO_geo_Xpos);
    point1.SetY(y1 - Inputs->VETO_geo_Ypos);
    point1.SetZ(z1 - Inputs->VETO_geo_Zpos);
    point2.SetX(x2 - Inputs->VETO_geo_Xpos);
    point2.SetY(y2 - Inputs->VETO_geo_Ypos);
    point2.SetZ(z2 - Inputs->VETO_geo_Zpos);
    
    point1.RotateZ(-1.0*Inputs->VETO_geo_RotZ);
    point1.RotateY(-1.0*Inputs->VETO_geo_RotY);
    point1.RotateX(-1.0*Inputs->VETO_geo_RotX);
    point2.RotateZ(-1.0*Inputs->VETO_geo_RotZ);
    point2.RotateY(-1.0*Inputs->VETO_geo_RotY);
    point2.RotateX(-1.0*Inputs->VETO_geo_RotX);
    
    // Now we go to a system where the specified layer has its center at z=0:
    point1.SetZ(point1.Z() + 0.5*(Inputs->VETO_geo_TotalBarThickness)*((Int_t) ((Inputs->VETO_geo_NsinglePlanes) - 1)));
    point1.SetZ(point1.Z() - (Inputs->VETO_geo_TotalBarThickness)*((Int_t) (VETOlayer - 1)));
    
    point2.SetZ(point2.Z() + 0.5*(Inputs->VETO_geo_TotalBarThickness)*((Int_t) ((Inputs->VETO_geo_NsinglePlanes) - 1)));
    point2.SetZ(point2.Z() - (Inputs->VETO_geo_TotalBarThickness)*((Int_t) (VETOlayer - 1)));
    
    // re-load into the internal variables:
    x1 = point1.X();
    y1 = point1.Y();
    z1 = point1.Z();
    x2 = point2.X();
    y2 = point2.Y();
    z2 = point2.Z();
    
    // make sure that z1<z2:
    if (z1>z2)
    {
        x2 = point1.X();
        y2 = point1.Y();
        z2 = point1.Z();
        x1 = point2.X();
        y1 = point2.Y();
        z1 = point2.Z();
    }
    
    // ======================================================================
    // Now project a line back to z=0:
    Double_t xproj = 0.0;
    Double_t yproj = 0.0;
    Int_t TheBar = 0;
    
    if (z2<0)
    {
        xproj = x1 + (x2-x1)*TMath::Abs(z1/z2);
        yproj = y1 + (y2-y1)*TMath::Abs(z1/z2);
    }
    else if (z1>0)
    {
        xproj = x2 + (x1-x2)*TMath::Abs(z2/z1);
        yproj = y2 + (y1-y2)*TMath::Abs(z2/z1);
    }
    else
    {
        if (Inputs->Bug_Report==6) {cout << "### BUG REPORT: wrong line projected!\n";}
    }
    
    // Now we must determine whether we have a vertical or horizontal plane:
    if (VETOlayer%2==0)
    {
        // even: vertical layer:
        xproj = xproj + ((Int_t) (Inputs->VETO_geo_NbarsPerPlane))*(Inputs->VETO_geo_TotalBarWidth)*0.5;
        
        // now it starts at 0 at the edge of the detector. divide by size:
        xproj = xproj/(Inputs->VETO_geo_TotalBarWidth);
        
        // between 0 and 1 is in bar 1. between 1 and 2 is in bar 2, etc...
        if (xproj<0.0) {TheBar = 0;}
        else if (xproj>((Int_t) Inputs->VETO_geo_NbarsPerPlane)) {TheBar = 0;}
        else {TheBar = 1 + ((Int_t) xproj);}
        
        // hence it returns zero unless we are actually in the VETO.
        
        // check if yproj is also between the limits:
        if (yproj<-0.5*Inputs->VETO_geo_TotalBarLength) {TheBar = 0;}
        if (yproj>0.5*Inputs->VETO_geo_TotalBarLength) {TheBar = 0;}
        
        // Done!
    }
    else
    {
        // odd: horizontal layer:
        yproj = yproj + ((Int_t) (Inputs->VETO_geo_NbarsPerPlane))*(Inputs->VETO_geo_TotalBarWidth)*0.5;
        
        // now it starts at 0 at the edge of the detector. divide by size:
        yproj = yproj/(Inputs->VETO_geo_TotalBarWidth);
        
        // between 0 and 1 is in bar 1. between 1 and 2 is in bar 2, etc...
        if (yproj<0.0) {TheBar = 0;}
        else if (yproj>((Int_t) Inputs->VETO_geo_NbarsPerPlane)) {TheBar = 0;}
        else {TheBar = 1 + ((Int_t) yproj);}
        
        // hence it returns zero unless we are actually in the VETO.
        
        // check if yproj is also between the limits:
        if (xproj<-0.5*Inputs->VETO_geo_TotalBarLength) {TheBar = 0;}
        if (xproj>0.5*Inputs->VETO_geo_TotalBarLength) {TheBar = 0;}
        
        // Done!
    }
    
    // Now we can return the answer:
    return TheBar;
}

void TrackerCluster_VETO_Condition(Leaf_Structure* const TheLeafs, Inputs_Structure* const Inputs, Tree_Structure* AnaTree, Int_t const Event, Int_t const Counter, TRandom3* generator)
{
  // This function performs the actual VETO condition. There are multiple versions of this function
  // available. This one uses the results from the Neutron tracker. However, contary to the normal
  // tracker VETO condition, it uses the lines from the clusters to connect reconstructed prim. interaction
  // points to the VETO hits, not a line from the target. The leafs that should be filled by the VETO
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
    
  // FIXME: Implement comparison between VETO hits and NeutHits. Implement Prim_E_Detected!
    
  // =============================================================================================
  // NOTE: Nothing needs to be re-declared, deleted, etc. This is done in other functions!
  // =============================================================================================
  // The purpose of this function is to VETO the primary particles based on the R3BNeutronTracker
  // Reconstructed primary interaction points. Note however that if the Tracker does not exist,
  // there is no point in running this function. 
    
  // First we reset everything so that nothing is VETOed:
  Int_t NbarsFired = AnaTree->NbarsFired_NeuLAND;
  for (Int_t k = 0; k<NbarsFired; ++k)
  {
    AnaTree->Neu_Bar_IsVETOed[k] = kFALSE;
  }
  
  if (Inputs->Store_AllBars==1)
  {
     Int_t NbarsNeu = (Inputs->NeuLAND_geo_NsinglePlanes)*50;
     for (Int_t k = 0; k<NbarsNeu; ++k)
     {
        AnaTree->AllBars_IsVETOed[k] = kFALSE;
     }
  }
  
  if (AnaTree->NeuLAND_Nclusters>0)
  {
      for (Int_t k = 0; k<(AnaTree->NeuLAND_Nclusters); ++k)
      {
          AnaTree->NeuLAND_Cluster_IsVETOed[k] = kFALSE;
      }
  }
      
  Int_t Nprims = AnaTree->Nprims_NeuLAND;
  for (Int_t k = 0; k<Nprims; ++k)
  {
      AnaTree->Prim_IsVETOed[k] = kFALSE;
  }
  
  // Then now test if we have to change some stuff. Only if the tracker is there.
  // Otherwise we simply do not do anything!
  if (TheLeafs->Tracker_Exists)
  {
      // Now we can start. We begin by looping over all the fired VETO bars.
      // This is because we will need to VETO one vertex for each bar that has fired.
      
      // First declare all variables that we need:
      Int_t NbarsFired = AnaTree->NbarsFired_VETO;
      Int_t Nclusters = AnaTree->NeuLAND_Nclusters;
      Int_t Nvertex = AnaTree->N_Reconstructed_PrimPoints;
      Int_t Nprims = AnaTree->Nprims_NeuLAND;
      Int_t VETObarNr = 0;
      Int_t BarOfLine = 0;
      Int_t ClusNo = 0;
      Int_t NClustersThroughBar = 0;
      Int_t Layer = 0;
      Int_t kmin = 0;
      
      Double_t VETObarTOF = 0.0;
      Double_t xstart = 0.0;
      Double_t ystart = 0.0;
      Double_t zstart = 0.0;
      Double_t tstart = 0.0;
      Double_t xstop = 0.0;
      Double_t ystop = 0.0;
      Double_t zstop = 0.0;
      Double_t tstop = 0.0;
      Double_t tvertex = 0.0;
      Double_t tpredict = 0.0;
      Double_t cvac = 29.9792458; // NOTE: Units!!! [cm]\[ns]
      Double_t ceff = Inputs->EffectiveC;
      Double_t MinDist = 0.0;
      Double_t theta = 0.0;
      Double_t phi = 0.0;
      Double_t SpaceTimeDist = 0.0;
      
      Bool_t ClusterThroughBar = kFALSE;
      
      TVector3 ThisPoint;
      TVector3 PrevPoint;
      TVector3 StartPoint;
      TVector3 StopPoint;
      TVector3 VertexPoint;
      
      // Check if we can make all of our loops:
      if ((NbarsFired>0)&&(Nclusters>0)&&(Nvertex>0))
      {
          for (Int_t kveto = 0; kveto<NbarsFired; ++kveto)
          {
              // The first thing we do is to get the TOF of this bar:
              VETObarTOF = AnaTree->VETO_TOF_Fired_Res[kveto];
              
              // Also get the barnumber:
              VETObarNr = AnaTree->VETO_BarNumber_Fired[kveto];
              
              // Next, we need to loop over the NeuLAND clusters.
              // We need to know if there is a cluster in the first layer
              // of NeuLAND that tracks back to this bar. Reset:
              ClusterThroughBar = kFALSE;
              NClustersThroughBar = 0;
              ClusNo = -1;
              
              // Find the VETO layer of this bar:
              Layer = 0;
              while (VETObarNr>0)
              {
                  Layer = Layer + 1;
                  VETObarNr = VETObarNr - (Inputs->VETO_geo_NbarsPerPlane);
              }
              // NOTE: Layer is always at least 1, as it should! Reset VETObarNr:
              VETObarNr = AnaTree->VETO_BarNumber_Fired[kveto];
              
              // Loop:
              for (Int_t kcluster = 0; kcluster<Nclusters; ++kcluster)
              {
                  // Get the start & stop position of that cluster:
                  xstart = AnaTree->NeuLAND_ClusterX[kcluster];
                  ystart = AnaTree->NeuLAND_ClusterY[kcluster];
                  zstart = AnaTree->NeuLAND_ClusterZ[kcluster];
                  
                  xstop = AnaTree->NeuLAND_CStopX[kcluster];
                  ystop = AnaTree->NeuLAND_CStopY[kcluster];
                  zstop = AnaTree->NeuLAND_CStopZ[kcluster];
                  
                  // Only clusters of size>1 should be taken along:
                  if (AnaTree->NeuLAND_ClusterSize[kcluster]>1)
                  {
                  
                    // check if it goes through this VETO bar:
                    BarOfLine = LineThroughVETObar(Inputs,xstart,ystart,zstart,xstop,ystop,zstop,Layer);
                  
                    if (BarOfLine==VETObarNr)
                    {
                      // In This case we know we should keep this cluster.
                      // However, we do not know for sure that there is
                      // only one cluster available. So what do we need
                      // to do if there are more then 1 clusters? Then we should
                      // take the best one.
                    
                      // Check for the boolian ClusterThroughBar:
                      if (ClusterThroughBar)
                      {
                          // then we already found a cluster, check if this
                          // one is better. We check on its starting position.
                          // Hence transform x,y,z to the NeuLAND standard frame.
                          // Do this both for this cluster and for the previous one!
                          ThisPoint.SetX(xstart - Inputs->NeuLAND_geo_Xpos);
                          ThisPoint.SetY(ystart - Inputs->NeuLAND_geo_Ypos);
                          ThisPoint.SetZ(zstart - Inputs->NeuLAND_geo_Zpos);
                          
                          PrevPoint.SetX(AnaTree->NeuLAND_ClusterX[ClusNo] - Inputs->NeuLAND_geo_Xpos);
                          PrevPoint.SetY(AnaTree->NeuLAND_ClusterY[ClusNo] - Inputs->NeuLAND_geo_Ypos);
                          PrevPoint.SetZ(AnaTree->NeuLAND_ClusterZ[ClusNo] - Inputs->NeuLAND_geo_Zpos);
                          
                          ThisPoint.RotateZ(-1.0*Inputs->NeuLAND_geo_RotZ);
                          ThisPoint.RotateY(-1.0*Inputs->NeuLAND_geo_RotY);
                          ThisPoint.RotateX(-1.0*Inputs->NeuLAND_geo_RotX);
                          
                          PrevPoint.RotateZ(-1.0*Inputs->NeuLAND_geo_RotZ);
                          PrevPoint.RotateY(-1.0*Inputs->NeuLAND_geo_RotY);
                          PrevPoint.RotateX(-1.0*Inputs->NeuLAND_geo_RotX);
                          
                          // Now in this frame, we define a cluster to be better if its zstart is smaller:
                          if (ThisPoint.Z()<PrevPoint.Z())
                          {
                              // Replace our choice of the cluster:
                              ClusNo = kcluster;
                              
                              // Update counter:
                              NClustersThroughBar = NClustersThroughBar + 1;
                              
                              // No need to update the boolian, it is already true...
                          }
                          
                          // That's it!
                      }
                      else   
                      {
                          // No cluster is selected yet, so we will be forced to
                          // take this cluster:
                          ClusterThroughBar = kTRUE;
                          NClustersThroughBar = NClustersThroughBar + 1;
                          ClusNo = kcluster;
                      }
                  
                    // that should do the trick!
                  }
                }
              }
              
              // Now that we have a cluster that tracks through the fired bar, we can loop over reconstructed vertices.
              // the vertex lying closest to this line should be VETOed. Hence loop:
              MinDist = 1e99;
              kmin = -1;
              
              for (Int_t kvertex = 0; kvertex<Nvertex; ++kvertex)
              {
                  // Load the startpoint:
                  StartPoint.SetX(AnaTree->NeuLAND_ClusterX[ClusNo]);
                  StartPoint.SetY(AnaTree->NeuLAND_ClusterY[ClusNo]);
                  StartPoint.SetZ(AnaTree->NeuLAND_ClusterZ[ClusNo]);
                  tstart = AnaTree->NeuLAND_ClusterTime[ClusNo];
                  
                  // load the stop point:
                  StopPoint.SetX(AnaTree->NeuLAND_CStopX[ClusNo]);
                  StopPoint.SetY(AnaTree->NeuLAND_CStopY[ClusNo]);
                  StopPoint.SetZ(AnaTree->NeuLAND_CStopZ[ClusNo]);
                  tstop = AnaTree->NeuLAND_CStopTime[ClusNo];
                  
                  // load the vertex:
                  VertexPoint.SetX(AnaTree->X_Reconstructed_PrimPoints[kvertex]);
                  VertexPoint.SetY(AnaTree->Y_Reconstructed_PrimPoints[kvertex]);
                  VertexPoint.SetZ(AnaTree->Z_Reconstructed_PrimPoints[kvertex]);
                  tvertex = AnaTree->T_Reconstructed_PrimPoints[kvertex];
              
                  // Shift to a frame where the startpoint is at the origin:
                  VertexPoint.SetX(VertexPoint.X() - StartPoint.X());
                  VertexPoint.SetX(VertexPoint.Y() - StartPoint.Y());
                  VertexPoint.SetX(VertexPoint.Z() - StartPoint.Z());
                  
                  StopPoint.SetX(StopPoint.X() - StartPoint.X());
                  StopPoint.SetX(StopPoint.Y() - StartPoint.Y());
                  StopPoint.SetX(StopPoint.Z() - StartPoint.Z());                  
                  
                  StartPoint.SetX(StartPoint.X() - StartPoint.X());
                  StartPoint.SetX(StartPoint.Y() - StartPoint.Y());
                  StartPoint.SetX(StartPoint.Z() - StartPoint.Z());
                  
                  // Rotate such that the stop point is parallel to
                  // the z-axis:
                  theta = StopPoint.Theta();
                  phi = StopPoint.Phi();
                  
                  VertexPoint.RotateZ(-1.0*phi);
                  VertexPoint.RotateY(-1.0*theta);
                  
                  StopPoint.RotateZ(-1.0*phi);
                  StopPoint.RotateY(-1.0*theta);
                  
                  // Check if this worked:
                  if (TMath::Abs(StopPoint.X())>0.001) {if (Inputs->Bug_Report==6) {cout << "### BUG REPORT: Rotatie of the clusterline Failed!\n";}}
                  if (TMath::Abs(StopPoint.Y())>0.001) {if (Inputs->Bug_Report==6) {cout << "### BUG REPORT: Rotatie of the clusterline Failed!\n";}}
                  
                  // Then one can compute the time difference between the cluster line and the vertex:
                  tpredict = tstart + (tstop-tstart)*(VertexPoint.Z()/StopPoint.Z());
                  
                  // If the vertex would be on the line, the tpredict would be the matching time of the cluster. Hence:
                  SpaceTimeDist = (VertexPoint.X())*(VertexPoint.X()) + (VertexPoint.Y())*(VertexPoint.X()) + ceff*ceff*(tpredict - tvertex)*(tpredict - tvertex);
                  SpaceTimeDist = TMath::Sqrt(SpaceTimeDist);
                  
                  // Now compare with other vertices:
                  if (SpaceTimeDist<MinDist) {MinDist = SpaceTimeDist; kmin = kvertex;}
              }
                  
              // Then now in the end, we know that we should VETO vertex labeled kmin.
              // But first we need to check the time differences:
              if (AnaTree->T_Reconstructed_PrimPoints[kmin]>VETObarTOF) {AnaTree->Reconstructed_PrimPoints_IsVETOed[kmin] = kTRUE;}
              // if (kmin>-1) {AnaTree->Reconstructed_PrimPoints_IsVETOed[kmin] = kTRUE;}
              
              // Now we VETOed the right vertices. We can now end our loop over VETO fired bars:
          }
          
          // Now the last thing we should do is to propagate the VETOed vertices to the primary particles:
          for (Int_t kprim = 0; kprim<Nprims; ++kprim)
          {
              for (Int_t kvertex = 0; kvertex<Nvertex; ++kvertex)
              {
                  if (AnaTree->TrackID_Reconstructed_PrimPoints[kvertex]==AnaTree->Prim_TrackID[kprim])
                  {
                      if (AnaTree->Reconstructed_PrimPoints_IsVETOed[kvertex])
                      {
                          AnaTree->Prim_IsVETOed[kprim] = kTRUE;
                      }
                  }
              }
          }
          
          // Done! Now close all blocks:
      }
  }
}
                  

      