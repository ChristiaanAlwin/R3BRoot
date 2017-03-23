#!/bin/bash

# This script replaces the AddVETO script in the Script-folder with the
# one in the Restore folder, so you get a fresh script.
cd ../Script/
rm -rf ./AddVETO
cd ../Restore/
cp -R ./AddVETO ../Script/AddVETO
