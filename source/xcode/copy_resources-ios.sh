#!/bin/sh

exists=$(ls -l "${TARGET_BUILD_DIR}/${PRODUCT_NAME}.app" | grep -xi 'd.*lost' | awk '{print $NF}')
if [[ "$exists" ]]
then
  rm -fR "${TARGET_BUILD_DIR}/${PRODUCT_NAME}.app/lost"
  rm -fR "${TARGET_BUILD_DIR}/${PRODUCT_NAME}.app/Launcher.tasklet"
  rm -fR "${TARGET_BUILD_DIR}/${PRODUCT_NAME}.app/lost.zip"
  rm -fR "${TARGET_BUILD_DIR}/${PRODUCT_NAME}.app/Launcher.tasklet.zip"
fi
cd resources/lost
zip -r  "${TARGET_BUILD_DIR}/${PRODUCT_NAME}.app/lost.zip" *  -x \*/\*.svn/\*
cd ../..
#cp -Rf resources/Launcher.tasklet "${BUILD_DIR}/${CONFIGURATION}/${PRODUCT_NAME}.app/Contents/Resources/Launcher.tasklet"
cd resources/Launcher.tasklet
zip -r "${TARGET_BUILD_DIR}/${PRODUCT_NAME}.app/Launcher.tasklet.zip" *  -x \*/\*.svn/\*
