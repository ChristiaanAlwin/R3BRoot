const Int_t gPaddlesPerPlane = 50;

const Double_t gPaddleDistance = 2.5;
const Double_t gPaddleBaseLength = 125.0;
const Double_t gPaddleConeLength = 5.;

const Double_t gBC408ConeRadius = 1.2;
const Double_t gBC408Thickness = 2.4;
const Double_t gAlThickness = 0.02;
const Double_t gTapeThickness = 0.05;


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
TGeoShape *BuildPaddleShape(const TString &name, const Double_t length, const Double_t width, const Double_t coneRadius, const Double_t coneLength)
{
    new TGeoBBox(name + "Box", length, width, width);
    new TGeoCone(name + "Cone", coneLength + 0.001, 0., coneRadius, 0., width * TMath::Sqrt(2.));
    new TGeoBBox(name + "Conebox", width, width, coneLength);
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
                                  gBC408Thickness,
                                  gBC408ConeRadius,
                                  gPaddleConeLength);

    BuildPaddleShape("shapeAlWrappingSolid",
                     gPaddleBaseLength,
                     gBC408Thickness + gAlThickness,
                     gBC408ConeRadius +  gAlThickness,
                     gPaddleConeLength);
    const TGeoShape *shapeAlWrapping = new TGeoCompositeShape("shapeAlWrapping", "shapeAlWrappingSolid - shapeBC408");

    BuildPaddleShape("shapeTapeWrappingSolid",
                     gPaddleBaseLength,
                     gBC408Thickness + gAlThickness + gTapeThickness,
                     gBC408ConeRadius +  gAlThickness + gTapeThickness,
                     gPaddleConeLength);
    const TGeoShape *shapeTapeWrapping = new TGeoCompositeShape("shapeTapeWrapping", "shapeTapeWrappingSolid - shapeAlWrappingSolid");

    // Build Volumes. Note that the volume names are important and reappear in R3BNeuland()
    TGeoVolume *volBC408 = new TGeoVolume("volBC408", shapeBC408, medBC408);
    volBC408->SetLineColor(33); //greyish plue
    volBC408->SetTransparency(30);
    TGeoVolume *volAlWrapping = new TGeoVolume("volAlWrapping", shapeAlWrapping, medAl);
    volAlWrapping->SetLineColor(17); //grey/silver
    TGeoVolume *volTapeWrapping = new TGeoVolume("volTapeWrapping", shapeTapeWrapping, medCH2);
    volTapeWrapping->SetLineColor(1); //black

    // Make the elementary assembly
    TGeoVolume *volPaddle = new TGeoVolumeAssembly("volPaddle");
    volPaddle->AddNode(volBC408, 1);
    volPaddle->AddNode(volAlWrapping, 1);
    volPaddle->AddNode(volTapeWrapping, 1);

    return volPaddle;
}


void NeuLAND_RealGeometry()																// NOTE: Change by C. A. Douma!
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
  Double_t X_position = Doubles->GetBinContent(111);
  Double_t Y_position = Doubles->GetBinContent(112);
  Double_t Z_position = Doubles->GetBinContent(113);
  
  Double_t Rotation_X = Doubles->GetBinContent(114);
  Double_t Rotation_Y = Doubles->GetBinContent(115);
  Double_t Rotation_Z = Doubles->GetBinContent(116);
  
  Int_t N_Single_PLanes = Integers->GetBinContent(2);
  
  // That's it.
  // ================================================================================

  // Process inputs:
  Int_t nPlanes = N_Single_PLanes; 
  
  TGeoRotation* GlobalRotation = new TGeoRotation();
  GlobalRotation->RotateX(Rotation_X);
  GlobalRotation->RotateY(Rotation_Y);
  GlobalRotation->RotateZ(Rotation_Z);
  
  TGeoCombiTrans* GlobalPos = new TGeoCombiTrans(X_position, Y_position, Z_position, GlobalRotation);
  
  // Now carry out the function:
    new FairGeoLoader("TGeo", "FairGeoLoader");
    gGeoManager->SetName("NEULANDgeom");

    TGeoVolume *volPaddle = BuildPaddleVolume();
    TGeoRotation *rot90 = new TGeoRotation();
    rot90->RotateZ(90.);

    TGeoVolume *volNeuland = new TGeoVolumeAssembly("volNeuland");
    Int_t nindex = 0, nPlane = 0;
    for (Double_t b = -nPlanes * gPaddleDistance + gPaddleDistance; b < nPlanes * gPaddleDistance; b += gPaddleDistance * 2) {
        nPlane++;
        for (Double_t a = -gPaddlesPerPlane * gPaddleDistance + gPaddleDistance; a < gPaddleDistance * gPaddlesPerPlane; a += gPaddleDistance * 2) {
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

    TString geoFileName = "../../Data/Geometry/NeuLAND_Real.geo.root";											// NOTE: Change by C. A. Douma!
    TFile *geoFile = new TFile(geoFileName, "RECREATE");
    top->Write();
    geoFile->Close();

    cout << "Done. " << nindex << " Paddles in " << nPlane << " Planes" << endl;
/*
    gStyle->SetCanvasPreferGL(kTRUE);
    gGeoManager->GetVolume("volNeuland")->Draw("ogl");
    TGLViewer *v = (TGLViewer *)gPad->GetViewer3D();
    v->CurrentCamera().RotateRad(0, 0.1);
    v->SetStyle(TGLRnrCtx::kOutline);
    v->RequestDraw();
    v->SavePicture("neuland_" + geoTag + ".png");
    */
}
