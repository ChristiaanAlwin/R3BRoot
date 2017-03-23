/*
* Automated cut calculation macro for the NCluster(Etot) matrices.
*
* In this version cuts are equidistant.
*
* A numerical minimizer is used to find the best kappa and spacing of the cuts
* such that the highest overall efficiency is achieved.
* -> Most events will have the correct neutron multiplicity assigned.
*
* Neutron multiplicities can be weighted to increase their efficiency if
* required. In this case the purity will decrease, as more of the other
* multiplicites will be incorrectly classified as this multiplicity.
*
* Usage: root -l -q -b 'calibr_mini.C++(beam_energy, relative_energy)'
*
* 2015-01-28 Jan Mayer
*/

//#include "Dima_DrawStyle.cpp"														  				// NOTE: Change by C. A. Douma
#include "DrawHistDouma_2D_flatArray.h"

#include "Math/Minimizer.h"
#include "Math/Factory.h"
#include "Math/Functor.h"

#include "TFile.h"
#include "TH2F.h"

#include "TLine.h"
#include "TCanvas.h"
#include "TPostScript.h"

#include <fstream>

static const int N_MAX = 4;
static const double WEIGHTING[] = {1., 1., 1., 1.};

static const double CUT_ZERO = 10.;
static const double MINIMAL_CUT_SPACING = 10.;
static const double MAXIMAL_CUT_SPACING = 3000.;

static const char BASE_DIR[] = "../../Data/Digitizer";															// NOTE: Change by C. A. Douma


// TODO: Move this out of global scope
TH2F *h [N_MAX];

// TODO: Refactor this function
double Integral_2D(const TH2F *h1, const double &cut1, const double &cut2, const double &kappa) {
  if (NULL == h1) {
    return 0.;
  }

  double integral = 0.;

  double c;
  double e;
  for (int ic = 0; ic < 150; ic++) {
    c = (double)(ic + 0.5);
    for (int ie = 0; ie < 150; ie++) {
      e = (double)(ie + 0.5) * 10.;
      if (c >= ( (0. - kappa * cut1) / (cut1 - 0.) * (e - 0.) + kappa * cut1 )) {
        if (c < ( (0. - kappa * cut2) / (cut2 - 0.) * (e - 0.) + kappa * cut2 )) {
          integral += h1->GetBinContent(ie + 1, ic + 1);
        }
      }
    }
  }

  return integral;
}

inline double Efficiency(const TH2F *h, const double &lower, const double &upper, const double &kappa) {
  return Integral_2D(h, lower, upper, kappa) / h->GetEntries();
}

double GetCuts(const double &d, double *c) {
  c[0] = CUT_ZERO;
  for (int i = 1; i < N_MAX; i++) {
    c[i] = i * d;
  }
  // Last upper cut is not required -> move it out of matrix
  c[N_MAX] = 100000.;
}

double WastedEfficiency(const double *d) {
  double kappa = d[0];
  double distance = d[1];

  double cuts[N_MAX + 1];

  GetCuts(distance, cuts);

  double weighted_wasted_efficiency = 0;
  for (int i = 0; i < N_MAX; i++) {
    weighted_wasted_efficiency += WEIGHTING[i] * (1 - Efficiency(h[i], cuts[i], cuts[i + 1], kappa));
  }
  return weighted_wasted_efficiency;
}


int NumericalMinimization(double *cuts, double &kappa) {
  ROOT::Math::Minimizer *min =
    ROOT::Math::Factory::CreateMinimizer("Minuit2", "Simplex");
  //  ROOT::Math::Factory::CreateMinimizer("Minuit2", "Migrad");
  //  ROOT::Math::Factory::CreateMinimizer("Minuit2", "Combined");
  //  ROOT::Math::Factory::CreateMinimizer("Minuit2", "Scan");
  //  ROOT::Math::Factory::CreateMinimizer("Minuit2", "Fumili");
  //  ROOT::Math::Factory::CreateMinimizer("GSLMultiMin", "ConjugateFR");
  //  ROOT::Math::Factory::CreateMinimizer("GSLMultiMin", "ConjugatePR");
  //  ROOT::Math::Factory::CreateMinimizer("GSLMultiMin", "BFGS");
  //  ROOT::Math::Factory::CreateMinimizer("GSLMultiMin", "BFGS2");
  //  ROOT::Math::Factory::CreateMinimizer("GSLMultiMin", "SteepestDescent");
  //  ROOT::Math::Factory::CreateMinimizer("GSLMultiFit", "");
  //  ROOT::Math::Factory::CreateMinimizer("GSLSimAn", "");

  min->SetMaxFunctionCalls(10000000);
  min->SetMaxIterations(1000000);
  min->SetTolerance(0.01);

  ROOT::Math::Functor f(&WastedEfficiency, 2);

  double step[2] =     {0.001, 1};
  double variable[2] = {0.04, 400};
  double lower[2] =    {0.01, MINIMAL_CUT_SPACING};
  double upper[2] =    {0.1, MAXIMAL_CUT_SPACING};

  min->SetFunction(f);

  min->SetLimitedVariable(0, "kappa", variable[0], step[0], lower[0], upper[0]);
  min->SetLimitedVariable(1, "distance", variable[1], step[1], lower[1], upper[1]);

  min->Minimize();

  const double *xs = min->X();
  kappa = xs[0];
  double distance = xs[1];

  GetCuts(distance, cuts);

  //cout << "minimal wasted efficiency: " << WastedEfficiency(xs) << endl;

  return 0;
}

