// C++ headers:
#include <iostream>
#include <fstream>
#include <string>

// ROOT headers:
#include "TMath.h"

// Own headers:
#include "../Analysis/Leaf_Connect.h"
#include "../Analysis/Inputs_Load.h"

// Use standard namespace:
using namespace std;

void G4Secondaries()
{
    // This function reads an output-file of a simulation run and then defines an ASCII-file of all
    // strictly secondary particles within that run. This is used to first let a G4 simulation run
    // and then use the result of the G4 run as input for a G3 run.
    
    // Written by C. A. Douma, for the R3B collaboration.
    
    // ============================================================================================
    // We begin by connecting to the files. We need a counter and a series for that. Use defaults first:
    Int_t Counter = 0;
    Int_t Series = 0;
    
    // Now connect to the inputs:
    Inputs_Structure TheInputs;
    Inputs_Structure* Inputs = &TheInputs;
    Bool_t Load = Inputs_Load(Inputs,Counter,Series);
    
    // Series = 0 is fine as it is for what we want. But the counter
    // should be adapted to read the right Monte Carlo file:
    Counter = Inputs->ThisCounter;

    // Now Connect to the Leafs of the Simulation data:
    Leaf_Structure TheLeafsen;
    Leaf_Structure* TheLeafs = &TheLeafsen;
    Bool_t Connect = Leaf_Connect(TheLeafs,Counter,Series);
    
    // Create the output ASCII-file:
    std::ofstream Our_ascii ("../../Data/Inputs/ASCII_Generator.dat", std::ofstream::out); // recreate file
    
    // =============================================================================================
    // Now begin by testing if all the connections worked:
    
    if (!(Load&&Connect&&(Our_ascii.is_open())))
    {
        cout << "### FATAL: Connection to the files of this run failed!\n\n\n";
    }
    else
    {
        cout << "Everything OK. We start now with writing the ASCII-file. Total: " << Inputs->nEvents << " events.\n\n";
        
        // Now we are in business! We can loop over the Monte Carlo file and write each secondary
        // particle to the ASCII-file. but there is a small problem. We first need to know
        // how many particles there are before we write. But only after we are done, we know how
        // may particles we found. That is why we work with an intermediate .txt-file.
        
        // Declare all the variables that we need:
        Long64_t nEvents = Inputs->nEvents;
        Long64_t nBytes = 0;
        Int_t nParticles = 0;
        Double_t pBeam = 0.0;
        Double_t b = 0.0;
        string Line;
        ifstream ReadEvent;
        
        Int_t MC_ArraySize = 0;
        Int_t TrackID = -2;
        Int_t MotherID = -2;
        Int_t MotherMotherID = -2;
        Int_t PDG = 0;
        Double_t tpoint = 0.0;
        Double_t xpoint = 0.0;
        Double_t ypoint = 0.0;
        Double_t zpoint = 0.0;
        Double_t pt = 0.0;
        Double_t px = 0.0;
        Double_t py = 0.0;
        Double_t pz = 0.0;
        Double_t Ekin = 0.0;
        Double_t Mass = 0.0;
        
        Int_t iPid = 0;
        Int_t Awrite = 0;
        Int_t Zwrite = 0;
        Int_t PDGsmall = 0;
        Int_t Z = 0;
        Int_t R = 0;
        Int_t z_Z = 0;
        Int_t z_R = 0;
        Double_t PDG_d = 0.0;
        Double_t Z_d = 0.0;
        Double_t R_d = 0.0;
        Double_t z_Z_d = 0.0;
        Double_t z_R_d = 0.0;
        
        // Loop over the Monte Carlo file:
        for (Long64_t Event = 0; Event<nEvents; ++Event)
        {            
            // Load the current event:
            nBytes = TheLeafs->TheTree->GetEntry(Event);
            
            // Recreate our intermediate txt-file:
            std::ofstream SingleEvent ("../../Data/Inputs/SingleEvent.txt", std::ofstream::out);
            
            // Reset the counter for the number of written particles:
            nParticles = 0;
            
            // Now loop over all Monte Carlo Tracks inside this event:
            MC_ArraySize = TheLeafs->MCTrack_fMass->GetNdata();
            
            // cout << "\n------------------------ Begin of new Event -----------------------------\n";
            
            for (Int_t k = 0; k<MC_ArraySize; ++k)
            {
                // Get all Information about this particle:
                TrackID = k;
                PDG = TheLeafs->MCTrack_fPdgCode->GetValue(k);
                tpoint = TheLeafs->MCTrack_fStartT->GetValue(k);
                xpoint = TheLeafs->MCTrack_fStartT->GetValue(k);
                ypoint = TheLeafs->MCTrack_fStartT->GetValue(k);
                zpoint = TheLeafs->MCTrack_fStartT->GetValue(k);
                px = TheLeafs->MCTrack_fPx->GetValue(k);
                py = TheLeafs->MCTrack_fPy->GetValue(k);
                pz = TheLeafs->MCTrack_fPz->GetValue(k);
                Mass = TheLeafs->MCTrack_fMass->GetValue(k);
                Ekin = TMath::Sqrt(px*px + py*py + pz*pz + Mass*Mass) - Mass;
                pt = Ekin + Mass;
                MotherID = TheLeafs->MCTrack_fMotherId->GetValue(k);
                if (MotherID>=0) {MotherMotherID = TheLeafs->MCTrack_fMotherId->GetValue(MotherID);}
                else {MotherMotherID = -2;}
                
                // Now if the MotherMotherID is equal to -1, then we know that we have a
                // strictly secondary particle, hence we can write it to our intermediate
                // file. Begin by checking this:
                
                // NOTE: We do not add the primary beam. If nothing happened with it and the primary beam is all there is, we should have an empty event in the secondaries! 
                // else, the second run might independently decide to do something with these empty events again and then we bias our results. We also cut off the delta-electrons and gammas.
                if ((MotherMotherID==-1)&&(TMath::Abs(PDG)!=11)&&(PDG!=22)&&(PDG<1000000000)) // FIXME! We should include the ions!
                {
                    // cout << TrackID << " | " << MotherID << " | " << MotherMotherID << " | " << PDG << " | " << tpoint << " | " << xpoint << " | " << ypoint << " | " << zpoint << " | " << px << " | " << py << " | " << pz << " | " << Mass << " | " << Ekin << "\n";

                    // Then we can write it. First distinguish between ions and particle:
                    if (TMath::Abs(PDG)>1000000000)
                    {
                        // Set iPid to -1:
                        iPid = -1;
                        
                        // Extract Z and A:
                        if (PDG>0) {PDGsmall = PDG - 1000000000;}
                        if (PDG<0) {PDGsmall = PDG + 1000000000;}
                        PDG_d = (Int_t) TMath::Abs(PDGsmall);

                        Z_d = PDG_d/10.0;
                        R_d = PDG_d/10000.0;
    
                        z_Z_d = PDG_d/10000.0;
                        z_R_d = PDG_d/10000000.0;
    
                        Z = (Double_t) Z_d;
                        R = (Double_t) R_d;
                       
                        Z_d = Z_d - 1000.0*((Int_t) R);
                        Z = (Double_t) Z_d;
                        Z_d = (Int_t) Z;
    
                        z_Z = (Double_t) z_Z_d;
                        z_R = (Double_t) z_R_d;
                        z_Z_d = z_Z_d - 1000.0*((Int_t) z_R);
                        z_Z = (Double_t) z_Z_d;
                        z_Z_d = (Int_t) z_Z;
    
                        Awrite = Z_d;
                        Zwrite = z_Z_d;
                    }
                    else
                    {
                        // Then we are dealing with an oridinary particle:
                        iPid = 1;
                        Awrite = PDG;
                        Zwrite = 0;
                    }
                    
                    // Then write to our intermediate file:
                    SingleEvent << "    " << iPid << "    " << Zwrite << "    " << Awrite << "    " << px << "    " << py << "    " << pz << "    " << xpoint << "    " << ypoint << "    " << zpoint << "    " << Mass << "\n";
                    // NOTE: Time is now running from the secondaries creation point! Important for the TOF later on!
                    // NOTE: This file is only ment for G3 input, so we do the sequence of A and Z OK right now for that!
                    
                    // Update the counter:
                    nParticles = nParticles + 1;
                }
            }
            
            // So now we have our intermediate file for this event and we have the number of particles.
            // close and re-open the intermediate file to reset all stuff for the reading:
            SingleEvent.close();
            ReadEvent.open("../../Data/Inputs/SingleEvent.txt");
            
            // Now we should write to the ASCII-file:
                    
            // Write the first line:
            Our_ascii << "    " << Event << "    " << nParticles << "    " << pBeam << "    " << b << "\n";
            
            // Then loop over the lines of the intermediate file and write them to the ASCII-file:
            while (getline(ReadEvent,Line) )
            {
                Our_ascii << Line << '\n';
            }
            
            // clean everything on the intermediate file:
            ReadEvent.close();
            
            // That's it!
            if ((Event+1)%1000==0) {cout << "We processed " << Event+1 << " events.\n";}
        }
        
        cout << "\nASCII_file was created succesfully.\n\n\n";
  }
  
  // Finally, close the ASCII-file and then we are done!
  Our_ascii.close();         
  
  // Done!
}
    
// ===========================================================================================================

// Now define the main for stand-alone compilation:

#ifndef __CINT__
#include <TApplication.h>
int main(int argc, char** argv)
{
  TApplication* TheApp = new TApplication("G4Secondaries",&argc,argv);
  G4Secondaries();
  return 0;
}
#endif
