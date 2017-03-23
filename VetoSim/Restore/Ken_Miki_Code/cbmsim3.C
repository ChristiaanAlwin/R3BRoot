//   In a ROOT session, you can do:
//      Root > .L cbmsim.C
//      Root > cbmsim t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
 //    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch


#define cbmsim_cxx
#include "cbmsim.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void cbmsim::InitTree()
{
  //miki++
  //For R3BROOT output 
  //  tLand = new TTree("land","Tree for LAND Data");
  //  tLand->Branch("test",&test,"neko");
  fLand = new TFile("tLand4.root","RECREATE"); // output file
  tLand = new TTree("tLand","hoge analysis"); // define the Tree

  tLand->Branch("Event", &Event   , "Event/I");
  Event=0;


  char name1[60]; 
  char name2[60]; 
  sprintf(name1, "GFIn"  );
  sprintf(name2, "GFIn/I");     
  tLand->Branch(name1, &GFIn, name2);

  sprintf(name1, "GFIp"  );
  sprintf(name2, "GFIp[GFIn]/I");     
  tLand->Branch(name1, GFIp , name2);
  for(Int_t iPlane=1; iPlane<=nPlane; iPlane++){
    sprintf(name1, "Nnp%02dn"            , iPlane);
    sprintf(name2, "Nnp%02dn/I"          , iPlane);     
    tLand->Branch(name1, &Nnpn[iPlane]   , name2);

    sprintf(name1, "Nnp%02di"            , iPlane);
    sprintf(name2, "Nnp%02di[Nnp%02dn]/I", iPlane, iPlane);     
    tLand->Branch(name1, &Nnpi[iPlane][0], name2);

    sprintf(name1, "Nnp%02dt"            , iPlane);
    sprintf(name2, "Nnp%02dt[Nnp%02dn]/D", iPlane, iPlane);     
    tLand->Branch(name1, &Nnpt[iPlane][0], name2);

    sprintf(name1, "Nnp%02de"            , iPlane);
    sprintf(name2, "Nnp%02de[Nnp%02dn]/D", iPlane, iPlane);     
    tLand->Branch(name1, &Nnpe[iPlane][0], name2);

    sprintf(name1, "Nnp%02dx"            , iPlane);
    sprintf(name2, "Nnp%02dx[Nnp%02dn]/D", iPlane, iPlane);     
    tLand->Branch(name1, &Nnpx[iPlane][0], name2);

    sprintf(name1, "Nnp%02dy"            , iPlane);
    sprintf(name2, "Nnp%02dy[Nnp%02dn]/D", iPlane, iPlane);     
    tLand->Branch(name1, &Nnpy[iPlane][0], name2);

    sprintf(name1, "Nnp%02dz"            , iPlane);
    sprintf(name2, "Nnp%02dz[Nnp%02dn]/D", iPlane, iPlane);     
    tLand->Branch(name1, &Nnpz[iPlane][0], name2);

    sprintf(name1, "Nnp%02dxReact"            , iPlane);
    sprintf(name2, "Nnp%02dxReact[Nnp%02dn]/D", iPlane, iPlane);     
    tLand->Branch(name1, &NnpxReact[iPlane][0], name2);

    sprintf(name1, "Nnp%02dyReact"            , iPlane);
    sprintf(name2, "Nnp%02dyReact[Nnp%02dn]/D", iPlane, iPlane);     
    tLand->Branch(name1, &NnpyReact[iPlane][0], name2);

    sprintf(name1, "Nnp%02dzReact"            , iPlane);
    sprintf(name2, "Nnp%02dzReact[Nnp%02dn]/D", iPlane, iPlane);     
    tLand->Branch(name1, &NnpzReact[iPlane][0], name2);

    sprintf(name1, "Nnp%02dp"            , iPlane);
    sprintf(name2, "Nnp%02dp[Nnp%02dn]/I", iPlane, iPlane);     
    tLand->Branch(name1, &Nnpp[iPlane][0], name2);
  }
  //miki--
}

