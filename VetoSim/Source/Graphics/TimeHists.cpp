// include the required C++ headers:
#include <iostream>

// include the required root headers:

// include own headers:
#include "../Analysis/Inputs_Load.h"
#include "AnaLeafs_Connect.h"
#include "DrawHistDouma.h"

// use the standard C++ variables:
using namespace std;

void TimeHists()
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
  Int_t Nprims = 0;
  Int_t Nrec = 0;
  TH1D* h1 = new TH1D("h1","h1",100,0.0,400.0);
  TH1D* h2 = new TH1D("h2","h2",100,0.0,400.0);
  TH1D* h3 = new TH1D("h3","h3",100,0.0,400.0);
  
  // Now perform the event loop over the tree:
  for (Long64_t Event = 0; Event<nEvents; ++Event)
  {
      // read out an event:
      nBytes = AnaLeafs->TheTree->GetEntry(Event);
      
      // NOTE: Put here your code to read out the analysis tree.
      Nprims = AnaLeafs->Nprims_NeuLAND->GetValue(0);
      Nrec = AnaLeafs->N_Reconstructed_PrimPoints->GetValue(0);
      
      for (Int_t k = 0; k<Nprims; ++k)
      {
          if (AnaLeafs->Prim_PDG->GetValue(k)==2112) {
              if (AnaLeafs->Prim_IsFromTarget->GetValue(k)==kTRUE) {
          h1->Fill(AnaLeafs->Prim_MCinteraction_T->GetValue(k));}}
      }
           
      for (Int_t kr = 0; kr<Nrec; ++kr)
      {
          for (Int_t kp = 0; kp<Nprims; ++kp)
          {
              if (AnaLeafs->TrackID_Reconstructed_PrimPoints->GetValue(kr)==AnaLeafs->Prim_TrackID->GetValue(kp))
              {
                  if (AnaLeafs->Prim_PDG->GetValue(kp)==2112) 
                  {
                      if (AnaLeafs->Prim_IsFromTarget->GetValue(kp)==kTRUE) 
                      {
                          h2->Fill(AnaLeafs->T_Reconstructed_PrimPoints->GetValue(kr));
                      }
                  }
              }
          }
      }
      
      for (Int_t k = 0; k<Nrec; ++k)
      {
          h3->Fill(AnaLeafs->T_Reconstructed_PrimPoints->GetValue(k));
      }  

      // Give a sign of life:
      if ((Event+1)%1000==0) {cout << "We processed " << Event+1 << " Events\n";}
  }
  
  cout << "The AnalysisTree is read out succesfully.\n\n"
       << "==========================================================================\n\n";
  
  // ====================================================================================
  // NOTE: Put here your code for visualization, like painting histograms, etc.
  
  // Paint:
  TCanvas* c = DrawHistDouma(h3,"TOF Of reconstructed Vertices","Time [ns]","Counts",1);
    
  gStyle->SetHistLineColor(2);
  gStyle->SetLineWidth(2.0);
  h1->UseCurrentStyle();
  h1->Draw("same");
  
  gStyle->SetHistLineColor(4);
  gStyle->SetLineWidth(2.0);
  h2->UseCurrentStyle();
  h2->Draw("same");

  // Legenda:
    TString Legenda[3];
    Legenda[0] = "Monte Carlo TOF for Neutrons from Target";
    Legenda[1] = "Reconstructed TOF for Neutrons from Tagret";
    Legenda[2] = "Reconstructed TOF for all particles";
    
    TPaveText** text = new TPaveText*[3];
    Double_t Xmin = h3->GetXaxis()->GetXmin();
    Double_t Xmax = h3->GetXaxis()->GetXmax();
    Double_t Xlength = Xmax - Xmin;
    Double_t Ymin = 0.0;
    Double_t Ymax = h3->GetMaximum();
    Double_t Ylength = Ymax - Ymin;
    
    for (Int_t k = 0; k<3; ++k)
    {   
      text[k] = new TPaveText(Xmin+Xlength*0.8,Ymin+Ylength*(0.9 - 0.05*((Int_t) k)),Xmin+Xlength*0.81,Ymin+Ylength*(0.91 - 0.05*((Int_t) k)),"NB");
      text[k]->SetFillColor(0);
      text[k]->AddText(Legenda[k]); 
      text[k]->SetTextSize(0.03); 
      text[k]->SetTextFont(1); 
      text[k]->SetTextAngle(0.0);
    
      if (k==0) {text[k]->SetTextColor(2);}
      if (k==1) {text[k]->SetTextColor(4);}
      if (k==2) {text[k]->SetTextColor(1);}
      
      text[k]->Draw("same");
    }
    
    // Done! Print it:
    c->SaveAs("../../UI/Pictures/TOF Of reconstructed Vertices.png");
  
  // Finish:       
  cout << "Program is finished now.\n\n\n";

  // Done!
}
}