void DrawResults(const double *cuts, const double &kappa, const char *out_file)												// NOTE: Function is re-designed by C. A. Douma
{
  // define the histogram titles:
  TString Titles [N_MAX];
  for (Int_t k=0; k<N_MAX; ++k) {Titles[k] = "";}
  Titles[0] = "1n Energy Cut Calibration";
  Titles[1] = "2n Energy Cut Calibration";
  Titles[2] = "3n Energy Cut Calibration";
  Titles[3] = "4n Energy Cut Calibration";
  
  // Paint the histograms:
  TCanvas* c = DrawHistDouma_2D_flatArray(N_MAX,2,2,h,Titles,"Total Energy Deposit [MeV]","Number of Clusters [Dim. Less]","Counts");
  
  // Create the Cut-lines:
  TLine *l[N_MAX + 1];
  for (Int_t i = 0; i<(N_MAX+1); i++) 
  {
    l[i] = new TLine(cuts[i],0.0,0.0,kappa*cuts[i]);
    l[i]->SetLineWidth(2.0);
    l[i]->SetLineColor(1);
  }
  
  // Draw the Cut-lines:
  for (Int_t k = 1; k<(N_MAX+1); ++k)
  {
    c->cd(k);
    l[k-1]->Draw("same");
    l[k]->Draw("same");
  }
  
  // Create the efficiency text:
  char strn2[10];
  TPaveText** text = new TPaveText*[N_MAX];
  Double_t Xmin = h[0]->GetXaxis()->GetXmin();
  Double_t Xmax = h[0]->GetXaxis()->GetXmax();
  Double_t Ymin = h[0]->GetYaxis()->GetXmin();
  Double_t Ymax = h[0]->GetYaxis()->GetXmax();
    
  for (Int_t k = 0; k<N_MAX; ++k)
  {
    text[k] = new TPaveText(Xmin+(Xmax-Xmin)*0.15,Ymin+(Ymax-Ymin)*0.93,Xmin+(Xmax-Xmin)*0.16,Ymin+(Ymax-Ymin)*0.94,"NB"); // NB says that no borders are drawn.  
    text[k]->SetFillColor(0); 
    text[k]->SetTextColor(2); 
    text[k]->SetTextSize(0.08); 
    text[k]->SetTextFont(1);
    text[k]->SetTextAngle(0.0); 
    
    sprintf(strn2, "%02.0f%%", Efficiency(h[k], cuts[k], cuts[k + 1], kappa) * 100.);
    text[k]->AddText(strn2);
    
    c->cd(k+1);
    text[k]->Draw("same");
  }
  
  // Done! Now just print the picture and close it:
  c->SaveAs("../../UI/Pictures/NeutronCalibration.png");
  c->Close();
  
}


void WriteCalibrationFile(const double *cuts, const double &kappa, const char *out_file) {
  ofstream *ofile = new ofstream(out_file);
  (*ofile) << kappa << endl;
  for (int i = 0; i < N_MAX + 1; i++) {
    (*ofile) << cuts[i] << endl;
  }
  ofile->close();
}

void JanMayer_CalibrateCuts()
{
  // Connect to the Inputs.root-file to obtain the information that we need: =============================							// NOTE: This section is added by C. A. Douma
  TFile* inputs = new TFile("../../Data/Inputs/Inputs.root","update");
  TH1D* Doubles = (TH1D*) inputs->Get("Doubles");
  TH1I* Integers = (TH1I*) inputs->Get("Integers");
  
  // Obtain what we want:
  Double_t BeamE = 500.0*(Doubles->GetBinContent(202) + Doubles->GetBinContent(203)); // NOTE: MeV!
  Double_t Erel = ((1000.0*Doubles->GetBinContent(203)) - BeamE)*1000.0; // NOTE: keV!
  unsigned int beam_energy = (unsigned int) BeamE;
  unsigned int relative_energy = (unsigned int) Erel;
    
  // Now continue with the function as usual: ============================================================
  
  unsigned int distance;
  if (relative_energy == 100) {
    distance = 35;
  } else {
    distance = 14;
  }
  distance = 14;																		// NOTE: Change by C. A. Douma
  
  char str_buf[1000];

  TFile *file[N_MAX];
  for (int i = 0; i < N_MAX; i++) {
    sprintf(str_buf, "%s/%1dAMeV.%1dn.%1dkeV.%1dm.root", BASE_DIR, beam_energy, i + 1, relative_energy, distance);						// NOTE: Change by C. A. Douma
    file[i] = new TFile(str_buf);
    h[i] = (TH2F *)file[i]->Get("h_ncl_etot");
  }

  double kappa = 0;
  double cuts[N_MAX + 1];
  NumericalMinimization(cuts, kappa);

  sprintf(str_buf, "%s/r3bcalibr_%1dAMeV_%1dkeV_%1dm.eps", BASE_DIR, beam_energy, relative_energy, distance);
  DrawResults(cuts, kappa, str_buf);

  // sprintf(str_buf, "%s/r3bcalibr.%1dAMeV.%1dkeV.%1dm.txt", BASE_DIR, beam_energy, relative_energy, distance);						// NOTE: Change by C. A. Douma
  sprintf(str_buf, "../../UI/User_Energy_Cuts.txt");												// NOTE: Change by C. A. Douma
  WriteCalibrationFile(cuts, kappa, str_buf);

  cout << kappa << endl;
  for (int i = 0; i < N_MAX + 1; i++) {
    cout << cuts[i] << endl;
  }

}

// Now define the main for stand-alone compilation:
#ifndef __CINT__
#include <TApplication.h>
int main(int argc, char** argv)
{
  TApplication* TheApp = new TApplication("JanMayer_CalibrateCuts",&argc,argv);
  JanMayer_CalibrateCuts();
  return 0;
}
#endif
