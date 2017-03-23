// include the required C++ headers:
#include <iostream>

// include the required root headers:
#include "TH2D.h"
#include "TH3D.h"

// include own headers:
#include "../Analysis/Leaf_Connect.h"
#include "../Analysis/Inputs_Load.h"
#include "AnaLeafs_Connect.h"
#include "../Analysis/Volumes.h"
#include "../Analysis/Secondary_Calculation.h"
#include "DrawHistDouma_2D_flat.h"

// use the standard C++ variables:
using namespace std;

void Ken_Miki()
{
  // This function paints the production points of all tracks with a given PDG-code.
  // This can be used to reproduce the slides of Ken Miki.
    
  // output is given on the screen.
    
  // Written by C. A. Douma.
    
  // ===================================================================================
  // Obtain the Series and the counter from sed. We also need the PDG-code from
  // sed to know which particles we are looking for:
  Int_t Series = 0;
Series = 0;

  Int_t Counter = 1;
Counter = 1;
  
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
  
  // In this particular case we also need to connect to the Monte-Carlo tree:
  Leaf_Structure MCLeafsen;
  Leaf_Structure* MCLeafs = &MCLeafsen;
  Bool_t MC_connect = Leaf_Connect(MCLeafs,Counter,Series);
  
  // Then check if everything worked:
  if (!(Connect&&Load&&MC_connect))
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
  
  // build the TH3D with production points:
  Int_t nbins = Inputs->Nbins_KenMiki;
  TH3D* Ken = new TH3D("Particle production points","Particle production points",nbins,-1000.0,500.0,nbins,-500.0,500.0,nbins*2,-500.0,2000.0);
  
  // Declare what we need:
  Int_t ThePDG = Inputs->PDG_KenMiki;
  Int_t EventSize = 0;
  Int_t PDG = 0;
  Double_t xpoint = 0.0;
  Double_t ypoint = 0.0;
  Double_t zpoint = 0.0;
  Bool_t select_veto = (Inputs->KenMiki_Aim==1);
  Double_t px = 0.0;
  Double_t py = 0.0;
  Double_t pz = 0.0;
  Bool_t Aimed_at_VETO = kFALSE;
  Double_t dist = 0.0;
  
  // Now perform the event loop over the tree:
  for (Long64_t Event = 0; Event<nEvents; ++Event)
  {
      // read out an event:
      nBytes = MCLeafs->TheTree->GetEntry(Event); // NOTE: The MC-tree!
      
      // fill the histogram:
      EventSize = MCLeafs->MCTrack_fPdgCode->GetNdata();
      
      if (EventSize>0)
      {
	for (Int_t k = 0; k<EventSize; ++k)
	{
	  PDG = MCLeafs->MCTrack_fPdgCode->GetValue(k);
	  xpoint = MCLeafs->MCTrack_fStartX->GetValue(k);
	  ypoint = MCLeafs->MCTrack_fStartY->GetValue(k);
	  zpoint = MCLeafs->MCTrack_fStartZ->GetValue(k);
	  
          if (select_veto)
	  {
	    px = MCLeafs->MCTrack_fPx->GetValue(k);
	    py = MCLeafs->MCTrack_fPy->GetValue(k);
	    pz = MCLeafs->MCTrack_fPz->GetValue(k);
	    dist = TrackAimedAtVETO(Inputs,xpoint,ypoint,zpoint,px,py,pz);
	    Aimed_at_VETO = (dist>0.0);
	  
	    if ((TMath::Abs(PDG)==ThePDG)&&Aimed_at_VETO) {Ken->Fill(xpoint,ypoint,zpoint);}
	    if ((ThePDG==0)&&(Aimed_at_VETO)) {Ken->Fill(xpoint,ypoint,zpoint);}
	  }
	  else
	  {
	    if (TMath::Abs(PDG)==ThePDG) {Ken->Fill(xpoint,ypoint,zpoint);}
	    if (ThePDG==0) {Ken->Fill(xpoint,ypoint,zpoint);}
	  }
	}
      }

      // Give a sign of life:
      if ((Event+1)%1000==0) {cout << "We processed " << Event+1 << " Events\n";}
  }
  
  cout << "The AnalysisTree is read out succesfully.\n\n"
       << "==========================================================================\n\n";
  
  // ====================================================================================
  // Now all we need to do is make a plot:
  TH2D* Ken_flat = (TH2D*) Ken->Project3D("xz");
  cout << "Histogram integral = " << Ken_flat->Integral() << "\n\n";
  
  // Define the title:
  TString Title = "Production Points";
  if (select_veto) {Title = Title + " Aimed at VETO";}
  if (ThePDG==2212) {Title = "Proton " + Title;}
  else if (ThePDG==2112) {Title = "Neutron " + Title;}
  else if (ThePDG==11) {Title = "Electron and Positron " + Title;}
  else if (ThePDG==22) {Title = "Gamma " + Title;}
  else if (ThePDG==211) {Title = "Pion " + Title;}
  else if (ThePDG==13) {Title = "Muon " + Title;}
  else if (ThePDG==1000020040) {Title = "Alpha " + Title;}
  else if (ThePDG==1000060120) {Title = "C_12 " + Title;}
  else {Title = "Particle " + Title;}
  
  TCanvas* c = DrawHistDouma_2D_flat(Ken_flat,Title,"Beam z-axis [cm]","x-axis [cm]","Counts");
  
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
  TApplication* TheApp = new TApplication("Ken_Miki",&argc,argv);
  Ken_Miki();
  TheApp->Run();
  return 0;
}
#endif