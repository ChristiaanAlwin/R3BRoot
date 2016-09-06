void NumericalMinimization_MaxEff(Double_t *TheCuts, Double_t &kappa, Int_t const Abeam, Int_t const Zbeam) 
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
  // to be used as a ROOT functor! We also need to give the amount of variables (N_MAX: kappa + distances).
  // as an argument. Hence the ROOT functor becomes:
  ROOT::Math::Functor f(&WastedEfficiency_MaxEff, N_MAX);

  // Now we aso need to Define the limits of all variables. All first values are about kappa (the slope)
  // and then the other values are about distances [MeV] between the cuts.
  Double_t step[N_MAX];     // step values for the Tinuit
  Double_t variable[N_MAX]; // Starting values for TMinuit
  Double_t lower[N_MAX];    // lower limits for TMinuit
  Double_t upper[N_MAX];    // Upper limits for TMinuit
  
  step[0] = 0.001;
  variable[0] = 0.03;
  lower[0] = 0.005;
  upper[0] = 0.06;
  
  for (Int_t k = 1; k<N_MAX; ++k)
  {
      step[k] = 1.0;
      variable[k] = (9.0/((Int_t) N_TOT))*((Emax-Emin)/(6500.0))*800.0;
      //lower[k] = ((Emax-Emin)/(6500.0))*600.0;
      lower[k] = 2.0;
      upper[k] = (9.0/((Int_t) N_TOT))*((Emax-Emin)/(6500.0))*1200.0;
      
      if (Abeam>0)
      {
          variable[k] = variable[k] + 500.0*((Int_t) N_PROT)*TMath::Abs(2.0 - ((Int_t) Abeam)/((Int_t) Zbeam))*TMath::Abs(2.0 - ((Int_t) Abeam)/((Int_t) Zbeam))*((Emax-Emin)/6500.0);
          lower[k] = variable[k] + 300.0*((Int_t) N_PROT)*TMath::Abs(2.0 - ((Int_t) Abeam)/((Int_t) Zbeam))*TMath::Abs(2.0 - ((Int_t) Abeam)/((Int_t) Zbeam))*((Emax-Emin)/6500.0);
          upper[k] = variable[k] + 700.0*((Int_t) N_PROT)*TMath::Abs(2.0 - ((Int_t) Abeam)/((Int_t) Zbeam))*TMath::Abs(2.0 - ((Int_t) Abeam)/((Int_t) Zbeam))*((Emax-Emin)/6500.0);
      }
      
  }

  // Now link the functor to the minimizer:
  Minimizer->SetFunction(f);

  // Then pass the variable limits we defined to the Minimizer:
  Minimizer->SetLimitedVariable(0, "kappa", variable[0], step[0], lower[0], upper[0]);
  
  TString st = "";
  TString k_str = "";
  for (Int_t k = 1; k<N_MAX; ++k)
  {
    k_str = st.Itoa(k,10);  
    k_str = "distance_"+k_str;
    Minimizer->SetLimitedVariable(k, k_str.Data(), variable[k], step[k], lower[k], upper[k]);
  }

  // Now everything is set. Execute the minimization process:
  Minimizer->Minimize();

  // Then extract the results:
  Double_t const *Results = Minimizer->X();
  kappa = Results[0];
  Double_t* Distances = new Double_t[N_MAX-1];
  for (Int_t k = 1; k<N_MAX; ++k)
  {
      Distances[k-1] = Results[k];
  }
  GetCuts_VarDist(Distances,TheCuts);

  // Done!
}