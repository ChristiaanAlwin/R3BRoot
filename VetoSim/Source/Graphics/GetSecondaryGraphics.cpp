// inclusion of required C++ headers:
#include <iostream>

// inclusion of required ROOT headers:
#include "TH3D.h"
#include "TH2D.h"

// Inclusion of own headers:
#include "DrawHistDouma_2D_flatArray.h"
#include "../../Source/Analysis/Inputs_Load.h"
#include "AnaLeafs_Connect.h"

void GetSecondaryGraphics()
{
  // This function obtains the histograms from the Analysis Files and plots them 
  // onto canvasses. One Canvas is used for each particle species. This means that
  // 5 histograms will appear per canvas: produced, aimed, reached, hit and detected.
  
  // the canvasses are created, saved as .png-files in the Pictures folder and then 
  // closed. This is all that we do here.
  
  // ===============================================================================
  
  // We will also require a counter that specifies which analysis file we should use.
  // This will be done with sed:
  Int_t Counter = 0;
Counter = 1;
  
  // We also need the series from the sed:
  Int_t Series = 0;
Series = 0;
  
  // We begin with connecting to the Inputs.root-file:
  Inputs_Structure TheInputs;
  Inputs_Structure* Inputs = &TheInputs;
  Bool_t Load = Inputs_Load(Inputs,Counter,Series);

  // The also connect to the analysis output:
  AnaLeafs_Structure TheLeafs;
  AnaLeafs_Structure* AnaLeafs = &TheLeafs;
  Bool_t Connect = AnaLeafs_Connect(AnaLeafs,Counter);

  // ===============================================================================
  // So now we have everything that we need.
  cout << "=========================================================================\n\n"
       << "The requested plots will now be produced.\n\n";
  
  // Define the labels:
  TString Title [5];
  TString xlabel = "Energy of secondary particle [GeV]";
  TString ylabel = "Z of production point [cm]";
  TString zlabel = "Travel distance in NeuLAND [cm]";
  TString flabel = "Counts";
  TString Name = "";
  TString st = "";
  TString Cstr = st.Itoa(Counter,10);
  
  // Define the histogram array:
  TH3D* Harray [10][5];
  
  // Define the plot array:
  TH2D* Plots [5];
  
  // Define the existence check:
  Bool_t exist = kTRUE;
  
  // Now loop over all particles:
  for (Int_t k = 0; k<10; ++k)
  {
    // Define the titles:
    Title[0] = "Run "+Cstr + " Secondary";
    
    if (k==0) {Title[0] = Title[0]+" Protons";}
    if (k==1) {Title[0] = Title[0]+" Neutrons";}
    if (k==2) {Title[0] = Title[0]+" Electrons";}
    if (k==3) {Title[0] = Title[0]+" Gammas";}
    if (k==4) {Title[0] = Title[0]+" Pions";}
    if (k==5) {Title[0] = Title[0]+" Muons";}
    if (k==6) {Title[0] = Title[0]+" Alphas";}
    if (k==7) {Title[0] = Title[0]+" Cions";}
    if (k==8) {Title[0] = Title[0]+" Fragments";}
    if (k==9) {Title[0] = Title[0]+" Particles";}
    
    Title[1] = Title[0] + " Aimed at VETO";
    Title[2] = Title[0] + " that Reached VETO";
    Title[3] = Title[0] + " that Hit the VETO";
    Title[4] = Title[0] + " Detected by VETO";
    
    // Now obtain the histograms:
    for (Int_t n = 0; n<5; ++n)
    {
      // Define the name:
      Name = "Secondary";
      if (k==0) {Name = Name + "_Protons";}
      if (k==1) {Name = Name + "_Neutrons";}
      if (k==2) {Name = Name + "_Electrons";}
      if (k==3) {Name = Name + "_Gammas";}
      if (k==4) {Name = Name + "_Pions";}
      if (k==5) {Name = Name + "_Muons";}
      if (k==6) {Name = Name + "_Alphas";}
      if (k==7) {Name = Name + "_C-ions";}
      if (k==8) {Name = Name + "_Fragments";}
      if (k==9) {Name = "All_" + Name;}
       
      if (n==0) {Name = Name + "_produced.";}
      if (n==1) {Name = Name + "_aimed_at_VETO.";}
      if (n==2) {Name = Name + "_which_reached_VETO.";}
      if (n==3) {Name = Name + "_which_hit_VETO.";}
      if (n==4) {Name = Name + "_detected_by_VETO.";}
      
      // Retrieve them from the File:
      Harray[k][n] = (TH3D*) AnaLeafs->TheFile->Get(Name);

      // Check that they exist:
      if (Harray[k][n]==0) {cout << "### ERROR: The histogram <<"+Name+">> does not exist!\n\n\n"; exist = kFALSE;}
      
      // Now obtain the plots:
      if (exist)
      {
      if (Inputs->Projection==1) {Plots[n] = (TH2D*) Harray[k][n]->Project3D("yx");}
      else if (Inputs->Projection==2) {Plots[n] = (TH2D*) Harray[k][n]->Project3D("zx");}
      else {cout << "### ERROR: You specified the wrong projection. Hence nothing is done!\n\n\n";}
      }
    }
    
    // So now we have the plots. Now we can plot them:
    if (exist)
    {
    if (Inputs->Projection==1) {DrawHistDouma_2D_flatArray(5,3,2,Plots,Title,xlabel,ylabel,flabel,kTRUE,kTRUE);}
    if (Inputs->Projection==2) {DrawHistDouma_2D_flatArray(5,3,2,Plots,Title,xlabel,zlabel,flabel,kTRUE,kTRUE);}
    }
  }
    
  // And this is all we wanted to do.
  cout << "The requested plots are now produced and stored in the Pictures-folder.\n\n\n";
  
}

// Now define the main for stand-alone compilation:
#ifndef __CINT__
#include <TApplication.h>
int main(int argc, char** argv)
{
  TApplication* TheApp = new TApplication("GetSecondaryGraphics",&argc,argv);
  GetSecondaryGraphics();
  //TheApp->Run();
  return 0;
}
#endif