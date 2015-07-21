#!/bin/bash

# Setup Qt enviroment
export PATH=Users/ASP2/Qt5.4/5.4/clang_64/bin:$PATH

#make clean
make clean

#build 64bit spec
qmake -spec /Users/ASP2/Qt/5.4/clang_64/mkspecs/macx-g++ $@

#do the build
make -j8
