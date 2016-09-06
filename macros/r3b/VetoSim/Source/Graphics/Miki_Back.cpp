// include the required C++ headers:
#include <iostream>

// include the required root headers:

// include own headers:
#include "../Analysis/Inputs_Load.h"
#include "AnaLeafs_Connect.h"
#include "DrawHistDouma.h"

// use the standard C++ variables:
using namespace std;

void Miki_Back()
{
  // This function Creates a plot of Yield vs X-coordinate for either a single NeuLAND plane,
  // or for the entire NeuLAND. These pictures are the same as in Ken Miki his slides.
  // These pictures are a bit complicated to explain. The first thing to understand is that
  // the particles in these planes are INCOMING particles. Then for each bar that such a
  // particle fired, one count is added in the histogram (for the respective coordinate).
  // Hence we plot #incoming particles on the y-axis against x-coordinate on the x-axis.
  // If a partilce fired multiple bars, the particle is counted multiple times.
    
  // Output is given on the screen.
    
  // Written by C. A. Douma.
    
  // ===================================================================================
  // NOTE: Get the counter and the series from sed:
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
  Double_t pos = 0.0;
  if (Plane%2==0)
  {
    // even planes are vertical. Hence we are interested in horizontal position:
    pos = Inputs->VETO_geo_Xpos;
  }
  else
  {
    pos = Inputs->VETO_geo_Ypos;
  }
  Int_t NbarsFired = 0;
  Int_t barNumber;
  Int_t ThisPlane = 0;
  Int_t TrackID = 0;
  Int_t Prim_TrackID = 0;
  Int_t Nprims = 0;
  Int_t PDG = 0;
  Double_t XY_hit = 0.0;
  Bool_t VETOed = kFALSE;
  Bool_t FromTarget = kFALSE;
  
  // Declare TH1D histograms:
  TH1D* protons = new TH1D("protons","protons",50,pos-125.0,pos+125.0);
  TH1D* neutrons = new TH1D("neutrons","neutrons",50,pos-125.0,pos+125.0);
  TH1D* NfromT = new TH1D("NfromT","NfromT",50,pos-125.0,pos+125.0);
  TH1D* electrons = new TH1D("electrons","electrons",50,pos-125.0,pos+125.0);
  TH1D* gammas = new TH1D("gammas","gammas",50,pos-125.0,pos+125.0);
  TH1D* pions = new TH1D("pions","pions",50,pos-125.0,pos+125.0);
  TH1D* muons = new TH1D("muons","muons",50,pos-125.0,pos+125.0);
  TH1D* alphas = new TH1D("alphas","alphas",50,pos-125.0,pos+125.0);
  TH1D* Cions = new TH1D("Cions","Cions",50,pos-125.0,pos+125.0);
  TH1D* fragments = new TH1D("fragments","fragments",50,pos-125.0,pos+125.0);
  TH1D* total = new TH1D("total","total",50,pos-125.0,pos+125.0);
  
  // Now perform the event loop over the tree:
  for (Long64_t Event = 0; Event<nEvents; ++Event)
  {
      // read out an event:
      nBytes = AnaLeafs->TheTree->GetEntry(Event);
      
      // Now we will read out this event:
      NbarsFired = AnaLeafs->NbarsFired_NeuLAND->GetValue(0);
      Nprims = AnaLeafs->Nprims_NeuLAND->GetValue(0);
      
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
	  
	  // Get the trackID of the incoming particle that fired this bar:
	  TrackID = AnaLeafs->Neu_Primary_TrackID->GetValue(k);
	  
	  // Get the PDG that belongs to this TrackID:
	  PDG = 0;
          VETOed = kFALSE; // Since we only want to add particles that are not VETOed:
          FromTarget = kFALSE; // reset.
	  for (Int_t n = 0; n<Nprims; ++n)
	  {
	    Prim_TrackID = AnaLeafs->Prim_TrackID->GetValue(n);
	    if (TrackID==Prim_TrackID)
	    {
	      PDG = AnaLeafs->Prim_PDG->GetValue(n);
              VETOed = AnaLeafs->Prim_IsVETOed->GetValue(n);
              FromTarget = AnaLeafs->Prim_IsFromTarget->GetValue(n);
	    }
	  }
	  if (PDG==0) {cout << "We have a fired bar without primary particle! Plane = " << ThisPlane << " | Bar in the plane = " << barNumber << " | TrackID = " << TrackID << "\n";}
	  
	  // so now we know the barnumber and the PDG of the incoming particles. Hence now define the hit:
	  XY_hit = -125.0 + 2.5 + 5.0*((Int_t) (barNumber-1));
          
	  // so now we can decide how to fill our histograms:
	  if (((Plane==ThisPlane)||(Plane==0))&&(!VETOed))
	  {
	    if (TMath::Abs(PDG)==2212) {protons->Fill(XY_hit);}
	    else if (TMath::Abs(PDG)==2112) 
            {
                neutrons->Fill(XY_hit);
                if (FromTarget) {NfromT->Fill(XY_hit);}
            }
	    else if (TMath::Abs(PDG)==11) {electrons->Fill(XY_hit);}
	    else if (TMath::Abs(PDG)==22) {gammas->Fill(XY_hit);}
	    else if (TMath::Abs(PDG)==211) {pions->Fill(XY_hit);}
	    else if (TMath::Abs(PDG)==13) {muons->Fill(XY_hit);}
	    else if (TMath::Abs(PDG)==1000020040) {alphas->Fill(XY_hit);}
	    else if ((TMath::Abs(PDG)>1000060000)&&(TMath::Abs(PDG)<1000060190)) {Cions->Fill(XY_hit);}
	    else if (TMath::Abs(PDG)>1000000000) {fragments->Fill(XY_hit);}
	    total->Fill(XY_hit);
	  }
	  
	  // Then close the blocks:
	}
      }

      // Give a sign of life:
      if ((Event+1)%1000==0) {cout << "We processed " << Event+1 << " Events\n";}
  }
  
  cout << "The AnalysisTree is read out succesfully.\n\n"
       << "==========================================================================\n\n";
  
  //total->Draw();
       
  // ====================================================================================
  // Now all that we need to do is to make a nice decent plot. We have to plot the histograms
  // on top of each other, which requires some more algebra.
  TString Title = "Incoming particles distribution in NeuLAND plane ";
  TString st = "";
  Title = Title + st.Itoa(Plane,10);
  TString xlabel = "";
  if (Plane%2==0)
  {
    xlabel = "X-position (vertical plane) [cm]";
  }
  else
  {
    xlabel = "Y-position (horizontal plane) [cm]";
  }
  TString ylabel = "Bar Multiplicity [dim. less]";
  
  // Now create the empty plot:
  TH1D* h_clone = (TH1D*) total->Clone("h_clone");
  TCanvas* c = DrawHistDouma(h_clone,Title,xlabel,ylabel,0);
  
  // Now we have to plot the individual particles:
  Double_t max = total->GetMaximum();
  Double_t min = total->GetMinimum();
  Double_t Xmin = -125.0+pos;
  Double_t Xmax = 125.0+pos;
  
  // C-ions:
  gStyle->SetHistLineColor(13);
  gStyle->SetHistFillColor(13);
  gStyle->SetLineWidth(1.0);
  TH1D* h_c = (TH1D*) total->Clone("h_c");
  h_c->UseCurrentStyle();
  h_c->Draw("same");
  total->Add(Cions,-1.0);
  
  // alphas:
  gStyle->SetHistLineColor(4);
  gStyle->SetHistFillColor(4);
  gStyle->SetLineWidth(1.0);
  TH1D* h_alpha = (TH1D*) total->Clone("h_alpha");
  h_alpha->UseCurrentStyle();
  h_alpha->Draw("same");
  total->Add(alphas,-1.0);
  
  // muons
  gStyle->SetHistLineColor(6);
  gStyle->SetHistFillColor(6);
  gStyle->SetLineWidth(1.0);
  TH1D* h_mu = (TH1D*) total->Clone("h_mu");
  h_mu->UseCurrentStyle();
  h_mu->Draw("same");
  total->Add(muons,-1.0);
  
  // pions:
  gStyle->SetHistLineColor(9);
  gStyle->SetHistFillColor(9);
  gStyle->SetLineWidth(1.0);
  TH1D* h_pi = (TH1D*) total->Clone("h_pi");
  h_pi->UseCurrentStyle();
  h_pi->Draw("same");
  total->Add(pions,-1.0);
  
  // gammas:
  gStyle->SetHistLineColor(3);
  gStyle->SetHistFillColor(3);
  gStyle->SetLineWidth(1.0);
  TH1D* h_gam = (TH1D*) total->Clone("h_gam");
  h_gam->UseCurrentStyle();
  h_gam->Draw("same");
  total->Add(gammas,-1.0);
  
  // electrons:
  gStyle->SetHistLineColor(7);
  gStyle->SetHistFillColor(7);
  gStyle->SetLineWidth(1.0);
  TH1D* h_e = (TH1D*) total->Clone("h_e");
  h_e->UseCurrentStyle();
  h_e->Draw("same");
  total->Add(electrons,-1.0);
  
  // fragments:
  gStyle->SetHistLineColor(28);
  gStyle->SetHistFillColor(28);
  gStyle->SetLineWidth(1.0);
  TH1D* h_frag = (TH1D*) total->Clone("h_frag");
  h_frag->UseCurrentStyle();
  h_frag->Draw("same");
  total->Add(fragments,-1.0);
  
  // protons:
  gStyle->SetHistLineColor(2);
  gStyle->SetHistFillColor(2);
  gStyle->SetLineWidth(1.0);
  TH1D* h_p = (TH1D*) total->Clone("h_p");
  h_p->UseCurrentStyle();
  h_p->Draw("same");
  total->Add(protons,-1.0);
  
  // neutrons:
  gStyle->SetHistLineColor(8);
  gStyle->SetHistFillColor(8);
  gStyle->SetLineWidth(1.0);
  TH1D* h_n = (TH1D*) total->Clone("h_n");
  h_n->UseCurrentStyle();
  h_n->Draw("same");
  total->Add(neutrons,-1.0);
  
  // neutrons from target:
  gStyle->SetHistFillColor(32);
  gStyle->SetHistLineColor(32);
  gStyle->SetLineWidth(1.0);
  NfromT->UseCurrentStyle();
  NfromT->Draw("same");
  
  if (total->Integral()>0.4) {cout << "### ERROR: You missed some particles!!!\n\n\n";}
  cout << "# Fragments = " << fragments->Integral() << "\n";
  cout << "# C-ions = " << Cions->Integral() << "\n";
  cout << "# Alphas = " << alphas->Integral() << "\n";
  cout << "# Muons = " << muons->Integral() << "\n";
  cout << "# Pions = " << pions->Integral() << "\n";
  cout << "# Gammas = " << gammas->Integral() << "\n";
  cout << "# Electrons = " << electrons->Integral() << "\n";
  cout << "# Neutrons = " << neutrons->Integral() << "\n";
  cout << "# N from Target = " << NfromT->Integral() << "\n";
  cout << "# Protons = " << protons->Integral() << "\n";
  
  // Now build the legenda:
  TPaveText** text = new TPaveText*[10];
  
  for (Int_t k = 0; k<10; ++k)
  {
    text[k] = new TPaveText(Xmin+(Xmax-Xmin)*0.95,(1.15-0.05*((Int_t) k))*max,Xmin+(Xmax-Xmin)*0.96,(1.16-0.05*((Int_t) k))*max,"NB"); // NB says that no borders are drawn. 
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
    if (k==9) {text[k]->SetTextColor(32); text[k]->AddText("Neutrons from Target");}
  }
  
  for (Int_t k = 0; k<10; ++k)
  {
    text[k]->Draw("same");
  }
  
  // Done! Now just print the picture:
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
  TApplication* TheApp = new TApplication("Miki_Back",&argc,argv);
  Miki_Back();
  TheApp->Run();
  return 0;
}
#endif