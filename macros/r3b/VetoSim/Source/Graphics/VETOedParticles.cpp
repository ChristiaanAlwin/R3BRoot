// include the required C++ headers:
#include <iostream>

// include the required root headers:

// include own headers:
#include "../Analysis/Inputs_Load.h"
#include "AnaLeafs_Connect.h"

// use the standard C++ variables:
using namespace std;

void VETOedParticles()
{
  // This function counts the number of VETOed, Detected and Kept particles
  // within one run. It then prints a table on the screen with results.
    
  // Written by C. A. Douma.
    
  // ===================================================================================
  // Before we do anything, we will obtain the Counter and the Series from sed:
  Int_t Series = 0;
Series = 0;

  Int_t Counter = 0;
Counter = 1;

  cout << "We now read out a single analysis tree.\n\n"
       << "=========================================================================\n\n";
  
  // We begin with connecting to the Inputs.root-file:
  Inputs_Structure TheInputs;
  Inputs_Structure* Inputs = &TheInputs;
  Bool_t Load = Inputs_Load(Inputs,Counter,Series);
  
  // Then connect to the analysis tree:
  AnaLeafs_Structure TheLeafs;
  AnaLeafs_Structure* AnaLeafs = &TheLeafs;
  Bool_t Connect = AnaLeafs_Connect(AnaLeafs,Counter);
  
  // Then check if everything worked:
  if (!(Connect&&Load))
  {
      cout << "### ERROR: Some initializers could not be created!\n"
           << "### ERROR: Without these initializers this task could not be performed!\n"
           << "### ERROR: Therefore the program is now terminated!\n\n";
  }
  else
  {
      cout << "All initializers are succesfully created.\n\n"
           << "======================================================================\n\n"
           << "We will now loop over the tree.\n\n";

  // ====================================================================================
  // Now get everything that we need for the event loop:
  Long64_t nEvents = Inputs->nEvents;
  Long64_t nBytes = 0;
  
  // We first declare what we need:
  Int_t Total_Particles[12];
  Int_t VETOed_Particles[12];
  Int_t Kept_Particles[12];
  Int_t Resolved_Particles[12];
  
  Int_t GoodEvents = 0;
  Int_t AppearentEvents = 0;
  Int_t AcceptableEvents = 0;
  Int_t NeutronCounter = 0;
  Int_t VETOedCounter = 0;
  Int_t pointscounter = 0;
  Int_t NtargetCounter = 0;
  Bool_t ThisEventIsGood = kFALSE;
  Bool_t ThisNeutronIsGood = kFALSE;
  Bool_t TargetTest = kFALSE;
  Bool_t Appearent_Test = kFALSE;
  
  for (Int_t k = 0; k<12; ++k)
  {
      Total_Particles[k] = 0;
      VETOed_Particles[k] = 0;
      Kept_Particles[k] = 0;
      Resolved_Particles[k] = 0;
  }
  Int_t Nprims = 0;
  Int_t Nrec = 0;
  
  // Now perform the event loop over the tree:
  for (Long64_t Event = 0; Event<nEvents; ++Event)
  {
      // read out an event:
      nBytes = AnaLeafs->TheTree->GetEntry(Event);
      
      // Reset the boolian for good events:
      ThisEventIsGood = kTRUE;
      NeutronCounter = 0; // number of target neutrons that passed the tests.
      VETOedCounter = 0; // number of other particles that passed the tests AND are detected!
      pointscounter = 0; // number of non-VETOed vertices.
      NtargetCounter = 0;
      
      // now we simply start counting particles:
      Nprims = AnaLeafs->Nprims_NeuLAND->GetValue(0);
      
      if (Nprims>0)
      {
          for (Int_t k = 0; k<Nprims; ++k)
          {
              if (AnaLeafs->Prim_PDG->GetValue(k)==2212) // protons
              {
                  Total_Particles[0] = Total_Particles[0] + 1;
                  if ((AnaLeafs->Prim_IsVETOed->GetValue(k))&&(AnaLeafs->Prim_Detected->GetValue(k))) {VETOed_Particles[0] = VETOed_Particles[0] + 1;}
                  if (AnaLeafs->Prim_Detected->GetValue(k)) {Kept_Particles[0] = Kept_Particles[0] + 1;}
                  if ((AnaLeafs->Prim_Resolved->GetValue(k))&&(AnaLeafs->Prim_Detected->GetValue(k))) {Resolved_Particles[0] = Resolved_Particles[0] + 1;}
                  
                  // For each charged particle it is the case: if it is detected but not VETOed, we are in trouble!
                  if ((AnaLeafs->Prim_Detected->GetValue(k)==kTRUE)&&(AnaLeafs->Prim_IsVETOed->GetValue(k)==kFALSE))
                  {
                      ThisEventIsGood = kFALSE;
                      VETOedCounter = VETOedCounter + 1;
                  }
              }
              
              else if (AnaLeafs->Prim_PDG->GetValue(k)==2112) // neutrons
              {
                  Total_Particles[1] = Total_Particles[1] + 1;
                  if ((AnaLeafs->Prim_IsVETOed->GetValue(k))&&(AnaLeafs->Prim_Detected->GetValue(k))) {VETOed_Particles[1] = VETOed_Particles[1] + 1;}
                  if (AnaLeafs->Prim_Detected->GetValue(k)) {Kept_Particles[1] = Kept_Particles[1] + 1;}
                  if ((AnaLeafs->Prim_Resolved->GetValue(k))&&(AnaLeafs->Prim_Detected->GetValue(k))) {Resolved_Particles[1] = Resolved_Particles[1] + 1;}
                  
                  if (AnaLeafs->Prim_IsFromTarget->GetValue(k)==kTRUE)
                  {
                      Total_Particles[11] = Total_Particles[11] + 1;
                      if ((AnaLeafs->Prim_IsVETOed->GetValue(k))&&(AnaLeafs->Prim_Detected->GetValue(k))) {VETOed_Particles[11] = VETOed_Particles[11] + 1;}
                      if (AnaLeafs->Prim_Detected->GetValue(k)) {Kept_Particles[11] = Kept_Particles[11] + 1;}
                      if ((AnaLeafs->Prim_Resolved->GetValue(k))&&(AnaLeafs->Prim_Detected->GetValue(k))) {Resolved_Particles[11] = Resolved_Particles[11] + 1;}
                      //Eh->Fill(AnaLeafs->Prim_E_Detected->GetValue(k));
                  }
                  
                  // For neutrons: There are two kinds. A neutron from the target, or a background neutron. Background neutrons should either not be
                  // detected, or be VETOed. Neutrons from the target should be detected, resolved & not VETOed:
                  
                  // reset:
                  ThisNeutronIsGood = kFALSE;
                  
                  // check background/target:
                  if (AnaLeafs->Prim_IsFromTarget->GetValue(k)==kTRUE)
                  {
                      NtargetCounter = NtargetCounter + 1;
                      
                      // then it should be resolved, detected & NOT vetoed:
                      if (AnaLeafs->Prim_Detected->GetValue(k)==kTRUE)
                      {
                          if (AnaLeafs->Prim_Resolved->GetValue(k)==kTRUE)
                          {
                              if (AnaLeafs->Prim_IsVETOed->GetValue(k)==kFALSE)
                              {
                                  ThisNeutronIsGood = kTRUE;
                                  NeutronCounter = NeutronCounter + 1;
                              }
                          }
                      }
                  }
                  else
                  {
                      // then it should either not be detected, or be VETOed:
                      if (AnaLeafs->Prim_Detected->GetValue(k)==kFALSE)
                      {
                          ThisNeutronIsGood = kTRUE;
                          // no counters!
                      }
                      else
                      {
                          // then it is a background detected neutron. it should be VETOed:
                          if (AnaLeafs->Prim_IsVETOed->GetValue(k)==kTRUE)
                          {
                              ThisNeutronIsGood = kTRUE;
                              VETOedCounter = VETOedCounter + 1; // counts all VETOed particles.
                              // only neutrons from the target should not be vetoed.
                          }
                      }
                  }                  

                  // Now the Boolian ThisNeutronIsGood only states something about this neutron.
                  // all neutrons in the event should be good to get a good event:
                  if (!ThisNeutronIsGood) {ThisEventIsGood = kFALSE;}
              }
              
              else if (AnaLeafs->Prim_PDG->GetValue(k)==11) // Electrons
              {
                  Total_Particles[2] = Total_Particles[2] + 1;
                  if ((AnaLeafs->Prim_IsVETOed->GetValue(k))&&(AnaLeafs->Prim_Detected->GetValue(k))) {VETOed_Particles[2] = VETOed_Particles[2] + 1;}
                  if (AnaLeafs->Prim_Detected->GetValue(k)) {Kept_Particles[2] = Kept_Particles[2] + 1;}
                  if ((AnaLeafs->Prim_Resolved->GetValue(k))&&(AnaLeafs->Prim_Detected->GetValue(k))) {Resolved_Particles[2] = Resolved_Particles[2] + 1;}
                  
                  // For each charged particle it is the case: if it is detected but not VETOed, we are in trouble!
                  if ((AnaLeafs->Prim_Detected->GetValue(k)==kTRUE)&&(AnaLeafs->Prim_IsVETOed->GetValue(k)==kFALSE))
                  {
                      ThisEventIsGood = kFALSE;
                      VETOedCounter = VETOedCounter + 1;
                  }
              }
              
              else if (AnaLeafs->Prim_PDG->GetValue(k)==22) // Gammas
              {
                  Total_Particles[3] = Total_Particles[3] + 1;
                  if ((AnaLeafs->Prim_IsVETOed->GetValue(k))&&(AnaLeafs->Prim_Detected->GetValue(k))) {VETOed_Particles[3] = VETOed_Particles[3] + 1;}
                  if (AnaLeafs->Prim_Detected->GetValue(k)) {Kept_Particles[3] = Kept_Particles[3] + 1;}
                  if ((AnaLeafs->Prim_Resolved->GetValue(k))&&(AnaLeafs->Prim_Detected->GetValue(k))) {Resolved_Particles[3] = Resolved_Particles[3] + 1;}
                  
                  // these should simply not be detected:
                  if (AnaLeafs->Prim_Detected->GetValue(k)==kTRUE) {ThisEventIsGood = kFALSE;} // NOTE: no gamma should be detected. hence no counter increase here!
              }
              
              else if (TMath::Abs(AnaLeafs->Prim_PDG->GetValue(k))==211) // pions
              {
                  Total_Particles[4] = Total_Particles[4] + 1;
                  if ((AnaLeafs->Prim_IsVETOed->GetValue(k))&&(AnaLeafs->Prim_Detected->GetValue(k))) {VETOed_Particles[4] = VETOed_Particles[4] + 1;}
                  if (AnaLeafs->Prim_Detected->GetValue(k)) {Kept_Particles[4] = Kept_Particles[4] + 1;}
                  if ((AnaLeafs->Prim_Resolved->GetValue(k))&&(AnaLeafs->Prim_Detected->GetValue(k))) {Resolved_Particles[4] = Resolved_Particles[4] + 1;}
                  
                  // For each charged particle it is the case: if it is detected but not VETOed, we are in trouble!
                  if ((AnaLeafs->Prim_Detected->GetValue(k)==kTRUE)&&(AnaLeafs->Prim_IsVETOed->GetValue(k)==kFALSE))
                  {
                      ThisEventIsGood = kFALSE;
                      VETOedCounter = VETOedCounter + 1;
                  }
              }
              
              else if (TMath::Abs(AnaLeafs->Prim_PDG->GetValue(k))==13) // muons
              {
                  Total_Particles[5] = Total_Particles[5] + 1;
                  if ((AnaLeafs->Prim_IsVETOed->GetValue(k))&&(AnaLeafs->Prim_Detected->GetValue(k))) {VETOed_Particles[5] = VETOed_Particles[5] + 1;}
                  if (AnaLeafs->Prim_Detected->GetValue(k)) {Kept_Particles[5] = Kept_Particles[5] + 1;}
                  if ((AnaLeafs->Prim_Resolved->GetValue(k))&&(AnaLeafs->Prim_Detected->GetValue(k))) {Resolved_Particles[5] = Resolved_Particles[5] + 1;}
                  
                  // For each charged particle it is the case: if it is detected but not VETOed, we are in trouble!
                  if ((AnaLeafs->Prim_Detected->GetValue(k)==kTRUE)&&(AnaLeafs->Prim_IsVETOed->GetValue(k)==kFALSE))
                  {
                      ThisEventIsGood = kFALSE;
                      VETOedCounter = VETOedCounter + 1;
                  }
              }
              
              else if (AnaLeafs->Prim_PDG->GetValue(k)==1000020040) // Alphas
              {
                  Total_Particles[6] = Total_Particles[6] + 1;
                  if ((AnaLeafs->Prim_IsVETOed->GetValue(k))&&(AnaLeafs->Prim_Detected->GetValue(k))) {VETOed_Particles[6] = VETOed_Particles[6] + 1;}
                  if (AnaLeafs->Prim_Detected->GetValue(k)) {Kept_Particles[6] = Kept_Particles[6] + 1;}
                  if ((AnaLeafs->Prim_Resolved->GetValue(k))&&(AnaLeafs->Prim_Detected->GetValue(k))) {Resolved_Particles[6] = Resolved_Particles[6] + 1;}
                  
                  // For each charged particle it is the case: if it is detected but not VETOed, we are in trouble!
                  if ((AnaLeafs->Prim_Detected->GetValue(k)==kTRUE)&&(AnaLeafs->Prim_IsVETOed->GetValue(k)==kFALSE))
                  {
                      ThisEventIsGood = kFALSE;
                      VETOedCounter = VETOedCounter + 1;
                  }
              }
              
              else if ((AnaLeafs->Prim_PDG->GetValue(k)>1000060000)&&(AnaLeafs->Prim_PDG->GetValue(k)<1000060190)) // Cions
              {
                  Total_Particles[7] = Total_Particles[7] + 1;
                  if ((AnaLeafs->Prim_IsVETOed->GetValue(k))&&(AnaLeafs->Prim_Detected->GetValue(k))) {VETOed_Particles[7] = VETOed_Particles[7] + 1;}
                  if (AnaLeafs->Prim_Detected->GetValue(k)) {Kept_Particles[7] = Kept_Particles[7] + 1;}
                  if ((AnaLeafs->Prim_Resolved->GetValue(k))&&(AnaLeafs->Prim_Detected->GetValue(k))) {Resolved_Particles[7] = Resolved_Particles[7] + 1;}
                  
                  // For each charged particle it is the case: if it is detected but not VETOed, we are in trouble!
                  if ((AnaLeafs->Prim_Detected->GetValue(k)==kTRUE)&&(AnaLeafs->Prim_IsVETOed->GetValue(k)==kFALSE))
                  {
                      ThisEventIsGood = kFALSE;
                      VETOedCounter = VETOedCounter + 1;
                  }
              }
              
              else if (AnaLeafs->Prim_PDG->GetValue(k)>1000000000) // Fragments
              {
                  Total_Particles[8] = Total_Particles[8] + 1;
                  if ((AnaLeafs->Prim_IsVETOed->GetValue(k))&&(AnaLeafs->Prim_Detected->GetValue(k))) {VETOed_Particles[8] = VETOed_Particles[8] + 1;}
                  if (AnaLeafs->Prim_Detected->GetValue(k)) {Kept_Particles[8] = Kept_Particles[8] + 1;}
                  if ((AnaLeafs->Prim_Resolved->GetValue(k))&&(AnaLeafs->Prim_Detected->GetValue(k))) {Resolved_Particles[8] = Resolved_Particles[8] + 1;}
                  
                  // For each charged particle it is the case: if it is detected but not VETOed, we are in trouble!
                  if ((AnaLeafs->Prim_Detected->GetValue(k)==kTRUE)&&(AnaLeafs->Prim_IsVETOed->GetValue(k)==kFALSE))
                  {
                      ThisEventIsGood = kFALSE;
                      VETOedCounter = VETOedCounter + 1;
                  }
              }
              
              else if (AnaLeafs->Prim_PDG->GetValue(k)==-11) // Positrons
              {
                  Total_Particles[9] = Total_Particles[9] + 1;
                  if ((AnaLeafs->Prim_IsVETOed->GetValue(k))&&(AnaLeafs->Prim_Detected->GetValue(k))) {VETOed_Particles[9] = VETOed_Particles[9] + 1;}
                  if (AnaLeafs->Prim_Detected->GetValue(k)) {Kept_Particles[9] = Kept_Particles[9] + 1;}
                  if ((AnaLeafs->Prim_Resolved->GetValue(k))&&(AnaLeafs->Prim_Detected->GetValue(k))) {Resolved_Particles[9] = Resolved_Particles[9] + 1;}
                  
                  // For each charged particle it is the case: if it is detected but not VETOed, we are in trouble!
                  if ((AnaLeafs->Prim_Detected->GetValue(k)==kTRUE)&&(AnaLeafs->Prim_IsVETOed->GetValue(k)==kFALSE))
                  {
                      ThisEventIsGood = kFALSE;
                      VETOedCounter = VETOedCounter + 1;
                  }
              }
              
              else {cout << "### ERROR: We have an unidentified primary particle! | Event = " << Event << " | PDG = " << AnaLeafs->Prim_PDG->GetValue(k) << " | Index = " << k << "\n";}
              
              Total_Particles[10] = Total_Particles[10] + 1;
              if ((AnaLeafs->Prim_IsVETOed->GetValue(k))&&(AnaLeafs->Prim_Detected->GetValue(k))) {VETOed_Particles[10] = VETOed_Particles[10] + 1;}
              if (AnaLeafs->Prim_Detected->GetValue(k)) {Kept_Particles[10] = Kept_Particles[10] + 1;}
              if ((AnaLeafs->Prim_Resolved->GetValue(k))&&(AnaLeafs->Prim_Detected->GetValue(k))) {Resolved_Particles[10] = Resolved_Particles[10] + 1;}              
          }
      }
      
      // --------------------------------------------------------------------------------- Now we looped over all particles, do the final issueas:
          
      // Count number of UnVETOed reconstructed points. Number of appearently good events is the number of events 
      // that have the right number of UnVETOed vertices. The number of acceptable events is if in addition, 
      // all UnVETOed vertices belong to neutrons from the target.
      Nrec = AnaLeafs->N_Reconstructed_PrimPoints->GetValue(0);
      pointscounter = 0;
      Appearent_Test = kTRUE;
      
      if (Nrec>0)
      {
        for (Int_t k = 0; k<Nrec; ++k)
        {
            if ((Inputs->VETO_select==3)||(Inputs->VETO_select==5)) // make something out of it in case we do not know which rec. prim. interaction points to VETO:
            {
                for (Int_t k2 = 0; k2<Nprims; ++k2)
                {
                    if (AnaLeafs->TrackID_Reconstructed_PrimPoints->GetValue(k)==AnaLeafs->Prim_TrackID->GetValue(k2))
                    {
                        if (AnaLeafs->Prim_IsVETOed->GetValue(k2)==kFALSE) 
                        {
                            // Not VETOed: count it:
                            pointscounter = pointscounter + 1;
                            
                            // Check that it is a resolved neutron from the target:
                            if (!((AnaLeafs->Prim_PDG->GetValue(k2)==2112)&&(AnaLeafs->Prim_IsFromTarget->GetValue(k2)==kTRUE)))
                            {
                                Appearent_Test = kFALSE;
                            }
                        }
                    }
                }
            }
            else
            {
              if (AnaLeafs->Reconstructed_PrimPoints_IsVETOed->GetValue(k)==kFALSE) 
              {
                  // If the vertex is not VETOed, then at first we count it.
                  pointscounter = pointscounter + 1;
                  
                  // In Addition, check that it belongs to a resolved neutron from the target:
                  for (Int_t k2 = 0; k2<Nprims; ++k2)
                  {
                      if (AnaLeafs->TrackID_Reconstructed_PrimPoints->GetValue(k)==AnaLeafs->Prim_TrackID->GetValue(k2))
                      {
                          if (!((AnaLeafs->Prim_PDG->GetValue(k2)==2112)&&(AnaLeafs->Prim_IsFromTarget->GetValue(k2)==kTRUE)))
                          {
                              Appearent_Test = kFALSE;
                          }
                      }
                  }
              }
            }
        }
      }
      
      // in order for this event to be really good, one also should have no 'ghost' reconstructed primary interaction points.
      // this means that there were more points reconstructed then there are primary particles. But only NON-VETOed ghost points
      // pose problems, since we will mistake them for neutrons.
      for (Int_t k = 0; k<Nrec; ++k)
      {
         if ((AnaLeafs->TrackID_Reconstructed_PrimPoints->GetValue(k)<0)&&(AnaLeafs->Reconstructed_PrimPoints_IsVETOed->GetValue(k)==kFALSE)) {ThisEventIsGood = kFALSE;}
      }
      
      // Now after we did all the particles: increase the good event counter. 
      // NOTE: in addition, the number of detected & VETOed particles + the number of good neutrons should equal the number of reconstructed points:
      // NOTE: in addition, the number of detected & VETOed particles + the number of good neutrons should equal the number of reconstructed points:
      if ((ThisEventIsGood)&&(NeutronCounter==Inputs->NneutronsGood)) {GoodEvents = GoodEvents + 1;}
      if ((pointscounter==Inputs->NneutronsGood)) {AppearentEvents = AppearentEvents + 1;}
      if ((pointscounter==Inputs->NneutronsGood)&&(Appearent_Test==kTRUE)&&(pointscounter==NtargetCounter)) {AcceptableEvents = AcceptableEvents + 1;}
      
      // We also would like to make a little less strict condition: An event is acceptable if all of its unVETOed vertices
      // belong to resolved neutrons from the target. The number of unVETOed vertices should also 
      
      // NOTE NOTE NOTE NOTE
      // Appearent events simply means the number of UnVETOed vertices equals 0, 1, ... since that is what we think are the amounts of neutrons.
      // ==> So we should only check that Inputs->NneutronsGood equals pointscounter.
      // Acceptable events means that, in addition, each UnVETOed vertex indeed belongs to a neutron from the target.
      // ==> So in addition, we should check that each unVETOed vertex belongs to a neutron from the target. We match
      // ==> the vertices with primary particles. If a match is found, but it is not a neutron from the target, the Appearent_Test is set
      // ==> to kFALSE. Hence in addition to Appearent Events and Appearent_Test, we should make sure there are no ghosts. We circumvent this
      // ==> by checking that the total number of vertices equals the total number of the neutrons from the target.
      // Good events means that, in addition, there are no UnVETOed charged particles present.
      // ==> At first we check for each particle that, if it is charged, it should either be VETOed or not detected.
      // ==> If it is a neutron not from the target, it is still OK, if this neutron is VETOed or not detected.
      // ==> if it is from the target, it should be resolved and not VETOed.
      // ==> In addition, there should be no ghosts. This is what we check for.

      // Give a sign of life:
      if ((Event+1)%1000==0) {cout << "We processed " << Event+1 << " Events\n";}
  }
  
  cout << "The AnalysisTree is read out succesfully.\n\n"
       << "==========================================================================\n\n";
  
  // ====================================================================================
  // Now write down our results:
       
  cout << "Identified primary particles:\n\n";
  cout << "Species:        %VETOed:          %Detected:        %Resolved:        Total:\n";
       
  for (Int_t k = 0; k<12; ++k)
  {
      if (k==0) {cout << "Protons:        ";}
      if (k==1) {cout << "Neutrons:       ";}
      if (k==2) {cout << "Electrons:      ";}
      if (k==3) {cout << "Gammas:         ";}
      if (k==4) {cout << "Pions:          ";}
      if (k==5) {cout << "Muons:          ";}
      if (k==6) {cout << "Alphas:         ";}
      if (k==7) {cout << "C-ions:         ";}
      if (k==8) {cout << "Fragments:      ";}
      if (k==9) {cout << "Positrons:      ";}
      if (k==10) {cout << "Total:          ";}
      if (k==11) {cout << "N from Target:  ";}
      
      if (Total_Particles[k]>0) {
      cout << 100.0*((Int_t) VETOed_Particles[k])/((Int_t) Total_Particles[k]) << "           "
           << 100.0*((Int_t) Kept_Particles[k])/((Int_t) Total_Particles[k]) << "           "
           << 100.0*((Int_t) Resolved_Particles[k])/((Int_t) Total_Particles[k]) << "           "
           << Total_Particles[k] << "\n";}
      else
      {
      cout << "0                 0                 0                 0\n";
      }
  }
  
  // Print the number of events:
  cout << "\n\nTotal Number of Events = " << nEvents << "\n"
           << "Appearent Number of fully solved Events = " << AppearentEvents << "\n"
           << "True Number of acceptably solved Events = " << AcceptableEvents << "\n"
           << "True Number of fully solved Events = " << GoodEvents << "\n"
           << "Number of neutrons we are looking for: " << Inputs->NneutronsGood << "\n\n\n";
  
  // Finish:
  cout << "Program is finished now.\n\n\n";
  // Done!
  
}
}

// Now define the main for stand-alone compilation:
#ifndef __CINT__
#include <TApplication.h>
int main(int argc, char** argv)
{
  TApplication* TheApp = new TApplication("VETOedParticles",&argc,argv);
  VETOedParticles();
  //TheApp->Run();
  return 0;
}
#endif