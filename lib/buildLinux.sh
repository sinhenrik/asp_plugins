#!/bin/bash

#!/bin/bash
export PATH=/opt/Qt5.4.2_x86/5.4/gcc/bin:$PATH

#make clean
make clean

#build 32bit spec
qmake -spec /opt/Qt5.4.2_x86/5.4/gcc/mkspecs/linux-g++-32 $@

#do the build
make -j8
