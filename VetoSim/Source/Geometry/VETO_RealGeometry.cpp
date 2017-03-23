Int_t gPaddlesPerPlane = 50;

Double_t gPaddleDistance_plane = 2.5;
Double_t gPaddleDistance_thick = 2.5;
Double_t gPaddleBaseLength = 125.0;
Double_t gPaddleConeLength = 5.;

Double_t gBC408ConeRadius = 1.2;
Double_t gBC408Thickness_width = 2.4;
Double_t gBC408Thickness_thick = 2.4;
Double_t gAlThickness = 0.02;
Double_t gTapeThickness = 0.05;


TGeoMedium *BuildMaterial(const TString material, FairGeoMedia *geoMedia, FairGeoBuilder *geoBuild)
{
    FairGeoMedium *fairMedium = geoMedia->getMedium(material);
    if (!fairMedium) {
        std::cout << "FairGeoMedium " << material << " not found" << std::endl;
    }

    geoBuild->createMedium(fairMedium);
    TGeoMedium *med = gGeoManager->GetMedium(material);
    if (!med) {
        std::cout << "TGeoMedium " << material << " not found" << std::endl;
    }
    return med;
}


/** Building the shape of a paddle
    The paddle end cones are created by intersection ("*") of a small box (width*width*coneLength) with a cone
    The intersected structures are moved, rotated and then to a base box (length*width*width) */
TGeoShape *BuildPaddleShape(const TString &name, const Double_t length, const Double_t width, const Double_t thick, const Double_t coneRadius, const Double_t coneLength)
{
    new TGeoBBox(name + "Box", length, width, thick);													// NOTE: Change by C. A. Douma!
    new TGeoCone(name + "Cone", coneLength + 0.001, 0., coneRadius, 0., TMath::Sqrt(width*width + thick*thick));					// NOTE: Change by C. A. Douma!
    new TGeoBBox(name + "Conebox", thick, width, coneLength);												// NOTE: Change by C. A. Douma!
    TGeoShape *shape = new TGeoCompositeShape(
        name,
        name + "Box + ((" + name + "Conebox*" + name + "Cone):trc1) + ((" + name + "Conebox*" + name + "Cone):trc2)"
    );
    return shape;
}

/** A paddle consists of the scintillator volume, wrapped with Al and Tape.
    The shapes for the wrappings are created by subtracting the solid inner part from a larger solid volume */
