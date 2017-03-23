// include the required C++ headers:
#include <iostream>

// include the required root headers:
#include "TH2D.h"
#include "TH3D.h"

// include own headers:
#include "../Analysis/Leaf_Connect.h"
#include "../Analysis/Inputs_Load.h"
#include "AnaLeafs_Connect.h"
#include "../Analysis/Volumes.h"
#include "DrawHistDouma_2D_flat.h"

// use the standard C++ variables:
using namespace std;

void Probability()
{
  // This function paints the production points of all tracks with a given PDG-code.
  // This can be used to reproduce the slides of Ken Miki.
    
  // output is given on the screen.
    
  // Written by C. A. Douma.
    
  // ===================================================================================
  // Obtain the Series and the counter from sed. We also need the PDG-code from
  // sed to know which particles we are looking for:
  Int_t Series = 0;
Series = 0;

  Int_t Counter = 1;
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
  
  // In this particular case we also need to connect to the Monte-Carlo tree:
  Leaf_Structure MCLeafsen;
  Leaf_Structure* MCLeafs = &MCLeafsen;
  Bool_t MC_connect = Leaf_Connect(MCLeafs,Counter,Series);
  
  // Then check if everything worked:
  if (!(Connect&&Load&&MC_connect))
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
  
  // Declare what we need:
  Int_t Size = 0;
  Int_t WindowCounter = 0;
  Int_t AirCounter = 0;
  Int_t xpoint = 0.0;
  Int_t ypoint = 0.0;
  Int_t zpoint = 0.0;
  Int_t MotherID = 0;
  Bool_t Window = kFALSE;
  Bool_t Air = kFALSE;
  Bool_t NeuLAND = kFALSE;
  Bool_t Target = kFALSE;
  
  // Now perform the event loop over the tree:
  for (Long64_t Event = 0; Event<nEvents; ++Event)
  {
      // read out an event:
      nBytes = MCLeafs->TheTree->GetEntry(Event); // NOTE: The MC-tree!
      
      // Size of MCTracks:
      Size = MCLeafs->MCTrack_fPdgCode->GetNdata();
      
      // reset boolians:
      Window = kFALSE;
      Air = kFALSE;
      
      for (Int_t k = 0; k<Size; ++k)
      {
          xpoint = MCLeafs->MCTrack_fStartX->GetValue(k);
          ypoint = MCLeafs->MCTrack_fStartY->GetValue(k);
          zpoint = MCLeafs->MCTrack_fStartZ->GetValue(k);
          MotherID = MCLeafs->MCTrack_fMotherId->GetValue(k);
          
          // check if the track came from the window:
          if ((xpoint<100.0)&&(xpoint>-100.0)&&(ypoint<100.0)&&(ypoint>-100.0)&&(zpoint>755.0)&&(zpoint<770.0))
          {
              MotherID = MCLeafs->MCTrack_fMotherId->GetValue(k);
              
              // Check if the mother came from the target:
              Target = (MCLeafs->MCTrack_fMotherId->GetValue(MotherID)==-1);
              
              // check if this track does not return to NeuLAND:
              NeuLAND = kFALSE;
              while (MotherID>=0)
              {
                  xpoint = MCLeafs->MCTrack_fStartX->GetValue(MotherID);
                  ypoint = MCLeafs->MCTrack_fStartY->GetValue(MotherID);
                  zpoint = MCLeafs->MCTrack_fStartZ->GetValue(MotherID);
                  MotherID = MCLeafs->MCTrack_fMotherId->GetValue(MotherID);
                  
                  if (PointInsideNeuLAND(Inputs,xpoint,ypoint,zpoint)) {NeuLAND = kTRUE;}
              }
              
              // Count the event:
              if ((Window==kFALSE)&&(Target==kTRUE)) {WindowCounter = WindowCounter + 1; Window = kTRUE;}
          }
          else if ((zpoint>770.0)&&(zpoint<1368.0)) // check for the air:
          {
              MotherID = MCLeafs->MCTrack_fMotherId->GetValue(k);
              
              // Check if the mother came from the target:
              Target = (MCLeafs->MCTrack_fMotherId->GetValue(MotherID)==-1);
              
              // chekc if we go back to NeuLAND:
              NeuLAND = kFALSE;
              while (MotherID>=0)
              {
                  xpoint = MCLeafs->MCTrack_fStartX->GetValue(MotherID);
                  ypoint = MCLeafs->MCTrack_fStartY->GetValue(MotherID);
                  zpoint = MCLeafs->MCTrack_fStartZ->GetValue(MotherID);
                  MotherID = MCLeafs->MCTrack_fMotherId->GetValue(MotherID);
                  
                  if (PointInsideNeuLAND(Inputs,xpoint,ypoint,zpoint)) {NeuLAND = kTRUE;}
              }
              
              // count event:
              if ((Air==kFALSE)&&(Target==kTRUE)) {AirCounter = AirCounter + 1; Air = kTRUE;}
          }
      }
      
      // Give a sign of life:
      if ((Event+1)%1000==0) {cout << "We processed " << Event+1 << " Events\n";}
  }
  
  cout << "The AnalysisTree is read out succesfully.\n\n"
       << "==========================================================================\n\n";
  
  // ====================================================================================
  cout << "WindowCounter = " << WindowCounter << "\n"
       << "AirCounter = " << AirCounter << "\n\n";  
       
  // Done!
}
}

// Now define the main for stand-alone compilation:
#ifndef __CINT__
#include <TApplication.h>
int main(int argc, char** argv)
{
  TApplication* TheApp = new TApplication("Probability",&argc,argv);
  Probability();
  //TheApp->Run();
  return 0;
}
#endif
