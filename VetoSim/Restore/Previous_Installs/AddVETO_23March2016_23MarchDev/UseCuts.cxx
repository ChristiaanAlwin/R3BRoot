  // Determine amount of neutrons
  // Loop over digis (also compute NeuLAND TOF):
  fNeuLAND_TOF = 1e99;
  sumTotalEnergy=0;   
  R3BLandDigi *digi;
  for(Int_t id = 0; id < fLandDigi->GetEntries(); id++) {
    digi = (R3BLandDigi*) fLandDigi->At(id);
    fEtot += digi->GetQdc();
    sumTotalEnergy += digi->GetQdc();
    fTOF = digi->GetTdc();
    if (fTOF<fNeuLAND_TOF) {fNeuLAND_TOF = fTOF;}
  }
  
  // Determine amount of protons:
  // Loop over veto digis:
  fvetoTOFhits = 0;
  R3Bveto_segDigi *digi_veto;
  fvetoHits = fveto_segDigi->GetEntries();
  for(Int_t id = 0; id < fvetoHits; id++) {
    digi_veto = (R3Bveto_segDigi*) fveto_segDigi->At(id);
    fTOF = digi_veto->GetTdc();
    if (fTOF<fNeuLAND_TOF) {fvetoTOFhits = fvetoTOFhits + 1;}    
  }
  fCurrentProtons = fvetoTOFhits; // FIXME: we treat every hit as a proton!
  
  // NOTE: we cannot access moe protons then e have calibration files for:
  if (fCurrentProtons>fMaxProtons) {fCurrentProtons = fMaxProtons;}
  
  // Double_t neutmult=0.0098379928527*fEtot/beamEnergy*600.;
  Double_t neutmult=0.00960093705145622332*fEtot/beamEnergy*600.;
  nNeut = (Int_t)(neutmult+0.5);
  hNeutmult->Fill(nNeut);

  h_nofclusters->Fill(fNofClusters);
  h_etot->Fill(fEtot);
  h_ncl_etot->Fill(fEtot, fNofClusters);
  h_ndigi_etot->Fill(fEtot, nentries);

  // Apply cuts:
  if (f2DCutEnabled)
  {
    // We have fNofClusters and fEtot which hold the point in the histogram. now we need to decide
    // between which cuts we are. The cuts go from 0 MeV (first) to 1e6 MeV (last). Then the lines
    // are defined as y = fKappa*(fCuts[k] - fEtot) So compute Y for 2 successive cuts and see
    // if fNofClusters is between them. 
    
    fdClusters = (Int_t) fNofClusters;

    for (Int_t k = 0; k<(fNumberOfCuts[fCurrentProtons]-1); ++k)
    {
      Ylow = fKappa[fCurrentProtons]*(fCuts[fCurrentProtons][k] - fEtot);
      Yhigh = fKappa[fCurrentProtons]*(fCuts[fCurrentProtons][k+1] - fEtot);
      if ((Ylow<fdClusters)&&(Yhigh>fdClusters)) {nNeut = k+1;}
    }
  }
  else 
  {
      nNeut = fNNeutrons;
  }
  
  h_ntracks->Fill(nNeut);
  
 