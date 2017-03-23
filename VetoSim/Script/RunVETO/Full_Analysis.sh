#!/bin/bash

# This script performs a full analysis of all MonteCarlo files that
# exist in the Data-folder AND that have the same NameTag as is
# defined in the Inputs.root-file AT THIS MOMENT.

# This allows the user to manage different simulation series with
# different NameTags and still analyse one series at a time.

# NOTE: If a Monte Carlo file misses from the series, the counters
# will no longer run parallel and the analysis will fail!
# therefore the user should not mess with the Monte Carlo files!
# for cleaning all files, there is a script called clean.sh
# in the User Interface directory.

# NOTE: This script has a single input parameter. If this parameter
# is equal to pos, the simulation files are not analysed as 
# independent files, but as a position series. This
# means that the VETO position is updated during the anaysis. Else, this
# is not done.

#########################################################################

# before we do anything, we have to re-generate the inputs. This is
# because the user might have run a series of simulations and some
# parameters may have been updated so they do no longer hold 
# the correct initial values:
cd ../../UI/
root -l -q SetParameters.cpp

# Now here we go. cd to the Monte Carlo folder and write the filenames
# in that folder to a .txt-file located in the Inputs folder:
cd ../Data/MonteCarlo/
find ./ -type f > ../Inputs/MC_Names.txt

# Now we need to obtain the NameTag from the Inputs.root-file:
cd ../../Source/Analysis/
root -l -q GetNameTag.cpp

# Now we need to obtain the number of files containing the
# nametag. cd to the Inputs-directory:
cd ../../Data/Inputs/

# Now read the NameTag:
read NameTag < ./NameTag.txt

# Then read this file and count:
COUNTER=0;         # Counter number of files
CHECHCOUNTER=0;    # counts whether there are files missing from the series
while read Line    # Read the file with filenames
do
if [[ ${Line} == *${NameTag}* ]]   # Check on the NameTag:
then
COUNTER=`expr ${COUNTER} + 1`      # Count
fi
done < ./MC_Names.txt

# So now we know exactly how many files containing the NameTag actually exist:
# So write this into the FullAnalysis.cpp-file:
sed -i "12 a Ntimes = ${COUNTER};" ../../Source/Analysis/FullAnalysis.cpp

# remove the previous line:
sed -i '14 d' ../../Source/Analysis/FullAnalysis.cpp

# remover the file MC_Names.txt:
rm -rf ./MC_Names.txt

#####################################################
# But if we use an analysis-series, we should write a different
# number to Ntimes: The number of times we have to re-analyze the same
# file (in that case we alayse the file with counter 1.

if [[ $1 == 'show' ]]
then
# Obtain this number:
cd ../../Source/Analysis/
root -l -q GetAnaNumber.cpp

cd ../../Data/Inputs/
read AnaNum < ./Nana.txt

sed -i "12 a Ntimes = ${AnaNum};" ../../Source/Analysis/FullAnalysis.cpp
sed -i '14 d' ../../Source/Analysis/FullAnalysis.cpp
rm -rf ./Nana.txt
fi

if [[ $1 == 'Eres' ]]
then
# Obtain this number:
cd ../../Source/Analysis/
root -l -q GetAnaNumber.cpp

cd ../../Data/Inputs/
read AnaNum < ./Nana.txt

sed -i "12 a Ntimes = ${AnaNum};" ../../Source/Analysis/FullAnalysis.cpp
sed -i '14 d' ../../Source/Analysis/FullAnalysis.cpp
rm -rf ./Nana.txt
fi

if [[ $1 == 'Xstart' ]]
then
# Obtain this number:
cd ../../Source/Analysis/
root -l -q GetAnaNumber.cpp

cd ../../Data/Inputs/
read AnaNum < ./Nana.txt

sed -i "12 a Ntimes = ${AnaNum};" ../../Source/Analysis/FullAnalysis.cpp
sed -i '14 d' ../../Source/Analysis/FullAnalysis.cpp
rm -rf ./Nana.txt
fi

if [[ $1 == 'Tcut' ]]
then
# Obtain this number:
cd ../../Source/Analysis/
root -l -q GetAnaNumber.cpp

