//  -------------------------------------------------------------------------
//
//   ----- General Macro for R3B simulation
//
//         Author: Denis Bertini <D.Bertini@gsi.de>
//
//         Last Update: 30/06/12 (marc Labiche)
//
//         Comments:
//               - 30/06/12 Adding Vaccum Vessel and cooling for new Si tracker
//               - 17/08/09 Adding R3BModule R3BDetector
//                          for global positionning
//               - 12/08/09 Adding R3B Special Physics List
//               - 06/08/09 Adding R3B specific Event Generator
//
//
//  -------------------------------------------------------------------------
//
//   Usage inside ROOT interpreter:
//        1) root -l
//        2)[root] .L r3ball.C
//
//        3)[root] r3ball( nevt,
//                         fDetList,     // List of Detectors
//                         TargetType,    // "LeadTarget"
//                         Visualization, // kFalse or kTRUE
//                         fMC ,        // "TGeant3" or "TGeant4"
//                         fGenerator   // Generator type
//
//  -------------------------------------------------------------------------



void VETOedBars_ProcessGeometry()															// NOTE: Changed by C. A. Douma!
{
  // ==================================================================================================================================
  // This section is added by C. A. Douma to obtain the inputs from Inputs.root
  // which would be passed as function input arguments otherwise. The rest of the 
  // code will be mostly unchanged code from r3ball.C
  
  // This function has therefore no inputs or outputs.
  
  // Adjustments are made by C. A. Douma.
  
  // ------------------------------------------------------------------------------------
  // We begin with connecting to the Inputs.root file:
  TFile* inputs = new TFile("../../Data/Inputs/Inputs.root","read");
  TH1D* Doubles = (TH1D*) inputs->Get("Doubles");
  TH1I* Integers = (TH1I*) inputs->Get("Integers");
  
  // Beam properties:
  Int_t Particle_ID = 211;
  Int_t Num_Prim_Particle = 1;
  Double_t Particle_Mass = 0.13957018;
  Double_t Emin = 0.0;
  Double_t Emax = 1.0;
  Double_t ThetaMin = 0.0;
  Double_t ThetaMax = 0.001;
  Double_t PhiMin = 0.0;
  Double_t PhiMax = 360.0;
  Double_t X_Beam = 0.0;
  Double_t Y_Beam = 0.0;
  Double_t Z_Beam = -500.0;
  
  // Correct for relativity: We specify kinetic energy, not total momentum!
  Emin = Emin + Particle_Mass;
  Emax = Emax + Particle_Mass;
  Emin = TMath::Sqrt(Emin*Emin - Particle_Mass*Particle_Mass);
  Emax = TMath::Sqrt(Emax*Emax - Particle_Mass*Particle_Mass);

  // Target properties:
  TObjString* Target_Material_Obj = (TObjString*) inputs->Get("Target_Material");
  TString Target_Material = Target_Material_Obj->GetString();
  
  // Number of Repetions: Runs, Events, etc.
  Int_t Nr_Events = 1;
  Int_t Nr_Run = 0;
  TString st = "";
  TString Nr_Run_str = st.Itoa(Nr_Run,10);
  
  // Software settings:
    TString Monte_Carlo_Engine = "TGeant3";
  TString Event_Generator = "box";
  TString Output_Name = "VETO_Condition_Visualization";

  Int_t Visualization_Test = 1;
  Int_t Secondary_Interaction = 1;
  Int_t Magnetic_Field = 1;
  Double_t Magnetic_Field_Current = 2000.0;
  
  // Used detectors:
  Int_t Target_Exists = Integers->GetBinContent(206);
  Int_t StaRTrack_Exists = Integers->GetBinContent(207); 
  Int_t PSP_Exists = Integers->GetBinContent(208); 
  Int_t VacVessel_Exists = Integers->GetBinContent(209);
  Int_t CALIFA_Exists = Integers->GetBinContent(210);
  Int_t mTOF_Exists = Integers->GetBinContent(211);
  Int_t TOF_Exists = Integers->GetBinContent(212);
  Int_t MFI_Exists = Integers->GetBinContent(213);
  Int_t DCH_Exists = Integers->GetBinContent(214);
  Int_t GLAD_Exists = Integers->GetBinContent(215);
  Int_t NeuLAND_Exists = Integers->GetBinContent(216);
  Int_t NEBULA_Exists = Integers->GetBinContent(230);
  Int_t VETO_Exists = Integers->GetBinContent(217);
  Int_t NEB_VETO_Exists = Integers->GetBinContent(231);
  
  // -------------------------------------------------------------------------------------
  // Afterwe have the inputs, we define the r3bsim-inputs to actually steer the macro:
  TString OutFile = "../../Data/MonteCarlo/" + Output_Name + ".root";
  TString ParFile = "../../Data/Parameters/" + Output_Name + ".root";
  TString InFile = "evt_gen.dat"; // NOTE: we do not control this! this is a filename in R3Broot that controls the R3B Event Generator. Hence we leave it alone!
  TString fMC = Monte_Carlo_Engine;
  TString fGenerator = Event_Generator;
  Bool_t fUserPList = (Secondary_Interaction==1);
  TString Target = Target_Material;
  Int_t nEvents = Nr_Events;
  Bool_t fVis = (Visualization_Test==1);
  Bool_t fR3BMagnet = (Magnetic_Field==1);
  Double_t fMeasCurrent = Magnetic_Field_Current;
  
  TMap detGeo;
  if (Target_Exists==1) {detGeo.Add(new TObjString("TARGET"),        new TObjString("Target.geo.root"));}
  if (GLAD_Exists==1) {detGeo.Add(new TObjString("GLAD"),          new TObjString("GLAD.geo.root"));}
  if (CALIFA_Exists==1) {detGeo.Add(new TObjString("CALIFA"),        new TObjString("CALIFA.geo.root"));}
  if (TOF_Exists==1) {detGeo.Add(new TObjString("TOF"),           new TObjString("TOF.geo.root"));}
  if (mTOF_Exists==1) {detGeo.Add(new TObjString("MTOF"),          new TObjString("mTOF.geo.root"));}
  if (DCH_Exists==1) {detGeo.Add(new TObjString("DCH"),           new TObjString("DCH.geo.root"));}
  if (StaRTrack_Exists==1) {detGeo.Add(new TObjString("STaRTrack"),     new TObjString("StaRTrack.geo.root"));}
  if (NeuLAND_Exists==1) {detGeo.Add(new TObjString("SCINTNEULAND"),  new TObjString("NeuLAND_VETOed_Bars.geo.root"));}
  if (NeuLAND_Exists==2) {detGeo.Add(new TObjString("REALNEULAND"),  new TObjString("NeuLAND_VETOed_Bars.geo.root"));}
  if (NEBULA_Exists==1) {detGeo.Add(new TObjString("SCINTNEBULA"),  new TObjString("NEBULA.geo.root"));}
  if (VacVessel_Exists==1) {detGeo.Add(new TObjString("VACVESSELCOOL"), new TObjString("Vacuum_Vessel.geo.root"));}
  if (MFI_Exists==1) {detGeo.Add(new TObjString("MFI"),           new TObjString("MFI.geo.root"));}
  if (VETO_Exists==1) {detGeo.Add(new TObjString("SCINTNVETO_SEG"),  new TObjString("VETO.geo.root"));}
  if (VETO_Exists==2) {detGeo.Add(new TObjString("REALNVETO_SEG"),  new TObjString("VETO_Real.geo.root"));}
  if (NEB_VETO_Exists==1) {detGeo.Add(new TObjString("SCINTNEB_VETO_SEG"),  new TObjString("NEBULA_VETO.geo.root"));}
  if (PSP_Exists==1) {detGeo.Add(new TObjString("PSP"),           new TObjString("PSP.geo.root"));}
  TMap* fDetList = &detGeo;

  // ==================================================================================================================================
  
  TString dir = getenv("VMCWORKDIR");
  TString r3bdir = dir + "/macros";
  
  TString r3b_geomdir = dir + "/geometry";
  gSystem->Setenv("GEOMPATH",r3b_geomdir.Data());
  gSystem->Setenv("GEOMPATH","../../Data/Geometry/");												// NOTE: Changed by C. A. Douma!
  
  TString r3b_confdir = dir + "gconfig";
  gSystem->Setenv("CONFIG_DIR",r3b_confdir.Data());
  
  // In general, the following parts need not be touched
  // ========================================================================
  
  // ----    Debug option   -------------------------------------------------
  gDebug = 0;
  // ------------------------------------------------------------------------
  
  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------
  
  
  // -----   Create simulation run   ----------------------------------------
  FairRunSim* run = new FairRunSim();
  run->SetName(fMC.Data());              // Transport engine
  run->SetOutputFile(OutFile.Data());          // Output file
  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  
  //  R3B Special Physics List in G4 case
  if ( (fUserPList  == kTRUE ) &&
      (fMC.CompareTo("TGeant4")   == 0)) {
    run->SetUserConfig("g4R3bConfig.C");
    run->SetUserCuts("SetR3BCuts.C");
  }
  
  
  // -----   Create media   -------------------------------------------------
  run->SetMaterials("media_r3b.geo");       // Materials
  
  
  // Magnetic field map type
  Int_t fFieldMap = 0;
  
  
  // Global Transformations
  //- Two ways for a Volume Rotation are supported
  //-- 1) Global Rotation (Euler Angles definition)
  //-- This represent the composition of : first a rotation about Z axis with
  //-- angle phi, then a rotation with theta about the rotated X axis, and
  //-- finally a rotation with psi about the new Z axis.
  Double_t phi,theta,psi;
  
  //-- 2) Rotation in Ref. Frame of the Volume
  //-- Rotation is Using Local Ref. Frame axis angles
  Double_t thetaX,thetaY,thetaZ;
  
  //- Global Translation  Lab. frame.
  Double_t tx,ty,tz;
  
  
  // -----   Create R3B geometry --------------------------------------------
  //R3B Cave definition
  FairModule* cave= new R3BCave("CAVE");
  cave->SetGeometryFileName("r3b_cave.geo");
  run->AddModule(cave);
  
  //R3B Target definition
  if (fDetList->FindObject("TARGET") ) {
    R3BModule* target= new R3BTarget(Target.Data());
    target->SetGeometryFileName(((TObjString*)fDetList->GetValue("TARGET"))->GetString().Data());
    run->AddModule(target);
  }
  
  //R3B SiTracker Cooling definition
  if (fDetList->FindObject("VACVESSELCOOL") ) {
    R3BModule* vesselcool= new R3BVacVesselCool(Target.Data());
    vesselcool->SetGeometryFileName(((TObjString*)fDetList->GetValue("VACVESSELCOOL"))->GetString().Data());
    run->AddModule(vesselcool);
  }
  
  //R3B Magnet definition
  if (fDetList->FindObject("GLAD") ) {
    fFieldMap = 1;
    R3BModule* mag = new R3BGladMagnet("GladMagnet");
    mag->SetGeometryFileName(((TObjString*)fDetList->GetValue("GLAD"))->GetString().Data());
    run->AddModule(mag);
  }
  
  if (fDetList->FindObject("CALIFA") ) {
    // CALIFA Calorimeter
    R3BDetector* calo = new R3BCalo("Califa", kTRUE);
    ((R3BCalo *)calo)->SelectGeometryVersion(17);												// NOTE: Changed by C. A. Douma!
    //Selecting the Non-uniformity of the crystals (1 means +-1% max deviation)
    ((R3BCalo *)calo)->SetNonUniformity(1.0);
    calo->SetGeometryFileName(((TObjString*)fDetList->GetValue("CALIFA"))->GetString().Data());
    run->AddModule(calo);
  }

  // STaRTrack
  if (fDetList->FindObject("STaRTrack")  ) {
    R3BDetector* tra = new R3BSTaRTra("STaRTrack", kTRUE);
    tra->SetGeometryFileName(((TObjString*)fDetList->GetValue("STaRTrack"))->GetString().Data());
    run->AddModule(tra);
  }
  
  // DCH drift chambers
  if (fDetList->FindObject("DCH") ) {
    R3BDetector* dch = new R3BDch("Dch", kTRUE);
    dch->SetGeometryFileName(((TObjString*)fDetList->GetValue("DCH"))->GetString().Data());
    run->AddModule(dch);
  }
  
  // Tof
  if (fDetList->FindObject("TOF") ) {
    R3BDetector* tof = new R3BTof("Tof", kTRUE);
    tof->SetGeometryFileName(((TObjString*)fDetList->GetValue("TOF"))->GetString().Data());
    run->AddModule(tof);
  }
  
  // mTof
  if (fDetList->FindObject("MTOF") ) {
    R3BDetector* mTof = new R3BmTof("mTof", kTRUE);
    mTof->SetGeometryFileName(((TObjString*)fDetList->GetValue("MTOF"))->GetString().Data());
    run->AddModule(mTof);
  }

  // NeuLand Scintillator Detector
  if(fDetList->FindObject("SCINTNEULAND")) {
    R3BDetector* land = new R3BLand("Land", kTRUE);
    land->SetVerboseLevel(1);
    land->SetGeometryFileName(((TObjString*)fDetList->GetValue("SCINTNEULAND"))->GetString().Data());
    run->AddModule(land);
  }
  
  // Real NeuLAND Detector:
  if(fDetList->FindObject("REALNEULAND")) {
    R3BDetector* neuland = new R3BNeuland();
    neuland->SetGeometryFileName(((TObjString*)fDetList->GetValue("REALNEULAND"))->GetString().Data());
    run->AddModule(neuland);
  }
  
  // NEBULA Scintillator Detector											                	// NOTE: Changed by C. A. Douma!
  if(fDetList->FindObject("SCINTNEBULA")) { 
    R3BDetector* neb = new R3BLand("nebula", kTRUE);
    neb->SetVerboseLevel(1);
    neb->SetGeometryFileName(((TObjString*)fDetList->GetValue("SCINTNEBULA"))->GetString().Data());
    run->AddModule(neb);
  }
  
  // Nveto_seg Scintillator Detector
  if(fDetList->FindObject("SCINTNVETO_SEG")) {
  R3BDetector* veto_seg = new R3Bveto_seg("veto_seg", kTRUE);
  veto_seg->SetVerboseLevel(1);
  veto_seg->SetGeometryFileName(((TObjString*)fDetList->GetValue("SCINTNVETO_SEG"))->GetString().Data());
  run->AddModule(veto_seg);
  }
  
  // Real VETO Detector:
  if(fDetList->FindObject("REALNVETO_SEG")) {
    R3BDetector* Nveto_seg = new R3BNveto_seg();
    Nveto_seg->SetGeometryFileName(((TObjString*)fDetList->GetValue("REALNVETO_SEG"))->GetString().Data());
    run->AddModule(Nveto_seg);
  }
  
  // NEBULA VETO Detector												                       // NOTE: Changed by C. A. Douma!
  if(fDetList->FindObject("SCINTNEB_VETO_SEG")) {
  R3BDetector* neb_veto = new R3Bveto_seg("neb_veto", kTRUE);
  neb_veto->SetVerboseLevel(1);
  neb_veto->SetGeometryFileName(((TObjString*)fDetList->GetValue("SCINTNEB_VETO_SEG"))->GetString().Data());
  run->AddModule(neb_veto);
  }

  // MFI Detector
  if(fDetList->FindObject("MFI")) {
    R3BDetector* mfi = new R3BMfi("Mfi", kTRUE);
    mfi->SetGeometryFileName(((TObjString*)fDetList->GetValue("MFI"))->GetString().Data());
    run->AddModule(mfi);
  }

  // PSP Detector
  if(fDetList->FindObject("PSP")) {
    R3BDetector* psp = new R3BPsp("Psp", kTRUE);
    psp->SetGeometryFileName(((TObjString*)fDetList->GetValue("PSP"))->GetString().Data());
    run->AddModule(psp);
  }
  
  
  // -----   Create R3B  magnetic field ----------------------------------------
  Int_t typeOfMagneticField = 0;
  Int_t fieldScale = 1;
  Bool_t fVerbose = kFALSE;
  
  //NB: <D.B>
  // If the Global Position of the Magnet is changed
  // the Field Map has to be transformed accordingly
  FairField *magField = NULL;
  if (fFieldMap == 0) {
    magField = new R3BAladinFieldMap("AladinMaps");
    ((R3BAladinFieldMap*)magField)->SetCurrent(fMeasCurrent);
    ((R3BAladinFieldMap*)magField)->SetScale(fieldScale);
    
    if ( fR3BMagnet == kTRUE ) {
      run->SetField(magField);
    } else {
      run->SetField(NULL);
    }
  } else if(fFieldMap == 1){
    magField = new R3BGladFieldMap("R3BGladMap");
    ((R3BGladFieldMap*)magField)->SetPosition(0., 0., +350-119.94);
    ((R3BGladFieldMap*)magField)->SetScale(fieldScale);
    
    if ( fR3BMagnet == kTRUE ) {
      run->SetField(magField);
    } else {
      run->SetField(NULL);
    }
  }  //! end of field map section
  
  
  // -----   Create PrimaryGenerator   --------------------------------------
  // 1 - Create the Main API class for the Generator
  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();
  
  if (fGenerator.CompareTo("box") == 0  ) {
    // 2- Define the BOX generator
    Int_t pdgId = Particle_ID; // pion beam													// NOTE: Changed by C. A. Douma!
    Int_t Nprim = Num_Prim_Particle; 														// NOTE: Changed by C. A. Douma!
    Double32_t theta1 = ThetaMin;  // polar angle distribution											// NOTE: Changed by C. A. Douma!
    Double32_t theta2 = ThetaMax;														// NOTE: Changed by C. A. Douma!
    Double32_t P_min = Emin;															// NOTE: Changed by C. A. Douma!
    Double32_t P_max = Emax;															// NOTE: Changed by C. A. Douma!
    Double32_t phi1 = PhiMin;															// NOTE: Changed by C. A. Douma!
    Double32_t phi2 = PhiMax;															// NOTE: Changed by C. A. Douma!
    FairBoxGenerator* boxGen = new FairBoxGenerator(pdgId, Nprim);										// NOTE: Changed by C. A. Douma!
    boxGen->SetThetaRange(theta1, theta2);													// NOTE: Changed by C. A. Douma!
    boxGen->SetPRange(P_min, P_max);														// NOTE: Changed by C. A. Douma!
    boxGen->SetPhiRange(phi1, phi2);														// NOTE: Changed by C. A. Douma!
    boxGen->SetXYZ(X_Beam, Y_Beam, Z_Beam);													// NOTE: Changed by C. A. Douma!
//    boxGen->SetXYZ(0.0, 0.0, -300.);
    // add the box generator
    primGen->AddGenerator(boxGen);
  }
  
  if (fGenerator.CompareTo("ascii") == 0  ) {
    R3BAsciiGenerator* gen = new R3BAsciiGenerator((dir+"/input/"+InFile).Data());
    primGen->AddGenerator(gen);
  }
  
  if (fGenerator.CompareTo("r3b") == 0  ) {
    R3BSpecificGenerator *pR3bGen = new R3BSpecificGenerator();
    
    // R3bGen properties
    pR3bGen->SetBeamInteractionFlag("off");
    pR3bGen->SetBeamInteractionFlag("off");
    pR3bGen->SetRndmFlag("off");
    pR3bGen->SetRndmEneFlag("off");
    pR3bGen->SetBoostFlag("off");
    pR3bGen->SetReactionFlag("on");
    pR3bGen->SetGammasFlag("off");
    pR3bGen->SetDecaySchemeFlag("off");
    pR3bGen->SetDissociationFlag("off");
    pR3bGen->SetBackTrackingFlag("off");
    pR3bGen->SetSimEmittanceFlag("off");
    
    // R3bGen Parameters
    pR3bGen->SetBeamEnergy(1.); // Beam Energy in GeV
    pR3bGen->SetSigmaBeamEnergy(1.e-03); // Sigma(Ebeam) GeV
    pR3bGen->SetParticleDefinition(2212); // Use Particle Pdg Code
    pR3bGen->SetEnergyPrim(0.3); // Particle Energy in MeV
    Int_t fMultiplicity = 50;
    pR3bGen->SetNumberOfParticles(fMultiplicity); // Mult.
    
    // Reaction type
    //        1: "Elas"
    //        2: "iso"
    //        3: "Trans"
    pR3bGen->SetReactionType("Elas");
    
    // Target  type
    //        1: "LeadTarget"
    //        2: "Parafin0Deg"
    //        3: "Parafin45Deg"
    //        4: "LiH"
    
    pR3bGen->SetTargetType(Target.Data());
    Double_t thickness = (0.11/2.)/10.;  // cm
    pR3bGen->SetTargetHalfThicknessPara(thickness); // cm
    pR3bGen->SetTargetThicknessLiH(3.5); // cm
    pR3bGen->SetTargetRadius(1.); // cm
    
    pR3bGen->SetSigmaXInEmittance(1.); //cm
    pR3bGen->SetSigmaXPrimeInEmittance(0.0001); //cm
    
    // Dump the User settings
    pR3bGen->PrintParameters();
    primGen->AddGenerator(pR3bGen);
  }
  
  run->SetGenerator(primGen);
  
  
  //-------Set visualisation flag to true------------------------------------
  run->SetStoreTraj(fVis);
  

  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");

  
  // -----   Initialize simulation run   ------------------------------------
  run->Init();

  
  // ------  Increase nb of step for CALO
  Int_t nSteps = -15000;
  gMC->SetMaxNStep(nSteps);
  
  
  // -----   Runtime database   ---------------------------------------------
  R3BFieldPar* fieldPar = (R3BFieldPar*) rtdb->getContainer("R3BFieldPar");
  if(NULL != magField)
  {
      fieldPar->SetParameters(magField);
      fieldPar->setChanged();
  }
  Bool_t kParameterMerged = kTRUE;
  FairParRootFileIo* parOut = new FairParRootFileIo(kParameterMerged);
  parOut->open(ParFile.Data());
  rtdb->setOutput(parOut);
  rtdb->saveOutput();
  rtdb->print();
  
  
  // -----   Start run   ----------------------------------------------------
  if(nEvents > 0) {
    run->Run(nEvents);
  }
  
  
  // -----   Finish   -------------------------------------------------------
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  cout << endl << endl;
  cout << "Macro finished succesfully." << endl;
  cout << "Output file is "    << OutFile << endl;
  cout << "Parameter file is " << ParFile << endl;
  cout << "Real time " << rtime << " s, CPU time " << ctime
  << "s" << endl << endl;
  // ------------------------------------------------------------------------

  
  cout << " Test passed" << endl;
  cout << " All ok " << endl;
}
