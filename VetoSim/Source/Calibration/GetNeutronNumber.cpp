void GetNeutronNumber()
{
  // This function obtains the max number of neutrons & protons for the calibration
  // from the inputs.root file and writes it to a .txt-file
  // so that the shell scripts have access to this number.
  
  // Written by C. A. Douma, for R3B collaboration.
  
  // ------------------------------------------------------------------------------------
  
  // We begin with connecting to the Inputs.root file:
  TFile* inputs = new TFile("../../Data/Inputs/Inputs.root","Update");
  TH1D* Doubles = (TH1D*) inputs->Get("Doubles");
  TH1I* Integers = (TH1I*) inputs->Get("Integers");
  
  // Get the number of simulations:
  Int_t Nneutrons = Integers->GetBinContent(244);
  Int_t Nprotons = Integers->GetBinContent(246);
  
  // convert it to a C++ int:
  int n = (Int_t) Nneutrons;
  int p = (Int_t) Nprotons;
  
  // Now create .txt-files with C++ code alone:
  ofstream txt_file_n = ("../../Data/Inputs/Nneutrons.txt");
  ofstream txt_file_p = ("../../Data/Inputs/Nprotons.txt");
  
  // Now write n to the .txt file:
  txt_file_n << n;
  txt_file_p << p;
  
  // close the txt file:
  txt_file_n.close();
  txt_file_p.close();
  
  // Done!
}
 