TGeoVolume *BuildPaddleVolume()
{
    // Load Interfaces to build materials
    FairGeoLoader    *geoLoad = FairGeoLoader::Instance();
    FairGeoInterface *geoFace = geoLoad->getGeoInterface();
    geoFace->setMediaFile(TString(gSystem->Getenv("VMCWORKDIR")) + "/geometry/media_r3b.geo");
    geoFace->readMedia();
    FairGeoBuilder  *geoBuild = geoLoad->getGeoBuilder();
    FairGeoMedia    *geoMedia = geoFace->getMedia();
    const TGeoMedium *medBC408 = BuildMaterial("BC408", geoMedia, geoBuild);
    const TGeoMedium *medCH2   = BuildMaterial("CH2", geoMedia, geoBuild);
    const TGeoMedium *medAl    = BuildMaterial("aluminium", geoMedia, geoBuild);

    // Prepare Transformations for cones
    TGeoRotation *r1 = new TGeoRotation();
    r1->RotateY(90);
    TGeoCombiTrans *trc1 = new TGeoCombiTrans(TGeoTranslation(-(gPaddleBaseLength + gPaddleConeLength), 0., 0.), *r1);
    trc1->SetName("trc1");
    trc1->RegisterYourself();

    TGeoRotation *r2 = new TGeoRotation();
    r2->RotateY(-90);
    TGeoCombiTrans *trc2 = new TGeoCombiTrans(TGeoTranslation(+(gPaddleBaseLength + gPaddleConeLength), 0., 0.), *r2);
    trc2->SetName("trc2");
    trc2->RegisterYourself();

    // Build shapes
    const TGeoShape *shapeBC408 = BuildPaddleShape("shapeBC408",
                                  gPaddleBaseLength,
                                  gBC408Thickness_width,												// NOTE: Change by C. A. Douma!
				  gBC408Thickness_thick,												// NOTE: Change by C. A. Douma!
                                  gBC408ConeRadius,
                                  gPaddleConeLength);

    BuildPaddleShape("shapeAlWrappingSolid",
                     gPaddleBaseLength,
                     gBC408Thickness_width + gAlThickness,												// NOTE: Change by C. A. Douma!
		     gBC408Thickness_thick + gAlThickness,												// NOTE: Change by C. A. Douma!
                     gBC408ConeRadius +  gAlThickness,
                     gPaddleConeLength);
    const TGeoShape *shapeAlWrapping = new TGeoCompositeShape("shapeAlWrapping", "shapeAlWrappingSolid - shapeBC408");

    BuildPaddleShape("shapeTapeWrappingSolid",
                     gPaddleBaseLength,
                     gBC408Thickness_width + gAlThickness + gTapeThickness,										// NOTE: Change by C. A. Douma!
		     gBC408Thickness_thick + gAlThickness + gTapeThickness,										// NOTE: Change by C. A. Douma!
                     gBC408ConeRadius +  gAlThickness + gTapeThickness,
                     gPaddleConeLength);
    const TGeoShape *shapeTapeWrapping = new TGeoCompositeShape("shapeTapeWrapping", "shapeTapeWrappingSolid - shapeAlWrappingSolid");

    // Build Volumes. Note that the volume names are important and reappear in R3BNeuland()
    TGeoVolume *volBC408 = new TGeoVolume("vollBC408_seg", shapeBC408, medBC408);
    volBC408->SetLineColor(33); //greyish plue
    volBC408->SetTransparency(30);
    TGeoVolume *volAlWrapping = new TGeoVolume("vollAlWrapping_seg", shapeAlWrapping, medAl);
    volAlWrapping->SetLineColor(17); //grey/silver
    TGeoVolume *volTapeWrapping = new TGeoVolume("vollTapeWrapping_seg", shapeTapeWrapping, medCH2);
    volTapeWrapping->SetLineColor(1); //black

    // Make the elementary assembly
    TGeoVolume *volPaddle = new TGeoVolumeAssembly("vollPaddle_seg");
    volPaddle->AddNode(volBC408, 1);
    volPaddle->AddNode(volAlWrapping, 1);
    volPaddle->AddNode(volTapeWrapping, 1);

    return volPaddle;
}


