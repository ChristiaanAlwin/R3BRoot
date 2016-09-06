// inclusion of the require C++ headers:
#include <iostream>

// inclusion of the required ROOT headers:
#include "TSystem.h"
#include "TFile.h"
#include "TMath.h"
#include "TH1D.h"
#include "TString.h"

// Using the standard C++ variables:
using namespace std;

// This headers contains the functions that we need for computation of the Bethe-Bloch formulas. It contains several relatively simple
// functions. In the end a function void BetheMaster() is defined that can be called to execute all tasks. the resulting histograms are
// then computed and stored in a file called Bethe.root which will be stored in <path to>/VetoSim/Data/Analysis/

// =====================================================================================================================================

Double_t GetCharge(Int_t const PDG)
{
  // Computes the electrical charge of a particle, given the particle PDG code.
  Double_t charge = 0.0;
  // determine if it is elementary, meson, baryon, or ion:
  
  if (TMath::Abs(PDG) < 100)
  {
    // elementary. Leave charge equal to zero unless we match something else:
    if (TMath::Abs(PDG) == 1) {charge = 2.0/3.0;}
    if (TMath::Abs(PDG) == 2) {charge = -1.0/3.0;}
    if (TMath::Abs(PDG) == 3) {charge = -1.0/3.0;}
    if (TMath::Abs(PDG) == 4) {charge = 2.0/3.0;}
    if (TMath::Abs(PDG) == 5) {charge = -1.0/3.0;}
    if (TMath::Abs(PDG) == 6) {charge = 2.0/3.0;}
    if (TMath::Abs(PDG) == 7) {charge = -1.0/3.0;}
    if (TMath::Abs(PDG) == 8) {charge = 2.0/3.0;}
    if (TMath::Abs(PDG) == 11) {charge = -1.0;}
    if (TMath::Abs(PDG) == 13) {charge = -1.0;}
    if (TMath::Abs(PDG) == 15) {charge = -1.0;}
    if (TMath::Abs(PDG) == 17) {charge = -1.0;}
    if (TMath::Abs(PDG) == 24) {charge = 1.0;}
    if (TMath::Abs(PDG) == 34) {charge = 1.0;}
    if (TMath::Abs(PDG) == 37) {charge = 1.0;}
    
    if (PDG<0) {charge = -1.0*charge;}
  }
  if ((TMath::Abs(PDG)>1000)&&(TMath::Abs(PDG)<1000000000))
  {
    // Baryon or diquark:
    Double_t PDG_d = (Int_t) TMath::Abs(PDG);
    
    Double_t q1_d = PDG_d/1000.0;
    Double_t q2_d = PDG_d/100.0;
    Double_t q3_d = PDG_d/10.0;
    
    Int_t q1 = (Double_t) q1_d;
    q2_d = q2_d - 10.0*((Int_t) q1);
    Int_t q2 = (Double_t) q2_d;
    q3_d = q3_d - 10.0*((Int_t) q2) - 100.0*((Int_t) q1);
    Int_t q3 = (Double_t) q3_d;
    
    Double_t c1 = 0.0;
    Double_t c2 = 0.0;
    Double_t c3 = 0.0;
    
    if (q1==1) {c1 = -1.0/3.0;}
    if (q1==2) {c1 = 2.0/3.0;}
    if (q1==3) {c1 = -1.0/3.0;}
    if (q1==4) {c1 = 2.0/3.0;}
    if (q1==5) {c1 = -1.0/3.0;}
    if (q1==6) {c1 = 2.0/3.0;}
    if (q1==7) {c1 = -1.0/3.0;}
    if (q1==8) {c1 = 2.0/3.0;}
    
    if (q2==1) {c2 = -1.0/3.0;}
    if (q2==2) {c2 = 2.0/3.0;}
    if (q2==3) {c2 = -1.0/3.0;}
    if (q2==4) {c2 = 2.0/3.0;}
    if (q2==5) {c2 = -1.0/3.0;}
    if (q2==6) {c2 = 2.0/3.0;}
    if (q2==7) {c2 = -1.0/3.0;}
    if (q2==8) {c2 = 2.0/3.0;}
    
    if (q3==1) {c3 = -1.0/3.0;}
    if (q3==2) {c3 = 2.0/3.0;}
    if (q3==3) {c3 = -1.0/3.0;}
    if (q3==4) {c3 = 2.0/3.0;}
    if (q3==5) {c3 = -1.0/3.0;}
    if (q3==6) {c3 = 2.0/3.0;}
    if (q3==7) {c3 = -1.0/3.0;}
    if (q3==8) {c3 = 2.0/3.0;}

    charge = c1 + c2 + c3;
    
    if (PDG<0) {charge = -1.0*charge;}
  }
  if ((TMath::Abs(PDG)>100)&&(TMath::Abs(PDG)<1000))
  {
    // Meson:
    Double_t PDG_d = (Int_t) TMath::Abs(PDG);
    
    Double_t q1_d = PDG_d/100.0;
    Double_t q2_d = PDG_d/10.0;
    
    Int_t q1 = (Double_t) q1_d;
    q2_d = q2_d - 10.0*((Int_t) q1);
    Int_t q2 = (Double_t) q2_d;
    
    Double_t c1 = 0.0;
    Double_t c2 = 0.0;
    
    if (q1==1) {c1 = -1.0/3.0;}
    if (q1==2) {c1 = 2.0/3.0;}
    if (q1==3) {c1 = -1.0/3.0;}
    if (q1==4) {c1 = 2.0/3.0;}
    if (q1==5) {c1 = -1.0/3.0;}
    if (q1==6) {c1 = 2.0/3.0;}
    if (q1==7) {c1 = -1.0/3.0;}
    if (q1==8) {c1 = 2.0/3.0;}
    
    if (q2==1) {c2 = -1.0/3.0;}
    if (q2==2) {c2 = 2.0/3.0;}
    if (q2==3) {c2 = -1.0/3.0;}
    if (q2==4) {c2 = 2.0/3.0;}
    if (q2==5) {c2 = -1.0/3.0;}
    if (q2==6) {c2 = 2.0/3.0;}
    if (q2==7) {c2 = -1.0/3.0;}
    if (q2==8) {c2 = 2.0/3.0;}

    charge = c1 - c2; // NOTE: this is minus because the second quark is an anti-quark!
    
    // Now the real deal: sign convention (bit of trial and error)
    Int_t d = q1 - q2;
    Double_t dd = 1.0;
    if (d==0) {dd = 1.0;}
    if (d==1) {dd = -1.0;}
    if (d==2) {dd = 1.0;}
    if (d==3) {dd = -1.0;}
    if (d==4) {dd = 1.0;}
    if (d==5) {dd = -1.0;}
    if (d==6) {dd = 1.0;}
    if (d==7) {dd = -1.0;}
    if (d==8) {dd = 1.0;}
    if (d==9) {dd = -1.0;}
    charge = charge*dd;
    
    if (q1==0) {dd = 1.0;}
    if (q1==1) {dd = -1.0;}
    if (q1==2) {dd = 1.0;}
    if (q1==3) {dd = -1.0;}
    if (q1==4) {dd = 1.0;}
    if (q1==5) {dd = -1.0;}
    if (q1==6) {dd = 1.0;}
    if (q1==7) {dd = -1.0;}
    if (q1==8) {dd = 1.0;}
    if (q1==9) {dd = -1.0;}
    charge = charge*dd;
        
    if (PDG>0) {charge = charge*-1.0;}
  }
  if (TMath::Abs(PDG)>1000000000)
  {
    // Ions: can only come out if we have 100.AAA.ZZZ.X
    // Hence we must isolate ZZZ:

    Int_t PDGsmall = 0;
    if (PDG>0) {PDGsmall = PDG - 1000000000;}
    if (PDG<0) {PDGsmall = PDG + 1000000000;}
    Double_t PDG_d = (Int_t) TMath::Abs(PDGsmall);

    Double_t Z_d = PDG_d/10000.0;
    Double_t R_d = PDG_d/10000000.0;
    
    Int_t Z = (Double_t) Z_d;
    Int_t R = (Double_t) R_d;
    Z_d = Z_d - 1000.0*((Int_t) R);
    Z = (Double_t) Z_d;
    Z_d = (Int_t) Z;

    // Z_d is now precisely the charge:
    charge = Z_d;
  }
  
  // Done:
  return charge;
  
}

