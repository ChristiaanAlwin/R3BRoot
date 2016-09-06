inline void ConnectParFileToRuntimeDb(const TString par_file, FairRuntimeDb* rtdb)
{
  FairParRootFileIo* io = new FairParRootFileIo();
  io->open(par_file);
  rtdb->setFirstInput(io);
  rtdb->setOutput(io);
  rtdb->saveOutput();
}

void Macro()
{
    // This is the reconstruction steering macro of this folder:
    
    // Written by C. A. Douma.
    
    // =========================================================
    
    // Set input files & output files:
    Int_t nEvents = 300;
    TString InPutFile = "./r3bsim.root";
    TString ParFile = "./r3bpar.root";
    TString OutPutFile = "./NeuLAND.root";
    TString CalFile = "./File.txt";
    
    // Ignite the time:
    TStopwatch timer;
    timer.Start();

    // Setting the FAIR input/output stuff:
    FairRunAna* run = new FairRunAna();
    run->SetInputFile(InPutFile);
    run->SetOutputFile(OutPutFile);
    ConnectParFileToRuntimeDb(ParFile, run->GetRuntimeDb());
    
    // Add Jan Mayer his new NeuLAND Digitizer to get NeuLAND detector outputs:
    R3BNeulandDigitizer* Digitizer = new R3BNeulandDigitizer(); 
    run->AddTask(Digitizer);
    
    // Use a Copy of this digitizer to generate VETO outputs:
    R3BNveto_segDigitizer* VetoDigitizer = new R3BNveto_segDigitizer(); 
    run->AddTask(VetoDigitizer);
    
    // Add Christiaan Douma his ClusterFinder:
    R3BNeuLANDClusterFinder* ClusterFinder = new R3BNeuLANDClusterFinder();
    ClusterFinder->SetSpaceRadius(7.5);       // Sets the cluster space radius to 7.5 cm.
    ClusterFinder->SetTimeRadius(1.0);        // Sets the cluster time radius to 1.0 ns.
    ClusterFinder->SetTopologyBoundary(5.0);  // Sets the topological boundary equal to 5.0 cm.
    ClusterFinder->SetHistograms(1500.0,50);  // Sets the boundaries of the Calibration histogram to 1500.0 MeV & 50 clusters.
    run->AddTask(ClusterFinder);
    // NOTE: The clusterfinder only needs the digitizer output, not the Monte Carlo simulation.
    //       Hence the clusterfinder can also run on real experimental data instead.
    
    // Christiaan Douma his NeuLANDTracker:
    R3BNeuLANDTracker* Tracker = new R3BNeuLANDTracker();
    Tracker->ReadCalibrationFile(CalFile);            // Passes the energy cuts & kappa to the tracker. Is pointless to use when SetNeutronNumber has been called.
    Tracker->SetBeamBeta(0.79);                       // Tells the tracker that the relativistic beta of the beam equals 0.79 (corresponds to 600 MeV neutrons)
    Tracker->SetBeamEnergy(600.0);                    // Tells the tracker that the beam energy equals 600 AMeV
    Tracker->SetBeamStartingPosition(0.0,0.0,0.0);    // Tells the tracker that the beam is fired from the origin (units are cm).
    Tracker->SetTargetPosition(0.0,0.0,0.0);          // Tells the tracker that the target is located at the origin (units are cm).
    Tracker->SetNSinglePlanes(60);                    // Tells the tracker that NeuLAND consists of 60 single planes.
    // Tracker->SetNeutronNumber(2);                     // Forces the tracker to reconstruct exactly 2 vertices each event.
    Tracker->SetEliminationLevel(1);                  // Tells the tracker up to how far in the cascade clusters of charged particles should be filtered.
                                                      // 0 means that for each hit in the VETO, only the cluster closest to it will be eliminated as coming from
                                                      // a charged particle. 1 means that also the clusters will be eliminated that come from elastic scattering 
                                                      // with the clusters directly behind the VETO hit. 2 means that clusters that are elastic scattering from
                                                      // elastic scattering eliminated clusters will also be eliminated. And so on...
    run->AddTask(Tracker);
    // NOTE: The Tracker only requires the digitizer output & the output of the clusterfinder.
    //       Again, Monte Carlo data & VETO digitizer outputs are not required. If the VETO digitizer
    //       output is not available, the Tracker will simply make a reconstruction without filtering
    //       out the charged particle tracks. If Monte Carlo data is not available, a comparison between
    //       Monte Carlo input & Reconstruction output cannot be made, but the reconstruction is still done.
    //       Hence the Tracker can perfectly run on real experimental data instead (also with real VETO data).
    
    // Then let the tasks run:
    run->Init();
    run->Run(0, nEvents);
    
    // And finally, end the macro:
    timer.Stop();
    cout << "\n\nMacro finished Succesfully!\n\n";
}