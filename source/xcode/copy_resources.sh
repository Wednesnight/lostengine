#!/bin/sh

exists=$(ls -l "${TARGET_BUILD_DIR}/${PRODUCT_NAME}.app/Contents" | grep -xi 'd.*Resources' | awk '{print $NF}')
if [[ "$exists" ]]
then
  rm -fR "${TARGET_BUILD_DIR}/${PRODUCT_NAME}.app/Contents/Resources/lost"
  rm -fR "${TARGET_BUILD_DIR}/${PRODUCT_NAME}.app/Contents/Resources/Launcher.tasklet"
  rm -fR "${TARGET_BUILD_DIR}/${PRODUCT_NAME}.app/Contents/Resources/lost.zip"
  rm -fR "${TARGET_BUILD_DIR}/${PRODUCT_NAME}.app/Contents/Resources/Launcher.tasklet.zip"
else
  mkdir "${TARGET_BUILD_DIR}/${PRODUCT_NAME}.app/Contents/Resources"
fi
cd resources/lost
zip -r  "${TARGET_BUILD_DIR}/${PRODUCT_NAME}.app/Contents/Resources/lost.zip" *  -x \*/\*.svn/\*
cd ../..
#cp -Rf resources/Launcher.tasklet "${BUILD_DIR}/${CONFIGURATION}/${PRODUCT_NAME}.app/Contents/Resources/Launcher.tasklet"
cd resources/Launcher.tasklet
zip -r "${TARGET_BUILD_DIR}/${PRODUCT_NAME}.app/Contents/Resources/Launcher.tasklet.zip" *  -x \*/\*.svn/\*
