#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "TLeaf.h"
#include "TH1D.h"

using namespace std;

void GetHist()
{
    TFile* f = new TFile("VetoSim_run=5.root","read");
    TTree* t = (TTree*) f->Get("cbmsim");
    TLeaf* L = t->FindLeaf("LandPoint.fLightYield");    
    
    Long64_t nEvents = t->GetEntries();
    Long64_t nBytes = 0;
    
    TH1D* h = new TH1D("hist","hist",100,0.0,1000.0);
    Double_t Edep = 0.0;
    Int_t Size;
    
    for (Long64_t Event = 0; Event<nEvents; ++Event)
    {
        nBytes = t->GetEntry(Event);
        
        Size = L->GetNdata();
        Edep = 0.0;
        
        for (Int_t k = 0; k<Size; ++k)
        {
            Edep = Edep + L->GetValue(k);
        }
        
        h->Fill(Edep*1e3);
        
        if ((Event+1)%100==0) {cout << Event+1 << "\n";}
        
    }   
    
    h->Draw();
    
}