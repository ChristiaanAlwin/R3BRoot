// include the required C++ headers:
#include <iostream>

// include the required root headers:
#include "TH3D.h"

// include own headers:
#include "../Analysis/Inputs_Load.h"
#include "AnaLeafs_Connect.h"
#include "DrawHistDouma.h"
#include "DrawHistDouma_2D_flat.h"

// use the standard C++ variables:
using namespace std;

void Background()
{
  // This function does...
  // output is given on the screen.
    
  // Written by C. A. Douma.
    
  // ===================================================================================
  // NOTE: Before you do anything, you have to specify the counter That specifies which
  // NOTE: AnalysisTreeyou analyse and you also have to specify the Series, which states
  // NOTE: in what way the AnalysisTree was analyzed.
    
  // Get those from sed:  
  Int_t Counter = 0;
Counter = 1;

  Int_t Series = 0;
Series = 0;
  
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
  
  // NOTE: Declare here any variables you whish to use during the event loop.
  Int_t nbins = Inputs->Nbins_KenMiki;
  TH3D* Ken = new TH3D("Particle production points","Particle production points",nbins,-1000.0,500.0,nbins,-500.0,500.0,nbins*2,-500.0,2000.0);
  TH1D* EnergyMC = new TH1D("Particle MC Energy","Particle MC Energy",Inputs->Nbins_Eres,0.0,Inputs->Eres_Emax);
  TH1D* EnergyRes = new TH1D("Particle Reconstructed Energy","Particle Reconstructed Energy",Inputs->Nbins_Eres,0.0,Inputs->Eres_Emax);
  Int_t ThePDG = Inputs->PDG_KenMiki;
  Int_t EventSize = 0;
  Int_t PDG = 0;
  Double_t xpoint = 0.0;
  Double_t ypoint = 0.0;
  Double_t zpoint = 0.0;
  Double_t Emc = 0.0;
  Double_t Eres = 0.0;
  
  // Now perform the event loop over the tree:
  for (Long64_t Event = 0; Event<nEvents; ++Event)
  {
      // read out an event:
      nBytes = AnaLeafs->TheTree->GetEntry(Event);
      
      // NOTE: Put here your code to read out the analysis tree.
      EventSize = AnaLeafs->Prim_MCproduction_Z->GetNdata();
      
      if ((Inputs->UseTrigger!=1)||((AnaLeafs->Full_Trigger->GetValue(0)==kTRUE)))
      {
      
      for (Int_t k = 0; k<EventSize; ++k)
      {
          // Get our data:
          PDG = AnaLeafs->Prim_PDG->GetValue(k);
          xpoint = AnaLeafs->Prim_MCproduction_X->GetValue(k);
          ypoint = AnaLeafs->Prim_MCproduction_Y->GetValue(k);
          zpoint = AnaLeafs->Prim_MCproduction_Z->GetValue(k);
          Emc = AnaLeafs->Prim_E->GetValue(k);
          Eres = AnaLeafs->Prim_E_Detected->GetValue(k);
          
          if ((Inputs->Only_Detected!=1)||((AnaLeafs->Prim_Detected->GetValue(k)==kTRUE)))
          {
          
          // fill the histograms:
          if (ThePDG==0)
          {
              Ken->Fill(xpoint,ypoint,zpoint);
              EnergyMC->Fill(Emc);
              EnergyRes->Fill(Eres);
          }
          else if (ThePDG==1)
          {
              if (!((PDG==2112)||(PDG==22)))
              {
                  Ken->Fill(xpoint,ypoint,zpoint);
                  EnergyMC->Fill(Emc);
                  EnergyRes->Fill(Eres);
              }
          }
          else
          {
              if (PDG==ThePDG)
              {
                  Ken->Fill(xpoint,ypoint,zpoint);
                  EnergyMC->Fill(Emc);
                  EnergyRes->Fill(Eres);
              }
          }
          }
      }
      }
                  
      // Give a sign of life:
      if ((Event+1)%1000==0) {cout << "We processed " << Event+1 << " Events\n";}
  }
  
  cout << "The AnalysisTree is read out succesfully.\n\n"
       << "==========================================================================\n\n";
  
  // ====================================================================================
  // NOTE: Put here your code for visualization, like painting histograms, etc.
       
  // Create the histogram titles:
  TString Title = "Production Points Reaching NeuLAND";
  TString TE = "Startpoint Monte Carlo Energy Reaching NeuLAND";
  TString TEr = "TOF Energy Reaching NeuLAND";
  TString Particle = "Particle ";
  
  if (ThePDG==1) {Particle = "Charged Particle ";}
  else if (ThePDG==2212) {Particle = "Proton ";}
  else if (ThePDG==2112) {Particle = "Neutron ";}
  else if (ThePDG==11) {Particle = "Electron ";}
  else if (ThePDG==22) {Particle = "Gamma ";}
  else if (ThePDG==211) {Particle = "Pion ";}
  else if (ThePDG==13) {Particle = "Muon ";}
  else if (ThePDG==1000020040) {Particle = "Alpha ";}
  else if (ThePDG==1000060120) {Particle = "C_12 ";}
  
  Title = Particle + Title;
  TE = Particle + TE;
  TEr = Particle + TEr;
  
  // Draw:
  TH2D* KenXZ = (TH2D*) Ken->Project3D("xz");
  TCanvas* c1 = DrawHistDouma_2D_flat(KenXZ,Title,"z-coordinate [cm]","x-coordinate [cm]","Counts");
  TCanvas* c2 = DrawHistDouma(EnergyMC,TE,"Energy [MeV]","Counts",1);
  TCanvas* c3 = DrawHistDouma(EnergyRes,TEr,"Energy [MeV]","Counts",1);
  
  // Save:
  c1->SaveAs("../../UI/Pictures/" + Title + ".png");
  c2->SaveAs("../../UI/Pictures/" + TE + ".png");
  c3->SaveAs("../../UI/Pictures/" + TEr + ".png");
  
  
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
  TApplication* TheApp = new TApplication("Background",&argc,argv);
  Background();
  TheApp->Run();
  return 0;
}
#endif