Double_t Dedx(Double_t const Ein, Int_t const PDG, Double_t const fMass, TString const material)
{
  // Computes the derivative dE/dx based on the Bethe-Bloch formula.
  
  // Define constants of physics first:
  Double_t c = 299792458;              // Speed of light
  Double_t me = 9.10938291e-31;        // electron mass
  Double_t e0 = 8.854187817e-12;       // epsilon_0 vacuum permittivity
  Double_t mu0 = 4*TMath::Pi()*1e-7;            // mu_0 vacumm permittivity
  Double_t qe = 1.60217657e-19;        // electron charge
  Double_t NA = 6.02214129e23;         // Avogadro number   
  Double_t u = 1.66053892e-27;         // atomic mass unit
  Double_t MC = 12.011;                // mass of a C12 atom [u]
  
  // Declare beam properties:
  Double_t Zbeam = 1.0;
  Double_t Abeam = 1.0;
  Double_t Mbeam = 1.0;

  // Define beam properties:
  Zbeam = GetCharge(PDG);
  Mbeam = ((fMass*1e9*qe)/(c*c))/u; // fMass in R3Broot is in GeV. Hence convert it to u.
  if (TMath::Abs(PDG)>1000000000)
  {
    Int_t PDGsmall = 0;
    if (PDG>0) {PDGsmall = PDG - 1000000000;}
    if (PDG<0) {PDGsmall = PDG + 1000000000;}
    Double_t PDG_d = (Int_t) TMath::Abs(PDGsmall);

    Double_t Z_d = PDG_d/10.0;
    Double_t R_d = PDG_d/10000.0;
    
    Int_t Z = (Double_t) Z_d;
    Int_t R = (Double_t) R_d;
    Z_d = Z_d - 1000.0*((Int_t) R);
    Z = (Double_t) Z_d;
    Z_d = (Int_t) Z;
    
    Abeam = Z_d;
  }
  else
  {
    Abeam = 1.0;
  }
  
  // Define Material properties:
  Double_t Zmaterial = 1.0; // [dim.less]
  Double_t rhomaterial = 1.0; // [kg/m^3]
  Double_t Mmaterial = 1.0; // [g/mol]
  if (material == "Air20")
  {
    Zmaterial = 7.209;
    rhomaterial = 1.225;
    Mmaterial = 14.423;
  }
  if (material == "Air0")
  {
    Zmaterial = 7.209;
    rhomaterial = 1.2882;
    Mmaterial = 14.423;
  }
  if (material == "Water")
  {
    Zmaterial = 3.33333;
    rhomaterial = 1000.0;
    Mmaterial = 6.0050;
  }
  if (material == "BC408")
  {
    Zmaterial = 3.3771;
    rhomaterial = 1032.0;
    Mmaterial = 6.2391;
  }
  if (material == "Densimet")
  {
    Zmaterial = 72.6;
    rhomaterial = 18978.0;
    Mmaterial = 180.0571;
  }
  if (material == "SS")
  {
    Zmaterial = 25.7190;
    rhomaterial = 7776.6;
    Mmaterial = 55.2791;
    // 13% Chromium, 0.15% Carbon, rest Iron.
  }
  
  // cout << "Z = " << Zbeam << "     Abeam = " << Abeam << "      Mbeam = " << Mbeam << "\n";
  
  // Convert some units:
  Double_t E = Ein*Abeam*qe*1e6; //from MeV/u to Joule.
  Double_t M = Mbeam*u; // from u=g/mol to kg.
  
  // Compute SR quantities:
  Double_t gamma = (E/(c*c) + M)/M;
  Double_t beta = TMath::Sqrt(1.0 - 1.0/(gamma*gamma));
  Double_t v = c*beta;
  Double_t Ek = M*c*c*((1.0/TMath::Sqrt(1 - beta*beta)) - 1.0);
  
  // Compute Bethe-Bloch quantities:
  Double_t n = Zmaterial*rhomaterial/(u*Mmaterial); // electron density
  Double_t I = (10.0*qe)*Zmaterial; // mean ionization potential.
  
  // Compute the real Bethe-Bloch formula:
  Double_t D = 1.0;
  Double_t L = 1.0;
  if ((Zbeam<0.0)&&(fMass<0.001))
  {
    // Electron version (select on mass & on charge, because we want everything else to be ordinary Bethe-Bloch.
    D = ((2.0*TMath::Pi())/(me*c*c))*((n*Zbeam*Zbeam)/(beta*beta))*((qe*qe)/(4*TMath::Pi()*e0))*((qe*qe)/(4*TMath::Pi()*e0));
    L = (Ek*me*c*c*beta*beta)/(2.0*I*I*(1.0-beta*beta));
    L = TMath::Log(L) - TMath::Log(2.0)*TMath::Log(2.0*TMath::Sqrt(1.0 - beta*beta) - 1.0 + beta*beta) + 1.0 - beta*beta + (1.0/8.0)*(1.0-TMath::Sqrt(1.0-beta*beta))*(1.0-TMath::Sqrt(1.0-beta*beta));
  }
  else
  {  
    // Positove ion version:
    D = ((4.0*TMath::Pi())/(me*c*c))*((n*Zbeam*Zbeam)/(beta*beta))*((qe*qe)/(4*TMath::Pi()*e0))*((qe*qe)/(4*TMath::Pi()*e0));
    L = (2.0*me*c*c*beta*beta)/(I*(1.0 - beta*beta));
    L = TMath::Log(L) - beta*beta;
  }
  D = -1.0*D*L;
  
  // Modify the low-energy behaviour if required:
  Double_t modify = TMath::Power((TMath::Exp(1.0)/(TMath::Power(beta,1.3)*TMath::Exp(1.0/(TMath::Power(beta,1.3))))),0.25);
  //D = D*modify;
  
  // Now convert D = dE/dx which is in J/m to (MeV/u)/m. This means:
  D = D/(Abeam*qe*1e6);
  
  // Finally, return the derivative:
  return D;
  
}

