// include the required C++ headers:
#include <iostream>

// include the required root headers:

// include own headers:
#include "../Analysis/Inputs_Load.h"
#include "AnaLeafs_Connect.h"

// use the standard C++ variables:
using namespace std;

void Table()
{
  // This function constructs the simple VETO performance from the
  // analysis tree. It requires the counter to select the appropriate tree.
  // The Series argument is required so that we know what kind of series we
  // are analyzing
    
  // output is given in the form of an std::vector containing the
  // performances of the various kinds.
    
  // Written by C. A. Douma.
    
  // ===================================================================================
  // Obtain the counter and the series from sed:
  Int_t Counter = 3;
Counter = 1;
  
  Int_t Series = 1; 
Series = 0;
  
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
  
  Int_t Table [10][5];
  for (Int_t k = 0; k<10; ++k)
  {
      for (Int_t n = 0; n<5; ++n)
      {
          Table[k][n] = 0;
      }
  }
  
  // Now perform the event loop over the tree:
  for (Long64_t Event = 0; Event<nEvents; ++Event)
  {
      // read out an event:
      nBytes = AnaLeafs->TheTree->GetEntry(Event);
      
      // NOTE: Put here your code to read out the analysis tree.
      for (Int_t k = 0; k<5; ++k)
      {
        Table[0][k] = Table[0][k] + AnaLeafs->Secondary_Nprotons->GetValue(k);
        Table[1][k] = Table[1][k] + AnaLeafs->Secondary_Nneutrons->GetValue(k);
        Table[2][k] = Table[2][k] + AnaLeafs->Secondary_Nelectrons->GetValue(k);
        Table[3][k] = Table[3][k] + AnaLeafs->Secondary_Ngammas->GetValue(k);
        Table[4][k] = Table[4][k] + AnaLeafs->Secondary_Npions->GetValue(k);
        Table[5][k] = Table[5][k] + AnaLeafs->Secondary_Nmuons->GetValue(k);
        Table[6][k] = Table[6][k] + AnaLeafs->Secondary_Nalphas->GetValue(k);
        Table[7][k] = Table[7][k] + AnaLeafs->Secondary_NCions->GetValue(k);
        Table[8][k] = Table[8][k] + AnaLeafs->Secondary_Nfragments->GetValue(k);
        Table[9][k] = Table[9][k] + AnaLeafs->Secondary_NallParticles->GetValue(k);
      }
      
      // Give a sign of life:
      if ((Event+1)%1000==0) {cout << "We processed " << Event+1 << " Events\n";}
  }
  
  cout << "The AnalysisTree is read out succesfully.\n\n"
       << "==========================================================================\n\n";
  
  // ====================================================================================
  
  // print model assumptions:
  cout << "### NOTE: Model assumptions are the following: row 1 are ALL produced particles within\n"
       << "### NOTE: the primary volume. Row 2 are the particles that have their momentum vector aimed\n"
       << "### NOTE: towards the VETO, but are produced outside the VETO (at the side of the VETO it\n"
       << "### NOTE: is treated as inside the VETO). Then the third row are only those particles that\n"
       << "### NOTE: have sufficient energy to travel outside NeuLAND. This is computed based on\n"
       << "### NOTE: Bethe-Bloch formula (modified for electrons). For the 4th row we take all Monte Carlo\n"
       << "### NOTE: points inside the VETO and track them back untill they leave the VETO. Hence this is\n"
       << "### NOTE: independent of the 3th row. The 5th is like the 4th, only now we also require that\n"
       << "### NOTE: the tracked back particle has a total energy deposition in at least one VETO bar that\n"
       << "### NOTE: is above the threshold. Hence for charged particles, row 3, 4 and 5 should be about\n"
       << "### NOTE: identical. There can be a difference of sqrt(n) between 3 and 4 since they come\n"
       << "### NOTE: from independent models (own model VS Geant4). In order to get agreements, two\n"
       << "### NOTE: additions need to be taken into account. The first is that for a particle, in order\n"
       << "### NOTE: to hit the VETO, it should not only travel through NeuLAND, but also through the\n"
       << "### NOTE: first layer of tape and foil of the VETO. Hence we add this distance to the\n"
       << "### NOTE: travel distance through NeuLAND (3th row). We correct the thickness of the Aluminium foil\n"
       << "### NOTE: for its density. The second thing is that for very low energies (order 50 keV),\n"
       << "### NOTE: the air becomes inpenetrabel. Hence we add to the 3th row the requirement that\n"
       << "### NOTE: we only count particles above a certain threshold (see SetParameters.cpp)\n\n\n";
       
  // print our table in absolute numbers:
  cout << "TABLE:           "
       << "Protons:    "
       << "Neutrons:   "
       << "Electrons:  "
       << "Gammas:     "
       << "Pions:      "
       << "Muons:      "
       << "Alphas:     "
       << "C-ions:     "
       << "Fragments:  "
       << "Total:      "
       << "\n";

  for (Int_t k = 0; k<5; ++k)
  {
      if (k==0) {cout << "Produced:        ";}
      if (k==1) {cout << "Aimed at VETO:   ";}
      if (k==2) {cout << "Reached VETO:    ";}
      if (k==3) {cout << "Hit VETO:        ";}
      if (k==4) {cout << "Detected:        ";}
      
      for (Int_t n = 0; n<10; ++n)
      {
          cout << Table[n][k] << "      ";
          
          if (k==1) {cout << " ";}
          if (k==2) {cout << "   ";}
          if (k==3) {cout << "   ";}
          if (k==4) {cout << "   ";}
      }
      cout << "\n";     
  }
  cout << "\n\n";     
  
  // Now print the table in percentages:
  Double_t Table2 [10][5];
  for (Int_t k = 0; k<10; ++k)
  {
      for (Int_t n = 0; n<5; ++n)
      {
          Table2[k][n] = 100.0*((Int_t) Table[k][n])/((Int_t) nEvents);
      }
  }
  
  cout << "% TABLE:         "
       << "Protons:    "
       << "Neutrons:   "
       << "Electrons:  "
       << "Gammas:     "
       << "Pions:      "
       << "Muons:      "
       << "Alphas:     "
       << "C-ions:     "
       << "Fragments:  "
       << "Total:      "
       << "\n";

  for (Int_t k = 0; k<5; ++k)
  {
      if (k==0) {cout << "Produced:        ";}
      if (k==1) {cout << "Aimed at VETO:   ";}
      if (k==2) {cout << "Reached VETO:    ";}
      if (k==3) {cout << "Hit VETO:        ";}
      if (k==4) {cout << "Detected:        ";}
      
      for (Int_t n = 0; n<10; ++n)
      {
          cout << Table2[n][k] << "     ";
          
          if (k==1) {cout << " ";}
          if (k==2) {cout << "  ";}
          if (k==3) {cout << "   ";}
          if (k==4) {cout << "   ";}
      }
      cout << "\n";     
  }
  cout << "\n\n";
  
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
  TApplication* TheApp = new TApplication("Table",&argc,argv);
  Table();
  // TheApp->Run(); // NOTE: No graphics here.
  return 0;
}
#endif