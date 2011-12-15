######################################################################
# Automatically generated by qmake (2.01a) Di. Jun 8 19:18:33 2010
######################################################################

include(../lib/includes.pri)
include(../lib/fastmathinclude.pri)
VERSION = 0.1

# Input
HEADERS += FilterSharpen.h SharpenPlugin.h wavelet.h FilterUSM.h FilterLLorens.h 
TRANSLATIONS = SharpenPlugin_de.ts SharpenPlugin_fr.ts

FORMS += sph_wavelet_sharpen.ui
SOURCES += SharpenPlugin.cpp    
SOURCES += ../lib/sphLayerFilter.cpp 
SOURCES += FilterSharpen.cpp wavelet.cpp dePepper.cpp
SOURCES += FilterLLorens.cpp LlorensSharpen.cpp 
SOURCES += FilterUSM.cpp ../lib/usm_iir.cpp
SOURCES += ../lib/convolution.cpp

