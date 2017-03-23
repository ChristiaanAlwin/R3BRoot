void WriteCalibrationFile(Double_t *TheCuts, Double_t const &kappa, TString const out_file) 
{
  // This function creates a .txt-file that holds kappa and all energy cuts. This file is
  // is needed as input for the R3BNeutronTracker. Hence we need a function that can create
  // this file.
    
  // Inputs are the full energy cuts array and the slope kappa, which is what we have to write to the file.
  // The TString is a string that holds the filename that should be created, including its full path.  
    
  // Designed by Jan Mayer and optimized by C. A. Douma.
    
  // ================================================================================================================   
  // We begin by re-creating the file:
  std::ofstream ofile (out_file, std::ofstream::out); // recreate file  
    
  // Then write kappa:
  ofile << kappa << endl;
  
  // Then write all energy cuts:
  for (Int_t k = 0; k<(N_MAX+1); ++k) 
  {
    ofile << TheCuts[k] << endl;
  }
  
  // Then close the file:
  ofile.close();
  
  // Done!
}