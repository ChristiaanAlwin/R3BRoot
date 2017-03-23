// Inclusion of C++ headers:
#include <iostream>
#include <fstream>

// inclusion of Math headers:
#include "Math/Minimizer.h"
#include "Math/Factory.h"
#include "Math/Functor.h"

// Inclusion of ROOT headers
#include "TFile.h"
#include "TH2F.h"
#include "TLine.h"
#include "TCanvas.h"
//#include "TPostScript.h"

// Inclusion of own headers:
#include "DrawHistDouma_2D_flatArray.h"

// Inluding the standard C++ variables:
using namespace std;

// Declaration of global variables. 
static const int N_MAX = 4;
TH2F* TheHists [N_MAX];

// NOTE: These variables HAVE to be defined as global because they are inputs to the function that is minimized
//       with TMinuit from ROOT. Just like fitting functions to histograms, this package requires a very special 
//       structure in input variables. Hence inputs that should stay const, etc should be put in as global
//       variables, not as inputs. This is just like the peak boundaries for random subtraction. If you want to
//       modify these inputs, then do it with sed.

// ==================================================================================================================
// NOTE: Here we start with our functions:

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

void GetCuts(Double_t const Step, Double_t* TheCuts) 
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


Double_t WastedEfficiency(Double_t const *Input) 
{
  // This function is the function that will be minimized by TMinuit to obtain the best calibration cuts.
  // Therefore it should have a single Double_t number as output and a single Double_t array as input.
  // The array should be pssed to this function in the way above to respect the TMinuit structure.
    
  // The input array Inputs consists of two numbers: the slope kappa and the distance Step between the 
  // Energy cuts. The calibration cuts are equidistant. This means that the energy values of the cuts are 
  // given by E(k) = start + Step * k. Since the first histogram should always be about 1 neutron,
  // the start should be equal to zero. Hence for the best cuts, only kappa (Inputs[0]) and Step (Inputs[1])
  // should be optimized. Hence these are the inputs of this function.
    
  // The output is a Double_t. Given a calibration histogram of k neutrons, we compute the efficiency
  // between the k-th and (k+1)-th cut. Then 1-efficiency is what should be minimized. Then we simply
  // sum all (1-efficieny) of all histograms. This we call wasted efficiency. This sum should then be
  // as low as possible, so this is what we minimize.
    
  // Designed by Jan Mayer and optimized by C. A. Douma.
    
  // ================================================================================================================   
  // First we obtain our inputs:
  Double_t kappa = Input[0];
  Double_t Step = Input[1];
  
  // Then we compute the full array of our energy cuts:
  Double_t* TheCuts = new Double_t[N_MAX+1];
  GetCuts(Step,TheCuts);

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
  return Wasted_Efficiency;
}

void NumericalMinimization(Double_t *TheCuts, Double_t &kappa) 
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
  Minimizer->SetTolerance(0.01);

  // Declare a ROOT functor. This functor contains the handle to the 
  // function that we want to minimize. This function is the WastedEfficiency function
  // that we defined earlier. Notice that it needs to have a single array as input in order 
  // to be used as a ROOT functor! We also need to give the amount of variables (2: kappa & distance)
  // as an argument. Hence the ROOT functor becomes:
  ROOT::Math::Functor f(&WastedEfficiency, 2);

  // Now we aso need to Define the limits of all variables. All first values are about kappa (the slope)
  // and all the second values are about distance [MeV] between the cuts.
  Double_t step[2] =     {0.001, 1.0};                              // step values for the Tinuit
  Double_t variable[2] = {0.04, ((Emax-Emin))/((Int_t) N_MAX)};     // Starting values for TMinuit
  Double_t lower[2] =    {0.01, 2.0};                               // lower limits for TMinuit
  Double_t upper[2] =    {0.15, (Emax-Emin)};                       // Upper limits for TMinuit

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
  GetCuts(Distance,TheCuts);

  // Done!
}


