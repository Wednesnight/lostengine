#!/bin/sh

exists=$(ls -l "${CONFIG}" | grep -xi 'd.*lost' | awk '{print $NF}')
if [[ "$exists" ]]
then
  rm -fR "${CONFIG}/lost"
fi
exists=$(ls -l "${CONFIG}" | grep -xi 'd.*Launcher.tasklet' | awk '{print $NF}')
if [[ "$exists" ]]
then
  rm -fR "${CONFIG}/Launcher.tasklet"
fi
cp -Rf resources/* "${CONFIG}/"

