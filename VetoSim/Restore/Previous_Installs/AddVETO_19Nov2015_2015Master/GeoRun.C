#include "TROOT.h"
#include "TString.h"

void GeoRun()
{
  // This macro runs the geometry macro for the segmented VETO.
  
  // Define the GeoTag:
  TString geoTag = "v12a_10m";
  
  // Load the macro:
  TString macro_geo = getenv("VMCWORKDIR");
  macro_geo += "/macros/r3b/geo/create_Nveto_seg_geo.C";
  gROOT->LoadMacro(macro_geo.Data());
  
  // Now run the macro:
  create_Nveto_seg_geo(geoTag);
  
  // Done!
}