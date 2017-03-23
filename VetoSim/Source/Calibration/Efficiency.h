Double_t Efficiency(TH2F* const Histogram, Double_t const E_cut_low, Double_t const E_cut_high, Double_t const kappa)
{
  // This function computes the percentage of how many histogram content a TH2F-histogram has between two of the 
  // calibration cuts. The lines are defined by the intersection with the x-axis (the energies) and their slope 
  // kappa. Note that all lines should have the same kappa.
    
  // inputs are Histogram, the histogram to integrate and 3 Double_t, the x-axis intersection (Energy in MeV) and the
  // slope kappa. output is a Double_t: the integral value.
    
  // Designed by Jan Mayer and optimized by C. A. Douma.
    
  // ================================================================================================================    
  // We begin by computing the integral between the cuts:
  Double_t Integral = Integral_2D(Histogram,E_cut_low,E_cut_high,kappa);
  
  // Then we also need the full integral. Declare it:
  Double_t Full_Integral = 0.0;
  
  // We begin with obtaining the boundaries from our histogram:
  Int_t NbinsE = Histogram->GetNbinsX();
  Double_t Emin = Histogram->GetXaxis()->GetXmin();
  Double_t Emax = Histogram->GetXaxis()->GetXmax();
      
  Int_t NbinsCluster = Histogram->GetNbinsY();
  Double_t ClusterStart = Histogram->GetYaxis()->GetXmin();
  Double_t ClusterEnd = Histogram->GetYaxis()->GetXmax();
     
  // Then now loop over the histogram:
  Double_t bincontent = 0.0;
  for (Int_t kc = 0; kc<(NbinsCluster+2); ++kc) // NOTE: include underflow and overflow bins!
  {
    for (Int_t kE = 0; kE<(NbinsE+2); ++kE) // NOTE: include underflow and overflow bins!
    {
      bincontent = Histogram->GetBinContent(kE,kc);
      Full_Integral = Full_Integral + bincontent;
    }
  }
  
  // And then we have the full histogram intergral.
  
  // Then we compute the efficiency:
  Double_t Eff = Integral/Full_Integral;
  
  // And we return the answer:
  return Eff;  
}