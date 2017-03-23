// C++ headers:
#include <iostream>
#include <fstream>

// ROOT headers:
#include "TMath.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH1I.h"
#include "TObjString.h"
#include "TString.h"

// Own headers:

// Use standard namespace:
using namespace std;

void CheckG4Engine()
{
    // We read the inputs-file and create a .txt-file with a number stating which
    // Monte Carlo Engine is used. This task is required by the shell-scripts to
    // steer the simulation.
    
    // Read the input-file:
    TFile* inputs = new TFile("../../Data/Inputs/Inputs.root","read");
    TH1D* Doubles = (TH1D*) inputs->Get("Doubles");
    TH1I* Integers = (TH1I*) inputs->Get("Integers");
    
    // Get the Monte Carlo String:
    TObjString* Monte_Carlo_Engine_Obj = (TObjString*) inputs->Get("Monte_Carlo_Engine");
    TString Monte_Carlo_Engine = Monte_Carlo_Engine_Obj->GetString();
    
    // Create the .txt-file:
    std::ofstream Engine ("../../Data/Inputs/MCengine.txt", std::ofstream::out); // recreate file
    
    // Decide and write the number:
    if (Monte_Carlo_Engine=="TGeant3") {Engine << "1";}
    else if (Monte_Carlo_Engine=="TGeant4") {Engine << "2";}
    else if (Monte_Carlo_Engine=="TGeant34") {Engine << "3";}
    else {Engine << "0";}
    
    // Close the fine and finish:
    Engine.close();
    
    // Done!
}
    
// ===========================================================================================================

// Now define the main for stand-alone compilation:

#ifndef __CINT__
#include <TApplication.h>
int main(int argc, char** argv)
{
  TApplication* TheApp = new TApplication("CheckG4Engine",&argc,argv);
  CheckG4Engine();
  return 0;
}
#endif
