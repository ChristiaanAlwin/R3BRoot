#include <string>
using namespace std;

// NOTE: Function to read the calibration file:
void R3BNeuLANDTracker::ReadCalibrationFile(TString const FileName)
{
  // Declare the pointer to the file:
  ifstream ThisFile;

  // Open the calibration file and link it to the pointer:
  ThisFile.open(FileName);
      
  // Check if opened succesfully
  if (ThisFile.is_open()) 
  {
    // Now we must read the calibration file. Begin with determining the amount of lines:
    Int_t NLines = 0;
    string ThisLine;
    while (getline(ThisFile,ThisLine)) {NLines = NLines + 1;}
  
    // Then determine the number of cuts:
    fNumberOfCuts = NLines - 1;
  
    // Then close the file:
    ThisFile.close();
      
    // And re-open, now to read:
    ThisFile.open(FileName);
  
    // Read out the kappa:
    ThisFile >> fKappa;
  
    // And then generate the array for the number of cuts:
    fCuts = new Double_t [fNumberOfCuts];
  
    // And loop to read the cuts:
    for (Int_t kE = 0; kE<fNumberOfCuts; ++kE) 
    {
      ThisFile >> fCuts[kE];
    }
  
    // Then close the file again:
    ThisFile.close();
  
    // Then print the output:
    cout << "### NOTE: Calibration file was read out succesfully.\n"
         << "          Number of Cuts = " << fNumberOfCuts << "\n"
         << "          Kappa slope = " << fKappa << "\n";
       
    for (Int_t kE = 0; kE<fNumberOfCuts; ++kE) 
    {
       cout << "          Enegy cut " << kE+1 << " = " << fCuts[kE] << "\n";
    } 
  
    // And then we are done. This time, no delete with the new, since the array
    // has to stay available during the rest of the algorithm.

    // Register the succes:
    fCalibration = kTRUE;
    fApplyCuts = kTRUE;
  }
  else
  {
      cout << "### ERROR: You specified the wrong calibration file!\n";
  }  
  
  // Done!
}