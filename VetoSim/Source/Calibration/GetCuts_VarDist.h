void GetCuts_VarDist(Double_t* Distances, Double_t* TheCuts) 
{
  // The calibration cuts are NOT equidistant. This means that the energy values of the cuts are 
  // given by E(k) = E(k-1) + Distance(k-1). Then the only real parameters that we need to optimize
  // are kappa (the slope) and the distances between the cuts. However, we also need a
  // function that fills in a complete array of energy cuts once the Distances are given. This is done
  // by this function. Then after this function is called, the energy cuts are available to do
  // optimiation, integration, etc.
    
  // Inputs are Double_t Step, the distance between the cuts, and a Double_t array of length N_MAX
  // This array is passed by reference to the function, so we can fill it here. Hence the function is a void.
    
  // Designed by Jan Mayer and optimized by C. A. Douma.
    
  // ================================================================================================================    
  // We can simply fill in the array with a for-loop:
    
  TheCuts[0] = 0.0;
    
  for (Int_t k = 1; k<N_MAX; ++k) 
  {
    TheCuts[k] = TheCuts[k-1] + Distances[k-1];
  }
  
  // Then since we do not need the last cut, we move it out of the histogram range:
  TheCuts[N_MAX] = 1000000.0;
  
  // Done!
}