#!/bin/bash

# This script searches if a previous version of the VETO existed yes/no.
# If it does exists then it is deleted before the new version is created:

cd ${VMCWORKDIR}

if [ -d "./Safe" ]
then
  # In this case the VETO exists:
  echo '-------------- Restoring Master Files...'
  rm -rf ./CMakeLists.txt
  rm -rf ./r3bdata/CMakeLists.txt
  rm -rf ./r3bdb/CMakeLists.txt
  rm -rf ./r3bdata/R3BDatLinkDef.h
  rm -rf ./r3bdata/R3BDetectorList.h
  rm -rf ./r3bdata/R3BMCTrack.cxx
  
  rm -rf ./land/R3BNeutronTracker2D_Original.cxx
  rm -rf ./land/R3BNeutronTracker2D_Original.h
  rm -rf ./land/R3BNeutronTracker2D_MultiCuts.cxx
  rm -rf ./land/R3BNeutronTracker2D_MultiCuts.h
  rm -rf ./land/R3BNeutronTracker2D_Transf.cxx
  rm -rf ./land/R3BNeutronTracker2D_Transf.h
  rm -rf ./neuland/Reconstruction
  
  cp ./Safe/media_r3b.geo ./geometry/media_r3b.geo
  
  cp ./Safe/CMakeLists_Master.txt ./CMakeLists.txt
  cp ./Safe/CMakeLists_r3bdata.txt ./r3bdata/CMakeLists.txt
  cp ./Safe/CMakeLists_r3bdb.txt ./r3bdb/CMakeLists.txt
  cp ./Safe/CMakeLists_neuland.txt ./neuland/CMakeLists.txt
  cp ./Safe/R3BDataLinkDef.h ./r3bdata/R3BDataLinkDef.h
  cp ./Safe/R3BDetectorList.h ./r3bdata/R3BDetectorList.h
  cp ./Safe/R3BMCTrack.cxx ./r3bdata/R3BMCTrack.cxx
  cp ./Safe/NeulandLinkDef.h ./neuland/NeulandLinkDef.h
  
  cp ./Safe/R3BNeutronCalibr2D.cxx ./land/R3BNeutronCalibr2D.cxx
  cp ./Safe/R3BNeutronCalibr2D.h ./land/R3BNeutronCalibr2D.h
  cp ./Safe/R3BNeutronTracker2D.cxx ./land/R3BNeutronTracker2D.cxx
  cp ./Safe/R3BNeutronTracker2D.h ./land/R3BNeutronTracker2D.h
  cp ./Safe/CMakeLists_land.txt ./land/CMakeLists.txt
  cp ./Safe/R3BNeulandDigitizer.cxx ./neuland/R3BNeulandDigitizer.cxx
  cp ./Safe/DigitizingEngine.cxx ./neuland/digitizing/DigitizingEngine.cxx
  cp ./Safe/R3BNeulandDigitizer.h ./neuland/R3BNeulandDigitizer.h
  cp ./Safe/DigitizingEngine.h ./neuland/digitizing/DigitizingEngine.h
  cp ./Safe/R3BNeuLandClusterFinder.h ./land/R3BNeuLandClusterFinder.h
  cp ./Safe/R3BNeuLandClusterFinder.cxx ./land/R3BNeuLandClusterFinder.cxx
  cp ./Safe/R3BNeuLandCluster.h ./land/R3BNeuLandCluster.h
  cp ./Safe/R3BNeuLandCluster.cxx ./land/R3BNeuLandCluster.cxx
  
  
  echo '-------------- Restoring Steering macros...'
  rm -rf ./macros/r3b/r3bsim.C
  rm -rf ./macros/r3b/r3ball.C
  cp ./Safe/r3bsim.C ./macros/r3b/r3bsim.C
  cp ./Safe/r3ball.C ./macros/r3b/r3ball.C
  
  echo '-------------- Deleting geometry...'
  rm -rf ./macros/r3b/geo/create_Nveto_seg_geo.C
  rm -rf ./geometry/Nveto_seg_v12a_10m.geo.root
  
  echo '-------------- Deleting VETO class...'
  rm -rf ./veto_seg
  
  echo '-------------- Deleting NVETO class...'
  rm -rf ./Nveto_seg
  
  echo '-------------- Deleting VETO Data class...'
  rm -rf ./r3bdata/veto_segData
  
  echo '-------------- Deleting NVETO Data class...'
  rm -rf ./r3bdata/Nveto_segData
  
  echo '-------------- Deleting VETO DB class...'
  rm -rf ./r3bdb/veto_segdb
  
  echo '-------------- Deleting Safe directory...'
  rm -rf ./Safe
  
  echo '-------------- Deleting VETO macros...'
  rm -rf ./macros/r3b/Nveto_seg
  
  echo '-------------- Done!'
  
  echo ''
  echo '### Now you should re-compile R3Broot to let the changes come into effect!'
  echo ''
  
else
  echo '### No VETO exists.'
  echo ''
fi