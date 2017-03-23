void DrawResults(Double_t const *TheCuts, Double_t const &kappa)
{
  // This function creates a nice picture of the results after the calibration cuts are properly
  // defined.
    
  // Inputs are the full energy cuts array and the slope kappa. The histograms are defined as global
  // variables. Hence then we have the cuts and the histograms, which is all we need to create a nice
  // picture. Hence this function is a void.
    
  // Designed by Jan Mayer and optimized by C. A. Douma.
    
  // ================================================================================================================   
  TFile* inputs = new TFile("../../Data/Inputs/Inputs.root","read");
  TH1D* Doubles = (TH1D*) inputs->Get("Doubles");
  TH1I* Integers = (TH1I*) inputs->Get("Integers");  
  
  Int_t CalNvert = Integers->GetBinContent(410);
  Int_t CalNhor = Integers->GetBinContent(411);
  
  // We begin by defining the strings of the histogram titles:
    
  TString* Titles = new TString[N_MAX];
  TString st = "";
  TString kstr = "";
  TString nstr = "";
  TString pstr = "";
  
  if (N_PROT>0)
  {
    for (Int_t kp = 0; kp<N_PROT; ++kp)
    {
      nstr = st.Itoa(0,10);
      pstr = st.Itoa(kp+1,10); // start at 1 proton!
      Titles[kp] = "Simulation of " + pstr + "p & " + nstr + "n.root";
    }
    
    for (Int_t kn = 0; kn<(N_MAX-N_PROT); ++kn)
    {
      nstr = st.Itoa(kn+1,10);
      pstr = st.Itoa(N_PROT,10); // start at 0 neutron!
      Titles[kn+N_PROT] = "Simulation of " + pstr + "p & " + nstr + "n.root";
    }
  }
  else
  {
    for (Int_t kn = 0; kn<N_MAX; ++kn)
    {
      nstr = st.Itoa(kn+1,10); // start at 1 neutron!
      pstr = st.Itoa(0,10);
      Titles[kn] = "Simulation of " + pstr + "p & " + nstr + "n.root";
    }
  }  
    
  // Once we have the titles, we can use our own histogram painting function
  // to create the canvas and paint the histograms.
  TCanvas* c = DrawHistDouma_2D_flatArray(N_MAX,CalNhor,CalNvert,TheHists,Titles,"Total Energy Deposit [MeV]","Number of Clusters [Dim. Less]","Counts");
  
  // In principle we are done now. However, the cuts and efficiencies still have to be painted on top of the histograms.
  
  // Hence we create TLines to paint the cuts:
  TLine** TheLines = new TLine*[N_MAX+1];
  for (Int_t k = 0; k<(N_MAX+1); k++) 
  {
    TheLines[k] = new TLine(TheCuts[k],0.0,0.0,kappa*TheCuts[k]);
    TheLines[k]->SetLineWidth(2.0);
    TheLines[k]->SetLineColor(1);
  }
  
  // Then we draw the relevant Cut-lines in the respective histograms:
  for (Int_t k = 1; k<(N_MAX+1); ++k)
  {
    c->cd(k);
    TheLines[k-1]->Draw("same");
    TheLines[k]->Draw("same");
  }
  
  // Then we create the efficieny texts. For that first obtain the histogram boundaries:
  Double_t Xmin = TheHists[0]->GetXaxis()->GetXmin();
  Double_t Xmax = TheHists[0]->GetXaxis()->GetXmax();
  Double_t Ymin = TheHists[0]->GetYaxis()->GetXmin();
  Double_t Ymax = TheHists[0]->GetYaxis()->GetXmax();
  
  // Then create a char array to print the text in:
  char strn2[10];
  
  // Create also an array of TPaveText:
  TPaveText** text = new TPaveText*[N_MAX];
    
  // Then fill the TPaveText array:
  for (Int_t k = 0; k<N_MAX; ++k)
  {
    // Create the object:
    text[k] = new TPaveText(Xmin+(Xmax-Xmin)*0.15,Ymin+(Ymax-Ymin)*0.93,Xmin+(Xmax-Xmin)*0.16,Ymin+(Ymax-Ymin)*0.94,"NB"); // NB says that no borders are drawn.  
    text[k]->SetFillColor(0); 
    text[k]->SetTextColor(2); 
    text[k]->SetTextSize(0.08); 
    text[k]->SetTextFont(1);
    text[k]->SetTextAngle(0.0); 
    
    // Create the text:
    sprintf(strn2, "%02.0f%%", Efficiency(TheHists[k],TheCuts[k],TheCuts[k+1],kappa)*100.0);
    text[k]->AddText(strn2);
    
    // Print the text:
    c->cd(k+1);
    text[k]->Draw("same");
  }
  
  // Done! Now just print the picture and close it:
  kstr = st.Itoa(N_PROT,10);
  c->SaveAs("../../UI/Calibration/NeutronCalibration_" + kstr + "p.png");
  c->Close();
  
  // Finished!
}