cd ../../Data/Inputs/
read AnaNum < ./Nana.txt

sed -i "12 a Ntimes = ${AnaNum};" ../../Source/Analysis/FullAnalysis.cpp
sed -i '14 d' ../../Source/Analysis/FullAnalysis.cpp
rm -rf ./Nana.txt
fi

if [[ $1 == 'veto' ]]
then
# Obtain this number:
cd ../../Source/Analysis/
root -l -q GetAnaNumber.cpp

cd ../../Data/Inputs/
read AnaNum < ./Nana.txt

sed -i "12 a Ntimes = ${AnaNum};" ../../Source/Analysis/FullAnalysis.cpp
sed -i '14 d' ../../Source/Analysis/FullAnalysis.cpp
rm -rf ./Nana.txt
fi

#####################################################
# Now we also have to tell the script whether we have a specific series:
# we could add more else if statements for other series. 1 is a 
# position series. 2 is a repeated analysis series for shower Radius.
# We will also write this 0 or 1 or ... to a
# file in the Inputs-directory:

if [[ $1 == 'pos' ]]
then
sed -i '15 a Series = 1;' ../../Source/Analysis/FullAnalysis.cpp
sed -i '17 d' ../../Source/Analysis/FullAnalysis.cpp
echo '1' > ../../Data/Inputs/AnalysisTag.txt
elif [[ $1 == 'show' ]]
then
sed -i '15 a Series = 2;' ../../Source/Analysis/FullAnalysis.cpp
sed -i '17 d' ../../Source/Analysis/FullAnalysis.cpp
echo '2' > ../../Data/Inputs/AnalysisTag.txt
elif [[ $1 == 'paddle' ]]
then
sed -i '15 a Series = 3;' ../../Source/Analysis/FullAnalysis.cpp
sed -i '17 d' ../../Source/Analysis/FullAnalysis.cpp
echo '3' > ../../Data/Inputs/AnalysisTag.txt
elif [[ $1 == 'thick' ]]
then
sed -i '15 a Series = 4;' ../../Source/Analysis/FullAnalysis.cpp
sed -i '17 d' ../../Source/Analysis/FullAnalysis.cpp
echo '4' > ../../Data/Inputs/AnalysisTag.txt
elif [[ $1 == 'Eres' ]]
then
sed -i '15 a Series = 5;' ../../Source/Analysis/FullAnalysis.cpp
sed -i '17 d' ../../Source/Analysis/FullAnalysis.cpp
echo '5' > ../../Data/Inputs/AnalysisTag.txt
elif [[ $1 == 'Xstart' ]]
then
sed -i '15 a Series = 6;' ../../Source/Analysis/FullAnalysis.cpp
sed -i '17 d' ../../Source/Analysis/FullAnalysis.cpp
echo '6' > ../../Data/Inputs/AnalysisTag.txt
elif [[ $1 == 'Tcut' ]]
then
sed -i '15 a Series = 7;' ../../Source/Analysis/FullAnalysis.cpp
sed -i '17 d' ../../Source/Analysis/FullAnalysis.cpp
echo '7' > ../../Data/Inputs/AnalysisTag.txt
elif [[ $1 == 'veto' ]]
then
sed -i '15 a Series = 8;' ../../Source/Analysis/FullAnalysis.cpp
sed -i '17 d' ../../Source/Analysis/FullAnalysis.cpp
echo '8' > ../../Data/Inputs/AnalysisTag.txt
else
sed -i '15 a Series = 0;' ../../Source/Analysis/FullAnalysis.cpp
sed -i '17 d' ../../Source/Analysis/FullAnalysis.cpp
echo '0' > ../../Data/Inputs/AnalysisTag.txt
fi

# now we are ready to go. Firts compile:
echo ''
echo '### NOTE: Re-compile analysis code:'
cd ../../Source/Analysis/
make clean
make -j 4

# Then run:
echo ''
echo '### NOTE: Run the analysis!'
echo ''
./FullAnalysis
echo ''

# Then in the end, remove the .txt files that we used:
cd ../../Data/Inputs/
rm -rf ./MC_Names.txt
rm -rf ./NameTag.txt

