#!/bin/bash

# This file rewrites the Masterfiles of R3Broot to add the
# segmented VETO.

# First change to the R3Broot directory:
cd ${VMCWORKDIR}

############################################################################
# Rewrite master CMakeLists.txt:

echo '-------------- Rewrite Master CMakeLists.txt...'
cp ./CMakeLists.txt ./Safe/CMakeLists_Master.txt
sed -i '290 a add_subdirectory (veto_seg)' ./CMakeLists.txt
sed -i '291 a add_subdirectory (Nveto_seg)' ./CMakeLists.txt

############################################################################
# Rewrite LinkDef:

# Copy it to safety:
echo '-------------- Rewrite R3BDataLinkDef...'
cp ./r3bdata/R3BDataLinkDef.h ./Safe/R3BDataLinkDef.h

# Load the content into an array:
NLines=`wc -l < ./r3bdata/R3BDataLinkDef.h`
declare -a Array[${NLines}]
COUNTER=0
while read Line
do
  Array[${COUNTER}]=${Line}
  COUNTER=`expr ${COUNTER} + 1`
done < ./r3bdata/R3BDataLinkDef.h

# Now loop over the array and test if a line contains a specific string:
WRITECOUNTER=60

for ((n=0; n<${NLines}; ++n))
do
  # Test for the same name strings as in the ReName-file.
  
  # First the NeuLAND strings:
  if [[ ${Array[${n}]} == *"NEULAND"* ]]
  then 
    # If this string is there, write the whole line to a file:
    echo "${Array[${n}]}" > ./Safe/FileLine.txt
    
    # Then edit this file with sed:
    sed -i -e "s:NEULAND:NVETO_SEG:" ./Safe/FileLine.txt
    
    # Then read this line again:
    read TheLine < ./Safe/FileLine.txt
    
    # Then Add this line to the LinkDef:
    sed -i "${WRITECOUNTER} a ${TheLine}" ./r3bdata/R3BDataLinkDef.h
    
    # Update the variables:
    WRITECOUNTER=`expr ${WRITECOUNTER} + 1`
    
    # Delete the FileLine:
    rm -rf ./Safe/FileLine.txt
    Array[${n}]=''
  fi
  
  if [[ ${Array[${n}]} == *"NeuLAND"* ]]
  then 
    echo "${Array[${n}]}" > ./Safe/FileLine.txt
    sed -i -e "s:NeuLAND:Nveto_seg:" ./Safe/FileLine.txt
    read TheLine < ./Safe/FileLine.txt
    sed -i "${WRITECOUNTER} a ${TheLine}" ./r3bdata/R3BDataLinkDef.h
    WRITECOUNTER=`expr ${WRITECOUNTER} + 1`
    rm -rf ./Safe/FileLine.txt
    Array[${n}]=''
  fi
  
  if [[ ${Array[${n}]} == *"NeuLand"* ]]
  then 
    echo "${Array[${n}]}" > ./Safe/FileLine.txt
    sed -i -e "s:NeuLand:Nveto_seg:" ./Safe/FileLine.txt
    read TheLine < ./Safe/FileLine.txt
    sed -i "${WRITECOUNTER} a ${TheLine}" ./r3bdata/R3BDataLinkDef.h
    WRITECOUNTER=`expr ${WRITECOUNTER} + 1`
    rm -rf ./Safe/FileLine.txt
    Array[${n}]=''
  fi
  
  if [[ ${Array[${n}]} == *"Neuland"* ]]
  then 
    echo "${Array[${n}]}" > ./Safe/FileLine.txt
    sed -i -e "s:Neuland:Nveto_seg:" ./Safe/FileLine.txt
    read TheLine < ./Safe/FileLine.txt
    sed -i "${WRITECOUNTER} a ${TheLine}" ./r3bdata/R3BDataLinkDef.h
    WRITECOUNTER=`expr ${WRITECOUNTER} + 1`
    rm -rf ./Safe/FileLine.txt
    Array[${n}]=''
  fi
  
  if [[ ${Array[${n}]} == *"neuland"* ]]
  then 
    echo "${Array[${n}]}" > ./Safe/FileLine.txt
    sed -i -e "s:neuland:Nveto_seg:" ./Safe/FileLine.txt
    read TheLine < ./Safe/FileLine.txt
    sed -i "${WRITECOUNTER} a ${TheLine}" ./r3bdata/R3BDataLinkDef.h
    WRITECOUNTER=`expr ${WRITECOUNTER} + 1`
    rm -rf ./Safe/FileLine.txt
    Array[${n}]=''
  fi
  
  # Then the Land strings:
  if [[ ${Array[${n}]} == *"LAND"* ]]
  then 
    echo "${Array[${n}]}" > ./Safe/FileLine.txt
    sed -i -e "s:LAND:VETO_SEG:" ./Safe/FileLine.txt
    read TheLine < ./Safe/FileLine.txt
    sed -i "${WRITECOUNTER} a ${TheLine}" ./r3bdata/R3BDataLinkDef.h
    WRITECOUNTER=`expr ${WRITECOUNTER} + 1`
    rm -rf ./Safe/FileLine.txt
    Array[${n}]=''
  fi
  
  if [[ ${Array[${n}]} == *"Land"* ]]
  then 
    echo "${Array[${n}]}" > ./Safe/FileLine.txt
    sed -i -e "s:Land:veto_seg:" ./Safe/FileLine.txt
    read TheLine < ./Safe/FileLine.txt
    sed -i "${WRITECOUNTER} a ${TheLine}" ./r3bdata/R3BDataLinkDef.h
    WRITECOUNTER=`expr ${WRITECOUNTER} + 1`
    rm -rf ./Safe/FileLine.txt
    Array[${n}]=''
  fi
  
  if [[ ${Array[${n}]} == *"land"* ]]
  then 
    echo "${Array[${n}]}" > ./Safe/FileLine.txt
    sed -i -e "s:land:veto_seg:" ./Safe/FileLine.txt
    read TheLine < ./Safe/FileLine.txt
    sed -i "${WRITECOUNTER} a ${TheLine}" ./r3bdata/R3BDataLinkDef.h
    WRITECOUNTER=`expr ${WRITECOUNTER} + 1`
    rm -rf ./Safe/FileLine.txt
    Array[${n}]=''
  fi
  
  # Then the other strings:
  if [[ ${Array[${n}]} == *"NeutHit"* ]]
  then 
    echo "${Array[${n}]}" > ./Safe/FileLine.txt
    sed -i -e "s:NeutHit:NeutHt_seg:" ./Safe/FileLine.txt
    read TheLine < ./Safe/FileLine.txt
    sed -i "${WRITECOUNTER} a ${TheLine}" ./r3bdata/R3BDataLinkDef.h
    WRITECOUNTER=`expr ${WRITECOUNTER} + 1`
    rm -rf ./Safe/FileLine.txt
    Array[${n}]=''
  fi
  
  if [[ ${Array[${n}]} == *"NeutronTracker2D"* ]]
  then 
    echo "${Array[${n}]}" > ./Safe/FileLine.txt
    sed -i -e "s:NeutronTracker2D:NeutronTrackr2D_seg:" ./Safe/FileLine.txt
    read TheLine < ./Safe/FileLine.txt
    sed -i "${WRITECOUNTER} a ${TheLine}" ./r3bdata/R3BDataLinkDef.h
    WRITECOUNTER=`expr ${WRITECOUNTER} + 1`
    rm -rf ./Safe/FileLine.txt
    Array[${n}]=''
  fi
  
  if [[ ${Array[${n}]} == *"NeutronTracker"* ]]
  then 
    echo "${Array[${n}]}" > ./Safe/FileLine.txt
    sed -i -e "s:NeutronTracker:NeutronTrackr_seg:" ./Safe/FileLine.txt
    read TheLine < ./Safe/FileLine.txt
    sed -i "${WRITECOUNTER} a ${TheLine}" ./r3bdata/R3BDataLinkDef.h
    WRITECOUNTER=`expr ${WRITECOUNTER} + 1`
    rm -rf ./Safe/FileLine.txt
    Array[${n}]=''
  fi
  
  if [[ ${Array[${n}]} == *"NeutronCalibr"* ]]
  then 
    echo "${Array[${n}]}" > ./Safe/FileLine.txt
    sed -i -e "s:NeutronCalibr:NeutronCalib_seg:" ./Safe/FileLine.txt
    read TheLine < ./Safe/FileLine.txt
    sed -i "${WRITECOUNTER} a ${TheLine}" ./r3bdata/R3BDataLinkDef.h
    WRITECOUNTER=`expr ${WRITECOUNTER} + 1`
    rm -rf ./Safe/FileLine.txt
    Array[${n}]=''
  fi
  
  if [[ ${Array[${n}]} == *"ConstantFraction"* ]]
  then 
    echo "${Array[${n}]}" > ./Safe/FileLine.txt
    sed -i -e "s:ConstantFraction:ConstantFracton_seg:" ./Safe/FileLine.txt
    read TheLine < ./Safe/FileLine.txt
    sed -i "${WRITECOUNTER} a ${TheLine}" ./r3bdata/R3BDataLinkDef.h
    WRITECOUNTER=`expr ${WRITECOUNTER} + 1`
    rm -rf ./Safe/FileLine.txt
    Array[${n}]=''
  fi
  
  if [[ ${Array[${n}]} == *"parameters"* ]]
  then 
    echo "${Array[${n}]}" > ./Safe/FileLine.txt
    sed -i -e "s:parameters:parameter_seg:" ./Safe/FileLine.txt
    read TheLine < ./Safe/FileLine.txt
    sed -i "${WRITECOUNTER} a ${TheLine}" ./r3bdata/R3BDataLinkDef.h
    WRITECOUNTER=`expr ${WRITECOUNTER} + 1`
    rm -rf ./Safe/FileLine.txt
    Array[${n}]=''
  fi
  
  if [[ ${Array[${n}]} == *"PrimPart"* ]]
  then 
    echo "${Array[${n}]}" > ./Safe/FileLine.txt
    sed -i -e "s:PrimPart:PrimPat_seg:" ./Safe/FileLine.txt
    read TheLine < ./Safe/FileLine.txt
    sed -i "${WRITECOUNTER} a ${TheLine}" ./r3bdata/R3BDataLinkDef.h
    WRITECOUNTER=`expr ${WRITECOUNTER} + 1`
    rm -rf ./Safe/FileLine.txt
    Array[${n}]=''
  fi
  
  if [[ ${Array[${n}]} == *"NeutronTrack"* ]]
  then 
    echo "${Array[${n}]}" > ./Safe/FileLine.txt
    sed -i -e "s:NeutronTrack:NeutronTrak_seg:" ./Safe/FileLine.txt
    read TheLine < ./Safe/FileLine.txt
    sed -i "${WRITECOUNTER} a ${TheLine}" ./r3bdata/R3BDataLinkDef.h
    WRITECOUNTER=`expr ${WRITECOUNTER} + 1`
    rm -rf ./Safe/FileLine.txt
    Array[${n}]=''
  fi
  
  if [[ ${Array[${n}]} == *"PaddleCrystalHit"* ]]
  then 
    echo "${Array[${n}]}" > ./Safe/FileLine.txt
    sed -i -e "s:PaddleCrystalHit:PaddleCrystalHt_seg:" ./Safe/FileLine.txt
    read TheLine < ./Safe/FileLine.txt
    sed -i "${WRITECOUNTER} a ${TheLine}" ./r3bdata/R3BDataLinkDef.h
    WRITECOUNTER=`expr ${WRITECOUNTER} + 1`
    rm -rf ./Safe/FileLine.txt
    Array[${n}]=''
  fi
  
