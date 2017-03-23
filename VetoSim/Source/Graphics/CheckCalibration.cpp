// include the required C++ headers:
#include <iostream>
#include <fstream>

// include the required root headers:
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH1I.h"

// include own headers:
#include "../Analysis/Inputs_Load.h"
#include "AnaLeafs_Connect.h"
#include "DrawHistDouma_2D_flat.h"
#include "../Calibration/Integral_2D.h"
#include "../Calibration/Efficiency.h"

// use the standard C++ variables:
using namespace std;

void CheckCalibration()
{
  // This function does...
    
  // output is given on the screen.
    
  // Written by C. A. Douma.
    
  // ===================================================================================
  // NOTE: Before you do anything, you have to specify the counter That specifies which
  // NOTE: AnalysisTreeyou analyse and you also have to specify the Series, which states
  // NOTE: in what way the AnalysisTree was analyzed.
    
  Int_t Counter = 1;
Counter = 1;

  Int_t Series = 0;
Series = 0;
  
  Int_t Nneutrons = 1;
Nneutrons = 2;

  Int_t Nprotons = 0;
Nprotons = 1;

  // ====================================================================================
  // Read out the calibration files:
  TFile* inputs = new TFile("../../Data/Inputs/Inputs.root","read");
  TH1I* Integers = (TH1I*) inputs->Get("Integers");
  Int_t fMaxProtons = Integers->GetBinContent(246);

  // We only need one file at a time, even that we read multiple of them.
  ifstream ThisFile;
  
  // Now read the files one-by-one:
  TString Name = "";
  TString kstr = "";
  TString st = "";
  Int_t NLines = 0;
  Int_t* fNumberOfCuts = new Int_t[fMaxProtons+1];
  Double_t** fCuts = new Double_t*[fMaxProtons+1];
  Double_t* fKappa = new Double_t[fMaxProtons+1];
  
  for (Int_t k = 0; k<(fMaxProtons+1); ++k)
  {
      // Define the filenames:
      kstr = st.Itoa(k,10);
      Name = "../../UI/Calibration/Energy_Cuts_" + kstr + "p.txt";
  
      // Open the files:
      ThisFile.open(Name);
      // Check if opened succesfully
      if(! ThisFile.is_open()) 
      {
        Fatal("ReadCalibrFile", "Calibration file does not exist! Aborting...");
      }

      // Now we must read the calibration file. First determine its size and process:
      NLines = 0;
      std::string ThisLine;
      while (std::getline(ThisFile,ThisLine)) {NLines = NLines + 1;}
      fNumberOfCuts[k] = NLines - 1;
      fCuts[k] = new Double_t[fNumberOfCuts[k]];
      ThisFile.close();
      
      // Then re-open and read:
      ThisFile.open(Name);
      ThisFile >> fKappa[k];
      for (Int_t kE = 0; kE<fNumberOfCuts[k]; ++kE) {ThisFile >> fCuts[k][kE];}
      ThisFile.close();  
      
      // Done!
  }
  
  // =====================================================================================
  
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
  Double_t NClusters = 0.0;
  Double_t Etot = 0.0;
  Int_t NbarsVETO = 0;
  Int_t Size = 0;
  Int_t NTargetCounter = 0;
  TH2D* h = new TH2D("h","h",200,0.0,1500.0,80,0.0,80.0);

  // Now perform the event loop over the tree:
  for (Long64_t Event = 0; Event<nEvents; ++Event)
  {
      // read out an event:
      nBytes = AnaLeafs->TheTree->GetEntry(Event);
      
      if ((Inputs->UseTrigger!=1)||((AnaLeafs->Full_Trigger->GetValue(0)==kTRUE)))
      {
      
      // NOTE: Put here your code to read out the analysis tree.
      Size = AnaLeafs->NeuLAND_ClusterEnergy->GetNdata();
      Etot = 0;
      for (Int_t k = 0; k<Size; ++k)
      {
          Etot = Etot + AnaLeafs->NeuLAND_ClusterEnergy->GetValue(k);
      }
      NClusters = (Int_t) AnaLeafs->NeuLAND_Nclusters->GetValue(0);
      
      NTargetCounter = 0;
      Size = AnaLeafs->Prim_PDG->GetNdata();
      for (Int_t k = 0; k<Size; ++k)
      {
          if ((AnaLeafs->Prim_PDG->GetValue(k)==2112)&&(AnaLeafs->Prim_IsFromTarget->GetValue(k)==kTRUE))
          {
              NTargetCounter = NTargetCounter + 1;
          }
      }
      
      Size = AnaLeafs->NbarsFired_VETO->GetValue(0);
      NbarsVETO = 0;
      for (Int_t k = 0; k<Size; ++k)
      {
          if (AnaLeafs->VETO_TOF_Fired_Res->GetValue(k)<AnaLeafs->Neu_TotalTOF_Res->GetValue(0))
          {
              NbarsVETO = NbarsVETO + 1;
          }
      }
      
      if ((NTargetCounter==Nneutrons)&&(NbarsVETO==Nprotons)) {h->Fill(Etot,NClusters);}
      }
      
      // Give a sign of life:
      if ((Event+1)%1000==0) {cout << "We processed " << Event+1 << " Events\n";}
  }
  
  cout << "The AnalysisTree is read out succesfully.\n\n"
       << "==========================================================================\n\n";
  
  // ====================================================================================
  // NOTE: Put here your code for visualization, like painting histograms, etc.
  TString st = "";
  TString sn = st.Itoa(Nneutrons,10);
  TString sp = st.Itoa(Nprotons,10);
       
  TCanvas* c1 = DrawHistDouma_2D_flat(h,"Total Edep vs #Clusters for "+sn+"n from Target Fired="+sp,"Energy [MeV]","#Clusters","Counts");
  
  TLine** TheLines = new TLine*[Nneutrons + Nprotons + 1];
  for (Int_t k = 0; k<(Nneutrons+Nprotons+1); k++) 
  {
    TheLines[k] = new TLine(fCuts[Nprotons][k],0.0,0.0,fKappa[Nprotons]*fCuts[Nprotons][k]);
    TheLines[k]->SetLineWidth(2.0);
    TheLines[k]->SetLineColor(1);
  }
  TheLines[Nneutrons+Nprotons-1]->Draw("same");
  TheLines[Nneutrons+Nprotons]->Draw("same");
  
  Double_t Xmin = h->GetXaxis()->GetXmin();
  Double_t Xmax = h->GetXaxis()->GetXmax();
  Double_t Ymin = h->GetYaxis()->GetXmin();
  Double_t Ymax = h->GetYaxis()->GetXmax();
  
  // Then create a char array to print the text in:
  char strn2[10];
  
  // Create also an array of TPaveText:

  TPaveText* text = new TPaveText(Xmin+(Xmax-Xmin)*0.20,Ymin+(Ymax-Ymin)*0.93,Xmin+(Xmax-Xmin)*0.21,Ymin+(Ymax-Ymin)*0.94,"NB"); // NB says that no borders are drawn. 
  text->SetFillColor(0); 
  text->SetTextColor(2); 
  text->SetTextSize(0.14); 
  text->SetTextFont(1);
  text->SetTextAngle(0.0); 
  sprintf(strn2, "%02.0f%%", Efficiency((TH2F*) h,fCuts[Nprotons][Nneutrons+Nprotons-1],fCuts[Nprotons][Nneutrons+Nprotons],fKappa[Nprotons])*100.0);
  text->AddText(strn2);
  text->Draw("same");
       
  c1->SaveAs("../../UI/Pictures/CheckCalibration_"+sn+"n_"+sp+"p.png");
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
  TApplication* TheApp = new TApplication("CheckCalibration",&argc,argv);
  CheckCalibration();
  TheApp->Run();
  return 0;
}
#endif