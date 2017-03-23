#include "GetNevents.h"
#include "DrawGraphDouma.h"

void DrawVETOperformance(Double_t const Ethreshold, Int_t const Nplot)
{
  // This function takes the VETO performance obtained from integrating the VETO energy
  // efficiency histograms and then plots the results against VETO distance. Inputs are
  // the energy threshold of the VETO you want to plot results for and Nplot, the number
  // of graphs you want to plot. It ranges from 1 to 10.

  // Written by C. A. Douma

  // ====================================================================================
  // We begin by requesting some inputs from the Inputs.root-file:

  // Connect to the file:
  TFile* file = new TFile("Inputs.root","read");
  
  // Load the data:
  TH1D* Doubles = file->Get("Doubles");
  TH1I* Integers = file->Get("Integers");
  
  // Now get our information:
  Int_t N_runs = Integers->GetBinContent(23);
  Double_t vetostep = Doubles->GetBinContent(26);
  Int_t Nevents_i = Integers->GetBinContent(26);
  Double_t Nevents = (Int_t) Nevents_i;
  
  // ====================================================================================
  // Now we create our arrays.
  
  // Create the size of the arrays:
  Int_t const N_i = N_runs;
  Int_t const N_ii = N_i;
  Int_t const N = N_ii;
  
  // Declare the arrays:
  Double_t TotalEvents [N];
  Double_t VetoDist [N];
  Double_t Naive_Veto [N];
  Double_t Back_Veto [N];
  Double_t Sec_Freq [N];
  Double_t Sec_Ev [N];
  Double_t MC_TOF_Veto [N];
  Double_t Rand_TOF_Veto [N];
  Double_t MC_TOF_Veto_prim [N];
  Double_t Rand_TOF_Veto_prim [N];
  Double_t Prim_Ev [N];
  Double_t Joined_Ev [N];

  // Fill in the x-quantity:
  for (Int_t k = 0; k<N; ++k)
  {
    VetoDist[k] = vetostep*((Int_t) k);
  }
  
  // ====================================================================================
  // Now we will loop over the simulation results:
  
  TFile* f = new TFile("Histograms.root");
  
  for (Int_t k = 0; k<N; ++k)
  {
    // Construct the energy thershold string:
    TString StupidStuff = "";
    Double_t DName1 = Ethreshold*1.0;
    Double_t DName2 = Ethreshold*100.0;
    Int_t IName1 = (Double_t) DName1; // Just cuts of at the "."; no roundof, just a cut. 24.93-->24
    Int_t IName2 = (Double_t) DName2;
    IName2 = IName2 - 100*IName1;
    TString SName1 = StupidStuff.Itoa(IName1,10);
    TString SName2 = StupidStuff.Itoa(IName2,10);
    TString Ename = SName1 + "." + SName2 + "[MeV]";

    // Construct the run-string:
    TString Srun = StupidStuff.Itoa(k,10);
  
    // Now we can define the correct names:
    TString SpectrumName = "Full_Spectrum_";
    SpectrumName = SpectrumName + Ename + "_" + Srun;
   
    TString VetoNameSec = "Veto_Sec_Probability_";
    VetoNameSec = VetoNameSec + Ename + "_" + Srun;
    TString VetoNamePass = "Veto_Pass_Probability_";
    VetoNamePass = VetoNamePass + Ename + "_" + Srun;
    TString VetoNameB = "Veto_Pure_Back_Probability_";
    VetoNameB = VetoNameB + Ename + "_" + Srun;
    TString VetoNameP = "Veto_Pure_Prim_Probability_";
    VetoNameP = VetoNameP + Ename + "_" + Srun;
    TString VetoNameBP = "Veto_Prim&Back_Probability_";
    VetoNameBP = VetoNameBP + Ename + "_" + Srun;
    TString VetoNameTOF = "Veto_Detection_Probability_TOF_MC_";
    VetoNameTOF = VetoNameTOF + Ename + "_" + Srun;
    TString VetoNameTOFR = "Veto_Detection_Probability_TOF100ps_";
    VetoNameTOFR = VetoNameTOFR + Ename + "_" + Srun;
    TString VetoNameTOFprim = "Veto_Detection_Probability_TOF_MC_prim_";
    VetoNameTOFprim = VetoNameTOFprim + Ename + "_" + Srun;
    TString VetoNameTOFRprim = "Veto_Detection_Probability_TOF100ps_prim_";
    VetoNameTOFRprim = VetoNameTOFRprim + Ename + "_" + Srun;
    TString Sec_freq_Name = "Secondary_frequency_";
    Sec_freq_Name = Sec_freq_Name + Ename + "_" + Srun;
  
    // Get the histograms:
    TH1D* spectrum = (TH1D*) f->Get(SpectrumName);
    TH1D* veto_hist_Pass = (TH1D*) f->Get(VetoNamePass);
    TH1D* veto_hist_PureBack = (TH1D*) f->Get(VetoNameB);
    TH1D* veto_hist_PurePrim = (TH1D*) f->Get(VetoNameP);
    TH1D* veto_hist_PrimBack = (TH1D*) f->Get(VetoNameBP);
    TH1D* veto_Sec_freq = (TH1D*) f->Get(Sec_freq_Name);
    TH1D* veto_hist_Sec = (TH1D*) f->Get(VetoNameSec);
    TH1D* veto_hist_TOF = (TH1D*) f->Get(VetoNameTOF);
    TH1D* veto_hist_TOF_Rand = (TH1D*) f->Get(VetoNameTOFR);
    TH1D* veto_hist_TOF_prim = (TH1D*) f->Get(VetoNameTOFprim);
    TH1D* veto_hist_TOF_Rand_prim = (TH1D*) f->Get(VetoNameTOFRprim);
 
    // Get their integrals:
    TotalEvents[k] = (Int_t) GetNevents(veto_hist_Pass,spectrum);
    Back_Veto[k] = (Int_t) GetNevents(veto_hist_PureBack,spectrum);
    Sec_Freq[k] = (Int_t) veto_Sec_freq->Integral();
    Sec_Ev[k] = (Int_t) GetNevents(veto_hist_Sec,spectrum);
    MC_TOF_Veto[k] = (Int_t) GetNevents(veto_hist_TOF,spectrum);
    Rand_TOF_Veto[k] = (Int_t) GetNevents(veto_hist_TOF_Rand,spectrum);
    MC_TOF_Veto_prim[k] = (Int_t) GetNevents(veto_hist_TOF_prim,spectrum);
    Rand_TOF_Veto_prim[k] = (Int_t) GetNevents(veto_hist_TOF_Rand_prim,spectrum);
    Prim_Ev[k] = (Int_t) GetNevents(veto_hist_PurePrim,spectrum);
    Joined_Ev[k] = (Int_t) GetNevents(veto_hist_PrimBack,spectrum);
    
    // Make events OK:
    Prim_Ev[k] = Prim_Ev[k] + Joined_Ev[k];
    Naive_Veto[k] = Prim_Ev[k] + Back_Veto[k];
    TotalEvents[k] = TotalEvents[k] + Naive_Veto[k];
  
    // Done!
  }
  
  // ====================================================================================
  // We also would like to print the content of the arrays:
  
  for (Int_t k = 0; k<N; ++k)
  {
    cout << "VETO results for run " << k << ":\n"
         << "Total number of events: " << TotalEvents[k] << "\n"
	 << "VETOed events for naive condition: " << Naive_Veto[k] << "\n"
	 << "Number of VETOed pure backscattered events: " << Back_Veto[k] << "\n"
	 << "Number of VETOed primary events: " << Prim_Ev[k] << "\n"
	 << "Number of events with both backscattering and primary interaction: " << Joined_Ev[k] << "\n"
	 << "Number of secondary particles: " << Sec_Freq[k] << "\n"
	 << "Number of detected events with secondary particles: " << Sec_Ev[k] << "\n"
	 << "VETOed events for a precise TOF condition: " << MC_TOF_Veto[k] << "\n"
	 << "This precise TOF condition contained " << MC_TOF_Veto_prim[k] << " primary events.\n"
	 << "VETOed events for a TOF condition with time resolution: " << Rand_TOF_Veto[k] << "\n"
	 << "This time resolution TOF condition contained " << Rand_TOF_Veto_prim[k] << " primary events.\n"
	 << "\n\n\n";
  }
  
  // =====================================================================================
  // Now we have the arrays requested. We will however also need arrays for the errors:
  
  // Declare the error arrays:
  Double_t TotalEvents_err [N];
  Double_t VetoDist_err [N];
  Double_t Naive_Veto_err [N];
  Double_t Back_Veto_err [N];
  Double_t Sec_Freq_err [N];
  Double_t Sec_Ev_err [N];
  Double_t MC_TOF_Veto_err [N];
  Double_t Rand_TOF_Veto_err [N];
  Double_t MC_TOF_Veto_prim_err [N];
  Double_t Rand_TOF_Veto_prim_err [N];
  Double_t Prim_Ev_err [N];
  Double_t Joined_Ev_err [N];

  // Fill them:
  for (Int_t k = 0; k<N; ++k)
  {
    VetoDist_err[k] = 0.0;
    TotalEvents_err[k] = 0.0;
    Naive_Veto_err[k] = TMath::Sqrt(Naive_Veto[k]);
    Back_Veto_err[k] = TMath::Sqrt(Back_Veto[k]);
    Sec_Freq_err[k] = TMath::Sqrt(Sec_Freq[k]);
    Sec_Ev_err[k] = TMath::Sqrt(Sec_Ev[k]);
    MC_TOF_Veto_err[k] = TMath::Sqrt(MC_TOF_Veto[k]);
    Rand_TOF_Veto_err[k] = TMath::Sqrt(Rand_TOF_Veto[k]);
    MC_TOF_Veto_prim_err[k] = TMath::Sqrt(MC_TOF_Veto_prim[k]);
    Rand_TOF_Veto_prim_err[k] = TMath::Sqrt(Rand_TOF_Veto_prim[k]);
    Prim_Ev_err[k] = TMath::Sqrt(Prim_Ev[k]);
    Joined_Ev_err[k] = TMath::Sqrt(Joined_Ev[k]);
  }
  
  // Now we have to normalize all y-arrays to 100% with respect to the total number of events:
  
  for (Int_t k = 0; k<N; ++k)
  {
    Naive_Veto[k] = (100.0/TotalEvents[k])*Naive_Veto[k];
    Back_Veto[k] = (100.0/TotalEvents[k])*Back_Veto[k];
    Sec_Freq[k] = (100.0/TotalEvents[k])*Sec_Freq[k];
    Sec_Ev[k] = (100.0/TotalEvents[k])*Sec_Ev[k];
    MC_TOF_Veto[k] = (100.0/TotalEvents[k])*MC_TOF_Veto[k];
    Rand_TOF_Veto[k] = (100.0/TotalEvents[k])*Rand_TOF_Veto[k];
    MC_TOF_Veto_prim[k] = (100.0/TotalEvents[k])*MC_TOF_Veto_prim[k];
    Rand_TOF_Veto_prim[k] = (100.0/TotalEvents[k])*Rand_TOF_Veto_prim[k];
    Prim_Ev[k] = (100.0/TotalEvents[k])*Prim_Ev[k];
    Joined_Ev[k] = (100.0/TotalEvents[k])*Joined_Ev[k];
    
    Naive_Veto_err[k] = (100.0/TotalEvents[k])*Naive_Veto_err[k];
    Back_Veto_err[k] = (100.0/TotalEvents[k])*Back_Veto_err[k];
    Sec_Freq_err[k] = (100.0/TotalEvents[k])*Sec_Freq_err[k];
    Sec_Ev_err[k] = (100.0/TotalEvents[k])*Sec_Ev_err[k];
    MC_TOF_Veto_err[k] = (100.0/TotalEvents[k])*MC_TOF_Veto_err[k];
    Rand_TOF_Veto_err[k] = (100.0/TotalEvents[k])*Rand_TOF_Veto_err[k];
    MC_TOF_Veto_prim_err[k] = (100.0/TotalEvents[k])*MC_TOF_Veto_prim_err[k];
    Rand_TOF_Veto_prim_err[k] = (100.0/TotalEvents[k])*Rand_TOF_Veto_prim_err[k];
    Prim_Ev_err[k] = (100.0/TotalEvents[k])*Prim_Ev_err[k];
    Joined_Ev_err[k] = (100.0/TotalEvents[k])*Joined_Ev_err[k];
  }
  
  // ======================================================================================
  // Now that we finally have all the arrays as they should be, we can now start to define
  // TGraphErrors with them and plot them.
  
  // Define the TGraphErrors:
  TGraphErrors* Naive = new TGraphErrors(N,&VetoDist[0],&Naive_Veto[0],&VetoDist_err[0],&Naive_Veto_err[0]);
  TGraphErrors* Back = new TGraphErrors(N,&VetoDist[0],&Back_Veto[0],&VetoDist_err[0],&Back_Veto_err[0]);
  TGraphErrors* Sec_F = new TGraphErrors(N,&VetoDist[0],&Sec_Freq[0],&VetoDist_err[0],&Sec_Freq_err[0]);
  TGraphErrors* Sec_E = new TGraphErrors(N,&VetoDist[0],&Sec_Ev[0],&VetoDist_err[0],&Sec_Ev_err[0]);
  TGraphErrors* MC_TOF = new TGraphErrors(N,&VetoDist[0],&MC_TOF_Veto[0],&VetoDist_err[0],&MC_TOF_Veto_err[0]);
  TGraphErrors* Rand_TOF = new TGraphErrors(N,&VetoDist[0],&Rand_TOF_Veto[0],&VetoDist_err[0],&Rand_TOF_Veto_err[0]);
  TGraphErrors* MC_TOF_prim = new TGraphErrors(N,&VetoDist[0],&MC_TOF_Veto_prim[0],&VetoDist_err[0],&MC_TOF_Veto_prim_err[0]);
  TGraphErrors* Rand_TOF_prim = new TGraphErrors(N,&VetoDist[0],&Rand_TOF_Veto_prim[0],&VetoDist_err[0],&Rand_TOF_Veto_prim_err[0]);
  TGraphErrors* Prim = new TGraphErrors(N,&VetoDist[0],&Prim_Ev[0],&VetoDist_err[0],&Prim_Ev_err[0]);
  TGraphErrors* Join = new TGraphErrors(N,&VetoDist[0],&Joined_Ev[0],&VetoDist_err[0],&Joined_Ev_err[0]);
  
  // Plot the first one (energy threshold condition):
  if (Nplot > 0) {TCanvas* c = DrawGraphDouma(Naive,"VETO performance versus VETO distance to NeuLAND","Distance between VETO and NeuLAND [cm]","Event fraction [%]",1);}

  // Now we change the colors and we plot the rest (in the right order!)
  
  if (Nplot > 1) {
  Back->UseCurrentStyle();
  Back->SetLineColor(2); // red color
  Back->SetLineWidth(2);
  Back->Draw("SAME");}
  
  if (Nplot > 2) {
  Prim->UseCurrentStyle();
  Prim->SetLineColor(14); // Grey color
  Prim->SetLineWidth(2);
  Prim->Draw("SAME");}
  
  if (Nplot > 3) {
  Join->UseCurrentStyle();
  Join->SetLineColor(28); // Brown
  Join->SetLineWidth(2);
  Join->Draw("SAME");}
  
  if (Nplot > 4) {
  MC_TOF->UseCurrentStyle();
  MC_TOF->SetLineColor(3); // Green color
  MC_TOF->SetLineWidth(2);
  MC_TOF->Draw("SAME");}
  
  if (Nplot > 5) {
  MC_TOF_prim->UseCurrentStyle();
  MC_TOF_prim->SetLineColor(6); // Magenta color
  MC_TOF_prim->SetLineWidth(2);
  MC_TOF_prim->Draw("SAME");}
  
  if (Nplot > 6) {
  Rand_TOF->UseCurrentStyle();
  Rand_TOF->SetLineColor(4); // Blue color 
  Rand_TOF->SetLineWidth(2);
  Rand_TOF->Draw("SAME");}
  
  if (Nplot > 7) {
  Rand_TOF_prim->UseCurrentStyle();
  Rand_TOF_prim->SetLineColor(7); // Cyaan color
  Rand_TOF_prim->SetLineWidth(2);
  Rand_TOF_prim->Draw("SAME");}
  
  if (Nplot > 8) {
  Sec_E->UseCurrentStyle();
  Sec_E->SetLineColor(46); // transparant red
  Sec_E->SetLineWidth(2);
  Sec_E->Draw("SAME");}
  
  if (Nplot > 9) {
  Sec_F->UseCurrentStyle();
  Sec_F->SetLineColor(5); // Yellow color 
  Sec_F->SetLineWidth(2);
  Sec_F->Draw("SAME");}
  
  // =====================================================================================
  // Then finally we should plot a legenda:
  
  Double_t Xmin = Naive->GetXaxis()->GetXmin(); // X-axis
  Double_t Xmax = Naive->GetXaxis()->GetXmax();
  Double_t Xlength = Xmax - Xmin;
  Double_t Ymin = 0.0;
  Double_t Ymax = Naive->GetYaxis()->GetXmax();
  Double_t Ylength = Ymax - Ymin;

  TPaveText* text [10];
  
  for (Int_t k = 0; k<10; ++k)
  {
    text[k] = new TPaveText(Xmin+Xlength*0.72,Ymin+Ylength*(0.93-0.025*((Int_t) (k+1))),Xmin+Xlength*0.73,Ymin+Ylength*(0.93-0.025*((Int_t) (k+1))+0.01),"NB");
    text[k]->SetFillColor(0); // Color of the box.
    text[k]->SetTextSize(0.02); // Define text size
    text[k]->SetTextFont(1); // Define text font
    text[k]->SetTextAngle(0.0); // Defines the angle at which the text is displayed.
  }
  
  // Define the text:
  text[0]->AddText("Energy threshold condition");
  text[1]->AddText("Pure backscattering events (tracking) in Energy threshold condition");
  text[2]->AddText("Primary events (tracking) in Energy threshold condition, possibly with backscattering");
  text[3]->AddText("Events in Energy threshold condition that are both primary and backscattering (tracking)");
  text[4]->AddText("Energy threshold and TOF condition (delta spike)");
  text[5]->AddText("Primary events (tracking) in Energy threshold and TOF condition (delta spike)");
  text[6]->AddText("Energy threshold and TOF condition (Gaussian)");
  text[7]->AddText("Primary events (tracking) in Energy threshold and TOF condition (Gaussian)");
  text[8]->AddText("Events in Energy threshold condition with some interaction before the VETO");
  text[9]->AddText("Secondary Particles in Energy threshold condition interacting with the VETO");
  
  // Set the colors:
  text[0]->SetTextColor(1);
  text[1]->SetTextColor(2);
  text[2]->SetTextColor(14);
  text[3]->SetTextColor(28);
  text[4]->SetTextColor(3);
  text[5]->SetTextColor(6);
  text[6]->SetTextColor(4);
  text[7]->SetTextColor(7);
  text[8]->SetTextColor(46);
  text[9]->SetTextColor(5);
  
  // Draw the legenda:
  for (Int_t k = 0; k<10; ++k) {if (Nplot > (k+0)) {text[k]->Draw("same");}}
  
  // Print the picture:
  TString strplots = StupidStuff.Itoa(Nplot,10);
  TString Filename = "./Pictures/VETO_distance_VS_performance_" + Ename + "_" + strplots + ".png";
  c->Print(Filename);
    
}