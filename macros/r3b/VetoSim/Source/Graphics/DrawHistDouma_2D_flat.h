// Include the required root-headers:
#include "TMath.h"
#include "TH1D.h"
#include "TH1I.h"
#include "TH2D.h"
#include "TStyle.h"
#include "TPad.h"
#include "TPaveText.h"
#include "TCanvas.h"
#include "TString.h"
#include "TFile.h"
#include "TSystem.h"

TCanvas* DrawHistDouma_2D_flat(TH2D* const Raw, TString const Title, TString const xlabel, TString const ylabel, TString const zlabel)
{
  // This program paints the histogram TH2D* Raw. 
  // This program controls every aspect of the plot
  // except the histogram data, which is taken from the input.
  // Hence all style aspects are defined here.
  // Title is the histogram and Canvas title you want pianted and
  
  // Output is max, the highest point in the histogram.
  
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
  // We begin with specifying some settings of the histogram:
  
  // We Add labels to the histogram:
  Raw->SetTitle(Title);
  Raw->SetXTitle(xlabel);
  Raw->SetYTitle(ylabel);
  Raw->SetZTitle(zlabel);
  
  // Control position of axis labels along the axis:
  Raw->GetXaxis()->CenterTitle(kTRUE);
  Raw->GetYaxis()->CenterTitle(kTRUE);
  Raw->GetZaxis()->CenterTitle(kTRUE);
  // kFALSE will put the labels at the end of the axis.
  
  // Set ranges of the histogram:
  Double_t Xmin = Raw->GetXaxis()->GetXmin(); // X-axis
  Double_t Xmax = Raw->GetXaxis()->GetXmax();
  Raw->SetAxisRange(Xmin,Xmax,"x");
  
  Double_t Ymin = Raw->GetYaxis()->GetXmin(); // Y-axis
  Double_t Ymax = Raw->GetYaxis()->GetXmax();
  Raw->SetAxisRange(Ymin,Ymax,"y");
  
  Double_t max = Raw->GetMaximum(); // Z-axis
  if (max == 0.0) {max = 0.06;}
  Raw->SetMaximum(1.0*max);
  Raw->SetMinimum(-0.1*max); // must be positive for log-scale!
  Raw->SetMinimum(0.3); // Log-scale option.
  
  // Add the histogram painting options:
  Raw->SetOption("COLZfbbb"); // fb = front box & bb = back box. Shuts these down.
  // We have "lego", etc. see THistPainter ROOT.
  
  // =======================================================================================
  // Then we define the current style for histogram plotting:
  
  // --------------------------------------------------------------------
  // General aspects:
  gStyle->SetFrameLineWidth(1.0); // Thickness of frame 
  gStyle->SetFrameLineColor(0); // Color of frame
  gStyle->SetHistLineColor(1); // line color of histogram
  gStyle->SetHistLineWidth(1.0); // line thickness of histogram
  gStyle->SetLineWidth(2.0); // Line thickness of axis!
  gStyle->SetOptStat(0); // No stat box
  gStyle->SetOptLogx(0); // log-scale on x-axis, either 0 or 1.
  gStyle->SetOptLogy(0); // log-scale on y-axis, either 0 or 1.
  gStyle->SetOptLogz(1); // log-scale on z-axis, either 0 or 1.
  gStyle->SetScreenFactor(ScaleFactor); // size of the canvas on the screen.
  gStyle->SetPadBottomMargin(0.14); // margin of the histogram to the boundary of the pad on the bottom.
  gStyle->SetPadTopMargin(0.11); // margin of the histogram to the boundary of the pad on the Top.
  gStyle->SetPadLeftMargin(0.13); // margin of the histogram to the boundary of the pad on the Left.
  gStyle->SetPadRightMargin(0.14); // margin of the histogram to the boundary of the pad on the Right.
  // 0=white 1=black 2=red 3=green 4=blue 5=yellow 6=magenta 7=cyaan 8=darkgreen 9=purple, there are lots of other options...
  
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
  gStyle->SetTitleOffset(1.05,"y"); // y-axis label vertical position
  
  // --------------------------------------------------------------------
  // Controlling the Z-axis is done in a similar manner. Now "z" tells that it is about the z-axis!
  
  gStyle->SetAxisColor(12,"z"); // z-axis own color
  gStyle->SetTickLength(0.03,"z"); // z-axis length of the numerical scale markers
  gStyle->SetNdivisions(410,"z"); // amount of scale markers. n = n1+100*n2+10000*n3 primary, secondary & third divisions. default = 510.
  
  gStyle->SetLabelFont(2,"z"); // z-axis numerical scale2.0,0.0 font type
  gStyle->SetLabelColor(12,"z"); // z-axis numerical scale font color
  gStyle->SetLabelSize(0.04,"z"); // z-axis numerical scale font size
  gStyle->SetLabelOffset(0.0,"z"); // z-axis numerical scale vertical position
  
  gStyle->SetTitleFont(1,"z"); // z-axis label font type
  gStyle->SetTitleColor(1,"z"); // z-axis label font color
  gStyle->SetTitleSize(0.05,"z"); // z-axis label font size
  gStyle->SetTitleOffset(0.85,"z"); // z-axis label horizontal position
  
  // =======================================================================================
  // We add some text inside the picture: 
  
  // First text box.
  TPaveText* text = new TPaveText(Xmin+(Xmax-Xmin)*0.93,Ymin+(Ymax-Ymin)*1.00,Xmin+(Xmax-Xmin)*0.94,Ymin+(Ymax-Ymin)*1.01,"NB"); // NB says that no borders are drawn. 
  // Creates an empty text box between (x1,y1) and (x2,y2). Input arguments are in the order x1,y1,x2,y2.
  // Coordinates are on a scale -1 to 1 on the pad!
  // x1 and y1 are left lower corner and x2 and y2 are right upper corner of the box.
  // Now we have the empty text field. Program the field now:
  text->SetFillColor(0); // Color of the box.
  text->AddText("Written by"); // Add text to the box. Text is printed at the center of the box.
  text->SetTextColor(7); // Define text color
  text->SetTextSize(0.03); // Define text size
  text->SetTextFont(1); // Define text font
  text->SetTextAngle(0.0); // Defines the angle at which the text is displayed.
  
  // Second text box:
  TPaveText* text2 = new TPaveText(Xmin+(Xmax-Xmin)*0.93,Ymin+(Ymax-Ymin)*0.96,Xmin+(Xmax-Xmin)*0.94,Ymin+(Ymax-Ymin)*0.97,"NB"); // NB says that no borders are drawn.  
  text2->SetFillColor(0); 
  text2->AddText("C. A. Douma"); 
  text2->SetTextColor(7); 
  text2->SetTextSize(0.03); 
  text2->SetTextFont(1);
  text2->SetTextAngle(0.0); 
  
  // ======================================================================================
  // Finally, we do the real painting:
  
  TCanvas* c = new TCanvas(Title); // To create a fresh canvas for the painting.
  
  Raw->UseCurrentStyle(); // To use all the gStyle settings!
  Raw->Draw(); // To actually draw.
  //text->Draw("same"); // Draw the text-boxes:
  //text2->Draw("same");
  gPad->SetPad(0.0,0.0,1.0,1.0); // To set the pad size correctly.
  // Size of the pad inside the canvas. order is x1,y1,x2,y2. (x1,y1) is left bottom corner.
  // (x2,y2) is right upper corner. Coordinates are on a scale from 0.0 to 1.0. You can blow up the
  // picture beyond these limits but you risk that it falls outside the figure then.
  
  // =========================================================================================
  // Finally, return the TCanvas:
  return c;
}
