Double_t Integral_2D(TH2F* const Histogram, Double_t const E_cut_low, Double_t const E_cut_high, Double_t const kappa) 
{
  // This function computes the integral of a TH2F-histogram between two of the calibration cuts. The lines are
  // defined by the intersection with the x-axis (the energies) and their slope kappa. Note that all lines should
  // have the same kappa.
    
  // inputs are Histogram, the histogram to integrate and 3 Double_t, the x-axis intersection (Energy in MeV) and the
  // slope kappa. output is a Double_t: the integral value.
    
  // Designed by Jan Mayer and optimized by C. A. Douma.
    
  // ================================================================================================================    
  // We begin with defining the integral (the output variable to return).
  Double_t Integral = 0.0;
  
  // Then test if the histogram exists. If it does, we can perform the integral. Else we simply return zero.
  if (Histogram != NULL)
  {
      // Now we can do our integral. Notice that the cuts are Double_t values on the Energy [MeV]. The kappa
      // is the (usually negative) slope of the lines that define the cuts.
      
      // We begin with obtaining the boundaries from our histogram:
      Int_t NbinsE = Histogram->GetNbinsX();
      Double_t Emin = Histogram->GetXaxis()->GetXmin();
      Double_t Emax = Histogram->GetXaxis()->GetXmax();
      
      Int_t NbinsCluster = Histogram->GetNbinsY();
      Double_t ClusterStart = Histogram->GetYaxis()->GetXmin();
      Double_t ClusterEnd = Histogram->GetYaxis()->GetXmax();
      
      // Now define our bincenters:
      Double_t ClusterCenter = 0.0;
      Double_t EnergyCenter = 0.0;
      
      // Define other variables we need:
      Double_t ylow = 0.0;
      Double_t yhigh = 0.0;
      Double_t bincontent = 0.0;
      
      // Then now we can start looping over the Bins of the histogram.
      for (Int_t kc = 0; kc<(NbinsCluster+2); ++kc) // NOTE: include underflow and overflow bins!
      {
          for (Int_t kE = 0; kE<(NbinsE+2); ++kE) // NOTE: include underflow and overflow bins!
          {
              // Now define the bincenters:
              ClusterCenter = ClusterStart + (ClusterEnd - ClusterStart)*((((Int_t) kc) - 0.5)/((Int_t) NbinsCluster));
              EnergyCenter = Emin + (Emax - Emin)*((((Int_t) kE) - 0.5)/((Int_t) NbinsE));
              
              // Then now we have to decide if this point is between the cuts yes/no. The lines are defined as
              // y = kappa*(Ecut-x) where kappa>0. So now compute Yhigh and ylow corresponding to EnergyCenter
              // and then see if ClusterCenteris between them yes/no.
              ylow = kappa*(E_cut_low - EnergyCenter);
              yhigh = kappa*(E_cut_high - EnergyCenter); // since E_cut_high > E_cut_low and kappa > 0 we have yhigh > ylow.
              
              if ((ClusterCenter>ylow)&&(ClusterCenter<yhigh))
              {
                  bincontent = Histogram->GetBinContent(kE,kc);
                  Integral = Integral + bincontent;
              }
              
              // So that should do it!
          }
      }
  }
  
  // Done, now return our integral (zero if histogram does not exists):
    return Integral;
}