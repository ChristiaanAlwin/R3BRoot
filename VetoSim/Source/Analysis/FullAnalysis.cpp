// This function loops over SingleAnalysis.cpp
// It serves no other purpose that to do this loop.

#include <iostream>
#include "Bethe.h"
#include "SingleAnalysis.cpp"

using namespace std;

void FullAnalysis()
{
  Int_t Ntimes = 0;
Ntimes = 1;

  Int_t Series = 0;
Series = 0;
  
  // NOTE: Before looping we first have to do the Bethe-Bloch!

  // Check if Bethe-Bloch is relevant:
  FileStat_t stom;
  Int_t inputs_test = gSystem->GetPathInfo("../../Data/Inputs/Inputs.root",stom);
   
  if (inputs_test==0)
  {
    TFile* inputs = new TFile("../../Data/Inputs/Inputs.root","read");
    TH1I* Integers = (TH1I*) inputs->Get("Integers");
    Int_t SecParticles = Integers->GetBinContent(308);
    
    // Then perform Bethe-Bloch:
    if (SecParticles==1)
    {
      BetheMaster();
    }
  }
 
  // Now we can do the real analysis:
  
  for (Int_t k = 1; k<(Ntimes+1); ++k)
  {
    cout << "######################################################################################################\n\n"
         << "### NOTE: We now start analysis run " << k << "\n\n";
	 
    SingleAnalysis(k,Series);
  } 
  
}

// Now define the main for stand-alone compilation:
#ifndef __CINT__
#include <TApplication.h>
int main(int argc, char** argv)
{
  TApplication* TheApp = new TApplication("FullAnalysis",&argc,argv);
  FullAnalysis();
  // TheApp->Run(); ==> This macro does not contain graphics.
  return 0;
}
#endif