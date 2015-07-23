TEMPLATE = lib
#QT -= gui core
#LIBS -= -lQtGui -lQtCore
TARGET = LabCurves
DEPENDPATH += .

QT += widgets 

INCLUDEPATH += ../lib
INCLUDEPATH += ../../Plugin_SDK_2014/Plugin

include(../lib/includes.pri)
include(../lib/fastmathinclude.pri)

RESOURCES += resources.qrc

VERSION = 0.1

# Input
HEADERS += LabCurvesFilterBefore.h LabCurvesFilterAfter.h LabCurvesPlugin.h srgblut.h
SOURCES += LabCurvesFilterBefore.cpp LabCurvesFilterAfter.cpp LabCurvesPlugin.cpp 

TRANSLATIONS = ./locale/LabCurves_en.ts ./locale/LabCurves_fr.ts
