// Include the required root-headers:
#include "TMath.h"
#include "TH1D.h"
#include "TH1I.h"
#include "TGraphErrors.h"
#include "TStyle.h"
#include "TPad.h"
#include "TPaveText.h"
#include "TCanvas.h"
#include "TString.h"
#include "TSystem.h"
#include "TFile.h"

void DrawGraphDoumaArray(Int_t const Npads, Int_t const Ngraphs, Int_t const Nhor, Int_t const Nvert, TGraphErrors*** const Raw, TString const Title[], TString const xlabel, TString const ylabel, Int_t const GraphColor[], TString const Legenda[], Bool_t const print, Bool_t const close)
{
  // This program paints a collections of TGraphErrors. A 2D array should be supplied.
  // The first index should have length Npads and represents the number of distinct
  // figures you want. The second index should have length Ngraphs and represents
  // the number of graphs within one figure that you want. Nhor and Nvert are the
  // number of horizontal and vertical figures on the canvas. Title is an array with
  // length Npads and represents the title of each figure. xlabal and ylabel represent
  // the text at the axis that you want. Finally, the boolian print states if we should
  // print this pictures yes/no. close states whether we should colse our graphs
  // afterward yes/no. Since the output is the graph, the function returns a void.
  // GraphColor is an array of size Ngraphs that contains the colors that the graphs
  // should have. Legenda is an array of TStrings that will be used to print a 
  // legenda: one for each graph within a figure. The colors will match with the graphs.
  
  // Written by C. A. Douma.
  
  // =====================================================================================
  // Before we actually create the picture, we first need to obtain our scale factor:
  Double_t ScaleFactor = 0.1;
  FileStat_t stom;
  Int_t inputs_test = gSystem->GetPathInfo("../../Data/Inputs/Inputs.root",stom);
   
  if (inputs_test==0)
  {
    // Connect to the Inputs.root file:
    TFile* inputs = new TFile("../../Data/Inputs/Inputs.root","read");
  
    // And then Get the histograms from this file:
    TH1D* Doubles = (TH1D*) inputs->Get("Doubles");
    TH1I* Integers = (TH1I*) inputs->Get("Integers");
    
    // obtain the scale factor:
    ScaleFactor = Doubles->GetBinContent(401);
  }
    
  // ======================================================================================
  // Now each figure should have its own maximum and minimum. Hence per figure we should
  // search for this.
  
  // So first declare some variables that we want:
  TString st = "";
  TString kstr = "";
  Double_t* Xmin = new Double_t [Npads];
  Double_t* Xmax = new Double_t [Npads];
  Double_t XcMax= 0.0;
  Double_t XcMin = 0.0;
  Double_t* Ymin = new Double_t [Npads];
  Double_t* Ymax = new Double_t [Npads];
  Double_t YcMax= 0.0;
  Double_t YcMin = 0.0;
  Double_t length = 0.0;
  
  // Then perform searches for the global min and max per figure:
  for (Int_t n = 0; n<Npads; ++n)
  {
    // Initialize the search:
    Xmax[n] = 0.0;
    Ymax[n] = 0.0;
    
    // search for the maximum:
    for (Int_t k = 0; k<Ngraphs; ++k)
    {
      XcMax = Raw[n][k]->GetXaxis()->GetXmax();
      YcMax = Raw[n][k]->GetYaxis()->GetXmax();
      if (XcMax>Xmax[n]) {Xmax[n] = XcMax;}
      if (YcMax>Ymax[n]) {Ymax[n] = YcMax;}
    }
    
    // initialise the search for the minimum:
    Xmin[n] = Xmax[n];
    Ymin[n] = Ymax[n];
    
    // search for the minimum:
    for (Int_t k = 0; k<Ngraphs; ++k)
    {
      XcMin = Raw[n][k]->GetXaxis()->GetXmin();
      YcMin = Raw[n][k]->GetYaxis()->GetXmin();
      if (XcMin<Xmin[n]) {Xmin[n] = XcMin;}
      if (YcMin<Ymin[n]) {Ymin[n] = YcMin;}
    }
  }
  
  // Then now we can set our graphs:
  for (Int_t n = 0; n<Npads; ++n)
  {
    for (Int_t k = 0; k<Ngraphs; ++k)
    {
  
      // We Add labels to the TGraph:
      Raw[n][k]->SetTitle(Title[n]);
      (Raw[n][k]->GetXaxis())->SetTitle(xlabel);
      (Raw[n][k]->GetYaxis())->SetTitle(ylabel);
  
      // Control position of axis labels along the axis:
      Raw[n][k]->GetXaxis()->CenterTitle(kTRUE);
      Raw[n][k]->GetYaxis()->CenterTitle(kTRUE);
      // kFALSE will put the labels at the end of the axis.
  
      // Set ranges of the TGraph:
      length = Xmax[n] - Xmin[n];
      (Raw[n][k]->GetXaxis())->SetLimits(Xmin[n]-0.07*length,Xmax[n]+0.01*length);
  
      length = Ymax[n] - Ymin[n];
      Raw[n][k]->SetMinimum(0.0);
      Raw[n][k]->SetMaximum(Ymax[n]+0.1*length);
      (Raw[n][k]->GetYaxis())->SetLimits(0.0,Ymax[n]+0.1*length);
    }
  }
  
  // We will set our color and plotting options down at the bottom.
  
  // =======================================================================================
  // Then we define the current style for graph plotting:
  
  // --------------------------------------------------------------------
  // General aspects:
  gStyle->SetFrameLineWidth(1.0); // Thickness of frame 
  gStyle->SetFrameLineColor(0); // Color of frame
  gStyle->SetLineWidth(2.0); // Line thickness of axis!
  gStyle->SetOptStat(0); // No stat box
  gStyle->SetOptLogx(0); // log-scale on x-axis, either 0 or 1.
  gStyle->SetOptLogy(0); // log-scale on y-axis, either 0 or 1.
  gStyle->SetScreenFactor(ScaleFactor); // size of the canvas on the screen.
  gStyle->SetPadBottomMargin(0.14); // margin of the graph to the boundary of the pad on the bottom.
  gStyle->SetPadTopMargin(0.11); // margin of the graph to the boundary of the pad on the Top.
  gStyle->SetPadLeftMargin(0.17); // margin of the graph to the boundary of the pad on the Left.
  gStyle->SetPadRightMargin(0.01); // margin of the graph to the boundary of the pad on the Right.
  // 0=white 1=black 2=red 3=green 4=blue 5=yellow 6=magenta 7=cyaan 8=darkgreen 9=purple, there are lots of other options...
  // NOTE: If you want to change the size of a figure within a sub-pad, change these SetPadMargins.
  
  // --------------------------------------------------------------------
  // Controlling the Title.
  
  gStyle->SetTitleFont(1); // Title font type
  gStyle->SetTitleTextColor(6); // Title font color
  gStyle->SetTitleW(1.0); // Title font size horizontal
  gStyle->SetTitleH(0.1); // Title font size vertical; real font size is an interplay between them!  gPad->SetPad(0.0,-0.05,1.0,1.0);
  gStyle->SetTitleY(0.99); // Title vertical position
  gStyle->SetTitleX(0.5); // Title horizontal position
  
  // --------------------------------------------------------------------
  // Controlling the X-axis is done in a similar manner. Now "x" tells that it is about the x-axis!
  // NB: root calls the numerical scale a label. The text table is called a title in root!
  
  gStyle->SetAxisColor(12,"x"); // x-axis own color
  gStyle->SetTickLength(0.03,"x"); // x-axis length of the numerical scale markers
  gStyle->SetNdivisions(1010,"x"); // amount of scale markers. n = n1+100*n2+10000*n3 primary, secondary & third divisions. default = 510.
  
  gStyle->SetLabelFont(2,"x"); // x-axis numerical scale font type
  gStyle->SetLabelColor(12,"x"); // x-axis numerical scale font color
  gStyle->SetLabelSize(0.04,"x"); // x-axis numerical scale font size
  gStyle->SetLabelOffset(0.01,"x"); // x-axis numerical scale vertical positionthe same as the histogram!
  
  gStyle->SetTitleFont(1,"x"); // x-axis label font type
  gStyle->SetTitleColor(1,"x"); // x-axis label font colorRaw->LabelsOption("v");
  gStyle->SetTitleSize(0.05,"x"); // x-axis label font size
  gStyle->SetTitleOffset(1.0,"x"); // x-axis label vertical position
  
  // --------------------------------------------------------------------
  // Controlling the Y-axis is done in a similar manner. Now "y" tells that it is about the y-axis!
  
  gStyle->SetAxisColor(12,"y"); // y-axis own color
  gStyle->SetTickLength(0.03,"y"); // y-axis length of the numerical scale markers
  gStyle->SetNdivisions(1010,"y"); // amount of scale markers. n = n1+100*n2+10000*n3 primary, secondary & third divisions. default = 510.
  
  gStyle->SetLabelFont(2,"y"); // y-axis numerical scale2.0,0.0 font type
  gStyle->SetLabelColor(12,"y"); // y-axis numerical scale font color
  gStyle->SetLabelSize(0.04,"y"); // y-axis numerical scale font size
  gStyle->SetLabelOffset(0.005,"y"); // y-axis numerical scale vertical position
  
  gStyle->SetTitleFont(1,"y"); // y-axis label font type
  gStyle->SetTitleColor(1,"y"); // y-axis label font color
  gStyle->SetTitleSize(0.05,"y"); // y-axis label font size
  gStyle->SetTitleOffset(1.75,"y"); // y-axis label vertical position
  
  // =======================================================================================
  // We do not add some text to the picture here. We do that later.
  // ======================================================================================
  // Finally, we do the real painting:
  
  // In this case we have to be Careful. At first we need to determine how many
  // canvasses we need:
  Int_t NCanvas = 0;
  Int_t GraphCount = Npads;
  while (GraphCount>0)
  {
    GraphCount = GraphCount - (Nvert*Nhor);
    NCanvas = NCanvas + 1;
  }
  
  // Now the number of canvasses we need is given. Make it constant:
  Int_t const CanvasArray_ii = NCanvas;
  Int_t const CanvasArray_i = CanvasArray_ii;
  Int_t const CanvasArray = CanvasArray_i;
  
  // Declare Canvas Array:
  TCanvas* Pages [CanvasArray];
  Int_t index = 0;
  Int_t padnr = 1;
  
  // Now start painting. Notice that the divided Canvas is 
  // first filled horizontally and then vertically!
  for (Int_t k=0; k<CanvasArray; ++k)
  {
    kstr = st.Itoa(k,10);
    Pages[k] = new TCanvas(Title[0] + "_Canvas_" + kstr);
    Pages[k]->Divide(Nhor,Nvert);
    padnr = 1;
    
    for (Int_t khor = 0; khor<Nhor; ++khor)
    {
      for (Int_t kvert = 0; kvert<Nvert; ++kvert)
      {
	padnr = 1 + khor + Nhor*kvert;
        index = (k*(Nhor*Nvert)) + padnr - 1;
	
	Pages[k]->cd(padnr);
	
	// Now draw all graphs at once:
	for (Int_t kgraph = 0; kgraph<Ngraphs; ++ kgraph)
	{
          if (index<Npads) {Raw[index][kgraph]->UseCurrentStyle();}
          if (index<Npads) {Raw[index][kgraph]->SetLineWidth(2.0);}
          if (index<Npads) {Raw[index][kgraph]->SetLineColor(GraphColor[kgraph]);}
          if (kgraph==0) {if (index<Npads) {Raw[index][kgraph]->Draw();}}
          if (kgraph>0) {if (index<Npads) {Raw[index][kgraph]->Draw("same");}}
	}
      }
    }
    Pages[k]->SetPad(0.0,0.0,1.0,1.0);
    // This function selects the margins of the entire canvas
    // to the boundaries. If you want to controle the margins
    // between the individual histograms, use the
    // gStyle->SetPadMargin functions higher up in this macro.
    
  }
  
  // That should do it!
  
  // =========================================================================================
  // Now we also want to build a legenda. This is done with the Legenda strings. They will be
  // printed in the first sub-canvas that is not used. But only if there is space.
  
  // check for this space:
  if (CanvasArray*Nhor*Nvert>Npads)
  {
    // Then there is space left. The canvas we should have is always the last canvas:
    // Pages[CanvasArray-1]. The sub-canvas we should have will be Ngraphs+1.
    
    // So we will first define the legenda:
    TPaveText** Text = new TPaveText*[Ngraphs];
    
    for (Int_t k = 0; k<Ngraphs; ++k)
    {
      Text[k] = new TPaveText(0.49,0.90-((Int_t)k)*0.06,0.51,0.91-((Int_t)k)*0.06,"NB");
      Text[k]->SetFillColor(0); 
      Text[k]->AddText(Legenda[k]);
      Text[k]->SetTextColor(GraphColor[k]); 
      Text[k]->SetTextSize(0.06); 
      Text[k]->SetTextFont(1);
      Text[k]->SetTextAngle(0.0); 
    }
    
    // And then we will paint it:
    Pages[CanvasArray-1]->cd(Ngraphs+1);
    
    for (Int_t k = 0; k<Ngraphs; ++k)
    {
      Text[k]->Draw("same");
    }
    
    // And that is our legenda.
  }
  
  // Then only after the Legenda is done, print the canvasses to a .png-file:
  for (Int_t k=0; k<CanvasArray; ++k)
  {
    if (print) {Pages[k]->SaveAs("../../UI/Pictures/"+Title[0] + " " + kstr + ".png");}
    if (close) {Pages[k]->Close();}
  }
  
  // Done!
  
}
