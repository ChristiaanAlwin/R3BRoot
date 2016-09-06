#!/bin/bash

# This script replaces the UI with the UI inside the Restore
# folder. This way the user gets a fresh UI if he/she wants.
cd ../
rm -rf ./UI
cd Restore
cp -R ./UI ../UI