Double_t Bethe1(Double_t const E0, Int_t const PDG, Double_t const fMass, TString const material, Double_t const Dx)
{
  // Performs Forward Euler integration based on the Bethe-Bloch formula. It returns the travel distance.
  
  // Define energy and position:
  Double_t Ecurrent = E0;
  Double_t Xpos = 0.0;
  Double_t derivative = 0.0;
  
  // Now loop untill the energy gets negative, or skip the loop if the charge is zero:
  Double_t charge = GetCharge(PDG);
  
  if (TMath::Abs(charge) > 0.0)
  {
    while (Ecurrent > 0.0)
    {
      // Compute what we need:
      Xpos = Xpos + Dx;
      derivative = Dedx(Ecurrent,PDG,fMass,material);
    
      // Perform Forward Euler:
      Ecurrent = Ecurrent + Dx*derivative;
    } 
  }
  else
  {
    Xpos = 1e100;
  }      

  // Now we are done, so return the travel distance:
  return Xpos;
    
}


TH1D* Bethe(Int_t const nbinsE, Double_t const Emin, Double_t const Emax, Double_t const Dx0, Int_t const PDG, Double_t const fMass, TString const material, TString const Name)
{
  // Computes travel distances with Bethe-Bloch formula.
  // Emax is in [GeV/u]. Dx is in [m].
  
  // Declare the histogram of our results (Name is needed to avoid memory leaks).
  TH1D* h = new TH1D(Name,Name,nbinsE,Emin*1000.0,Emax*1000.0); // Convert from GeV to MeV!
  
  // Now we fill it with a for-loop:
  Double_t Ecurrent = Emin*1000.0; // Conversion to MeV/u !
  Double_t range = 0.0;
  
  // Adapt step size:
  Double_t Dx = Dx0;
  
  for (Int_t k = 1; k<(nbinsE+2); ++k) // NOTE: WE generate an overflow bin on purpose! We need this later on in the analysis!
  {
    Ecurrent = Emin + (((Int_t) k) - 1.0)*((Emax-Emin)/((Int_t) nbinsE)); // so bin 1 holds the min. energy (e.g. 0) and 2 one step higher, etc.
    Dx = Dx0*(Ecurrent/Emax);
    
    Ecurrent = Ecurrent*1000.0; // Conversion to MeV/u !
    
    range = Bethe1(Ecurrent,PDG,fMass,material,Dx);
    
    range = range*100.0; // Conversion from [m] to [cm].
    
    h->SetBinContent(k,range);
    
    cout << "Run " << k << "/" << nbinsE << " is done.\n";
    
  }
  
  // NOTE: WE also generate an underflow bin. But we must set it by hand:
  h->SetBinContent(0,0.0);
  
  // Then we return the histogram:
  return h;
  
}

