// include the required C++ headers:
#include <iostream>

// include the required root headers:
#include "TRandom3.h"

// include own headers:
#include "../Analysis/Inputs_Load.h"
#include "AnaLeafs_Connect.h"

// use the standard C++ variables:
using namespace std;

std::vector<Int_t> GetSimplePerformance(Int_t const Counter, Int_t const Series)
{
  // This function constructs the simple VETO performance from the
  // analysis tree. It requires the counter to select the appropriate tree.
  // The Series argument is required so that we know what kind of series we
  // are analyzing
    
  // output is given in the form of an std::vector containing the
  // performances of the various kinds.
    
  // Written by C. A. Douma.
    
  // ===================================================================================
  // Before we do anything, we declare the output:
  std::vector<Int_t> TheOutput;
  TheOutput.clear();
  
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
  
  // Set the counters to zero:
  Int_t nEvents_NeuLAND_Ethres = 0;
  Int_t nEvents_VETO_Ethres = 0;
  Int_t nEvents_VETO_EthresTOF = 0;
  Int_t nEvents_VETO_EthresTOFR = 0;
  Int_t nEvents_VETO_EthresBack = 0;
  Int_t nEvents_VETO_EthresPrim = 0;
  Int_t nEvents_VETO_EthresBoth = 0;
  Int_t nEvents_VETO_EthresTOFPrim = 0;
  Int_t nEvents_VETO_EthresTOFRPrim = 0;
  Int_t nEvents_SecParticles = 0;
  Int_t Number_SecParticles = 0;

  // Declare the variables that we need:
  Int_t NbarsFired_Neu = 0;
  Int_t NbarsFired_VETO = 0;
  Double_t Neu_TOF = 0.0;
  Double_t VETO_TOF = 0.0;
  Double_t Neu_TOF_Res = 0.0;
  Double_t VETO_TOF_Res = 0.0;
  Bool_t Prim = kFALSE;
  Bool_t Back = kFALSE;
  Bool_t SecPart = kFALSE;
  
  TRandom3* generator = new TRandom3();
  
  // Now perform the event loop over the tree:
  for (Long64_t Event = 0; Event<nEvents; ++Event)
  {
      // read out an event:
      nBytes = AnaLeafs->TheTree->GetEntry(Event);
      
      // First we count events that are detected by NeuLAND:
      NbarsFired_Neu = AnaLeafs->NbarsFired_NeuLAND->GetValue(0);
      if (NbarsFired_Neu>0)
      {
          nEvents_NeuLAND_Ethres = nEvents_NeuLAND_Ethres+1;
          
          // Then we count how many of those events meet the energy
          // condition on the VETO:
          NbarsFired_VETO = AnaLeafs->NbarsFired_VETO->GetValue(0);
      
          if (NbarsFired_VETO>0)
          {
              // Then only events that are detected by both NeuLAND and
              // the VETO are rocessed from now on:
              nEvents_VETO_Ethres = nEvents_VETO_Ethres+1;
      
              // Then we obtain the tracking of particles (just overall Energetic tracking)
              Prim = AnaLeafs->Overal_EPrim->GetValue(0);
              Back = AnaLeafs->Overal_EBack->GetValue(0);
	      
	      // Then obtain the presence of secondary particles:
	      SecPart = AnaLeafs->Secondary_Particles->GetValue(0);
	      Number_SecParticles = Number_SecParticles + AnaLeafs->Secondary_NallParticles->GetValue(4);
      
	      // ============ Then we will obtain the TOF. Think about the inputs!
	      
              // Then we obtain NeuLAND TOF:
              if ((Inputs->TOFchoice==1)||(Inputs->TOFchoice==2)||(Inputs->TOFchoice==3)||(Inputs->TOFchoice==4)) {Neu_TOF = AnaLeafs->Neu_TotalTOF->GetValue(0);} // in all 4 cases, the precise NeuLAND TOF is the same.
              if ((Inputs->TOFchoice==1)||(Inputs->TOFchoice==3)) {Neu_TOF_Res = AnaLeafs->Neu_TotalTOF->GetValue(0) + generator->Gaus(0.0,1e-3*(Inputs->Neu_TimeResolution));} // put in resolution after minimizing
              if ((Inputs->TOFchoice==2)||(Inputs->TOFchoice==4)) {Neu_TOF_Res = AnaLeafs->Neu_TotalTOF_Res->GetValue(0);} // put in resolution before minimizing.
              
              // Then obtain VETO precise TOF. at 1 & 2 we use ordinary TOF, 3 & 4 are TrackTOF:
              if ((Inputs->TOFchoice==1)||(Inputs->TOFchoice==2)) {VETO_TOF = AnaLeafs->VETO_TotalTOF->GetValue(0);}
              if ((Inputs->TOFchoice==3)||(Inputs->TOFchoice==4)) {VETO_TOF = AnaLeafs->VETO_TotalTrackTOF->GetValue(0);}
              
              // Then obtain VETO resolution TOF. There are 4 ways to actually do this:
	      if (Inputs->TOFchoice==1) {VETO_TOF_Res = AnaLeafs->VETO_TotalTOF->GetValue(0) + generator->Gaus(0.0,1e-3*(Inputs->VETO_TimeResolution));} // ordinary TOF, add resolution after minimizing
	      if (Inputs->TOFchoice==2) {VETO_TOF_Res = AnaLeafs->VETO_TotalTOF_Res->GetValue(0);} // ordinary TOF, add resolution before minimizing
	      if (Inputs->TOFchoice==3) {VETO_TOF_Res = AnaLeafs->VETO_TotalTrackTOF->GetValue(0) + generator->Gaus(0.0,1e-3*(Inputs->VETO_TimeResolution));} // TrackTOF, add resolution after minimizing
	      if (Inputs->TOFchoice==4) {VETO_TOF_Res = AnaLeafs->VETO_TotalTrackTOF_Res->GetValue(0);} // TrackTOF, add resolution before minimizing
              
              // ============

              // then now we can do the rest of our counting:
              if (Prim) {nEvents_VETO_EthresPrim = nEvents_VETO_EthresPrim + 1;}
              if (Back&&(!Prim)) {nEvents_VETO_EthresBack = nEvents_VETO_EthresBack + 1;}
              if (Prim&&Back) {nEvents_VETO_EthresBoth = nEvents_VETO_EthresBoth + 1;}
              if (VETO_TOF<Neu_TOF) {nEvents_VETO_EthresTOF = nEvents_VETO_EthresTOF + 1;}
              if (VETO_TOF_Res<Neu_TOF_Res) {nEvents_VETO_EthresTOFR = nEvents_VETO_EthresTOFR + 1;}
              if (Prim&&(VETO_TOF<Neu_TOF)) {nEvents_VETO_EthresTOFPrim = nEvents_VETO_EthresTOFPrim + 1;}
              if (Prim&&(VETO_TOF_Res<Neu_TOF_Res)) {nEvents_VETO_EthresTOFRPrim = nEvents_VETO_EthresTOFRPrim + 1;}
              if (SecPart) {nEvents_SecParticles = nEvents_SecParticles + 1;}
              
              // And now we close all blocks:
          }
      }              
      
      // Give a sign of life:
      if ((Event+1)%1000==0) {cout << "We processed " << Event+1 << " Events\n";}
  }
  
  cout << "The AnalysisTree is read out succesfully.\n\n"
       << "==========================================================================\n\n";
  
  // ====================================================================================
  // And then finally we have to put our information into our std::vector:
       
  TheOutput.push_back(nEvents);
  TheOutput.push_back(nEvents_NeuLAND_Ethres);
  TheOutput.push_back(nEvents_VETO_Ethres);
  TheOutput.push_back(nEvents_VETO_EthresBack);
  TheOutput.push_back(nEvents_VETO_EthresPrim);
  TheOutput.push_back(nEvents_VETO_EthresBoth);
  TheOutput.push_back(nEvents_VETO_EthresTOF);
  TheOutput.push_back(nEvents_VETO_EthresTOFR);  
  TheOutput.push_back(nEvents_VETO_EthresTOFPrim);
  TheOutput.push_back(nEvents_VETO_EthresTOFRPrim);
  TheOutput.push_back(nEvents_SecParticles);
  TheOutput.push_back(Number_SecParticles);
  
  // Then close all blocks:
  }
  
  // Finish:
  cout << "Program is finished now.\n\n\n";
  return TheOutput;
  // Done!
}