#!/bin/sh
# TODO add gyp calls for other formats.
gyp -f scons --depth=. --generator-output=scons LostEnginge.gyp