done

############################################################################
# Rewrite CMakeLists in r3bdata:

# Copy it to safety:
echo '-------------- Rewrite r3bdata CMakeLists.txt...'
cp ./r3bdata/CMakeLists.txt ./Safe/CMakeLists_r3bdata.txt

# Load the content into an array:
NLines2=`wc -l < ./r3bdata/CMakeLists.txt`
declare -a Array2[${NLines2}]
COUNTER2=0
while read Line2
do
  Array2[${COUNTER2}]=${Line2}
  COUNTER2=`expr ${COUNTER2} + 1`
done < ./r3bdata/CMakeLists.txt

# Now loop over the array and test if a line contains a specific string:
WRITECOUNTER2=75

for ((n=0; n<${NLines2}; ++n))
do
  # Now this is much easier since all strings we are interested start with "landdata/":

  if [[ ${Array2[${n}]} == *"landData/"* ]]
  then
    # If this string is there, write the whole line to a file:
    echo "${Array2[${n}]}" > ./Safe/FileLine.txt
    
    # Then first adapt the landdata string:
    sed -i -e "s:neulandData/:Nveto_segData/:" ./Safe/FileLine.txt
    sed -i -e "s:landData/:veto_segData/:" ./Safe/FileLine.txt
    
    # Then adapt the filenames:
    
    # Neulands:
    sed -i -e "s:NEULAND:NVETO_SEG:" ./Safe/FileLine.txt
    sed -i -e "s:NeuLAND:Nveto_seg:" ./Safe/FileLine.txt
    sed -i -e "s:NeuLand:Nveto_seg:" ./Safe/FileLine.txt
    sed -i -e "s:Neuland:Nveto_seg:" ./Safe/FileLine.txt
    sed -i -e "s:neuland:Nveto_seg:" ./Safe/FileLine.txt

    # Lands:
    sed -i -e "s:LAND:VETO_SEG:" ./Safe/FileLine.txt
    sed -i -e "s:Land:veto_seg:" ./Safe/FileLine.txt
    sed -i -e "s:land:veto_seg:" ./Safe/FileLine.txt

    # Rest of the stuff:
    sed -i -e "s:NeutHit:NeutHt_seg:" ./Safe/FileLine.txt
    sed -i -e "s:NeutronTracker2D:NeutronTrackr2D_seg:" ./Safe/FileLine.txt
    sed -i -e "s:NeutronTracker:NeutronTrackr_seg:" ./Safe/FileLine.txt
    sed -i -e "s:NeutronCalibr2D:NeutronCalib2D_seg:" ./Safe/FileLine.txt
    sed -i -e "s:ConstantFraction:ConstantFracton_seg:" ./Safe/FileLine.txt
    sed -i -e "s:parameters:parameter_seg:" ./Safe/FileLine.txt
    sed -i -e "s:PrimPart:PrimPat_seg:" ./Safe/FileLine.txt
    sed -i -e "s:NeutronTrack:NeutronTrak_seg:" ./Safe/FileLine.txt
    sed -i -e "s:PaddleCrystalHit:PaddleCrystalHt_seg:" ./Safe/FileLine.txt
    
    # Then read this line again:
    read TheLine < ./Safe/FileLine.txt
    
    # Then Add this line to the CMakeLists:
    sed -i "${WRITECOUNTER2} a ${TheLine}" ./r3bdata/CMakeLists.txt
    
    # Update the variables:
    WRITECOUNTER2=`expr ${WRITECOUNTER2} + 1`
    
    # Delete the FileLine:
    rm -rf ./Safe/FileLine.txt
    Array2[${n}]=''
  fi
  
