#!/bin/bash
sed -i "20 a Nr_Run = $1;" ../Source/Simulation/eventDisplay.cpp
sed -i '22 d' ../Source/Simulation/eventDisplay.cpp
cd ../Source/Simulation/
root -l eventDisplay.cpp