void Update_zVETO()
{
  // This function does nothing more then to update the z-coordinate of the VETO geometry:
  
  // Written by C. A. Douma, for R3B collaboration.
  
  // ------------------------------------------------------------------------------------
  
  // We begin with connecting to the Inputs.root file:
  TFile* inputs = new TFile("../../Data/Inputs/Inputs.root","Update");
  TH1D* Doubles = (TH1D*) inputs->Get("Doubles");
  TH1I* Integers = (TH1I*) inputs->Get("Integers");
  
  // Get the VETO z-number:
  Double_t zVETO = Doubles->GetBinContent(123);
  
  // Get the stepsize:
  Double_t stepsize = Doubles->GetBinContent(133);
  
  // Update the VETO z-number:
  zVETO = zVETO - stepsize;
  
  // Load it back into the histograms:
  Doubles->SetBinContent(123,zVETO);
  
  // Write to the file:
  Doubles->Write("Doubles",2);
  Integers->Write("Integers",2);
  
  // Done!
}
 