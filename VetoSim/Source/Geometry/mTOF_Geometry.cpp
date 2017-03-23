#include <iomanip>
#include <iostream>
#include "TGeoManager.h"
#include "TMath.h"

// Create Matrix Unity
TGeoRotation *fGlobalRot = new TGeoRotation();

// Create a null translation
TGeoTranslation *fGlobalTrans = new TGeoTranslation();
TGeoRotation *fRefRot = NULL;

TGeoManager*  gGeoMan = NULL;

Double_t fThetaX = 0.;
Double_t fThetaY = 0.;
Double_t fThetaZ = 0.;
Double_t fPhi   = 0.;
Double_t fTheta = 0.;
Double_t fPsi   = 0.;
Double_t fX = 0.;
Double_t fY = 0.;
Double_t fZ = 0.;
Bool_t fLocalTrans = kFALSE;
Bool_t fLabTrans = kTRUE;

TGeoCombiTrans* GetGlobalPosition(TGeoCombiTrans *fRef);

void mTOF_Geometry()										// NOTE: Change by C. A. Douma!
{
  // =================================================================================
  // NOTE: This section is added by C. A. Douma.
  
  // The function will no longer take inputs or give outputs. Inputs are
  // taken from the Inputs.root file. Hence in this section we will collect the 
  // inputs we need to build the geometry. The rest of the code will be
  // unchanged except for the adjustments we need to make to correctly process
  // the input parameters.
  
  // We begin with connecting to the Inputs.root file:
  TFile* inputs = new TFile("../../Data/Inputs/Inputs.root","read");
  TH1D* Doubles = (TH1D*) inputs->Get("Doubles");
  TH1I* Integers = (TH1I*) inputs->Get("Integers");
  
  // Now we get our inputs:
  Double_t X_position = Doubles->GetBinContent(1);
  Double_t Y_position = Doubles->GetBinContent(2);
  Double_t Z_position = Doubles->GetBinContent(3);
  
  Double_t Theta_Rotation_X = Doubles->GetBinContent(4);
  Double_t Theta_Rotation_Y = Doubles->GetBinContent(5);
  Double_t Theta_Rotation_Z = Doubles->GetBinContent(6);
  
  Double_t Phi_Rotation_X = Doubles->GetBinContent(7);
  Double_t Phi_Rotation_Y = Doubles->GetBinContent(8);
  Double_t Phi_Rotation_Z = Doubles->GetBinContent(9);
  
  // That's it.
  // ================================================================================
  
  fGlobalTrans->SetTranslation(0.0,0.0,0.0);

  // -------   Load media from media file   -----------------------------------
  FairGeoLoader*    geoLoad = new FairGeoLoader("TGeo","FairGeoLoader");
  FairGeoInterface* geoFace = geoLoad->getGeoInterface();
  TString geoPath = gSystem->Getenv("VMCWORKDIR");
  TString medFile = geoPath + "/geometry/media_r3b.geo";
  geoFace->setMediaFile(medFile);
  geoFace->readMedia();
  gGeoMan = gGeoManager;
  // --------------------------------------------------------------------------


  // -------   Geometry file name (output)   ----------------------------------
  // TString geoFileName = geoPath + "/geometry/mtof_";
  // geoFileName = geoFileName + geoTag + ".geo.root";
  TString geoFileName = "../../Data/Geometry/mTOF.geo.root";						// NOTE: Change by C. A. Douma!
  // --------------------------------------------------------------------------


  // -----------------   Get and create the required media    -----------------
  FairGeoMedia*   geoMedia = geoFace->getMedia();
  FairGeoBuilder* geoBuild = geoLoad->getGeoBuilder();

  FairGeoMedium* mAir      = geoMedia->getMedium("Air");
  if ( ! mAir ) Fatal("Main", "FairMedium Air not found");
  geoBuild->createMedium(mAir);
  TGeoMedium* pMed2 = gGeoMan->GetMedium("Air");
  if ( ! pMed2 ) Fatal("Main", "Medium Air not found");
  
  FairGeoMedium* mTof      = geoMedia->getMedium("plasticFormTOF");
  if ( ! mTof ) Fatal("Main", "FairMedium plasticFormTOF not found");
  geoBuild->createMedium(mTof);
  TGeoMedium* pMed34 = gGeoMan->GetMedium("plasticFormTOF");
  if ( ! pMed34 ) Fatal("Main", "Medium plasticFormTOF not found");
  // --------------------------------------------------------------------------



  // --------------   Create geometry and top volume  -------------------------
  gGeoMan = (TGeoManager*)gROOT->FindObject("FAIRGeom");
  gGeoMan->SetName("mTOFgeom");
  TGeoVolume* top = new TGeoVolumeAssembly("TOP");
  gGeoMan->SetTopVolume(top);
  // --------------------------------------------------------------------------



  // out-of-file geometry definition
  Double_t dx,dy,dz;
  Double_t a;
  Double_t thx, phx, thy, phy, thz, phz;
  Double_t z, density, w;
  Int_t nel, numed;
  

  
  // TRANSFORMATION MATRICES
  // Combi transformation:
  //NTF position
  //dx = -154.815998;//Justyna
  //dy = 0.000000;  //Justyna
  //dz = 761.755160;//Justyna
  //dx = -157.536214;//Justyna new
  //dy = -0.010000;  //Justyna new
  ////dz = 760.939056;//Justyna new
  //dz = 760.139056;//Justyna
  //dx = -155.709;//dE tracker
  //dy = 0.524;
  ////dz = 761.487;
  //dz = 760.687;	//try -0.8 like Justyna
  
  //LABPOS(FTF,-155.824045,0.523976,761.870346)
  dx = -155.824045;//dE tracker, correction due to wrong angle
  dy = 0.523976;
  dz = 761.870346;
  
  // Rotation:
  thx = -106.700000;    phx = 0.000000;
  thy = 90.000000;    phy = 90.000000;
  thz = -16.700000;    phz = 0.000000;
  
  // Processing of inputs: 										// NOTE: Change by C. A. Douma!
  dx = X_position;											// NOTE: Change by C. A. Douma!
  dy = Y_position;											// NOTE: Change by C. A. Douma!
  dz = Z_position;											// NOTE: Change by C. A. Douma!
  thx = Theta_Rotation_X;										// NOTE: Change by C. A. Douma!
  thy = Theta_Rotation_Y;										// NOTE: Change by C. A. Douma!
  thz = Theta_Rotation_Z;										// NOTE: Change by C. A. Douma!
  phx = Phi_Rotation_X;											// NOTE: Change by C. A. Douma!
  phy = Phi_Rotation_Y;											// NOTE: Change by C. A. Douma!
  phz = Phi_Rotation_Z;											// NOTE: Change by C. A. Douma!
  
  /*    dx = -171.1;
   dy = 2.400000;
   dz = 548.95;
   // dz = 0.;
   // Rotation:
   thx = -121.000000;    phx = 0.000000;
   thy = 90.000000;    phy = 90.000000;
   thz = -31.000000;    phz = 0.000000;*/
  
  TGeoRotation *pMatrix3 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
  TGeoCombiTrans*
  pMatrix2 = new TGeoCombiTrans("", dx,dy,dz,pMatrix3);
  
  /*
   // TRANSFORMATION MATRICES
   // Combi transformation:
   dx = 151.000000;
   dy = 0.000000;
   dz = 758.000000;
   // Rotation:
   thx = 106.700000;    phx = 0.000000;
   thy = 90.000000;    phy = 90.000000;
   thz = 16.700000;    phz = 0.000000;
   TGeoRotation *pMatrix3 = new TGeoRotation("",thx,phx,thy,phy,thz,phz);
   TGeoCombiTrans*
   pMatrix2 = new TGeoCombiTrans("", dx,dy,dz,pMatrix3);
   */
  
  // Shape: World type: TGeoBBox
  TGeoVolume* pWorld = gGeoManager->GetTopVolume();
  pWorld->SetVisLeaves(kTRUE);
  
  // Create a global Mother Volume
  /*
   dx = 200.000000;
   dy = 200.000000;
   dz = 200.000000;
   TGeoShape *pBoxWorld = new TGeoBBox("mTofBoxWorld", dx,dy,dz);
   TGeoVolume*
   pWorld  = new TGeoVolume("mTofBoxLogWorld",pBoxWorld, pMed2);
   pWorld->SetVisLeaves(kTRUE);
   TGeoCombiTrans *pGlobalc = GetGlobalPosition();
   
   // add the sphere as Mother Volume
   pAWorld->AddNode(pWorld, 0, pGlobalc);
   */
  
  
  // SHAPES, VOLUMES AND GEOMETRICAL HIERARCHY
  // Shape: mTOFBox type: TGeoBBox
  dx = 24.000000;
  dy = 24.000000;
  //dz = 0.250000;	//wrong: should be 0.5->1cm total
  dz = 0.500000;
  
  /*   dx = 94.450000;  //TFW size
   dy = 73.450000;
   dz = 0.500000;*/
  
  TGeoShape *pmTOFBox = new TGeoBBox("mTOFBox", dx,dy,dz);
  // Volume: mTOFLog
  TGeoVolume *
  pmTOFLog = new TGeoVolume("mTOFLog",pmTOFBox, pMed34);
  pmTOFLog->SetVisLeaves(kTRUE);
  
  TGeoVolumeAssembly *pmTof = new TGeoVolumeAssembly("mTOF");
  TGeoCombiTrans *t0 = new TGeoCombiTrans("t0");
  pmTof->AddNode(pmTOFLog, 0, t0);
  
  TGeoCombiTrans *pGlobal = GetGlobalPosition(pMatrix2);
  if (pGlobal){
    pWorld->AddNode(pmTof, 0, pGlobal);
  }else{
    pWorld->AddNode(pmTof, 0, pMatrix2);
  }
  
  
//  AddSensitiveVolume(pmTOFLog);
//  fNbOfSensitiveVol+=1;
  
  
  
  // ---------------   Finish   -----------------------------------------------
  gGeoMan->CloseGeometry();
  gGeoMan->CheckOverlaps(0.001);
  gGeoMan->PrintOverlaps();
  gGeoMan->Test();

  TFile* geoFile = new TFile(geoFileName, "RECREATE");
  top->Write();
  geoFile->Close();
  // --------------------------------------------------------------------------
}


