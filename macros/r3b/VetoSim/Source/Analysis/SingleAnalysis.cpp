// Inclusion of the required C++ headers
#include <iostream>

// Inclusion of the required ROOT headers:
#include "TRandom3.h"

// Inclusion of our own headers:
#include "Inputs_Load.h"
#include "Leaf_Connect.h"
#include "Tree_Definition.h"
#include "Volumes.h"
#include "Secondary_Calculation.h"
#include "DeleteRemainders.h" 
#include "Tdiff.h"
#include "GetTotalEdep.h"

// Inclusion of our analysis task (AFTER the other headers!!!)
#include "DeleteEverything.h"
#include "Identify_PrimParticles.h"
#include "TrackEvents.h"
#include "QTDC.h"
#include "QTDC_Digi.h"
#include "Secondary_Particles.h"
#include "ProcessClusters.h"
#include "ProcessTracker.h"
#include "Detectors.h"
// NOTE: add you analysis tasks here!

// Inclusion of our VETO conditions:
#include "No_VETO_Condition.h"
#include "Naive_VETO_Condition.h"
#include "TOF_VETO_Condition.h"
#include "Segmented_VETO_Condition.h"
#include "Cluster_VETO_Condition.h"
#include "Tracker_VETO_Condition.h"
#include "TrackerCluster_VETO_Condition.h"
#include "Full_VETO_Condition.h"
#include "Perfect_TrackerVETO_Condition.h"
#include "Combined_VETO_Condition.h"

// Using the standard C++ variables:
using namespace std;

