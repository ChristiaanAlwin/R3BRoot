#define tLand_cxx
//#include "tLand.h"
#include "tLand4.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void tLand::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L tLand.C
//      Root > tLand t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Inser statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
  if (fChain == 0) {
    printf("No chain.\n");
    return;
  }


   // Cutoff values
   Double_t dEthr  =  0.003; // [GeV]
   Double_t Toflwr = 50.0  ; // [ns]
   Double_t Tofupr = 70.0  ; // [ns]  
   //   Double_t Toflwr = 0.0  ; // [ns]
   //   Double_t Tofupr = 10000000.0  ; // [ns]  

   // >>>>>>>>> Definition of histograms >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   // ** neglect Pi0&neutrino contributions because they are very small **
   // Nnp01x distribution 
   TH1F *hNnp01x   = new TH1F("hNnp01x"  ,"Nnp01x",60,-150.0,150.0); // all
   TH1F *hNnp01xtn = new TH1F("hNnp01xtn","Nnp01x",60,-150.0,150.0); // neutron from target
   TH1F *hNnp01xon = new TH1F("hNnp01xon","Nnp01x",60,-150.0,150.0); // neutron from others
   TH1F *hNnp01xto = new TH1F("hNnp01xto","Nnp01x",60,-150.0,150.0); // others  from target
   TH1F *hNnp01xoo = new TH1F("hNnp01xoo","Nnp01x",60,-150.0,150.0); // others  from others

   TH1F *hNnp01xap  = new TH1F("hNnp01xap" ,"Nnp01x",60,-150.0,150.0); // proton            from anywhere
   TH1F *hNnp01xadt = new TH1F("hNnp01xadt","Nnp01x",60,-150.0,150.0); // deuteron & triton from anywhere
   TH1F *hNnp01xahe = new TH1F("hNnp01xahe","Nnp01x",60,-150.0,150.0); // helium isotopes   from anywhere
   TH1F *hNnp01xag  = new TH1F("hNnp01xag" ,"Nnp01x",60,-150.0,150.0); // gamma             from anywhere
   TH1F *hNnp01xapm = new TH1F("hNnp01xapm","Nnp01x",60,-150.0,150.0); // pion & muon       from anywhere

   // Histograms for Production position (neutron hitting Nnp01)
   TH1F *hZReactn  = new TH1F("hZReactn" ,"ZReact"          , 200,-200.0,1800.0                  );
   TH2F *hZXReactn = new TH2F("hZXReactn","XReact vs ZReact", 200,-200.0,1800.0, 120,-400.0,200.0);
   TH2F *hZYReactn = new TH2F("hZYReactn","YReact vs ZReact", 200,-200.0,1800.0, 120,-400.0,200.0);

   // Histograms for Production position (othres  hitting Nnp01)
   TH1F *hZReacto  = new TH1F("hZReacto" ,"ZReact"          , 200,-200.0,1800.0                  );
   TH2F *hZXReacto = new TH2F("hZXReacto","XReact vs ZReact", 200,-200.0,1800.0, 120,-400.0,200.0);
   TH2F *hZYReacto = new TH2F("hZYReacto","YReact vs ZReact", 200,-200.0,1800.0, 120,-400.0,200.0);
   // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   Long64_t nentries = fChain->GetEntriesFast();
   printf("Nentries: %lld\n",nentries); fflush(stdout);

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      if(jentry%1000==0){printf("\r%lld",jentry); fflush(stdout);}
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;

      for(Int_t iHit01=0; iHit01<Nnp01n; iHit01++){
	if(Nnp01e[iHit01]>dEthr && Nnp01t[iHit01]>Toflwr && Nnp01t[iHit01]<Tofupr) {
	  
	  hNnp01x->Fill(Nnp01x[iHit01]);

	  if(Nnp01p[iHit01]==2112){ // neutron
	    hZReactn->Fill(Nnp01zReact[iHit01]);
	    hZXReactn->Fill(Nnp01zReact[iHit01],Nnp01xReact[iHit01]);
	    hZYReactn->Fill(Nnp01zReact[iHit01],Nnp01yReact[iHit01]);
	    if(abs(Nnp01zReact[iHit01])<5.0){ // from target
	      hNnp01xtn->Fill(Nnp01x[iHit01]);
	    } else {                          // from other places
	      hNnp01xon->Fill(Nnp01x[iHit01]);
	    }
	  } else {                 // others
	    hZReacto->Fill(Nnp01zReact[iHit01]);
	    hZXReacto->Fill(Nnp01zReact[iHit01],Nnp01xReact[iHit01]);
	    hZYReacto->Fill(Nnp01zReact[iHit01],Nnp01yReact[iHit01]);
	    if(abs(Nnp01zReact[iHit01])<5.0){ // from target
	      hNnp01xto->Fill(Nnp01x[iHit01]);
	    } else {                          // from other places
	      hNnp01xoo->Fill(Nnp01x[iHit01]);
	    }
	    
	    if(Nnp01p[iHit01]==2212) {                                            // proton            from anywhere
	      hNnp01xap->Fill(Nnp01x[iHit01]);
	    } else if(Nnp01p[iHit01]==1000010020 || Nnp01p[iHit01]==1000010030) { // deuteron & triton from anywhere
	      hNnp01xadt->Fill(Nnp01x[iHit01]);
	    } else if(Nnp01p[iHit01]> 1000020000 && Nnp01p[iHit01]< 1000030000) { // Helium isotopes   from anywhere
	      hNnp01xahe->Fill(Nnp01x[iHit01]);
	    } else if(Nnp01p[iHit01]==22) {                                       // gamma             from anywhere
	      hNnp01xag->Fill(Nnp01x[iHit01]);
	    } else if(abs(Nnp01p[iHit01])==211 || Nnp01p[iHit01]==111             // pions
                                               || abs(Nnp01p[iHit01])==13)      { // muons             from anywhere
	      hNnp01xapm->Fill(Nnp01x[iHit01]);
	    }
	  }
	}
      }
	
   }

   TFile fout("tLand_hist.root","RECREATE");
   fout.cd();
   hNnp01x->Write();
   hNnp01xtn->Write();
   hNnp01xon->Write();
   hNnp01xto->Write();
   hNnp01xoo->Write();

   hNnp01xap->Write();
   hNnp01xadt->Write();
   hNnp01xahe->Write();
   hNnp01xag->Write();
   hNnp01xapm->Write();
   
   hZReactn->Write();
   hZXReactn->Write();
   hZYReactn->Write();
   hZReacto->Write();
   hZXReacto->Write();
   hZYReacto->Write();
}
