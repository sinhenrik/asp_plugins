######################################################################
# Automatically generated by qmake (2.01a) Di. Jun 8 19:18:33 2010
######################################################################

include(../lib/includes.pri)
include(../lib/fastmathinclude.pri)
VERSION = 0.1

# Input
HEADERS += FilterGelatin.h GelatinPlugin.h ../lib/colorspace.h ../lib/blend_modes.h
TRANSLATIONS = GelatinPlugin_de.ts GelatinPlugin_fr.ts GelatinPlugin_it.ts GelatinPlugin_ja.ts GelatinPlugin_nl.ts

FORMS += GelatinPlugin.ui
SOURCES += GelatinPlugin.cpp    
SOURCES += FilterGelatin.cpp 

unix {
!mac {
	unix:QMAKE_CFLAGS +=  -ffinite-math-only -fno-trapping-math -fno-signaling-nans -fno-rounding-math -funsafe-math-optimizations -funsafe-loop-optimizations -fsingle-precision-constant 
	unix:QMAKE_CXXFLAGS += -ffinite-math-only -fno-trapping-math -fno-signaling-nans -fno-rounding-math -funsafe-math-optimizations -funsafe-loop-optimizations -fsingle-precision-constant
	unix:QMAKE_LFLAGS += -ffinite-math-only -fno-trapping-math -fno-signaling-nans -fno-rounding-math -funsafe-math-optimizations -funsafe-loop-optimizations -fsingle-precision-constant 
}
}

