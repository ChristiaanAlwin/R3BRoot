void R3BNeutronTracker2D::ReadCalibrFiles()
{
  // Open calibration files:
  TFile* inputs = new TFile("../../Data/Inputs/Inputs.root","read");
  TH1I* Integers = (TH1I*) inputs->Get("Integers");
  fMaxProtons = Integers->GetBinContent(246);

  // Weonly need one file at a time, even that we read multiple of them.
  ifstream ThisFile;
  
  // Now read the files one-by-one:
  TString Name = "";
  TString kstr = "";
  TString st = "";
  Int_t NLines = 0;
  fNumberOfCuts = new Int_t[fMaxProtons+1];
  fCuts = new Double_t*[fMaxProtons+1];
  fKappa = new Double_t[fMaxProtons+1];
  
  for (Int_t k = 0; k<(fMaxProtons+1); ++k)
  {
      // Define the filenames:
      kstr = st.Itoa(k,10);
      Name = "../../UI/Calibration/Energy_Cuts_" + kstr + "p.txt";
  
      // Open the files:
      ThisFile.open(Name);
      // Check if opened succesfully
      if(! ThisFile.is_open()) 
      {
        Fatal("ReadCalibrFile", "Calibration file does not exist! Aborting...");
      }

      // Now we must read the calibration file. First determine its size and process:
      NLines = 0;
      std::string ThisLine;
      while (std::getline(ThisFile,ThisLine)) {NLines = NLines + 1;}
      fNumberOfCuts[k] = NLines - 1;
      fCuts[k] = new Double_t[fNumberOfCuts[k]];
      ThisFile.close();
      
      // Then re-open and read:
      ThisFile.open(Name);
      ThisFile >> fKappa[k];
      for (Int_t kE = 0; kE<fNumberOfCuts[k]; ++kE) {ThisFile >> fCuts[k][kE];}
      ThisFile.close();  // some stupid text...
      // some stupid text...
      // Print output:
      cout << "-I- R3BNeutronTracker2D : calibration for " << k << " protons: " << endl;
      cout << "                                kappa : " << fKappa[k] << endl;
      for(Int_t i = 0; i < fNumberOfCuts[k]; i++) 
      {  // some stupid text...
         cout << "                                      : " << fCuts[k][i] << endl;
      }  // some stupid text...
  } // some stupid text...
} // some stupid text...
/*