void SingleAnalysis(Int_t const Counter, Int_t const Series)
{
  // This function performs the analysis of a single simulation run.
  // The input argument counter is used to select which files should
  // be analysed. The outputs are stored in a tree called AnalysisTree.
  // therefore this function has no outputs. The input PosSeries
  // tells us which input parameters should be updated with the counter.
  
  // The AnalysisTree is stored in a file with the same name as
  // the MonteCarlo files, but in a different directory. It is
  // stored in the Analysis-directory instead of the 
  // MonteCarlo directory.
  
  // Written by C. A. Douma, for the R3B collaboration.
  
  // =====================================================================================================
  // The first thing we need to do is to obtain all our inputs, leafs, etc 
  // to work with and to create our AnalysisTree. So before doing anything else,
  // this is what we will do.
  
  cout << "========================================================================================\n\n"
       << "Starting the analysis of a single simulation run...\n\n";

  // Connect to the Leafs of the simulation data:
  Leaf_Structure TheLeafsen;
  Leaf_Structure* TheLeafs = &TheLeafsen;
  Bool_t Connect = Leaf_Connect(TheLeafs,Counter,Series);
  
  // Load all inputs from the Inputs.root-file:
  Inputs_Structure TheInputs;
  Inputs_Structure* Inputs = &TheInputs;
  Bool_t Load = Inputs_Load(Inputs,Counter,Series);

  // Now Create the AnalysisTree and all variables we need to fill it:
  Tree_Structure AnaTrees;
  Tree_Structure* AnaTree = &AnaTrees;
  Bool_t Define = Tree_Definition(AnaTree,Counter,Series);
  
  // Now check if this all worked OK:
  if (!(Connect&&Load&&Define))
  {
    cout << "### ERROR: some initializers failed! ==>\n";
    
    if (!Connect) {cout << "### ERROR: ==> Connection to the simulation Leafs failed!\n";}
    if (!Load) {cout << "### ERROR: ==> Loading of the input parameters failed!\n";}
    if (!Define) {cout << "### ERROR: ==> Definition of the Analysis Tree failed!\n";}
    
    cout << "\n### ERROR: Without these initializers the analysis cannot be performed!\n"
         << "### ERROR: Therefore the program is terminated now. Fix the errors first!\n\n\n";
  }
  else
  {
    cout << "### NOTE: All initializers are there!\n\n"
         << "========================================================================================\n\n"
	 << "We begin the analysis now.\n\n\n";
    
  // =====================================================================================================
  // Now that everything we need is there, we can begin with our loop over the Trees that hold our
  // simulation results:
  Long64_t nBytes_MC;
  Long64_t nBytes_Ndigi;
  Long64_t nBytes_Vdigi;
  Long64_t nBytes_Tracker;
  Long64_t nBytes_Other;
  Long64_t nBytes_Star;
  Long64_t N_Events = Inputs->nEvents;
  TRandom3* generator = new TRandom3();
  
  for (Long64_t Event = 0; Event<N_Events; ++Event)
  {
    
    // We begin with loading the current event:
    nBytes_MC = TheLeafs->TheTree->GetEntry(Event);
    if ((TheLeafs->Geant3)&&((Inputs->Detectors_NeuLAND_Exists==1)||(Inputs->Detectors_NeuLAND_Exists==2))&&(Inputs->Detectors_NEBULA_Exists!=1)&&(Inputs->Detectors_NebVETO_Exists!=1)) {nBytes_Ndigi = TheLeafs->NeuDigiTree->GetEntry(Event);}
    if ((TheLeafs->Geant3)&&((Inputs->Detectors_VETO_Exists==1)||(Inputs->Detectors_VETO_Exists==2))) {nBytes_Vdigi = TheLeafs->VetoDigiTree->GetEntry(Event);}
    if (TheLeafs->Tracker_Exists) {nBytes_Tracker = TheLeafs->TrackTree->GetEntry(Event);}
    if ((TheLeafs->Geant3)&&((TheLeafs->CALIFA_exists)||(TheLeafs->STaRTrack_exists))) {nBytes_Other = TheLeafs->OtherTree->GetEntry(Event);}
    if ((TheLeafs->Geant3)&&(TheLeafs->STaRTrack_exists)) {nBytes_Star = TheLeafs->StarTree->GetEntry(Event);}
    
    // Now we perform our analysis tasks:
    if (Inputs->PrimParticles_Exe==1) {Identify_PrimParticles(TheLeafs,Inputs,AnaTree,Event,Counter,generator);} else {Empty_Identify_PrimParticles(TheLeafs,Inputs,AnaTree,Event,Counter,generator);}
    if (Inputs->TrackEvents_Exe==1) {TrackEvents(TheLeafs,Inputs,AnaTree,Event,Counter,generator);} else {Empty_TrackEvents(TheLeafs,Inputs,AnaTree,Event,Counter,generator);}
    if (Inputs->QTDC_Exe==1)
    { if (Inputs->UseDigitizer>0) {QTDC_Digi(TheLeafs,Inputs,AnaTree,Event,Counter,generator);} else {QTDC(TheLeafs,Inputs,AnaTree,Event,Counter,generator);}
    } else {Empty_QTDC(TheLeafs,Inputs,AnaTree,Event,Counter,generator);}    
    if (Inputs->Secondaries_Exe==1) {Secondary_Particles(TheLeafs,Inputs,AnaTree,Event,Counter,generator);} else {Empty_Secondary_Particles(TheLeafs,Inputs,AnaTree,Event,Counter,generator);}
    if (Inputs->Cluster_Exe==1) {ProcessClusters(TheLeafs,Inputs,AnaTree,Event,Counter,generator);} else {ProcessClusters(TheLeafs,Inputs,AnaTree,Event,Counter,generator);}
    if ((TheLeafs->Tracker_Exists)&&(Inputs->AnaTracker_Exe==1)) {ProcessTracker(TheLeafs,Inputs,AnaTree,Event,Counter,generator);} else {Empty_ProcessTracker(TheLeafs,Inputs,AnaTree,Event,Counter,generator);}
    Detectors(TheLeafs,Inputs,AnaTree,Event,Counter,generator);
       
    // After our analysis tasks we perform the VETO condition:
    if (Inputs->VETO_select==1) {Naive_VETO_Condition(TheLeafs,Inputs,AnaTree,Event,Counter,generator);}
    else if (Inputs->VETO_select==2) {TOF_VETO_Condition(TheLeafs,Inputs,AnaTree,Event,Counter,generator);}
    else if (Inputs->VETO_select==3) {Segmented_VETO_Condition(TheLeafs,Inputs,AnaTree,Event,Counter,generator);}
    else if (Inputs->VETO_select==4) {Tracker_VETO_Condition(TheLeafs,Inputs,AnaTree,Event,Counter,generator);}
    else if (Inputs->VETO_select==5) {Cluster_VETO_Condition(TheLeafs,Inputs,AnaTree,Event,Counter,generator);}
    else if (Inputs->VETO_select==6) {TrackerCluster_VETO_Condition(TheLeafs,Inputs,AnaTree,Event,Counter,generator);}
    else if (Inputs->VETO_select==7) {Full_VETO_Condition(TheLeafs,Inputs,AnaTree,Event,Counter,generator);}
    else if ((Inputs->VETO_select==9)||(Inputs->VETO_select==11)||(Inputs->VETO_select==18)) {Perfect_TrackerVETO_Condition(TheLeafs,Inputs,AnaTree,Event,Counter,generator);}
    else if ((Inputs->VETO_select==8)||(Inputs->VETO_select==10)||(Inputs->VETO_select==12)||(Inputs->VETO_select==13)||(Inputs->VETO_select==14)||(Inputs->VETO_select==15)||(Inputs->VETO_select==16)||(Inputs->VETO_select==17))
    {Combined_VETO_Condition(TheLeafs,Inputs,AnaTree,Event,Counter,generator);}
    
    else {No_VETO_Condition(TheLeafs,Inputs,AnaTree,Event,Counter,generator);}
       
    // NOTE: Add your analysis task here in the following form: 
    // NOTE: NewTask(TheLeafs,Inputs,AnaTree,Event,Counter,generator);
    
    // Then we fill the Analysis Tree:
    AnaTree->TheTree->Fill();
  
    // And after the filling, we must perform all the delete operators 
    // of all dynamical arrays:
    DeleteEverything(TheLeafs,Inputs,AnaTree,Event,Counter);
  
    // Then we give a progress report:
    if ((Event+1)%100 == 0) {cout << "We processed " << Event+1 << " events.\n";}
  
    // And then we are done!
  }
  
  delete generator;
  
  // =====================================================================================================
  // write our analysis tree to a file:
  
  cout << "\nOur analysis is succesfully completed.\n\n"
       << "We will now write the obtained Analysis Tree to a file.\n\n";
       
  // since the file for this tree is the only file that still has a pointer to it and this
  // file is also created last, we do:
  AnaTree->TheTree->Write("AnalysisTree",2);
  
  // also write all histograms to the file (if we need to do this):
  
  if (Inputs->Secondaries_Exe==1)
  {
    for (Int_t k1 = 0; k1<10; ++k1)
    {
      for (Int_t k2 = 0; k2<5; ++k2)
      {
        AnaTree->Harray[k1][k2]->Write();
      }
    }
  }
  
  // In order to do everything OK, we must now delete our static arrays, histograms, etc.
  // NOTE: After this command nothing can be done anymore since everything is gone!
  DeleteRemainders(TheLeafs,Inputs,AnaTree);
  
  // Finally: give output:
  cout << "Analysis Tree is written to the correct file.\n\n"
       << "Series parameters: Series = " << Series << "\n"
       << "VETO position = " << Inputs->VETO_geo_Zpos << " [cm]\n"
       << "Analysis Shower radius = " << Inputs->Shower_Radius << " [cm]\n"
       << "Analysis Energy Cut = " << Inputs->VETO_Ecut << " [MeV]\n"
       << "VETO Number of paddles per plane = " << Inputs->VETO_geo_NbarsPerPlane << " [dim. less]\n"
       << "VETO Bar width = " << Inputs->VETO_geo_TotalBarWidth << " [cm]\n"
       << "VETO Bar thickness = " << Inputs->VETO_geo_TotalBarThickness << " [cm]\n"
       << "Proton X starting position = " << Inputs->NeuProtonBegin_X << " [cm]\n"
       << "VETO combined TimeCut = " << Inputs->TimeCut << " [ns]\n"
       << "\n\n"
       << "========================================================================================\n\n"
       << "The program has ended without errors.\n\n\n";
       
  // Close all blocks and finish up:
  }
}