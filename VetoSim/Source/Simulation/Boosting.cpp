// include C++ headers:
#include <iostream>
#include <fstream>

// include required ROOT headers:
#include "TLorentzVector.h"
#include "TRandom3.h"
#include "TMath.h"
#include "TH1D.h"
#include "TVector3.h"

// include our own headers:
#include "../Analysis/Inputs_Load.h"

// Use standard namespace:
using namespace std;

// NOTE: This header generates the total 4-momenta of the particles
// that we fire. It does not write this to an ASCII-file, it simply
// calculates the 4-momenta. In the inputs one can specify 3 different
// beams. In this file, the three different beams are assumed to be
// the result of a mother primary beam (from Super-FRS) colliding with 
// a fixed target. The target is assumed to be at rest and the Mother beam
// is assumed to have an energy equal to the first specified beam. These
// energies are in AGeV, so if one specifies an energy of 1 GeV in SetParameters
// and a mother beam with 100 nucleons, the total nucleon energy will be 
// 100 GeV. The target and the beam are then first boosted to the CM frame.
// then the 4-momenta of the 3 beams are generated inside the CM-frame.
// each nucleon of the outgoing beams gets the same fraction of the total 
// energy available in the CM-frame. Each particle/nuclei is emitted over
// a uniform random sphere in the CM-frame. Then those 4-mometa are boosted
// back into the LAB-frame. These momenta are then the outputs of this header.
// Then another function can write them into an ASCII-file.

// ==================================================================================
// First define an inputs/outputs structure.

struct Beams_Struct {
    
    // Settings:
    Bool_t print;
    
    // First Beam: Given Data:
    Bool_t Beam1_Exists;
    Int_t Beam1_iPid1;
    Int_t Beam1_PDG;
    Int_t Beam1_Z;
    Int_t Beam1_A;
    Int_t Beam1_Mult;
    Double_t Beam1_Mass;
    Double_t Beam1_Xpos;
    Double_t Beam1_Ypos;
    Double_t Beam1_Zpos;
    
    // First beam: Computed Data:
    Double_t* Beam1_Ptime;
    Double_t* Beam1_Px;
    Double_t* Beam1_Py;
    Double_t* Beam1_Pz;
    
    // Second Beam: Given Data:
    Bool_t Beam2_Exists;
    Int_t Beam2_iPid1;
    Int_t Beam2_PDG;
    Int_t Beam2_Z;
    Int_t Beam2_A;
    Int_t Beam2_Mult;
    Double_t Beam2_Mass;
    Double_t Beam2_Xpos;
    Double_t Beam2_Ypos;
    Double_t Beam2_Zpos;
    
    // Second beam: Computed Data:
    Double_t* Beam2_Ptime;
    Double_t* Beam2_Px;
    Double_t* Beam2_Py;
    Double_t* Beam2_Pz;
    
    // Third Beam: Given Data:
    Bool_t Beam3_Exists;
    Int_t Beam3_iPid1;
    Int_t Beam3_PDG;
    Int_t Beam3_Z;
    Int_t Beam3_A;
    Int_t Beam3_Mult;
    Double_t Beam3_Mass;
    Double_t Beam3_Xpos;
    Double_t Beam3_Ypos;
    Double_t Beam3_Zpos;
    
    // Third beam: Computed Data:
    Double_t* Beam3_Ptime;
    Double_t* Beam3_Px;
    Double_t* Beam3_Py;
    Double_t* Beam3_Pz;
    
    // Mother Beam: Given Data:
    Int_t Mother_Z;
    Int_t Mother_A;
    Double_t Mother_Mass;
    Double_t Mother_EkinMin;
    Double_t Mother_EkinMax;
    
    // Mother Beam: Computed Data:
    Double_t Mother_Ptime;
    Double_t Mother_Px;
    Double_t Mother_Py;
    Double_t Mother_Pz;
    
    // Fixed target: Given Data:
    Int_t Target_Z;
    Int_t Target_A;
    Double_t Target_Mass;
    
    // Fixed Target: Computed Data:
    Double_t Target_Ptime;
    Double_t Target_Px;
    Double_t Target_Py;
    Double_t Target_Pz;
    
    // The random generator & other stuff:
    TRandom3* generator;
    Int_t nEvents;
};

// =====================================================================================
// Now we are going to need a function that initializes all the given data.
// It needs no inputs, since they should be taken from the Inputs.root-file.
// The output should be a pointer to the above structure. All variables should be
// initialized. The computed data should be set to zeros, the given data should be filled.

