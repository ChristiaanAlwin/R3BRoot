#!/bin/bash

# only rerun the neutron tracker (after calibration)
# be sure to set the counter yourself!

cd ../Source/Simulation/
root -l -q NeuLAND_Tracker.cpp

# Done!