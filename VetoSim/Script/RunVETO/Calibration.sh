#!/bin/bash

# This script runs the calibration simulations to obtain the input for the R3BneutronTracker.
# The simulations will have to be re-done for different numbers of Neutrons. Hence we must
# loop over the number of neutrons. We will first however check if the Inputs.root-file exists.

# If it exists, we must make sure that it is safe during the changes.
cd ../../Data/Inputs/
if [ -f "Inputs.root" ]
then
# We are OK! Now continue:

# Make sure that the Inputs.root-file is safe:
cp ./Inputs.root ./Safe_Inputs.root

# Then before we do anything, we must first estimate the proton energy
# from our data:
cd ../../Source/Calibration/
make clean
make -j 4
./Proton_Energy_Estimation

# Then now we obtain the number of neutrons we have to loop over.
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

# Then now we can loop over the number of neutrons and do the calibration runs:
cd ../../Script/RunVETO/

FOR_BOUND=`expr ${MAX_NEUTRONS} + 1`
for (( NEUTRONS=${NEUTRON_START}; NEUTRONS<${FOR_BOUND}; ++NEUTRONS ))
do

echo '########################################################################################'
echo ''
echo "We will now perform a NeuLAND Calibration run for ${NEUTRONS} Neutrons and ${PROTONS} Protons..."
echo ''

#####################################################
# We begin with setting our parameters up correctly.

# Switch to the calibration directory:
cd ../../Source/Calibration/

# Write the proper number of neutrons in the source code for SetSim:
sed -i "33 a N_neutrons = ${NEUTRONS};" ./SetSim.cpp
sed -i '35 d' ./SetSim.cpp
sed -i "37 a N_protons = ${PROTONS};" ./SetSim.cpp
sed -i '39 d' ./SetSim.cpp

# Then re-compile and run:
make clean
make -j 4
./SetSim

# Then we must only continue if the beam is aimed on neuland:
cd ../../Data/Inputs/
read Line < ./Aimed.txt
rm -rf ./Aimed.txt

if [[ ${Line} == "1" ]]
then

#####################################################
# Now we can actually run the simulation.

# Switch to the simulation-directory:
cd ../../Source/Simulation/

# Generate the ascii-file:
make clean
make -j 4
./Create_ASCII_File

# Then actually run the simulation:
root -l -q R3B_Simulation.cpp

# clear the directory:
rm -rf ./calor.out
rm -rf ./flukaerr.dat
rm -rf ./gphysi.dat

# Then run the NeuLAND Digitizer:
root -l -q NeuLAND_Digitizer.cpp

# Then run the VETO-digitizer:
root -l -q VETO_Digitizer.cpp

# And that is a single run! Now return to the script-directory:
cd ../../Script/RunVETO/

else
echo '### ERROR: Your Primary beam is not aimed at NeuLAND! Fix this in SetParameters.cpp and then re-run!'
echo ''
fi

######################################################
done

# then we compute the neutron cuts:
cd ../../Source/Calibration/
make clean
make -j 4
./CalibrateCuts

# Return to the initial directory:
cd ../../Script/RunVETO/

# Done!
done

# Then finally, we can restore the Inputs:
cd ../../Data/Inputs/
cp ./Safe_Inputs.root ./Inputs.root
rm -rf ./Safe_Inputs.root
rm -rf ./Nneutrons.txt
rm -rf ./Nprotons.txt

fi
