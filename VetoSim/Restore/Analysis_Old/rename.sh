#!/bin/bash

# Renames all files for the paddles.
NAMETAG='Paddle600'
COUNTER=1

for (( k=1; k<10; ++k ))
do
OLDNAME="./${NAMETAG}_run=${COUNTER}.root"
NEWNAME="./${NAMETAG}Nr${COUNTER}_run=1.root"
echo "${OLDNAME}   ${NEWNAME}"
mv ${OLDNAME} ${NEWNAME}
COUNTER=`expr ${COUNTER} + 1`
done