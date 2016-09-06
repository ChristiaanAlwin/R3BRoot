// include the required C++ headers:
#include <iostream>

// include the required root headers:
#include "TH1D.h"

// include own headers:
#include "../Analysis/Inputs_Load.h"
#include "AnaLeafs_Connect.h"
#include "DrawHistDouma.h"

// use the standard C++ variables:
using namespace std;

void Reconstructed_Energy()
{
  // This function plots the reconstructed energy of several primary particles.
    
  // output is given on the screen.
    
  // Written by C. A. Douma.
    
  // ===================================================================================
  // Before we do anything we have to obtain the Series and the Counter from sed:
  Int_t Counter = 0;
Counter = 9;

  Int_t Series = 0;
Series = 3;

  // Now we can go:
  
  cout << "We now read out a single analysis tree.\n\n"
       << "=========================================================================\n\n";
  
  // We begin with connecting to the Inputs.root-file:
  Inputs_Structure TheInputs;
  Inputs_Structure* Inputs = &TheInputs;
  Bool_t Load = Inputs_Load(Inputs,Counter,Series);
  
  // Then connect to the analysis tree:
  AnaLeafs_Structure TheLeafs;
  AnaLeafs_Structure* AnaLeafs = &TheLeafs;
  Bool_t Connect = AnaLeafs_Connect(AnaLeafs,Counter);
  
  // Then check if everything worked:
  if (!(Connect&&Load))
  {
      cout << "### ERROR: Some initializers could not be created!\n"
           << "### ERROR: Without these initializers this task could not be performed!\n"
           << "### ERROR: Therefore the program is now terminated!\n\n";
  }
  else
  {
      cout << "All initializers are succesfully created.\n\n"
           << "======================================================================\n\n"
           << "We will now loop over the tree.\n\n";

  // ====================================================================================
  // Now get everything that we need for the event loop:
  Long64_t nEvents = Inputs->nEvents;
  Long64_t nBytes = 0;
  Int_t Efull = Inputs->Eres_full;
  
  Double_t Emax = Inputs->Eres_Emax;
  
  Int_t nbins = Inputs->Nbins_Eres;
  TH1D* Eres_Protons = new TH1D("Proton Reconstructed Energy","Proton Reconstructed Energy",nbins,0.0,Emax);
  TH1D* Eres_Neutrons = new TH1D("Neutron Reconstructed Energy","Neutron Reconstructed Energy",nbins,0.0,Emax);
  TH1D* Eres_Electrons = new TH1D("Electron Reconstructed Energy","Electron Reconstructed Energy",nbins,0.0,Emax);
  TH1D* Eres_Gammas = new TH1D("Gamma Reconstructed Energy","Gamma Reconstructed Energy",nbins,0.0,Emax);
  TH1D* Eres_Pions = new TH1D("Pion Reconstructed Energy","Pion Reconstructed Energy",nbins,0.0,Emax);
  TH1D* Eres_Muons = new TH1D("Muon Reconstructed Energy","Muon Reconstructed Energy",nbins,0.0,Emax);
  TH1D* Eres_Alphas = new TH1D("Alpha Reconstructed Energy","Alpha Reconstructed Energy",nbins,0.0,Emax);
  TH1D* Eres_Cions = new TH1D("C-ion Reconstructed Energy","C-ion Reconstructed Energy",nbins,0.0,Emax);
  TH1D* Eres_Fragments = new TH1D("Fragment Reconstructed Energy","Fragment Reconstructed Energy",nbins,0.0,Emax);
  TH1D* Eres_Total = new TH1D("Total Reconstructed Energy","Total Reconstructed Energy",nbins,0.0,Emax);
  
  Int_t Nprims = 0;
  
  // Now perform the event loop over the tree:
  for (Long64_t Event = 0; Event<nEvents; ++Event)
  {
      // read out an event:
      nBytes = AnaLeafs->TheTree->GetEntry(Event);
      
      Nprims = AnaLeafs->Nprims_NeuLAND->GetValue(0);
      for (Int_t k = 0; k<Nprims; ++k)
      {
          if (Efull==1) 
	  {
	  if (AnaLeafs->Prim_PDG->GetValue(k)==2212) {Eres_Protons->Fill(AnaLeafs->Prim_E->GetValue(k));}  
	  else if (AnaLeafs->Prim_PDG->GetValue(k)==2112) {Eres_Neutrons->Fill(AnaLeafs->Prim_E->GetValue(k));}
          else if (AnaLeafs->Prim_PDG->GetValue(k)==11) {Eres_Electrons->Fill(AnaLeafs->Prim_E->GetValue(k));}
          else if (AnaLeafs->Prim_PDG->GetValue(k)==22) {Eres_Gammas->Fill(AnaLeafs->Prim_E->GetValue(k));}
          else if (TMath::Abs(AnaLeafs->Prim_PDG->GetValue(k)==211)) {Eres_Pions->Fill(AnaLeafs->Prim_E->GetValue(k));}
          else if (TMath::Abs(AnaLeafs->Prim_PDG->GetValue(k)==13)) {Eres_Muons->Fill(AnaLeafs->Prim_E->GetValue(k));}
          else if (AnaLeafs->Prim_PDG->GetValue(k)==1000020040) {Eres_Alphas->Fill(AnaLeafs->Prim_E->GetValue(k));}
          else if ((AnaLeafs->Prim_PDG->GetValue(k)>1000060000)&&(AnaLeafs->Prim_PDG->GetValue(k)>1000060190)) {Eres_Cions->Fill(AnaLeafs->Prim_E->GetValue(k));}
          else if (AnaLeafs->Prim_PDG->GetValue(k)==1000000000) {Eres_Fragments->Fill(AnaLeafs->Prim_E->GetValue(k));}
          Eres_Total->Fill(AnaLeafs->Prim_E->GetValue(k));
	  }
	  else
	  {
	  if (AnaLeafs->Prim_PDG->GetValue(k)==2212) {Eres_Protons->Fill(AnaLeafs->Prim_E_Detected->GetValue(k));}  
	  else if (AnaLeafs->Prim_PDG->GetValue(k)==2112) {Eres_Neutrons->Fill(AnaLeafs->Prim_E_Detected->GetValue(k));}
          else if (AnaLeafs->Prim_PDG->GetValue(k)==11) {Eres_Electrons->Fill(AnaLeafs->Prim_E_Detected->GetValue(k));}
          else if (AnaLeafs->Prim_PDG->GetValue(k)==22) {Eres_Gammas->Fill(AnaLeafs->Prim_E_Detected->GetValue(k));}
          else if (TMath::Abs(AnaLeafs->Prim_PDG->GetValue(k)==211)) {Eres_Pions->Fill(AnaLeafs->Prim_E_Detected->GetValue(k));}
          else if (TMath::Abs(AnaLeafs->Prim_PDG->GetValue(k)==13)) {Eres_Muons->Fill(AnaLeafs->Prim_E_Detected->GetValue(k));}
          else if (AnaLeafs->Prim_PDG->GetValue(k)==1000020040) {Eres_Alphas->Fill(AnaLeafs->Prim_E_Detected->GetValue(k));}
          else if ((AnaLeafs->Prim_PDG->GetValue(k)>1000060000)&&(AnaLeafs->Prim_PDG->GetValue(k)>1000060190)) {Eres_Cions->Fill(AnaLeafs->Prim_E_Detected->GetValue(k));}
          else if (AnaLeafs->Prim_PDG->GetValue(k)==1000000000) {Eres_Fragments->Fill(AnaLeafs->Prim_E_Detected->GetValue(k));}
          Eres_Total->Fill(AnaLeafs->Prim_E_Detected->GetValue(k));
	  }
      }

      // Give a sign of life:
      if ((Event+1)%1000==0) {cout << "We processed " << Event+1 << " Events\n";}
  }
  
  cout << "The AnalysisTree is read out succesfully.\n\n"
       << "==========================================================================\n\n";
  
  // ====================================================================================
  // Now we have to paint our histograms.   
       
  // Draw the total first:
  Eres_Protons->SetMaximum(1000.0*((Int_t) nEvents)/25000.0);     
  Eres_Neutrons->SetMaximum(1000.0*((Int_t) nEvents)/25000.0);
  Eres_Electrons->SetMaximum(1000.0*((Int_t) nEvents)/25000.0);     
  Eres_Gammas->SetMaximum(1000.0*((Int_t) nEvents)/25000.0);  
  Eres_Pions->SetMaximum(1000.0*((Int_t) nEvents)/25000.0);     
  Eres_Muons->SetMaximum(1000.0*((Int_t) nEvents)/25000.0);  
  Eres_Alphas->SetMaximum(1000.0*((Int_t) nEvents)/25000.0);     
  Eres_Cions->SetMaximum(1000.0*((Int_t) nEvents)/25000.0);  
  Eres_Fragments->SetMaximum(1000.0*((Int_t) nEvents)/25000.0);     
  Eres_Total->SetMaximum(1000.0*((Int_t) nEvents)/25000.0);    
       
  TString Title = "Incoming particle Reconstructed Energy by NeuLAND";
  if (Efull==1) {Title = "Monte Carlo Energy of Incoming Particles";}
  
  TCanvas* c = DrawHistDouma(Eres_Total,Title,"Particle Energy [MeV]","Counts",1); // black
  
  // theb draw the particles on top of it:
  gStyle->SetHistLineColor(2); // red
  Eres_Protons->UseCurrentStyle();
  Eres_Protons->Draw("same");
  
  gStyle->SetHistLineColor(8); // dark green
  Eres_Neutrons->UseCurrentStyle();
  Eres_Neutrons->Draw("same");
  
  gStyle->SetHistLineColor(7); // light blue
  Eres_Electrons->UseCurrentStyle();
  Eres_Electrons->Draw("same");
  
  gStyle->SetHistLineColor(3); // bright green
  Eres_Gammas->UseCurrentStyle();
  Eres_Gammas->Draw("same");
  
  gStyle->SetHistLineColor(9); // purple
  Eres_Pions->UseCurrentStyle();
  Eres_Pions->Draw("same");
  
  gStyle->SetHistLineColor(6); // magenta
  Eres_Muons->UseCurrentStyle();
  Eres_Muons->Draw("same");
  
  gStyle->SetHistLineColor(4); // dark blue
  Eres_Alphas->UseCurrentStyle();
  Eres_Alphas->Draw("same");
  
  gStyle->SetHistLineColor(13); // grey
  Eres_Cions->UseCurrentStyle();
  Eres_Cions->Draw("same");
  
  gStyle->SetHistLineColor(28); // brown
  Eres_Fragments->UseCurrentStyle();
  Eres_Fragments->Draw("same");
  
  // Then we paint a legenda:
  Double_t max = Eres_Protons->GetMaximum();
  Double_t min = Eres_Protons->GetMinimum();
  Double_t Xmin = 0.0;
  Double_t Xmax = 1.0*Emax;
  TPaveText** text = new TPaveText*[10];
  
  for (Int_t k = 0; k<10; ++k)
  {
    text[k] = new TPaveText(Xmin+(Xmax-Xmin)*0.95,(1.10-0.05*((Int_t) k))*max,Xmin+(Xmax-Xmin)*0.96,(1.11-0.05*((Int_t) k))*max,"NB"); // NB says that no borders are drawn. 
    text[k]->SetFillColor(0); 
    text[k]->SetTextSize(0.03); 
    text[k]->SetTextFont(1);
    text[k]->SetTextAngle(0.0);
    
    if (k==0) {text[k]->SetTextColor(2); text[k]->AddText("Protons");}
    if (k==1) {text[k]->SetTextColor(8); text[k]->AddText("Neutrons");}
    if (k==2) {text[k]->SetTextColor(7); text[k]->AddText("Electrons");}
    if (k==3) {text[k]->SetTextColor(3); text[k]->AddText("Gammas");}
    if (k==4) {text[k]->SetTextColor(9); text[k]->AddText("Pions");}
    if (k==5) {text[k]->SetTextColor(6); text[k]->AddText("Muons");}
    if (k==6) {text[k]->SetTextColor(4); text[k]->AddText("Alphas");}
    if (k==7) {text[k]->SetTextColor(13); text[k]->AddText("C-ions");}
    if (k==8) {text[k]->SetTextColor(28); text[k]->AddText("fragments");}
    if (k==9) {text[k]->SetTextColor(1); text[k]->AddText("All");}
  }
  
  for (Int_t k = 0; k<10; ++k)
  {
    text[k]->Draw("same");
  }
  
  // Save the picture:
  c->SaveAs("../../UI/Pictures/" + Title + ".png");
  
  // Finish:
  cout << "Program is finished now.\n\n\n";
  // Done!
}
}

// Now define the main for stand-alone compilation:
#ifndef __CINT__
#include <TApplication.h>
int main(int argc, char** argv)
{
  TApplication* TheApp = new TApplication("Reconstructed_Energy",&argc,argv);
  Reconstructed_Energy();
  TheApp->Run();
  return 0;
}
#endif