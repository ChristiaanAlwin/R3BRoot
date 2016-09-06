#!/bin/bash
cd ../Script/RunVETO/

if [[ "$1" == "digi" ]]
then
./ReCalibration.sh
elif [[ "$1" == "cal" ]]
then
./ReCalculation.sh
else
./Calibration.sh
fi