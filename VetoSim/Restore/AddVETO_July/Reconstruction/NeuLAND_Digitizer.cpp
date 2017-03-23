inline void ConnectParFileToRuntimeDb(const TString par_file, FairRuntimeDb* rtdb)
{
  FairParRootFileIo* io = new FairParRootFileIo();
  io->open(par_file);
  rtdb->setFirstInput(io);
  rtdb->setOutput(io);
  rtdb->saveOutput();
}


void NeuLAND_Digitizer()																// NOTE: Change by C. A. Douma
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
  
  // Then we get what we need:
  TObjString* Output_Name_Obj = (TObjString*) inputs->Get("Output_Name");
  TObjString* Monte_Carlo_Engine_Obj = (TObjString*) inputs->Get("Monte_Carlo_Engine");
  TString Output_Name = Output_Name_Obj->GetString();
  TString Monte_Carlo_Engine = Monte_Carlo_Engine_Obj->GetString();
  Int_t Nr_Run = Integers->GetBinContent(1);
  TString st = "";
  TString Nr_Run_str = st.Itoa(Nr_Run,10);
  Double_t BeamE_d = 500.0*(Doubles->GetBinContent(202) + Doubles->GetBinContent(203)); // NOTE: MeV!
  Int_t BeamE = (Int_t) BeamE_d;
  Int_t CalibrationID = Integers->GetBinContent(241);
  Int_t Nneutrons = Integers->GetBinContent(244);
  Int_t Nprotons = Integers->GetBinContent(246);
  Int_t Nparticles = Integers->GetBinContent(219);
  Int_t G3Test = Integers->GetBinContent(249);
  if (Integers->GetBinContent(221)==1) {Nparticles = Nparticles + Integers->GetBinContent(223);}
  if (Integers->GetBinContent(225)==1) {Nparticles = Nparticles + Integers->GetBinContent(227);}
  Double_t Calibration_Eboundary = 0.7*BeamE_d*((Int_t) (Nneutrons+Nprotons));
  Double_t Calibration_Cboundary = ((Int_t) (Nneutrons+Nprotons))*(BeamE_d/50.0);
  Double_t Normal_Eboundary = 1.4*BeamE_d*((Int_t) Nparticles);
  Double_t Normal_Cboundary = ((Int_t) Nparticles)*(BeamE_d/16.0);
  Double_t ClusterSpaceRadius = Doubles->GetBinContent(242);
  Double_t ClusterTimeRadius = Doubles->GetBinContent(243);
  
  // adapt calibration boundaries if we calibrate at an ion:
  Int_t Particle_ID = Integers->GetBinContent(251);
  Bool_t Track_IsIon = (Particle_ID>1000000000);
  Int_t Abeam = -1.0;
  Int_t Zbeam = -1.0;
  if (TMath::Abs(Particle_ID)>1000000000)
  {
    Int_t PDGsmall = 0;
    if (Particle_ID>0) {PDGsmall = Particle_ID - 1000000000;}
    if (Particle_ID<0) {PDGsmall = Particle_ID + 1000000000;}
    Double_t PDG_d = (Int_t) TMath::Abs(PDGsmall);

    Double_t Z_d = PDG_d/10.0;
    Double_t R_d = PDG_d/10000.0;
    
    Double_t z_Z_d = PDG_d/10000.0;
    Double_t z_R_d = PDG_d/10000000.0;
    
    Int_t Z = (Double_t) Z_d;
    Int_t R = (Double_t) R_d;
    Z_d = Z_d - 1000.0*((Int_t) R);
    Z = (Double_t) Z_d;
    Z_d = (Int_t) Z;
    
    Int_t z_Z = (Double_t) z_Z_d;
    Int_t z_R = (Double_t) z_R_d;
    z_Z_d = z_Z_d - 1000.0*((Int_t) z_R);
    z_Z = (Double_t) z_Z_d;
    z_Z_d = (Int_t) z_Z;
    
    Abeam = Z_d;
    Zbeam = z_Z_d;
  }
  if (Track_IsIon) {Calibration_Eboundary = Calibration_Eboundary + 100.0*((Int_t) Nprotons)*TMath::Abs(1.0 - ((Int_t) Abeam)/((Int_t) Zbeam));}
  
  TString OutFile = "";
  TString ParFile = "";
  TString VisFile = "";  
  if (CalibrationID==1)																	// NOTE: Change by C. A. Douma
  {
    OutFile = "../../Data/MonteCarlo/" + Output_Name;
    ParFile = "../../Data/Parameters/" + Output_Name;
    VisFile = "../../Data/Parameters/Vis_" + Output_Name;
  }
  else
  {
    OutFile = "../../Data/MonteCarlo/" + Output_Name + "_run=" + Nr_Run_str + ".root";
    ParFile = "../../Data/Parameters/" + Output_Name + "_run=" + Nr_Run_str + ".root";
    VisFile = "../../Data/Parameters/" + Output_Name + "_run=" + Nr_Run_str + "_Vis.root";
  }
  
  UInt_t num_events = (Int_t) Integers->GetBinContent(201); // input for the digitizer!!!
  
  Int_t Neu_Exists = Integers->GetBinContent(216);
  Int_t NEB_Exists = Integers->GetBinContent(230);
  Int_t NEBv_Exists = Integers->GetBinContent(231);
  
  Bool_t Neu = ((Neu_Exists==1)||(Neu_Exists==2));
  Bool_t NEB = (!(NEB_Exists==1));
  Bool_t NEBv = (!(NEBv_Exists==1));
  Bool_t Geant3 = (Monte_Carlo_Engine=="TGeant3");
  if (G3Test==0) {Geant3 = kTRUE;}

  if (Neu&&NEB&&NEBv&&Geant3)
  {

  // ==================================================================================================================================
    
  TStopwatch timer;
  timer.Start();

  // Setting the inputs and outputs:
  TString sim_file = OutFile;
  TString par_file = ParFile;
  TString out_file = "";
  if (CalibrationID==1)																	// NOTE: Change by C. A. Douma
  {  
    out_file = "../../Data/Digitizer/" + Output_Name;
  }
  else
  {
    out_file = "../../Data/Digitizer/" + Output_Name + "_run=" + Nr_Run_str + "_NeuLAND.root";
  }

  FairRunAna* run = new FairRunAna();
  run->SetInputFile(sim_file);
  run->SetOutputFile(out_file);
  ConnectParFileToRuntimeDb(par_file, run->GetRuntimeDb());
  
  // Defining the relevant FairTasks:
  R3BNeulandDigitizer* Digitizer = new R3BNeulandDigitizer(); // Jan Mayer his NeuLAND Digitizer. Notice that this does NOT have a UseBeam-function!
  run->AddTask(Digitizer);
  
  R3BNeulandDigiMon* DigiMonitor = new R3BNeulandDigiMon(); // Jan Mayer his Monitior of the Digitizer
  run->AddTask(DigiMonitor);
  
  R3BNeuLandClusterFinder* ClusterFinder = new R3BNeuLandClusterFinder(0); // The NeuLAND Clusterfinder
  ClusterFinder->SetSpaceRadius(ClusterSpaceRadius);
  ClusterFinder->SetTimeRadius(ClusterTimeRadius);
  run->AddTask(ClusterFinder);
  
  R3BNeutronCalibr2D* Calibrator = new R3BNeutronCalibr2D(); // The Pre-calibration of the 2D cuts on the number of Neutrons. It requires the UseBeam BeamE!
  Calibrator->UseBeam(BeamE);
  if (CalibrationID==1)																	// NOTE: Change by C. A. Douma
  {  
    Calibrator->SetEnergyBoundary(Calibration_Eboundary);
    Calibrator->SetClusterBoundary(Calibration_Cboundary);
  }
  else
  {  
    Calibrator->SetEnergyBoundary(Normal_Eboundary);
    Calibrator->SetClusterBoundary(Normal_Cboundary);
  }
  run->AddTask(Calibrator);
  
  // Run the tasks:
  run->Init();
  run->Run(0, num_events);

  // End the macro:
  timer.Stop();

  cout << endl;
  cout << "Macro finished succesfully!" << endl;
  cout << "Output file writen: " << out_file << endl;
  cout << "Parameter file writen: " << par_file << endl;
  cout << "Real time: " << timer.RealTime() << "s, CPU time: " << timer.CpuTime() << "s" << endl;
  cout << endl;
    
  // ==================================================================================================================================			// NOTE: Change by C. A. Douma
  
  }
  else
  {
    cout << "### NOTE: The digitizer can only run with Geant3 as Monte Carlo Engine\n"
         << "### NOTE: and with a NeuLAND detector in the setup!\n"
         << "### NOTE: You selected a different Monte Carlo Engine,\n"
	 << "### NOTE: or you eliminated NeuLAND from the setup!\n"
         << "### NOTE: (or you added NEBULA (veto) to the setup!)\n"
	 << "### NOTE: Therefore the digitizer did not run!\n"
	 << "### NOTE: The rest of the simulation can however be used.\n"
	 << "### NOTE: The analysis will automatically take this into account.\n"
	 << "### NOTE: It will simply skip the parts that require the digitizer.\n"
	 << "### NOTE: Then the rest of the simulation and analysis can be used perfectly.\n\n";
  }
}
