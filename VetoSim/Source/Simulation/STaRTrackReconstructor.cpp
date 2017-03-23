#include <iostream>
#include <fstream>

#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TBranchElement.h"
#include "TLeaf.h"
#include "TH1D.h"
#include "TH1F.h"
#include "TH2D.h"
#include "TH2F.h"
#include "TMath.h"
#include "TStyle.h"
#include "TROOT.h"
#include "TString.h"

using namespace std;

//  -------------------------------------------------------------------------
//
//   ----- General Macro for R3B StarTracker Analysis results
//         Author: Marc Labiche <marc.labiche@stfc.ac.uk>
//         Last Update: 02/05/14
//         Comments: from RecResultsGene 
//			Include strip clustering
//		       
//	
//  -------------------------------------------------------------------------
//
//   Usage: 
//      > .L RecResultsClust.C
//      > RecResultsClust(mode,"outputName.root")
//
//     mode=0 for pp or pp' reaction channels
//     mode=1 for p2p reaction channel        
//                 
//  -------------------------------------------------------------------------

// ===============================================================================================================================================
Double_t GetThetaTrk(Double_t X, Double_t Y, Double_t Z){
	
	Double_t Theta;
	
	Theta=atan2(sqrt(X*X+Y*Y), Z);   // atan2(double Y, double X)
	if(Theta<0)Theta=Theta+TMath::Pi();

	//if(Z>0)Theta=atan(sqrt(X*X+Y*Y)/Z);
	//if(Z<0)Theta=pi+atan(sqrt(X*X+Y*Y)/Z);

	return Theta;
}



/////..............oooooOOOOOOOoooooo............oooooooOOOOOOOOoooooooo................//////////
/////..............oooooOOOOOOOoooooo............oooooooOOOOOOOOoooooooo................//////////


