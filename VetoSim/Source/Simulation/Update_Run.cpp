void Update_Run()
{
  // This function does nothing more then to update the run number.
  
  // Wriiten by C. A. Douma, for R3B collaboration.
  
  // ------------------------------------------------------------------------------------
  
  // We begin with connecting to the Inputs.root file:
  TFile* inputs = new TFile("../../Data/Inputs/Inputs.root","Update");
  TH1D* Doubles = (TH1D*) inputs->Get("Doubles");
  TH1I* Integers = (TH1I*) inputs->Get("Integers");
  
  // Get the run number:
  Int_t Nr_Run = Integers->GetBinContent(1);
  
  // Update the run number:
  Nr_Run = Nr_Run + 1;
  
  // Load it back into the histograms:
  Integers->SetBinContent(1,Nr_Run);
  
  // Write to the file:
  Doubles->Write("Doubles",2);
  Integers->Write("Integers",2);
  
  // Done!
}
 