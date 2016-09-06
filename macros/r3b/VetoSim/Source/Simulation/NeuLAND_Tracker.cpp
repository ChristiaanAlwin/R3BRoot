inline void ConnectParFileToRuntimeDb(const TString par_file, FairRuntimeDb* rtdb)
{
  FairParRootFileIo* io = new FairParRootFileIo();
  io->open(par_file);
  rtdb->setFirstInput(io);
  rtdb->setOutput(io);
  rtdb->saveOutput();
}


void NeuLAND_Tracker()																// NOTE: Change by C. A. Douma
{
  // ==================================================================================================================================
  // This section is added by C. A. Douma to obtain the names of the
  // visualization files from inputs.root
  // The output is the digitizer file.
  
  // This function has therefore no inputs or outputs.
  
  // Adjustments are made by C. A. Douma.
  
  // We begin with connecting to the Inputs.root file:
  TFile* inputs = new TFile("../../Data/Inputs/Inputs.root","read");
  TH1D* Doubles = (TH1D*) inputs->Get("Doubles");
  TH1I* Integers = (TH1I*) inputs->Get("Integers");
  
  // Then we get the inputs that we need:
  TObjString* Output_Name_Obj = (TObjString*) inputs->Get("Output_Name");
  TObjString* Monte_Carlo_Engine_Obj = (TObjString*) inputs->Get("Monte_Carlo_Engine");
  TString Output_Name = Output_Name_Obj->GetString();
  TString Monte_Carlo_Engine = Monte_Carlo_Engine_Obj->GetString();
  
  Int_t Nr_Run = Integers->GetBinContent(1);
  TString st = "";
  TString Nr_Run_str = st.Itoa(Nr_Run,10);
  
  Double_t BeamX = Doubles->GetBinContent(208);
  Double_t BeamY = Doubles->GetBinContent(209);
  Double_t BeamZ = Doubles->GetBinContent(210);
  
  Double_t TargetX = Doubles->GetBinContent(331);
  Double_t TargetY = Doubles->GetBinContent(332);
  Double_t TargetZ = Doubles->GetBinContent(333);
  
  Int_t NeuLANDClass = Integers->GetBinContent(216);
  Int_t Nplanes = Integers->GetBinContent(2);
  Int_t VetoCondition = Integers->GetBinContent(309);
  
  Int_t Nparticles = Integers->GetBinContent(219);
  if (Integers->GetBinContent(221)==1) {Nparticles = Nparticles + Integers->GetBinContent(223);}
  if (Integers->GetBinContent(225)==1) {Nparticles = Nparticles + Integers->GetBinContent(227);}
  
  UInt_t num_events = (Int_t) Integers->GetBinContent(201);
  
  Int_t Run_Tracker = Integers->GetBinContent(242);
  
  Double_t BeamE = 500.0*(Doubles->GetBinContent(202) + Doubles->GetBinContent(203)); // NOTE: MeV!
  Double_t NeutronMass = 939.565413358; // MeV.
  Double_t Etotal = BeamE+NeutronMass;
  Double_t gamma = Etotal/NeutronMass;
  Double_t BeamBeta = TMath::Sqrt(1.0 - (1.0/(gamma*gamma)));
  cout << "### NOTE: Beam Information: || Ekin = " << BeamE << " [MeV] | Relativistic Beta = " << BeamBeta << "\n";
  
  // Then we can define the filenames that we need:
  TString SimFile = "../../Data/MonteCarlo/" + Output_Name + "_run=" + Nr_Run_str + ".root";
  TString ParFile = "../../Data/Parameters/" + Output_Name + "_run=" + Nr_Run_str + ".root";
  TString VisFile = "../../Data/Parameters/" + Output_Name + "_run=" + Nr_Run_str + "_Vis.root";
  TString DigiFile = "../../Data/Digitizer/" + Output_Name + "_run=" + Nr_Run_str + "_NeuLAND.root";
  TString DigiVetoFile = "../../Data/Digitizer/" + Output_Name + "_run=" + Nr_Run_str + "_VETO.root";
  TString CaliFile = "../../UI/Calibration/Energy_Cuts_0p.txt";
  
  // Then we have to establish the conditions to safely run this macro. This basically means that
  // all the above files exist. Hence:
  FileStat_t stom [6];
  Int_t test0 = gSystem->GetPathInfo(SimFile,stom[0]);
  Int_t test1 = gSystem->GetPathInfo(ParFile,stom[1]);
  Int_t test2 = gSystem->GetPathInfo(VisFile,stom[2]);
  Int_t test3 = gSystem->GetPathInfo(DigiFile,stom[3]);
  Int_t test4 = gSystem->GetPathInfo(CaliFile,stom[4]);
  Int_t test5 = gSystem->GetPathInfo(DigiVetoFile,stom[5]);
  Int_t TheTest = test0+test1+test3+test4;
  
  // Then now we can run if TheTest equals zero (all files exist) and if we actually should run:
  if ((Run_Tracker==1)||(Run_Tracker==2)||(Run_Tracker==3)) {
  if (TheTest==0)
  {

  // ==================================================================================================================================
    
  TStopwatch timer;
  timer.Start();

  // Setting the inputs and outputs:
  TString TrackerFile = "../../Data/Digitizer/" + Output_Name + "_run=" + Nr_Run_str + "_NeutronTracker.root";
  FairRunAna* run = new FairRunAna();
  ConnectParFileToRuntimeDb(ParFile, run->GetRuntimeDb());
  
  if (NeuLANDClass==2)
  {
    run->SetInputFile(DigiFile);
    run->SetOutputFile(TrackerFile);
    run->AddFriend(DigiVetoFile);
    run->AddFriend(SimFile);
      
    R3BNeuLANDTracker* Tracker = new R3BNeuLANDTracker();
    Tracker->ReadCalibrationFile(CaliFile);                         // Passes the energy cuts & kappa to the tracker. Is pointless to use when SetNeutronNumber has been called.
    Tracker->SetBeamBeta(BeamBeta);                                 // Tells the tracker that the relativistic beta of the beam equals 0.79 (corresponds to 600 MeV neutrons)
    Tracker->SetBeamEnergy(BeamE);                                  // Tells the tracker that the beam energy equals 600 AMeV
    Tracker->SetBeamStartingPosition(BeamX,BeamY,BeamZ);            // Tells the tracker that the beam is fired from the origin (units are cm).
    Tracker->SetTargetPosition(TargetX,TargetY,TargetZ);            // Tells the tracker that the target is located at the origin (units are cm).
    Tracker->SetNSinglePlanes(Nplanes);                             // Tells the tracker that NeuLAND consists of 60 single planes.
    if (Run_Tracker==3) {Tracker->SetNeutronNumber(Nparticles); cout << "\n\n #################### FERFECT CALIBRATION!!! ###################\n\n";}
    Tracker->SetEliminationLevel(VetoCondition);                                // Tells the tracker up to how far in the cascade clusters of charged particles should be filtered.
    run->AddTask(Tracker);
  }
  else
  {   
  run->SetInputFile(SimFile);
  run->AddFriend(DigiFile); // NOTE: The tracker needs the digitizer!
  run->AddFriend(DigiVetoFile); // NOTE: The modified tracker needs the VETO digitizer too!
  run->SetOutputFile(TrackerFile);

  // Then we set up the Neutron Tracker:
  R3BNeutronTracker2D* Tracker = new R3BNeutronTracker2D();
  Tracker->UseBeam(BeamE,BeamBeta);
  if (Run_Tracker==3) {Tracker->Disable2DEventCut(Nparticles); cout << "\n\n #################### FERFECT CALIBRATION!!! ###################\n\n";}
  else {Tracker->ReadCalibrFiles();}
  // else {Tracker->ReadCalibrFile(CaliFile.Data());} // NOTE: Need here the .Data() behind the TString. ==> Original tracker!
  Tracker->SetTargetPosition(TargetX,TargetY,TargetZ);
  Tracker->SetBeamPosition(BeamX,BeamY,BeamZ);
  Tracker->EnablePrimParts();
  if (Monte_Carlo_Engine=="TGeant34")
  {
      Tracker->SetBeamPosition(TargetX,TargetY,TargetZ); // NOTE: Because we then shoot the secondaries from the target!
      Tracker->DisablePrimParts(); // to prevent seg-faults. we don't need it anyway...
  }
  run->AddTask(Tracker);
  }
  
  // Run the tasks:
  run->Init();
  run->Run(0, num_events);

  // End the macro:
  timer.Stop();

  cout << endl;
  cout << "Macro finished succesfully!" << endl;
  cout << "Output file writen: " << TrackerFile << endl;
  cout << "Parameter file writen: " << ParFile << endl;
  cout << "Real time: " << timer.RealTime() << "s, CPU time: " << timer.CpuTime() << "s" << endl;
  cout << endl;
    
  // ==================================================================================================================================			// NOTE: Change by C. A. Douma
  
  }
  else
  {
    cout << "### NOTE: The Tracker can only run when ALL 4 input files exist!\n"
         << "### NOTE: These files are the Monte Carlo simulation file, the Parameter file,\n"
	 << "### NOTE: the Digitizer file AND the Calibration file.\n"
	 << "### NOTE: If any of these files misses, it cannot run. In particular\n"
	 << "### NOTE: this means that the tracker can only run if the Digitizer could\n"
	 << "### NOTE: run and if the user has performed a neutron calibration run.\n"
	 << "### NOTE: So do this first and then re-run!\n\n\n";
    
	 cout << "Input files detection:\n\nMonte Carlo Simulation File: ";
	 if (test0==0) {cout << "found.\n";} else {cout << "NOT FOUND!\n";}
	 cout << "Parameter File: ";
	 if (test1==0) {cout << "found.\n";} else {cout << "NOT FOUND!\n";}
	 cout << "Digitizer Output File: ";
	 if (test3==0) {cout << "found.\n";} else {cout << "NOT FOUND!\n";}
	 cout << "Neutron Calibration .txt-file: ";
	 if (test4==0) {cout << "found.\n";} else {cout << "NOT FOUND!\n";}
	 cout << "\n\n\n\n";
  }
  }
  else
  {
    cout << "You chose not to use the Neutron Tracker. It will not run.\n\n\n";
  }
}
