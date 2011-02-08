#!/bin/sh

exists=$(ls -l "${BUILD_DIR}/${CONFIGURATION}/${PRODUCT_NAME}.app/Contents" | grep -xi 'd.*Resources' | awk '{print $NF}')
if [[ "$exists" ]]
then
  rm -fR "${BUILD_DIR}/${CONFIGURATION}/${PRODUCT_NAME}.app/Contents/Resources/lost"
  rm -fR "${BUILD_DIR}/${CONFIGURATION}/${PRODUCT_NAME}.app/Contents/Resources/Launcher.tasklet"
else
  mkdir "${BUILD_DIR}/${CONFIGURATION}/${PRODUCT_NAME}.app/Contents/Resources"
fi
cp -Rf resources/* "${BUILD_DIR}/${CONFIGURATION}/${PRODUCT_NAME}.app/Contents/Resources/"