void Histograms()
{
  // This function does...
    
  // output is given on the screen.
    
  // Written by C. A. Douma.
    
  // ===================================================================================
  // NOTE: Before you do anything, you have to specify the counter That specifies which
  // NOTE: AnalysisTreeyou analyse and you also have to specify the Series, which states
  // NOTE: in what way the AnalysisTree was analyzed.
  Int_t Counter = 1; // Modify if you want.
  Int_t Series = 0;  // Modify if you want.
  
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
  TH1D* h1 = new TH1D("h1","h1",100,0.0,1000.0);
  TH1D* h2 = new TH1D("h2","h2",200,0.0,1000.0);
  TH1D* h3 = new TH1D("h3","h3",100,0.0,1000.0);
  TH1D* h4 = new TH1D("h4","h4",200,0.0,1000.0);
  TH1D* h5 = new TH1D("h5","h5",100,0.0,1000.0);
  TH1D* h6 = new TH1D("h6","h6",100,0.0,1000.0);
  Int_t Size = 0;
  
  // Now perform the event loop over the tree:
  for (Long64_t Event = 0; Event<nEvents; ++Event)
  {
      // read out an event:
      nBytes = AnaLeafs->TheTree->GetEntry(Event);
      
      // NOTE: Put here your code to read out the analysis tree.
      Size = AnaLeafs->Prim_E->GetNdata();
      
      for (Int_t k = 0; k<Size; ++k)
      {
          if ((AnaLeafs->Prim_IsFromTarget->GetValue(k)==kTRUE)&&(AnaLeafs->Prim_PDG->GetValue(k)==2112))
          {
              h1->Fill(AnaLeafs->Prim_E->GetValue(k));
              h2->Fill(AnaLeafs->Prim_MCinteraction_T->GetValue(k));
          }
          else if ((AnaLeafs->Prim_MCproduction_X->GetValue(k)<-120)&&(AnaLeafs->Prim_MCproduction_Z->GetValue(k)>970.0)&&(AnaLeafs->Prim_PDG->GetValue(k)==2112))
          {
              h3->Fill(AnaLeafs->Prim_E->GetValue(k));
              h4->Fill(AnaLeafs->Prim_MCinteraction_T->GetValue(k));
          }
          else if ((!((AnaLeafs->Prim_MCproduction_X->GetValue(k)<-120)&&(AnaLeafs->Prim_MCproduction_Z->GetValue(k)>970.0)))&&(AnaLeafs->Prim_PDG->GetValue(k)==2212))
          {
              h5->Fill(AnaLeafs->Prim_E->GetValue(k));
          }
          else if ((AnaLeafs->Prim_MCproduction_X->GetValue(k)<-120)&&(AnaLeafs->Prim_MCproduction_Z->GetValue(k)>970.0)&&(AnaLeafs->Prim_PDG->GetValue(k)==2212))
          {
              h6->Fill(AnaLeafs->Prim_E->GetValue(k));
          }
      }

      // Give a sign of life:
      if ((Event+1)%1000==0) {cout << "We processed " << Event+1 << " Events\n";}
  }
  
  cout << "The AnalysisTree is read out succesfully.\n\n"
       << "==========================================================================\n\n";
  
  // ====================================================================================
  // NOTE: Put here your code for visualization, like painting histograms, etc.
  TCanvas* c1 = DrawHistDouma(h1,"MC Energy of Neutrons","Energy [MeV]","Counts",8);
  h3->UseCurrentStyle();
  h3->SetLineColor(2);
  h3->SetLineWidth(2.0);
  h3->Draw("same");
  
  // Legenda:
    TString Legenda[2];
    Legenda[0] = "Neutrons from the target";
    Legenda[1] = "Neutrons from the shield";
    
    TPaveText** text = new TPaveText*[2];
    Double_t Xmin = h1->GetXaxis()->GetXmin();
    Double_t Xmax = h1->GetXaxis()->GetXmax();
    Double_t Xlength = Xmax - Xmin;
    Double_t Ymin = 0.0;
    Double_t Ymax = h1->GetMaximum();
    Double_t Ylength = Ymax - Ymin;
    
    for (Int_t k = 0; k<2; ++k)
    {   
      text[k] = new TPaveText(Xmin+Xlength*0.8,Ymin+Ylength*(0.9 - 0.05*((Int_t) k)),Xmin+Xlength*0.81,Ymin+Ylength*(0.91 - 0.05*((Int_t) k)),"NB");
      text[k]->SetFillColor(0);
      text[k]->AddText(Legenda[k]); 
      text[k]->SetTextSize(0.03); 
      text[k]->SetTextFont(1); 
      text[k]->SetTextAngle(0.0);
    
      if (k==0) {text[k]->SetTextColor(8);}
      if (k==1) {text[k]->SetTextColor(2);}
      
      text[k]->Draw("same");
    }
  
  TCanvas* c2 = DrawHistDouma(h2,"MC Time of First neutron interaction","Time [ns]","Counts",8);
  h4->UseCurrentStyle();
  h4->SetLineColor(2);
  h4->SetLineWidth(2.0);
  h4->Draw("same");
  
    // Legenda:
    Legenda[0] = "Neutrons from the target";
    Legenda[1] = "Neutrons from the shield";
    
    Xmin = h2->GetXaxis()->GetXmin();
    Xmax = h2->GetXaxis()->GetXmax();
    Xlength = Xmax - Xmin;
    Ymin = 0.0;
    Ymax = h2->GetMaximum();
    Ylength = Ymax - Ymin;
    
    for (Int_t k = 0; k<2; ++k)
    {   
      text[k] = new TPaveText(Xmin+Xlength*0.8,Ymin+Ylength*(0.9 - 0.05*((Int_t) k)),Xmin+Xlength*0.81,Ymin+Ylength*(0.91 - 0.05*((Int_t) k)),"NB");
      text[k]->SetFillColor(0);
      text[k]->AddText(Legenda[k]); 
      text[k]->SetTextSize(0.03); 
      text[k]->SetTextFont(1); 
      text[k]->SetTextAngle(0.0);
    
      if (k==0) {text[k]->SetTextColor(8);}
      if (k==1) {text[k]->SetTextColor(2);}
      
      text[k]->Draw("same");
    }
  
  TCanvas* c3 = DrawHistDouma(h6,"MC energy of Protons","Energy [MeV]","Counts",6);
  h5->UseCurrentStyle();
  h5->SetLineColor(4);
  h5->SetLineWidth(2.0);
  h5->Draw("same");
  
    // Legenda:
    Legenda[0] = "Scattered Protons";
    Legenda[1] = "Protons from the shield";
    
    Xmin = h6->GetXaxis()->GetXmin();
    Xmax = h6->GetXaxis()->GetXmax();
    Xlength = Xmax - Xmin;
    Ymin = 0.0;
    Ymax = h6->GetMaximum();
    Ylength = Ymax - Ymin;
    
    for (Int_t k = 0; k<2; ++k)
    {   
      text[k] = new TPaveText(Xmin+Xlength*0.8,Ymin+Ylength*(0.9 - 0.05*((Int_t) k)),Xmin+Xlength*0.81,Ymin+Ylength*(0.91 - 0.05*((Int_t) k)),"NB");
      text[k]->SetFillColor(0);
      text[k]->AddText(Legenda[k]); 
      text[k]->SetTextSize(0.03); 
      text[k]->SetTextFont(1); 
      text[k]->SetTextAngle(0.0);
    
      if (k==0) {text[k]->SetTextColor(6);}
      if (k==1) {text[k]->SetTextColor(4);}
      
      text[k]->Draw("same");
    }
  
  c1->SaveAs("../../UI/Pictures/NeutronEnergies.png");
  c2->SaveAs("../../UI/Pictures/NeutronTimes.png");
  c3->SaveAs("../../UI/Pictures/ProtonEnergies.png");
       
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
  TApplication* TheApp = new TApplication("TimeHists",&argc,argv);
  TimeHists();
  TApplication* TheApp2 = new TApplication("Histograms",&argc,argv);
  Histograms();
  TheApp->Run();
  return 0;
}
#endif