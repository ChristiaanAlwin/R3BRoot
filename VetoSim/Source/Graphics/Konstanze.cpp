// include the required C++ headers:
#include <iostream>

// include the required root headers:

// include own headers:
#include "../Analysis/Inputs_Load.h"
#include "AnaLeafs_Connect.h"
#include "DrawHistDouma.h"
#include "DrawHistDouma_2D_flat.h"

// use the standard C++ variables:
using namespace std;

void Konstanze()
{
  // This function does...
    
  // output is given on the screen.
    
  // Written by C. A. Douma.
    
  // ===================================================================================
  // NOTE: Before you do anything, you have to specify the counter That specifies which
  // NOTE: AnalysisTreeyou analyse and you also have to specify the Series, which states
  // NOTE: in what way the AnalysisTree was analyzed.
  Int_t Counter = 0;
Counter = 1;

  Int_t Series = 0;
Series = 0;

  Double_t Time_low = 0.0;
Time_low = 52.;

  Double_t Time_up = 60.0;
Time_up = 72.;

  Int_t PDG = 0;
PDG = 0;
  
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
  
  Int_t nbins = 200;
  TH1D* time = new TH1D("time","time",nbins,0.0,200.0);
  TH1D* time2 = new TH1D("time2","time2",nbins,0.0,200.0);
  TH2D* pos = new TH2D("pos","pos",200,-400.0,2000.0,200,-700.0,400.0);
  TH2D* pint = new TH2D("pint","pint",200,-400.0,2000.0,200,-700.0,400.0);
  TH1D* E = new TH1D("E","E",nbins,0.0,3000.0);
  Double_t t_hit = 0.0;
  Int_t Size = 0;
  
  // NOTE: Declare here any variables you whish to use during the event loop.
  
  // Now perform the event loop over the tree:
  for (Long64_t Event = 0; Event<nEvents; ++Event)
  {
      // read out an event:
      nBytes = AnaLeafs->TheTree->GetEntry(Event);
      
      // NOTE: Put here your code to read out the analysis tree.
      Size = AnaLeafs->Prim_MCinteraction_T->GetNdata();
      
      for (Int_t k = 0; k<Size; ++k)
      {
          if ((AnaLeafs->Prim_PDG->GetValue(k)==PDG)||(PDG==0)||((PDG==1)&&(!((AnaLeafs->Prim_PDG->GetValue(k)==2112)||(AnaLeafs->Prim_PDG->GetValue(k)==22)))))
          {
          t_hit = AnaLeafs->Prim_MCinteraction_T->GetValue(k);
          time->Fill(t_hit);
          
          if (AnaLeafs->Prim_IsFromTarget->GetValue(k)==kFALSE) {time2->Fill(t_hit);}
          
          if (t_hit<Time_up&&t_hit>Time_low) 
          {
              pos->Fill(AnaLeafs->Prim_MCproduction_Z->GetValue(k),AnaLeafs->Prim_MCproduction_X->GetValue(k)); 
              E->Fill(AnaLeafs->Prim_E->GetValue(k)); 
              pint->Fill(AnaLeafs->Prim_MCinteraction_Z->GetValue(k),AnaLeafs->Prim_MCinteraction_X->GetValue(k));
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
 
  time->SetBinContent(nbins+1,0.0);
  TCanvas* c = DrawHistDouma(time,"MC time of particle first hit","Time [ns]","Counts",2);
  time2->UseCurrentStyle();
  time2->SetLineColor(4);
  time2->Draw("same");
  TCanvas* c2 = DrawHistDouma_2D_flat(pos,"Particle Production points between the TimeCuts","z [cm]","x [cm]","Counts");
  TCanvas* c3 = DrawHistDouma_2D_flat(pint,"Particle First Hit points between the TimeCuts","z [cm]","x [cm]","Counts");
  
  c->SaveAs("../../UI/Pictures/MCintT.png");
  c2->SaveAs("../../UI/Pictures/pos.png");
  c3->SaveAs("../../UI/Pictures/pint.png");
       
  cout << "### NOTE: Here we write down the numbers that we used:\n"
       << "Lower Time Cuts = " << Time_low << " [ns]\n"
       << "Upper Time Cuts = " << Time_up << " [ns]\n"
       << "Particle PDG    = " << PDG << "\n"
       << "Production  histogram integral = " << pos->Integral() << "\n"
       << "Interaction histogram integral = " << pint->Integral() << "\n\n";
  
  // Finish:
  cout << "Program is finished now.\n\n\n";

  // Done!
}
}

// Define the main for stand alone compilation:
#ifndef __CINT__
#include <TApplication.h>
int main(int argc, char** argv)
{
  TApplication* TheApp = new TApplication("Konstanze",&argc,argv);
  Konstanze();
  TheApp->Run();
  return 0;
}
#endif