Double_t GetPhiTrk(Double_t X, Double_t Y){
	
	Double_t Phi;
	
	Phi=atan2(Y,X);
	if(Phi<0)Phi=Phi+2*TMath::Pi();


	return Phi;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// From Nick:
// Calculate Theta in X or Y direction

Float_t GetThetaTrack(Float_t X, Float_t Y){

  Float_t Theta;

  Theta=atan(X/Y);

  return Theta;
}



// Calculate Scattering angle

Float_t GetThetaScat(Float_t X, Float_t Y, Float_t Z){
  
  Float_t Theta;
  
  Theta=atan(sqrt(pow(X,2)+pow(Y,2))/Z);
  
  return Theta;
}



// Calculate Phi

Float_t GetPhiScat(Float_t X, Float_t Y, Float_t Z){
  
  Float_t Phi = 0.0;
  
  if(X>0 && Y>0)Phi=1.5*TMath::Pi()+atan(Y/X);
  if(X<0 && Y>=0)Phi=TMath::PiOver2()+atan(Y/X);
  if(X<0 && Y<0)Phi=TMath::PiOver2()+atan(Y/X);
  if(X>0 && Y<=0)Phi=1.5*TMath::Pi()+atan(Y/X);

  if(X=0 && Y>0)Phi=1.5*TMath::Pi()+TMath::Pi()/2;
  if(X=0 && Y<0)Phi=TMath::PiOver2()+TMath::Pi()/2;

  
  return Phi;
}



// Calculate Energy from momenta

Float_t GetEnergy(Float_t X, Float_t Y, Float_t Z, Float_t M){
  
  Float_t E;
  Float_t T;
  Float_t P;
  Float_t M2; // GeV/C^2
  Float_t amugev = 931.5016/1000;

  // E=(pow(X,2)+pow(Y,2)+pow(Z,2))/(2.*M); //Non-relativistic

  P=sqrt(pow(X,2) + pow(Y,2) + pow(Z,2));
  E=sqrt(pow(P,2) + pow(1.007825*amugev,2));
  T=E-1.007825*amugev;
 

  return T;

  //return E;
}





// for elastic:
Double_t GetERec(Double_t Ep, Double_t Th, Double_t Phi, Double_t Ebeam){

  float pb,p3l,p3x,p3y,p3z;
  float prec,erec,trec;
  float amumev = 931.5016;
  float Ex = 0.;

  pb = sqrt((12*amumev*2*Ebeam*1000) + pow(Ebeam*1000,2));
	  
  p3l = sqrt(pow((Ep*1000+1.007825*amumev),2) - pow((1.007825*amumev+Ex),2));

  p3x = p3l*cos(Phi)*sin(Th);
  p3y = p3l*sin(Phi)*sin(Th);
  p3z = p3l*cos(Th);
  
  prec = sqrt(pow(p3x,2) + pow(p3y,2) +pow((pb-p3z),2));
  
  erec = sqrt(pow(prec,2) + pow(12*amumev,2))/1000.;
  
  trec = erec - 12*amumev/1000;

  return trec;

}

// for p2p:
Float_t GetERec(Float_t Ep3, Float_t Ep4, Float_t Th3, Float_t Phi3, Float_t Th4, Float_t Phi4, Float_t Ebeam){

  Float_t pb,p3l,p3x,p3y,p3z;
  Float_t p4l,p4x,p4y,p4z;
  Float_t prec,erec,trec;
  Float_t amugev = 931.5016/1000.;
  Float_t Ex = 0.;

  pb = sqrt((12*amugev*2*Ebeam) + pow(Ebeam,2));
	  
  p3l = sqrt(pow((Ep3+1.007825*amugev),2) - pow((1.007825*amugev+Ex),2));
  p4l = sqrt(pow((Ep4+1.007825*amugev),2) - pow((1.007825*amugev+Ex),2));

  p3x = p3l*cos(Phi3)*sin(Th3);
  p3y = p3l*sin(Phi3)*sin(Th3);
  p3z = p3l*cos(Th3);
  
  p4x = p4l*cos(Phi4)*sin(Th4);
  p4y = p4l*sin(Phi4)*sin(Th4);
  p4z = p4l*cos(Th4);

  prec = sqrt(pow((p3x+p4x),2) + pow((p3y+p4y),2) +pow((pb-p3z-p4z),2));
  
  erec = sqrt(pow(prec,2) + pow(11.0093054*amugev,2));
  
  trec = erec - 11.0093054*amugev;

  return trec;

}


Float_t GetThetaCM(Float_t th3l){

  Float_t m[5] = {0.,12.,1.007825,1.007825,12.};
  Float_t mass[5] = {0.,0.,0.,0.,0.};
  Float_t y_int,y_new,y_loop,y_adj;
  Float_t betac,beta3c,beta3l;
  Float_t gamci2,gam3cf,gam3c2;
  Float_t a,b,c,d;
  Float_t amugev = 931.5016/1000.;
  Float_t ebeam = 4.8;
  Float_t e3cm,ecmi,ecmf;
  Float_t th3cm = 0.0;
  Float_t Q = 0.;

  for(int j=0; j<5; j++) mass[j]=m[j]*amugev;

  y_int=mass[1]+mass[2];
  
  betac=sqrt(ebeam*1000.*(ebeam+2.*mass[1]))/(y_int+ebeam); // beam energy in MeV
    
  gamci2=1.-pow(betac,2);
    
  ecmi=sqrt(pow(y_int,2)+2.*ebeam*mass[2]);
    
  ecmf=ecmi+Q-y_int+mass[3]+mass[4];
    
  e3cm=(pow(ecmf,2)+pow(mass[3],2)-pow(mass[4],2))/(2.*ecmf); // centre of Mass energy of particle 3
    
  gam3c2=pow((e3cm/mass[3]),2);	      
    
  beta3c=sqrt(1.-1./gam3c2);
    
  y_new=gam3c2*gamci2;
    
  b=-betac*cos(th3l);
    
  a=y_new+pow(b,2);
    
  //c=1.-y_new;
  c=0.;
    
  d=pow(b,2)-(a*c);
    
  if(d > 0.){
    
    beta3l=(-b+sqrt(d))/a;
        
    y_loop=(beta3l*cos(th3l)-betac)/((1.-betac*beta3l*cos(th3l))*beta3c);
    
    if(fabs(y_loop) <= 1) y_adj = y_loop;
    //if(fabs(y_loop) <= 1.00001) y_adj = -y_loop/fabs(y_loop);
        
    th3cm=acos(y_adj);

  }

  return th3cm;

}



void DistToBeam( Float_t Xa,  Float_t Ya, Float_t Za ,  Float_t Xb,  Float_t Yb, Float_t Zb, Float_t Beam_spot, Double_t* radius, Double_t* Xtarget, Double_t* Ytarget) 
{
  Double_t a,b,c;
  Double_t Xtrk0,Ytrk0;
  Double_t t0;
  //Double_t r;

  //Colinear vector
  a=(Xb-Xa);
  b=(Yb-Ya);
  c=(Zb-Za);

  // For thin solid target, one calculates the distance r between z axis and the intersection point between the track and the plan (x,y) at Ztrk=0 (target position)
  // according to the straight line parametric equation:
  //  Xtrk= at+Xa
  //  Ytrk= bt+Ya
  //  Ztrk= ct+Za =0 -> t0=Za/c
  //
  // and compare it the the beam spot: 

  t0=-Za/c;
  Xtrk0=a*t0+Xa;
  Ytrk0=b*t0+Ya;
  
  //r=sqrt(pow(Xtrk0,2)+pow(Ytrk0,2));
  *radius=sqrt(pow(Xtrk0,2)+pow(Ytrk0,2));
  *Xtarget= Xtrk0;
  *Ytarget= Ytrk0;

  //cout << "r=" << r << endl;
  //cout << "radius=" << *radius << endl;
  //cout << "Xtarget=" << *Xtarget << " Ytarget=" << *Ytarget << endl;

  // return 0;
 

}

 
Bool_t IsFromTarget( Float_t Xa,  Float_t Ya, Float_t Za ,  Float_t Xb,  Float_t Yb, Float_t Zb, Float_t Beam_spot_radius, Float_t target_thick) 
{
  Double_t a,b,c;
  Double_t Xtrk0,Ytrk0;
  Double_t Xtrkmin,Ytrkmin;
  Double_t Xtrkmax,Ytrkmax;
  Double_t t0,tmin,tmax, r;

  //Colinear vector
  a=(Xb-Xa);
  b=(Yb-Ya);
  c=(Zb-Za);

  // For thin solid target, one calculates the distance r between z axis and the intersection point between the track and the plan (x,y) at Ztrk=0 (target position)
  // according to the straight line parametric equation:
  //  Xtrk= at+Xa
  //  Ytrk= bt+Ya
  //  Ztrk= ct+Za =0 -> t=Za/c
  //
  // and compare it the the beam spot: 

  // First let's consider Ztrk=0
  t0=-Za/c; // At Ztrk=0
  Xtrk0=a*t0+Xa;
  Ytrk0=b*t0+Ya;

  // Now let's take into accoun the target thickness
  if(c>0)
    {
      tmin=(-target_thick/2 -Za)/c;
      tmax=(target_thick/2 -Za)/c;
    }else
    {
      tmax=(-target_thick/2 -Za)/c;
      tmin=(target_thick/2 -Za)/c;
    }

  if(a>0)
    {
      Xtrkmin=a*tmin+Xa;
      Xtrkmax=a*tmax+Xa;
    }else
    {
      Xtrkmin=a*tmax+Xa;
      Xtrkmax=a*tmin+Xa;
      
    }

  if(b>0)
    {
      Ytrkmin=b*tmin+Ya;
      Ytrkmax=b*tmax+Ya;
    }else
    {
      Ytrkmax=b*tmin+Ya;
      Ytrkmin=b*tmax+Ya;
    }
  
    r=sqrt(pow(Xtrk0,2)+pow(Ytrk0,2));



    //if(  r<=Beam_spot ||
    //	 (Xtrk0>=Xtrkmin && Xtrk0<=Xtrkmax)  ||
    //	 (Ytrk0>=Ytrkmin && Ytrk0<=Ytrkmax) ) 
     
    //if(  r<=Beam_spot_radius || 
    //	 ( (Xtrkmin>=-Beam_spot_radius && Xtrkmin<Beam_spot_radius) && (Xtrkmax>-Beam_spot_radius && Xtrkmax<=Beam_spot_radius) ) ||
    //   ( (Ytrkmin>=-Beam_spot_radius && Ytrkmin<Beam_spot_radius) && (Ytrkmax>-Beam_spot_radius && Ytrkmax<=Beam_spot_radius) ) )

    if(  r<=Beam_spot_radius || 
	 ( Xtrk0>=-Beam_spot_radius && Xtrk0<=Beam_spot_radius) ||        // At least one of the coordinates (X or Y) at Z=0 
         ( Ytrk0>=-Beam_spot_radius && Ytrk0<=Beam_spot_radius)    )      // valids the beam_spot_radius condition to take into account the fact that pixels of the detector are not point-like.
    {
      /*
      cout << "t0=" << t0 << " tmin=" << tmin  << " tmax=" << tmax  << endl;
      cout << "Xtrk0=" << Xtrk0 << " Xtrkmin=" << Xtrkmin  << " Xtrkmax=" << Xtrkmax  << endl;
      cout << "Trrk0=" << Ytrk0 << " Ytrkmin=" << Ytrkmin  << " Ytrkmax=" << Ytrkmax  << endl;
      cout << "Xa=" << Xa << " Xb=" << Xb <<  " a=" << a << endl;
      cout << "Ya=" << Ya << " Yb=" << Yb <<  " b=" << b << endl;
      cout << "Za=" << Za << " Zb=" << Zb <<  " c=" << c << endl;
      */
      return kTRUE;

    }else
    {
      return kFALSE;
    }
    
}
			

// ===============================================================================================================================================

void STaRTrack_TrackFinder(Int_t const mode, const char* input, const char* output) {


  gROOT->Reset();
  gROOT->SetStyle("Default");
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);


	char title0[250];
	char title1[250];
	
	//SETTINGS 

	Double_t pi = TMath::Pi();



	//sprintf(title0,"%s","startrackerAnaDigit_Notrgt_PencilBeam.root");  	
	sprintf(title0,"%s","startrackerAnaDigit.root");  	
 	//sprintf(title1,"%s","r3bsim.root");  
	TFile *file0 = new TFile(input,"read");
	//TFile *file1 = TFile::Open(title1);
	
	
		
	// ----    Debug option   -------------------------------------------------
	gDebug = 0;
	// ------------------------------------------------------------------------
		
	
	gROOT->SetStyle("Default");
	//gStyle->SetOptTitle(0);
	gStyle->SetOptStat(0);
	gStyle->SetOptFit(0);
	
	
	//HISTOGRAMS DEFINITION
	TH1F *Xtrack = new TH1F("Xtrack","Position X in Lab (Reconstructed from hit strips)",400,-100.,100.);
	TH1F *Ytrack = new TH1F("Ytrack","Position Y in Lab (Reconstructed from hit strips) ",400,-100.,100.);
	TH1F *Ztrack = new TH1F("Ztrack","Position Z in Lab (Reconstructed from hit strips) ",400,-100.,100.);
	TH2F *XtrackZtrack = new TH2F("XtrackZtrack"," Z vs X in Lab (Reconstructed from hit strips)",400,-100.,100., 400,-100.,100.);
	TH2F *XtrackYtrack = new TH2F("XtrackYtrack"," Y vs X in Lab (Reconstructed from hit strips)",400,-100.,100., 400,-100.,100.);
	TH2F *ZtrackYtrack = new TH2F("ZtrackYtrack"," Y vs Z in Lab (Reconstructed from hit strips)",400,-100.,100., 400,-100.,100.);
	
	
	TH1F *TrackMult_Inner = new TH1F("TrackMult_Inner","Multiplicity in inner layer ",50,0.,50.);
	TH1F *TrackMult_Middl = new TH1F("TrackMult_Middl","Multiplicity in middle layer ",50,0.,50.);
	TH1F *TrackMult_Outer = new TH1F("TrackMult_Outer","Multiplicity in outer layer ",50,0.,50.);
	TH1F *TrackMult_Inner40 = new TH1F("TrackMult_Inner40","Multiplicity in inner layer for 40keV threhshold ",50,0.,50.);
	TH1F *TrackMult_Middl40 = new TH1F("TrackMult_Middl40","Multiplicity in middle layer for 40keV threhshold ",50,0.,50.);
	TH1F *TrackMult_Outer40 = new TH1F("TrackMult_Outer40","Multiplicity in outer layer for 40keV threhshold ",50,0.,50.);
	TH1F *StripA_Inner = new TH1F("StripA_Inner","Strip ID A ",2500,0.,2500.);
	TH1F *StripB_Inner = new TH1F("StripB_Inner","Strip ID B ",2500,0.,2500.);
	TH1F *StripA_Middl = new TH1F("StripA_Middl","Strip ID A ",2500,0.,2500.);
	TH1F *StripB_Middl = new TH1F("StripB_Middl","Strip ID B ",2500,0.,2500.);
	TH1F *StripA_Outer = new TH1F("StripA_Outer","Strip ID A ",2500,0.,2500.);
	TH1F *StripB_Outer = new TH1F("StripB_Outer","Strip ID B ",2500,0.,2500.);
	TH1F *StripA_Inner40 = new TH1F("StripA_Inner40","Strip ID A 40keV threshold",2500,0.,2500.);
	TH1F *StripB_Inner40 = new TH1F("StripB_Inner40","Strip ID B 40keV threshold",2500,0.,2500.);
	TH1F *StripA_Middl40 = new TH1F("StripA_Middl40","Strip ID A 40keV threshold",2500,0.,2500.);
	TH1F *StripB_Middl40 = new TH1F("StripB_Middl40","Strip ID B 40keV threshold",2500,0.,2500.);
	TH1F *StripA_Outer40 = new TH1F("StripA_Outer40","Strip ID A 40keV threshold",2500,0.,2500.);
	TH1F *StripB_Outer40 = new TH1F("StripB_Outer40","Strip ID B 40keV threshold",2500,0.,2500.);
	
	TH1F *Theta = new TH1F("Theta","Reconstructed from hit strips",180,0.,180.);
	TH1F *Phi = new TH1F("Phi","Reconstructed from hit strips",360,0.,360.);
			
	TH1F *ELossInner = new TH1F("ELossInner","Energy loss in inner detector",100,0.,1.);  // 0 to 1 MeV
	TH1F *EnergyDeltaE = new TH1F("EnergyDeltaE","Energy distribution of delta electrons",200,0.,2.);  // 0 to 2 MeV
	TH1F *ELossMiddl = new TH1F("ELossMiddl","Energy loss in middle detector",100,0.,1.);
	TH1F *ELossOuter = new TH1F("ELossOuter","Energy loss in outer detector",100,0.,1.);


	TH1F *TargRadius = new TH1F("TargRadius","Position(radius) distribution in target",200,0.,4.);
	TH2F *XTargYTarg = new TH2F("XTargYTarg","Image reconstructed of target+beam overlap",200,-4.,4., 200,-4.,4.);
	
	//For p2p case:
	TH1F *AngTheta1 = new TH1F("AngTheta1","G ",180,0.,180.);
	TH1F *AngTheta2 = new TH1F("AngTheta2","G ",180,0.,180.);
	TH2F *Theta1Theta2 = new TH2F("Theta1Theta2","G ",180,0.,180., 180,0.,180.);
	TH1F *AngPhi1 = new TH1F("AngPhi1","G ",180,0.,360.);
	TH1F *AngPhi2 = new TH1F("AngPhi2","G ",180,0.,360.);
	TH2F *Phi1Phi2 = new TH2F("Phi1Phi2","G ",180,0.,360., 180,0,360);
	TH1F *dEp1 = new TH1F("dEp1","G ",250,0.,5.);   // nrj loss of proton 1,  in all tracker    
	TH1F *dEp2 = new TH1F("dEp2","G ",250,0.,5.);   // nrj loss of proton 2, ain all tracker    

	TH1F *Tracks = new TH1F("Tracks","Number of tracks per event",20,0.,20.);
	TH1F *PhiDiff_ab = new TH1F("PhiDiff_ab","G ",400,0.,4.);
	TH1F *PhiDiff_bc = new TH1F("PhiDiff_bc","G ",400,0.,4.);

	TH2F *ThetaELoss = new TH2F("ThetaEloss","G ",180,0.,180., 100,0.,1.);
	TH2F *ThetaELoss1Cryst = new TH2F("ThetaEloss1cryst","G ",180,0.,180., 100,0.,1.);
	TH2F *dE_E = new TH2F("dE_E","G ",500,0.,.5, 100,0.,.01);       
	TH1F *InvMass = new TH1F("InvMass","Reconstructed of invariant mass",100.,0.,100);


	//For elastic case:
	TH1F *E_detected = new TH1F("E_Detected","Energy Detected",120.,0.,0.006);
	TH1F *Total_E = new TH1F("TotalE","Energy ",2000.,0.,2.);  
	TH1F *Energy_reso = new TH1F("Energy_reso","Resolution",5000,-0.5,0.5);
	TH1F *Theta_reso = new TH1F("Theta_reso","Resolution",600,6.,12.);
	TH1F *EDiff = new TH1F("Qvalue","Energy",1000,-.2,.2);
	TH1F *Theta_cm_reso = new TH1F("Theta_cm_reso","Resolution",600,6.,12.);
	TH1F *Thetacm = new TH1F("Theta_cm","Theta ",1800,0.,180.);
	
	TH2F *ETheta = new TH2F("ETheta","Theta vs Energy",1800,0.,180.,1000.,0.,2.);
	TH2F *Theta12 = new TH2F("Theta12","Theta vs Theta_raw",1800,0.,180.,1800,0.,180.);
	TH2F *ETh_rec = new TH2F("ETh_rec","DiffTh vs Energy",600,-3.,3.,750.,0.,1.6);
	TH2F *ThE_rec = new TH2F("ThE_rec","DiffE vs Theta",1800,0.,180.,5000,-0.5,0.5);


	//Input trees:
	TTree* TStarTrackDigit = (TTree*)file0->Get("cbmsim");
	//TTree* TStarTracker = (TTree*)file1->Get("cbmsim");
	
	//Output file:
	//output File and Tree for the analysis result
	TFile* outFile = new TFile(output,"RECREATE");  
	outFile->cd();

	// for the tracker
	/*
	TBranchElement *brMCHeaderTheta1 = (TBranchElement*)TStarTracker->GetBranch("MCEventHeader.ftheta1");
	TBranchElement *brMCHeaderTheta2 = (TBranchElement*)TStarTracker->GetBranch("MCEventHeader.ftheta2");
	TBranchElement *brMCHeaderPhi1 = (TBranchElement*)TStarTracker->GetBranch("MCEventHeader.fphi1");
	TBranchElement *brMCHeaderPhi2 = (TBranchElement*)TStarTracker->GetBranch("MCEventHeader.fphi2");

	TBranchElement *brMCStackPx = (TBranchElement*)TStarTracker->GetBranch("MCTrack.fPx");         // px at vertex
	TBranchElement *brMCStackPy = (TBranchElement*)TStarTracker->GetBranch("MCTrack.fPy");         // py at vertex
	TBranchElement *brMCStackPz = (TBranchElement*)TStarTracker->GetBranch("MCTrack.fPz");         // pz at vertex
	TBranchElement *brMCStackPDG = (TBranchElement*)TStarTracker->GetBranch("MCTrack.fPdgCode");   // PDG code of particle
	TBranchElement *brMCStackMass = (TBranchElement*)TStarTracker->GetBranch("MCTrack.fMass");     // Mass of the particle (GeV/c2
	TBranchElement *brMCStackMother = (TBranchElement*)TStarTracker->GetBranch("MCTrack.fMotherId"); // id of the parents particle

	TBranchElement *branchTrackX = (TBranchElement*)TStarTracker->GetBranch("STaRTraPoint.fX");
	TBranchElement *branchTrackY = (TBranchElement*)TStarTracker->GetBranch("STaRTraPoint.fY");
	TBranchElement *branchTrackZ = (TBranchElement*)TStarTracker->GetBranch("STaRTraPoint.fZ");
	TBranchElement *branchTrackPX = (TBranchElement*)TStarTracker->GetBranch("STaRTraPoint.fPx");
	TBranchElement *branchTrackPY = (TBranchElement*)TStarTracker->GetBranch("STaRTraPoint.fPy");
	TBranchElement *branchTrackPZ = (TBranchElement*)TStarTracker->GetBranch("STaRTraPoint.fPz");
	TBranchElement *branchELoss = (TBranchElement*)TStarTracker->GetBranch("STaRTraPoint.fELoss");
	TBranchElement *branchDetectorID = (TBranchElement*)TStarTracker->GetBranch("STaRTraPoint.fDetectorID");
	TBranchElement *branchDetCopyID = (TBranchElement*)TStarTracker->GetBranch("STaRTraPoint.fDetCopyID");
	TBranchElement *branchTrackID = (TBranchElement*)TStarTracker->GetBranch("STaRTraPoint.fTrackID");
	*/
	
	// for the tracker digit
                     
	TBranchElement *branchDigitELoss = (TBranchElement*)TStarTrackDigit->GetBranch("STaRTrackerDigitHit.fEnergy");
	TBranchElement *branchDigitDetID = (TBranchElement*)TStarTrackDigit->GetBranch("STaRTrackerDigitHit.fDetector");
	TBranchElement *branchChip = (TBranchElement*)TStarTrackDigit->GetBranch("STaRTrackerDigitHit.fChip");
	TBranchElement *branchSide = (TBranchElement*)TStarTrackDigit->GetBranch("STaRTrackerDigitHit.fSide");
	TBranchElement *branchStrip = (TBranchElement*)TStarTrackDigit->GetBranch("STaRTrackerDigitHit.fStrip");
	//TBranchElement *branchStripfrt = (TBranchElement*)TStarTrackDigit->GetBranch("STaRTrackerDigitHit.fStripfrt");
	//TBranchElement *branchStripbck = (TBranchElement*)TStarTrackDigit->GetBranch("STaRTrackerDigitHit.fStripbck");
	TBranchElement *branchDigitTime = (TBranchElement*)TStarTrackDigit->GetBranch("STaRTrackerDigitHit.fTime");

	//for califa
	/*
	TBranchElement *branchXBallX = (TBranchElement*)TStarTracker->GetBranch("CrystalPoint.fX");
	TBranchElement *branchXBallY = (TBranchElement*)TStarTracker->GetBranch("CrystalPoint.fY");
	TBranchElement *branchXBallZ = (TBranchElement*)TStarTracker->GetBranch("CrystalPoint.fZ");
	TBranchElement *branchELossCryst = (TBranchElement*)TStarTracker->GetBranch("CrystalPoint.fELoss");
	TBranchElement *branchTypeCryst = (TBranchElement*)TStarTracker->GetBranch("CrystalPoint.fCrystalType");
	TBranchElement *branchNbCryst = (TBranchElement*)TStarTracker->GetBranch("CrystalPoint.fCrystalNb");
	*/

	// For primary and secondary particles 
	Int_t PartMult=0;
	Int_t *Mother_part=0;      // Parent ID of primaries and secondaries at vertex
	Int_t *PDG_part=0;         // PDG code of primaries and secondaries at vertex
	Double_t *PX_part=0;       // momentum X in Lab frame of primaries and secondaries at vertex
	Double_t *PY_part=0;       // momentum Y in Lab frame of primaries and secondaries at vertex 
	Double_t *PZ_part=0;       // momentum Z in Lab frame of primaries and secondaries at vertex
	Double_t NRJDeltaE=0.0;       // kinetic energy of delta e- produced
	
	// For Si tracker:
	Int_t TrackMult=0;
	Int_t TotalTrackMult=0;
	Int_t TrackMultDecr=0;
	Int_t TrackMultClust=0;
	Int_t TrkMult_Inner=0;
	Int_t TrkMult_Middl=0;
	Int_t TrkMult_Outer=0;
	Int_t TrkMult_Inner40=0;
	Int_t TrkMult_Middl40=0;
	Int_t TrkMult_Outer40=0;
	Int_t XballMult=0;  // ! = nb of hits
	
	Int_t *DetID=0;      // Detector ID (=7 for inner layer, =8 for middle layer, =9 for outer layer)
	Int_t *DetCopyID=0;  // Detector Copy ID
	Int_t *DetCopyID_init=0;  // Detector Copy ID
	Int_t *DetCopyIDClust=0;  // Detector Copy ID in Cluster

	Int_t *Chip=0;  // front strip ID
	Int_t *Side=0;  // front strip ID
	Int_t *Strip=0;  // front strip ID

	Int_t *Stripfrt=0;  // front strip ID
	Int_t *Stripbck=0;  // back Strip ID
	Int_t *StripfrtClust=0;  // main front Strip ID in cluster
	Int_t *StripbckClust=0;  // main back Strip ID in cluster
 
	Int_t StripfrtClustID=0;
        Int_t StripbckClustID=0;
	
	Double_t *X_track=0;       // Position X in Lab frame
	Double_t *Y_track=0;       // Position Y in Lab frame  
	Double_t *Z_track=0;       // Position Z in Lab frame
	Double_t *PX_track=0;       // momentum X in Lab frame
	Double_t *PY_track=0;       // momentum Y in Lab frame  
	Double_t *PZ_track=0;       // momentum Z in Lab frame
	
	
	Double_t X_intersect = 0.0;   // Position X in Detector frame
	Double_t Y_intersect = 0.0;   // Position Y in Detector frame
	Double_t Z_intersect = 0.0;   // Position Z in Detector frame
	
	Double_t *Theta_track=0;  // theta value  assuming interaction at (0,0,0) 
	Double_t *Phi_track =0;    // phi value assuming interaction at (0,0,0)

	Double_t *theta=0;  // theta from tracking
	Double_t *phi=0;    // phi from tracking
	Double_t *Ep=0;     // Energy loss by each tracked particle
	
	Double_t *PTot=0;
	
	Double_t *X_Xball=0;
	Double_t *Y_Xball=0;
	Double_t *Z_Xball=0;
	Double_t *Theta_Xball=0;
	Double_t *Cryst_Type=0;
	Double_t *Cryst_Nb=0;
	
	Double_t *ELoss=0;
	Double_t *ELoss_init=0;
	Double_t ELossSumClust = 0.0;
	Double_t *ELossClust=0;
	Double_t *ELossCryst=0;
	Double_t *EDepos=0;
	Double_t *EDepos1Cryst=0;

	Double_t *TimeStp_init=0;
	

	Double_t *Xa=0;  // inner layer
	Double_t *Ya=0;
	Double_t *Za=0;
	Double_t *Phia=0;
	Double_t *Epa=0;
	Double_t *part_a=0;  
	Double_t *Xb=0;  // intermidiate layer
	Double_t *Yb=0;
	Double_t *Zb=0;
	Double_t *Phib=0;
	Double_t *Epb=0;
	Double_t *part_b=0;
	Double_t *Xc=0;  // Outer layer
	Double_t *Yc=0;
	Double_t *Zc=0;
	Double_t *Phic=0;
	Double_t *Epc=0;
	Double_t *part_c=0;


	// added for elastic case
	Double_t *TotalE=0;  // Energy loss for a same track 
	Double_t *E_particle=0; // 
	Int_t *Track=0;
	Double_t Angle=0.;
	Double_t Angle_phi=0.;
	Double_t Angle_cm=0.;
	Double_t Energy=0.;
	Double_t Eng=0.;
	Double_t CsIE=0.;
	Double_t CsIER=0.;
	Double_t Theta_trk=0.;
	Double_t Xdiff=0.;
	Double_t Ydiff=0.;
	Double_t Zdiff=0.;
	Double_t TotE=0.;
	Double_t Phi_trk=0.;
	Double_t Qvalue=0.;
	Double_t Theta_cm=0.;
	Double_t DiffE=0.;
	Double_t DiffTh=0.;
	Double_t DiffTh_cm=0.;
	Double_t Erec=0.;
	// end of elastic addition
	
	//
	// Si Geometrical parameters (To be store in DataBase:
	//
	
	// Inner layer
	Double_t Length1  = 21.794 ; // cm
	Double_t WidthMax1  = 8.1912 ; // cm
	Double_t WidthMin1  = 1.971 ; // cmIbrahim@ipno.in2p3.fr
	//Double_t StripPitch1= 0.0050 ; // = 50 um
	Double_t StripPitch1= 0.00385 + 0.0012 + 0.0001 + 0.000127+ 2e-6; // = strip pitch 0.00385 + interstrip = 52.8 um
	//Double_t InclAng1=14.9;
	Double_t InclAng1=14.3; // deg
	Double_t Rmin1=1.75;    // cm
	//Double_t AngRangeMin1=7.;    
	Double_t AngRangeMin1=5.26;//deg    
	Double_t AngTrap1=atan((WidthMax1/2 -WidthMin1/2)/Length1);
	Double_t WidthHalf1=WidthMax1 - (Length1/cos(AngTrap1))*sin(AngTrap1);
	Double_t StepZ1= StripPitch1/sin(AngTrap1) ; // step along the z axis of the detector (in xz plan)
	Double_t StepX1= StripPitch1/cos(AngTrap1) ; // step along the x axis of the detector (in xz plan)
	Int_t    NbStrip1   = int(WidthMax1/StepX1); //
	cout << "NbStrip1= " << NbStrip1 << endl;
	Double_t Xlab1, Ylab1, Zlab1;  // see trunk/tracker/R3BTra.cxx
	Xlab1=0;
	Ylab1=-((Length1/2)*sin(InclAng1*pi/180.)+ Rmin1);
	Zlab1=-Length1*cos(InclAng1*pi/180.)/2 + (Rmin1/tan(AngRangeMin1*pi/180.));
	Double_t ShiftalongZ;
	Double_t SlopA,SlopB,OffsetA,OffsetB;
	
	// Middle layer
	//Double_t Length2  = 30.06 ; // cm
	Double_t Length2  = 33.83875 ; // cm
	//Double_t WidthMax2  = 10.4 ; // cm
	Double_t WidthMax2  = 10.80295 ; // cm
	//Double_t WidthMin2  = 1.3 ; // cm
	Double_t WidthMin2  = 1.1406 ; // cm
	//Double_t StripPitch2= 0.005 ; // = 50 um
	Double_t StripPitch2= 0.00385 + 0.0012 + 0.0001 + 0.00007; // =52.2 um    
	Double_t InclAng2=32.155; // deg    
	Double_t Rmin2=2.22;    // cm
	Double_t AngRangeMin2=5.3;// deg    
	Double_t AngTrap2= atan((WidthMax2/2 -WidthMin2/2)/Length2); // 0.14835 ; // in rad = 17/2 degrees
	Double_t StepZ2= StripPitch2/sin(AngTrap2) ; // step along the z axis of the detector (in xz plan)
	Double_t StepX2= StripPitch2/cos(AngTrap2) ; // step along the x axis of the detector (in xz plan)
	Int_t    NbStrip2   = int(WidthMax2/StepX2); //
	cout << "NbStrip2= " << NbStrip2 << endl;
	Double_t Xlab2, Ylab2, Zlab2;  // see trunk/tracker/R3BTra.cxx
	Xlab2=0;
	Ylab2=-((Length2/2)*sin(InclAng2*pi/180.)+ Rmin2);
	Zlab2=-Length2*cos(InclAng2*pi/180.)/2 + (Rmin2/tan(AngRangeMin2*pi/180.));
	
	// Outer layer
	//Double_t Length3  = 30.6 ; // cm
	Double_t Length3  = 33.83875 ; // cm
	//Double_t WidthMax3  = 10.4 ; // cm
	Double_t WidthMax3  = 10.80295 ; // cm
	//Double_t WidthMin3  = 1.3 ; // cm
	Double_t WidthMin3  = 1.1406 ; // cm
	//Double_t StripPitch3= 0.005 ; // = 50 um
	Double_t StripPitch3= 0.00385 + 0.0012 + 0.0001 + 0.00007; // =52.2 um
	Double_t InclAng3=32.155; // deg    
	Double_t Rmin3=2.95;    // cm
	Double_t AngRangeMin3=6.76; // deg   
	Double_t AngTrap3=  atan((WidthMax3/2 -WidthMin3/2)/Length3); //0.14835 ; // in rad = 17/2 degrees
	Double_t StepZ3= StripPitch3/sin(AngTrap3) ; // step along the z axis of the detector (in xz plan)
	Double_t StepX3= StripPitch3/cos(AngTrap3) ; // step along the x axis of the detector (in xz plan)
	Int_t    NbStrip3   = int(WidthMax3/StepX3); //
	cout << "NbStrip3= " << NbStrip3 << endl;
	Double_t Xlab3, Ylab3, Zlab3;  // see trunk/tracker/R3BTra.cxx
	Xlab3=0;
	Ylab3=-((Length3/2)*sin(InclAng3*pi/180.)+ Rmin3);
	Zlab3=-Length3*cos(InclAng3*pi/180.)/2 + (Rmin3/tan(AngRangeMin3*pi/180.));
        
        // DEFINITION OF OUTPUT TREE:
        Int_t trk = 0;
        Int_t CountGoodTrk = 0;
        TTree* Tout = new TTree("STarTrack_Tracks","STarTrack_Tracks");
        
        Tout->Branch("TrackSize",&TrackMult,"TrackSize/I");
        Tout->Branch("Xtrack",X_track,"Xtrack[TrackSize]/D");
        Tout->Branch("Ytrack",Y_track,"Ytrack[TrackSize]/D");
        Tout->Branch("Ztrack",Z_track,"Ztrack[TrackSize]/D");
        Tout->Branch("ThetaTrack",Theta_track,"ThetaTrack[TrackSize]/D");
        Tout->Branch("PhiTrack",Phi_track,"PhiTrack[TrackSize]/D");
        Tout->Branch("ElossTrack",ELoss,"ElossTrack[TrackSize]/D");
              
        Tout->Branch("NumberOfSolvedTracks",&trk,"NumberOfSolvedTracks/I");
        Tout->Branch("NumberOfGoodTracks",&CountGoodTrk,"NumberOfGoodTracks/I");
        Tout->Branch("MiddleMult",&TrkMult_Middl40,"MiddleMult/I");
        Tout->Branch("theta",theta,"theta[MiddleMult]/D");
        Tout->Branch("phi",phi,"phi[MiddleMult]/D");
        Tout->Branch("Ep",Ep,"Ep[MiddleMult]/D");
 


// Transformation  matrices (ie transformation from Lab to det coord. system)  // Also to be stored in DB

  // Inner layer:
  Double_t M_Inner[6][4][4]=
    {

      {
	{0.866025404,	-0.5,             0,            0},  // Matrice 1 row 0
	{0.484507866,	0.83919224,	-0.246999013,	7.222645407},  // Matrice 1 row 1
	{0.123499506,	0.21390742,	0.969015731,	-8.449337001},   // Matrice 1 row 2
	{          0,           0,             0,            1}   // Matrice 1 row 3
      },

      {
	{             0,	     -1,              0,              0},  // Matrice 2 row 0
	{   0.969015731,	0,	-0.246999013,	7.222645407},  // Matrice 2 row 1
	{   0.246999013,	0,	0.969015731,	-8.449337001},  // Matrice 2 row 2
	{             0,	             0,	             0,             1}   // Matrice 2 row 3
      },

      {
	{  -0.866025404,	-0.5,             0,             0},  // Matrice 3 row 0
	{   0.484507866,	-0.83919224,	-0.246999013,	7.222645407},  // Matrice 3 row 1
	{   0.123499506,	-0.21390742,	0.969015731,	-8.449337001},  // Matrice 3 row 2
	{             0,	      0,             0,             1}   // Matrice 3 row 3
      },

      {
	{-0.866025404,	0.5,             0,             0},  // Matrice 4 row 0
	{-0.484507866,	-0.83919224,	-0.246999013,	7.222645407},  // Matrice 4 row 1
	{-0.123499506,	-0.21390742,	0.969015731,	-8.449337001},  // Matrice 4 row 2
	{          0.,	            0,	           0,             1}   // Matrice 4 row 3
      },

      {
	{   0., 	        1,             0,             0},  // Matrice 5 row 0
	{  -0.969015731,	0,	-0.246999013,	7.222645407},  // Matrice 5 row 1
	{  -0.246999013,	0,	0.969015731,	-8.449337001},  // Matrice 5 row 2
	{             0,	     0,	            0,             1}   // Matrice 5 row 3
      },

      {
	{  0.866025404,	0.5,             0,             0},  // Matrice 6 row 0
	{ -0.484507866,	0.83919224,	-0.246999013,	7.222645407},  // Matrice 6 row 1
	{ -0.123499506,	0.21390742,	0.969015731,	-8.449337001},  // Matrice 6 row 2
	{            0,	            0,	           0,             1}   // Matrice 6 row 3
      }

   };
 
  /*
  cout << M_Inner[0][0][0] << " " << M_Inner[0][0][1] << " " << M_Inner[0][0][2] << " " << M_Inner[0][0][3] << " " << endl;
  cout << M_Inner[0][1][0] << " " << M_Inner[0][1][1] << " " << M_Inner[0][1][2] << " " << M_Inner[0][1][3] << " " << endl;
  cout << M_Inner[0][2][0] << " " << M_Inner[0][2][1] << " " << M_Inner[0][2][2] << " " << M_Inner[0][2][3] << " " << endl;
  cout << M_Inner[0][3][0] << " " << M_Inner[0][3][1] << " " << M_Inner[0][3][2] << " " << M_Inner[0][3][3] << " " << endl;
  */

   // Middle layer:
     Double_t M_Mid[12][4][4]=
    {
      {
	{0.965925826,	-0.258819045,             0,            0},  // Matrice 1 row 0
	{0.219119161,	0.81776384,  -0.532211513,  20.45625232},  // Matrice 1 row 1
	{0.137746476,	0.514076846,   0.846611425, -9.606737174},  // Matrice 1 row 2
	{0,           0,             0,            1}   // Matrice 1 row 3
      },

      {
	{0.707106781,	-0.707106781,             0,            0},  // Matrice 2 row 0
	{0.59864468,	0.59864468,  -0.532211513,  20.45625232},  // Matrice 2 row 1
	{0.37633037,	0.37633037,   0.846611425, -9.606737174},  // Matrice 2 row 2
	{          0,           0,             0,            1}   // Matrice 2 row 3
      },

      {
	{0.258819045,	-0.965925826,            0,             0},  // Matrice 3 row 0
	{0.81776384,	0.219119161, -0.532211513,   20.45625232},  // Matrice 3 row 1
	{0.514076846,	0.137746476,  0.846611425,  -9.606737174},  // Matrice 3 row 2
	{          0,             0,            0,             1}   // Matrice 3 row 3
      },

      {
	{-0.258819045,	-0.965925826,             0,            0},  // Matrice 4 row 0
	{0.81776384,	-0.219119161,  -0.532211513,  20.45625232},  // Matrice 4 row 1
	{0.514076846,	-0.137746476,   0.846611425, -9.606737174},  // Matrice 4 row 2
	{          0,           0,             0,            1}   // Matrice 4 row 3
      },

      {
	{-0.707106781,	-0.707106781,            0,             0},  // Matrice 5 row 0
	{0.59864468,	-0.59864468, -0.532211513,   20.45625232},  // Matrice 5 row 1
	{0.37633037,	-0.37633037,  0.846611425,  -9.606737174},  // Matrice 5 row 2
	{          0,             0,            0,             1}   // Matrice 5 row 3
      },

      {
	{-0.965925826,	-0.258819045,            0,             0},  // Matrice 6 row 0
	{ 0.219119161,	-0.81776384, -0.532211513,   20.45625232},  // Matrice 6 row 1
	{ 0.137746476,	-0.514076846,  0.846611425,  -9.606737174},  // Matrice 6 row 2
	{           0,             0,            0,             1}   // Matrice 6 row 3
      },

      {
	{-0.965925826,	0.258819045,            0,             0},  // Matrice 7 row 0
	{-0.219119161,	-0.81776384, -0.532211513,   20.45625232},  // Matrice 7 row 1
	{-0.137746476,	-0.514076846,  0.846611425,  -9.606737174},  // Matrice 7 row 2
	{           0,             0,            0,             1}   // Matrice 7 row 3
      },

      {
	{-0.707106781,	0.707106781,             0,            0},  // Matrice 8 row 0
	{ -0.59864468,	-0.59864468,  -0.532211513,  20.45625232},  // Matrice 8 row 1
	{ -0.37633037,	-0.37633037,   0.846611425, -9.606737174},  // Matrice 8 row 2
	{ 0,            0,             0,            1}   // Matrice 8 row 3
      },

      {
	{-0.258819045,	0.965925826,             0,            0},  // Matrice 9 row 0
	{-0.81776384,	-0.219119161,  -0.532211513,  20.45625232},  // Matrice 9 row 1
	{-0.514076846,	-0.137746476,   0.846611425, -9.606737174},  // Matrice 9 row 2
	{           0,            0,             0,            1}   // Matrice 9 row 3
      },

      {
	{0.258819045,	0.965925826,             0,            0},  // Matrice 10 row 0
	{-0.81776384,	0.219119161,  -0.532211513,  20.45625232},  // Matrice 10 row 1
	{-0.514076846,	0.137746476,   0.846611425, -9.606737174},  // Matrice 10 row 2
	{           0,   0,             0,            1}   // Matrice 10 row 3
      },

      {
	{0.707106781,	0.707106781,            0,             0},  // Matrice 11 row 0
	{-0.59864468,	0.59864468,  -0.532211513,   20.45625232},  // Matrice 11 row 1
	{-0.37633037,	0.37633037,  0.846611425,  -9.606737174},  // Matrice 11 row 2
	{           0,            0,            0,             1}   // Matrice 11 row 3
      },

      {
	{ 0.965925826,	0.258819045,             0,            0},  // Matrice 12 row 0
	{-0.219119161,	0.81776384,  -0.532211513,  20.45625232},  // Matrice 12 row 1
	{-0.137746476,	0.514076846,   0.846611425, -9.606737174},  // Matrice 12 row 2
	{           0,            0,             0,            1}   // Matrice 12 row 3
      }

    };
  

     // Outer layer:
     Double_t M_Out[12][4][4]=  
    {
      {
	{0.965925826,	-0.258819045,             0,            0},  // Matrice 1 row 0
	{0.219119161,	0.81776384,  -0.532211513,  21.18625232},  // Matrice 1 row 1
	{0.137746476,	0.514076846,   0.846611425, -10.56307389},  // Matrice 1 row 2
	{0,           0,             0,            1}   // Matrice 1 row 3
      },

      {
	{0.707106781,	-0.707106781,             0,            0},  // Matrice 2 row 0
	{0.59864468,	0.59864468,  -0.532211513,  21.18625232},  // Matrice 2 row 1
	{0.37633037,	0.37633037,   0.846611425, -10.56307389},  // Matrice 2 row 2
	{          0,           0,             0,            1}   // Matrice 2 row 3
      },

      {
	{0.258819045,	-0.965925826,               0,             0},  // Matrice 3 row 0
	{0.81776384,	0.219119161,	-0.532211513,   21.18625232},  // Matrice 3 row 1
	{0.514076846,	0.137746476,	 0.846611425,  -10.56307389},  // Matrice 3 row 2
	{          0,             0,               0,             1}   // Matrice 3 row 3
      },

      {
	{-0.258819045,	-0.965925826,             0,            0},  // Matrice 4 row 0
	{0.81776384,	-0.219119161,  -0.532211513,  21.18625232},  // Matrice 4 row 1
	{0.514076846,	-0.137746476,   0.846611425, -10.56307389},  // Matrice 4 row 2
	{          0,           0,             0,            1}   // Matrice 4 row 3
      },

      {
	{-0.707106781,	-0.707106781,               0,             0},  // Matrice 5 row 0
	{0.59864468,	-0.59864468,	-0.532211513,   21.18625232},  // Matrice 5 row 1
	{0.37633037,	-0.37633037,	 0.846611425,  -10.56307389},  // Matrice 5 row 2
	{          0,             0,               0,             1}   // Matrice 5 row 3
      },


      {
	{-0.965925826,	-0.258819045,            0,             0},  // Matrice 6 row 0
	{ 0.219119161,	-0.81776384,	-0.532211513,   21.18625232},  // Matrice 6 row 1
	{ 0.137746476,	-0.514076846,	0.846611425,  -10.56307389},  // Matrice 6 row 2
	{           0,             0,            0,             1}   // Matrice 6 row 3
      },

      {
	{-0.965925826,	0.258819045,                 0,            0},  // Matrice 7 row 0
	{ -0.219119161,	-0.81776384,	-0.532211513,  21.18625232},  // Matrice 7 row 1
	{ -0.137746476,	-0.514076846,	 0.846611425, -10.56307389},  // Matrice 7 row 2
	{ 0,            0,                 0,            1}   // Matrice 7 row 3
      },

      {
	{-0.707106781,	0.707106781,               0,            0},  // Matrice 8 row 0
	{-0.59864468,	-0.59864468,	-0.532211513,  21.18625232},  // Matrice 8 row 1
	{-0.37633037,	-0.37633037,	 0.846611425, -10.56307389},  // Matrice 8 row 2
	{           0,            0,               0,            1}   // Matrice 8 row 3
      },

      {
	{-0.258819045,	0.965925826,              0,             0},  // Matrice 9 row 0
	{-0.81776384,	-0.219119161,	-0.532211513,   21.18625232},  // Matrice 9 row 1
	{-0.514076846,	-0.137746476,	 0.846611425,  -10.56307389},  // Matrice 9 row 2
	{           0,             0,              0,             1}   // Matrice 9 row 3
      },

      {
	{0.258819045,	0.965925826,             0,            0},  // Matrice 10 row 0
	{-0.81776384,	0.219119161,  -0.532211513,  21.18625232},  // Matrice 10 row 1
	{-0.514076846,	0.137746476,   0.846611425, -10.56307389},  // Matrice 10 row 2
	{           0,   0,             0,            1}   // Matrice 10 row 3
      },

      {
	{0.707106781,	0.707106781,               0,             0},  // Matrice 11 row 0
	{-0.59864468,	0.59864468,	-0.532211513,   21.18625232},  // Matrice 11 row 1
	{-0.37633037,	0.37633037,	 0.846611425,  -10.56307389},  // Matrice 11 row 2
	{           0,            0,               0,             1}   // Matrice 11 row 3
      },

      {
	{ 0.965925826,	0.258819045,             0,            0},  // Matrice 12 row 0
	{-0.219119161,	0.81776384,  -0.532211513,  21.18625232},  // Matrice 12 row 1
	{-0.137746476,	0.514076846,   0.846611425, -10.56307389},  // Matrice 12 row 2
	{           0,            0,             0,            1}   // Matrice 12 row 3
      }

    };
  



	// Transformation inverse matrices: (ie transformation from det coord. system to lab)
	
	// Inner layer:
	Double_t M_INV_Inner[6][4][4]=
	  {
	    {
	      { 0.866025404,	0.484507866,	0.123499506,	-2.455939562},  // Matrice 1 row 0
	      {-0.5,	0.83919224,	0.21390742,	-4.253812102},  // Matrice 1 row 1
	      {0,	-0.246999013,	0.969015731,	9.971526758},  // Matrice 1 row 2
	      {           0,             0,            0,            1},  // Matrice 1 row 3
	    },
	    
	    {
	      {  0,	0.969015731,	0.246999013,	-4.911879124},   // Matrice 2 row 0
	      { -1,	5.93594E-17,	1.51305E-17,	-3.00889E-16},   // Matrice 2 row 1
	      {            0,  -0.246999013,	0.969015731,    9.971526758},   // Matrice 2 row 2
	      {          0,	          0,	          0,	          1},   // Matrice 2 row 3
	    },
	    
	    {
	      { -0.866025404,	0.484507866,	0.123499506,	-2.455939562},   // Matrice 3 row 0
	      { -0.5,	-0.83919224,	-0.21390742,	4.253812102},   // Matrice 3 row 1
	      {            0, -0.246999013,  0.969015731,   9.971526758},   // Matrice 3 row 2
	      {            0,	           0,	         0,	        1},   // Matrice 3 row 3
	    },
	    
	    {
	      { -0.866025404,	-0.484507866,	-0.123499506,	2.455939562},   // Matrice 4 row 0
	      {  0.5,	-0.83919224,	-0.21390742,	4.253812102},   // Matrice 4 row 1
	      {           0., -0.246999013,   0.969015731,   9.971526758},   // Matrice 4 row 2
	      {           0.,	          0.,	         0.,	        1.},   // Matrice 4 row 3
	    },
	    
	    {
	      { -1.83772E-16,	-0.969015731,	-0.246999013,	4.911879124},   // Matrice 5 row 0
	      {  1,	-1.78078E-16,	-4.53916E-17,	9.02667E-16},   // Matrice 5 row 1
	      {           0.,  -0.246999013,   0.969015731,   9.971526758},   // Matrice 5 row 2
	      {           0.,	           0.,	          0.,	         1.},   // Matrice 5 row 3
	    },
	    
	    {
	      {  0.866025404,	-0.484507866,	-0.123499506,	2.455939562},   // Matrice 6 row 0
	      {  0.5,	0.83919224,	0.21390742,	-4.253812102},   // Matrice 6 row 1
	      {           0., -0.246999013,	 0.969015731,   9.971526758},   // Matrice 6 row 2
	      {           0.,	          0.,	          0.,	         1.},   // Matrice 6 row 3
	    }
	    
	    
	  };
	
	// Middle layer:
	Double_t M_INV_Mid[12][4][4]=
	  {
	    {
	      {0.965925826,	0.219119161,	0.137746476,	-3.159062649},  // Matrice 1 row 0
	      {-0.258819045,	0.81776384,	0.514076846,	-11.78978231},  // Matrice 1 row 1
	      {0, -0.532211513,   0.846611425,  19.02022645},  // Matrice 1 row 2
	      {0,            0,             0,            1}   // Matrice 1 row 3
	    },
	    
	    {
	      {0.707106781,	0.59864468,	0.37633037,	-8.630719662},  // Matrice 2 row 0
	      {-0.707106781,	0.59864468,	0.37633037,	-8.630719662},  // Matrice 2 row 1
	      {          0,  -0.532211513,	0.846611425,	19.02022645},  // Matrice 2 row 2
	      {          0,             0,             0,             1}   // Matrice 2 row 3
	    },
	    
	    {
	      { 0.258819045,	0.81776384,	0.514076846,	-11.78978231},  // Matrice 3 row 0
	      {-0.965925826,	0.219119161,	0.137746476,	-3.159062649},  // Matrice 3 row 1
	      {           0,  -0.532211513,	0.846611425,	19.02022645},  // Matrice 3 row 2
	      {           0,             0,            0,             1}   // Matrice 3 row 3
	    },
	    
	    {
	      { -0.258819045,	0.81776384,	0.514076846,	-11.78978231},  // Matrice 4 row 0
	      {  -0.965925826,	-0.219119161,	-0.137746476,	3.159062649},  // Matrice 4 row 1
	      {         0.,   -0.532211513,	0.846611425,	19.02022645},  // Matrice 4 row 2
	      {         0.,              0,            0,             1}   // Matrice 4 row 3
	    },
	    
	    {
	      { -0.707106781,	0.59864468,	0.37633037,	-8.630719662},  // Matrice 5 row 0
	      {-0.707106781,	-0.59864468,	-0.37633037,	8.630719662},  // Matrice 5 row 1
	      {           0,  -0.532211513,	0.846611425,	19.02022645},  // Matrice 5 row 2
	      {           0,             0,            0,             1}   // Matrice 5 row 3
	    },
	    
	    {
	      { -0.965925826,	0.219119161,	0.137746476,	-3.159062649},  // Matrice 6 row 0
	      { -0.258819045,	-0.81776384,	-0.514076846,	11.78978231},  // Matrice 6 row 1
	      {            0,  -0.532211513,	 0.846611425,	19.02022645},  // Matrice 6 row 2
	      {            0,             0,             0,             1}   // Matrice 6 row 3
	    },
	    
	    {
	      {-0.965925826,	-0.219119161,	-0.137746476,	3.159062649},  // Matrice 7 row 0
	      { 0.258819045,	-0.81776384,	-0.514076846,	11.78978231},  // Matrice 7 row 1
	      { 0, -0.532211513,	 0.846611425,	19.02022645},  // Matrice 7 row 2
	      { 0,            0,                 0,             1}   // Matrice 7 row 3
	    },
	    
	    {
	      {-0.707106781,	-0.59864468,	-0.37633037,	8.630719662},  // Matrice 8 row 0
	      { 0.707106781,	-0.59864468,	-0.37633037,	8.630719662},  // Matrice 8 row 1
	      {           0,  -0.532211513,	 0.846611425,	19.02022645},  // Matrice 8 row 2
	      {           0,             0,             0,              1}   // Matrice 8 row 3
	    },
	    
	    {
	      { -0.258819045,	-0.81776384,	-0.514076846,	11.78978231},  // Matrice 9 row 0
	      { 0.965925826,	-0.219119161,	-0.137746476,	3.159062649},  // Matrice 9 row 1
	      {           0,  -0.532211513,	0.846611425,	19.02022645},  // Matrice 9 row 2
	      {           0,             0,            0,             1}   // Matrice 9 row 3
	    },
	    
	    {
	      { 0.258819045,	-0.81776384,	-0.514076846,	11.78978231},  // Matrice 10 row 0
	      { 0.965925826,	0.219119161,	0.137746476,	-3.159062649},  // Matrice 10 row 1
	      {         0.,   -0.532211513,	0.846611425,	19.02022645},  // Matrice 10 row 2
	      {         0.,              0,            0,             1}   // Matrice 10 row 3
	    },
	    
	    {
	      { 0.707106781,	-0.59864468,	-0.37633037,	8.630719662},  // Matrice 11 row 0
	      { 0.707106781,	0.59864468,	0.37633037,	-8.630719662},  // Matrice 11 row 1
	      {           0,  -0.532211513,	0.846611425,	19.02022645},  // Matrice 11 row 2
	      {           0,             0,            0,              1}    // Matrice 11 row 3
	    },
	    
	    {
	      { 0.965925826,	-0.219119161,	-0.137746476,	3.159062649},  // Matrice 12 row 0
	      { 0.258819045,	0.81776384,	0.514076846,	-11.78978231},  // Matrice 12 row 1
	      {           0,  -0.532211513,	0.846611425,	19.02022645},  // Matrice 12 row 2
	      {           0,             0,             0,              1}    // Matrice 12 row 3
	    }
	  };
	
	
	
	// Outer layer:
	Double_t M_INV_Out[12][4][4]=
	  {
	    {
	      {0.965925826,	0.219119161,	0.137746476,	-3.187287624},  // Matrice 1 row 0
	      {-0.258819045,	0.81776384,	0.514076846,	-11.89511935},  // Matrice 1 row 1
	      {0, -0.532211513,	0.846611425,	 20.21838645},  // Matrice 1 row 2
	      {0,            0,                 0,               1}   // Matrice 1 row 3
	    },
	    
	    {
	      {0.707106781,	0.59864468,	0.37633037,	-8.707831728},  // Matrice 2 row 0
	      {-0.707106781,	0.59864468,	0.37633037,	-8.707831728},  // Matrice 2 row 1
	      {          0,  -0.532211513,	0.846611425,	 20.21838645},  // Matrice 2 row 2
	      {          0,             0,             0,             1}   // Matrice 2 row 3
	    },
	    
	    {
	      { 0.258819045,	0.81776384,	0.514076846,	-11.89511935},  // Matrice 3 row 0
	      {-0.965925826,	0.219119161,	0.137746476,	-3.187287624},  // Matrice 3 row 1
	      {           0,  -0.532211513,	0.846611425,	20.21838645},  // Matrice 3 row 2
	      {           0,             0,            0,             1}   // Matrice 3 row 3
	    },
	    
	    {
	      { -0.258819045,	0.81776384,	0.514076846,	-11.89511935},  // Matrice 4 row 0
	      { -0.965925826,	-0.219119161,	-0.137746476,	3.187287624},  // Matrice 4 row 1
	      {         0.,   -0.532211513,	0.846611425,	 20.21838645},  // Matrice 4 row 2
	      {         0.,              0,            0.,               1}   // Matrice 4 row 3
	    },
	    
	    {
	      {-0.707106781,	0.59864468,	0.37633037,	-8.707831728},  // Matrice 5 row 0
	      {-0.707106781,	-0.59864468,	-0.37633037,	8.707831728},  // Matrice 5 row 1
	      {           0,  -0.532211513,	0.846611425,	20.21838645},  // Matrice 5 row 2
	      {           0,             0,            0,              1}   // Matrice 5 row 3
	    },
	    
	    {
	      { -0.965925826,	0.219119161,	0.137746476,	-3.187287624},  // Matrice 6 row 0
	      {  -0.258819045,	-0.81776384,	-0.514076846,	11.89511935},  // Matrice 6 row 1
	      {            0,  -0.532211513,	0.846611425,	20.21838645},  // Matrice 6 row 2
	      {            0,             0,            0,             1}   // Matrice 6 row 3
	    },
	    
	    {
	      {-0.965925826,	-0.219119161,	-0.137746476,	3.187287624},  // Matrice 7 row 0
	      { 0.258819045,	-0.81776384,	-0.514076846,	11.89511935},  // Matrice 7 row 1
	      { 0, -0.532211513,	 0.846611425,	20.21838645},  // Matrice 7 row 2
	      { 0,            0,                 0,             1}   // Matrice 7 row 3
	    },
	    
	    {
	      {-0.707106781,	-0.59864468,	-0.37633037,	8.707831728},  // Matrice 8 row 0
	      { 0.707106781,	-0.59864468,	-0.37633037,	8.707831728},  // Matrice 8 row 1
	      {           0,  -0.532211513,	0.846611425,	20.21838645},  // Matrice 8 row 2
	      {           0,             0,             0,            1}   // Matrice 8 row 3
	    },
	    
	    {
	      { -0.258819045,	-0.81776384,	-0.514076846,	11.89511935},  // Matrice 9 row 0
	      { 0.965925826,	-0.219119161,	-0.137746476,	3.187287624},  // Matrice 9 row 1
	      {           0,  -0.532211513,	0.846611425,	20.21838645},  // Matrice 9 row 2
	      {           0,             0,            0,             1}   // Matrice 9 row 3
	    },
	    
	    {
	      { 0.258819045,	-0.81776384,	-0.514076846,	11.89511935},  // Matrice 10 row 0
	      { 0.965925826,	0.219119161,	0.137746476,	-3.187287624},  // Matrice 10 row 1
	      {         0.,   -0.532211513,	0.846611425,	20.21838645},  // Matrice 10 row 2
	      {         0.,              0,            0,             1}   // Matrice 10 row 3
	    },
	    
	    {
	      { 0.707106781,	-0.59864468,	-0.37633037,	8.707831728},  // Matrice 11 row 0
	      { 0.707106781,	0.59864468,	0.37633037,	-8.707831728},  // Matrice 11 row 1
	      {           0,  -0.532211513,	0.846611425,	20.21838645},  // Matrice 11 row 2
	      {           0,             0,            0,              1}    // Matrice 11 row 3
	    },
	    
	    {
	      { 0.965925826,	-0.219119161,	-0.137746476,	3.187287624},  // Matrice 12 row 0
	      { 0.258819045,	0.81776384,	0.514076846,	-11.89511935},  // Matrice 12 row 1
	      {           0,  -0.532211513,	0.846611425,	20.21838645},  // Matrice 12 row 2
	      {           0,             0,             0,              1}    // Matrice 12 row 3
	    }
	  };




	//if one input trees
	Long64_t nevents = TStarTrackDigit->GetEntries();

		cout << "The number of events is: "<< nevents <<endl;

		//if(nevents!= totalEvents * multiplicity) 
		//cout << "WARNING: The number of events ("<< nevents 
		//	<<") in the tree is not totalEvents*multiplicity ("<< totalEvents<<"*"<<multiplicity<<") !!"<<endl;

	Int_t nb = 0;


	CountGoodTrk = 0;
	Int_t CountTrk = 0;
	Int_t CountGoodTrk2 = 0;
	Double_t CountTrk2 = 0;

       	for(Int_t i=0;i<nevents;i++){
       	//for(Int_t i=0;i<20;i++){

	    if(i%100 == 0) printf("Event:%i\n",i);

	    //if one input tree:     
	    nb += TStarTrackDigit->GetEvent(i);

	    // reinitialise for each event
	    TrkMult_Inner=0;
	    TrkMult_Middl=0;
	    TrkMult_Outer=0;
	    TrkMult_Inner40=0;
	    TrkMult_Middl40=0;
	    TrkMult_Outer40=0;


	    /*   
	    PartMult= brMCStackPDG->GetNdata();  //To be checked !!

	    for(Int_t j=0; j<PartMult; j++)
	      {

		PX_part[j]=brMCStackPx->GetValue(j,0,true);
		//cout << "PX_part=" << PX_part[j] << endl;
      
		PY_part[j]=brMCStackPy->GetValue(j,0,true);
		//cout << "PY_part=" << PY_part[j] << endl;
		
		PZ_part[j]=brMCStackPz->GetValue(j,0,true);
		//cout << "PZ_part=" << PZ_part[j] << endl;
		
		Mother_part[j]=brMCStackMother->GetValue(j,0,true);
		//cout << "Mother_part=" << Mother_part[j] << endl;
		
		PDG_part[j]=brMCStackPDG->GetValue(j,0,true);
		//cout << "PDG_part=" << PDG_part[j] << endl;
		
		if(Mother_part[j]==0 && PDG_part[j]==11){     // PDG code for e- =11
		  
		  NRJDeltaE=sqrt(PX_part[j]*PX_part[j] + PY_part[j]*PY_part[j] + PZ_part[j]*PZ_part[j] +511e-6*511e-6) - 511e-6; // GeV
		  //cout << "EnergyDeltaE=" << NRJDeltaE  << endl;
		  
		  EnergyDeltaE->Fill(NRJDeltaE*1000.);   // in MeV
		  
		}
		
	      }
	    */
            
            

	    //
	    // Si Tracker:
	    //

	    //TrackMult=branchStripfrt->GetNdata();
	    TotalTrackMult= branchDigitELoss->GetNdata();  // (Multiplicity= 1 means 1 front OR 1 back strip hit)
	    //cout <<" "<< endl;
	    //cout << "Total Multiplicity in Tracker=" << TotalTrackMult << endl;

	    Chip=new Int_t[TotalTrackMult];     // Strip front id
	    Side=new Int_t[TotalTrackMult];     // Strip front id
	    Strip=new Int_t[TotalTrackMult];     // Strip front id
	    ELoss_init=new Double_t[TotalTrackMult];
	    DetCopyID_init=new Int_t[TotalTrackMult];    // Detector Copy ID
	    TimeStp_init=new Double_t[TotalTrackMult];



	    TrackMult=int(TotalTrackMult/2);
	    //cout << "Multiplicity in Tracker=" << TrackMult << endl;

	    DetID=new Int_t[TrackMult];        // Detector ID (=7 inner layer, =8 middle layer, =9 outer layer)
	    DetCopyID=new Int_t[TrackMult];    // Detector Copy ID
	    DetCopyIDClust=new Int_t[TrackMult];    // Detector Copy ID of cluster of strips

	    Stripfrt=new Int_t[TrackMult];     // Strip front id
	    Stripbck=new Int_t[TrackMult];     // Strip bck id

	    StripfrtClust=new Int_t[TrackMult];     // main Strip front id of cluster of strips
	    StripbckClust=new Int_t[TrackMult];     // main Strip bck id of cluster of strips

	    X_track=new Double_t[TrackMult];   // Position X in Lab frame
	    Y_track=new Double_t[TrackMult];   // Position Y in Lab frame
	    Z_track=new Double_t[TrackMult];   // Position Z in Lab frame
	    PX_track=new Double_t[TrackMult];   // momentum X in Lab frame
	    PY_track=new Double_t[TrackMult];   // momentum Y in Lab frame
	    PZ_track=new Double_t[TrackMult];   // momentum Z in Lab frame
	    PTot=new Double_t[TrackMult];   // momentum tot in Lab frame


	    Theta_track=new Double_t[TrackMult];
	    Phi_track=new Double_t[TrackMult];
	    
	    TotalE=new Double_t[TrackMult];
	    E_particle=new Double_t[TrackMult];
	    Track=new Int_t[TrackMult];

	    ELoss=new Double_t[TrackMult];
	    ELossClust=new Double_t[TrackMult];  // sum of energy loss in cluster of strips
	    // ELossCryst=new Double_t[XballMult]; // uncomment if XBall used
	    EDepos=new Double_t[TrackMult];
	    EDepos1Cryst=new Double_t[TrackMult];

	    Xa  =new Double_t[TrackMult];  // inner layer
	    Ya  =new Double_t[TrackMult];  // inner layer
	    Za  =new Double_t[TrackMult];  // inner layer
	    Phia  =new Double_t[TrackMult];  // inner layer
	    Epa  =new Double_t[TrackMult];  // inner layer
	    Xb  =new Double_t[TrackMult];  // intermidiate layer
	    Yb  =new Double_t[TrackMult];  // intermidiate layer
	    Zb  =new Double_t[TrackMult];  // intermidiate layer
	    Phib  =new Double_t[TrackMult];  // intermidiate layer
	    Epb  =new Double_t[TrackMult];  // intermidiate layer
	    Xc  =new Double_t[TrackMult];  // Outer layer
	    Yc  =new Double_t[TrackMult];  // Outer layer
	    Zc  =new Double_t[TrackMult];  // Outer layer
	    Phic  =new Double_t[TrackMult];  // Outer layer
	    Epc  =new Double_t[TrackMult];  // Outer layer


	    for(Int_t j=0; j<TrackMult; j++){

	      ELoss[j]=0;
	      EDepos[j]=0;
	      // ELossCryst[j]=0;  // uncomment if XBall used
	      Xa[j]=Ya[j]=Za[j]=Phia[j]=Epa[j]=0;
	      Xb[j]=Yb[j]=Zb[j]=Phib[j]=Epb[j]=0;
	      Xc[j]=Yc[j]=Zc[j]=Phic[j]=Epc[j]=0;

	    }

	    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	    //
	    // 1st loop over TOTAL Tracker  Multiplicity = hit multiplicity (reading the tree)
	    //

	    //cout << "#### Reading the Tree  ####" <<  endl;


	    for(Int_t j=0; j<TotalTrackMult; j++)
	      {
		//cout <<"j="<< j <<  endl;
		Chip[j]= branchChip->GetValue(j,0,true);
		//cout << "Chip=" << Chip[j] << endl;	 
		Side[j]= branchSide->GetValue(j,0,true);
		//cout << "Side=" << Side[j] << endl;	 
		Strip[j]= branchStrip->GetValue(j,0,true);
		//cout << "Strip=" << Strip[j] << endl;	 

		ELoss_init[j]=branchDigitELoss->GetValue(j,0,true); // in GeV      // *1000.;   // *1000 to change from GeV to MeV
		//cout << "ELoss_init=" << ELoss_init[j] << endl;	 
		
		DetCopyID_init[j]=branchDigitDetID->GetValue(j,0,true);
		//cout << "DetCopyID_init=" << DetCopyID_init[j] << endl;

		//One assumes that the time sorting is done
		//TimeStp_init[j]=branchDigitTime->GetValue(j,0,true); // in GeV      // *1000.;   // *1000 to change from GeV to MeV


		if(Side[j]==0)
		  {
		  if(j==0)
		    {
		      Stripfrt[j]= Chip[j]*128 + (Strip[j]+1);
		      //cout << "Side= " <<  Side[j] << " j=" << j << endl;
		      //cout << "Stripfrt= " <<  Stripfrt[j] << " j=" << j << endl;
		      ELoss[j]=ELoss_init[j];   // One only keeps the energy loss measured in front strip
		      DetCopyID[j]=DetCopyID_init[j]; // One only keeps the Detector ID from the hit in front strip
		    }else
		    {
		      Stripfrt[int(j/2)]= Chip[j]*128 + (Strip[j]+1);
		      //cout << "Side= " <<  Side[j] << " j=" << j << endl;
		      //cout << "Stripfrt= " <<  Stripfrt[int(j/2)] << " int(j/2)=" << int(j/2) << endl;
		      ELoss[int(j/2)]=ELoss_init[j];   // One only keeps the energy loss measured in front strip
		      DetCopyID[int(j/2)]=DetCopyID_init[j]; // One only keeps the Detector ID from the hit in front strip
		    }
		}

		if(Side[j]==1)    // This assume the first strip hit is the front strip : True in simulation but not guarranty in real data
		  {

		    if(j%2 !=0 )
		      {
			Stripbck[int((j-Side[j])/2)]= Chip[j]*128 + (Strip[j]+1);
			//cout << "Chip= " << Chip[j] << " j=" << j << endl;
			//cout << "Strip= " << Strip[j] << " j=" << j << endl;
			//cout << "Stripbck= " << Stripbck[int( (j-Side[j])/2)] << " int( (j-Side[j])/2 ) =" << int((j-Side[j])/2)  << endl;
		      }else
		      {
			if(j==0)
			  {
			    Stripbck[j]= Chip[j]*128 + (Strip[j]+1);
			  }else
			  {
			    Stripbck[j-Side[j]]= Chip[j]*128 + (Strip[j]+1);
			  }

			//cout << "Chip= " << Chip[j] << " j=" << j << endl;
			//cout << "Strip= " << Strip[j] << " j=" << j << endl;
			//cout << "Stripbck= " << Stripbck[int( (j-Side[j])/2)] << " int( (j-Side[j])/2 ) =" << int((j-Side[j])/2)  << endl;
		      }

		  }
	      }


	    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	    //
	    // 2nd loop over reduced Tracker  Multiplicity (TrackMult)
	    //

	    //cout << "#### Reading the Tree  ####" <<  endl;

	    for(Int_t j=0; j<TrackMult; j++)
	      {
		//ELoss[j]=branchDigitELoss->GetValue(j,0,true); // in GeV      // *1000.;   // *1000 to change from GeV to MeV
		//cout << "ELoss=" << ELoss[j] << endl;	 
		
		//DetCopyID[j]=branchDigitDetID->GetValue(j,0,true);
		//cout << "DetCopyID=" << DetCopyID[j] << endl;

		if(DetCopyID[j]<7)
		  {
			   if(Stripfrt[j]>NbStrip1)cout << "StripA_Id=" <<  Stripfrt[j] <<  ">NbStrip1 in event #"<< i << endl;
			   if(Stripbck[j]>NbStrip1)cout << "StripB_Id= "<<  Stripbck[j] << ">NbStrip1 in event #"<< i << endl;
		  }
	      }


	    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
	    //
	    //  3nd loop over tracker multiplicity to
	    //  Check clusters of neighbouring strips and take strip with highiest energy 
	    //
	    //  !!! NOT tested yet

	    //cout << "#### looking for clusters of strips  ####" <<  endl;

	    TrackMultClust=TrackMult;

	    for(Int_t j=0; j<(TrackMult-1); j++)
	      {

		for(Int_t k=j+1; k<TrackMult; k++)
		  {

		    if( DetCopyID[j] == DetCopyID[k])  // = same ladder (= same detector)
		      {
			// front strips:
			if((fabs(Stripfrt[j]-Stripfrt[k]) ==1))  // we then assume it is the same particle that hit those neighbouring strips
			  {

			    if(ELoss[j]<ELoss[k]){

			      Stripfrt[j] = Stripfrt[k];  // pick the strip with higher energy

			    }else
			      {	
				// Do nothing, keep going
			      } 
			  
			    ELoss[j] += ELoss[k];  // Summing the energy loss in cluster (Only for front strips !!)

			    TrackMultClust--;  

			  }
			// back strips:
		    
			if((fabs(Stripbck[j]-Stripbck[k]) ==1) )  // we then assume it is the same particle that hi those strips
			  {
			    if(ELoss[j]<ELoss[k]){
			      Stripbck[j] = Stripbck[k];
			    }else
			      {
				// Do nothing, keep going
			      }
			    TrackMultClust--;
			  }
			
		      }
		  }

		// New multiplicity is TrackMultClust;
			  
	      }
	    
	    //
	    //  Then TrackMult is changed to the new multiplicity TrackMultCluster
	    //
	    if(TrackMultClust>TrackMult){
	      cout<< "PROBLEM: TrackMultClust should be smaller than TRACKMULT"<< endl;
	      cout<< "TrackMultClust" << TrackMultClust << endl;
	      cout<< "TrackMult" << TrackMult << endl;
	      cout<< " " << endl;
	    }else
	      {
		TrackMult=TrackMultClust;
	      }

	    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	    //
	    // Reconstruction of the positions in Si detectors, in the laboratory frame, from the detector id and strip id information:
	    //

	    for(Int_t j=0; j<TrackMult; j++)
	      {


		if(Stripfrt[j]==0 && Stripbck[j]==0)
		  {
		    cout << " Attention problem with branch extracted data !!" << endl;
		  }else
		  {

		    //////////////////////////////////////// INNER Layer //////////////////////////////////////////////////////
		    
		    if(DetCopyID[j]<7)        // if(DetID[j]==7)  // = inner layer  (=7 with target4 and =4 with target2 ???)
		      {
		        TrkMult_Inner++;
			
			SlopA= 2*(Length1)/(WidthMin1-WidthMax1);		        
			SlopB= -SlopA;
			
			ShiftalongZ= ( Xlab1*M_Inner[0][2][0] 
				       + Ylab1*M_Inner[0][2][1] 
				       + Zlab1*M_Inner[0][2][2] 
				       +     1*M_Inner[0][2][3])+ 0.03; // shift along z axis (-1.35855= z lab coordinate of the center of 
			                                                //the detector after inverse transformation, 0.03 is an extra shift 
			                                                //thought to be coinciding with the middle line of a strip: to be checked !! ).
			//cout << "Shiftalongz1:" << ShiftalongZ << endl;


			OffsetA= (-Length1/2 + ShiftalongZ) - SlopB*(-WidthMax1/2 + (StepX1/2)*(2*Stripfrt[j]+1));
			OffsetB= (-Length1/2 + ShiftalongZ) - SlopB*(-WidthMax1/2 + (StepX1/2)*(2*Stripbck[j]+1));

			// find intersection of the 2 hit strips:
			X_intersect= (OffsetB-OffsetA)/(SlopA-SlopB);  
			Y_intersect= 0.836737;  // Constant determined by the way the transformation matrices have been built (see analysLampShadeSpec1Tree.C)   
			Z_intersect= SlopA*X_intersect+OffsetA;  

		  // then transform in Lab frame:
			      
			   X_track[j]=   X_intersect*M_INV_Inner[DetCopyID[j]-1][0][0] 
			               + Y_intersect*M_INV_Inner[DetCopyID[j]-1][0][1]
		                       + Z_intersect*M_INV_Inner[DetCopyID[j]-1][0][2]
		                       +             M_INV_Inner[DetCopyID[j]-1][0][3];
			   Y_track[j]=   X_intersect*M_INV_Inner[DetCopyID[j]-1][1][0] 
		                       + Y_intersect*M_INV_Inner[DetCopyID[j]-1][1][1]
		                       + Z_intersect*M_INV_Inner[DetCopyID[j]-1][1][2]
		                       +             M_INV_Inner[DetCopyID[j]-1][1][3];
			   Z_track[j]=   X_intersect*M_INV_Inner[DetCopyID[j]-1][2][0] 
		                       + Y_intersect*M_INV_Inner[DetCopyID[j]-1][2][1]
		                       + Z_intersect*M_INV_Inner[DetCopyID[j]-1][2][2]
		                       +             M_INV_Inner[DetCopyID[j]-1][2][3];

			   if(Stripfrt[j]>NbStrip1)cout << "StripA_Id=" <<  Stripfrt[j] <<  ">NbStrip1 in event #"<< i << endl;
			   if(Stripbck[j]>NbStrip1)cout << "StripB_Id= "<<  Stripbck[j] << ">NbStrip1 in event #"<< i << endl;
		   
			   // then calculate polar and Azimuth angle in lab frame, assuming interaction position is (x=0,y=0,z=0):
			   
			    Theta_track[j]= GetThetaTrk(X_track[j], Y_track[j], Z_track[j]) *180./TMath::Pi();   // degree
			   Phi_track[j]= GetPhiTrk(X_track[j], Y_track[j]) *180./TMath::Pi();       // degree
	      
			   //cout << "Theta_track=" << Theta_track[j] << endl;
			   //cout << "Phi_track=" << Phi_track[j] << endl;

			   // fill histograms
			   StripA_Inner->Fill(Stripfrt[j]);
			   StripB_Inner->Fill(Stripbck[j]);
			   if(ELoss[j]>0.00004)  //40 keV
			     {
			       StripA_Inner40->Fill(Stripfrt[j]);
			       StripB_Inner40->Fill(Stripbck[j]);
			     }
			   Xtrack->Fill(X_track[j]);
			   Ytrack->Fill(Y_track[j]);
			   Ztrack->Fill(Z_track[j]);
			   XtrackZtrack->Fill(X_track[j], Z_track[j]);
			   XtrackYtrack->Fill(X_track[j], Y_track[j]);
			   ZtrackYtrack->Fill(Z_track[j], Y_track[j]);
			   Theta->Fill(Theta_track[j]);
			   Phi->Fill(Phi_track[j]);
			   ELossInner->Fill(ELoss[j]*1000.);  // in MeV
			   //cout << "ELoss= " << ELoss[j] << endl;

                           
			   
			   if(ELoss[j]>0.00004) TrkMult_Inner40++;
			   			   
		      }   // end of Det=7 ( inner layer)


		    //////////////////////////////////////// MIDDLE Layer /////////////////////////////////////////////////////////////////

		    if( DetCopyID[j]>6 && DetCopyID[j]<19 ) // if(DetID[j]==9)  // = middle layer (=9 with target4 and =6 with target2 ???)
		      {
		        TrkMult_Middl++;
			
			SlopA= (2*Length2)/(WidthMin2-WidthMax2);
			SlopB= -SlopA;

			ShiftalongZ= ( Xlab2*M_Mid[0][2][0] 
				       + Ylab2*M_Mid[0][2][1] 
				       + Zlab2*M_Mid[0][2][2] 
				       +     1*M_Mid[0][2][3])+ 0.03; // shift along z axis (z lab coordinate of the center of the detector after inverse transformation, 0.03 is an extra shift thought to be coinciding with the middle line of a strip: to be checked !! ).
			//cout << "Shiftalongz2:" << ShiftalongZ << endl;

			OffsetA= (-Length2/2 + ShiftalongZ) - SlopA*(WidthMax2/2 -(StepX2/2)*(2*Stripfrt[j]+1));  // 1st strip proj at x=0 
			OffsetB= (-Length2/2 + ShiftalongZ) - SlopB*(-WidthMax2/2 + (StepX2/2)*(2*Stripbck[j]+1));
			
			// find intersection of the 2 hit strips:
			X_intersect= (OffsetB-OffsetA)/(SlopA-SlopB);
			//X_intersect= X_track_det[j];  
			Y_intersect= 5.85549;   // Constant determined by the way the transformation matrices have been built (see analysLampShadeSpec1Tree.C) 
			Z_intersect= SlopA*X_intersect+OffsetA;  
			//Z_intersect= Z_track_det[j];
			
			// then transform back in Lab frame:
			
			X_track[j]=   X_intersect*M_INV_Mid[DetCopyID[j]-7][0][0] 
			  + Y_intersect*M_INV_Mid[DetCopyID[j]-7][0][1]
			  + Z_intersect*M_INV_Mid[DetCopyID[j]-7][0][2]
			  +             M_INV_Mid[DetCopyID[j]-7][0][3];
			Y_track[j]=   X_intersect*M_INV_Mid[DetCopyID[j]-7][1][0] 
			  + Y_intersect*M_INV_Mid[DetCopyID[j]-7][1][1]
			  + Z_intersect*M_INV_Mid[DetCopyID[j]-7][1][2]
			  +             M_INV_Mid[DetCopyID[j]-7][1][3];
			Z_track[j]=   X_intersect*M_INV_Mid[DetCopyID[j]-7][2][0] 
			  + Y_intersect*M_INV_Mid[DetCopyID[j]-7][2][1]
			  + Z_intersect*M_INV_Mid[DetCopyID[j]-7][2][2]
			  +             M_INV_Mid[DetCopyID[j]-7][2][3];
			
						
			if(Stripfrt[j]>NbStrip2)
			  {
			    cout << "StripA_Id>NbStrip2 in event #"<< i << endl;
			  }
			if(Stripbck[j]>NbStrip2)
			  {
			    cout << "StripB_Id>NbStrip2 in event #"<< i << endl;
			  }
			
			// then calculate polar and Azimuth angle in lab frame, assuming interaction position is (x=0,y=0,z=0):
			
			Theta_track[j]= GetThetaTrk(X_track[j], Y_track[j], Z_track[j]) *180./TMath::Pi();   // degree
			Phi_track[j]= GetPhiTrk(X_track[j], Y_track[j]) *180./TMath::Pi();       // degree
			
			//cout << "Theta_track=" << Theta_track[j] << endl;
			//cout << "Phi_track=" << Phi_track[j] << endl;
			
			// fill histograms
			StripA_Middl->Fill(Stripfrt[j]);
			StripB_Middl->Fill(Stripbck[j]);
			if(ELoss[j]>0.00004) // 40 keV
			  {
			    StripA_Middl40->Fill(Stripfrt[j]);
			    StripB_Middl40->Fill(Stripbck[j]);
			  }
			Xtrack->Fill(X_track[j]);
			Ytrack->Fill(Y_track[j]);
			Ztrack->Fill(Z_track[j]);
			XtrackZtrack->Fill(X_track[j], Z_track[j]);
			XtrackYtrack->Fill(X_track[j], Y_track[j]);
			ZtrackYtrack->Fill(Z_track[j], Y_track[j]);
			Theta->Fill(Theta_track[j]);
			Phi->Fill(Phi_track[j]);
			ELossMiddl->Fill(ELoss[j]*1000);
			
			if(ELoss[j]>0.00004) TrkMult_Middl40++;
			
			
		      }   // end of Det=9 ( Middle layer)	      

		    //////////////////////////////////////// OUTER Layer ///////////////////////////////////////////////////////////////////
		    
		    
		    if( DetCopyID[j]>18 && DetCopyID[j]<31 ) // if(DetID[j]==8)  // = outer layer  (=8 with target4 and =5 with target2 ???)
		      {
			
			TrkMult_Outer++;
			
			SlopA= (2*Length3)/(WidthMin3-WidthMax3);
			SlopB= -SlopA;

			ShiftalongZ= ( Xlab3*M_Out[0][2][0] 
				       + Ylab3*M_Out[0][2][1] 
				       + Zlab3*M_Out[0][2][2] 
				       +     1*M_Out[0][2][3])+ 0.03; // shift along z axis (z lab coordinate of the center of the detector after inverse tr			ansformation, 0.03 is an extra shift thought to be coinciding with the middle line of a strip: to be checked !! ).
			 //cout << "Shiftalongz3:" << ShiftalongZ << endl;

			OffsetA= (-Length3/2 + ShiftalongZ) - SlopA*(WidthMax3/2 -(StepX3/2)*(2*Stripfrt[j]+1));  // 1st strip proj at x=0 
			OffsetB= (-Length3/2 + ShiftalongZ) - SlopB*(-WidthMax3/2 + (StepX3/2)*(2*Stripbck[j]+1));


			// find intersection of the 2 hit strips:
			X_intersect= (OffsetB-OffsetA)/(SlopA-SlopB);
			Y_intersect= 5.45489;  // Constant determined by the way the transformation matrices have been built (see analysLampShadeSpec1Tree.C)
			Z_intersect= SlopA*X_intersect+OffsetA;  
			
			// then transform back in Lab frame:
			X_track[j]=   X_intersect*M_INV_Out[DetCopyID[j]-19][0][0] 
			  + Y_intersect*M_INV_Out[DetCopyID[j]-19][0][1]
			  + Z_intersect*M_INV_Out[DetCopyID[j]-19][0][2]
			  +             M_INV_Out[DetCopyID[j]-19][0][3];
			Y_track[j]=   X_intersect*M_INV_Out[DetCopyID[j]-19][1][0] 
			  + Y_intersect*M_INV_Out[DetCopyID[j]-19][1][1]
			  + Z_intersect*M_INV_Out[DetCopyID[j]-19][1][2]
			  +             M_INV_Out[DetCopyID[j]-19][1][3];
			Z_track[j]=   X_intersect*M_INV_Out[DetCopyID[j]-19][2][0] 
			  + Y_intersect*M_INV_Out[DetCopyID[j]-19][2][1]
			  + Z_intersect*M_INV_Out[DetCopyID[j]-19][2][2]
			  +             M_INV_Out[DetCopyID[j]-19][2][3];
			
			
			if(Stripfrt[j]>NbStrip3)
			  {
			    cout << "StripA_Id>NbStrip3 in event #"<< i << endl;
			  }
			if(Stripbck[j]>NbStrip3)
			  {
			    cout << "StripB_Id>NbStrip3 in event #"<< i << endl;
			  }
			
			// then calculate polar and Azimuth angle in lab frame, assuming interaction position is (x=0,y=0,z=0):
			
			Theta_track[j]= GetThetaTrk(X_track[j], Y_track[j], Z_track[j]) *180./TMath::Pi();   // degree
			Phi_track[j]= GetPhiTrk(X_track[j], Y_track[j]) *180./TMath::Pi();       // degree
			
			//cout << "Theta_track=" << Theta_track[j] << endl;
			//cout << "Phi_track=" << Phi_track[j] << endl;
			
			
			// fill histograms
			StripA_Outer->Fill(Stripfrt[j]);
			StripB_Outer->Fill(Stripbck[j]);
		      
			if(ELoss[j]>0.00004)   // 40 keV
			  {
			    StripA_Outer40->Fill(Stripfrt[j]);
			    StripB_Outer40->Fill(Stripbck[j]);
			  }
			Xtrack->Fill(X_track[j]);
			Ytrack->Fill(Y_track[j]);
			Ztrack->Fill(Z_track[j]);
			XtrackZtrack->Fill(X_track[j], Z_track[j]);
			XtrackYtrack->Fill(X_track[j], Y_track[j]);
			ZtrackYtrack->Fill(Z_track[j], Y_track[j]);
			Theta->Fill(Theta_track[j]);
			Phi->Fill(Phi_track[j]);
			
			ELossOuter->Fill(ELoss[j]*1000);
			
			if(ELoss[j]>0.00004)TrkMult_Outer40++;
			
		      }   // end of Det>18 ( outer layer)
		    
		  }

	      } // end of 1st loop on Multiplicity TrackMult
	      

	    TrackMult_Inner->Fill(TrkMult_Inner);
	    TrackMult_Middl->Fill(TrkMult_Middl);
	    TrackMult_Outer->Fill(TrkMult_Outer);
	    TrackMult_Inner40->Fill(TrkMult_Inner40);
	    TrackMult_Middl40->Fill(TrkMult_Middl40);
	    TrackMult_Outer40->Fill(TrkMult_Outer40);


	    
	    ///////////////////////////////////////////////////////////////////////////
	    //
	    // Track Reconstruction for (pp), (pp'), (pd), (pd') ... reaction chsnnels:
	    //

	    if(mode==0)
	      {
		
		if( TrackMult>=2 )   // Minimum requirement to have 1 charged particle  going through at least 2 layers
		  {

		    Int_t InLay=0;   // index for inner layer  = equivalenet to TrkMult_Inner40
		    Int_t InterLay=0;   // index for intermediate layer = equivalenet to TrkMult_Middl40
		    Int_t OutLay=0;   // index for outer layer = equivalenet to TrkMult_Outer40
		    
		    Double_t r=0;  // reconstructed radius of target+beam
		    Double_t Xtarg=0;  // reconstructed X of target+beam
		    Double_t Ytarg=0;  // reconstructed Y of target+beam
		    trk=0;

                    delete theta;
                    delete phi;
                    delete Ep;
                    delete part_a;
                    delete part_b;
                    delete part_c;
		    Double_t* theta = new Double_t[TrackMult];  // using TrackMult_Middl40 here since we look for tracking between inner and intermediate layer.		
		    Double_t* phi=new Double_t[TrackMult];    // using TrackMult_Middl40 here since we look for tracking between inner and intermediate layer.		
		    Double_t* Ep=new Double_t[TrackMult];     // using TrackMult_Middl40 here since we look for tracking between inner and intermediate layer.		
		    Double_t* part_a=new Double_t[TrackMult];
		    Double_t* part_b=new Double_t[TrackMult];
		    Double_t* part_c=new Double_t[TrackMult];

		    for(Int_t k=0; k<TrackMult; k++){
		      
		      if( DetCopyID[k]<7 && (ELoss[k]*1000)>0.04 && InLay<TrackMult)                           // inner layer
			{
			  Xa[InLay]=X_track[k];
			  Ya[InLay]=Y_track[k];
			  Za[InLay]=Z_track[k];
			  Phia[InLay]=GetPhiTrk(Xa[InLay],Ya[InLay]); 
			  Epa[InLay]=ELoss[k]; 
			  InLay++;  // equivalenet to TrckMult_Inner40
			}
		      
		      if( ( DetCopyID[k]>6 && DetCopyID[k]<19 )  && (ELoss[k]*1000)>0.04 && InterLay<TrackMult)  // middle layer
			{
			  Xb[InterLay]=X_track[k];
			  Yb[InterLay]=Y_track[k];
			  Zb[InterLay]=Z_track[k];
			  Phib[InterLay]=GetPhiTrk(Xb[InterLay],Yb[InterLay]);
			  Epb[InterLay]=ELoss[k];
			  InterLay++;
			}
		      
		      if( ( DetCopyID[k]>18 && DetCopyID[k]<31 )  && (ELoss[k]*1000)>0.04 && OutLay<TrackMult)  // outer later
			{
			  Xc[OutLay]=X_track[k];
			  Yc[OutLay]=Y_track[k];
			  Zc[OutLay]=Z_track[k];
			  Phic[OutLay]=GetPhiTrk(Xc[OutLay],Yc[OutLay]);
			  Epc[OutLay]=ELoss[k];
			  OutLay++;
			}
		  
		    } // end of k=TrackMult loop 

		    if(InLay!=TrkMult_Inner40) cout << "Attention x and TrckMult_inner40 should be identical !!! InLay: = "<< InLay << " TrkMult_inner40= " << TrkMult_Inner40 << endl;
		    if(InterLay!=TrkMult_Middl40) cout << "Attention x and TrckMult_inner40 should be identical !!! InterLa: = "<< InterLay << " TrkMult_Middl40= " << TrkMult_Middl40 << endl;
		    if(OutLay!=TrkMult_Outer40) cout << "Attention x and TrckMult_inner40 should be identical !!! : OutLay= "<< OutLay << " TrkMult_outer40= " << TrkMult_Outer40 << endl;
		    
		    // Now We are only interested in event with Multiplicity>=2 and the inner and intermediate layer are hit:		
		    if(InLay>=1 && InterLay>=1)  // At least inner and intermediate layer are hit (= better case than inner and outer layers because of straggling in intermediate layer 
		      {
			
			//cout << "TrackMult = " << TrackMult << endl; 
			//cout << "Phia[0] = " << Phia[0]*180/pi << endl; 
			//cout << "Phib[0] = " << Phib[0]*180/pi << endl; 
			//cout << "Phia[1] = " << Phia[1]*180/pi << endl; 
			//cout << "Phib[1] = " << Phib[1]*180/pi << endl; 
			//cout << "Phia[0]-Phib[0] = " << (Phia[0]-Phib[0])*180/pi << endl; 
			//cout << "Phia[0]-Phib[1] = " << (Phia[0]-Phib[1])*180/pi << endl; 
			//cout << "fabs = " << fabs((Phia[0]-Phib[0])/( Phia[0]-Phib[1])) << endl; 
			
			
			// For general tracking  between inner and intermediate layer:
			for (Int_t tk=0; tk<TrkMult_Middl40; tk++){theta[tk]=0.0; phi[tk]= 0.0; Ep[tk]= 0.0; part_b[tk]=0.0;}		     
			for (Int_t tk=0; tk<TrkMult_Inner40; tk++){part_a[tk]=0.0;}
			for (Int_t tk=0; tk<TrkMult_Outer40; tk++){part_c[tk]=0.0;}
			
			
			// General tracking
			//
			// Case for particle going through at least 2 layers 
			//
			
			//cout << "InLay:" << InLay << " InterLay:" << InterLay << " OutLay:" << OutLay << endl;
			
			if (InLay>0) {
			for (Int_t ia=0;ia<InLay;ia++) // looking at all hits in Inner layer
			  {
                              if (InterLay>0) {
			    for (Int_t ib=0;ib<InterLay;ib++) // looking at all hits in Intermediate layer
			      {

				//cout << "fabs((Phia[ia]-Phib[ib]):" << fabs((Phia[ia]-Phib[ib]))  << endl;
				PhiDiff_ab->Fill(TMath::Abs((Phia[ia]-Phib[ib])));

			        CountTrk++; // Count all tracks identified between inner and outer layer
				
				// let's first compare phi in inner and intermediate layers. (0.2rad has been determined from PhiDiff_bc)
				if( TMath::Abs((Phia[ia]-Phib[ib]))<0.2 ) // 0.2 rad ~ 11.5deg = considered this as same track
				  {				

				    // ToDo Check the track goes through the Target+Beam overlap
				    //r=DistToBeam(Xa[ia],Ya[ia],Za[ia],Xb[ib],Yb[ib],Zb[ib],1.5);
				    DistToBeam(Xa[ia],Ya[ia],Za[ia],Xb[ib],Yb[ib],Zb[ib],1.5, &r, &Xtarg, &Ytarg);
				    //cout << "r=" << r << endl;
				    //cout << "Xtarg=" << Xtarg << " Ytarg=" << Ytarg << endl;


				// Now let's check the track comes from the target:
      				    if(IsFromTarget(Xa[ia],Ya[ia],Za[ia],Xb[ib],Yb[ib],Zb[ib],1.5, 0.001)) // Assuming beam spot radius = 1.5 cm, target thickness 10um
				      {
					part_a[trk]=ia; 
					part_b[trk]=ib;
					if ((trk+1)<(TrackMult)) {trk = trk + 1;} // this is track #trk

					if(trk==1)CountGoodTrk++; // increment number of good track identified
					                           

					//cout << "ia:" << ia << " ib:"<< ib << endl;
					
					if (OutLay>0) {
					for (Int_t ic=0; ic<OutLay; ic++) // looking at all hits in  Outer layer
					  {
					    PhiDiff_bc->Fill(TMath::Abs((Phib[ib]-Phic[ic])));
					    // let's compare phi in intermediate and outer layers. (0.05rad has been determined from PhiDiff_bc)
					    if( TMath::Abs((Phib[ib]-Phic[ic]))<0.05 ) // = considered as same track (here we could maybe put more conditions to be more restrictive)
					      {
						part_b[trk]=ib;
						part_c[trk]=ic;
						//     cout << "ia:" << ia << " ib:"<< ib << " ic:" << ic << endl;
					      }
					    
					  }}

					
					XTargYTarg->Fill(Xtarg,Ytarg);
					TargRadius->Fill(r);					


				      } // end of IsFromTarget
				  }  // end of Phi condition
				
			      }}
			  }}
			
						
			//loop other the tracks when the number of track is higher than 1: 
			
			//cout << "trk=" << trk << endl;
			//cout << " "<< endl;
			Tracks->Fill(trk);

			
			if(trk>0)
			  {
			    for (Int_t t=0;t<trk;t++) // 
			      {
                                if ((((Int_t) part_a[t])<TrackMult)&&(((Int_t) part_b[t])<TrackMult)&&(((Int_t) part_c[t])<TrackMult)&&(t<TrackMult))
                                {
                                    if ((((Int_t) part_a[t])>-1)&&(((Int_t) part_b[t])>-1)&&(((Int_t) part_c[t])>-1)&&(t>-1))
                                    {
                                        
				phi[t]= GetPhiTrk((Xb[(Int_t) part_b[t]] - Xa[(Int_t) part_a[t]]),(Yb[(Int_t) part_b[t]] - Ya[(Int_t) part_a[t]]));   // one calculates the azimuthal angle form the position in inner and intermediate layer
				theta[t]= GetThetaTrk( Xb[(Int_t) part_b[t] ]- Xa[ (Int_t) part_a[t] ], Yb[ (Int_t) part_b[t] ]-Ya[ (Int_t) part_a[t]  ], Zb[ (Int_t) part_b[t] ]-Za[ (Int_t) part_a[t] ] );  // one calculates the polar angle form the position in inner and intermediate layer
				
				Ep[t]= Epa[ (Int_t) part_a[t] ] + Epb[ (Int_t) part_b[t] ] + Epc[(Int_t) part_c[t] ];  // energy loss in the three layer
                                    }
                                }
			      }
			    
			    if(trk>1)  // ie there is more than 1 tracks detected 
			      {
				for (Int_t t=0;t<trk;t++) // 
				  {
				    //cout << "fabs(phi[t]-phi[t+1])=" << fabs(phi[t]-phi[t+1]) << " " << phi[t] << " " <<  phi[t+1] << endl;
				    if( fabs(phi[t]-phi[t+1])>(pi-1) && fabs(phi[t]-phi[t+1])<(pi+1) )
				      {
					//filling histograms
					Theta1Theta2->Fill(theta[t]*180/pi,theta[t+1]*180/pi);
					Phi1Phi2->Fill(phi[t]*180/pi,phi[t+1]*180/pi);
					AngTheta1->Fill(theta[t]*180/pi);
					AngTheta2->Fill(theta[t+1]*180/pi);
					Phi1Phi2->Fill(phi[t]*180/pi,phi[t+1]*180/pi);
					AngPhi1->Fill(phi[t]*180/pi);
					AngPhi2->Fill(phi[t+1]*180/pi);
					dEp1->Fill(Ep[t]*1000.);
					dEp2->Fill(Ep[t+1]*1000.);
				      }
				    
				    
				  }
				
			      }
			    
			  }
			/////////////////// End of general tracking
			
		    
		      }


		  }  // end of TrackMult>=2
	      }// endo fo mode 0

	    
	    ///////////////////////////////////////////////////////////////////////////
	    //
	    // Track Reconstruction for p2p reaction:
	    //
        
	    if(mode==1)
	      {
		//
		//if( TrackMult==6 )   // = 3 Si layers and 2 particles going through
		if( TrackMult>=4 )   // Minimum requirement to have 2 protons going through at least 2 layers
		  {
		
		    Int_t InLay=0;   // index for inner layer  = equivalenet to TrkMult_Inner40
		    Int_t InterLay=0;   // index for intermediate layer = equivalenet to TrkMult_Middl40
		    Int_t OutLay=0;   // index for outer layer = equivalenet to TrkMult_Outer40
		    
		    Double_t r=0;  // reconstructed radius of target+beam
		    Double_t Xtarg=0;  // reconstructed X of target+beam
		    Double_t Ytarg=0;  // reconstructed Y of target+beam
                  
		    ///////////////////////////////////////
		    // For basic tracking:
		       Double_t theta1, theta2, phi1, phi2;
		       Double_t Ep1, Ep2;
		    
		    // For general tracking:
		    //Int_t trk = 0;
                    
                    Bool_t ThisIsZero = kFALSE;
                    if (TrackMult == 0) {TrackMult = 1; ThisIsZero = kTRUE;}
                       
                    delete theta;
                    delete phi;
                    delete Ep;
                    delete part_a;
                    delete part_b;
                    delete part_c;
		    Double_t* theta = new Double_t[TrackMult];  // using TrackMult_Middl40 here since we look for tracking between inner and intermediate layer.		
		    Double_t* phi=new Double_t[TrackMult];    // using TrackMult_Middl40 here since we look for tracking between inner and intermediate layer.		
		    Double_t* Ep=new Double_t[TrackMult];     // using TrackMult_Middl40 here since we look for tracking between inner and intermediate layer.		
		    Double_t* part_a=new Double_t[TrackMult];
		    Double_t* part_b=new Double_t[TrackMult];
		    Double_t* part_c=new Double_t[TrackMult];
                    
                    if (ThisIsZero) {TrackMult = 0;}
                  
		    ////////////////////////////////////////
                  
		    for(Int_t k=0; k<TrackMult; k++){
		      
		      if( DetCopyID[k]<7 && (ELoss[k]*1000)>0.04 && InLay<TrackMult)                           // inner layer
			{
			  Xa[InLay]=X_track[k];
			  Ya[InLay]=Y_track[k];
			  Za[InLay]=Z_track[k];
			  Phia[InLay]=GetPhiTrk(Xa[InLay],Ya[InLay]); 
			  Epa[InLay]=ELoss[k]; 
			  InLay++;  // equivalenet to TrckMult_Inner40
			}
		      
		      if( ( DetCopyID[k]>6 && DetCopyID[k]<19 )  && (ELoss[k]*1000)>0.04 && InterLay<TrackMult)  // middle layer
			{
			  Xb[InterLay]=X_track[k];
			  Yb[InterLay]=Y_track[k];
			  Zb[InterLay]=Z_track[k];
			  Phib[InterLay]=GetPhiTrk(Xb[InterLay],Yb[InterLay]);
			  Epb[InterLay]=ELoss[k];
			  InterLay++;
			}
		      
		      if( ( DetCopyID[k]>18 && DetCopyID[k]<31 )  && (ELoss[k]*1000)>0.04 && OutLay<TrackMult)  // outer later
			{
			  Xc[OutLay]=X_track[k];
			  Yc[OutLay]=Y_track[k];
			  Zc[OutLay]=Z_track[k];
			  Phic[OutLay]=GetPhiTrk(Xc[OutLay],Yc[OutLay]);
			  Epc[OutLay]=ELoss[k];
			  OutLay++;
			}
		      
		    } // end of k=TrackMult loop.
		    
		    if(InLay!=TrkMult_Inner40) cout << "Attention x and TrckMult_inner40 should be identical !!! InLay: = "<< InLay << " TrkMult_inner40= " << TrkMult_Inner40 << endl;
		    if(InterLay!=TrkMult_Middl40) cout << "Attention x and TrckMult_inner40 should be identical !!! InterLa: = "<< InterLay << " TrkMult_Middl40= " << TrkMult_Middl40 << endl;
		    if(OutLay!=TrkMult_Outer40) cout << "Attention x and TrckMult_inner40 should be identical !!! : OutLay= "<< OutLay << " TrkMult_outer40= " << TrkMult_Outer40 << endl;
                  
		    // Now We are only interested in event with Multiplicity>=4 and the inner and intermediate layer are hit:		
		    //if(InterLay==2 && InterLay==2)  // At least inner and intermediate layer are hit (= better case than inner and outer layers because of straggling in intermediate layer 
		    if(InLay>=2 && InterLay>=2)  // At least inner and intermediate layer are hit (= better case than inner and outer layers because of straggling in intermediate layer 
		      {
                      
			//cout << "TrackMult = " << TrackMult << endl; 
			//cout << "Phia[0] = " << Phia[0]*180/pi << endl; 
			//cout << "Phib[0] = " << Phib[0]*180/pi << endl; 
			//cout << "Phia[1] = " << Phia[1]*180/pi << endl; 
			//cout << "Phib[1] = " << Phib[1]*180/pi << endl; 
			//cout << "Phia[0]-Phib[0] = " << (Phia[0]-Phib[0])*180/pi << endl; 
			//cout << "Phia[0]-Phib[1] = " << (Phia[0]-Phib[1])*180/pi << endl; 
			//cout << "fabs = " << fabs((Phia[0]-Phib[0])/( Phia[0]-Phib[1])) << endl; 
			
			//////////////////////////////////////////////
			// For basic tracking only:
			   theta2=0.;
			   theta1=0.;
			   phi1=0.;
			   phi2=0.;
			   Ep1=0.;
			   Ep2=0.;
                      
			   // 
			   // Basic tracking for 2 tracked particles:
			   //
			   
			   if( fabs((Phia[0]-Phib[0]))<0.1 && fabs((Phia[1]-Phib[1]))<0.1)  // 0.1 radian Window of the azimuthal angle to accept that it is likely to be the same particle
			   {
			   phi1= GetPhiTrk(Xb[0]-Xa[0], Yb[0]-Ya[0]);                   // one calculates the azimuthal angle form the position in inner and intermediate layer
			   theta1= GetThetaTrk(Xb[0]-Xa[0], Yb[0]-Ya[0], Zb[0]-Za[0]);  // one calculates the polar angle form the position in inner and intermediate layer
			   Ep1=Epa[0]+Epb[0]+Epc[0];
			   
			   phi2= GetPhiTrk(Xb[1]-Xa[1], Yb[1]-Ya[1]);
			   theta2= GetThetaTrk(Xb[1]-Xa[1], Yb[1]-Ya[1], Zb[1]-Za[1]);
			   Ep2=Epa[1]+Epb[1]+Epc[1];
			   
			   
			   //filling histograms
			   Theta1Theta2->Fill(theta1*180/pi,theta2*180/pi);
			   Phi1Phi2->Fill(phi1*180/pi,phi2*180/pi);
			   AngTheta1->Fill(theta1*180/pi);
			   AngTheta2->Fill(theta2*180/pi);
			   Phi1Phi2->Fill(phi1*180/pi,phi2*180/pi);
			   AngPhi1->Fill(phi1*180/pi);
			   AngPhi2->Fill(phi2*180/pi);
			   dEp1->Fill(Ep1*1000.);
			   dEp2->Fill(Ep2*1000.);
			   }
			   
			   
			   //cout << "theta1 = " << theta1*180/pi << endl; 
			   //cout << "phi1 = " << phi1*180/pi << endl; 
			   //cout << "theta2 = " << theta2*180/pi << endl; 
			   //cout << "phi2 = " << phi2*180/pi << endl; 
                      
			   
			
			// For general tracking  between inner and intermediate layer:
			for (Int_t tk=0; tk<TrkMult_Middl40; tk++){theta[tk]=0.0; phi[tk]= 0.0; Ep[tk]= 0.0; part_b[tk]=0.0;}		     
			for (Int_t tk=0; tk<TrkMult_Inner40; tk++){part_a[tk]=0;}
			for (Int_t tk=0; tk<TrkMult_Outer40; tk++){part_c[tk]=0;}
			
                      
			// General tracking
			//
			// Case for particle going through 3 layers (Todo: add case when the paricle goes through only 2 layers) 
			//
			
			//cout << "InLay:" << InLay << " InterLay:" << InterLay << " OutLay:" << OutLay << endl;
                      
                        trk = 0;
                    
			if (InLay>0) {
                        for (Int_t ia=0;ia<InLay;ia++) // looking at all hits in Inner layer
			  {
                            if (InterLay>0) {
                              for (Int_t ib=0;ib<InterLay;ib++) // looking at all hits in Intermediate layer
			      {
                              
				PhiDiff_ab->Fill(TMath::Abs((Phia[ia]-Phib[ib])));
				
			        CountTrk++; // Count all tracks identified between inner and outer layer
			        CountTrk2=CountTrk2+0.5; // Count all tracks identified between inner and outer layer (there should be at least 2 tracks
                              
				// let's first compare phi in inner and intermediate layers. (0.2rad has been determined from PhiDiff_bc)				// ToDo Check the track goes thorug the Target+Beam overlap
				if(TMath::Abs((Phia[ia]-Phib[ib]))<0.2 ) // = considered as same track (here we could maybe put more conditions to be more restrictive)
				  {
				    part_a[trk]= (Int_t) ia; 
				    part_b[trk]= (Int_t) ib;
                                    
                                    //cout << " TrkMult_Middl40 = " << TrkMult_Middl40 << " | trk = " << trk << "\n\n";
                                    
                                    if ((trk+1)<(TrackMult)) {trk = trk + 1;} // this is track #trk
                                    
				    //trk++; // this is track #trk
				    //cout << "ia:" << ia << " ib:"<< ib << endl;

				    // Reconstruct the target image (radius, X, Y)
				    DistToBeam(Xa[ia],Ya[ia],Za[ia],Xb[ib],Yb[ib],Zb[ib],1.5, &r, &Xtarg, &Ytarg);
				    //cout << "r=" << r << endl;
				    //cout << "Xtarg=" << Xtarg << " Ytarg=" << Ytarg << endl;

				    // Now let's check the track really comes from the target:
                                  
      				    if(IsFromTarget(Xa[ia],Ya[ia],Za[ia],Xb[ib],Yb[ib],Zb[ib],1.5, 0.001)) // Assuming beam spot radius = 1.5 cm, target thickness 10um
				      {
					part_a[trk]= (Int_t) ia; 
					part_b[trk]= (Int_t) ib;
					if ((trk+1)<(TrackMult)) {trk = trk + 1;} // this is track #trk
					
					if(trk==1)CountGoodTrk++; // increment number of good track identified
					if(trk==2)CountGoodTrk2++; // increment number of good 2 tracks identified
					                           

					//cout << "ia:" << ia << " ib:"<< ib << endl;
					
					if (OutLay>0) {
					for (Int_t ic=0; ic<OutLay; ic++) // looking at all hits in  Outer layer
					  {
					    PhiDiff_bc->Fill(TMath::Abs((Phib[ib]-Phic[ic])));
					    // let's compare phi in intermediate and outer layers. (0.05rad has been determined from PhiDiff_bc)
					    if( TMath::Abs((Phib[ib]-Phic[ic]))<0.05 ) // = considered as same track (here we could maybe put more conditions to be more restrictive)
					      {
						part_b[trk]=ib;
						part_c[trk]=ic;
						//     cout << "ia:" << ia << " ib:"<< ib << " ic:" << ic << endl;
					      }
					    
					  }}

					
					XTargYTarg->Fill(Xtarg,Ytarg);
					TargRadius->Fill(r);					

				      } // end of IsFromTarget
			    
				  } // end of Phi condition

			      }}
			  }}
			
                      
                         
			
			//loop other the tracks when the number of track is higher than 1: 
			
			//cout << "trk=" << trk << endl;
			//cout << " "<< endl;
			Tracks->Fill((Int_t) trk);
			
                                                                       
			if(trk>0)
			  {
			    for (Int_t t=0;t<trk;t++) // 
			      {
				if ((((Int_t) part_a[t])<TrackMult)&&(((Int_t) part_b[t])<TrackMult)&&(((Int_t) part_c[t])<TrackMult)&&(t<TrackMult))
                                {
                                    if ((((Int_t) part_a[t])>-1)&&(((Int_t) part_b[t])>-1)&&(((Int_t) part_c[t])>-1)&&(t>-1))
                                    {
                                  
				phi[t]= GetPhiTrk( Xb[ (Int_t) part_b[t] ]- Xa[ (Int_t) part_a[t] ], Yb[ (Int_t) part_b[t] ]-Ya[ (Int_t) part_a[t] ] );   // one calculates the azimuthal angle form the position in inner and intermediate layer
				theta[t]= GetThetaTrk( Xb[ (Int_t) part_b[t] ]- Xa[ (Int_t) part_a[t] ], Yb[ (Int_t) part_b[t] ]-Ya[ (Int_t) part_a[t]  ], Zb[ (Int_t) part_b[t] ]-Za[ (Int_t) part_a[t] ] );  // one calculates the polar angle form the position in inner and intermediate layer
				
                                Ep[t] = Epa[ (Int_t) part_a[t] ] + Epb[ (Int_t) part_b[t] ] + Epc[ (Int_t) part_c[t] ];
                                }}
				
			      }
                          
			    
			    if(trk==2)  // ie there is 2 tracks only 
			      {
				
				//filling histograms
				Theta1Theta2->Fill(theta[0]*180/pi,theta[1]*180/pi);
				Phi1Phi2->Fill(phi[0]*180/pi,phi[1]*180/pi);
				AngTheta1->Fill(theta[0]*180/pi);
				AngTheta2->Fill(theta[1]*180/pi);
				Phi1Phi2->Fill(phi[0]*180/pi,phi[1]*180/pi);
				AngPhi1->Fill(phi[0]*180/pi);
				AngPhi2->Fill(phi[1]*180/pi);
				dEp1->Fill(Ep[0]*1000.);
				dEp2->Fill(Ep[1]*1000.);
				
			      }
			    
			    if(trk>2)  // ie there is more than 2 tracks detected 
			      {
				for (Int_t t=0;t<trk;t++) // 
				  {
				    //cout << "fabs(phi[t]-phi[t+1])=" << fabs(phi[t]-phi[t+1]) << " " << phi[t] << " " <<  phi[t+1] << endl;
				    if( fabs(phi[t]-phi[t+1])>(pi-1) && fabs(phi[t]-phi[t+1])<(pi+1) )
				      {
					//filling histograms
					Theta1Theta2->Fill(theta[t]*180/pi,theta[t+1]*180/pi);
					Phi1Phi2->Fill(phi[t]*180/pi,phi[t+1]*180/pi);
					AngTheta1->Fill(theta[t]*180/pi);
					AngTheta2->Fill(theta[t+1]*180/pi);
					Phi1Phi2->Fill(phi[t]*180/pi,phi[t+1]*180/pi);
					AngPhi1->Fill(phi[t]*180/pi);
					AngPhi2->Fill(phi[t+1]*180/pi);
					dEp1->Fill(Ep[t]*1000.);
					dEp2->Fill(Ep[t+1]*1000.);
				      }
				    
				    
				  }
				
			      }
			    
			  }
			/////////////////// End of general tracking
			
		    
		      }

		

		// Todo: 
		// Calculate Ep1 and Ep2 with additional information from the calorimeter, and the invariant mass with:
		// Inv_Mass= GetERec(Ep1, Ep2, theta1, phi1, theta2, phi2, Ebeam);
		// InvMass=->Fill(Inv_Mass);

		  }   // end of Multiplicity >= 4 and end of p2p reconstruction.


	      }// end of p2p track analysis

	      // ----------------------------------------------------------------------------------------------------
              // filling the output tree:
            
              Tout->SetBranchAddress("TrackSize",&TrackMult);
              Tout->SetBranchAddress("Xtrack",(void*) X_track);
              Tout->SetBranchAddress("Ytrack",(void*) Y_track);
              Tout->SetBranchAddress("Ztrack",(void*) Z_track);
              Tout->SetBranchAddress("ThetaTrack",(void*) Theta_track);
              Tout->SetBranchAddress("PhiTrack",(void*) Phi_track);
              Tout->SetBranchAddress("ElossTrack",(void*) ELoss);
              
              Tout->SetBranchAddress("NumberOfSolvedTracks",&trk);
              Tout->SetBranchAddress("NumberOfGoodTracks",&CountGoodTrk);
              Tout->SetBranchAddress("MiddleMult",&TrkMult_Middl40);
              Tout->SetBranchAddress("theta",(void*) theta);
              Tout->SetBranchAddress("phi",(void*) phi);
              Tout->SetBranchAddress("Ep",(void*) Ep);
                           
              Tout->Fill();
              
              // ----------------------------------------------------------------------------------------------------
              
	delete[]  Mother_part;
	delete[]  PDG_part;
	delete[]  PX_part;
	delete[]  PY_part;
	delete[]  PZ_part;

	delete[] Stripfrt;  // front strip ID
	delete[] Stripbck;  // back Strip ID
	delete[] StripfrtClust;  // main front Strip ID in cluster
	delete[] StripbckClust;  // main back Strip ID in cluster
 
	delete[]  X_track;
	delete[]  Y_track;
	delete[]  Z_track;
	delete[]  PX_track;
	delete[]  PY_track;
	delete[]  PZ_track;
	delete[]  Theta_track;
	delete[]  Phi_track;
	delete[]  PTot;
	delete[]  ELoss;
	delete[]  ELossClust;
	delete[]  DetID;
	delete[]  DetCopyID;
	delete[]  DetCopyIDClust;
	delete[]  EDepos;

	delete[]  Chip;
	delete[]  Side;
	delete[]  Strip;
	delete[]  DetCopyID_init;
	delete[]  ELoss_init;
	delete[]  TimeStp_init;

	delete[]  X_Xball;
	delete[]  Y_Xball;
	delete[]  Z_Xball;
	delete[]  ELossCryst;
	delete[]  Cryst_Type;
	delete[]  Cryst_Nb;


	delete[]  theta;
	delete[]  phi;
	delete[]  Ep;


	delete[] Xa;  // inner layer
	delete[] Ya;
	delete[] Za;
	delete[] Phia;
	delete[] Epa;
	delete[] part_a;
	delete[] Xb;  // intermidiate layer
	delete[] Yb;
	delete[] Zb;
	delete[] Phib;
	delete[] Epb;
	delete[] part_b;
	delete[] Xc;  // Outer layer
	delete[] Yc;
	delete[] Zc;
	delete[] Phic;
	delete[] Epc;
	delete[] part_c;

	// } // end of TrackMult<XballMult       // uncomment if XBall used
 
  } // end of event loop


	if(mode==0)
	  {
	    cout << "Tracking Efficiency (tracks identified as coming from target / total number of tracks) : " << CountGoodTrk*100./CountTrk  << "%" << endl;
	    cout << "Tracking Global Efficiency (tracks identified as coming from target / total number of events): " << CountGoodTrk*100./nevents << "%" << endl;
	  }else if(mode==1)
	  {
	    cout << "Tracking Efficiency (tracks identified as coming from target / total number of tracks) : " << CountGoodTrk*100./CountTrk  << "%" << endl;
	    cout << " Pair of Tracks identified as coming from the target : " << CountGoodTrk2 << " Events with at least 2 tracks " << CountTrk2  << endl;
	    cout << "Tracking Efficiency for 2 tracks (nb of pair of tracks identified as coming from target / total number of pair of track) : " << CountGoodTrk2*100./int(CountTrk2)  << "%" << endl;
	    cout << "Tracking Global Efficiency (pair of tracks identified as coming from target / total number of events): " << CountGoodTrk2*100./nevents << "%" << endl;

	  }


  // save histogram in output file

  Xtrack->Write();
  Ytrack->Write();
  Ztrack->Write();
  XtrackZtrack->Write();
  XtrackYtrack->Write();
  ZtrackYtrack->Write();


  TrackMult_Inner->Write();
  TrackMult_Middl->Write();
  TrackMult_Outer->Write();
  TrackMult_Inner40->Write();
  TrackMult_Middl40->Write();
  TrackMult_Outer40->Write();

  StripA_Inner->Write();
  StripB_Inner->Write();
  StripA_Middl->Write();
  StripB_Middl->Write();
  StripA_Outer->Write();
  StripB_Outer->Write();

  StripA_Inner40->Write();
  StripB_Inner40->Write();
  StripA_Middl40->Write();
  StripB_Middl40->Write();
  StripA_Outer40->Write();
  StripB_Outer40->Write();

  Theta->Write();
  Phi->Write();

  ELossInner->Write();
  ELossMiddl->Write();
  ELossOuter->Write();

  EnergyDeltaE->Write();


  ThetaELoss->Write();

  ThetaELoss1Cryst->Write();

  TargRadius->Write();
  XTargYTarg->Write();

  Tracks->Write();
  PhiDiff_ab->Write();
  PhiDiff_bc->Write();
 
 Theta1Theta2->Write();
  Phi1Phi2->Write();
  AngTheta1->Write();
  AngTheta2->Write();
  AngPhi1->Write();
  AngPhi2->Write();
  dEp1->Write();
  dEp2->Write();

  dE_E->Write();
  InvMass->Write();


  //  for elastic:
  E_detected->Write();
  Total_E->Write();
  Energy_reso->Write();
  Theta_reso->Write();
  EDiff->Write();
  Theta_cm_reso->Write();
  Thetacm->Write();
  Theta12->Write();
  ETheta->Write();
  ETh_rec->Write();
  ThE_rec->Write();
  
  Tout->Write();

  outFile->Close();
}	 

