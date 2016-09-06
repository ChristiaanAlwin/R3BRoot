inline void ConnectParFileToRuntimeDb(const TString par_file, FairRuntimeDb* rtdb)
{
  FairParRootFileIo* io = new FairParRootFileIo();
  io->open(par_file);
  rtdb->setFirstInput(io);
  rtdb->setOutput(io);
  rtdb->saveOutput();
}


void Other_Digitizers()
{
  // ==================================================================================================================================
  // This section is added by C. A. Douma to obtain the names of the
  // input and output files from inputs.root instead of having to set them manually.
  
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
  Int_t CalibrationID = Integers->GetBinContent(241);
  Int_t G3Test = Integers->GetBinContent(249);
  TString st = "";
  TString Nr_Run_str = st.Itoa(Nr_Run,10);
  Int_t STaRTrack_Exists = Integers->GetBinContent(207);
  Int_t CALIFA_Exists = Integers->GetBinContent(210);
  Bool_t Geant3 = (Monte_Carlo_Engine=="TGeant3");
  
  // Define the input files and output files:
  TString OutFile = "";
  TString ParFile = "";
  TString VisFile = "";  
  if (CalibrationID==1)
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
  
  if (G3Test==0) {Geant3 = kTRUE;}

  if (Geant3)
  {
      
  // Now that we have all the inputs, we can go:
    
  TStopwatch timer;
  timer.Start();

  // Setting the inputs and outputs files:
  TString sim_file = OutFile;
  TString par_file = ParFile;
  TString out_file = "";
  if (CalibrationID==1)
  {  
    out_file = "../../Data/Digitizer/" + Output_Name;
  }
  else
  {
    out_file = "../../Data/Digitizer/" + Output_Name + "_run=" + Nr_Run_str + "_Others.root";
  }

  // Define Fair Runtime:
  FairRunAna* run = new FairRunAna();
  run->SetInputFile(sim_file);
  run->SetOutputFile(out_file);
  ConnectParFileToRuntimeDb(par_file, run->GetRuntimeDb());
  
  // ==========================================================================================
  // NOTE: The section that matters: Defining the relevant FairTasks.
  
  // Define CALIFA Hit Finder:
  if (CALIFA_Exists==1)
  {
  R3BCaloHitFinder* caloHF = new R3BCaloHitFinder();
  caloHF->SelectGeometryVersion(17);   
  caloHF->SetClusteringAlgorithm(1,0);
  caloHF->SetDetectionThreshold(0.000050);  //50 KeV
  caloHF->SetExperimentalResolution(6.);    //6% at 1 MeV
  caloHF->SetComponentResolution(.25);      //sigma = 0.25 MeV
  caloHF->SetPhoswichResolution(3.,5.);     //percent @ 1 MeV for LaBr and LaCl 
  caloHF->SetAngularWindow(3.2,3.2);        //[0.25 around 14.3 degrees, 3.2 for the complete calorimeter]
  run->AddTask(caloHF);
  }
  
  if (STaRTrack_Exists==1)
  {
  // Define STaRTrack Hit Finder:
  R3BSTaRTraHitFinder* startraHF = new R3BSTaRTraHitFinder(); 
  startraHF->SetDetectionThreshold(0.000050);//50 KeV
  startraHF->SetExperimentalResolution(0.00005);  // 50 keV
  run->AddTask(startraHF);
  
  // Define STaRTrack Digitizer (Raw DAQ output-like):
  R3BSTaRTraDigit* startraDigi = new R3BSTaRTraDigit();         
  startraDigi->SetDetectionThreshold(0.000000);  // 0 KeV
  startraDigi->SetExperimentalResolution(0.0000);  // 0 keV
  run->AddTask(startraDigi);
  }
  
  // ==========================================================================================
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
    
  // ==================================================================================================================================	
  
  }
  else
  {
    cout << "### NOTE: These digitizers can only run with the right Monte Carlo Engine\n"
         << "### NOTE: and with the right detectors in the setup. Hence re-check these\n"
         << "### NOTE: settings and then try to run again.\n\n\n";
  }
}