void cbmsim::Loop(){
  if (fChain == 0) return;
  
  Long64_t nentries = fChain->GetEntriesFast();
  
  Long64_t nbytes = 0, nb = 0;
  Long64_t jentry;
  for (jentry=0; jentry<nentries;jentry++) {
    if(jentry%1000==0){printf("\r%lld",jentry); fflush(stdout);}
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   
    nbytes += nb;
    
    //INITIALIZE
    //miki++
    Event++;
    for (Int_t i=1; i<=nBarAll; i++){
      de[i]=0.0;
      //    ELossMax[i]=0.0;
      TOFmin[i]=1.e20;
      Xmin[i]=-1.e20;
      Ymin[i]=-1.e20;
      Zmin[i]=-1.e20;
      //    PIDEmax[i]=0;
      PIDTmin[i]=0;
    }
    //miki--
    
    //MAIN
    //miki++
    // calculate deltaE & find minimum TOF particle

    // Int_t iBar, iPlane;
    // printf("---- %u -----\n",jentry);
    // printf("GFI Points: %d\n",GFIPoint_);
    // for (Int_t iPoint=0; iPoint<GFIPoint_; iPoint++){
    //   printf("%d  %d\n",iPoint,MCTrack_fPdgCode[GFIPoint_fTrackID[iPoint]]);
    // }
      
    Int_t iBar, iPlane;
    for (Int_t iPoint=0; iPoint<LandPoint_; iPoint++){
      iBar = LandPoint_fSector[iPoint];
      de[iBar] += LandPoint_fELoss[iPoint];
      iPlane = (iBar-1)/nBar + 1;
      
      //    if(fELoss>ELossMax[cp2]){
      //      ELossMax[cp2] = fELoss;
      //      PIDEmax[cp2] = gMC->TrackPid(); //Particle ID
      //    }
      
      if(LandPoint_fTime[iPoint] < TOFmin[iBar]){
	TOFmin[iBar] = LandPoint_fTime[iPoint];
	Xmin[iBar] = LandPoint_fX[iPoint];
	//Ymin[iBar] = LandPoint_fZ[iPoint]; bug: fixed on 201408271748
	Ymin[iBar] = LandPoint_fY[iPoint];
	Zmin[iBar] = LandPoint_fZ[iPoint];

	//Loop back to the Reaction point
	Int_t iTrack = LandPoint_fTrackID[iPoint];
	while(IsInNeuLANDPlane(iPlane, MCTrack_fStartX[iTrack], MCTrack_fStartY[iTrack], MCTrack_fStartZ[iTrack])){
	  iTrack = MCTrack_fMotherId[iTrack];
	}
	XReact[iBar] = MCTrack_fStartX[iTrack];
	YReact[iBar] = MCTrack_fStartY[iTrack];
	ZReact[iBar] = MCTrack_fStartZ[iTrack];
	PIDTmin[iBar] = MCTrack_fPdgCode[iTrack];
      }
    }

    // Fill in tree
    //GFI
    GFIn=GFIPoint_;
    for(Int_t iPoint=0; iPoint<GFIn; iPoint++){
      GFIp[iPoint] = MCTrack_fPdgCode[GFIPoint_fTrackID[iPoint]];
    }

    //NeuLAND
    for (iPlane=1; iPlane<=nPlane; iPlane++){
      Nnpn[iPlane] = 0;
    }
    for (Int_t i=1; i<=nBarAll; i++){
      if(de[i]>dEthr){
	//      printf("%d %lf %lf\n",i,de[i],TOFmin[i]);
	
	Int_t iPlane = (i-1) / nBar + 1;
	iBar   = (i-1) % nBar + 1;
	Nnpn[iPlane]++;
	Int_t iHit = Nnpn[iPlane] - 1;
	
	Nnpi[iPlane][iHit] = iBar;
	Nnpt[iPlane][iHit] = TOFmin[i];
	Nnpe[iPlane][iHit] = de[i];
	Nnpx[iPlane][iHit] = Xmin[i];
	Nnpy[iPlane][iHit] = Ymin[i];
	Nnpz[iPlane][iHit] = Zmin[i];
	NnpxReact[iPlane][iHit] = XReact[i];
	NnpyReact[iPlane][iHit] = YReact[i];
	NnpzReact[iPlane][iHit] = ZReact[i];
	Nnpp[iPlane][iHit] = PIDTmin[i];
	//      Nnpp[iPlane][iHit] = PIDEmax[i];
	//      printf("nekosan %d %lf %lf %lf %lf\n",iBar,Xmin[i],Ymin[i],Zmin[i],TOFmin[i]);
      }
    }
    tLand->Fill();
    //miki--
  }

  printf("\r%lld END\n",jentry);
  //Output root file
  //miki++
  tLand->Write();
  fLand->Close();
  //miki--
}


Int_t cbmsim::IsInNeuLAND(Double32_t x, Double32_t y, Double32_t z){
  if(abs(x-xcNeuLAND)<dxNeuLAND
     && abs(y-ycNeuLAND)<dyNeuLAND
     && abs(z-zcNeuLAND)<dzNeuLAND){
    return 1;
  } else {
    return 0;
  }
}

Int_t cbmsim::IsInNeuLANDPlane(Int_t iPlane, Double32_t x, Double32_t y, Double32_t z){
  if(abs(x-xcNeuLAND)<dxNeuLANDPlane
     && abs(y-ycNeuLAND)<dyNeuLANDPlane
     && abs(z- (zcNeuLAND+ (iPlane-nPlane/2.0-0.5)*dzNeuLANDPlane*2.0) )<dzNeuLANDPlane){
    return 1;
  } else {
    return 0;
  }
}
