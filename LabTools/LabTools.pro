TEMPLATE = lib
#QT -= gui core
#LIBS -= -lQtGui -lQtCore
TARGET = LabTools
DEPENDPATH += .
INCLUDEPATH += .
INCLUDEPATH += ../../Plugin

RESOURCES += resources.qrc

VERSION = 0.6

# Input
HEADERS += LabToolsFilter.h LabToolsPlugin.h fastmath.h
SOURCES += LabToolsFilter.cpp LabToolsPlugin.cpp 

CONFIG(debug,debug|release) {
	message( debug )

	UI_DIR		=	build/objects/debug/ui
	MOC_DIR		=	build/objects/debug/moc
	OBJECTS_DIR	=	build/objects/debug/obj
	RCC_DIR	    =	build/objects/debug/rcc
	UI_HEADERS_DIR = build/objects/debug/uih
	UI_SOURCES_DIR = build/objects/debug/uisrc
}

CONFIG(release,debug|release) {
	message( release )

	UI_DIR		=	build/objects/release/ui
	MOC_DIR		=	build/objects/release/moc
	OBJECTS_DIR	=	build/objects/release/obj
	RCC_DIR	    =	build/objects/release/rcc
	UI_HEADERS_DIR = build/objects/release/uih
	UI_SOURCES_DIR = build/objects/release/uisrc
}

unix {
!mac {
   QMAKE_CFLAGS   += -m32 -mfpmath=sse -msse2
   QMAKE_CXXFLAGS += -fno-strict-aliasing -m32 -mfpmath=sse -msse2 
   QMAKE_LFLAGS   += -m32 -mfpmath=sse -msse2
   QMAKE_LIBS     += -L/usr/lib32
}
}

include( ../common/mac.pri )



