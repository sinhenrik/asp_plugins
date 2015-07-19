#!/bin/bash
export PATH=/opt/Qt5.4.2_x64/5.4/gcc_64/bin:$PATH

#make clean
make clean

#build 64bit spec
qmake -spec /opt/Qt5.4.2_x64/5.4/gcc_64/mkspecs/linux-g++-64 $@

#do the build
make -j8
