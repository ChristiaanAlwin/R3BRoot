void Update_VETO_Paddles()
{
  // This function does nothing more then to update the z-coordinate of the VETO geometry:
  
  // Written by C. A. Douma, for R3B collaboration.
  
  // ------------------------------------------------------------------------------------
  
  // We begin with connecting to the Inputs.root file:
  TFile* inputs = new TFile("../../Data/Inputs/Inputs.root","Update");
  TH1D* Doubles = (TH1D*) inputs->Get("Doubles");
  TH1I* Integers = (TH1I*) inputs->Get("Integers");
  
  // Get the VETO number of paddles and the bar width:
  Double_t VETO_barWidth = Doubles->GetBinContent(128);
  Int_t nPaddles_1plane = Integers->GetBinContent(4);
  
  // now cut one bar in half:
  VETO_barWidth = VETO_barWidth*0.5;
  nPaddles_1plane = nPaddles_1plane*2;
  
  // Load it back into the histograms:
  Doubles->SetBinContent(128,VETO_barWidth);
  Integers->SetBinContent(4,nPaddles_1plane);
  
  // Write to the file:
  Doubles->Write("Doubles",2);
  Integers->Write("Integers",2);
  
  // Done!
}
 