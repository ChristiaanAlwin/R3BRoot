// include the required C++ headers:
#include <iostream>

// include the required root headers:

// include own headers:
#include "../Analysis/Inputs_Load.h"
#include "AnaLeafs_Connect.h"
#include "DrawHistDouma_2D_flat.h"

// use the standard C++ variables:
using namespace std;

void TOFvsEdep()
{
  // This function plots the TOF of a single bar against the Edep of that bar.
//   // we either sum up all bars of NeuLAND, or we sum up over the bars within a single plane.
    
  // output is given on the screen.
    
  // Written by C. A. Douma.
    
  // ===================================================================================
  // Get the Series and the Counter from sed:
  Int_t Series = 0;
Series = 0;
  
  Int_t Counter = 0;
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
  Int_t Plane = Inputs->NeuContent_Plane;
  Int_t nbins = Inputs->Nbins_ETOF;
  Double_t Emin = 0.0;
  Double_t Emax = Inputs->ETOF_Emax;
  Double_t Tmin = Inputs->ETOF_Tmin;
  Double_t Tmax = Inputs->ETOF_Tmax;
  Int_t NbarsFired = 0;
  Int_t barNumber = 0;
  Int_t ThisPlane = 0;
  Double_t TOF = 0.0;
  Double_t Edep = 0.0;
  Bool_t VETOed = kFALSE;
  
  TH2D* h = new TH2D("Edep VS TOF","Edep VS TOF",nbins,Emin,Emax,nbins,Tmin,Tmax);

  // Now perform the event loop over the tree:
  for (Long64_t Event = 0; Event<nEvents; ++Event)
  {
      // read out an event:
      nBytes = AnaLeafs->TheTree->GetEntry(Event);
      
      // Now we will read out this event:
      NbarsFired = AnaLeafs->NbarsFired_NeuLAND->GetValue(0);
      
      if (NbarsFired>0)
      {
	for (Int_t k = 0; k<NbarsFired; ++k)
	{
	  // request barnumber:
	  barNumber = AnaLeafs->Neu_BarNumber_Fired->GetValue(k);
	  
	  // find out the barnumber within that plane and the plane number:
	  ThisPlane = 1;
	  while (barNumber>50)
	  {
	    ThisPlane = ThisPlane + 1;
	    barNumber = barNumber - 50;
	  }
	  
	  // Obtain TOF and Edep:
	  TOF = AnaLeafs->Neu_TOF_Fired_Res->GetValue(k);
	  if ((Inputs->TOFchoice==1)||(Inputs->TOFchoice==3)) {TOF = AnaLeafs->Neu_TOF_Fired->GetValue(k);}
	  Edep = AnaLeafs->Neu_Edep_Fired->GetValue(k);
	  
	  // But we only want to add bars that are not VETOed:
	  VETOed = AnaLeafs->Neu_Bar_IsVETOed->GetValue(k);
	  
	  // so now we can decide how to fill our histograms:
	  if (((Plane==ThisPlane)||(Plane==0))&&(!VETOed))
	  {
	    h->Fill(Edep,TOF);
	  }
	  
	  // Close the blocks:
	}
      }

      // Give a sign of life:
      if ((Event+1)%1000==0) {cout << "We processed " << Event+1 << " Events\n";}
  }
  
  cout << "The AnalysisTree is read out succesfully.\n\n"
       << "==========================================================================\n\n";
  
  // ====================================================================================
  // Now create our picture:
 
  TString Title = "TOF vs. Deposited Energy per non-VETOed bar for NeuLAND plane ";
  TString st = "";
  Title = Title + st.Itoa(Plane,10);
  TString xlabel = "Deposited Energy [MeV]";
  TString ylabel = "Time of Flight [ns]";
  TString zlabel = "Counts";
       
  TCanvas* c = DrawHistDouma_2D_flat(h,Title,xlabel,ylabel,zlabel);
  
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
  TApplication* TheApp = new TApplication("TOFvsEdep",&argc,argv);
  TOFvsEdep();
  TheApp->Run();
  return 0;
}
#endif