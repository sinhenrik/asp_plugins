######################################################################
# Automatically generated by qmake (2.01a) Di. Jun 8 19:18:33 2010
######################################################################
TARGET = Angy
VERSION = 0.1

include(../lib/includes.pri)
include(../lib/fastmathinclude.pri)



# Input
HEADERS += FilterAngy.h AngyPlugin.h ../lib/colorspace.h ../lib/blend_modes.h ../lib/sqrtlut.h
TRANSLATIONS = AngyPlugin_de.ts AngyPlugin_fr.ts AngyPlugin_it.ts AngyPlugin_ja.ts AngyPlugin_nl.ts
RESOURCES += res.qrc

FORMS += AngyPlugin.ui
SOURCES += AngyPlugin.cpp    
SOURCES += FilterAngy.cpp 

