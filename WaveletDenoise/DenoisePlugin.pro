######################################################################
# Automatically generated by qmake (2.01a) Di. Jun 8 19:18:33 2010
######################################################################
TARGET = WaveletDenoise
VERSION = 0.1

include(../lib/includes.pri)
include(../lib/fastmathinclude.pri)

# Input
HEADERS += DenoiseFilter.h DenoisePlugin.h wavelet.h
FORMS += sph_wavelet_denoise.ui
SOURCES += DenoiseFilter.cpp DenoisePlugin.cpp wavelet.cpp dePepper.cpp ../lib/sphLayerFilter.cpp ../lib/convolution.cpp 

TRANSLATIONS = DenoiseFilter_de.ts DenoiseFilter_fr.ts DenoiseFilter_it.ts DenoiseFilter_ja.ts DenoiseFilter_nl.ts

