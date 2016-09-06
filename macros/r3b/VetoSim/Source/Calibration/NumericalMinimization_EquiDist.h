void NumericalMinimization_EquiDist(Double_t *TheCuts, Double_t &kappa, Int_t const Abeam, Int_t const Zbeam) 
{
  // This function performs the TMinuit ROOT minimization procedure.
    
  // Inputs are the full energy cuts array and the slop kappa. Actually they are not inputs, since this
  // function will not use their values. But they are passed on by reference, so that the answer to
  // the minimization procedure can be put in them. Hence actually theyare the outputs of the function.
    
  // Since the outputs are passed on by reference, this function is a void.
    
  // Designed by Jan Mayer and optimized by C. A. Douma.
    
  // ================================================================================================================   
  // Before we do anything, we will obtain some information that we need,
  // like the boundaries of the histograms:

  Int_t NbinsE = TheHists[0]->GetNbinsX();
  Double_t Emin = TheHists[0]->GetXaxis()->GetXmin();
  Double_t Emax = TheHists[0]->GetXaxis()->GetXmax();
      
  Int_t NbinsCluster = TheHists[0]->GetNbinsY();
  Double_t ClusterStart = TheHists[0]->GetYaxis()->GetXmin();
  Double_t ClusterEnd = TheHists[0]->GetYaxis()->GetXmax();
    
  // Then we declare the ROOT minimizer:  
  ROOT::Math::Minimizer* Minimizer = ROOT::Math::Factory::CreateMinimizer("Minuit2", "Simplex");

  // Then we set the properties of the Minimizer:
  Minimizer->SetMaxFunctionCalls(10000000);
  Minimizer->SetMaxIterations(1000000);
  Minimizer->SetTolerance(0.001);

  // Declare a ROOT functor. This functor contains the handle to the 
  // function that we want to minimize. This function is the WastedEfficiency function
  // that we defined earlier. Notice that it needs to have a single array as input in order 
  // to be used as a ROOT functor! We also need to give the amount of variables (2: kappa & distance)
  // as an argument. Hence the ROOT functor becomes:
  ROOT::Math::Functor f(&WastedEfficiency_EquiDist, 2);

  // Now we aso need to Define the limits of all variables. All first values are about kappa (the slope)
  // and all the second values are about distance [MeV] between the cuts.
  Double_t step[2] =     {0.001, 1.0};                                                   // step values for the Tinuit
  Double_t variable[2] = {0.03, (9.0/((Int_t) N_TOT))*((Emax-Emin)/(6500.0))*800.0};     // Starting values for TMinuit
  Double_t lower[2] =    {0.005, 2.0};                                                   // lower limits for TMinuit
  Double_t upper[2] =    {0.06,  (9.0/((Int_t) N_TOT))*((Emax-Emin)/(6500.0))*1200.0};   // Upper limits for TMinuit

  if (Abeam>0)
  {
      variable[1] = variable[1] + 500.0*((Int_t) N_PROT)*TMath::Abs(2.0 - ((Int_t) Abeam)/((Int_t) Zbeam))*TMath::Abs(2.0 - ((Int_t) Abeam)/((Int_t) Zbeam))*((Emax-Emin)/6500.0);
      lower[1] = variable[1] + 300.0*((Int_t) N_PROT)*TMath::Abs(2.0 - ((Int_t) Abeam)/((Int_t) Zbeam))*TMath::Abs(2.0 - ((Int_t) Abeam)/((Int_t) Zbeam))*((Emax-Emin)/6500.0);
      upper[1] = variable[1] + 700.0*((Int_t) N_PROT)*TMath::Abs(2.0 - ((Int_t) Abeam)/((Int_t) Zbeam))*TMath::Abs(2.0 - ((Int_t) Abeam)/((Int_t) Zbeam))*((Emax-Emin)/6500.0);
  }

  // Now link the functor to the minimizer:
  Minimizer->SetFunction(f);

  // Then pass the variable limits we defined to the Minimizer:
  Minimizer->SetLimitedVariable(0, "kappa", variable[0], step[0], lower[0], upper[0]);
  Minimizer->SetLimitedVariable(1, "distance", variable[1], step[1], lower[1], upper[1]);

  // Now everything is set. Execute the minimization process:
  Minimizer->Minimize();

  // Then extract the results:
  Double_t const *Results = Minimizer->X();
  kappa = Results[0];
  Double_t Distance = Results[1];
  GetCuts_EquiDist(Distance,TheCuts);

  // Done!
}