void GetCuts_EquiDist(Double_t const Step, Double_t* TheCuts) 
{
  // The calibration cuts are equidistant. This means that the energy values of the cuts are 
  // given by E(k) = start + step * k. Since the first histogram should always be about 1 neutron,
  // the start should be equal to zero. Then the only real two parameters that we need to optimize
  // are kappa (the slope) and Step (the distance between the cuts). However, we also need a
  // function that fills in a complete array of energy cuts once Step is given. This is done
  // by this function. Then after this function is called, the energy cuts are available to do
  // optimiation, integration, etc.
    
  // Inputs are Double_t Step, the distance between the cuts, and a Double_t array of length N_MAX
  // This array is passed by reference to the function, so we can fill it here. Hence the function is a void.
    
  // Designed by Jan Mayer and optimized by C. A. Douma.
    
  // ================================================================================================================    
  // Wecan simply fill in the array with a for-loop:
    
  for (Int_t k = 0; k<N_MAX; ++k) 
  {
    TheCuts[k] = ((Int_t) k)*Step;
  }
  
  // Then since we do not need the last cut, we move it out of the histogram range:
  TheCuts[N_MAX] = 1000000.0;
  
  // Done!
}