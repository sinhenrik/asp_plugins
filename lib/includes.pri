TEMPLATE = lib
DEPENDPATH += .

# This is used in the source - use the TargetVersion.h file
DEFINES += TARGET_VERSION=$$VERSION
HEADERS += ../lib/TargetVersion.h

#QT += network

# we are including local header files
INCLUDEPATH += .

# we are including the ASP SDK headers from various locations where they may be
DEPENDPATH += ../../Plugin_SDK_2014/Plugin ../lib ../lib/PluginTools
INCLUDEPATH += ../../Plugin_SDK_2014/Plugin ../lib ../lib/PluginTools

# include mac build defines only on mac. These are from the SDK in the /common directory and are required for mac compiles - Please include these in your project for mac builds.
mac {
    include( mac.pri )
    include( Release.xcconfig )
    #TODO: add optimzed MAC compiler flags!
}


# unix {
# !mac {
# 	unix:QMAKE_CFLAGS   +=  -fPIC -Wall -O2  -mfpmath=sse -mtune=amdfam10 -msse2 -pipe
# 	unix:QMAKE_CXXFLAGS +=  -fPIC -Wall -O2  -mfpmath=sse -mtune=amdfam10 -msse2 -pipe
#         unix:QMAKE_LFLAGS   +=  -fPIC -Wall -O2  -mfpmath=sse -mtune=amdfam10 -msse2 -pipe
# 	CONFIG += x86
# 	unix:LIBS += -L/usr/lib32
# }
# }
# 

# # SSE2, faster over size, optimization
windows {
     QMAKE_CXXFLAGS += /arch:SSE2 /Oi /Ot
}




CONFIG(debug,debug|release) {
	message( debug )
	UI_DIR		=	builddir/objects/debug/ui
	MOC_DIR		=	builddir/objects/debug/moc
	OBJECTS_DIR	=	builddir/objects/debug/obj
	RCC_DIR	    =	builddir/objects/debug/rcc
	UI_HEADERS_DIR = builddir/objects/debug/uih
	UI_SOURCES_DIR = builddir/objects/debug/uisrc
    linux-g++-64 {
    !linux-g++-32 {
        message(Linux 64-bit debug build)
        DESTDIR=../Builds/Linux64/debug
    }
    }

    linux-g++-32 {
    !linux-g++-64 {
        message(Linux 32-bit debug build)
        DESTDIR=../Builds/Linux32/debug
    }
    }
}

CONFIG(release,debug|release) {
	message( release )
	UI_DIR		=	builddir/objects/release/ui
	MOC_DIR		=	builddir/objects/release/moc
	OBJECTS_DIR	=	builddir/objects/release/obj
	RCC_DIR	    =	builddir/objects/release/rcc
	UI_HEADERS_DIR = builddir/objects/release/uih
	UI_SOURCES_DIR = builddir/objects/release/uisrc
    linux-g++-64 {
    !linux-g++-32 {
        message(Linux 64-bit release build)
        DESTDIR=../Builds/Linux64/release
    }
    }

    linux-g++-32 {
    !linux-g++-64 {
        message(Linux 32-bit release build)
        DESTDIR=../Builds/Linux32/release
    }
    }
}
