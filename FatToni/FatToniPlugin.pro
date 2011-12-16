######################################################################
# Automatically generated by qmake (2.01a) Di. Jun 8 19:18:33 2010
######################################################################

include(../lib/includes.pri)
include(../lib/fastmathinclude.pri)
VERSION = 0.1

# Input
HEADERS += FilterFatToni.h FatToniPlugin.h ../lib/colorspace.h ../lib/blend_modes.h
TRANSLATIONS = FatToniPlugin_de.ts FatToniPlugin_fr.ts FatToniPlugin_it.ts FatToniPlugin_ja.ts FatToniPlugin_nl.ts
RESOURCES += res.qrc

FORMS += FatToniPlugin.ui
SOURCES += FatToniPlugin.cpp    
SOURCES += FilterFatToni.cpp 

unix {
!mac {
	unix:QMAKE_CFLAGS +=  -ffinite-math-only -fno-trapping-math -fno-signaling-nans -fno-rounding-math -funsafe-math-optimizations -funsafe-loop-optimizations -fsingle-precision-constant 
	unix:QMAKE_CXXFLAGS += -ffinite-math-only -fno-trapping-math -fno-signaling-nans -fno-rounding-math -funsafe-math-optimizations -funsafe-loop-optimizations -fsingle-precision-constant
	unix:QMAKE_LFLAGS += -ffinite-math-only -fno-trapping-math -fno-signaling-nans -fno-rounding-math -funsafe-math-optimizations -funsafe-loop-optimizations -fsingle-precision-constant 
}
}

