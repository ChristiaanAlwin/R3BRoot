//inclusion of C++ headers:
#include <iostream>

//inclusion of required ROOT headers:


// inclusion of our own headers:
#include "../Analysis/Leaf_Connect.h"
#include "../Analysis/Inputs_Load.h"

// Using standard C++ variables:
using namespace std;

void Proton_Energy_Estimation()
{
    // This function estimates the energy of the incoming
    // protons. It does not need series arguments, counters,
    // etc. as inputs, because it simply takes the run
    // that is just performed. 
    
    // Inputsare taken from the inputs-file. Outputs are
    // written to a tree and to a histogram. Hence we do
    // not have any inputs or outputs.
    
    // The idea of this function is to come up with an initial
    // estimate of the proton energy (or pion, muon, etc).
    // This is done by taking ONLY events that have a
    // single hit in the VETO and a single hit in the first
    // single-plane of NeuLAND. We know then that this hit is a
    // proton and we know which hits belong to each other. We take the
    // TOF difference between them and compute a beta from
    // this. From this beta and the PDG code one can then
    // come up with an energy.
    
    // If we only take events in which the VETO and the first plane,
    // have a single hit, we will have an under-estimate of the number of particles.
    // This is no problem, since we are looking for the ensemble
    // average energy to use in our calibration runs. The amount
    // of charged particles is already known from the number of
    // bars that fired in the VETO.
    
    // The sole purpose of this function is to come up with this
    // ensemble average so that we can use that in our calibration
    // runs to determine the number of reconstructed points in NeuLAND.
    
    // written by C. A. Douma, for the R3B collaboration.
    
    // ============================================================================================
    // We begin with loading our inputs and the leafs from our runs:
    
    cout << "=============================================================================\n\n"
         << "We now produce our initial estimate of the proton energy...\n\n";
    
    FileStat_t stom;
    Int_t inputs_test = gSystem->GetPathInfo("../../Data/Inputs/Inputs.root",stom);
    if (inputs_test==1)
    {
      cout << "### ERROR: The Inputs.root-file does not exist! Create it first!\n\n";
    }
    else
    {
   
    // Obtain the Counter and the Series from the current run:
    TFile* inputs = new TFile("../../Data/Inputs/Inputs.root","read");
    TH1D* Integers = (TH1D*) inputs->Get("Integers");
    Int_t Counter = Integers->GetBinContent(1);
    Int_t AutoCal = Integers->GetBinContent(242);
    if (AutoCal!=2) {Counter = Counter - 1;} // Because then you have already updated the runs.
    if (Counter==0) {Counter = 1;} // Otherwise you might run in trouble if you re-run the inputs-command.
    Int_t Series = 0;
         
    // Connect to the Leafs of the simulation data:
    Leaf_Structure TheLeafsen;
    Leaf_Structure* TheLeafs = &TheLeafsen;
    Bool_t Connect = Leaf_Connect(TheLeafs,Counter,Series);
  
    // Load all inputs from the Inputs.root-file:
    Series = -1;
    Inputs_Structure TheInputs;
    Inputs_Structure* Inputs = &TheInputs;
    Bool_t Load = Inputs_Load(Inputs,Counter,Series);
    
    // Check if we indeed have all digitizers. Else we cannot proceed:
    TObjString* Monte_Carlo_Engine_Obj = (TObjString*) inputs->Get("Monte_Carlo_Engine");
    TString Monte_Carlo_Engine = Monte_Carlo_Engine_Obj->GetString();
    
    Int_t Neu_Ex = Integers->GetBinContent(216);
    Int_t VETO_Ex = Integers->GetBinContent(217);
    Int_t NEB_Ex = Integers->GetBinContent(230);
    Int_t NEBv_Ex = Integers->GetBinContent(231);
    Int_t G3Test = Integers->GetBinContent(249);
    
    Bool_t Neu = ((Neu_Ex==1)||(Neu_Ex==2));
    Bool_t Vet = ((VETO_Ex==1)||(VETO_Ex==2));
    Bool_t NEB = (!(NEB_Ex==1));
    Bool_t NEBv = (!(NEBv_Ex==1));
    Bool_t Gean = (Monte_Carlo_Engine=="TGeant3");
    if (G3Test==0) {Gean = kTRUE;}
    
    Bool_t Digis_Exits = Neu&&Vet&&NEB&&NEBv&&Gean;
    
    // Then test if we can safely continue:
    Bool_t test = Load&&Connect&&Digis_Exits;
    
    if (!test)
    {
        if (!(Load&&Connect)) {cout << "### ERROR: Some of the Initializers do not exist. Recheck your code!!!\n\n";}
        else if (!Digis_Exits) {cout << "### ERROR: Some of the digitizers do not exist. We cannot run!\n\n";}
        else {cout << "### ERROR: Some serious bug!!!!\n\n\n\n\n\n\n\n";}
    }
    else
    {
        
    // ==============================================================================================    
    // Now we are in real business. First create our output tree and histograms:
        
    inputs->Close();
    TString st = "";
    TString kstr = st.Itoa(Counter,10);
    TFile* est_file = new TFile("../../Data/Analysis/Proton_Estimation_"+(Inputs->Output_Name) + "_run=" + kstr + ".root","recreate");
    TTree* est_tree = new TTree("EstimationTree","EstimationTree");
    
    // NOTE: we do not need arrays, because we only use events that have 1-hit, 1-hit!
    Int_t Nhits_VETO = 0;           // Number of bars Fired in the VETO.
    Int_t Nhits_NeuPlane1 = 0;      // Number of hits in NeuLAND first single-plane.
    Double_t Beta_Estimate = 0;     // Contains TOF + travel distance beta estimation.
    Int_t PDG_Estimate = 0;         // Contains estimate of PDG code.
    Double_t Energy_Estimate = 0;   // Contains Energy estimate [MeV] based on beta and PDG code.
    
    est_tree->Branch("Nhits_VETO",&(Nhits_VETO),"Nhits_VETO/I");
    est_tree->Branch("Nhits_NeuPlane1",&(Nhits_NeuPlane1),"Nhits_NeuPlane1/I");
    est_tree->Branch("Beta_Estimate",&(Beta_Estimate),"Beta_Estimate/D");
    est_tree->Branch("PDG_Estimate",&(PDG_Estimate),"PDG_Estimate/I");
    est_tree->Branch("Energy_Estimate",&(Energy_Estimate),"Energy_Estimate/D");
    
    Int_t hist_nbins = 150;
    TH1D* beta_hist = new TH1D("Ensemble Estimation of Relativistic Beta","Ensemble Estimation of Relativistic Beta",hist_nbins,0.0,1.5);
    TH1D* Energy_hist = new TH1D("Ensemble Estimation of Energy","Ensemble Estimation of Energy",hist_nbins,0.0,1500.0*(Inputs->Beam_Emax)); // Beam_Emax is in [GeV], we want it in [MeV].
    
    // ==============================================================================================
    // Now we can loop over the simulation output results.
    
    // This is going to be a tree-loop. Declare the variables we need:
    Long64_t nEvents = TheLeafs->NeuDigiTree->GetEntries();
    Long64_t nBytes_Neu = 0;
    Long64_t nBytes_VETO = 0;
    
    Int_t nVETOhits = 0;
    Int_t nNeuhits = 0;
    Int_t nFirstPlaneHits = 0;
    Int_t bar_hit = 0;
    Int_t nVETOfirstHits = 0;
    
    Double_t NeuLAND_TOF = 0.0;
    Double_t gamma = 0.0;
    Double_t Distance = 0.0;
    Double_t Delta_t;
    Double_t beta;
    
    std::vector<Double_t> Neu_time;
    std::vector<Double_t> Neu_x;
    std::vector<Double_t> Neu_y;
    std::vector<Double_t> Neu_z;
    
    std::vector<Double_t> VETO_time;
    std::vector<Double_t> VETO_x;
    std::vector<Double_t> VETO_y;
    std::vector<Double_t> VETO_z;
    
    // Define the speed of light in cm/ns:
    Double_t c = 29.9792458; // NOTE: Units!!!
    
    // Define proton mass:
    Double_t Mproton = 1e3*Inputs->pTracksMass_NeutronCalibration; // Mass of the particle [MeV].
    
    // Check if it is an ion. If this is the case, find A & Z:
    Int_t Particle_ID = Inputs->pTracksPDG_NeutronCalibration;
    Bool_t Track_IsIon = (Particle_ID>1000000000);
    Int_t Abeam = -1.0;
    Int_t Zbeam = -1.0;
    if (TMath::Abs(Particle_ID)>1000000000)
    {
    Int_t PDGsmall = 0;
    if (Particle_ID>0) {PDGsmall = Particle_ID - 1000000000;}
    if (Particle_ID<0) {PDGsmall = Particle_ID + 1000000000;}
    Double_t PDG_d = (Int_t) TMath::Abs(PDGsmall);

    Double_t Z_d = PDG_d/10.0;
    Double_t R_d = PDG_d/10000.0;
    
    Double_t z_Z_d = PDG_d/10000.0;
    Double_t z_R_d = PDG_d/10000000.0;
    
    Int_t Z = (Double_t) Z_d;
    Int_t R = (Double_t) R_d;
    Z_d = Z_d - 1000.0*((Int_t) R);
    Z = (Double_t) Z_d;
    Z_d = (Int_t) Z;
    
    Int_t z_Z = (Double_t) z_Z_d;
    Int_t z_R = (Double_t) z_R_d;
    z_Z_d = z_Z_d - 1000.0*((Int_t) z_R);
    z_Z = (Double_t) z_Z_d;
    z_Z_d = (Int_t) z_Z;
    
    Abeam = Z_d;
    Zbeam = z_Z_d;
  }
    
    // Find the maximum beta. This is when a proton travels with Emax. FIXME: Is the particle always a proton?
    Double_t Emax = 1000.0*Inputs->Beam_Emax; // only Ekin. Conversion from [GeV] to [MeV].
    if (Track_IsIon) {Emax = (1000.0*Inputs->Beam_Emax)*((Int_t) Abeam);}
    Emax = Emax + Mproton; // full energy.
    gamma = Emax/Mproton; // gamma.
    Double_t BetaMax = TMath::Sqrt(1.0 - (1.0/(gamma*gamma)));
    
    // Now loop over the tree:
    cout << "Everything checks out. we will now execute the tree-loop. nEvents = " << nEvents << "\n\n";
    
    for (Long64_t Event = 0; Event<nEvents; ++Event)
    {
        // Begin with loading the current event:
        nBytes_Neu = TheLeafs->NeuDigiTree->GetEntry(Event);
        nBytes_VETO = TheLeafs->VetoDigiTree->GetEntry(Event);
        
        // Get the number of hits:
        nVETOhits = TheLeafs->veto_segDigi_fTdc->GetNdata();
        nNeuhits = TheLeafs->LandDigi_fTdc->GetNdata();
        
        // Now we must obtain the number of hits
        // in the first single-plane and store it in std::vectors. 
        // At the same time we also compute the full NeuLAND TOF.
        Neu_time.clear();
        Neu_x.clear();
        Neu_y.clear();
        Neu_z.clear();
        nFirstPlaneHits = 0;
        NeuLAND_TOF = 1e99;
        
        for (Int_t k = 0 ;k<nNeuhits; ++k)
        {
            // we make a loop over all hits. if 1<=BarNr<=50, we are in single-plane 1.
            bar_hit = TheLeafs->LandDigi_fPaddleNb->GetValue(k);  // NOTE: fPaddleNb starts at 0, not 1!
             
            if (bar_hit<50) // NOTE: fPaddleNb starts at 0, not 1!
            {
                nFirstPlaneHits = nFirstPlaneHits + 1;
                Neu_time.push_back(TheLeafs->LandDigi_fTdc->GetValue(k));
                Neu_x.push_back(TheLeafs->LandDigi_fxx->GetValue(k));
                Neu_y.push_back(TheLeafs->LandDigi_fyy->GetValue(k));
                Neu_z.push_back(TheLeafs->LandDigi_fzz->GetValue(k));
            }
            
            // Compute full NeuLAND TOF:
            if ((TheLeafs->LandDigi_fTdc->GetValue(k))<NeuLAND_TOF) {NeuLAND_TOF = TheLeafs->LandDigi_fTdc->GetValue(k);}
        }
        Nhits_NeuPlane1 = nFirstPlaneHits;
        
        // Now we must also apply a selection to the VETO hits. Only hits with a TOF before NeuLAND should be taken along.
        VETO_time.clear();
        VETO_x.clear();
        VETO_y.clear();
        VETO_z.clear();
        nVETOfirstHits = 0;
        
        for (Int_t k = 0; k<nVETOhits; ++k)
        {
            if ((TheLeafs->veto_segDigi_fTdc->GetValue(k))<NeuLAND_TOF)
            {
                VETO_time.push_back(TheLeafs->veto_segDigi_fTdc->GetValue(k));
                VETO_x.push_back(TheLeafs->veto_segDigi_fxx->GetValue(k));
                VETO_y.push_back(TheLeafs->veto_segDigi_fyy->GetValue(k));
                VETO_z.push_back(TheLeafs->veto_segDigi_fzz->GetValue(k));
                nVETOfirstHits = nVETOfirstHits + 1;
            }
        }
        Nhits_VETO = nVETOfirstHits;
        
        // So now the only events we should keep is if both nVETOfirstHits==1 & nFirstPlaneHits==1:
        if ((nVETOfirstHits==1)&&(nFirstPlaneHits==1))
        {
            // Compute distance between the hits:
            Distance = (VETO_x.at(0) - Neu_x.at(0))*(VETO_x.at(0) - Neu_x.at(0));
            Distance = (VETO_y.at(0) - Neu_y.at(0))*(VETO_y.at(0) - Neu_y.at(0));
            Distance = (VETO_z.at(0) - Neu_z.at(0))*(VETO_z.at(0) - Neu_z.at(0));
            Distance = TMath::Sqrt(Distance);
            
            // Compute time difference:
            Delta_t = (Neu_time.at(0) - VETO_time.at(0));
            
            // Compute beta:
            Beta_Estimate = (Distance)/(c*Delta_t);
            beta = Beta_Estimate;
            
            // limit beta:
            if (beta>=1.0) {beta = 0.9999;}
            
            // Compute Energy:
            gamma = 1.0/TMath::Sqrt(1.0 - beta*beta);
            
            // FIXME: We now assume that each of those tracks is a proton!
            PDG_Estimate = Inputs->pTracksPDG_NeutronCalibration;
            Energy_Estimate = (gamma - 1.0)*Mproton;
            if (Track_IsIon) {Energy_Estimate = Energy_Estimate/((Int_t) Abeam);}
            
            // Now we make a decision. If we are above beta==1, we will not count
            // the event in energy and PDG, but we do count beta!
            if (Beta_Estimate<1.0)
            {
              // Fill our histograms:
              beta_hist->Fill(Beta_Estimate);
              Energy_hist->Fill(Energy_Estimate);    
              PDG_Estimate = Inputs->pTracksPDG_NeutronCalibration; // FIXME: only these!
            }
            else
            {
              // Just like below (for what we do not count)!
              beta_hist->Fill(Beta_Estimate);
              PDG_Estimate = 0;
              Energy_Estimate = -100.0;
            }
        }
        else
        {
            // In this case we actually do not want anything, but we still need to
            // fill in something in the tree, so be sure it is something that
            // is easily cut away:
            Beta_Estimate = -2.0;
            PDG_Estimate = 0;
            Energy_Estimate = -100.0;
            
            // Do not fill our histograms!
        }
                        
        // Now we can finally fill our tree:
        est_tree->Fill();
        
        // Then we can give a sign of life:
        if ((Event+1)%100==0) {cout << "We processed " << Event+1 << " events.\n";}
        
        // Then close the tree-loop.
    }
    
    // Then write everything to our file:
    cout << "\nTree-loop finished succesfully. We will now write our results to a file.\n\n";
    est_tree->Write("EstimationTree",2);
    beta_hist->Write("Beta_Estimates",2);
    Energy_hist->Write("Energy_Estimates",2);
    cout << "Everything is done. Program is finished.\n\n\n";
    
    // So now we are done. close all blocks:
    }
    }
}

// Now define the main for stand-alone compilation:
#ifndef __CINT__
#include <TApplication.h>
int main(int argc, char** argv)
{
  TApplication* TheApp = new TApplication("Proton_Energy_Estimation",&argc,argv);
  Proton_Energy_Estimation();
  return 0;
}
#endif
