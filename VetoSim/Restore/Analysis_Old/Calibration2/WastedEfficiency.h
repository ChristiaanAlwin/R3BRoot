Double_t WastedEfficiency(Double_t const *Input) 
{
  // This function is the function that will be minimized by TMinuit to obtain the best calibration cuts.
  // Therefore it should have a single Double_t number as output and a single Double_t array as input.
  // The array should be pssed to this function in the way above to respect the TMinuit structure.
    
  // The input array Inputs consists of first the slope kappa and then the distances between the 
  // Energy cuts. The calibration cuts are NOT equidistant. Hence E(k) = E(k-1) + Distance(k-1);
    
  // The output is a Double_t. Given a calibration histogram of k neutrons, we compute the efficiency
  // between the k-th and (k+1)-th cut. Then 1-efficiency is what should be minimized. Then we simply
  // sum all (1-efficieny) of all histograms. This we call wasted efficiency. This sum should then be
  // as low as possible, so this is what we minimize.
    
  // Designed by Jan Mayer and optimized by C. A. Douma.
    
  // ================================================================================================================   
  // First we obtain our inputs:
  Double_t kappa = Input[0];
  Double_t* Distances = new Double_t[N_MAX-1];
  for (Int_t k = 1; k<N_MAX; ++k)
  {
      Distances[k-1] = Input[k];
  }
  
  // Then we compute the full array of our energy cuts:
  Double_t* TheCuts = new Double_t[N_MAX+1];
  GetCuts(Distances,TheCuts);

  // Then define the efficiencies we need to work with:
  Double_t Wasted_Efficiency = 0.0;
  Double_t This_Efficiency = 0.0;
  
  // Then loop over all histograms and sum up the wasted efficiencies (1-efficiency):
  
  for (Int_t k = 0; k < N_MAX; ++k) 
  {
      This_Efficiency = Efficiency(TheHists[k],TheCuts[k],TheCuts[k+1],kappa);
      Wasted_Efficiency = Wasted_Efficiency + (1.0 - This_Efficiency);
  }
  
  // Finally, return the answer:
  delete Distances;
  delete TheCuts;
  return Wasted_Efficiency;
}