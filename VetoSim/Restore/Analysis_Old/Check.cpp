#include <iostream>

#include "TFile.h"
#include "TTree.h"
#include "TLeaf.h"
#include "TH1D.h"
#include "TCanvas.h"

using namespace std;

void Check()
{
    TFile* f = new TFile("Test_run=1.root","read");
    TFile* fa = new TFile("../Analysis/Test_run=1.root","read");
    
    TTree* t = (TTree*) f->Get("cbmsim");
    TTree* ta = (TTree*) fa->Get("AnalysisTree");
    
    TH1D* h = new TH1D("h","h",100,0.0,170.0);
    TH1D* ha = new TH1D("ha","ha",100,0.0,170.0);
    
    Int_t n = t->GetEntries();
    
    TLeaf* p = (TLeaf*) t->FindLeaf("LandFirstHits.fT0");
    TLeaf* pa = (TLeaf*) ta->FindLeaf("Prim_MCinteraction_T");
    
    Double_t d = 0.0;
    Double_t da = 0.0;
    Int_t nb = 0;
    Int_t nba = 0;
    Int_t Size = 0;
    
    for (Int_t k = 0; k<n; ++k)
    {
        nb = t->GetEntry(k);
        nba = ta->GetEntry(k);
        
        Size = p->GetNdata();
        for (Int_t i = 0; i<Size; ++i)
        {
            d = p->GetValue(i);
            h->Fill(d*1e9);
        }
        
        Size = pa->GetNdata();
        for (Int_t i = 0; i<Size; ++i)
        {
            da = pa->GetValue(i);
            ha->Fill(da);
        }
    }
    
    h->SetMaximum(40.0);    
    ha->SetMaximum(40.0);
    
    TCanvas* c = new TCanvas("name");
    c->Divide(2,1);
    c->cd(1);
    h->Draw();
    c->cd(2);
    ha->Draw();
    
}