done

# Add the new subdirectories:
sed -i '27 a ${R3BROOT_SOURCE_DIR}/r3bdata/veto_segData' ./r3bdata/CMakeLists.txt
sed -i '28 a ${R3BROOT_SOURCE_DIR}/r3bdata/Nveto_segData' ./r3bdata/CMakeLists.txt

####################################################################################
# Rewrite CMakeLists in r3bdb:

echo '-------------- Rewrite r3bdb CMakeLists.txt...'
cp ./r3bdb/CMakeLists.txt ./Safe/CMakeLists_r3bdb.txt
sed -i '5 a add_subdirectory (veto_segdb)' ./r3bdb/CMakeLists.txt

####################################################################################
# Rewrite the R3BDetectorList:

echo '-------------- Rewrite DetectorList...'
cp ./r3bdata/R3BDetectorList.h ./Safe/R3BDetectorList.h
sed -i -e "s:kLUMON:kLUMON,kVETO_SEG,kNVETO_SEG:" ./r3bdata/R3BDetectorList.h
sed -i -e "s:kLuMonPoint:kLuMonPoint,kveto_segPoint,kNveto_segPoint:" ./r3bdata/R3BDetectorList.h

####################################################################################
# Rewrite the R3BMCTrack.cxx
  
echo '-------------- Rewrite MCTrack...'
cp ./r3bdata/R3BMCTrack.cxx ./Safe/R3BMCTrack.cxx
sed -i '122 a << GetNPoints(kLUMON) << ", VETO_SEG " << GetNPoints(kVETO_SEG) << ", NVETO_SEG " << GetNPoints(kNVETO_SEG) << ", NeuLAND " << GetNPoints(kNEULAND) << endl;' ./r3bdata/R3BMCTrack.cxx
sed -i '124 d' ./r3bdata/R3BMCTrack.cxx

