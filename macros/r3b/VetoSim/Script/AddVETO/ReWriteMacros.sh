#!/bin/bash

# This script adds the segmented VETO to the steering macros of R3Broot:
cd ${VMCWORKDIR}

###################################################################################
# Add the VETO to r3bsim.C:

echo '-------------- Rewriting r3bsim.C...'
cp ./macros/r3b/r3bsim.C ./Safe/r3bsim.C
sed -i '98 a //  detGeo.Add(new TObjString("SCINTNVETO_SEG"),  new TObjString("Nveto_seg_v12a_10m.geo.root"));' ./macros/r3b/r3bsim.C

###################################################################################
# Add the VETO to r3ball.C:
echo '-------------- Rewriting r3ball.C...'
cp ./macros/r3b/r3ball.C ./Safe/r3ball.C
sed -i '225 a   // Nveto_seg Scintillator Detector' ./macros/r3b/r3ball.C
sed -i '226 a   if(fDetList.FindObject("SCINTNVETO_SEG")) {' ./macros/r3b/r3ball.C
sed -i '227 a     R3BDetector* veto_seg = new R3Bveto_seg("veto_seg", kTRUE);' ./macros/r3b/r3ball.C
sed -i '228 a     veto_seg->SetVerboseLevel(1);' ./macros/r3b/r3ball.C
sed -i '229 a     veto_seg->SetGeometryFileName(((TObjString*)fDetList.GetValue("SCINTNVETO_SEG"))->GetString().Data());' ./macros/r3b/r3ball.C
sed -i '230 a     run->AddModule(veto_seg);' ./macros/r3b/r3ball.C
sed -i '231 a   }' ./macros/r3b/r3ball.C
sed -i '232 a   //' ./macros/r3b/r3ball.C

###################################################################################
# Done!
echo '-------------- Done!'