#include "TMath.h"
#include "TH1D.h"
#include "TH2D.h"

void Secondary_Particles(Leaf_Structure* const TheLeafs, Inputs_Structure* const Inputs, Tree_Structure* AnaTree, Int_t const Event, Int_t const Counter, TRandom3* generator)
{
  // This function identifies the secondary particles within a single event and puts the results
  // into the analysis tree. Therefore no outputs are required. The inputs are the pointers
  // to the 3 structures that hold all our information and the number of the current event.
  // Also some overall histograms are filled.
  // We also pass the TRandom3 generator along as non-=const so that seeds are updated every time.
  
  // NOTE: AnaTree should NOT be const, otherwise we cannot modify the structure.
  
  // Template is Written by C. A. Douma, for the R3B collaboration
    
  // =============================================================================================
  // Before starting your calculations, the values of the variables holding
  // information that will be put into the tree should be reset. This should be
  // done for single valued leafs and for static arrays:
  
  for (Int_t k = 0; k<5; ++k)
  {
    AnaTree->Secondary_Nprotons[k] = 0;
    AnaTree->Secondary_Nneutrons[k] = 0;
    AnaTree->Secondary_Nelectrons[k] = 0;
    AnaTree->Secondary_Ngammas[k] = 0;
    AnaTree->Secondary_Npions[k] = 0;
    AnaTree->Secondary_Nmuons[k] = 0;
    AnaTree->Secondary_Nalphas[k] = 0;
    AnaTree->Secondary_NCions[k] = 0;
    AnaTree->Secondary_Nfragments[k] = 0;
    AnaTree->Secondary_NallParticles[k] = 0;
  }    
  AnaTree->Secondary_Particles = kFALSE;
  
  // =============================================================================================
  // NOTE: Do your calculations here. What we want is to accumulate all secondary particles
  // into our leafs and histograms. for this we will loop over all Monte Carlo points.
  // If the point in INSIDE the primary volume, we will consider it a secondary particle
  // and process it.
  
  // Hence we begin with obtaining the amount of Monte Carlo points:
  Int_t ArraySize = TheLeafs->MCTrack_fPdgCode->GetNdata();
  
  // Then we declare the intermediate variables that we need:
  Double_t px = 0.0;
  Double_t py = 0.0;
  Double_t pz = 0.0;
  Double_t fMass = 0.0;
  Double_t xpoint = 0.0;
  Double_t ypoint = 0.0;
  Double_t zpoint = 0.0;
  Double_t Esecondary = 0.0;
  Double_t theta = 0.0;
  Double_t phi = 0.0;
  Int_t PDG = 0;
  Int_t TrackID = 0;
  Bool_t AimVETO = kFALSE;
  Double_t TravelDist = 0.0;
  Double_t MaxTravel = 0.0;
  Double_t AirTravelDist = 0.0;
  Double_t MaxAirTravelDist = 0.0;
  Double_t E_add = 0.0;
  Double_t light = 0.0;
  Int_t ThisBar = 0;
  Double_t repeat = 0.0;
  Double_t repeat_E = 0.0;
  Double_t E_test = 0.0;
  Double_t FullTravelDist = 0.0;
  
  // Now if we actually have an Monte Carlo points, we can proceed:
  if (ArraySize>0)
  {  
     // Then we will loop over the Monte Carlo points:
     for(Int_t k = 0; k<ArraySize; ++k)
     {
         // We begin with obtaining the information of this exact point:
	 px = TheLeafs->MCTrack_fPx->GetValue(k);
         py = TheLeafs->MCTrack_fPy->GetValue(k);
         pz = TheLeafs->MCTrack_fPz->GetValue(k);
	 fMass = TheLeafs->MCTrack_fMass->GetValue(k);
	 zpoint = TheLeafs->MCTrack_fStartZ->GetValue(k);
	 xpoint = TheLeafs->MCTrack_fStartX->GetValue(k);
	 ypoint = TheLeafs->MCTrack_fStartY->GetValue(k);
	 PDG = TheLeafs->MCTrack_fPdgCode->GetValue(k);
	 Esecondary = TMath::Sqrt(px*px + py*py + pz*pz +fMass*fMass) - fMass;
	 
	 // then we only continue if the point is inside the primary volume:
	 if (PointInsidePrimVolume(Inputs,xpoint,ypoint,zpoint))
	 {
	    // Now we have to compute if the trajectory will hit the VETO yes/no:
	    FullTravelDist = TrackAimedAtVETO(Inputs,xpoint,ypoint,zpoint,px,py,pz);
	    AimVETO = (FullTravelDist>0.0); // this information is into the sign!
	    FullTravelDist = TMath::Abs(FullTravelDist); // make it positive again.
	 
	    // Then we have to get the traveldistance through NeuLAND:
	    TravelDist = GetTravelDistance(Inputs,xpoint,ypoint,zpoint,px,py,pz);
	 
	    // Then we have to decide if this traveldistance is bigger/smaller then what we can
	    // travel with our energy:
	    MaxTravel = GetMaxTravel(Inputs,Esecondary,PDG,generator);
	 
	    // so now we can already start filling some of our histograms and leafs:
	    
	    // produced particles:
	    if (TMath::Abs(PDG)==2212) {AnaTree->Secondary_Nprotons[0] = AnaTree->Secondary_Nprotons[0] + 1; AnaTree->Harray[0][0]->Fill(Esecondary,zpoint,TravelDist);}
	    else if (TMath::Abs(PDG)==2112) {AnaTree->Secondary_Nneutrons[0] = AnaTree->Secondary_Nneutrons[0] + 1; AnaTree->Harray[1][0]->Fill(Esecondary,zpoint,TravelDist);}
	    else if (TMath::Abs(PDG)==11) {AnaTree->Secondary_Nelectrons[0] = AnaTree->Secondary_Nelectrons[0] + 1; AnaTree->Harray[2][0]->Fill(Esecondary,zpoint,TravelDist);}
	    else if (TMath::Abs(PDG)==22) {AnaTree->Secondary_Ngammas[0] = AnaTree->Secondary_Ngammas[0] + 1; AnaTree->Harray[3][0]->Fill(Esecondary,zpoint,TravelDist);}
	    else if ((TMath::Abs(PDG)==211)||(TMath::Abs(PDG)==111)) {AnaTree->Secondary_Npions[0] = AnaTree->Secondary_Npions[0] + 1; AnaTree->Harray[4][0]->Fill(Esecondary,zpoint,TravelDist);}
	    else if (TMath::Abs(PDG)==13) {AnaTree->Secondary_Nmuons[0] = AnaTree->Secondary_Nmuons[0] + 1; AnaTree->Harray[5][0]->Fill(Esecondary,zpoint,TravelDist);}
	    else if (TMath::Abs(PDG)==1000020040) {AnaTree->Secondary_Nalphas[0] = AnaTree->Secondary_Nalphas[0] + 1; AnaTree->Harray[6][0]->Fill(Esecondary,zpoint,TravelDist);}
	    else if ((TMath::Abs(PDG)>1000060000)&&(TMath::Abs(PDG)<1000060190)) {AnaTree->Secondary_NCions[0] = AnaTree->Secondary_NCions[0] + 1; AnaTree->Harray[7][0]->Fill(Esecondary,zpoint,TravelDist);}
	    else if (TMath::Abs(PDG)>1000000000) {AnaTree->Secondary_Nfragments[0] = AnaTree->Secondary_Nfragments[0] + 1; AnaTree->Harray[8][0]->Fill(Esecondary,zpoint,TravelDist);}
	    else if ((TMath::Abs(PDG)==12)||(TMath::Abs(PDG)==14)) {} // neutrino's we do not take them into account.
	    else {if (Inputs->Bug_Report==2) {cout << "### BUG REPORT: We have an undetermined produced particle! Event = " << Event << "\n";}}
	    AnaTree->Secondary_NallParticles[0] = AnaTree->Secondary_NallParticles[0] + 1; AnaTree->Harray[9][0]->Fill(Esecondary,zpoint,TravelDist);
	    
	    // aimed particles:
	    if (AimVETO)
	    {
	    if (TMath::Abs(PDG)==2212) {AnaTree->Secondary_Nprotons[1] = AnaTree->Secondary_Nprotons[1] + 1; AnaTree->Harray[0][1]->Fill(Esecondary,zpoint,TravelDist);}
	    else if (TMath::Abs(PDG)==2112) {AnaTree->Secondary_Nneutrons[1] = AnaTree->Secondary_Nneutrons[1] + 1; AnaTree->Harray[1][1]->Fill(Esecondary,zpoint,TravelDist);}
	    else if (TMath::Abs(PDG)==11) {AnaTree->Secondary_Nelectrons[1] = AnaTree->Secondary_Nelectrons[1] + 1; AnaTree->Harray[2][1]->Fill(Esecondary,zpoint,TravelDist);}
	    else if (TMath::Abs(PDG)==22) {AnaTree->Secondary_Ngammas[1] = AnaTree->Secondary_Ngammas[1] + 1; AnaTree->Harray[3][1]->Fill(Esecondary,zpoint,TravelDist);}
	    else if ((TMath::Abs(PDG)==211)||(TMath::Abs(PDG)==111)) {AnaTree->Secondary_Npions[1] = AnaTree->Secondary_Npions[1] + 1; AnaTree->Harray[4][1]->Fill(Esecondary,zpoint,TravelDist);}
	    else if (TMath::Abs(PDG)==13) {AnaTree->Secondary_Nmuons[1] = AnaTree->Secondary_Nmuons[1] + 1; AnaTree->Harray[5][1]->Fill(Esecondary,zpoint,TravelDist);}
	    else if (TMath::Abs(PDG)==1000020040) {AnaTree->Secondary_Nalphas[1] = AnaTree->Secondary_Nalphas[1] + 1; AnaTree->Harray[6][1]->Fill(Esecondary,zpoint,TravelDist);}
	    else if ((TMath::Abs(PDG)>1000060000)&&(TMath::Abs(PDG)<1000060190)) {AnaTree->Secondary_NCions[1] = AnaTree->Secondary_NCions[1] + 1; AnaTree->Harray[7][1]->Fill(Esecondary,zpoint,TravelDist);}
	    else if (TMath::Abs(PDG)>1000000000) {AnaTree->Secondary_Nfragments[1] = AnaTree->Secondary_Nfragments[1] + 1; AnaTree->Harray[8][1]->Fill(Esecondary,zpoint,TravelDist);}
	    else if ((TMath::Abs(PDG)==12)||(TMath::Abs(PDG)==14)) {} // neutrino's we do not take them into account.
	    else {if (Inputs->Bug_Report==2) {cout << "### BUG REPORT: We have an undetermined particle aimed at the VETO! Event = " << Event << "\n";}}
	    AnaTree->Secondary_NallParticles[1] = AnaTree->Secondary_NallParticles[1] + 1; AnaTree->Harray[9][1]->Fill(Esecondary,zpoint,TravelDist);
	    
	    // take the air into account:
	    if ((PDG!=2212)&&(PDG!=2112)&&(PDG!=22)&&(PDG!=211)) // these particles pass through the Air:
	    {
	      if (!PointInsideNeuLAND(Inputs,xpoint,ypoint,zpoint)) {TravelDist = FullTravelDist; MaxTravel = GetMaxAirTravelDist(Inputs,Esecondary,PDG);}
	      // NOTE: if a particle is produced outside neuland, we pretend it always travels the full trip through the air. If it is produced
	      // inside NeuLAND, we pretend that the air between NeuLAND and teh VETO is negligible.
	    }
	    
	    // neutrons and gammas pass also through the air, although their Max. Travel distance calculation suggests otherwise:
	    if (((TMath::Abs(PDG)==2112)||(TMath::Abs(PDG)==22))&&(!PointInsideNeuLAND(Inputs,xpoint,ypoint,zpoint))) {MaxTravel = 1e99;}
	    
	    // Reached particles:
	    if ((TravelDist<MaxTravel)&&(Esecondary>0.001*Inputs->Ecutoff)) // Note the conversion to [GeV].
	    {	      
	    if (TMath::Abs(PDG)==2212) {AnaTree->Secondary_Nprotons[2] = AnaTree->Secondary_Nprotons[2] + 1; AnaTree->Harray[0][2]->Fill(Esecondary,zpoint,TravelDist);}
	    else if (TMath::Abs(PDG)==2112) {AnaTree->Secondary_Nneutrons[2] = AnaTree->Secondary_Nneutrons[2] + 1; AnaTree->Harray[1][2]->Fill(Esecondary,zpoint,TravelDist);}
	    else if (TMath::Abs(PDG)==11) {AnaTree->Secondary_Nelectrons[2] = AnaTree->Secondary_Nelectrons[2] + 1; AnaTree->Harray[2][2]->Fill(Esecondary,zpoint,TravelDist);}
	    else if (TMath::Abs(PDG)==22) {AnaTree->Secondary_Ngammas[2] = AnaTree->Secondary_Ngammas[2] + 1; AnaTree->Harray[3][2]->Fill(Esecondary,zpoint,TravelDist);}
	    else if ((TMath::Abs(PDG)==211)||(TMath::Abs(PDG)==111)) {AnaTree->Secondary_Npions[2] = AnaTree->Secondary_Npions[2] + 1; AnaTree->Harray[4][2]->Fill(Esecondary,zpoint,TravelDist);}
	    else if (TMath::Abs(PDG)==13) {AnaTree->Secondary_Nmuons[2] = AnaTree->Secondary_Nmuons[2] + 1; AnaTree->Harray[5][2]->Fill(Esecondary,zpoint,TravelDist);}
	    else if (TMath::Abs(PDG)==1000020040) {AnaTree->Secondary_Nalphas[2] = AnaTree->Secondary_Nalphas[2] + 1; AnaTree->Harray[6][2]->Fill(Esecondary,zpoint,TravelDist);}
	    else if ((TMath::Abs(PDG)>1000060000)&&(TMath::Abs(PDG)<1000060190)) {AnaTree->Secondary_NCions[2] = AnaTree->Secondary_NCions[2] + 1; AnaTree->Harray[7][2]->Fill(Esecondary,zpoint,TravelDist);}
	    else if (TMath::Abs(PDG)>1000000000) {AnaTree->Secondary_Nfragments[2] = AnaTree->Secondary_Nfragments[2] + 1; AnaTree->Harray[8][2]->Fill(Esecondary,zpoint,TravelDist);}
	    else if ((TMath::Abs(PDG)==12)||(TMath::Abs(PDG)==14)) {} // neutrino's we do not take them into account.
	    else {if (Inputs->Bug_Report==2) {cout << "### BUG REPORT: We have an undetermined particle that reached the VETO! Event = " << Event << "\n";}}
	    AnaTree->Secondary_NallParticles[2] = AnaTree->Secondary_NallParticles[2] + 1; AnaTree->Harray[9][2]->Fill(Esecondary,zpoint,TravelDist);
	      
	      // Close all blocks:
	    }
	    }
	 }
     }
  }
  
  // ========================================================================================================
  // Now for the next part of our analysis, we have to loop over the VETO points, not over the Monte Carlo
  // points. This is because we now want to know exactly which particle did hit the VETO.
  
  // We will now define some ROOT histograms to help us keep track of which particles
  // are already detected yes/no:
  Int_t NbarsVETO = (Inputs->VETO_geo_NsinglePlanes)*(Inputs->VETO_geo_NbarsPerPlane);
  TH1D* Registered = new TH1D("Registered","Registered",ArraySize,0.0,1.0);
  TH1D* Registered_E = new TH1D("Registered_E","Registered_E",ArraySize,0.0,1.0);
  TH2D* E_deposition = new TH2D("E_deposition","E_deposition",ArraySize,0.0,1.0,NbarsVETO,0.0,1.0);
  // Notice that all histograms hold zeros by default!

  // Now in order to loop over the Veto Points, we first need to obtain the size:
  Int_t VetoSize = TheLeafs->veto_segPoint_fTrackID->GetNdata();
  
  // Now only continue of course if there are indeed points inside the VETO:
  if (VetoSize>0)
  { 
     // Then loop over the VETO:
     for (Int_t kveto = 0; kveto<ArraySize; ++kveto)
     {
       // Request the trackID so that we can retrace this in the MCTrack-leafs:
       TrackID = TheLeafs->veto_segPoint_fTrackID->GetValue(kveto);
       
       if (TrackID<0) {TrackID=0;} // prevent seg faults!
       
       // The obtain the production point of this track:
       zpoint = TheLeafs->MCTrack_fStartZ->GetValue(TrackID);
       xpoint = TheLeafs->MCTrack_fStartX->GetValue(TrackID);
       ypoint = TheLeafs->MCTrack_fStartY->GetValue(TrackID);
       
       // Then track it back untill we leave the VETO:
       while (PointInsideVETO(Inputs,xpoint,ypoint,zpoint))
       {
         TrackID = TheLeafs->MCTrack_fMotherId->GetValue(TrackID);
	 if (TrackID<0) {TrackID=0;} // NOTE: do not place the particle gun inside the VETO!
	 zpoint = TheLeafs->MCTrack_fStartZ->GetValue(TrackID);
         xpoint = TheLeafs->MCTrack_fStartX->GetValue(TrackID);
         ypoint = TheLeafs->MCTrack_fStartY->GetValue(TrackID);
       }
 
       // Now TrackID holds the index of the particle that is responsible
       // for precisely this hitpoint in the VETO. also xpoint, ypoint, zpoint
       // hold the production point of this particle.
       
       // Request impulses:
       px = TheLeafs->MCTrack_fPx->GetValue(TrackID);
       py = TheLeafs->MCTrack_fPy->GetValue(TrackID);
       pz = TheLeafs->MCTrack_fPz->GetValue(TrackID);
       fMass = TheLeafs->MCTrack_fMass->GetValue(TrackID);

       // Now compute the energy of the secondary particle:
       Esecondary = TMath::Sqrt(px*px + py*py + pz*pz + fMass*fMass) - fMass;
       
       // Request particle ID:
       PDG = TheLeafs->MCTrack_fPdgCode->GetValue(TrackID);
       
       // Request the energy deposition of this interaction point of the VETO and the bar where it is:
       light = TheLeafs->veto_segPoint_fLightYield->GetValue(kveto);
       ThisBar = TheLeafs->veto_segPoint_fSector->GetValue(kveto);
       
       if (ThisBar<NbarsVETO+1) {
       // Add this Energy deposition to the tracked mother:
       E_add = E_deposition->GetBinContent(TrackID+1,ThisBar);
       E_add = E_add + light;
       E_deposition->SetBinContent(TrackID+1,ThisBar,E_add);}
       
       // So now we should decide if this tracked mother should be registered
       // in our AnalysisTree and in our histograms yes/no. It should be added
       // if it is a secondary (hence inside the primary volume) and if it
       // is not regsistererd before.
       
       // Hence request the registration:
       repeat = Registered->GetBinContent(TrackID+1);
       
       // So now we can decide:
       if ((repeat<0.5)&&PointInsidePrimVolume(Inputs,xpoint,ypoint,zpoint))
       {
	 // Now we can add it to our registration depending on its PDG:
	 if (TMath::Abs(PDG)==2212) {AnaTree->Secondary_Nprotons[3] = AnaTree->Secondary_Nprotons[3] + 1; AnaTree->Harray[0][3]->Fill(Esecondary,zpoint,0.5*Inputs->DistMax);}
	 else if (TMath::Abs(PDG)==2112) {AnaTree->Secondary_Nneutrons[3] = AnaTree->Secondary_Nneutrons[3] + 1; AnaTree->Harray[1][3]->Fill(Esecondary,zpoint,0.5*Inputs->DistMax);}
	 else if (TMath::Abs(PDG)==11) {AnaTree->Secondary_Nelectrons[3] = AnaTree->Secondary_Nelectrons[3] + 1; AnaTree->Harray[2][3]->Fill(Esecondary,zpoint,0.5*Inputs->DistMax);}
	 else if (TMath::Abs(PDG)==22) {AnaTree->Secondary_Ngammas[3] = AnaTree->Secondary_Ngammas[3] + 1; AnaTree->Harray[3][3]->Fill(Esecondary,zpoint,0.5*Inputs->DistMax);}
	 else if ((TMath::Abs(PDG)==211)||(TMath::Abs(PDG)==111)) {AnaTree->Secondary_Npions[3] = AnaTree->Secondary_Npions[3] + 1; AnaTree->Harray[4][3]->Fill(Esecondary,zpoint,0.5*Inputs->DistMax);}
	 else if (TMath::Abs(PDG)==13) {AnaTree->Secondary_Nmuons[3] = AnaTree->Secondary_Nmuons[3] + 1; AnaTree->Harray[5][3]->Fill(Esecondary,zpoint,0.5*Inputs->DistMax);}
	 else if (TMath::Abs(PDG)==1000020040) {AnaTree->Secondary_Nalphas[3] = AnaTree->Secondary_Nalphas[3] + 1; AnaTree->Harray[6][3]->Fill(Esecondary,zpoint,0.5*Inputs->DistMax);}
	 else if ((TMath::Abs(PDG)>1000060000)&&(TMath::Abs(PDG)<1000060190)) {AnaTree->Secondary_NCions[3] = AnaTree->Secondary_NCions[3] + 1; AnaTree->Harray[7][3]->Fill(Esecondary,zpoint,0.5*Inputs->DistMax);}
	 else if (TMath::Abs(PDG)>1000000000) {AnaTree->Secondary_Nfragments[3] = AnaTree->Secondary_Nfragments[3] + 1; AnaTree->Harray[8][3]->Fill(Esecondary,zpoint,0.5*Inputs->DistMax);}
	 else {if (Inputs->Bug_Report==2) {cout << "### BUG REPORT: We have an undetermined particle hitting the VETO! Event = " << Event << "\n";}}
	 AnaTree->Secondary_NallParticles[3] = AnaTree->Secondary_NallParticles[3] + 1; AnaTree->Harray[9][3]->Fill(Esecondary,zpoint,0.5*Inputs->DistMax);
	 
         // Then since we registered this particle, we must set the registration:
	 Registered->SetBinContent(TrackID+1,1.0);
       }
       
       // Now we can do exactly the same, only now we also must take into account
       // the energy deposition within one bar, maximized over the barnumbers:
       repeat_E = Registered_E->GetBinContent(TrackID+1);
       E_test = 0.0;
       for (Int_t q = 0; q<(NbarsVETO+2); ++q)
       {
	 if ((E_deposition->GetBinContent(TrackID+1,q+1))>E_test) {E_test = E_deposition->GetBinContent(TrackID+1,q+1);}
       }

       // So now we can decide again:
       if ((repeat_E<0.5)&&PointInsidePrimVolume(Inputs,xpoint,ypoint,zpoint)&&(1e6*E_test>(Inputs->Veto_Ethreshold))) // NOTE: conversion between keV and GeV!
       {
	 // Now we can add it again to our registration depending on its PDG:
	 if (TMath::Abs(PDG)==2212) {AnaTree->Secondary_Nprotons[4] = AnaTree->Secondary_Nprotons[4] + 1; AnaTree->Harray[0][4]->Fill(Esecondary,zpoint,0.5*Inputs->DistMax);}
	 else if (TMath::Abs(PDG)==2112) {AnaTree->Secondary_Nneutrons[4] = AnaTree->Secondary_Nneutrons[4] + 1; AnaTree->Harray[1][4]->Fill(Esecondary,zpoint,0.5*Inputs->DistMax);}
	 else if (TMath::Abs(PDG)==11) {AnaTree->Secondary_Nelectrons[4] = AnaTree->Secondary_Nelectrons[4] + 1; AnaTree->Harray[2][4]->Fill(Esecondary,zpoint,0.5*Inputs->DistMax);}
	 else if (TMath::Abs(PDG)==22) {AnaTree->Secondary_Ngammas[4] = AnaTree->Secondary_Ngammas[4] + 1; AnaTree->Harray[3][4]->Fill(Esecondary,zpoint,0.5*Inputs->DistMax);}
	 else if ((TMath::Abs(PDG)==211)||(TMath::Abs(PDG)==111)) {AnaTree->Secondary_Npions[4] = AnaTree->Secondary_Npions[4] + 1; AnaTree->Harray[4][4]->Fill(Esecondary,zpoint,0.5*Inputs->DistMax);}
	 else if (TMath::Abs(PDG)==13) {AnaTree->Secondary_Nmuons[4] = AnaTree->Secondary_Nmuons[4] + 1; AnaTree->Harray[5][4]->Fill(Esecondary,zpoint,0.5*Inputs->DistMax);}
	 else if (TMath::Abs(PDG)==1000020040) {AnaTree->Secondary_Nalphas[4] = AnaTree->Secondary_Nalphas[4] + 1; AnaTree->Harray[6][4]->Fill(Esecondary,zpoint,0.5*Inputs->DistMax);}
	 else if ((TMath::Abs(PDG)>1000060000)&&(TMath::Abs(PDG)<1000060190)) {AnaTree->Secondary_NCions[4] = AnaTree->Secondary_NCions[4] + 1; AnaTree->Harray[7][4]->Fill(Esecondary,zpoint,0.5*Inputs->DistMax);}
	 else if (TMath::Abs(PDG)>1000000000) {AnaTree->Secondary_Nfragments[4] = AnaTree->Secondary_Nfragments[4] + 1; AnaTree->Harray[8][4]->Fill(Esecondary,zpoint,0.5*Inputs->DistMax);}
	 else {if (Inputs->Bug_Report==2) {cout << "### BUG REPORT: We have an undetermined particle detected by the VETO! Event = " << Event << "\n";}}
	 AnaTree->Secondary_NallParticles[4] = AnaTree->Secondary_NallParticles[4] + 1; AnaTree->Harray[9][4]->Fill(Esecondary,zpoint,0.5*Inputs->DistMax);
	 
         // Then since we registered this particle, we must set the registration:
	 Registered_E->SetBinContent(TrackID+1,1.0);
       }
     
       // And that's it. Close all blocks:
     }
  }
  
  // Finally we want to set a boolian that says if we found secondary particles in the VETO yes/no:
  if ((AnaTree->Secondary_NallParticles[4])>0) {AnaTree->Secondary_Particles = kTRUE;}
  
  // And that's it! Done! Now delete the histograms:
  delete Registered;
  delete Registered_E;
  delete E_deposition;
  
  // =============================================================================================
  // Now that the calculations are done, the results have to be put into the Analysis Tree.
  // since some of your leafs might be dynamical arrays, one first has to
  // re-allocate the arrays. Then, a SetBranchAddress should be performed, 
  // because the delete-operations destroys the link to the Analysis Tree. Only after all this,
  // the results of your calculations can be put into the Dynamical arrays. Then this
  // will be filled into the tree automatically. The delete commands that avoid memory leaks
  // should be put into the DeleteEverything function.
  
  // This analysis task does not use Dynamical arrays.
  
  // =============================================================================================
  // And now we are done. All variables that we declared here without new are lost
  // after this function is complete, and that is how we like it (memory leaks)! The results are
  // NOT lost, since we put them into the arrays that are part of the AnaTree-structure.
  // here we only work with a pointer to this structure, so the results are put
  // in our structure outside this function. Then outside this function, the
  // command tree->Fill() is given and results are store. You are done now.
}

// ####################################################################################################
// put in the empty function:

void Empty_Secondary_Particles(Leaf_Structure* const TheLeafs, Inputs_Structure* const Inputs, Tree_Structure* AnaTree, Int_t const Event, Int_t const Counter, TRandom3* generator)
{
  for (Int_t k = 0; k<5; ++k)
  {
    AnaTree->Secondary_Nprotons[k] = 0;
    AnaTree->Secondary_Nneutrons[k] = 0;
    AnaTree->Secondary_Nelectrons[k] = 0;
    AnaTree->Secondary_Ngammas[k] = 0;
    AnaTree->Secondary_Npions[k] = 0;
    AnaTree->Secondary_Nmuons[k] = 0;
    AnaTree->Secondary_Nalphas[k] = 0;
    AnaTree->Secondary_NCions[k] = 0;
    AnaTree->Secondary_Nfragments[k] = 0;
    AnaTree->Secondary_NallParticles[k] = 0;
  }    
  
  AnaTree->Secondary_Particles = kFALSE;
}