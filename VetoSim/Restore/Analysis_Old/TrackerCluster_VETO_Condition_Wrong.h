static bool sort_using_greater_than(double u, double v) {return u > v;}

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
      if (Inputs->Bug_Report==6) {cout << "### BUG REPORT: The tracker exists!\n";}
      
      // Now we should do the actual VETOing. We have the reconstructed
      // Primary interaction points available. From the VETO digitizer
      // we also have the points of the VETO interactions available.
      // Hence we must now somehow compare them and then decide which
      // points to VETO.
      
      // We also have the clusters available. Hence our strategy is now the following:
      // the cluster start and stop define a line (and a Delta t as well). we can first
      // sorth the clusters by energy size. Then, we will start with the biggest E cluster.
      // we check if it has 1) a starting point in the first layer of NeuLAND and 2) it has
      // its line pointing through a VETO bar that fired BEFORE NeuLAND, we do something
      // with it, else we ignore the cluster. If we do something, we VETO the reconstructed
      // primary interaction point closest to that line.
      
      // first determine that there are clusters:
      Int_t NClusters = AnaTree->NeuLAND_Nclusters;
      
      // Then set a VETO counter:
      Int_t VETOcounter = 0;
      
      // also find the number of fired bars in the VETO that have a smaller TOF the NeuLAND:
      Int_t NbarsFired_OnTime = 0;
      for (Int_t kTOF = 0; kTOF<(AnaTree->NbarsFired_VETO); ++kTOF)
      {
          if (AnaTree->VETO_TOF_Fired_Res[kTOF]<AnaTree->Neu_TotalTOF_Res) {NbarsFired_OnTime = NbarsFired_OnTime + 1;}
      }
      
      // Now we can actually start:
      if (NClusters>0)
      {
        // Then declare what we need.
        TVector3 StartPoint;
        TVector3 StopPoint;
        TVector3 Vertex;
        
        Double_t xstart = 0.0;
        Double_t ystart = 0.0;
        Double_t zstart = 0.0;
        Double_t tstart = 0.0;
        
        Double_t xstop = 0.0;
        Double_t ystop = 0.0;
        Double_t zstop = 0.0;
        Double_t tstop = 0.0;
        
        Double_t xvertex = 0.0;
        Double_t yvertex = 0.0;
        Double_t zvertex = 0.0;
        Double_t tvertex = 0.0;
        
        Double_t plane_lowBoundary = 0.0;
        Double_t plane_upBoundary = 0.0;
        Double_t TOFbar = 0.0;
        Double_t phi = 0.0;
        Double_t theta = 0.0;
        Double_t beta = 0.0;
        Double_t tpredict = 0.0;
        Double_t MinDist = 0.0;
        Double_t SpaceTimeDist = 0.0;
        
        Double_t c = 29.9792458; // NOTE: Units!!! [cm]\[ns]
        c = Inputs->EffectiveC;
        
        Int_t NvetoPlanes = Inputs->VETO_geo_NsinglePlanes;
        Int_t NVertices = 0;
        Int_t kminDist = 0;
        Int_t LocalBarNr = 0;
        Int_t GlobalBarNr = 0;
        
        Bool_t VETOsomething = kFALSE;
          
        // =====================================================================
        // Now before we begin, sort all clusters in energy:
        std::vector<Double_t> Ecluster;
        Ecluster.clear();
      
        for (Int_t k = 0; k<NClusters; ++k)
        {
            Ecluster.push_back(AnaTree->NeuLAND_ClusterEnergy[k]);
        }
        
        std::sort(Ecluster.begin(), Ecluster.end(), sort_using_greater_than);
        
        // This will put the highest cluster energy at the first index and so on.
        
        // ======================================================================
        // Now loop over all clusters:
        Int_t kCluster = 0;
        
        for (Int_t k1 = 0; k1<NClusters; ++k1)
        {
            kCluster = 0;
            
            for (Int_t k2 = 0; k2<NClusters; ++k2)
            {
                if (TMath::Abs(Ecluster.at(k1) - AnaTree->NeuLAND_ClusterEnergy[k2])<0.001) // 1 keV!
                {
                    kCluster = k2;
                }
            }
            
            // now use kCluster while looping over k1! In that way, we loop from high energy to
            // low energy!
            
            // test if kCluster is in the first layer of NeuLAND:
            xstart = AnaTree->NeuLAND_ClusterX[kCluster];
            ystart = AnaTree->NeuLAND_ClusterY[kCluster];
            zstart = AnaTree->NeuLAND_ClusterZ[kCluster];
            
            StartPoint.SetX(xstart - Inputs->NeuLAND_geo_Xpos);
            StartPoint.SetY(ystart - Inputs->NeuLAND_geo_Ypos);
            StartPoint.SetZ(zstart - Inputs->NeuLAND_geo_Zpos);
            
            StartPoint.RotateZ(-1.0*Inputs->NeuLAND_geo_RotZ);
            StartPoint.RotateY(-1.0*Inputs->NeuLAND_geo_RotY);
            StartPoint.RotateX(-1.0*Inputs->NeuLAND_geo_RotX);
            
            xstart = StartPoint.X();
            ystart = StartPoint.Y();
            zstart = StartPoint.Z();
            
            plane_lowBoundary = -2.5*((Int_t) Inputs->NeuLAND_geo_NsinglePlanes);
            plane_upBoundary = plane_lowBoundary + 5.0;
            
            if ((zstart>plane_lowBoundary)&&(zstart<plane_upBoundary))
            {
            // then this cluster starts at the first layer. so we can use it.
            // otherwise, we will ignore the cluster.
                
            // Also important: Test if the cluster has at least size 2. Else you cannot
            // project a decent line:
            if (AnaTree->NeuLAND_ClusterSize[kCluster]>1)
            {
                // Now we can go for it:            
                
                // Compute the line through the cluster. Use global coordinates:
                xstart = AnaTree->NeuLAND_ClusterX[kCluster];
                ystart = AnaTree->NeuLAND_ClusterY[kCluster];
                zstart = AnaTree->NeuLAND_ClusterZ[kCluster];
                tstart = AnaTree->NeuLAND_ClusterTime[kCluster];
                
                xstop = AnaTree->NeuLAND_CStopX[kCluster];
                ystop = AnaTree->NeuLAND_CStopY[kCluster];
                zstop = AnaTree->NeuLAND_CStopZ[kCluster];
                tstop = AnaTree->NeuLAND_CStopTime[kCluster];
                
                // Find the barnumber in the VETO planes where this line passes
                // through. loop over all planes to test all bars.
                
                // reset variables:
                TOFbar = 1e99; // (start high, so if no bar is found, nothing is VETOed!)
                VETOsomething = kFALSE;
                
                if (NvetoPlanes>0)
                {
                    for (Int_t kplane = 0; kplane<NvetoPlanes; ++kplane)
                    {
                        // get the bar number this line goes though:
                        LocalBarNr = LineThroughVETObar(Inputs,xstart,ystart,zstart,xstop,ystop,zstop,kplane+1); // We start at 1.
                        
                        // Convert to a global bar nr:
                        GlobalBarNr = LocalBarNr + kplane*(Inputs->VETO_geo_NbarsPerPlane); // kplane starts at 0.
                        
                        // loop over all fired bars and see if this bar is in between.
                        // if that is the case, get the TOF of that bar:                        
                        if (AnaTree->NbarsFired_VETO>0)
                        {
                            for (Int_t kveto = 0; kveto<(AnaTree->NbarsFired_VETO); ++kveto)
                            {
                                // match bar numbers:
                                if (AnaTree->VETO_BarNumber_Fired[kveto] == GlobalBarNr)
                                {
                                    // then the bar fired. now check the TOF:
                                    if (AnaTree->VETO_TOF_Fired_Res[kveto]<AnaTree->Neu_TotalTOF_Res)
                                    {
                                        // then the TOF condition is satisfied.
                                        
                                        // make sure we get the smallest TOF for all planes:
                                        if (TOFbar>AnaTree->VETO_TOF_Fired_Res[kveto]) {TOFbar = AnaTree->VETO_TOF_Fired_Res[kveto];}
                                        
                                        // make sure that this cluster should do something:
                                        VETOsomething = kTRUE;
                                    }
                                }
                            }
                        }
                        
                        // Now we checked if the line of this cluster goes through a fired VETO bar.
                        // The bioolian VETOsomething holds the result. So now we can move on.
                    }
                }
                
                // Now we have to actually VETO a vertex. we will take the cluster start and stop point
                // and draw a line through this. The vertex closest to this line will be VETOed.
                
                // reset variables:
                NVertices = AnaTree->N_Reconstructed_PrimPoints;
                kminDist = -2;
                MinDist = 1e99;
                
                if (NVertices>0)
                {
                    for (Int_t kvertex = 0; kvertex<NVertices; ++kvertex)
                    {
                        // load the vertex:
                        xvertex = AnaTree->X_Reconstructed_PrimPoints[kvertex];
                        yvertex = AnaTree->Y_Reconstructed_PrimPoints[kvertex];
                        zvertex = AnaTree->Z_Reconstructed_PrimPoints[kvertex];
                        tvertex = AnaTree->T_Reconstructed_PrimPoints[kvertex];
                        
                        // The two clusterpoints have been loaded already.
                        
                        // load the points into TVector3:
                        StartPoint.SetX(xstart);
                        StartPoint.SetY(ystart);
                        StartPoint.SetZ(zstart);
                        
                        StopPoint.SetX(xstop);
                        StopPoint.SetY(ystop);
                        StopPoint.SetZ(zstop);
                        
                        Vertex.SetX(xvertex);
                        Vertex.SetY(yvertex);
                        Vertex.SetZ(zvertex);
                        
                        // shift to a frame where the startpoint is in the origin:
                        StopPoint.SetX(StopPoint.X() - StartPoint.X());
                        StopPoint.SetY(StopPoint.Y() - StartPoint.Y());
                        StopPoint.SetZ(StopPoint.Z() - StartPoint.Z());
                        
                        Vertex.SetX(Vertex.X() - StartPoint.X());
                        Vertex.SetY(Vertex.Y() - StartPoint.Y());
                        Vertex.SetZ(Vertex.Z() - StartPoint.Z());
                        
                        StartPoint.SetX(StartPoint.X() - StartPoint.X());
                        StartPoint.SetY(StartPoint.Y() - StartPoint.Y());
                        StartPoint.SetZ(StartPoint.Z() - StartPoint.Z());
                        
                        // Check for the shift:
                        if ((TMath::Abs(StartPoint.X())>0.001)||(TMath::Abs(StartPoint.Y())>0.001)||(TMath::Abs(StartPoint.Z())>0.001))
                        {
                          if (Inputs->Bug_Report==6) {cout << "### ERROR: The shifting of the line went wrong! Event = " << Event << " | Index Cluster = " << k1 << " | Vertex = " << kvertex << "\n";}
                        }
                        
                        // rotate so that the line is parallel to the z-axis:
                        phi = StopPoint.Phi();
                        theta = StopPoint.Theta();
                        
                        Vertex.RotateZ(-1.0*phi);
                        StopPoint.RotateZ(-1.0*phi);
                        
                        Vertex.RotateY(-1.0*theta);
                        StopPoint.RotateY(-1.0*theta);
                        
                        // Check the rotation:
                        if ((TMath::Abs(StopPoint.X())>0.001)||(TMath::Abs(StopPoint.Y())>0.001)||(StopPoint.Z()<0))
                        {
                          if (Inputs->Bug_Report==6) {cout << "### ERROR: The rotation of the line went wrong! Event = " << Event << " | Index Cluster = " << k1 << " | Vertex = " << kvertex << "\n";}
                        }
                        
                        // Now we can compute the distance to the line. But we also need some way of predicting the time.
                        // we know tvertex. But we need to extrapolate tstart to the position of zvertex:                        
                        beta = StopPoint.Z()/(c*(tstop-tstart));
                        tpredict = tstart + beta*zvertex;
                        
                        // Now we can compute spacetime distance to the line:
                        SpaceTimeDist = 0.0;
                        SpaceTimeDist = SpaceTimeDist + (Vertex.X())*(Vertex.X());
                        SpaceTimeDist = SpaceTimeDist + (Vertex.Z())*(Vertex.Y());
                        SpaceTimeDist = SpaceTimeDist + c*c*(tpredict-tvertex)*(tpredict-tvertex);
                        SpaceTimeDist = TMath::Sqrt(SpaceTimeDist);
                        
                        // search for the minimum:
                        if (MinDist>SpaceTimeDist)
                        {
                            MinDist = SpaceTimeDist;
                            kminDist = kvertex;
                        }
                    }
                }
                
                // At this point we know that the vertex kminDist is closest to the line through our cluster.
                // the boolian VETOsomething tells us if the line goes through a correct VETO bar, hence if
                // we actually should VETO something. Now the last condition that we should impose is
                // that we should never VETO more vertices then the number of bars in the VETO that fired BEFORE
                // NeuLAND.
                
                // so now apply the VETO condition:
                if ((VETOcounter<NbarsFired_OnTime)&&(VETOsomething)&&(kminDist>-1))
                {
                    AnaTree->Reconstructed_PrimPoints_IsVETOed[kminDist] = kTRUE;
                    VETOcounter = VETOcounter + 1;
                }
                
                // Notice that this counter prevents that too many vertices are VETOed. One might have 
                // more then 1 cluster per fired VETO bar in the first plane of NeuLAND. By sorting
                // the clusters on energy and then applying this counter, one will always use the 
                // most energetic clusters for the VETO condition.
                
                // So now that we decided which vertices to be VETOed, one can finish up the cluster loop:
            }
            }
        }
      }
      
      // Now all that we need to do is to communicate the VETOed vertices to VETOed particles:
      Int_t Nrec = AnaTree->N_Reconstructed_PrimPoints;
      Int_t Nprims = AnaTree->Nprims_NeuLAND;
      
      if ((Nrec>0)&&(Nprims>0))
      {
        for (Int_t krec = 0; krec<Nrec; ++krec)
        {
        for (Int_t kprim = 0; kprim<Nprims; ++kprim)
        {
            if (AnaTree->Prim_TrackID[kprim]==AnaTree->TrackID_Reconstructed_PrimPoints[krec])
            {
               if (AnaTree->Reconstructed_PrimPoints_IsVETOed[krec]) 
               {
                   AnaTree->Prim_IsVETOed[kprim] = kTRUE;
               }
            }
        }
        }
      }
      
      // Then also compute the reconstructed energy. But this is done already in process tracker.
      // So we are done here!
     
      // Then we did all that we had to do. Close all blocks and finish up:
  }
}      