Bool_t Initialize(Beams_Struct* Beams)
{
    // Written by C. A. Douma, for the R3B collaboration.
    
    // First get the inputs-structure. That is easy.
    Inputs_Structure TheInputs;
    Inputs_Structure* Inputs = &TheInputs;
    Int_t Series = 0;
    Int_t Counter = 1;                                                                          // FIXME!!!
    Bool_t Load = Inputs_Load(Inputs,Counter,Series);
    
    // Define the output:
    Bool_t TheOutput = kFALSE;
    
    if (!Load)
    {
        cout << "### ERROR: You did not create the Inputs.root-file!\n"
             << "### ERROR: We cannot initialize the Beams-structure!\n"
             << "### ERROR: Program is terminated now and failure is returned.\n\n";
    }
    else
    {
        // Now we can go for it! Start filling:
        Beams->print = kFALSE;
        
        // ------------------------------------------------------------------------
        // Fill beam 1:
        Beams->Beam1_Exists = kTRUE;
        Beams->Beam1_iPid1 = 1;
        Beams->Beam1_Z = 0;
        Beams->Beam1_A = Inputs->Beam_PDG;
        Beams->Beam1_PDG = Inputs->Beam_PDG;
        Beams->Beam1_Mult = Inputs->Beam_Nparticles;
        Beams->Beam1_Mass = Inputs->Beam_Mass;
        Beams->Beam1_Xpos = Inputs->Beam_Xpos;
        Beams->Beam1_Ypos = Inputs->Beam_Ypos;
        Beams->Beam1_Zpos = Inputs->Beam_Zpos;
        Beams->Beam1_Ptime = new Double_t[Beams->Beam1_Mult];
        Beams->Beam1_Px = new Double_t[Beams->Beam1_Mult];
        Beams->Beam1_Py = new Double_t[Beams->Beam1_Mult];
        Beams->Beam1_Pz = new Double_t[Beams->Beam1_Mult];
        
        for (Int_t k = 0; k<(Beams->Beam1_Mult); ++k)
        {
            Beams->Beam1_Ptime[k] = 0.0;
            Beams->Beam1_Px[k] = 0.0;
            Beams->Beam1_Py[k] = 0.0;
            Beams->Beam1_Pz[k] = 0.0;
        }
        
        // Now if this is an ion, adapt the iPid1, Z and A:    
        Int_t Abeam = -1.0;
        Int_t Zbeam = -1.0;
        Int_t Particle_ID = Inputs->Beam_PDG;
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

          // Fill in:
          Beams->Beam1_A = Abeam;
          Beams->Beam1_Z = Zbeam;
          Beams->Beam1_iPid1 = -1;
        }
        
        // ------------------------------------------------------------------------
        // Fill beam 2:
        Beams->Beam2_Exists = kFALSE;
        if (Inputs->Beam2_Exe==1) {Beams->Beam2_Exists = kTRUE;}
        Beams->Beam2_iPid1 = 1;
        Beams->Beam2_Z = 0;
        Beams->Beam2_A = Inputs->Beam2_PDG;
        Beams->Beam2_PDG = Inputs->Beam2_PDG;
        Beams->Beam2_Mult = Inputs->Beam2_Nparticles;
        Beams->Beam2_Mass = Inputs->Beam2_Mass;
        Beams->Beam2_Xpos = Inputs->Beam_Xpos; // same as beam 1! else it makes no sense...
        Beams->Beam2_Ypos = Inputs->Beam_Ypos; // same as beam 1! else it makes no sense...
        Beams->Beam2_Zpos = Inputs->Beam_Zpos; // same as beam 1! else it makes no sense...
        Beams->Beam2_Ptime = new Double_t[Beams->Beam2_Mult];
        Beams->Beam2_Px = new Double_t[Beams->Beam2_Mult];
        Beams->Beam2_Py = new Double_t[Beams->Beam2_Mult];
        Beams->Beam2_Pz = new Double_t[Beams->Beam2_Mult];
        
        for (Int_t k = 0; k<(Beams->Beam2_Mult); ++k)
        {
            Beams->Beam2_Ptime[k] = 0.0;
            Beams->Beam2_Px[k] = 0.0;
            Beams->Beam2_Py[k] = 0.0;
            Beams->Beam2_Pz[k] = 0.0;
        }
        
        // Now if this is an ion, adapt the iPid1, Z and A:    
        Abeam = -1.0;
        Zbeam = -1.0;
        Particle_ID = Inputs->Beam2_PDG;
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

          // Fill in:
          Beams->Beam2_A = Abeam;
          Beams->Beam2_Z = Zbeam;
          Beams->Beam2_iPid1 = -1;
        }
        
        // ------------------------------------------------------------------------
        // Fill beam 3:
        Beams->Beam3_Exists = kFALSE;
        if (Inputs->Beam3_Exe==1) {Beams->Beam3_Exists = kTRUE;}
        Beams->Beam3_iPid1 = 1;
        Beams->Beam3_Z = 0;
        Beams->Beam3_A = Inputs->Beam3_PDG;
        Beams->Beam3_PDG = Inputs->Beam3_PDG;
        Beams->Beam3_Mult = Inputs->Beam3_Nparticles;
        Beams->Beam3_Mass = Inputs->Beam3_Mass;
        Beams->Beam3_Xpos = Inputs->Beam_Xpos; // same as beam 1! else it makes no sense...
        Beams->Beam3_Ypos = Inputs->Beam_Ypos; // same as beam 1! else it makes no sense...
        Beams->Beam3_Zpos = Inputs->Beam_Zpos; // same as beam 1! else it makes no sense...
        Beams->Beam3_Ptime = new Double_t[Beams->Beam3_Mult];
        Beams->Beam3_Px = new Double_t[Beams->Beam3_Mult];
        Beams->Beam3_Py = new Double_t[Beams->Beam3_Mult];
        Beams->Beam3_Pz = new Double_t[Beams->Beam3_Mult];
        
        for (Int_t k = 0; k<(Beams->Beam3_Mult); ++k)
        {
            Beams->Beam3_Ptime[k] = 0.0;
            Beams->Beam3_Px[k] = 0.0;
            Beams->Beam3_Py[k] = 0.0;
            Beams->Beam3_Pz[k] = 0.0;
        }
        
        // Now if this is an ion, adapt the iPid1, Z and A:    
        Abeam = -1.0;
        Zbeam = -1.0;
        Particle_ID = Inputs->Beam3_PDG;
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

          // Fill in:
          Beams->Beam3_A = Abeam;
          Beams->Beam3_Z = Zbeam;
          Beams->Beam3_iPid1 = -1;
        }
        
        // ------------------------------------------------------------------------
        // Fill Mother beam:
        Beams->Mother_Z = 0;
        Beams->Mother_A = Inputs->MotherBeamPDG;
        Beams->Mother_Mass = Inputs->MotherBeamMass;
        Beams->Mother_EkinMin = Inputs->Beam_Emin;
        Beams->Mother_EkinMax = Inputs->Beam_Emax;
        Beams->Mother_Ptime = 0.0;
        Beams->Mother_Px = 0.0;
        Beams->Mother_Py = 0.0;
        Beams->Mother_Pz = 0.0;
        
        // Now if this is an ion, adapt the iPid1, Z and A:    
        Abeam = -1.0;
        Zbeam = -1.0;
        Particle_ID = Inputs->MotherBeamPDG;
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

          // Fill in:
          Beams->Mother_A = Abeam;
          Beams->Mother_Z = Zbeam;
          Beams->Mother_EkinMin = (Beams->Mother_EkinMin)*((Int_t) Abeam); // multiply with number of nucleons!
          Beams->Mother_EkinMax = (Beams->Mother_EkinMax)*((Int_t) Abeam); // multiply with number of nucleons!
        }
        
        // ------------------------------------------------------------------------
        // Fill Target:
        Beams->Target_Z = 0;
        Beams->Target_A = Inputs->Target_PDG;
        Beams->Target_Mass = Inputs->Target_Mass;
        Beams->Target_Ptime = 0.0;
        Beams->Target_Px = 0.0;
        Beams->Target_Py = 0.0;
        Beams->Target_Pz = 0.0;
        
        // Now if this is an ion, adapt the iPid1, Z and A:    
        Abeam = -1.0;
        Zbeam = -1.0;
        Particle_ID = Inputs->Target_PDG;
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

          // Fill in:
          Beams->Target_A = Abeam;
          Beams->Target_Z = Zbeam;
        }
        
        // -------------------------------------------------------------------------
        // finally: create the random generator:
        Beams->generator = new TRandom3(0);
        Beams->nEvents = Inputs->nEvents;
        
        // -------------------------------------------------------------------------
        // Done! Everything is now properly initialized. Hence close all blocks and
        // return the boolian:
        TheOutput = kTRUE;
    }
    
    return TheOutput;
    
}
    
