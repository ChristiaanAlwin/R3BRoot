inline void ConnectParFileToRuntimeDb(const TString parFile, FairRuntimeDb* rtdb)
{
    FairParRootFileIo* io = new FairParRootFileIo();
    io->open(parFile);
    rtdb->setFirstInput(io);
    rtdb->setOutput(io);
    rtdb->saveOutput();
}

void Step2_Digitizin_segClustering(const TString simFile)
{
    TStopwatch timer;
    timer.Start();

    const TString parFile = (TString(simFile).ReplaceAll(".sim.", ".par."));
    const TString outFile = (TString(simFile).ReplaceAll(".sim.", ".digi."));

    FairRunAna* run = new FairRunAna();
    // FairLogger::GetLogger()->SetLogScreenLevel("DEBUG");
    run->SetInputFile(simFile);
    run->SetOutputFile(outFile);
    ConnectParFileToRuntimeDb(parFile, run->GetRuntimeDb());

    run->AddTask(new R3BNveto_segDigitizer());
    run->AddTask(new R3BNveto_segClusterFinder());

    // Optional
    // run->AddTask(new R3BNveto_segMCMon());
    // run->AddTask(new R3BNveto_segDigiMon());
    // run->AddTask(new R3BNveto_segClusterMon());

    run->Init();
    run->Run(0, 0);

    timer.Stop();

    cout << "Macro finished succesfully!" << endl;
    cout << "Output file writen: " << outFile << endl;
    cout << "Parameter file writen: " << parFile << endl;
    cout << "Real time: " << timer.RealTime() << "s, CPU time: " << timer.CpuTime() << "s" << endl;
}
