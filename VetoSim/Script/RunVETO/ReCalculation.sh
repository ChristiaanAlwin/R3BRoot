#!/bin/bash

# This script runs the calibration macro to obtain the input for the R3BneutronTracker.
# It uses files that are already there.

# Then first we obtain the number of neutrons and protons we have to loop over.
cd ../../Source/Calibration/
root -l -q GetNeutronNumber.cpp
cd ../../Data/Inputs/

read MAX_NEUTRONS < ./Nneutrons.txt
read MAX_PROTONS < ./Nprotons.txt

echo "Number of neutrons = ${MAX_NEUTRONS}"
echo "Number of protons  = ${MAX_PROTONS}"

cd ../../Script/RunVETO/

# Now we should loop over the total number of protons.
MAX_PROTONS=`expr ${MAX_PROTONS} + 1` # (for the loop)
for (( PROTONS=0; PROTONS<${MAX_PROTONS}; ++PROTONS ))
do

# Compute where we should start in computing neutrons:
NEUTRON_START=1
if [[ ${PROTONS}>0 ]]
then
NEUTRON_START=0
fi

echo "Neutron Start = ${NEUTRON_START}"

# Write these numbers to the CalibrateCuts-file. Notice that
# the number of histograms is p_current+ n_max:
MAX_PARTICLES=`expr ${MAX_NEUTRONS} + ${PROTONS}` # (we do 0-n neutrons and 1-n protons, unless n_p==0. Then we do 1-n neutrons. Hence always #p + max#n histograms!
cd ../../Source/Calibration/
sed -i "23 a static const Int_t N_MAX = ${MAX_PARTICLES};" ./CalibrateCuts.cpp
sed -i '25 d' ./CalibrateCuts.cpp
sed -i "24 a static const Int_t N_PROT = ${PROTONS};" ./CalibrateCuts.cpp
sed -i '26 d' ./CalibrateCuts.cpp
sed -i "25 a static const Int_t N_TOT = ${MAX_NEUTRONS} + ${MAX_PROTONS};" ./CalibrateCuts.cpp
sed -i '27 d' ./CalibrateCuts.cpp

# Then re-compile and run:
make clean
make -j 4
./CalibrateCuts

cd ../../Script/RunVETO/

# Done!
done