void DrawResults(Double_t const *TheCuts, Double_t const &kappa)
{
  // This function creates a nice picture of the results after the calibration cuts are properly
  // defined.
    
  // Inputs are the full energy cuts array and the slope kappa. The histograms are defined as global
  // variables. Hence then we have the cuts and the histograms, which is all we need to create a nice
  // picture. Hence this function is a void.
    
  // Designed by Jan Mayer and optimized by C. A. Douma.
    
  // ================================================================================================================   
    
  // We begin by defining the strings of the histogram titles:
  TString* Titles = new TString[N_MAX];
  TString st = "";
  TString k_str = "";
  for (Int_t k=0; k<N_MAX; ++k) 
  {
      k_str = st.Itoa(k,10);
      Titles[k] = k_str + "n Energy Cut Calibration";
  }
  
  // Once we have the titles, we can use our own histogram painting function
  // to create the canvas and paint the histograms.
  TCanvas* c = DrawHistDouma_2D_flatArray(N_MAX,3,3,TheHists,Titles,"Total Energy Deposit [MeV]","Number of Clusters [Dim. Less]","Counts");
  
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
  c->SaveAs("../../UI/Pictures/NeutronCalibration.png");
  c->Close();
  
  // Finished!
}


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

void CalibrateCuts()
{
  // This is the main function to obtain the calibration cuts. Therefore it has no inputs or outputs.
  // One can just call it and it will do everything by itself.
    
  // Designed by Jan Mayer and optimized by C. A. Douma.

  // ================================================================================================================
  // We begin by connecting to our Inputs.root-file:
  TFile* inputs = new TFile("../../Data/Inputs/Inputs.root","update");
  TH1D* Doubles = (TH1D*) inputs->Get("Doubles");
  TH1I* Integers = (TH1I*) inputs->Get("Integers");
  
  // Then we obtain the information that we need:
  Double_t BeamE = 500.0*(Doubles->GetBinContent(202) + Doubles->GetBinContent(203)); // NOTE: MeV!
  Double_t Erel = ((1000.0*Doubles->GetBinContent(203)) - BeamE)*1000.0; // NOTE: keV!
  Int_t beam_energy = (Int_t) BeamE;
  Int_t relative_energy = (Int_t) Erel;

  // NOTE: The Integers bin nr. 244 should be put in here by sed, not from this file. Otherwise
  // this method will not work. Hence we do not collect it here.
  
  // Now we have to connect to the output files of our calibration simulations. They
  // also hold the histograms that we need.
  
  // Define an array of TFiles:
  TFile** file = new TFile*[N_MAX];
  
  // Then use sprintf to correctly define the filenames. With that we can fill the TFile-array
  // and then obtain our histograms, which we will load into our global variables.
  char str_buf[1000];
  static const char BASE_DIR[] = "../../Data/Digitizer";
  Int_t distance = 14;
  for (Int_t k = 0; k<N_MAX; ++k) 
  {
    sprintf(str_buf,"%s/%1dAMeV.%1dn.%1dkeV.%1dm.root",BASE_DIR,beam_energy,k+1,relative_energy,distance);
    file[k] = new TFile(str_buf);
    TheHists[k] = (TH2F*) file[k]->Get("h_ncl_etot");
  }

  // Then now declare our array to hold the cuts and declare the slope kappa:
  Double_t kappa = 0.0;
  Double_t* TheCuts = new Double_t[N_MAX+1];
  
  // Then execute the minimization:
  NumericalMinimization(TheCuts,kappa);

  // Then draw our results:
  DrawResults(TheCuts,kappa);

  // Then, finally, write the calibration file so that the tracker can use it.
  sprintf(str_buf, "../../UI/User_Energy_Cuts.txt");
  TString out_file = (TString) str_buf;
  WriteCalibrationFile(TheCuts,kappa,out_file);

  // Done!
}

// Now define the main for stand-alone compilation:
#ifndef __CINT__
#include <TApplication.h>
int main(int argc, char** argv)
{
  TApplication* TheApp = new TApplication("CalibrateCuts",&argc,argv);
  CalibrateCuts();
  return 0;
}
#endif
