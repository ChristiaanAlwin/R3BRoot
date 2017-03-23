void GeometryDisplay()
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
  TString Output_Name = "Geometry_Visualization";
  TString OutFile = "../../Data/MonteCarlo/" + Output_Name + ".root";
  TString ParFile = "../../Data/Parameters/" + Output_Name + ".root";
  TString VisFile = "../../Data/Parameters/" + Output_Name + "_Vis.root";

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
  
  fMan->Init();
}
