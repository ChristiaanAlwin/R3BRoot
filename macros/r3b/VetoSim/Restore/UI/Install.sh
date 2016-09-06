#!/bin/bash
if [[ $1 == 'prev' ]]
then
echo '1' > ../Source/Simulation/Version.txt
cd ../Script/AddVETO_July/
else
echo '2' > ../Source/Simulation/Version.txt
cd ../Script/AddVETO/
fi
./AddVETO.sh