TEMPLATE = lib
#QT -= gui core
#LIBS -= -lQtGui -lQtCore
TARGET = LabTools
DEPENDPATH += .

QT += widgets 

INCLUDEPATH += ../lib
INCLUDEPATH += ../../Plugin_SDK_2014/Plugin

include(../lib/includes.pri)
include(../lib/fastmathinclude.pri)

RESOURCES += resources.qrc

VERSION = 0.6

# Input
HEADERS += LabToolsFilter.h LabToolsPlugin.h fastmath.h
SOURCES += LabToolsFilter.cpp LabToolsPlugin.cpp 

