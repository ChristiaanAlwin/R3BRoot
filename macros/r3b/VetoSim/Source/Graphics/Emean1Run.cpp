// include the required C++ headers:
#include <iostream>

// include the required root headers:

// include own headers:
// #include "../Analysis/Inputs_Load.h" // NOTE: They are already included in a previous task!
// #include "AnaLeafs_Connect.h" // NOTE: They are already included in a previous task!

// use the standard C++ variables:
using namespace std;

std::vector<Double_t> Emean1Run(Int_t const Counter, Int_t const Series)
{
  // This function fills a histogram with reconstructed energy per primary particle
  // within one run. It then returns the mean and sigma of those histograms
  // into an std::vector. Counter and Series do what they always do.
    
  // Written by C. A. Douma.
    
  // ===================================================================================
 
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
  
  // We first declare what we need:
  TH1D** Eres = new TH1D*[11];
  TString st = "";
  TString kstr = "";
  Int_t nbins = Inputs->Nbins_Eres;
  Double_t Emin = 0.0;
  Double_t Emax = Inputs->Eres_Emax;
  Double_t ThisEnergy = 0.0;
  Int_t ThisPDG = 0;
  Int_t Nprims = 0;
  for (Int_t k = 0; k<11; ++k)
  {
    kstr = st.Itoa(k,10);  
    Eres[k] = new TH1D("h"+kstr,"h"+kstr,nbins,Emin,Emax);
  }
  
  // Now perform the event loop over the tree:
  for (Long64_t Event = 0; Event<nEvents; ++Event)
  {
      // read out an event:
      nBytes = AnaLeafs->TheTree->GetEntry(Event);
      
      // now we simply start counting particles:
      Nprims = AnaLeafs->Nprims_NeuLAND->GetValue(0);
      
      if (Nprims>0)
      {
          for (Int_t k = 0; k<Nprims; ++k)
          {
              ThisEnergy = AnaLeafs->Prim_E_Detected->GetValue(k);
	      ThisPDG = AnaLeafs->Prim_PDG->GetValue(k);
	    
	      if (ThisPDG==2212) {Eres[0]->Fill(ThisEnergy);}
              else if (ThisPDG==2112) {Eres[1]->Fill(ThisEnergy);}
              else if (ThisPDG==11) {Eres[2]->Fill(ThisEnergy);}
              else if (ThisPDG==22) {Eres[3]->Fill(ThisEnergy);}
              else if (TMath::Abs(ThisPDG)==211) {Eres[4]->Fill(ThisEnergy);}
              else if (TMath::Abs(ThisPDG)==13) {Eres[5]->Fill(ThisEnergy);}
              else if (ThisPDG==1000020040) {Eres[6]->Fill(ThisEnergy);}
              else if ((ThisPDG>1000060000)&&(ThisPDG<1000060190)) {Eres[7]->Fill(ThisEnergy);}
              else if (ThisPDG>1000000000) {Eres[8]->Fill(ThisEnergy);}
              else if (ThisPDG==-11) {Eres[9]->Fill(ThisEnergy);}
              else {cout << "### ERROR: We have an unidentified primary particle! | Event = " << Event << " | PDG = " << ThisPDG << " | Index = " << k << "\n";}
              Eres[10]->Fill(ThisEnergy);
          }
      }

      // Give a sign of life:
      if ((Event+1)%1000==0) {cout << "We processed " << Event+1 << " Events\n";}
  }
  
  cout << "The AnalysisTree is read out succesfully.\n\n"
       << "==========================================================================\n\n";
  
  // ====================================================================================
  // Now put our results into an std::vector:
  std::vector<Double_t> Results;
  Results.clear();
  
  Double_t mean;
  Double_t sigma;
  
  for (Int_t k = 0; k<11; ++k)
  {
      // We should now compute mean and sigma of all of our particle-histograms:     
      mean = Eres[k]->GetMean(1); // 1 means along x-axis.
      sigma = Eres[k]->GetMeanError(1); // 1 means again x-axis. NOTE: GetMeanError and GetStdDev are 2 differet things (StdDev is the sigma!)
      Results.push_back(mean);
      Results.push_back(sigma);
  }
  
  // To avoid crash with incompatibilities...
  Results.push_back(0.0);
  Results.push_back(0.0);
       
  // Then return the result:
  return Results;
  
}
}