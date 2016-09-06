void R3BNeuLANDTracker::Compare()
{
  // This function makes a comparison between the reconstructed Vertices and the
  // Monte Carlo points of the original data. One can then easily see how
  // well the reconstruction went.
    
  // In order to do this, one first needs to know how many vertices and primary
  // points we have:
  Int_t Nvertices = fArrayVertices->GetEntries();
  Int_t NMCpoints = fArrayNeuPrimPoints->GetEntries();
  
  // We will lionk the points basd on their minimum spacetime distance.
  // hence create an array with those distances:
  Double_t** Spacetime_Dist = new Double_t*[Nvertices];
  
  for (Int_t kvertex = 0; kvertex<Nvertices; ++kvertex)
  {
      Spacetime_Dist[kvertex] = new Double_t[NMCpoints];
      
      for (Int_t kMC = 0; kMC<NMCpoints; ++kMC)
      {
          Spacetime_Dist[kvertex][kMC] = 0.0;
      }
  }
  
  // Now loop over all vertices and compute the spacetime distances. First declare what we need:
  Double_t ceff = 14.0;
  R3BNeuLANDVertex* Vertex;
  FairMCPoint* NeuPrimPoint;
  R3BMCTrack* Track;
  
  // Then loop over the vertices:
  for (Int_t kvertex = 0; kvertex<Nvertices; ++kvertex)
  {
      // load the vertex:
      Vertex = (R3BNeuLANDVertex*) fArrayVertices->At(kvertex);
      
      // Set that ist has been compared:
      Vertex->PointIsCompared();
      
      // Now loop over the MC points:
      for (Int_t kMC = 0; kMC<NMCpoints; ++kMC)
      {
          // Load the MC point:
          NeuPrimPoint = (FairMCPoint*) fArrayNeuPrimPoints->At(kMC);
          
          // Compute the spacetime distance:
          Spacetime_Dist[kvertex][kMC] = Spacetime_Dist[kvertex][kMC] + (Vertex->GetRecX() - NeuPrimPoint->GetX())*(Vertex->GetRecX() - NeuPrimPoint->GetX());
          Spacetime_Dist[kvertex][kMC] = Spacetime_Dist[kvertex][kMC] + (Vertex->GetRecY() - NeuPrimPoint->GetY())*(Vertex->GetRecY() - NeuPrimPoint->GetY());
          Spacetime_Dist[kvertex][kMC] = Spacetime_Dist[kvertex][kMC] + (Vertex->GetRecZ() - NeuPrimPoint->GetZ())*(Vertex->GetRecZ() - NeuPrimPoint->GetZ());
          Spacetime_Dist[kvertex][kMC] = Spacetime_Dist[kvertex][kMC] + ceff*ceff*(Vertex->GetRecTime() - NeuPrimPoint->GetTime())*(Vertex->GetRecTime() - NeuPrimPoint->GetTime());
          
          Spacetime_Dist[kvertex][kMC] = TMath::Sqrt(Spacetime_Dist[kvertex][kMC]);
      }
  }
  
  // Now we must find the overall minimum. Declare what we need:
  Double_t Min = 1e98;
  Int_t VertexMin = -1;
  Int_t MCMin = -1;
  Int_t NLink = Nvertices;
  if (Nvertices>NMCpoints) {NLink = NMCpoints;}
  
  // Link the points one-by-one. We can of course only link NLink-points at most:
  for (Int_t klink = 0; klink<NLink; ++klink)
  {
      // Reset our minimum searchers:
      Min = 1e98;
      VertexMin = -1;
      MCMin = -1;
      
      // Loop over the array to find the minimum:
      for (Int_t kvertex = 0; kvertex<Nvertices; ++kvertex)
      {
          for (Int_t kMC = 0; kMC<NMCpoints; ++kMC)
          {
              if (Spacetime_Dist[kvertex][kMC]<Min)
              {
                  Min = Spacetime_Dist[kvertex][kMC];
                  VertexMin = kvertex;
                  MCMin = kMC;
              }
          }
      }
      
      // This means that we can now link VertexMin & MCMin:
      Vertex = (R3BNeuLANDVertex*) fArrayVertices->At(VertexMin);
      NeuPrimPoint = (FairMCPoint*) fArrayNeuPrimPoints->At(MCMin);
      
      // Load the MC-data into the vertex:
      Vertex->SetMCPoint(NeuPrimPoint->GetTime(), NeuPrimPoint->GetX(), NeuPrimPoint->GetY(), NeuPrimPoint->GetZ());
      Vertex->PointHasMC();
      Vertex->SetTrackID(NeuPrimPoint->GetTrackID());
      
      // Load the data from the MCTracks:
      Track = (R3BMCTrack*) fArrayMCTracks->At(NeuPrimPoint->GetTrackID());
      Vertex->SetPDG(Track->GetPdgCode());
      
      // Compute the Monte Carlo Energy. Notice the conversion from GeV to MeV:
      Vertex->SetMCE(1000.0*(TMath::Sqrt(Track->GetPx()*Track->GetPx() + Track->GetPy()*Track->GetPy() + Track->GetPz()*Track->GetPz() + Track->GetMass()*Track->GetMass()) - Track->GetMass()));
      
      // That's it.
  }
      
  // Now the final step in the comparison: check whether we reconstructed
  // the entire event properly:
  Bool_t fSucces = kTRUE;
  
  if (Nvertices!=NMCpoints) {fSucces = kFALSE;}
  
  if (fSucces)
  {
      for (Int_t kvertex = 0; kvertex<Nvertices; ++kvertex)
      {
          Vertex = (R3BNeuLANDVertex*) fArrayVertices->At(kvertex);
          
          if (Vertex->GetMCPDG()!=2112) {fSucces = kFALSE;}
      }
  }
  
  // Now load the succes into each vertex:
  for (Int_t kvertex = 0; kvertex<Nvertices; ++kvertex)
  {
      Vertex = (R3BNeuLANDVertex*) fArrayVertices->At(kvertex);
        
      if (fSucces) {Vertex->EventSucceeded();}
      else {Vertex->EventFailed();}
  }
      
  // I gues that is all one can do for the comparison of the reconstruction...
}