void VETO_RealGeometry()																// NOTE: Change by C. A. Douma!
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
  Double_t X_position = Doubles->GetBinContent(121);
  Double_t Y_position = Doubles->GetBinContent(122);
  Double_t Z_position = Doubles->GetBinContent(123);
  
  Double_t Rotation_X = Doubles->GetBinContent(124);
  Double_t Rotation_Y = Doubles->GetBinContent(125);
  Double_t Rotation_Z = Doubles->GetBinContent(126);
  
  Double_t Scintillator_Barlength = Doubles->GetBinContent(127);    // dimensions are including gabs, foil & wrapping tape!
  Double_t Scintillator_BarWidth = Doubles->GetBinContent(128);
  Double_t Scintillator_BarThickness = Doubles->GetBinContent(129);
  Double_t Gab_Size = Doubles->GetBinContent(130);
  Double_t Foil_Thickness = Doubles->GetBinContent(131);
  Double_t Tape_Thickness = Doubles->GetBinContent(132);
  
  Int_t N_Single_PLanes = Integers->GetBinContent(3);
  Int_t N_Bars_OnePlane = Integers->GetBinContent(4);
  
  // That's it.
  // ================================================================================

  // Process inputs:
  Int_t nPlanes = N_Single_PLanes; 
  
  TGeoRotation* GlobalRotation = new TGeoRotation();
  GlobalRotation->RotateX(Rotation_X);
  GlobalRotation->RotateY(Rotation_Y);
  GlobalRotation->RotateZ(Rotation_Z);
  
  TGeoCombiTrans* GlobalPos = new TGeoCombiTrans(X_position, Y_position, Z_position, GlobalRotation);
  
  gPaddlesPerPlane = N_Bars_OnePlane;
  gPaddleDistance_thick = 0.5*Scintillator_BarThickness;
  gPaddleDistance_plane = 0.5*Scintillator_BarWidth;
  gPaddleBaseLength = 0.5*Scintillator_Barlength;
  gAlThickness = Foil_Thickness;
  gTapeThickness = Tape_Thickness;
  gBC408Thickness_width = 0.5*(Scintillator_BarWidth - 2.0*Foil_Thickness - 2.0*Tape_Thickness - 2.0*Gab_Size);
  gBC408Thickness_thick = 0.5*(Scintillator_BarThickness - 2.0*Foil_Thickness - 2.0*Tape_Thickness - 2.0*Gab_Size);
  
  Double_t max = 0.0;
  Double_t min = 0.0;
  if (Scintillator_BarWidth>Scintillator_BarThickness)
  {
    max = Scintillator_BarWidth;
    min = Scintillator_BarThickness;
  }
  else
  {
    min = Scintillator_BarWidth;
    max = Scintillator_BarThickness;
  }
  gPaddleConeLength = 0.5*max;
  gBC408ConeRadius = 0.5*min;
  
  
    
  // Now carry out the function:
    new FairGeoLoader("TGeo", "FairGeoLoader");
    gGeoManager->SetName("NEULANDgeom");

    TGeoVolume *volPaddle = BuildPaddleVolume();
    TGeoRotation *rot90 = new TGeoRotation();
    rot90->RotateZ(90.);

    TGeoVolume *volNeuland = new TGeoVolumeAssembly("volNveto_seg");
    Int_t nindex = 0, nPlane = 0;
    for (Double_t b = -nPlanes * gPaddleDistance_thick + gPaddleDistance_thick; b < nPlanes * gPaddleDistance_thick; b += gPaddleDistance_thick * 2) {
        nPlane++;
        for (Double_t a = -gPaddlesPerPlane * gPaddleDistance_plane + gPaddleDistance_plane; a < gPaddleDistance_plane * gPaddlesPerPlane; a += gPaddleDistance_plane * 2) {
            nindex++;
            if (nPlane % 2 == 1) {
                volNeuland->AddNode(volPaddle, nindex, new TGeoTranslation(0, a, b));
            } else {
                volNeuland->AddNode(volPaddle, nindex, new TGeoCombiTrans(a, 0, b, rot90));
            }
        }
    }


    TGeoVolume *top = new TGeoVolumeAssembly("TOP");
    gGeoManager->SetTopVolume(top);
    top->AddNode(volNeuland, 1, GlobalPos);														// NOTE: Change by C. A. Douma!

    gGeoManager->CloseGeometry();
    gGeoManager->CheckOverlaps(0.001);
    gGeoManager->PrintOverlaps();
    gGeoManager->Test();

    TString geoFileName = "../../Data/Geometry/VETO_Real.geo.root";											// NOTE: Change by C. A. Douma!
    TFile *geoFile = new TFile(geoFileName, "RECREATE");
    top->Write();
    geoFile->Close();

    cout << "Done. " << nindex << " Paddles in " << nPlane << " Planes" << endl;
/*
    gStyle->SetCanvasPreferGL(kTRUE);
    gGeoManager->GetVolume("volNveto_seg")->Draw("ogl");
    TGLViewer *v = (TGLViewer *)gPad->GetViewer3D();
    v->CurrentCamera().RotateRad(0, 0.1);
    v->SetStyle(TGLRnrCtx::kOutline);
    v->RequestDraw();
    v->SavePicture("neuland_" + geoTag + ".png");
    */
}
