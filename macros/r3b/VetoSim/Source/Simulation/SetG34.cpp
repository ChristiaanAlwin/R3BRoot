// C++ headers:
#include <iostream>
#include <fstream>

// ROOT headers:
#include "TMath.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH1I.h"
#include "TObjString.h"

// Own headers:

// Use standard namespace:
using namespace std;

void SetG34()
{
    // We read the inputs-file and re-set this file to a TGeant34 MC Engine.
    
    // Read the input-file:
    TFile* inputs = new TFile("../../Data/Inputs/Inputs.root","update");
    TH1D* Doubles = (TH1D*) inputs->Get("Doubles");
    TH1I* Integers = (TH1I*) inputs->Get("Integers");
    
    // Set the Monte Carlo String:
    TObjString* Monte_Carlo_Engine = new TObjString("TGeant34");
    Integers->SetBinContent(249,0); // So that we do execute digitizers, etc with G4.
    Integers->SetBinContent(204,1); // So that we really do a full run.
    
    // Write everything to the inputs-file:
    Monte_Carlo_Engine->Write("Monte_Carlo_Engine",2);
    Doubles->Write("Doubles",2);
    Integers->Write("Integers",2);
    
    // Done!
}
    
// ===========================================================================================================

// Now define the main for stand-alone compilation:

#ifndef __CINT__
#include <TApplication.h>
int main(int argc, char** argv)
{
  TApplication* TheApp = new TApplication("SetG34",&argc,argv);
  SetG34();
  return 0;
}
#endif
