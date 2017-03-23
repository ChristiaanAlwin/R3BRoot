void eventDisplay()
{
  // ==================================================================================================================================
  // This section is added by C. A. Douma to obtain the names of the
  // visualization files from inputs.root
  // The output is on the screen.
  
  // This function has therefore no inputs or outputs.
  
  // Adjustments are made by C. A. Douma.
  
  // We begin with connecting to the Inputs.root file:
  TFile* inputs = new TFile("../../Data/Inputs/Inputs.root","read");
  TH1D* Doubles = (TH1D*) inputs->Get("Doubles");
  TH1I* Integers = (TH1I*) inputs->Get("Integers");
  
  // Then we get what we need:
  TObjString* Output_Name_Obj = (TObjString*) inputs->Get("Output_Name");
  TString Output_Name = Output_Name_Obj->GetString();
  Int_t Nr_Run = 0; // NOTE: Next line is replaced every time with sed!
Nr_Run = 1;
  TString st = "";
  TString Nr_Run_str = st.Itoa(Nr_Run,10);
  
  Int_t CalibrationID = Integers->GetBinContent(241);
  TString OutFile = "";
  TString ParFile = "";
  TString VisFile = "";  
  if (CalibrationID==1)
  {
    OutFile = "../../Data/MonteCarlo/" + Output_Name;
    ParFile = "../../Data/Parameters/" + Output_Name;
    VisFile = "../../Data/Parameters/" + "Vis_" + Output_Name;
  }
  else
  {
    OutFile = "../../Data/MonteCarlo/" + Output_Name + "_run=" + Nr_Run_str + ".root";
    ParFile = "../../Data/Parameters/" + Output_Name + "_run=" + Nr_Run_str + ".root";
    VisFile = "../../Data/Parameters/" + Output_Name + "_run=" + Nr_Run_str + "_Vis.root";
  }
  
  /*
  OutFile = "../../Data/MonteCarlo/CalibrationFile_2p_1n.root";
  ParFile = "../../Data/Parameters/CalibrationFile_2p_1n.root";
  VisFile = "../../Data/Parameters/CalibrationFile_2p_1n_Vis.root";
  */

  // ==================================================================================================================================
  
  FairRunAna *fRun= new FairRunAna();
  
  FairRuntimeDb* rtdb = fRun->GetRuntimeDb();
  FairParRootFileIo*  parIo1 = new FairParRootFileIo();
  parIo1->open(ParFile);
  rtdb->setFirstInput(parIo1);
  rtdb->print();
  
  fRun->SetInputFile(OutFile);
  fRun->SetOutputFile(VisFile);
  
  FairEventManager *fMan= new FairEventManager();
  FairMCTracks *Track =  new FairMCTracks ("Monte-Carlo Tracks");
  FairMCPointDraw *LandPoints =   new FairMCPointDraw ("LandPoint",kOrange,  kFullSquare);
  FairMCPointDraw *veto_segPoints =   new FairMCPointDraw ("veto_segPoint",kRed,  kFullSquare);
  FairMCPointDraw *STaRTraPoints =   new FairMCPointDraw ("STaRTraPoint",kGreen,  kFullSquare);
  FairMCPointDraw *CaloCrystalHitSims =   new FairMCPointDraw ("CaloCrystalHitSim",kBlue,  kFullSquare);
  FairMCPointDraw *DCHPoints =   new FairMCPointDraw ("DCHPoint",kBlack,  kFullSquare);
  FairMCPointDraw *TOFPoints =   new FairMCPointDraw ("TOFPoint",kOrange,  kFullSquare);
  FairMCPointDraw *mTOFPoints =   new FairMCPointDraw ("mTOFPoint",kOrange,  kFullSquare);
  FairMCPointDraw *MFIPoints =   new FairMCPointDraw ("MFIPoint",kOrange,  kFullSquare);
  FairMCPointDraw *PSPPoints =   new FairMCPointDraw ("PSPPoint",kYellow,  kFullSquare);
  FairMCPointDraw *GFIPoints =   new FairMCPointDraw ("GFIPoint",kYellow,  kFullSquare);
  
  fMan->AddTask(Track);
  
  fMan->AddTask(LandPoints);
  fMan->AddTask(veto_segPoints);
  fMan->AddTask(STaRTraPoints);
  fMan->AddTask(CaloCrystalHitSims);
  fMan->AddTask(DCHPoints);
  fMan->AddTask(TOFPoints);
  fMan->AddTask(mTOFPoints);
  fMan->AddTask(MFIPoints);
  fMan->AddTask(PSPPoints);
  fMan->AddTask(GFIPoints);
  
  fMan->Init();
}
