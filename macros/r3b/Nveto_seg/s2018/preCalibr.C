void preCalibr(Int_t nNeutrons = 4, Int_t nEvents = 100, Int_t beamE = 600, Int_t Erel = 500)
{
  Int_t d;
  d = 15;
 
  // ----- Files ---------------------------------------------------------------
  char strDir[] = ".";
  

  char str[100];
  sprintf(str, "%1dAMeV.%1dn.%1dkeV.%1dm.root", beamE,nNeutrons, Erel, d);
  TString inFile  = TString(strDir) + "/r3bsim." + TString(str);
  TString parFile  = TString(strDir) + "/r3bpar." + TString(str);
  TString outFile  = TString(strDir) + "/r3bcalibr." + TString(str);
  // ---------------------------------------------------------------------------



  // ----- Timer ---------------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ---------------------------------------------------------------------------



  // ----- Digitization --------------------------------------------------------
  FairRunAna *fRun= new FairRunAna();
  fRun->SetInputFile(inFile);
  fRun->SetOutputFile(outFile);
  // ---------------------------------------------------------------------------



  // ----- Connect the Digitization Task ---------------------------------------
  R3Bveto_segDigitizer* veto_segDigitizer  = new R3Bveto_segDigitizer(0);
  veto_segDigitizer->UseBeam(beamE);
  veto_segDigitizer->SetTOFRange(400.);
  fRun->AddTask(veto_segDigitizer);

  R3Bveto_segDigitizerQA* veto_segDigitizerQA = new R3Bveto_segDigitizerQA();
  veto_segDigitizerQA->SetBeamEnergy(beamE);
  fRun->AddTask(veto_segDigitizerQA);
  // ---------------------------------------------------------------------------



  // ----- Find clusters -------------------------------------------------------
  R3BNVeto_segClusterFinder* veto_segClusterFinder  = new R3BNVeto_segClusterFinder(0);
  fRun->AddTask(veto_segClusterFinder);
  // ---------------------------------------------------------------------------



  // ----- Calibration of 2D cuts ----------------------------------------------
  R3BNeutronCalib2D_seg* calibr  = new R3BNeutronCalib2D_seg();
  calibr->UseBeam(beamE);
  fRun->AddTask(calibr);
  // ---------------------------------------------------------------------------



  // ----- Runtime DataBase info -----------------------------------------------
  FairRuntimeDb* rtdb = fRun->GetRuntimeDb();
  FairParRootFileIo*  parIo1 = new FairParRootFileIo();
  parIo1->open(parFile.Data());
  rtdb->setFirstInput(parIo1);
  rtdb->setOutput(parIo1);
  rtdb->saveOutput();
  // ---------------------------------------------------------------------------

  
  // ----- Intialise and run ---------------------------------------------------
  fRun->Init();
  fRun->Run(0, nEvents);
  // ---------------------------------------------------------------------------



  // ----- Finish --------------------------------------------------------------
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  cout << endl << endl;
  cout << "Macro finished succesfully." << endl;
  cout << "Output file writen:  "    << outFile << endl;
  cout << "Parameter file writen " << parFile << endl;
  cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << endl;
  cout << endl;
  cout << " Test passed" << endl;
  cout << " All ok " << endl;
  // ---------------------------------------------------------------------------
}
