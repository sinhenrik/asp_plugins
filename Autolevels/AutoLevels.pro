TEMPLATE = lib
#QT -= gui core
#LIBS -= -lQtGui -lQtCore

QT += widgets

TARGET = AutoLevels

DEPENDPATH += .

INCLUDEPATH += .

INCLUDEPATH += ../lib
INCLUDEPATH += ../../Plugin_SDK_2014/Plugin

include(../lib/includes.pri)
include(../lib/fastmathinclude.pri)

VERSION = 0.3

# Input
HEADERS += AutoLevelsFilter.h AutoLevelsData.h AutoLevelsPlugin.h srgblut.h
SOURCES += AutoLevelsFilter.cpp AutoLevelsData.cpp AutoLevelsPlugin.cpp 

TRANSLATIONS = ./locale/AutoLevels_en.ts ./locale/AutoLevels_fr.ts