void BetheMaster()
{
  // this function computes all the required histograms from the Bethe-Bloch formula and stores them in a file.
  // Before we can however do this, we must connect with the Inputs.root-file:
  
  cout << "=============================================================================\n\n"
       << "We will now compute the maximum travel distances through NeuLAND\n"
       << "for the secondary particles.\n\n"
       << "We connect to the Inputs.root-file first\n\n";
  
  FileStat_t stom;
  Int_t inputs_test = gSystem->GetPathInfo("../../Data/Inputs/Inputs.root",stom);
  
  if (inputs_test==1)
  {
    cout << "### ERROR: The Inputs.root-file does not exist! Create it first!\n"
         << "### ERROR: Without this file nothing is computed.\n\n";
  }
  else
  {
  
  // Connect to the Inputs.root file:
  TFile* inputs = new TFile("../../Data/Inputs/Inputs.root","read");
  cout << "Connection to the Inputs.root file was succesful.\n\n";
   
  // And then Get the histograms from this file:
  TH1D* Doubles = (TH1D*) inputs->Get("Doubles");
  TH1I* Integers = (TH1I*) inputs->Get("Integers");
  
  // ======================================================================================
  // So now we can run:
  
  // Declare/define what we need:
  Double_t Emin = 0.0; 					// Always! secondary particles can have small energies!
  Double_t Emax = Doubles->GetBinContent(203);		// Always! Maximum energy is the beam energy!
  Double_t Dx = -1.0;					// Should be re-defined every time...
  Double_t fMass = -1.0;				// Should be re-defined every time...
  TString material = "BC408";				// Always! This is the NeuLAND material!
  TString Name = "";					// Should be re-defined every time...
  Int_t nbinsEs = Integers->GetBinContent(301);		// Always! All histograms have the same number of bins!
  Int_t PDG = 0;					// Should be re-defined every time...
  
  // Protons in NeuLAND:
  cout << "Performing Bethe-Bloch simulation of PROTONS in NeuLAND\n";
  Dx = 0.001*Doubles->GetBinContent(316); // conversion to [m]!
  PDG = 2212; // PDG-code.
  fMass = 0.93827204621; // Proton mass in GeV.
  Name = "Protons_TravelDist";
  TH1D* Protons_TravelDist = Bethe(nbinsEs,Emin,Emax,Dx,PDG,fMass,material,Name);
  cout << "The Bethe-Bloch simulation of PROTONS in NeuLAND is complete.\n\n";
  
  // NOTE: Neutrons are neutral, hence we cannot compute them...
  
  // Electrons in NeuLAND:
  cout << "Performing Bethe-Bloch simulation of ELECTRONS in NeuLAND\n";
  Dx = 0.001*Doubles->GetBinContent(317); // conversion to [m]!
  PDG = 11; // PDG-code.
  fMass = 0.511e-3; // Electron mass in GeV.
  Name = "Electrons_TravelDist";
  TH1D* Electrons_TravelDist = Bethe(nbinsEs,Emin,Emax,Dx,PDG,fMass,material,Name);
  cout << "The Bethe-Bloch simulation of ELECTRONS in NeuLAND is complete.\n\n";
  
  // NOTE: Gammas are neutral, hence we cannot compute them...
  
  // Pions in NeuLAND:
  cout << "Performing Bethe-Bloch simulation of PIONS in NeuLAND\n";
  Dx = 0.001*Doubles->GetBinContent(318); // conversion to [m]!
  PDG = 211; // PDG-code.
  fMass = 0.1395701835; // Pion mass in GeV.
  Name = "Pions_TravelDist";
  TH1D* Pions_TravelDist = Bethe(nbinsEs,Emin,Emax,Dx,PDG,fMass,material,Name);
  cout << "The Bethe-Bloch simulation of PIONS in NeuLAND is complete.\n\n";
  
  // Muons in NeuLAND:
  cout << "Performing Bethe-Bloch simulation of MUONS in NeuLAND\n";
  Dx = 0.001*Doubles->GetBinContent(319); // conversion to [m]!
  PDG = 13; // PDG-code.
  fMass = 0.105658371535; // Muon mass in GeV.
  Name = "Muons_TravelDist";
  TH1D* Muons_TravelDist = Bethe(nbinsEs,Emin,Emax,Dx,PDG,fMass,material,Name);
  cout << "The Bethe-Bloch simulation of MUONS in NeuLAND is complete.\n\n";
  
  // Alphas in NeuLAND:
  cout << "Performing Bethe-Bloch simulation of ALPHAS in NeuLAND\n";
  Emax = Emax/4.0; // NOTE: Energy should be per nucleon in Bethe-Bloch formula!
  Dx = 0.001*Doubles->GetBinContent(320); // conversion to [m]!
  PDG = 1000020040; // PDG-code.
  fMass = 3.7273; // Alpha mass in GeV.
  Name = "Alphas_TravelDist";
  TH1D* Alphas_TravelDist = Bethe(nbinsEs,Emin,Emax,Dx,PDG,fMass,material,Name);
  Emax = Emax*4.0; // NOTE: Energy should be per nucleon in Bethe-Bloch formula!
  cout << "The Bethe-Bloch simulation of ALPHAS in NeuLAND is complete.\n\n";
  
  // C-ions in NeuLAND:
  cout << "Performing Bethe-Bloch simulation of C-IONS in NeuLAND\n";
  Emax = Emax/12.0; // NOTE: Energy should be per nucleon in Bethe-Bloch formula!
  Dx = 0.001*Doubles->GetBinContent(321); // conversion to [m]!
  PDG = 1000060120; // PDG-code.
  fMass = 11.188175; // C-ion mass in GeV.
  Name = "Cions_TravelDist";
  TH1D* Cions_TravelDist = Bethe(nbinsEs,Emin,Emax,Dx,PDG,fMass,material,Name);
  Emax = Emax*12.0; // NOTE: Energy should be per nucleon in Bethe-Bloch formula!
  cout << "The Bethe-Bloch simulation of C-IONS in NeuLAND is complete.\n\n";
  
  // ============================================================================================
  // Now that we have this, we can write the histograms to a file:
  cout << "Calculations are done! We now write our results to a file.\n\n";
  
  TFile* f = new TFile("../../Data/Analysis/Bethe.root","recreate");
  
  Protons_TravelDist->Write("Protons_TravelDist",2);
  // Neutrons are neutral, no Bethe-Bloch
  Electrons_TravelDist->Write("Electrons_TravelDist",2);
  // Gammas are neutral, no Bethe-Bloch
  Pions_TravelDist->Write("Pions_TravelDist",2);
  Muons_TravelDist->Write("Muons_TravelDist",2);
  Alphas_TravelDist->Write("Alphas_TravelDist",2);
  Cions_TravelDist->Write("Cions_TravelDist",2);
  // Fragments are indetermined, no Bethe-Bloch
  f->Close();
  
  cout << "Bethe-Bloch simulations are performed succesfully!\n\n"
       << "=============================================================================\n\n";
       
  // Done!
  }
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
}