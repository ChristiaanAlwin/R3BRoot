// Configuration macro for Geant4 VirtualMC. Modified by C. A. Douma.
void Config()
{
   // =================================================================================								// NOTE: Change by C. A. Douma
   // Connect to Inputs.root:
   TFile* inputs = new TFile("../../Data/Inputs/Inputs.root","read");
   TH1D* Doubles = (TH1D*) inputs->Get("Doubles");
   TH1I* Integers = (TH1I*) inputs->Get("Integers");
   
   Int_t G4_PhysicsList = Integers->GetBinContent(243);
   // =================================================================================
  
   // Load the User Defined Physics List
   cout << "-I- R3B Config(): Loading G4 User Defined Physics List " << endl;
   gSystem->Load("libR3BPlist");
   cout<<endl;

   // Create the Run Configuration
   R3BRunConfiguration* runConfiguration
     = new R3BRunConfiguration("geomRoot", "specialCuts+stackPopper+stepLimiter+specialControls");

   // Create the G4 VMC 
   TGeant4* geant4 = new TGeant4("TGeant4", "The Geant4 Monte Carlo",
				 runConfiguration);
   cout << "-I- R3B Config(): Geant4 with R3B dedicated Physics list " << endl;
   cout << "-I- R3B Config(): has been created..." << endl;

   // create the Specific stack
   R3BStack *stack = new R3BStack(1000); 
   stack->SetDebug(kFALSE);
   stack->StoreSecondaries(kTRUE);
   stack->SetMinPoints(0);
   geant4->SetStack(stack);

   if(FairRunSim::Instance()->IsExtDecayer()){
      TVirtualMCDecayer* decayer = TPythia6Decayer::Instance();
      geant4->SetExternalDecayer(decayer);
   }
  
/// Customise Geant4 setting
/// (verbose level, global range cut, ..)

   TString configm(gSystem->Getenv("VMCWORKDIR"));
   configm1 = configm + "/gconfig/g4r3bconfig.in";
   if (G4_PhysicsList==1) {configm1 = "./g4r3bconfig_Standard.in";}										// NOTE: Change by C. A. Douma
   if (G4_PhysicsList==2) {configm1 = "./g4r3bconfig_HeavyIons.in";}										// NOTE: Change by C. A. Douma
   if (G4_PhysicsList==3) {configm1 = "./g4r3bconfig_KenMiki.in";}										// NOTE: Change by C. A. Douma
   cout << " -I g4Config() using g4conf  macro: " << configm1 << endl;

   //set geant4 specific stuff
  geant4->SetMaxNStep(10000);  // default is 30000
  geant4->ProcessGeantMacro(configm1.Data());

}
