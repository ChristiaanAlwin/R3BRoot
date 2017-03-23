// include the required C++ headers:
#include <iostream>

// include the required root headers:
#include "TSystem.h"
#include "TH1D.h"
#include "TH1I.h"
#include "TFile.h"

// include own headers:
#include "VETOed1Run.cpp"

// use the standard C++ variables:
using namespace std;

void GetEventList()
{
    // This function basically repeats the ./foto vetf 1 command for different numbers
    // of neutrons (up till 8) and then prints the results on the screen.
    
    // Written by C. A. Douma, for the R3B Collaboration.
    
    // ===============================================================================
    // First we obtain the number of runs and the Series from sed:
    Int_t Series = 0;
Series = 0;

    Int_t Nruns = 1;
Nruns = 1;

    // We also want to know which graphs we should paint:
    Int_t Select = 1;
Select = 1;

    // ==================================================================================
    // Simply execute a for-loop:
    
    // First declare what we need:
    std::vector<Double_t> Run;
    Run.clear();
    Double_t nEvents = 0.0;
    Double_t Appearent [9];
    Double_t Acceptable [9];
    Double_t True [9];
    Double_t Appearent_Back [9];
    Double_t Acceptable_Back [9];
    Double_t True_Back [9];
    
    for (Int_t k = 0; k<9; ++k)
    {
        Appearent[k] = 0.0;
        Acceptable[k] = 0.0;
        True[k] = 0.0;
        Appearent_Back[k] = 0.0;
        Acceptable_Back[k] = 0.0;
        True_Back[k] = 0.0;
    }
    
    // then loop:
    for (Int_t k = 0; k<9; ++k)
    {
        cout << "#############################################################################################\n\n"
	     << "We will now process run nr. " << k+1 << "...\n\n";
        
        // Then run the script and get the results:
        Run = VETOed1Run(Nruns,Series,Select,k); // set last one to k, so that it overwrites the inputs!
        nEvents = Run.at(38);
        
        // Then store the arrays:
        Appearent[k] = Run.at(26)*nEvents/100.0;
        Acceptable[k] = Run.at(28)*nEvents/100.0;
        True[k] = Run.at(24)*nEvents/100.0;
        
        Appearent_Back[k] = Run.at(32)*nEvents/100.0;
        Acceptable_Back[k] = Run.at(34)*nEvents/100.0;
        True_Back[k] = Run.at(36)*nEvents/100.0;
        
        // Done!
    }
    
    // Print results:
    cout << "\n\n\nEvent Identification Results:\n\n";
    cout << "Total number of events/Background events: " << nEvents << "/" << Run.at(30)*nEvents/100.0 << "\n\n";
    cout << "      Appearent Events:      Acceptable Events:       Truely Good Events:\n";
    for (Int_t k = 0; k<9; ++k)
    {
        cout << k << "n    " << Appearent[k] << "/" << Appearent_Back[k] << "                   " << Acceptable[k] << "/" << Acceptable_Back[k] << "                      " << True[k] << "/" << True_Back[k] << "\n";
    }
    cout << "\n\nProgram is finished now.\n\n";

    // Done!
}

// Now define the main for stand-alone compilation:
#ifndef __CINT__
#include <TApplication.h>
int main(int argc, char** argv)
{
  TApplication* TheApp = new TApplication("GetEventList",&argc,argv);
  GetEventList();
  return 0;
}
#endif
      