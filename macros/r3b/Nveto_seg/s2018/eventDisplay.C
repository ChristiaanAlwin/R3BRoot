{
  // -----   Reconstruction run   -------------------------------------------
  FairRunAna *fRun= new FairRunAna();
  
  // -----   Runtime database   ---------------------------------------------
  FairRuntimeDb* rtdb = fRun->GetRuntimeDb();
  FairParRootFileIo*  parIo1 = new FairParRootFileIo();
  //parIo1->open("r3bveto_segpar.root");
  parIo1->open("r3bpar.600AMeV.1n.500keV.15m.root");
  rtdb->setFirstInput(parIo1);
  rtdb->print();
 
  //fRun->SetInputFile("r3bveto_segsim.root");
  fRun->SetInputFile("r3bsim.600AMeV.1n.500keV.15m.root");
//  fRun->AddFriend("veto_seg_digi.root"); // 2nd file where veto_segHits or whatever are stored
  fRun->SetOutputFile("test.root");

  R3BEventManager *fMan= new R3BEventManager();
  R3BMCTracks *Track =  new R3BMCTracks ("Monte-Carlo Tracks");
  FairMCPointDraw *veto_segPoints =   new FairMCPointDraw ("veto_segPoint", kOrange,  kFullSquare);
//  FairHitDraw *veto_segRecoHit = new FairHitDraw("veto_segNeTracks");
//  FairMCPointDraw *veto_segRecoHit =   new FairMCPointDraw ("veto_segNeTracks",kGreen,  kFullSquare);

  fMan->AddTask(Track);
  fMan->AddTask(veto_segPoints);
//  fMan->AddTask(veto_segRecoHit);  
  
  fMan->Init();  
}
