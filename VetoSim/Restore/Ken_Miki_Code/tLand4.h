//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sun Aug 10 10:29:22 2014 by ROOT version 5.34/13
// from TTree tLand/hoge analysis
// found on file: tLand2.root
//////////////////////////////////////////////////////////

#ifndef tLand_h
#define tLand_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class tLand {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   static const int maxHit=20;
   static const int GFIMaxHit=100;

   // Declaration of leaf types
   Int_t           Event;
   Int_t           GFIn;
   Int_t           GFIp[GFIMaxHit];
   Int_t           Nnp01n;
   Int_t           Nnp01i[maxHit];   //[Nnp01n]
   Double_t        Nnp01t[maxHit];   //[Nnp01n]
   Double_t        Nnp01e[maxHit];   //[Nnp01n]
   Double_t        Nnp01x[maxHit];   //[Nnp01n]
   Double_t        Nnp01y[maxHit];   //[Nnp01n]
   Double_t        Nnp01z[maxHit];   //[Nnp01n]
   Double_t        Nnp01xReact[maxHit];   //[Nnp01n]
   Double_t        Nnp01yReact[maxHit];   //[Nnp01n]
   Double_t        Nnp01zReact[maxHit];   //[Nnp01n]
   Int_t           Nnp01p[maxHit];   //[Nnp01n]
   Int_t           Nnp02n;
   Int_t           Nnp02i[maxHit];   //[Nnp02n]
   Double_t        Nnp02t[maxHit];   //[Nnp02n]
   Double_t        Nnp02e[maxHit];   //[Nnp02n]
   Double_t        Nnp02x[maxHit];   //[Nnp02n]
   Double_t        Nnp02y[maxHit];   //[Nnp02n]
   Double_t        Nnp02z[maxHit];   //[Nnp02n]
   Double_t        Nnp02xReact[maxHit];   //[Nnp02n]
   Double_t        Nnp02yReact[maxHit];   //[Nnp02n]
   Double_t        Nnp02zReact[maxHit];   //[Nnp02n]
   Int_t           Nnp02p[maxHit];   //[Nnp02n]
   Int_t           Nnp03n;
   Int_t           Nnp03i[maxHit];   //[Nnp03n]
   Double_t        Nnp03t[maxHit];   //[Nnp03n]
   Double_t        Nnp03e[maxHit];   //[Nnp03n]
   Double_t        Nnp03x[maxHit];   //[Nnp03n]
   Double_t        Nnp03y[maxHit];   //[Nnp03n]
   Double_t        Nnp03z[maxHit];   //[Nnp03n]
   Double_t        Nnp03xReact[maxHit];   //[Nnp03n]
   Double_t        Nnp03yReact[maxHit];   //[Nnp03n]
   Double_t        Nnp03zReact[maxHit];   //[Nnp03n]
   Int_t           Nnp03p[maxHit];   //[Nnp03n]
   Int_t           Nnp04n;
   Int_t           Nnp04i[maxHit];   //[Nnp04n]
   Double_t        Nnp04t[maxHit];   //[Nnp04n]
   Double_t        Nnp04e[maxHit];   //[Nnp04n]
   Double_t        Nnp04x[maxHit];   //[Nnp04n]
   Double_t        Nnp04y[maxHit];   //[Nnp04n]
   Double_t        Nnp04z[maxHit];   //[Nnp04n]
   Double_t        Nnp04xReact[maxHit];   //[Nnp04n]
   Double_t        Nnp04yReact[maxHit];   //[Nnp04n]
   Double_t        Nnp04zReact[maxHit];   //[Nnp04n]
   Int_t           Nnp04p[maxHit];   //[Nnp04n]
   Int_t           Nnp05n;
   Int_t           Nnp05i[maxHit];   //[Nnp05n]
   Double_t        Nnp05t[maxHit];   //[Nnp05n]
   Double_t        Nnp05e[maxHit];   //[Nnp05n]
   Double_t        Nnp05x[maxHit];   //[Nnp05n]
   Double_t        Nnp05y[maxHit];   //[Nnp05n]
   Double_t        Nnp05z[maxHit];   //[Nnp05n]
   Double_t        Nnp05xReact[maxHit];   //[Nnp05n]
   Double_t        Nnp05yReact[maxHit];   //[Nnp05n]
   Double_t        Nnp05zReact[maxHit];   //[Nnp05n]
   Int_t           Nnp05p[maxHit];   //[Nnp05n]
   Int_t           Nnp06n;
   Int_t           Nnp06i[maxHit];   //[Nnp06n]
   Double_t        Nnp06t[maxHit];   //[Nnp06n]
   Double_t        Nnp06e[maxHit];   //[Nnp06n]
   Double_t        Nnp06x[maxHit];   //[Nnp06n]
   Double_t        Nnp06y[maxHit];   //[Nnp06n]
   Double_t        Nnp06z[maxHit];   //[Nnp06n]
   Double_t        Nnp06xReact[maxHit];   //[Nnp06n]
   Double_t        Nnp06yReact[maxHit];   //[Nnp06n]
   Double_t        Nnp06zReact[maxHit];   //[Nnp06n]
   Int_t           Nnp06p[maxHit];   //[Nnp06n]
   Int_t           Nnp07n;
   Int_t           Nnp07i[maxHit];   //[Nnp07n]
   Double_t        Nnp07t[maxHit];   //[Nnp07n]
   Double_t        Nnp07e[maxHit];   //[Nnp07n]
   Double_t        Nnp07x[maxHit];   //[Nnp07n]
   Double_t        Nnp07y[maxHit];   //[Nnp07n]
   Double_t        Nnp07z[maxHit];   //[Nnp07n]
   Double_t        Nnp07xReact[maxHit];   //[Nnp07n]
   Double_t        Nnp07yReact[maxHit];   //[Nnp07n]
   Double_t        Nnp07zReact[maxHit];   //[Nnp07n]
   Int_t           Nnp07p[maxHit];   //[Nnp07n]
   Int_t           Nnp08n;
   Int_t           Nnp08i[maxHit];   //[Nnp08n]
   Double_t        Nnp08t[maxHit];   //[Nnp08n]
   Double_t        Nnp08e[maxHit];   //[Nnp08n]
   Double_t        Nnp08x[maxHit];   //[Nnp08n]
   Double_t        Nnp08y[maxHit];   //[Nnp08n]
   Double_t        Nnp08z[maxHit];   //[Nnp08n]
   Double_t        Nnp08xReact[maxHit];   //[Nnp08n]
   Double_t        Nnp08yReact[maxHit];   //[Nnp08n]
   Double_t        Nnp08zReact[maxHit];   //[Nnp08n]
   Int_t           Nnp08p[maxHit];   //[Nnp08n]

   // List of branches
   TBranch        *b_Event;   //!
   TBranch        *b_GFIn;   //!
   TBranch        *b_GFIp;   //!
   TBranch        *b_Nnp01n;   //!
   TBranch        *b_Nnp01i;   //!
   TBranch        *b_Nnp01t;   //!
   TBranch        *b_Nnp01e;   //!
   TBranch        *b_Nnp01x;   //!
   TBranch        *b_Nnp01y;   //!
   TBranch        *b_Nnp01z;   //!
   TBranch        *b_Nnp01xReact;   //!
   TBranch        *b_Nnp01yReact;   //!
   TBranch        *b_Nnp01zReact;   //!
   TBranch        *b_Nnp01p;   //!
   TBranch        *b_Nnp02n;   //!
   TBranch        *b_Nnp02i;   //!
   TBranch        *b_Nnp02t;   //!
   TBranch        *b_Nnp02e;   //!
   TBranch        *b_Nnp02x;   //!
   TBranch        *b_Nnp02y;   //!
   TBranch        *b_Nnp02z;   //!
   TBranch        *b_Nnp02xReact;   //!
   TBranch        *b_Nnp02yReact;   //!
   TBranch        *b_Nnp02zReact;   //!
   TBranch        *b_Nnp02p;   //!
   TBranch        *b_Nnp03n;   //!
   TBranch        *b_Nnp03i;   //!
   TBranch        *b_Nnp03t;   //!
   TBranch        *b_Nnp03e;   //!
   TBranch        *b_Nnp03x;   //!
   TBranch        *b_Nnp03y;   //!
   TBranch        *b_Nnp03z;   //!
   TBranch        *b_Nnp03xReact;   //!
   TBranch        *b_Nnp03yReact;   //!
   TBranch        *b_Nnp03zReact;   //!
   TBranch        *b_Nnp03p;   //!
   TBranch        *b_Nnp04n;   //!
   TBranch        *b_Nnp04i;   //!
   TBranch        *b_Nnp04t;   //!
   TBranch        *b_Nnp04e;   //!
   TBranch        *b_Nnp04x;   //!
   TBranch        *b_Nnp04y;   //!
   TBranch        *b_Nnp04z;   //!
   TBranch        *b_Nnp04xReact;   //!
   TBranch        *b_Nnp04yReact;   //!
   TBranch        *b_Nnp04zReact;   //!
   TBranch        *b_Nnp04p;   //!
   TBranch        *b_Nnp05n;   //!
   TBranch        *b_Nnp05i;   //!
   TBranch        *b_Nnp05t;   //!
   TBranch        *b_Nnp05e;   //!
   TBranch        *b_Nnp05x;   //!
   TBranch        *b_Nnp05y;   //!
   TBranch        *b_Nnp05z;   //!
   TBranch        *b_Nnp05xReact;   //!
   TBranch        *b_Nnp05yReact;   //!
   TBranch        *b_Nnp05zReact;   //!
   TBranch        *b_Nnp05p;   //!
   TBranch        *b_Nnp06n;   //!
   TBranch        *b_Nnp06i;   //!
   TBranch        *b_Nnp06t;   //!
   TBranch        *b_Nnp06e;   //!
   TBranch        *b_Nnp06x;   //!
   TBranch        *b_Nnp06y;   //!
   TBranch        *b_Nnp06z;   //!
   TBranch        *b_Nnp06xReact;   //!
   TBranch        *b_Nnp06yReact;   //!
   TBranch        *b_Nnp06zReact;   //!
   TBranch        *b_Nnp06p;   //!
   TBranch        *b_Nnp07n;   //!
   TBranch        *b_Nnp07i;   //!
   TBranch        *b_Nnp07t;   //!
   TBranch        *b_Nnp07e;   //!
   TBranch        *b_Nnp07x;   //!
   TBranch        *b_Nnp07y;   //!
   TBranch        *b_Nnp07z;   //!
   TBranch        *b_Nnp07xReact;   //!
   TBranch        *b_Nnp07yReact;   //!
   TBranch        *b_Nnp07zReact;   //!
   TBranch        *b_Nnp07p;   //!
   TBranch        *b_Nnp08n;   //!
   TBranch        *b_Nnp08i;   //!
   TBranch        *b_Nnp08t;   //!
   TBranch        *b_Nnp08e;   //!
   TBranch        *b_Nnp08x;   //!
   TBranch        *b_Nnp08y;   //!
   TBranch        *b_Nnp08z;   //!
   TBranch        *b_Nnp08xReact;   //!
   TBranch        *b_Nnp08yReact;   //!
   TBranch        *b_Nnp08zReact;   //!
   TBranch        *b_Nnp08p;   //!

   tLand(TTree *tree=0);
   virtual ~tLand();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual void     PDG2AZ(Int_t pdgid, Int_t A, Int_t Z);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef tLand_cxx
