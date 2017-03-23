// Add the C++ headers:
#include <iostream>
#include <unistd.h> // NOTE: Linux-specific! Allows us to use chdir-command.

// Add the required root headers:


// Add our own required headers:
#include "SetSim.cpp"
#include "../Simulation/Create_ASCII_File.cpp"
#include "../Simulation/R3B_Simulation.cpp"
#include "../Simulation/NeuLAND_Digitizer.cpp"
#include "../Simulation/VETO_Digitizer.cpp"

// Use the standard C++ variables:
using namespace std;

void RunSim()
{
  // This function runs all the simulations and FairTasks required 
  // For Dima his program to compute the cuts on the number of neutrons.
  // We will simply use the code we already have. All we do is to just
  // add a few changes in the parameters...
  
  // Repeat the simulation for different numbers of neutrons:
  Int_t out = 0;
  Bool_t Aimed = kFALSE;
  
  for (Int_t N_Neutrons = 1; N_Neutrons<6; ++N_Neutrons)
  {
    cout << "###################################################################################\n\n"
         << " We Will now perform a Calibration run for " << N_Neutrons << " neutrons...\n\n";
	 
    // Set our parameters OK:
    Aimed = SetSim(N_Neutrons);
    
    if (Aimed)
    {
      // Create our ASCII-file:
      Create_ASCII_File();
    
      // Run our simulation:
      R3B_Simulation();
    
      // Execute the digitizers:
      NeuLAND_Digitizer();
      VETO_Digitizer();
      
      // And that's it. Done!
      // NOTE: that we do not update the counter here because this is a calibration run!
    }
    else
    {
      cout << "### ERROR: The simulation could not be performed!\n### ERROR: The primary beam is not Aimed at NeuLAND!\n\n";
    }    
  }
}