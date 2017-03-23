#!/bin/bash

# This program renames all files in a directory
# descriped by the variable RE_PATH. It performs the
# renaming according to land==>veto_seg

###########################################################################
# At first we will create a .txt file containing all the filenames in this file.

# First cd to this directory.
cd ${RE_PATH}

# Then write all names to a file:
find ./ -type f > ./OldNames.txt

# Then use a sed-command on the file OldNames.txt to include the full path:
sed -i -e "s:./:${RE_PATH}/:" ./OldNames.txt

# Then create a copy to change into the new names:
cp ./OldNames.txt ./NewNames.txt

###########################################################################
# Then we will use sed to rename all files. Since we are already not in
# a directory names land anymore, we also do not have to take this into 
# account. Hence we simply start renaming:

# Neulands:
sed -i -e "s:NEULAND:NVETO_SEG:" ./NewNames.txt
sed -i -e "s:NeuLAND:Nveto_seg:" ./NewNames.txt
sed -i -e "s:NeuLand:Nveto_seg:" ./NewNames.txt
sed -i -e "s:Neuland:Nveto_seg:" ./NewNames.txt
sed -i -e "s:neuland:Nveto_seg:" ./NewNames.txt

# Lands:
sed -i -e "s:LAND:VETO_SEG:" ./NewNames.txt
sed -i -e "s:Land:veto_seg:" ./NewNames.txt
sed -i -e "s:land:veto_seg:" ./NewNames.txt

# Rest of the stuff:
sed -i -e "s:NeutHit:NeutHt_seg:" ./NewNames.txt
sed -i -e "s:NeutronTracker2D:NeutronTrackr2D_seg:" ./NewNames.txt
sed -i -e "s:NeutronTracker:NeutronTrackr_seg:" ./NewNames.txt
sed -i -e "s:NeutronCalibr2D:NeutronCalib2D_seg:" ./NewNames.txt
sed -i -e "s:ConstantFraction:ConstantFracton_seg:" ./NewNames.txt
sed -i -e "s:parameters:parameter_seg:" ./NewNames.txt
sed -i -e "s:PrimPart:PrimPat_seg:" ./NewNames.txt
sed -i -e "s:NeutronTrack:NeutronTrak_seg:" ./NewNames.txt
sed -i -e "s:PaddleCrystalHit:PaddleCrystalHt_seg:" ./NewNames.txt
sed -i -e "s:Digitizing:Digitizin_seg:" ./NewNames.txt
sed -i -e "s:Monitoring:Monitorin_seg:" ./NewNames.txt
sed -i -e "s:compare.c:compar_seg.c:" ./NewNames.txt
sed -i -e "s:Validated:Validatd_seg:" ./NewNames.txt
sed -i -e "s:PaddleTamex:Paddle_segTamex:" ./NewNames.txt
sed -i -e "s:ClusteringEngine:NvetoClustringEngine:" ./NewNames.txt
sed -i -e "s:Neutron2Dcalibr:NvetoNeutron2Dcalbr:" ./NewNames.txt

# Rename the OldNames.txt to prevent an error message:
sed -i -e "s:OldNames.txt:OldNames_re.txt:" ./NewNames.txt
sed -i -e "s:CMakeLists.txt:CMakeLists_re.txt:" ./NewNames.txt
sed -i -e "s:readme.md:readme_re.md:" ./NewNames.txt

#########################################################################
# So now we have 2 .txt files that contain the old and new names.
# Now we can use mv to rename the files:

# We begin by getting the amount of filenames:
NFileNames=`wc -l < ./OldNames.txt`

# Then we will declare arrays to store the
# filesnames in:
declare -a Old[${NFileNames}]
declare -a New[${NFileNames}]

# Now store all old names:
COUNTER=0
while read OldLine
do
  Old[${COUNTER}]=${OldLine}
  COUNTER=`expr ${COUNTER} + 1`
done < ./OldNames.txt

# Now store all new names:
COUNTER=0
while read NewLine
do
  New[${COUNTER}]=${NewLine}
  COUNTER=`expr ${COUNTER} + 1`
done < ./NewNames.txt

# Now that we have all the arrays, we can do the actual renaming:
for (( n=0; n<${NFileNames}; ++n ))
do
  mv ${Old[${n}]} ${New[${n}]} 2>/dev/null
done

##########################################################################
# And now the renaming part is done. All that is left is to remove the 
# .txt files that we used for the actual renaming:
rm -rf ./OldNames_re.txt
rm -rf ./NewNames.txt

# Done!

