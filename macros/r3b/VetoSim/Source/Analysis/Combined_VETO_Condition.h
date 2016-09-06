void Combined_VETO_Condition(Leaf_Structure* const TheLeafs, Inputs_Structure* const Inputs, Tree_Structure* AnaTree, Int_t const Event, Int_t const Counter, TRandom3* generator)
{
   // This VETO condition is basically nothing new. We simply use the VETO conditions that we already have
   // and call them here. Only now we make a decision per event which VETO condition to call.
    
   // Written by C. A. Douma, for the R3B Collaboration.
    
   // =====================================================================================================
    
   // Let's experiment simple first. If there is a track in STaRTrack, use our Tracker VETO condition.
   // if there is no track: VETO everything.
    
   Int_t Ntracks = TheLeafs->TrackSize->GetValue(0);
   Int_t NcalHits = TheLeafs->CaloHitSim_fEnergy->GetNdata();
   Int_t Nprims = AnaTree->Nprims_NeuLAND;
   Int_t Nrec = AnaTree->N_Reconstructed_PrimPoints;   
   
   if ((Ntracks>0)||(NcalHits>0))
   {
       if (Inputs->VETO_select==8) {Tracker_VETO_Condition(TheLeafs,Inputs,AnaTree,Event,Counter,generator);}
       else if (Inputs->VETO_select==10) {TrackerCluster_VETO_Condition(TheLeafs,Inputs,AnaTree,Event,Counter,generator);}
       else if (Inputs->VETO_select==12) {No_VETO_Condition(TheLeafs,Inputs,AnaTree,Event,Counter,generator);}
       else if (Inputs->VETO_select==13) {Perfect_TrackerVETO_Condition(TheLeafs,Inputs,AnaTree,Event,Counter,generator);}
       else if (Inputs->VETO_select==14) {Perfect_TrackerVETO_Condition(TheLeafs,Inputs,AnaTree,Event,Counter,generator);}
       else if (Inputs->VETO_select==15) {Naive_VETO_Condition(TheLeafs,Inputs,AnaTree,Event,Counter,generator);}
       else if (Inputs->VETO_select==16) {TOF_VETO_Condition(TheLeafs,Inputs,AnaTree,Event,Counter,generator);}
       else if (Inputs->VETO_select==17) {Perfect_TrackerVETO_Condition(TheLeafs,Inputs,AnaTree,Event,Counter,generator);}
       else {No_VETO_Condition(TheLeafs,Inputs,AnaTree,Event,Counter,generator);}
       
       // In addition: VETO any vertex that has a reconstructed time above
       // a certain threshold:
       for (Int_t krec = 0; krec<Nrec; ++krec)
       {
         if (AnaTree->T_Reconstructed_PrimPoints[krec]>Inputs->TimeCut)
         {
             AnaTree->Reconstructed_PrimPoints_IsVETOed[krec] = kTRUE;
             
             for (Int_t kprim = 0; kprim<Nprims; ++kprim)
             {
                 if (AnaTree->TrackID_Reconstructed_PrimPoints[krec]==AnaTree->Prim_TrackID[kprim])
                 {
                     AnaTree->Prim_IsVETOed[kprim] = kTRUE;
                 }
             }
         }
       } 
       
   }
   else
   {
       Full_VETO_Condition(TheLeafs,Inputs,AnaTree,Event,Counter,generator);
   }
    
}