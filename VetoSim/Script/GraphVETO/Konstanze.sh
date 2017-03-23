#!/bin/bash

# This script generates pictures of produced secondary particles. Since this code
# is rougly written in the same way as the analysis code, this script, that
# controls this code, is also written according to the principle of the
# Full_Analysis.sh script.

############################################################################
# first check for the input:
if [[ -z "$4" ]]
then
echo '### NOTE: The commando kons requires more inputs. Call the script like'
echo '### NOTE: ./foto kons 1 PDG t1 t2 to show particles of type PDG between'
echo '### NOTE: times t1 and t2.'
echo ''
else

# re-generate the Inputs.root file:
cd ../../UI/
root -l -q SetParameters.cpp

# Write the counter:
cd ../Source/Graphics/
sed -i "27 a Counter = $1;" ./Konstanze.cpp
sed -i '29 d' ./Konstanze.cpp

# Then write the series:
cd ../../Data/Inputs/
if [ -f "./AnalysisTag.txt" ]
then
read Series < ./AnalysisTag.txt
cd ../../Source/Graphics/
sed -i "30 a Series = ${Series};" ./Konstanze.cpp
sed -i '32 d' ./Konstanze.cpp

# Write the outher stuff.
sed -i "33 a Time_low = $3;" ./Konstanze.cpp
sed -i '35 d' ./Konstanze.cpp

sed -i "36 a Time_up = $4;" ./Konstanze.cpp
sed -i '38 d' ./Konstanze.cpp

sed -i "39 a PDG = $2;" ./Konstanze.cpp
sed -i '41 d' ./Konstanze.cpp

# Now that we did this, we can compile and run:
find ./ -type f ! -name "*.*" -exec rm -rf {} \;
rm -rf ./*.o
cp ../../Compile/Graphics/Konstanze ./Makefile
make clean
make -j 4
./Konstanze

# Done!

else

# In case the analysis was not performed:
echo 'You did not perform an analysis! Hence we cannot run this code!'
echo ''
fi

# finish all other if-statements:
fi
