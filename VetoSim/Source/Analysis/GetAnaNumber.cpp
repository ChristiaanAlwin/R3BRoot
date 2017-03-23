void GetAnaNumber()
{
  // This function obtains the number of analysis repettion from
  // the inputs.root file and writes it to a .txt-file
  // so that the shell scripts have access to this number.
  
  // Written by C. A. Douma, for R3B collaboration.
  
  // ------------------------------------------------------------------------------------
  
  // We begin with connecting to the Inputs.root file:
  TFile* inputs = new TFile("../../Data/Inputs/Inputs.root","Update");
  TH1D* Doubles = (TH1D*) inputs->Get("Doubles");
  TH1I* Integers = (TH1I*) inputs->Get("Integers");
  
  // Get the number of simulations:
  Int_t Nana = Integers->GetBinContent(311);
  
  // convert it to a C++ int:
  int n = (Int_t) Nana;
  
  // Now create a .txt file with C++ code alone:
  ofstream txt_file = ("../../Data/Inputs/Nana.txt");
  
  // Now write n to the .txt file:
  txt_file << n;
  
  // close the txt file:
  txt_file.close();
  
  // Done!
}
 