TGeoCombiTrans* GetGlobalPosition(TGeoCombiTrans *fRef)
{
  if (fLocalTrans == kTRUE ) {
    
    if ( ( fThetaX == 0 )  && ( fThetaY==0 )  && ( fThetaZ == 0 )
	 &&
	 ( fX == 0 ) && ( fY == 0 ) && ( fZ == 0 )
	 )  return fRef;
    
    
    // X axis
    Double_t xAxis[3] = { 1. , 0. , 0. };
    Double_t yAxis[3] = { 0. , 1. , 0. };
    Double_t zAxis[3] = { 0. , 0. , 1. };
    // Reference Rotation
    fRefRot = fRef->GetRotation();
    
    if (fRefRot) {
      Double_t mX[3] = {0.,0.,0.};
      Double_t mY[3] = {0.,0.,0.};
      Double_t mZ[3] = {0.,0.,0.};
      
      fRefRot->LocalToMasterVect(xAxis,mX);
      fRefRot->LocalToMasterVect(yAxis,mY);
      fRefRot->LocalToMasterVect(zAxis,mZ);
      
      Double_t a[4]={ mX[0],mX[1],mX[2], fThetaX };
      Double_t b[4]={ mY[0],mY[1],mY[2], fThetaY };
      Double_t c[4]={ mZ[0],mZ[1],mZ[2], fThetaZ };
      
      ROOT::Math::AxisAngle aX(a,a+4);
      ROOT::Math::AxisAngle aY(b,b+4);
      ROOT::Math::AxisAngle aZ(c,c+4);
      
      ROOT::Math::Rotation3D fMatX( aX );
      ROOT::Math::Rotation3D fMatY( aY );
      ROOT::Math::Rotation3D fMatZ( aZ );
      
      ROOT::Math::Rotation3D  fRotXYZ = (fMatZ * (fMatY * fMatX));
      
      //cout << fRotXYZ << endl;
      
      Double_t fRotable[9]={0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
      fRotXYZ.GetComponents(
			    fRotable[0],fRotable[3],fRotable[6],
			    fRotable[1],fRotable[4],fRotable[7],
			    fRotable[2],fRotable[5],fRotable[8]
			    );
      TGeoRotation *pRot = new TGeoRotation();
      pRot->SetMatrix(fRotable);
      TGeoCombiTrans *pTmp = new TGeoCombiTrans(*fGlobalTrans,*pRot);
      
      // ne peut pas etre applique ici
      // il faut differencier trans et rot dans la multi.
      TGeoRotation rot_id;
      rot_id.SetAngles(0.0,0.0,0.0);
      
      TGeoCombiTrans c1;
      c1.SetRotation(rot_id);
      const Double_t *t = pTmp->GetTranslation();
      c1.SetTranslation(t[0],t[1],t[2]);
      
      TGeoCombiTrans c2;
      c2.SetRotation(rot_id);
      const Double_t *tt = fRefRot->GetTranslation();
      c2.SetTranslation(tt[0],tt[1],tt[2]);
      
      TGeoCombiTrans cc = c1 * c2 ;
      
      TGeoCombiTrans c3;
      c3.SetRotation(pTmp->GetRotation());
      TGeoCombiTrans c4;
      c4.SetRotation(fRefRot);
      
      TGeoCombiTrans ccc = c3 * c4;
      
      TGeoCombiTrans pGlobal;
      pGlobal.SetRotation(ccc.GetRotation());
      const Double_t *allt = cc.GetTranslation();
      pGlobal.SetTranslation(allt[0],allt[1],allt[2]);
      
      return  ( new TGeoCombiTrans( pGlobal ) );
      
    }else{
      
      cout << "-E- R3BDetector::GetGlobalPosition() \
	      No. Ref. Transformation defined ! " << endl;
      cout << "-E- R3BDetector::GetGlobalPosition() \
	      cannot create Local Transformation " << endl;
      return NULL;
    } //! fRefRot
    
  } else {
    // Lab Transf.
    if ( ( fPhi == 0 )  && ( fTheta==0 )  && ( fPsi == 0 )
	 &&
	 ( fX == 0 ) && ( fY == 0 ) && ( fZ == 0 )
	 )  return fRef;
    
    
    return ( new TGeoCombiTrans(*fGlobalTrans,*fGlobalRot) );
    
  }
}

