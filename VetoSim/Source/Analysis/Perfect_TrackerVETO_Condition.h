void Perfect_TrackerVETO_Condition(Leaf_Structure* const TheLeafs, Inputs_Structure* const Inputs, Tree_Structure* AnaTree, Int_t const Event, Int_t const Counter, TRandom3* generator)
{
   // This VETO condition is only for comparison purposes. It VETOes each vertex based on whether it is
   // linked to a neutron coming from the target yes/no. This way one can see the best possible efficiency
   // one can possibly achieve with a VETO condition.
    
   // Written by C. A. Douma, for the R3B Collaboration.
    
   // =====================================================================================================
   // Reset all VETO variables:
   No_VETO_Condition(TheLeafs,Inputs,AnaTree,Event,Counter,generator);
    
   // Loop over all particles:
   Int_t Nprims = AnaTree->Nprims_NeuLAND;
   Int_t Nrec = AnaTree->N_Reconstructed_PrimPoints;   
   Int_t Nfired = AnaTree->NbarsFired_VETO;
   
   // Declare variables:
   Bool_t test = kFALSE;
   
   // NOTE: Only linked particles are decided. If a primary particle is not linked to a
   // reconstructed vertex, it is automatically NOT vetoed, as it should be. If it has no vertex,
   // we cannot VETO it. Also, a vertex that does not belong to a prim. particle will not be
   // VETOed here.
   for (Int_t krec = 0; krec<Nrec; ++krec)
   {
       for (Int_t kprim = 0; kprim<Nprims; ++kprim)
       {
           if (AnaTree->TrackID_Reconstructed_PrimPoints[krec]==AnaTree->Prim_TrackID[kprim])
           {
               // check for the PDG & target (if necessary):
               if ((Inputs->VETO_select==9)||(Inputs->VETO_select==13)) {test = (((AnaTree->Prim_PDG[kprim]==2112)||(AnaTree->Prim_PDG[kprim]==22))&&(AnaTree->Prim_IsFromTarget[kprim]==kTRUE));}
               else if ((Inputs->VETO_select==11)||(Inputs->VETO_select==14)||(Inputs->VETO_select==17)||(Inputs->VETO_select==18)) {test = ((AnaTree->Prim_PDG[kprim]==2112)||(AnaTree->Prim_PDG[kprim]==22));}
               else {test = kFALSE;}
               
               if (test)
               {
                   AnaTree->Prim_IsVETOed[kprim] = kFALSE;
                   AnaTree->Reconstructed_PrimPoints_IsVETOed[krec] = kFALSE;
               }
               else
               {
                   if (((Inputs->VETO_select==17)||(Inputs->VETO_select==18))&&(Nfired>0))
                   {
                        AnaTree->Prim_IsVETOed[kprim] = kTRUE;
                        AnaTree->Reconstructed_PrimPoints_IsVETOed[krec] = kTRUE;
                   }
                   else if ((Inputs->VETO_select!=17)&&(Inputs->VETO_select!=18))
                   {
                        AnaTree->Prim_IsVETOed[kprim] = kTRUE;
                        AnaTree->Reconstructed_PrimPoints_IsVETOed[krec] = kTRUE;
                   }
                   else
                   {
                        AnaTree->Prim_IsVETOed[kprim] = kFALSE;
                        AnaTree->Reconstructed_PrimPoints_IsVETOed[krec] = kFALSE;
                   }
               }
           }
       }
   }
   
   // Therefore we manually VETO these ghost vertices, to achieve max. possible efficiency.
   // This is because there is no reason to expect that in theb ideal case, ghost points are
   // UnVETOed. Look at the startracker-coincidence, etc. There they are VETOed too.
   for (Int_t krec = 0; krec<Nrec; ++krec)
   {
       if (AnaTree->TrackID_Reconstructed_PrimPoints[krec]==-1)
       {
           // Comment it out: see NOTE: down!
           // AnaTree->Reconstructed_PrimPoints_IsVETOed[krec] = kTRUE;
           
           // NOTE: One should VETO ghost vertices if one whishes to know the highest
           // theoretical VETO condition. However, If we VETO one vertex for each
           // fired bar in the VETO, then with such a condition, the highest
           // achieveable VETO condition is obtained when we do not VETO ghost
           // vertices. This is because no matter what method is chosen in
           // linking the fired VETO bars with the vertices, the ghost vertices are
           // always 'too much' and will therefore never be VETOed by such a condition.
           // And since the VETO provides no way of knowing which vertices are ghost
           // vertices, the practically highest achieveable result is obtained when we do
           // not VETO ghost vertices.
       }
   }
   
   // Done!
    
}