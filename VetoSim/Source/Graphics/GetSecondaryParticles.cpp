// include the required C++ headers:
#include <iostream>

// include the required root headers:

// include own headers:
#include "../Analysis/Inputs_Load.h"
#include "AnaLeafs_Connect.h"

// use the standard C++ variables:
using namespace std;

Int_t** GetSecondaryParticles(Int_t const Counter, Int_t const Series)
{
  // This function counts the number of secondary particles in the Analysis Tree.
  // It requires the counter to connect to the right analysis tree. It requires
  // the Series to get the right inputs.
    
  // output is given in the form of a 2D array of integers: our table. The array is
  // declared with new and then a pointer to this table is returned.
    
  // Written by C. A. Douma.
    
  // ===================================================================================
  
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

  // NOTE: We declare the table here with new on purpose. Then we can give a pointer to this
  // table as output. When the function is ended, the table is then not destroyed, exactly
  // as we want it!
  Int_t** Table = new Int_t*[10];
  for (Int_t k = 0; k<10; ++k)
  {
    Table[k] = new Int_t[5];
  }
  
  // Now initialize the table with zeros:
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
      
      // count our particles:
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
  // Now all we have to do is to return the pointer to the table:
  return Table;
  
  // Finish:
  cout << "Program is finished now.\n\n\n";
  // Done!
}
}