// ==========================================================================================
// Now we must create a function that will compute the required 4-momenta in the structure.
// The initialize-function is called only once, but this one will be called for every event.
// Hence it must be fast & optimal. For random number generation, the generator inside The
// structure should be used, so that the seeding works properly.

void ComputeMomenta(Beams_Struct* Beams)
{
    // Computes the 4-momenta inside the structure as given. Every time this function is
    // called, the answers will be different, since the generator is used.
    
    // written by C. A. Douma, for the R3B collaboration.
    
    // ========================================================================
    if (Beams->print) {cout << "----------- Begin of Event --------------\n";}
    
    // begin by assigning the correct 4-momenta to the Mother-Beam and to the
    // target. It is assumed that the target is at rest in the LAB-frame and
    // the Mother beam flies along the z-axis.
    
    // target:
    Beams->Target_Ptime = Beams->Target_Mass; // NOTE: everything is in [GeV]!
    Beams->Target_Px = 0.0;
    Beams->Target_Py = 0.0;
    Beams->Target_Pz = 0.0;
    
    // Mother beam:
    Double_t Ekin = Beams->Mother_EkinMin + Beams->generator->Uniform(Beams->Mother_EkinMax - Beams->Mother_EkinMin);
    Beams->Mother_Ptime = Beams->Mother_Mass + Ekin;
    Beams->Mother_Pz = TMath::Sqrt((Beams->Mother_Ptime)*(Beams->Mother_Ptime) - (Beams->Mother_Mass)*(Beams->Mother_Mass));
    Beams->Mother_Px = 0.0;
    Beams->Mother_Py = 0.0;
    
    // create the Lorentz Vectors before the collision:
    TLorentzVector Mother;
    TLorentzVector Target;
    TLorentzVector Total;
    
    Mother.SetT(Beams->Mother_Ptime);
    Mother.SetX(Beams->Mother_Px);
    Mother.SetY(Beams->Mother_Py);
    Mother.SetZ(Beams->Mother_Pz);
    
    Target.SetT(Beams->Target_Ptime);
    Target.SetX(Beams->Target_Px);
    Target.SetY(Beams->Target_Py);
    Target.SetZ(Beams->Target_Pz);
    
    Total.SetT(Beams->Mother_Ptime + Beams->Target_Ptime);
    Total.SetX(Beams->Mother_Px + Beams->Target_Px);
    Total.SetY(Beams->Mother_Py + Beams->Target_Py);
    Total.SetZ(Beams->Mother_Pz + Beams->Target_Pz);
    
    if (Beams->print) {cout << "Mother Beam inv. mass before boost = " << Mother.M() << " | Mother Beam initialized mass before boost = "  << Beams->Mother_Mass << " | CM-frame total inv. mass before boost = " << Total.M() << "\n";}
    
    // Compute beta:
    Double_t beta = (TMath::Sqrt(Total.X()*Total.X() + Total.Y()*Total.Y() + Total.Z()*Total.Z()))/(Total.T());
    
    // Compute theta & phi:
    Double_t theta = Total.Theta();
    Double_t phi = Total.Phi();
    
    // boost to CM-frame:
    Total.RotateZ(-1.0*phi);
    Target.RotateZ(-1.0*phi);
    Mother.RotateZ(-1.0*phi);
    
    Total.RotateY(-1.0*theta);
    Target.RotateY(-1.0*theta);
    Mother.RotateY(-1.0*theta);
    
    Total.Boost(0.0,0.0,-1.0*beta);
    Target.Boost(0.0,0.0,-1.0*beta);
    Mother.Boost(0.0,0.0,-1.0*beta);
    
    if (Beams->print) {cout << "Mother Beam inv. mass after boost = " << Mother.M() << " | Mother Beam initialized mass after boost = " << Beams->Mother_Mass << " | CM-frame total inv. mass after boost = " << Total.M() << "\n";}
    
    // Check that the boost is succesful:
    if (TMath::Abs(Total.X())>0.1e-7) {cout << "### ERROR: Boost failed in X! Recheck your code!!!\n";}
    if (TMath::Abs(Total.Y())>0.1e-7) {cout << "### ERROR: Boost failed in Y! Recheck your code!!!\n";}
    if (TMath::Abs(Total.Z())>0.1e-7) {cout << "### ERROR: Boost failed in Z! Recheck your code!!!\n";}
    
    // Then get the total amount of energy (incl. masses!):
    Double_t Etot = Total.T();
    
    // Now this must be divided among all nucleons!
    Int_t N_nucleons = 0;
    if (Beams->Beam1_iPid1>0) {N_nucleons = N_nucleons + Beams->Beam1_Mult;} else {N_nucleons = N_nucleons + (Beams->Beam1_Mult)*(Beams->Beam1_A);}
    if (Beams->Beam2_Exists) {if (Beams->Beam2_iPid1>0) {N_nucleons = N_nucleons + Beams->Beam2_Mult;} else {N_nucleons = N_nucleons + (Beams->Beam2_Mult)*(Beams->Beam2_A);}}
    if (Beams->Beam3_Exists) {if (Beams->Beam3_iPid1>0) {N_nucleons = N_nucleons + Beams->Beam3_Mult;} else {N_nucleons = N_nucleons + (Beams->Beam3_Mult)*(Beams->Beam3_A);}}
    
    if (Beams->print) {
    cout << "theta-boost = " << theta << " [rad]\n";
    cout << "phi-boost = " << phi << " [rad]\n";
    cout << "beta-boost = " << beta << " [dim.less]\n";
    cout << "Ekin = " << Ekin << " [GeV]\n";
    cout << "Beam Mass = " << Beams->Mother_Mass << " [GeV]\n";
    cout << "Target Mass = " << Beams->Target_Mass << " [GeV]\n";
    cout << "Etot before boost = " << Beams->Mother_Mass + Beams->Target_Mass + Ekin << " [GeV]\n";
    cout << "Etot after boost = " << Etot << " [GeV]\n";
    cout << "#nucleons = " << N_nucleons << "\n\n";}
    
    // Hence the total energy per nucleon available is:
    Double_t Enuc = Etot/((Int_t) N_nucleons);
    
    if (Beams->print) {
    cout << "Energy per mucleon = " << Enuc << " [GeV]\n"
         << "Beam1 Mass = " << Beams->Beam1_Mass << " | Beam1 A = " << Beams->Beam1_A << " | Beam1 iPid1 = " << Beams->Beam1_iPid1 << "\n"
         << "Beam2 Mass = " << Beams->Beam2_Mass << " | Beam2 A = " << Beams->Beam2_A << " | Beam2 iPid1 = " << Beams->Beam2_iPid1 << "\n"
         << "Beam3 Mass = " << Beams->Beam3_Mass << " | Beam3 A = " << Beams->Beam3_A << " | Beam3 iPid1 = " << Beams->Beam3_iPid1 << "\n";}

    // Check that this is a realistic amount:
    if (Beams->Beam1_iPid1>0)
    {
        if (!(Enuc>Beams->Beam1_Mass)) 
        {
            cout << "### ERROR: Not enough energy per nucleon for beam 1! <Particle Beam> You simulate an unphysical experiment!!!\n";
        }
    }
    else
    {
        if (!(Enuc*((Int_t) Beams->Beam1_A)>Beams->Beam1_Mass))
        {
            cout << "### ERROR: Not enough energy per nucleon for beam 1! <Nuclear Beam> You simulate an unphysical experiment!!!\n";
        }
    }
    
    if (Beams->Beam2_Exists)
    {
        if (Beams->Beam2_iPid1>0)
        {
            if (!(Enuc>Beams->Beam2_Mass)) 
            {
                cout << "### ERROR: Not enough energy per nucleon for beam 2! <Particle Beam> You simulate an unphysical experiment!!!\n";
            }
        }
        else
        {
            if (!(Enuc*((Int_t) Beams->Beam2_A)>Beams->Beam2_Mass))
            {
                cout << "### ERROR: Not enough energy per nucleon for beam 2! <Nuclear Beam> You simulate an unphysical experiment!!!\n";
            }
        }
    }
    
    if (Beams->Beam3_Exists)
    {
        if (Beams->Beam3_iPid1>0)
        {
            if (!(Enuc>Beams->Beam3_Mass)) 
            {
                cout << "### ERROR: Not enough energy per nucleon for beam 3! <Particle Beam> You simulate an unphysical experiment!!!\n";
            }
        }
        else
        {
            if (!(Enuc*((Int_t) Beams->Beam3_A)>Beams->Beam3_Mass))
            {
                cout << "### ERROR: Not enough energy per nucleon for beam 3! <Nuclear Beam> You simulate an unphysical experiment!!!\n";
            }
        }
    }
        
    // So now we can begin by creating the Lorentz Vectors for the beams.
   
    // ------------------------------------------------------------------
    // Do beam 1 first:
    
    Double_t* Energy_B1 = new Double_t[Beams->Beam1_Mult];
    Double_t* Theta_B1 = new Double_t[Beams->Beam1_Mult];
    Double_t* Phi_B1 = new Double_t[Beams->Beam1_Mult];
    Double_t* P3_B1 = new Double_t[Beams->Beam1_Mult];
    TLorentzVector* Lorentz_B1 = new TLorentzVector[Beams->Beam1_Mult];
    
    for (Int_t k = 0; k<(Beams->Beam1_Mult); ++k)
    {
        if (Beams->Beam1_iPid1>0) {Energy_B1[k] = Enuc;} else {Energy_B1[k] = Enuc*((Int_t) Beams->Beam1_A);}
        
        Theta_B1[k] = -1.0 + Beams->generator->Uniform(2.0);
        Theta_B1[k] = TMath::ACos(Theta_B1[k]);
        
        Phi_B1[k] = Beams->generator->Uniform(2.0*TMath::Pi());
        
        P3_B1[k] = TMath::Sqrt(Energy_B1[k]*Energy_B1[k] - (Beams->Beam1_Mass)*(Beams->Beam1_Mass));
        
        Lorentz_B1[k].SetT(Energy_B1[k]);
        Lorentz_B1[k].SetX(P3_B1[k]*TMath::Sin(Theta_B1[k])*TMath::Cos(Phi_B1[k]));
        Lorentz_B1[k].SetY(P3_B1[k]*TMath::Sin(Theta_B1[k])*TMath::Sin(Phi_B1[k]));
        Lorentz_B1[k].SetZ(P3_B1[k]*TMath::Cos(Theta_B1[k]));
        
        if (Beams->print) {cout << "Beam1 Generated theta = " << 180.0*Theta_B1[k]/TMath::Pi() << " [deg]\n";}
        if (Beams->print) {cout << "Beam1 Lorentz theta before boost = " << 180.0*Lorentz_B1[k].Theta()/TMath::Pi() << " [deg]\n";}
    }
    
    // ------------------------------------------------------------------
    // Then beam 2:

    Double_t* Energy_B2 = new Double_t[Beams->Beam2_Mult];
    Double_t* Theta_B2 = new Double_t[Beams->Beam2_Mult];
    Double_t* Phi_B2 = new Double_t[Beams->Beam2_Mult];
    Double_t* P3_B2 = new Double_t[Beams->Beam2_Mult];
    TLorentzVector* Lorentz_B2 = new TLorentzVector[Beams->Beam2_Mult];
    
    if (Beams->Beam2_Exists)
    {
    
    for (Int_t k = 0; k<(Beams->Beam2_Mult); ++k)
    {
        if (Beams->Beam2_iPid1>0) {Energy_B2[k] = Enuc;} else {Energy_B2[k] = Enuc*((Int_t) Beams->Beam2_A);}
        
        Theta_B2[k] = -1.0 + Beams->generator->Uniform(2.0);
        Theta_B2[k] = TMath::ACos(Theta_B2[k]);
        
        Phi_B2[k] = Beams->generator->Uniform(2.0*TMath::Pi());
        
        P3_B2[k] = TMath::Sqrt(Energy_B2[k]*Energy_B2[k] - (Beams->Beam2_Mass)*(Beams->Beam2_Mass));
        
        Lorentz_B2[k].SetT(Energy_B2[k]);
        Lorentz_B2[k].SetX(P3_B2[k]*TMath::Sin(Theta_B2[k])*TMath::Cos(Phi_B2[k]));
        Lorentz_B2[k].SetY(P3_B2[k]*TMath::Sin(Theta_B2[k])*TMath::Sin(Phi_B2[k]));
        Lorentz_B2[k].SetZ(P3_B2[k]*TMath::Cos(Theta_B2[k]));
    }
        
    }
    
    // ------------------------------------------------------------------
    // Then beam 3:

    Double_t* Energy_B3 = new Double_t[Beams->Beam3_Mult];
    Double_t* Theta_B3 = new Double_t[Beams->Beam3_Mult];
    Double_t* Phi_B3 = new Double_t[Beams->Beam3_Mult];
    Double_t* P3_B3 = new Double_t[Beams->Beam3_Mult];
    TLorentzVector* Lorentz_B3 = new TLorentzVector[Beams->Beam3_Mult];
    
    if (Beams->Beam3_Exists)
    {
    
    for (Int_t k = 0; k<(Beams->Beam3_Mult); ++k)
    {
        if (Beams->Beam3_iPid1>0) {Energy_B3[k] = Enuc;} else {Energy_B3[k] = Enuc*((Int_t) Beams->Beam3_A);}
        
        Theta_B3[k] = -1.0 + Beams->generator->Uniform(2.0);
        Theta_B3[k] = TMath::ACos(Theta_B3[k]);
        
        Phi_B3[k] = Beams->generator->Uniform(2.0*TMath::Pi());
        
        P3_B3[k] = TMath::Sqrt(Energy_B3[k]*Energy_B3[k] - (Beams->Beam3_Mass)*(Beams->Beam3_Mass));
        
        Lorentz_B3[k].SetT(Energy_B3[k]);
        Lorentz_B3[k].SetX(P3_B3[k]*TMath::Sin(Theta_B3[k])*TMath::Cos(Phi_B3[k]));
        Lorentz_B3[k].SetY(P3_B3[k]*TMath::Sin(Theta_B3[k])*TMath::Sin(Phi_B3[k]));
        Lorentz_B3[k].SetZ(P3_B3[k]*TMath::Cos(Theta_B3[k]));
    }
        
    }
    
    // -------------------------------------------------------------------
   
    // Now all these Lorentz Vectors are created within the CM-frame. Hence
    // we should boost them to the LAB-frame:
    
    for (Int_t k = 0; k<(Beams->Beam1_Mult); ++k)
    {
        Lorentz_B1[k].Boost(0.0,0.0,beta);
        Lorentz_B1[k].RotateY(theta);
        Lorentz_B1[k].RotateZ(phi);
        
        if (Beams->print) {cout << "Beam1 Lorentz theta after boost = " << 180.0*Lorentz_B1[k].Theta()/TMath::Pi() << " [deg]\n";}
    }
    
    if (Beams->Beam2_Exists)
    {
        for (Int_t k = 0; k<(Beams->Beam2_Mult); ++k)
        {
            Lorentz_B2[k].Boost(0.0,0.0,beta);
            Lorentz_B2[k].RotateY(theta);
            Lorentz_B2[k].RotateZ(phi);
        }
    }
    
    if (Beams->Beam3_Exists)
    {
        for (Int_t k = 0; k<(Beams->Beam3_Mult); ++k)
        {
            Lorentz_B3[k].Boost(0.0,0.0,beta);
            Lorentz_B3[k].RotateY(theta);
            Lorentz_B3[k].RotateZ(phi);
        }
    }
        
    // --------------------------------------------------------------------
    
    // Now that these boosts are all done, we can put them all into
    // our structure. Target & Mother are already done, so now we
    // have to do the 3 beams:
    
    for (Int_t k = 0; k<(Beams->Beam1_Mult); ++k)
    {
        Beams->Beam1_Ptime[k] = Lorentz_B1[k].T();
        Beams->Beam1_Px[k] = Lorentz_B1[k].X();
        Beams->Beam1_Py[k] = Lorentz_B1[k].Y();
        Beams->Beam1_Pz[k] = Lorentz_B1[k].Z();
    }
    
    if (Beams->Beam2_Exists)
    {
        for (Int_t k = 0; k<(Beams->Beam2_Mult); ++k)
        {
            Beams->Beam2_Ptime[k] = Lorentz_B2[k].T();
            Beams->Beam2_Px[k] = Lorentz_B2[k].X();
            Beams->Beam2_Py[k] = Lorentz_B2[k].Y();
            Beams->Beam2_Pz[k] = Lorentz_B2[k].Z();
        }
    }
    
    if (Beams->Beam3_Exists)
    {
        for (Int_t k = 0; k<(Beams->Beam3_Mult); ++k)
        {
            Beams->Beam3_Ptime[k] = Lorentz_B3[k].T();
            Beams->Beam3_Px[k] = Lorentz_B3[k].X();
            Beams->Beam3_Py[k] = Lorentz_B3[k].Y();
            Beams->Beam3_Pz[k] = Lorentz_B3[k].Z();
        }
    }
    // -------------------------------------------------------------------
    
    // Now that completes our job. Hence finish up with the right deletes:
    
    delete Energy_B1;
    delete Theta_B1;
    delete Phi_B1;
    delete P3_B1;
    //delete[] Lorentz_B1;
    
    delete Energy_B2;
    delete Theta_B2;
    delete Phi_B2;
    delete P3_B2;
    //delete Lorentz_B2;

    delete Energy_B3;
    delete Theta_B3;
    delete Phi_B3;
    delete P3_B3;
    //delete Lorentz_B3;
    
    // -------------------------------------------------------------------
    
    // Done!
    if (Beams->print) {cout << "-------------- End of Event ----------------\n\n\n";}
}
    
    
// ============================================================================================
// Now all these functions have been assigned correctly, all we need is a function to
// actually write the data into a textfile.

