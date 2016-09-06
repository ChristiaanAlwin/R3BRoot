// Include C++ headers:
#include <iomanip>
#include <iostream>

// include ROOT headers:
#include "TGeoManager.h"
#include "TMath.h"

// Declare require global variables for the geometry transformations:
TGeoRotation *fGlobalRot = new TGeoRotation();
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
Double_t fZ = 1550.;
Bool_t fLocalTrans = kFALSE;
Bool_t fLabTrans = kTRUE;
TGeoCombiTrans* GetGlobalPosition(TGeoCombiTrans *fRef);

// Define the main function:
void Chamber_Dummy_Geometry()
{
  // This function creates a geometry file for the R3B vacuum chamber behind
  //the GLAD magnet. The geometry is not exact, just a simple volume that we can
  // use until the real chamber is implemented.
    
  // Written by C. A. Douma, for the R3B Collaboration.
  
  // ===========================================================================
  // Before we do anything: Draw the global positioning parameters from
  // our inputs-file:
  TFile* inputs = new TFile("../../Data/Inputs/Inputs.root","read");
  TH1D* Doubles = (TH1D*) inputs->Get("Doubles");
  TH1I* Integers = (TH1I*) inputs->Get("Integers");
  
  // Now we get our parameters:
  Double_t X_position = Doubles->GetBinContent(151);
  Double_t Y_position = Doubles->GetBinContent(152);
  Double_t Z_position = Doubles->GetBinContent(153);
  
  Double_t Rotation_X = Doubles->GetBinContent(154);
  Double_t Rotation_Y = Doubles->GetBinContent(155);
  Double_t Rotation_Z = Doubles->GetBinContent(156);
  
  // NOTE: Shift z, so we can use zero as default in inputs:
  Z_position = Z_position + 1000.0;

  // Define the global positioning transformation:
  fGlobalTrans->SetTranslation(0.0,0.0,0.0);

  // Coonect to the FAIR medium file:
  FairGeoLoader*    geoLoad = new FairGeoLoader("TGeo","FairGeoLoader");
  FairGeoInterface* geoFace = geoLoad->getGeoInterface();
  TString geoPath = gSystem->Getenv("VMCWORKDIR");
  TString medFile = geoPath + "/geometry/media_r3b.geo";
  geoFace->setMediaFile(medFile);
  geoFace->readMedia();
  gGeoMan = gGeoManager;
  FairGeoMedia*   geoMedia = geoFace->getMedia();
  FairGeoBuilder* geoBuild = geoLoad->getGeoBuilder();

  // Get pointers to the media we need:
  FairGeoMedium* mAl = geoMedia->getMedium("aluminium");
  if ( ! mAl ) Fatal("Main","FairMedium aluminium not found");
  geoBuild->createMedium(mAl);
  TGeoMedium* pMedAl = gGeoMan->GetMedium("aluminium");
  if ( ! pMedAl ) Fatal("Main", "Medium aluminium not found");
  
  FairGeoMedium* mVac = geoMedia->getMedium("vacuum");
  if ( ! mVac ) Fatal("Main","FairMedium vacuum not found");
  geoBuild->createMedium(mVac);
  TGeoMedium* pMedVac = gGeoMan->GetMedium("vacuum");
  if ( ! pMedVac ) Fatal("Main", "Medium vacuum not found");

  // Create the top-volume:
  gGeoMan = (TGeoManager*)gROOT->FindObject("FAIRGeom");
  gGeoMan->SetName("NEULANDgeom");
  TGeoVolume* top = new TGeoVolumeAssembly("TOP");
  gGeoMan->SetTopVolume(top);
  
  // Define the output filename-string:vacuum
  TString geoFileName = "../../Data/Geometry/Chamber.geo.root";

  // ======================================================================================
  // Now we can build our actual geometry:
  
  // Build the basic rotation & translation blocks we need:
  TGeoRotation* zeroRotation = new TGeoRotation("zeroRotation");
  zeroRotation->RotateX(0.0);
  zeroRotation->RotateY(0.0);
  zeroRotation->RotateZ(0.0);
  zeroRotation->RegisterYourself();

  Double_t x_pos = 0.0;
  Double_t y_pos = 0.0;
  Double_t z_pos = 0.0;
  
  // Build the vacuum chamber basic outline:
  TGeoVolume* box1 = gGeoManager->MakeBox("box1",pMedAl,532.38/2.0,171.6/2.0,260.32/2.0); // overall vac. chamber box.
  
  // Build the cutoff-boxes:
  TGeoVolume* box2 = gGeoManager->MakeBox("box2",pMedAl,191.9/2.0,171.6/2.0,191.9/2.0); // Neutron window cutoff.
  TGeoVolume* box3 = gGeoManager->MakeBox("box3",pMedAl,395.5/2.0,171.6/2.0,395.5/2.0); // proton window cutoff.
  TGeoVolume* box4 = gGeoManager->MakeBox("box4",pMedAl,111.9/2.0,171.6/2.0,111.9/2.0); // third cutoff.
  
  // Rotate them & add them to the subtraction box:

  // transform box 2:  
  TGeoRotation* rot2 = new TGeoRotation("rot2");
  rot2->RotateX(0.0);
  rot2->RotateY(0.0);
  rot2->RotateZ(14.0);
  TGeoTranslation* trans2 = new TGeoTranslation("trans2",161.36,0.0,226.11);
  trans2->RegisterYourself();
  rot2->RegisterYourself();
  
  // Now subtract these boxes from the original box:
  TGeoCompositeShape* BasicChamber_shape = new TGeoCompositeShape("BasicChamber_shape","box1-(box2:rot2):trans2");
  TGeoVolume* BasicChamber = new TGeoVolume("BasicChamber",BasicChamber_shape,pMedAl);
  
  // Build the full assembly:
  TGeoVolume* Full = new TGeoVolumeAssembly("FULL");
  Full->AddNode(BasicChamber,1,new TGeoCombiTrans(x_pos,y_pos,z_pos,zeroRotation));
  
  // Create global transformations & add the assembly to the top:
  TGeoRotation* G_Rotation = new TGeoRotation();
  G_Rotation->RotateX(Rotation_X);
  G_Rotation->RotateY(Rotation_Y);
  G_Rotation->RotateZ(Rotation_Z);

  top->AddNode(Full,1,new TGeoCombiTrans(X_position,Y_position,Z_position,G_Rotation));
  
  /*
   * MakeCtub (const char *name, TGeoMedium *medium, Double_t rmin, Double_t rmax, Double_t dz, Double_t phi1, Double_t phi2, Double_t lx, Double_t ly, Double_t lz, Double_t tx, Double_t ty, Double_t tz)
   */

  // ======================================================================================
  // Finish our geometry & Write to a file:
  gGeoMan->CloseGeometry();
  gGeoMan->CheckOverlaps(0.001);
  gGeoMan->PrintOverlaps();
  gGeoMan->Test();
  TFile* geoFile = new TFile(geoFileName, "RECREATE");
  top->Write();
  geoFile->Close();

  // Done!
}

// Definition of the Global Positioning function needed by each geometry maro:
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

