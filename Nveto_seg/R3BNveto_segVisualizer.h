#ifndef R3BNVETO_SEGVISUALIZER_H
#define R3BNVETO_SEGVISUALIZER_H 1

#include "TFile.h"
#include "TTree.h"
#include "TH3D.h"
#include "TCanvas.h"

class R3BNveto_segVisualizer {
public:
    R3BNveto_segVisualizer(const TString &input_file, const TString &what);
    virtual ~R3BNveto_segVisualizer();

    inline void ShowEvent(const UInt_t i)
    {
        fIndex = i;
        Visualize();
    }

    inline void ShowNext()
    {
        fIndex++;
        Visualize();
    }

protected:
    void Visualize();

private:
    TFile *fFile;
    TTree *fTree;
    TH3D *fh3;
    TCanvas *fCanvas;
    UInt_t fIndex;

    ClassDef(R3BNveto_segVisualizer, 0);
};

#endif //R3BNVETO_SEGVISUALIZER_H