void Create_ASCII_File()
{
    cout << "=============== Generating ASCII-file of realistic distributions...\n\n";
    
    // Initialize our structure:
    cout << "Initializing Beam structure...\n\n";
    Beams_Struct TheBeams;
    Beams_Struct* Beams = &TheBeams;
    Bool_t Succes = Initialize(Beams);
    
    if (!Succes)
    {
        cout << "### ERROR: Beam structure could not be created! Program is terminated.\n\n\n";
    }
    else
    {

    cout << "Beams-structure was created succesfully.\n\n";
    
    // Then create the ascii-file:
    std::ofstream Our_ascii ("../../Data/Inputs/ASCII_Generator.dat", std::ofstream::out); // recreate file
    
    // create control histograms:
    TH1D* h1 = new TH1D("Beam 1 Angular Spread in degrees","Beam 1 Angular Spread in degrees",3600,-180.0,180.0);
    TH1D* h2 = new TH1D("Beam 2 Angular Spread in degrees","Beam 2 Angular Spread in degrees",3600,-180.0,180.0);
    TH1D* h3 = new TH1D("Beam 3 Angular Spread in degrees","Beam 3 Angular Spread in degrees",3600,-180.0,180.0);
    
    // Then loop over the amount of events:
    Int_t nEvents = Beams->nEvents;
    Int_t nTracks = Beams->Beam1_Mult;
    if (Beams->Beam2_Exists) {nTracks = nTracks + Beams->Beam2_Mult;}
    if (Beams->Beam3_Exists) {nTracks = nTracks + Beams->Beam3_Mult;}
    Double_t pBeam = 0.0;
    Double_t b = 0.0;
    TVector3 vec;
    
    // re-initialize the generator (I do not know why...)
    Beams->generator = new TRandom3(0);
    
    cout << "ASCII-file & Control histograms were created succesfully.\n\n"
         << "We now start the Event loop of nEvents = " << nEvents << "\n\n";
    
    for (Int_t Event = 0; Event<nEvents; ++Event)
    {
        // Begin by writing the first line:
        Our_ascii << "    " << Event << "    " << nTracks << "    " << pBeam << "    " << b << "\n";
             
        // Create momenta:
        ComputeMomenta(Beams);
        
        // Then write the first beam:
        for (Int_t k = 0; k<(Beams->Beam1_Mult); ++k)
        {
            Our_ascii << "    " << Beams->Beam1_iPid1 << "    " << Beams->Beam1_Z << "    " << Beams->Beam1_A << "    " << Beams->Beam1_Px[k] << "    " << Beams->Beam1_Py[k] << "    " << Beams->Beam1_Pz[k] << "    " << Beams->Beam1_Xpos << "    " << Beams->Beam1_Ypos << "    " << Beams->Beam1_Zpos << "    " << Beams->Beam1_Mass << "\n";
            
            vec.SetX(Beams->Beam1_Px[k]);
            vec.SetY(Beams->Beam1_Py[k]);
            vec.SetZ(Beams->Beam1_Pz[k]);
            h1->Fill(vec.Theta()*180.0/(TMath::Pi()));
        }
    
        // Then write the second beam:
        if (Beams->Beam2_Exists) {
        for (Int_t k = 0; k<(Beams->Beam2_Mult); ++k)
        {
            Our_ascii << "    " << Beams->Beam2_iPid1 << "    " << Beams->Beam2_Z << "    " << Beams->Beam2_A << "    " << Beams->Beam2_Px[k] << "    " << Beams->Beam2_Py[k] << "    " << Beams->Beam2_Pz[k] << "    " << Beams->Beam2_Xpos << "    " << Beams->Beam2_Ypos << "    " << Beams->Beam2_Zpos << "    " << Beams->Beam2_Mass << "\n";
            
            vec.SetX(Beams->Beam2_Px[k]);
            vec.SetY(Beams->Beam2_Py[k]);
            vec.SetZ(Beams->Beam2_Pz[k]);
            h2->Fill(vec.Theta()*180.0/(TMath::Pi()));
        }}
        
        // Then write the third beam:
        if (Beams->Beam2_Exists) {
        for (Int_t k = 0; k<(Beams->Beam3_Mult); ++k)
        {
            Our_ascii << "    " << Beams->Beam3_iPid1 << "    " << Beams->Beam3_Z << "    " << Beams->Beam3_A << "    " << Beams->Beam3_Px[k] << "    " << Beams->Beam3_Py[k] << "    " << Beams->Beam3_Pz[k] << "    " << Beams->Beam3_Xpos << "    " << Beams->Beam3_Ypos << "    " << Beams->Beam3_Zpos << "    " << Beams->Beam3_Mass << "\n";
            
            vec.SetX(Beams->Beam3_Px[k]);
            vec.SetY(Beams->Beam3_Py[k]);
            vec.SetZ(Beams->Beam3_Pz[k]);
            h3->Fill(vec.Theta()*180.0/(TMath::Pi()));
        }}
        
        // That's about all. Give a sign of life:
        if ((Event+1)%1000==0) {cout << "We processed " << Event+1 << " events.\n";}
    }
    
    cout << "\n\nEvent-loop finished succesfully.\n\n";
    
    // Now finish up. Close the ASCII-file:
    Our_ascii.close();
        
    // Write the control histograms to a file:
    TFile* f = new TFile("../../Data/Inputs/ASCII_ControlHistograms.root","recreate");
    h1->Write("B1_Theta",2);
    h2->Write("B2_Theta",2);
    h3->Write("B3_Theta",2);
    f->Close();
    
    if (Beams->print)
    {
        h1->SetLineColor(1);
        h1->Draw();
        h2->SetLineColor(2);
        h2->Draw("same");
        h3->SetLineColor(3);
        h3->Draw("same");
    }
        
    // Done!
    cout << "ASCII-file & Control histograms were created & saved succesfully.\nProgram is now finished.\n\n\n";
    }
}

// Now define the main for stand-alone compilation:
#ifndef __CINT__
#include <TApplication.h>
int main(int argc, char** argv)
{
  TApplication* TheApp = new TApplication("Create_ASCII_File",&argc,argv);
  Create_ASCII_File();
  return 0;
}
#endif