tLand::tLand(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("tLand4.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("tLand4.root");
      }
      f->GetObject("tLand",tree);

   }
   Init(tree);
}

tLand::~tLand()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t tLand::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t tLand::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void tLand::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("Event", &Event, &b_Event);

   fChain->SetBranchAddress("GFIn", &GFIn, &b_GFIn);
   fChain->SetBranchAddress("GFIp",  GFIp, &b_GFIp);

   fChain->SetBranchAddress("Nnp01n", &Nnp01n, &b_Nnp01n);
   fChain->SetBranchAddress("Nnp01i", Nnp01i, &b_Nnp01i);
   fChain->SetBranchAddress("Nnp01t", Nnp01t, &b_Nnp01t);
   fChain->SetBranchAddress("Nnp01e", Nnp01e, &b_Nnp01e);
   fChain->SetBranchAddress("Nnp01x", Nnp01x, &b_Nnp01x);
   fChain->SetBranchAddress("Nnp01y", Nnp01y, &b_Nnp01y);
   fChain->SetBranchAddress("Nnp01z", Nnp01z, &b_Nnp01z);
   fChain->SetBranchAddress("Nnp01xReact", Nnp01xReact, &b_Nnp01xReact);
   fChain->SetBranchAddress("Nnp01yReact", Nnp01yReact, &b_Nnp01yReact);
   fChain->SetBranchAddress("Nnp01zReact", Nnp01zReact, &b_Nnp01zReact);
   fChain->SetBranchAddress("Nnp01p", Nnp01p, &b_Nnp01p);
   fChain->SetBranchAddress("Nnp02n", &Nnp02n, &b_Nnp02n);
   fChain->SetBranchAddress("Nnp02i", Nnp02i, &b_Nnp02i);
   fChain->SetBranchAddress("Nnp02t", Nnp02t, &b_Nnp02t);
   fChain->SetBranchAddress("Nnp02e", Nnp02e, &b_Nnp02e);
   fChain->SetBranchAddress("Nnp02x", Nnp02x, &b_Nnp02x);
   fChain->SetBranchAddress("Nnp02y", Nnp02y, &b_Nnp02y);
   fChain->SetBranchAddress("Nnp02z", Nnp02z, &b_Nnp02z);
   fChain->SetBranchAddress("Nnp02xReact", Nnp02xReact, &b_Nnp02xReact);
   fChain->SetBranchAddress("Nnp02yReact", Nnp02yReact, &b_Nnp02yReact);
   fChain->SetBranchAddress("Nnp02zReact", Nnp02zReact, &b_Nnp02zReact);
   fChain->SetBranchAddress("Nnp02p", Nnp02p, &b_Nnp02p);
   fChain->SetBranchAddress("Nnp03n", &Nnp03n, &b_Nnp03n);
   fChain->SetBranchAddress("Nnp03i", Nnp03i, &b_Nnp03i);
   fChain->SetBranchAddress("Nnp03t", Nnp03t, &b_Nnp03t);
   fChain->SetBranchAddress("Nnp03e", Nnp03e, &b_Nnp03e);
   fChain->SetBranchAddress("Nnp03x", Nnp03x, &b_Nnp03x);
   fChain->SetBranchAddress("Nnp03y", Nnp03y, &b_Nnp03y);
   fChain->SetBranchAddress("Nnp03z", Nnp03z, &b_Nnp03z);
   fChain->SetBranchAddress("Nnp03xReact", Nnp03xReact, &b_Nnp03xReact);
   fChain->SetBranchAddress("Nnp03yReact", Nnp03yReact, &b_Nnp03yReact);
   fChain->SetBranchAddress("Nnp03zReact", Nnp03zReact, &b_Nnp03zReact);
   fChain->SetBranchAddress("Nnp03p", Nnp03p, &b_Nnp03p);
   fChain->SetBranchAddress("Nnp04n", &Nnp04n, &b_Nnp04n);
   fChain->SetBranchAddress("Nnp04i", Nnp04i, &b_Nnp04i);
   fChain->SetBranchAddress("Nnp04t", Nnp04t, &b_Nnp04t);
   fChain->SetBranchAddress("Nnp04e", Nnp04e, &b_Nnp04e);
   fChain->SetBranchAddress("Nnp04x", Nnp04x, &b_Nnp04x);
   fChain->SetBranchAddress("Nnp04y", Nnp04y, &b_Nnp04y);
   fChain->SetBranchAddress("Nnp04z", Nnp04z, &b_Nnp04z);
   fChain->SetBranchAddress("Nnp04xReact", Nnp04xReact, &b_Nnp04xReact);
   fChain->SetBranchAddress("Nnp04yReact", Nnp04yReact, &b_Nnp04yReact);
   fChain->SetBranchAddress("Nnp04zReact", Nnp04zReact, &b_Nnp04zReact);
   fChain->SetBranchAddress("Nnp04p", Nnp04p, &b_Nnp04p);
   fChain->SetBranchAddress("Nnp05n", &Nnp05n, &b_Nnp05n);
   fChain->SetBranchAddress("Nnp05i", Nnp05i, &b_Nnp05i);
   fChain->SetBranchAddress("Nnp05t", Nnp05t, &b_Nnp05t);
   fChain->SetBranchAddress("Nnp05e", Nnp05e, &b_Nnp05e);
   fChain->SetBranchAddress("Nnp05x", Nnp05x, &b_Nnp05x);
   fChain->SetBranchAddress("Nnp05y", Nnp05y, &b_Nnp05y);
   fChain->SetBranchAddress("Nnp05z", Nnp05z, &b_Nnp05z);
   fChain->SetBranchAddress("Nnp05xReact", Nnp05xReact, &b_Nnp05xReact);
   fChain->SetBranchAddress("Nnp05yReact", Nnp05yReact, &b_Nnp05yReact);
   fChain->SetBranchAddress("Nnp05zReact", Nnp05zReact, &b_Nnp05zReact);
   fChain->SetBranchAddress("Nnp05p", Nnp05p, &b_Nnp05p);
   fChain->SetBranchAddress("Nnp06n", &Nnp06n, &b_Nnp06n);
   fChain->SetBranchAddress("Nnp06i", Nnp06i, &b_Nnp06i);
   fChain->SetBranchAddress("Nnp06t", Nnp06t, &b_Nnp06t);
   fChain->SetBranchAddress("Nnp06e", Nnp06e, &b_Nnp06e);
   fChain->SetBranchAddress("Nnp06x", Nnp06x, &b_Nnp06x);
   fChain->SetBranchAddress("Nnp06y", Nnp06y, &b_Nnp06y);
   fChain->SetBranchAddress("Nnp06z", Nnp06z, &b_Nnp06z);
   fChain->SetBranchAddress("Nnp06xReact", Nnp06xReact, &b_Nnp06xReact);
   fChain->SetBranchAddress("Nnp06yReact", Nnp06yReact, &b_Nnp06yReact);
   fChain->SetBranchAddress("Nnp06zReact", Nnp06zReact, &b_Nnp06zReact);
   fChain->SetBranchAddress("Nnp06p", Nnp06p, &b_Nnp06p);
   fChain->SetBranchAddress("Nnp07n", &Nnp07n, &b_Nnp07n);
   fChain->SetBranchAddress("Nnp07i", Nnp07i, &b_Nnp07i);
   fChain->SetBranchAddress("Nnp07t", Nnp07t, &b_Nnp07t);
   fChain->SetBranchAddress("Nnp07e", Nnp07e, &b_Nnp07e);
   fChain->SetBranchAddress("Nnp07x", Nnp07x, &b_Nnp07x);
   fChain->SetBranchAddress("Nnp07y", Nnp07y, &b_Nnp07y);
   fChain->SetBranchAddress("Nnp07z", Nnp07z, &b_Nnp07z);
   fChain->SetBranchAddress("Nnp07xReact", Nnp07xReact, &b_Nnp07xReact);
   fChain->SetBranchAddress("Nnp07yReact", Nnp07yReact, &b_Nnp07yReact);
   fChain->SetBranchAddress("Nnp07zReact", Nnp07zReact, &b_Nnp07zReact);
   fChain->SetBranchAddress("Nnp07p", Nnp07p, &b_Nnp07p);
   fChain->SetBranchAddress("Nnp08n", &Nnp08n, &b_Nnp08n);
   fChain->SetBranchAddress("Nnp08i", Nnp08i, &b_Nnp08i);
   fChain->SetBranchAddress("Nnp08t", Nnp08t, &b_Nnp08t);
   fChain->SetBranchAddress("Nnp08e", Nnp08e, &b_Nnp08e);
   fChain->SetBranchAddress("Nnp08x", Nnp08x, &b_Nnp08x);
   fChain->SetBranchAddress("Nnp08y", Nnp08y, &b_Nnp08y);
   fChain->SetBranchAddress("Nnp08z", Nnp08z, &b_Nnp08z);
   fChain->SetBranchAddress("Nnp08xReact", Nnp08xReact, &b_Nnp08xReact);
   fChain->SetBranchAddress("Nnp08yReact", Nnp08yReact, &b_Nnp08yReact);
   fChain->SetBranchAddress("Nnp08zReact", Nnp08zReact, &b_Nnp08zReact);
   fChain->SetBranchAddress("Nnp08p", Nnp08p, &b_Nnp08p);
   Notify();
}

Bool_t tLand::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void tLand::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t tLand::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef tLand_cxx
