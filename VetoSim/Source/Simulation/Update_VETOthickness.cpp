void Update_VETOthickness()
{
  // This function does nothing more then to update the scintillator bar thickness the VETO geometry:
  
  // Written by C. A. Douma, for R3B collaboration.
  
  // ------------------------------------------------------------------------------------
  
  // We begin with connecting to the Inputs.root file:
  TFile* inputs = new TFile("../../Data/Inputs/Inputs.root","Update");
  TH1D* Doubles = (TH1D*) inputs->Get("Doubles");
  TH1I* Integers = (TH1I*) inputs->Get("Integers");
  
  // Get the VETO bar thickness:
  Double_t VETO_bar_thickness = Doubles->GetBinContent(129);
  
  // Get the stepsize:
  Double_t stepsize = Doubles->GetBinContent(134);
  
  // Update the VETO z-number:
  VETO_bar_thickness = VETO_bar_thickness + stepsize;
  
  // Load it back into the histograms:
  Doubles->SetBinContent(129,VETO_bar_thickness);
  
  // Write to the file:
  Doubles->Write("Doubles",2);
  Integers->Write("Integers",2);
  
  // Done!
}
 