sed -i '183 a     else if (detId == kVETO_SEG)' ./r3bdata/R3BMCTrack.cxx
sed -i '184 a        return ((fNPoints & 0x300000000) >> 32);' ./r3bdata/R3BMCTrack.cxx
sed -i '185 a     else if (detId == kNVETO_SEG)' ./r3bdata/R3BMCTrack.cxx
sed -i '186 a        return ((fNPoints & 0xC00000000) >> 34);' ./r3bdata/R3BMCTrack.cxx

sed -i '267 a     else if (iDet == kVETO_SEG)' ./r3bdata/R3BMCTrack.cxx
sed -i '268 a     {' ./r3bdata/R3BMCTrack.cxx
sed -i '269 a         fNPoints = (fNPoints & (~0x300000000)) | (nPoints << 32);' ./r3bdata/R3BMCTrack.cxx
sed -i '270 a     }' ./r3bdata/R3BMCTrack.cxx
sed -i '271 a     else if (iDet == kNVETO_SEG)' ./r3bdata/R3BMCTrack.cxx
sed -i '272 a     {' ./r3bdata/R3BMCTrack.cxx
sed -i '273 a         fNPoints = (fNPoints & (~0xC00000000)) | (nPoints << 34);' ./r3bdata/R3BMCTrack.cxx
sed -i '274 a     }' ./r3bdata/R3BMCTrack.cxx

####################################################################################
# Thatś it!
echo '-------------- Done!'
  
  