// ======================================================================================================================
// Define main functions for stand alone compilation:
void STarTrackReconstructor()
{
  TFile* inputs = new TFile("../../Data/Inputs/Inputs.root","read");
  TH1D* Doubles = (TH1D*) inputs->Get("Doubles");
  TH1I* Integers = (TH1I*) inputs->Get("Integers");
  
  // Then we get what we need:
  TObjString* Output_Name_Obj = (TObjString*) inputs->Get("Output_Name");
  TString Output_Name = Output_Name_Obj->GetString();
  Int_t Nr_Run = Integers->GetBinContent(1);
  TString st = "";
  TString Nr_Run_str = st.Itoa(Nr_Run,10);
  Int_t STaRTrack_Exists = Integers->GetBinContent(207);

  // Define inputs for the track reconstructor:
  Int_t mode = Integers->GetBinContent(250);
  TString InputFile = "../../Data/Digitizer/" + Output_Name + "_run=" + Nr_Run_str + "_Others.root";
  TString OutputFile = "../../Data/Digitizer/" + Output_Name + "_run=" + Nr_Run_str + "_STaRTrack.root";
  
  // And then finally run it:
  if (STaRTrack_Exists==1)
  {
    STaRTrack_TrackFinder(mode,InputFile.Data(),OutputFile.Data());
  }
  else
  {
      cout << "### NOTE: You cannot run the STarTrack Track Reconstructor when STaRTrack does not exist!\n\n\n";
  }
  
  // Done!
}

// Now define the main for stand-alone compilation:
#ifndef __CINT__
#include <TApplication.h>
int main(int argc, char** argv)
{
  TApplication* TheApp = new TApplication("STarTrackReconstructor",&argc,argv);
  STarTrackReconstructor();
  return 0;
}
#endif


